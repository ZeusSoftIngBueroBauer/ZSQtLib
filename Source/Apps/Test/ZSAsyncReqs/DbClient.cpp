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
#include <QtCore/qwaitcondition.h>
#include <QtNetwork/qhostinfo.h>

#include "DbClient.h"
#include "DbClientGateway.h"
#include "Msg.h"

#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysRequestSequencer.h"
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
class CDbClient : public QObject
*******************************************************************************/

/*==============================================================================
public: // class methods (request sequencer execution functions)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClient::startGatewayThread( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDbClient* pDbClt = dynamic_cast<CDbClient*>(i_pObjFctExecute);

    if( pDbClt != nullptr )
    {
        pReq = pDbClt->startGatewayThread( i_iTimeout_ms, i_bWait, i_iReqIdParent );
    }
    return pReq;

} // startGatewayThread

//------------------------------------------------------------------------------
CRequest* CDbClient::stopGatewayThread( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDbClient* pDbClt = dynamic_cast<CDbClient*>(i_pObjFctExecute);

    if( pDbClt != nullptr )
    {
        pReq = pDbClt->stopGatewayThread( i_iTimeout_ms, i_bWait, i_iReqIdParent );
    }
    return pReq;

} // stopGatewayThread

//------------------------------------------------------------------------------
CRequest* CDbClient::dbConnect( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDbClient* pDbClt = dynamic_cast<CDbClient*>(i_pObjFctExecute);

    if( pDbClt != nullptr )
    {
        pReq = pDbClt->dbConnect( i_iTimeout_ms, i_bWait, i_iReqIdParent );
    }
    return pReq;

} // dbConnect

//------------------------------------------------------------------------------
CRequest* CDbClient::dbDisconnect( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDbClient* pDbClt = dynamic_cast<CDbClient*>(i_pObjFctExecute);

    if( pDbClt != nullptr )
    {
        pReq = pDbClt->dbDisconnect( i_iTimeout_ms, i_bWait, i_iReqIdParent );
    }
    return pReq;

} // dbDisconnect

//------------------------------------------------------------------------------
CRequest* CDbClient::register_( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDbClient* pDbClt = dynamic_cast<CDbClient*>(i_pObjFctExecute);

    if( pDbClt != nullptr )
    {
        pReq = pDbClt->register_( i_iTimeout_ms, i_bWait, i_iReqIdParent );
    }
    return pReq;

} // register_

//------------------------------------------------------------------------------
CRequest* CDbClient::unregister( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDbClient* pDbClt = dynamic_cast<CDbClient*>(i_pObjFctExecute);

    if( pDbClt != nullptr )
    {
        pReq = pDbClt->unregister( i_iTimeout_ms, i_bWait, i_iReqIdParent );
    }
    return pReq;

} // unregister

//------------------------------------------------------------------------------
CRequest* CDbClient::selectSchema( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDbClient* pDbClt = dynamic_cast<CDbClient*>(i_pObjFctExecute);

    if( pDbClt != nullptr )
    {
        pReq = pDbClt->selectSchema( i_iTimeout_ms, i_bWait, i_iReqIdParent );
    }
    return pReq;

} // selectSchema

//------------------------------------------------------------------------------
CRequest* CDbClient::selectData( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDbClient* pDbClt = dynamic_cast<CDbClient*>(i_pObjFctExecute);

    if( pDbClt != nullptr )
    {
        pReq = pDbClt->selectData( i_iTimeout_ms, i_bWait, i_iReqIdParent );
    }
    return pReq;

} // selectData

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDbClient::CDbClient( const QString& i_strDbObjName ) :
//------------------------------------------------------------------------------
    QObject(),
    m_socketTypeCurr(Ipc::ESocketTypeInProcMsg),
    m_socketTypeNew(Ipc::ESocketTypeUndefined),
    // Connection settings (depending on connection type):
    m_pDb(nullptr),
    m_hostSettings(),
    m_pDbCltGtwThread(nullptr),
    m_pDbCltGtw(nullptr),
    // Requests from the client itself:
    m_pReqSeqrConnect(nullptr),
    m_pReqSeqrDisconnect(nullptr),
    m_pReqSeqrChangeSettings(nullptr),
    m_requestQueue(this,true),
    m_reqDscrTimeout(),
    m_pTmrReqTimeout(nullptr),
    m_bMsgReqContinuePending(false),
    // Requests received by the database:
    m_iReqMsgIdDb(-1),
    m_iReqIdDb(-1),
    m_pReqDb(nullptr),
    // Admin:
    m_state(EStateDisconnected),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strDbObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::AsyncRequests", "CDbClient", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_hostSettings.m_socketType = Ipc::ESocketTypeUndefined;
    // Socket connection parameters (valid after connection is established)
    m_hostSettings.m_strRemoteHostName = QHostInfo::localHostName();
    m_hostSettings.m_uRemotePort = 19673;
    m_hostSettings.m_iConnectTimeout_ms = 30000;
    m_hostSettings.m_uBufferSize = 4096;
    // Database specific parameters
    //m_hostSettings.m_strDriverType = "";
    //m_hostSettings.m_bDatabaseNameUsed = false;
    //m_hostSettings.m_strDatabaseName = "";
    //m_hostSettings.m_bUserNameUsed = false;
    //m_hostSettings.m_strUserName = "";
    //m_hostSettings.m_bPasswordUsed = false;
    //m_hostSettings.m_strPassword = "";

    m_pTmrReqTimeout = new QTimer(this);

    if( !QObject::connect(
        /* pObjSender   */ m_pTmrReqTimeout,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestTimeout()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Want to trace the sequencer. So it must be created after the trace server.
    // But before the central widget which creates the models accessing the sequencers.
    // In addition the request sequencers must be destroyed before the request queue.
    // On clearing the sequencer the sequence might access (abort) requests
    // which have already been destroyed by the request queue.
    m_pReqSeqrConnect = new CRequestSequencer(
            /* strObjName             */ "ZS::Apps::Test::AsyncRequests::CDbClient::" + objectName() + ".connect",
            /* pObjParent             */ this,
            /* bKeepReqDscrInExecTree */ false );

    m_pReqSeqrDisconnect = new CRequestSequencer(
            /* strObjName             */ "ZS::Apps::Test::AsyncRequests::CDbClient::" + objectName() + ".disconnect",
            /* pObjParent             */ this,
            /* bKeepReqDscrInExecTree */ false );

    // <RequestSequencer> Connect
    //----------------------------

    qint64 iReqIdStartGatewayThread = m_pReqSeqrConnect->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CDbClient",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestStartThread,
        /* strRequest               */ Request2Str(ERequestStartThread),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ startGatewayThread,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ -1 );

    qint64 iReqIdDbConnect = m_pReqSeqrConnect->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CDbClient",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestDbConnect,
        /* strRequest               */ Request2Str(ERequestDbConnect),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ dbConnect,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ iReqIdStartGatewayThread );

    qint64 iReqIdSelectSchema = m_pReqSeqrConnect->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CDbClient",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestSelectSchema,
        /* strRequest               */ Request2Str(ERequestSelectSchema),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ selectSchema,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ iReqIdDbConnect );

    qint64 iReqIdSelectData = m_pReqSeqrConnect->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CDbClient",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestSelectData,
        /* strRequest               */ Request2Str(ERequestSelectData),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ selectData,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ iReqIdSelectSchema );

    /*qint64 iReqIdRegister =*/ m_pReqSeqrConnect->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CDbClient",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestRegister,
        /* strRequest               */ Request2Str(ERequestRegister),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ register_,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ iReqIdSelectData );

    // <RequestSequencer> Disconnect
    //------------------------------

    qint64 iReqIdUnregister = m_pReqSeqrDisconnect->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CDbClient",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestUnregister,
        /* strRequest               */ Request2Str(ERequestUnregister),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ unregister,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ -1 );

    qint64 iReqIdDbDisconnect = m_pReqSeqrDisconnect->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CDbClient",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestDbDisconnect,
        /* strRequest               */ Request2Str(ERequestDbDisconnect),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ dbDisconnect,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ iReqIdUnregister );

    /*qint64 iReqIdStopGatewayThread =*/ m_pReqSeqrDisconnect->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CDbClient",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestStopThread,
        /* strRequest               */ Request2Str(ERequestStopThread),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ stopGatewayThread,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ iReqIdDbDisconnect );

} // ctor

//------------------------------------------------------------------------------
CDbClient::~CDbClient()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    // The request sequencers must be destroyed before the request queue.
    // On clearing the sequencer the sequence might access (abort) requests
    // which have already been destroyed by the request queue.

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("dtor");

    try
    {
        delete m_pReqSeqrChangeSettings;
    }
    catch(...)
    {
    }
    m_pReqSeqrChangeSettings = nullptr;

    try
    {
        delete m_pReqSeqrConnect;
    }
    catch(...)
    {
    }
    m_pReqSeqrConnect = nullptr;

    try
    {
        delete m_pReqSeqrDisconnect;
    }
    catch(...)
    {
    }
    m_pReqSeqrDisconnect = nullptr;

    try
    {
        delete m_pReqDb;
    }
    catch(...)
    {
    }
    m_pReqDb = nullptr;

    if( m_pDbCltGtwThread != nullptr )
    {
        m_pDbCltGtwThread->quit();

        if( !m_pDbCltGtwThread->wait(5000) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultTimeout);
            errResultInfo.setAddErrInfoDscr( "Waiting for StopThread timed out." );

            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
            m_pDbCltGtwThread->deleteLater();
            m_pDbCltGtwThread = nullptr;
        }
        else
        {
            try
            {
                delete m_pDbCltGtwThread;
            }
            catch(...)
            {
            }
            m_pDbCltGtwThread = nullptr;
        }
    } // if( m_pDbCltGtwThread != nullptr )

    m_pDb = nullptr;
    m_pDbCltGtw = nullptr;
    m_pTmrReqTimeout = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::setSocketType( ZS::Ipc::ESocketType i_socketType )
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

            m_hostSettings.m_socketType = m_socketTypeCurr;
        }
    }

} // setSocketType

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::setDb( CDb* i_pDb )
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

    if( m_pDb != nullptr )
    {
        if( !QObject::disconnect(
            /* pObjSender   */ m_pDb,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onDbDestroyed(QObject*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }

    if( m_pDb != nullptr && m_pDb != i_pDb )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Changing database reference is not allowed" );
    }

    m_pDb = i_pDb;

    if( m_pDb != nullptr )
    {
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

    } // if( m_pDb != nullptr )

} // setDb

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClient::changeSettings( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strClassName */ "CDbClient",
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
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeChangeSettingsRequest( pReq, pReqInProgress );
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // changeSettings

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClient::connect_( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("connect");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbClient",
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
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeConnectRequest( pReq, pReqInProgress );
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // connect_

//------------------------------------------------------------------------------
CRequest* CDbClient::disconnect_( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("disconnect");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbClient",
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
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeDisconnectRequest( pReq, pReqInProgress );
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // disconnect_

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClient::register_( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strMethod    */ "register",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("register");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbClient",
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
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeRegisterRequest( pReq, pReqInProgress );
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // register_

//------------------------------------------------------------------------------
CRequest* CDbClient::unregister( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strClassName */ "CDbClient",
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
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeUnregisterRequest( pReq, pReqInProgress );
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // unregister

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClient::selectSchema( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strClassName */ "CDbClient",
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
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeSelectSchemaRequest( pReq, pReqInProgress );
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // selectSchema

//------------------------------------------------------------------------------
CRequest* CDbClient::updateSchema( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strClassName */ "CDbClient",
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
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeUpdateSchemaRequest( pReq, pReqInProgress );
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // updateSchema

//------------------------------------------------------------------------------
CRequest* CDbClient::selectData( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strClassName */ "CDbClient",
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
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeSelectDataRequest( pReq, pReqInProgress );
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // selectData

//------------------------------------------------------------------------------
CRequest* CDbClient::updateData( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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
        /* strClassName */ "CDbClient",
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
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeUpdateDataRequest( pReq, pReqInProgress );
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // updateData

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClient::onReceivedMsgReqUpdateSchema( CMsgReqUpdateSchema* i_pMsgReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo  = "MsgReq: " + i_pMsgReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( m_pReqDb == nullptr )
        {
            strAddTrcInfo += ", ReqDb: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
            strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onReceivedMsgReqUpdateSchema",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pReqDb != nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, "m_pDbReqDb != nullptr" );
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("onReceivedMsgReqUpdateSchema");

    m_iReqMsgIdDb = i_pMsgReq->getMsgId();
    m_iReqIdDb = i_pMsgReq->getRequestId();

    // Initialized as an asynchronous request (ResultUndefined).
    m_pReqDb = new CRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CDbClient",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestUpdateSchema,
        /* strRequest               */ Request2Str(ERequestUpdateSchema),
        /* iReqIdParent             */ m_iReqIdDb,
        /* bKeepReqDscrInExecTree   */ true,
        /* iTimeout_ms              */ 0,
        /* bIsBlocking              */ false );

    m_pReqDb->setErrResultInfo(errResultInfo);
    m_pReqDb->setProgressInPerCent(100);
    m_pReqDb->update();

    return m_pReqDb;

} // onReceivedMsgReqUpdateSchema

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClient::onReceivedMsgReqUpdateData( CMsgReqUpdateData* i_pMsgReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo  = "MsgReq: " + i_pMsgReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( m_pReqDb == nullptr )
        {
            strAddTrcInfo += ", ReqDb: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
            strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onReceivedMsgReqUpdateData",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pReqDb != nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, "m_pDbReqDb != nullptr" );
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("onReceivedMsgReqUpdateData");

    m_iReqMsgIdDb = i_pMsgReq->getMsgId();
    m_iReqIdDb = i_pMsgReq->getRequestId();

    // Initialized as an asynchronous request (ResultUndefined).
    m_pReqDb = new CRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CDbClient",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestUpdateData,
        /* strRequest               */ Request2Str(ERequestUpdateData),
        /* iReqIdParent             */ m_iReqIdDb,
        /* bKeepReqDscrInExecTree   */ true,
        /* iTimeout_ms              */ 0,
        /* bIsBlocking              */ false );

    m_pReqDb->setErrResultInfo(errResultInfo);
    m_pReqDb->setProgressInPerCent(100);
    m_pReqDb->update();

    return m_pReqDb;

} // onReceivedMsgReqUpdateData

/*==============================================================================
protected: // overridables (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::executeNextPostponedRequest()
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
void CDbClient::executeChangeSettingsRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( m_pReqDb == nullptr )
        {
            strAddTrcInfo += ", ReqDb: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
            strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeChangeSettingsRequest",
        /* strAddInfo   */ strAddTrcInfo );

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

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    qint64 iReqIdParent = i_pReq->getParentId();

    // If the client is not connected ...
    if( m_state == EStateDisconnected )
    {
        if( m_socketTypeNew != Ipc::ESocketTypeUndefined )
        {
            m_socketTypeCurr = m_socketTypeNew;
            m_socketTypeNew = Ipc::ESocketTypeUndefined;

            m_hostSettings.m_socketType = m_socketTypeCurr;
        }

        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();

    } // if( m_state == EStateDisconnected )

    else // if( m_state != EStateDisconnected )
    {
        delete m_pReqSeqrChangeSettings;
        m_pReqSeqrChangeSettings = nullptr;

        bool bWasIpcSocketType = !(m_socketTypeCurr == Ipc::ESocketTypeInProcMsg);
        bool bIsIpcSocketType = bWasIpcSocketType;

        if( m_socketTypeNew != Ipc::ESocketTypeUndefined )
        {
            bIsIpcSocketType = !(m_socketTypeNew == Ipc::ESocketTypeInProcMsg);
        }

        // If the gateway has to be newly created or is no longer needed and has to be destroyed ..
        if( bIsIpcSocketType != bWasIpcSocketType )
        {
            m_pReqSeqrChangeSettings = new CRequestSequencer(
                /* strObjName             */ "ZS::Apps::Test::AsyncRequests::CDbClient::" + objectName() + "changeSettings",
                /* pObjParent             */ this,
                /* bKeepReqDscrInExecTree */ false );

            // Disconnect
            //------------------------------

            qint64 iReqIdUnregister = m_pReqSeqrChangeSettings->addRequest(
                /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
                /* strCreatorClassName      */ "CDbClient",
                /* pObjCreator              */ this,
                /* iRequest                 */ ERequestUnregister,
                /* strRequest               */ Request2Str(ERequestUnregister),
                /* pvObjFctExecute          */ this,
                /* fctExecute               */ unregister,
                /* pvExec                   */ nullptr,
                /* iReqIdParent             */ -1,
                /* iReqIdPredecessor        */ -1 );

            qint64 iReqIdDbDisconnect = m_pReqSeqrChangeSettings->addRequest(
                /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
                /* strCreatorClassName      */ "CDbClient",
                /* pObjCreator              */ this,
                /* iRequest                 */ ERequestDbDisconnect,
                /* strRequest               */ Request2Str(ERequestDbDisconnect),
                /* pvObjFctExecute          */ this,
                /* fctExecute               */ dbDisconnect,
                /* pvExec                   */ nullptr,
                /* iReqIdParent             */ -1,
                /* iReqIdPredecessor        */ iReqIdUnregister );

            qint64 iReqIdStopGatewayThread = m_pReqSeqrChangeSettings->addRequest(
                /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
                /* strCreatorClassName      */ "CDbClient",
                /* pObjCreator              */ this,
                /* iRequest                 */ ERequestStopThread,
                /* strRequest               */ Request2Str(ERequestStopThread),
                /* pvObjFctExecute          */ this,
                /* fctExecute               */ stopGatewayThread,
                /* pvExec                   */ nullptr,
                /* iReqIdParent             */ -1,
                /* iReqIdPredecessor        */ iReqIdDbDisconnect );

            // Connect
            //----------------------------

            qint64 iReqIdStartGatewayThread = m_pReqSeqrChangeSettings->addRequest(
                /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
                /* strCreatorClassName      */ "CDbClient",
                /* pObjCreator              */ this,
                /* iRequest                 */ ERequestStartThread,
                /* strRequest               */ Request2Str(ERequestStartThread),
                /* pvObjFctExecute          */ this,
                /* fctExecute               */ startGatewayThread,
                /* pvExec                   */ nullptr,
                /* iReqIdParent             */ -1,
                /* iReqIdPredecessor        */ iReqIdStopGatewayThread );

            qint64 iReqIdDbConnect = m_pReqSeqrChangeSettings->addRequest(
                /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
                /* strCreatorClassName      */ "CDbClient",
                /* pObjCreator              */ this,
                /* iRequest                 */ ERequestDbConnect,
                /* strRequest               */ Request2Str(ERequestDbConnect),
                /* pvObjFctExecute          */ this,
                /* fctExecute               */ dbConnect,
                /* pvExec                   */ nullptr,
                /* iReqIdParent             */ -1,
                /* iReqIdPredecessor        */ iReqIdStartGatewayThread );

            qint64 iReqIdSelectSchema = m_pReqSeqrChangeSettings->addRequest(
                /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
                /* strCreatorClassName      */ "CDbClient",
                /* pObjCreator              */ this,
                /* iRequest                 */ ERequestSelectSchema,
                /* strRequest               */ Request2Str(ERequestSelectSchema),
                /* pvObjFctExecute          */ this,
                /* fctExecute               */ selectSchema,
                /* pvExec                   */ nullptr,
                /* iReqIdParent             */ -1,
                /* iReqIdPredecessor        */ iReqIdDbConnect );

            qint64 iReqIdSelectData = m_pReqSeqrChangeSettings->addRequest(
                /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
                /* strCreatorClassName      */ "CDbClient",
                /* pObjCreator              */ this,
                /* iRequest                 */ ERequestSelectData,
                /* strRequest               */ Request2Str(ERequestSelectData),
                /* pvObjFctExecute          */ this,
                /* fctExecute               */ selectData,
                /* pvExec                   */ nullptr,
                /* iReqIdParent             */ -1,
                /* iReqIdPredecessor        */ iReqIdSelectSchema );

            /*qint64 iReqIdRegister =*/ m_pReqSeqrChangeSettings->addRequest(
                /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
                /* strCreatorClassName      */ "CDbClient",
                /* pObjCreator              */ this,
                /* iRequest                 */ ERequestRegister,
                /* strRequest               */ Request2Str(ERequestRegister),
                /* pvObjFctExecute          */ this,
                /* fctExecute               */ register_,
                /* pvExec                   */ nullptr,
                /* iReqIdParent             */ -1,
                /* iReqIdPredecessor        */ iReqIdSelectData );

            m_pReqSeqrChangeSettings->start(i_pReq);

        } // if( bIsIpcSocketType != bWasIpcSocketType )

        // If it is not necesary to newly create or destroy the gateway ..
        else // if( bIsIpcSocketType == bWasIpcSocketType )
        {
            if( m_socketTypeCurr == Ipc::ESocketTypeInProcMsg )
            {
                // No Gateway needed. Messages are directly exchanged between the database and the client.
                if( m_pDb == nullptr )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultRequestRefused);
                    errResultInfo.setAddErrInfoDscr( "Reference to database object not defined." );
                }

                // The changes can be directly taken over (synchronous request and no request sequence needed).
            }
            else // if( m_socketTypeCurr != Ipc::ESocketTypeInProcMsg )
            {
                if( m_pDbCltGtw == nullptr )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultRequestRefused);
                    errResultInfo.setAddErrInfoDscr( "Gateway is not created." );
                }
                else
                {
                    // The change settings request message hast to be forwarded to the gateway.
                    CMsgReqChangeSettings* pMsgReq = new CMsgReqChangeSettings(
                        /* pObjSender       */ this,
                        /* pObjReceiver     */ m_pDbCltGtw,
                        /* bMustBeConfirmed */ true,
                        /* iReqId           */ i_pReq->getId(),
                        /* iMsgId           */ -1 );
                    Ipc::SClientHostSettings hostSettingsNew = m_hostSettings;
                    if( m_socketTypeNew != Ipc::ESocketTypeUndefined )
                    {
                        hostSettingsNew.m_socketType = m_socketTypeNew;
                    }
                    pMsgReq->setClientHostSettings(hostSettingsNew);
                    POST_OR_DELETE_MESSAGE(pMsgReq);

                    if( i_pReq->isBlockingRequest() )
                    {
                        if( !i_pReq->wait() )
                        {
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultTimeout);
                            errResultInfo.setAddErrInfoDscr( "Waiting for ChangeSettings timed out." );
                        }
                        if( errResultInfo.getResult() == EResultUndefined )
                        {
                            errResultInfo.setErrResult(ErrResultSuccess);
                        }
                        i_pReq->setErrResultInfo(errResultInfo);
                        i_pReq->setProgressInPerCent(100);
                        i_pReq->update();
                        deleteResultConfirmationMessage(i_pReq, this);
                    }
                    else // if( !i_pReq->isBlockingRequest() )
                    {
                        if( i_pReq->getTimeoutInMs() > 0 )
                        {
                            m_reqDscrTimeout = i_pReq->getDescriptor();
                            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
                        }
                    }
                }
            }
        }
    } // if( m_state != EStateDisconnected )

    if( errResultInfo.isErrorResult() )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();
    }

    if( !isAsynchronousRequest(i_pReq) )
    {
        if( m_requestQueue.isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
        {
            m_requestQueue.removeRequestInProgress( i_pReq->getId() );
        }

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

        bool bIsWaiting = false;

        if( iReqIdParent >= 0 )
        {
            bIsWaiting = CRequestExecTree::GetInstance()->isWaiting(iReqIdParent);
        }

        if( bIsWaiting )
        {
            CRequestExecTree::GetInstance()->wake(iReqIdParent);
        }
        else
        {
            CMsgCon* pMsgCon = dynamic_cast<CMsgCon*>(i_pReq->takeInitiatorConfirmationMessage());

            if( pMsgCon != nullptr )
            {
                pMsgCon->setErrResultInfo(errResultInfo);
                pMsgCon->setProgress(100);
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }
        }
    } // if( !isAsynchronousRequest(i_pReq) )

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
void CDbClient::executeConnectRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( m_pReqDb == nullptr )
        {
            strAddTrcInfo += ", ReqDb: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
            strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeConnectRequest",
        /* strAddInfo   */ strAddTrcInfo );

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

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    qint64 iReqIdParent = i_pReq->getParentId();

    // If the client is not connected ...
    if( m_state == EStateDisconnected )
    {
        m_pReqSeqrConnect->start(i_pReq);
    }
    else // if( m_state != EStateDisconnected )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Client is already connected." );

    } // if( m_state != EStateDisconnected )

    if( errResultInfo.isErrorResult() )
    {
        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();
    }

    if( !isAsynchronousRequest(i_pReq) )
    {
        if( m_requestQueue.isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
        {
            m_requestQueue.removeRequestInProgress( i_pReq->getId() );
        }

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

        bool bIsWaiting = false;

        if( iReqIdParent >= 0 )
        {
            bIsWaiting = CRequestExecTree::GetInstance()->isWaiting(iReqIdParent);
        }

        if( bIsWaiting )
        {
            CRequestExecTree::GetInstance()->wake(iReqIdParent);
        }
        else
        {
            CMsgCon* pMsgCon = dynamic_cast<CMsgCon*>(i_pReq->takeInitiatorConfirmationMessage());

            if( pMsgCon != nullptr )
            {
                pMsgCon->setErrResultInfo(errResultInfo);
                pMsgCon->setProgress(100);
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }
        }
    } // if( !isAsynchronousRequest(i_pReq) )

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeConnectRequest

//------------------------------------------------------------------------------
void CDbClient::executeDisconnectRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( m_pReqDb == nullptr )
        {
            strAddTrcInfo += ", ReqDb: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
            strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeDisconnectRequest",
        /* strAddInfo   */ strAddTrcInfo );

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

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    qint64 iReqIdParent = i_pReq->getParentId();

    // If the client is connected ...
    if( m_state >= EStateConnected )
    {
        m_pReqSeqrDisconnect->start(i_pReq);
    }
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

    if( !isAsynchronousRequest(i_pReq) )
    {
        if( m_requestQueue.isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
        {
            m_requestQueue.removeRequestInProgress( i_pReq->getId() );
        }

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

        bool bIsWaiting = false;

        if( iReqIdParent >= 0 )
        {
            bIsWaiting = CRequestExecTree::GetInstance()->isWaiting(iReqIdParent);
        }

        if( bIsWaiting )
        {
            CRequestExecTree::GetInstance()->wake(iReqIdParent);
        }
        else
        {
            CMsgCon* pMsgCon = dynamic_cast<CMsgCon*>(i_pReq->takeInitiatorConfirmationMessage());

            if( pMsgCon != nullptr )
            {
                pMsgCon->setErrResultInfo(errResultInfo);
                pMsgCon->setProgress(100);
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }
        }
    } // if( !isAsynchronousRequest(i_pReq) )

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
void CDbClient::executeRegisterRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( m_pReqDb == nullptr )
        {
            strAddTrcInfo += ", ReqDb: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
            strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeRegisterRequest",
        /* strAddInfo   */ strAddTrcInfo );

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

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    qint64 iReqIdParent = i_pReq->getParentId();

    // If the client is connected ...
    if( m_state == EStateConnected )
    {
        QObject* pMsgReqReceiver = nullptr;

        if( m_socketTypeCurr == Ipc::ESocketTypeInProcMsg )
        {
            // No Gateway needed. Messages are directly exchanged between the database and the client.
            if( m_pDb == nullptr )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultRequestRefused);
                errResultInfo.setAddErrInfoDscr( "Reference to database object not defined." );
            }
            else
            {
                pMsgReqReceiver = m_pDb;
            }
        }
        else // if( m_socketTypeCurr != Ipc::ESocketTypeInProcMsg )
        {
            if( m_pDbCltGtw == nullptr )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultRequestRefused);
                errResultInfo.setAddErrInfoDscr( "Gateway is not created." );
            }
            else
            {
                pMsgReqReceiver = m_pDbCltGtw;
            }
        }

        if( !errResultInfo.isErrorResult() )
        {
            CMsgReqRegister* pMsgReq = new CMsgReqRegister(
                /* pObjSender       */ this,
                /* pObjReceiver     */ pMsgReqReceiver,
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
                    errResultInfo.setAddErrInfoDscr( "Waiting for register confirmation timed out." );
                }
                if( errResultInfo.getResult() == EResultUndefined )
                {
                    errResultInfo.setErrResult(ErrResultSuccess);
                }
                i_pReq->setErrResultInfo(errResultInfo);
                i_pReq->setProgressInPerCent(100);
                i_pReq->update();
                deleteResultConfirmationMessage(i_pReq, this);

                // If the database client has been registered ..
                if( !errResultInfo.isErrorResult() )
                {
                    EState statePrev = m_state;
                    m_state = EStateRegistered;
                    emit stateChanged( m_state, statePrev );
                }
            }
            else // if( !i_pReq->isBlockingRequest() )
            {
                if( i_pReq->getTimeoutInMs() > 0 )
                {
                    m_reqDscrTimeout = i_pReq->getDescriptor();
                    m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
                }
            }
        } // if( !errResultInfo.isErrorResult() )
    } // if( m_state == EStateConnected )

    // If the client is not connected ...
    else // if( m_state != EStateConnected )
    {
        // If the client is already registered ...
        if( m_state == EStateRegistered )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Client is already registered." );
        }
        else
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Client is not connected." );
        }
    } // if( m_state != EStateConnected )

    if( errResultInfo.isErrorResult() )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();
    }

    if( !isAsynchronousRequest(i_pReq) )
    {
        if( m_requestQueue.isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
        {
            m_requestQueue.removeRequestInProgress( i_pReq->getId() );
        }

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

        bool bIsWaiting = false;

        if( iReqIdParent >= 0 )
        {
            bIsWaiting = CRequestExecTree::GetInstance()->isWaiting(iReqIdParent);
        }

        if( bIsWaiting )
        {
            CRequestExecTree::GetInstance()->wake(iReqIdParent);
        }
        else
        {
            CMsgCon* pMsgCon = dynamic_cast<CMsgCon*>(i_pReq->takeInitiatorConfirmationMessage());

            if( pMsgCon != nullptr )
            {
                pMsgCon->setErrResultInfo(errResultInfo);
                pMsgCon->setProgress(100);
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }
        }
    } // if( !isAsynchronousRequest(i_pReq) )

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeRegisterRequest

//------------------------------------------------------------------------------
void CDbClient::executeUnregisterRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( m_pReqDb == nullptr )
        {
            strAddTrcInfo += ", ReqDb: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
            strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeUnregisterRequest",
        /* strAddInfo   */ strAddTrcInfo );

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

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    qint64 iReqIdParent = i_pReq->getParentId();

    // If the client is registered ...
    if( m_state == EStateRegistered )
    {
        QObject* pMsgReqReceiver = nullptr;

        if( m_socketTypeCurr == Ipc::ESocketTypeInProcMsg )
        {
            // No Gateway needed. Messages are directly exchanged between the database and the client.
            if( m_pDb == nullptr )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultRequestRefused);
                errResultInfo.setAddErrInfoDscr( "Reference to database object not defined." );
            }
            else
            {
                pMsgReqReceiver = m_pDb;
            }
        }
        else // if( m_socketTypeCurr != Ipc::ESocketTypeInProcMsg )
        {
            if( m_pDbCltGtw == nullptr )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultRequestRefused);
                errResultInfo.setAddErrInfoDscr( "Gateway is not created." );
            }
            else
            {
                pMsgReqReceiver = m_pDbCltGtw;
            }
        }

        if( !errResultInfo.isErrorResult() )
        {
            CMsgReqUnregister* pMsgReq = new CMsgReqUnregister(
                /* pObjSender       */ this,
                /* pObjReceiver     */ pMsgReqReceiver,
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
                    errResultInfo.setAddErrInfoDscr( "Waiting for unregister confirmation timed out." );
                }
                if( errResultInfo.getResult() == EResultUndefined )
                {
                    errResultInfo.setErrResult(ErrResultSuccess);
                }
                i_pReq->setErrResultInfo(errResultInfo);
                i_pReq->setProgressInPerCent(100);
                i_pReq->update();
                deleteResultConfirmationMessage(i_pReq, this);
            }
            else // if( !i_pReq->isBlockingRequest() )
            {
                if( i_pReq->getTimeoutInMs() > 0 )
                {
                    m_reqDscrTimeout = i_pReq->getDescriptor();
                    m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
                }
            }
        } // if( !errResultInfo.isErrorResult() )
    } // if( m_state == EStateRegistered )

    // If the client is not registered ...
    else // if( m_state != EStateRegistered )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Client is not registered." );

    } // if( m_state != EStateRegistered )

    if( errResultInfo.isErrorResult() )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();
    }

    if( !isAsynchronousRequest(i_pReq) )
    {
        if( m_requestQueue.isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
        {
            m_requestQueue.removeRequestInProgress( i_pReq->getId() );
        }

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

        bool bIsWaiting = false;

        if( iReqIdParent >= 0 )
        {
            bIsWaiting = CRequestExecTree::GetInstance()->isWaiting(iReqIdParent);
        }

        if( bIsWaiting )
        {
            CRequestExecTree::GetInstance()->wake(iReqIdParent);
        }
        else
        {
            CMsgCon* pMsgCon = dynamic_cast<CMsgCon*>(i_pReq->takeInitiatorConfirmationMessage());

            if( pMsgCon != nullptr )
            {
                pMsgCon->setErrResultInfo(errResultInfo);
                pMsgCon->setProgress(100);
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }
        }
    } // if( !isAsynchronousRequest(i_pReq) )

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
void CDbClient::executeSelectSchemaRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( m_pReqDb == nullptr )
        {
            strAddTrcInfo += ", ReqDb: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
            strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeSelectSchemaRequest",
        /* strAddInfo   */ strAddTrcInfo );

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

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    qint64 iReqIdParent = i_pReq->getParentId();

    // If the client is connected ...
    if( m_state >= EStateConnected )
    {
        QObject* pMsgReqReceiver = nullptr;

        if( m_socketTypeCurr == Ipc::ESocketTypeInProcMsg )
        {
            // No Gateway needed. Messages are directly exchanged between the database and the client.
            if( m_pDb == nullptr )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultRequestRefused);
                errResultInfo.setAddErrInfoDscr( "Reference to database object not defined." );
            }
            else
            {
                pMsgReqReceiver = m_pDb;
            }
        }
        else // if( m_socketTypeCurr != Ipc::ESocketTypeInProcMsg )
        {
            if( m_pDbCltGtw == nullptr )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultRequestRefused);
                errResultInfo.setAddErrInfoDscr( "Gateway is not created." );
            }
            else
            {
                pMsgReqReceiver = m_pDbCltGtw;
            }
        }

        if( !errResultInfo.isErrorResult() )
        {
            CMsgReqSelectSchema* pMsgReq = new CMsgReqSelectSchema(
                /* pObjSender   */ this,
                /* pObjReceiver */ pMsgReqReceiver,
                /* iReqId       */ i_pReq->getId(),
                /* iMsgId       */ -1 );
            POST_OR_DELETE_MESSAGE(pMsgReq);

            // Querying data from the database as a blocking request doesn't really make sense.
            // The confirmation message must contain data which will be received in the gateway thread.
            // The gateway would have to store the data somewhere in the request before waking up
            // the request changing the thread affinity of the data objects.
            if( i_pReq->isBlockingRequest() )
            {
                if( !i_pReq->wait() )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultTimeout);
                    errResultInfo.setAddErrInfoDscr( "Waiting for select schema confirmation timed out." );
                }
                if( errResultInfo.getResult() == EResultUndefined )
                {
                    errResultInfo.setErrResult(ErrResultSuccess);
                }
                i_pReq->setErrResultInfo(errResultInfo);
                i_pReq->setProgressInPerCent(100);
                i_pReq->update();
                deleteResultConfirmationMessage(i_pReq, this);
            }
            else // if( !i_pReq->isBlockingRequest() )
            {
                if( i_pReq->getTimeoutInMs() > 0 )
                {
                    m_reqDscrTimeout = i_pReq->getDescriptor();
                    m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
                }
            }
        } // if( !errResultInfo.isErrorResult() )
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
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();
    }

    if( !isAsynchronousRequest(i_pReq) )
    {
        if( m_requestQueue.isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
        {
            m_requestQueue.removeRequestInProgress( i_pReq->getId() );
        }

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

        bool bIsWaiting = false;

        if( iReqIdParent >= 0 )
        {
            bIsWaiting = CRequestExecTree::GetInstance()->isWaiting(iReqIdParent);
        }

        if( bIsWaiting )
        {
            CRequestExecTree::GetInstance()->wake(iReqIdParent);
        }
        else
        {
            CMsgCon* pMsgCon = dynamic_cast<CMsgCon*>(i_pReq->takeInitiatorConfirmationMessage());

            if( pMsgCon != nullptr )
            {
                pMsgCon->setErrResultInfo(errResultInfo);
                pMsgCon->setProgress(100);
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }
        }
    } // if( !isAsynchronousRequest(i_pReq) )

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeSelectSchemaRequest

//------------------------------------------------------------------------------
void CDbClient::executeUpdateSchemaRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( m_pReqDb == nullptr )
        {
            strAddTrcInfo += ", ReqDb: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
            strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeUpdateSchemaRequest",
        /* strAddInfo   */ strAddTrcInfo );

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

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    qint64 iReqIdParent = i_pReq->getParentId();

    // If the client is connected ...
    if( m_state >= EStateConnected )
    {
        QObject* pMsgReqReceiver = nullptr;

        if( m_socketTypeCurr == Ipc::ESocketTypeInProcMsg )
        {
            // No Gateway needed. Messages are directly exchanged between the database and the client.
            if( m_pDb == nullptr )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultRequestRefused);
                errResultInfo.setAddErrInfoDscr( "Reference to database object not defined." );
            }
            else
            {
                pMsgReqReceiver = m_pDb;
            }
        }
        else // if( m_socketTypeCurr != Ipc::ESocketTypeInProcMsg )
        {
            if( m_pDbCltGtw == nullptr )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultRequestRefused);
                errResultInfo.setAddErrInfoDscr( "Gateway is not created." );
            }
            else
            {
                pMsgReqReceiver = m_pDbCltGtw;
            }
        }

        if( !errResultInfo.isErrorResult() )
        {
            CMsgReqUpdateSchema* pMsgReq = new CMsgReqUpdateSchema(
                /* pObjSender       */ this,
                /* pObjReceiver     */ pMsgReqReceiver,
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
                    errResultInfo.setAddErrInfoDscr( "Waiting for update schema confirmation timed out." );
                }
                if( errResultInfo.getResult() == EResultUndefined )
                {
                    errResultInfo.setErrResult(ErrResultSuccess);
                }
                i_pReq->setErrResultInfo(errResultInfo);
                i_pReq->setProgressInPerCent(100);
                i_pReq->update();
                deleteResultConfirmationMessage(i_pReq, this);
            }
            else // if( !i_pReq->isBlockingRequest() )
            {
                if( i_pReq->getTimeoutInMs() > 0 )
                {
                    m_reqDscrTimeout = i_pReq->getDescriptor();
                    m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
                }
            }
        } // if( !errResultInfo.isErrorResult() )
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
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();
    }

    if( !isAsynchronousRequest(i_pReq) )
    {
        if( m_requestQueue.isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
        {
            m_requestQueue.removeRequestInProgress( i_pReq->getId() );
        }

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

        bool bIsWaiting = false;

        if( iReqIdParent >= 0 )
        {
            bIsWaiting = CRequestExecTree::GetInstance()->isWaiting(iReqIdParent);
        }

        if( bIsWaiting )
        {
            CRequestExecTree::GetInstance()->wake(iReqIdParent);
        }
        else
        {
            CMsgCon* pMsgCon = dynamic_cast<CMsgCon*>(i_pReq->takeInitiatorConfirmationMessage());

            if( pMsgCon != nullptr )
            {
                pMsgCon->setErrResultInfo(errResultInfo);
                pMsgCon->setProgress(100);
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }
        }
    } // if( !isAsynchronousRequest(i_pReq) )

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
void CDbClient::executeSelectDataRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( m_pReqDb == nullptr )
        {
            strAddTrcInfo += ", ReqDb: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
            strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeSelectDataRequest",
        /* strAddInfo   */ strAddTrcInfo );

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

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    qint64 iReqIdParent = i_pReq->getParentId();

    // If the client is connected ...
    if( m_state >= EStateConnected )
    {
        QObject* pMsgReqReceiver = nullptr;

        if( m_socketTypeCurr == Ipc::ESocketTypeInProcMsg )
        {
            // No Gateway needed. Messages are directly exchanged between the database and the client.
            if( m_pDb == nullptr )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultRequestRefused);
                errResultInfo.setAddErrInfoDscr( "Reference to database object not defined." );
            }
            else
            {
                pMsgReqReceiver = m_pDb;
            }
        }
        else // if( m_socketTypeCurr != Ipc::ESocketTypeInProcMsg )
        {
            if( m_pDbCltGtw == nullptr )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultRequestRefused);
                errResultInfo.setAddErrInfoDscr( "Gateway is not created." );
            }
            else
            {
                pMsgReqReceiver = m_pDbCltGtw;
            }
        }

        if( !errResultInfo.isErrorResult() )
        {
            CMsgReqSelectData* pMsgReq = new CMsgReqSelectData(
                /* pObjSender   */ this,
                /* pObjReceiver */ pMsgReqReceiver,
                /* iReqId       */ i_pReq->getId(),
                /* iMsgId       */ -1 );
            POST_OR_DELETE_MESSAGE(pMsgReq);

            // Querying data from the database as a blocking request doesn't really make sense.
            // The confirmation message must contain data which will be received in the gateway thread.
            // The gateway would have to store the data somewhere in the request before waking up
            // the request changing the thread affinity of the data objects.
            if( i_pReq->isBlockingRequest() )
            {
                if( !i_pReq->wait() )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultTimeout);
                    errResultInfo.setAddErrInfoDscr( "Waiting for select data confirmation timed out." );
                }
                if( errResultInfo.getResult() == EResultUndefined )
                {
                    errResultInfo.setErrResult(ErrResultSuccess);
                }
                i_pReq->setErrResultInfo(errResultInfo);
                i_pReq->setProgressInPerCent(100);
                i_pReq->update();
                deleteResultConfirmationMessage(i_pReq, this);
            }
            else // if( !i_pReq->isBlockingRequest() )
            {
                if( i_pReq->getTimeoutInMs() > 0 )
                {
                    m_reqDscrTimeout = i_pReq->getDescriptor();
                    m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
                }
            }
        } // if( !errResultInfo.isErrorResult() )
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
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();
    }

    if( !isAsynchronousRequest(i_pReq) )
    {
        if( m_requestQueue.isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
        {
            m_requestQueue.removeRequestInProgress( i_pReq->getId() );
        }

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

        bool bIsWaiting = false;

        if( iReqIdParent >= 0 )
        {
            bIsWaiting = CRequestExecTree::GetInstance()->isWaiting(iReqIdParent);
        }

        if( bIsWaiting )
        {
            CRequestExecTree::GetInstance()->wake(iReqIdParent);
        }
        else
        {
            CMsgCon* pMsgCon = dynamic_cast<CMsgCon*>(i_pReq->takeInitiatorConfirmationMessage());

            if( pMsgCon != nullptr )
            {
                pMsgCon->setErrResultInfo(errResultInfo);
                pMsgCon->setProgress(100);
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }
        }
    } // if( !isAsynchronousRequest(i_pReq) )

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeSelectDataRequest

//------------------------------------------------------------------------------
void CDbClient::executeUpdateDataRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( m_pReqDb == nullptr )
        {
            strAddTrcInfo += ", ReqDb: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
            strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeUpdateDataRequest",
        /* strAddInfo   */ strAddTrcInfo );

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

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    qint64 iReqIdParent = i_pReq->getParentId();

    // If the client is connected ...
    if( m_state >= EStateConnected )
    {
        QObject* pMsgReqReceiver = nullptr;

        if( m_socketTypeCurr == Ipc::ESocketTypeUndefined )
        {
            // No Gateway needed. Messages are directly exchanged between the database and the client.
            if( m_pDb == nullptr )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultRequestRefused);
                errResultInfo.setAddErrInfoDscr( "Reference to database object not defined." );
            }
            else
            {
                pMsgReqReceiver = m_pDb;
            }
        }
        else // if( getIpcSocketType(m_socketTypeCurr) != Ipc::ESocketTypeUndefined )
        {
            if( m_pDbCltGtw == nullptr )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultRequestRefused);
                errResultInfo.setAddErrInfoDscr( "Gateway is not created." );
            }
            else
            {
                pMsgReqReceiver = m_pDbCltGtw;
            }
        }

        if( !errResultInfo.isErrorResult() )
        {
            CMsgReqUpdateData* pMsgReq = new CMsgReqUpdateData(
                /* pObjSender       */ this,
                /* pObjReceiver     */ pMsgReqReceiver,
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
                    errResultInfo.setAddErrInfoDscr( "Waiting for update data confirmation timed out." );
                }
                if( errResultInfo.getResult() == EResultUndefined )
                {
                    errResultInfo.setErrResult(ErrResultSuccess);
                }
                i_pReq->setErrResultInfo(errResultInfo);
                i_pReq->setProgressInPerCent(100);
                i_pReq->update();
                deleteResultConfirmationMessage(i_pReq, this);
            }
            else // if( !i_pReq->isBlockingRequest() )
            {
                if( i_pReq->getTimeoutInMs() > 0 )
                {
                    m_reqDscrTimeout = i_pReq->getDescriptor();
                    m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
                }
            }
        } // if( !errResultInfo.isErrorResult() )
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
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();
    }

    if( !isAsynchronousRequest(i_pReq) )
    {
        if( m_requestQueue.isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
        {
            m_requestQueue.removeRequestInProgress( i_pReq->getId() );
        }

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

        bool bIsWaiting = false;

        if( iReqIdParent >= 0 )
        {
            bIsWaiting = CRequestExecTree::GetInstance()->isWaiting(iReqIdParent);
        }

        if( bIsWaiting )
        {
            CRequestExecTree::GetInstance()->wake(iReqIdParent);
        }
        else
        {
            CMsgCon* pMsgCon = dynamic_cast<CMsgCon*>(i_pReq->takeInitiatorConfirmationMessage());

            if( pMsgCon != nullptr )
            {
                pMsgCon->setErrResultInfo(errResultInfo);
                pMsgCon->setProgress(100);
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }
        }
    } // if( !isAsynchronousRequest(i_pReq) )

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeUpdateDataRequest

/*==============================================================================
protected: // internal request sequencer methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClient::startGatewayThread( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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

        if( m_pReqDb == nullptr )
        {
            strAddTrcInfo += ", ReqDb: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
            strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startGatewayThread",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("startGatewayThread");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqStartThread = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbClient",
        /* iRequest     */ ERequestStartThread,
        /* strRequest   */ Request2Str(ERequestStartThread),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    if( m_socketTypeNew != Ipc::ESocketTypeUndefined )
    {
        m_socketTypeCurr = m_socketTypeNew;
        m_socketTypeNew = Ipc::ESocketTypeUndefined;
    }

    if( m_socketTypeCurr == Ipc::ESocketTypeUndefined )
    {
        // No Gateway needed. Messages are directly exchanged between the database and the client.
        pReqStartThread->setErrResultInfo(errResultInfo);
        pReqStartThread->setProgressInPerCent(100);
        pReqStartThread->update();

    } // if( getIpcSocketType(m_socketTypeCurr) == Ipc::ESocketTypeUndefined )

    else // if( getIpcSocketType(m_socketTypeCurr) != Ipc::ESocketTypeUndefined )
    {
        if( !QObject::connect(
            /* pObjSender   */ pReqStartThread,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        m_requestQueue.addRequestInProgress(pReqStartThread);

        m_pDbCltGtwThread = new CDbClientGatewayThread(
            /* strObjName */ objectName() + "Gateway",
            /* pDbClient  */ this );

        QMutex         mtx;
        QWaitCondition waitCond;
        mtx.lock();
        m_pDbCltGtwThread->start(&waitCond);
        waitCond.wait(&mtx);
        mtx.unlock();

        m_pDbCltGtw = m_pDbCltGtwThread->getGateway();

        // StartThread is an internal sequencer request. No one else may have
        // signal/slot connections to this request. So there is no need to lock
        // the queue on updating this request (but its necessary to update the
        // request to indicate the progress in the request execution tree).
        m_requestQueue.removeRequestInProgress( pReqStartThread->getId() );
        //m_requestQueue.lock();
        pReqStartThread->setErrResultInfo(errResultInfo);
        pReqStartThread->setProgressInPerCent(100);
        pReqStartThread->update();
        //m_requestQueue.unlock();

    } // if( getIpcSocketType(m_socketTypeCurr) != Ipc::ESocketTypeUndefined )

    // Don't delete the request right now but return the request to the caller
    // as the caller may want to evaluate some details of the executed request.
    // E.g. if the request has been started by a request sequencer the sequencer
    // wants to update the request id.
    // The request will be set at the request queue to be deleted later
    // (on destroying the queue or if the next sync request will be set).

    // The request has been finished and will be deleted sometimes later.
    // This doesn't matter me ...
    QObject::disconnect(
        /* pObjSender   */ pReqStartThread,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

    m_requestQueue.setSyncRequestToBeDeletedLater(pReqStartThread);

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReqStartThread->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqStartThread;

} // startGatewayThread

//------------------------------------------------------------------------------
CRequest* CDbClient::stopGatewayThread( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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

        if( m_pReqDb == nullptr )
        {
            strAddTrcInfo += ", ReqDb: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
            strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "stopGatewayThread",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("stopGatewayThread");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqStopThread = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbClient",
        /* iRequest     */ ERequestStopThread,
        /* strRequest   */ Request2Str(ERequestStopThread),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    if( m_pDbCltGtwThread != nullptr )
    {
        if( !QObject::connect(
            /* pObjSender   */ pReqStopThread,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        m_requestQueue.addRequestInProgress(pReqStopThread);

        m_pDbCltGtwThread->quit();

        int iThreadWaitTimeout_ms = pReqStopThread->getTimeoutInMs();
        unsigned long uThreadWaitTimeout_ms = static_cast<unsigned long>(iThreadWaitTimeout_ms);

        if( iThreadWaitTimeout_ms <= 0 )
        {
            uThreadWaitTimeout_ms = ULONG_MAX;
        }

        if( !m_pDbCltGtwThread->wait(uThreadWaitTimeout_ms) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultTimeout);
            errResultInfo.setAddErrInfoDscr( "Waiting for StopThread timed out." );

            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
            m_pDbCltGtwThread->deleteLater();
            m_pDbCltGtwThread = nullptr;
        }
        else
        {
            try
            {
                delete m_pDbCltGtwThread;
            }
            catch(...)
            {
            }
            m_pDbCltGtwThread = nullptr;
        }

        m_requestQueue.removeRequestInProgress( pReqStopThread->getId() );

    } // if( m_pDbCltGtwThread != nullptr )

    m_pDbCltGtw = nullptr;

    // StopThread is an internal sequencer request. No one else may have
    // signal/slot connections to this request. So there is no need to lock
    // the queue on updating this request (but its necessary to update the
    // request to indicate the progress in the request execution tree).
    //m_requestQueue.lock();
    pReqStopThread->setErrResultInfo(errResultInfo);
    pReqStopThread->setProgressInPerCent(100);
    pReqStopThread->update();
    //m_requestQueue.unlock();

    // Don't delete the request right now but return the request to the caller
    // as the caller may want to evaluate some details of the executed request.
    // E.g. if the request has been started by a request sequencer the sequencer
    // wants to update the request id.
    // The request will be set at the request queue to be deleted later
    // (on destroying the queue or if the next sync request will be set).

    // The request has been finished and will be deleted sometimes later.
    // This doesn't matter me ...
    QObject::disconnect(
        /* pObjSender   */ pReqStopThread,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

    m_requestQueue.setSyncRequestToBeDeletedLater(pReqStopThread);

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReqStopThread->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqStopThread;

} // stopGatewayThread

/*==============================================================================
protected: // internal request sequencer methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClient::dbConnect( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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

        if( m_pReqDb == nullptr )
        {
            strAddTrcInfo += ", ReqDb: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
            strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dbConnect",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("dbConnect");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqDbConnect = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbClient",
        /* iRequest     */ ERequestDbConnect,
        /* strRequest   */ Request2Str(ERequestDbConnect),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    QObject* pMsgReqReceiver = nullptr;

    if( m_socketTypeCurr == Ipc::ESocketTypeUndefined )
    {
        // No Gateway needed. Messages are directly exchanged between the database and the client.
        if( m_pDb == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Reference to database object not defined." );
        }
        else
        {
            pMsgReqReceiver = m_pDb;
        }
    }
    else // if( getIpcSocketType(m_socketTypeCurr) != Ipc::ESocketTypeUndefined )
    {
        if( m_pDbCltGtw == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Gateway is not created." );
        }
        else
        {
            pMsgReqReceiver = m_pDbCltGtw;
        }
    }

    if( errResultInfo.isErrorResult() )
    {
        pReqDbConnect->setErrResultInfo(errResultInfo);
        pReqDbConnect->setProgressInPerCent(100);
        pReqDbConnect->update();

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else // if( !errResultInfo.isErrorResult() )
    {
        if( !QObject::connect(
            /* pObjSender   */ pReqDbConnect,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        m_requestQueue.addRequestInProgress(pReqDbConnect);

        CMsgReqConnect* pMsgReq = new CMsgReqConnect(
            /* pObjSender       */ this,
            /* pObjReceiver     */ pMsgReqReceiver,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ pReqDbConnect->getId(),
            /* iMsgId           */ -1 );

        if( m_socketTypeCurr == Ipc::ESocketTypeUndefined )
        {
            pMsgReq->setDb(m_pDb);
        }
        else
        {
            pMsgReq->setHostSettings(m_hostSettings);
        }

        POST_OR_DELETE_MESSAGE(pMsgReq);
    }

    if( pReqDbConnect->isBlockingRequest() )
    {
        if( !pReqDbConnect->wait() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultTimeout);
            errResultInfo.setAddErrInfoDscr( "Waiting for connect confirmation timed out." );

            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        // DbConnect is an internal sequencer request. No one else may have
        // signal/slot connections to this request. So there is no need to lock
        // the queue on updating this request (but its necessary to update the
        // request to indicate the progress in the request execution tree).
        m_requestQueue.removeRequestInProgress( pReqDbConnect->getId() );
        //m_requestQueue.lock();
        pReqDbConnect->setErrResultInfo(errResultInfo);
        pReqDbConnect->setProgressInPerCent(100);
        pReqDbConnect->update();
        //m_requestQueue.unlock();
        deleteResultConfirmationMessage(pReqDbConnect, this);

        // If the database client has been connected ..
        if( !errResultInfo.isErrorResult() )
        {
            EState statePrev = m_state;
            m_state = EStateConnected;
            emit stateChanged( m_state, statePrev );
        }

        // If the database client has not been connected ..
        else // if( errResultInfo.isErrorResult() )
        {
            if( m_pDbCltGtwThread != nullptr )
            {
                // .. stop thread again.
                CRequest* pReqStopThread = m_requestQueue.createRequest(
                    /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
                    /* strClassName */ "CDbClient",
                    /* iRequest     */ ERequestStopThread,
                    /* strRequest   */ Request2Str(ERequestStopThread),
                    /* iReqIdParent */ i_iReqIdParent,
                    /* iTimeout_ms  */ i_iTimeout_ms,
                    /* bIsBlocking  */ i_bWait );

                m_requestQueue.addRequestInProgress(pReqStopThread);

                m_pDbCltGtwThread->quit();

                int iThreadWaitTimeout_ms = pReqStopThread->getTimeoutInMs();
                unsigned long uThreadWaitTimeout_ms = static_cast<unsigned long>(iThreadWaitTimeout_ms);

                if( iThreadWaitTimeout_ms <= 0 )
                {
                    uThreadWaitTimeout_ms = ULONG_MAX;
                }
                if( !m_pDbCltGtwThread->wait(uThreadWaitTimeout_ms) )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultTimeout);
                    errResultInfo.setAddErrInfoDscr( "Waiting for StopThread timed out." );

                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                    m_pDbCltGtwThread->deleteLater();
                    m_pDbCltGtwThread = nullptr;
                }
                else
                {
                    try
                    {
                        delete m_pDbCltGtwThread;
                    }
                    catch(...)
                    {
                    }
                    m_pDbCltGtwThread = nullptr;
                }

                m_pDbCltGtw = nullptr;

                // StopThread is an internal request. No one else may have
                // signal/slot connections to this request. So there is no need to lock
                // the queue on updating this request (but its necessary to update the
                // request to indicate the progress in the request execution tree).
                m_requestQueue.removeRequestInProgress( pReqStopThread->getId() );
                //m_requestQueue.lock();
                pReqStopThread->setErrResultInfo(errResultInfo);
                pReqStopThread->setProgressInPerCent(100);
                pReqStopThread->update();
                //m_requestQueue.unlock();
                delete pReqStopThread;
                pReqStopThread = nullptr;

            } // if( m_pDbCltGtwThread != nullptr )
        } // if( errResultInfo.isErrorResult() )
    } // if( pReqDbConnect->isBlockingRequest() )

    else // if( !pReqDbConnect->isBlockingRequest() )
    {
        if( i_iTimeout_ms > 0 )
        {
            m_reqDscrTimeout = pReqDbConnect->getDescriptor();
            m_pTmrReqTimeout->start(i_iTimeout_ms);
        }
    }

    if( !isAsynchronousRequest(pReqDbConnect) )
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
            /* pObjSender   */ pReqDbConnect,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(pReqDbConnect);
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReqDbConnect->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqDbConnect;

} // dbConnect

//------------------------------------------------------------------------------
CRequest* CDbClient::dbDisconnect( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
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

        if( m_pReqDb == nullptr )
        {
            strAddTrcInfo += ", ReqDb: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
            strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dbDisconnect",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("dbDisconnect");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqDbDisconnect = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDbClient",
        /* iRequest     */ ERequestDbDisconnect,
        /* strRequest   */ Request2Str(ERequestDbDisconnect),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    QObject* pMsgReqReceiver = nullptr;

    if( m_socketTypeCurr == Ipc::ESocketTypeUndefined )
    {
        // No Gateway needed. Messages are directly exchanged between the database and the client.
        if( m_pDb == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Reference to database object not defined." );
        }
        else
        {
            pMsgReqReceiver = m_pDb;
        }
    }
    else // if( getIpcSocketType(m_socketTypeCurr) != Ipc::ESocketTypeUndefined )
    {
        if( m_pDbCltGtw == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Gateway is not created." );
        }
        else
        {
            pMsgReqReceiver = m_pDbCltGtw;
        }
    }

    if( errResultInfo.isErrorResult() )
    {
        pReqDbDisconnect->setErrResultInfo(errResultInfo);
        pReqDbDisconnect->setProgressInPerCent(100);
        pReqDbDisconnect->update();

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else // if( !errResultInfo.isErrorResult() )
    {
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
            /* pObjReceiver     */ pMsgReqReceiver,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ pReqDbDisconnect->getId(),
            /* iMsgId           */ -1 );

        POST_OR_DELETE_MESSAGE(pMsgReq);
    }

    if( pReqDbDisconnect->isBlockingRequest() )
    {
        if( !pReqDbDisconnect->wait() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultTimeout);
            errResultInfo.setAddErrInfoDscr( "Waiting for disconnect confirmation timed out." );

            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        // DbDisconnect is an internal sequencer request. No one else may have
        // signal/slot connections to this request. So there is no need to lock
        // the queue on updating this request (but its necessary to update the
        // request to indicate the progress in the request execution tree).
        m_requestQueue.removeRequestInProgress( pReqDbDisconnect->getId() );
        //m_requestQueue.lock();
        pReqDbDisconnect->setErrResultInfo(errResultInfo);
        pReqDbDisconnect->setProgressInPerCent(100);
        pReqDbDisconnect->update();
        //m_requestQueue.unlock();
        deleteResultConfirmationMessage(pReqDbDisconnect, this);

        // If the database client has been disconnected ..
        if( !errResultInfo.isErrorResult() )
        {
            EState statePrev = m_state;
            m_state = EStateDisconnected;
            emit stateChanged( m_state, statePrev );
        }
        // If the database client has not been disconnected ..
        else // if( errResultInfo.isErrorResult() )
        {
            // .. remain in connected state. But this should never happen !!
        }
    } // if( pReqDbDisconnect->isBlockingRequest() )

    else // if( !pReqDbDisconnect->isBlockingRequest() )
    {
        if( i_iTimeout_ms > 0 )
        {
            m_reqDscrTimeout = pReqDbDisconnect->getDescriptor();
            m_pTmrReqTimeout->start(i_iTimeout_ms);
        }
    }

    if( !isAsynchronousRequest(pReqDbDisconnect) )
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
            /* pObjSender   */ pReqDbDisconnect,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(pReqDbDisconnect);
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReqDbDisconnect->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqDbDisconnect;

} // dbDisconnect

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::onRequestTimeout()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( m_pReqDb == nullptr )
        {
            strAddTrcInfo += ", ReqDb: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
            strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
        }
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
void CDbClient::onRequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo  = "ReqDscr: " + i_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( m_pReqDb == nullptr )
        {
            strAddTrcInfo += ", ReqDb: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
            strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRequestChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_requestQueue.isRequestInProgress(i_reqDscr.m_iId) )
    {
        CRequest* pReq = m_requestQueue.getRequestInProgress(i_reqDscr.m_iId);

        CRequest* pReqMainInProgress = m_requestQueue.getRequestInProgress();
        bool      bIsMainRequest = false;

        if( pReq == pReqMainInProgress )
        {
            bIsMainRequest = true;
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

        qint64 iReqIdParent = i_reqDscr.m_iParentId;

        if( pReqMainInProgress != nullptr )
        {
            iReqIdParent = pReqMainInProgress->getParentId();
        }

        bool bIsWaitingRequest = false;

        if( iReqIdParent >= 0 )
        {
            bIsWaitingRequest = CRequestExecTree::GetInstance()->isWaiting(iReqIdParent);
        }

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

        } // if( i_reqDscr.m_iProgress_perCent >= 100 )

        CMsgCon* pMsgConInitiator = nullptr;

        // If the request has not yet been finished ...
        if( i_reqDscr.m_iProgress_perCent < 100 )
        {
            if( bIsMainRequest && !bIsWaitingRequest )
            {
                pMsgConInitiator = pReq->getInitiatorConfirmationMessage();

                // Send intermediate results to the initiator of the request.
                if( pMsgConInitiator != nullptr )
                {
                    pMsgConInitiator = dynamic_cast<CMsgCon*>(pMsgConInitiator->clone());
                    pMsgConInitiator->setErrResultInfo(i_reqDscr.m_errResultInfo);
                    pMsgConInitiator->setProgress(i_reqDscr.m_iProgress_perCent);
                }
            } // if( bIsMainRequest && !bIsWaitingRequest )
        } // if( i_reqDscr.m_iProgress_perCent < 100 )

        // If the request has been finished ..
        else if( i_reqDscr.m_iProgress_perCent >= 100 )
        {
            if( bIsMainRequest && !bIsWaitingRequest )
            {
                pMsgConInitiator = pReq->takeInitiatorConfirmationMessage();

                // Send final results to the initiator of the request.
                if( pMsgConInitiator != nullptr )
                {
                    pMsgConInitiator->setErrResultInfo(errResultInfo);
                    pMsgConInitiator->setProgress(i_reqDscr.m_iProgress_perCent);
                }
            }
        } // if( i_reqDscr.m_iProgress_perCent >= 100 )

        switch( i_reqDscr.m_request )
        {
            case Database::ERequestConnect:
            {
                // If the request has been finished ..
                if( i_reqDscr.m_iProgress_perCent >= 100 )
                {
                    if( !errResultInfo.isErrorResult() )
                    {
                        // Connecting the client is a sequence of several requests.
                        // Even if some requests fail the client might be connected.
                        // E.g. if "loadData" failed the client is connected with the
                        // database but may not contain valid (all) data.
                        // Please note that the state has already been changed if the
                        // DbConnect sub request has been successfully finished.

                    } // if( !errResultInfo.isErrorResult() )
                } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                break;
            } // case Database::ERequestConnect

            case Database::ERequestDisconnect:
            {
                // If the request has been finished ..
                if( i_reqDscr.m_iProgress_perCent >= 100 )
                {
                    if( !errResultInfo.isErrorResult() )
                    {
                        // Please note that the state has already been changed if the DbDisonnect sub request
                        // has been finished.

                    } // if( !errResultInfo.isErrorResult() )
                } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                break;
            } // case Database::ERequestDisconnect

            case Database::ERequestRegister:
            {
                // If the request has been finished ..
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
                    } // if( !errResultInfo.isErrorResult() )
                } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                break;
            } // case Database::ERequestRegister

            case Database::ERequestUnregister:
            {
                // If the request has been finished ..
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
                    } // if( !errResultInfo.isErrorResult() )
                } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                break;
            } // case Database::ERequestUnregister

            case Database::ERequestSelectSchema:
            case Database::ERequestUpdateSchema:
            case Database::ERequestSelectData:
            case Database::ERequestUpdateData:
            {
                break;
            }

            case Database::ERequestStartThread:
            {
                if( pReqMainInProgress == nullptr )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError, "StartThread must be Child of the Main Connect Request" );
                }

                if( errResultInfo.isErrorResult() )
                {
                    if( m_pDbCltGtwThread != nullptr )
                    {
                        m_pDbCltGtwThread->quit();

                        if( !m_pDbCltGtwThread->wait(5000) )
                        {
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultTimeout);
                            errResultInfo.setAddErrInfoDscr( "Waiting for StopThread timed out." );

                            if( CErrLog::GetInstance() != nullptr )
                            {
                                CErrLog::GetInstance()->addEntry(errResultInfo);
                            }
                            m_pDbCltGtwThread->deleteLater();
                            m_pDbCltGtwThread = nullptr;
                        }
                        else
                        {
                            try
                            {
                                delete m_pDbCltGtwThread;
                            }
                            catch(...)
                            {
                            }
                            m_pDbCltGtwThread = nullptr;
                        }
                    } // if( m_pDbCltGtwThread != nullptr )

                    m_pDbCltGtw = nullptr;

                } // if( errResultInfo.isErrorResult() )

                // The request sequencer will call the next request method.
                break;
            } // case Database::ERequestStartThread

            case Database::ERequestStopThread:
            {
                if( pReqMainInProgress == nullptr )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError, "StopThread must be Child of the Main Disconnect Request" );
                }
                // The request sequencer will call the next request method.
                break;
            } // case Database::ERequestStopThread

            case Database::ERequestDbConnect:
            {
                if( pReqMainInProgress == nullptr )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError, "DbConnect must be Child of the Main Connect Request" );
                }

                // If the sub request has been finished ..
                if( i_reqDscr.m_iProgress_perCent >= 100 )
                {
                    if( errResultInfo.isErrorResult() )
                    {
                        if( m_pDbCltGtwThread != nullptr )
                        {
                            m_pDbCltGtwThread->quit();

                            if( !m_pDbCltGtwThread->wait(5000) )
                            {
                                errResultInfo.setSeverity(EResultSeverityError);
                                errResultInfo.setResult(EResultTimeout);
                                errResultInfo.setAddErrInfoDscr( "Waiting for StopThread timed out." );

                                if( CErrLog::GetInstance() != nullptr )
                                {
                                    CErrLog::GetInstance()->addEntry(errResultInfo);
                                }
                                m_pDbCltGtwThread->deleteLater();
                                m_pDbCltGtwThread = nullptr;
                            }
                            else
                            {
                                try
                                {
                                    delete m_pDbCltGtwThread;
                                }
                                catch(...)
                                {
                                }
                                m_pDbCltGtwThread = nullptr;
                            }
                        } // if( m_pDbCltGtwThread != nullptr )

                        m_pDbCltGtw = nullptr;

                    } // if( errResultInfo.isErrorResult() )

                    else // if( !errResultInfo.isErrorResult() )
                    {
                        // Client has been connected.
                        if( m_state != EStateConnected )
                        {
                            EState statePrev = m_state;
                            m_state = EStateConnected;
                            emit stateChanged( m_state, statePrev );
                        }
                    } // if( !errResultInfo.isErrorResult() )
                } // if( i_reqDscr.m_iProgress_perCent >= 100 )

                // The request sequencer will call the next request method.
                break;
            } // case Database::ERequestDbConnect

            case Database::ERequestDbDisconnect:
            {
                if( pReqMainInProgress == nullptr )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError, "DbDisconnect must be Child of the Main Disconnect Request" );
                }

                // If the sub request has been finished ..
                if( i_reqDscr.m_iProgress_perCent >= 100 )
                {
                    if( !errResultInfo.isErrorResult() )
                    {
                        // Client has been disconnected.
                        if( m_state != EStateDisconnected )
                        {
                            EState statePrev = m_state;
                            m_state = EStateDisconnected;
                            emit stateChanged( m_state, statePrev );
                        }
                    } // if( !errResultInfo.isErrorResult() )
                } // if( i_reqDscr.m_iProgress_perCent >= 100 )

                // The request sequencer will call the next request method.
                break;
            } // case Database::ERequestDbDisconnect

            default:
            {
                break;
            }

        } // switch( i_reqDscr.m_request )

        // If the main request has been finished ...
        if( i_reqDscr.m_iProgress_perCent >= 100 && bIsMainRequest )
        {
            // The main request has been finished. The main request has been created by me
            // on starting the sequence. Usually the one finishing and updating the request
            // should delete the request. This woud be the request sequencer which got the
            // main request as the parent request (beoming its root entry). The sequencer
            // does not delete its parent request as the sequencer is not the owner of the
            // request. The main request got to be deleted by me. But not now as after this
            // "onRequestChanged" slot further "onRequestChanged" slots of other classes may
            // be called. So we don't delete the request right now but the request will be
            // set at the request queue to be deleted later (on destroying the queue or if
            // the next sync request will be set).
            if( !pReq->isLocked() )
            {
                // The request has been finished and will be deleted sometimes later.
                // This doesn't matter me ...
                QObject::disconnect(
                    /* pObjSender   */ pReq,
                    /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

                m_requestQueue.setSyncRequestToBeDeletedLater(pReq);
            }
            if( bIsWaitingRequest )
            {
                CRequestExecTree::GetInstance()->wake(iReqIdParent);
            }
        } // if( i_reqDscr.m_iProgress_perCent >= 100 && bIsMainRequest )

        if( pMsgConInitiator != nullptr )
        {
            POST_OR_DELETE_MESSAGE(pMsgConInitiator);
            pMsgConInitiator = nullptr;
        }

        m_requestQueue.unlock();

        if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
        {
            executeNextPostponedRequest();
        }
    } // if( m_requestQueue.isRequestInProgress(i_reqDscr.m_iId) )

} // onRequestChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::onDbDestroyed( QObject* /*i_pDb*/ )
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

    try
    {
        delete m_pReqDb;
    }
    catch(...)
    {
    }
    m_pReqDb = nullptr;
    m_iReqMsgIdDb = -1;
    m_iReqIdDb = -1;

    // If using a gateway (socket connection to database) the disconnected indication message
    // will be sent by the gateway if the client is currently connected with the database.
    // If not using a gateway the indication message will not be received.
    if( m_pDbCltGtw == nullptr )
    {
        m_requestQueue.lock();

        if( m_requestQueue.isRequestInProgress() )
        {
            CRequest* pReq = m_requestQueue.getRequestInProgress();

            SErrResultInfo errResultInfo = pReq->getErrResultInfo();

            switch( pReq->getRequest() )
            {
                case ERequestConnect:
                case ERequestDisconnect:
                case ERequestRegister:
                case ERequestUnregister:
                case ERequestSelectSchema:
                case ERequestUpdateSchema:
                case ERequestSaveSchema:
                case ERequestSelectData:
                case ERequestUpdateData:
                case ERequestSaveData:
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultRequestAborted);
                    errResultInfo.setAddErrInfoDscr("Connection to database has been lost");
                    pReq->lock();
                    pReq->setErrResultInfo(errResultInfo);
                    pReq->setProgressInPerCent(100);
                    pReq->update();
                    pReq->unlock();
                    deleteRequest(pReq);
                    pReq = nullptr;
                    break;
                }
                default:
                {
                    break;
                }
            } // switch( pReq->getRequest() )
        } // if( m_requestQueue.isRequestInProgress() )

        if( m_state >= EStateConnected )
        {
            EState statePrev = m_state;
            m_state = EStateDisconnected;
            emit stateChanged( m_state, statePrev );
        }

        m_requestQueue.unlock();

    } // if( m_pDbCltGtw == nullptr )

} // onDbDestroyed

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CDbClient::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    CMsg* pMsg = dynamic_cast<CMsg*>(i_pEv);

    if( pMsg != nullptr )
    {
        QString strAddTrcInfo;

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
        {
            int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

            strAddTrcInfo  = "Msg: " + pMsg->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", State: " + State2Str(m_state);
            strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);

            if( m_pReqDb == nullptr )
            {
                strAddTrcInfo += ", ReqDb: nullptr";
            }
            else
            {
                strAddTrcInfo += ", ReqDb: " + m_pReqDb->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
                strAddTrcInfo += ", ReqMsgIdDb: " + QString::number(m_iReqMsgIdDb);
                strAddTrcInfo += ", ReqIdDb: " + QString::number(m_iReqIdDb);
            }
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDetailLevel */ ETraceDetailLevelMethodCalls,
            /* strMethod    */ "event",
            /* strAddInfo   */ strAddTrcInfo );

        if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq )
        {
            CMsgReq* pMsgReqInitiator = dynamic_cast<CMsgReq*>(pMsg);
            if( pMsgReqInitiator == nullptr )
            {
                throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
            }

            SErrResultInfo errResultInfo = ErrResultInfoSuccess(pMsgReqInitiator->msgTypeToStr());

            CRequest*      pReq             = nullptr;
            qint64         iReqIdParent     = pMsgReqInitiator->getRequestId();
            CMsgCon*       pMsgConInitiator = nullptr;

            switch( static_cast<int>(pMsg->type()) )
            {
                case EMsgTypeReqChangeSettings:
                {
                    // Someone else sent the request message to the client.
                    // E.g. if the client is hosted in the DbClientThread and if the
                    // corresponding request method of the DbClientThread was called.
                    CMsgReqChangeSettings* pMsgReqChangeSettings = dynamic_cast<CMsgReqChangeSettings*>(pMsgReqInitiator);
                    if( pMsgReqChangeSettings == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }

                    Ipc::SClientHostSettings hostSettings = pMsgReqChangeSettings->getClientHostSettings();
                    m_socketTypeNew = hostSettings.m_socketType;

                    if( pMsgReqChangeSettings->mustBeConfirmed() )
                    {
                        CMsgConChangeSettings* pMsgConChangeSettings = dynamic_cast<CMsgConChangeSettings*>(pMsgReqInitiator->createConfirmationMessage());
                        pMsgConChangeSettings->setSocketDscr( pMsgReqChangeSettings->getClientHostSettings().getSocketDscr() );
                        pMsgConInitiator = pMsgConChangeSettings;
                    }
                    pReq = changeSettings( 0, false, iReqIdParent );
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqChangeSettings

                case EMsgTypeReqConnect:
                {
                    // Someone else sent the request message to the client.
                    // E.g. if the client is hosted in the DbClientThread and if the
                    // corresponding request method of the DbClientThread was called.
                    CMsgReqConnect* pMsgReqConnect = dynamic_cast<CMsgReqConnect*>(pMsgReqInitiator);
                    if( pMsgReqConnect == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqConnect->mustBeConfirmed() )
                    {
                        CMsgConConnect* pMsgConConnect = dynamic_cast<CMsgConConnect*>(pMsgReqInitiator->createConfirmationMessage());
                        pMsgConConnect->setSocketDscr( pMsgReqConnect->getHostSettings().getSocketDscr() );
                        pMsgConInitiator = pMsgConConnect;
                    }
                    pReq = connect_(0, false, iReqIdParent);
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqConnect

                case EMsgTypeReqDisconnect:
                {
                    // Someone else sent the request message to the client.
                    // E.g. if the client is hosted in the DbClientThread and if the
                    // corresponding request method of the DbClientThread was called.
                    CMsgReqDisconnect* pMsgReqDisconnect = dynamic_cast<CMsgReqDisconnect*>(pMsgReqInitiator);
                    if( pMsgReqDisconnect == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqDisconnect->mustBeConfirmed() )
                    {
                        CMsgConDisconnect* pMsgConDisconnect = dynamic_cast<CMsgConDisconnect*>(pMsgReqInitiator->createConfirmationMessage());
                        pMsgConDisconnect->setSocketDscr( pMsgReqDisconnect->getSocketDscr() );
                        pMsgConInitiator = pMsgConDisconnect;
                    }
                    pReq = disconnect_( 0, false, iReqIdParent );
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqDisconnect:

                case EMsgTypeReqRegister:
                {
                    // Someone else sent the request message to the client.
                    // E.g. if the client is hosted in the DbClientThread and if the
                    // corresponding request method of the DbClientThread was called.
                    CMsgReqRegister* pMsgReqRegister = dynamic_cast<CMsgReqRegister*>(pMsgReqInitiator);
                    if( pMsgReqRegister == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqRegister->mustBeConfirmed() )
                    {
                        CMsgConRegister* pMsgConRegister = dynamic_cast<CMsgConRegister*>(pMsgReqInitiator->createConfirmationMessage());
                        pMsgConInitiator = pMsgConRegister;
                    }
                    pReq = register_( 0, false, iReqIdParent );
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqRegister

                case EMsgTypeReqUnregister:
                {
                    // Someone else sent the request message to the client.
                    // E.g. if the client is hosted in the DbClientThread and if the
                    // corresponding request method of the DbClientThread was called.
                    CMsgReqUnregister* pMsgReqUnregister = dynamic_cast<CMsgReqUnregister*>(pMsgReqInitiator);
                    if( pMsgReqUnregister == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqUnregister->mustBeConfirmed() )
                    {
                        CMsgConUnregister* pMsgConUnregister = dynamic_cast<CMsgConUnregister*>(pMsgReqInitiator->createConfirmationMessage());
                        pMsgConInitiator = pMsgConUnregister;
                    }
                    pReq = unregister( 0, false, iReqIdParent );
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqUnregister

                case EMsgTypeReqSelectSchema:
                {
                    // Someone else sent the request message to the client.
                    // E.g. if the client is hosted in the DbClientThread and if the
                    // corresponding request method of the DbClientThread was called.
                    CMsgReqSelectSchema* pMsgReqSelectSchema = dynamic_cast<CMsgReqSelectSchema*>(pMsgReqInitiator);
                    if( pMsgReqSelectSchema == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqSelectSchema->mustBeConfirmed() )
                    {
                        CMsgConSelectSchema* pMsgConSelectSchema = dynamic_cast<CMsgConSelectSchema*>(pMsgReqInitiator->createConfirmationMessage());
                        pMsgConInitiator = pMsgConSelectSchema;
                    }
                    pReq = selectSchema( 0, false, iReqIdParent );
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqSelectSchema

                case EMsgTypeReqUpdateSchema:
                {
                    // This request message may be sent by someone else to the client.
                    // E.g. if the client is hosted in the DbClientThread and if the
                    // corresponding request method of the DbClientThread was called.
                    // But this request message may have also been sent from the client's
                    // gateway if data in the database has been changed.
                    CMsgReqUpdateSchema* pMsgReqUpdateSchema = dynamic_cast<CMsgReqUpdateSchema*>(pMsgReqInitiator);
                    if( pMsgReqUpdateSchema == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqUpdateSchema->mustBeConfirmed() )
                    {
                        CMsgConUpdateSchema* pMsgConUpdateSchema = dynamic_cast<CMsgConUpdateSchema*>(pMsgReqInitiator->createConfirmationMessage());
                        pMsgConInitiator = pMsgConUpdateSchema;
                    }

                    // Request sent from the database or from this client's gateway
                    // (request message originally sent from database and forwarded by the gateway to this client) ..
                    if( pMsgReqUpdateSchema->getSender() == m_pDb || pMsgReqUpdateSchema->getSender() == m_pDbCltGtw )
                    {
                        // .. it is a request which cannot be queued as otherwise we may end up in a deadlock
                        // (the client may be waiting for a confirmation message from the database while the
                        // database is waiting for a confirmation message from the client).
                        // The gateway will already refuse requests if the client is busy with requests received from
                        // the database and the following check is not really necessary but should clarify the facts.
                        if( m_iReqMsgIdDb != -1 || m_iReqIdDb != -1 || m_pReqDb != nullptr )
                        {
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultRequestRefused);
                            errResultInfo.setAddErrInfoDscr( "Client is busy" );

                            pReq->setErrResultInfo(errResultInfo);
                            pReq->setProgressInPerCent(100);
                            pReq->update();
                        }
                        else
                        {
                            pReq = onReceivedMsgReqUpdateSchema(pMsgReqUpdateSchema);
                        }
                    }

                    // Request sent from someone else
                    // (usually sent by calling a method of the DbClientThread instance) ..
                    else // if( pMsgReq->getSender() != m_pDb && pMsgReq->getSender() != m_pDbCltGtw )
                    {
                        // .. it is a request which might be queued if currently a request is in progress.
                        pReq = updateSchema( 0, false, iReqIdParent );
                    }
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqUpdateSchema

                case EMsgTypeReqSelectData:
                {
                    // Someone else sent the request message to the client.
                    // E.g. if the client is hosted in the DbClientThread and if the
                    // corresponding request method of the DbClientThread was called.
                    CMsgReqSelectData* pMsgReqSelectData = dynamic_cast<CMsgReqSelectData*>(pMsgReqInitiator);
                    if( pMsgReqSelectData == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqSelectData->mustBeConfirmed() )
                    {
                        CMsgConSelectData* pMsgConSelectData = dynamic_cast<CMsgConSelectData*>(pMsgReqInitiator->createConfirmationMessage());
                        pMsgConInitiator = pMsgConSelectData;
                    }
                    pReq = selectData( 0, false, iReqIdParent );
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqSelectData

                case EMsgTypeReqUpdateData:
                {
                    // This request message may be sent by someone else to the client.
                    // E.g. if the client is hosted in the DbClientThread and if the
                    // corresponding request method of the DbClientThread was called.
                    // But this request message may have also been sent from the client's
                    // gateway if data in the database has been changed.
                    CMsgReqUpdateData* pMsgReqUpdateData = dynamic_cast<CMsgReqUpdateData*>(pMsgReqInitiator);
                    if( pMsgReqUpdateData == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqUpdateData->mustBeConfirmed() )
                    {
                        CMsgConUpdateData* pMsgConUpdateData = dynamic_cast<CMsgConUpdateData*>(pMsgReqInitiator->createConfirmationMessage());
                        pMsgConInitiator = pMsgConUpdateData;
                    }

                    // Request sent from the database or from this client's gateway
                    // (request message originally sent from database and forwarded by the gateway to this client) ..
                    if( pMsgReqUpdateData->getSender() == m_pDb || pMsgReqUpdateData->getSender() == m_pDbCltGtw )
                    {
                        // .. it is a request which cannot be queued as otherwise we may end up in a deadlock
                        // (the client may be waiting for a confirmation message from the database while the
                        // database is waiting for a confirmation message from the client).
                        // The gateway will already refuse requests if the client is busy with requests received from
                        // the database and the following check is not really necessary but should clarify the facts.
                        if( m_iReqMsgIdDb != -1 || m_iReqIdDb != -1 || m_pReqDb != nullptr )
                        {
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultRequestRefused);
                            errResultInfo.setAddErrInfoDscr( "Client is busy" );
                        }
                        else
                        {
                            pReq = onReceivedMsgReqUpdateData(pMsgReqUpdateData);
                        }
                    }

                    // Request sent from someone else
                    // (usually sent by calling a method of the DbClientThread instance) ..
                    else // if( pMsgReq->getSender() != m_pDb && pMsgReq->getSender() != m_pDbCltGtw )
                    {
                        // .. it is a request which might be queued if currently a request is in progress.
                        pReq = updateData( 0, false, iReqIdParent );
                    }
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqUpdateData

                case EBaseMsgTypeReqContinue:
                {
                    m_bMsgReqContinuePending = false;

                    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
                    {
                        CRequest* pReqPostponed = m_requestQueue.takeFirstPostponedRequest();

                        if( pReqPostponed != nullptr )
                        {
                            switch( pReqPostponed->getRequest() )
                            {
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
                                case ERequestSelectSchema:
                                {
                                    executeSelectSchemaRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestUpdateSchema:
                                {
                                    executeUpdateSchemaRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestSelectData:
                                {
                                    executeSelectDataRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestUpdateData:
                                {
                                    executeUpdateDataRequest(pReqPostponed);
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

                    } // if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )

                    bEventHandled = true;
                    break;

                } // case EBaseMsgTypeReqContinue

                default:
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInvalidMessage);
                    errResultInfo.setAddErrInfoDscr( pMsg->msgTypeToStr() );

                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                    else
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, errResultInfo );
                    }
                    break;
                }
            } // switch( pMsg->type() )

            if( isAsynchronousRequest(pReq) )
            {
                pReq->setInitiatorConfirmationMessage(pMsgConInitiator);
                pMsgConInitiator = nullptr;
            }
            else // if( !isAsynchronousRequest(pReq) )
            {
                bool bIsWaiting = false;

                if( iReqIdParent >= 0 )
                {
                    bIsWaiting = CRequestExecTree::GetInstance()->isWaiting(iReqIdParent);
                }

                if( bIsWaiting != 0 )
                {
                    CRequestExecTree::GetInstance()->wake(iReqIdParent);
                }
                else if( pMsgConInitiator != nullptr )
                {
                    if( pReq != nullptr )
                    {
                        errResultInfo = pReq->getErrResultInfo();
                    }
                    else if( errResultInfo.getResult() == EResultUndefined )
                    {
                        errResultInfo.setResult(EResultSuccess);
                    }
                    pMsgConInitiator->setErrResultInfo(errResultInfo);
                    pMsgConInitiator->setProgress(100);

                    POST_OR_DELETE_MESSAGE(pMsgConInitiator);
                    pMsgConInitiator = nullptr;
                }

                if( pReq == m_pReqDb )
                {
                    m_iReqMsgIdDb = -1;
                    m_iReqIdDb = -1;
                    deleteRequest(m_pReqDb);
                    m_pReqDb = nullptr;
                }
            } // if( !isAsynchronousRequest(pReq) )

            delete pMsgConInitiator;
            pMsgConInitiator = nullptr;

        } // if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq )

        else if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeInd )
        {
            CMsgInd* pMsgIndInitiator = dynamic_cast<CMsgInd*>(pMsg);
            if( pMsgIndInitiator == nullptr )
            {
                throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
            }

            if( m_pDbCltGtw != nullptr && pMsgIndInitiator->getSender() != m_pDbCltGtw )
            {
                throw CException (__FILE__, __LINE__, EResultInvalidSenderObjInMessage );
            }

            // Please note that messages may be sent by the gateway right before the gateway
            // is destroyed. In this case the message may be received right after the gateway
            // has been destroyed, we can't access the message sender (the gateway) anymore
            // and also a dynamic_cast from the message sender to the gateway object will
            // result in an access violation exception.

            SErrResultInfo errResultInfo = ErrResultInfoSuccess(pMsgIndInitiator->msgTypeToStr());

            switch( static_cast<int>(pMsg->type()) )
            {
                case EMsgTypeIndDisconnected:
                {
                    CMsgIndDisconnected* pMsgIndDisconnected = dynamic_cast<CMsgIndDisconnected*>(pMsgIndInitiator);
                    if( pMsgIndDisconnected == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgIndDisconnected->getSender() == nullptr || pMsgIndDisconnected->getSender() != m_pDbCltGtw )
                    {
                        throw CException (__FILE__, __LINE__, EResultInvalidSenderObjInMessage );
                    }

                    m_requestQueue.lock();

                    if( m_requestQueue.isRequestInProgress() )
                    {
                        CRequest* pReq = m_requestQueue.getRequestInProgress();

                        switch( pReq->getRequest() )
                        {
                            case ERequestConnect:
                            case ERequestDisconnect:
                            case ERequestRegister:
                            case ERequestUnregister:
                            case ERequestSelectSchema:
                            case ERequestUpdateSchema:
                            case ERequestSaveSchema:
                            case ERequestSelectData:
                            case ERequestUpdateData:
                            case ERequestSaveData:
                            {
                                errResultInfo.setSeverity(EResultSeverityError);
                                errResultInfo.setResult(EResultRequestAborted);
                                errResultInfo.setAddErrInfoDscr("Connection to database has been lost");
                                pReq->lock();
                                pReq->setErrResultInfo(errResultInfo);
                                pReq->setProgressInPerCent(100);
                                pReq->update();
                                pReq->unlock();
                                deleteRequest(pReq);
                                pReq = nullptr;
                                break;
                            }
                            default:
                            {
                                break;
                            }
                        } // switch( pReq->getRequest() )
                    } // if( m_requestQueue.isRequestInProgress() )

                    if( m_state == EStateConnected )
                    {
                        EState statePrev = m_state;
                        m_state = EStateDisconnected;
                        emit stateChanged( m_state, statePrev );
                    }

                    if( m_pDbCltGtwThread != nullptr )
                    {
                        m_pDbCltGtwThread->quit();

                        if( !m_pDbCltGtwThread->wait(5000) )
                        {
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultTimeout);
                            errResultInfo.setAddErrInfoDscr( "Waiting for StopThread timed out." );

                            if( CErrLog::GetInstance() != nullptr )
                            {
                                CErrLog::GetInstance()->addEntry(errResultInfo);
                            }
                            m_pDbCltGtwThread->deleteLater();
                            m_pDbCltGtwThread = nullptr;
                        }
                        else
                        {
                            try
                            {
                                delete m_pDbCltGtwThread;
                            }
                            catch(...)
                            {
                            }
                            m_pDbCltGtwThread = nullptr;
                        }
                    } // if( m_pDbCltGtwThread != nullptr )

                    m_pDbCltGtw = nullptr;

                    m_requestQueue.unlock();

                    if( m_requestQueue.hasPostponedRequest() )
                    {
                        executeNextPostponedRequest();
                    }
                    bEventHandled = true;
                    break;

                } // case EMsgTypeIndDisconnected

                default:
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInvalidMessage);
                    errResultInfo.setAddErrInfoDscr( pMsg->msgTypeToStr() );

                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                    else
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, errResultInfo );
                    }
                    break;
                }
            } // switch( pMsg->type() )
        } // else if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeInd )

        else if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeCon )
        {
            CMsgCon* pMsgConExec = dynamic_cast<CMsgCon*>(pMsg);
            if( pMsgConExec == nullptr )
            {
                throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
            }

            // Direct connection to database ..
            if( m_socketTypeCurr == Ipc::ESocketTypeUndefined )
            {
                if( pMsgConExec->getSender() != m_pDb )
                {
                    throw CException (__FILE__, __LINE__, EResultInvalidSenderObjInMessage );
                }
            }
            // Connection to database via gateway ..
            else // if( m_socketTypeCurr != Ipc::ESocketTypeUndefined )
            {
                // .. the gateway is created in the context of the gateway thread and
                // if the thread has been started the startup confirmation message will
                // be sent providing the address of the gateway client.

                // Please note that messages may be sent by the gateway right before the gateway
                // is destroyed. In this case the message may be received right after the gateway
                // has been destroyed, we can't access the message sender (the gateway) anymore
                // and also a dynamic_cast from the message sender to the gateway object will
                // result in an access violation exception.

                if( pMsgConExec->getMsgType() == EMsgTypeConStartup )
                {
                    CMsgConStartup* pMsgConStartup = dynamic_cast<CMsgConStartup*>(pMsgConExec);
                    if( pMsgConStartup == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }

                    if( m_pDbCltGtwThread != nullptr )
                    {
                        if( m_pDbCltGtw == nullptr )
                        {
                            try
                            {
                                m_pDbCltGtw = dynamic_cast<CDbClientGateway*>(pMsgConExec->getSender());
                            }
                            catch(...)
                            {
                                m_pDbCltGtw = nullptr;
                            }
                        }
                    }
                } // if( pMsgConExec->getMsgType() == EMsgTypeConStartup )

                if( m_pDbCltGtw != nullptr && pMsgConExec->getSender() != m_pDbCltGtw )
                {
                    throw CException (__FILE__, __LINE__, EResultInvalidSenderObjInMessage );
                }
            } // if( getIpcSocketType(m_socketTypeCurr) != Ipc::ESocketTypeUndefined )

            CRequest* pReq = m_requestQueue.getRequestInProgress(pMsgConExec->getRequestId());

            if( pReq != nullptr )
            {
                SErrResultInfo errResultInfo = pMsgConExec->getErrResultInfo();
                int            iProgress_perCent = pMsgConExec->getProgress();

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

        } // if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeCon )
    } // if( pMsg != nullptr )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pEv);
    }
    return bEventHandled;

} // event
