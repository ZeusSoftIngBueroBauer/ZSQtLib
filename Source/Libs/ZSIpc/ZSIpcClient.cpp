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
#include <QtCore/qsettings.h>
#include <QtCore/qtimer.h>
#include <QtCore/qwaitcondition.h>
#include <QtNetwork/qhostinfo.h>

#include "ZSIpc/ZSIpcClient.h"
#include "ZSIpc/ZSIpcClientGateway.h"
#include "ZSIpc/ZSIpcBlkTypeL.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMutex.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysRequestQueue.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcMthFile.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;


/*******************************************************************************
class CClient : public QObject
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrStates[CClient::EStateCount] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( CClient::EStateUnconnected, "Unconnected", "Unconnected" ),
    /* 1 */ SEnumEntry( CClient::EStateConnected,   "Connected",   "Connected"   )
};

//------------------------------------------------------------------------------
QString CClient::State2Str( int i_iState, bool i_bDetailed )
//------------------------------------------------------------------------------
{
    QString strState;

    if( i_bDetailed )
    {
        strState = ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrStates,EStateCount,i_iState,0);
    }
    else
    {
        strState = ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrStates,EStateCount,i_iState,1);
    }
    return strState;

} // state2Str

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrRequests[CClient::ERequestCount] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( CClient::ERequestNone,               "None"               ),
    /* 1 */ SEnumEntry( CClient::ERequestConnect,            "Connect"            ),
    /* 2 */ SEnumEntry( CClient::ERequestDisconnect,         "Disconnect"         ),
    /* 3 */ SEnumEntry( CClient::ERequestChangeSettings,     "ChangeSettings"     ),
    /* 4 */ SEnumEntry( CClient::ERequestSendData,           "SendData"           ),
    /* 5 */ SEnumEntry( CClient::ERequestStartGatewayThread, "StartGatewayThread" ),
    /* 6 */ SEnumEntry( CClient::ERequestStopGatewayThread,  "StopGatewayThread"  ),
    /* 7 */ SEnumEntry( CClient::ERequestConnectGateway,     "ConnectGateway"     ),
    /* 8 */ SEnumEntry( CClient::ERequestDisconnectGateway,  "DisconnectGateway"  )
};

//------------------------------------------------------------------------------
QString CClient::Request2Str( int i_iRequest )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumStrRequests, _ZSArrLen(s_arEnumStrRequests), i_iRequest );
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates the client.

    @param i_strObjName [in] Name of the client.
    @param i_bMultiThreadedAccess [in] Default: false
        If true each access to member variables will be protected by a mutex
        (and the class becomes thread safe).
    @param i_eTrcMthFileDetailLevel [in]
        If trace outputs should not be forwarded to the trace server but directly
        to a trace method file allocated by the client, this detail level has to
        be to a value greater than None.
    @param i_eTrcMthFileDetailLevelMutex [in]
        If the locking and unlocking of the mutex of client
        should be logged a value greater than 0 (EMethodTraceDetailLevel::None)
        could be passed here. But the value will be ignored if the detail
        level for the client tracer is None.
    @param i_eTrcMthFileDetailLevelGateway [in]
        This is the trace method detail level forwarded to the gateway thread
        and to the gateway. This allows to enable/disable trace output for the
        gateway separately from the anchor class.
*/
CClient::CClient(
    const QString& i_strObjName,
    bool i_bMultiThreadedAccess,
    EMethodTraceDetailLevel i_eTrcMthFileDetailLevel,
    EMethodTraceDetailLevel i_eTrcMthFileDetailLevelMutex,
    EMethodTraceDetailLevel i_eTrcMthFileDetailLevelGateway ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pMtx(nullptr),
    m_strObjName(i_strObjName),
    m_pErrLog(nullptr),
    // Connection settings
    m_bOnlyLocalHostConnectionsAreAllowed(false),
    m_arSocketTypes(),
    m_hostSettings(),
    m_socketDscr(),
    m_bWatchDogTimerUsed(true),
    m_watchDogTimerSettings(
        /* bEnabled     */ false,
        /* iInterval_ms */ 10000,
        /* iTimeout_ms  */ 5000 ),
    m_pBlkType(nullptr),
    // Gateway
    m_pGatewayThread(nullptr),
    m_pGateway(nullptr),
    // State Machine
    m_state(EStateUnconnected),
    m_bReqExecTreeCreated(false),
    m_pRequestQueue(nullptr),
    m_reqDscrTimeout(),
    m_pTmrReqTimeout(nullptr),
    m_bMsgReqContinuePending(false),
    m_bIsBeingDestroyed(false),
    // Tracing
    m_arpTrcMsgLogObjects(),
    m_eTrcMthFileDetailLevel(i_eTrcMthFileDetailLevel),
    m_eTrcMthFileDetailLevelGateway(i_eTrcMthFileDetailLevelGateway),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
{
    if( m_strObjName.isEmpty() )
    {
        m_strObjName = "IpcClient";
    }
    setObjectName(m_strObjName);

    // If this is the trace client (maybe used in a test) tracing through the trace server
    // is not possible as that would lead to deadlocks and endless recursions. The Client
    // may want to trace a method call and sends it through the trace server to itself
    // whereupon a method should be trace (receiving data) which again should be traced
    // through the trace server and so on ....
    if( m_strObjName.endsWith("TrcClient") )
    {
        QString strLocalTrcFileAbsFilePath = CTrcServer::GetLocalTrcFileAbsoluteFilePath();
        m_pTrcMthFile = CTrcMthFile::Alloc(strLocalTrcFileAbsFilePath);
    }
    else
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());
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
        /* strMthInArgs       */ "" );

    if( i_bMultiThreadedAccess )
    {
        if( m_pTrcMthFile != nullptr )
        {
            m_pMtx = new CMutex(QMutex::Recursive, ClassName() + "-" + i_strObjName, i_eTrcMthFileDetailLevelMutex);
        }
        else
        {
            m_pMtx = new CMutex(QMutex::Recursive, ClassName() + "-" + i_strObjName);
        }
    }

    m_pErrLog = CErrLog::GetInstance();

    // Connection settings
    //--------------------

    m_arSocketTypes.append(ESocketTypeTcp);

    m_hostSettings.m_socketType = ESocketTypeTcp;
    m_hostSettings.m_strRemoteHostName = "---";
    m_hostSettings.m_uRemotePort = 24763;
    m_hostSettings.m_iConnectTimeout_ms = 30000;
    m_hostSettings.m_uBufferSize = 4096;

    m_pBlkType = new CBlkTypeL(EBlkLenDataTypeBinUINT32);

    // State Machine
    //--------------

    if( CRequestExecTree::GetInstance() == nullptr )
    {
        CRequestExecTree::CreateInstance();
        m_bReqExecTreeCreated = true;
    }

    m_pRequestQueue = new CRequestQueue(
        /* strCreatorNameSpace    */ "ZS::Ipc",
        /* strCreatorClassName    */ "CClient",
        /* pObjParent             */ this,
        /* bKeepReqDscrInExecTree */ false );

    m_pTmrReqTimeout = new QTimer(this);

    // Please note that if called from within different thread contexts signal/slot
    // connections for requests cannot be queued as otherwise on updating a request the
    // "requestChanged" slot may be called sometimes later not updating the internal states
    // at the expected time (before e.g. the "startup" method returns but sometimes later).

    if( !QObject::connect(
        /* pObjSender   */ m_pTmrReqTimeout,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestTimeout()),
        /* cnctType     */ m_pMtx != nullptr ? Qt::DirectConnection : Qt::AutoConnection) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CClient::~CClient()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strAddInfo         */ strAddTrcInfo );

    m_bIsBeingDestroyed = true;

    abortAllRequests(); // Deletes or at least invalidates the current request in progress.

    stopGatewayThread();

    // Delete the request queue before the mutex as on deleting the queue
    // requests postponed to be deleted later may be deleted and the
    // "onRequestChanged" slot may be called accessing the mutex.
    try
    {
        delete m_pRequestQueue;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pMtx;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pBlkType;
    }
    catch(...)
    {
    }

    if( m_pTrcMthFile != nullptr )
    {
        m_pTrcMthFile->close();
        CTrcMthFile::Free(m_pTrcMthFile);
    }

    // Don't emit any other signals than destroyed. The receiver of the
    // "disconnected" and "stateChanged" signals may not access the object
    // on receiving the signals if the object is being destroyed.
    //if( statePrev != m_state )
    //{
    //    emit stateChanged(this,m_state);
    //}

    mthTracer.onAdminObjAboutToBeReleased();

    if( m_pTrcAdminObj != nullptr )
    {
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }

    if( m_bReqExecTreeCreated )
    {
        CRequestExecTree::DestroyInstance();
    }

    m_pMtx = nullptr;
    //m_strObjName;
    m_pErrLog = nullptr;
    // Connection settings (depending on connection type)
    m_bOnlyLocalHostConnectionsAreAllowed = false;
    //m_arSocketTypes;
    //m_hostSettings;
    //m_socketDscr;
    m_bWatchDogTimerUsed = false;
    //m_watchDogTimerSettings;
    m_pBlkType = nullptr;
    // Gateway
    m_pGatewayThread = nullptr;
    m_pGateway = nullptr;
    // State Machine
    m_state = static_cast<EState>(0);
    m_bReqExecTreeCreated = false;
    m_pRequestQueue = nullptr;
    //m_reqDscrTimeout;
    m_pTmrReqTimeout = nullptr;
    m_bMsgReqContinuePending = false;
    m_bIsBeingDestroyed = false;
    // Tracing
    //m_arpTrcMsgLogObjects;
    m_eTrcMthFileDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
    m_eTrcMthFileDetailLevelGateway = static_cast<EMethodTraceDetailLevel>(0);
    m_pTrcMthFile = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CClient::setKeepReqDscrInExecTree( bool i_bKeep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "Keep: " + bool2Str(i_bKeep);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setKeepReqDscrInExecTree",
        /* strAddInfo         */ strAddTrcInfo );

    if( m_pRequestQueue != nullptr )
    {
        m_pRequestQueue->setKeepReqDscrInExecTree(i_bKeep);
    }

} // setKeepReqDscrInExecTree

//------------------------------------------------------------------------------
bool CClient::keepReqDscrInExecTree() const
//------------------------------------------------------------------------------
{
    bool bKeep = false;

    if( m_pRequestQueue != nullptr )
    {
        bKeep = m_pRequestQueue->keepReqDscrInExecTree();
    }
    return bKeep;

} // keepReqDscrInExecTree

/*==============================================================================
public: // overridables of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CClient::connect_( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "connect_",
        /* strAddInfo         */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("connect");

    CRequest* pReq = m_pRequestQueue->createRequest(
        /* strNameSpace */ "ZS::Ipc",
        /* strClassName */ "CClient",
        /* iRequest     */ ERequestConnect,
        /* strRequest   */ Request2Str(ERequestConnect),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    // Please note that the "onRequestChanged" slot should also be connected
    // to the changed signal of the request if the request will be postponed
    // as postponed requests may have parent request which may be aborted.

    // Please also note that if called from within different thread contexts signal/slot
    // connections for requests cannot be queued as otherwise on updating a request the
    // "requestChanged" slot may be called sometimes later not updating the internal states
    // at the expected time (before e.g. the "startup" method returns but sometimes later).

    if( !QObject::connect(
        /* pObjSender   */ pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)),
        /* cnctType     */ m_pMtx != nullptr ? Qt::DirectConnection : Qt::AutoConnection) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    CMsgReqConnect* pMsgReq = new CMsgReqConnect(
        /* pObjSender       */ this,
        /* pObjReceiver     */ this, // must be set to m_pGateway on executing the request
        /* hostSettings     */ m_hostSettings,
        /* watchDogSettings */ m_watchDogTimerSettings,
        /* pBlkType         */ m_pBlkType, // will be cloned
        /* bMustBeConfirmed */ true,
        /* iReqId           */ pReq->getId() );
    pReq->setExecutionRequestMessage(pMsgReq);
    pMsgReq = nullptr;

    bool bIsParentReqInProgress = m_pRequestQueue->isRequestInProgress(i_iReqIdParent);
    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_pRequestQueue->isIdle() || bIsParentReqInProgress )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        if( pReq->isBlockingRequest() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Cannot wait if request is currently in progress. Don't mix blocking calls with non blocking calls." );

            if( m_pErrLog != nullptr )
            {
                m_pErrLog->addEntry(errResultInfo);
            }

            //m_pRequestQueue->lock(); // Not necessary as request queue is not idle.
            pReq->setErrResultInfo(errResultInfo);
            pReq->setProgressInPerCent(100);
            pReq->update();
            //m_pRequestQueue->unlock();

            // The request is finished and has to be deleted later.
            m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
        }
        else
        {
            EResult resultPostpone = m_pRequestQueue->postponeRequest(pReq);

            if( resultPostpone != EResultSuccess )
            {
                pReq->setSeverity(EResultSeverityError);
                pReq->setResult(resultPostpone);
                pReq->setAddErrInfoDscr( "Request could not have been queued." );
                pReq->setProgressInPerCent(100);
                pReq->update();

                // The request is finished and has to be deleted later.
                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
            }
        }
    } // if( bPostponeRequest )

    else // if( !bPostponeRequest )
    {
        executeConnectRequest(pReq);
    }

    // Please note that if executing this request did not become asynchronous the
    // "execute<>Request" method above has been called and the "onRequestChanged"
    // slot method has already been disconnected from the request's changed signal.

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = QString( pReq == nullptr ? "SUCCESS" : pReq->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // connect_

//------------------------------------------------------------------------------
CRequest* CClient::disconnect_( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "disconnect_",
        /* strAddInfo         */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("disconnect");

    CRequest* pReq = m_pRequestQueue->createRequest(
        /* strNameSpace */ "ZS::Ipc",
        /* strClassName */ "CClient",
        /* iRequest     */ ERequestDisconnect,
        /* strRequest   */ Request2Str(ERequestDisconnect),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    // Please note that the "onRequestChanged" slot should also be connected
    // to the changed signal of the request if the request will be postponed
    // as postponed requests may have parent request which may be aborted.

    // Please also note that if called from within different thread contexts signal/slot
    // connections for requests cannot be queued as otherwise on updating a request the
    // "requestChanged" slot may be called sometimes later not updating the internal states
    // at the expected time (before e.g. the "startup" method returns but sometimes later).

    if( !QObject::connect(
        /* pObjSender   */ pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)),
        /* cnctType     */ m_pMtx != nullptr ? Qt::DirectConnection : Qt::AutoConnection) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    CMsgReqDisconnect* pMsgReq = new CMsgReqDisconnect(
        /* pObjSender       */ this,
        /* pObjReceiver     */ this, // must be set to m_pGateway on executing the request
        /* socketDscr       */ m_socketDscr,
        /* bMustBeConfirmed */ true,
        /* iReqId           */ pReq->getId() );
    pReq->setExecutionRequestMessage(pMsgReq);
    pMsgReq = nullptr;

    bool bIsParentReqInProgress = m_pRequestQueue->isRequestInProgress(i_iReqIdParent);
    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_pRequestQueue->isIdle() || bIsParentReqInProgress )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        if( pReq->isBlockingRequest() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Cannot wait if request is currently in progress. Don't mix blocking calls with non blocking calls." );

            if( m_pErrLog != nullptr )
            {
                m_pErrLog->addEntry(errResultInfo);
            }

            //m_pRequestQueue->lock(); // Not necessary as request queue is not idle.
            pReq->setErrResultInfo(errResultInfo);
            pReq->setProgressInPerCent(100);
            pReq->update();
            //m_pRequestQueue->unlock();

            // The request is finished and has to be deleted later.
            m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
        }
        else
        {
            EResult resultPostpone = m_pRequestQueue->postponeRequest(pReq);

            if( resultPostpone != EResultSuccess )
            {
                pReq->setSeverity(EResultSeverityError);
                pReq->setResult(resultPostpone);
                pReq->setAddErrInfoDscr( "Request could not have been queued." );
                pReq->setProgressInPerCent(100);
                pReq->update();

                // The request is finished and has to be deleted later.
                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
            }
        }
    } // if( bPostponeRequest )

    else // if( !bPostponeRequest )
    {
        executeDisconnectRequest(pReq);
    }

    // Please note that if executing this request did not become asynchronous the
    // "execute<>Request" method above has been called and the "onRequestChanged"
    // slot method has already been disconnected from the request's changed signal.

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = QString( pReq == nullptr ? "SUCCESS" : pReq->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // disconnect_

/*==============================================================================
public: // overridables of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CClient::changeSettings( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "changeSettings",
        /* strAddInfo         */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("changeSettings");

    CRequest* pReq = m_pRequestQueue->createRequest(
        /* strNameSpace */ "ZS::Ipc",
        /* strClassName */ "CClient",
        /* iRequest     */ ERequestChangeSettings,
        /* strRequest   */ Request2Str(ERequestChangeSettings),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    // Please note that the "onRequestChanged" slot should also be connected
    // to the changed signal of the request if the request will be postponed
    // as postponed requests may have parent request which may be aborted.

    // Please also note that if called from within different thread contexts signal/slot
    // connections for requests cannot be queued as otherwise on updating a request the
    // "requestChanged" slot may be called sometimes later not updating the internal states
    // at the expected time (before e.g. the "startup" method returns but sometimes later).

    if( !QObject::connect(
        /* pObjSender   */ pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)),
        /* cnctType     */ m_pMtx != nullptr ? Qt::DirectConnection : Qt::AutoConnection) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    bool bIsParentReqInProgress = m_pRequestQueue->isRequestInProgress(i_iReqIdParent);
    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_pRequestQueue->isIdle() || bIsParentReqInProgress )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        if( pReq->isBlockingRequest() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Cannot wait if request is currently in progress. Don't mix blocking calls with non blocking calls." );

            if( m_pErrLog != nullptr )
            {
                m_pErrLog->addEntry(errResultInfo);
            }

            //m_pRequestQueue->lock(); // Not necessary as request queue is not idle.
            pReq->setErrResultInfo(errResultInfo);
            pReq->setProgressInPerCent(100);
            pReq->update();
            //m_pRequestQueue->unlock();

            // The request is finished and has to be deleted later.
            m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
        }
        else
        {
            EResult resultPostpone = m_pRequestQueue->postponeRequest(pReq);

            if( resultPostpone != EResultSuccess )
            {
                pReq->setSeverity(EResultSeverityError);
                pReq->setResult(resultPostpone);
                pReq->setAddErrInfoDscr( "Request could not have been queued." );
                pReq->setProgressInPerCent(100);
                pReq->update();

                // The request is finished and has to be deleted later.
                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
            }
        }
    } // if( bPostponeRequest )

    else // if( !bPostponeRequest )
    {
        executeChangeSettingsRequest(pReq);
    }

    // Please note that if executing this request did not become asynchronous the
    // "execute<>Request" method above has been called and the "onRequestChanged"
    // slot method has already been disconnected from the request's changed signal.

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = QString( pReq == nullptr ? "SUCCESS" : pReq->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // changeSettings

/*==============================================================================
public: // overridables of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CClient::sendData( const QByteArray& i_byteArr, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "ByteArr[" + QString::number(i_byteArr.size()) + "]";
        if( getMethodCallsTraceDetailLevel() < EMethodTraceDetailLevel::ArgsVerbose ) {
            strAddTrcInfo += "(" + truncateStringWithEllipsisInTheMiddle(byteArr2Str(i_byteArr), 30) + ")";
        } else {
            strAddTrcInfo += "(" + truncateStringWithEllipsisInTheMiddle(byteArr2Str(i_byteArr), 100) + ")";
        }
        strAddTrcInfo += ", Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "sendData",
        /* strAddInfo         */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("sendData");

    CRequest* pReq = m_pRequestQueue->createRequest(
        /* strNameSpace */ "ZS::Ipc",
        /* strClassName */ "CClient",
        /* iRequest     */ ERequestSendData,
        /* strRequest   */ Request2Str(ERequestSendData),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    // Please note that the "onRequestChanged" slot should also be connected
    // to the changed signal of the request if the request will be postponed
    // as postponed requests may have parent request which may be aborted.

    // Please also note that if called from within different thread contexts signal/slot
    // connections for requests cannot be queued as otherwise on updating a request the
    // "requestChanged" slot may be called sometimes later not updating the internal states
    // at the expected time (before e.g. the "startup" method returns but sometimes later).

    if( !QObject::connect(
        /* pObjSender   */ pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)),
        /* cnctType     */ m_pMtx != nullptr ? Qt::DirectConnection : Qt::AutoConnection) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( i_byteArr.size() == 0 )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "No data to send." );

        pReq->setErrResultInfo(errResultInfo);
        pReq->setProgressInPerCent(100);
        pReq->update();

        // The request is finished and has to be deleted later.
        m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
    }

    if( !errResultInfo.isErrorResult() )
    {
        CMsgReqSendData* pMsgReq = new CMsgReqSendData(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pGateway,
            /* iSocketId        */ m_socketDscr.m_iSocketId,
            /* byteArr          */ i_byteArr,
            /* bMustBeConfirmed */ false,
            /* iReqId           */ pReq->getId() );
        pReq->setExecutionRequestMessage(pMsgReq);
        pMsgReq = nullptr;

        bool bIsParentReqInProgress = m_pRequestQueue->isRequestInProgress(i_iReqIdParent);
        bool bPostponeRequest = true;

        // The request can be executed immediately if the request queue is idle (no request
        // in progress, queue not locked and no requests postponed) or
        // if the request method has been started as a child of the request in progress
        // (e.g. as part of a request sequence) ..
        if( m_pRequestQueue->isIdle() || bIsParentReqInProgress )
        {
            bPostponeRequest = false;
        }

        if( bPostponeRequest )
        {
            if( pReq->isBlockingRequest() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultRequestRefused);
                errResultInfo.setAddErrInfoDscr( "Cannot wait if request is currently in progress. Don't mix blocking calls with non blocking calls." );

                if( m_pErrLog != nullptr )
                {
                    m_pErrLog->addEntry(errResultInfo);
                }

                //m_pRequestQueue->lock(); // Not necessary as request queue is not idle.
                pReq->setErrResultInfo(errResultInfo);
                pReq->setProgressInPerCent(100);
                pReq->update();
                //m_pRequestQueue->unlock();

                // The request is finished and has to be deleted later.
                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
            }
            else
            {
                EResult resultPostpone = m_pRequestQueue->postponeRequest(pReq);

                if( resultPostpone != EResultSuccess )
                {
                    pReq->setSeverity(EResultSeverityError);
                    pReq->setResult(resultPostpone);
                    pReq->setAddErrInfoDscr( "Request could not have been queued." );
                    pReq->setProgressInPerCent(100);
                    pReq->update();

                    // The request is finished and has to be deleted later.
                    m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
                }
            }
        } // if( bPostponeRequest )

        else // if( !bPostponeRequest )
        {
            executeSendDataRequest(pReq);
        }
    } // if( !errResultInfo.isErrorResult() )

    // Please note that if executing this request did not become asynchronous the
    // "execute<>Request" method above has been called and the "onRequestChanged"
    // slot method has already been disconnected from the request's changed signal.

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(pReq);
    }

    return pReq;

} // sendData

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CClient::setOnlyLocalHostConnectionsAreAllowed( bool i_bAllowOnlyLocalHostConnections )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "Allowed: " + bool2Str(i_bAllowOnlyLocalHostConnections);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setOnlyLocalHostConnectionsAreAllowed",
        /* strAddInfo         */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_bOnlyLocalHostConnectionsAreAllowed != i_bAllowOnlyLocalHostConnections )
    {
        m_bOnlyLocalHostConnectionsAreAllowed = i_bAllowOnlyLocalHostConnections;

        if( m_bOnlyLocalHostConnectionsAreAllowed )
        {
            QString strLocalHostName = QHostInfo::localHostName();

            SClientHostSettings hostSettings = m_hostSettings;

            if( hostSettings.m_strRemoteHostName.compare("localhost",Qt::CaseInsensitive) != 0
             && hostSettings.m_strRemoteHostName.compare("127.0.0.1",Qt::CaseInsensitive) != 0
             && hostSettings.m_strRemoteHostName.compare(strLocalHostName,Qt::CaseInsensitive) != 0 )
            {
                hostSettings.m_strRemoteHostName = strLocalHostName;

                setHostSettings(hostSettings);
            }
        } // if( m_bOnlyLocalHostConnectionsAreAllowed )
    } // if( m_bOnlyLocalHostConnectionsAreAllowed != i_bAllowOnlyLocalHostConnections )

} // setOnlyLocalHostConnectionsAreAllowed

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QList<ESocketType> CClient::getSocketTypes() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return m_arSocketTypes;
}

//------------------------------------------------------------------------------
int CClient::getSocketTypesCount() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return m_arSocketTypes.size();
}

//------------------------------------------------------------------------------
ESocketType CClient::getSocketType( int i_idx ) const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    ESocketType socketType = ESocketTypeUndefined;

    if( i_idx >= 0 && i_idx < m_arSocketTypes.size() )
    {
        socketType = m_arSocketTypes[i_idx];
    }
    return socketType;

} // getSocketType

//------------------------------------------------------------------------------
bool CClient::isSocketTypeChangeable() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return (m_arSocketTypes.size() > 1);
}

//------------------------------------------------------------------------------
bool CClient::isSocketTypeSupported( ESocketType i_socketType ) const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    bool bIsSupported = false;
    int  idxSocketType;

    for( idxSocketType = 0; idxSocketType < m_arSocketTypes.size(); idxSocketType++ )
    {
        if( i_socketType == m_arSocketTypes[idxSocketType] )
        {
            bIsSupported = true;
            break;
        }
    }
    return bIsSupported;

} // isSocketTypeSupported

/*==============================================================================
public: // instance methods changing and reading the host settings
==============================================================================*/

//------------------------------------------------------------------------------
QString CClient::getConnectionString() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    SClientHostSettings hostSettings = getHostSettings();
    return hostSettings.getConnectionString();
}

//------------------------------------------------------------------------------
SClientHostSettings CClient::getHostSettings() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return m_hostSettings;
}

//------------------------------------------------------------------------------
void CClient::setHostSettings( const SClientHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ? 1 : 0;

        strAddTrcInfo = "HostSettings: " + i_hostSettings.getConnectionString(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setHostSettings",
        /* strAddInfo         */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);;

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_bOnlyLocalHostConnectionsAreAllowed )
    {
        QString strLocalHostName = QHostInfo::localHostName();

        if( i_hostSettings.m_strRemoteHostName.compare("localhost",Qt::CaseInsensitive) != 0
         && i_hostSettings.m_strRemoteHostName.compare("127.0.0.1",Qt::CaseInsensitive) != 0
         && i_hostSettings.m_strRemoteHostName.compare(strLocalHostName,Qt::CaseInsensitive) != 0 )
        {
            if( m_pErrLog != nullptr )
            {
                SErrResultInfo errResultInfo = ErrResultInfoError(
                    /* errSource  */ "setHostSettings",
                    /* result     */ EResultArgOutOfRange,
                    /* strAddInfo */ "Only local host connections are allowed" );
                m_pErrLog->addEntry(errResultInfo);
            }
            return;
        }
    } // if( m_bOnlyLocalHostConnectionsAreAllowed )

    bool bSocketTypeSupported = false;
    int  idxSocketType;

    for( idxSocketType = 0; idxSocketType < m_arSocketTypes.size(); idxSocketType++ )
    {
        if( i_hostSettings.m_socketType == m_arSocketTypes[idxSocketType] )
        {
            bSocketTypeSupported = true;
            break;
        }
    }

    if( !bSocketTypeSupported )
    {
        if( m_pErrLog != nullptr )
        {
            SErrResultInfo errResultInfo = ErrResultInfoError(
                /* errSource  */ "setHostSettings",
                /* result     */ EResultArgOutOfRange,
                /* strAddInfo */ "Socket type " + socketType2Str(i_hostSettings.m_socketType) + " is not supported" );
            m_pErrLog->addEntry(errResultInfo);
        }
        return;
    }

    if( i_hostSettings != m_hostSettings )
    {
        m_hostSettings = i_hostSettings;

        emit settingsChanged(this);
    }

} // setHostSettings

/*==============================================================================
public: // instance methods changing and reading the watch dog settings
==============================================================================*/

//------------------------------------------------------------------------------
void CClient::setWatchDogTimerUsed( bool i_bUsed )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = bool2Str(i_bUsed);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setWatchDogTimerUsed",
        /* strAddInfo         */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    // This method should only be called once right after creating the client.
    if( m_bWatchDogTimerUsed != i_bUsed )
    {
        if( m_bWatchDogTimerUsed )
        {
            m_watchDogTimerSettings.m_bEnabled = false;
        }

        m_bWatchDogTimerUsed = i_bUsed;

        if( m_bWatchDogTimerUsed )
        {
            throw CException(__FILE__, __LINE__, EResultMethodNotYetImplemented, "Cannot use watch dog timer if once set to unused");
        }

        emit settingsChanged(this);

    } // if( m_bWatchDogTimerUsed != i_bUsed )

} // setWatchDogTimerUsed

//------------------------------------------------------------------------------
bool CClient::isWatchDogTimerEnabled() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    STimerSettings watchDogSettings = getWatchDogSettings();
    return watchDogSettings.m_bEnabled;
}

//------------------------------------------------------------------------------
int CClient::getWatchDogTimerIntervalInMs() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    STimerSettings watchDogSettings = getWatchDogSettings();
    return watchDogSettings.m_iInterval_ms;
}

//------------------------------------------------------------------------------
int CClient::getWatchDogTimeoutInMs() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    STimerSettings watchDogSettings = getWatchDogSettings();
    return watchDogSettings.m_iTimeout_ms;
}

//------------------------------------------------------------------------------
STimerSettings CClient::getWatchDogSettings() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return m_watchDogTimerSettings;
}

//------------------------------------------------------------------------------
void CClient::setWatchDogSettings( const STimerSettings& i_settings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo  = "Enabled: " + bool2Str(i_settings.m_bEnabled);
        strAddTrcInfo += ", Interval: " + QString::number(i_settings.m_iInterval_ms);
        strAddTrcInfo += ", Timeout: " + QString::number(i_settings.m_iTimeout_ms) + " ms";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setWatchDogSettings",
        /* strAddInfo         */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_bWatchDogTimerUsed )
    {
        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Watch dog timer is not used");
    }

    if( m_watchDogTimerSettings != i_settings )
    {
        m_watchDogTimerSettings = i_settings;
        emit settingsChanged(this);
    }

} // setWatchDogSettings

/*==============================================================================
public: // instance methods of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
void CClient::setBlkType( CBlkType* i_pBlkType ) // The class takes ownership of the data block.
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "BlkType: " + QString( i_pBlkType == nullptr ? "nullptr" : blkType2Str(i_pBlkType->type()) );
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setBlkType",
        /* strAddInfo         */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_pBlkType != i_pBlkType )
    {
        delete m_pBlkType;
        m_pBlkType = i_pBlkType;
    }

    emit settingsChanged(this);

} // setBlkType

//------------------------------------------------------------------------------
CBlkType* CClient::getBlkType()
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return m_pBlkType;
}

/*==============================================================================
public: // instance methods of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
int CClient::getSocketId() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return m_socketDscr.m_iSocketId;
}

//------------------------------------------------------------------------------
SSocketDscr CClient::getSocketDscr() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return m_socketDscr;
}

//------------------------------------------------------------------------------
int CClient::getConnectTimeoutInMs() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return m_hostSettings.m_iConnectTimeout_ms;
}

/*==============================================================================
public: // instance methods (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
CClient::EState CClient::getState() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return m_state;
}

//------------------------------------------------------------------------------
QString CClient::state2Str() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return State2Str(m_state);
}

//------------------------------------------------------------------------------
bool CClient::isConnected() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    bool bIsConnected = false;

    switch( m_state )
    {
        case EStateUnconnected:
        {
            break;
        }
        case EStateConnected:
        default:
        {
            if( m_socketDscr.m_socketState == ESocketStateConnected )
            {
                bIsConnected = true;
            }
            break;
        }
    } // switch( m_state )

    return bIsConnected;

} // isConnected

/*==============================================================================
public: // instance methods (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
bool CClient::isBusy() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return !m_pRequestQueue->isIdle();
}

//------------------------------------------------------------------------------
CClient::ERequest CClient::requestInProgress() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    ERequest  request = ERequestNone;
    CRequest* pReq    = m_pRequestQueue->getRequestInProgress();

    if( pReq != nullptr )
    {
        request = static_cast<ERequest>(pReq->getRequest());
    }
    return request;

} // requestInProgress

//------------------------------------------------------------------------------
QString CClient::requestInProgress2Str( bool /*i_bShort*/ ) const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return Request2Str( requestInProgress() );
}

//------------------------------------------------------------------------------
CRequest* CClient::getRequestInProgress() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return m_pRequestQueue->getRequestInProgress();
}

/*==============================================================================
public: // instance methods (aborting requests)
==============================================================================*/

//------------------------------------------------------------------------------
void CClient::abortRequest( qint64 i_iRequestId )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "Request: " + CRequestExecTree::GetAddTrcInfoStr(i_iRequestId);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "abortRequest",
        /* strAddInfo         */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    CRequest* pReq = m_pRequestQueue->getRequestInProgress(i_iRequestId);

    if( pReq != nullptr )
    {
        m_pRequestQueue->lock();

        // The one who calls update for the finished request got to delete the request.
        // That's been done after the request has been aborted. Several tasks may still
        // have to be processed before the request is really finished and the request
        // will be deleted in the "onRequestChanged" slot.
        pReq->abort();

        m_pRequestQueue->unlock();

    } // if( pReq != nullptr }

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

} // abortRequest

//------------------------------------------------------------------------------
void CClient::abortRequestInProgress()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "abortRequestInProgress",
        /* strAddInfo         */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    CRequest* pReq = m_pRequestQueue->getRequestInProgress();

    if( pReq != nullptr )
    {
        abortRequest( pReq->getId() );
    }

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

} // abortRequestInProgress

//------------------------------------------------------------------------------
void CClient::abortAllRequests()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "abortAllRequests",
        /* strAddInfo         */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    CRequest* pReq;

    while( m_pRequestQueue->hasPostponedRequest() )
    {
        pReq = m_pRequestQueue->takeLastPostponedRequest();

        abortRequest( pReq->getId() );
    }

    if( m_pRequestQueue->isRequestInProgress() )
    {
        abortRequestInProgress();
    }

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

} // abortAllRequests

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CClient::addTrcMsgLogObject( QObject* i_pObj )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    if( i_pObj == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pObj == nullptr");
    }

    int idxObj = m_arpTrcMsgLogObjects.indexOf(i_pObj);

    if( idxObj < 0 )
    {
        m_arpTrcMsgLogObjects.append(i_pObj);
    }

    if( m_pGateway != nullptr )
    {
        if( requestInProgress() != ERequestDisconnect )
        {
            CClientGateway* pGateway = dynamic_cast<CClientGateway*>(m_pGateway);

            if( pGateway != nullptr )
            {
                pGateway->addTrcMsgLogObject(i_pObj);
            }
        }
    }

} // addTrcMsgLogObject

//------------------------------------------------------------------------------
void CClient::removeTrcMsgLogObject( QObject* i_pObj )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    if( i_pObj == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pObj == nullptr");
    }

    int idxObj = m_arpTrcMsgLogObjects.indexOf(i_pObj);

    if( idxObj >= 0 && idxObj < m_arpTrcMsgLogObjects.size() )
    {
        m_arpTrcMsgLogObjects.removeAt(idxObj);
    }

    if( m_pGateway != nullptr )
    {
        if( requestInProgress() != ERequestDisconnect )
        {
            CClientGateway* pGateway = dynamic_cast<CClientGateway*>(m_pGateway);

            if( pGateway != nullptr )
            {
                pGateway->removeTrcMsgLogObject(i_pObj);
            }
        }
    }

} // removeTrcMsgLogObject

/*==============================================================================
public: // instance methods to trace methods calls
==============================================================================*/

//------------------------------------------------------------------------------
void CClient::setMethodTraceDetailLevel( EMethodTraceDetailLevel i_eTrcDetailLevel )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel != i_eTrcDetailLevel )
    {
        m_eTrcMthFileDetailLevel = i_eTrcDetailLevel;

        if( m_eTrcMthFileDetailLevel > EMethodTraceDetailLevel::None && m_pTrcMthFile == nullptr )
        {
            QString strLocalTrcFileAbsFilePath = CTrcServer::GetLocalTrcFileAbsoluteFilePath();
            m_pTrcMthFile = CTrcMthFile::Alloc(strLocalTrcFileAbsFilePath);
        }
        else if( m_pTrcMthFile != nullptr )
        {
            CTrcMthFile::Free(m_pTrcMthFile);
            m_pTrcMthFile = nullptr;
        }
    }
}

/*==============================================================================
protected: // overridables of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
void CClient::onReceivedData( const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ByteArr [" + QString::number(i_byteArr.size()) + "]";
        if( getMethodCallsTraceDetailLevel() < EMethodTraceDetailLevel::ArgsVerbose ) {
            strMthInArgs += "(" + truncateStringWithEllipsisInTheMiddle(byteArr2Str(i_byteArr), 30) + ")";
        } else {
            strMthInArgs += "(" + truncateStringWithEllipsisInTheMiddle(byteArr2Str(i_byteArr), 100) + ")";
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onReceivedData",
        /* strAddInfo         */ strMthInArgs );

    // Not necessary as method only called internally if mutex is already locked:
    //CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;
        QString strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

} // onReceivedData

/*==============================================================================
protected: // instance methods of internal state machine
==============================================================================*/

//------------------------------------------------------------------------------
void CClient::executeNextPostponedRequest()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "executeNextPostponedRequest",
        /* strAddInfo         */ strAddTrcInfo );

    // Not necessary as method only called internally if mutex is already locked:
    //CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
    {
        if( !m_bMsgReqContinuePending )
        {
            m_bMsgReqContinuePending = true;

            CMsgReqContinue* pMsgReq = new CMsgReqContinue(
                /* pObjSender       */ this,
                /* pObjReceiver     */ this,
                /* bMustBeConfirmed */ false,
                /* iReqId           */ -1,
                /* iMsgId           */ -1 );
            POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ELogDetailLevel::Debug);
        }
    } // if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )

} // executeNextPostponedRequest

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CClient::executeConnectRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "executeConnectRequest",
        /* strAddInfo         */ strAddTrcInfo );

    // Not necessary as method only called internally if mutex is already locked:
    //CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SRequestDscr reqDscrPrev;

    CRequest* pReqInProgress = m_pRequestQueue->getRequestInProgress();

    if( pReqInProgress != nullptr )
    {
        reqDscrPrev = pReqInProgress->getDescriptor();
    }

    i_pReq->lock();

    m_pRequestQueue->lock();

    // If the request has been started as a child of the current request in progress ..
    if( m_pRequestQueue->isRequestInProgress() )
    {
        m_pRequestQueue->addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_pRequestQueue->setRequestInProgress(i_pReq);
    }

    emit requestInProgressChanged( this, i_pReq->getDescriptor() );

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    EState statePrev = m_state;

    bool bWasConnected = isConnected();

    // If the client is not connected ...
    if( m_state == EStateUnconnected )
    {
        m_pGatewayThread = createGatewayThread();

        errResultInfo = startGatewayThread(10000, i_pReq->getId());

        if( !errResultInfo.isErrorResult() )
        {
            CRequest* pReqConnectGatway = connectGateway(
                /* iTimeout_ms  */ i_pReq->getTimeoutInMs(),
                /* bWait        */ i_pReq->isBlockingRequest(),
                /* iReqIdParent */ i_pReq->getId() );

            if( !isAsynchronousRequest(pReqConnectGatway) )
            {
                i_pReq->setErrResultInfo( pReqConnectGatway->getErrResultInfo() );
                i_pReq->setProgressInPerCent(100);
                i_pReq->update();
            }
        } // if( !errResultInfo.isErrorResult() )
    } // if( m_state == EStateUnconnected )

    else // if( m_state != EStateUnconnected )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Client is already connected." );

    } // if( m_state != EStateUnconnected )

    if( errResultInfo.isErrorResult() )
    {
        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();
    }

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
        {
            m_pRequestQueue->removeRequestInProgress( i_pReq->getId() );
        }

        // Please note that the "onRequestChanged" slot is connected with the "changed"
        // signal of the request. If the request has been finished and updated a view
        // lines above the slot will be called disconnecting itself from the signal.

        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).
        m_pRequestQueue->setSyncRequestToBeDeletedLater(i_pReq);

    } // if( !isAsynchronousRequest(i_pReq) )

    SRequestDscr reqDscrCurr;

    pReqInProgress = m_pRequestQueue->getRequestInProgress();

    if( pReqInProgress != nullptr )
    {
        reqDscrCurr = pReqInProgress->getDescriptor();
    }

    if( reqDscrPrev != reqDscrCurr )
    {
        emit requestInProgressChanged(this, reqDscrCurr);
    }

    if( statePrev != m_state )
    {
        emit stateChanged(this, m_state);
    }

    if( bWasConnected && !isConnected() )
    {
        emit disconnected(this);
    }
    else if( !bWasConnected && isConnected() )
    {
        emit connected(this);
    }

    m_pRequestQueue->unlock();

    i_pReq->unlock();

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeConnectRequest

//------------------------------------------------------------------------------
void CClient::executeDisconnectRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "executeDisconnectRequest",
        /* strAddInfo         */ strAddTrcInfo );

    // Not necessary as method only called internally if mutex is already locked:
    //CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SRequestDscr reqDscrPrev;

    CRequest* pReqInProgress = m_pRequestQueue->getRequestInProgress();

    if( pReqInProgress != nullptr )
    {
        reqDscrPrev = pReqInProgress->getDescriptor();
    }

    i_pReq->lock();

    m_pRequestQueue->lock();

    // If the request has been started as a child of the current request in progress ..
    if( m_pRequestQueue->isRequestInProgress() )
    {
        m_pRequestQueue->addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_pRequestQueue->setRequestInProgress(i_pReq);
    }

    emit requestInProgressChanged( this, i_pReq->getDescriptor() );

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    EState statePrev = m_state;

    bool bWasConnected = isConnected();

    // If the client is connected ...
    if( m_state >= EStateConnected )
    {
        CRequest* pReqDisconnectGateway = disconnectGateway(
            /* iTimeout_ms  */ i_pReq->getTimeoutInMs(),
            /* bWait        */ i_pReq->isBlockingRequest(),
            /* iReqIdParent */ i_pReq->getId() );

        if( !isAsynchronousRequest(pReqDisconnectGateway) )
        {
            errResultInfo = stopGatewayThread(i_pReq->getTimeoutInMs(), i_pReq->getId());

            if( errResultInfo.isErrorResult() )
            {
                if( m_pErrLog != nullptr )
                {
                    m_pErrLog->addEntry(errResultInfo);
                }
            }

            i_pReq->setErrResultInfo( pReqDisconnectGateway->getErrResultInfo() );
            i_pReq->setProgressInPerCent(100);
            i_pReq->update();
        }
    } // if( m_state >= EStateConnected )

    else // if( m_state < EStateConnected )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Client is not connected." );

    } // if( m_state < EStateConnected )

    if( errResultInfo.isErrorResult() )
    {
        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();
    }

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
        {
            m_pRequestQueue->removeRequestInProgress( i_pReq->getId() );
        }

        // Please note that the "onRequestChanged" slot is connected with the "changed"
        // signal of the request. If the request has been finished and updated a view
        // lines above the slot will be called disconnecting itself from the signal.

        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).
        m_pRequestQueue->setSyncRequestToBeDeletedLater(i_pReq);

    } // if( !isAsynchronousRequest(i_pReq) )

    SRequestDscr reqDscrCurr;

    pReqInProgress = m_pRequestQueue->getRequestInProgress();

    if( pReqInProgress != nullptr )
    {
        reqDscrCurr = pReqInProgress->getDescriptor();
    }

    if( reqDscrPrev != reqDscrCurr )
    {
        emit requestInProgressChanged(this, reqDscrCurr);
    }

    if( statePrev != m_state )
    {
        emit stateChanged(this, m_state);
    }

    if( bWasConnected && !isConnected() )
    {
        emit disconnected(this);
    }
    else if( !bWasConnected && isConnected() )
    {
        emit connected(this);
    }

    m_pRequestQueue->unlock();

    i_pReq->unlock();

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeDisconnectRequest

//------------------------------------------------------------------------------
void CClient::executeChangeSettingsRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "executeChangeSettingsRequest",
        /* strAddInfo         */ strAddTrcInfo );

    // Not necessary as method only called internally if mutex is already locked:
    //CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SRequestDscr reqDscrPrev;

    CRequest* pReqInProgress = m_pRequestQueue->getRequestInProgress();

    if( pReqInProgress != nullptr )
    {
        reqDscrPrev = pReqInProgress->getDescriptor();
    }

    i_pReq->lock();

    m_pRequestQueue->lock();

    // If the request has been started as a child of the current request in progress ..
    if( m_pRequestQueue->isRequestInProgress() )
    {
        m_pRequestQueue->addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_pRequestQueue->setRequestInProgress(i_pReq);
    }

    emit requestInProgressChanged( this, i_pReq->getDescriptor() );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("executeChangeSettingsRequest");

    EState statePrev = m_state;

    bool bWasConnected = isConnected();

    bool bSettingsChanged = false;

    // If the client is not connected ...
    if( m_state == EStateUnconnected )
    {
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();

    } // if( m_state == EStateUnconnected )

    else // if( m_state != EStateUnconnected )
    {
        if( m_pGateway == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Gateway is not created." );
        }
        else if( !bSettingsChanged )
        {
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }

            i_pReq->setErrResultInfo(errResultInfo);
            i_pReq->setProgressInPerCent(100);
            i_pReq->update();
        }
        else // if( bSettingsChanged )
        {
            CMsgReqChangeSettings* pMsgReq = new CMsgReqChangeSettings(
                /* pObjSender       */ this,
                /* pObjReceiver     */ m_pGateway,
                /* hostSettings     */ m_hostSettings,
                /* watchDogSettings */ m_watchDogTimerSettings,
                /* bMustBeConfirmed */ true,
                /* iReqId           */ i_pReq->getId() );
            pMsgReq->setBlkType(m_pBlkType); // The block will be cloned.

            POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ELogDetailLevel::Debug);
            pMsgReq = nullptr;

            if( i_pReq->isBlockingRequest() )
            {
                if( i_pReq->wait() )
                {
                    CMsgCon* pMsgCon = i_pReq->getExecutionConfirmationMessage();
                    if( pMsgCon != nullptr )
                    {
                        errResultInfo = pMsgCon->getErrResultInfo();
                    }
                    else
                    {
                        errResultInfo = i_pReq->getErrResultInfo();
                    }
                }
                else // if( !i_pReq->wait() )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultTimeout);
                    errResultInfo.setAddErrInfoDscr( "Waiting for change settings confirmation from gateway timed out." );
                }
                if( errResultInfo.getResult() == EResultUndefined )
                {
                    errResultInfo.setErrResult(ErrResultSuccess);
                }
                i_pReq->setErrResultInfo(errResultInfo);
                i_pReq->setProgressInPerCent(100);
                i_pReq->update();
            }
        } // if( m_pGateway != nullptr )
    } // if( m_state != EStateUnconnected )

    if( errResultInfo.isErrorResult() )
    {
        if( m_pErrLog != nullptr )
        {
            m_pErrLog->addEntry(errResultInfo);
        }
        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();
    }

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
        {
            m_pRequestQueue->removeRequestInProgress( i_pReq->getId() );
        }

        // Please note that the "onRequestChanged" slot is connected with the "changed"
        // signal of the request. If the request has been finished and updated a view
        // lines above the slot will be called disconnecting itself from the signal.

        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).
        m_pRequestQueue->setSyncRequestToBeDeletedLater(i_pReq);

    } // if( !isAsynchronousRequest(i_pReq) )

    SRequestDscr reqDscrCurr;

    pReqInProgress = m_pRequestQueue->getRequestInProgress();

    if( pReqInProgress != nullptr )
    {
        reqDscrCurr = pReqInProgress->getDescriptor();
    }

    if( reqDscrPrev != reqDscrCurr )
    {
        emit requestInProgressChanged(this, reqDscrCurr);
    }

    if( statePrev != m_state )
    {
        emit stateChanged(this, m_state);
    }

    if( bWasConnected && !isConnected() )
    {
        emit disconnected(this);
    }
    else if( !bWasConnected && isConnected() )
    {
        emit connected(this);
    }

    m_pRequestQueue->unlock();

    i_pReq->unlock();

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeChangeSettingsRequest

//------------------------------------------------------------------------------
void CClient::executeSendDataRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "executeSendDataRequest",
        /* strAddInfo         */ strAddTrcInfo );

    // Not necessary as method only called internally if mutex is already locked:
    //CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SRequestDscr reqDscrPrev;

    CRequest* pReqInProgress = m_pRequestQueue->getRequestInProgress();

    if( pReqInProgress != nullptr )
    {
        reqDscrPrev = pReqInProgress->getDescriptor();
    }

    i_pReq->lock();

    m_pRequestQueue->lock();

    // If the request has been started as a child of the current request in progress ..
    if( m_pRequestQueue->isRequestInProgress() )
    {
        m_pRequestQueue->addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_pRequestQueue->setRequestInProgress(i_pReq);
    }

    emit requestInProgressChanged( this, i_pReq->getDescriptor() );

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    EState statePrev = m_state;

    bool bWasConnected = isConnected();

    // If the client is connected ...
    if( m_state >= EStateConnected )
    {
        if( m_pGateway == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Gateway is not created." );
        }
        else // if( m_pGateway != nullptr )
        {
            CMsgReqSendData* pMsgReq = dynamic_cast<CMsgReqSendData*>(i_pReq->getExecutionRequestMessage());
            i_pReq->setExecutionRequestMessage(nullptr);

            if( pMsgReq != nullptr )
            {
                pMsgReq->setReceiver(m_pGateway);
                pMsgReq->setRequestId(i_pReq->getId());

                POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ELogDetailLevel::Debug);
                pMsgReq = nullptr;
            }

            errResultInfo.setErrResult(ErrResultSuccess);

            i_pReq->setErrResultInfo(errResultInfo);
            i_pReq->setProgressInPerCent(100);
            i_pReq->update();

        } // if( m_pGateway != nullptr )
    } // if( m_state >= EStateConnected )

    // If the client is not connected ...
    else // if( m_state < EStateConnected )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Client is not connected." );

    } // if( m_state < EStateConnected )

    if( errResultInfo.isErrorResult() )
    {
        if( m_pErrLog != nullptr )
        {
            m_pErrLog->addEntry(errResultInfo);
        }
        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();
    }

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
        {
            m_pRequestQueue->removeRequestInProgress( i_pReq->getId() );
        }

        // Please note that the "onRequestChanged" slot is connected with the "changed"
        // signal of the request. If the request has been finished and updated a view
        // lines above the slot will be called disconnecting itself from the signal.

        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).
        m_pRequestQueue->setSyncRequestToBeDeletedLater(i_pReq);

    } // if( !isAsynchronousRequest(i_pReq) )

    SRequestDscr reqDscrCurr;

    pReqInProgress = m_pRequestQueue->getRequestInProgress();

    if( pReqInProgress != nullptr )
    {
        reqDscrCurr = pReqInProgress->getDescriptor();
    }

    if( reqDscrPrev != reqDscrCurr )
    {
        emit requestInProgressChanged(this, reqDscrCurr);
    }

    if( statePrev != m_state )
    {
        emit stateChanged(this, m_state);
    }

    if( bWasConnected && !isConnected() )
    {
        emit disconnected(this);
    }
    else if( !bWasConnected && isConnected() )
    {
        emit connected(this);
    }

    m_pRequestQueue->unlock();

    i_pReq->unlock();

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeSendDataRequest

/*==============================================================================
protected: // overridables (auxiliary methods)
==============================================================================*/

//------------------------------------------------------------------------------
CSrvCltBaseGatewayThread* CClient::createGatewayThread()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "createGatewayThread",
        /* strAddInfo         */ strAddTrcInfo );

    // Not necessary as method only called internally if mutex is already locked:
    //CMutexLocker mtxLocker(m_pMtx);

    return new CClientGatewayThread(
        /* szObjNameGateway       */ m_strObjName,
        /* pServer                */ this,
        /* pModelErrLog           */ m_pErrLog,
        /* iTrcMthFileDetailLevel */ m_eTrcMthFileDetailLevelGateway );

} // createGatewayThread

//------------------------------------------------------------------------------
SErrResultInfo CClient::startGatewayThread(
    int    i_iTimeout_ms,
    qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "startGatewayThread",
        /* strAddInfo         */ strAddTrcInfo );

    // Not necessary as method only called internally if mutex is already locked:
    //CMutexLocker mtxLocker(m_pMtx);

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("startGatewayThread");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqStartThread = m_pRequestQueue->createRequest(
        /* strNameSpace */ "ZS::Ipc",
        /* strClassName */ "CClient",
        /* iRequest     */ ERequestStartGatewayThread,
        /* strRequest   */ Request2Str(ERequestStartGatewayThread),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ true );

    // Please see note in run method of class CSrvCltBaseGatewayThread
    // on synchronizing with the thread using startup confirmation messages.
    m_pGatewayThread->start( pReqStartThread->getId() );

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        mthTracer.trace( "-+ ReqStartThread.wait()" );
    }

    if( pReqStartThread->wait() )
    {
        m_pGateway = m_pGatewayThread->getGateway();

        if( m_pGateway != nullptr )
        {
            for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
            {
                QObject* pObj = m_arpTrcMsgLogObjects[idxObj];

                CClientGateway* pGateway = dynamic_cast<CClientGateway*>(m_pGateway);

                if( pGateway != nullptr )
                {
                    pGateway->addTrcMsgLogObject(pObj);
                }
            }
        }
    }
    else // if( !pReqStartThread->wait() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultTimeout);
        errResultInfo.setAddErrInfoDscr( "Waiting for StartThread timed out." );

        if( m_pErrLog != nullptr )
        {
            m_pErrLog->addEntry(errResultInfo);
        }
    }

    if( errResultInfo.getResult() == EResultUndefined )
    {
        errResultInfo.setErrResult(ErrResultSuccess);
    }

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        mthTracer.trace( "+- ReqStartThread.wait(): " + errResultInfo.getAddErrInfoDscr() );
    }

    pReqStartThread->setErrResultInfo(errResultInfo);
    pReqStartThread->setProgressInPerCent(100);
    pReqStartThread->update();

    deleteRequest(pReqStartThread);
    pReqStartThread = nullptr;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = errResultInfo.getAddErrInfoDscr();
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return errResultInfo;

} // startGatewayThread

//------------------------------------------------------------------------------
SErrResultInfo CClient::stopGatewayThread(
    int    i_iTimeout_ms,
    qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "stopGatewayThread",
        /* strAddInfo         */ strAddTrcInfo );

    // Not necessary as method only called internally if mutex is already locked:
    //CMutexLocker mtxLocker(m_pMtx);

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("stopGatewayThread");

    if( m_pGatewayThread != nullptr )
    {
        // Initialized as an asynchronous request (ResultUndefined).
        CRequest* pReqStopThread = m_pRequestQueue->createRequest(
            /* strNameSpace */ "ZS::Ipc",
            /* strClassName */ "CClient",
            /* iRequest     */ ERequestStopGatewayThread,
            /* strRequest   */ Request2Str(ERequestStopGatewayThread),
            /* iReqIdParent */ i_iReqIdParent,
            /* iTimeout_ms  */ i_iTimeout_ms,
            /* bIsBlocking  */ true );

        m_pGatewayThread->quit();

        if( !m_pGatewayThread->wait(i_iTimeout_ms <= 0 ? ULONG_MAX : static_cast<unsigned long>(i_iTimeout_ms)) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultTimeout);
            errResultInfo.setAddErrInfoDscr( "Waiting for StopThread timed out." );

            m_pGatewayThread->deleteLater();
            m_pGatewayThread = nullptr;
            m_pGateway = nullptr;
        }
        else
        {
            try
            {
                delete m_pGatewayThread;
            }
            catch(...)
            {
            }
            m_pGatewayThread = nullptr;
            m_pGateway = nullptr;
        }

        pReqStopThread->setErrResultInfo(errResultInfo);
        pReqStopThread->setProgressInPerCent(100);
        pReqStopThread->update();

        deleteRequest(pReqStopThread);
        pReqStopThread = nullptr;

    } // if( m_pGatewayThread != nullptr )

    m_pGateway = nullptr;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = errResultInfo.getAddErrInfoDscr();
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return errResultInfo;

} // stopGatewayThread

/*==============================================================================
protected: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CClient::connectGateway( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "connectGateway",
        /* strAddInfo         */ strAddTrcInfo );

    // Not necessary as method only called internally if mutex is already locked:
    //CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("connectGateway");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqConnectGateway = m_pRequestQueue->createRequest(
        /* strNameSpace */ "ZS::Ipc",
        /* strClassName */ "CClient",
        /* iRequest     */ ERequestConnectGateway,
        /* strRequest   */ Request2Str(ERequestConnectGateway),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    // Please note that if called from within different thread contexts signal/slot
    // connections for requests cannot be queued as otherwise on updating a request the
    // "requestChanged" slot may be called sometimes later not updating the internal states
    // at the expected time (before e.g. the "startup" method returns but sometimes later).

    if( !QObject::connect(
        /* pObjSender   */ pReqConnectGateway,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)),
        /* cnctType     */ m_pMtx != nullptr ? Qt::DirectConnection : Qt::AutoConnection) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( m_pGateway == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Gateway is not created." );

        // ConnectGateway is an internal sequencer request. No one else may have
        // signal/slot connections to this request. So there is no need to lock
        // the queue on updating this request (but its necessary to update the
        // request to indicate the progress in the request execution tree).
        //m_pRequestQueue->lock();
        pReqConnectGateway->setErrResultInfo(errResultInfo);
        pReqConnectGateway->setProgressInPerCent(100);
        pReqConnectGateway->update();
        //m_pRequestQueue->unlock();

        if( m_pErrLog != nullptr )
        {
            m_pErrLog->addEntry(errResultInfo);
        }
    }
    else // if( m_pGateway != nullptr )
    {
        // The parent request must have been the connect request.
        CRequest* pReqConnect = m_pRequestQueue->getRequestInProgress(i_iReqIdParent);

        if( pReqConnect == nullptr )
        {
            throw CException(__FILE__, __LINE__, EResultInternalStateMachineError, "Parent request of connect gateway must be the connect request" );
        }
        else if( pReqConnect->getRequest() != ERequestConnect )
        {
            throw CException(__FILE__, __LINE__, EResultInternalStateMachineError, "Parent request of connect gateway must be the connect request" );
        }

        CMsgReqConnect* pMsgReq = dynamic_cast<CMsgReqConnect*>(pReqConnect->getExecutionRequestMessage());
        pReqConnect->setExecutionRequestMessage(nullptr);

        if( pMsgReq == nullptr )
        {
            throw CException(__FILE__, __LINE__, EResultInternalStateMachineError, "Parent request of connect gateway must be the connect request with a valid request message" );
        }

        pMsgReq->setReceiver(m_pGateway);
        pMsgReq->setRequestId( pReqConnectGateway->getId() );

        m_pRequestQueue->addRequestInProgress(pReqConnectGateway);

        POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ELogDetailLevel::Debug);

    } // if( m_pGateway != nullptr )

    if( pReqConnectGateway->isBlockingRequest() )
    {
        if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            mthTracer.trace( "-+ ReqConnectGateway.wait()" );
        }

        if( pReqConnectGateway->wait() )
        {
            CMsgCon* pMsgCon = pReqConnectGateway->getExecutionConfirmationMessage();
            if( pMsgCon != nullptr )
            {
                errResultInfo = pMsgCon->getErrResultInfo();
            }
            else
            {
                errResultInfo = pReqConnectGateway->getErrResultInfo();
            }
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }
        }
        else // if( !pReqConnectGateway->wait() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultTimeout);
            errResultInfo.setAddErrInfoDscr( "Waiting for connect confirmation timed out." );

            if( m_pErrLog != nullptr )
            {
                m_pErrLog->addEntry(errResultInfo);
            }
        }

        if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            mthTracer.trace( "+- ReqConnectGateway.wait(): " + errResultInfo.getAddErrInfoDscr() );
        }

        // ConnectGateway is an internal sequencer request. No one else may have
        // signal/slot connections to this request. So there is no need to lock
        // the queue on updating this request (but its necessary to update the
        // request to indicate the progress in the request execution tree).
        //m_pRequestQueue->lock();
        pReqConnectGateway->setErrResultInfo(errResultInfo);
        pReqConnectGateway->setProgressInPerCent(100);
        pReqConnectGateway->update();
        //m_pRequestQueue->unlock();

    } // if( pReqConnectGateway->isBlockingRequest() )

    else // if( !pReqConnectGateway->isBlockingRequest() )
    {
        if( i_iTimeout_ms > 0 )
        {
            m_reqDscrTimeout = pReqConnectGateway->getDescriptor();
            m_pTmrReqTimeout->start(i_iTimeout_ms);
        }
    }

    if( !isAsynchronousRequest(pReqConnectGateway) )
    {
        // Please note that the "onRequestChanged" slot is connected with the "changed"
        // signal of the request. If the request has been finished and updated a view
        // lines above the slot will be called disconnecting itself from the signal.

        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).
        m_pRequestQueue->setSyncRequestToBeDeletedLater(pReqConnectGateway);
    }

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = QString( pReqConnectGateway == nullptr ? "SUCCESS" : pReqConnectGateway->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqConnectGateway;

} // connectGateway

//------------------------------------------------------------------------------
CRequest* CClient::disconnectGateway( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "disconnectGateway",
        /* strAddInfo         */ strAddTrcInfo );

    // Not necessary as method only called internally if mutex is already locked:
    //CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("disconnectGateway");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqDisconnectGateway = m_pRequestQueue->createRequest(
        /* strNameSpace */ "ZS::Ipc",
        /* strClassName */ "CClient",
        /* iRequest     */ ERequestDisconnectGateway,
        /* strRequest   */ Request2Str(ERequestDisconnectGateway),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    // Please note that if called from within different thread contexts signal/slot
    // connections for requests cannot be queued as otherwise on updating a request the
    // "requestChanged" slot may be called sometimes later not updating the internal states
    // at the expected time (before e.g. the "startup" method returns but sometimes later).

    if( !QObject::connect(
        /* pObjSender   */ pReqDisconnectGateway,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)),
        /* cnctType     */ m_pMtx != nullptr ? Qt::DirectConnection : Qt::AutoConnection) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( m_pGateway == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Gateway is not created." );

        pReqDisconnectGateway->setErrResultInfo(errResultInfo);
        pReqDisconnectGateway->setProgressInPerCent(100);
        pReqDisconnectGateway->update();

        if( m_pErrLog != nullptr )
        {
            m_pErrLog->addEntry(errResultInfo);
        }
    }
    else // if( m_pGateway != nullptr )
    {
        // The parent request must have been the connect request.
        CRequest* pReqDisonnect = m_pRequestQueue->getRequestInProgress(i_iReqIdParent);

        if( pReqDisonnect == nullptr )
        {
            throw CException(__FILE__, __LINE__, EResultInternalStateMachineError, "Parent request of connect gateway must be the connect request" );
        }
        else if( pReqDisonnect->getRequest() != ERequestDisconnect )
        {
            throw CException(__FILE__, __LINE__, EResultInternalStateMachineError, "Parent request of connect gateway must be the connect request" );
        }

        CMsgReqDisconnect* pMsgReq = dynamic_cast<CMsgReqDisconnect*>(pReqDisonnect->getExecutionRequestMessage());
        pReqDisonnect->setExecutionRequestMessage(nullptr);

        if( pMsgReq == nullptr )
        {
            throw CException(__FILE__, __LINE__, EResultInternalStateMachineError, "Parent request of connect gateway must be the connect request with a valid request message" );
        }

        pMsgReq->setReceiver(m_pGateway);
        pMsgReq->setRequestId( pReqDisconnectGateway->getId() );

        m_pRequestQueue->addRequestInProgress(pReqDisconnectGateway);

        POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ELogDetailLevel::Debug);

    } // if( m_pGateway != nullptr )

    if( pReqDisconnectGateway->isBlockingRequest() )
    {
        if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            mthTracer.trace( "-+ ReqDisconnectGateway.wait()" );
        }

        if( pReqDisconnectGateway->wait() )
        {
            CMsgCon* pMsgCon = pReqDisconnectGateway->getExecutionConfirmationMessage();
            if( pMsgCon != nullptr )
            {
                errResultInfo = pMsgCon->getErrResultInfo();
            }
            else
            {
                errResultInfo = pReqDisconnectGateway->getErrResultInfo();
            }
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }
        }
        else if( !pReqDisconnectGateway->wait() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultTimeout);
            errResultInfo.setAddErrInfoDscr( "Waiting for disconnect confirmation timed out." );

            if( m_pErrLog != nullptr )
            {
                m_pErrLog->addEntry(errResultInfo);
            }
        }

        if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            mthTracer.trace( "+- ReqDisconnectGateway.wait(): " + errResultInfo.getAddErrInfoDscr() );
        }

        // DisconnectGateway is an internal sequencer request. No one else may have
        // signal/slot connections to this request. So there is no need to lock
        // the queue on updating this request (but its necessary to update the
        // request to indicate the progress in the request execution tree).
        //m_pRequestQueue->lock();
        pReqDisconnectGateway->setErrResultInfo(errResultInfo);
        pReqDisconnectGateway->setProgressInPerCent(100);
        pReqDisconnectGateway->update();
        //m_pRequestQueue->unlock();

    } // if( pReqDisconnectGateway->isBlockingRequest() )

    else // if( !pReqDisconnectGateway->isBlockingRequest() )
    {
        if( i_iTimeout_ms > 0 )
        {
            m_reqDscrTimeout = pReqDisconnectGateway->getDescriptor();
            m_pTmrReqTimeout->start(i_iTimeout_ms);
        }
    }

    if( !isAsynchronousRequest(pReqDisconnectGateway) )
    {
        // Please note that the "onRequestChanged" slot is connected with the "changed"
        // signal of the request. If the request has been finished and updated a view
        // lines above the slot will be called disconnecting itself from the signal.

        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).
        m_pRequestQueue->setSyncRequestToBeDeletedLater(pReqDisconnectGateway);
    }

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = QString( pReqDisconnectGateway == nullptr ? "SUCCESS" : pReqDisconnectGateway->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqDisconnectGateway;

} // disconnectGateway

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CClient::onRequestTimeout()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onRequestTimeout",
        /* strAddInfo         */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    m_pTmrReqTimeout->stop();

    CRequest* pReq = m_pRequestQueue->getRequestInProgress(m_reqDscrTimeout.m_iId);

    if( pReq != nullptr )
    {
        SErrResultInfo errResultInfo = ErrResultInfoError("onRequestTimeout", EResultTimeout, pReq->getRequestName());

        m_pRequestQueue->lock();

        pReq->lock();
        pReq->setErrResultInfo(errResultInfo);
        pReq->setProgressInPerCent(100);
        pReq->update();
        pReq->unlock();

        m_pRequestQueue->unlock();

        // The one who calls update for the finished request must delete the request.
        deleteRequest(pReq); // The "onRequestChanged" slot will be called again. The request has already been removed from the queue.
        pReq = nullptr;

        if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
        {
            executeNextPostponedRequest();
        }
    } // if( pReq != nullptr )

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

} // onRequestTimeout

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CClient::onRequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onRequestChanged",
        /* strAddInfo         */ strAddTrcInfo );

    // Not necessary as method only called internally if mutex is already locked:
    //CMutexLocker mtxLocker(m_pMtx);

    if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("onRequestChanged");

    QString strAddErrInfo;

    bool bIsPostponedRequest = false;
    int  idxPostponedRequest = -1;

    CRequest* pReqChanged = m_pRequestQueue->findRequestById(
        /* iRequestId               */ i_reqDscr.m_iId,
        /* bIgnorePostponedRequests */ false,
        /* pbIsPostponedRequest     */ &bIsPostponedRequest,
        /* pidxPostponedRequest     */ &idxPostponedRequest );

    if( pReqChanged == nullptr )
    {
        strAddErrInfo = "Slot method called for request \"" + i_reqDscr.getAddTrcInfoStr(0) + "\" but request is not in request queue.";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    } // if( pReqChanged == nullptr )

    CRequest* pReqMainInProgress  = m_pRequestQueue->getRequestInProgress();

    if( pReqMainInProgress == nullptr )
    {
        strAddErrInfo = "Slot method called for request \"" + i_reqDscr.getAddTrcInfoStr(0) + "\" while no request is in progress.";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    } // if( pReqMainInProgress == nullptr )

    if( !errResultInfo.isErrorResult() )
    {
        errResultInfo = i_reqDscr.m_errResultInfo;

        bool         bIsMainRequest = false;
        EState       statePrev      = m_state;
        SRequestDscr reqDscrPrev;

        if( pReqChanged == pReqMainInProgress )
        {
            bIsMainRequest = true;
            reqDscrPrev = i_reqDscr;
        }

        // Please note that this slot method may also be called for postponed requests
        // if the parent of those requests is e.g. aborted.
        if( !bIsPostponedRequest )
        {
            emit requestInProgressChanged(this, i_reqDscr);
        }

        bool bWasConnected = isConnected();

        m_pRequestQueue->lock();

        SErrResultInfo errResultInfo = i_reqDscr.m_errResultInfo;

        // Please note that not all error results should end up in the error log file.
        // This depends which request could not have been successfully processed.
        // E.g. if a connect request fails the client is not connected. But this is
        // not really an error.

        // If the request has been finished ...
        if( i_reqDscr.m_iProgress_perCent >= 100 )
        {
            // Please note that the "onRequestChanged" slot is also connected
            // to the changed signal of postponed requests.
            if( bIsPostponedRequest )
            {
                m_pRequestQueue->removePostponedRequest(pReqChanged);
            }
            else // if( !bIsPostponedRequest )
            {
                if( m_pTmrReqTimeout->isActive() )
                {
                    m_pTmrReqTimeout->stop();
                }
            } // if( !bIsPostponedRequest )

            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }
            m_pRequestQueue->removeRequestInProgress(i_reqDscr.m_iId);

            // On further changes to the request this slot should not be called anymore.
            QObject::disconnect(
                /* pObjSender   */ pReqChanged,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        } // if( i_reqDscr.m_iProgress_perCent >= 100 )

        // The Ipc server does not trigger requests from "foreign" classes.
        // So the following string compare is unnecessary as this slot may only
        // be called if the server itself updates the request.
        //if( i_reqDscr.m_strCreatorClassName.compare(className(),Qt::CaseInsensitive) == 0 )

        if( !bIsPostponedRequest )
        {
            // Confirmation messages received from the gateway.
            CMsgCon* pMsgConExec = pReqChanged->getExecutionConfirmationMessage();

            switch( i_reqDscr.m_request )
            {
                case ERequestConnect:
                {
                    // If the request has been finished ..
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        if( errResultInfo.isErrorResult() )
                        {
                            errResultInfo = stopGatewayThread();

                            if( errResultInfo.isErrorResult() )
                            {
                                if( m_pErrLog != nullptr )
                                {
                                    m_pErrLog->addEntry(errResultInfo);
                                }
                            }

                            m_state = EStateUnconnected;

                        } // if( errResultInfo.isErrorResult() )

                        else // if( !errResultInfo.isErrorResult() )
                        {
                            m_state = EStateConnected;
                        }
                    } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                    break;
                } // case ERequestConnect

                case ERequestDisconnect:
                {
                    // If the request has been finished ..
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        m_state = EStateUnconnected;
                    }
                    break;
                } // case ERequestDisconnect

                case ERequestChangeSettings:
                {
                    // If the request has been finished ..
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        CMsgConChangeSettings* pMsgConChangeSettings = dynamic_cast<CMsgConChangeSettings*>(pMsgConExec);

                        if( pMsgConChangeSettings != nullptr )
                        {
                            m_socketDscr = pMsgConChangeSettings->getClientSocketDscr();

                            if( m_socketDscr.m_socketState == ESocketStateUnconnected )
                            {
                                m_state = EStateUnconnected;
                            }
                        }

                        if( m_state == EStateUnconnected )
                        {
                            SErrResultInfo errResultInfoTmp = stopGatewayThread();

                            if( errResultInfoTmp.isErrorResult() )
                            {
                                if( m_pErrLog != nullptr )
                                {
                                    m_pErrLog->addEntry(errResultInfoTmp);
                                }
                            }
                        }
                    } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                    break;
                } // case ERequestChangeSettings

                case ERequestSendData:
                {
                    break;
                }

                case ERequestConnectGateway:
                {
                    // If the sub request has been finished ..
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        CMsgConConnect* pMsgConConnect = dynamic_cast<CMsgConConnect*>(pMsgConExec);

                        if( pMsgConConnect != nullptr )
                        {
                            if( !errResultInfo.isErrorResult() )
                            {
                                errResultInfo = pMsgConConnect->getErrResultInfo();
                            }

                            m_socketDscr = pMsgConConnect->getSocketDscr();

                            if( m_socketDscr.m_socketState == ESocketStateConnected )
                            {
                                m_state = EStateConnected;
                            }
                        } // if( pMsgConConnect != nullptr )

                        if( m_state == EStateUnconnected )
                        {
                            SErrResultInfo errResultInfoTmp = stopGatewayThread();

                            if( errResultInfoTmp.isErrorResult() )
                            {
                                if( m_pErrLog != nullptr )
                                {
                                    m_pErrLog->addEntry(errResultInfoTmp);
                                }
                            }
                        }

                        pReqMainInProgress->setErrResultInfo(errResultInfo);
                        pReqMainInProgress->setProgressInPerCent(i_reqDscr.m_iProgress_perCent);
                        pReqMainInProgress->lock();
                        pReqMainInProgress->update();
                        pReqMainInProgress->unlock();

                        // The one who calls update for the finished request must delete the request.
                        deleteRequest(pReqMainInProgress);
                        pReqMainInProgress = nullptr;

                    } // if( i_reqDscr.m_iProgress_perCent >= 100 )

                    break;
                } // case ERequestConnectGateway

                case ERequestDisconnectGateway:
                {
                    // If the sub request has been finished ..
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        CMsgConDisconnect* pMsgConDisconnect = dynamic_cast<CMsgConDisconnect*>(pMsgConExec);

                        if( pMsgConDisconnect != nullptr )
                        {
                            if( !errResultInfo.isErrorResult() )
                            {
                                errResultInfo = pMsgConDisconnect->getErrResultInfo();
                            }
                            m_socketDscr = pMsgConDisconnect->getSocketDscr();

                        } // if( pMsgConDisconnect != nullptr )

                        SErrResultInfo errResultInfoTmp = stopGatewayThread();

                        if( errResultInfoTmp.isErrorResult() )
                        {
                            if( m_pErrLog != nullptr )
                            {
                                m_pErrLog->addEntry(errResultInfoTmp);
                            }
                        }

                        pReqMainInProgress->setErrResultInfo(errResultInfo);
                        pReqMainInProgress->setProgressInPerCent(i_reqDscr.m_iProgress_perCent);
                        pReqMainInProgress->lock();
                        pReqMainInProgress->update();
                        pReqMainInProgress->unlock();

                        // The one who calls update for the finished request must delete the request.
                        deleteRequest(pReqMainInProgress);
                        pReqMainInProgress = nullptr;

                    } // if( i_reqDscr.m_iProgress_perCent >= 100 )

                    // The request sequencer will call the next request method.
                    break;
                } // case ERequestDisconnectGateway

                case ERequestStartGatewayThread:
                case ERequestStopGatewayThread:
                default:
                {
                    break;
                }
            } // switch( i_reqDscr.m_request )
        } // if( !bIsPostponedRequest )

        // If the main request has been finished ...
        if( i_reqDscr.m_iProgress_perCent >= 100 && bIsMainRequest )
        {
            if( pReqChanged != nullptr && !pReqChanged->isLocked() )
            {
                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReqChanged);
            }
        } // if( i_reqDscr.m_iProgress_perCent >= 100 && bIsMainRequest )

        SRequestDscr reqDscrCurr;

        CRequest* pReqInProgress = m_pRequestQueue->getRequestInProgress();

        if( pReqInProgress != nullptr )
        {
            reqDscrCurr = pReqInProgress->getDescriptor();
        }

        if( reqDscrPrev != reqDscrCurr )
        {
            if( reqDscrCurr.m_objState == EObjState::Created )
            {
                emit requestInProgressChanged(this, reqDscrCurr);
            }
        }

        if( statePrev != m_state )
        {
            emit stateChanged(this, m_state);
        }

        if( bWasConnected && !isConnected() )
        {
            emit disconnected(this);
        }
        else if( !bWasConnected && isConnected() )
        {
            emit connected(this);
        }

        //--------------------------------------------------------------
        // Sending request messages to the client by proxies is not yet supported.
        // For this setting request and confirmation messages at the request instance
        // must be revised as for blocking requests the gateway also sets the
        // confirmation message to indicate the result of the executed request.
        //--------------------------------------------------------------

        //if( pMsgCon != nullptr )
        //{
        //    POST_OR_DELETE_MESSAGE(pMsgCon, &mthTracer, ELogDetailLevel::Debug);
        //    pMsgCon = nullptr;
        //}

        m_pRequestQueue->unlock();

        if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
        {
            executeNextPostponedRequest();
        }

        if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            int iAddTrcInfoDetailLevel = 0;
            if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
            else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

            strAddTrcInfo  = "State: " + State2Str(m_state);
            strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
            mthTracer.trace(strAddTrcInfo);
        }
    } // if( !errResultInfo.isErrorResult() )

} // onRequestChanged

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CClient::areTraceMethodCallsActive( EMethodTraceDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(i_eFilterDetailLevel) )
    {
        bIsActive = true;
    }
    else if( m_pTrcMthFile != nullptr && m_eTrcMthFileDetailLevel >= i_eFilterDetailLevel )
    {
        bIsActive = true;
    }
    return bIsActive;
}

//------------------------------------------------------------------------------
EMethodTraceDetailLevel CClient::getMethodCallsTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    EMethodTraceDetailLevel eDetailLevel = EMethodTraceDetailLevel::None;

    if( m_pTrcAdminObj != nullptr )
    {
        eDetailLevel = m_pTrcAdminObj->getMethodCallsTraceDetailLevel();
    }
    else if( m_pTrcMthFile != nullptr )
    {
        eDetailLevel = m_eTrcMthFileDetailLevel;
    }
    return eDetailLevel;
}

//------------------------------------------------------------------------------
bool CClient::isTraceRuntimeInfoActive( ELogDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isRuntimeInfoActive(i_eFilterDetailLevel) )
    {
        bIsActive = true;
    }
    return bIsActive;
}

//------------------------------------------------------------------------------
ELogDetailLevel CClient::getRuntimeInfoTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    ELogDetailLevel eDetailLevel = ELogDetailLevel::None;

    if( m_pTrcAdminObj != nullptr )
    {
        eDetailLevel = m_pTrcAdminObj->getRuntimeInfoTraceDetailLevel();
    }
    return eDetailLevel;
}

/*==============================================================================
protected: // overridables of inherited class QObject (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
bool CClient::event( QEvent* i_pMsg )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    if( i_pMsg->type() >= QEvent::User )
    {
        QString strAddTrcInfo;
        QString strAddErrInfo;

        CMsg* pMsg = nullptr;

        try
        {
            pMsg = dynamic_cast<CMsg*>(i_pMsg);
        }
        catch(...)
        {
            pMsg = nullptr;
        }

        if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
        {
            int iAddTrcInfoDetailLevel = 0;
            if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
            else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

            strAddTrcInfo = "Msg {" + QString( pMsg == nullptr ? "nullptr" : pMsg->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
        }

        CMethodTracer mthTracer(
            /* pAdminObj          */ m_pTrcAdminObj,
            /* pTrcMthFile        */ m_pTrcMthFile,
            /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ nameSpace(),
            /* strClassName       */ className(),
            /* strObjName         */ objectName(),
            /* strMethod          */ "event",
            /* strAddInfo         */ strAddTrcInfo );

        if( pMsg != nullptr )
        {
            CMutexLocker mtxLocker(m_pMtx);

            bEventHandled = true;

            SErrResultInfo errResultInfo = ErrResultInfoSuccess(pMsg->msgTypeToStr());

            if( pMsg->isBaseMsgType() )
            {
                if( pMsg->getMsgType() == ZS::System::EBaseMsgTypeIndErr )
                {
                    CMsgErr* pMsgErr = dynamic_cast<CMsgErr*>(pMsg);
                    if( pMsgErr == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultInvalidMessageContent,strAddTrcInfo);
                    }

                    errResultInfo = pMsgErr->getErrResultInfo();

                    if( m_pErrLog != nullptr && !errResultInfo.isAddedToErrLogModel() )
                    {
                        m_pErrLog->addEntry(errResultInfo);
                        errResultInfo.setAddedToErrLogModel();
                    }
                }
                else if( pMsg->getMsgType() ==  EBaseMsgTypeReqContinue )
                {
                    m_bMsgReqContinuePending = false;

                    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
                    {
                        CRequest* pReqPostponed = m_pRequestQueue->takeFirstPostponedRequest();

                        if( pReqPostponed != nullptr )
                        {
                            switch( pReqPostponed->getRequest() )
                            {
                                case ERequestConnect:
                                {
                                    executeConnectRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestDisconnect:
                                {
                                    executeDisconnectRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestChangeSettings:
                                {
                                    executeChangeSettingsRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestSendData:
                                {
                                    executeSendDataRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                default:
                                {
                                    break;
                                }
                            } // switch( pReqPostponed->getRequest() )
                        } // if( pReqPostponed != nullptr )

                        delete pReqPostponed;
                        pReqPostponed = nullptr;

                    } // if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
                } // else if( pMsg->getMsgType() ==  EBaseMsgTypeReqContinue )
            } // if( pMsg->isBaseMsgType() )

            else // if( !pMsg->isBaseMsgType() )
            {
                if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
                {
                    int iAddTrcInfoDetailLevel = 0;
                    if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
                    else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

                    strAddTrcInfo  = "State: " + State2Str(m_state);
                    strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
                    mthTracer.trace(strAddTrcInfo);
                }

                CRequest* pReq;

                if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq )
                {
                    //--------------------------------------------------------------
                    // Sending request messages to the client by proxies is not yet supported.
                    // For this setting request and confirmation messages at the request instance
                    // must be revised as for blocking requests the gateway also sets the
                    // confirmation message to indicate the result of the executed request.
                    //--------------------------------------------------------------

                    CMsgReq* pMsgReq = dynamic_cast<CMsgReq*>(pMsg);
                    if( pMsgReq == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }

                    //qint64   iReqIdParent = pMsgReq->getRequestId();
                    //CMsgCon* pMsgCon = nullptr;

                    //if( isAsynchronousRequest(pReq) )
                    //{
                    //    pReq->setConfirmationMessage(pMsgCon);
                    //    pMsgCon = nullptr;
                    //}
                    //else // if( !isAsynchronousRequest(pReq) )
                    //{
                    //    bool bIsWaiting = false;

                    //    if( iReqIdParent >= 0 )
                    //    {
                    //        bIsWaiting = CRequestExecTree::GetInstance()->isWaiting(iReqIdParent);
                    //    }

                    //    if( bIsWaiting != 0 )
                    //    {
                    //        CRequestExecTree::GetInstance()->wake(iReqIdParent);
                    //    }
                    //    else if( pMsgCon != nullptr )
                    //    {
                    //        if( pReq != nullptr )
                    //        {
                    //            errResultInfo = pReq->getErrResultInfo();
                    //        }
                    //        else if( errResultInfo.getResult() == EResultUndefined )
                    //        {
                    //            errResultInfo.setResult(EResultSuccess);
                    //        }
                    //        pMsgCon->setErrResultInfo(errResultInfo);
                    //        pMsgCon->setProgress(100);

                    //        POST_OR_DELETE_MESSAGE(pMsgCon, &mthTracer, ELogDetailLevel::Debug);
                    //        pMsgCon = nullptr;
                    //    }
                    //} // if( !isAsynchronousRequest(pReq) )

                    //delete pMsgCon;
                    //pMsgCon = nullptr;

                } // if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq )

                else if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeInd )
                {
                    //----------------------------------------------------------
                    // Indication messages sent by the gateway to this client.
                    //----------------------------------------------------------

                    CMsgInd* pMsgInd = dynamic_cast<CMsgInd*>(pMsg);
                    if( pMsgInd == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }

                    if( m_pGateway != nullptr && pMsgInd->getSender() != m_pGateway )
                    {
                        throw CException (__FILE__, __LINE__, EResultInvalidSenderObjInMessage );
                    }

                    // Please note that messages may be sent by the gateway right before the gateway
                    // is destroyed. In this case the message may be received right after the gateway
                    // has been destroyed, we can't access the message sender (the gateway) anymore
                    // and also a dynamic_cast from the message sender to the gateway object will
                    // result in an access violation exception.

                    switch( static_cast<int>(pMsgInd->type()) )
                    {
                        //------------------------------------------------------------------
                        case EMsgTypeIndDisconnected:
                        //------------------------------------------------------------------
                        {
                            CMsgIndDisconnected* pMsgIndDisconnected = dynamic_cast<CMsgIndDisconnected*>(pMsg);
                            if( pMsgIndDisconnected == nullptr )
                            {
                                throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch, "MsgIndDisconnected == nullptr! " + strAddTrcInfo );
                            }
                            if( pMsgIndDisconnected->getSender() == nullptr || pMsgIndDisconnected->getSender() != m_pGateway )
                            {
                                throw CException (__FILE__, __LINE__, EResultInvalidSenderObjInMessage );
                            }
                            if( pMsgIndDisconnected->getSocketId() != 0 )
                            {
                                QString strAddTrcInfo = "event( ";
                                strAddTrcInfo += "MsgIndDisconnected: SocketId = " + QString::number(pMsgIndDisconnected->getSocketId()) + " != 0 )";
                                throw ZS::System::CException( __FILE__, __LINE__, EResultSocketIdOutOfRange, strAddTrcInfo );
                            }

                            m_socketDscr.m_socketState = ESocketStateUnconnected;
                            m_socketDscr.m_iSocketId = -1;

                            // Connection lost is not an error but a connection state info.
                            //if( m_pErrLog != nullptr )
                            //{
                            //    m_pErrLog->addEntry( SErrResultInfo(
                            //        /* severity   */ EResultSeverityInfo,
                            //        /* result     */ EResultRemoteHostClosed,
                            //        /* strAddInfo */ m_socketDscr.getConnectionString(),
                            //        /* strSource  */ m_strObjName ) );
                            //}

                            errResultInfo = stopGatewayThread();

                            if( errResultInfo.isErrorResult() )
                            {
                                if( m_pErrLog != nullptr )
                                {
                                    m_pErrLog->addEntry(errResultInfo);
                                }
                            }

                            bool bRequestAborted = false;

                            // If the remote host closed the connection while the disconnect request progress
                            // has just been started there will be no confirmation to the disconnect request
                            // from the gateway (as it has already been deleted).
                            if( requestInProgress() == ERequestDisconnect )
                            {
                                bRequestAborted = true;

                                // For the caller of the original disconnect request it makes no difference
                                // why the connection has been closed and we inform him that his request
                                // has been executed.
                                m_state = EStateUnconnected;

                                pReq = m_pRequestQueue->getRequestInProgress();

                                m_pRequestQueue->lock();

                                // Invalidate request in progress before updating the request and emitting signals
                                // so that "isRequestInProgress" returns false and "getRequestInProgress" returns None.
                                // The request queue is locked and therefore "isIdle" returns false so that new requests
                                // started as reentries will be postponed.
                                m_pRequestQueue->setRequestInProgress(nullptr);

                                pReq->setErrResultInfo(errResultInfo);
                                pReq->setProgressInPerCent(100);
                                pReq->lock();
                                pReq->update();
                                pReq->unlock();

                                //// The "onRequestChanged" slot will not emit the signal anymore as the
                                //// request has been removed from the request queue.
                                //emit requestInProgressChanged( this, pReq->getDescriptor() );

                                m_pRequestQueue->unlock();

                                //CMsgCon* pMsgConReq = pReq->takeConfirmationMessage();

                                deleteRequest(pReq);
                                pReq = nullptr;

                                //if( pMsgConReq != nullptr )
                                //{
                                //    pMsgConReq->setErrResultInfo(errResultInfo);
                                //    POST_OR_DELETE_MESSAGE(pMsgConReq, &mthTracer, ELogDetailLevel);
                                //    pMsgConReq = nullptr;
                                //}
                            }
                            else if( requestInProgress() == ERequestChangeSettings )
                            {
                                // The confirmation message will follow to finish the request in progress.
                            }
                            else
                            {
                                m_state = EStateUnconnected;

                                emit disconnected(this);
                                emit stateChanged(this, m_state);
                            }

                            if( bRequestAborted && !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
                            {
                                // The current request has been refused but executed. Check if there are pending requests.
                                executeNextPostponedRequest();
                            }
                            break;
                        } // case EMsgTypeIndDisconnected

                        //------------------------------------------------------------------
                        case EMsgTypeIndReceivedData:
                        //------------------------------------------------------------------
                        {
                            CMsgIndReceivedData* pMsgInd = dynamic_cast<CMsgIndReceivedData*>(pMsg);
                            bool                 bValidMessage = true;

                            // The client may have been shutdown and is waiting for the gateway thread
                            // to be quit. But the gateway may have been receiving data while the client
                            // is waiting for the thread to be finished and therefore indication messages
                            // may already be in Qt's event loop that will be dispatched to the client
                            // after he has been shutdown. So we ignore the indication messages if the
                            // client does no longer expect them and the socket id is no longer valid.
                            if( pMsgInd == nullptr )
                            {
                                bValidMessage = false;
                            }
                            else if( m_state == EStateUnconnected )
                            {
                                bValidMessage = false;
                            }
                            else if( pMsgInd->getSender() != m_pGateway )
                            {
                                bValidMessage = false;
                            }
                            else if( pMsgInd->getSocketId() != 0 )
                            {
                                bValidMessage = false;
                            }
                            if( bValidMessage )
                            {
                                onReceivedData( pMsgInd->getByteArray() );

                                emit receivedData( this, pMsgInd->getByteArray() );
                            }
                            break;
                        } // case EMsgTypeIndReceivedData

                        //------------------------------------------------------------------
                        default:
                        //------------------------------------------------------------------
                        {
                            QString strAddErrInfo = pMsg->getAddTrcInfoStr();

                            if( m_pErrLog != nullptr )
                            {
                                errResultInfo.setSeverity(EResultSeverityError);
                                errResultInfo.setResult(EResultInvalidMessage);
                                errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                                m_pErrLog->addEntry(errResultInfo);
                            }
                            else
                            {
                                throw CException(__FILE__,__LINE__,EResultInvalidMessage,strAddErrInfo);
                            }
                        }
                    } // switch( pMsgInd->type() )
                } // if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeInd )

                else if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeCon )
                {
                    CMsgCon* pMsgCon = dynamic_cast<CMsgCon*>(pMsg);
                    if( pMsgCon == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }

                    pReq = m_pRequestQueue->getRequestInProgress(pMsgCon->getRequestId());

                    if( pReq != nullptr )
                    {
                        SErrResultInfo errResultInfo = pMsgCon->getErrResultInfo();
                        int            iProgress_perCent = pMsgCon->getProgress();

                        m_pRequestQueue->lock();

                        // Please note that the message (QEvent) will be deleted by Qt's event dispatcher.
                        // The confirmation may only be temporarily accessed by receivers of the request
                        // changed signal while handling the slot method.
                        pReq->setExecutionConfirmationMessage(pMsgCon);
                        pReq->setErrResultInfo(errResultInfo);
                        pReq->setProgressInPerCent(iProgress_perCent);
                        pReq->lock();
                        pReq->update(); // "onRequestChanged" is called as a reentry removing the request from the queue.
                        pReq->unlock();
                        pReq->setExecutionConfirmationMessage(nullptr); // Qt will delete the message.

                        if( iProgress_perCent >= 100 )
                        {
                            // The one who calls update for the finished request must delete the request.
                            deleteRequest(pReq); // The "onRequestChanged" slot will be called again. The request has already been removed from the queue.
                            pReq = nullptr;
                        }

                        m_pRequestQueue->unlock();

                        if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
                        {
                            executeNextPostponedRequest();
                        }
                    } // if( pReq != nullptr )
                } // if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeCon )

                if( isTraceRuntimeInfoActive(ELogDetailLevel::Debug) )
                {
                    int iAddTrcInfoDetailLevel = 0;
                    if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
                    else if( getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

                    strAddTrcInfo  = "State: " + State2Str(m_state);
                    strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
                }

            } // if( !pMsg->isBaseMsgType() )
        } // if( pMsg != nullptr )
    } // if( pMsg->type() >= QEvent::User )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pMsg);
    }
    return bEventHandled;

} // event
