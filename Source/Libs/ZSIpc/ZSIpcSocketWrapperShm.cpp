/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSIpc/ZSIpcSocketWrapperShm.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysSleeperThread.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
// As "min" will be defined as a macro with two arguments and qdatetime uses "min"
// as a function with no arguments "windows.h" must be included after qdatetime
// (which is included by ZSSysTime (which again is included by ZSSysAux)).
#include <windows.h>
#endif

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;

//#if QT_VERSION < 0x040501
//#define isAttached isValid
//#endif


/*******************************************************************************
struct SShmMainHdr
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
SShmMsgHdr* SShmMainHdr::getMsg( QSharedMemory* i_pSharedMemory, quint32 i_uBlock )
//------------------------------------------------------------------------------
{
    SShmMsgHdr* pShmMsgHdr = nullptr;

    if( i_uBlock < m_uMsgCount )
    {
        char*        pcShmMsgHdr = reinterpret_cast<char*>(i_pSharedMemory->data()) + sizeof(SShmMainHdr);
        unsigned int uBlock;

        pShmMsgHdr = reinterpret_cast<SShmMsgHdr*>(pcShmMsgHdr);

        for( uBlock = 0; uBlock < i_uBlock; uBlock++ )
        {
            pcShmMsgHdr += sizeof(SShmMsgHdr) + pShmMsgHdr->m_uMsgLen;
            pShmMsgHdr = reinterpret_cast<SShmMsgHdr*>(pcShmMsgHdr);
        }
    }
    return pShmMsgHdr;

} // getMsg

//------------------------------------------------------------------------------
SShmMsgHdr* SShmMainHdr::findMsg( QSharedMemory* i_pSharedMemory, qint32 i_iMsgId )
//------------------------------------------------------------------------------
{
    SShmMsgHdr* pShmMsgHdr = nullptr;

    if( m_uMsgCount > 0 )
    {
        char*        pcShmMsgHdr = reinterpret_cast<char*>(i_pSharedMemory->data()) + sizeof(SShmMainHdr);
        unsigned int uBlock;

        pShmMsgHdr = reinterpret_cast<SShmMsgHdr*>(pcShmMsgHdr);

        for( uBlock = 0; uBlock < m_uMsgCount; uBlock++ )
        {
            if( pShmMsgHdr->m_iMsgId == i_iMsgId )
            {
                break;
            }
            pcShmMsgHdr += sizeof(SShmMsgHdr) + pShmMsgHdr->m_uMsgLen;
            pShmMsgHdr = reinterpret_cast<SShmMsgHdr*>(pcShmMsgHdr);
        }
        if( uBlock >= m_uMsgCount )
        {
            pShmMsgHdr = nullptr;
        }
    }
    return pShmMsgHdr;

} // findMsg

//------------------------------------------------------------------------------
void SShmMainHdr::removeMsgAtIdx( QSharedMemory* i_pSharedMemory, quint32 i_uBlock )
//------------------------------------------------------------------------------
{
    SShmMsgHdr* pShmMsgHdr = getMsg(i_pSharedMemory,i_uBlock);

    if( pShmMsgHdr != nullptr )
    {
        removeMsg(i_pSharedMemory,pShmMsgHdr);
    }

} // removeMsgAtIdx

//------------------------------------------------------------------------------
void SShmMainHdr::removeMsgById( QSharedMemory* i_pSharedMemory, qint32 i_iMsgId )
//------------------------------------------------------------------------------
{
    SShmMsgHdr* pShmMsgHdr = findMsg(i_pSharedMemory,i_iMsgId);

    if( pShmMsgHdr != nullptr )
    {
        removeMsg(i_pSharedMemory,pShmMsgHdr);
    }

} // removeMsgById

//------------------------------------------------------------------------------
void SShmMainHdr::removeMsg( QSharedMemory* i_pSharedMemory, SShmMsgHdr* i_pMsgHdr )
//------------------------------------------------------------------------------
{
    if( m_uMsgCount > 0 && m_uBlockLen > 0 && i_pMsgHdr->m_uMsgLen > 0 )
    {
        char*   pcShmDataBeg = reinterpret_cast<char*>(i_pSharedMemory->data()) + sizeof(SShmMainHdr);
        char*   pcShmDataEnd = pcShmDataBeg + m_uBlockLen;
        quint32 uBlockLenRemove = sizeof(SShmMsgHdr) + i_pMsgHdr->m_uMsgLen; // as default the whole message will be removed

        // Not really necessary ..
        if( uBlockLenRemove > m_uBlockLen )
        {
            uBlockLenRemove = m_uBlockLen;
        }

        // If each byte within the shared memory block has been read ...
        if( uBlockLenRemove == m_uBlockLen )
        {
            memset( pcShmDataBeg, 0x00, uBlockLenRemove );

            m_uMsgCount = 0;
            m_uDataMsgCount = 0;
            m_uBlockLen = 0;
        }
        // If not all of the bytes have been removed from the shared memory ...
        else
        {
            // .. a gap need to be filled by moving the still available messages up
            // by the number of removed bytes.

            char* pcMsgBeg = reinterpret_cast<char*>(i_pMsgHdr);
            char* pcMsgEnd = reinterpret_cast<char*>(i_pMsgHdr) + sizeof(SShmMsgHdr) + i_pMsgHdr->m_uMsgLen;

            memcpy( pcMsgBeg, pcMsgEnd, m_uBlockLen-uBlockLenRemove );
            pcShmDataEnd -= uBlockLenRemove;
            memset( pcShmDataEnd, 0x00, uBlockLenRemove );

            m_uMsgCount--;

            if( i_pMsgHdr->m_msgType == EMsgTypeReqReceiveData && m_uDataMsgCount > 1 )
            {
                m_uDataMsgCount--;
            }
            m_uBlockLen -= uBlockLenRemove;
        }
    }

} // removeMsg

//------------------------------------------------------------------------------
SShmMsgHdr* SShmMainHdr::allocMsg( QSharedMemory* i_pSharedMemory, EMsgType i_msgType, quint32 i_uMsgLen )
//------------------------------------------------------------------------------
{
    SShmMsgHdr* pMsgHdr = nullptr;
    int         iShmSize = i_pSharedMemory->size();

    if( sizeof(SShmMainHdr) + m_uBlockLen + i_uMsgLen < static_cast<unsigned int>(iShmSize) )
    {
        char* pcMsgHdr = reinterpret_cast<char*>(i_pSharedMemory->data()) + sizeof(SShmMainHdr) + m_uBlockLen;
        pMsgHdr = reinterpret_cast<SShmMsgHdr*>(pcMsgHdr);

        pMsgHdr->m_msgType = i_msgType;

        m_uBlockLen += i_uMsgLen;
        m_uMsgCount++;

        if( i_msgType == EMsgTypeReqReceiveData )
        {
            m_uDataMsgCount++;
        }
        m_iMsgId++;
    }
    return pMsgHdr;

} // allocMsg


/*******************************************************************************
struct SShmMsgReqReceiveData
*******************************************************************************/

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//quint32 SShmMsgReqReceiveData::read( QSharedMemory* i_pSharedMemory, char* i_pcData, quint32 i_uMaxSize )
////------------------------------------------------------------------------------
//{
//    quint32      uBlockLenRead = 0;
//    SShmMainHdr* pShmMainHdr = reinterpret_cast<SShmMainHdr*>(i_pSharedMemory->data());
//
//    if( pShmMainHdr->m_uMsgCount > 0 && pShmMainHdr->m_uDataMsgCount > 0 && pShmMainHdr->m_uBlockLen > 0 && m_uMsgLen > 0 )
//    {
//        char* pcMsgDataBeg = &m_arcData[m_uReadPos];
//
//        uBlockLenRead = m_uBlockLenData - m_uReadPos; // as default the whole message will be read
//
//        // If just part of the message data should be read ...
//        if( i_uMaxSize > 0 && i_uMaxSize < m_uBlockLenData - m_uReadPos )
//        {
//            uBlockLenRead = i_uMaxSize;
//            memcpy( i_pcData, pcMsgDataBeg, uBlockLenRead );
//            m_uReadPos += uBlockLenRead;
//        }
//        // If the whole message data has been read ...
//        else
//        {
//            memcpy( i_pcData, pcMsgDataBeg, uBlockLenRead );
//            pShmMainHdr->removeMsg(i_pSharedMemory,this);
//        }
//    }
//    return uBlockLenRead;
//
//} // read

//------------------------------------------------------------------------------
QByteArray SShmMsgReqReceiveData::read( QSharedMemory* i_pSharedMemory, quint32 i_uMaxSize )
//------------------------------------------------------------------------------
{
    QByteArray   byteArrRead;
    SShmMainHdr* pShmMainHdr = reinterpret_cast<SShmMainHdr*>(i_pSharedMemory->data());

    if( pShmMainHdr->m_uMsgCount > 0 && pShmMainHdr->m_uDataMsgCount > 0 && pShmMainHdr->m_uBlockLen > 0 && m_uMsgLen > 0 )
    {
        char*   pcMsgDataBeg = &m_arcData[m_uReadPos];
        quint32 uBlockLenRead = m_uBlockLenData - m_uReadPos; // as default the whole message will be read

        // If just part of the message data should be read ...
        if( i_uMaxSize > 0 && i_uMaxSize < m_uBlockLenData - m_uReadPos )
        {
            uBlockLenRead = i_uMaxSize;
            byteArrRead.resize(uBlockLenRead);
            memcpy( byteArrRead.data(), pcMsgDataBeg, uBlockLenRead );
            m_uReadPos += uBlockLenRead;
        }
        // If the whole message data has been read ...
        else
        {
            byteArrRead.resize(uBlockLenRead);
            memcpy( byteArrRead.data(), pcMsgDataBeg, uBlockLenRead );
            pShmMainHdr->removeMsg(i_pSharedMemory,this);
        }
    }
    return byteArrRead;

} // read

//------------------------------------------------------------------------------
QByteArray SShmMsgReqReceiveData::readAll( QSharedMemory* i_pSharedMemory )
//------------------------------------------------------------------------------
{
    QByteArray   byteArrRead;
    SShmMainHdr* pShmMainHdr = reinterpret_cast<SShmMainHdr*>(i_pSharedMemory->data());

    if( pShmMainHdr->m_uMsgCount > 0 && pShmMainHdr->m_uDataMsgCount > 0 && pShmMainHdr->m_uBlockLen > 0 && m_uMsgLen > 0 )
    {
        char*   pcMsgDataBeg = &m_arcData[m_uReadPos];
        quint32 uBlockLenRead = m_uBlockLenData - m_uReadPos;

        byteArrRead.resize(uBlockLenRead);
        memcpy( byteArrRead.data(), pcMsgDataBeg, uBlockLenRead );
        pShmMainHdr->removeMsg(i_pSharedMemory,this);
    }
    return byteArrRead;

} // readAll


/*******************************************************************************
class CShmSocketWrapper : public CIpcSocketWrapper
*******************************************************************************/

#define SHM_SERVERCTRLCHAN_LOCK \
    m_iSharedMemoryServerCtrlChanLockCount++; \
    m_pSharedMemoryServerCtrlChan->lock()

#define SHM_SERVERCTRLCHAN_UNLOCK \
    m_iSharedMemoryServerCtrlChanLockCount--; \
    m_pSharedMemoryServerCtrlChan->unlock()

#define SHM_SOCKETCOMMCHAN_LOCK \
    m_iSharedMemorySocketCommChanLockCount++; \
    m_pSharedMemorySocketCommChan->lock()

#define SHM_SOCKETCOMMCHAN_UNLOCK \
    m_iSharedMemorySocketCommChanLockCount--; \
    m_pSharedMemorySocketCommChan->unlock()


/*==============================================================================
public: // class methods
==============================================================================*/

struct SSharedMemoryIntVal2WrapperIntVal
{
    int m_iSharedMemoryVal;
    int m_iWrapperVal;
};

/* Converting tcp socket errors into the error definitions of the wrapper class
==============================================================================*/

const SSharedMemoryIntVal2WrapperIntVal s_arSharedMemoryError2WrapperError[] =
{
    //#if QT_VERSION < 0x040501
    //{ QSharedMemory::OutOfMemory,      EResultDatagramTooLarge },
    //#else
    { QSharedMemory::NoError,          EResultSuccess },
    //#endif
    { QSharedMemory::PermissionDenied, EResultPermissionDenied },
    { QSharedMemory::InvalidSize,      EResultDatagramTooLarge },
    { QSharedMemory::KeyError,         EResultHostNotFound },
    { QSharedMemory::AlreadyExists,    EResultAddressAlreadyInUse },
    { QSharedMemory::NotFound,         EResultHostNotFound },
    { QSharedMemory::LockError,        EResultConnectionRefused },
    { QSharedMemory::OutOfResources,   EResultSocketResourceError },
    { QSharedMemory::UnknownError,     EResultUndefined }
};

//------------------------------------------------------------------------------
EResult CShmSocketWrapper::SharedMemoryError2Result( QSharedMemory::SharedMemoryError i_shmError )
//------------------------------------------------------------------------------
{
    EResult result = EResultUndefined;

    if( i_shmError >= 0 && i_shmError < static_cast<int>(_ZSArrLen(s_arSharedMemoryError2WrapperError)) )
    {
        if( i_shmError == s_arSharedMemoryError2WrapperError[i_shmError].m_iSharedMemoryVal )
        {
            result = static_cast<EResult>(s_arSharedMemoryError2WrapperError[i_shmError].m_iWrapperVal);
        }
        else
        {
            for( int idx = 0; idx < static_cast<int>(_ZSArrLen(s_arSharedMemoryError2WrapperError)); idx++ )
            {
                if( i_shmError == s_arSharedMemoryError2WrapperError[idx].m_iSharedMemoryVal )
                {
                    result = static_cast<EResult>(s_arSharedMemoryError2WrapperError[idx].m_iWrapperVal);
                    break;
                }
            }
        }
    }
    return result;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CShmSocketWrapper::CShmSocketWrapper(
    const QString& i_strObjName,
    ESrvCltType    i_srvCltType,
    int            i_iSocketId,
    CTrcMthFile*   i_pTrcMthFile,
    int            i_iTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    CIpcSocketWrapper(
        /* strObjName             */ i_strObjName,
        /* srvCltType             */ i_srvCltType,
        /* socketType             */ ESocketTypeShm,
        /* iSocketId              */ i_iSocketId,
        /* pSocket                */ nullptr,
        /* pTrcMthFile            */ i_pTrcMthFile,
        /* iTrcMthFileDetailLevel */ i_iTrcMthFileDetailLevel ),
    m_pSharedMemoryServerCtrlChan(nullptr),
    m_iShmMsgReqConnectMsgId(0),
    m_iSharedMemoryServerCtrlChanLockCount(0),
    m_pTimerServerCtrlChanReqConnect(nullptr),
    m_pTimerServerCtrlChanConConnect(nullptr),
    m_iTimeElapsedServerCtrlChanConConnect_ms(0),
    m_pSharedMemorySocketCommChan(nullptr),
    m_iSharedMemorySocketCommChanLockCount(0),
    m_pTimerSocketCommChanListenForReq(nullptr),
    m_pTimerSocketCommChanWaitForCon(nullptr),
    m_pSharedMemorySocketCommChanWatchDog(nullptr),
    m_strKeySocketCommChanWatchDog(),
    m_pTimerSocketCommChanWatchDog(nullptr),
    m_errResultInfo()
{
    // The derived classes must instantiate the trace admin object and trace the ctor.
    if( m_pTrcMthFile == nullptr )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strAddInfo         */ "" );

    m_pSharedMemoryServerCtrlChan = new QSharedMemory();

    m_socketDscr.m_strLocalHostName = "Undefined";
    m_socketDscr.m_socketState = ESocketStateUnconnected;

    createTimer();

} // ctor

//------------------------------------------------------------------------------
CShmSocketWrapper::CShmSocketWrapper(
    const QString&     i_strObjName,
    const SSocketDscr& i_socketDscr,
    QSharedMemory*     i_pSharedMemory,
    CTrcMthFile*       i_pTrcMthFile,
    int                i_iTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    CIpcSocketWrapper(
        /* strObjName             */ i_strObjName,
        /* socketDscr             */ i_socketDscr,
        /* pSocket                */ i_pSharedMemory,
        /* pTrcMthFile            */ i_pTrcMthFile,
        /* iTrcMthFileDetailLevel */ i_iTrcMthFileDetailLevel ),
    m_pSharedMemoryServerCtrlChan(nullptr),
    m_iShmMsgReqConnectMsgId(0),
    m_iSharedMemoryServerCtrlChanLockCount(0),
    m_pTimerServerCtrlChanReqConnect(nullptr),
    m_pTimerServerCtrlChanConConnect(nullptr),
    m_iTimeElapsedServerCtrlChanConConnect_ms(0),
    m_pSharedMemorySocketCommChan(i_pSharedMemory),
    m_iSharedMemorySocketCommChanLockCount(0),
    m_pTimerSocketCommChanListenForReq(nullptr),
    m_pTimerSocketCommChanWaitForCon(nullptr),
    m_pSharedMemorySocketCommChanWatchDog(nullptr),
    m_strKeySocketCommChanWatchDog(),
    m_pTimerSocketCommChanWatchDog(nullptr),
    m_errResultInfo()
{
    // The derived classes must instantiate the trace admin object and trace the ctor.
    if( m_pTrcMthFile == nullptr )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strAddInfo         */ "" );

    m_pSharedMemoryServerCtrlChan = new QSharedMemory();

    createTimer();

    if( m_pSharedMemorySocketCommChan != nullptr && m_pSharedMemorySocketCommChan->isAttached() )
    {
        m_strKeySocketCommChanWatchDog = m_pSharedMemorySocketCommChan->key() + "/WatchDog/" + srvCltType2Str(m_socketDscr.getRemoteSrvCltType());

        QString strMySocketCommChanWatchDogKey = m_pSharedMemorySocketCommChan->key() + "/WatchDog/" + srvCltType2Str(m_socketDscr.m_srvCltType);

        m_pSharedMemorySocketCommChanWatchDog = new QSharedMemory();
        m_pSharedMemorySocketCommChanWatchDog->setKey(strMySocketCommChanWatchDogKey);
        m_pSharedMemorySocketCommChanWatchDog->create(32);

        //#if QT_VERSION < 0x040501
        //m_pSharedMemorySocketCommChanWatchDog->attach();
        //#endif

        // Just for debugging purposes ...
        //int iSharedMemorySocketCommChanWatchDogSize = m_pSharedMemorySocketCommChanWatchDog->size();
        // ... looks like 4096 is the minimum size for shared memory blocks.

        SHM_SOCKETCOMMCHAN_LOCK;

        SShmMainHdr* pShmMainHdr = reinterpret_cast<SShmMainHdr*>(m_pSharedMemorySocketCommChan->data());

        pShmMainHdr->m_iMsgId = 0;
        pShmMainHdr->m_uMsgCount = 0;
        pShmMainHdr->m_uDataMsgCount = 0;
        pShmMainHdr->m_uBlockLen = 0;

        SHM_SOCKETCOMMCHAN_UNLOCK;

        m_pTimerSocketCommChanListenForReq->start(c_iTimerPeriodSocketCommChanListenForReq_ms);
        m_pTimerSocketCommChanWatchDog->start(c_iTimerPeriodSocketCommChanWatchDog_ms);
    }

} // ctor

//------------------------------------------------------------------------------
CShmSocketWrapper::~CShmSocketWrapper()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strAddInfo         */ "" );

    try
    {
        delete m_pSharedMemorySocketCommChanWatchDog;
    }
    catch(...)
    {
    }
    m_pSharedMemorySocketCommChanWatchDog = nullptr;

    try
    {
        delete m_pSharedMemorySocketCommChan;
    }
    catch(...)
    {
    }
    m_pSharedMemorySocketCommChan = nullptr;

    try
    {
        delete m_pSharedMemoryServerCtrlChan;
    }
    catch(...)
    {
    }
    m_pSharedMemoryServerCtrlChan = nullptr;

    m_pTimerServerCtrlChanReqConnect = nullptr;
    m_pTimerServerCtrlChanConConnect = nullptr;
    m_pTimerSocketCommChanListenForReq = nullptr;
    m_pTimerSocketCommChanWaitForCon = nullptr;
    m_pTimerSocketCommChanWatchDog = nullptr;

    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();

        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;
    }

} // dtor

/*==============================================================================
public: // must overridables of base class CIpcSocketWrapper
==============================================================================*/

//------------------------------------------------------------------------------
void CShmSocketWrapper::connectToHost(
    const QString& i_strRemoteHostName,
    unsigned int   i_uRemotePort,
    unsigned int   i_uBufferSize )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "connectToHost",
        /* strAddInfo         */ "" );

    if( m_pSharedMemorySocketCommChan != nullptr || m_socketDscr.m_socketState != ESocketStateUnconnected )
    {
        m_errResultInfo.setResult(EResultInvalidMethodCall);
        emit CIpcSocketWrapper::error(this,m_errResultInfo);
        return;
    }

    m_socketDscr.m_strRemoteHostName = i_strRemoteHostName;
    m_socketDscr.m_uServerListenPort = i_uRemotePort;
    m_socketDscr.m_uBufferSize = i_uBufferSize;

    if( m_pSharedMemoryServerCtrlChan->isAttached() )
    {
        m_pSharedMemoryServerCtrlChan->detach();
    }

    QString strKey = m_socketDscr.m_strRemoteHostName + "/ShmServer:" + QString::number(m_socketDscr.m_uServerListenPort);

    m_pSharedMemoryServerCtrlChan->setKey(strKey);

    // The server must create its control channel and we need to wait until the server
    // is listening for incoming connections ...
    if( !m_pSharedMemoryServerCtrlChan->attach() )
    {
        m_errResultInfo.setResult( SharedMemoryError2Result(m_pSharedMemoryServerCtrlChan->error()) );
        emit CIpcSocketWrapper::error(this,m_errResultInfo);
        return;
    }

    SHM_SERVERCTRLCHAN_LOCK;

    SShmMainHdr*       pShmMainHdrCtrlChan = reinterpret_cast<SShmMainHdr*>(m_pSharedMemoryServerCtrlChan->data());
    quint32            uMsgLen = sizeof(SShmMsgHdrReqConnect) + m_socketDscr.m_strLocalHostName.length();
    SShmMsgReqConnect* pShmMsgReqConnect = reinterpret_cast<SShmMsgReqConnect*>(pShmMainHdrCtrlChan->allocMsg(m_pSharedMemoryServerCtrlChan,EMsgTypeReqConnect,uMsgLen));

    if( pShmMsgReqConnect == nullptr )
    {
        m_errResultInfo.setResult(EResultDatagramTooLarge);
        emit CIpcSocketWrapper::error(this,m_errResultInfo);
        SHM_SERVERCTRLCHAN_UNLOCK;
        return;
    }

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QByteArray byteArrLocalHost = m_socketDscr.m_strLocalHostName.toAscii();
    #else
    QByteArray byteArrLocalHost = m_socketDscr.m_strLocalHostName.toUtf8();
    #endif

    pShmMsgReqConnect->m_srvCltTypeReceiver = ESrvCltTypeServer;
    pShmMsgReqConnect->m_msgType = EMsgTypeReqConnect;
    pShmMsgReqConnect->m_iMsgId = pShmMainHdrCtrlChan->m_iMsgId;
    pShmMsgReqConnect->m_uMsgLen = uMsgLen - sizeof(SShmMsgHdr);
    pShmMsgReqConnect->m_bAck = true; // connection requests must always be confirmed
    pShmMsgReqConnect->m_uBufferSize = m_socketDscr.m_uBufferSize;
    pShmMsgReqConnect->m_uBlockLenHostNameClient = byteArrLocalHost.size();
    memcpy( pShmMsgReqConnect->m_arcHostNameClient, byteArrLocalHost.data(), byteArrLocalHost.size() );

    m_iShmMsgReqConnectMsgId = pShmMsgReqConnect->m_iMsgId;

    if( !m_pTimerServerCtrlChanConConnect->isActive() )
    {
        m_iTimeElapsedServerCtrlChanConConnect_ms = 0;
        m_pTimerServerCtrlChanConConnect->start(c_iTimerPeriodServerCtrlChanConConnect_ms);
    }
    SHM_SERVERCTRLCHAN_UNLOCK;

    m_socketDscr.m_socketState = ESocketStateConnecting;

} // connectToHost

//------------------------------------------------------------------------------
bool CShmSocketWrapper::waitForConnected( int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "waitForConnected",
        /* strAddInfo         */ "" );

    bool bConnected = (m_socketDscr.m_socketState == ESocketStateConnected);

    if( !bConnected && i_iTimeout_ms > 0 )
    {
        double fWaitStartTime_ms = Time::getProcTimeInMilliSec();
        double fWaitCurrTime_ms = fWaitStartTime_ms;
        int    iSleepTime_ms = 100;

        while( fWaitCurrTime_ms - fWaitStartTime_ms < static_cast<double>(i_iTimeout_ms) )
        {
            CSleeperThread::msleep(iSleepTime_ms);

            fWaitCurrTime_ms = Time::getProcTimeInMilliSec();

            onTimeoutServerCtrlChanConConnect();

            bConnected = (m_socketDscr.m_socketState == ESocketStateConnected);
        }
    }

    return bConnected;

} // waitForConnected

//------------------------------------------------------------------------------
void CShmSocketWrapper::disconnectFromHost()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "disconnectFromHost",
        /* strAddInfo         */ "" );

    if( m_socketDscr.m_socketState == ESocketStateConnecting )
    {
        if( m_pSharedMemorySocketCommChan != nullptr )
        {
            m_errResultInfo.setResult(EResultInvalidMethodCall);
            emit CIpcSocketWrapper::error(this,m_errResultInfo);
        }

        SHM_SERVERCTRLCHAN_LOCK;

        SShmMainHdr*       pShmMainHdrCtrlChan = reinterpret_cast<SShmMainHdr*>(m_pSharedMemoryServerCtrlChan->data());
        SShmMsgReqConnect* pShmMsgReqConnect = reinterpret_cast<SShmMsgReqConnect*>(pShmMainHdrCtrlChan->findMsg(m_pSharedMemoryServerCtrlChan,m_iShmMsgReqConnectMsgId));

        if( pShmMsgReqConnect != nullptr )
        {
            pShmMainHdrCtrlChan->removeMsg(m_pSharedMemoryServerCtrlChan,pShmMsgReqConnect);
            pShmMsgReqConnect = nullptr;
        }
        SHM_SERVERCTRLCHAN_UNLOCK;
    }

    else if( m_socketDscr.m_socketState == ESocketStateConnected )
    {
        if( m_pSharedMemorySocketCommChan == nullptr || !m_pSharedMemorySocketCommChan->isAttached() )
        {
            m_errResultInfo.setResult(EResultInvalidMethodCall);
            emit CIpcSocketWrapper::error(this,m_errResultInfo);
        }
        else
        {
            SHM_SOCKETCOMMCHAN_LOCK;

            SShmMainHdr*          pShmMainHdrCommChan = reinterpret_cast<SShmMainHdr*>(m_pSharedMemorySocketCommChan->data());
            quint32               uMsgLen = sizeof(SShmMsgReqDisconnect) + m_socketDscr.m_strLocalHostName.length();
            SShmMsgReqDisconnect* pShmMsgReqDisconnect = reinterpret_cast<SShmMsgReqDisconnect*>(pShmMainHdrCommChan->allocMsg(m_pSharedMemorySocketCommChan,EMsgTypeReqDisconnect,uMsgLen));

            if( pShmMsgReqDisconnect == nullptr )
            {
                m_errResultInfo.setResult(EResultDatagramTooLarge);
                emit CIpcSocketWrapper::error(this,m_errResultInfo);
                SHM_SOCKETCOMMCHAN_UNLOCK;
                return;
            }

            pShmMsgReqDisconnect->m_srvCltTypeReceiver = ESrvCltTypeServer;
            pShmMsgReqDisconnect->m_msgType = EMsgTypeReqDisconnect;
            pShmMsgReqDisconnect->m_iMsgId = pShmMainHdrCommChan->m_iMsgId;
            pShmMsgReqDisconnect->m_uMsgLen = uMsgLen - sizeof(SShmMsgHdr);
            pShmMsgReqDisconnect->m_bAck = true; // this disconnect request should be confirmed

            SHM_SOCKETCOMMCHAN_UNLOCK;
        }
    }
    m_socketDscr.m_socketState = ESocketStateClosing; // not really necessary

    if( m_pTimerServerCtrlChanReqConnect->isActive() )
    {
        m_pTimerServerCtrlChanReqConnect->stop();
    }
    if( m_pTimerSocketCommChanWatchDog->isActive() )
    {
        m_pTimerSocketCommChanWatchDog->stop();
    }
    if( m_pTimerSocketCommChanListenForReq->isActive() )
    {
        m_pTimerSocketCommChanListenForReq->stop();
    }
    if( !m_pTimerSocketCommChanWaitForCon->isActive() )
    {
        m_pTimerSocketCommChanWaitForCon->start(c_iTimerPeriodSocketCommChanWaitForCon_ms);
    }

} // disconnectFromHost

//------------------------------------------------------------------------------
bool CShmSocketWrapper::waitForDisconnected( int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "waitForDisconnected",
        /* strAddInfo         */ "" );

    bool bDisconnected = (m_socketDscr.m_socketState == ESocketStateUnconnected);

    if( !bDisconnected && i_iTimeout_ms > 0 )
    {
        double fWaitStartTime_ms = Time::getProcTimeInMilliSec();
        double fWaitCurrTime_ms = fWaitStartTime_ms;
        int    iSleepTime_ms = 100;

        while( fWaitCurrTime_ms - fWaitStartTime_ms < static_cast<double>(i_iTimeout_ms) )
        {
            CSleeperThread::msleep(iSleepTime_ms);

            fWaitCurrTime_ms = Time::getProcTimeInMilliSec();

            onTimeoutSocketCommChanWaitForCon();

            bDisconnected = (m_socketDscr.m_socketState == ESocketStateUnconnected);
        }
    }

    return bDisconnected;

} // waitForDisconnected

//------------------------------------------------------------------------------
void CShmSocketWrapper::abort()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "abort",
        /* strAddInfo         */ "" );

    if( m_socketDscr.m_socketState == ESocketStateConnecting )
    {
        if( m_pSharedMemorySocketCommChan != nullptr )
        {
            m_errResultInfo.setResult(EResultInvalidMethodCall);
            emit CIpcSocketWrapper::error(this,m_errResultInfo);
        }

        SHM_SERVERCTRLCHAN_LOCK;

        SShmMainHdr*       pShmMainHdrCtrlChan = reinterpret_cast<SShmMainHdr*>(m_pSharedMemoryServerCtrlChan->data());
        SShmMsgReqConnect* pShmMsgReqConnect = reinterpret_cast<SShmMsgReqConnect*>(pShmMainHdrCtrlChan->findMsg(m_pSharedMemoryServerCtrlChan,m_iShmMsgReqConnectMsgId));

        if( pShmMsgReqConnect != nullptr )
        {
            pShmMainHdrCtrlChan->removeMsg(m_pSharedMemoryServerCtrlChan,pShmMsgReqConnect);
            pShmMsgReqConnect = nullptr;
        }
        SHM_SERVERCTRLCHAN_UNLOCK;
    }

    else if( m_socketDscr.m_socketState == ESocketStateConnected )
    {
        if( m_pSharedMemorySocketCommChan == nullptr || !m_pSharedMemorySocketCommChan->isAttached() )
        {
            m_errResultInfo.setResult(EResultInvalidMethodCall);
            emit CIpcSocketWrapper::error(this,m_errResultInfo);
        }
        else
        {
            SHM_SOCKETCOMMCHAN_LOCK;

            SShmMainHdr*          pShmMainHdrCommChan = reinterpret_cast<SShmMainHdr*>(m_pSharedMemorySocketCommChan->data());
            quint32               uMsgLen = sizeof(SShmMsgReqDisconnect) + m_socketDscr.m_strLocalHostName.length();
            SShmMsgReqDisconnect* pShmMsgReqDisconnect = reinterpret_cast<SShmMsgReqDisconnect*>(pShmMainHdrCommChan->allocMsg(m_pSharedMemorySocketCommChan,EMsgTypeReqDisconnect,uMsgLen));

            if( pShmMsgReqDisconnect == nullptr )
            {
                m_errResultInfo.setResult(EResultDatagramTooLarge);
                emit CIpcSocketWrapper::error(this,m_errResultInfo);
                SHM_SOCKETCOMMCHAN_UNLOCK;
                return;
            }

            pShmMsgReqDisconnect->m_srvCltTypeReceiver = ESrvCltTypeServer;
            pShmMsgReqDisconnect->m_msgType = EMsgTypeReqDisconnect;
            pShmMsgReqDisconnect->m_iMsgId = pShmMainHdrCommChan->m_iMsgId;
            pShmMsgReqDisconnect->m_uMsgLen = uMsgLen - sizeof(SShmMsgHdr);

            // The server need to delete its socket and the corresponding socket communication channels
            // shared memory block whereupon the memory block should be freed by the operating system.
            // This is indicated by setting the acknowledge flag to false.
            pShmMsgReqDisconnect->m_bAck = false;

            SHM_SOCKETCOMMCHAN_UNLOCK;
        }
    }
    m_socketDscr.m_socketState = ESocketStateClosing; // not really necessary

    if( m_pTimerServerCtrlChanReqConnect->isActive() )
    {
        m_pTimerServerCtrlChanReqConnect->stop();
    }
    if( m_pTimerServerCtrlChanConConnect->isActive() )
    {
        m_pTimerServerCtrlChanConConnect->stop();
    }
    if( m_pTimerSocketCommChanWatchDog->isActive() )
    {
        m_pTimerSocketCommChanWatchDog->stop();
    }
    if( m_pTimerSocketCommChanListenForReq->isActive() )
    {
        m_pTimerSocketCommChanListenForReq->stop();
    }
    if( m_pTimerSocketCommChanWaitForCon->isActive() )
    {
        m_pTimerSocketCommChanWaitForCon->stop();
    }

    // Delete shared memory instance used to access the shared memory block.
    delete m_pSharedMemorySocketCommChan;
    m_pSharedMemorySocketCommChan = nullptr;

    m_pSharedMemoryServerCtrlChan->detach();

    m_socketDscr.m_socketState = ESocketStateUnconnected;
    emit CIpcSocketWrapper::disconnected(this);

} // abort

////------------------------------------------------------------------------------
//qint64 CShmSocketWrapper::write( const char* i_pcData, qint64 i_iMaxSize )
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj          */ m_pTrcAdminObj,
//        /* pTrcMthFile        */ m_pTrcMthFile,
//        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
//        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
//        /* strNameSpace       */ nameSpace(),
//        /* strClassName       */ className(),
//        /* strObjName         */ objectName(),
//        /* strMethod          */ "write",
//        /* strAddInfo         */ "" );
//
//    if( m_pSharedMemorySocketCommChan == nullptr )
//    {
//        QString strAddInfo = "write( m_pSharedMemorySocketCommChan == nullptr )";
//        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
//    }
//
//    quint32 uDataLen = 0;
//
//    if( i_iMaxSize > UINT_MAX )
//    {
//        uDataLen = UINT_MAX;
//    }
//    else
//    {
//        uDataLen = static_cast<quint32>(i_iMaxSize);
//    }
//
//    SHM_SOCKETCOMMCHAN_LOCK;
//
//    SShmMainHdr*           pShmMainHdrCommChan = reinterpret_cast<SShmMainHdr*>(m_pSharedMemorySocketCommChan->data());
//    quint32                uBlockLen = sizeof(SShmMsgHdrReqReceiveData) + uDataLen;
//    SShmMsgReqReceiveData* pShmMsgReqReceiveData = reinterpret_cast<SShmMsgReqReceiveData*>(pShmMainHdrCommChan->allocMsg(m_pSharedMemorySocketCommChan,EMsgTypeReqReceiveData,uBlockLen));
//
//    if( pShmMsgReqReceiveData == nullptr )
//    {
//        m_errResultInfo.setResult(EResultDatagramTooLarge);
//        emit CIpcSocketWrapper::error(this,m_errResultInfo);
//        SHM_SOCKETCOMMCHAN_UNLOCK;
//        return -1;
//    }
//
//    pShmMsgReqReceiveData->m_srvCltTypeReceiver = m_socketDscr.getRemoteSrvCltType();
//    pShmMsgReqReceiveData->m_msgType = EMsgTypeReqReceiveData;
//    pShmMsgReqReceiveData->m_iMsgId = pShmMainHdrCommChan->m_iMsgId;
//    pShmMsgReqReceiveData->m_uMsgLen = uBlockLen - sizeof(SShmMsgHdr);
//    pShmMsgReqReceiveData->m_uReadPos = 0;
//    pShmMsgReqReceiveData->m_uBlockLenData = uDataLen;
//    memcpy( pShmMsgReqReceiveData->m_arcData, i_pcData, uDataLen );
//
//    SHM_SOCKETCOMMCHAN_UNLOCK;
//
//    return uDataLen;
//
//} // write

//------------------------------------------------------------------------------
qint64 CShmSocketWrapper::write( const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "write",
        /* strAddInfo         */ "" );

    if( m_pSharedMemorySocketCommChan == nullptr )
    {
        QString strAddInfo = "write( m_pSharedMemorySocketCommChan == nullptr )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
    }

    SHM_SOCKETCOMMCHAN_LOCK;

    SShmMainHdr*           pShmMainHdrCommChan = reinterpret_cast<SShmMainHdr*>(m_pSharedMemorySocketCommChan->data());
    quint32                uBlockLen = sizeof(SShmMsgHdrReqReceiveData) + i_byteArr.size();
    SShmMsgReqReceiveData* pShmMsgReqReceiveData = reinterpret_cast<SShmMsgReqReceiveData*>(pShmMainHdrCommChan->allocMsg(m_pSharedMemorySocketCommChan,EMsgTypeReqReceiveData,uBlockLen));

    if( pShmMsgReqReceiveData == nullptr )
    {
        m_errResultInfo.setResult(EResultDatagramTooLarge);
        emit CIpcSocketWrapper::error(this,m_errResultInfo);
        SHM_SOCKETCOMMCHAN_UNLOCK;
        return -1;
    }

    pShmMsgReqReceiveData->m_srvCltTypeReceiver = m_socketDscr.getRemoteSrvCltType();
    pShmMsgReqReceiveData->m_msgType = EMsgTypeReqReceiveData;
    pShmMsgReqReceiveData->m_iMsgId = pShmMainHdrCommChan->m_iMsgId;
    pShmMsgReqReceiveData->m_uMsgLen = uBlockLen - sizeof(SShmMsgHdr);
    pShmMsgReqReceiveData->m_uReadPos = 0;
    pShmMsgReqReceiveData->m_uBlockLenData = i_byteArr.size();
    memcpy( pShmMsgReqReceiveData->m_arcData, i_byteArr.data(), i_byteArr.size() );

    SHM_SOCKETCOMMCHAN_UNLOCK;

    return i_byteArr.size();

} // write

//------------------------------------------------------------------------------
bool CShmSocketWrapper::waitForReadyRead( int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "waitForReadyRead",
        /* strAddInfo         */ "" );

    bool bDataAvailable = false;

    SShmMainHdr* pShmMainHdrCommChan = reinterpret_cast<SShmMainHdr*>(m_pSharedMemorySocketCommChan->data());

    SHM_SOCKETCOMMCHAN_LOCK;

    if( pShmMainHdrCommChan->m_uDataMsgCount > 0 )
    {
        bDataAvailable = true;
    }
    SHM_SOCKETCOMMCHAN_UNLOCK;

    if( !bDataAvailable )
    {
        int iTimeElapsed_ms = 0;

        while( !bDataAvailable && iTimeElapsed_ms < i_iTimeout_ms )
        {
            if( i_iTimeout_ms < 10 )
            {
                CSleeperThread::msleep(i_iTimeout_ms);
                iTimeElapsed_ms += i_iTimeout_ms;
            }
            else
            {
                CSleeperThread::msleep(10);
                iTimeElapsed_ms += 10;
            }

            SHM_SOCKETCOMMCHAN_LOCK;

            if( pShmMainHdrCommChan->m_uDataMsgCount > 0 )
            {
                bDataAvailable = true;
            }
            SHM_SOCKETCOMMCHAN_UNLOCK;
        }
    }
    return bDataAvailable;

} // waitForReadyRead

//------------------------------------------------------------------------------
qint64 CShmSocketWrapper::bytesAvailable() const
//------------------------------------------------------------------------------
{
    SHM_SOCKETCOMMCHAN_LOCK;

    qint64       iBytesAvailable = 0;
    SShmMainHdr* pShmMainHdrCommChan = reinterpret_cast<SShmMainHdr*>(m_pSharedMemorySocketCommChan->data());
    unsigned int uBlock;

    for( uBlock = 0; uBlock < pShmMainHdrCommChan->m_uMsgCount; uBlock++ )
    {
        SShmMsgHdr* pShmMsgHdr = pShmMainHdrCommChan->getMsg(m_pSharedMemorySocketCommChan,uBlock);

        if( pShmMsgHdr->m_srvCltTypeReceiver == m_socketDscr.m_srvCltType && pShmMsgHdr->m_msgType == EMsgTypeReqReceiveData )
        {
            SShmMsgReqReceiveData* pShmMsgReqReceiveData = reinterpret_cast<SShmMsgReqReceiveData*>(pShmMsgHdr);

            iBytesAvailable += pShmMsgReqReceiveData->m_uBlockLenData - pShmMsgReqReceiveData->m_uReadPos;

        } // if( pShmMsgHdr->m_msgType == EMsgTypeReqReceiveData )
    } // for( uBlock < pShmMainHdrCommChan->m_uBlockCount )

    SHM_SOCKETCOMMCHAN_UNLOCK;

    return iBytesAvailable;

} // bytesAvailable

////------------------------------------------------------------------------------
//qint64 CShmSocketWrapper::read( char* i_pcData, qint64 i_iMaxSize )
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj          */ m_pTrcAdminObj,
//        /* pTrcMthFile        */ m_pTrcMthFile,
//        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
//        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
//        /* strNameSpace       */ nameSpace(),
//        /* strClassName       */ className(),
//        /* strObjName         */ objectName(),
//        /* strMethod          */ "read",
//        /* strAddInfo         */ "" );
//
//    if( m_pSharedMemorySocketCommChan == nullptr )
//    {
//        QString strAddInfo = "write( m_pSharedMemorySocketCommChan == nullptr )";
//        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
//    }
//
//    SHM_SOCKETCOMMCHAN_LOCK;
//
//    SShmMainHdr* pShmMainHdrCommChan = reinterpret_cast<SShmMainHdr*>(m_pSharedMemorySocketCommChan->data());
//    qint64       iReadBlockSize = 0;
//    unsigned int uBlock;
//
//    for( uBlock = 0; uBlock < pShmMainHdrCommChan->m_uMsgCount; uBlock++ )
//    {
//        SShmMsgHdr* pShmMsgHdr = pShmMainHdrCommChan->getMsg(m_pSharedMemorySocketCommChan,uBlock);
//        ESrvCltType srvCltTypeReceiver = pShmMsgHdr->m_srvCltTypeReceiver;
//
//        if( pShmMsgHdr != nullptr && srvCltTypeReceiver == m_socketDscr.m_srvCltType && pShmMsgHdr->m_msgType == EMsgTypeReqReceiveData )
//        {
//            SShmMsgReqReceiveData* pShmMsgReqReceiveData = reinterpret_cast<SShmMsgReqReceiveData*>(pShmMsgHdr);
//            iReadBlockSize = pShmMsgReqReceiveData->read( m_pSharedMemorySocketCommChan, i_pcData, static_cast<quint32>(i_iMaxSize) );
//            break;
//        }
//    } // for( uBlock < pShmMainHdrCommChan->m_uBlockCount )
//
//    SHM_SOCKETCOMMCHAN_UNLOCK;
//
//    return iReadBlockSize;
//
//} // read

//------------------------------------------------------------------------------
QByteArray CShmSocketWrapper::read( qint64 i_iMaxSize )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "read",
        /* strAddInfo         */ "" );

    if( m_pSharedMemorySocketCommChan == nullptr )
    {
        QString strAddInfo = "write( m_pSharedMemorySocketCommChan == nullptr )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
    }

    SHM_SOCKETCOMMCHAN_LOCK;

    SShmMainHdr* pShmMainHdrCommChan = reinterpret_cast<SShmMainHdr*>(m_pSharedMemorySocketCommChan->data());
    QByteArray   byteArrRead;
    unsigned int uBlock;

    for( uBlock = 0; uBlock < pShmMainHdrCommChan->m_uMsgCount; uBlock++ )
    {
        SShmMsgHdr* pShmMsgHdr = pShmMainHdrCommChan->getMsg(m_pSharedMemorySocketCommChan,uBlock);
        ESrvCltType srvCltTypeReceiver = pShmMsgHdr->m_srvCltTypeReceiver;

        if( pShmMsgHdr != nullptr && srvCltTypeReceiver == m_socketDscr.m_srvCltType && pShmMsgHdr->m_msgType == EMsgTypeReqReceiveData )
        {
            SShmMsgReqReceiveData* pShmMsgReqReceiveData = reinterpret_cast<SShmMsgReqReceiveData*>(pShmMsgHdr);
            byteArrRead = pShmMsgReqReceiveData->read( m_pSharedMemorySocketCommChan, static_cast<quint32>(i_iMaxSize) );
            break;
        }
    } // for( uBlock < pShmMainHdrCommChan->m_uBlockCount )

    SHM_SOCKETCOMMCHAN_UNLOCK;

    return byteArrRead;

} // read

//------------------------------------------------------------------------------
QByteArray CShmSocketWrapper::readAll()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "readAll",
        /* strAddInfo         */ "" );

    if( m_pSharedMemorySocketCommChan == nullptr )
    {
        QString strAddInfo = "write( m_pSharedMemorySocketCommChan == nullptr )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
    }

    SHM_SOCKETCOMMCHAN_LOCK;

    SShmMainHdr* pShmMainHdrCommChan = reinterpret_cast<SShmMainHdr*>(m_pSharedMemorySocketCommChan->data());
    QByteArray   byteArrRead;
    unsigned int uBlock;

    for( uBlock = 0; uBlock < pShmMainHdrCommChan->m_uMsgCount; uBlock++ )
    {
        SShmMsgHdr* pShmMsgHdr = pShmMainHdrCommChan->getMsg(m_pSharedMemorySocketCommChan,uBlock);
        ESrvCltType srvCltTypeReceiver = pShmMsgHdr->m_srvCltTypeReceiver;

        if( pShmMsgHdr != nullptr && srvCltTypeReceiver == m_socketDscr.m_srvCltType && pShmMsgHdr->m_msgType == EMsgTypeReqReceiveData )
        {
            SShmMsgReqReceiveData* pShmMsgReqReceiveData = reinterpret_cast<SShmMsgReqReceiveData*>(pShmMsgHdr);
            byteArrRead = pShmMsgReqReceiveData->readAll(m_pSharedMemorySocketCommChan);
            break;
        }
    } // for( uBlock < pShmMainHdrCommChan->m_uBlockCount )

    SHM_SOCKETCOMMCHAN_UNLOCK;

    return byteArrRead;

} // readAll

//------------------------------------------------------------------------------
EResult CShmSocketWrapper::error() const
//------------------------------------------------------------------------------
{
    return m_errResultInfo.getResult();
}

//------------------------------------------------------------------------------
QString CShmSocketWrapper::errorString() const
//------------------------------------------------------------------------------
{
    return m_errResultInfo.getResultStr();
}

//------------------------------------------------------------------------------
SErrResultInfo CShmSocketWrapper::errResultInfo() const
//------------------------------------------------------------------------------
{
    return m_errResultInfo;
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CShmSocketWrapper::createTimer()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "createTimer",
        /* strAddInfo         */ "" );

    m_pTimerServerCtrlChanReqConnect = new QTimer(this);
    m_pTimerServerCtrlChanConConnect = new QTimer(this);
    m_pTimerSocketCommChanWatchDog = new QTimer(this);
    m_pTimerSocketCommChanListenForReq = new QTimer(this);
    m_pTimerSocketCommChanWaitForCon = new QTimer(this);

    if( !QObject::connect(
        /* pObjSender   */ m_pTimerServerCtrlChanReqConnect,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTimeoutServerCtrlChanReqConnect()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTimerServerCtrlChanConConnect,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTimeoutServerCtrlChanConConnect()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTimerSocketCommChanWatchDog,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTimeoutSocketCommChanWatchDog()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTimerSocketCommChanListenForReq,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTimeoutSocketCommChanListenForReq()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTimerSocketCommChanWaitForCon,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTimeoutSocketCommChanWaitForCon()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // createTimer

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CShmSocketWrapper::onTimeoutServerCtrlChanReqConnect()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTimeoutServerCtrlChanReqConnect",
        /* strAddInfo         */ "" );

} // onTimeoutServerCtrlChanReqConnect

//------------------------------------------------------------------------------
void CShmSocketWrapper::onTimeoutServerCtrlChanConConnect()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTimeoutServerCtrlChanConConnect",
        /* strAddInfo         */ "" );

    SHM_SERVERCTRLCHAN_LOCK;

    SShmMainHdr* pShmMainHdrCtrlChan = reinterpret_cast<SShmMainHdr*>(m_pSharedMemoryServerCtrlChan->data());
    unsigned int uBlock = 0;
    bool         bTimeout = false;

    if( pShmMainHdrCtrlChan != nullptr )
    {
        while( uBlock < pShmMainHdrCtrlChan->m_uMsgCount )
        {
            SShmMsgHdr* pShmMsgHdr = pShmMainHdrCtrlChan->getMsg(m_pSharedMemoryServerCtrlChan,uBlock);

            uBlock++;

            if( pShmMsgHdr->m_srvCltTypeReceiver == m_socketDscr.m_srvCltType && pShmMsgHdr->m_msgType == EMsgTypeConConnect )
            {
                SShmMsgReqConnect* pShmMsgReqConnect = reinterpret_cast<SShmMsgReqConnect*>(pShmMsgHdr);

                if( pShmMsgReqConnect->m_uBlockLenHostNameClient > 0 )
                {
                    QByteArray byteArrLocalHost;

                    byteArrLocalHost.resize(pShmMsgReqConnect->m_uBlockLenHostNameClient);
                    memcpy( byteArrLocalHost.data(), pShmMsgReqConnect->m_arcHostNameClient, pShmMsgReqConnect->m_uBlockLenHostNameClient );

                    // If the server confirmed the connection request message of this socket ..
                    if( m_iShmMsgReqConnectMsgId == pShmMsgReqConnect->m_iMsgId && byteArrLocalHost == m_socketDscr.m_strLocalHostName )
                    {
                        // .. no longer need to poll the servers control channel for the confirmation message:
                        m_pTimerServerCtrlChanConConnect->stop();

                        // Read the port number for the shared memory block further on used as the
                        // communication channel between the servers ShmSocket and this socket.
                        m_socketDscr.m_uLocalPort = pShmMsgReqConnect->m_uPort;

                        // Remove the connection request message from the servers control channel:
                        pShmMainHdrCtrlChan->removeMsg(m_pSharedMemoryServerCtrlChan,pShmMsgReqConnect);
                        uBlock--;
                        pShmMsgReqConnect = nullptr;

                        // Create shared memory instance used to access the shared memory block.
                        m_pSharedMemorySocketCommChan = new QSharedMemory();
                        m_pSharedMemorySocketCommChan->setKey( m_socketDscr.m_strRemoteHostName + "/ShmSocket:" + QString::number(m_socketDscr.m_uLocalPort) );

                        // The server must have created the shared memory block that will further on be used
                        // as the control and communication channel between the ShmSocket at the server and
                        // the ShmSocket at the client's side.
                        if( !m_pSharedMemorySocketCommChan->attach() )
                        {
                            m_socketDscr.m_socketState = ESocketStateUnconnected;
                            m_errResultInfo.setResult( SharedMemoryError2Result(m_pSharedMemorySocketCommChan->error()) );
                            emit CIpcSocketWrapper::error(this,m_errResultInfo);
                            delete m_pSharedMemorySocketCommChan;
                            m_pSharedMemorySocketCommChan = nullptr;
                        }
                        else
                        {
                            m_strKeySocketCommChanWatchDog = m_pSharedMemorySocketCommChan->key() + "/WatchDog/" + srvCltType2Str(m_socketDscr.getRemoteSrvCltType());

                            QString strMySocketCommChanWatchDogKey = m_pSharedMemorySocketCommChan->key() + "/WatchDog/" + srvCltType2Str(m_socketDscr.m_srvCltType);

                            if( m_pSharedMemorySocketCommChanWatchDog != nullptr )
                            {
                                delete m_pSharedMemorySocketCommChanWatchDog;
                                m_pSharedMemorySocketCommChanWatchDog = nullptr;
                            }
                            m_pSharedMemorySocketCommChanWatchDog = new QSharedMemory();
                            m_pSharedMemorySocketCommChanWatchDog->setKey(strMySocketCommChanWatchDogKey);
                            m_pSharedMemorySocketCommChanWatchDog->create(32);

                            //#if QT_VERSION < 0x040501
                            //m_pSharedMemorySocketCommChanWatchDog->attach();
                            //#endif

                            // Just for debugging purposes ...
                            //int iSharedMemorySocketCommChanWatchDogSize = m_pSharedMemorySocketCommChanWatchDog->size();
                            // ... looks like 4096 is the minimum size for shared memory blocks.

                            m_socketDscr.m_socketState = ESocketStateConnected;
                            emit CIpcSocketWrapper::connected(this);
                            m_pTimerSocketCommChanWatchDog->start(c_iTimerPeriodSocketCommChanWatchDog_ms);
                            m_pTimerSocketCommChanListenForReq->start(c_iTimerPeriodSocketCommChanListenForReq_ms);
                        }
                    } // if( byteArrLocalHost == m_socketDscr.m_strLocalHostName )
                } // if( pShmMsgConConnect->m_uBlockLenHostNameClient > 0 )
            } // if( pShmMsgHdr->m_msgType == EMsgTypeConConnect )
        } // while( uBlock < pShmMainHdrCtrlChan->m_uBlockCount )

        if( m_pTimerServerCtrlChanConConnect->isActive() )
        {
            m_iTimeElapsedServerCtrlChanConConnect_ms += c_iTimerPeriodServerCtrlChanConConnect_ms;

            if( m_iTimeElapsedServerCtrlChanConConnect_ms > c_iTimeoutServerCtrlChanConConnect_ms )
            {
                bTimeout = true;

                m_pTimerServerCtrlChanConConnect->stop();

                // Remove the request message from the servers control channel.
                pShmMainHdrCtrlChan->removeMsgById(m_pSharedMemoryServerCtrlChan,m_iShmMsgReqConnectMsgId);
                m_iShmMsgReqConnectMsgId = 0;
            }
        }
    } // if( pShmMainHdrCtrlChan != nullptr )

    SHM_SERVERCTRLCHAN_UNLOCK;

    if( bTimeout )
    {
        m_socketDscr.m_socketState = ESocketStateUnconnected;
        m_errResultInfo.setResult(EResultTimeout);
        emit CIpcSocketWrapper::error(this,m_errResultInfo);
    }

} // onTimeoutServerCtrlChanConConnect

//------------------------------------------------------------------------------
void CShmSocketWrapper::onTimeoutSocketCommChanListenForReq()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTimeoutSocketCommChanListenForReq",
        /* strAddInfo         */ "" );

    // The timer could have already been stopped after receiving the confirmation
    // message to the disconnect request but the timeout could have already been
    // pending in Qt's event loop ..
    if( m_pSharedMemorySocketCommChan == nullptr )
    {
        return;
    }

    SHM_SOCKETCOMMCHAN_LOCK;

    SShmMainHdr* pShmMainHdrCommChan = reinterpret_cast<SShmMainHdr*>(m_pSharedMemorySocketCommChan->data());
    bool         bDisconnected = false;
    unsigned int uBlock = 0;

    if( pShmMainHdrCommChan != nullptr )
    {
        while( uBlock < pShmMainHdrCommChan->m_uMsgCount && !bDisconnected )
        {
            SShmMsgHdr* pShmMsgHdr = pShmMainHdrCommChan->getMsg(m_pSharedMemorySocketCommChan,uBlock);
            ESrvCltType srvCltTypeReceiver = pShmMsgHdr->m_srvCltTypeReceiver;

            uBlock++;

            if( pShmMsgHdr != nullptr && srvCltTypeReceiver == m_socketDscr.m_srvCltType )
            {
                if( pShmMsgHdr->m_msgType == EMsgTypeReqDisconnect )
                {
                    SShmMsgReqDisconnect* pShmMsgReqDisconnect = reinterpret_cast<SShmMsgReqDisconnect*>(pShmMsgHdr);

                    bDisconnected = true;

                    if( pShmMsgReqDisconnect->m_bAck )
                    {
                        // Confirm the request message through the sockets communication channel:
                        if( srvCltTypeReceiver == ESrvCltTypeServer )
                        {
                            srvCltTypeReceiver = ESrvCltTypeClient;
                        }
                        else // if( srvCltTypeReceiver == ESrvCltTypeClient )
                        {
                            srvCltTypeReceiver = ESrvCltTypeServer;
                        }
                        pShmMsgReqDisconnect->m_srvCltTypeReceiver = srvCltTypeReceiver;
                        pShmMsgReqDisconnect->m_msgType = EMsgTypeConDisconnect;
                        pShmMsgReqDisconnect->m_bAck = false;
                    }
                }
                else if( pShmMsgHdr->m_msgType == EMsgTypeReqReceiveData )
                {
                    emit readyRead(this);
                }
            } // if( pShmMsgHdr != nullptr && pShmMsgHdr->m_srvCltTypeReceiver == m_socketDscr.m_srvCltType )
        } // while( uBlock < pShmMainHdrCommChan->m_uBlockCount )
    } // if( pShmMainHdrCommChan != nullptr )

    SHM_SOCKETCOMMCHAN_UNLOCK;

    if( bDisconnected )
    {
        // No longer need to poll the sockets communication channel for request messages:
        m_pTimerSocketCommChanWatchDog->stop();
        m_pTimerSocketCommChanListenForReq->stop();

        delete m_pSharedMemorySocketCommChan;
        m_pSharedMemorySocketCommChan = nullptr;

        emit CIpcSocketWrapper::disconnected(this);
    }

} // onTimeoutSocketCommChanListenForReq

//------------------------------------------------------------------------------
void CShmSocketWrapper::onTimeoutSocketCommChanWaitForCon()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTimeoutSocketCommChanWaitForCon",
        /* strAddInfo         */ "" );

    // The timer could have already been stopped after receiving the confirmation
    // message to the disconnect request but the timeout could have already been
    // pending in Qt's event loop ..
    if( m_pSharedMemorySocketCommChan == nullptr )
    {
        return;
    }

    SHM_SOCKETCOMMCHAN_LOCK;

    SShmMainHdr* pShmMainHdrCommChan = reinterpret_cast<SShmMainHdr*>(m_pSharedMemorySocketCommChan->data());
    bool         bDisconnected = false;
    unsigned int uBlock = 0;

    if( pShmMainHdrCommChan != nullptr )
    {
        while( uBlock < pShmMainHdrCommChan->m_uMsgCount && !bDisconnected )
        {
            SShmMsgHdr* pShmMsgHdr = pShmMainHdrCommChan->getMsg(m_pSharedMemorySocketCommChan,uBlock);

            uBlock++;

            if( pShmMsgHdr != nullptr && pShmMsgHdr->m_srvCltTypeReceiver == m_socketDscr.m_srvCltType )
            {
                if( pShmMsgHdr->m_msgType == EMsgTypeConDisconnect )
                {
                    SShmMsgReqDisconnect* pShmMsgReqDisconnect = reinterpret_cast<SShmMsgReqDisconnect*>(pShmMsgHdr);

                    // Remove the disconnect request message from the sockets communication channel:
                    pShmMainHdrCommChan->removeMsg(m_pSharedMemorySocketCommChan,pShmMsgReqDisconnect);
                    uBlock--;
                    pShmMsgReqDisconnect = nullptr;
                    bDisconnected = true;
                }
            } // if( pShmMsgHdr != nullptr && pShmMsgHdr->m_srvCltTypeReceiver == m_socketDscr.m_srvCltType )
        } // while( uBlock < pShmMainHdrCommChan->m_uBlockCount )
    } // if( pShmMainHdrCommChan != nullptr )

    SHM_SOCKETCOMMCHAN_UNLOCK;

    if( bDisconnected )
    {
        // No longer need to poll the sockets communication channel for the confirmation message:
        m_pTimerSocketCommChanWaitForCon->stop();

        // Delete shared memory instance used to access the shared memory block.
        delete m_pSharedMemorySocketCommChan;
        m_pSharedMemorySocketCommChan = nullptr;

        m_pSharedMemoryServerCtrlChan->detach();

        m_socketDscr.m_socketState = ESocketStateUnconnected;
        emit CIpcSocketWrapper::disconnected(this);
    }

} // onTimeoutSocketCommChanWaitForCon

//------------------------------------------------------------------------------
void CShmSocketWrapper::onTimeoutSocketCommChanWatchDog()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTimeoutSocketCommChanWatchDog",
        /* strAddInfo         */ "" );

    bool bCounterpartPresent = true;

    // If it is possible to create a shared memory segment with the counterparts
    // watch dog key the counterparts shared memory watch dog segment does no
    // longer exist and the counterparts socket must have been destroyed.

    QSharedMemory* pSharedMemorySocketCommChanWatchDogCounterpart = new QSharedMemory();

    pSharedMemorySocketCommChanWatchDogCounterpart->setKey(m_strKeySocketCommChanWatchDog);
    if( pSharedMemorySocketCommChanWatchDogCounterpart->create(32) )
    {
        bCounterpartPresent = false;
    }
    delete pSharedMemorySocketCommChanWatchDogCounterpart;
    pSharedMemorySocketCommChanWatchDogCounterpart = nullptr;

    if( !bCounterpartPresent )
    {
        m_socketDscr.m_socketState = ESocketStateUnconnected;
        abort();
    }

} // onTimeoutSocketCommChanWatchDog

