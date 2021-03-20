/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include "DatabaseThread.h"
#include "Msg.h"

#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysSleeperThread.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::AsyncRequests;
using namespace ZS::Apps::Test::AsyncRequests::Database;


/*******************************************************************************
class CDbThread : public QThread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDbThread::CDbThread( const QString& i_strObjName ) :
//------------------------------------------------------------------------------
    QThread(),
    m_pDb(nullptr),
    m_state(EStateCreated),
    m_requestQueue(this,true),
    m_reqDscrTimeout(),
    m_pTmrReqTimeout(nullptr),
    m_bMsgReqContinuePending(false),
    m_iReqIdStartThread(-1),
    m_bReqModeSingleShot(false),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::AsyncRequests", "CDbThread", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pTmrReqTimeout = new QTimer(this);

    if( !QObject::connect(
        /* pObjSender   */ m_pTmrReqTimeout,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestTimeout()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CDbThread::~CDbThread()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    quit();

    if( !wait(30000) )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            SErrResultInfo errResultInfo = ErrResultInfoError("dtor", EResultTimeout, "Waiting for thread to quit timed out");
            CErrLog::GetInstance()->addEntry(errResultInfo);
         }
    }

    try
    {
        delete m_pDb;
    }
    catch(...)
    {
    }
    m_pDb = nullptr;

    m_pTmrReqTimeout = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbThread::setReqModeSingleShot( bool i_bModeSingleShot )
//------------------------------------------------------------------------------
{
    m_bReqModeSingleShot = i_bModeSingleShot;

    if( m_pDb != nullptr )
    {
        // In a real application you shouldn't access member variables of objects
        // living in another thread without protecting them using a mutex.
        m_pDb->setReqModeSingleShot(m_bReqModeSingleShot);
    }

} // setReqModeSingleShot

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbThread::startup( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startup",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("startup");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbThread",
        /* iRequest     */ ERequestStartup,
        /* strRequest   */ Request2Str(ERequestStartup),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_iReqIdParent) )
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

            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }

            //m_requestQueue.lock(); // Not necessary as request queue is not idle.
            pReq->setErrResultInfo(errResultInfo);
            pReq->setProgressInPerCent(100);
            pReq->update();
            //m_requestQueue.unlock();
        }
        else
        {
            EResult resultPostpone = m_requestQueue.postponeRequest(pReq);

            if( resultPostpone != EResultSuccess )
            {
                pReq->setSeverity(EResultSeverityError);
                pReq->setResult(resultPostpone);
                pReq->setAddErrInfoDscr( "Request could not have been queued." );
            }
        }
    } // if( bPostponeRequest )

    else // if( !bPostponeRequest )
    {
        executeStartupRequest(pReq,pReqInProgress);
    }

    return pReq;

} // startup

//------------------------------------------------------------------------------
CRequest* CDbThread::shutdown( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "shutdown",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("shutdown");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbThread",
        /* iRequest     */ ERequestShutdown,
        /* strRequest   */ Request2Str(ERequestShutdown),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_iReqIdParent) )
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

            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }

            //m_requestQueue.lock(); // Not necessary as request queue is not idle.
            pReq->setErrResultInfo(errResultInfo);
            pReq->setProgressInPerCent(100);
            pReq->update();
            //m_requestQueue.unlock();
        }
        else
        {
            EResult resultPostpone = m_requestQueue.postponeRequest(pReq);

            if( resultPostpone != EResultSuccess )
            {
                pReq->setSeverity(EResultSeverityError);
                pReq->setResult(resultPostpone);
                pReq->setAddErrInfoDscr( "Request could not have been queued." );
            }
        }
    } // if( bPostponeRequest )

    else // if( !bPostponeRequest )
    {
        executeShutdownRequest(pReq,pReqInProgress);
    }

    return pReq;

} // shutdown

/*==============================================================================
protected: // overridables (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
void CDbThread::executeNextPostponedRequest()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeNextPostponedRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
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
            POST_OR_DELETE_MESSAGE(pMsgReq);
        }
    } // if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )

} // executeNextPostponedRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbThread::executeStartupRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "ReqStartup: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeStartupRequest",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    i_pReq->lock();

    m_requestQueue.lock();

    // If started as child request of request in progress (as part of a request sequence) ..
    if( i_pReqMainInProgress != nullptr )
    {
        m_requestQueue.addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_requestQueue.setRequestInProgress(i_pReq);
    }

    // If the thread is not yet running ...
    if( m_state == EStateCreated )
    {
        // The Startup request is split into two child requests (StartThread and DbStartup).
        // The child requests should also be indicated in the request tree.
        CRequest* pReqStartThread = m_requestQueue.createRequest(
            /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
            /* strClassName */ "CDbThread",
            /* iRequest     */ ERequestStartThread,
            /* strRequest   */ Request2Str(ERequestStartThread),
            /* iReqIdParent */ i_pReq->getId(),
            /* iTimeout_ms  */ i_pReq->getTimeoutInMs(),
            /* bIsBlocking  */ i_pReq->isBlockingRequest() );

        if( !QObject::connect(
            /* pObjSender   */ pReqStartThread,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        m_requestQueue.addRequestInProgress(pReqStartThread);

        m_iReqIdStartThread = pReqStartThread->getId();

        start();

        if( pReqStartThread->isBlockingRequest() )
        {
            if( !pReqStartThread->wait() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for StartThread timed out." );
            }

            // The sub request has been finished.
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }

            pReqStartThread->setErrResultInfo(errResultInfo);
            pReqStartThread->setProgressInPerCent(100);
            pReqStartThread->lock();
            pReqStartThread->update(); // The "onRequestChanged" slot is called removing the request from the request queue.
            pReqStartThread->unlock();

            deleteRequest(pReqStartThread); // The "onRequestChanged" slot will be called again.
            pReqStartThread = nullptr;

        } // if( pReqStartThread->isBlockingRequest() )

        else // if( !pReqStartThread->isBlockingRequest() )
        {
            if( pReqStartThread->getTimeoutInMs() > 0 )
            {
                m_reqDscrTimeout = pReqStartThread->getDescriptor();
                m_pTmrReqTimeout->start( pReqStartThread->getTimeoutInMs() );
            }
        }
    } // if( m_state == EStateCreated )

    // If the database has already been started (could have already been started before
    // the startup method was called or because we have been waiting until the database
    // has been started using the requests wait condition) or if an error occurred ...
    else if( m_state == EStateRunning )
    {
        // The request has been finished.
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.

    } // if( m_state == EStateRunning )

    if( !isAsynchronousRequest(i_pReq) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);
    }

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeStartupRequest

//------------------------------------------------------------------------------
void CDbThread::executeShutdownRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "ReqShutdown: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeShutdownRequest",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    i_pReq->lock();

    m_requestQueue.lock();

    // If started as child request of request in progress (as part of a request sequence) ..
    if( i_pReqMainInProgress != nullptr )
    {
        m_requestQueue.addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_requestQueue.setRequestInProgress(i_pReq);
    }

    // If the thread is running ...
    if( m_state == EStateRunning )
    {
        // The Shutdown request is split into two child requests (DbShutdown and StopThread).
        // The child requests should also be indicated in the request tree.
        CRequest* pReqDbShutdown = m_requestQueue.createRequest(
            /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
            /* strClassName */ "CDbThread",
            /* iRequest     */ ERequestDbShutdown,
            /* strRequest   */ Request2Str(ERequestDbShutdown),
            /* iReqIdParent */ i_pReq->getId(),
            /* iTimeout_ms  */ i_pReq->getTimeoutInMs(),
            /* bIsBlocking  */ i_pReq->isBlockingRequest() );

        if( !QObject::connect(
            /* pObjSender   */ pReqDbShutdown,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        m_requestQueue.addRequestInProgress(pReqDbShutdown);

        CMsgReqShutdown* pMsgReq = new CMsgReqShutdown(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pDb,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ pReqDbShutdown->getId(),
            /* iMsgId           */ -1 );

        POST_OR_DELETE_MESSAGE(pMsgReq);

        if( pReqDbShutdown->isBlockingRequest() )
        {
            if( !pReqDbShutdown->wait() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for DbShutdown timed out." );
            }

            // The sub request has been finished.
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }

            pReqDbShutdown->setErrResultInfo(errResultInfo);
            pReqDbShutdown->setProgressInPerCent(100);
            pReqDbShutdown->lock();
            pReqDbShutdown->update(); // The "onRequestChanged" slot is called removing the request from the request queue.
            pReqDbShutdown->unlock();
            deleteResultConfirmationMessage(pReqDbShutdown, this);

            deleteRequest(pReqDbShutdown); // The "onRequestChanged" slot will be called again.
            pReqDbShutdown = nullptr;

        } // if( pReqDbShutdown->isBlockingRequest() )

        else // if( !pReqDbShutdown->isBlockingRequest() )
        {
            if( pReqDbShutdown->getTimeoutInMs() > 0 )
            {
                m_reqDscrTimeout = pReqDbShutdown->getDescriptor();
                m_pTmrReqTimeout->start( pReqDbShutdown->getTimeoutInMs() );
            }
        }
    } // if( m_state == EStateRunning )

    // If the database has already been shutdown  ...
    else if( m_state == EStateCreated )
    {
        // The request has been finished.
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.

    } // if( m_state == EStateCreated )

    if( !isAsynchronousRequest(i_pReq) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);
    }

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeShutdownRequest

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDbThread::onRequestTimeout()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRequestTimeout",
        /* strAddInfo   */ strAddTrcInfo );

    m_pTmrReqTimeout->stop();

    CRequest* pReq = m_requestQueue.getRequestInProgress(m_reqDscrTimeout.m_iId);

    if( pReq != nullptr )
    {
        SErrResultInfo errResultInfo = ErrResultInfoError("onRequestTimeout", EResultTimeout, pReq->getRequestName());
        m_requestQueue.lock();
        pReq->lock();
        pReq->setErrResultInfo(errResultInfo);
        pReq->setProgressInPerCent(100);
        pReq->update();
        pReq->unlock();
        m_requestQueue.unlock();

        // The one who calls update for the finished request must delete the request.
        deleteRequest(pReq); // The "onRequestChanged" slot will be called again. The request has already been removed from the queue.
        pReq = nullptr;
    }

} // onRequestTimeout

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDbThread::onRequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqDscr: " + i_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRequestChanged",
        /* strAddInfo   */ strAddTrcInfo );

    // A completely processed request (Progress >= 100) will be removed from the request queue.
    // If a request has been finished and has been removed from the request queue it will also be
    // estroyed sometimes later by the creator of the request. As the "onRequestChanged" slot may
    // still be connected the slot may be called again (with "dscr.m_objState == EObjState::Destroyed")
    // but is still no longer in progress ("getRequestInProgress" will fail). So we have to check
    // whether the request is still in progress before handling the request changes.

    // If the request has not yet been removed from the request queue ..
    if( m_requestQueue.isRequestInProgress(i_reqDscr.m_iId) )
    {
        CRequest* pReq = m_requestQueue.getRequestInProgress(i_reqDscr.m_iId);

        CRequest* pReqMainInProgress = m_requestQueue.getRequestInProgress();

        if( pReq == pReqMainInProgress )
        {
            pReqMainInProgress = nullptr;
        }

        m_requestQueue.lock();

        if( i_reqDscr.m_errResultInfo.isErrorResult() )
        {
            if( !i_reqDscr.m_errResultInfo.isAddedToErrLogModel() )
            {
                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(i_reqDscr.m_errResultInfo);
                }
            }
        }

        SErrResultInfo errResultInfo = i_reqDscr.m_errResultInfo;

        // If the request has been finished ...
        if( i_reqDscr.m_iProgress_perCent >= 100 )
        {
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }
            m_requestQueue.removeRequestInProgress(i_reqDscr.m_iId);

            if( m_pTmrReqTimeout->isActive() )
            {
                m_pTmrReqTimeout->stop();
            }
        }

        switch( i_reqDscr.m_request )
        {
            case Database::ERequestStartup:
            {
                // If the main request has been finished ..
                if( i_reqDscr.m_iProgress_perCent >= 100 )
                {
                    if( !errResultInfo.isErrorResult() )
                    {
                        // Database has been started.
                        if( m_state != EStateRunning )
                        {
                            EState statePrev = m_state;
                            m_state = EStateRunning;
                            emit stateChanged( m_state, statePrev );
                        }
                    } // if( !errResultInfo.isErrorResult() )
                } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                break;
            } // case Database::ERequestStartup

            case Database::ERequestShutdown:
            {
                // If the main request has been finished ..
                if( i_reqDscr.m_iProgress_perCent >= 100 )
                {
                    if( !errResultInfo.isErrorResult() )
                    {
                        // Database has been shut down.
                        if( m_state != EStateCreated )
                        {
                            EState statePrev = m_state;
                            m_state = EStateCreated;
                            emit stateChanged( m_state, statePrev );
                        }
                    } // if( !errResultInfo.isErrorResult() )
                } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                break;
            } // case Database::ERequestShutdown

            case Database::ERequestStartThread:
            {
                if( pReqMainInProgress == nullptr )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError, "StartThread must be Child of the Main Startup Request" );
                }

                // If the sub request has not yet been finished ..
                if( i_reqDscr.m_iProgress_perCent < 100 )
                {
                    // Update progress of main request.
                    int iProgress_perCent = (i_reqDscr.m_iProgress_perCent*10)/100;
                    if( iProgress_perCent >= 100 )
                    {
                        iProgress_perCent = 90;
                    }
                    pReqMainInProgress->setProgressInPerCent(iProgress_perCent);
                    pReqMainInProgress->update();
                }

                // If the sub request has been finished ..
                else // if( i_reqDscr.m_iProgress_perCent >= 100 )
                {
                    if( errResultInfo.isErrorResult() )
                    {
                        quit();

                        if( !wait(30000) )
                        {
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultTimeout);
                            errResultInfo.setAddErrInfoDscr( "Waiting for StopThread timed out." );

                            if( CErrLog::GetInstance() != nullptr )
                            {
                                CErrLog::GetInstance()->addEntry(errResultInfo);
                            }
                        }

                        // The main request is finished.
                        pReqMainInProgress->setErrResultInfo(errResultInfo);
                        pReqMainInProgress->setProgressInPerCent(100);
                        pReqMainInProgress->update();

                    } // if( errResultInfo.isErrorResult() )

                    // The startup request is partially processed.
                    else // if( !errResultInfo.isErrorResult() )
                    {
                        pReqMainInProgress->setErrResultInfo(errResultInfo);
                        pReqMainInProgress->setProgressInPerCent(10);
                        pReqMainInProgress->update();

                        // The Startup request is split into two child requests (StartThread and DbStartup).
                        // The child requests should also be indicated in the request tree.
                        CRequest* pReqDbStartup = m_requestQueue.createRequest(
                            /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
                            /* strClassName */ "CDbThread",
                            /* iRequest     */ ERequestDbStartup,
                            /* strRequest   */ Request2Str(ERequestDbStartup),
                            /* iReqIdParent */ pReqMainInProgress->getId(),
                            /* iTimeout_ms  */ pReqMainInProgress->getTimeoutInMs(),
                            /* bIsBlocking  */ pReqMainInProgress->isBlockingRequest() );

                        if( !QObject::connect(
                            /* pObjSender   */ pReqDbStartup,
                            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                            /* pObjReceiver */ this,
                            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
                        {
                            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                        }

                        m_requestQueue.addRequestInProgress(pReqDbStartup);

                        CMsgReqStartup* pMsgReq = new CMsgReqStartup(
                            /* pObjSender       */ this,
                            /* pObjReceiver     */ m_pDb,
                            /* bMustBeConfirmed */ true,
                            /* iReqId           */ pReqDbStartup->getId(),
                            /* iMsgId           */ -1 );

                        POST_OR_DELETE_MESSAGE(pMsgReq);
                        pMsgReq = nullptr;

                        if( pReqDbStartup->isBlockingRequest() )
                        {
                            SErrResultInfo errResultInfo = pReqDbStartup->getErrResultInfo();

                            if( !pReqDbStartup->wait() )
                            {
                                errResultInfo.setSeverity(EResultSeverityError);
                                errResultInfo.setResult(EResultTimeout);
                                errResultInfo.setAddErrInfoDscr( "Waiting for DbStartup timed out." );
                            }

                            // The sub request has been finished.
                            if( errResultInfo.getResult() == EResultUndefined )
                            {
                                errResultInfo.setErrResult(ErrResultSuccess);
                            }

                            pReqDbStartup->setErrResultInfo(errResultInfo);
                            pReqDbStartup->setProgressInPerCent(100);
                            pReqDbStartup->lock();
                            pReqDbStartup->update(); // The "onRequestChanged" slot is called removing the request from the request queue.
                            pReqDbStartup->unlock();
                            deleteResultConfirmationMessage(pReqDbStartup, this);

                            deleteRequest(pReqDbStartup); // The "onRequestChanged" slot will be called again.
                            pReqDbStartup = nullptr;

                        } // if( pReqDbStartup->isBlockingRequest() )

                        else // if( !pReqDbStartup->isBlockingRequest() )
                        {
                            if( pReqDbStartup->getTimeoutInMs() > 0 )
                            {
                                m_reqDscrTimeout = pReqDbStartup->getDescriptor();
                                m_pTmrReqTimeout->start( pReqDbStartup->getTimeoutInMs() );
                            }
                        }

                    } // if( !errResultInfo.isErrorResult() )
                } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                break;
            } // case Database::ERequestStartThread

            case Database::ERequestStopThread:
            {
                if( pReqMainInProgress == nullptr )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError, "StopThread must be Child of the Main Shutdown Request" );
                }

                // If the sub request has not yet been finished ..
                if( i_reqDscr.m_iProgress_perCent < 100 )
                {
                    // Update progress of main request.
                    int iProgress_perCent = 90 + (i_reqDscr.m_iProgress_perCent*10)/100;
                    if( iProgress_perCent >= 100 )
                    {
                        iProgress_perCent = 90;
                    }
                    pReqMainInProgress->setProgressInPerCent(iProgress_perCent);
                    pReqMainInProgress->update();
                }
                // If the sub request has been finished ..
                else // if( i_reqDscr.m_iProgress_perCent >= 100 )
                {
                    // Main request has been finished.
                    pReqMainInProgress->setErrResultInfo(errResultInfo);
                    pReqMainInProgress->setProgressInPerCent(100);
                    pReqMainInProgress->lock();
                    pReqMainInProgress->update(); // "onRequestChanged" is called as a reentry removing the request from the queue.
                    pReqMainInProgress->unlock();

                    // The one who calls update for the finished request must delete the request.
                    deleteRequest(pReqMainInProgress);
                    pReqMainInProgress = nullptr;

                } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                break;
            } // case Database::ERequestStopThread

            case Database::ERequestDbStartup:
            {
                // DbStartup must be the child request of Startup.
                if( pReqMainInProgress == nullptr )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError, "DbStartup must be Child of the Main Startup Request" );
                }

                // If the sub request has not yet been finished ..
                if( i_reqDscr.m_iProgress_perCent < 100 )
                {
                    // Update progress of main request.
                    int iProgress_perCent = 10 + (i_reqDscr.m_iProgress_perCent*90)/100;
                    if( iProgress_perCent >= 100 )
                    {
                        iProgress_perCent = 90;
                    }
                    pReqMainInProgress->setProgressInPerCent(iProgress_perCent);
                    pReqMainInProgress->update();
                }
                // If the sub request has been finished ..
                else // if( i_reqDscr.m_iProgress_perCent >= 100 )
                {
                    // On error ..
                    if( errResultInfo.isErrorResult() )
                    {
                        CRequest* pReqStopThread = m_requestQueue.createRequest(
                            /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
                            /* strClassName */ "CDbClientThread",
                            /* iRequest     */ ERequestStopThread,
                            /* strRequest   */ Request2Str(ERequestStopThread),
                            /* iReqIdParent */ pReqMainInProgress->getId(),
                            /* iTimeout_ms  */ pReqMainInProgress->getTimeoutInMs(),
                            /* bIsBlocking  */ pReqMainInProgress->isBlockingRequest() );

                        // Don't connect the "onRequestChanged" slot to the stop thread request.
                        // Otherwise on updating the stop thread request this slot would be called
                        // as a reentry with sub request in progress = StopThread and main request in
                        // progress = Startup. That would successfully finish the startup main request.

                        errResultInfo = pReqStopThread->getErrResultInfo();

                        m_requestQueue.addRequestInProgress(pReqStopThread);

                        quit();

                        int iThreadWaitTimeout_ms = pReqStopThread->getTimeoutInMs();
                        unsigned long uThreadWaitTimeout_ms = static_cast<unsigned long>(iThreadWaitTimeout_ms);

                        if( iThreadWaitTimeout_ms <= 0 )
                        {
                            uThreadWaitTimeout_ms = ULONG_MAX;
                        }
                        if( !wait(uThreadWaitTimeout_ms) )
                        {
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultTimeout);
                            errResultInfo.setAddErrInfoDscr( "Waiting for StopThread timed out." );
                        }

                        // The sub request has been finished.
                        if( errResultInfo.getResult() == EResultUndefined )
                        {
                            errResultInfo.setErrResult(ErrResultSuccess);
                        }

                        // It is necessary to remove the request from the request queue again before
                        // updating (finishing) the main startup request as on removing the main request
                        // from the request queue sub requests would be accessed (e.g. to report error
                        // that queue is not empty).
                        m_requestQueue.removeRequestInProgress(pReqStopThread);

                        pReqStopThread->setErrResultInfo(errResultInfo);
                        pReqStopThread->setProgressInPerCent(100);
                        pReqStopThread->lock();
                        pReqStopThread->update(); // We did not connect the "onRequestChanged" slot (see comment above).
                        pReqStopThread->unlock();

                        // The one who calls update for the finished request must delete the request.
                        deleteRequest(pReqStopThread);
                        pReqStopThread = nullptr;

                        // Main request has been unsuccessfully finished.
                        pReqMainInProgress->setErrResultInfo(i_reqDscr.m_errResultInfo);
                        pReqMainInProgress->setProgressInPerCent(100);
                        pReqMainInProgress->lock();
                        pReqMainInProgress->update(); // "onRequestChanged" is called as a reentry removing the request from the queue.
                        pReqMainInProgress->unlock();

                        // The one who calls update for the finished request must delete the request.
                        deleteRequest(pReqMainInProgress);
                        pReqMainInProgress = nullptr;

                    } // if( errResultInfo.isErrorResult() )

                    // Main request has been finished.
                    else // if( !errResultInfo.isErrorResult() )
                    {
                        pReqMainInProgress->setErrResultInfo(errResultInfo);
                        pReqMainInProgress->setProgressInPerCent(100);
                        pReqMainInProgress->lock();
                        pReqMainInProgress->update(); // "onRequestChanged" is called as a reentry removing the request from the queue.
                        pReqMainInProgress->unlock();

                        // The one who calls update for the finished request must delete the request.
                        deleteRequest(pReqMainInProgress);
                        pReqMainInProgress = nullptr;
                    }
                } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                break;
            } // case Database::ERequestDbStartup

            case Database::ERequestDbShutdown:
            {
                // DbShutdown must be the child request of Shutdown.
                if( pReqMainInProgress == nullptr )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError, "DbShutdown must be Child of the Main Shutdown Request" );
                }

                // If the sub request has not yet been finished ..
                if( i_reqDscr.m_iProgress_perCent < 100 )
                {
                    // Update progress of main request.
                    int iProgress_perCent = (i_reqDscr.m_iProgress_perCent*90)/100;
                    if( iProgress_perCent >= 100 )
                    {
                        iProgress_perCent = 90;
                    }
                    pReqMainInProgress->setProgressInPerCent(iProgress_perCent);
                    pReqMainInProgress->update();
                }

                // If the sub request has been finished ..
                else // if( i_reqDscr.m_iProgress_perCent >= 100 )
                {
                    // Also if an error Occurred (e.g. timeout) the thread will be finished
                    // and the shutdown request is partially processed.
                    pReqMainInProgress->setProgressInPerCent(90);
                    pReqMainInProgress->update();

                    // The Shutdown request is split into two child requests (DbShutdown and StopThread).
                    // The child requests should also be indicated in the request tree.
                    CRequest* pReqStopThread = m_requestQueue.createRequest(
                        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
                        /* strClassName */ "CDbThread",
                        /* iRequest     */ ERequestStopThread,
                        /* strRequest   */ Request2Str(ERequestStopThread),
                        /* iReqIdParent */ pReqMainInProgress->getId(),
                        /* iTimeout_ms  */ pReqMainInProgress->getTimeoutInMs(),
                        /* bIsBlocking  */ pReqMainInProgress->isBlockingRequest() );

                    if( !QObject::connect(
                        /* pObjSender   */ pReqStopThread,
                        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }

                    errResultInfo = pReqStopThread->getErrResultInfo();

                    m_requestQueue.addRequestInProgress(pReqStopThread);

                    quit();

                    int iThreadWaitTimeout_ms = pReqStopThread->getTimeoutInMs();
                    unsigned long uThreadWaitTimeout_ms = static_cast<unsigned long>(iThreadWaitTimeout_ms);

                    if( iThreadWaitTimeout_ms <= 0 )
                    {
                        uThreadWaitTimeout_ms = ULONG_MAX;
                    }
                    if( !wait(uThreadWaitTimeout_ms) )
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultTimeout);
                        errResultInfo.setAddErrInfoDscr( "Waiting for StopThread timed out." );
                    }

                    // The sub request has been finished.
                    if( errResultInfo.getResult() == EResultUndefined )
                    {
                        errResultInfo.setErrResult(ErrResultSuccess);
                    }

                    pReqStopThread->setErrResultInfo(errResultInfo);
                    pReqStopThread->setProgressInPerCent(100);
                    pReqStopThread->lock();
                    pReqStopThread->update(); // "onRequestChanged" is called as a reentry removing the request from the queue.
                    pReqStopThread->unlock();

                    // The one who calls update for the finished request must delete the request.
                    deleteRequest(pReqStopThread);
                    pReqStopThread = nullptr;

                } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                break;
            } // case Database::ERequestDbShutdown

            default:
            {
                break;
            }
        } // switch( i_reqDscr.m_request )

        m_requestQueue.unlock();

        if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
        {
            executeNextPostponedRequest();
        }
    } // if( m_requestQueue.isRequestInProgress(i_reqDscr.m_iId) )

} // onRequestChanged

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CDbThread::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    CMsg* pMsg = dynamic_cast<CMsg*>(i_pEv);

    if( pMsg != nullptr )
    {
        CMsgReq* pMsgReq = nullptr;
        CMsgCon* pMsgCon = nullptr;

        if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq )
        {
            pMsgReq = dynamic_cast<CMsgReq*>(pMsg);
        }
        else if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeCon )
        {
            pMsgCon = dynamic_cast<CMsgCon*>(pMsg);
        }

        QString strAddTrcInfo;

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
        {
            int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
            strAddTrcInfo  = "Msg: " + pMsg->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", State: " + State2Str(m_state);
            strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDetailLevel */ ETraceDetailLevelMethodCalls,
            /* strMethod    */ "event",
            /* strAddInfo   */ strAddTrcInfo );

        if( pMsgReq != nullptr )
        {
            switch( static_cast<int>(pMsg->type()) )
            {
                case EBaseMsgTypeReqContinue:
                {
                    CMsgReqContinue* pMsgReq = dynamic_cast<CMsgReqContinue*>(pMsg);

                    if( pMsgReq == nullptr )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch, "MsgReqContinue == nullptr! " );
                    }

                    m_bMsgReqContinuePending = false;

                    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
                    {
                        CRequest* pReqPostponed = m_requestQueue.takeFirstPostponedRequest();

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
                                default:
                                {
                                    break;
                                }
                            } // switch( pReqPostponed->getRequest() )
                        } // if( pReqPostponed != nullptr )

                        deleteRequest(pReqPostponed);
                        pReqPostponed = nullptr;

                    } // if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )

                    bEventHandled = true;
                    break;
                } // case EBaseMsgTypeReqContinue

                default:
                {
                    break;
                }
            } // switch( pMsg->type() )
        } // if( pMsgReq != nullptr )

        else if( pMsgCon != nullptr )
        {
            CRequest* pReq = m_requestQueue.getRequestInProgress(pMsgCon->getRequestId());

            if( pReq != nullptr )
            {
                SErrResultInfo errResultInfo = pMsgCon->getErrResultInfo();
                int            iProgress_perCent = pMsgCon->getProgress();

                m_requestQueue.lock();

                pReq->setErrResultInfo(errResultInfo);
                pReq->setProgressInPerCent(iProgress_perCent);
                pReq->lock();
                pReq->update(); // "onRequestChanged" is called as a reentry removing the request from the queue.
                pReq->unlock();

                if( iProgress_perCent >= 100 )
                {
                    // The one who calls update for the finished request must delete the request.
                    deleteRequest(pReq); // The "onRequestChanged" slot will be called again. The request has already been removed from the queue.
                    pReq = nullptr;
                }
                m_requestQueue.unlock();

            } // if( pReq != nullptr )

            bEventHandled = true;

        } // if( pMsgCon != nullptr )

        if( !bEventHandled )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMessage, pMsg->msgTypeToStr() );
        }

    } // if( pMsg != nullptr )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pEv);
    }
    return bEventHandled;

} // event

/*==============================================================================
protected: // overridables of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
void CDbThread::start()
//------------------------------------------------------------------------------
{
    QThread::start();
}

//------------------------------------------------------------------------------
void CDbThread::run()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "run",
        /* strAddInfo   */ "" );

    m_pDb = new CDb( objectName() );

    emit dbCreated(m_pDb);

    m_pDb->setReqModeSingleShot(m_bReqModeSingleShot);

    // Please note that requests should only be changed from within the
    // thread in which they have been created. Otherwise the "changed"
    // signal is queued and may be received while the request has already
    // been deleted by the creator.
    //pReqStartThread->setResult(EResultSuccess);
    //pReqStartThread->setProgressInPerCent(100);
    //pReqStartThread->update();

    if( CRequestExecTree::GetInstance()->isWaiting(m_iReqIdStartThread) )
    {
        CRequestExecTree::GetInstance()->wake(m_iReqIdStartThread);
    }
    else
    {
        int iTimeout_ms = CRequestExecTree::GetInstance()->getTimeoutInMs(m_iReqIdStartThread);

        // To allow testing timeouts and progress bars for this request.
        if( iTimeout_ms > 0 )
        {
            int iProgress_perCent = 50;

            // Starting thread times out if timeout is set to Short.
            while( iProgress_perCent < 100 )
            {
                CSleeperThread::msleep(Database::c_iTimeoutShort_ms);

                // Send startup confirmation message to the thread itself (to change thread context).
                CMsgConStartup* pMsgCon = new CMsgConStartup(
                    /* pObjSender        */ m_pDb,
                    /* pObjReceiver      */ this,
                    /* iReqId            */ m_iReqIdStartThread,
                    /* iMsgIdReq         */ m_iReqIdStartThread,
                    /* errResultInfo     */ SErrResultInfo(),
                    /* iProgress_perCent */ iProgress_perCent );

                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;

                iProgress_perCent += 50;
            }
        } // if( iTimeout_ms > 0 )

        // Send startup confirmation message to the thread itself (to change thread context).
        CMsgConStartup* pMsgCon = new CMsgConStartup(
            /* pObjSender        */ m_pDb,
            /* pObjReceiver      */ this,
            /* iReqId            */ m_iReqIdStartThread,
            /* iMsgIdReq         */ m_iReqIdStartThread,
            /* errResultInfo     */ SErrResultInfo(),
            /* iProgress_perCent */ 100 );

        POST_OR_DELETE_MESSAGE(pMsgCon);
        pMsgCon = nullptr;
    }

    exec();

    try
    {
        delete m_pDb;
    }
    catch(...)
    {
    }
    m_pDb = nullptr;

} // run
