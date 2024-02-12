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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qtimer.h>
#include <QtCore/qmutex.h>
#include <QtNetwork/qhostinfo.h>

#include "ZSIpc/ZSIpcInProcMsgSocket.h"
#include "ZSIpc/ZSIpcInProcMsgServer.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;


/*******************************************************************************
class CInProcMsgSocketsAdminObj : public QObject
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

// Should be protected class members.
// But python bindings generated with shiboken will not be compilable then.

CInProcMsgSocketsAdminObj* /*CInProcMsgSocketsAdminObj::*/s_pInProcMsgSocketsAdminObj = nullptr;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CInProcMsgSocketsAdminObj* CInProcMsgSocketsAdminObj::GetInstance()
//------------------------------------------------------------------------------
{
    return s_pInProcMsgSocketsAdminObj;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CInProcMsgSocketsAdminObj::CInProcMsgSocketsAdminObj( bool i_bTracingEnabled ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pMutex(nullptr),
    m_mapSockets(),
    m_bDestroying(false),
    m_bTracingEnabled(i_bTracingEnabled),
    m_pTrcAdminObj(nullptr)
{
    #pragma push_macro("_ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE")
    #ifdef _WINDOWS
    #pragma warning( disable : 4005 )
    #endif
    #define _ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE 0

    if( m_bTracingEnabled )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Ipc", "CInProcMsgSocketsAdminObj", "theAdminObj");
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pMutex = new QMutex();

    #ifdef _WINDOWS
    #pragma warning( default : 4005 )
    #endif
    #pragma pop_macro("_ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE")

} // ctor

//------------------------------------------------------------------------------
CInProcMsgSocketsAdminObj::~CInProcMsgSocketsAdminObj()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_bDestroying = true;

    if( m_mapSockets.size() > 0 )
    {
        CInProcMsgSocket* pSocket;

        QMap<unsigned int,CInProcMsgSocket*>::iterator itSockets = m_mapSockets.begin();

        while( itSockets != m_mapSockets.end() )
        {
            pSocket = itSockets.value();

            if( pSocket != nullptr )
            {
                try
                {
                    delete pSocket;
                }
                catch(...)
                {
                }
                pSocket = nullptr;
            }
            ++itSockets;
        }
    }
    m_mapSockets.clear();

    try
    {
        delete m_pMutex;
    }
    catch(...)
    {
    }

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pMutex = nullptr;
    //m_mapSockets;
    m_bDestroying = false;
    m_bTracingEnabled = false;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CInProcMsgSocket* CInProcMsgSocketsAdminObj::find( unsigned int i_uPort ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);

    CInProcMsgSocket* pSocket = nullptr;

    if( m_mapSockets.contains(i_uPort) )
    {
        pSocket = m_mapSockets[i_uPort];
    }
    return pSocket;

} // find

//------------------------------------------------------------------------------
QList<unsigned int> CInProcMsgSocketsAdminObj::getUsedPorts() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_mapSockets.keys();
}

//------------------------------------------------------------------------------
unsigned int CInProcMsgSocketsAdminObj::GetUniquePort() const
//------------------------------------------------------------------------------
{
    unsigned int uPort = 0;

    while( find(uPort) != nullptr )
    {
        uPort++;
    }
    return uPort;

} // GetUniquePort

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CInProcMsgSocketsAdminObj::onSocketCreated( CInProcMsgSocket* i_pSocket )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        if( i_pSocket == nullptr )
        {
            strAddTrcInfo = "Socket: nullptr";
        }
        else
        {
            strAddTrcInfo = "Socket: " + i_pSocket->objectName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onSocketCreated",
        /* strAddInfo   */ strAddTrcInfo );

    QMutexLocker mtxLocker(m_pMutex);

    if( i_pSocket == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "Socket == nullptr" );
    }
    if( m_mapSockets.contains(i_pSocket->getLocalPort()) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjAlreadyInList, "Socket with port number " + QString::number(i_pSocket->getLocalPort()) + " already existing." );
    }

    m_mapSockets.insert( i_pSocket->getLocalPort(), i_pSocket );

} // onSocketCreated

//------------------------------------------------------------------------------
void CInProcMsgSocketsAdminObj::onSocketDestroyed( CInProcMsgSocket* i_pSocket )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        if( i_pSocket == nullptr )
        {
            strAddTrcInfo = "Socket: nullptr";
        }
        else
        {
            strAddTrcInfo = "Socket: " + i_pSocket->objectName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onSocketDestroyed",
        /* strAddInfo   */ strAddTrcInfo );

    if( !m_bDestroying )
    {
        QMutexLocker mtxLocker(m_pMutex);

        if( i_pSocket == nullptr )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pSocket == nullptr" );
        }
        if( !m_mapSockets.contains(i_pSocket->getLocalPort()) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultObjNotInList, "Socket with port number " + QString::number(i_pSocket->getLocalPort()) + " not existing." );
        }

        m_mapSockets.remove( i_pSocket->getLocalPort() );

    } // if( !m_bDestroying )

} // onSocketDestroyed


/*******************************************************************************
class CInProcMsgSocket : public QThread
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CInProcMsgSocketsAdminObj* CInProcMsgSocket::GetAdminObjInstance()
//------------------------------------------------------------------------------
{
    return s_pInProcMsgSocketsAdminObj;
}

//------------------------------------------------------------------------------
CInProcMsgSocket* CInProcMsgSocket::Find( unsigned int i_uPort )
//------------------------------------------------------------------------------
{
    CInProcMsgSocket* pSocket = nullptr;

    if( s_pInProcMsgSocketsAdminObj != nullptr )
    {
        pSocket = s_pInProcMsgSocketsAdminObj->find(i_uPort);
    }
    return pSocket;

} // find

//------------------------------------------------------------------------------
QList<unsigned int> CInProcMsgSocket::GetUsedPorts()
//------------------------------------------------------------------------------
{
    QList<unsigned int> aruPorts;

    if( s_pInProcMsgSocketsAdminObj != nullptr )
    {
        aruPorts = s_pInProcMsgSocketsAdminObj->getUsedPorts();
    }
    return aruPorts;

} // getUsedPorts

//------------------------------------------------------------------------------
unsigned int CInProcMsgSocket::GetUniquePort()
//------------------------------------------------------------------------------
{
    unsigned int uPort = 0;

    if( s_pInProcMsgSocketsAdminObj != nullptr )
    {
        uPort = s_pInProcMsgSocketsAdminObj->GetUniquePort();
    }
    return uPort;

} // GetUniquePort

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CInProcMsgSocket::CInProcMsgSocket(
    const QString& i_strObjName,
    ESrvCltType    i_srvCltType,
    bool           i_bTracingEnabled ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pMutex(nullptr),
    m_srvCltType(i_srvCltType),
    m_pInProcMsgServer(nullptr),
    m_pInProcMsgSocketPeer(nullptr),
    m_socketState(ESocketStateUnconnected),
    m_iReqMsgId(-1),
    m_uServerListenPort(0),
    m_strLocalHostName("127.0.0.1"),
    m_uLocalPort(0),
    m_strRemoteHostName("127.0.0.1"),
    m_uRemotePort(0),
    m_pTimer(nullptr),
    m_bOnTimerTimeoutConnected(false),
    m_fTimerStartTime_ms(0.0),
    m_iConnectTimeout_ms(-1),
    m_iDisconnectTimeout_ms(-1),
    m_errResultInfo(),
    m_copyDepthMsgReadBuff(ECopyDepth::FlatReleaseOwnwership),
    m_arpMsgReadBuff(),
    m_bTracingEnabled(i_bTracingEnabled),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    if( m_bTracingEnabled && !i_strObjName.contains("TrcServer") && !i_strObjName.contains("TrcClient") )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Ipc", "CInProcMsgSocket", i_strObjName);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    #pragma push_macro("_ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE")
    #ifdef _WINDOWS
    #pragma warning( disable : 4005 )
    #endif
    #define _ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE 0

    if( s_pInProcMsgSocketsAdminObj == nullptr )
    {
        s_pInProcMsgSocketsAdminObj = new CInProcMsgSocketsAdminObj();
    }

    #ifdef _WINDOWS
    #pragma warning( default : 4005 )
    #endif
    #pragma pop_macro("_ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE")

    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    m_pMutex = new QRecursiveMutex();
    #else
    m_pMutex = new QMutex(QMutex::Recursive);
    #endif
    m_pTimer = new QTimer(this);

    m_uLocalPort = GetUniquePort();

    s_pInProcMsgSocketsAdminObj->onSocketCreated(this);

} // ctor

//------------------------------------------------------------------------------
CInProcMsgSocket::CInProcMsgSocket(
    const QString&    i_strObjName,
    CInProcMsgServer* i_pServer,
    CInProcMsgSocket* i_pSocketClient,
    bool              i_bTracingEnabled ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pMutex(nullptr),
    m_srvCltType(ESrvCltTypeServer),
    m_pInProcMsgServer(i_pServer),
    m_pInProcMsgSocketPeer(i_pSocketClient),
    m_socketState(ESocketStateUnconnected),
    m_iReqMsgId(-1),
    m_uServerListenPort(0),
    m_strLocalHostName("127.0.0.1"),
    m_uLocalPort(0),
    m_strRemoteHostName("127.0.0.1"),
    m_uRemotePort(0),
    m_pTimer(nullptr),
    m_bOnTimerTimeoutConnected(false),
    m_fTimerStartTime_ms(0.0),
    m_iConnectTimeout_ms(-1),
    m_iDisconnectTimeout_ms(-1),
    m_errResultInfo(),
    m_copyDepthMsgReadBuff(ECopyDepth::FlatReleaseOwnwership),
    m_arpMsgReadBuff(),
    m_bTracingEnabled(i_bTracingEnabled),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    if( m_bTracingEnabled && !i_strObjName.contains("TrcServer") && !i_strObjName.contains("TrcClient") )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Ipc", "CInProcMsgSocket", i_strObjName);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    if( s_pInProcMsgSocketsAdminObj == nullptr )
    {
        s_pInProcMsgSocketsAdminObj = new CInProcMsgSocketsAdminObj();
    }

    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    m_pMutex = new QRecursiveMutex();
    #else
    m_pMutex = new QMutex(QMutex::Recursive);
    #endif
    m_pTimer = new QTimer(this);

    if( i_pServer->getPort() != i_pSocketClient->getServerListenPort() )
    {
        QString strAddErrInfo;
        strAddErrInfo  = "Server's Listening Port " + QString::number(i_pServer->getPort());
        strAddErrInfo += " is different from Socket's Remote Server Port " + QString::number(i_pSocketClient->getServerListenPort());
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, strAddErrInfo );
    }

    m_uLocalPort = GetUniquePort();

    QObject::connect(
        m_pInProcMsgSocketPeer, &QObject::destroyed,
        this, &CInProcMsgSocket::onSocketPeerDestroyed);

    s_pInProcMsgSocketsAdminObj->onSocketCreated(this);

} // ctor

//------------------------------------------------------------------------------
CInProcMsgSocket::~CInProcMsgSocket()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    s_pInProcMsgSocketsAdminObj->onSocketDestroyed(this);

    if( m_arpMsgReadBuff.size() > 0 )
    {
        CMsg* pMsg;
        int   idxMsg;

        for( idxMsg = m_arpMsgReadBuff.size()-1; idxMsg >= 0; idxMsg-- )
        {
            pMsg = m_arpMsgReadBuff[idxMsg];
            m_arpMsgReadBuff[idxMsg] = nullptr;

            try
            {
                delete pMsg;
            }
            catch(...)
            {
            }
            pMsg = nullptr;
        }
        m_arpMsgReadBuff.clear();
    }

    try
    {
        delete m_pMutex;
    }
    catch(...)
    {
    }

    m_pMutex = nullptr;
    m_pInProcMsgServer = nullptr;
    m_pInProcMsgSocketPeer = nullptr;
    m_pTimer = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
ECopyDepth CInProcMsgSocket::getMsgReadBuffCopyDepth() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_copyDepthMsgReadBuff;
}

//------------------------------------------------------------------------------
void CInProcMsgSocket::setMsgReadBuffCopyDepth( ECopyDepth i_copyDepth )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strAddTrcInfo = CEnumCopyDepth::toString(i_copyDepth);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMsgReadBuffCopyDepth",
        /* strAddInfo   */ strAddTrcInfo );

    QMutexLocker mtxLocker(m_pMutex);
    m_copyDepthMsgReadBuff = i_copyDepth;

} // setMsgReadBuffCopyDepth

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
ESrvCltType CInProcMsgSocket::getSrvCltType() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_srvCltType;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
unsigned int CInProcMsgSocket::getServerListenPort() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_uServerListenPort;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CInProcMsgSocket::setLocalHostName( const QString& i_strLocalHostName )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strAddTrcInfo = i_strLocalHostName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setLocalHostName",
        /* strAddInfo   */ strAddTrcInfo );

    QMutexLocker mtxLocker(m_pMutex);
    m_strLocalHostName = i_strLocalHostName;

} // setLocalHostName

//------------------------------------------------------------------------------
QString CInProcMsgSocket::getLocalHostName() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_strLocalHostName;
}

//------------------------------------------------------------------------------
unsigned int CInProcMsgSocket::getLocalPort() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_uLocalPort;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CInProcMsgSocket::getRemoteHostName() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_strRemoteHostName;
}

//------------------------------------------------------------------------------
unsigned int CInProcMsgSocket::getRemotePort() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_uRemotePort;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
ESocketState CInProcMsgSocket::state() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_socketState;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CInProcMsgSocket::connectToServer(
    const QString& i_strRemoteHostName,
    unsigned int   i_uRemotePort,
    int            i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strAddTrcInfo  = i_strRemoteHostName + ":" + QString::number(i_uRemotePort);
        strAddTrcInfo += ", Timeout: " + QString::number(i_iTimeout_ms);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "connectToServer",
        /* strAddInfo   */ strAddTrcInfo );

    QMutexLocker mtxLocker(m_pMutex);

    if( m_srvCltType != ESrvCltTypeClient )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Calling connectToServer is only valid for sockets on client's side" );
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("connectToServer");

    if( m_socketState != ESocketStateUnconnected )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInternalStateMachineError);
        errResultInfo.setAddErrInfoDscr( "ConnectToServer called in state " + socketState2Str(m_socketState) );
    }
    else if( m_pInProcMsgSocketPeer != nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInternalStateMachineError);
        errResultInfo.setAddErrInfoDscr( "ConnectToServer called while socket is already connected with " + QString::number(m_pInProcMsgSocketPeer->getServerListenPort()) );
    }

    if( !errResultInfo.isErrorResult() )
    {
        if( m_pInProcMsgServer != nullptr )
        {
            QObject::disconnect(
                m_pInProcMsgServer, &QObject::destroyed,
                this, &CInProcMsgSocket::onServerDestroyed);
            m_pInProcMsgServer = nullptr;
        }

        m_strRemoteHostName = i_strRemoteHostName;
        m_uServerListenPort = i_uRemotePort;
        m_iConnectTimeout_ms = i_iTimeout_ms;

        m_socketState = ESocketStateConnecting;
        m_iReqMsgId = -1;

        m_pInProcMsgServer = CInProcMsgServer::Find(m_uServerListenPort);

        if( m_pInProcMsgServer != nullptr )
        {
            QObject::connect(
                m_pInProcMsgServer, &QObject::destroyed,
                this, &CInProcMsgSocket::onServerDestroyed,
                Qt::DirectConnection);

            SSocketDscr socketDscr(
                /* strRemoteHostName */ m_strLocalHostName,     // the server's IP address (same as client's address for InProcMsg)
                /* uServerListenPort */ m_uServerListenPort,    // the server's listen port
                /* socketType        */ ESocketTypeInProcMsg,
                /* srvCltType        */ ESrvCltTypeClient );

            socketDscr.m_socketState = ESocketStateConnecting;
            socketDscr.m_iConnectTimeout_ms = m_iConnectTimeout_ms;
            socketDscr.m_strLocalHostName = m_strLocalHostName;
            socketDscr.m_uLocalPort = m_uLocalPort;
            socketDscr.m_strRemoteHostName = m_strRemoteHostName;
            //socketDscr.m_uRemotePort = 0; must be returned by confirmation message

            CMsgReqConnect* pMsgReq = new CMsgReqConnect(
                /* pObjSender       */ this,
                /* pObjReceiver     */ m_pInProcMsgServer,
                /* socketDscr       */ socketDscr,
                /* watchDogSettings */ STimerSettings(),
                /* pBlkType         */ nullptr,
                /* bMustBeConfirmed */ true,
                /* iReqId           */ -1 );
            m_iReqMsgId = pMsgReq->getMsgId();
            POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ELogDetailLevel::Debug);
            pMsgReq = nullptr;

        } // if( m_pInProcMsgServer != nullptr )

        // As timers cannot be started from another thread and
        // this method may have been called from another thread:
        CMsgReqStartTimer* pMsgReqStartTimer = new CMsgReqStartTimer(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* iTimerId     */ -1,
            /* bSingleShot  */ false,
            /* iInterval_ms */ -1 );
        POST_OR_DELETE_MESSAGE(pMsgReqStartTimer, &mthTracer, ELogDetailLevel::Debug);
        pMsgReqStartTimer = nullptr;

    } // if( !errResultInfo.isErrorResult() )

    if( errResultInfo.isErrorResult() )
    {
        m_errResultInfo = errResultInfo;
    }

} // connectToServer

//------------------------------------------------------------------------------
void CInProcMsgSocket::disconnectFromServer( int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strAddTrcInfo = "Timeout: " + QString::number(i_iTimeout_ms);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "disconnectFromServer",
        /* strAddInfo   */ strAddTrcInfo );

    QMutexLocker mtxLocker(m_pMutex);

    if( m_srvCltType != ESrvCltTypeClient )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Calling disconnectFromServer is only valid for sockets on client's side" );
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("disconnectFromServer");

    if( m_socketState != ESocketStateConnected )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInternalStateMachineError);
        errResultInfo.setAddErrInfoDscr( "DisconnectFromServer called in state " + socketState2Str(m_socketState) );
    }
    else if( m_pInProcMsgServer == nullptr || m_pInProcMsgSocketPeer == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInternalStateMachineError);
        errResultInfo.setAddErrInfoDscr( "DisconnectFromServer called while socket is not connected" );
    }

    if( !errResultInfo.isErrorResult() )
    {
        m_iDisconnectTimeout_ms = i_iTimeout_ms;

        m_socketState = ESocketStateClosing;
        m_iReqMsgId = -1;

        SSocketDscr socketDscr(
            /* strRemoteHostName */ m_strLocalHostName,     // the server's IP address (same as client's address for InProcMsg)
            /* uServerListenPort */ m_uServerListenPort,    // the server's listen port
            /* socketType        */ ESocketTypeInProcMsg,
            /* srvCltType        */ ESrvCltTypeClient );

        socketDscr.m_socketState = ESocketStateClosing;
        socketDscr.m_iConnectTimeout_ms = m_iDisconnectTimeout_ms;
        socketDscr.m_strLocalHostName = m_strLocalHostName;
        socketDscr.m_uLocalPort = m_uLocalPort;
        socketDscr.m_uRemotePort = m_uRemotePort; // must have been returned by connect confirmation message

        #ifdef _WINDOWS
        #pragma message(__TODO__"Use SocketPeer instead of Server as receiver of the disconnect message")
        #endif
        CMsgReqDisconnect* pMsgReq = new CMsgReqDisconnect(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pInProcMsgServer,
            /* socketDscr       */ socketDscr,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ -1 );
        m_iReqMsgId = pMsgReq->getMsgId();
        POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ELogDetailLevel::Debug);
        pMsgReq = nullptr;

        // As timers cannot be started from another thread and
        // this method may have been called from another thread:
        CMsgReqStartTimer* pMsgReqStartTimer = new CMsgReqStartTimer(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* iTimerId     */ -1,
            /* bSingleShot  */ false,
            /* iInterval_ms */ -1 );
        POST_OR_DELETE_MESSAGE(pMsgReqStartTimer, &mthTracer, ELogDetailLevel::Debug);
        pMsgReqStartTimer = nullptr;

    } // if( !errResultInfo.isErrorResult() )

    if( errResultInfo.isErrorResult() )
    {
        m_socketState = ESocketStateUnconnected;
        m_iReqMsgId = -1;
        m_errResultInfo = errResultInfo;
        emit error( m_errResultInfo.getResult() );
        // No reentry on emitting error changing the current state ..
        if( m_socketState == ESocketStateUnconnected )
        {
            emit disconnected();
        }
    }

} // disconnectFromServer

//------------------------------------------------------------------------------
void CInProcMsgSocket::abort()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "abort",
        /* strAddInfo   */ "" );

    QMutexLocker mtxLocker(m_pMutex);

    if( m_bOnTimerTimeoutConnected )
    {
        QObject::disconnect(
            m_pTimer, &QTimer::timeout,
            this, &CInProcMsgSocket::onTimerTimeout);
        m_bOnTimerTimeoutConnected = false;
    }

    if( m_pTimer->isActive() )
    {
        // As timers cannot be started and stopped from another thread and
        // this method may have been called from another thread:
        CMsgReqStopTimer* pMsgReqStopTimer = new CMsgReqStopTimer(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* iTimerId     */ -1 );
        POST_OR_DELETE_MESSAGE(pMsgReqStopTimer, &mthTracer, ELogDetailLevel::Debug);
        pMsgReqStopTimer = nullptr;
    }

    ESocketState socketStatePrev = m_socketState;

    m_socketState = ESocketStateUnconnected;
    m_iReqMsgId = -1;

    // No reentry on emitting error changing the current state ..
    if( socketStatePrev == ESocketStateConnected )
    {
        emit disconnected();
    }

} // abort

//------------------------------------------------------------------------------
int CInProcMsgSocket::messagesAvailable() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_arpMsgReadBuff.size();
}

//------------------------------------------------------------------------------
void CInProcMsgSocket::writeMessage( CMsg* i_pMsg )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        if( i_pMsg == nullptr )
        {
            strAddTrcInfo = "nullptr";
        }
        else if( m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsDetailed) )
        {
            strAddTrcInfo = i_pMsg->getAddTrcInfoStr();
        }
        else
        {
            strAddTrcInfo = i_pMsg->msgTypeToStr();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "writeMessage",
        /* strAddInfo   */ strAddTrcInfo );

    QMutexLocker mtxLocker(m_pMutex);

    if( m_pInProcMsgSocketPeer != nullptr )
    {
        i_pMsg->setReceiver(m_pInProcMsgSocketPeer);

        POST_OR_DELETE_MESSAGE(i_pMsg, &mthTracer, ELogDetailLevel::Debug);
    }
    else
    {
        delete i_pMsg;
        i_pMsg = nullptr;
    }

} // writeMessage

//------------------------------------------------------------------------------
CMsg* CInProcMsgSocket::readMessage()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "readMessage",
        /* strAddInfo   */ strAddTrcInfo );

    QMutexLocker mtxLocker(m_pMutex);

    CMsg* pMsg = nullptr;

    if( !m_arpMsgReadBuff.isEmpty() )
    {
        pMsg = m_arpMsgReadBuff.takeFirst();
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        if( pMsg == nullptr )
        {
            strAddTrcInfo = "nullptr";
        }
        else if( m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsDetailed) )
        {
            strAddTrcInfo = pMsg->getAddTrcInfoStr();
        }
        else
        {
            strAddTrcInfo = pMsg->msgTypeToStr();
        }
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pMsg;

} // readMessage

//------------------------------------------------------------------------------
ZS::System::EResult CInProcMsgSocket::error() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_errResultInfo.getResult();
}

//------------------------------------------------------------------------------
QString CInProcMsgSocket::errorString() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_errResultInfo.getResultStr();
}

//------------------------------------------------------------------------------
SErrResultInfo CInProcMsgSocket::errResultInfo() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_errResultInfo;
}

/*==============================================================================
protected: // overridables of inherited class QObject (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
bool CInProcMsgSocket::event( QEvent* i_pMsg )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);

    bool bEventHandled = false;

    CMsg* pMsg = dynamic_cast<CMsg*>(i_pMsg);

    if( pMsg != nullptr )
    {
        bEventHandled = true;

        QString strAddTrcInfo;

        if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
        {
            if( pMsg == nullptr )
            {
                strAddTrcInfo = "nullptr";
            }
            else if( m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsDetailed) )
            {
                strAddTrcInfo = pMsg->getAddTrcInfoStr();
            }
            else
            {
                strAddTrcInfo = pMsg->msgTypeToStr();
            }
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strMethod    */ "event",
            /* strAddInfo   */ strAddTrcInfo );

        QString strAddErrInfo;

        if( pMsg->isBaseMsgType() )
        {
            switch( pMsg->getMsgType() )
            {
                case ZS::System::EBaseMsgTypeIndErr:
                {
                    CMsgErr* pMsgErr = dynamic_cast<CMsgErr*>(pMsg);
                    if( pMsgErr == nullptr )
                    {
                        strAddErrInfo  = "Msg: " + msgType2Str(pMsg->getMsgType());
                        strAddErrInfo += ", Id: " + QString::number(pMsg->getMsgId());
                        throw CException( __FILE__, __LINE__, EResultMessageTypeMismatch, strAddErrInfo );
                    }
                    m_errResultInfo = pMsgErr->getErrResultInfo();
                    emit error( m_errResultInfo.getResult() );
                    break;
                }
                case ZS::System::EBaseMsgTypeReqStartTimer:
                {
                    CMsgReqStartTimer* pMsgReq = dynamic_cast<CMsgReqStartTimer*>(i_pMsg);
                    if( pMsgReq == nullptr )
                    {
                        strAddErrInfo  = "Msg: " + msgType2Str(pMsg->getMsgType());
                        strAddErrInfo += ", Id: " + QString::number(pMsg->getMsgId());
                        throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch );
                    }
                    if( m_pTimer->isActive() )
                    {
                        m_pTimer->stop();
                    }
                    if( !m_bOnTimerTimeoutConnected )
                    {
                        m_bOnTimerTimeoutConnected = QObject::connect(
                            m_pTimer, &QTimer::timeout,
                            this, &CInProcMsgSocket::onTimerTimeout);
                        if( !m_bOnTimerTimeoutConnected ) {
                            throw CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                        }
                        m_fTimerStartTime_ms = Time::getProcTimeInMilliSec();
                        m_pTimer->start(100);
                    }
                    if( pMsgReq->mustBeConfirmed() )
                    {
                        CMsgConStartTimer* pMsgCon = new CMsgConStartTimer(
                            /* pObjSender    */ this,
                            /* pObjReceiver  */ pMsgReq->getSender(),
                            /* iTimerId      */ pMsgReq->getTimerId(),
                            /* iMsgIdReq     */ pMsgReq->getMsgId(),
                            /* errResultInfo */ SErrResultInfo() );
                        POST_OR_DELETE_MESSAGE(pMsgCon, &mthTracer, ELogDetailLevel::Debug);
                        pMsgCon = nullptr;
                    }
                    break;
                }
                case ZS::System::EBaseMsgTypeReqStopTimer:
                {
                    CMsgReqStopTimer* pMsgReq = dynamic_cast<CMsgReqStopTimer*>(i_pMsg);
                    if( pMsgReq == nullptr )
                    {
                        strAddErrInfo  = "Msg: " + msgType2Str(pMsg->getMsgType());
                        strAddErrInfo += ", Id: " + QString::number(pMsg->getMsgId());
                        throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch );
                    }
                    if( m_pTimer->isActive() )
                    {
                        m_pTimer->stop();
                    }
                    if( m_bOnTimerTimeoutConnected )
                    {
                        QObject::disconnect(
                            m_pTimer, &QTimer::timeout,
                            this, &CInProcMsgSocket::onTimerTimeout);
                        m_bOnTimerTimeoutConnected = false;
                    }
                    if( pMsgReq->mustBeConfirmed() )
                    {
                        CMsgConStopTimer* pMsgCon = new CMsgConStopTimer(
                            /* pObjSender    */ this,
                            /* pObjReceiver  */ pMsgReq->getSender(),
                            /* iTimerId      */ pMsgReq->getTimerId(),
                            /* iMsgIdReq     */ pMsgReq->getMsgId(),
                            /* errResultInfo */ SErrResultInfo() );
                        POST_OR_DELETE_MESSAGE(pMsgCon, &mthTracer, ELogDetailLevel::Debug);
                        pMsgCon = nullptr;
                    }
                    break;
                }
                default:
                {
                    strAddErrInfo  = "Msg: " + msgType2Str(pMsg->getMsgType());
                    strAddErrInfo += ", Id: " + QString::number(pMsg->getMsgId());
                    throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMessage, strAddErrInfo );
                }
            } // switch( pMsg->getMsgType() )
        } // if( pMsg->isBaseMsgType() )

        else // if( !pMsg->isBaseMsgType() )
        {
            switch( static_cast<int>(i_pMsg->type()) )
            {
                //------------------------------------------------------------------
                case EMsgTypeConConnect:
                //------------------------------------------------------------------
                {
                    CMsgConConnect* pMsgCon = dynamic_cast<CMsgConConnect*>(i_pMsg);
                    if( pMsgCon == nullptr )
                    {
                        strAddErrInfo  = "Msg: " + msgType2Str(pMsg->getMsgType());
                        strAddErrInfo += ", Id: " + QString::number(pMsg->getMsgId());
                        throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch );
                    }

                    SErrResultInfo errResultInfo = pMsgCon->getErrResultInfo();

                    SSocketDscr socketDscr = pMsgCon->getSocketDscr();

                    CInProcMsgServer* pInProcMsgServer = dynamic_cast<CInProcMsgServer*>(pMsgCon->getSender());
                    if( pInProcMsgServer == nullptr )
                    {
                        throw CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "ConConnect.Sender() != InProcMsgServer" );
                    }

                    if( pInProcMsgServer != m_pInProcMsgServer )
                    {
                        throw CException( __FILE__, __LINE__, EResultInvalidSenderObjInMessage );
                    }

                    switch( m_socketState )
                    {
                        case ESocketStateConnecting:
                        {
                            if( m_pTimer->isActive() )
                            {
                                m_pTimer->stop();
                            }
                            if( m_bOnTimerTimeoutConnected )
                            {
                                QObject::disconnect(
                                    m_pTimer, &QTimer::timeout,
                                    this, &CInProcMsgSocket::onTimerTimeout);
                                m_bOnTimerTimeoutConnected = false;
                            }
                            if( errResultInfo.isErrorResult() )
                            {
                                m_socketState = ESocketStateUnconnected;
                                m_iReqMsgId = -1;
                                m_errResultInfo = errResultInfo;
                                emit error( m_errResultInfo.getResult() );
                            }
                            else if( m_iReqMsgId == pMsgCon->getMsgId() )
                            {
                                m_uRemotePort = socketDscr.m_uLocalPort;
                                m_socketState = socketDscr.m_socketState;
                                m_iReqMsgId = -1;
                                m_errResultInfo = errResultInfo;
                                m_pInProcMsgSocketPeer = CInProcMsgSocket::Find(m_uRemotePort);
                                QObject::connect(
                                    m_pInProcMsgSocketPeer, &QObject::destroyed,
                                    this, &CInProcMsgSocket::onSocketPeerDestroyed);
                                emit connected();
                            }
                            break;
                        } // case ESocketStateConnecting:

                        case ESocketStateUnconnected:
                        case ESocketStateHostLookup:
                        case ESocketStateConnected:
                        case ESocketStateBound:
                        case ESocketStateListening:
                        case ESocketStateClosing:
                        default:
                        {
                            break;
                        }
                    } // switch( m_socketState )
                    break;
                } // case EMsgTypeConConnect

                //------------------------------------------------------------------
                case EMsgTypeConDisconnect:
                //------------------------------------------------------------------
                {
                    CMsgConDisconnect* pMsgCon = dynamic_cast<CMsgConDisconnect*>(i_pMsg);
                    if( pMsgCon == nullptr )
                    {
                        strAddErrInfo  = "Msg: " + msgType2Str(pMsg->getMsgType());
                        strAddErrInfo += ", Id: " + QString::number(pMsg->getMsgId());
                        throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch );
                    }

                    SErrResultInfo errResultInfo = pMsgCon->getErrResultInfo();

                    switch( m_socketState )
                    {
                        case ESocketStateClosing:
                        {
                            if( m_pTimer->isActive() )
                            {
                                m_pTimer->stop();
                            }
                            if( m_bOnTimerTimeoutConnected )
                            {
                                QObject::disconnect(
                                    m_pTimer, &QTimer::timeout,
                                    this, &CInProcMsgSocket::onTimerTimeout);
                                m_bOnTimerTimeoutConnected = false;
                            }
                            if( errResultInfo.isErrorResult() )
                            {
                                m_socketState = ESocketStateUnconnected;
                                m_iReqMsgId = -1;
                                m_errResultInfo = errResultInfo;
                                emit error( m_errResultInfo.getResult() );
                                // No reentry on emitting error changing the current state ..
                                if( m_socketState == ESocketStateUnconnected )
                                {
                                    emit disconnected();
                                }
                            }
                            else if( m_iReqMsgId == pMsgCon->getMsgId() )
                            {
                                m_socketState = ESocketStateUnconnected;
                                m_iReqMsgId = -1;
                                m_errResultInfo = errResultInfo;
                                emit disconnected();
                            }
                            break;
                        } // case ESocketStateClosing:

                        case ESocketStateUnconnected:
                        case ESocketStateHostLookup:
                        case ESocketStateConnecting:
                        case ESocketStateConnected:
                        case ESocketStateBound:
                        case ESocketStateListening:
                        default:
                        {
                            break;
                        }
                    } // switch( m_socketState )
                    break;
                } // case EMsgTypeConDisconnect

                //------------------------------------------------------------------
                default:
                //------------------------------------------------------------------
                {
                    m_arpMsgReadBuff.append(pMsg->clone(m_copyDepthMsgReadBuff));
                    emit readyRead();
                    break;
                }

            } // switch( i_pMsg->type() )

        } // if( !pMsg->isBaseMsgType() )

    } // if( pMsg != nullptr )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pMsg);
    }
    return bEventHandled;

} // event

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CInProcMsgSocket::onTimerTimeout()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);

    bool bStopTimer = false;
    bool bEmitError = false;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::ArgsDetailed,
        /* strMethod    */ "onTimerTimeout",
        /* strAddInfo   */ "" );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("onTimerTimeout");

    switch( m_socketState )
    {
        case ESocketStateConnecting:
        {
            if( m_iConnectTimeout_ms > 0 )
            {
                double fTimeCurrent_ms = Time::getProcTimeInMilliSec();

                if( fTimeCurrent_ms - m_fTimerStartTime_ms > static_cast<double>(m_iConnectTimeout_ms) )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultTimeout);
                    errResultInfo.setAddErrInfoDscr( "ConnectToServer timed out" );
                }
            }

            if( !errResultInfo.isErrorResult() )
            {
                if( m_pInProcMsgServer == nullptr )
                {
                    m_pInProcMsgServer = CInProcMsgServer::Find(m_uServerListenPort);

                    if( m_pInProcMsgServer != nullptr )
                    {
                        SSocketDscr socketDscr(
                            /* strRemoteHostName */ m_strLocalHostName,     // the server's IP address (same as client's address for InProcMsg)
                            /* uServerListenPort */ m_uServerListenPort,    // the server's listen port
                            /* socketType        */ ESocketTypeInProcMsg,
                            /* srvCltType        */ ESrvCltTypeClient );

                        socketDscr.m_socketState = ESocketStateConnecting;
                        socketDscr.m_iConnectTimeout_ms = m_iConnectTimeout_ms;
                        socketDscr.m_strLocalHostName = m_strLocalHostName;
                        socketDscr.m_uLocalPort = m_uLocalPort;
                        //socketDscr.m_uRemotePort = 0; // must be returned by confirmation message

                        CMsgReqConnect* pMsgReq = new CMsgReqConnect(
                            /* pObjSender       */ this,
                            /* pObjReceiver     */ m_pInProcMsgServer,
                            /* socketDscr       */ socketDscr,
                            /* watchDogSettings */ STimerSettings(),
                            /* pBlkType         */ nullptr,
                            /* bMustBeConfirmed */ true,
                            /* iReqId           */ -1 );
                        POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ELogDetailLevel::Debug);
                        pMsgReq = nullptr;

                    } // if( m_pInProcMsgServer != nullptr )
                } // if( m_pInProcMsgServer == nullptr )
            }// if( !errResultInfo.isErrorResult() )

            if( errResultInfo.isErrorResult() )
            {
                m_socketState = ESocketStateUnconnected;
                m_iReqMsgId = -1;
                m_errResultInfo = errResultInfo;
                bStopTimer = true;
                bEmitError = true;
            }
            break;
        } // case ESocketStateConnecting:

        case ESocketStateClosing:
        {
            if( m_iDisconnectTimeout_ms > 0 )
            {
                double fTimeCurrent_ms = Time::getProcTimeInMilliSec();

                if( fTimeCurrent_ms - m_fTimerStartTime_ms > static_cast<double>(m_iDisconnectTimeout_ms) )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultTimeout);
                    errResultInfo.setAddErrInfoDscr( "DisconnectFromServer timed out" );
                }
            }

            if( errResultInfo.isErrorResult() )
            {
                m_socketState = ESocketStateUnconnected;
                m_iReqMsgId = -1;
                m_errResultInfo = errResultInfo;
                bStopTimer = true;
                bEmitError = true;
            }
            break;
        } // case ESocketStateClosing:

        case ESocketStateUnconnected:
        case ESocketStateHostLookup:
        case ESocketStateConnected:
        case ESocketStateBound:
        case ESocketStateListening:
        default:
        {
            bStopTimer = true;
            break;
        }
    } // switch( m_socketState )

    if( bStopTimer )
    {
        m_pTimer->stop();
        QObject::disconnect(
            m_pTimer, &QTimer::timeout,
            this, &CInProcMsgSocket::onTimerTimeout);
        m_bOnTimerTimeoutConnected = false;
    }

    if( bEmitError )
    {
        // On emitting signals methods of the instance may be called as reentries
        // changing the internal states of the instance. So the internal states
        // (instance members) got to be set before emitting signals.
        emit error( m_errResultInfo.getResult() );
    }

} // onTimerTimeout

//------------------------------------------------------------------------------
void CInProcMsgSocket::onServerDestroyed( QObject* /*i_pServer*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onServerDestroyed",
        /* strAddInfo   */ "" );

    QMutexLocker mtxLocker(m_pMutex);
    m_pInProcMsgServer = nullptr;
    emit disconnected();

} // onServerDestroyed

//------------------------------------------------------------------------------
void CInProcMsgSocket::onSocketPeerDestroyed( QObject* /*i_pSocketPeer*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onSocketPeerDestroyed",
        /* strAddInfo   */ "" );

    QMutexLocker mtxLocker(m_pMutex);
    m_pInProcMsgSocketPeer = nullptr;
    emit disconnected();

} // onSocketPeerDestroyed
