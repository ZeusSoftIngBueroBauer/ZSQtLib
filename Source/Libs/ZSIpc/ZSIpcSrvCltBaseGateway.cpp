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
#include <QtCore/qdatetime.h>
#include <QtCore/qtimer.h>
#include <QtCore/qwaitcondition.h>

#include "ZSIpc/ZSIpcSrvCltBaseGateway.h"
#include "ZSIpc/ZSIpcBlkType.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequest.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;


/*******************************************************************************
class CSrvCltBaseGatewayThread : public QThread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CSrvCltBaseGatewayThread::CSrvCltBaseGatewayThread(
    const QString& i_strObjNameGateway,
    CErrLog*       i_pErrLog,
    EMethodTraceDetailLevel i_eTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    QThread(),
    m_strObjNameGateway(i_strObjNameGateway),
    m_pGateway(nullptr),
    m_pErrLog(i_pErrLog),
    m_iReqIdStartThread(-1),
    m_pWaitCondition(nullptr),
    m_eTrcMthFileDetailLevel(i_eTrcMthFileDetailLevel),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
{
    // Please note that thread names must be unique within the application.
    // The object name is the thread name. "Gateway" should be included in the thread name.
    if( !m_strObjNameGateway.contains("Gtw", Qt::CaseInsensitive) && !m_strObjNameGateway.contains("Gateway", Qt::CaseInsensitive) )
    {
        setObjectName(m_strObjNameGateway + "Gateway");
    }
    else
    {
        setObjectName(m_strObjNameGateway);
    }

    // The derived classes must instantiate the trace admin object and trace the ctor.
    //if( m_pTrcMthFile == nullptr && !i_strObjName.contains("TrcServer") && !i_strObjName.contains("TrcClient") )
    //{
    //    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    //}

    //CMethodTracer mthTracer(
    //    /* pAdminObj          */ m_pTrcAdminObj,
    //    /* pTrcMthFile        */ m_pTrcMthFile,
    //    /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
    //    /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
    //    /* strNameSpace       */ nameSpace(),
    //    /* strClassName       */ className(),
    //    /* strObjName         */ objectName(),
    //    /* strMethod          */ "ctor",
    //    /* strAddInfo         */ "" );

} // ctor

//------------------------------------------------------------------------------
CSrvCltBaseGatewayThread::~CSrvCltBaseGatewayThread()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strAddInfo         */ "" );

    quit();

    if( !wait(30000) )
    {
        if( m_pErrLog != nullptr )
        {
            SErrResultInfo errResultInfo = ErrResultInfoError("dtor", EResultTimeout, "Waiting for thread to quit timed out");
            m_pErrLog->addEntry(errResultInfo);
         }
    }

    // Can't delete the gateway object as it has been created in the context of
    // the gateway thread but the dtor of the thread is (usually) called from
    // within the main thread. The gateway instance must have already been deleted
    // on leaving the "run" method of the thread.
    //try
    //{
    //    delete m_pGateway;
    //}
    //catch(...)
    //{
    //}

    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();

        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;
    }

    //m_strObjNameGateway;
    m_pGateway = nullptr;
    m_pErrLog = nullptr;
    m_iReqIdStartThread = 0;
    m_pWaitCondition = nullptr;
    m_eTrcMthFileDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
    m_pTrcMthFile = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CSrvCltBaseGatewayThread::msleep( unsigned long i_uTime_ms )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = QString::number(i_uTime_ms);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "msleep",
        /* strAddInfo         */ strAddTrcInfo );

    QThread::msleep(i_uTime_ms);

} // msleep

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CSrvCltBaseGatewayThread::start( qint64 i_iReqIdStartThread )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "ReqId: " + QString::number(i_iReqIdStartThread);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "start",
        /* strAddInfo         */ strAddTrcInfo );

    m_iReqIdStartThread = i_iReqIdStartThread;

    QThread::start();

} // start

//------------------------------------------------------------------------------
void CSrvCltBaseGatewayThread::start( QWaitCondition* i_pWaitCondition )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "WaitCondition: " + pointer2Str(i_pWaitCondition);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "start",
        /* strAddInfo         */ strAddTrcInfo );

    m_pWaitCondition = i_pWaitCondition;

    QThread::start();

} // start

/*==============================================================================
protected: // overridables of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
void CSrvCltBaseGatewayThread::start()
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
        /* strMethod          */ "start",
        /* strAddInfo         */ strAddTrcInfo );

    QThread::start();

} // start

/*==============================================================================
protected: // overridables of base class QThread
==============================================================================*/

#if 0
//------------------------------------------------------------------------------
void CSrvCltBaseGatewayThread::run()
//------------------------------------------------------------------------------
{
    /*--------------------------------------------------------------------------
    !!! This is only a template for run methods of derived classes !!!
    --------------------------------------------------------------------------*/

    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "run",
        /* strAddInfo         */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("run");

    m_pGateway = new CClientGateway(
        /* szObjName              */ m_strObjNameGateway,
        /* pClient                */ m_pClient,
        /* pThread                */ this,
        /* pModelErrLog           */ m_pErrLog,
        /* pTrcMthFile            */ m_pTrcMthFile,
        /* iTrcMthFileDetailLevel */ m_eTrcMthFileDetailLevel );

    /*-------------------------------------------------------------------------
    !! Important note on using WaitCondition to synchronize starting threads !!
    ---------------------------------------------------------------------------

    One idea was to start a thread providing a wait condition as follows:

        QMutex         mtx;
        QWaitCondition waitCond;
        mtx.lock();
        m_pGatewayThread->start(&waitCond);
        waitCond.wait(&mtx, 200);
        mtx.unlock();

    If run method of the thread is called the wait condition should be signalled
    waking up the thread which have started the thread as follows:

    void CServerGatewayThread::run()
    {
        m_pGateway = new CServerGateway(..)
        m_pWaitCond->wakeAll();
        ...
    }

    But in some circumstances (very seldom) the threads run method has already been
    started and "wakeAll" has already been applied before the calling thread has
    called "wait" on the wait condition ("wakeAll" was called right between
    "thread.start" and "waitCond.wait").

    If that happened either "wakeAll" or "wait" could deadlock the application as
    either "wakeAll" or "wait" did never return (only after the infinite wait
    time has expired).

    To fix the "thread already running before calling wait" it has been decided to
    use startup confirmation messages sent from within the run method of the read
    to the callee. This requires more code within the callees code but finally
    pointed out to be the only safe way for the callee of "thread.start" to ensure
    that the thread is already running and its event loop has been started before
    continuing doing something with the child thread.
    --------------------------------------------------------------------------*/

    if( m_pWaitCondition != nullptr )
    {
        // The thread starting object must have called "wait" on the
        // wait condition. Otherwise "wakeAll" may never return. To ensure
        // this we force this thread to sleep for a moment ...
        msleep(10);

        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) )
        {
            mthTracer.trace( "-+ WaitCondition.wakeAll()" );
        }

        m_pWaitCondition->wakeAll();

        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) )
        {
            mthTracer.trace( "+- WaitCondition.wakeAll()" );
        }
    }
    else if( m_iReqIdStartThread >= 0 )
    {
        CRequestExecTree* pReqExecTree = CRequestExecTree::GetInstance();

        bool bIsBlockingRequest = false;

        if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(m_iReqIdStartThread) )
        {
            bIsBlockingRequest = true;

            // The thread starting object must have called "wait" on the
            // wait condition. Otherwise "wakeAll" may never return. To ensure
            // this we force this thread to sleep for a moment ...
            // If the request is still alive (wait timeout may have occured
            // and request instance may have been destroyed) ...
            if( pReqExecTree->findRequest(m_iReqIdInProgress) != nullptr )
            {
                while( !pReqExecTree->isWaiting(m_iReqIdStartThread) && !pReqExecTree->waitTimedOut(m_iReqIdInProgress) )
                {
                    msleep(10);
                }
            }

            if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) )
            {
                mthTracer.trace( "-+ ReqStartThread.WaitCondition.wakeAll()" );
            }

            if( !pReqExecTree->wake(m_iReqIdStartThread) )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultInternalStateMachineError);
                errResultInfo.setAddErrInfoDscr("Waking up thread waiting for start thread request failed");

                if( m_pErrLog != nullptr )
                {
                    m_pErrLog->addEntry(errResultInfo);
                }
            }

            if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) )
            {
                mthTracer.trace( "+- ReqStartThread.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
            }
        } // if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(m_iReqIdStartThread) )

        if( !bIsBlockingRequest )
        {
            CMsgConStartup* pMsgConStartup = new CMsgConStartup(
                /* pObjSender        */ m_pGateway,
                /* pObjReceiver      */ m_pServerOrClient,
                /* iReqId            */ m_iReqIdStartThread,
                /* iMsgIdReq         */ -1,
                /* errResultInfo     */ errResultInfo,
                /* iProgress_perCent */ 100 );
            POST_OR_DELETE_MESSAGE(pMsgConStartup, &mthTracer, ELogDetailLevel::Debug);
        }
    } // if( m_iReqIdStartThread >= 0 )

    try
    {
        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) )
        {
            mthTracer.trace( "-+ QThread::exec()" );
        }

        exec();

        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) )
        {
            mthTracer.trace( "+- QThread::exec()" );
        }
    }
    catch( CException* )
    {
        // This type of exception has already been logged on throwing (creating) the exception.
    }
    catch( CException& )
    {
        // This type of exception has already been logged on throwing (creating) the exception.
    }
    catch(...)
    {
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultUnknownException);

        if( m_pErrLog != nullptr )
        {
            m_pErrLog->addEntry(errResultInfo);
        }
    }

    /*--------------------------------------------------------------------------
    !! Note on deleting worker instances like the gateway instance created !!
    !! in the context of worker threads like this gateway thread.          !!

    Can't delete the gateway object in the dtor of the thread as it has been
    created in the context of the gateway thread. The dtor of the thread is
    (or must) be called from within the thread from which the thread has
    been created and started. So the worker gateway instance must be deleted
    on leaving the "run" method of the thread.
    --------------------------------------------------------------------------*/

    try
    {
        delete m_pGateway;
    }
    catch(...)
    {
    }
    m_pGateway = nullptr;

} // run
#endif // #if 0

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CSrvCltBaseGatewayThread::areTraceMethodCallsActive( EMethodTraceDetailLevel i_eFilterDetailLevel ) const
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
EMethodTraceDetailLevel CSrvCltBaseGatewayThread::getMethodCallsTraceDetailLevel() const
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
bool CSrvCltBaseGatewayThread::isTraceRuntimeInfoActive( ELogDetailLevel i_eFilterDetailLevel ) const
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
ELogDetailLevel CSrvCltBaseGatewayThread::getRuntimeInfoTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    ELogDetailLevel eDetailLevel = ELogDetailLevel::None;

    if( m_pTrcAdminObj != nullptr )
    {
        eDetailLevel = m_pTrcAdminObj->getRuntimeInfoTraceDetailLevel();
    }
    return eDetailLevel;
}


/*******************************************************************************
class CSrvCltBaseGateway : public QObject
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrRequests[CSrvCltBaseGateway::ERequestCount] =
//------------------------------------------------------------------------------
{
    SEnumEntry( CSrvCltBaseGateway::ERequestNone,           "None"           ),
    SEnumEntry( CSrvCltBaseGateway::ERequestStartup,        "Startup"        ),
    SEnumEntry( CSrvCltBaseGateway::ERequestShutdown,       "Shutdown"       ),
    SEnumEntry( CSrvCltBaseGateway::ERequestConnect,        "Connect"        ),
    SEnumEntry( CSrvCltBaseGateway::ERequestDisconnect,     "Disconnect"     ),
    SEnumEntry( CSrvCltBaseGateway::ERequestChangeSettings, "ChangeSettings" ),
    SEnumEntry( CSrvCltBaseGateway::ERequestReceiveData,    "ReceiveData"    ),
    SEnumEntry( CSrvCltBaseGateway::ERequestSendData,       "SendData"       )
};

//------------------------------------------------------------------------------
QString CSrvCltBaseGateway::request2Str( int i_iRequest )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrRequests,ERequestCount,i_iRequest);
}

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CSrvCltBaseGateway::CSrvCltBaseGateway(
    const QString&            i_strObjName,
    ESrvCltType               i_srvCltType,
    QObject*                  i_pCltSrv,
    CSrvCltBaseGatewayThread* i_pThreadGateway,
    CErrLog*                  i_pErrLog,
    EMethodTraceDetailLevel i_eTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pErrLog(i_pErrLog),
    m_srvCltType(i_srvCltType),
    m_pSrvClt(i_pCltSrv),
    m_pGatewayThread(i_pThreadGateway),
    m_pBlkType(nullptr),
    m_pByteArrWatchDog(nullptr),
    m_requestInProgress(ERequestNone),
    m_iReqIdInProgress(-1),
    m_iMsgIdReqInProgress(-1),
    m_pMsgCon(nullptr),
    m_arpTrcMsgLogObjects(),
    m_eTrcMthFileDetailLevel(i_eTrcMthFileDetailLevel),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
{
    if( i_strObjName.isEmpty() )
    {
        setObjectName("IpcClientGateway");
    }
    else
    {
        setObjectName(i_strObjName);
    }

    // The derived classes must instantiate the trace admin object and trace the ctor.
    //if( m_pTrcMthFile == nullptr && !i_strObjName.contains("TrcServer") && !i_strObjName.contains("TrcClient") )
    //{
    //    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    //}

    //CMethodTracer mthTracer(
    //    /* pAdminObj          */ m_pTrcAdminObj,
    //    /* pTrcMthFile        */ m_pTrcMthFile,
    //    /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
    //    /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
    //    /* strNameSpace       */ nameSpace(),
    //    /* strClassName       */ className(),
    //    /* strObjName         */ objectName(),
    //    /* strMethod          */ "ctor",
    //    /* strAddInfo         */ "" );

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CSrvCltBaseGateway::~CSrvCltBaseGateway()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strAddInfo         */ "" );

    try
    {
        delete m_pMsgCon;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pByteArrWatchDog;
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

    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();

        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;
    }

    m_pErrLog = nullptr;
    m_srvCltType = static_cast<ESrvCltType>(0);
    m_pSrvClt = nullptr;
    m_pGatewayThread = nullptr;
    m_pBlkType = nullptr;
    m_pByteArrWatchDog = nullptr;
    m_requestInProgress = static_cast<ERequest>(0);
    m_iReqIdInProgress = 0;
    m_iMsgIdReqInProgress = 0;
    m_pMsgCon = nullptr;
    //m_arpTrcMsgLogObjects;
    m_eTrcMthFileDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
    m_pTrcMthFile = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CSrvCltBaseGateway::addTrcMsgLogObject( QObject* i_pObj )
//------------------------------------------------------------------------------
{
    if( i_pObj == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pObj == nullptr");
    }

    int idxObj = m_arpTrcMsgLogObjects.indexOf(i_pObj);

    if( idxObj < 0 )
    {
        m_arpTrcMsgLogObjects.append(i_pObj);
    }

} // addTrcMsgLogObject

//------------------------------------------------------------------------------
void CSrvCltBaseGateway::removeTrcMsgLogObject( QObject* i_pObj )
//------------------------------------------------------------------------------
{
    if( i_pObj == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pObj == nullptr");
    }

    int idxObj = m_arpTrcMsgLogObjects.indexOf(i_pObj);

    if( idxObj >= 0 && idxObj < m_arpTrcMsgLogObjects.size() )
    {
        m_arpTrcMsgLogObjects.removeAt(idxObj);
    }

} // removeTrcMsgLogObject

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CSrvCltBaseGateway::areTraceMethodCallsActive( EMethodTraceDetailLevel i_eFilterDetailLevel ) const
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
EMethodTraceDetailLevel CSrvCltBaseGateway::getMethodCallsTraceDetailLevel() const
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
bool CSrvCltBaseGateway::isTraceRuntimeInfoActive( ELogDetailLevel i_eFilterDetailLevel ) const
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
ELogDetailLevel CSrvCltBaseGateway::getRuntimeInfoTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    ELogDetailLevel eDetailLevel = ELogDetailLevel::None;

    if( m_pTrcAdminObj != nullptr )
    {
        eDetailLevel = m_pTrcAdminObj->getRuntimeInfoTraceDetailLevel();
    }
    return eDetailLevel;
}
