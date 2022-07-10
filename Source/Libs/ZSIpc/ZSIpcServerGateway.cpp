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
#include <QtCore/qtimer.h>
#include <QtCore/qwaitcondition.h>
#include <QtNetwork/qhostinfo.h>

#include "ZSIpc/ZSIpcServerGateway.h"
#include "ZSIpc/ZSIpcBlkTypeL.h"
#include "ZSIpc/ZSIpcServer.h"
#include "ZSIpc/ZSIpcServerWrapperInProcMsg.h"
#include "ZSIpc/ZSIpcServerWrapperShm.h"
#include "ZSIpc/ZSIpcServerWrapperTcp.h"
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
#include "ZSSys/ZSSysTrcMthFile.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;


/*******************************************************************************
class CServerGatewayThread : public CSrvCltBaseGatewayThread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CServerGatewayThread::CServerGatewayThread(
    const QString& i_strObjNameGateway,
    CServer* i_pServer,
    CErrLog* i_pErrLog,
    EMethodTraceDetailLevel i_eTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    CSrvCltBaseGatewayThread(i_strObjNameGateway, i_pErrLog, i_eTrcMthFileDetailLevel),
    m_pServer(i_pServer)
{
    // If the parent is the trace server the detail level of trace outputs may not be
    // controlled by trace admin objects as they belong to the trace server itself.
    if( i_pServer->objectName().endsWith("TrcServer") )
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
        /* strAddInfo         */ "" );

} // ctor

//------------------------------------------------------------------------------
CServerGatewayThread::~CServerGatewayThread()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strAddInfo         */ "" );

    if( m_pTrcMthFile != nullptr )
    {
        m_pTrcMthFile->close();
        CTrcMthFile::Free(m_pTrcMthFile);
    }

    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();

        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;
    }

    m_pServer = nullptr;

} // dtor

/*==============================================================================
protected: // Template of base class ZS::Ipc::CSrvCltBaseGatewayThread which MUST be overwritten
==============================================================================*/

//------------------------------------------------------------------------------
void CServerGatewayThread::run()
//------------------------------------------------------------------------------
{
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

    m_pGateway = new CServerGateway(
        /* szObjName              */ m_strObjNameGateway,
        /* pServer                */ m_pServer,
        /* pThread                */ this,
        /* pModelErrLog           */ m_pErrLog,
        /* iTrcMthFileDetailLevel */ m_eTrcMthFileDetailLevel );

    //--------------------------------------------------------------------------
    // Please see comment at template run method of base class CSrvCltBaseGatewayThread.
    //--------------------------------------------------------------------------

    if( m_pWaitCondition != nullptr )
    {
        // The thread starting object must have called "wait" on the
        // wait condition. Otherwise "wakeAll" may never return. To ensure
        // this we force this thread to sleep for a moment ...
        msleep(10);

        if( isTraceRuntimeInfoActive(ELogDetailLevel::DebugNormal) )
        {
            mthTracer.trace( "-+ WaitCondition.wakeAll()" );
        }

        m_pWaitCondition->wakeAll();

        if( isTraceRuntimeInfoActive(ELogDetailLevel::DebugNormal) )
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
            if( pReqExecTree->findRequest(m_iReqIdStartThread) != nullptr )
            {
                while( !pReqExecTree->isWaiting(m_iReqIdStartThread) && !pReqExecTree->waitTimedOut(m_iReqIdStartThread) )
                {
                    msleep(10);
                }
            }

            if( isTraceRuntimeInfoActive(ELogDetailLevel::DebugNormal) )
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

            if( isTraceRuntimeInfoActive(ELogDetailLevel::DebugNormal) )
            {
                mthTracer.trace( "+- ReqStartThread.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
            }
        } // if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(m_iReqIdStartThread) )

        if( !bIsBlockingRequest )
        {
            CMsgConStartup* pMsgConStartup = new CMsgConStartup(
                /* pObjSender        */ m_pGateway,
                /* pObjReceiver      */ m_pServer,
                /* iReqId            */ m_iReqIdStartThread,
                /* iMsgIdReq         */ -1,
                /* errResultInfo     */ errResultInfo,
                /* iProgress_perCent */ 100 );
            POST_OR_DELETE_MESSAGE(pMsgConStartup, &mthTracer, ELogDetailLevel::DebugNormal);
            pMsgConStartup = nullptr;
        }
    } // if( m_iReqIdStartThread >= 0 )

    try
    {
        if( isTraceRuntimeInfoActive(ELogDetailLevel::DebugNormal) )
        {
            mthTracer.trace( "-+ QThread::exec()" );
        }

        exec();

        if( isTraceRuntimeInfoActive(ELogDetailLevel::DebugNormal) )
        {
            mthTracer.trace( "+- QThread::exec()" );
        }
    }
    catch( ZS::System::CException* )
    {
        // This type of exception has already been logged on throwing (creating) the exception.
    }
    catch( ZS::System::CException& )
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
class CServerGateway : public CSrvCltBaseGateway
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CServerGateway::CServerGateway(
    const QString& i_strObjName,
    CServer* i_pServer,
    CServerGatewayThread* i_pThreadGateway,
    CErrLog* i_pErrLog,
    EMethodTraceDetailLevel i_eTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    CSrvCltBaseGateway(
        /* szObjName              */ i_strObjName,
        /* srvCltType             */ ESrvCltTypeServer,
        /* pCltSrv                */ i_pServer,
        /* pThreadGateway         */ i_pThreadGateway,
        /* pModelErrLog           */ i_pErrLog,
        /* iTrcMthFileDetailLevel */ i_eTrcMthFileDetailLevel ),
    m_hostSettings(),
    m_pIpcServerWrapper(nullptr),
    m_arpIpcSocketWrapper()
{
    // The derived classes must instantiate the trace admin object and trace the ctor.
    if( m_eTrcMthFileDetailLevel > EMethodTraceDetailLevel::None )
    {
        QString strLocalTrcFileAbsFilePath = CTrcServer::GetLocalTrcFileAbsoluteFilePath();
        m_pTrcMthFile = CTrcMthFile::Alloc(strLocalTrcFileAbsFilePath);
    }
    // If the parent is the trace server the detail level of trace outputs may not be
    // controlled by trace admin objects as they belong to the trace server itself.
    // If the trace admin object would be created there the gateway will registered the
    // trace admin object at the server which would like to send the trace admin object
    // to the client through this gateway. But the server waits for the gateway thread
    // to be started creating this gateway instance. And this gateway would now wait
    // for the trace admin object to be registered. The gateway waits for the server and
    // the server waits for the gateway -> Deadlock.
    else if( !i_pServer->objectName().endsWith("TrcServer") )
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
        /* strAddInfo         */ "" );

} // ctor

//------------------------------------------------------------------------------
CServerGateway::~CServerGateway()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strAddInfo         */ "" );

    int iSocketId;

    if( m_arpIpcSocketWrapper.size() > 0 )
    {
        for( iSocketId = m_arpIpcSocketWrapper.size()-1; iSocketId >= 0; iSocketId-- )
        {
            try
            {
                delete m_arpIpcSocketWrapper[iSocketId];
            }
            catch(...)
            {
            }
            m_arpIpcSocketWrapper[iSocketId] = nullptr;
        }
    }
    m_arpIpcSocketWrapper.clear();

    if( m_pIpcServerWrapper != nullptr )
    {
        if( m_pIpcServerWrapper->isListening() )
        {
            m_pIpcServerWrapper->close();
        }
    }
    try
    {
        delete m_pIpcServerWrapper;
    }
    catch(...)
    {
    }
    m_pIpcServerWrapper = nullptr;

    if( m_pTrcMthFile != nullptr )
    {
        m_pTrcMthFile->close();
        CTrcMthFile::Free(m_pTrcMthFile);
    }

    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();

        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;
    }

} // dtor

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CServerGateway::onNewConnectionPending( QObject* /*i_pServerWrapper*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "RequestInProgress: " + request2Str(m_requestInProgress);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onNewConnectionPending",
        /* strAddInfo         */ strAddTrcInfo );

    CIpcSocketWrapper* pIpcSocketWrapper = m_pIpcServerWrapper->nextPendingConnection();

    if( pIpcSocketWrapper == nullptr )
    {
        QString strAddErrInfo = "newConnectionPending( ";
        strAddErrInfo += "pIpcSocketWrapper == nullptr )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddErrInfo);
    }

    int iSocketId;

    // Search free entry in list of sockets ..
    for( iSocketId = 0; iSocketId < m_arpIpcSocketWrapper.size(); iSocketId++ )
    {
        if( m_arpIpcSocketWrapper[iSocketId] == nullptr )
        {
            break;
        }
    }
    if( iSocketId >= m_arpIpcSocketWrapper.size() )
    {
        m_arpIpcSocketWrapper.append(pIpcSocketWrapper);
    }
    else
    {
        m_arpIpcSocketWrapper[iSocketId] = pIpcSocketWrapper;
    }

    pIpcSocketWrapper->setSocketId(iSocketId);

    if( !QObject::connect(
        /* pObjSender   */ m_arpIpcSocketWrapper[iSocketId],
        /* szSignal     */ SIGNAL(disconnected(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDisconnected(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_arpIpcSocketWrapper[iSocketId],
        /* szSignal     */ SIGNAL(readyRead(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onReadyRead(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_arpIpcSocketWrapper[iSocketId],
        /* szSignal     */ SIGNAL(error(QObject*,ZS::System::SErrResultInfo&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onError(QObject*,ZS::System::SErrResultInfo&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    CMsgIndConnected* pMsgInd = new CMsgIndConnected(
        /* pObjSender   */ this,
        /* pObjReceiver */ m_pSrvClt,
        /* socketDscr   */ pIpcSocketWrapper->getSocketDscr() );
    POST_OR_DELETE_MESSAGE(pMsgInd, &mthTracer, ELogDetailLevel::DebugNormal);
    pMsgInd = nullptr;

    //if( m_pBlkType->hasConnectionGreetingBlock(m_srvCltType) )
    //{
    //    QByteArray* pByteArrGreeting = m_pBlkType->createConnectionGreetingBlock(m_srvCltType);

    //    bool bOk = (pIpcSocketWrapper->write(*pByteArrGreeting) == pByteArrGreeting->size());

    //    delete pByteArrGreeting;
    //    pByteArrGreeting = nullptr;

    //    if( !bOk )
    //    {
    //        if( CErrLog::GetInstance() != nullptr )
    //        {
    //            SErrResultInfo errResultInfo = pIpcSocketWrapper->errResultInfo();
    //            CErrLog::GetInstance()->addEntry(errResultInfo);
    //        }
    //    }
    //} // if( m_pBlkType->hasConnectionGreetingBlock(m_srvCltType) )

} // onNewConnectionPending

/*==============================================================================
protected slots: // must overridables of base class CSrvCltBaseGateway
==============================================================================*/

//------------------------------------------------------------------------------
void CServerGateway::onDisconnected( QObject* i_pSocketWrapper )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "RequestInProgress: " + request2Str(m_requestInProgress);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onDisconnected",
        /* strAddInfo         */ strAddTrcInfo );

    CIpcSocketWrapper* pIpcSocketWrapper = dynamic_cast<CIpcSocketWrapper*>(i_pSocketWrapper);

    if( pIpcSocketWrapper == nullptr )
    {
        QString strAddInfo = "disconnected( ";
        strAddInfo += "pIpcSocketWrapper == nullptr )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
    }

    int iSocketId = pIpcSocketWrapper->getSocketId();

    if( iSocketId < 0 || iSocketId >= m_arpIpcSocketWrapper.size() )
    {
        QString strAddInfo = "disconnected( ";
        strAddInfo += "SocketId = " + QString::number(iSocketId) + " > " + QString::number(m_arpIpcSocketWrapper.size()) + " )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
    }
    if( m_arpIpcSocketWrapper[iSocketId] != i_pSocketWrapper )
    {
        QString strAddInfo = "disconnected( ";
        strAddInfo += "m_arpIpcSocketWrapper[" + QString::number(iSocketId) + "] != i_pSocketWrapper )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
    }

    switch( m_requestInProgress )
    {
        case ERequestNone:
        case ERequestConnect:
        case ERequestDisconnect:
        case ERequestChangeSettings:
        {
            if( !QObject::disconnect(
                /* pObjSender   */ m_arpIpcSocketWrapper[iSocketId],
                /* szSignal     */ SIGNAL(disconnected(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDisconnected(QObject*)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::disconnect(
                /* pObjSender   */ m_arpIpcSocketWrapper[iSocketId],
                /* szSignal     */ SIGNAL(readyRead(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onReadyRead(QObject*)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::disconnect(
                /* pObjSender   */ m_arpIpcSocketWrapper[iSocketId],
                /* szSignal     */ SIGNAL(error(QObject*,ZS::System::SErrResultInfo&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onError(QObject*,ZS::System::SErrResultInfo&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            SSocketDscr socketDscr = pIpcSocketWrapper->getSocketDscr();

            try
            {
                delete m_arpIpcSocketWrapper[iSocketId];
            }
            catch(...)
            {
            }
            m_arpIpcSocketWrapper[iSocketId] = nullptr;

            CMsgIndDisconnected* pMsgInd = new CMsgIndDisconnected(
                /* pObjSender   */ this,
                /* pObjReceiver */ m_pSrvClt,
                /* socketDscr   */ socketDscr );
            POST_OR_DELETE_MESSAGE(pMsgInd, &mthTracer, ELogDetailLevel::DebugNormal);
            pMsgInd = nullptr;
            break;
        }
        case ERequestShutdown:
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
        case ERequestStartup:
        default:
        {
            throw CException(__FILE__,__LINE__,EResultMethodStateMismatch,request2Str(m_requestInProgress));
        }
    }

} // onDisconnected

//------------------------------------------------------------------------------
void CServerGateway::onReadyRead( QObject* i_pSocketWrapper )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "RequestInProgress: " + request2Str(m_requestInProgress);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onReadyRead",
        /* strAddInfo         */ strAddTrcInfo );

    CIpcSocketWrapper* pIpcSocketWrapper = dynamic_cast<CIpcSocketWrapper*>(i_pSocketWrapper);

    if( pIpcSocketWrapper == nullptr )
    {
        QString strAddErrInfo = "readyRead( pIpcSocketWrapper == nullptr )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddErrInfo);
    }

    CMsgReqReceiveData* pMsgReq = new CMsgReqReceiveData(
        /* pObjSender       */ this,
        /* pObjReceiver     */ this,
        /* iSocketId        */ pIpcSocketWrapper->getSocketId(),
        /* bMustBeConfirmed */ false,
        /* iReqId           */ -1 );
    POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ELogDetailLevel::DebugNormal);
    pMsgReq = nullptr;

} // onReadyRead

//------------------------------------------------------------------------------
void CServerGateway::onError( QObject* i_pSocketWrapper, ZS::System::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo  =  i_errResultInfo.getResultStr();
        strAddTrcInfo += ", RequestInProgress: " + request2Str(m_requestInProgress);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onError",
        /* strAddInfo         */ strAddTrcInfo );

    CIpcSocketWrapper* pIpcSocketWrapper = dynamic_cast<CIpcSocketWrapper*>(i_pSocketWrapper);

    if( pIpcSocketWrapper == nullptr )
    {
        QString strAddErrInfo = "onError( ";
        strAddErrInfo += "pIpcSocketWrapper == nullptr )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddErrInfo);
    }

    SErrResultInfo      errResultInfo = i_errResultInfo;
    ZS::System::EResult result = i_errResultInfo.getResult();

    if( errResultInfo.getErrSource().isObjectPathEmpty() )
    {
        errResultInfo.setErrSource(nameSpace(), className(), objectName(), "onError");
    }

    if( pIpcSocketWrapper->getRemoteHostName().isEmpty() )
    {
        errResultInfo.setAddErrInfoDscr( "Connection with ---:" + QString::number(pIpcSocketWrapper->getServerListenPort()) );
    }
    else
    {
        errResultInfo.setAddErrInfoDscr( "Connection with " + pIpcSocketWrapper->getRemoteHostName() + ":" + QString::number(pIpcSocketWrapper->getServerListenPort()) );
    }

    switch( result )
    {
        // If the remote client closed the connection ..
        case EResultRemoteHostClosed:
        {
            // .. its not a real error. The connection will be closed calling the disconnected slot.
            result = EResultSuccess;
        }
        default:
        {
            break;
        }
    }

    switch( m_requestInProgress )
    {
        case ERequestNone:
        case ERequestStartup:
        case ERequestShutdown:
        case ERequestChangeSettings:
        {
            break;
        }
        case ERequestReceiveData:
        {
            switch( pIpcSocketWrapper->error() )
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
        }
        case ERequestSendData:
        {
            break;
        }
        case ERequestConnect:
        case ERequestDisconnect:
        default:
        {
            throw CException(__FILE__,__LINE__,EResultInternalProgramError,request2Str(m_requestInProgress));
        }
    }

    if( result != EResultSuccess )
    {
        CMsgErr* pMsgErr = new CMsgErr(
            /* pObjSender    */ this,
            /* pObjReceiver  */ m_pSrvClt,
            /* errResultInfo */ errResultInfo );
        POST_OR_DELETE_MESSAGE(pMsgErr, &mthTracer, ELogDetailLevel::DebugNormal);
        pMsgErr = nullptr;
    }

} // onError

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CServerGateway::event( QEvent* i_pMsg )
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
            /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ nameSpace(),
            /* strClassName       */ className(),
            /* strObjName         */ objectName(),
            /* strMethod          */ "event",
            /* strAddInfo         */ strAddTrcInfo );

        if( pMsg != nullptr )
        {
            bEventHandled = true;

            SErrResultInfo errResultInfo = ErrResultInfoSuccess( pMsg->msgTypeToStr() );

            CRequestExecTree* pReqExecTree = CRequestExecTree::GetInstance();

            switch( static_cast<int>(pMsg->type()) )
            {
                case EMsgTypeReqStartup:
                {
                    CMsgReqStartup* pMsgReq = dynamic_cast<CMsgReqStartup*>(i_pMsg);

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

                    bool bRequestFinished = false;

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
                        m_requestInProgress   = ERequestStartup;
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
                        }
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

                        m_hostSettings = pMsgReq->getHostSettings();

                        switch( m_hostSettings.m_socketType )
                        {
                            case ESocketTypeTcp:
                            {
                                m_pIpcServerWrapper = new CTcpServerWrapper(objectName(), m_pTrcMthFile, m_eTrcMthFileDetailLevel);
                                break;
                            }
                            case ESocketTypeShm:
                            {
                                m_pIpcServerWrapper = new CShmServerWrapper(objectName(), m_pTrcMthFile, m_eTrcMthFileDetailLevel);
                                break;
                            }
                            case ESocketTypeInProcMsg:
                            {
                                m_pIpcServerWrapper = new CInProcMsgServerWrapper(objectName(), m_pTrcMthFile, m_eTrcMthFileDetailLevel);
                                break;
                            }
                            default:
                            {
                                break;
                            }
                        }

                        if( !QObject::connect(
                            /* pObjSender   */ m_pIpcServerWrapper,
                            /* szSignal     */ SIGNAL(newConnection(QObject*)),
                            /* pObjReceiver */ this,
                            /* szSlot       */ SLOT(onNewConnectionPending(QObject*)) ) )
                        {
                            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                        }

                        m_pIpcServerWrapper->setMaxPendingConnections( m_hostSettings.m_uMaxPendingConnections );
                        m_pIpcServerWrapper->setLocalHostName(m_hostSettings.m_strLocalHostName);
                        m_pIpcServerWrapper->setLocalPort(m_hostSettings.m_uLocalPort);

                        if( !m_pIpcServerWrapper->listen() )
                        {
                            errResultInfo = m_pIpcServerWrapper->errResultInfo();
                            errResultInfo.setAddErrInfoDscr( m_hostSettings.m_strLocalHostName + ":" + QString::number(m_hostSettings.m_uLocalPort) );
                        }

                        bRequestFinished = true;

                    } // if( m_requestInProgress == ERequestNone )

                    if( bRequestFinished && m_requestInProgress == ERequestStartup )
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

                            if( isTraceRuntimeInfoActive(ELogDetailLevel::DebugNormal) )
                            {
                                mthTracer.trace( "-+ ReqStartup.WaitCondition.wakeAll()" );
                            }

                            if( !pReqExecTree->wake(m_iReqIdInProgress) )
                            {
                                errResultInfo.setSeverity(EResultSeverityError);
                                errResultInfo.setResult(EResultInternalStateMachineError);
                                errResultInfo.setAddErrInfoDscr("Waking up thread waiting for startup request failed");

                                if( m_pErrLog != nullptr )
                                {
                                    m_pErrLog->addEntry(errResultInfo);
                                }
                            }

                            if( isTraceRuntimeInfoActive(ELogDetailLevel::DebugNormal) )
                            {
                                mthTracer.trace( "+- ReqStartup.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
                            }
                        } // if( bIsBlockingRequest )

                        else if( m_pMsgCon != nullptr )
                        {
                            POST_OR_DELETE_MESSAGE(m_pMsgCon, &mthTracer, ELogDetailLevel::DebugNormal);
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

                    } // if( bRequestFinished && m_requestInProgress == ERequestStartup )
                    break;
                } // case EMsgTypeReqStartup:

                case EMsgTypeReqShutdown:
                {
                    CMsgReqShutdown* pMsgReq = dynamic_cast<CMsgReqShutdown*>(i_pMsg);
                    int              iSocketId;

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

                    bool bRequestFinished = false;

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

                    if( m_arpIpcSocketWrapper.size() > 0 )
                    {
                        for( iSocketId = m_arpIpcSocketWrapper.size()-1; iSocketId >= 0; iSocketId-- )
                        {
                            try
                            {
                                delete m_arpIpcSocketWrapper[iSocketId];
                            }
                            catch(...)
                            {
                            }
                            m_arpIpcSocketWrapper[iSocketId] = nullptr;
                        }
                        m_arpIpcSocketWrapper.clear();
                    }

                    m_pIpcServerWrapper->close();

                    if( !QObject::disconnect(
                        /* pObjSender   */ m_pIpcServerWrapper,
                        /* szSignal     */ SIGNAL(newConnection(QObject*)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onNewConnectionPending(QObject*)) ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }

                    try
                    {
                        delete m_pIpcServerWrapper;
                    }
                    catch(...)
                    {
                    }
                    m_pIpcServerWrapper = nullptr;

                    bRequestFinished = true;

                    if( bRequestFinished && m_requestInProgress == ERequestShutdown )
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

                            if( isTraceRuntimeInfoActive(ELogDetailLevel::DebugNormal) )
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

                            if( isTraceRuntimeInfoActive(ELogDetailLevel::DebugNormal) )
                            {
                                mthTracer.trace( "+- ReqShudown.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
                            }
                        } // if( bIsBlockingRequest )

                        else if( m_pMsgCon != nullptr )
                        {
                            POST_OR_DELETE_MESSAGE(m_pMsgCon, &mthTracer, ELogDetailLevel::DebugNormal);
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

                    } // if( bRequestFinished && m_requestInProgress == ERequestShutdown )
                    break;
                } // case EMsgTypeReqShutdown:

                case EMsgTypeReqChangeSettings:
                {
                    CMsgReqChangeSettings* pMsgReq = dynamic_cast<CMsgReqChangeSettings*>(i_pMsg);

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

                    bool bRequestFinished = false;

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
                        }
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

                        SServerHostSettings hostSettingsNew = pMsgReq->getServerHostSettings();

                        if( hostSettingsNew != m_hostSettings )
                        {
                            int iSocketId;

                            if( m_arpIpcSocketWrapper.size() > 0 )
                            {
                                for( iSocketId = m_arpIpcSocketWrapper.size()-1; iSocketId >= 0; iSocketId-- )
                                {
                                    if( m_arpIpcSocketWrapper[iSocketId] != nullptr )
                                    {
                                        onDisconnected(m_arpIpcSocketWrapper[iSocketId]);
                                    }

                                    try
                                    {
                                        delete m_arpIpcSocketWrapper[iSocketId];
                                    }
                                    catch(...)
                                    {
                                    }
                                    m_arpIpcSocketWrapper[iSocketId] = nullptr;
                                }
                                m_arpIpcSocketWrapper.clear();
                            }

                            m_pIpcServerWrapper->close();

                            m_hostSettings = hostSettingsNew;

                            if( m_pIpcServerWrapper->socketType() != m_hostSettings.m_socketType )
                            {
                                try
                                {
                                    delete m_pIpcServerWrapper;
                                }
                                catch(...)
                                {
                                    m_pIpcServerWrapper = nullptr;
                                }
                                switch( m_hostSettings.m_socketType )
                                {
                                    case ESocketTypeTcp:
                                    {
                                        m_pIpcServerWrapper = new CTcpServerWrapper(objectName());
                                        break;
                                    }
                                    case ESocketTypeShm:
                                    {
                                        m_pIpcServerWrapper = new CShmServerWrapper(objectName());
                                        break;
                                    }
                                    case ESocketTypeInProcMsg:
                                    {
                                        m_pIpcServerWrapper = new CInProcMsgServerWrapper(objectName());
                                        break;
                                    }
                                    default:
                                    {
                                        break;
                                    }
                                }

                                if( !QObject::connect(
                                    /* pObjSender   */ m_pIpcServerWrapper,
                                    /* szSignal     */ SIGNAL(newConnection(QObject*)),
                                    /* pObjReceiver */ this,
                                    /* szSlot       */ SLOT(onNewConnectionPending(QObject*)) ) )
                                {
                                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                                }
                            }

                            m_pIpcServerWrapper->setMaxPendingConnections(m_hostSettings.m_uMaxPendingConnections);
                            m_pIpcServerWrapper->setLocalHostName(m_hostSettings.m_strLocalHostName);
                            m_pIpcServerWrapper->setLocalPort(m_hostSettings.m_uLocalPort);

                        } // if( m_hostSettings != pMsgReq->getSocketDscr() )

                        if( !m_pIpcServerWrapper->isListening() )
                        {
                            if( !m_pIpcServerWrapper->listen() )
                            {
                                errResultInfo = m_pIpcServerWrapper->errResultInfo();

                                if( errResultInfo.getErrSource().isObjectPathEmpty() )
                                {
                                    errResultInfo.setErrSource(nameSpace(), className(), objectName(), pMsg->msgTypeToStr());
                                }
                                errResultInfo.setAddErrInfoDscr( m_hostSettings.m_strLocalHostName + ":" + QString::number(m_hostSettings.m_uLocalPort) );
                            }
                        }

                        bRequestFinished = true;

                    } // if( m_requestInProgress == ERequestNone )

                    if( bRequestFinished && m_requestInProgress == ERequestChangeSettings )
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

                            if( isTraceRuntimeInfoActive(ELogDetailLevel::DebugNormal) )
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

                            if( isTraceRuntimeInfoActive(ELogDetailLevel::DebugNormal) )
                            {
                                mthTracer.trace( "+- ReqChangeSettings.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
                            }
                        } // if( bIsBlockingRequest )

                        else if( m_pMsgCon != nullptr )
                        {
                            POST_OR_DELETE_MESSAGE(m_pMsgCon, &mthTracer, ELogDetailLevel::DebugNormal);
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

                    } // if( bRequestFinished )
                    break;
                } // case EMsgTypeReqChangeSettings:

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

                    strAddErrInfo = "Sending data to clients";

                    if( m_requestInProgress == ERequestNone && m_arpIpcSocketWrapper.size() > 0 )
                    {
                        m_requestInProgress   = ERequestSendData;
                        m_iReqIdInProgress    = pMsgReq->getRequestId();
                        m_iMsgIdReqInProgress = pMsgReq->getMsgId();

                        bool               bSocketIdIsValid = true;
                        int                iSocketId = pMsgReq->getSocketId();
                        int                iSocketIdMin = 0;
                        int                iSocketIdMax = m_arpIpcSocketWrapper.size()-1;
                        int                iSockets = 0;
                        CIpcSocketWrapper* pIpcSocketWrapper = nullptr;

                        if( iSocketId != ESocketIdAllSockets )
                        {
                            if( iSocketId < 0 || iSocketId >= m_arpIpcSocketWrapper.size() )
                            {
                                bSocketIdIsValid = false;
                            }
                            else if( m_arpIpcSocketWrapper[iSocketId] == nullptr )
                            {
                                bSocketIdIsValid = false;
                            }
                            else if( iSocketId != m_arpIpcSocketWrapper[iSocketId]->getSocketId() )
                            {
                                bSocketIdIsValid = false;
                            }
                            iSocketIdMin = static_cast<unsigned int>(iSocketId);
                            iSocketIdMax = static_cast<unsigned int>(iSocketId);

                        } // if( iSocketId != ESocketIdAllSockets )

                        if( bSocketIdIsValid )
                        {
                            for( iSocketId = iSocketIdMin, iSockets = 0; iSocketId <= iSocketIdMax && iSockets < m_arpIpcSocketWrapper.size(); iSocketId++ )
                            {
                                pIpcSocketWrapper = m_arpIpcSocketWrapper[iSocketId];

                                if( pIpcSocketWrapper != nullptr )
                                {
                                    iSockets++;

                                    if( pMsgReq->getByteArray().size() > 0 )
                                    {
                                        if( m_pBlkType != nullptr )
                                        {
                                            m_pBlkType->writeDataBlock(
                                                /* srvCltType          */ m_srvCltType,
                                                /* pObjGtw             */ this,
                                                /* pObjSrvClt          */ m_pSrvClt,
                                                /* pSocketWrapper      */ pIpcSocketWrapper,
                                                /* byteArr             */ pMsgReq->getByteArray(),
                                                /* isWatchDog          */ false,
                                                /* pTrcAdminObj        */ &mthTracer,
                                                /* arpTrcMsgLogObjects */ m_arpTrcMsgLogObjects );
                                        } // if( m_pBlkType != nullptr )
                                    } // if( pMsgReq->getByteArray().size() > 0 )

                                    // Please note that the socket may have been closed while sending data
                                    // emitting the "error" or "disconnected" signals (see corresponding slots).
                                    if( pIpcSocketWrapper->socketState() == ESocketStateUnconnected )
                                    {
                                        onDisconnected(pIpcSocketWrapper);
                                    }
                                } // if( pIpcSocketWrapper != nullptr )
                            } // for( iSocketId = iSocketIdMin, iSockets = 0; iSocketId <= iSocketIdMax && iSockets < m_arpIpcSocketWrapper.size(); iSocketId++ )
                        } // if( bSocketIdIsValid )

                        m_requestInProgress   = ERequestNone;
                        m_iReqIdInProgress    = -1;
                        m_iMsgIdReqInProgress = -1;

                    } // if( m_requestInProgress == ERequestNone && m_arpIpcSocketWrapper != nullptr && m_uActiveConnections > 0 )
                    break;
                } // case EMsgTypeReqSendData:

                case EMsgTypeReqReceiveData:
                {
                    CMsgReqReceiveData* pMsgReq = dynamic_cast<CMsgReqReceiveData*>(i_pMsg);
                    if( pMsgReq == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }

                    bool               bSocketIdIsValid = true;
                    int                iSocketId = pMsgReq->getSocketId();
                    CIpcSocketWrapper* pIpcSocketWrapper = nullptr;

                    if( iSocketId < 0 || iSocketId >= m_arpIpcSocketWrapper.size() )
                    {
                        bSocketIdIsValid = false;
                    }
                    else if( m_arpIpcSocketWrapper[iSocketId] == nullptr )
                    {
                        bSocketIdIsValid = false;
                    }
                    else if( iSocketId != m_arpIpcSocketWrapper[iSocketId]->getSocketId() )
                    {
                        bSocketIdIsValid = false;
                    }
                    if( bSocketIdIsValid )
                    {
                        pIpcSocketWrapper = m_arpIpcSocketWrapper[iSocketId];
                    }
                    if( pIpcSocketWrapper != nullptr )
                    {
                        if( pIpcSocketWrapper->socketState() == ESocketStateConnected )
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
                                   /* pSocketWrapper      */ pIpcSocketWrapper,
                                   /* pByteArrWatchDog    */ m_pByteArrWatchDog,
                                   /* pTrcAdminObj        */ &mthTracer,
                                   /* arpTrcMsgLogObjects */ m_arpTrcMsgLogObjects );

                                for( int idxBlk = 0; idxBlk < arByteArrs.size(); idxBlk++ )
                                {
                                    Ipc::CMsgIndReceivedData* pMsgInd = new Ipc::CMsgIndReceivedData(
                                        /* pObjSender   */ this,
                                        /* pObjReceiver */ m_pSrvClt,
                                        /* iSocketId    */ pIpcSocketWrapper->getSocketId(),
                                        /* byteArr      */ arByteArrs[idxBlk] );
                                    POST_OR_DELETE_MESSAGE(pMsgInd, &mthTracer, ELogDetailLevel::DebugNormal);
                                    pMsgInd = nullptr;
                                }
                            } // if( m_pBlkType != nullptr )

                            m_requestInProgress   = ERequestNone;
                            m_iReqIdInProgress    = -1;
                            m_iMsgIdReqInProgress = -1;

                            // Please note that the socket may have been closed while waiting for data
                            // emitting the "error" or "disconnected" signals (see corresponding slots).
                            if( pIpcSocketWrapper->socketState() == ESocketStateUnconnected )
                            {
                                onDisconnected(pIpcSocketWrapper);
                            }
                            else if( pIpcSocketWrapper->bytesAvailable() > 0 )
                            {
                                CMsgReqReceiveData* pMsgReq = new CMsgReqReceiveData(
                                    /* pObjSender       */ this,
                                    /* pObjReceiver     */ this,
                                    /* iSocketId        */ pIpcSocketWrapper->getSocketId(),
                                    /* bMustBeConfirmed */ false,
                                    /* iReqId           */ -1 );
                                POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ELogDetailLevel::DebugNormal);
                                pMsgReq = nullptr;
                            }
                        } // if( pIpcSocketWrapper->socketState() == ESocketStateConnected )
                    } // if( pIpcSocketWrapper != nullptr )
                    break;
                } // case EMsgTypeReqReceiveData:

                default:
                {
                    break;
                }
            } // switch( pMsg->type() )
        } // if( pMsg != nullptr )
    } // if( i_pMsg->type() >= QEvent::User )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pMsg);
    }
    return bEventHandled;

} // event
