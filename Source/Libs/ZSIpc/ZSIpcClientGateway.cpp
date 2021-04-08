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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qtimer.h>
#include <QtCore/qwaitcondition.h>
#include <QtNetwork/qhostinfo.h>

#include "ZSIpc/ZSIpcClientGateway.h"
#include "ZSIpc/ZSIpcBlkType.h"
#include "ZSIpc/ZSIpcClient.h"
#include "ZSIpc/ZSIpcSocketWrapperInProcMsg.h"
#include "ZSIpc/ZSIpcSocketWrapperShm.h"
#include "ZSIpc/ZSIpcSocketWrapperTcp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysSleeperThread.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;


/*******************************************************************************
class CClientGatewayThread : public CSrvCltBaseGatewayThread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CClientGatewayThread::CClientGatewayThread(
    const QString& i_strObjNameGateway,
    CClient*       i_pClient,
    CErrLog*       i_pErrLog,
    CTrcMthFile*   i_pTrcMthFile,
    int            i_iTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    CSrvCltBaseGatewayThread(i_strObjNameGateway, i_pErrLog, i_pTrcMthFile, i_iTrcMthFileDetailLevel),
    m_pClient(i_pClient)
{
    // The derived classes must instantiate the trace admin object and trace the ctor.
    if( m_pTrcMthFile == nullptr )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());
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

} // ctor

//------------------------------------------------------------------------------
CClientGatewayThread::~CClientGatewayThread()
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

    if( m_pTrcAdminObj != nullptr )
    {
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;
    }

    m_pClient = nullptr;

} // dtor

/*==============================================================================
protected: // Template of base class ZS::Ipc::CSrvCltBaseGatewayThread which MUST be overwritten
==============================================================================*/

//------------------------------------------------------------------------------
void CClientGatewayThread::run()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
        /* iTrcMthFileDetailLevel */ m_iTrcMthFileDetailLevel );

    //--------------------------------------------------------------------------
    // Please see comment at template run method of base class CSrvCltBaseGatewayThread.
    //--------------------------------------------------------------------------

    if( m_pWaitCondition != nullptr )
    {
        // The thread starting object must have called "wait" on the
        // wait condition. Otherwise "wakeAll" may never return. To ensure
        // this we force this thread to sleep for a moment ...
        msleep(10);

        if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
        {
            mthTracer.trace( "-+ WaitCondition.wakeAll()" );
        }

        m_pWaitCondition->wakeAll();

        if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
        {
            mthTracer.trace( "-+ WaitCondition.wakeAll()" );
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
            if( pReqExecTree->findRequest(m_iReqIdStartThread) != nullptr )
            {
                while( !pReqExecTree->isWaiting(m_iReqIdStartThread) && !pReqExecTree->waitTimedOut(m_iReqIdStartThread) )
                {
                    msleep(10);
                }
            }

            if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
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

            if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "+- ReqStartThread.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
            }
        } // if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(m_iReqIdStartThread) )

        if( !bIsBlockingRequest )
        {
            CMsgConStartup* pMsgConStartup = new CMsgConStartup(
                /* pObjSender        */ m_pGateway,
                /* pObjReceiver      */ m_pClient,
                /* iReqId            */ m_iReqIdStartThread,
                /* iMsgIdReq         */ -1,
                /* errResultInfo     */ errResultInfo,
                /* iProgress_perCent */ 100 );
            POST_OR_DELETE_MESSAGE(pMsgConStartup, &mthTracer, ETraceDetailLevelRuntimeInfo);
            pMsgConStartup = nullptr;
        }
    } // if( m_iReqIdStartThread >= 0 )

    try
    {
        if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
        {
            mthTracer.trace( "-+ QThread::exec()" );
        }

        exec();

        if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
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

    // Please see comment at template run method of base class CSrvCltBaseGatewayThread.
    try
    {
        delete m_pGateway;
    }
    catch(...)
    {
    }
    m_pGateway = nullptr;

} // run


/*******************************************************************************
class CClientGateway : public CSrvCltBaseGateway
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CClientGateway::CClientGateway(
    const QString&        i_strObjName,
    CClient*              i_pClient,
    CClientGatewayThread* i_pThreadGateway,
    CErrLog*              i_pErrLog,
    CTrcMthFile*          i_pTrcMthFile,
    int                   i_iTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    CSrvCltBaseGateway(
        /* szObjName              */ i_strObjName,
        /* srvCltType             */ ESrvCltTypeClient,
        /* pCltSrv                */ i_pClient,
        /* pThreadGateway         */ i_pThreadGateway,
        /* pModelErrLog           */ i_pErrLog,
        /* pTrcMthFile            */ i_pTrcMthFile,
        /* iTrcMthFileDetailLevel */ i_iTrcMthFileDetailLevel ),
    m_socketDscr(ESrvCltTypeClient),
    m_pIpcSocketWrapper(nullptr),
    m_pTimerConnect(nullptr),
    m_watchDogSettings(),
    m_pTimerWatchDog(nullptr)
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

    m_pTimerConnect = new QTimer(this);
    m_pTimerConnect->setSingleShot(true);

    if( !QObject::connect(
        /* pObjSender   */ m_pTimerConnect,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTimeoutConnect()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Set the default value for the local host name to the computers name.
    m_socketDscr.m_strLocalHostName = QHostInfo::localHostName();

    m_pTimerWatchDog = new QTimer(this);

    if( !QObject::connect(
        /* pObjSender   */ m_pTimerWatchDog,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTimeoutWatchDog()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CClientGateway::~CClientGateway()
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
        delete m_pIpcSocketWrapper;
    }
    catch(...)
    {
    }

    if( m_pTrcAdminObj != nullptr )
    {
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;
    }

    //m_socketDscr;
    m_pIpcSocketWrapper = nullptr;
    m_pTimerConnect = nullptr;
    //m_watchDogSettings;
    m_pTimerWatchDog = nullptr;

} // dtor

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CClientGateway::onConnected( QObject* /*i_pSocketWrapper*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "RequestInProgress: " + request2Str(m_requestInProgress);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onConnected",
        /* strAddInfo         */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("onConnected");

    if( m_pTimerWatchDog->isActive() )
    {
        m_pTimerWatchDog->stop();
    }
    if( m_pTimerConnect->isActive() )
    {
        m_pTimerConnect->stop();
    }

    m_socketDscr.m_socketState = m_pIpcSocketWrapper->socketState();
    m_socketDscr.m_iSocketId = m_pIpcSocketWrapper->getSocketId();
    m_socketDscr.m_uServerListenPort = m_pIpcSocketWrapper->getServerListenPort();
    m_socketDscr.m_strRemoteHostName = m_pIpcSocketWrapper->getRemoteHostName();
    m_socketDscr.m_hostAddrRemote = m_pIpcSocketWrapper->getRemoteHostAddress();
    m_socketDscr.m_uRemotePort = m_pIpcSocketWrapper->getRemotePort();
    m_socketDscr.m_uBufferSize = m_pIpcSocketWrapper->getBufferSize();
    m_socketDscr.m_uLocalPort = m_pIpcSocketWrapper->getLocalPort();

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
                    /* transmitDir  */ ETransmitDirUndefined,
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

            if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "-+ ReqConnect.WaitCondition.wakeAll()" );
            }

            if( !pReqExecTree->wake(m_iReqIdInProgress) )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultInternalStateMachineError);
                errResultInfo.setAddErrInfoDscr("Waking up thread waiting for connect request failed");

                if( m_pErrLog != nullptr )
                {
                    m_pErrLog->addEntry(errResultInfo);
                }
            }

            if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
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
            m_pTimerWatchDog->start(m_watchDogSettings.m_iInterval_ms);
        }
    } // if( m_requestInProgress == ERequestConnect )

    else // if( m_requestInProgress != ERequestConnect )
    {
        CMsgIndConnected* pMsgInd = new CMsgIndConnected(
            /* pObjSender   */ this,
            /* pObjReceiver */ m_pSrvClt,
            /* socketDscr   */ m_socketDscr );
        POST_OR_DELETE_MESSAGE(pMsgInd, &mthTracer, ETraceDetailLevelRuntimeInfo);
        pMsgInd = nullptr;
    }

} // onConnected

//------------------------------------------------------------------------------
void CClientGateway::onTimeoutConnect()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTimeoutConnect",
        /* strAddInfo         */ "" );

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

        if( m_pTimerWatchDog->isActive() )
        {
            m_pTimerWatchDog->stop();
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
                    /* transmitDir  */ ETransmitDirUndefined,
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

            if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "-+ ReqConnect.WaitCondition.wakeAll()" );
            }

            if( !pReqExecTree->wake(m_iReqIdInProgress) )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultInternalStateMachineError);
                errResultInfo.setAddErrInfoDscr("Waking up thread waiting for connect request failed");

                if( m_pErrLog != nullptr )
                {
                    m_pErrLog->addEntry(errResultInfo);
                }
            }

            if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
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
void CClientGateway::onTimeoutWatchDog()
//------------------------------------------------------------------------------
{
    if( m_pByteArrWatchDog != nullptr )
    {
        QString strAddTrcInfo;

        CMethodTracer mthTracer(
            /* pAdminObj          */ m_pTrcAdminObj,
            /* pTrcMthFile        */ m_pTrcMthFile,
            /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelVerbose,
            /* strNameSpace       */ nameSpace(),
            /* strClassName       */ className(),
            /* strObjName         */ objectName(),
            /* strMethod          */ "onTimeoutWatchDog",
            /* strAddInfo         */ strAddTrcInfo );

        QString strAddErrInfo;

        bool bTimeout = false;

        if( m_watchDogSettings.m_bEnabled && m_watchDogSettings.m_iTimeout_ms > 0 )
        {
            if( m_pIpcSocketWrapper != nullptr )
            {
                double fTimeCurrent_ms = ZS::System::Time::getProcTimeInMilliSec();
                double fTimeDiff_ms    = fTimeCurrent_ms - m_pIpcSocketWrapper->getLastTimeDataBlockReceivedInMs();

                if( fTimeDiff_ms > static_cast<double>(m_watchDogSettings.m_iTimeout_ms) )
                {
                    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
                    {
                        mthTracer.trace( "TIMEOUT: disconnecting from host " + m_pIpcSocketWrapper->getRemoteHostName() + ":" + QString::number(m_pIpcSocketWrapper->getServerListenPort()) + ":" + QString::number(m_pIpcSocketWrapper->getRemotePort()) );
                    }

                    if( m_pErrLog != nullptr )
                    {
                        strAddErrInfo  = "Duration since last message received from ";
                        strAddErrInfo += m_pIpcSocketWrapper->getRemoteHostName() + ":" + QString::number(m_pIpcSocketWrapper->getServerListenPort()) + ":" + QString::number(m_pIpcSocketWrapper->getRemotePort());
                        strAddErrInfo += " exceeded watch dog timeout of " + QString::number(m_watchDogSettings.m_iTimeout_ms) + " ms";
                        SErrResultInfo errResultInfo = ErrResultInfoWarning("onTimeoutWatchDog", EResultTimeout, strAddErrInfo);
                        m_pErrLog->addEntry(errResultInfo);
                    }

                    if( m_arpTrcMsgLogObjects.size() > 0 )
                    {
                        strAddTrcInfo = "-+ IpcSocket::disconnectFromHost( " + m_socketDscr.getConnectionString() + " )";

                        for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                        {
                            CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                /* pObjSender   */ this,
                                /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                /* transmitDir  */ ETransmitDirUndefined,
                                /* bBold        */ false,
                                /* strMsg       */ strAddTrcInfo );
                            POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                            pMsgLogItem = nullptr;
                        }
                    }

                    bTimeout = true;
                    m_pIpcSocketWrapper->disconnectFromHost();

                    if( m_arpTrcMsgLogObjects.size() > 0 )
                    {
                        strAddTrcInfo = "+- IpcSocket::disconnectFromHost( SUCCESS )";

                        for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                        {
                            CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                /* pObjSender   */ this,
                                /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                /* transmitDir  */ ETransmitDirUndefined,
                                /* bBold        */ false,
                                /* strMsg       */ strAddTrcInfo );
                            POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                            pMsgLogItem = nullptr;
                        }
                    }
                } // if( fTimeDiff_ms > static_cast<double>(m_watchDogSettings.m_iTimeout_ms) )
            } // if( m_pIpcSocketWrapper != nullptr )
        } // if( m_watchDogSettings.m_bEnabled && m_watchDogSettings.m_iTimeout_ms > 0 )

        if( !bTimeout )
        {
            m_pBlkType->writeDataBlock(
               /* srvCltType          */ m_srvCltType,
               /* pObjGtw             */ this,
               /* pObjSrvClt          */ m_pSrvClt,
               /* pSocketWrapper      */ m_pIpcSocketWrapper,
               /* byteArr             */ *m_pByteArrWatchDog,
               /* isWatchDog          */ true,
               /* pTrcAdminObj        */ &mthTracer,
               /* arpTrcMsgLogObjects */ m_arpTrcMsgLogObjects );
        }
    } // if( m_pByteArrWatchDog != nullptr )

} // onTimeoutWatchDog

/*==============================================================================
protected slots: // must overridables of base class CSrvCltBaseGateway
==============================================================================*/

//------------------------------------------------------------------------------
void CClientGateway::onDisconnected( QObject* /*i_pSocketWrapper*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "RequestInProgress: " + request2Str(m_requestInProgress);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onDisconnected",
        /* strAddInfo         */ strAddTrcInfo );

    if( m_pTimerWatchDog->isActive() )
    {
        m_pTimerWatchDog->stop();
    }
    if( m_pTimerConnect->isActive() )
    {
        m_pTimerConnect->stop();
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
                /* transmitDir  */ ETransmitDirUndefined,
                /* bBold        */ false,
                /* strMsg       */ strAddTrcInfo );
            POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
            pMsgLogItem = nullptr;
        }
    }

    QString strAddErrInfo;

    switch( m_requestInProgress )
    {
        case ERequestShutdown:
        {
            break;
        }

        case ERequestConnect:
        {
            strAddErrInfo = "Remote host " + m_socketDscr.m_strRemoteHostName + ":" + QString::number(m_socketDscr.m_uServerListenPort) + " disconnected";
            ZS::System::SErrResultInfo errResultInfo = ErrResultInfoError("connect", EResultRequestRefused, strAddErrInfo);

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

                if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
                {
                    mthTracer.trace( "-+ ReqConnect.WaitCondition.wakeAll()" );
                }

                if( !pReqExecTree->wake(m_iReqIdInProgress) )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInternalStateMachineError);
                    errResultInfo.setAddErrInfoDscr("Waking up thread waiting for connect request failed");

                    if( m_pErrLog != nullptr )
                    {
                        m_pErrLog->addEntry(errResultInfo);
                    }
                }

                if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
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
            ZS::System::SErrResultInfo errResultInfo = ErrResultInfoSuccess("disconnect");

            if( m_pMsgCon != nullptr )
            {
                m_pMsgCon->setErrResultInfo(errResultInfo);
                m_pMsgCon->setProgress(100);
                CMsgConDisconnect* pMsgCon = dynamic_cast<CMsgConDisconnect*>(m_pMsgCon);
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

                if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
                {
                    mthTracer.trace( "-+ ReqDisconnect.WaitCondition.wakeAll()" );
                }

                if( !pReqExecTree->wake(m_iReqIdInProgress) )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInternalStateMachineError);
                    errResultInfo.setAddErrInfoDscr("Waking up thread waiting for disconnect request failed");

                    if( m_pErrLog != nullptr )
                    {
                        m_pErrLog->addEntry(errResultInfo);
                    }
                }

                if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
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

        case ERequestChangeSettings:
        {
            break;
        }

        case ERequestReceiveData:
        {
            // The socket state will be checked after the "receiveDataBlock" method returns.
            break;
        }
        case ERequestSendData:
        {
            // The socket state will be checked after the "writeDataBlock" method returns.
            break;
        }

        case ERequestNone:
        default:
        {
            CMsgIndDisconnected* pMsgInd = new CMsgIndDisconnected(
                /* pObjSender   */ this,
                /* pObjReceiver */ m_pSrvClt,
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
void CClientGateway::onReadyRead( QObject* i_pSocketWrapper )
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
        /* strMethod          */ "onReadyRead",
        /* strAddInfo         */ "" );

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
void CClientGateway::onError( QObject* /*i_pSocketWrapper*/, ZS::System::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo =  i_errResultInfo.getResultStr();
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onError",
        /* strAddInfo         */ strAddTrcInfo );

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
                    /* socketDscr       */ m_socketDscr,
                    /* watchDogSettings */ m_watchDogSettings,
                    /* pBlkType         */ m_pBlkType,
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
                if( m_pTimerWatchDog->isActive() )
                {
                    m_pTimerWatchDog->stop();
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
                            /* transmitDir  */ ETransmitDirUndefined,
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

                    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
                    {
                        mthTracer.trace( "-+ ReqConnect.WaitCondition.wakeAll()" );
                    }

                    if( !pReqExecTree->wake(m_iReqIdInProgress) )
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultInternalStateMachineError);
                        errResultInfo.setAddErrInfoDscr("Waking up thread waiting for connect request failed");

                        if( m_pErrLog != nullptr )
                        {
                            m_pErrLog->addEntry(errResultInfo);
                        }
                    }

                    if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
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

        case ERequestReceiveData:
        {
            switch( m_pIpcSocketWrapper->error() )
            {
                // Timeout while waiting for data ..
                case EResultSocketTimeout:
                {
                    // .. will be reported by the "receiveDataBlock" methods.
                    result = EResultSuccess;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        } // case ERequestReceiveData

        case ERequestStartup:
        case ERequestShutdown:
        case ERequestChangeSettings:
        case ERequestSendData:
        default:
        {
            break;
        }
    } // switch( m_requestInProgress )

    if( result != EResultSuccess )
    {
        CMsgErr* pMsgErr = new CMsgErr(
            /* pObjSender    */ this,
            /* pObjReceiver  */ m_pSrvClt,
            /* errResultInfo */ errResultInfo );
        POST_OR_DELETE_MESSAGE(pMsgErr, &mthTracer, ETraceDetailLevelRuntimeInfo);
        pMsgErr = nullptr;
    }

} // onError

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CClientGateway::event( QEvent* i_pMsg )
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
            strAddTrcInfo = "Addr: 0x" + QString::number(reinterpret_cast<qint64>(i_pMsg),16);

            if( pMsg == nullptr )
            {
                strAddTrcInfo += ", Content {MsgType: " + QString::number(i_pMsg->type()) + "}";
            }
            else
            {
                strAddTrcInfo += ", Content {" + pMsg->getAddTrcInfoStr() + "}";
            }
            strAddTrcInfo += ", ReqInProgress: " + request2Str(m_requestInProgress);
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
            bEventHandled = true;

            bool bRequestFinished = false;

            SErrResultInfo errResultInfo = ErrResultInfoSuccess( pMsg->msgTypeToStr() );

            CRequestExecTree* pReqExecTree = CRequestExecTree::GetInstance();

            switch( static_cast<int>(pMsg->type()) )
            {
                case EMsgTypeReqShutdown:
                {
                    CMsgReqShutdown* pMsgReq = dynamic_cast<CMsgReqShutdown*>(i_pMsg);

                    if( pMsgReq == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReq->getSender() != m_pSrvClt )
                    {
                        throw CException(__FILE__,__LINE__,EResultInvalidSenderObjInMessage);
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
                        strAddErrInfo = "RequestInProgress: " + request2Str(m_requestInProgress) + ", " + pMsg->getAddTrcInfoStr();
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultMessageStateMismatch);
                        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                    }

                    m_requestInProgress   = ERequestShutdown;
                    m_iReqIdInProgress    = pMsgReq->getRequestId();
                    m_iMsgIdReqInProgress = pMsgReq->getMsgId();

                    if( m_pTimerWatchDog->isActive() )
                    {
                        m_pTimerWatchDog->stop();
                    }

                    if( m_pIpcSocketWrapper != nullptr )
                    {
                        // Either the "disconnected" or "error" signal will be emitted by the
                        // sockets calling the "disconnectFromHost" or "abort" methods.
                        if( m_pIpcSocketWrapper->socketState() != ESocketStateUnconnected )
                        {
                            if( m_arpTrcMsgLogObjects.size() > 0 )
                            {
                                strAddTrcInfo = "-+ IpcSocket::abort( " + m_socketDscr.getConnectionString() + " )";

                                for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                                {
                                    CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                        /* pObjSender   */ this,
                                        /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                        /* transmitDir  */ ETransmitDirUndefined,
                                        /* bBold        */ false,
                                        /* strMsg       */ strAddTrcInfo );
                                    POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                    pMsgLogItem = nullptr;
                                }
                            }

                            m_pIpcSocketWrapper->abort();

                            if( m_arpTrcMsgLogObjects.size() > 0 )
                            {
                                strAddTrcInfo = "+- IpcSocket::abort( SUCCESS )";

                                for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                                {
                                    CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                        /* pObjSender   */ this,
                                        /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                        /* transmitDir  */ ETransmitDirUndefined,
                                        /* bBold        */ false,
                                        /* strMsg       */ strAddTrcInfo );
                                    POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                    pMsgLogItem = nullptr;
                                }
                            }
                        } // if( m_pIpcSocketWrapper->socketState() != ESocketStateUnconnected )

                        try
                        {
                            delete m_pIpcSocketWrapper;
                        }
                        catch(...)
                        {
                        }
                        m_pIpcSocketWrapper = nullptr;

                    } // if( m_pIpcSocketWrapper != nullptr )

                    bRequestFinished = true;

                    if( m_pMsgCon != nullptr )
                    {
                        m_pMsgCon->setErrResultInfo(errResultInfo);
                        m_pMsgCon->setProgress(100);
                        CMsgConShutdown* pMsgCon = dynamic_cast<CMsgConShutdown*>(m_pMsgCon);
                        if( pMsgCon != nullptr )
                        {
                            pMsgCon->setSocketState(m_socketDscr.m_socketState);
                        }
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

                        if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
                        {
                            mthTracer.trace( "-+ ReqShutdown.WaitCondition.wakeAll()" );
                        }

                        if( !pReqExecTree->wake(m_iReqIdInProgress) )
                        {
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultInternalStateMachineError);
                            errResultInfo.setAddErrInfoDscr("Waking up thread waiting for shutdown request failed");

                            if( m_pErrLog != nullptr )
                            {
                                m_pErrLog->addEntry(errResultInfo);
                            }
                        }

                        if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
                        {
                            mthTracer.trace( "+- ReqShutdown.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
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

                case EMsgTypeReqConnect:
                {
                    CMsgReqConnect* pMsgReq = dynamic_cast<CMsgReqConnect*>(i_pMsg);

                    if( pMsgReq == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch,strAddTrcInfo);
                    }
                    if( pMsgReq->getSender() != m_pSrvClt && pMsgReq->getSender() != this )
                    {
                        throw CException(__FILE__,__LINE__,EResultInvalidSenderObjInMessage,strAddTrcInfo);
                    }

                    bool bIsBlockingRequest = false;

                    if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(pMsgReq->getRequestId()) )
                    {
                        bIsBlockingRequest = true;
                    }

                    if( pMsgReq->getSender() == m_pSrvClt )
                    {
                        delete m_pMsgCon;
                        m_pMsgCon = nullptr;

                        if( pMsgReq->mustBeConfirmed() || bIsBlockingRequest )
                        {
                            m_pMsgCon = pMsgReq->createConfirmationMessage();
                        }
                    }

                    if( m_requestInProgress != ERequestNone && m_requestInProgress != ERequestConnect )
                    {
                        strAddErrInfo = "RequestInProgress: " + request2Str(m_requestInProgress) + ", " + pMsg->getAddTrcInfoStr();
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultMessageStateMismatch);
                        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                        bRequestFinished = true;
                    }
                    else // if( m_requestInProgress == ERequestNone || m_requestInProgress == ERequestConnect )
                    {
                        // The connect request may also have been send from myself from within the error
                        // method if the host did not respond but the timeout has not been expired.

                        // If I received the connect request from myself because the connect timeout has not yet been expired ..
                        if( pMsgReq->getSender() == this )
                        {
                            if( m_pIpcSocketWrapper == nullptr )
                            {
                                throw CException(__FILE__,__LINE__,EResultMessageStateMismatch,strAddTrcInfo+", m_pIpcSocketWrapper==nullptr");
                            }
                        }

                        // If I received the initial connect request from the client ..
                        else // if( pMsgReq->getSender() == m_pSrvClt )
                        {
                            m_socketDscr = pMsgReq->getHostSettings().getSocketDscr();

                            m_watchDogSettings = pMsgReq->getWatchDogSettings();

                            m_requestInProgress   = ERequestConnect;
                            m_iReqIdInProgress    = pMsgReq->getRequestId();
                            m_iMsgIdReqInProgress = pMsgReq->getMsgId();

                            if( m_pIpcSocketWrapper != nullptr )
                            {
                                if( m_pIpcSocketWrapper->socketType() != m_socketDscr.m_socketType )
                                {
                                    if( m_pIpcSocketWrapper->socketState() != ESocketStateUnconnected )
                                    {
                                        if( m_arpTrcMsgLogObjects.size() > 0 )
                                        {
                                            strAddTrcInfo = "-+ IpcSocket::abort( " + m_socketDscr.getConnectionString() + " )";

                                            for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                                            {
                                                CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                                    /* pObjSender   */ this,
                                                    /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                                    /* transmitDir  */ ETransmitDirUndefined,
                                                    /* bBold        */ false,
                                                    /* strMsg       */ strAddTrcInfo );
                                                POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                                pMsgLogItem = nullptr;
                                            }
                                        }

                                        m_pIpcSocketWrapper->abort();

                                        if( m_arpTrcMsgLogObjects.size() > 0 )
                                        {
                                            strAddTrcInfo = "+- IpcSocket::abort( SUCCESS )";

                                            for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                                            {
                                                CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                                    /* pObjSender   */ this,
                                                    /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                                    /* transmitDir  */ ETransmitDirUndefined,
                                                    /* bBold        */ false,
                                                    /* strMsg       */ strAddTrcInfo );
                                                POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                                pMsgLogItem = nullptr;
                                            }
                                        }
                                    } // if( m_pIpcSocketWrapper->socketState() != ESocketStateUnconnected )

                                    try
                                    {
                                        delete m_pIpcSocketWrapper;
                                    }
                                    catch(...)
                                    {
                                    }
                                    m_pIpcSocketWrapper = nullptr;

                                } // if( m_pIpcSocketWrapper->socketType() != pMsgReq->getSocketType() )
                            } // if( m_pIpcSocketWrapper != nullptr && m_pIpcSocketWrapper->socketType() != pMsgReq->getSocketType() )

                            if( m_pIpcSocketWrapper == nullptr )
                            {
                                switch( m_socketDscr.m_socketType )
                                {
                                    case ESocketTypeTcp:
                                    {
                                        m_pIpcSocketWrapper = new CTcpSocketWrapper( objectName(), ESrvCltTypeClient, 0, nullptr );
                                        break;
                                    }
                                    case ESocketTypeShm:
                                    {
                                        m_pIpcSocketWrapper = new CShmSocketWrapper( objectName(), ESrvCltTypeClient, 0 );
                                        break;
                                    }
                                    case ESocketTypeInProcMsg:
                                    {
                                        m_pIpcSocketWrapper = new CInProcMsgSocketWrapper( objectName(), ESrvCltTypeClient, 0, nullptr );
                                        break;
                                    }
                                    default:
                                    {
                                        errResultInfo.setSeverity(EResultSeverityError);
                                        errResultInfo.setResult(EResultArgOutOfRange);
                                        errResultInfo.setAddErrInfoDscr( "Unknown Socket Type " + socketType2Str(m_socketDscr.m_socketType) );
                                        break;
                                    }
                                } // switch( m_socketDscr.m_socketType )

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
                            } // if( m_pIpcSocketWrapper == nullptr )

                            if( pMsgReq->getBlkType() != nullptr )
                            {
                                if( pMsgReq->getBlkType() != m_pBlkType )
                                {
                                    try
                                    {
                                        delete m_pBlkType;
                                    }
                                    catch(...)
                                    {
                                    }
                                    m_pBlkType = pMsgReq->getBlkType(true);

                                    try
                                    {
                                        delete m_pByteArrWatchDog;
                                    }
                                    catch(...)
                                    {
                                    }
                                    m_pByteArrWatchDog = m_pBlkType->createWatchDogBlock();
                                }
                            } // if( pMsgReq->getBlkType() != nullptr )

                            else // if( pMsgReq->getBlkType() == nullptr )
                            {
                                try
                                {
                                    delete m_pBlkType;
                                }
                                catch(...)
                                {
                                }
                                m_pBlkType = nullptr;

                                try
                                {
                                    delete m_pByteArrWatchDog;
                                }
                                catch(...)
                                {
                                }
                                m_pByteArrWatchDog = nullptr;

                            } // if( pMsgReq->getBlkType() == nullptr )
                        } // if( pMsgReq->getSender() == m_pSrvClt )

                        if( m_pIpcSocketWrapper == nullptr )
                        {
                            bRequestFinished = true;
                        }
                        else // if( m_pIpcSocketWrapper != nullptr )
                        {
                            if( m_pIpcSocketWrapper->socketState() != ESocketStateConnecting )
                            {
                                if( m_arpTrcMsgLogObjects.size() > 0 )
                                {
                                    strAddTrcInfo = "-+ IpcSocket::connectToHost( " + m_socketDscr.getConnectionString() + " )";

                                    for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                                    {
                                        CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                            /* pObjSender   */ this,
                                            /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                            /* transmitDir  */ ETransmitDirUndefined,
                                            /* bBold        */ false,
                                            /* strMsg       */ strAddTrcInfo );
                                        POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                        pMsgLogItem = nullptr;
                                    }
                                }

                                // The slot "onConnected" will be called if the socket is connected.
                                // The slot will send the confirmation message or wakes up the waiting request
                                // and resets the request in progress to None.
                                m_pIpcSocketWrapper->connectToHost( m_socketDscr.m_strRemoteHostName, m_socketDscr.m_uServerListenPort, m_socketDscr.m_uBufferSize );

                                if( m_arpTrcMsgLogObjects.size() > 0 )
                                {
                                    strAddTrcInfo = "+- IpcSocket::connectToHost()";

                                    for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                                    {
                                        CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                            /* pObjSender   */ this,
                                            /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                            /* transmitDir  */ ETransmitDirUndefined,
                                            /* bBold        */ false,
                                            /* strMsg       */ strAddTrcInfo );
                                        POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                        pMsgLogItem = nullptr;
                                    }
                                }
                            } // if( m_pIpcSocketWrapper->socketState() != ESocketStateConnecting )

                            if( m_pIpcSocketWrapper->socketState() == ESocketStateConnected )
                            {
                                bRequestFinished = true;
                            }
                            else if( m_socketDscr.m_iConnectTimeout_ms > 0 )
                            {
                                // Start single shot timer:
                                if( !m_pTimerConnect->isActive() )
                                {
                                    m_pTimerConnect->start(m_socketDscr.m_iConnectTimeout_ms);
                                }
                            }
                        } // if( m_pIpcSocketWrapper != nullptr )
                    } // if( m_requestInProgress == ERequestNone || m_requestInProgress == ERequestConnect )

                    if( bRequestFinished && m_requestInProgress == ERequestConnect )
                    {
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

                            if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
                            {
                                mthTracer.trace( "-+ ReqConnect.WaitCondition.wakeAll()" );
                            }

                            if( !pReqExecTree->wake(m_iReqIdInProgress) )
                            {
                                errResultInfo.setSeverity(EResultSeverityError);
                                errResultInfo.setResult(EResultInternalStateMachineError);
                                errResultInfo.setAddErrInfoDscr("Waking up thread waiting for connect request failed");

                                if( m_pErrLog != nullptr )
                                {
                                    m_pErrLog->addEntry(errResultInfo);
                                }
                            }

                            if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
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

                    } // if( bRequestFinished && m_requestInProgress == ERequestConnect )
                    break;
                } // case EMsgTypeReqConnect

                case EMsgTypeReqDisconnect:
                {
                    CMsgReqDisconnect* pMsgReq = dynamic_cast<CMsgReqDisconnect*>(i_pMsg);

                    if( pMsgReq == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReq->getSender() != m_pSrvClt )
                    {
                        throw CException(__FILE__,__LINE__,EResultInvalidSenderObjInMessage,strAddTrcInfo);
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
                        strAddErrInfo = "RequestInProgress: " + request2Str(m_requestInProgress) + ", " + pMsg->getAddTrcInfoStr();
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultMessageStateMismatch);
                        errResultInfo.setAddErrInfoDscr(strAddErrInfo);

                        if( m_pErrLog != nullptr )
                        {
                            m_pErrLog->addEntry(errResultInfo);
                        }
                    }

                    m_requestInProgress   = ERequestDisconnect;
                    m_iReqIdInProgress    = pMsgReq->getRequestId();
                    m_iMsgIdReqInProgress = pMsgReq->getMsgId();

                    if( m_pTimerWatchDog->isActive() )
                    {
                        m_pTimerWatchDog->stop();
                    }

                    if( m_pIpcSocketWrapper != nullptr )
                    {
                        // Either the "disconnected" or "error" signal will be emitted by the
                        // sockets calling the "disconnectFromHost" or "abort" methods.
                        if( m_pIpcSocketWrapper->socketState() == ESocketStateConnected )
                        {
                            if( m_arpTrcMsgLogObjects.size() > 0 )
                            {
                                strAddTrcInfo = "-+ IpcSocket::disconnectFromHost( " + m_socketDscr.getConnectionString() + " )";

                                for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                                {
                                    CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                        /* pObjSender   */ this,
                                        /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                        /* transmitDir  */ ETransmitDirUndefined,
                                        /* bBold        */ false,
                                        /* strMsg       */ strAddTrcInfo );
                                    POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                    pMsgLogItem = nullptr;
                                }
                            }

                            // The slot "onDisconnected" will be called if the socket is disconnected.
                            // The slot will send the confirmation message or wakes up the waiting request
                            // and resets the request in progress to None.
                            m_pIpcSocketWrapper->disconnectFromHost();

                            if( m_arpTrcMsgLogObjects.size() > 0 )
                            {
                                strAddTrcInfo = "+- IpcSocket::disconnectFromHost( SUCCESS )";

                                for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                                {
                                    CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                        /* pObjSender   */ this,
                                        /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                        /* transmitDir  */ ETransmitDirUndefined,
                                        /* bBold        */ false,
                                        /* strMsg       */ strAddTrcInfo );
                                    POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                    pMsgLogItem = nullptr;
                                }
                            }
                        } // if( m_pIpcSocketWrapper->socketState() == ESocketStateConnected )

                        else if( m_pIpcSocketWrapper->socketState() == ESocketStateConnecting )
                        {
                            if( m_arpTrcMsgLogObjects.size() > 0 )
                            {
                                strAddTrcInfo = "-+ IpcSocket::abort( " + m_socketDscr.getConnectionString() + " )";

                                for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                                {
                                    CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                        /* pObjSender   */ this,
                                        /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                        /* transmitDir  */ ETransmitDirUndefined,
                                        /* bBold        */ false,
                                        /* strMsg       */ strAddTrcInfo );
                                    POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                    pMsgLogItem = nullptr;
                                }
                            }

                            m_pIpcSocketWrapper->abort();

                            if( m_arpTrcMsgLogObjects.size() > 0 )
                            {
                                strAddTrcInfo = "+- IpcSocket::abort( SUCCESS )";

                                for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                                {
                                    CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                        /* pObjSender   */ this,
                                        /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                        /* transmitDir  */ ETransmitDirUndefined,
                                        /* bBold        */ false,
                                        /* strMsg       */ strAddTrcInfo );
                                    POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                    pMsgLogItem = nullptr;
                                }
                            }
                        } // if( m_pIpcSocketWrapper->socketState() == ESocketStateConnecting )

                        if( m_pIpcSocketWrapper->socketState() == ESocketStateUnconnected )
                        {
                            bRequestFinished = true;
                        }
                    } // if( m_pIpcSocketWrapper != nullptr )

                    if( bRequestFinished && m_requestInProgress == ERequestDisconnect )
                    {
                        if( m_pMsgCon != nullptr )
                        {
                            m_pMsgCon->setErrResultInfo(errResultInfo);
                            m_pMsgCon->setProgress(100);
                            CMsgConDisconnect* pMsgCon = dynamic_cast<CMsgConDisconnect*>(m_pMsgCon);
                            if( pMsgCon != nullptr )
                            {
                                pMsgCon->setSocketState(m_socketDscr.m_socketState);
                            }
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

                            if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
                            {
                                mthTracer.trace( "-+ ReqDisconnect.WaitCondition.wakeAll()" );
                            }

                            if( !pReqExecTree->wake(m_iReqIdInProgress) )
                            {
                                errResultInfo.setSeverity(EResultSeverityError);
                                errResultInfo.setResult(EResultInternalStateMachineError);
                                errResultInfo.setAddErrInfoDscr("Waking up thread waiting for disconnect request failed");

                                if( m_pErrLog != nullptr )
                                {
                                    m_pErrLog->addEntry(errResultInfo);
                                }
                            }

                            if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
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

                    } // if( bRequestFinished && m_requestInProgress == ERequestDisconnect )
                    break;
                } // case EMsgTypeReqDisconnect

                case EMsgTypeReqChangeSettings:
                {
                    CMsgReqChangeSettings* pMsgReq = dynamic_cast<CMsgReqChangeSettings*>(i_pMsg);

                    if( pMsgReq == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReq->getSender() != m_pSrvClt )
                    {
                        throw CException(__FILE__,__LINE__,EResultInvalidSenderObjInMessage,strAddTrcInfo);
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
                        strAddErrInfo = "RequestInProgress: " + request2Str(m_requestInProgress) + ", " + pMsg->getAddTrcInfoStr();
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultMessageStateMismatch);
                        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                        bRequestFinished = true;
                    }
                    else // if( m_requestInProgress == ERequestNone )
                    {
                        SClientHostSettings hostSettingsNew = pMsgReq->getClientHostSettings();
                        SClientHostSettings hostSettingsOld = m_socketDscr;

                        bool bWatchDogSettingsChanged = false;

                        m_requestInProgress   = ERequestChangeSettings;
                        m_iReqIdInProgress    = pMsgReq->getRequestId();
                        m_iMsgIdReqInProgress = pMsgReq->getMsgId();

                        if( pMsgReq->getBlkType() != nullptr )
                        {
                            if( pMsgReq->getBlkType() != m_pBlkType )
                            {
                                try
                                {
                                    delete m_pBlkType;
                                }
                                catch(...)
                                {
                                }
                                m_pBlkType = pMsgReq->getBlkType(true);

                                try
                                {
                                    delete m_pByteArrWatchDog;
                                }
                                catch(...)
                                {
                                }
                                m_pByteArrWatchDog = m_pBlkType->createWatchDogBlock();
                            }
                        } // if( pMsgReq->getBlkType() != nullptr )

                        else // if( pMsgReq->getBlkType() == nullptr )
                        {
                            try
                            {
                                delete m_pBlkType;
                            }
                            catch(...)
                            {
                            }
                            m_pBlkType = nullptr;

                            try
                            {
                                delete m_pByteArrWatchDog;
                            }
                            catch(...)
                            {
                            }
                            m_pByteArrWatchDog = nullptr;

                        } // if( pMsgReq->getBlkType() == nullptr )

                        if( pMsgReq->getWatchDogSettings() != m_watchDogSettings )
                        {
                            m_watchDogSettings = pMsgReq->getWatchDogSettings();
                            bWatchDogSettingsChanged = true;
                        }

                        if( hostSettingsNew == hostSettingsOld )
                        {
                            if( bWatchDogSettingsChanged )
                            {
                                if( m_pTimerWatchDog->isActive() )
                                {
                                    m_pTimerWatchDog->stop();
                                }
                                if( m_watchDogSettings.m_bEnabled && m_watchDogSettings.m_iInterval_ms > 0 )
                                {
                                    m_pTimerWatchDog->start(m_watchDogSettings.m_iInterval_ms);
                                }
                            }
                        }
                        else // if( hostSettingsNew != hostSettingsOld )
                        {
                            if( m_pTimerWatchDog->isActive() )
                            {
                                m_pTimerWatchDog->stop();
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
                                        if( m_arpTrcMsgLogObjects.size() > 0 )
                                        {
                                            strAddTrcInfo = "-+ IpcSocket::abort( " + m_socketDscr.getConnectionString() + " )";

                                            for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                                            {
                                                CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                                    /* pObjSender   */ this,
                                                    /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                                    /* transmitDir  */ ETransmitDirUndefined,
                                                    /* bBold        */ false,
                                                    /* strMsg       */ strAddTrcInfo );
                                                POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                                pMsgLogItem = nullptr;
                                            }
                                        }

                                        m_pIpcSocketWrapper->abort();

                                        if( m_arpTrcMsgLogObjects.size() > 0 )
                                        {
                                            strAddTrcInfo = "+- IpcSocket::abort( SUCCESS )";

                                            for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                                            {
                                                CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                                    /* pObjSender   */ this,
                                                    /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                                    /* transmitDir  */ ETransmitDirUndefined,
                                                    /* bBold        */ false,
                                                    /* strMsg       */ strAddTrcInfo );
                                                POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                                pMsgLogItem = nullptr;
                                            }
                                        }
                                    } // if( m_pIpcSocketWrapper->socketState() != ESocketStateUnconnected )

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
                                            if( m_arpTrcMsgLogObjects.size() > 0 )
                                            {
                                                strAddTrcInfo = "-+ IpcSocket::disconnectFromHost( " + m_socketDscr.getConnectionString() + " )";

                                                for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                                                {
                                                    CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                                        /* pObjSender   */ this,
                                                        /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                                        /* transmitDir  */ ETransmitDirUndefined,
                                                        /* bBold        */ false,
                                                        /* strMsg       */ strAddTrcInfo );
                                                    POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                                    pMsgLogItem = nullptr;
                                                }
                                            }

                                            m_pIpcSocketWrapper->disconnectFromHost();

                                            if( m_arpTrcMsgLogObjects.size() > 0 )
                                            {
                                                strAddTrcInfo = "+- IpcSocket::disconnectFromHost( SUCCESS )";

                                                for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                                                {
                                                    CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                                        /* pObjSender   */ this,
                                                        /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                                        /* transmitDir  */ ETransmitDirUndefined,
                                                        /* bBold        */ false,
                                                        /* strMsg       */ strAddTrcInfo );
                                                    POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                                    pMsgLogItem = nullptr;
                                                }
                                            }
                                        } // if( m_pIpcSocketWrapper->socketState() == ESocketStateConnected )

                                        else if( m_pIpcSocketWrapper->socketState() == ESocketStateConnecting )
                                        {
                                            if( m_arpTrcMsgLogObjects.size() > 0 )
                                            {
                                                strAddTrcInfo = "-+ IpcSocket::abort( " + m_socketDscr.getConnectionString() + " )";

                                                for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                                                {
                                                    CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                                        /* pObjSender   */ this,
                                                        /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                                        /* transmitDir  */ ETransmitDirUndefined,
                                                        /* bBold        */ false,
                                                        /* strMsg       */ strAddTrcInfo );
                                                    POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                                    pMsgLogItem = nullptr;
                                                }
                                            }

                                            m_pIpcSocketWrapper->abort();

                                            if( m_arpTrcMsgLogObjects.size() > 0 )
                                            {
                                                strAddTrcInfo = "+- IpcSocket::abort( SUCCESS )";

                                                for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                                                {
                                                    CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                                                        /* pObjSender   */ this,
                                                        /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                                                        /* transmitDir  */ ETransmitDirUndefined,
                                                        /* bBold        */ false,
                                                        /* strMsg       */ strAddTrcInfo );
                                                    POST_OR_DELETE_MESSAGE(pMsgLogItem, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                                    pMsgLogItem = nullptr;
                                                }
                                            }
                                        } // else if( m_pIpcSocketWrapper->socketState() == ESocketStateConnecting )
                                    } // if( m_pIpcSocketWrapper->getRemoteHostName() != m_socketDscr.m_strRemoteHostName ..
                                } // if( m_pIpcSocketWrapper->socketType() == m_socketDscr.m_socketType )
                            } // if( m_pIpcSocketWrapper != nullptr )

                            if( m_pIpcSocketWrapper == nullptr )
                            {
                                switch( m_socketDscr.m_socketType )
                                {
                                    case ESocketTypeTcp:
                                    {
                                        m_pIpcSocketWrapper = new CTcpSocketWrapper( objectName(), ESrvCltTypeClient, 0, nullptr, m_pTrcMthFile, m_iTrcMthFileDetailLevel );
                                        break;
                                    }
                                    case ESocketTypeShm:
                                    {
                                        m_pIpcSocketWrapper = new CShmSocketWrapper( objectName(), ESrvCltTypeClient, 0, m_pTrcMthFile, m_iTrcMthFileDetailLevel );
                                        break;
                                    }
                                    case ESocketTypeInProcMsg:
                                    {
                                        m_pIpcSocketWrapper = new CInProcMsgSocketWrapper( objectName(), ESrvCltTypeClient, 0, nullptr, m_pTrcMthFile, m_iTrcMthFileDetailLevel );
                                        break;
                                    }
                                    default:
                                    {
                                        errResultInfo.setSeverity(EResultSeverityError);
                                        errResultInfo.setResult(EResultArgOutOfRange);
                                        errResultInfo.setAddErrInfoDscr( "Unknown Socket Type " + socketType2Str(m_socketDscr.m_socketType) );
                                        break;
                                    }
                                } // switch( m_socketDscr.m_socketType )

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
                            } // if( m_pIpcSocketWrapper == nullptr )
                        } // if( hostSettingsNew != hostSettingsOld )

                        bRequestFinished = true;

                    } // if( m_requestInProgress == ERequestNone )

                    if( bRequestFinished && m_requestInProgress == ERequestChangeSettings )
                    {
                        if( m_pMsgCon != nullptr )
                        {
                            m_pMsgCon->setErrResultInfo(errResultInfo);
                            m_pMsgCon->setProgress(100);
                            CMsgConChangeSettings* pMsgCon = dynamic_cast<CMsgConChangeSettings*>(m_pMsgCon);
                            if( pMsgCon != nullptr )
                            {
                                pMsgCon->setClientSocketState(m_socketDscr.m_socketState);
                            }
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

                            if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
                            {
                                mthTracer.trace( "-+ ReqChangeSettings.WaitCondition.wakeAll()" );
                            }

                            if( !pReqExecTree->wake(m_iReqIdInProgress) )
                            {
                                errResultInfo.setSeverity(EResultSeverityError);
                                errResultInfo.setResult(EResultInternalStateMachineError);
                                errResultInfo.setAddErrInfoDscr("Waking up thread waiting for change settings request failed");

                                if( m_pErrLog != nullptr )
                                {
                                    m_pErrLog->addEntry(errResultInfo);
                                }
                            }

                            if( isMethodTraceActive(ETraceDetailLevelInternalStates) )
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

                        m_requestInProgress   = ERequestNone;
                        m_iReqIdInProgress    = -1;
                        m_iMsgIdReqInProgress = -1;

                    } // if( bRequestFinished && m_requestInProgress == ERequestChangeSettings )
                    break;
                } // case EMsgTypeReqChangeSettings

                case EMsgTypeReqSendData:
                {
                    CMsgReqSendData* pMsgReq = dynamic_cast<CMsgReqSendData*>(i_pMsg);

                    if( pMsgReq == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReq->getSender() != m_pSrvClt && pMsgReq->getSender() != this ) // the watch dog data block is sent by myself
                    {
                        throw CException(__FILE__,__LINE__,EResultInvalidSenderObjInMessage);
                    }

                    if( m_pIpcSocketWrapper != nullptr && m_pIpcSocketWrapper->socketState() == ESocketStateConnected && pMsgReq->getByteArray().size() > 0 )
                    {
                        m_requestInProgress   = ERequestSendData;
                        m_iReqIdInProgress    = pMsgReq->getRequestId();
                        m_iMsgIdReqInProgress = pMsgReq->getMsgId();

                        if( m_pBlkType != nullptr )
                        {
                            m_pBlkType->writeDataBlock(
                               /* srvCltType          */ m_srvCltType,
                               /* pObjGtw             */ this,
                               /* pObjSrvClt          */ m_pSrvClt,
                               /* pSocketWrapper      */ m_pIpcSocketWrapper,
                               /* byteArr             */ pMsgReq->getByteArray(),
                               /* isWatchDog          */ false,
                               /* pTrcAdminObj        */ &mthTracer,
                               /* arpTrcMsgLogObjects */ m_arpTrcMsgLogObjects );

                        } // if( m_pBlkType != nullptr )

                        m_requestInProgress   = ERequestNone;
                        m_iReqIdInProgress    = -1;
                        m_iMsgIdReqInProgress = -1;

                        // Please note that the socket may have been closed while sending data
                        // emitting the "error" or "disconnected" signals (see corresponding slots).
                        if( m_pIpcSocketWrapper->socketState() == ESocketStateUnconnected )
                        {
                            onDisconnected(m_pIpcSocketWrapper);
                        }

                    } // if( m_pIpcSocketWrapper != nullptr && m_pIpcSocketWrapper->socketState() == ESocketStateConnected && pMsgReq->getByteArray().size() > 0 )
                    break;
                } // case EMsgTypeReqSendData

                case EMsgTypeReqReceiveData:
                {
                    CMsgReqReceiveData* pMsgReq = dynamic_cast<CMsgReqReceiveData*>(i_pMsg);

                    if( pMsgReq == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }

                    if( m_pIpcSocketWrapper->socketState() == ESocketStateConnected )
                    {
                        m_requestInProgress   = ERequestReceiveData;
                        m_iReqIdInProgress    = pMsgReq->getRequestId();
                        m_iMsgIdReqInProgress = pMsgReq->getMsgId();

                        if( m_pBlkType != nullptr )
                        {
                            QList<QByteArray> arByteArrs = m_pBlkType->receiveDataBlocks(
                               /* srvCltType          */ m_srvCltType,
                               /* pObjGtw             */ this,
                               /* pObjSrvClt          */ m_pSrvClt,
                               /* pSocketWrapper      */ m_pIpcSocketWrapper,
                               /* pByteArrWatchDog    */ m_pByteArrWatchDog,
                               /* pTrcAdminObj        */ &mthTracer,
                               /* arpTrcMsgLogObjects */ m_arpTrcMsgLogObjects );

                            for( int idxBlk = 0; idxBlk < arByteArrs.size(); idxBlk++ )
                            {
                                Ipc::CMsgIndReceivedData* pMsgInd = new Ipc::CMsgIndReceivedData(
                                    /* pObjSender   */ this,
                                    /* pObjReceiver */ m_pSrvClt,
                                    /* iSocketId    */ m_pIpcSocketWrapper->getSocketId(),
                                    /* byteArr      */ arByteArrs[idxBlk] );
                                POST_OR_DELETE_MESSAGE(pMsgInd, &mthTracer, ETraceDetailLevelRuntimeInfo);
                                pMsgInd = nullptr;
                            }
                        } // if( m_pBlkType != nullptr )

                        m_requestInProgress   = ERequestNone;
                        m_iReqIdInProgress    = -1;
                        m_iMsgIdReqInProgress = -1;

                        // Please note that the socket may have been closed while waiting for data
                        // emitting the "error" or "disconnected" signals (see corresponding slots).
                        if( m_pIpcSocketWrapper->socketState() == ESocketStateUnconnected )
                        {
                            onDisconnected(m_pIpcSocketWrapper);
                        }
                        else if( m_pIpcSocketWrapper->bytesAvailable() > 0 )
                        {
                            CMsgReqReceiveData* pMsgReq = new CMsgReqReceiveData(
                                /* pObjSender       */ this,
                                /* pObjReceiver     */ this,
                                /* iSocketId        */ m_pIpcSocketWrapper->getSocketId(),
                                /* bMustBeConfirmed */ false,
                                /* iReqId           */ -1 );
                            POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ETraceDetailLevelRuntimeInfo);
                            pMsgReq = nullptr;
                        }
                    } // if( m_pIpcSocketWrapper->socketState() == ESocketStateConnected )
                    break;
                } // case EMsgTypeReqReceiveData

                default:
                {
                    break;
                }
            } //switch( pMsg->type() )
        } // if( pMsg != nullptr )
    } // if( i_pMsg->type() >= QEvent::User )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pMsg);
    }
    return bEventHandled;

} // event
