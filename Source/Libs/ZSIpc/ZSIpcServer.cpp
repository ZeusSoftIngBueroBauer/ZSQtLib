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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qmutex.h>
#include <QtCore/qsettings.h>
#include <QtCore/qtimer.h>
#include <QtCore/qwaitcondition.h>
#include <QtNetwork/qhostinfo.h>

#include "ZSIpc/ZSIpcServer.h"
#include "ZSIpc/ZSIpcServerGateway.h"
#include "ZSIpc/ZSIpcBlkTypeL.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysRequestQueue.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;


/*******************************************************************************
class CServer : public QObject
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrStates[CServer::EStateCount] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( CServer::EStateIdle,      "Idle",      "Idle"      ),
    /* 1 */ SEnumEntry( CServer::EStateListening, "Listening", "Listening" )
};

//------------------------------------------------------------------------------
QString CServer::State2Str( int i_iState, bool i_bDetailed )
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

} // State2Str

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrRequests[CServer::ERequestCount] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( CServer::ERequestNone,               "None"               ),
    /* 1 */ SEnumEntry( CServer::ERequestStartup,            "Startup"            ),
    /* 2 */ SEnumEntry( CServer::ERequestShutdown,           "Shutdown"           ),
    /* 3 */ SEnumEntry( CServer::ERequestChangeSettings,     "ChangeSettings"     ),
    /* 4 */ SEnumEntry( CServer::ERequestSendData,           "SendData"           ),
    /* 5 */ SEnumEntry( CServer::ERequestStartGatewayThread, "StartGatewayThread" ),
    /* 6 */ SEnumEntry( CServer::ERequestStopGatewayThread,  "StopGatewayThread"  ),
    /* 7 */ SEnumEntry( CServer::ERequestStartupGateway,     "StartupGateway"     ),
    /* 8 */ SEnumEntry( CServer::ERequestShutdownGateway,    "ShutdownGateway"    )
};

//------------------------------------------------------------------------------
QString CServer::Request2Str( int i_iRequest )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrRequests, ERequestCount, i_iRequest);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CServer::CServer(
    const QString& i_strObjName,
    bool           i_bMultiThreadedAccess,
    CTrcMthFile*   i_pTrcMthFile,
    int            i_iTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pMtx(nullptr),
    m_strObjName(i_strObjName),
    m_pErrLog(nullptr),
    // Connection settings
    m_arSocketTypes(),
    m_hostSettings(),
    m_pBlkType(nullptr),
    m_arpSocketDscr(),
    // Gateway
    m_pGatewayThread(nullptr),
    m_pGateway(nullptr),
    // State Machine
    m_state(EStateIdle),
    m_bReqExecTreeCreated(false),
    m_pRequestQueue(nullptr),
    m_reqDscrTimeout(),
    m_pTmrReqTimeout(nullptr),
    m_bMsgReqContinuePending(false),
    m_bIsBeingDestroyed(false),
    // Tracing
    m_arpTrcMsgLogObjects(),
    m_iTrcMthFileDetailLevel(i_iTrcMthFileDetailLevel),
    m_pTrcMthFile(i_pTrcMthFile),
    m_pTrcAdminObj(nullptr)
{
    if( m_strObjName.isEmpty() )
    {
        m_strObjName = "IpcServer";
    }
    setObjectName(m_strObjName);

    if( m_pTrcMthFile == nullptr )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), m_strObjName);
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
        /* strMthInArgs       */ "" );

    if( i_bMultiThreadedAccess )
    {
        m_pMtx = new QMutex(QMutex::Recursive);
    }

    m_pErrLog = CErrLog::GetInstance();

    // Connection settings
    //--------------------

    m_arSocketTypes.append(ESocketTypeTcp);

    // Set the default value for the local host name to the computers
    // name before reading the settings from the ini file.

    m_hostSettings.m_strLocalHostName = QHostInfo::localHostName();
    m_hostSettings.m_socketType = ESocketTypeTcp;
    m_hostSettings.m_uLocalPort = 24763;

    if( m_hostSettings.m_uMaxPendingConnections == 0 )
    {
        m_hostSettings.m_uMaxPendingConnections = 30;
    }

    m_pBlkType = new CBlkTypeL(EBlkLenDataTypeBinUINT32);

    // State Machine
    //--------------

    if( CRequestExecTree::GetInstance() == nullptr )
    {
        // Using the Dll interface for the Trace Ipc Server it might happen that the Trace Ipc Server
        // is not created in the context of the Qt applications main thread. As it cannot be guaranteed
        // the thread creating the Trace Server has a Qt event loop the Trace server will be moved to
        // the Qt Main applications thread.
        // When moving the server to another thread also all childrens of the server handling signals and
        // slots must be moved to that thread. In order to move the request execution tree together
        // with the Ipc server the parent object must be passed.
        if( CRequestExecTree::CreateInstance(this) == nullptr )
        {
            QString strAddTrcInfo = "RequestExecTree not created (needed to handle asynchronous requests).";
            throw ZS::System::CException(__FILE__, __LINE__, EResultSingletonClassNotInstantiated, strAddTrcInfo);
        }
        m_bReqExecTreeCreated = true;
    }

    m_pRequestQueue = new CRequestQueue(
        /* strCreatorNameSpace    */ nameSpace(),
        /* strCreatorClassName    */ className(),
        /* pObjParent             */ this,
        /* bKeepReqDscrInExecTree */ false );

    m_pTmrReqTimeout = new QTimer(this);

    // If called from within different thread contexts signal/slots connections for requests
    // cannot be queued as otherwise on updating a request the "requestChanged" slot may
    // be called sometimes later not updating the internal states at the expected time
    // (before e.g. the "startup" method returns but sometimes later).
    if( !QObject::connect(
        /* pObjSender   */ m_pTmrReqTimeout,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestTimeout()),
        /* cnctType     */ m_pMtx != nullptr ? Qt::DirectConnection : Qt::AutoConnection) )
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultSignalSlotConnectionFailed);
    }

} // ctor

//------------------------------------------------------------------------------
CServer::~CServer()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
    // Connection settings
    //m_arSocketTypes;
    //m_hostSettings;
    m_pBlkType = nullptr;
    //m_arpSocketDscr;
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
    m_iTrcMthFileDetailLevel = 0;
    m_pTrcMthFile = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CServer::setKeepReqDscrInExecTree( bool i_bKeep )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Keep: " + bool2Str(i_bKeep);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
bool CServer::keepReqDscrInExecTree() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

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
CRequest* CServer::startup(
    int    i_iTimeout_ms,
    bool   i_bWait,
    qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "startup",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("startup");

    EState statePrev = m_state;

    CRequest* pReq = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestStartup,
        /* strRequest   */ Request2Str(ERequestStartup),
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
        else // if( !pReq->isBlockingRequest() )
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
        executeStartupRequest(pReq);
    }

    // Please note that if executing this request did not become asynchronous the
    // "execute<>Request" method above has been called and the "onRequestChanged"
    // slot method has already been disconnected from the request's changed signal.

    if( statePrev != m_state )
    {
        emit stateChanged(this,m_state);
    }

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString( pReq == nullptr ? "SUCCESS" : pReq->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // startup

//------------------------------------------------------------------------------
CRequest* CServer::shutdown(
    int    i_iTimeout_ms,
    bool   i_bWait,
    qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "shutdown",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("shutdown");

    EState statePrev = m_state;

    CRequest* pReq = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestShutdown,
        /* strRequest   */ Request2Str(ERequestShutdown),
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
        executeShutdownRequest(pReq);
    }

    // Please note that if executing this request did not become asynchronous the
    // "execute<>Request" method above has been called and the "onRequestChanged"
    // slot method has already been disconnected from the request's changed signal.

    if( statePrev != m_state )
    {
        emit stateChanged(this,m_state);
    }

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString( pReq == nullptr ? "SUCCESS" : pReq->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // shutdown

/*==============================================================================
public: // overridables of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CServer::changeSettings( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "changeSettings",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("changeSettings");

    EState statePrev = m_state;

    CRequest* pReq = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
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
        else // if( !pReq->isBlockingRequest() )
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

    if( statePrev != m_state )
    {
        emit stateChanged(this,m_state);
    }

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
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
ZS::System::CRequest* CServer::sendData(
    int               i_iSocketId,
    const QByteArray& i_byteArr,
    int               i_iTimeout_ms,
    bool              i_bWait,
    qint64            i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "SocketId: " + socketId2Str(i_iSocketId);

        if( i_iSocketId >= 0 )
        {
            SSocketDscr socketDscr = getSocketDscr(i_iSocketId);
            strAddTrcInfo += " (" + socketDscr.m_strRemoteHostName + ":" + QString::number(socketDscr.m_uServerListenPort) + ":" + QString::number(socketDscr.m_uRemotePort) + ")";
        }

        strAddTrcInfo += ", ByteArr[" + QString::number(i_byteArr.size()) + "]";
        if( getMethodTraceDetailLevel() < ETraceDetailLevelVerbose )
        {
            strAddTrcInfo += "(" + truncateStringWithEllipsisInTheMiddle(QString(i_byteArr), 20) + ")";
        }
        else // if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose )
        {
            strAddTrcInfo += "(" + truncateStringWithEllipsisInTheMiddle(QString(i_byteArr), 50) + ")";
        }
        strAddTrcInfo += ", Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "sendData",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("sendData" );

    CRequest* pReq = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestSendData,
        /* strRequest   */ Request2Str(ERequestSendData),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    EState statePrev = m_state;

    if( i_byteArr.size() == 0 )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "No data to send." );

        if( pReq != nullptr )
        {
            pReq->setErrResultInfo(errResultInfo);
            pReq->setProgressInPerCent(100);
            pReq->update();

            // The request is finished and has to be deleted later.
            m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);

        } // if( pReq != nullptr )
    } // if( i_byteArr.size() == 0 )

    else if( i_byteArr.size() > 0 )
    {
        errResultInfo = checkSocket(i_iSocketId);

        if( errResultInfo.isErrorResult() )
        {
            if( pReq != nullptr )
            {
                pReq->setErrResultInfo(errResultInfo);
                pReq->setProgressInPerCent(100);
                pReq->update();

                // The request is finished and has to be deleted later.
                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);

            } // if( pReq != nullptr )
        } // if( errResultInfo.isErrorResult() )
    } // if( i_byteArr.size() > 0 )

    if( !errResultInfo.isErrorResult() )
    {
        // Please note that the "onRequestChanged" slot should also be connected
        // to the changed signal of the request if the request will be postponed
        // as postponed requests may have parent request which may be aborted.

        // The trace server may call the sendData method from any thread.
        // If it is called from another thread then the thread the
        // trace server and the request queue has been created in the
        // request may not be queued (and the mutex must have been created).
        // Otherwise the changed signal will be queued to the thread of
        // the request queue and the "onRequestChanged" slot may be called
        // after the request has been executed and the request has already
        // been destroyed.
        if( pReq->thread() == this->thread() )
        {
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
        }

        CMsgReqSendData* pMsgReq = new CMsgReqSendData(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pGateway,
            /* iSocketId        */ i_iSocketId,
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
            if( pReq->thread() != this->thread() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultRequestRefused);
                errResultInfo.setAddErrInfoDscr( "Cannot queue sendData request if called from another thread than the servers parent thread." );

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
            else if( pReq->isBlockingRequest() )
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

        // Please note that if executing this request did not become asynchronous the
        // "execute<>Request" method above has been called and the "onRequestChanged"
        // slot method has already been disconnected from the request's changed signal.

        if( statePrev != m_state )
        {
            emit stateChanged(this,m_state);
        }
    } // if( !errResultInfo.isErrorResult() )

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString( pReq == nullptr ? "SUCCESS" : pReq->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // sendData

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QList<ESocketType> CServer::getSocketTypes() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_arSocketTypes;
}

//------------------------------------------------------------------------------
int CServer::getSocketTypesCount() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_arSocketTypes.size();
}

//------------------------------------------------------------------------------
ESocketType CServer::getSocketType( int i_idx ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    ESocketType socketType = ESocketTypeUndefined;

    if( i_idx >= 0 && i_idx < m_arSocketTypes.size() )
    {
        socketType = m_arSocketTypes[i_idx];
    }
    return socketType;

} // getSocketType

//------------------------------------------------------------------------------
bool CServer::isSocketTypeChangeable() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return (m_arSocketTypes.size() > 1);
}

//------------------------------------------------------------------------------
bool CServer::isSocketTypeSupported( ESocketType i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

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
SServerHostSettings CServer::getHostSettings() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_hostSettings;
}

//------------------------------------------------------------------------------
void CServer::setHostSettings( const SServerHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "HostSettings: " + i_hostSettings.getConnectionString(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setHostSettings",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

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
            QString strAddErrInfo = "Socket type " + socketType2Str(i_hostSettings.m_socketType) + " is not supported";
            SErrResultInfo errResultInfo = ErrResultInfoError("setHostSettings", EResultArgOutOfRange, strAddErrInfo);
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
public: // instance methods of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
void CServer::setBlkType( CBlkType* i_pBlkType )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "BlkType: " + QString( i_pBlkType == nullptr ? "nullptr" : blkType2Str(i_pBlkType->type()) );
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setBlkType",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

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
CBlkType* CServer::getBlkType()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_pBlkType;
}

/*==============================================================================
public: // instance methods of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
int CServer::getSocketId( int i_idxSocket ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int          iSocketId = -1;
    SSocketDscr* pSocketDscr;

    if( i_idxSocket >= 0 && i_idxSocket < m_arpSocketDscr.size() )
    {
        pSocketDscr = m_arpSocketDscr[i_idxSocket];

        if( pSocketDscr != nullptr )
        {
            iSocketId = pSocketDscr->m_iSocketId;
        }
    }

    return iSocketId;

} // getSocketId

//------------------------------------------------------------------------------
SSocketDscr CServer::getSocketDscr( int i_iSocketId ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    SSocketDscr  socketDscr(ESrvCltTypeServer, m_hostSettings.m_socketType);
    SSocketDscr* pSocketDscr;

    if( i_iSocketId >= 0 && i_iSocketId < m_arpSocketDscr.size() )
    {
        pSocketDscr = m_arpSocketDscr[i_iSocketId];

        if( pSocketDscr != nullptr )
        {
            socketDscr = *pSocketDscr;
        }
    }

    return socketDscr;

} // getSocketDscr

//------------------------------------------------------------------------------
SSocketDscr CServer::findSocketDscr(
    const QString& i_strRemoteHostName,
    unsigned int   i_uRemotePort ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    SSocketDscr  socketDscr(ESrvCltTypeServer, m_hostSettings.m_socketType);
    SSocketDscr* pSocketDscr;

    for( int iSocketId = 0; iSocketId < m_arpSocketDscr.size(); iSocketId++ )
    {
        pSocketDscr = m_arpSocketDscr[iSocketId];

        if( pSocketDscr != nullptr )
        {
            if( (pSocketDscr->m_strRemoteHostName.compare(i_strRemoteHostName,Qt::CaseInsensitive) == 0)
             && (pSocketDscr->m_uRemotePort == i_uRemotePort) )
            {
                socketDscr = *pSocketDscr;
                break;
            }
        }
    }

    return socketDscr;

} // findSocketDscr

//------------------------------------------------------------------------------
int CServer::getArrLenConnections() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_arpSocketDscr.size();
}

//------------------------------------------------------------------------------
int CServer::getActiveConnections() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int iActiveConnections = 0;

    int iSocketId;

    for( iSocketId = 0; iSocketId < m_arpSocketDscr.size(); iSocketId++ )
    {
        if( m_arpSocketDscr[iSocketId] != nullptr )
        {
            iActiveConnections++;
        }
    }
    return iActiveConnections;

} // getActiveConnections

//------------------------------------------------------------------------------
void CServer::setSocketName( int i_iSocketId, const QString& i_strSocketName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "SocketId: " + QString::number(i_iSocketId);
        strAddTrcInfo += ", SocketName: " + i_strSocketName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setSocketName",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SSocketDscr* pSocketDscr;

    if( i_iSocketId >= 0 && i_iSocketId < m_arpSocketDscr.size() )
    {
        pSocketDscr = m_arpSocketDscr[i_iSocketId];

        if( pSocketDscr != nullptr )
        {
            if( pSocketDscr->m_strSocketName != i_strSocketName )
            {
                pSocketDscr->m_strSocketName = i_strSocketName;

                emit socketDscrChanged(this, *pSocketDscr);
            }
        }
    } // if( i_iSocketId >= 0 && i_iSocketId < m_arpSocketDscr.size() )

} // setSocketName


/*==============================================================================
public: // instance methods (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
CServer::EState CServer::getState() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_state;
}

//------------------------------------------------------------------------------
QString CServer::state2Str() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return State2Str(m_state);
}

//------------------------------------------------------------------------------
bool CServer::isListening() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return (m_state == EStateListening);
}

//------------------------------------------------------------------------------
bool CServer::isStartingUp() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bStartingUp = false;

    CRequest* pReq = m_pRequestQueue->getRequestInProgress();

    if( pReq != nullptr && pReq->getRequest() == ERequestStartup )
    {
        bStartingUp = true;
    }
    return bStartingUp;

} // isStartingUp

//------------------------------------------------------------------------------
bool CServer::isConnected( int i_iSocketId ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool         bIsConnected = false;
    int          iSocketId;
    SSocketDscr* pSocketDscr;

    int iActiveConnections = getActiveConnections();

    if( iActiveConnections > 0 )
    {
        switch( m_state )
        {
            case EStateIdle:
            {
                break;
            }
            case EStateListening:
            default:
            {
                if( i_iSocketId == ESocketIdUndefined )
                {
                    bIsConnected = true;
                }
                else if( i_iSocketId == ESocketIdAllSockets )
                {
                    bIsConnected = true;

                    for( iSocketId = 0; iSocketId < m_arpSocketDscr.size(); iSocketId++ )
                    {
                        pSocketDscr = m_arpSocketDscr[iSocketId];

                        if( pSocketDscr != nullptr )
                        {
                            if( pSocketDscr->m_socketState != ESocketStateConnected )
                            {
                                bIsConnected = false;
                                break;
                            }
                        }
                    }
                }
                else if( i_iSocketId >= 0 && i_iSocketId < m_arpSocketDscr.size() )
                {
                    pSocketDscr = m_arpSocketDscr[i_iSocketId];

                    if( pSocketDscr != nullptr )
                    {
                        if( pSocketDscr->m_socketState == ESocketStateConnected )
                        {
                            bIsConnected = true;
                        }
                    }
                }
                break;
            }
        } // switch( m_state )
    } // if( m_uActiveConnections > 0 )

    return bIsConnected;

} // isConnected

/*==============================================================================
public: // instance methods (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
bool CServer::isBusy() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return !m_pRequestQueue->isIdle();
}

//------------------------------------------------------------------------------
CServer::ERequest CServer::requestInProgress() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    ERequest  request = ERequestNone;
    CRequest* pReq    = m_pRequestQueue->getRequestInProgress();

    if( pReq != nullptr )
    {
        request = static_cast<ERequest>(pReq->getRequest());
    }
    return request;

} // requestInProgress

//------------------------------------------------------------------------------
QString CServer::requestInProgress2Str( bool /*i_bShort*/ ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return Request2Str( requestInProgress() );
}

//------------------------------------------------------------------------------
CRequest* CServer::getRequestInProgress() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_pRequestQueue->getRequestInProgress();
}

/*==============================================================================
public: // instance methods (aborting requests)
==============================================================================*/

//------------------------------------------------------------------------------
void CServer::abortRequest( qint64 i_iRequestId )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Request: " + CRequestExecTree::GetAddTrcInfoStr(i_iRequestId);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "abortRequest",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

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

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

} // abortRequest

//------------------------------------------------------------------------------
void CServer::abortRequestInProgress()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "abortRequestInProgress",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    CRequest* pReq = m_pRequestQueue->getRequestInProgress();

    if( pReq != nullptr )
    {
        abortRequest( pReq->getId() );
    }

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

} // abortRequestInProgress

//------------------------------------------------------------------------------
void CServer::abortAllRequests()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "abortAllRequests",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

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

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

} // abortAllRequests

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CServer::addTrcMsgLogObject( QObject* i_pObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( i_pObj == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pObj == nullptr");
    }

    int idxObj = m_arpTrcMsgLogObjects.indexOf(i_pObj);

    if( idxObj < 0 )
    {
        m_arpTrcMsgLogObjects.append(i_pObj);
    }

    if( m_pGateway != nullptr )
    {
        if( requestInProgress() != ERequestShutdown )
        {
            CServerGateway* pGateway = dynamic_cast<CServerGateway*>(m_pGateway);

            if( pGateway != nullptr )
            {
                pGateway->addTrcMsgLogObject(i_pObj);
            }
        }
    }

} // addTrcMsgLogObject

//------------------------------------------------------------------------------
void CServer::removeTrcMsgLogObject( QObject* i_pObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( i_pObj == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pObj == nullptr");
    }

    int idxObj = m_arpTrcMsgLogObjects.indexOf(i_pObj);

    if( idxObj >= 0 && idxObj < m_arpTrcMsgLogObjects.size() )
    {
        m_arpTrcMsgLogObjects.removeAt(idxObj);
    }

    if( m_pGateway != nullptr )
    {
        if( requestInProgress() != ERequestShutdown )
        {
            CServerGateway* pGateway = dynamic_cast<CServerGateway*>(m_pGateway);

            if( pGateway != nullptr )
            {
                pGateway->removeTrcMsgLogObject(i_pObj);
            }
        }
    }

} // removeTrcMsgLogObject

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CServer::onReceivedData( int i_iSocketId, const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    // Not necessary as method only called internally if mutex is already locked:
    //QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo += "SocketId: " + QString::number(i_iSocketId);

        SSocketDscr socketDscr = getSocketDscr(i_iSocketId);

        strAddTrcInfo += " (" + socketDscr.m_strRemoteHostName + ":" + QString::number(socketDscr.m_uServerListenPort) + ":" + QString::number(socketDscr.m_uRemotePort) + ")";

        strAddTrcInfo += ", ByteArr [" + QString::number(i_byteArr.size()) + "]";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onReceivedData",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

} // onReceivedData

/*==============================================================================
protected: // instance methods (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
void CServer::executeNextPostponedRequest()
//------------------------------------------------------------------------------
{
    // Not necessary as method only called internally if mutex is already locked:
    //QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "executeNextPostponedRequest",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
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
            POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ETraceDetailLevelRuntimeInfo);
        }
    } // if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )

} // executeNextPostponedRequest

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CServer::executeStartupRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    // Not necessary as method only called internally if mutex is already locked:
    //QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "executeStartupRequest",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
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

    // If the Ipc server in the gateway thread has not yet been started ...
    if( m_state == EStateIdle )
    {
        m_pGatewayThread = createGatewayThread();

        errResultInfo = startGatewayThread(10000, i_pReq->getId());

        if( !errResultInfo.isErrorResult() )
        {
            CRequest* pReqStartupGateway = startupGateway(
                /* iTimeout_ms  */ i_pReq->getTimeoutInMs(),
                /* bWait        */ i_pReq->isBlockingRequest(),
                /* iReqIdParent */ i_pReq->getId() );

            if( !isAsynchronousRequest(pReqStartupGateway) )
            {
                i_pReq->setErrResultInfo( pReqStartupGateway->getErrResultInfo() );
                i_pReq->setProgressInPerCent(100);
                i_pReq->update();
            }
        } // if( !errResultInfo.isErrorResult() )
    } // if( m_state == EStateIdle )

    else // if( m_state != EStateIdle )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Server is already running." );

    } // if( m_state != EStateIdle )

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

    if( statePrev != m_state )
    {
        emit stateChanged(this,m_state);
    }

    m_pRequestQueue->unlock();

    i_pReq->unlock();

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeStartupRequest

//------------------------------------------------------------------------------
void CServer::executeShutdownRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    // Not necessary as method only called internally if mutex is already locked:
    //QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "executeShutdownRequest",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
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

    if( m_state == EStateListening )
    {
        CRequest* pReqShutdownGateway = shutdownGateway(
            /* iTimeout_ms  */ i_pReq->getTimeoutInMs(),
            /* bWait        */ i_pReq->isBlockingRequest(),
            /* iReqIdParent */ i_pReq->getId() );

        if( !isAsynchronousRequest(pReqShutdownGateway) )
        {
            errResultInfo = stopGatewayThread(i_pReq->getTimeoutInMs(), i_pReq->getId());

            if( errResultInfo.isErrorResult() )
            {
                if( m_pErrLog != nullptr )
                {
                    m_pErrLog->addEntry(errResultInfo);
                }
            }

            i_pReq->setErrResultInfo( pReqShutdownGateway->getErrResultInfo() );
            i_pReq->setProgressInPerCent(100);
            i_pReq->update();
        }
    } // if( m_state == EStateListening )

    else // if( m_state != EStateListening )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Server is not started." );

    } // if( m_state != EStateListening )

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

    if( statePrev != m_state )
    {
        emit stateChanged(this,m_state);
    }

    m_pRequestQueue->unlock();

    i_pReq->unlock();

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeShutdownRequest

//------------------------------------------------------------------------------
void CServer::executeChangeSettingsRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    // Not necessary as method only called internally if mutex is already locked:
    //QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "executeChangeSettingsRequest",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
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

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("executeChangeSettingsRequest" );

    EState statePrev = m_state;

    bool bSettingsChanged = false;

    // If the Ipc server in the gateway thread has not yet been started ...
    if( m_state == EStateIdle )
    {
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();

    } // if( m_state == EStateIdle )

    else // if( m_state != EStateIdle )
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
                /* bMustBeConfirmed */ true,
                /* iReqId           */ i_pReq->getId() );
            pMsgReq->setBlkType(m_pBlkType); // The block will be cloned.

            POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ETraceDetailLevelRuntimeInfo);
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
    } // if( m_state != EStateIdle )

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

    if( statePrev != m_state )
    {
        emit stateChanged(this,m_state);
    }

    m_pRequestQueue->unlock();

    i_pReq->unlock();

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeChangeSettingsRequest

//------------------------------------------------------------------------------
void CServer::executeSendDataRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    // Not necessary as method only called internally if mutex is already locked:
    //QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "executeSendDataRequest",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
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

    // If the Ipc server in the gateway thread has been started ...
    if( m_state == EStateListening )
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

                POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ETraceDetailLevelRuntimeInfo);
                pMsgReq = nullptr;
            }

            errResultInfo.setErrResult(ErrResultSuccess);

            i_pReq->setErrResultInfo(errResultInfo);
            i_pReq->setProgressInPerCent(100);
            i_pReq->update();

        } // if( m_pGateway != nullptr )
    } // if( m_state == EStateListening )

    // If the Ipc server in the gateway thread has not yet been started ...
    else // if( m_state != EStateListening )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Server is not listening." );

    } // if( m_state != EStateListening )

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

        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...

        // The trace server may call the sendData method from any thread.
        // If it is called from another thread then the thread the
        // trace server and the request queue has been created in the
        // request may not be queued (and the mutex must have been created).
        // Otherwise the changed signal will be queued to the thread of
        // the request queue and the "onRequestChanged" slot may be called
        // after the request has been executed and the request has already
        // been destroyed.
        if( i_pReq->thread() == this->thread() )
        {
            QObject::disconnect(
                /* pObjSender   */ i_pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );
        }

        m_pRequestQueue->setSyncRequestToBeDeletedLater(i_pReq);

    } // if( !isAsynchronousRequest(i_pReq) )

    if( statePrev != m_state )
    {
        emit stateChanged(this, m_state);
    }

    m_pRequestQueue->unlock();

    i_pReq->unlock();

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

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
CSrvCltBaseGatewayThread* CServer::createGatewayThread()
//------------------------------------------------------------------------------
{
    // Not necessary as method only called internally if mutex is already locked:
    //QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "createGatewayThread",
        /* strAddInfo         */ strAddTrcInfo );

    return new CServerGatewayThread(
        /* szObjNameGateway       */ m_strObjName,
        /* pServer                */ this,
        /* pModelErrLog           */ m_pErrLog,
        /* pTrcMthFile            */ m_pTrcMthFile,
        /* iTrcMthFileDEtailLevel */ m_iTrcMthFileDetailLevel );

} // createGatewayThread

//------------------------------------------------------------------------------
SErrResultInfo CServer::startGatewayThread( int i_iTimeout_ms, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    // Not necessary as method only called internally if mutex is already locked:
    //QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "startGatewayThread",
        /* strAddInfo         */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("startGatewayThread");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqStartThread = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestStartGatewayThread,
        /* strRequest   */ Request2Str(ERequestStartGatewayThread),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ true );

    // Please see note in run method of class CSrvCltBaseGatewayThread
    // on synchronizing with the thread using startup confirmation messages.
    m_pGatewayThread->start( pReqStartThread->getId() );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
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

                CServerGateway* pGateway = dynamic_cast<CServerGateway*>(m_pGateway);

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

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        mthTracer.trace( "+- ReqStartThread.wait(): " + errResultInfo.getAddErrInfoDscr() );
    }

    pReqStartThread->setErrResultInfo(errResultInfo);
    pReqStartThread->setProgressInPerCent(100);
    pReqStartThread->update();

    deleteRequest(pReqStartThread);
    pReqStartThread = nullptr;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = errResultInfo.getAddErrInfoDscr();
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return errResultInfo;

} // startGatewayThread

//------------------------------------------------------------------------------
SErrResultInfo CServer::stopGatewayThread( int i_iTimeout_ms, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    // Not necessary as method only called internally if mutex is already locked:
    //QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "stopGatewayThread",
        /* strAddInfo         */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("stopGatewayThread");

    if( m_pGatewayThread != nullptr )
    {
        // Initialized as an asynchronous request (ResultUndefined).
        CRequest* pReqStopThread = m_pRequestQueue->createRequest(
            /* strNameSpace */ nameSpace(),
            /* strClassName */ className(),
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

    SSocketDscr socketDscr(ESrvCltTypeServer);
    int         iSocketId;

    for( iSocketId = 0; iSocketId < m_arpSocketDscr.size(); iSocketId++ )
    {
        if( m_arpSocketDscr[iSocketId] != nullptr )
        {
            socketDscr = *m_arpSocketDscr[iSocketId];
            socketDscr.m_socketState = ESocketStateUnconnected;

            try
            {
                delete m_arpSocketDscr[iSocketId];
            }
            catch(...)
            {
            }
            m_arpSocketDscr[iSocketId] = nullptr;

            // If "stopGatewayThread" is called by the dtor don't emit any other
            // signals than destroyed. The receiver of the "disconnected" and
            // "stateChanged" signals may not access the object on receiving the
            // signals if the object is being destroyed (e.g. dynamic_cast will fail).
            if( !m_bIsBeingDestroyed )
            {
                emit disconnected(this,socketDscr);
            }
        }
    }

    m_arpSocketDscr.clear();

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = errResultInfo.getAddErrInfoDscr();
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return errResultInfo;

} // stopGatewayThread

/*==============================================================================
protected: // overridables (auxiliary methods)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CServer::startupGateway( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    // Not necessary as method only called internally if mutex is already locked:
    //QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "startupGateway",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("startupGateway" );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqStartupGateway = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestStartupGateway,
        /* strRequest   */ Request2Str(ERequestStartupGateway),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    // Please also note that if called from within different thread contexts signal/slot
    // connections for requests cannot be queued as otherwise on updating a request the
    // "requestChanged" slot may be called sometimes later not updating the internal states
    // at the expected time (before e.g. the "startup" method returns but sometimes later).

    if( !QObject::connect(
        /* pObjSender   */ pReqStartupGateway,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)),
        /* cnctType     */ m_pMtx != nullptr ? Qt::DirectConnection : Qt::AutoConnection) )
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultSignalSlotConnectionFailed);
    }

    if( m_pGateway == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Gateway is not created." );
    }
    else // if( m_pGateway != nullptr )
    {
        // The parent request must have been the startup request.
        CRequest* pReqStartup = m_pRequestQueue->getRequestInProgress(i_iReqIdParent);

        if( pReqStartup == nullptr )
        {
            throw CException(__FILE__, __LINE__, EResultInternalStateMachineError, "Parent request of connect gateway must be the startup request" );
        }
        else if( pReqStartup->getRequest() != ERequestStartup )
        {
            throw CException(__FILE__, __LINE__, EResultInternalStateMachineError, "Parent request of connect gateway must be the startup request" );
        }

        CMsgReqStartup* pMsgReq = new CMsgReqStartup(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pGateway,
            /* hostSettings     */ m_hostSettings,
            /* pBlkType         */ m_pBlkType, // will be cloned
            /* bMustBeConfirmed */ true,
            /* iReqId           */ pReqStartupGateway->getId() );

        m_pRequestQueue->addRequestInProgress(pReqStartupGateway);

        POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ETraceDetailLevelRuntimeInfo);

    } // if( m_pGateway != nullptr )

    if( pReqStartupGateway->isBlockingRequest() )
    {
        if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
        {
            mthTracer.trace( "-+ ReqStartupGateway.wait()" );
        }

        if( pReqStartupGateway->wait() )
        {
            CMsgCon* pMsgCon = pReqStartupGateway->getExecutionConfirmationMessage();
            if( pMsgCon != nullptr )
            {
                errResultInfo = pMsgCon->getErrResultInfo();
            }
            else
            {
                errResultInfo = pReqStartupGateway->getErrResultInfo();
            }
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }
        }
        else // if( !pReqStartupGateway->wait() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultTimeout);
            errResultInfo.setAddErrInfoDscr( "Waiting for connect confirmation timed out." );

            if( m_pErrLog != nullptr )
            {
                m_pErrLog->addEntry(errResultInfo);
            }
        }

        if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
        {
            mthTracer.trace( "+- ReqStartupGateway.wait(): " + errResultInfo.getAddErrInfoDscr() );
        }

        //m_pRequestQueue->lock();
        pReqStartupGateway->setErrResultInfo(errResultInfo);
        pReqStartupGateway->setProgressInPerCent(100);
        pReqStartupGateway->update(); // onRequestChanged is called removing the request from the requestQueue
        //m_pRequestQueue->unlock();

    } // if( pReqStartupGateway->isBlockingRequest() )

    else // if( !pReqStartupGateway->isBlockingRequest() )
    {
        if( i_iTimeout_ms > 0 )
        {
            m_reqDscrTimeout = pReqStartupGateway->getDescriptor();
            m_pTmrReqTimeout->start(i_iTimeout_ms);
        }
    }

    if( !isAsynchronousRequest(pReqStartupGateway) )
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
        m_pRequestQueue->setSyncRequestToBeDeletedLater(pReqStartupGateway);
    }

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString( pReqStartupGateway == nullptr ? "SUCCESS" : pReqStartupGateway->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqStartupGateway;

} // startupGateway

//------------------------------------------------------------------------------
CRequest* CServer::shutdownGateway( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    // Not necessary as method only called internally if mutex is already locked:
    //QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "shutdownGateway",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("shutdownGateway" );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqShutdownGateway = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestShutdownGateway,
        /* strRequest   */ Request2Str(ERequestShutdownGateway),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    // Please also note that if called from within different thread contexts signal/slot
    // connections for requests cannot be queued as otherwise on updating a request the
    // "requestChanged" slot may be called sometimes later not updating the internal states
    // at the expected time (before e.g. the "startup" method returns but sometimes later).

    if( !QObject::connect(
        /* pObjSender   */ pReqShutdownGateway,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)),
        /* cnctType     */ m_pMtx != nullptr ? Qt::DirectConnection : Qt::AutoConnection) )
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultSignalSlotConnectionFailed);
    }

    if( m_pGateway == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Gateway is not created." );

        pReqShutdownGateway->setErrResultInfo(errResultInfo);
        pReqShutdownGateway->setProgressInPerCent(100);
        pReqShutdownGateway->update();

        if( m_pErrLog != nullptr )
        {
            m_pErrLog->addEntry(errResultInfo);
        }
    }
    else // if( m_pGateway != nullptr )
    {
        // The parent request must have been the shutdown request.
        CRequest* pReqShutdown = m_pRequestQueue->getRequestInProgress(i_iReqIdParent);

        if( pReqShutdown == nullptr )
        {
            throw CException(__FILE__, __LINE__, EResultInternalStateMachineError, "Parent request of connect gateway must be the shutdown request" );
        }
        else if( pReqShutdown->getRequest() != ERequestShutdown )
        {
            throw CException(__FILE__, __LINE__, EResultInternalStateMachineError, "Parent request of connect gateway must be the shutdown request" );
        }

        CMsgReqShutdown* pMsgReq = new CMsgReqShutdown(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pGateway,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ pReqShutdownGateway->getId() );

        m_pRequestQueue->addRequestInProgress(pReqShutdownGateway);

        POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ETraceDetailLevelRuntimeInfo);

    } // if( m_pGateway != nullptr )

    if( pReqShutdownGateway->isBlockingRequest() )
    {
        if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
        {
            mthTracer.trace( "-+ ReqShutdownGateway.wait()" );
        }

        if( pReqShutdownGateway->wait() )
        {
            CMsgCon* pMsgCon = pReqShutdownGateway->getExecutionConfirmationMessage();
            if( pMsgCon != nullptr )
            {
                errResultInfo = pMsgCon->getErrResultInfo();
            }
            else
            {
                errResultInfo = pReqShutdownGateway->getErrResultInfo();
            }
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }
        }
        else // if( !pReqShutdownGateway->wait() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultTimeout);
            errResultInfo.setAddErrInfoDscr( "Waiting for disconnect confirmation timed out." );

            if( m_pErrLog != nullptr )
            {
                m_pErrLog->addEntry(errResultInfo);
            }
        }

        if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
        {
            mthTracer.trace( "+- ReqShutdownGateway.wait(): " + errResultInfo.getAddErrInfoDscr() );
        }

        // ShutdownGateway is an internal sequencer request. No one else may have
        // signal/slot connections to this request. So there is no need to lock
        // the queue on updating this request (but its necessary to update the
        // request to indicate the progress in the request execution tree).
        //m_pRequestQueue->lock();
        pReqShutdownGateway->setErrResultInfo(errResultInfo);
        pReqShutdownGateway->setProgressInPerCent(100);
        pReqShutdownGateway->update();
        //m_pRequestQueue->unlock();

    } // if( pReqShutdownGateway->isBlockingRequest() )

    else // if( !pReqShutdownGateway->isBlockingRequest() )
    {
        if( i_iTimeout_ms > 0 )
        {
            m_reqDscrTimeout = pReqShutdownGateway->getDescriptor();
            m_pTmrReqTimeout->start(i_iTimeout_ms);
        }
    }

    if( !isAsynchronousRequest(pReqShutdownGateway) )
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
        m_pRequestQueue->setSyncRequestToBeDeletedLater(pReqShutdownGateway);
    }

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString( pReqShutdownGateway == nullptr ? "SUCCESS" : pReqShutdownGateway->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqShutdownGateway;

} // shutdownGateway

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CServer::onRequestTimeout()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onRequestTimeout",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    m_pTmrReqTimeout->stop();

    CRequest* pReq = m_pRequestQueue->getRequestInProgress(m_reqDscrTimeout.m_iId);

    if( pReq != nullptr )
    {
        SErrResultInfo errResultInfo = ErrResultInfoError("onRequestTimeout", EResultTimeout, pReq->getAddErrInfoDscr());
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

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

} // onRequestTimeout

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CServer::onRequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    // Necessary as this slot may be called on updating requests from another thread
    // (direct but not queued signal/slot connections).
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onRequestChanged",
        /* strAddInfo         */ strAddTrcInfo );

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

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

        m_pRequestQueue->lock();

        // Please note that not all error results should end up in the error log file.
        // This depends which request could not have been successfully processed.

        // If the changed request has been finished ...
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

        // Handle the changed request
        //---------------------------

        // The Ipc server does not trigger requests of "foreign" classes.
        // So the following string compare is unnecessary as this slot may only
        // be called if the server itself updates the request.
        //if( i_reqDscr.m_strCreatorClassName.compare(className(),Qt::CaseInsensitive) == 0 )

        if( !bIsPostponedRequest )
        {
            // Confirmation messages received from the gateway.
            CMsgCon* pMsgConExec = pReqChanged->getExecutionConfirmationMessage();

            switch( i_reqDscr.m_request )
            {
                case ERequestStartup:
                {
                    // If the request has been finished ..
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        if( errResultInfo.isErrorResult() )
                        {
                            SErrResultInfo errResultInfoTmp = stopGatewayThread();

                            if( errResultInfoTmp.isErrorResult() )
                            {
                                if( m_pErrLog != nullptr )
                                {
                                    m_pErrLog->addEntry(errResultInfoTmp);
                                }
                            }

                            m_state = EStateIdle;

                        } // if( errResultInfo.isErrorResult() )

                        else // if( !errResultInfo.isErrorResult() )
                        {
                            m_state = EStateListening;
                        }
                    } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                    break;
                } // case ERequestStartup

                case ERequestShutdown:
                {
                    // If the request has been finished ..
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        m_state = EStateIdle;
                    }
                    break;
                } // case ERequestShutdown

                case ERequestChangeSettings:
                {
                    break;
                } // case ERequestChangeSettings

                case ERequestSendData:
                {
                    break;
                }

                case ERequestStartupGateway:
                {
                    // If the sub request has been finished ..
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        CMsgConStartup* pMsgConStartup = dynamic_cast<CMsgConStartup*>(pMsgConExec);

                        if( pMsgConStartup != nullptr )
                        {
                            if( !errResultInfo.isErrorResult() )
                            {
                                errResultInfo = pMsgConStartup->getErrResultInfo();
                            }
                        } // if( pMsgConConnect != nullptr )

                        if( errResultInfo.isErrorResult() )
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
                } // case ERequestStartupGateway

                case ERequestShutdownGateway:
                {
                    // If the sub request has been finished ..
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        CMsgConShutdown* pMsgConShutdown = dynamic_cast<CMsgConShutdown*>(pMsgConExec);

                        if( pMsgConShutdown != nullptr )
                        {
                            if( !errResultInfo.isErrorResult() )
                            {
                                errResultInfo = pMsgConShutdown->getErrResultInfo();
                            }
                        } // if( pMsgConShutdown != nullptr )

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

                    break;
                } // case ERequestShutdownGateway

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
            if( !pReqChanged->isLocked() )
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

        m_pRequestQueue->unlock();

        if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
        {
            executeNextPostponedRequest();
        }
    } // if( !errResultInfo.isErrorResult() )

    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

} // onRequestChanged

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CServer::checkSocket( int i_iSocketId ) const
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo = ErrResultInfoSuccess("checkSocket");

    int iActiveConnections = getActiveConnections();

    if( iActiveConnections == 0 )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "No active connection." );
    }
    else if( i_iSocketId != ESocketIdAllSockets )
    {
        if( i_iSocketId < 0 || i_iSocketId >= m_arpSocketDscr.size() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Socket id out of range." );
        }
        else if( m_arpSocketDscr[i_iSocketId] == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "No active connection at socket id." );
        }
        else if( m_arpSocketDscr[i_iSocketId]->m_iSocketId != i_iSocketId )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Internal program error." );
        }
    }

    return errResultInfo;

} // checkSocket

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CServer::isMethodTraceActive( int i_iFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(i_iFilterDetailLevel) )
    {
        bIsActive = true;
    }
    else if( m_pTrcMthFile != nullptr && m_iTrcMthFileDetailLevel >= i_iFilterDetailLevel )
    {
        bIsActive = true;
    }
    return bIsActive;

} // isMethodTraceActive

//------------------------------------------------------------------------------
int CServer::getMethodTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    int iDetailLevel = ETraceDetailLevelNone;

    if( m_pTrcAdminObj != nullptr )
    {
        iDetailLevel = getMethodTraceDetailLevel();
    }
    else if( m_pTrcMthFile != nullptr )
    {
        iDetailLevel = m_iTrcMthFileDetailLevel;
    }
    return iDetailLevel;

} // getMethodTraceDetailLevel

/*==============================================================================
protected: // overridables of inherited class QObject (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
bool CServer::event( QEvent* i_pMsg )
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

        if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
        {
            int iAddTrcInfoDetailLevel = 0;
            if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
            else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

            strAddTrcInfo = "Msg {" + QString( pMsg == nullptr ? "nullptr" : pMsg->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
        }

        CMethodTracer mthTracer(
            /* pAdminObj          */ m_pTrcAdminObj,
            /* pTrcMthFile        */ m_pTrcMthFile,
            /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ nameSpace(),
            /* strClassName       */ className(),
            /* strObjName         */ objectName(),
            /* strMethod          */ "event",
            /* strAddInfo         */ strAddTrcInfo );

        if( pMsg != nullptr )
        {
            QMutexLocker mtxLocker(m_pMtx);

            bEventHandled = true;

            SErrResultInfo errResultInfo = ErrResultInfoSuccess(pMsg->msgTypeToStr());

            if( pMsg->isBaseMsgType() )
            {
                if( pMsg->getMsgType() == ZS::System::EBaseMsgTypeIndErr )
                {
                    CMsgErr* pMsgErr = dynamic_cast<CMsgErr*>(pMsg);
                    if( pMsgErr == nullptr )
                    {
                        throw CException(__FILE__, __LINE__, EResultInvalidMessageContent, strAddTrcInfo);
                    }

                    errResultInfo = pMsgErr->getErrResultInfo();

                    if( m_pErrLog != nullptr && !errResultInfo.isAddedToErrLogModel() )
                    {
                        m_pErrLog->addEntry(errResultInfo);
                        errResultInfo.setAddedToErrLogModel();
                    }
                } // if( pMsg->getMsgType() == ZS::System::EBaseMsgTypeIndErr )

                else if( pMsg->getMsgType() == EBaseMsgTypeReqContinue )
                {
                    m_bMsgReqContinuePending = false;

                    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
                    {
                        CRequest* pReqPostponed = m_pRequestQueue->takeFirstPostponedRequest();

                        if( pReqPostponed != nullptr )
                        {
                            switch( pReqPostponed->getRequest() )
                            {
                                case ERequestStartup:
                                {
                                    executeStartupRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestShutdown:
                                {
                                    executeShutdownRequest(pReqPostponed);
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
                } // if( pMsg->getMsgType() == EBaseMsgTypeReqContinue )
            } // if( pMsg->isBaseMsgType() )

            else // if( !pMsg->isBaseMsgType() )
            {
                if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
                {
                    int iAddTrcInfoDetailLevel = 0;
                    if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
                    else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

                    strAddTrcInfo  = "State: " + State2Str(m_state);
                    strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
                    mthTracer.trace(strAddTrcInfo);
                }

                CRequest* pReq;

                EState statePrev = m_state;

                if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq )
                {
                    //--------------------------------------------------------------
                    // Sending request messages to the server by proxies is not yet supported.
                    // For this setting request and confirmation messages at the request instance
                    // must be revised as for blocking requests the gateway also sets the
                    // confirmation message to indicate the result of the executed request.
                    //--------------------------------------------------------------

                    CMsgReq* pMsgReq = dynamic_cast<CMsgReq*>(pMsg);
                    if( pMsgReq == nullptr )
                    {
                        throw CException(__FILE__, __LINE__, EResultMessageTypeMismatch);
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

                    //        POST_OR_DELETE_MESSAGE(pMsgCon, &mthTracer, ETraceDetailLevelRuntimeInfo);
                    //        pMsgCon = nullptr;
                    //    }
                    //} // if( !isAsynchronousRequest(pReq) )

                    //delete pMsgCon;
                    //pMsgCon = nullptr;

                } // if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq )

                else if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeInd )
                {
                    //----------------------------------------------------------
                    // Indication messages sent by the gateway to this server.
                    //----------------------------------------------------------

                    CMsgInd* pMsgInd = dynamic_cast<CMsgInd*>(pMsg);
                    if( pMsgInd == nullptr )
                    {
                        throw CException(__FILE__, __LINE__, EResultMessageTypeMismatch);
                    }

                    if( m_pGateway != nullptr && pMsgInd->getSender() != m_pGateway )
                    {
                        throw CException (__FILE__, __LINE__, EResultInvalidSenderObjInMessage);
                    }

                    // Please note that messages may be sent by the gateway right before the gateway
                    // is destroyed. In this case the message may be received right after the gateway
                    // has been destroyed, we can't access the message sender (the gateway) anymore
                    // and also a dynamic_cast from the message sender to the gateway object will
                    // result in an access violation exception.

                    switch( static_cast<int>(pMsgInd->type()) )
                    {
                        //------------------------------------------------------------------
                        case EMsgTypeIndConnected:
                        //------------------------------------------------------------------
                        {
                            CMsgIndConnected* pMsgInd = dynamic_cast<CMsgIndConnected*>(pMsg);
                            if( pMsgInd == nullptr )
                            {
                                throw ZS::System::CException(__FILE__, __LINE__, EResultMessageTypeMismatch, "MsgIndConnected == nullptr! " + strAddTrcInfo);
                            }

                            // Please note that the server could have been shutdown in the meantime .
                            if( m_state != EStateIdle && requestInProgress() != ERequestShutdown )
                            {
                                int iSocketId = pMsgInd->getSocketId();

                                if( iSocketId >= m_arpSocketDscr.size() )
                                {
                                    m_arpSocketDscr.resize(iSocketId+1);

                                    for( ; iSocketId < m_arpSocketDscr.size(); iSocketId++ )
                                    {
                                        m_arpSocketDscr[iSocketId] = nullptr;
                                    }
                                }

                                iSocketId = pMsgInd->getSocketId(); // SocketId may have been modified if array has been resized.

                                if( m_arpSocketDscr[iSocketId] != nullptr )
                                {
                                    QString strAddTrcInfo = "event( ";
                                    strAddTrcInfo += "MsgIndConnected: SocketId = " + QString::number(iSocketId) + " already used )";
                                    throw CException(__FILE__, __LINE__, EResultSocketIdAlreadyUsed, strAddTrcInfo);
                                }

                                m_arpSocketDscr[iSocketId] = new SSocketDscr( pMsgInd->getSocketDscr() );
                                emit connected( this, getSocketDscr(iSocketId) );

                            } // if( m_state != EStateIdle && getRequestInProgress() != ERequestShutdown )
                            break;
                        } // case EMsgTypeIndConnected

                        //------------------------------------------------------------------
                        case EMsgTypeIndDisconnected:
                        //------------------------------------------------------------------
                        {
                            CMsgIndDisconnected* pMsgInd = dynamic_cast<CMsgIndDisconnected*>(pMsg);
                            if( pMsgInd == nullptr )
                            {
                                throw ZS::System::CException(__FILE__, __LINE__, EResultMessageTypeMismatch, "MsgIndDisconnected == nullptr! " + strAddTrcInfo);
                            }

                            // Please note that the server could have been shutdown in the meantime .
                            if( m_state != EStateIdle && requestInProgress() != ERequestShutdown )
                            {
                                SSocketDscr socketDscr(ESrvCltTypeServer);
                                int         iSocketId = pMsgInd->getSocketId();

                                errResultInfo = checkSocket(iSocketId);

                                if( errResultInfo.getResult() != EResultSuccess )
                                {
                                    if( m_pErrLog != nullptr )
                                    {
                                        m_pErrLog->addEntry(errResultInfo);
                                    }
                                }
                                else
                                {
                                    socketDscr = *m_arpSocketDscr[iSocketId];
                                    socketDscr.m_socketState = ESocketStateUnconnected;

                                    try
                                    {
                                        delete m_arpSocketDscr[iSocketId];
                                    }
                                    catch(...)
                                    {
                                    }
                                    m_arpSocketDscr[iSocketId] = nullptr;
                                    emit disconnected(this,socketDscr);
                                }

                            } // if( m_state != EStateIdle && getRequestInProgress() != ERequestShutdown )
                            break;
                        } // case EMsgTypeIndDisconnected

                        //------------------------------------------------------------------
                        case EMsgTypeIndReceivedData:
                        //------------------------------------------------------------------
                        {
                            CMsgIndReceivedData* pMsgInd = dynamic_cast<CMsgIndReceivedData*>(pMsg);
                            bool                 bValidMessage = true;
                            int                  iSocketId = -1;

                            if( pMsgInd != nullptr )
                            {
                                iSocketId = pMsgInd->getSocketId();
                            }

                            // The server may have been shutdown and is waiting for the gateway thread
                            // to be quit. But the gateway may have been receiving data while the server
                            // is waiting for the thread to be finished and therefore indication messages
                            // may already be in Qt's event loop that will be dispatched to the server
                            // after he has been shutdown. So we ignore the indication messages if the
                            // server does no longer expect them and the socket id is no longer valid.
                            if( pMsgInd == nullptr )
                            {
                                bValidMessage = false;
                            }
                            else if( m_state == EStateIdle )
                            {
                                bValidMessage = false;
                            }
                            else if( pMsgInd->getSender() != m_pGateway )
                            {
                                bValidMessage = false;
                            }
                            else if( iSocketId < 0 || iSocketId >= m_arpSocketDscr.size() )
                            {
                                bValidMessage = false;
                            }
                            else if( m_arpSocketDscr[iSocketId] == nullptr )
                            {
                                bValidMessage = false;
                            }
                            else if( m_arpSocketDscr[iSocketId]->m_iSocketId != iSocketId )
                            {
                                bValidMessage = false;
                            }
                            if( bValidMessage )
                            {
                                onReceivedData( iSocketId, pMsgInd->getByteArray() );

                                emit receivedData( this, iSocketId, pMsgInd->getByteArray() );
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
                                throw CException(__FILE__, __LINE__, EResultInvalidMessage, strAddErrInfo);
                            }
                        }
                    } // switch( pMsgInd->type() )
                } // if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeInd )

                else if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeCon )
                {
                    CMsgCon* pMsgCon = dynamic_cast<CMsgCon*>(pMsg);
                    if( pMsgCon == nullptr )
                    {
                        throw CException(__FILE__, __LINE__, EResultMessageTypeMismatch);
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

                if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
                {
                    int iAddTrcInfoDetailLevel = 0;
                    if( getMethodTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
                    else if( getMethodTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

                    strAddTrcInfo  = "State: " + State2Str(m_state);
                    strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
                    mthTracer.trace(strAddTrcInfo);
                }

                if( statePrev != m_state )
                {
                    emit stateChanged(this,m_state);
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
