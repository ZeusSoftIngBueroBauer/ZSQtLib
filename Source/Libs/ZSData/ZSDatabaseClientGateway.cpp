/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/qtimer.h>
#include <QtNetwork/qhostinfo.h>

#include "ZSData/ZSDatabaseClientGateway.h"
#include "ZSData/ZSDatabaseClient.h"
#include "ZSData/ZSDataMsg.h"
#include "ZSIpc/ZSIpcSocketWrapperShm.h"
#include "ZSIpc/ZSIpcSocketWrapperTcp.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysSleeperThread.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Data;


/*******************************************************************************
class CDbClientGateway : public QObject
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

/*------------------------------------------------------------------------------
enum ERequest
------------------------------------------------------------------------------*/

const SEnumEntry s_arstrConnectionGatewayRequests[CDbClientGateway::ERequestCount] = {
    /* 0: */ SEnumEntry( CDbClientGateway::ERequestNone,               "None"               ),
    /* 1: */ SEnumEntry( CDbClientGateway::ERequestStartup,            "Startup"            ),
    /* 2: */ SEnumEntry( CDbClientGateway::ERequestShutdown,           "Shutdown"           ),
    /* 3: */ SEnumEntry( CDbClientGateway::ERequestConnect,            "Connect"            ),
    /* 4: */ SEnumEntry( CDbClientGateway::ERequestDisconnect,         "Disconnect"         ),
    /* 5: */ SEnumEntry( CDbClientGateway::ERequestUpdateHostSettings, "UpdateHostSettings" )
};

//------------------------------------------------------------------------------
QString CDbClientGateway::Request2Str( int i_iRequest )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arstrConnectionGatewayRequests, _ZSArrLen(s_arstrConnectionGatewayRequests), i_iRequest );
}

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CDbClientGateway::CDbClientGateway(
    CDbClient*              i_pClient,
    CDbClientGatewayThread* i_pGatewayThread ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pClient(i_pClient),
    m_pGatewayThread(i_pGatewayThread),
    m_protocolType(EProtocolTypeUndefined),
    m_socketDscr(ESrvCltTypeClient),
    m_pIpcSocketWrapper(nullptr),
    m_pTmrConnect(nullptr),
    m_watchDogSettings(),
    m_pTmrWatchDog(nullptr),
    m_requestInProgress(ERequestNone),
    m_iReqIdInProgress(-1),
    m_iMsgIdReqInProgress(-1),
    m_pMsgCon(nullptr),
    m_arpTrcMsgLogObjects(),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( m_pClient->objectName() );

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pTmrConnect = new QTimer(this);
    m_pTmrConnect->setSingleShot(true);

    if( !QObject::connect(
        /* pObjSender   */ m_pTmrConnect,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTimeoutConnect()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Set the default value for the local host name to the computers name.
    m_socketDscr.m_strLocalHostName = QHostInfo::localHostName();

    m_pTmrWatchDog = new QTimer(this);

    if( !QObject::connect(
        /* pObjSender   */ m_pTmrWatchDog,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTmrWatchDogTimeout()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDbClientGateway::~CDbClientGateway()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try
    {
        delete m_pIpcSocketWrapper;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pMsgCon;
    }
    catch(...)
    {
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pClient = nullptr;
    m_pGatewayThread = nullptr;
    m_protocolType = static_cast<EProtocolType>(0);
    //m_socketDscr;
    m_pIpcSocketWrapper = nullptr;
    m_pTmrConnect = nullptr;
    //m_watchDogSettings;
    m_pTmrWatchDog = nullptr;
    m_requestInProgress = static_cast<ERequest>(0);
    m_iReqIdInProgress = 0;
    m_iMsgIdReqInProgress = 0;
    m_pMsgCon = nullptr;
    m_arpTrcMsgLogObjects.clear();
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientGateway::abortRequestInProgress()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "abortRequestInProgress",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "ReqInProgress: " + Request2Str(m_requestInProgress);
        strAddTrcInfo += ", ReqIdInProgress: " + QString::number(m_iReqIdInProgress);
        strAddTrcInfo += ", MsgReqIdProgress: " + QString::number(m_iMsgIdReqInProgress);
        mthTracer.trace(strAddTrcInfo);
    }

    //switch( m_requestInProgress )
    //{
    //    case ERequestNone:
    //    default:
    //    {
    //        break;
    //    }
    //}

} // abortRequestInProgress

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientGateway::addTrcMsgLogObject( QObject* i_pObj )
//------------------------------------------------------------------------------
{
    if( i_pObj == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pObj == nullptr");
    }

    QObject* pObj;
    int      idxObj;

    for( idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
    {
        pObj = m_arpTrcMsgLogObjects[idxObj];

        if( pObj == i_pObj )
        {
            throw CException(__FILE__,__LINE__,EResultObjAlreadyInList);
        }
    }

    m_arpTrcMsgLogObjects.append(i_pObj);

} // addTrcMsgLogObject

//------------------------------------------------------------------------------
void CDbClientGateway::removeTrcMsgLogObject( QObject* i_pObj )
//------------------------------------------------------------------------------
{
    if( i_pObj == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pObj == nullptr");
    }

    QObject* pObj;
    int      idxObj = -1;

    if( m_arpTrcMsgLogObjects.size() > 0 )
    {
        for( idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
        {
            pObj = m_arpTrcMsgLogObjects[idxObj];

            if( pObj == i_pObj )
            {
                break;
            }
        }
    }
    if( idxObj < 0 || idxObj > m_arpTrcMsgLogObjects.size() )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    m_arpTrcMsgLogObjects.removeAt(idxObj);

} // removeTrcMsgLogObject

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientGateway::onConnected( QObject* /*i_pSocketWrapper*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onConnected",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "RequestInProgress: " + Request2Str(m_requestInProgress);
        mthTracer.trace(strAddTrcInfo);
    }

    m_socketDscr.m_socketState = m_pIpcSocketWrapper->socketState();
    m_socketDscr.m_iSocketId = m_pIpcSocketWrapper->getSocketId();
    m_socketDscr.m_uServerListenPort = m_pIpcSocketWrapper->getServerListenPort();
    m_socketDscr.m_strRemoteHostName = m_pIpcSocketWrapper->getRemoteHostName();
    m_socketDscr.m_hostAddrRemote = m_pIpcSocketWrapper->getRemoteHostAddress();
    m_socketDscr.m_uRemotePort = m_pIpcSocketWrapper->getRemotePort();
    m_socketDscr.m_uBufferSize = m_pIpcSocketWrapper->getBufferSize();
    m_socketDscr.m_uLocalPort = m_pIpcSocketWrapper->getLocalPort();

    onConnected();

} // onConnected

//------------------------------------------------------------------------------
void CDbClientGateway::onTimeoutConnect()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ 2,
        /* strMethod    */ "onTimeoutConnect",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "RequestInProgress: " + Request2Str(m_requestInProgress);
        mthTracer.trace(strAddTrcInfo);
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("onTimeoutConnect");

    errResultInfo.setSeverity(EResultSeverityError);
    errResultInfo.setResult(EResultTimeout);

    if( m_socketDscr.m_strRemoteHostName.isEmpty() )
    {
        errResultInfo.setAddErrInfoDscr( "Connection with ---:" + QString::number(m_socketDscr.m_uServerListenPort) );
    }
    else
    {
        errResultInfo.setAddErrInfoDscr( "Connection with " + m_socketDscr.m_strRemoteHostName + ":" + QString::number(m_socketDscr.m_uServerListenPort) );
    }

    if( m_requestInProgress == ERequestConnect )
    {
        if( m_pIpcSocketWrapper != nullptr && m_pIpcSocketWrapper->socketState() == ESocketStateConnecting )
        {
            // The "disconnected" slot will be invoked and the request in progress changes.
            m_pIpcSocketWrapper->abort();
        }

        if( m_pTmrWatchDog->isActive() )
        {
            m_pTmrWatchDog->stop();
        }

        m_socketDscr.m_socketState = ESocketStateUnconnected;

        // The connection request has been executed. An error will be reported.
        if( m_arpTrcMsgLogObjects.size() > 0 )
        {
            strAddTrcInfo = "-- IpcSocket::connectToHost( TIMEOUT )";

            for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
            {
                CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                    /* pObjSender   */ this,
                    /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                    /* transmitDir  */ ETransmitDir::Undefined,
                    /* bBold        */ false,
                    /* strMsg       */ strAddTrcInfo );
                POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                pMsgLogItem = nullptr;
            }
        }

        if( m_pMsgCon != nullptr )
        {
            m_pMsgCon->setErrResultInfo(errResultInfo);
            m_pMsgCon->setProgress(100);
            CMsgConConnect* pMsgCon = dynamic_cast<CMsgConConnect*>(m_pMsgCon);
            if( pMsgCon != nullptr )
            {
                pMsgCon->setSocketState(m_socketDscr.m_socketState);
            }
        }

        CRequestExecTree* pReqExecTree = CRequestExecTree::GetInstance();

        bool bIsBlockingRequest = false;

        if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(m_iReqIdInProgress) )
        {
            bIsBlockingRequest = true;

            // The thread starting object must have called "wait" on the
            // wait condition. Otherwise "wakeAll" may never return. To ensure
            // this we force this thread to sleep for a moment ...
            // If the request is still alive (wait timeout may have occured
            // and request instance may have been destroyed) ...
            if( pReqExecTree->findRequest(m_iReqIdInProgress) != nullptr )
            {
                while( !pReqExecTree->isWaiting(m_iReqIdInProgress) && !pReqExecTree->waitTimedOut(m_iReqIdInProgress) )
                {
                    CSleeperThread::msleep(10);
                }
            }
        }

        if( bIsBlockingRequest )
        {
            pReqExecTree->setExecutionConfirmationMessage(m_iReqIdInProgress, m_pMsgCon);
            m_pMsgCon = nullptr;

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "-+ ReqConnect.WaitCondition.wakeAll()" );
            }

            if( !pReqExecTree->wake(m_iReqIdInProgress) )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultInternalStateMachineError);
                errResultInfo.setAddErrInfoDscr("Waking up thread waiting for connect request failed");

                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
            }

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "+- ReqConnect.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
            }
        } // if( bIsBlockingRequest )

        else if( m_pMsgCon != nullptr )
        {
            POST_OR_DELETE_MESSAGE(m_pMsgCon, &mthTracer, ETraceDetailLevelRuntimeInfo);
            m_pMsgCon = nullptr;
        }
        else if( errResultInfo.isErrorResult() )
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }

        delete m_pMsgCon;
        m_pMsgCon = nullptr;

        m_requestInProgress   = ERequestNone;
        m_iReqIdInProgress    = -1;
        m_iMsgIdReqInProgress = -1;

    } // if( m_requestInProgress == ERequestConnect )

} // onTimeoutConnect

//------------------------------------------------------------------------------
void CDbClientGateway::onTmrWatchDogTimeout()
//------------------------------------------------------------------------------
{
    if( m_watchDogSettings.m_bEnabled )
    {
        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDetailLevel */ 3,
            /* strMethod    */ "onTmrWatchDogTimeout",
            /* strAddInfo   */ "" );

        SErrResultInfo errResultInfo = checkConnection();

        if( errResultInfo.isErrorResult() )
        {
            CMsgIndDisconnected* pMsgInd = new CMsgIndDisconnected(
                /* pObjSender   */ this,
                /* pObjReceiver */ m_pClient,
                /* protocolType */ m_protocolType,
                /* socketDscr   */ m_socketDscr );
            POST_OR_DELETE_MESSAGE(pMsgInd,&mthTracer);
            pMsgInd = nullptr;
        }
    } // if( m_watchDogSettings.m_bEnabled )

} // onTmrWatchDogTimeout

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientGateway::onDisconnected( QObject* /*i_pSocketWrapper*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDisconnected",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "RequestInProgress: " + Request2Str(m_requestInProgress);
        mthTracer.trace(strAddTrcInfo);
    }

    onDisconnected();

} // onDisconnected

//------------------------------------------------------------------------------
void CDbClientGateway::onReadyRead( QObject* i_pSocketWrapper )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ 1,
        /* strMethod    */ "onReadyRead",
        /* strAddInfo   */ "" );

    if( i_pSocketWrapper == nullptr )
    {
        QString strAddErrInfo = "onReadyRead( i_pSocketWrapper == nullptr )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddErrInfo);
    }
    if( m_pIpcSocketWrapper != i_pSocketWrapper )
    {
        QString strAddErrInfo = "onReadyRead( m_pIpcSocketWrapper != i_pSocketWrapper )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddErrInfo);
    }

    CMsgReqReceiveData* pMsgReq = new CMsgReqReceiveData(
        /* pObjSender       */ this,
        /* pObjReceiver     */ this,
        /* iSocketId        */ m_pIpcSocketWrapper->getSocketId(),
        /* bMustBeConfirmed */ false,
        /* iReqId           */ -1 );
    POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ETraceDetailLevelRuntimeInfo);
    pMsgReq = nullptr;

} // onReadyRead

//------------------------------------------------------------------------------
void CDbClientGateway::onError( QObject* /*i_pSocketWrapper*/, ZS::System::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo =  i_errResultInfo.getResultStr();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onError",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "RequestInProgress: " + Request2Str(m_requestInProgress);
        mthTracer.trace(strAddTrcInfo);
    }

    EResult        result = i_errResultInfo.getResult();
    SErrResultInfo errResultInfo = i_errResultInfo;

    if( errResultInfo.getErrSource().isObjectPathEmpty() )
    {
        errResultInfo.setErrSource(nameSpace(), className(), objectName(), "onError");
    }

    if( m_socketDscr.m_strRemoteHostName.isEmpty() )
    {
        errResultInfo.setAddErrInfoDscr( "Connection with ---:" + QString::number(m_socketDscr.m_uServerListenPort) );
    }
    else
    {
        errResultInfo.setAddErrInfoDscr( "Connection with " + m_socketDscr.m_strRemoteHostName + ":" + QString::number(m_socketDscr.m_uServerListenPort) );
    }

    switch( m_requestInProgress )
    {
        case ERequestNone:
        {
            switch( m_pIpcSocketWrapper->error() )
            {
                // If the remote client closed the connection ..
                case EResultRemoteHostClosed:
                {
                    // .. its not a real error. The connection will be closed calling the disconnected slot.
                    result = EResultSuccess;
                    break;
                }
                // Timeout reported by the socket while no request is active ..
                case EResultTimeout:
                {
                    // ..the connect timeout has been expired before (see slot "onTimeoutConnection")
                    // and has already been reported to the client.
                    result = EResultSuccess;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        } // case ERequestNone

        case ERequestConnect:
        {
            // If the "connectToHost" method timed out and if a connect timeout has been specified...
            if( /*result == EResultTimeout &&*/ m_socketDscr.m_iConnectTimeout_ms > 0 )
            {
                // .. its the slot "onTimeoutConnect" that will report the timeout. If
                // the "connectToHost" method timed out the specified timeout has not really
                // been expired and we can try to establish the connection again ...
                CMsgReqConnect* pMsgReq = new CMsgReqConnect(
                    /* pObjSender       */ this,
                    /* pObjReceiver     */ this,
                    /* protocolType     */ m_protocolType,
                    /* hostSettings     */ m_socketDscr,
                    /* bMustBeConfirmed */ false,
                    /* iReqId           */ -1 );
                POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ETraceDetailLevelRuntimeInfo);
                pMsgReq = nullptr;
            }
            else // if( m_socketDscr.m_iConnectTimeout_ms <= 0 )
            {
                if( m_pIpcSocketWrapper != nullptr && m_pIpcSocketWrapper->socketState() == ESocketStateConnecting )
                {
                    m_pIpcSocketWrapper->abort();
                }
                if( m_pTmrWatchDog->isActive() )
                {
                    m_pTmrWatchDog->stop();
                }

                m_socketDscr.m_socketState = ESocketStateUnconnected;

                // The connection request has been executed. An error will be reported.
                if( m_arpTrcMsgLogObjects.size() > 0 )
                {
                    strAddTrcInfo = "-- IpcSocket::error( " + errResultInfo.getAddErrInfoDscr() + " )";

                    for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                    {
                        CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                            /* pObjSender   */ this,
                            /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                            /* transmitDir  */ ETransmitDir::Undefined,
                            /* bBold        */ false,
                            /* strMsg       */ strAddTrcInfo );
                        POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                        pMsgLogItem = nullptr;
                    }
                }

                if( m_pMsgCon != nullptr )
                {
                    m_pMsgCon->setErrResultInfo(errResultInfo);
                    m_pMsgCon->setProgress(100);
                    CMsgConConnect* pMsgCon = dynamic_cast<CMsgConConnect*>(m_pMsgCon);
                    if( pMsgCon != nullptr )
                    {
                        pMsgCon->setSocketState(m_socketDscr.m_socketState);
                    }
                }

                CRequestExecTree* pReqExecTree = CRequestExecTree::GetInstance();

                bool bIsBlockingRequest = false;

                if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(m_iReqIdInProgress) )
                {
                    bIsBlockingRequest = true;

                    // The thread starting object must have called "wait" on the
                    // wait condition. Otherwise "wakeAll" may never return. To ensure
                    // this we force this thread to sleep for a moment ...
                    // If the request is still alive (wait timeout may have occured
                    // and request instance may have been destroyed) ...
                    if( pReqExecTree->findRequest(m_iReqIdInProgress) != nullptr )
                    {
                        while( !pReqExecTree->isWaiting(m_iReqIdInProgress) && !pReqExecTree->waitTimedOut(m_iReqIdInProgress) )
                        {
                            CSleeperThread::msleep(10);
                        }
                    }
                }

                if( bIsBlockingRequest )
                {
                    pReqExecTree->setExecutionConfirmationMessage(m_iReqIdInProgress, m_pMsgCon);
                    m_pMsgCon = nullptr;

                    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                    {
                        mthTracer.trace( "-+ ReqConnect.WaitCondition.wakeAll()" );
                    }

                    if( !pReqExecTree->wake(m_iReqIdInProgress) )
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultInternalStateMachineError);
                        errResultInfo.setAddErrInfoDscr("Waking up thread waiting for connect request failed");

                        if( CErrLog::GetInstance() != nullptr )
                        {
                            CErrLog::GetInstance()->addEntry(errResultInfo);
                        }
                    }

                    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                    {
                        mthTracer.trace( "+- ReqConnect.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
                    }
                } // if( bIsBlockingRequest )

                else if( m_pMsgCon != nullptr )
                {
                    POST_OR_DELETE_MESSAGE(m_pMsgCon, &mthTracer, ETraceDetailLevelRuntimeInfo);
                    m_pMsgCon = nullptr;
                }
                else if( errResultInfo.isErrorResult() )
                {
                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                }

                delete m_pMsgCon;
                m_pMsgCon = nullptr;

                m_requestInProgress   = ERequestNone;
                m_iReqIdInProgress    = -1;
                m_iMsgIdReqInProgress = -1;

            } // if( m_socketDscr.m_iConnectTimeout_ms <= 0 )

            // Its not a real error but the connection was not established.
            result = EResultSuccess;
            break;
        } // case ERequestConnect

        case ERequestDisconnect:
        {
            m_requestInProgress   = ERequestNone;
            m_iReqIdInProgress    = -1;
            m_iMsgIdReqInProgress = -1;
            break;
        }

        case ERequestStartup:
        case ERequestShutdown:
        case ERequestUpdateHostSettings:
        default:
        {
            break;
        }
    } // switch( m_requestInProgress )

    if( result != EResultSuccess )
    {
        CMsgErr* pMsgErr = new CMsgErr(
            /* pObjSender    */ this,
            /* pObjReceiver  */ m_pClient,
            /* errResultInfo */ errResultInfo );
        POST_OR_DELETE_MESSAGE(pMsgErr, &mthTracer, ETraceDetailLevelRuntimeInfo);
        pMsgErr = nullptr;
    }

} // onError

/*==============================================================================
protected: // overridables (auxiliary methods)
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientGateway::onConnected()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onConnected",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "RequestInProgress: " + Request2Str(m_requestInProgress);
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("onConnected");

    if( m_pTmrWatchDog->isActive() )
    {
        m_pTmrWatchDog->stop();
    }
    if( m_pTmrConnect->isActive() )
    {
        m_pTmrConnect->stop();
    }

    if( m_requestInProgress == ERequestConnect )
    {
        if( m_arpTrcMsgLogObjects.size() > 0 )
        {
            strAddTrcInfo = "-- IpcSocket::connected";

            for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
            {
                CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                    /* pObjSender   */ this,
                    /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                    /* transmitDir  */ ETransmitDir::Undefined,
                    /* bBold        */ false,
                    /* strMsg       */ strAddTrcInfo );
                POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                pMsgLogItem = nullptr;
            }
        }

        if( m_pMsgCon != nullptr )
        {
            m_pMsgCon->setErrResultInfo(errResultInfo);
            m_pMsgCon->setProgress(100);
            CMsgConConnect* pMsgCon = dynamic_cast<CMsgConConnect*>(m_pMsgCon);
            if( pMsgCon != nullptr )
            {
                pMsgCon->setSocketDscr(m_socketDscr);
            }
        }

        CRequestExecTree* pReqExecTree = CRequestExecTree::GetInstance();

        bool bIsBlockingRequest = false;

        if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(m_iReqIdInProgress) )
        {
            bIsBlockingRequest = true;

            // The thread starting object must have called "wait" on the
            // wait condition. Otherwise "wakeAll" may never return. To ensure
            // this we force this thread to sleep for a moment ...
            // If the request is still alive (wait timeout may have occured
            // and request instance may have been destroyed) ...
            if( pReqExecTree->findRequest(m_iReqIdInProgress) != nullptr )
            {
                while( !pReqExecTree->isWaiting(m_iReqIdInProgress) && !pReqExecTree->waitTimedOut(m_iReqIdInProgress) )
                {
                    CSleeperThread::msleep(10);
                }
            }
        }

        if( bIsBlockingRequest )
        {
            pReqExecTree->setExecutionConfirmationMessage(m_iReqIdInProgress, m_pMsgCon);
            m_pMsgCon = nullptr;

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "-+ ReqConnect.WaitCondition.wakeAll()" );
            }

            if( !pReqExecTree->wake(m_iReqIdInProgress) )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultInternalStateMachineError);
                errResultInfo.setAddErrInfoDscr("Waking up thread waiting for connect request failed");

                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
            }

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "+- ReqConnect.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
            }
        } // if( bIsBlockingRequest )

        else if( m_pMsgCon != nullptr )
        {
            POST_OR_DELETE_MESSAGE(m_pMsgCon, &mthTracer, ETraceDetailLevelRuntimeInfo);
            m_pMsgCon = nullptr;
        }
        else if( errResultInfo.isErrorResult() )
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }

        delete m_pMsgCon;
        m_pMsgCon = nullptr;

        m_requestInProgress   = ERequestNone;
        m_iReqIdInProgress    = -1;
        m_iMsgIdReqInProgress = -1;

        if( m_watchDogSettings.m_bEnabled && m_watchDogSettings.m_iInterval_ms > 0 )
        {
            m_pTmrWatchDog->start(m_watchDogSettings.m_iInterval_ms);
        }
    } // if( m_requestInProgress == ERequestConnect )

    else // if( m_requestInProgress != ERequestConnect )
    {
        CMsgIndConnected* pMsgInd = new CMsgIndConnected(
            /* pObjSender   */ this,
            /* pObjReceiver */ m_pClient,
            /* protocolType */ m_protocolType,
            /* socketDscr   */ m_socketDscr );
        POST_OR_DELETE_MESSAGE(pMsgInd, &mthTracer, ETraceDetailLevelRuntimeInfo);
        pMsgInd = nullptr;
    }

} // onConnected

//------------------------------------------------------------------------------
void CDbClientGateway::onDisconnected()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDisconnected",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "RequestInProgress: " + Request2Str(m_requestInProgress);
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_pTmrWatchDog->isActive() )
    {
        m_pTmrWatchDog->stop();
    }
    if( m_pTmrConnect->isActive() )
    {
        m_pTmrConnect->stop();
    }

    m_socketDscr.m_socketState = ESocketStateUnconnected;

    if( m_arpTrcMsgLogObjects.size() > 0 )
    {
        strAddTrcInfo = "-- IpcSocket::disconnected";

        for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
        {
            CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                /* pObjSender   */ this,
                /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                /* transmitDir  */ ETransmitDir::Undefined,
                /* bBold        */ false,
                /* strMsg       */ strAddTrcInfo );
            POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
            pMsgLogItem = nullptr;
        }
    }

    switch( m_requestInProgress )
    {
        case ERequestShutdown:
        {
            break;
        }

        case ERequestConnect:
        {
            SErrResultInfo errResultInfo = ErrResultInfoSuccess("connect");

            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Remote host " + m_socketDscr.m_strRemoteHostName + ":" + QString::number(m_socketDscr.m_uServerListenPort) + " disconnected" );

            if( m_pMsgCon != nullptr )
            {
                m_pMsgCon->setErrResultInfo(errResultInfo);
                m_pMsgCon->setProgress(100);
                CMsgConConnect* pMsgCon = dynamic_cast<CMsgConConnect*>(m_pMsgCon);
                if( pMsgCon != nullptr )
                {
                    pMsgCon->setSocketState(m_socketDscr.m_socketState);
                }
            }

            CRequestExecTree* pReqExecTree = CRequestExecTree::GetInstance();

            bool bIsBlockingRequest = false;

            if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(m_iReqIdInProgress) )
            {
                bIsBlockingRequest = true;

                // The thread starting object must have called "wait" on the
                // wait condition. Otherwise "wakeAll" may never return. To ensure
                // this we force this thread to sleep for a moment ...
                // If the request is still alive (wait timeout may have occured
                // and request instance may have been destroyed) ...
                if( pReqExecTree->findRequest(m_iReqIdInProgress) != nullptr )
                {
                    while( !pReqExecTree->isWaiting(m_iReqIdInProgress) && !pReqExecTree->waitTimedOut(m_iReqIdInProgress) )
                    {
                        CSleeperThread::msleep(10);
                    }
                }
            }

            if( bIsBlockingRequest )
            {
                pReqExecTree->setExecutionConfirmationMessage(m_iReqIdInProgress, m_pMsgCon);
                m_pMsgCon = nullptr;

                if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                {
                    mthTracer.trace( "-+ ReqConnect.WaitCondition.wakeAll()" );
                }

                if( !pReqExecTree->wake(m_iReqIdInProgress) )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInternalStateMachineError);
                    errResultInfo.setAddErrInfoDscr("Waking up thread waiting for connect request failed");

                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                }

                if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                {
                    mthTracer.trace( "+- ReqConnect.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
                }
            } // if( bIsBlockingRequest )

            else if( m_pMsgCon != nullptr )
            {
                POST_OR_DELETE_MESSAGE(m_pMsgCon, &mthTracer, ETraceDetailLevelRuntimeInfo);
                m_pMsgCon = nullptr;
            }
            else if( errResultInfo.isErrorResult() )
            {
                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
            }

            delete m_pMsgCon;
            m_pMsgCon = nullptr;

            m_requestInProgress   = ERequestNone;
            m_iReqIdInProgress    = -1;
            m_iMsgIdReqInProgress = -1;
            break;
        } // case ERequestConnect

        case ERequestDisconnect:
        {
            SErrResultInfo errResultInfo = ErrResultInfoSuccess("onDisconnected");

            if( m_pMsgCon != nullptr )
            {
                m_pMsgCon->setErrResultInfo(errResultInfo);
                m_pMsgCon->setProgress(100);
            }

            CRequestExecTree* pReqExecTree = CRequestExecTree::GetInstance();

            bool bIsBlockingRequest = false;

            if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(m_iReqIdInProgress) )
            {
                bIsBlockingRequest = true;

                // The thread starting object must have called "wait" on the
                // wait condition. Otherwise "wakeAll" may never return. To ensure
                // this we force this thread to sleep for a moment ...
                // If the request is still alive (wait timeout may have occured
                // and request instance may have been destroyed) ...
                if( pReqExecTree->findRequest(m_iReqIdInProgress) != nullptr )
                {
                    while( !pReqExecTree->isWaiting(m_iReqIdInProgress) && !pReqExecTree->waitTimedOut(m_iReqIdInProgress) )
                    {
                        CSleeperThread::msleep(10);
                    }
                }
            }

            if( bIsBlockingRequest )
            {
                pReqExecTree->setExecutionConfirmationMessage(m_iReqIdInProgress, m_pMsgCon);
                m_pMsgCon = nullptr;

                if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                {
                    mthTracer.trace( "-+ ReqDisconnect.WaitCondition.wakeAll()" );
                }

                if( !pReqExecTree->wake(m_iReqIdInProgress) )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInternalStateMachineError);
                    errResultInfo.setAddErrInfoDscr("Waking up thread waiting for disconnect request failed");

                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                }

                if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                {
                    mthTracer.trace( "+- ReqDisconnect.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
                }
            } // if( bIsBlockingRequest )

            else if( m_pMsgCon != nullptr )
            {
                POST_OR_DELETE_MESSAGE(m_pMsgCon, &mthTracer, ETraceDetailLevelRuntimeInfo);
                m_pMsgCon = nullptr;
            }
            else if( errResultInfo.isErrorResult() )
            {
                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
            }

            delete m_pMsgCon;
            m_pMsgCon = nullptr;

            m_requestInProgress   = ERequestNone;
            m_iReqIdInProgress    = -1;
            m_iMsgIdReqInProgress = -1;
            break;
        } // case ERequestDisconnect

        case ERequestUpdateHostSettings:
        {
            break;
        }

        case ERequestNone:
        default:
        {
            CMsgIndDisconnected* pMsgInd = new CMsgIndDisconnected(
                /* pObjSender   */ this,
                /* pObjReceiver */ m_pClient,
                /* protocolType */ m_protocolType,
                /* socketDscr   */ m_socketDscr ); // Socket Id for clients is always 0
            POST_OR_DELETE_MESSAGE(pMsgInd, &mthTracer, ETraceDetailLevelRuntimeInfo);
            pMsgInd = nullptr;
            break;
        } // case ERequestNone
    } // switch( m_requestInProgress )

    // Set here after send the indication or confirmation messages.
    m_socketDscr.m_iSocketId = -1;

} // onDisconnected

//------------------------------------------------------------------------------
void CDbClientGateway::onError( ZS::System::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "ErrResult {" + i_errResultInfo.toString(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onError",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "RequestInProgress: " + Request2Str(m_requestInProgress);
    }

    EResult        result = i_errResultInfo.getResult();
    SErrResultInfo errResultInfo = i_errResultInfo;

    if( errResultInfo.getErrSource().isObjectPathEmpty() )
    {
        errResultInfo.setErrSource(nameSpace(), className(), objectName(), "onError");
    }

    if( m_socketDscr.m_strRemoteHostName.isEmpty() )
    {
        errResultInfo.setAddErrInfoDscr( "Connection with ---:" + QString::number(m_socketDscr.m_uServerListenPort) );
    }
    else
    {
        errResultInfo.setAddErrInfoDscr( "Connection with " + m_socketDscr.m_strRemoteHostName + ":" + QString::number(m_socketDscr.m_uServerListenPort) );
    }

    switch( m_requestInProgress )
    {
        case ERequestNone:
        {
            switch( result )
            {
                // If the remote client closed the connection ..
                case EResultRemoteHostClosed:
                {
                    // .. its not a real error. The connection will be closed calling the disconnected slot.
                    result = EResultSuccess;
                    break;
                }
                // Timeout reported by the socket while no request is active ..
                case EResultTimeout:
                {
                    // ..the connect timeout has been expired before (see slot "onTimeoutConnection")
                    // and has already been reported to the client.
                    result = EResultSuccess;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        } // case ERequestNone

        case ERequestConnect:
        {
            m_socketDscr.m_socketState = ESocketStateUnconnected;

            // The connection request has been executed. An error will be reported.
            if( m_arpTrcMsgLogObjects.size() > 0 )
            {
                strAddTrcInfo = "-- IpcSocket::error( " + errResultInfo.getAddErrInfoDscr() + " )";

                for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                {
                    CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                        /* pObjSender   */ this,
                        /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                        /* transmitDir  */ ETransmitDir::Undefined,
                        /* bBold        */ false,
                        /* strMsg       */ strAddTrcInfo );
                    POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                    pMsgLogItem = nullptr;
                }
            }

            if( m_pMsgCon != nullptr )
            {
                m_pMsgCon->setErrResultInfo(errResultInfo);
                m_pMsgCon->setProgress(100);
                CMsgConConnect* pMsgCon = dynamic_cast<CMsgConConnect*>(m_pMsgCon);
                if( pMsgCon != nullptr )
                {
                    pMsgCon->setSocketState(m_socketDscr.m_socketState);
                }
            }

            CRequestExecTree* pReqExecTree = CRequestExecTree::GetInstance();

            bool bIsBlockingRequest = false;

            if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(m_iReqIdInProgress) )
            {
                bIsBlockingRequest = true;

                // The thread starting object must have called "wait" on the
                // wait condition. Otherwise "wakeAll" may never return. To ensure
                // this we force this thread to sleep for a moment ...
                // If the request is still alive (wait timeout may have occured
                // and request instance may have been destroyed) ...
                if( pReqExecTree->findRequest(m_iReqIdInProgress) != nullptr )
                {
                    while( !pReqExecTree->isWaiting(m_iReqIdInProgress) && !pReqExecTree->waitTimedOut(m_iReqIdInProgress) )
                    {
                        CSleeperThread::msleep(10);
                    }
                }
            }

            if( bIsBlockingRequest )
            {
                pReqExecTree->setExecutionConfirmationMessage(m_iReqIdInProgress, m_pMsgCon);
                m_pMsgCon = nullptr;

                if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                {
                    mthTracer.trace( "-+ ReqConnect.WaitCondition.wakeAll()" );
                }

                if( !pReqExecTree->wake(m_iReqIdInProgress) )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInternalStateMachineError);
                    errResultInfo.setAddErrInfoDscr("Waking up thread waiting for connect request failed");

                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                }

                if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                {
                    mthTracer.trace( "+- ReqConnect.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
                }
            } // if( bIsBlockingRequest )

            else if( m_pMsgCon != nullptr )
            {
                POST_OR_DELETE_MESSAGE(m_pMsgCon, &mthTracer, ETraceDetailLevelRuntimeInfo);
                m_pMsgCon = nullptr;
            }
            else if( errResultInfo.isErrorResult() )
            {
                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
            }

            delete m_pMsgCon;
            m_pMsgCon = nullptr;

            m_requestInProgress   = ERequestNone;
            m_iReqIdInProgress    = -1;
            m_iMsgIdReqInProgress = -1;

            // Its not a real error but the connection was not established.
            result = EResultSuccess;
            break;
        } // case ERequestConnect

        case ERequestDisconnect:
        {
            m_requestInProgress   = ERequestNone;
            m_iReqIdInProgress    = -1;
            m_iMsgIdReqInProgress = -1;
            break;
        }

        case ERequestStartup:
        case ERequestShutdown:
        case ERequestUpdateHostSettings:
        default:
        {
            break;
        }
    } // switch( m_requestInProgress )

    if( result != EResultSuccess )
    {
        CMsgErr* pMsgErr = new CMsgErr(
            /* pObjSender    */ this,
            /* pObjReceiver  */ m_pClient,
            /* errResultInfo */ errResultInfo );
        POST_OR_DELETE_MESSAGE(pMsgErr, &mthTracer, ETraceDetailLevelRuntimeInfo);
        pMsgErr = nullptr;
    }

} // onError

/*==============================================================================
protected: // overridables (auxiliary methods)
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CDbClientGateway::checkConnection()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ 3,
        /* strMethod    */ "checkConnection",
        /* strAddInfo   */ "" );

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".checkConnection";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    return errResultInfo;

} // checkConnection

/*==============================================================================
public: // must overridables (overridables of base class QObject)
==============================================================================*/

//------------------------------------------------------------------------------
bool CDbClientGateway::event( QEvent* i_pMsg )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    if( i_pMsg->type() >= QEvent::User )
    {
        CMsg* pMsg = nullptr;

        try
        {
            pMsg = dynamic_cast<CMsg*>(i_pMsg);
        }
        catch(...)
        {
            pMsg = nullptr;
        }

        if( pMsg != nullptr )
        {
            bEventHandled = true;

            QString strMth = nameSpace() + "::" + className() + "::" + objectName() + "." + pMsg->msgTypeToStr();

            SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

            QString strAddErrInfo;

            QString strAddTrcInfo;

            if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
            {
                int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
                strAddTrcInfo = "Msg {" + pMsg->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
            }

            CMethodTracer mthTracer(
                /* pAdminObj    */ m_pTrcAdminObj,
                /* iDetailLevel */ ETraceDetailLevelMethodCalls,
                /* strMethod    */ "event",
                /* strAddInfo   */ strAddTrcInfo );

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                strAddTrcInfo  = "ReqInProgress: " + Request2Str(m_requestInProgress);
                strAddTrcInfo += ", ReqIdInProgress: " + QString::number(m_iReqIdInProgress);
                strAddTrcInfo += ", MsgReqIdProgress: " + QString::number(m_iMsgIdReqInProgress);
                mthTracer.trace(strAddTrcInfo);
            }

            CRequestExecTree* pReqExecTree = CRequestExecTree::GetInstance();

            if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeReq )
            {
                switch( static_cast<int>(pMsg->type()) )
                {
                    case EMsgTypeReqStartup:
                    {
                        CMsgReqStartup* pMsgReq = dynamic_cast<CMsgReqStartup*>(i_pMsg);

                        if( pMsgReq == nullptr )
                        {
                            throw CException(__FILE__, __LINE__, EResultMessageTypeMismatch, pMsg->getAddTrcInfoStr());
                        }
                        if( pMsgReq->getSender() != m_pClient )
                        {
                            throw CException(__FILE__, __LINE__, EResultInvalidSenderObjInMessage, pMsg->getAddTrcInfoStr());
                        }

                        delete m_pMsgCon;
                        m_pMsgCon = nullptr;

                        bool bIsBlockingRequest = false;

                        if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(pMsgReq->getRequestId()) )
                        {
                            bIsBlockingRequest = true;
                        }

                        if( pMsgReq->mustBeConfirmed() || bIsBlockingRequest )
                        {
                            m_pMsgCon = pMsgReq->createConfirmationMessage();
                        }

                        if( m_requestInProgress != ERequestNone )
                        {
                            strAddErrInfo = "RequestInProgress: " + Request2Str(m_requestInProgress) + ", " + pMsg->getAddTrcInfoStr();
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultMessageStateMismatch);
                            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                        }
                        else // if( m_requestInProgress == ERequestNone )
                        {
                            m_requestInProgress   = ERequestStartup;
                            m_iReqIdInProgress    = pMsgReq->getRequestId();
                            m_iMsgIdReqInProgress = pMsgReq->getMsgId();

                            m_socketDscr = pMsgReq->getClientHostSettings().getSocketDscr();

                            if( pMsgReq->getWatchDogSettings() != nullptr )
                            {
                                m_watchDogSettings = *pMsgReq->getWatchDogSettings();
                            }

                            delete m_pIpcSocketWrapper;
                            m_pIpcSocketWrapper = nullptr;

                            if( m_socketDscr.m_socketType == ESocketTypeTcp )
                            {
                                m_pIpcSocketWrapper = new CTcpSocketWrapper( objectName(), ESrvCltTypeClient, 0, nullptr );
                            }
                            else if( m_socketDscr.m_socketType == ESocketTypeShm )
                            {
                                m_pIpcSocketWrapper = new CShmSocketWrapper( objectName(), ESrvCltTypeClient, 0 );
                            }
                            else if( m_socketDscr.m_socketType == ESocketTypeInProcMsg )
                            {
                                // No socket wrapper used. Messages are directly exchanged between the database and the client gateway.
                            }

                            if( m_pIpcSocketWrapper != nullptr )
                            {
                                m_pIpcSocketWrapper->setLocalHostName(m_socketDscr.m_strLocalHostName);

                                if( !QObject::connect(
                                    /* pObjSender   */ m_pIpcSocketWrapper,
                                    /* szSignal     */ SIGNAL(connected(QObject*)),
                                    /* pObjReceiver */ this,
                                    /* szSlot       */ SLOT(onConnected(QObject*)) ) )
                                {
                                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                                }
                                if( !QObject::connect(
                                    /* pObjSender   */ m_pIpcSocketWrapper,
                                    /* szSignal     */ SIGNAL(disconnected(QObject*)),
                                    /* pObjReceiver */ this,
                                    /* szSlot       */ SLOT(onDisconnected(QObject*)) ) )
                                {
                                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                                }
                                if( !QObject::connect(
                                    /* pObjSender   */ m_pIpcSocketWrapper,
                                    /* szSignal     */ SIGNAL(readyRead(QObject*)),
                                    /* pObjReceiver */ this,
                                    /* szSlot       */ SLOT(onReadyRead(QObject*)) ) )
                                {
                                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                                }
                                if( !QObject::connect(
                                    /* pObjSender   */ m_pIpcSocketWrapper,
                                    /* szSignal     */ SIGNAL(error(QObject*,ZS::System::SErrResultInfo&)),
                                    /* pObjReceiver */ this,
                                    /* szSlot       */ SLOT(onError(QObject*,ZS::System::SErrResultInfo&)) ) )
                                {
                                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                                }
                            } // if( m_pIpcSocketWrapper != nullptr )
                        } // if( m_requestInProgress == ERequestNone )

                        if( m_pMsgCon != nullptr )
                        {
                            m_pMsgCon->setErrResultInfo(errResultInfo);
                            m_pMsgCon->setProgress(100);
                        }

                        if( bIsBlockingRequest )
                        {
                            // The thread starting object must have called "wait" on the
                            // wait condition. Otherwise "wakeAll" may never return. To ensure
                            // this we force this thread to sleep for a moment ...
                            // If the request is still alive (wait timeout may have occured
                            // and request instance may have been destroyed) ...
                            if( pReqExecTree->findRequest(m_iReqIdInProgress) != nullptr )
                            {
                                while( !pReqExecTree->isWaiting(m_iReqIdInProgress) && !pReqExecTree->waitTimedOut(m_iReqIdInProgress) )
                                {
                                    CSleeperThread::msleep(10);
                                }
                            }

                            pReqExecTree->setExecutionConfirmationMessage(m_iReqIdInProgress, m_pMsgCon);
                            m_pMsgCon = nullptr;

                            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                            {
                                mthTracer.trace( "-+ ReqStartup.WaitCondition.wakeAll()" );
                            }

                            if( !pReqExecTree->wake(m_iReqIdInProgress) )
                            {
                                errResultInfo.setSeverity(EResultSeverityError);
                                errResultInfo.setResult(EResultInternalStateMachineError);
                                errResultInfo.setAddErrInfoDscr("Waking up thread waiting for startup request failed");

                                if( CErrLog::GetInstance() != nullptr )
                                {
                                    CErrLog::GetInstance()->addEntry(errResultInfo);
                                }
                            }

                            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                            {
                                mthTracer.trace( "+- ReqStartup.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
                            }
                        } // if( bIsBlockingRequest )

                        else if( m_pMsgCon != nullptr )
                        {
                            POST_OR_DELETE_MESSAGE(m_pMsgCon, &mthTracer, ETraceDetailLevelRuntimeInfo);
                            m_pMsgCon = nullptr;
                        }
                        else if( errResultInfo.isErrorResult() )
                        {
                            if( CErrLog::GetInstance() != nullptr )
                            {
                                CErrLog::GetInstance()->addEntry(errResultInfo);
                            }
                        }

                        delete m_pMsgCon;
                        m_pMsgCon = nullptr;

                        if( m_requestInProgress == ERequestStartup )
                        {
                            m_requestInProgress   = ERequestNone;
                            m_iReqIdInProgress    = -1;
                            m_iMsgIdReqInProgress = -1;
                        }
                        break;
                    } // case EMsgTypeReqStartup

                    //------------------------------------------------------------------
                    case EMsgTypeReqShutdown:
                    //------------------------------------------------------------------
                    {
                        ZS::Data::CMsgReqShutdown* pMsgReq = dynamic_cast<ZS::Data::CMsgReqShutdown*>(i_pMsg);

                        if( pMsgReq == nullptr )
                        {
                            throw CException(__FILE__, __LINE__, EResultMessageTypeMismatch, pMsg->getAddTrcInfoStr());
                        }
                        if( pMsgReq->getSender() != m_pClient )
                        {
                            throw CException(__FILE__, __LINE__, EResultInvalidSenderObjInMessage, pMsg->getAddTrcInfoStr());
                        }

                        delete m_pMsgCon;
                        m_pMsgCon = nullptr;

                        bool bIsBlockingRequest = false;

                        if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(pMsgReq->getRequestId()) )
                        {
                            bIsBlockingRequest = true;
                        }

                        if( pMsgReq->mustBeConfirmed() || bIsBlockingRequest )
                        {
                            m_pMsgCon = pMsgReq->createConfirmationMessage();
                        }

                        if( m_requestInProgress != ERequestNone )
                        {
                            strAddErrInfo = "RequestInProgress: " + Request2Str(m_requestInProgress) + ", " + pMsg->getAddTrcInfoStr();
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultMessageStateMismatch);
                            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                        }

                        m_requestInProgress   = ERequestShutdown;
                        m_iReqIdInProgress    = pMsgReq->getRequestId();
                        m_iMsgIdReqInProgress = pMsgReq->getMsgId();

                        if( m_pTmrWatchDog->isActive() )
                        {
                            m_pTmrWatchDog->stop();
                        }

                        if( m_pIpcSocketWrapper != nullptr )
                        {
                            // Either the "disconnected" or "error" signal will be emitted by the
                            // sockets calling the "disconnectFromHost" or "abort" methods.
                            if( m_pIpcSocketWrapper->socketState() != ESocketStateUnconnected )
                            {
                                m_pIpcSocketWrapper->abort();
                            }

                            try
                            {
                                delete m_pIpcSocketWrapper;
                            }
                            catch(...)
                            {
                            }
                            m_pIpcSocketWrapper = nullptr;

                        } // if( m_pIpcSocketWrapper != nullptr )

                        if( m_pMsgCon != nullptr )
                        {
                            m_pMsgCon->setErrResultInfo(errResultInfo);
                            m_pMsgCon->setProgress(100);
                        }

                        if( bIsBlockingRequest )
                        {
                            // The thread starting object must have called "wait" on the
                            // wait condition. Otherwise "wakeAll" may never return. To ensure
                            // this we force this thread to sleep for a moment ...
                            // If the request is still alive (wait timeout may have occured
                            // and request instance may have been destroyed) ...
                            if( pReqExecTree->findRequest(m_iReqIdInProgress) != nullptr )
                            {
                                while( !pReqExecTree->isWaiting(m_iReqIdInProgress) && !pReqExecTree->waitTimedOut(m_iReqIdInProgress) )
                                {
                                    CSleeperThread::msleep(10);
                                }
                            }

                            pReqExecTree->setExecutionConfirmationMessage(m_iReqIdInProgress, m_pMsgCon);
                            m_pMsgCon = nullptr;

                            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                            {
                                mthTracer.trace( "-+ ReqShutdown.WaitCondition.wakeAll()" );
                            }

                            if( !pReqExecTree->wake(m_iReqIdInProgress) )
                            {
                                errResultInfo.setSeverity(EResultSeverityError);
                                errResultInfo.setResult(EResultInternalStateMachineError);
                                errResultInfo.setAddErrInfoDscr("Waking up thread waiting for shutdown request failed");

                                if( CErrLog::GetInstance() != nullptr )
                                {
                                    CErrLog::GetInstance()->addEntry(errResultInfo);
                                }
                            }

                            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                            {
                                mthTracer.trace( "+- ReqShudown.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
                            }
                        } // if( bIsBlockingRequest )

                        else if( m_pMsgCon != nullptr )
                        {
                            POST_OR_DELETE_MESSAGE(m_pMsgCon, &mthTracer, ETraceDetailLevelRuntimeInfo);
                            m_pMsgCon = nullptr;
                        }
                        else if( errResultInfo.isErrorResult() )
                        {
                            if( CErrLog::GetInstance() != nullptr )
                            {
                                CErrLog::GetInstance()->addEntry(errResultInfo);
                            }
                        }

                        delete m_pMsgCon;
                        m_pMsgCon = nullptr;

                        m_requestInProgress   = ERequestNone;
                        m_iReqIdInProgress    = -1;
                        m_iMsgIdReqInProgress = -1;

                        break;
                    } // case EMsgTypeReqShutdown

                    //------------------------------------------------------------------
                    case EMsgTypeReqConnect:
                    //------------------------------------------------------------------
                    {
                        CMsgReqConnect* pMsgReq = dynamic_cast<CMsgReqConnect*>(i_pMsg);

                        if( pMsgReq == nullptr )
                        {
                            throw CException(__FILE__, __LINE__, EResultMessageTypeMismatch, pMsg->getAddTrcInfoStr());
                        }
                        if( pMsgReq->getSender() != m_pClient && pMsgReq->getSender() != this )
                        {
                            throw CException(__FILE__, __LINE__, EResultInvalidSenderObjInMessage, pMsg->getAddTrcInfoStr());
                        }

                        bool bIsBlockingRequest = false;

                        if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(pMsgReq->getRequestId()) )
                        {
                            bIsBlockingRequest = true;
                        }

                        // Please note that the connect request message may also have been send from within the
                        // "onTmrConnectTimeout" method or from within the error method if the host did not respond
                        // but the timeout has not been expired.
                        if( pMsgReq->getSender() == m_pClient )
                        {
                            delete m_pMsgCon;
                            m_pMsgCon = nullptr;

                            if( pMsgReq->mustBeConfirmed() || bIsBlockingRequest )
                            {
                                m_pMsgCon = pMsgReq->createConfirmationMessage();
                            }
                        }

                        bool bRequestFinished = true;

                        if( m_requestInProgress != ERequestNone && m_requestInProgress != ERequestConnect )
                        {
                            strAddErrInfo = "RequestInProgress: " + Request2Str(m_requestInProgress) + ", " + pMsg->getAddTrcInfoStr();
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultMessageStateMismatch);
                            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                        }
                        else // if( m_requestInProgress == ERequestNone || m_requestInProgress == ERequestConnect )
                        {
                            // If I received the initial connect request from the client ..
                            if( pMsgReq->getSender() == m_pClient )
                            {
                                m_requestInProgress   = ERequestConnect;
                                m_iReqIdInProgress    = pMsgReq->getRequestId();
                                m_iMsgIdReqInProgress = pMsgReq->getMsgId();

                                SSocketDscr socketDscr = pMsgReq->getHostSettings().getSocketDscr();

                                if( pMsgReq->getWatchDogSettings() != nullptr )
                                {
                                    m_watchDogSettings = *pMsgReq->getWatchDogSettings();
                                }

                                if( socketDscr.m_socketType != m_socketDscr.m_socketType )
                                {
                                    strAddErrInfo = "Cannot change socket type via connect message.";
                                    errResultInfo.setSeverity(EResultSeverityError);
                                    errResultInfo.setResult(EResultMessageStateMismatch);
                                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                                }
                                else if( m_socketDscr.m_socketType == ESocketTypeTcp || m_socketDscr.m_socketType == ESocketTypeShm )
                                {
                                    m_socketDscr = socketDscr;

                                    if( m_pIpcSocketWrapper == nullptr )
                                    {
                                        strAddErrInfo = "Ipc Socket wrapper not created.";
                                        errResultInfo.setSeverity(EResultSeverityError);
                                        errResultInfo.setResult(EResultMessageStateMismatch);
                                        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                                    }
                                    else // if( m_pIpcSocketWrapper != nullptr )
                                    {
                                        if( m_pIpcSocketWrapper->socketState() != ESocketStateUnconnected )
                                        {
                                            m_pIpcSocketWrapper->abort();
                                        }
                                        m_pIpcSocketWrapper->setLocalHostName(m_socketDscr.m_strLocalHostName);
                                    }
                                } // if( m_socketDscr.m_socketType == ESocketTypeTcp || m_socketDscr.m_socketType == ESocketTypeShm )
                                else // if( m_socketDscr.m_socketType != ESocketTypeTcp && m_socketDscr.m_socketType ! ESocketTypeShm )
                                {
                                    m_socketDscr = socketDscr;

                                    m_socketDscr.m_pObjLocal = this;
                                    m_socketDscr.m_strLocalHostName = objectName();

                                    if( m_pIpcSocketWrapper != nullptr )
                                    {
                                        strAddErrInfo = "Ipc Socket wrapper created but no wrapper for " + socketType2Str(m_socketDscr.m_socketType) + " socket type needed.";
                                        errResultInfo.setSeverity(EResultSeverityError);
                                        errResultInfo.setResult(EResultMessageStateMismatch);
                                        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                                    }
                                } // if( m_socketDscr.m_socketType != ESocketTypeTcp && m_socketDscr.m_socketType ! ESocketTypeShm )
                            } // if( pMsgReq->getSender() == m_pClient )

                            if( m_pIpcSocketWrapper == nullptr ) // (SocketTypeInProcMsg)
                            {
                                CMsgReqConnect* pMsgReqConnect = dynamic_cast<CMsgReqConnect*>(pMsgReq->clone(ECopyDepth::Deep));
                                pMsgReqConnect->setSender(this);                            // the original sender was the database client
                                pMsgReqConnect->setReceiver(m_socketDscr.m_pObjRemote);     // the original receiver was this gateway
                                POST_OR_DELETE_MESSAGE(pMsgReqConnect, &mthTracer);
                                pMsgReqConnect = nullptr;
                                bRequestFinished = false;
                            }
                            else if( m_pIpcSocketWrapper != nullptr ) // (SocketTypeTcp || SocketTypeShm)
                            {
                                if( m_pIpcSocketWrapper->socketState() == ESocketStateConnecting )
                                {
                                    bRequestFinished = false;
                                }
                                else // if( m_pIpcSocketWrapper->socketState() != ESocketStateConnecting )
                                {
                                    // The slot "onConnected" will be called if the socket is connected.
                                    // The slot will send the confirmation message or wakes up the waiting request
                                    // and resets the request in progress to None.
                                    m_pIpcSocketWrapper->connectToHost( m_socketDscr.m_strRemoteHostName, m_socketDscr.m_uServerListenPort, m_socketDscr.m_uBufferSize );

                                    if( m_pIpcSocketWrapper->socketState() != ESocketStateConnected )
                                    {
                                        bRequestFinished = false;

                                        if( m_socketDscr.m_iConnectTimeout_ms > 0 )
                                        {
                                            // Start single shot timer:
                                            if( !m_pTmrConnect->isActive() )
                                            {
                                                m_pTmrConnect->start(m_socketDscr.m_iConnectTimeout_ms);
                                            }
                                        }
                                    } // if( m_pIpcSocketWrapper->socketState() != ESocketStateConnected )
                                } // if( m_pIpcSocketWrapper->socketState() != ESocketStateConnecting )
                            } // if( m_pIpcSocketWrapper != nullptr ) // (SocketTypeTcp || SocketTypeShm )
                        } // if( m_requestInProgress == ERequestNone || m_requestInProgress == ERequestConnect )

                        if( bRequestFinished )
                        {
                            if( m_pMsgCon != nullptr )
                            {
                                m_pMsgCon->setErrResultInfo(errResultInfo);
                                m_pMsgCon->setProgress(100);
                            }

                            if( bIsBlockingRequest )
                            {
                                // The thread starting object must have called "wait" on the
                                // wait condition. Otherwise "wakeAll" may never return. To ensure
                                // this we force this thread to sleep for a moment ...
                                // If the request is still alive (wait timeout may have occured
                                // and request instance may have been destroyed) ...
                                if( pReqExecTree->findRequest(m_iReqIdInProgress) != nullptr )
                                {
                                    while( !pReqExecTree->isWaiting(m_iReqIdInProgress) && !pReqExecTree->waitTimedOut(m_iReqIdInProgress) )
                                    {
                                        CSleeperThread::msleep(10);
                                    }
                                }

                                pReqExecTree->setExecutionConfirmationMessage(m_iReqIdInProgress, m_pMsgCon);
                                m_pMsgCon = nullptr;

                                if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                                {
                                    mthTracer.trace( "-+ ReqStartup.WaitCondition.wakeAll()" );
                                }

                                if( !pReqExecTree->wake(m_iReqIdInProgress) )
                                {
                                    errResultInfo.setSeverity(EResultSeverityError);
                                    errResultInfo.setResult(EResultInternalStateMachineError);
                                    errResultInfo.setAddErrInfoDscr("Waking up thread waiting for startup request failed");

                                    if( CErrLog::GetInstance() != nullptr )
                                    {
                                        CErrLog::GetInstance()->addEntry(errResultInfo);
                                    }
                                }

                                if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                                {
                                    mthTracer.trace( "+- ReqStartup.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
                                }
                            } // if( bIsBlockingRequest )

                            else if( m_pMsgCon != nullptr )
                            {
                                POST_OR_DELETE_MESSAGE(m_pMsgCon, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                m_pMsgCon = nullptr;
                            }
                            else if( errResultInfo.isErrorResult() )
                            {
                                if( CErrLog::GetInstance() != nullptr )
                                {
                                    CErrLog::GetInstance()->addEntry(errResultInfo);
                                }
                            }

                            delete m_pMsgCon;
                            m_pMsgCon = nullptr;

                            if( m_requestInProgress == ERequestConnect )
                            {
                                m_requestInProgress   = ERequestNone;
                                m_iReqIdInProgress    = -1;
                                m_iMsgIdReqInProgress = -1;
                            }
                        } // if( bRequestFinished )
                        break;
                    } // case EMsgTypeReqConnect

                    //------------------------------------------------------------------
                    case EMsgTypeReqDisconnect:
                    //------------------------------------------------------------------
                    {
                        CMsgReqDisconnect* pMsgReq = dynamic_cast<CMsgReqDisconnect*>(i_pMsg);

                        if( pMsgReq == nullptr )
                        {
                            throw CException(__FILE__, __LINE__, EResultMessageTypeMismatch, pMsg->getAddTrcInfoStr());
                        }
                        if( pMsgReq->getSender() != m_pClient )
                        {
                            throw CException(__FILE__, __LINE__, EResultInvalidSenderObjInMessage, pMsg->getAddTrcInfoStr());
                        }

                        delete m_pMsgCon;
                        m_pMsgCon = nullptr;

                        bool bIsBlockingRequest = false;

                        if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(pMsgReq->getRequestId()) )
                        {
                            bIsBlockingRequest = true;
                        }

                        if( pMsgReq->mustBeConfirmed() || bIsBlockingRequest )
                        {
                            m_pMsgCon = pMsgReq->createConfirmationMessage();
                        }

                        bool bRequestFinished = true;

                        if( m_requestInProgress != ERequestNone )
                        {
                            strAddErrInfo = "RequestInProgress: " + Request2Str(m_requestInProgress) + ", " + pMsg->getAddTrcInfoStr();
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultMessageStateMismatch);
                            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                        }
                        else // if( m_requestInProgress == ERequestNone )
                        {
                            m_requestInProgress   = ERequestDisconnect;
                            m_iReqIdInProgress    = pMsgReq->getRequestId();
                            m_iMsgIdReqInProgress = pMsgReq->getMsgId();

                            if( m_pTmrWatchDog->isActive() )
                            {
                                m_pTmrWatchDog->stop();
                            }

                            if( m_socketDscr.m_socketType == ESocketTypeTcp || m_socketDscr.m_socketType == ESocketTypeShm )
                            {
                                if( m_pIpcSocketWrapper == nullptr )
                                {
                                    strAddErrInfo = "Ipc Socket wrapper not created.";
                                    errResultInfo.setSeverity(EResultSeverityError);
                                    errResultInfo.setResult(EResultMessageStateMismatch);
                                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                                }
                                else // if( m_pIpcSocketWrapper != nullptr )
                                {
                                    // Either the "disconnected" or "error" signal will be emitted by the
                                    // sockets calling the "disconnectFromHost" or "abort" methods.
                                    if( m_pIpcSocketWrapper->socketState() == ESocketStateConnected )
                                    {
                                        // The slot "onDisconnected" will be called if the socket is disconnected.
                                        // The slot will send the confirmation message or wakes up the waiting request
                                        // and resets the request in progress to None.
                                        m_pIpcSocketWrapper->disconnectFromHost();

                                    } // if( m_pIpcSocketWrapper->socketState() == ESocketStateConnected )

                                    else if( m_pIpcSocketWrapper->socketState() == ESocketStateConnecting )
                                    {
                                        m_pIpcSocketWrapper->abort();

                                    } // if( m_pIpcSocketWrapper->socketState() == ESocketStateConnecting )

                                    if( m_pIpcSocketWrapper->socketState() != ESocketStateUnconnected )
                                    {
                                        bRequestFinished = false;
                                    }
                                } // if( m_pIpcSocketWrapper != nullptr )
                            } // if( m_socketDscr.m_socketType == ESocketTypeTcp || m_socketDscr.m_socketType == ESocketTypeShm )
                            else // if( m_socketDscr.m_socketType != ESocketTypeTcp && m_socketDscr.m_socketType ! ESocketTypeShm )
                            {
                                if( m_pIpcSocketWrapper != nullptr )
                                {
                                    strAddErrInfo = "Ipc Socket wrapper created but no wrapper for " + socketType2Str(m_socketDscr.m_socketType) + " socket type needed.";
                                    errResultInfo.setSeverity(EResultSeverityError);
                                    errResultInfo.setResult(EResultMessageStateMismatch);
                                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                                }
                                else // if( m_pIpcSocketWrapper == nullptr ) // (SocketTypeInProcMsg)
                                {
                                    CMsgReqDisconnect* pMsgReqDisconnect = dynamic_cast<CMsgReqDisconnect*>(pMsgReq->clone(ECopyDepth::Deep));
                                    pMsgReqDisconnect->setSender(this);                         // the original sender was the database client
                                    pMsgReqDisconnect->setReceiver(m_socketDscr.m_pObjRemote);  // the original receiver was this gateway
                                    POST_OR_DELETE_MESSAGE(pMsgReqDisconnect, &mthTracer);
                                    pMsgReqDisconnect = nullptr;
                                    bRequestFinished = false;
                                }
                            } // if( m_socketDscr.m_socketType != ESocketTypeTcp && m_socketDscr.m_socketType ! ESocketTypeShm )
                        } // if( m_requestInProgress == ERequestNone )

                        if( bRequestFinished )
                        {
                            if( m_pMsgCon != nullptr )
                            {
                                m_pMsgCon->setErrResultInfo(errResultInfo);
                                m_pMsgCon->setProgress(100);
                            }

                            if( bIsBlockingRequest )
                            {
                                // The thread starting object must have called "wait" on the
                                // wait condition. Otherwise "wakeAll" may never return. To ensure
                                // this we force this thread to sleep for a moment ...
                                // If the request is still alive (wait timeout may have occured
                                // and request instance may have been destroyed) ...
                                if( pReqExecTree->findRequest(m_iReqIdInProgress) != nullptr )
                                {
                                    while( !pReqExecTree->isWaiting(m_iReqIdInProgress) && !pReqExecTree->waitTimedOut(m_iReqIdInProgress) )
                                    {
                                        CSleeperThread::msleep(10);
                                    }
                                }

                                pReqExecTree->setExecutionConfirmationMessage(m_iReqIdInProgress, m_pMsgCon);
                                m_pMsgCon = nullptr;

                                if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                                {
                                    mthTracer.trace( "-+ ReqStartup.WaitCondition.wakeAll()" );
                                }

                                if( !pReqExecTree->wake(m_iReqIdInProgress) )
                                {
                                    errResultInfo.setSeverity(EResultSeverityError);
                                    errResultInfo.setResult(EResultInternalStateMachineError);
                                    errResultInfo.setAddErrInfoDscr("Waking up thread waiting for startup request failed");

                                    if( CErrLog::GetInstance() != nullptr )
                                    {
                                        CErrLog::GetInstance()->addEntry(errResultInfo);
                                    }
                                }

                                if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                                {
                                    mthTracer.trace( "+- ReqStartup.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
                                }
                            } // if( bIsBlockingRequest )

                            else if( m_pMsgCon != nullptr )
                            {
                                POST_OR_DELETE_MESSAGE(m_pMsgCon, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                m_pMsgCon = nullptr;
                            }
                            else if( errResultInfo.isErrorResult() )
                            {
                                if( CErrLog::GetInstance() != nullptr )
                                {
                                    CErrLog::GetInstance()->addEntry(errResultInfo);
                                }
                            }

                            delete m_pMsgCon;
                            m_pMsgCon = nullptr;

                            if( m_requestInProgress == ERequestDisconnect )
                            {
                                m_requestInProgress   = ERequestNone;
                                m_iReqIdInProgress    = -1;
                                m_iMsgIdReqInProgress = -1;
                            }
                        } // if( RequestFinished )
                        break;
                    } // case EMsgTypeReqDisconnect;

                    //------------------------------------------------------------------
                    case EMsgTypeReqChangeSettings:
                    //------------------------------------------------------------------
                    {
                        ZS::Data::CMsgReqChangeSettings* pMsgReq = dynamic_cast<ZS::Data::CMsgReqChangeSettings*>(i_pMsg);

                        if( pMsgReq == nullptr )
                        {
                            throw CException(__FILE__, __LINE__, EResultMessageTypeMismatch, pMsg->getAddTrcInfoStr());
                        }
                        if( pMsgReq->getSender() != m_pClient )
                        {
                            throw CException(__FILE__, __LINE__, EResultInvalidSenderObjInMessage, pMsg->getAddTrcInfoStr());
                        }

                        delete m_pMsgCon;
                        m_pMsgCon = nullptr;

                        bool bIsBlockingRequest = false;

                        if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(pMsgReq->getRequestId()) )
                        {
                            bIsBlockingRequest = true;
                        }

                        if( pMsgReq->mustBeConfirmed() || bIsBlockingRequest )
                        {
                            m_pMsgCon = pMsgReq->createConfirmationMessage();
                        }

                        if( m_requestInProgress != ERequestNone )
                        {
                            strAddErrInfo = "RequestInProgress: " + Request2Str(m_requestInProgress) + ", " + pMsg->getAddTrcInfoStr();
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultMessageStateMismatch);
                            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                        }
                        else // if( m_requestInProgress == ERequestNone )
                        {
                            SClientHostSettings hostSettingsNew = pMsgReq->getClientHostSettings();
                            SClientHostSettings hostSettingsOld = m_socketDscr;

                            bool bWatchDogSettingsChanged = false;

                            m_requestInProgress   = ERequestUpdateHostSettings;
                            m_iReqIdInProgress    = pMsgReq->getRequestId();
                            m_iMsgIdReqInProgress = pMsgReq->getMsgId();

                            if( pMsgReq->getWatchDogSettings() != nullptr )
                            {
                                if( *pMsgReq->getWatchDogSettings() != m_watchDogSettings )
                                {
                                    m_watchDogSettings = *pMsgReq->getWatchDogSettings();
                                    bWatchDogSettingsChanged = true;
                                }
                            }

                            if( hostSettingsNew == hostSettingsOld )
                            {
                                if( bWatchDogSettingsChanged )
                                {
                                    if( m_pTmrWatchDog->isActive() )
                                    {
                                        m_pTmrWatchDog->stop();
                                    }
                                    if( m_watchDogSettings.m_bEnabled && m_watchDogSettings.m_iInterval_ms > 0 )
                                    {
                                        m_pTmrWatchDog->start(m_watchDogSettings.m_iInterval_ms);
                                    }
                                }
                            }
                            else // if( hostSettingsNew != hostSettingsOld )
                            {
                                if( m_pTmrWatchDog->isActive() )
                                {
                                    m_pTmrWatchDog->stop();
                                }

                                // Important to set it before disconnecting from the remote host.
                                // The disconnected slot will be called immediately starting the
                                // change host settings request and using and transferring the
                                // socket parameters to the socket.
                                hostSettingsNew.toSocketDscr(m_socketDscr);

                                if( m_pIpcSocketWrapper != nullptr )
                                {
                                    if( m_pIpcSocketWrapper->socketType() != m_socketDscr.m_socketType )
                                    {
                                        if( m_pIpcSocketWrapper->socketState() != ESocketStateUnconnected )
                                        {
                                            m_pIpcSocketWrapper->abort();
                                        }

                                        try
                                        {
                                            delete m_pIpcSocketWrapper;
                                        }
                                        catch(...)
                                        {
                                        }
                                        m_pIpcSocketWrapper = nullptr;

                                    } // if( m_pIpcSocketWrapper->socketType() != m_socketDscr.m_socketType )

                                    else // if( m_pIpcSocketWrapper->socketType() == m_socketDscr.m_socketType )
                                    {
                                        if( m_pIpcSocketWrapper->getRemoteHostName() != m_socketDscr.m_strRemoteHostName
                                         || m_pIpcSocketWrapper->getRemoteHostAddress() != m_socketDscr.m_hostAddrRemote
                                         || m_pIpcSocketWrapper->getServerListenPort() != m_socketDscr.m_uServerListenPort )
                                        {
                                            if( m_pIpcSocketWrapper->socketState() == ESocketStateConnected )
                                            {
                                                m_pIpcSocketWrapper->disconnectFromHost();
                                            }
                                            else if( m_pIpcSocketWrapper->socketState() == ESocketStateConnecting )
                                            {
                                                m_pIpcSocketWrapper->abort();
                                            }
                                        } // if( m_pIpcSocketWrapper->getRemoteHostName() != m_socketDscr.m_strRemoteHostName ..
                                    } // if( m_pIpcSocketWrapper->socketType() == m_socketDscr.m_socketType )
                                } // if( m_pIpcSocketWrapper != nullptr )

                                m_socketDscr = hostSettingsNew.getSocketDscr();

                                if( m_pIpcSocketWrapper == nullptr && m_socketDscr.m_socketType == ESocketTypeTcp )
                                {
                                    m_pIpcSocketWrapper = new CTcpSocketWrapper( objectName(), ESrvCltTypeClient, 0, nullptr );
                                }
                                else if( m_pIpcSocketWrapper == nullptr && m_socketDscr.m_socketType == ESocketTypeShm )
                                {
                                    m_pIpcSocketWrapper = new CShmSocketWrapper( objectName(), ESrvCltTypeClient, 0 );
                                }

                                if( m_pIpcSocketWrapper != nullptr )
                                {
                                    m_pIpcSocketWrapper->setLocalHostName(m_socketDscr.m_strLocalHostName);

                                    if( !QObject::connect(
                                        /* pObjSender   */ m_pIpcSocketWrapper,
                                        /* szSignal     */ SIGNAL(connected(QObject*)),
                                        /* pObjReceiver */ this,
                                        /* szSlot       */ SLOT(onConnected(QObject*)) ) )
                                    {
                                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                                    }
                                    if( !QObject::connect(
                                        /* pObjSender   */ m_pIpcSocketWrapper,
                                        /* szSignal     */ SIGNAL(disconnected(QObject*)),
                                        /* pObjReceiver */ this,
                                        /* szSlot       */ SLOT(onDisconnected(QObject*)) ) )
                                    {
                                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                                    }
                                    if( !QObject::connect(
                                        /* pObjSender   */ m_pIpcSocketWrapper,
                                        /* szSignal     */ SIGNAL(readyRead(QObject*)),
                                        /* pObjReceiver */ this,
                                        /* szSlot       */ SLOT(onReadyRead(QObject*)) ) )
                                    {
                                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                                    }
                                    if( !QObject::connect(
                                        /* pObjSender   */ m_pIpcSocketWrapper,
                                        /* szSignal     */ SIGNAL(error(QObject*,ZS::System::SErrResultInfo&)),
                                        /* pObjReceiver */ this,
                                        /* szSlot       */ SLOT(onError(QObject*,ZS::System::SErrResultInfo&)) ) )
                                    {
                                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                                    }
                                } // if( m_pIpcSocketWrapper != nullptr )
                            } // if( hostSettingsNew != hostSettingsOld )
                        } // if( m_requestInProgress == ERequestNone )

                        if( m_pMsgCon != nullptr )
                        {
                            m_pMsgCon->setErrResultInfo(errResultInfo);
                            m_pMsgCon->setProgress(100);
                        }

                        if( bIsBlockingRequest )
                        {
                            // The thread starting object must have called "wait" on the
                            // wait condition. Otherwise "wakeAll" may never return. To ensure
                            // this we force this thread to sleep for a moment ...
                            // If the request is still alive (wait timeout may have occured
                            // and request instance may have been destroyed) ...
                            if( pReqExecTree->findRequest(m_iReqIdInProgress) != nullptr )
                            {
                                while( !pReqExecTree->isWaiting(m_iReqIdInProgress) && !pReqExecTree->waitTimedOut(m_iReqIdInProgress) )
                                {
                                    CSleeperThread::msleep(10);
                                }
                            }

                            pReqExecTree->setExecutionConfirmationMessage(m_iReqIdInProgress, m_pMsgCon);
                            m_pMsgCon = nullptr;

                            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                            {
                                mthTracer.trace( "-+ ReqChangeSettings.WaitCondition.wakeAll()" );
                            }

                            if( !pReqExecTree->wake(m_iReqIdInProgress) )
                            {
                                errResultInfo.setSeverity(EResultSeverityError);
                                errResultInfo.setResult(EResultInternalStateMachineError);
                                errResultInfo.setAddErrInfoDscr("Waking up thread waiting for change settings request failed");

                                if( CErrLog::GetInstance() != nullptr )
                                {
                                    CErrLog::GetInstance()->addEntry(errResultInfo);
                                }
                            }

                            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                            {
                                mthTracer.trace( "+- ReqChangeSettings.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
                            }
                        } // if( bIsBlockingRequest )

                        else if( m_pMsgCon != nullptr )
                        {
                            POST_OR_DELETE_MESSAGE(m_pMsgCon, &mthTracer, ETraceDetailLevelRuntimeInfo);
                            m_pMsgCon = nullptr;
                        }
                        else if( errResultInfo.isErrorResult() )
                        {
                            if( CErrLog::GetInstance() != nullptr )
                            {
                                CErrLog::GetInstance()->addEntry(errResultInfo);
                            }
                        }

                        delete m_pMsgCon;
                        m_pMsgCon = nullptr;

                        if( m_requestInProgress == ERequestUpdateHostSettings )
                        {
                            m_requestInProgress   = ERequestNone;
                            m_iReqIdInProgress    = -1;
                            m_iMsgIdReqInProgress = -1;
                        }
                        break;
                    } // case EMsgTypeReqChangeSettings

                    default:
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultInvalidMessage);
                        errResultInfo.setAddErrInfoDscr( pMsg->msgTypeToStr() );

                        if( CErrLog::GetInstance() != nullptr )
                        {
                            CErrLog::GetInstance()->addEntry(errResultInfo);
                        }
                        break;
                    }
                } // switch( pMsg->type() )
            } // if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeReq )

            else if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeInd )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultInvalidMessage);
                errResultInfo.setAddErrInfoDscr( pMsg->msgTypeToStr() );

                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
            } // if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeInd )

            else if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeCon )
            {
                switch( static_cast<int>(pMsg->type()) )
                {
                    case EMsgTypeConConnect:
                    {
                        ZS::Data::CMsgConConnect* pMsgCon = dynamic_cast<ZS::Data::CMsgConConnect*>(i_pMsg);

                        if( pMsgCon == nullptr )
                        {
                            throw CException(__FILE__, __LINE__, EResultMessageTypeMismatch, pMsg->getAddTrcInfoStr());
                        }
                        if( pMsgCon->getSender() != m_socketDscr.m_pObjRemote )
                        {
                            throw CException(__FILE__, __LINE__, EResultInvalidSenderObjInMessage, pMsg->getAddTrcInfoStr());
                        }

                        if( pMsgCon->getErrResultInfo().isErrorResult() )
                        {
                            SErrResultInfo errResultInfo = pMsgCon->getErrResultInfo();
                            onError(errResultInfo);
                        }
                        else // if( !pMsgCon->getErrResultInfo().isErrorResult() )
                        {
                            onConnected();
                        }
                        break;
                    } // case EMsgTypeConConnect

                    case EMsgTypeConDisconnect:
                    {
                        ZS::Data::CMsgConDisconnect* pMsgCon = dynamic_cast<ZS::Data::CMsgConDisconnect*>(i_pMsg);

                        if( pMsgCon == nullptr )
                        {
                            throw CException(__FILE__, __LINE__, EResultMessageTypeMismatch, pMsg->getAddTrcInfoStr());
                        }
                        if( pMsgCon->getSender() != m_socketDscr.m_pObjRemote )
                        {
                            throw CException(__FILE__, __LINE__, EResultInvalidSenderObjInMessage, pMsg->getAddTrcInfoStr());
                        }

                        if( pMsgCon->getErrResultInfo().isErrorResult() )
                        {
                            SErrResultInfo errResultInfo = pMsgCon->getErrResultInfo();
                            onError(errResultInfo);
                        }
                        else // if( !pMsgCon->getErrResultInfo().isErrorResult() )
                        {
                            onDisconnected();
                        }
                        break;
                    } // case EMsgTypeConDisconnect

                    default:
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultInvalidMessage);
                        errResultInfo.setAddErrInfoDscr( pMsg->msgTypeToStr() );

                        if( CErrLog::GetInstance() != nullptr )
                        {
                            CErrLog::GetInstance()->addEntry(errResultInfo);
                        }
                        break;
                    }
                } // switch( pMsg->type() )
            } // if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeCon )

            else // if( pMsg->getSystemMsgType() != ZS::System::MsgProtocol::ESystemMsgTypeAck )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultInvalidMessage);
                errResultInfo.setAddErrInfoDscr( pMsg->msgTypeToStr() );

                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
            }
        } // if( pMsg != nullptr )
    } // if( i_pMsg->type() >= QEvent::User )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pMsg);
    }
    return bEventHandled;

} // event
