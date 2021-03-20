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

#include "DbClientThread.h"
#include "DbClient.h"
#include "Msg.h"

#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysSleeperThread.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::AsyncRequests;
using namespace ZS::Apps::Test::AsyncRequests::Database;


/*******************************************************************************
class CDbClientThread : public QThread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDbClientThread::CDbClientThread( const QString& i_strObjName ) :
//------------------------------------------------------------------------------
    QThread(),
    m_socketTypeCurr(Ipc::ESocketTypeInProcMsg),
    m_socketTypeNew(Ipc::ESocketTypeUndefined),
    m_pDb(nullptr),
    m_pDbClt(nullptr),
    m_state(EStateCreated),
    m_requestQueue(this,true),
    m_reqDscrTimeout(),
    m_pTmrReqTimeout(nullptr),
    m_bMsgReqContinuePending(false),
    m_iReqIdStartThread(-1),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::AsyncRequests", "CDbClientThread", objectName());

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
CDbClientThread::~CDbClientThread()
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
        delete m_pDbClt;
    }
    catch(...)
    {
    }
    m_pDbClt = nullptr;

    m_pDb = nullptr;
    m_pTmrReqTimeout = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientThread::setSocketType( ZS::Ipc::ESocketType i_socketType )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "SocketType: " + Ipc::socketType2Str(i_socketType);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setSocketType",
        /* strAddInfo   */ strAddTrcInfo );

    m_socketTypeNew = i_socketType;

    if( m_state < EStateConnected )
    {
        if( m_requestQueue.isIdle() )
        {
            m_socketTypeCurr = m_socketTypeNew;
            m_socketTypeNew = Ipc::ESocketTypeUndefined;
        }
    }

} // setSocketType

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientThread::setDb( CDb* i_pDb )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        if( i_pDb == nullptr )
        {
            strAddTrcInfo  = "Db: nullptr";
        }
        else
        {
            strAddTrcInfo  = "Db: " + i_pDb->objectName();
        }
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDb",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pDb == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pDb == nullptr" );
    }
    if( m_pDb != nullptr && m_pDb != i_pDb )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Changing database reference is not allowed" );
    }

    if( m_pDb == nullptr )
    {
        m_pDb = i_pDb;

        // Using "BlockingQueuedConnection" for the destroyed signal leads to a deadlock even
        // if the object is destroyed in the run method of a different thread.

        //if( !QObject::connect(
        //    /* pObjSender   */ m_pDb,
        //    /* szSignal     */ SIGNAL(destroyed(QObject*)),
        //    /* pObjReceiver */ this,
        //    /* szSlot       */ SLOT(onDbDestroyed(QObject*)),
        //    /* cnctType     */ m_pDb->thread() == thread() ? Qt::DirectConnection : Qt::BlockingQueuedConnection ) )
        //{
        //    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        //}

        if( !QObject::connect(
            /* pObjSender   */ m_pDb,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onDbDestroyed(QObject*)),
            /* cnctType     */ Qt::DirectConnection ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        if( m_pDbClt != nullptr )
        {
            m_pDbClt->setDb(m_pDb);
        }

    } // if( m_pDb == nullptr )

} //  setDb

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClientThread::startup( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strClassName */ "CDbClientThread",
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
CRequest* CDbClientThread::shutdown( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strClassName */ "CDbClientThread",
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
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClientThread::changeSettings( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strMethod    */ "changeSettings",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("changeSettings");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbClientThread",
        /* iRequest     */ ERequestChangeSettings,
        /* strRequest   */ Request2Str(ERequestChangeSettings),
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
        executeChangeSettingsRequest(pReq,pReqInProgress);
    }

    return pReq;

} // changeSettings

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClientThread::connect_( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strMethod    */ "connect_",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("connect_");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbClientThread",
        /* iRequest     */ ERequestConnect,
        /* strRequest   */ Request2Str(ERequestConnect),
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
        executeConnectRequest(pReq,pReqInProgress);
    }

    return pReq;

} // connect_

//------------------------------------------------------------------------------
CRequest* CDbClientThread::disconnect_( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strMethod    */ "disconnect_",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("disconnect_");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbClientThread",
        /* iRequest     */ ERequestDisconnect,
        /* strRequest   */ Request2Str(ERequestDisconnect),
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
        executeDisconnectRequest(pReq,pReqInProgress);
    }

    return pReq;

} // disconnect_

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClientThread::register_( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strMethod    */ "register_",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("register");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbClientThread",
        /* iRequest     */ ERequestRegister,
        /* strRequest   */ Request2Str(ERequestRegister),
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
        executeRegisterRequest(pReq,pReqInProgress);
    }

    return pReq;

} // register_

//------------------------------------------------------------------------------
CRequest* CDbClientThread::unregister( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strMethod    */ "unregister",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("unregister");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbClientThread",
        /* iRequest     */ ERequestUnregister,
        /* strRequest   */ Request2Str(ERequestUnregister),
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
        executeUnregisterRequest(pReq,pReqInProgress);
    }

    return pReq;

} // unregister

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClientThread::selectSchema( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strMethod    */ "selectSchema",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("selectSchema");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbClientThread",
        /* iRequest     */ ERequestSelectSchema,
        /* strRequest   */ Request2Str(ERequestSelectSchema),
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
        executeSelectSchemaRequest(pReq,pReqInProgress);
    }

    return pReq;

} // selectSchema

//------------------------------------------------------------------------------
CRequest* CDbClientThread::updateSchema( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strMethod    */ "updateSchema",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("updateSchema");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbClientThread",
        /* iRequest     */ ERequestUpdateSchema,
        /* strRequest   */ Request2Str(ERequestUpdateSchema),
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
        executeUpdateSchemaRequest(pReq,pReqInProgress);
    }

    return pReq;

} // updateSchema

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClientThread::selectData( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strMethod    */ "selectData",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("selectData");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbClientThread",
        /* iRequest     */ ERequestSelectData,
        /* strRequest   */ Request2Str(ERequestSelectData),
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
        executeSelectDataRequest(pReq,pReqInProgress);
    }

    return pReq;

} // selectData

//------------------------------------------------------------------------------
CRequest* CDbClientThread::updateData( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strMethod    */ "updateData",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("updateData");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbClientThread",
        /* iRequest     */ ERequestUpdateData,
        /* strRequest   */ Request2Str(ERequestUpdateData),
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
        executeUpdateDataRequest(pReq,pReqInProgress);
    }

    return pReq;

} // updateData

/*==============================================================================
protected: // overridables (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientThread::executeNextPostponedRequest()
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
void CDbClientThread::executeStartupRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
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
        m_iReqIdStartThread = i_pReq->getId();

        start();

        if( i_pReq->isBlockingRequest() )
        {
            if( !i_pReq->wait() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for StartThread timed out." );
            }

            // The request has been finished.
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }

            i_pReq->setErrResultInfo(errResultInfo);
            i_pReq->setProgressInPerCent(100);
            i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.
            deleteResultConfirmationMessage(i_pReq, this);

        } // if( i_pReq->isBlockingRequest() )

        else // if( !i_pReq->isBlockingRequest() )
        {
            if( i_pReq->getTimeoutInMs() > 0 )
            {
                m_reqDscrTimeout = i_pReq->getDescriptor();
                m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
            }
        }
    } // if( m_state == EStateCreated )

    // If the thread has already been started ...
    else // if( m_state != EStateCreated )
    {
        // The request has been finished.
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.

    } // if( m_state > EStateCreated || errResultInfo.isErrorResult() )

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
void CDbClientThread::executeShutdownRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
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

    // If the thread is running and if the client is connected ...
    if( m_state >= EStateConnected )
    {
        // The Shutdown request will be split into the two subrequests "DbDisconnect" and "StopThread".
        CRequest* pReqDbDisconnect = m_requestQueue.createRequest(
            /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
            /* strClassName */ "CDbClientThread",
            /* iRequest     */ ERequestDbDisconnect,
            /* strRequest   */ Request2Str(ERequestDbDisconnect),
            /* iReqIdParent */ i_pReq->getId(),
            /* iTimeout_ms  */ i_pReq->getTimeoutInMs(),
            /* bIsBlocking  */ i_pReq->isBlockingRequest() );

        if( !QObject::connect(
            /* pObjSender   */ pReqDbDisconnect,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        m_requestQueue.addRequestInProgress(pReqDbDisconnect);

        CMsgReqDisconnect* pMsgReq = new CMsgReqDisconnect(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pDbClt,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ pReqDbDisconnect->getId(),
            /* iMsgId           */ -1 );

        POST_OR_DELETE_MESSAGE(pMsgReq);

        if( pReqDbDisconnect->isBlockingRequest() )
        {
            if( !pReqDbDisconnect->wait() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for DbDisconnect timed out." );
            }

            // The sub request has been finished.
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }

            pReqDbDisconnect->setErrResultInfo(errResultInfo);
            pReqDbDisconnect->setProgressInPerCent(100);
            pReqDbDisconnect->lock();
            pReqDbDisconnect->update(); // The "onRequestChanged" slot is called removing the request from the request queue.
            pReqDbDisconnect->unlock();
            deleteResultConfirmationMessage(pReqDbDisconnect, this);

            deleteRequest(pReqDbDisconnect); // The "onRequestChanged" slot will be called again.
            pReqDbDisconnect = nullptr;

        } // if( i_pReq->isBlockingRequest() )

        else // if( !i_pReq->isBlockingRequest() )
        {
            if( pReqDbDisconnect->getTimeoutInMs() > 0 )
            {
                m_reqDscrTimeout = pReqDbDisconnect->getDescriptor();
                m_pTmrReqTimeout->start( pReqDbDisconnect->getTimeoutInMs() );
            }
        }
    } // if( m_state >= EStateConnected )

    // If the thread is running and if the client is not connected ...
    else if( m_state == EStateDisconnected )
    {
        quit();

        int iThreadWaitTimeout_ms = i_pReq->getTimeoutInMs();
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

        // The request has been finished.
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.

    } // if( m_state == EStateDisconnected )

    // If the thread has already been stopped ..
    else // if( m_state < EStateDisconnected )
    {
        // The request has been finished.
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.

    } // if( m_state < EStateDisconnected )

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
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientThread::executeChangeSettingsRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "ReqChangeSettings: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

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
        /* strMethod    */ "executeChangeSettingsRequest",
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

    if( m_state < EStateConnected )
    {
        if( m_socketTypeNew != Ipc::ESocketTypeUndefined )
        {
            m_socketTypeCurr = m_socketTypeNew;
            m_socketTypeNew = Ipc::ESocketTypeUndefined;
        }

        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.

    } // if( m_state < EStateConnected )

    // If the client thread is running and if the client is connected ...
    else // if( m_state >= EStateConnected )
    {
        CMsgReqChangeSettings* pMsgReq = new CMsgReqChangeSettings(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pDbClt,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ i_pReq->getId(),
            /* iMsgId           */ -1 );
        Ipc::SClientHostSettings hostSettings;
        if( m_socketTypeNew != Ipc::ESocketTypeUndefined )
        {
            hostSettings.m_socketType = m_socketTypeNew;
        }
        pMsgReq->setClientHostSettings(hostSettings);
        POST_OR_DELETE_MESSAGE(pMsgReq);

        if( i_pReq->isBlockingRequest() )
        {
            if( !i_pReq->wait() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for ChangeSettings timed out." );
            }

            // The request has been finished.
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }

            i_pReq->setErrResultInfo(errResultInfo);
            i_pReq->setProgressInPerCent(100);
            i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.
            deleteResultConfirmationMessage(i_pReq, this);

        } // if( i_pReq->isBlockingRequest() )

        else // if( !i_pReq->isBlockingRequest() )
        {
            if( i_pReq->getTimeoutInMs() > 0 )
            {
                m_reqDscrTimeout = i_pReq->getDescriptor();
                m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
            }
        }
    } // if( m_state >= EStateConnected )

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

} // executeChangeSettingsRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientThread::executeConnectRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "ReqConnect: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

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
        /* strMethod    */ "executeConnectRequest",
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

    if( m_state < EStateDisconnected )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Client thread is not started." );

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.

    } // if( m_state < EStateDisconnected )

    // If the client thread is running and if the client is not yet connected ...
    else // if( m_state >= EStateDisconnected )
    {
        CMsgReqConnect* pMsgReq = new CMsgReqConnect(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pDbClt,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ i_pReq->getId(),
            /* iMsgId           */ -1 );
        pMsgReq->setDb(m_pDb);

        POST_OR_DELETE_MESSAGE(pMsgReq);

        if( i_pReq->isBlockingRequest() )
        {
            if( !i_pReq->wait() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for DbConnect timed out." );
            }

            // The request has been finished.
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }

            i_pReq->setErrResultInfo(errResultInfo);
            i_pReq->setProgressInPerCent(100);
            i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.
            deleteResultConfirmationMessage(i_pReq, this);

        } // if( i_pReq->isBlockingRequest() )

        else // if( !i_pReq->isBlockingRequest() )
        {
            if( i_pReq->getTimeoutInMs() > 0 )
            {
                m_reqDscrTimeout = i_pReq->getDescriptor();
                m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
            }
        }
    } // if( m_state == EStateDisconnected )

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

} // executeConnectRequest

//------------------------------------------------------------------------------
void CDbClientThread::executeDisconnectRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "ReqDisconnect: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

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
        /* strMethod    */ "executeDisconnectRequest",
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

    // If the client thread is running and if the client has been connected ...
    if( m_state >= EStateConnected )
    {
        CMsgReqDisconnect* pMsgReq = new CMsgReqDisconnect(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pDbClt,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ i_pReq->getId(),
            /* iMsgId           */ -1 );

        POST_OR_DELETE_MESSAGE(pMsgReq);

        if( i_pReq->isBlockingRequest() )
        {
            if( !i_pReq->wait() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for DbDisconnect timed out." );
            }

            // The request has been finished.
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }

            i_pReq->setErrResultInfo(errResultInfo);
            i_pReq->setProgressInPerCent(100);
            i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.
            deleteResultConfirmationMessage(i_pReq, this);

        } // if( i_pReq->isBlockingRequest() )

        else // if( !i_pReq->isBlockingRequest() )
        {
            if( i_pReq->getTimeoutInMs() > 0 )
            {
                m_reqDscrTimeout = i_pReq->getDescriptor();
                m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
            }
        }
    } // if( m_state >= EStateConnected )

    // If the client has already been disconnected (could have already been disconnected before
    // the disconnect method was called or because we have been waiting until the database client
    // has been disconnected using the requests wait condition) or if an error occurred ...
    else // if( m_state < EStateConnected )
    {
        // The request has been finished.
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.

    } // if( m_state < EStateConnected )

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

} // executeDisconnectRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientThread::executeRegisterRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "ReqRegister: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

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
        /* strMethod    */ "executeRegisterRequest",
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

    if( m_state < EStateConnected )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Client is not connected." );

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.

    } // if( m_state < EStateConnected )

    // If the client thread is running and if the client is connected ...
    else // if( m_state >= EStateConnected )
    {
        CMsgReqRegister* pMsgReq = new CMsgReqRegister(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pDbClt,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ i_pReq->getId(),
            /* iMsgId           */ -1 );

        POST_OR_DELETE_MESSAGE(pMsgReq);

        if( i_pReq->isBlockingRequest() )
        {
            if( !i_pReq->wait() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for DbRegister timed out." );
            }

            // The request has been finished.
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }

            i_pReq->setErrResultInfo(errResultInfo);
            i_pReq->setProgressInPerCent(100);
            i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.
            deleteResultConfirmationMessage(i_pReq, this);

        } // if( i_pReq->isBlockingRequest() )

        else // if( !i_pReq->isBlockingRequest() )
        {
            if( i_pReq->getTimeoutInMs() > 0 )
            {
                m_reqDscrTimeout = i_pReq->getDescriptor();
                m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
            }
        }
    } // if( m_state >= EStateConnected )

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

} // executeRegisterRequest

//------------------------------------------------------------------------------
void CDbClientThread::executeUnregisterRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "ReqUnregister: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

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
        /* strMethod    */ "executeUnregisterRequest",
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

    if( m_state < EStateRegistered )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Client is not registered." );

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.

    } // if( m_state < EStateRegistered )

    // If the client thread is running and if the client is connected ...
    else // if( m_state >= EStateRegistered )
    {
        CMsgReqUnregister* pMsgReq = new CMsgReqUnregister(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pDbClt,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ i_pReq->getId(),
            /* iMsgId           */ -1 );

        POST_OR_DELETE_MESSAGE(pMsgReq);

        if( i_pReq->isBlockingRequest() )
        {
            if( !i_pReq->wait() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for DbUnregister timed out." );
            }

            // The request has been finished.
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }

            i_pReq->setErrResultInfo(errResultInfo);
            i_pReq->setProgressInPerCent(100);
            i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.
            deleteResultConfirmationMessage(i_pReq, this);

        } // if( i_pReq->isBlockingRequest() )

        else // if( !i_pReq->isBlockingRequest() )
        {
            if( i_pReq->getTimeoutInMs() > 0 )
            {
                m_reqDscrTimeout = i_pReq->getDescriptor();
                m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
            }
        }
    } // if( m_state >= EStateRegistered )

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

} // executeUnregisterRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientThread::executeSelectSchemaRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "ReqSelectSchema: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

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
        /* strMethod    */ "executeSelectSchemaRequest",
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

    if( m_state < EStateConnected )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Client is not connected." );

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.

    } // if( m_state < EStateConnected )

    // If the client thread is running and if the client is connected ...
    else // if( m_state >= EStateConnected )
    {
        CMsgReqSelectSchema* pMsgReq = new CMsgReqSelectSchema(
            /* pObjSender   */ this,
            /* pObjReceiver */ m_pDbClt,
            /* iReqId       */ i_pReq->getId(),
            /* iMsgId       */ -1 );

        POST_OR_DELETE_MESSAGE(pMsgReq);

        if( i_pReq->isBlockingRequest() )
        {
            if( !i_pReq->wait() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for DbSelectSchema timed out." );
            }

            // The request has been finished.
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }

            i_pReq->setErrResultInfo(errResultInfo);
            i_pReq->setProgressInPerCent(100);
            i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.
            deleteResultConfirmationMessage(i_pReq, this);

        } // if( i_pReq->isBlockingRequest() )

        else // if( !i_pReq->isBlockingRequest() )
        {
            if( i_pReq->getTimeoutInMs() > 0 )
            {
                m_reqDscrTimeout = i_pReq->getDescriptor();
                m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
            }
        }
    } // if( m_state >= EStateConnected )

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

} // executeSelectSchemaRequest

//------------------------------------------------------------------------------
void CDbClientThread::executeUpdateSchemaRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "ReqUpdateSchema: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

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
        /* strMethod    */ "executeUpdateSchemaRequest",
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

    if( m_state < EStateConnected )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Client is not connected." );

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.

    } // if( m_state < EStateConnected )

    // If the client thread is running and if the client is connected ...
    else // if( m_state >= EStateConnected )
    {
        CMsgReqUpdateSchema* pMsgReq = new CMsgReqUpdateSchema(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pDbClt,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ i_pReq->getId(),
            /* iMsgId           */ -1 );

        POST_OR_DELETE_MESSAGE(pMsgReq);

        if( i_pReq->isBlockingRequest() )
        {
            if( !i_pReq->wait() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for DbUpdateSchema timed out." );
            }

            // The request has been finished.
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }

            i_pReq->setErrResultInfo(errResultInfo);
            i_pReq->setProgressInPerCent(100);
            i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.
            deleteResultConfirmationMessage(i_pReq, this);

        } // if( i_pReq->isBlockingRequest() )

        else // if( !i_pReq->isBlockingRequest() )
        {
            if( i_pReq->getTimeoutInMs() > 0 )
            {
                m_reqDscrTimeout = i_pReq->getDescriptor();
                m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
            }
        }
    } // if( m_state >= EStateConnected )

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

} // executeUpdateSchemaRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientThread::executeSelectDataRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "ReqSelectData: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

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
        /* strMethod    */ "executeSelectDataRequest",
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

    if( m_state < EStateConnected )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Client is not connected." );

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.

    } // if( m_state < EStateConnected )

    // If the client thread is running and if the client is connected ...
    else // if( m_state >= EStateConnected )
    {
        CMsgReqSelectData* pMsgReq = new CMsgReqSelectData(
            /* pObjSender   */ this,
            /* pObjReceiver */ m_pDbClt,
            /* iReqId       */ i_pReq->getId(),
            /* iMsgId       */ -1 );

        POST_OR_DELETE_MESSAGE(pMsgReq);

        if( i_pReq->isBlockingRequest() )
        {
            if( !i_pReq->wait() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for DbSelectData timed out." );
            }

            // The request has been finished.
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }

            i_pReq->setErrResultInfo(errResultInfo);
            i_pReq->setProgressInPerCent(100);
            i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.
            deleteResultConfirmationMessage(i_pReq, this);

        } // if( i_pReq->isBlockingRequest() )

        else // if( !i_pReq->isBlockingRequest() )
        {
            if( i_pReq->getTimeoutInMs() > 0 )
            {
                m_reqDscrTimeout = i_pReq->getDescriptor();
                m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
            }
        }
    } // if( m_state >= EStateConnected )

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

} // executeSelectDataRequest

//------------------------------------------------------------------------------
void CDbClientThread::executeUpdateDataRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "ReqUpdateData: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

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
        /* strMethod    */ "executeUpdateDataRequest",
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

    if( m_state < EStateConnected )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Client is not connected." );

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.

    } // if( m_state < EStateConnected )

    // If the client thread is running and if the client is connected ...
    else // if( m_state >= EStateConnected )
    {
        CMsgReqUpdateData* pMsgReq = new CMsgReqUpdateData(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pDbClt,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ i_pReq->getId(),
            /* iMsgId           */ -1 );

        POST_OR_DELETE_MESSAGE(pMsgReq);

        if( i_pReq->isBlockingRequest() )
        {
            if( !i_pReq->wait() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for DbUpdateData timed out." );
            }

            // The request has been finished.
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }

            i_pReq->setErrResultInfo(errResultInfo);
            i_pReq->setProgressInPerCent(100);
            i_pReq->update(); // The "onRequestChanged" slot is called removing the request from the request queue.
            deleteResultConfirmationMessage(i_pReq, this);

        } // if( i_pReq->isBlockingRequest() )

        else // if( !i_pReq->isBlockingRequest() )
        {
            if( i_pReq->getTimeoutInMs() > 0 )
            {
                m_reqDscrTimeout = i_pReq->getDescriptor();
                m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
            }
        }
    } // if( m_state >= EStateConnected )

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

} // executeUpdateDataRequest

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientThread::onDbDestroyed( QObject* /*i_pDb*/ )
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
        /* strMethod    */ "onDbDestroyed",
        /* strAddInfo   */ strAddTrcInfo );

    m_pDb = nullptr;

} // onDbDestroyed

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientThread::onDbCltStateChanged( int i_iStateCurr, int i_iStatePrev )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "StateCurr: " + State2Str(i_iStateCurr);
        strAddTrcInfo += ", StatePrev: " + State2Str(i_iStatePrev);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbCltStateChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_iStateCurr != m_state )
    {
        EState statePrev = m_state;
        m_state = static_cast<EState>(i_iStateCurr);
        emit stateChanged( m_state, statePrev );
    }

} // onDbCltStateChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientThread::onRequestTimeout()
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
void CDbClientThread::onRequestChanged( ZS::System::SRequestDscr i_reqDscr )
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
                if( i_reqDscr.m_iProgress_perCent >= 100 )
                {
                    // If starting the thread failed ..
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
                                CErrLog::GetInstance()->addEntry(i_reqDscr.m_errResultInfo);
                            }
                        }
                    }
                    // If starting the thread was successful ..
                    else
                    {
                        if( m_state == EStateCreated )
                        {
                            EState statePrev = m_state;
                            m_state = EStateDisconnected;
                            emit stateChanged( m_state, statePrev );

                            if( !QObject::connect(
                                /* pObjSender   */ m_pDbClt,
                                /* szSignal     */ SIGNAL(stateChanged(int,int)),
                                /* pObjReceiver */ this,
                                /* szSlot       */ SLOT(onDbCltStateChanged(int,int)) ) )
                            {
                                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                            }
                        }
                    }
                } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                break;
            } // case Database::ERequestStartup

            case Database::ERequestShutdown:
            {
                if( i_reqDscr.m_iProgress_perCent >= 100 )
                {
                    if( !errResultInfo.isErrorResult() )
                    {
                        if( m_state != EStateCreated )
                        {
                            EState statePrev = m_state;
                            m_state = EStateCreated;
                            emit stateChanged( m_state, statePrev );
                        }
                    }
                } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                break;
            } // case Database::ERequestShutdown

            case Database::ERequestConnect:
            {
                if( i_reqDscr.m_iProgress_perCent >= 100 )
                {
                    if( !errResultInfo.isErrorResult() )
                    {
                        if( m_state < EStateConnected )
                        {
                            EState statePrev = m_state;
                            m_state = EStateConnected;
                            emit stateChanged( m_state, statePrev );
                        }
                    }
                }
                break;
            } // case Database::ERequestConnect

            case Database::ERequestDisconnect:
            {
                if( i_reqDscr.m_iProgress_perCent >= 100 )
                {
                    if( !errResultInfo.isErrorResult() )
                    {
                        if( m_state >= EStateConnected )
                        {
                            EState statePrev = m_state;
                            m_state = EStateDisconnected;
                            emit stateChanged( m_state, statePrev );
                        }
                    }
                } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                break;
            } // case Database::ERequestDisconnect

            case Database::ERequestRegister:
            {
                if( i_reqDscr.m_iProgress_perCent >= 100 )
                {
                    if( !errResultInfo.isErrorResult() )
                    {
                        if( m_state == EStateConnected )
                        {
                            EState statePrev = m_state;
                            m_state = EStateRegistered;
                            emit stateChanged( m_state, statePrev );
                        }
                    }
                }
                break;
            } // case Database::ERequestRegister

            case Database::ERequestUnregister:
            {
                if( i_reqDscr.m_iProgress_perCent >= 100 )
                {
                    if( !errResultInfo.isErrorResult() )
                    {
                        if( m_state == EStateRegistered )
                        {
                            EState statePrev = m_state;
                            m_state = EStateConnected;
                            emit stateChanged( m_state, statePrev );
                        }
                    }
                }
                break;
            } // case Database::ERequestUnregister

            case Database::ERequestStopThread:
            {
                // StopThread must be the child request of Shutdown.
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

            case Database::ERequestDbDisconnect:
            {
                // DbDisconnect must be the child request of Shutdown.
                if( pReqMainInProgress == nullptr )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError, "DbDisconnect must be Child of the Main Shutdown Request" );
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
                    // The shutdown request is partially processed.
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
            } // case Database::ERequestDbDisconnect

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
bool CDbClientThread::event( QEvent* i_pEv )
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
                                case ERequestChangeSettings:
                                {
                                    executeChangeSettingsRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
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
                                case ERequestRegister:
                                {
                                    executeRegisterRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestUnregister:
                                {
                                    executeUnregisterRequest(pReqPostponed);
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

                } // case EBaseMsgTypeReqContinue:

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
                pReq->update();
                pReq->unlock();

                if( iProgress_perCent >= 100 )
                {
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
void CDbClientThread::start()
//------------------------------------------------------------------------------
{
    QThread::start();
}

//------------------------------------------------------------------------------
void CDbClientThread::run()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "run",
        /* strAddInfo   */ "" );

    m_pDbClt = new CDbClient( objectName() );

    m_pDbClt->setDb(m_pDb);
    m_pDbClt->setSocketType(m_socketTypeCurr);

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

                // Send startup confirmation message to the database client which created this thread.
                CMsgConStartup* pMsgCon = new CMsgConStartup(
                    /* pObjSender        */ m_pDbClt,
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
            /* pObjSender        */ m_pDbClt,
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
        delete m_pDbClt;
    }
    catch(...)
    {
    }
    m_pDbClt = nullptr;

} // run
