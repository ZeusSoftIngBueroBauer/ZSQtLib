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
#include <QtCore/qmutex.h>
#include <QtNetwork/qhostinfo.h>

#include "ZSIpc/ZSIpcInProcMsgServer.h"
#include "ZSIpc/ZSIpcInProcMsgSocket.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;


/*******************************************************************************
class CInProcMsgServersAdminObj : public QObject
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

// Should be protected class members.
// But python bindings generated with shiboken will not be compilable then.

CInProcMsgServersAdminObj* /*CInProcMsgServersAdminObj::*/s_pInProcMsgServersAdminObj = nullptr;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CInProcMsgServersAdminObj* CInProcMsgServersAdminObj::GetInstance()
//------------------------------------------------------------------------------
{
    return s_pInProcMsgServersAdminObj;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CInProcMsgServersAdminObj::CInProcMsgServersAdminObj( bool i_bTracingEnabled ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pMutex(nullptr),
    m_mapServers(),
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
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Ipc", "CInProcMsgServersAdminObj", "theAdminObj");
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
CInProcMsgServersAdminObj::~CInProcMsgServersAdminObj()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_bDestroying = true;

    if( m_mapServers.size() > 0 )
    {
        CInProcMsgServer* pServer;

        QMap<unsigned int,CInProcMsgServer*>::iterator itServers = m_mapServers.begin();

        while( itServers != m_mapServers.end() )
        {
            pServer = itServers.value();

            if( pServer != nullptr )
            {
                try
                {
                    delete pServer;
                }
                catch(...)
                {
                }
                pServer = nullptr;
            }
            ++itServers;
        }
    }
    m_mapServers.clear();

    try
    {
        delete m_pMutex;
    }
    catch(...)
    {
    }

    m_pMutex = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CInProcMsgServer* CInProcMsgServersAdminObj::find( unsigned int i_uPort ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);

    CInProcMsgServer* pServer = nullptr;

    if( m_mapServers.contains(i_uPort) )
    {
        pServer = m_mapServers[i_uPort];
    }
    return pServer;

} // find

//------------------------------------------------------------------------------
QList<unsigned int> CInProcMsgServersAdminObj::getUsedPorts() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_mapServers.keys();
}

//------------------------------------------------------------------------------
unsigned int CInProcMsgServersAdminObj::GetUniquePort() const
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
void CInProcMsgServersAdminObj::onServerListen( CInProcMsgServer* i_pServer )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        if( i_pServer == nullptr )
        {
            strAddTrcInfo = "Server: nullptr";
        }
        else
        {
            strAddTrcInfo = "Server: " + i_pServer->objectName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onServerListen",
        /* strAddInfo   */ strAddTrcInfo );

    QMutexLocker mtxLocker(m_pMutex);

    if( i_pServer == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "Server == nullptr" );
    }
    if( m_mapServers.contains(i_pServer->getPort()) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjAlreadyInList, "Server with port number " + QString::number(i_pServer->getPort()) + " already existing." );
    }

    m_mapServers.insert( i_pServer->getPort(), i_pServer );

} // onServerListen

//------------------------------------------------------------------------------
void CInProcMsgServersAdminObj::onServerClose( CInProcMsgServer* i_pServer )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        if( i_pServer == nullptr )
        {
            strAddTrcInfo = "Server: nullptr";
        }
        else
        {
            strAddTrcInfo = "Server: " + i_pServer->objectName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onServerClose",
        /* strAddInfo   */ "" );

    if( !m_bDestroying )
    {
        QMutexLocker mtxLocker(m_pMutex);

        if( i_pServer == nullptr )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pServer == nullptr" );
        }
        if( !m_mapServers.contains(i_pServer->getPort()) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultObjNotInList, "Server with port number " + QString::number(i_pServer->getPort()) + " not existing." );
        }

        m_mapServers.remove( i_pServer->getPort() );

    } // if( !m_bDestroying )

} // onServerClose


/*******************************************************************************
class CInProcMsgServer : public QObject
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CInProcMsgServersAdminObj* CInProcMsgServer::GetAdminObjInstance()
//------------------------------------------------------------------------------
{
    return s_pInProcMsgServersAdminObj;
}

//------------------------------------------------------------------------------
CInProcMsgServer* CInProcMsgServer::Find( unsigned int i_uPort )
//------------------------------------------------------------------------------
{
    CInProcMsgServer* pServer = nullptr;

    if( s_pInProcMsgServersAdminObj != nullptr )
    {
        pServer = s_pInProcMsgServersAdminObj->find(i_uPort);
    }
    return pServer;

} // find

//------------------------------------------------------------------------------
QList<unsigned int> CInProcMsgServer::GetUsedPorts()
//------------------------------------------------------------------------------
{
    QList<unsigned int> aruPorts;

    if( s_pInProcMsgServersAdminObj != nullptr )
    {
        aruPorts = s_pInProcMsgServersAdminObj->getUsedPorts();
    }
    return aruPorts;

} // getUsedPorts

//------------------------------------------------------------------------------
unsigned int CInProcMsgServer::GetUniquePort()
//------------------------------------------------------------------------------
{
    unsigned int uPort = 0;

    if( s_pInProcMsgServersAdminObj != nullptr )
    {
        uPort = s_pInProcMsgServersAdminObj->GetUniquePort();
    }
    return uPort;

} // GetUniquePort

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CInProcMsgServer::CInProcMsgServer( const QString& i_strObjName, bool i_bTracingEnabled ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pMutex(nullptr),
    m_socketState(ESocketStateUndefined),
    m_strHostName("127.0.0.1"),
    m_uPort(0),
    m_errResultInfo(),
    m_iMaxPendingConnections(30),
    m_arpPendingConnections(),
    m_copyDepthMsgReadBuff(ECopyDepth::FlatReleaseOwnwership),
    m_bTracingEnabled(i_bTracingEnabled),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    if( m_bTracingEnabled && !i_strObjName.contains("TrcServer") )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Ipc", "CInProcMsgServer", i_strObjName);
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

    if( s_pInProcMsgServersAdminObj == nullptr )
    {
        s_pInProcMsgServersAdminObj = new CInProcMsgServersAdminObj();
    }

    #ifdef _WINDOWS
    #pragma warning( default : 4005 )
    #endif
    #pragma pop_macro("_ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE")

    m_pMutex = new QRecursiveMutex();

    m_arpPendingConnections.reserve(m_iMaxPendingConnections);

} // ctor

//------------------------------------------------------------------------------
CInProcMsgServer::~CInProcMsgServer()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    close();

    try
    {
        delete m_pMutex;
    }
    catch(...)
    {
    }

    m_pMutex = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
ECopyDepth CInProcMsgServer::getMsgReadBuffCopyDepth() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_copyDepthMsgReadBuff;
}

//------------------------------------------------------------------------------
void CInProcMsgServer::setMsgReadBuffCopyDepth( ECopyDepth i_copyDepth )
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
        /* strAddInfo   */ "" );

    QMutexLocker mtxLocker(m_pMutex);
    m_copyDepthMsgReadBuff = i_copyDepth;

} // setMsgReadBuffCopyDepth

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
unsigned int CInProcMsgServer::getPort() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_uPort;
}

/*==============================================================================
public: // must overridables of base class CIpcServer
==============================================================================*/

//------------------------------------------------------------------------------
void CInProcMsgServer::setMaxPendingConnections( int i_iMaxConnections )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strAddTrcInfo = QString::number(i_iMaxConnections);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMaxPendingConnections",
        /* strAddInfo   */ strAddTrcInfo );

    QMutexLocker mtxLocker(m_pMutex);
    m_iMaxPendingConnections = i_iMaxConnections;

} // setMaxPendingConnections

//------------------------------------------------------------------------------
int CInProcMsgServer::maxPendingConnections() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_iMaxPendingConnections;
}

//------------------------------------------------------------------------------
bool CInProcMsgServer::listen( unsigned int i_uPort )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strAddTrcInfo = "Port: " + QString::number(i_uPort);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "listen",
        /* strAddInfo   */ strAddTrcInfo );

    QMutexLocker mtxLocker(m_pMutex);

    m_uPort = i_uPort;

    s_pInProcMsgServersAdminObj->onServerListen(this);

    m_socketState = ESocketStateListening;

    return true;

} // listen

//------------------------------------------------------------------------------
bool CInProcMsgServer::isListening() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_socketState == ESocketStateListening;
}

//------------------------------------------------------------------------------
void CInProcMsgServer::close()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "close",
        /* strAddInfo   */ "" );

    QMutexLocker mtxLocker(m_pMutex);

    if( m_socketState == ESocketStateListening )
    {
        s_pInProcMsgServersAdminObj->onServerClose(this);

        m_socketState = ESocketStateClosing;

        int idxConnection;

        for( idxConnection = 0; idxConnection < m_arpPendingConnections.size(); idxConnection++ )
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

        m_socketState = ESocketStateUndefined;

    } // if( m_socketState == ESocketStateListening )

} // close

//------------------------------------------------------------------------------
bool CInProcMsgServer::hasPendingConnections() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return (m_arpPendingConnections.size() > 0);
}

//------------------------------------------------------------------------------
CInProcMsgSocket* CInProcMsgServer::nextPendingConnection()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "nextPendingConnection",
        /* strAddInfo   */ "" );

    QMutexLocker mtxLocker(m_pMutex);

    CInProcMsgSocket* pInProcMsgSocket = nullptr;
    int               idxConnection;

    // Search pending connection ...
    for( idxConnection = 0; idxConnection < m_arpPendingConnections.size(); idxConnection++ )
    {
        if( m_arpPendingConnections[idxConnection] != nullptr )
        {
            pInProcMsgSocket = m_arpPendingConnections[idxConnection];
            m_arpPendingConnections.remove(idxConnection);
            break;
        }
    }
    return pInProcMsgSocket;

} // nextPendingConnection

//------------------------------------------------------------------------------
ZS::System::EResult CInProcMsgServer::error() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_errResultInfo.getResult();
}

//------------------------------------------------------------------------------
QString CInProcMsgServer::errorString() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_errResultInfo.getResultStr();
}

//------------------------------------------------------------------------------
SErrResultInfo CInProcMsgServer::errResultInfo() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMutex);
    return m_errResultInfo;
}

/*==============================================================================
protected: // overridables of inherited class QObject (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
bool CInProcMsgServer::event( QEvent* i_pMsg )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    CMsg* pMsg = dynamic_cast<CMsg*>(i_pMsg);

    if( pMsg != nullptr )
    {
        bEventHandled = true;

        QMutexLocker mtxLocker(m_pMutex);

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
            if( pMsg->getMsgType() == ZS::System::EBaseMsgTypeIndErr )
            {
                CMsgErr* pMsgErr = dynamic_cast<CMsgErr*>(pMsg);
                if( pMsgErr == nullptr )
                {
                    strAddErrInfo  = "Msg: " + msgType2Str(pMsg->getMsgType());
                    strAddErrInfo += ", Id: " + QString::number(pMsg->getMsgId());
                    throw CException( __FILE__, __LINE__, EResultMessageTypeMismatch, strAddErrInfo );
                }
            }
        } // if( pMsg->isBaseMsgType() )

        else // if( !pMsg->isBaseMsgType() )
        {
            SErrResultInfo errResultInfo = ErrResultInfoSuccess( pMsg->msgTypeToStr() );

            switch( static_cast<int>(i_pMsg->type()) )
            {
                case EMsgTypeReqConnect:
                {
                    CMsgReqConnect* pMsgReq = dynamic_cast<CMsgReqConnect*>(i_pMsg);
                    if( pMsgReq == nullptr )
                    {
                        strAddErrInfo  = "Msg: " + msgType2Str(pMsg->getMsgType());
                        strAddErrInfo += ", Id: " + QString::number(pMsg->getMsgId());
                        throw CException( __FILE__, __LINE__, EResultMessageTypeMismatch, strAddErrInfo );
                    }

                    SSocketDscr socketDscrClient = pMsgReq->getHostSettings().getSocketDscr();

                    CInProcMsgSocket* pInProcMsgSocketClient = dynamic_cast<CInProcMsgSocket*>(pMsgReq->getSender());
                    if( pInProcMsgSocketClient == nullptr )
                    {
                        throw CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "ReqConnect.Sender() != InProcMsgSocket" );
                    }

                    // Both ways should work (dynamic_cast of sender and find with port number).
                    pInProcMsgSocketClient = CInProcMsgSocket::Find(socketDscrClient.m_uLocalPort);

                    // If the client's socket has not been destroyed in the meantime ..
                    if( pInProcMsgSocketClient != nullptr )
                    {
                        SSocketDscr socketDscrServer(
                            /* uServerListenPort */ socketDscrClient.m_uServerListenPort,  // the server's (my) listen port
                            /* socketType        */ ESocketTypeInProcMsg,
                            /* srvCltType        */ ESrvCltTypeServer );

                        socketDscrServer.m_strLocalHostName = m_strHostName;
                        //socketDscrServer.m_uLocalPort = 0; assigned below on creating the socket
                        socketDscrServer.m_strRemoteHostName = socketDscrClient.m_strLocalHostName;
                        socketDscrServer.m_uRemotePort = socketDscrClient.m_uLocalPort;

                        CInProcMsgSocket* pInProcMsgSocketServer = nullptr;

                        if( m_socketState != ESocketStateListening )
                        {
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultHostNotFound);
                            errResultInfo.setAddErrInfoDscr( "Server is not listening" );
                        }
                        else if( socketDscrClient.m_uServerListenPort != m_uPort )
                        {
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultHostNotFound);
                            errResultInfo.setAddErrInfoDscr( "Server is not listening on port " + QString::number(socketDscrClient.m_uServerListenPort) );
                        }
                        else if( m_arpPendingConnections.size() >= m_iMaxPendingConnections )
                        {
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultMaxConnectionsExceeded);
                            errResultInfo.setAddErrInfoDscr( "Pending connections: " + QString::number(m_arpPendingConnections.size()) + " (max: " + QString::number(m_iMaxPendingConnections) + ")" );
                        }
                        else
                        {
                            QString strSocketName = objectName() + "-" + socketDscrClient.m_strLocalHostName;
                            pInProcMsgSocketServer = new CInProcMsgSocket( strSocketName, this, pInProcMsgSocketClient, m_bTracingEnabled );

                            pInProcMsgSocketServer->m_socketState = ESocketStateConnected;
                            pInProcMsgSocketServer->m_uServerListenPort = socketDscrClient.m_uServerListenPort;  // the server's (my) listen port
                            pInProcMsgSocketServer->m_strRemoteHostName = socketDscrClient.m_strLocalHostName;
                            pInProcMsgSocketServer->m_uRemotePort = socketDscrClient.m_uLocalPort;

                            m_arpPendingConnections.append(pInProcMsgSocketServer);
                            emit newConnection();

                            socketDscrServer.m_socketState = ESocketStateConnected;
                            socketDscrServer.m_uLocalPort  = pInProcMsgSocketServer->getLocalPort();
                        }

                        CMsgConConnect* pMsgCon = new CMsgConConnect(
                            /* pObjSender         */ this,
                            /* pObjReceiver       */ pInProcMsgSocketClient,
                            /* socketDscr         */ socketDscrServer,
                            /* iReqId             */ pMsgReq->getRequestId(),
                            /* iMsgIdReq          */ pMsgReq->getMsgId(),
                            /* errResultInfo      */ errResultInfo,
                            /* iProgressInPerCent */ 100 );
                        POST_OR_DELETE_MESSAGE(pMsgCon, &mthTracer, ELogDetailLevel::Debug);
                        pMsgCon = nullptr;

                    } // if( pInProcMsgSocketClient != nullptr )
                    break;
                } // case EMsgTypeReqConnect

                case EMsgTypeReqDisconnect:
                {
                    CMsgReqDisconnect* pMsgReq = dynamic_cast<CMsgReqDisconnect*>(i_pMsg);
                    if( pMsgReq == nullptr )
                    {
                        strAddErrInfo  = "Msg: " + msgType2Str(pMsg->getMsgType());
                        strAddErrInfo += ", Id: " + QString::number(pMsg->getMsgId());
                        throw CException( __FILE__, __LINE__, EResultMessageTypeMismatch, strAddErrInfo );
                    }

                    SSocketDscr socketDscrClient = pMsgReq->getSocketDscr();

                    CInProcMsgSocket* pInProcMsgSocketClient = dynamic_cast<CInProcMsgSocket*>(pMsgReq->getSender());
                    if( pInProcMsgSocketClient == nullptr )
                    {
                        throw CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "ReqConnect.Sender() != InProcMsgSocket" );
                    }

                    // Both ways should work (dynamic_cast of receiver and find with port number).
                    pInProcMsgSocketClient = CInProcMsgSocket::Find(socketDscrClient.m_uLocalPort);

                    CInProcMsgSocket* pInProcMsgSocketServer = CInProcMsgSocket::Find(socketDscrClient.m_uRemotePort);

                    // If neither the client's socket not the server's socket has not been destroyed in the meantime ..
                    if( pInProcMsgSocketClient != nullptr && pInProcMsgSocketServer != nullptr )
                    {
                        SSocketDscr socketDscrServer(
                            /* uServerListenPort */ socketDscrClient.m_uServerListenPort,  // the server's (my) listen port
                            /* socketType        */ ESocketTypeInProcMsg,
                            /* srvCltType        */ ESrvCltTypeServer );

                        socketDscrServer.m_strLocalHostName = m_strHostName;
                        //socketDscrServer.m_uLocalPort = 0; assigned below on creating the socket
                        socketDscrServer.m_strRemoteHostName = socketDscrClient.m_strLocalHostName;
                        socketDscrServer.m_uRemotePort = socketDscrClient.m_uLocalPort;

                        if( m_socketState != ESocketStateListening )
                        {
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultHostNotFound);
                            errResultInfo.setAddErrInfoDscr( "Server is not listening" );
                        }
                        else if( socketDscrClient.m_uServerListenPort != m_uPort )
                        {
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultHostNotFound);
                            errResultInfo.setAddErrInfoDscr( "Server is not listening on port " + QString::number(socketDscrClient.m_uServerListenPort) );
                        }
                        else
                        {
                            socketDscrServer.m_socketState = ESocketStateUnconnected;
                            socketDscrServer.m_uLocalPort  = pInProcMsgSocketServer->getLocalPort();

                            delete pInProcMsgSocketServer;
                            pInProcMsgSocketServer = nullptr;
                        }

                        CMsgConDisconnect* pMsgCon = new CMsgConDisconnect(
                            /* pObjSender         */ this,
                            /* pObjReceiver       */ pInProcMsgSocketClient,
                            /* socketDscr         */ socketDscrServer,
                            /* iReqId             */ pMsgReq->getRequestId(),
                            /* iMsgIdReq          */ pMsgReq->getMsgId(),
                            /* errResultInfo      */ errResultInfo,
                            /* iProgressInPerCent */ 100 );
                        POST_OR_DELETE_MESSAGE(pMsgCon, &mthTracer, ELogDetailLevel::Debug);
                        pMsgCon = nullptr;

                    } // if( pInProcMsgSocketClient != nullptr )
                    break;
                } // case EMsgTypeReqDisconnect

                //------------------------------------------------------------------
                default:
                //------------------------------------------------------------------
                {
                    strAddErrInfo  = "Msg: " + msgType2Str(pMsg->getMsgType());
                    strAddErrInfo += ", Id: " + QString::number(pMsg->getMsgId());
                    throw CException( __FILE__, __LINE__, EResultInvalidMessage, strAddErrInfo );
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
