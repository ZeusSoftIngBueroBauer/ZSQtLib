/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

This file may be used with no license restrictions for your needs. But it is not
allowed to resell any modules of the ZSQtLib veiling the original developer of
the modules. Therefore the copyright link to ZeusSoft, Ing. Buero Bauer must not
be removed from the header of the source code modules.

ZeusSoft, Ing. Buero Bauer provides the source code as is without any guarantee
that the code is written without faults.

ZeusSoft, Ing. Buero Bauer does not assume any liability for any damages which
may result in using the software modules.

*******************************************************************************/

#include <QtCore/qthread.h>
#include <QtCore/qtimer.h>

#include "ZSIpc/ZSIpcServerWrapperShm.h"
#include "ZSIpc/ZSIpcSocketWrapperShm.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
// As "min" will be defined as a macro with two arguments and qdatetime uses "min"
// as a function with no arguments "windows.h" must be included after qdatetime
// (which is included by ZSSysTime (which again is included by ZSSysAux)).
#include <windows.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;

//#if QT_VERSION < 0x040501
//#define isAttached isValid
//#endif


/*******************************************************************************
class CShmServerWrapper : public CIpcServerWrapper
*******************************************************************************/

#define SHAREDMEMORY_LOCK \
    m_iSharedMemoryLockCount++; \
    m_pSharedMemory->lock()

#define SHAREDMEMORY_UNLOCK \
    m_iSharedMemoryLockCount--; \
    m_pSharedMemory->unlock()

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CShmServerWrapper::CShmServerWrapper(
    const QString& i_strObjName,
    CTrcMthFile*   i_pTrcMthFile,
    EMethodTraceDetailLevel i_eTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    CIpcServerWrapper(i_strObjName, ESocketTypeShm, i_pTrcMthFile, i_eTrcMthFileDetailLevel),
    m_pSharedMemory(nullptr),
    m_iSharedMemoryLockCount(0),
    m_pTimerListen(nullptr),
    m_iMaxPendingConnections(30),
    m_iPendingConnections(0),
    m_arpPendingConnections(nullptr)
{
    // The derived classes must instantiate the trace admin object and trace the ctor.
    if( m_pTrcMthFile == nullptr && !i_strObjName.contains("TrcServer") )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strAddInfo         */ "" );

    m_pServer = m_pSharedMemory = new QSharedMemory();

    m_pTimerListen = new QTimer(this);

    if( !QObject::connect(
        /* pObjSender   */ m_pTimerListen,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTimeoutListen()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CShmServerWrapper::~CShmServerWrapper()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strAddInfo         */ "" );

    if( m_pSharedMemory != nullptr )
    {
        m_pSharedMemory->detach();

        try
        {
            delete m_pSharedMemory;
        }
        catch(...)
        {
        }
        m_pSharedMemory = nullptr;
    }

    if( m_arpPendingConnections != nullptr )
    {
        int idxConnection;

        for( idxConnection = 0; idxConnection < m_iMaxPendingConnections; idxConnection++ )
        {
            if( m_arpPendingConnections[idxConnection] != nullptr )
            {
                m_arpPendingConnections[idxConnection]->abort();

                try
                {
                    delete m_arpPendingConnections[idxConnection];
                }
                catch(...)
                {
                }
                m_arpPendingConnections[idxConnection] = nullptr;
            }
        }
    }

    try
    {
        delete [] m_arpPendingConnections;
    }
    catch(...)
    {
    }
    m_arpPendingConnections = nullptr;

    m_pTimerListen = nullptr;

    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();

        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;
    }

} // dtor

/*==============================================================================
public: // must overridables of base class CIpcServerWrapper
==============================================================================*/

//------------------------------------------------------------------------------
void CShmServerWrapper::setMaxPendingConnections( int i_iMaxConnections )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setMaxPendingConnections",
        /* strAddInfo         */ "" );

    if( m_arpPendingConnections == nullptr )
    {
        m_iMaxPendingConnections = i_iMaxConnections;
    }

} // setMaxPendingConnections

//------------------------------------------------------------------------------
int CShmServerWrapper::maxPendingConnections() const
//------------------------------------------------------------------------------
{
    return m_iMaxPendingConnections;
}

//------------------------------------------------------------------------------
bool CShmServerWrapper::listen()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "listen",
        /* strAddInfo         */ "" );

    m_pSharedMemory->setKey( m_strLocalHostName + "/ShmServer:" + QString::number(m_uLocalPort) );

    #ifdef _WINDOWS
    #pragma message(__TODO__"Size of shared memory block should be set via startup message" )
    #endif
    if( !m_pSharedMemory->create(4096) )
    {
        if( !m_pSharedMemory->attach() )
        {
            return false;
        }
    }

    //#if QT_VERSION < 0x040501
    //if( !m_pSharedMemory->attach() )
    //{
    //    return false;
    //}
    //#endif

    SShmMainHdr* pShmMainHdr = reinterpret_cast<SShmMainHdr*>(m_pSharedMemory->data());

    pShmMainHdr->m_iMsgId = 0;
    pShmMainHdr->m_uMsgCount = 0;
    pShmMainHdr->m_uDataMsgCount = 0;
    pShmMainHdr->m_uBlockLen = 0;

    if( m_arpPendingConnections == nullptr )
    {
        m_arpPendingConnections = new CShmSocketWrapper*[m_iMaxPendingConnections];
        memset( m_arpPendingConnections, 0x00, m_iMaxPendingConnections*sizeof(CShmSocketWrapper*) );
    }

    m_pTimerListen->start(c_iTimerPeriodServerCtrlChanListen_ms);

    return true;

} // listen

//------------------------------------------------------------------------------
bool CShmServerWrapper::isListening() const
//------------------------------------------------------------------------------
{
    if( m_pTimerListen->isActive() )
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
void CShmServerWrapper::close()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "close",
        /* strAddInfo         */ "" );

    m_pTimerListen->stop();
    m_pSharedMemory->detach();

} // close

//------------------------------------------------------------------------------
bool CShmServerWrapper::hasPendingConnections() const
//------------------------------------------------------------------------------
{
    return (m_iPendingConnections > 0);
}

//------------------------------------------------------------------------------
CIpcSocketWrapper* CShmServerWrapper::nextPendingConnection()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "nextPendingConnection",
        /* strAddInfo         */ "" );

    SHAREDMEMORY_LOCK;

    CShmSocketWrapper* pSocketWrapper = nullptr;
    int                idxConnection;

    // Search pending connection ...
    for( idxConnection = 0; idxConnection < m_iMaxPendingConnections; idxConnection++ )
    {
        if( m_arpPendingConnections[idxConnection] != nullptr )
        {
            pSocketWrapper = m_arpPendingConnections[idxConnection];
            m_arpPendingConnections[idxConnection] = nullptr;
            break;
        }
    }
    SHAREDMEMORY_UNLOCK;

    return pSocketWrapper;

} // nextPendingConnection

//------------------------------------------------------------------------------
EResult CShmServerWrapper::error() const
//------------------------------------------------------------------------------
{
    return CShmSocketWrapper::SharedMemoryError2Result( m_pSharedMemory->error() );
}

//------------------------------------------------------------------------------
QString CShmServerWrapper::errorString() const
//------------------------------------------------------------------------------
{
    return m_pSharedMemory->errorString();
}

//------------------------------------------------------------------------------
SErrResultInfo CShmServerWrapper::errResultInfo() const
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo;

    EResult result = error();

    if( result != EResultSuccess )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(result);
        errResultInfo.setAddErrInfoDscr(errorString());
    }
    return errResultInfo;

} // errResultInfo

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CShmServerWrapper::onTimeoutListen()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTimeoutListen",
        /* strAddInfo         */ "" );

    SHAREDMEMORY_LOCK;

    SShmMainHdr* pShmMainHdr = reinterpret_cast<SShmMainHdr*>(m_pSharedMemory->data());
    bool         bNewConnectionPending = false;
    unsigned int uBlock = 0;

    while( uBlock < pShmMainHdr->m_uMsgCount && !bNewConnectionPending )
    {
        SShmMsgHdr* pShmMsgHdr = pShmMainHdr->getMsg(m_pSharedMemory,uBlock);
        bool        bValidMsg = false;

        uBlock++;

        // Only connection requests and connection confirmation messages are allowed
        // in the servers control channel. All other messages will be ignored and removed.
        if( pShmMsgHdr->m_srvCltTypeReceiver == ESrvCltTypeServer || pShmMsgHdr->m_srvCltTypeReceiver == ESrvCltTypeClient )
        {
            if( pShmMsgHdr->m_msgType == EMsgTypeReqConnect || pShmMsgHdr->m_msgType == EMsgTypeConConnect )
            {
                bValidMsg = true;
            }
        }
        if( !bValidMsg )
        {
            // Remove the message from the servers control channel:
            pShmMainHdr->removeMsg(m_pSharedMemory,pShmMsgHdr);
            uBlock--;
            pShmMsgHdr = nullptr;
        }
        else if( pShmMsgHdr->m_srvCltTypeReceiver == ESrvCltTypeServer && pShmMsgHdr->m_msgType == EMsgTypeReqConnect )
        {
            quint32 uPendingConnection;

            // Search free entry ...
            for( uPendingConnection = 0; uPendingConnection < static_cast<unsigned int>(m_iMaxPendingConnections); uPendingConnection++ )
            {
                if( m_arpPendingConnections[uPendingConnection] == nullptr )
                {
                    break;
                }
            }
            if( uPendingConnection < static_cast<unsigned int>(m_iMaxPendingConnections) )
            {
                if( pShmMsgHdr->m_uMsgLen > 0 )
                {
                    SShmMsgReqConnect* pShmMsgReqConnect = reinterpret_cast<SShmMsgReqConnect*>(pShmMsgHdr);
                    QByteArray         byteArrRemoteHost;
                    quint32            uPort;
                    QString            strKey;

                    byteArrRemoteHost.resize(pShmMsgReqConnect->m_uBlockLenHostNameClient);
                    memcpy( byteArrRemoteHost.data(), pShmMsgReqConnect->m_arcHostNameClient, pShmMsgReqConnect->m_uBlockLenHostNameClient );

                    // Create the shared memory block used as the communication channel between
                    // the ShmSocket at the server's and the ShmSocket at the client's side.
                    QSharedMemory* pSharedMemorySocketCommChan = new QSharedMemory();

                    // Search free port ...
                    for( uPort = 0; uPort < 1024; uPort++ )
                    {
                        strKey = m_strLocalHostName + "/ShmSocket:" + QString::number(uPort);
                        pSharedMemorySocketCommChan->setKey(strKey);

                        if( pSharedMemorySocketCommChan->create(static_cast<int>(pShmMsgReqConnect->m_uBufferSize)) )
                        {
                            //#if QT_VERSION < 0x040501
                            //pSharedMemorySocketCommChan->attach();
                            //#endif
                            break;
                        }
                    }
                    if( !pSharedMemorySocketCommChan->isAttached() )
                    {
                        delete pSharedMemorySocketCommChan;
                        pSharedMemorySocketCommChan = nullptr;
                    }
                    else
                    {
                        // Confirm the request message through the servers control channel:
                        if( pShmMsgReqConnect->m_bAck )
                        {
                            pShmMsgReqConnect->m_bAck = false;
                            pShmMsgReqConnect->m_srvCltTypeReceiver = ESrvCltTypeClient;
                            pShmMsgReqConnect->m_msgType = EMsgTypeConConnect;
                            pShmMsgReqConnect->m_uPort = uPort;
                        }

                        SSocketDscr socketDscr(
                            /* uServerListenPort */ m_uLocalPort,  // the server's (my) listen port
                            /* socketType        */ ESocketTypeShm,
                            /* srvCltType        */ ESrvCltTypeServer );

                        socketDscr.m_socketState = ESocketStateConnected;
                        socketDscr.m_uBufferSize = pSharedMemorySocketCommChan->size();
                        socketDscr.m_strLocalHostName = m_strLocalHostName;
                        socketDscr.m_uLocalPort = m_uLocalPort;
                        socketDscr.m_strRemoteHostName = byteArrRemoteHost;
                        socketDscr.m_uRemotePort = uPort;

                        // Please note that the constructor of the shm socket wrapper will initialize the
                        // main header of the shared memory block data.
                        CShmSocketWrapper* pSocketWrapper = new CShmSocketWrapper( objectName(), socketDscr, pSharedMemorySocketCommChan );

                        m_arpPendingConnections[uPendingConnection] = pSocketWrapper;

                        bNewConnectionPending = true;
                        break;
                    }
                } // if( pShmBlockHdr->m_iBlockLen > 0 )
            } // if( iConnection < m_iMaxPendingConnections )
        } // if( pShmMsgHdr->m_msgType == EMsgTypeReqConnect )
    } // while( uBlock < pShmMainHdrCtrlChan->m_uBlockCount )

    SHAREDMEMORY_UNLOCK;

    if( bNewConnectionPending )
    {
        emit CIpcServerWrapper::newConnection(this);
    }

} // onTimeoutListen
