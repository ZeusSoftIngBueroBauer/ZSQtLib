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

#include "DbClientGateway.h"
#include "DbClient.h"
#include "Msg.h"

#include "ZSIpc/ZSIpcBlkType.h"
#include "ZSIpc/ZSIpcSocketWrapperInProcMsg.h"
#include "ZSIpc/ZSIpcSocketWrapperShm.h"
#include "ZSIpc/ZSIpcSocketWrapperTcp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysException.h"
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
class CDbClientGatewayThread : public QThread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDbClientGatewayThread::CDbClientGatewayThread(
    const QString& i_strObjName,
    CDbClient*     i_pDbClient ) :
//------------------------------------------------------------------------------
    QThread(),
    m_pDbClt(i_pDbClient),
    m_pWaitCond(nullptr),
    m_pDbCltGtw(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::AsyncRequests", "CDbClientGatewayThread", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
CDbClientGatewayThread::~CDbClientGatewayThread()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    // Can't delete the gateway object as it has been created in the context of
    // the gateway thread. The gateway instance must have already been deleted
    // on leaving the "run" method of the thread.
    //try
    //{
    //    delete m_pDbCltGtw;
    //}
    //catch(...)
    //{
    //}

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDbClt = nullptr;
    m_pWaitCond = nullptr;
    m_pDbCltGtw = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
protected: // hiding overridable of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientGatewayThread::start()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "start",
        /* strAddInfo   */ strAddTrcInfo );

    QThread::start();

} // start

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientGatewayThread::start( QWaitCondition* i_pWaitCond )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "WaitCond: " + pointer2Str(i_pWaitCond);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "start",
        /* strAddInfo   */ strAddTrcInfo );

    m_pWaitCond = i_pWaitCond;

    QThread::start();

} // start

/*==============================================================================
public: // overridables of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientGatewayThread::run()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "run",
        /* strAddInfo   */ "" );

    m_pDbCltGtw = new CDbClientGateway( objectName(), m_pDbClt );

    if( m_pWaitCond != nullptr )
    {
        m_pWaitCond->wakeAll();
    }

    exec();

    try
    {
        delete m_pDbCltGtw;
    }
    catch(...)
    {
    }
    m_pDbCltGtw = nullptr;

} // run


/*******************************************************************************
class CDbClientGateway : public QObject
*******************************************************************************/

/*==============================================================================
Macros
==============================================================================*/

#define M_TRACE( _strAddTrcInfo ) \
    if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) ) { \
        mthTracer.trace(_strAddTrcInfo); \
    } \
    if( m_arpTrcMsgLogObjects.size() > 0 ) { \
        for( int __idxObj = 0; __idxObj < m_arpTrcMsgLogObjects.size(); __idxObj++ ) { \
            CMsgReqAddLogItem* __pMsgLogItem = new CMsgReqAddLogItem( \
                /* pObjSender   */ this, \
                /* pObjReceiver */ m_arpTrcMsgLogObjects[__idxObj], \
                /* transmitDir  */ ETransmitDir::Undefined, \
                /* bBold        */ false, \
                /* strMsg       */ _strAddTrcInfo ); \
            POST_OR_DELETE_MESSAGE(__pMsgLogItem); \
            __pMsgLogItem = nullptr; \
        } \
    }

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDbClientGateway::CDbClientGateway( const QString& i_strObjName, CDbClient* i_pDbClt ) :
//------------------------------------------------------------------------------
    QObject(),
    // Request initiated by request message received from the gateway's client:
    m_pDbClt(i_pDbClt),
    m_socketDscr(),
    m_state(EStateDisconnected),
    m_iReqMsgIdDbClt(-1),
    m_iReqIdDbClt(-1),
    m_reqDbClt(ERequestNone),
    m_pTmrReqTimeout(nullptr),
    m_hshReqsDb(),
    m_watchDogSettings(),
    m_pTmrWatchDog(nullptr),
    m_pByteArrWatchDog(nullptr),
    m_pIpcSocketWrapper(nullptr),
    m_arpTrcMsgLogObjects(),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::AsyncRequests", "CDbClientGateway", objectName());

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

    m_pTmrWatchDog = new QTimer(this);

    if( !QObject::connect(
        /* pObjSender   */ m_pTmrWatchDog,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onWatchDogTimeout()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CDbClientGateway::~CDbClientGateway()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    QHash<qint64,CRequest*>::iterator itReqs;

    CRequest* pReq;
    qint64    iReqId;

    while( !m_hshReqsDb.isEmpty() )
    {
        itReqs = m_hshReqsDb.begin();

        iReqId = itReqs.key();
        pReq = itReqs.value();

        m_hshReqsDb.remove(iReqId);

        delete pReq;
        pReq = nullptr;
    }

    try
    {
        delete m_pIpcSocketWrapper;
    }
    catch(...)
    {
    }
    m_pIpcSocketWrapper = nullptr;

    try
    {
        delete m_pByteArrWatchDog;
    }
    catch(...)
    {
    }
    m_pByteArrWatchDog = nullptr;

    m_pDbClt = nullptr;
    m_pTmrReqTimeout = nullptr;
    m_pTmrWatchDog = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

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
        strAddTrcInfo = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqDbClt: " + Request2Str(m_reqDbClt);
        strAddTrcInfo += ", ReqMsgIdDbClt: " + QString::number(m_iReqMsgIdDbClt);
        strAddTrcInfo += ", ReqIdDbClt: " + QString::number(m_iReqIdDbClt);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onConnected",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("onConnected");

    if( m_pTmrWatchDog->isActive() )
    {
        m_pTmrWatchDog->stop();
    }
    if( m_pTmrReqTimeout->isActive() )
    {
        m_pTmrReqTimeout->stop();
    }

    m_socketDscr = m_pIpcSocketWrapper->getSocketDscr();

    m_state = EStateConnected;

    switch( m_reqDbClt )
    {
        case ERequestConnect:
        {
            if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
            {
                M_TRACE( "-- IpcSocket::connected" );
            }

            if( CRequestExecTree::GetInstance()->isWaiting(m_iReqIdDbClt) )
            {
                CRequestExecTree::GetInstance()->wake(m_iReqIdDbClt);
            }
            else
            {
                CMsgConConnect* pMsgCon = new CMsgConConnect(
                    /* pObjSender    */ this,
                    /* pObjReceiver  */ m_pDbClt,
                    /* iReqId        */ m_iReqIdDbClt,
                    /* iMsgIdReq     */ m_iReqMsgIdDbClt,
                    /* errResultInfo */ errResultInfo,
                    /* iProgress     */ 100 );
                pMsgCon->setSocketDscr(m_socketDscr);
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }

            m_iReqMsgIdDbClt = -1;
            m_iReqIdDbClt = -1;
            m_reqDbClt = ERequestNone;

            if( m_watchDogSettings.m_bEnabled && m_watchDogSettings.m_iInterval_ms > 0 )
            {
                m_pTmrWatchDog->start(m_watchDogSettings.m_iInterval_ms);
            }
            break;
        } // case ERequestConnect

        case ERequestChangeSettings:
        {
            if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
            {
                M_TRACE( "-- IpcSocket::connected" );
            }

            if( CRequestExecTree::GetInstance()->isWaiting(m_iReqIdDbClt) )
            {
                CRequestExecTree::GetInstance()->wake(m_iReqIdDbClt);
            }
            else
            {
                CMsgConChangeSettings* pMsgCon = new CMsgConChangeSettings(
                    /* pObjSender    */ this,
                    /* pObjReceiver  */ m_pDbClt,
                    /* iReqId        */ m_iReqIdDbClt,
                    /* iMsgIdReq     */ m_iReqMsgIdDbClt,
                    /* errResultInfo */ errResultInfo,
                    /* iProgress     */ 100 );
                pMsgCon->setSocketDscr(m_socketDscr);
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }

            m_iReqMsgIdDbClt = -1;
            m_iReqIdDbClt = -1;
            m_reqDbClt = ERequestNone;

            if( m_watchDogSettings.m_bEnabled && m_watchDogSettings.m_iInterval_ms > 0 )
            {
                m_pTmrWatchDog->start(m_watchDogSettings.m_iInterval_ms);
            }
            break;
        } // case ERequestChangeSettings

        case ERequestNone:
        case ERequestDisconnect:
        default:
        {
            // Could be that a timeout occurred just before receiving the connected signal.
            break;
        }
    } // switch( m_reqDbClt )

} // onConnected

//------------------------------------------------------------------------------
void CDbClientGateway::onDisconnected( QObject* /*i_pSocketWrapper*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqDbClt: " + Request2Str(m_reqDbClt);
        strAddTrcInfo += ", ReqMsgIdDbClt: " + QString::number(m_iReqMsgIdDbClt);
        strAddTrcInfo += ", ReqIdDbClt: " + QString::number(m_iReqIdDbClt);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDisconnected",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("onDisconnected");

    if( m_pTmrWatchDog->isActive() )
    {
        m_pTmrWatchDog->stop();
    }
    if( m_pTmrReqTimeout->isActive() )
    {
        m_pTmrReqTimeout->stop();
    }

    m_socketDscr.m_socketState = Ipc::ESocketStateUnconnected;

    m_state = EStateDisconnected;

    switch( m_reqDbClt )
    {
        case ERequestNone:
        {
            if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
            {
                strAddTrcInfo = "-- IpcSocket::disconnected";
                M_TRACE(strAddTrcInfo);
            }

            CMsgIndDisconnected* pMsgInd = new CMsgIndDisconnected(
                /* pObjSender          */ this,
                /* pObjReceiver        */ m_pDbClt,
                /* bMustBeAcknowledged */ false,
                /* iReqId              */ -1,
                /* iMsgId              */ -1 );
            pMsgInd->setSocketDscr(m_socketDscr); // Socket Id for clients is always 0
            POST_OR_DELETE_MESSAGE(pMsgInd);
            pMsgInd = nullptr;
            break;
        } // case ERequestNone

        case ERequestShutdown:
        {
            break;
        }

        case ERequestConnect:
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Remote host " + m_socketDscr.m_strRemoteHostName + ":" + QString::number(m_socketDscr.m_uRemotePort) + " disconnected" );

            if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
            {
                strAddTrcInfo = "-- IpcSocket::disconnected";
                M_TRACE(strAddTrcInfo);
            }

            if( CRequestExecTree::GetInstance()->isWaiting(m_iReqIdDbClt) )
            {
                CRequestExecTree::GetInstance()->wake(m_iReqIdDbClt);
            }
            else
            {
                CMsgConConnect* pMsgCon = new CMsgConConnect(
                    /* pObjSender    */ this,
                    /* pObjReceiver  */ m_pDbClt,
                    /* iReqId        */ m_iReqIdDbClt,
                    /* iMsgIdReq     */ m_iReqMsgIdDbClt,
                    /* errResultInfo */ errResultInfo,
                    /* iProgress     */ 100 );
                pMsgCon->setSocketDscr(m_socketDscr);
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }

            m_iReqMsgIdDbClt = -1;
            m_iReqIdDbClt = -1;
            m_reqDbClt = ERequestNone;
            break;
        } // case ERequestConnect

        case ERequestDisconnect:
        {
            if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
            {
                strAddTrcInfo = "-- IpcSocket::disconnected";
                M_TRACE(strAddTrcInfo);
            }

            if( CRequestExecTree::GetInstance()->isWaiting(m_iReqIdDbClt) )
            {
                CRequestExecTree::GetInstance()->wake(m_iReqIdDbClt);
            }
            else
            {
                CMsgConDisconnect* pMsgCon = new CMsgConDisconnect(
                    /* pObjSender    */ this,
                    /* pObjReceiver  */ m_pDbClt,
                    /* iReqId        */ m_iReqIdDbClt,
                    /* iMsgIdReq     */ m_iReqMsgIdDbClt,
                    /* errResultInfo */ errResultInfo,
                    /* iProgress     */ 100 );
                pMsgCon->setSocketDscr(m_socketDscr);
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }

            m_iReqMsgIdDbClt = -1;
            m_iReqIdDbClt = -1;
            m_reqDbClt = ERequestNone;
            break;
        } // case ERequestDisconnect

        case ERequestChangeSettings:
        {
            if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
            {
                strAddTrcInfo = "-- IpcSocket::disconnected";
                M_TRACE(strAddTrcInfo);
            }

            CMsgIndDisconnected* pMsgInd = new CMsgIndDisconnected(
                /* pObjSender          */ this,
                /* pObjReceiver        */ m_pDbClt,
                /* bMustBeAcknowledged */ false,
                /* iReqId              */ -1,
                /* iMsgId              */ -1 );
            pMsgInd->setSocketDscr(m_socketDscr); // Socket Id for clients is always 0
            POST_OR_DELETE_MESSAGE(pMsgInd);
            pMsgInd = nullptr;

            if( m_pIpcSocketWrapper->socketType() != m_socketDscr.m_socketType )
            {
                try
                {
                    delete m_pIpcSocketWrapper;
                }
                catch(...)
                {
                }
                m_pIpcSocketWrapper = nullptr;

                switch( m_socketDscr.m_socketType )
                {
                    case Ipc::ESocketTypeTcp:
                    {
                        m_pIpcSocketWrapper = new Ipc::CTcpSocketWrapper( objectName(), Ipc::ESrvCltTypeClient, 0, nullptr );
                        break;
                    }
                    case Ipc::ESocketTypeShm:
                    {
                        m_pIpcSocketWrapper = new Ipc::CShmSocketWrapper( objectName(), Ipc::ESrvCltTypeClient, 0 );
                        break;
                    }
                    case Ipc::ESocketTypeInProcMsg:
                    {
                        m_pIpcSocketWrapper = new Ipc::CInProcMsgSocketWrapper( objectName(), Ipc::ESrvCltTypeClient, 0, nullptr );
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

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
            } // if( m_pIpcSocketWrapper->socketType() != m_socketDscr.m_socketType )

            m_pIpcSocketWrapper->setLocalHostName(m_socketDscr.m_strLocalHostName);

            if( m_pIpcSocketWrapper->socketState() != Ipc::ESocketStateConnecting )
            {
                if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                {
                    strAddTrcInfo = "-+ IpcSocket::connectToHost( " + m_socketDscr.m_strRemoteHostName + ":" + QString::number(m_socketDscr.m_uServerListenPort) + " )";
                    M_TRACE(strAddTrcInfo);
                }

                // The slot connectedTcpSocket will be called if the socket is connected.
                // The slot method sends the connect confirmation message to the server.
                m_pIpcSocketWrapper->connectToHost( m_socketDscr.m_strRemoteHostName, m_socketDscr.m_uServerListenPort );

                if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                {
                    strAddTrcInfo = "+- IpcSocket::connectToHost()";
                    M_TRACE(strAddTrcInfo);
                }
            }

            if( m_socketDscr.m_iConnectTimeout_ms > 0 )
            {
                // Start single shot timer:
                m_pTmrReqTimeout->start(m_socketDscr.m_iConnectTimeout_ms);
            }
            break;
        } // case ERequestChangeSettings:

        default:
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultMethodStateMismatch);
            errResultInfo.setAddErrInfoDscr( Request2Str(m_reqDbClt) );

            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
            else
            {
                throw ZS::System::CException( __FILE__, __LINE__, errResultInfo );
            }
        }
    } // switch( m_reqDbClt )

} // onDisconnected

//------------------------------------------------------------------------------
void CDbClientGateway::onReadyRead( QObject* i_pSocketWrapper )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqDbClt: " + Request2Str(m_reqDbClt);
        strAddTrcInfo += ", ReqMsgIdDbClt: " + QString::number(m_iReqMsgIdDbClt);
        strAddTrcInfo += ", ReqIdDbClt: " + QString::number(m_iReqIdDbClt);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onReadyRead",
        /* strAddInfo   */ strAddTrcInfo );

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
    POST_OR_DELETE_MESSAGE(pMsgReq);
    pMsgReq = nullptr;

} // onReadyRead

//------------------------------------------------------------------------------
void CDbClientGateway::onError( QObject* /*i_pSocketWrapper*/, ZS::System::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = i_errResultInfo.getResultStr();
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqDbClt: " + Request2Str(m_reqDbClt);
        strAddTrcInfo += ", ReqMsgIdDbClt: " + QString::number(m_iReqMsgIdDbClt);
        strAddTrcInfo += ", ReqIdDbClt: " + QString::number(m_iReqIdDbClt);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onError",
        /* strAddInfo   */ strAddTrcInfo );

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

    switch( m_reqDbClt )
    {
        case ERequestNone:
        {
            switch( m_pIpcSocketWrapper->error() )
            {
                // If the remote client closed the connection ..
                case EResultRemoteHostClosed:
                {
                    // .. its not a real error. The connection will be closed calling the disconnected slot.
                    errResultInfo.setErrResult(ErrResultSuccess);
                    break;
                }
                // Timeout reported by the socket while no request is active ..
                case EResultTimeout:
                {
                    // ..the connect timeout has been expired before (see slot "onTimeoutConnection")
                    // and has already been reported to the client.
                    errResultInfo.setErrResult(ErrResultSuccess);
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        } // case ERequestNone:

        case ERequestConnect:
        {
            if( errResultInfo.getResult() == EResultTimeout )
            {
                // If the "connectToHost" method timed out and if a connect timeout has been specified...
                if( m_socketDscr.m_iConnectTimeout_ms > 0 )
                {
                    // .. its the slot "onRequestTimeout" that will report the timeout. If
                    // the "connectToHost" method timed out the specified timeout has not really
                    // been expired and we can try to establish the connection again ...
                    CMsgReqConnect* pMsgReq = new CMsgReqConnect(
                        /* pObjSender       */ this,
                        /* pObjReceiver     */ this,
                        /* bMustBeConfirmed */ true,
                        /* iReqId           */ -1 );
                    pMsgReq->setHostSettings(m_socketDscr);
                    POST_OR_DELETE_MESSAGE(pMsgReq);
                    pMsgReq = nullptr;
                    errResultInfo.setErrResult(ErrResultSuccess);
                }
            }

            if( errResultInfo.isErrorResult() )
            {
                if( m_pIpcSocketWrapper != nullptr && m_pIpcSocketWrapper->socketState() == Ipc::ESocketStateConnecting )
                {
                    m_pIpcSocketWrapper->abort();
                }
                if( m_pTmrWatchDog->isActive() )
                {
                    m_pTmrWatchDog->stop();
                }

                m_socketDscr.m_socketState = Ipc::ESocketStateUnconnected;

                if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                {
                    strAddTrcInfo = "-- IpcSocket::error( " + errResultInfo.getAddErrInfoDscr() + " )";
                    M_TRACE(strAddTrcInfo);
                }

                if( CRequestExecTree::GetInstance()->isWaiting(m_iReqIdDbClt) )
                {
                    CRequestExecTree::GetInstance()->wake(m_iReqIdDbClt);
                }
                else
                {
                    CMsgConConnect* pMsgCon = new CMsgConConnect(
                        /* pObjSender    */ this,
                        /* pObjReceiver  */ m_pDbClt,
                        /* iReqId        */ m_iReqIdDbClt,
                        /* iMsgIdReq     */ m_iReqMsgIdDbClt,
                        /* errResultInfo */ errResultInfo,
                        /* iProgress     */ 100 );
                    pMsgCon->setSocketDscr(m_socketDscr);
                    POST_OR_DELETE_MESSAGE(pMsgCon);
                    pMsgCon = nullptr;
                }

                m_iReqMsgIdDbClt = -1;
                m_iReqIdDbClt = -1;
                m_reqDbClt = ERequestNone;

            } // if( m_socketDscr.m_iConnectTimeout_ms <= 0 )

            // Its not a real error but the connection was not established.
            errResultInfo.setErrResult(ErrResultSuccess);
            break;
        } // case ERequestConnect:

        case ERequestChangeSettings:
        {
            // If the "connectToHost" method timed out and if a connect timeout has been specified...
            if( m_socketDscr.m_iConnectTimeout_ms > 0 )
            {
                // .. its the slot "onRequestTimeout" that will report the timeout. If
                // the "connectToHost" method timed out the specified timeout has not really
                // been expired and we can try to establish the connection again ...
                CMsgReqConnect* pMsgReq = new CMsgReqConnect(
                    /* pObjSender       */ this,
                    /* pObjReceiver     */ this,
                    /* bMustBeConfirmed */ true,
                    /* iReqId           */ -1 );
                pMsgReq->setHostSettings(m_socketDscr);
                POST_OR_DELETE_MESSAGE(pMsgReq);
                pMsgReq = nullptr;
            }
            else // if( m_socketDscr.m_iConnectTimeout_ms <= 0 )
            {
                if( m_pIpcSocketWrapper != nullptr && m_pIpcSocketWrapper->socketState() == Ipc::ESocketStateConnecting )
                {
                    m_pIpcSocketWrapper->abort();
                }
                if( m_pTmrWatchDog->isActive() )
                {
                    m_pTmrWatchDog->stop();
                }

                m_socketDscr.m_socketState = Ipc::ESocketStateUnconnected;

                if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                {
                    strAddTrcInfo = "-- IpcSocket::error( " + errResultInfo.getAddErrInfoDscr() + " )";
                    M_TRACE(strAddTrcInfo);
                }

                if( CRequestExecTree::GetInstance()->isWaiting(m_iReqIdDbClt) )
                {
                    CRequestExecTree::GetInstance()->wake(m_iReqIdDbClt);
                }
                else
                {
                    CMsgConChangeSettings* pMsgCon = new CMsgConChangeSettings(
                        /* pObjSender    */ this,
                        /* pObjReceiver  */ m_pDbClt,
                        /* iReqId        */ m_iReqIdDbClt,
                        /* iMsgIdReq     */ m_iReqMsgIdDbClt,
                        /* errResultInfo */ errResultInfo,
                        /* iProgress     */ 100 );
                    pMsgCon->setSocketDscr(m_socketDscr);
                    POST_OR_DELETE_MESSAGE(pMsgCon);
                    pMsgCon = nullptr;
                }

                m_iReqMsgIdDbClt = -1;
                m_iReqIdDbClt = -1;
                m_reqDbClt = ERequestNone;

            } // if( m_socketDscr.m_iConnectTimeout_ms <= 0 )

            // Its not a real error but the connection was not established.
            errResultInfo.setErrResult(ErrResultSuccess);
            break;
        } // case ERequestChangeSettings

        case ERequestDisconnect:
        {
            if( CRequestExecTree::GetInstance()->isWaiting(m_iReqIdDbClt) )
            {
                CRequestExecTree::GetInstance()->wake(m_iReqIdDbClt);
            }
            else
            {
                CMsgConDisconnect* pMsgCon = new CMsgConDisconnect(
                    /* pObjSender    */ this,
                    /* pObjReceiver  */ m_pDbClt,
                    /* iReqId        */ m_iReqIdDbClt,
                    /* iMsgIdReq     */ m_iReqMsgIdDbClt,
                    /* errResultInfo */ errResultInfo,
                    /* iProgress     */ 100 );
                pMsgCon->setSocketDscr(m_socketDscr);
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }

            m_iReqMsgIdDbClt = -1;
            m_iReqIdDbClt = -1;
            m_reqDbClt = ERequestNone;
            break;
        }

        case ERequestShutdown:
        {
            break;
        }

        case ERequestStartup:
        default:
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultMethodStateMismatch);
            errResultInfo.setAddErrInfoDscr( Request2Str(m_reqDbClt) );

            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
            else
            {
                throw ZS::System::CException( __FILE__, __LINE__, errResultInfo );
            }
        }
    } // switch( m_reqDbClt )

    if( errResultInfo.isErrorResult() )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

} // onError

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClientGateway::onRequestTimeout()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqDbClt: " + Request2Str(m_reqDbClt);
        strAddTrcInfo += ", ReqMsgIdDbClt: " + QString::number(m_iReqMsgIdDbClt);
        strAddTrcInfo += ", ReqIdDbClt: " + QString::number(m_iReqIdDbClt);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRequestTimeout",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pTmrReqTimeout->isActive() )
    {
        m_pTmrReqTimeout->stop();
    }

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("onRequestTimeout");

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

    switch( m_reqDbClt )
    {
        case ERequestConnect:
        {
            if( m_pIpcSocketWrapper != nullptr && m_pIpcSocketWrapper->socketState() == Ipc::ESocketStateConnecting )
            {
                if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                {
                    M_TRACE( "-+ IpcSocket:abort" );
                }

                // The "disconnected" slot will be invoked and the request in progress changes.
                m_pIpcSocketWrapper->abort();

                if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                {
                    M_TRACE( "+- IpcSocket:abort" );
                }
            }

            if( m_pTmrWatchDog->isActive() )
            {
                m_pTmrWatchDog->stop();
            }

            // The request has been executed. An error will be reported.
            m_socketDscr.m_socketState = Ipc::ESocketStateUnconnected;

            if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
            {
                M_TRACE( "-- IpcSocket::connectToHost( TIMEOUT )" );
            }

            if( CRequestExecTree::GetInstance()->isWaiting(m_iReqIdDbClt) )
            {
                CRequestExecTree::GetInstance()->wake(m_iReqIdDbClt);
            }
            else
            {
                CMsgConConnect* pMsgCon = new CMsgConConnect(
                    /* pObjSender    */ this,
                    /* pObjReceiver  */ m_pDbClt,
                    /* iReqId        */ m_iReqIdDbClt,
                    /* iMsgIdReq     */ m_iReqMsgIdDbClt,
                    /* errResultInfo */ errResultInfo,
                    /* iProgress     */ 100 );
                pMsgCon->setSocketDscr(m_socketDscr);
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }
            break;
        } // case ERequestConnect

        case ERequestChangeSettings:
        {
            if( m_pIpcSocketWrapper != nullptr && m_pIpcSocketWrapper->socketState() == Ipc::ESocketStateConnecting )
            {
                if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                {
                    M_TRACE( "-+ IpcSocket::abort" );
                }

                // The "disconnected" slot will be invoked and the request in progress changes.
                m_pIpcSocketWrapper->abort();

                if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                {
                    M_TRACE( "+- IpcSocket::abort" );
                }
            }

            if( m_pTmrWatchDog->isActive() )
            {
                m_pTmrWatchDog->stop();
            }

            // The request has been executed. An error will be reported.
            m_socketDscr.m_socketState = Ipc::ESocketStateUnconnected;

            if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
            {
                M_TRACE( "-- IpcSocket::connectToHost( TIMEOUT )" );
            }

            if( CRequestExecTree::GetInstance()->isWaiting(m_iReqIdDbClt) )
            {
                CRequestExecTree::GetInstance()->wake(m_iReqIdDbClt);
            }
            else
            {
                CMsgConChangeSettings* pMsgCon = new CMsgConChangeSettings(
                    /* pObjSender    */ this,
                    /* pObjReceiver  */ m_pDbClt,
                    /* iReqId        */ m_iReqIdDbClt,
                    /* iMsgIdReq     */ m_iReqMsgIdDbClt,
                    /* errResultInfo */ errResultInfo,
                    /* iProgress     */ 100 );
                pMsgCon->setSocketDscr(m_socketDscr);
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }
            break;
        } // case ERequestChangeSettings

        case ERequestDisconnect:
        case ERequestNone:
        default:
        {
            break;
        }
    } // switch( m_reqDbClt )

    m_iReqMsgIdDbClt = -1;
    m_iReqIdDbClt = -1;
    m_reqDbClt = ERequestNone;

} // onRequestTimeout

//------------------------------------------------------------------------------
void CDbClientGateway::onWatchDogTimeout()
//------------------------------------------------------------------------------
{
    if( m_pByteArrWatchDog != nullptr )
    {
        QString strAddTrcInfo;

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDetailLevel */ 3,
            /* strMethod    */ "onWatchDogTimeout",
            /* strAddInfo   */ strAddTrcInfo );

        QString strAddErrInfo;

        bool bTimeout = false;

        if( m_watchDogSettings.m_bEnabled && m_watchDogSettings.m_iTimeout_ms > 0 )
        {
            if( m_pIpcSocketWrapper != nullptr )
            {
                SErrResultInfo errResultInfo = ErrResultInfoSuccess("onWatchDogTimeout");

                double fTimeCurrent_ms = ZS::System::Time::getProcTimeInMilliSec();
                double fTimeDiff_ms    = fTimeCurrent_ms - m_pIpcSocketWrapper->getLastTimeDataBlockReceivedInMs();

                if( fTimeDiff_ms > static_cast<double>(m_watchDogSettings.m_iTimeout_ms) )
                {
                    if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) )
                    {
                        mthTracer.trace( "TIMEOUT: disconnecting from host " + m_pIpcSocketWrapper->getRemoteHostName() + ":" + QString::number(m_pIpcSocketWrapper->getServerListenPort()) + ":" + QString::number(m_pIpcSocketWrapper->getRemotePort()) );
                    }

                    if( CErrLog::GetInstance() != nullptr )
                    {
                        strAddErrInfo  = "Duration since last message received from ";
                        strAddErrInfo += m_pIpcSocketWrapper->getRemoteHostName() + ":" + QString::number(m_pIpcSocketWrapper->getServerListenPort()) + ":" + QString::number(m_pIpcSocketWrapper->getRemotePort());
                        strAddErrInfo += " exceeded watch dog timeout of " + QString::number(m_watchDogSettings.m_iTimeout_ms) + " ms";
                        SErrResultInfo errResultInfoTmp = ErrResultInfoWarning("onWatchDogTimeout", EResultTimeout, strAddErrInfo);
                        CErrLog::GetInstance()->addEntry(errResultInfoTmp);
                    }

                    if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                    {
                        strAddTrcInfo = "-+ IpcSocket::disconnectFromHost( " + m_socketDscr.getConnectionString() + " )";
                        M_TRACE(strAddTrcInfo);
                    }

                    bTimeout = true;
                    m_pIpcSocketWrapper->disconnectFromHost();

                    if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                    {
                        strAddTrcInfo = "+- IpcSocket::disconnectFromHost( SUCCESS )";
                        M_TRACE(strAddTrcInfo);
                    }
                } // if( fTimeDiff_ms > static_cast<double>(m_watchDogSettings.m_iTimeout_ms) )
            } // if( m_pIpcSocketWrapper != nullptr )
        } // if( m_watchDogSettings.m_bEnabled && m_watchDogSettings.m_iTimeout_ms > 0 )

        if( !bTimeout )
        {
            //CMsgReqSendData* pMsgReq = new CMsgReqSendData(
            //    /* pObjSender       */ this,
            //    /* pObjReceiver     */ this,
            //    /* iSocketId        */ m_socketDscr.m_iSocketId,
            //    /* byteArr          */ *m_pByteArrWatchDog,
            //    /* bMustBeConfirmed */ false,
            //    /* iReqId           */ -1 );
            //POST_OR_DELETE_MESSAGE(pMsgReq);
            //pMsgReq = nullptr;
        }
    } // if( m_pByteArrWatchDog != nullptr )

} // onWatchDogTimeout

/*==============================================================================
public: //instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CDbClientGateway::receiveDataBlock_L( ZS::Ipc::CIpcSocketWrapper* i_pSocketWrapper )
//------------------------------------------------------------------------------
{
    // +--------+----------+
    // | LENgth | Data ... |
    // +--------+----------+

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo += i_pSocketWrapper->getRemoteHostName() + ":" + QString::number(i_pSocketWrapper->getServerListenPort()) + ":" + QString::number(i_pSocketWrapper->getRemotePort());

        strAddTrcInfo += i_pSocketWrapper->getRemoteHostName() + ":" + QString::number(i_pSocketWrapper->getServerListenPort()) + ":" + QString::number(i_pSocketWrapper->getRemotePort());
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqDbClt: " + Request2Str(m_reqDbClt);
        strAddTrcInfo += ", ReqMsgIdDbClt: " + QString::number(m_iReqMsgIdDbClt);
        strAddTrcInfo += ", ReqIdDbClt: " + QString::number(m_iReqIdDbClt);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ 1,
        /* strMethod    */ "receiveDataBlock_L",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("receiveDataBlock_L");

    QByteArray byteArr;

    bool bBlockReceived = false;
    bool bIsWatchDogBlock = false;

    while( !bBlockReceived && !errResultInfo.isErrorResult() && i_pSocketWrapper->bytesAvailable() )
    {
        qint32      iBlkSize = 0;
        QByteArray  byteArrBlkSize;
        bool        bReadyRead;
        const int   iReadTimeout_ms = 1000;
        double      fStartTime_ms;
        double      fCurrTime_ms;

        // Wait until the number of bytes containing the length of the block have been received ..
        fStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
        while( i_pSocketWrapper->bytesAvailable() < static_cast<qint64>(sizeof(iBlkSize)) )
        {
            fCurrTime_ms = ZS::System::Time::getProcTimeInMilliSec();

            bReadyRead = i_pSocketWrapper->waitForReadyRead(iReadTimeout_ms);

            if( !bReadyRead || (fCurrTime_ms - fStartTime_ms) > static_cast<double>(iReadTimeout_ms) )
            {
                // Empty receive buffer
                i_pSocketWrapper->read(i_pSocketWrapper->bytesAvailable());

                // Please note that the socket may have been closed in the meantime on
                // emitting the "error" or "disconnected" signals (see corresponding slots).

                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultUnfinishedSocketOperation);

                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
                break;
            }
        } // while( i_pSocketWrapper->bytesAvailable() < iBlkSizeLen )

        // If the length of the block has been received ..
        if( !errResultInfo.isErrorResult() )
        {
            // ... read the length of the block.
            byteArrBlkSize = i_pSocketWrapper->read(sizeof(iBlkSize));

            memcpy( &iBlkSize, byteArrBlkSize.data(), sizeof(iBlkSize) );

            // Now wait until as many bytes as defined by the length of the block have been received.
            fStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
            while( i_pSocketWrapper->bytesAvailable() < iBlkSize )
            {
                fCurrTime_ms = ZS::System::Time::getProcTimeInMilliSec();

                bReadyRead = i_pSocketWrapper->waitForReadyRead(iReadTimeout_ms);

                if( !bReadyRead || (fCurrTime_ms - fStartTime_ms) > static_cast<double>(iReadTimeout_ms) )
                {
                    // Empty receive buffer
                    i_pSocketWrapper->read(i_pSocketWrapper->bytesAvailable());

                    // Please note that the socket may have been closed in the meantime on
                    // emitting the "error" or "disconnected" signals (see corresponding slots).

                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultUnfinishedSocketOperation);

                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                    break;
                }
            } // while( i_pSocketWrapper->bytesAvailable() < iBlkSize )
        } // if( !errResultInfo.isErrorResult() )

        // If the number of bytes as defined by the block length have been received ..
        if( !errResultInfo.isErrorResult() )
        {
            i_pSocketWrapper->setLastTimeDataBlockReceivedInMs( ZS::System::Time::getProcTimeInMilliSec() );

            if( iBlkSize > 0 )
            {
                // ... read those bytes.
                byteArr = i_pSocketWrapper->read(iBlkSize);
            }

            bBlockReceived = true; // finish outer while loop

            //if( m_pByteArrWatchDog != nullptr )
            //{
            //    if( memcmp( m_pByteArrWatchDog->data(), byteArr.data(), m_pByteArrWatchDog->length() ) == 0 )
            //    {
            //        bIsWatchDogBlock = true;
            //    }
            //}

            if( bIsWatchDogBlock )
            {
                //// Only servers acknowledge the receipt of watch dog data blocks.
                //// Servers are the "passive" part of the watch dog communication.
                //// Clients are the "active" part of the watch dog communication.
                //CMsgReqSendData* pMsgReq = new CMsgReqSendData(
                //    /* pObjSender       */ this,
                //    /* pObjReceiver     */ this,
                //    /* iSocketId        */ i_pSocketWrapper->getSocketId(),
                //    /* byteArr          */ *m_pByteArrWatchDog,
                //    /* bMustBeConfirmed */ false,
                //    /* iReqId           */ -1 );
                //POST_OR_DELETE_MESSAGE(pMsgReq);
                //pMsgReq = nullptr;

                //// From the point of view of data transfer watch dog blocks got to be ignored.
                //byteArr.clear();

            } // if( bIsWatchDogBlock )
        } // if( !errResultInfo.isErrorResult() )

        // Someone may sent a lot of data very quickly.
        // Let the GUI main thread process the received data.
        //m_pGatewayThread->msleep(50);

    } // while( !bBlockReceived && !errResultInfo.isErrorResult() && i_pSocketWrapper->bytesAvailable() )

    // Please note that if still data is available in the read buffer the "onReadyReady" slot
    // (which invoked this "receiveDataBlock" method) the message "ReqReceiveData" will be sent
    // to the event method of the gateway. This way also data send requests may be executed even
    // if a lot of data is sent to the socket.

    return byteArr;

} // receiveDataBlock_L

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CDbClientGateway::writeDataBlock_L( ZS::Ipc::CIpcSocketWrapper* i_pSocketWrapper, const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    // +--------+----------+
    // | LENgth | Data ... |
    // +--------+----------+

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo += i_pSocketWrapper->getRemoteHostName() + ":" + QString::number(i_pSocketWrapper->getServerListenPort()) + ":" + QString::number(i_pSocketWrapper->getRemotePort());
        strAddTrcInfo += ", Len: " + QString::number(i_byteArr.size());
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqDbClt: " + Request2Str(m_reqDbClt);
        strAddTrcInfo += ", ReqMsgIdDbClt: " + QString::number(m_iReqMsgIdDbClt);
        strAddTrcInfo += ", ReqIdDbClt: " + QString::number(m_iReqIdDbClt);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ 1,
        /* strMethod    */ "writeDataBlock_L",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("writeDataBlock_L");

    qint32 iBlkSizeLen = sizeof(qint32);
    qint32 iBlkSize = i_byteArr.size();

    if( quint64(iBlkSizeLen+iBlkSize) > blockLenDataType2MaxSizeInBytes(Ipc::EBlkLenDataTypeBinINT32) )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultTooMuchData);
        errResultInfo.setAddErrInfoDscr( "On sending data to " + i_pSocketWrapper->getRemoteHostName() + ":" + QString::number(i_pSocketWrapper->getServerListenPort()) + ":" + QString::number(i_pSocketWrapper->getRemotePort()) );
    }

    if( !errResultInfo.isErrorResult() )
    {
        const char* pcBlkLen = reinterpret_cast<const char*>(&iBlkSize);

        QByteArray* pByteArr = new QByteArray(iBlkSizeLen,0x00);

        // Copy size info to destination array (LSB at the beginning).
        memcpy( pByteArr->data(), pcBlkLen, iBlkSizeLen );

        //const Data::MsgProtocol::SMsgHeader* pMsgHdr = reinterpret_cast<const Data::MsgProtocol::SMsgHeader*>(i_byteArr.data());

        // Add data to be sent to the peer.
        *pByteArr += i_byteArr;

        bool bOk = (i_pSocketWrapper->write(*pByteArr) == pByteArr->size());

        delete pByteArr;
        pByteArr = nullptr;

        if( !bOk )
        {
            errResultInfo = i_pSocketWrapper->errResultInfo();
        }

    } // if( !errResultInfo.isErrorResult() )

    return errResultInfo;

} // writeDataBlock_L

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CDbClientGateway::event( QEvent* i_pEv )
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
            strAddTrcInfo += ", ReqDbClt: " + Request2Str(m_reqDbClt);
            strAddTrcInfo += ", ReqMsgIdDbClt: " + QString::number(m_iReqMsgIdDbClt);
            strAddTrcInfo += ", ReqIdDbClt: " + QString::number(m_iReqIdDbClt);
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDetailLevel */ ETraceDetailLevelMethodCalls,
            /* strMethod    */ "event",
            /* strAddInfo   */ strAddTrcInfo );

        if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeReq )
        {
            if( static_cast<int>(pMsg->type()) == EMsgTypeReqConnect )
            {
                CMsgReqConnect* pMsgReq = dynamic_cast<CMsgReqConnect*>(pMsg);
                if( pMsgReq == nullptr )
                {
                    throw CException( __FILE__, __LINE__, EResultMessageTypeMismatch );
                }
                if( pMsgReq->getSender() != m_pDbClt && pMsgReq->getSender() != this )
                {
                    throw CException( __FILE__, __LINE__, EResultInvalidSenderObjInMessage );
                }

                SErrResultInfo errResultInfo = ErrResultInfoSuccess("MsgReqConnect");

                if( m_state >= EStateConnected )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultMessageStateMismatch);
                    errResultInfo.setAddErrInfoDscr( "Gateway is already connected" );
                }
                else if( m_reqDbClt != Database::ERequestNone )
                {
                    // The connect request may also have been send from myself from within the error
                    // method if the host did not respond but the timeout has not been expired.
                    if( m_reqDbClt != ERequestConnect && pMsgReq->getSender() != this )
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultMessageStateMismatch);
                        errResultInfo.setAddErrInfoDscr( "Gateway is busy" );
                    }
                }

                m_socketDscr = pMsgReq->getHostSettings().getSocketDscr();

                Ipc::ESocketType socketType = m_socketDscr.m_socketType;

                if( socketType < 0 || socketType >= Ipc::ESocketTypeCount )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultArgOutOfRange);
                    errResultInfo.setAddErrInfoDscr( "Invalid socket type " + Ipc::socketType2Str(m_socketDscr.m_socketType) );
                }

                if( !errResultInfo.isErrorResult() )
                {
                    // If I did not receive the connect request from myself because the connect timeout has not yet been expired ..
                    if( pMsgReq->getSender() != this )
                    {
                        m_iReqMsgIdDbClt = pMsgReq->getMsgId();
                        m_iReqIdDbClt = pMsgReq->getRequestId();
                        m_reqDbClt = ERequestConnect;

                        // If the socket type has been changed ..
                        if( m_pIpcSocketWrapper != nullptr && m_pIpcSocketWrapper->socketType() != socketType )
                        {
                            // .. the socket wrapper object need to be replaced.
                            if( m_pIpcSocketWrapper->socketState() != Ipc::ESocketStateUnconnected )
                            {
                                if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                                {
                                    strAddTrcInfo = "-+ IpcSocket::abort( " + m_pIpcSocketWrapper->getRemoteHostName() + ":" + QString::number(m_pIpcSocketWrapper->getServerListenPort()) + " )";
                                    M_TRACE(strAddTrcInfo);
                                }

                                m_pIpcSocketWrapper->abort();

                                if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                                {
                                    strAddTrcInfo = "+- IpcSocket::abort( SUCCESS )";
                                    M_TRACE(strAddTrcInfo);
                                }
                            } // if( m_pIpcSocketWrapper->socketState() != Ipc::ESocketStateUnconnected )

                            // If the socket type has been changed the socket wrapper object need to be replaced.
                            if( m_pIpcSocketWrapper != nullptr )
                            {
                                if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                                {
                                    strAddTrcInfo = "-+ IpcSocket::dtor()";
                                    M_TRACE(strAddTrcInfo);
                                }

                                try
                                {
                                    delete m_pIpcSocketWrapper;
                                }
                                catch(...)
                                {
                                }
                                m_pIpcSocketWrapper = nullptr;

                                if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                                {
                                    strAddTrcInfo = "+- IpcSocket::dtor()";
                                    M_TRACE(strAddTrcInfo);
                                }
                            } // if( m_pIpcSocketWrapper != nullptr )
                        } // if( m_pIpcSocketWrapper != nullptr && m_pIpcSocketWrapper->socketType() != pMsgReq->getSocketType() )

                        if( m_pIpcSocketWrapper == nullptr )
                        {
                            if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                            {
                                strAddTrcInfo = "-+ IpcSocket::ctor()";
                                M_TRACE(strAddTrcInfo);
                            }

                            switch( socketType )
                            {
                                case Ipc::ESocketTypeTcp:
                                {
                                    m_pIpcSocketWrapper = new Ipc::CTcpSocketWrapper( objectName(), Ipc::ESrvCltTypeClient, 0 );
                                    break;
                                }
                                case Ipc::ESocketTypeShm:
                                {
                                    m_pIpcSocketWrapper = new Ipc::CShmSocketWrapper( objectName(), Ipc::ESrvCltTypeClient, 0 );
                                    break;
                                }
                                case Ipc::ESocketTypeInProcMsg:
                                {
                                    if( m_socketDscr.m_strRemoteHostName.isEmpty() )
                                    {
                                        m_socketDscr.m_strRemoteHostName = "Db";
                                    }
                                    if( m_socketDscr.m_strLocalHostName.isEmpty() )
                                    {
                                        m_socketDscr.m_strLocalHostName = objectName();
                                    }
                                    m_pIpcSocketWrapper = new Ipc::CInProcMsgSocketWrapper( objectName(), Ipc::ESrvCltTypeClient, 0 );
                                    m_pIpcSocketWrapper->setLocalHostName( objectName() );
                                    break;
                                }
                                default:
                                {
                                    break;
                                }
                            } // switch( m_socketDscr.m_socketType )

                            if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                            {
                                strAddTrcInfo = "+- IpcSocket::ctor()";
                                M_TRACE(strAddTrcInfo);
                            }

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
                        } // if( m_pIpcSocketWrapper == nullptr )
                    } // if( pMsgReq->getSender() != this )

                    if( m_pIpcSocketWrapper->socketState() != Ipc::ESocketStateConnecting )
                    {
                        if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                        {
                            strAddTrcInfo = "-+ IpcSocket::connectToHost( " + m_socketDscr.getConnectionString() + " )";
                            M_TRACE(strAddTrcInfo);
                        }

                        // The slot onConnected will be called if the socket is connected.
                        // The slot method sends the connect confirmation message to the client.
                        m_pIpcSocketWrapper->connectToHost( m_socketDscr.m_strRemoteHostName, m_socketDscr.m_uServerListenPort );

                    } // if( m_pIpcSocketWrapper->socketState() != Ipc::ESocketStateConnecting )
                } // if( !errResultInfo.isErrorResult() )

                if( errResultInfo.isErrorResult() )
                {
                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                }

                if( m_reqDbClt == ERequestNone )
                {
                    if( CRequestExecTree::GetInstance()->isWaiting(m_iReqIdDbClt) )
                    {
                        CRequestExecTree::GetInstance()->wake(m_iReqIdDbClt);
                    }
                    else
                    {
                        CMsgConConnect* pMsgCon = new CMsgConConnect(
                            /* pObjSender    */ this,
                            /* pObjReceiver  */ m_pDbClt,
                            /* iReqId        */ m_iReqIdDbClt,
                            /* iMsgIdReq     */ m_iReqMsgIdDbClt,
                            /* errResultInfo */ errResultInfo,
                            /* iProgress     */ 100 );
                        pMsgCon->setSocketDscr(m_socketDscr);
                        POST_OR_DELETE_MESSAGE(pMsgCon);
                        pMsgCon = nullptr;
                    }

                    m_iReqMsgIdDbClt = -1;
                    m_iReqIdDbClt = -1;

                } // if( m_reqDbClt == ERequestNone )

                else // if( m_reqDbClt != ERequestNone )
                {
                    if( m_socketDscr.m_iConnectTimeout_ms > 0 )
                    {
                        if( !m_pTmrReqTimeout->isActive() )
                        {
                            m_pTmrReqTimeout->start(m_socketDscr.m_iConnectTimeout_ms);
                        }
                    }
                }
                bEventHandled = true;

            } // if( pMsg->type() == EMsgTypeReqConnect )

            else if( static_cast<int>(pMsg->type()) == EMsgTypeReqDisconnect )
            {
                CMsgReqDisconnect* pMsgReq = dynamic_cast<CMsgReqDisconnect*>(pMsg);
                if( pMsgReq == nullptr )
                {
                    throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                }
                if( pMsgReq->getSender() != m_pDbClt )
                {
                    throw CException( __FILE__, __LINE__, EResultInvalidSenderObjInMessage );
                }

                SErrResultInfo errResultInfo = ErrResultInfoSuccess("MsgReqDisconnect");

                if( m_state < EStateConnected )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultMessageStateMismatch);
                    errResultInfo.setAddErrInfoDscr( "Gateway is not connected" );
                }
                else if( m_reqDbClt != Database::ERequestNone )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultMessageStateMismatch);
                    errResultInfo.setAddErrInfoDscr( "Gateway is busy" );
                }

                if( m_pTmrWatchDog->isActive() )
                {
                    m_pTmrWatchDog->stop();
                }

                if( m_pIpcSocketWrapper != nullptr )
                {
                    m_iReqMsgIdDbClt = pMsgReq->getMsgId();
                    m_iReqIdDbClt = pMsgReq->getRequestId();
                    m_reqDbClt = ERequestDisconnect;

                    // Either the "disconnected" or "error" signal will be emitted by the
                    // sockets calling the "disconnectFromHost" or "abort" methods.
                    if( m_pIpcSocketWrapper->socketState() == Ipc::ESocketStateConnected )
                    {
                        if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                        {
                            strAddTrcInfo = "-+ IpcSocket::disconnectFromHost( " + m_socketDscr.getConnectionString() + " )";
                            M_TRACE(strAddTrcInfo);
                        }

                        m_pIpcSocketWrapper->disconnectFromHost();

                        if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                        {
                            strAddTrcInfo = "+- IpcSocket::disconnectFromHost( SUCCESS )";
                            M_TRACE(strAddTrcInfo);
                        }
                    } // if( m_pIpcSocketWrapper->socketState() == Ipc::ESocketStateConnected )

                    else if( m_pIpcSocketWrapper->socketState() == Ipc::ESocketStateConnecting )
                    {
                        if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                        {
                            strAddTrcInfo = "-+ IpcSocket::abort( " + m_socketDscr.getConnectionString() + " )";
                            M_TRACE(strAddTrcInfo);
                        }

                        m_pIpcSocketWrapper->abort();

                        if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) || m_arpTrcMsgLogObjects.size() > 0 )
                        {
                            strAddTrcInfo = "+- IpcSocket::abort( SUCCESS )";
                            M_TRACE(strAddTrcInfo);
                        }
                    } // if( m_pIpcSocketWrapper->socketState() == Ipc::ESocketStateConnecting )
                } // if( m_pIpcSocketWrapper != nullptr )

                else // if( m_pIpcSocketWrapper == nullptr )
                {
                    CMsgConDisconnect* pMsgCon = new CMsgConDisconnect(
                        /* pObjSender    */ this,
                        /* pObjReceiver  */ m_pDbClt,
                        /* iReqId        */ m_iReqIdDbClt,
                        /* iMsgIdReq     */ m_iReqMsgIdDbClt,
                        /* errResultInfo */ errResultInfo,
                        /* iProgress     */ 100 );
                    pMsgCon->setSocketDscr(m_socketDscr);
                    POST_OR_DELETE_MESSAGE(pMsgCon);
                    pMsgCon = nullptr;

                    m_iReqMsgIdDbClt = -1;
                    m_iReqIdDbClt = -1;
                    m_reqDbClt = ERequestNone;
                }

                if( errResultInfo.isErrorResult() )
                {
                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                }

                if( m_reqDbClt == ERequestNone )
                {
                    if( CRequestExecTree::GetInstance()->isWaiting(m_iReqIdDbClt) )
                    {
                        CRequestExecTree::GetInstance()->wake(m_iReqIdDbClt);
                    }
                    else
                    {
                        CMsgConDisconnect* pMsgCon = new CMsgConDisconnect(
                            /* pObjSender    */ this,
                            /* pObjReceiver  */ m_pDbClt,
                            /* iReqId        */ m_iReqIdDbClt,
                            /* iMsgIdReq     */ m_iReqMsgIdDbClt,
                            /* errResultInfo */ errResultInfo,
                            /* iProgress     */ 100 );
                        pMsgCon->setSocketDscr(m_socketDscr);
                        POST_OR_DELETE_MESSAGE(pMsgCon);
                        pMsgCon = nullptr;
                    }

                    m_iReqMsgIdDbClt = -1;
                    m_iReqIdDbClt = -1;

                } // if( m_reqDbClt == ERequestNone )

                else // if( m_reqDbClt != ERequestNone )
                {
                    if( m_socketDscr.m_iConnectTimeout_ms > 0 )
                    {
                        if( !m_pTmrReqTimeout->isActive() )
                        {
                            m_pTmrReqTimeout->start(m_socketDscr.m_iConnectTimeout_ms);
                        }
                    }
                }
            } // if( pMsg->type() == EMsgTypeReqDisconnect )

            else if( static_cast<int>(pMsg->type()) == EMsgTypeReqReceiveData )
            {
                CMsgReqReceiveData* pMsgReq = dynamic_cast<CMsgReqReceiveData*>(pMsg);
                if( pMsgReq == nullptr )
                {
                    throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                }

                if( m_pIpcSocketWrapper != nullptr )
                {
                    if( m_pIpcSocketWrapper->socketState() == Ipc::ESocketStateConnected )
                    {
                        QByteArray byteArr = receiveDataBlock_L(m_pIpcSocketWrapper);

                        if( byteArr.size() > 0 )
                        {
                            CMsg* pMsg = CMsgClassFactory::CreateInstance(byteArr);

                            if( pMsg != nullptr )
                            {
                                if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) )
                                {
                                    mthTracer.trace( "Received: " + pMsg->getAddTrcInfoStr() );
                                }

                                // On receiving a confirmation message ..
                                if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeCon )
                                {
                                    // .. the original request must have been stored in the gateways hash of requests
                                    // (see comment on sending the request message to the database). To avoid memory
                                    // leaks the request got to be deleted again.
                                    CMsgCon* pMsgCon = dynamic_cast<CMsgCon*>(pMsg);
                                    qint64   iReqId = pMsgCon->getRequestId();

                                    if( m_hshReqsDb.contains(iReqId) )
                                    {
                                        CRequest* pReq = m_hshReqsDb[iReqId];
                                        qint64 iReqIdParent = pReq->getParentId();

                                        pReq->setProgressInPerCent(pMsgCon->getProgress());
                                        pReq->setErrResultInfo(pMsgCon->getErrResultInfo());
                                        pReq->update();

                                        if( pMsgCon->getProgress() < 100 )
                                        {
                                            if( !CRequestExecTree::GetInstance()->isBlockingRequest(iReqIdParent) )
                                            {
                                                // Forward message to client:
                                                pMsgCon->setSender(this);
                                                pMsgCon->setSenderId(m_pIpcSocketWrapper->getSocketId());
                                                pMsgCon->setReceiver(m_pDbClt);
                                                pMsgCon->setRequestId(iReqIdParent);
                                                POST_OR_DELETE_MESSAGE(pMsgCon);
                                                pMsgCon = nullptr;
                                                pMsg = nullptr;
                                            }
                                        }
                                        else // if( pMsgCon->getProgress() >= 100 )
                                        {
                                            m_hshReqsDb.remove(iReqId);
                                            delete pReq;
                                            pReq = nullptr;

                                            // Querying data from the database as a blocking request doesn't really make sense.
                                            // The confirmation message must contain data which will be received in the gateway thread.
                                            // The gateway would have to store the data somewhere in the request before waking up
                                            // the request changing the thread affinity of the data objects.
                                            if( CRequestExecTree::GetInstance()->isWaiting(iReqIdParent) )
                                            {
                                                CRequestExecTree::GetInstance()->wake(iReqIdParent);
                                            }
                                            else
                                            {
                                                // Forward message to client:
                                                pMsgCon->setSender(this);
                                                pMsgCon->setSenderId(m_pIpcSocketWrapper->getSocketId());
                                                pMsgCon->setReceiver(m_pDbClt);
                                                pMsgCon->setRequestId(iReqIdParent);
                                                POST_OR_DELETE_MESSAGE(pMsgCon);
                                                pMsgCon = nullptr;
                                                pMsg = nullptr;
                                            }
                                        } // if( pMsgCon->getProgress() >= 100 )
                                    } // if( m_hshReqsDb.contains(iReqId) )
                                }
                                else // if( pMsg->getSystemMsgType() != ZS::System::MsgProtocol::ESystemMsgTypeCon )
                                {
                                    // Forward message to client:
                                    pMsg->setSender(this);
                                    pMsg->setSenderId(m_pIpcSocketWrapper->getSocketId());
                                    pMsg->setReceiver(m_pDbClt);
                                    POST_OR_DELETE_MESSAGE(pMsg);
                                    pMsg = nullptr;
                                }

                                // If not sent delete message.
                                delete pMsg;
                                pMsg = nullptr;

                            } // if( pMsg != nullptr )
                        } // if( byteArr.size() > 0 )

                        // Please note that the socket may have been closed while waiting for data
                        // emitting the "error" or "disconnected" signals (see corresponding slots).
                        if( m_pIpcSocketWrapper->socketState() == Ipc::ESocketStateUnconnected )
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
                            POST_OR_DELETE_MESSAGE(pMsgReq);
                            pMsgReq = nullptr;
                        }
                    } // if( pIpcSocketWrapper->socketState() == Ipc::ESocketStateConnected )
                } // if( pIpcSocketWrapper != nullptr )
            } // if( pMsg->type() == EMsgTypeReqReceiveData )

            else // if( pMsg->type() != EMsgTypeReqConnect && pMsg->type() != EMsgTypeReqDisconnect && pMsg->type() != EMsgTypeReqReceiveData )
            {
                CMsgReq* pMsgReq = dynamic_cast<CMsgReq*>(pMsg);

                if( pMsgReq == nullptr )
                {
                    throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                }
                if( pMsgReq->getSender() != m_pDbClt )
                {
                    throw CException( __FILE__, __LINE__, EResultInvalidSenderObjInMessage );
                }

                SErrResultInfo errResultInfo = ErrResultInfoSuccess(pMsgReq->msgTypeToStr());

                if( m_state != EStateConnected )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultRequestRefused);
                    errResultInfo.setAddErrInfoDscr( "Gateway is not connected" );
                }
                else if( m_reqDbClt != Database::ERequestNone )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultRequestRefused);
                    errResultInfo.setAddErrInfoDscr( "Gateway is busy" );
                }
                else if( m_pIpcSocketWrapper == nullptr )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInternalProgramError);
                    errResultInfo.setAddErrInfoDscr( "No Socket" );
                }
                else if( m_pIpcSocketWrapper->socketState() != Ipc::ESocketStateConnected )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInternalProgramError);
                    errResultInfo.setAddErrInfoDscr( "Socket Not Connected" );
                }

                QByteArray byteArr = pMsgReq->serialize();

                if( byteArr.size() == 0 )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInternalProgramError);
                    errResultInfo.setAddErrInfoDscr( "Serializing message resulted in empty data buffer." );
                }

                if( !errResultInfo.isErrorResult() )
                {
                    // The clients request method may have been called as a blocking request method.
                    // "isBlockingRequest" would therefore return true for the request id of the client
                    // passed with the request message. If we would not create a request here the receiver
                    // of the request connect message (the database) may wake the blocked thread (which is
                    // the thread of the client) and will not send the confirmation message to this gateway.
                    // For this we need to create an unblocked request as a child of the clients request.

                    CRequest* pReq = new CRequest(
                        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
                        /* strCreatorClassName      */ "CDbClientGateway",
                        /* pObjCreator              */ this,
                        /* iRequest                 */ pMsgReq->type(),
                        /* strRequest               */ pMsgReq->msgTypeToStr(),
                        /* iReqIdParent             */ pMsgReq->getRequestId(),
                        /* bKeepReqDscrInExecTree   */ true,
                        /* iTimeout_ms              */ 0,
                        /* bIsBlocking              */ false );
                    m_hshReqsDb[pReq->getId()] = pReq;

                    System::MsgProtocol::SMsgHeader* pMsgHdr = reinterpret_cast<System::MsgProtocol::SMsgHeader*>(byteArr.data());

                    pMsgHdr->m_idRequest = pReq->getId();

                    errResultInfo = writeDataBlock_L( m_pIpcSocketWrapper, byteArr );

                    // Please note that the socket may have been closed while sending data
                    // emitting the "error" or "disconnected" signals (see corresponding slots).
                    if( m_pIpcSocketWrapper->socketState() == Ipc::ESocketStateUnconnected )
                    {
                        onDisconnected(m_pIpcSocketWrapper);
                    }
                } // if( !errResultInfo.isErrorResult() )

                if( errResultInfo.isErrorResult() )
                {
                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }

                    if( CRequestExecTree::GetInstance()->isWaiting(pMsgReq->getRequestId()) )
                    {
                        CRequestExecTree::GetInstance()->wake(pMsgReq->getRequestId());
                    }
                    else // if( !CRequestExecTree::GetInstance()->isBlockingRequest(pMsgReq->getRequestId()) )
                    {
                        CMsgCon* pMsgCon = nullptr;

                        switch( static_cast<int>(pMsg->type()) )
                        {
                            case EMsgTypeReqRegister:
                            {
                                pMsgCon = new CMsgConRegister(
                                    /* pObjSender    */ this,
                                    /* pObjReceiver  */ pMsgReq->getSender(),
                                    /* iReqId        */ pMsgReq->getRequestId(),
                                    /* iMsgIdReq     */ pMsgReq->getMsgId(),
                                    /* errResultInfo */ errResultInfo,
                                    /* iProgress     */ 100 );
                                break;
                            }
                            case EMsgTypeReqUnregister:
                            {
                                pMsgCon = new CMsgConUnregister(
                                    /* pObjSender    */ this,
                                    /* pObjReceiver  */ pMsgReq->getSender(),
                                    /* iReqId        */ pMsgReq->getRequestId(),
                                    /* iMsgIdReq     */ pMsgReq->getMsgId(),
                                    /* errResultInfo */ errResultInfo,
                                    /* iProgress     */ 100 );
                                break;
                            }
                            case EMsgTypeReqSelectSchema:
                            {
                                pMsgCon = new CMsgConSelectSchema(
                                    /* pObjSender    */ this,
                                    /* pObjReceiver  */ pMsgReq->getSender(),
                                    /* iReqId        */ pMsgReq->getRequestId(),
                                    /* iMsgIdReq     */ pMsgReq->getMsgId(),
                                    /* errResultInfo */ errResultInfo,
                                    /* iProgress     */ 100 );
                                break;
                            }
                            case EMsgTypeReqUpdateSchema:
                            {
                                pMsgCon = new CMsgConUpdateSchema(
                                    /* pObjSender    */ this,
                                    /* pObjReceiver  */ pMsgReq->getSender(),
                                    /* iReqId        */ pMsgReq->getRequestId(),
                                    /* iMsgIdReq     */ pMsgReq->getMsgId(),
                                    /* errResultInfo */ errResultInfo,
                                    /* iProgress     */ 100 );
                                break;
                            }
                            case EMsgTypeReqSelectData:
                            {
                                pMsgCon = new CMsgConSelectData(
                                    /* pObjSender    */ this,
                                    /* pObjReceiver  */ pMsgReq->getSender(),
                                    /* iReqId        */ pMsgReq->getRequestId(),
                                    /* iMsgIdReq     */ pMsgReq->getMsgId(),
                                    /* errResultInfo */ errResultInfo,
                                    /* iProgress     */ 100 );
                                break;
                            }
                            case EMsgTypeReqUpdateData:
                            {
                                pMsgCon = new CMsgConUpdateData(
                                    /* pObjSender    */ this,
                                    /* pObjReceiver  */ pMsgReq->getSender(),
                                    /* iReqId        */ pMsgReq->getRequestId(),
                                    /* iMsgIdReq     */ pMsgReq->getMsgId(),
                                    /* errResultInfo */ errResultInfo,
                                    /* iProgress     */ 100 );
                                break;
                            }
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
                                    throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMessage, pMsg->msgTypeToStr() );
                                }
                                break;
                            }
                        } // switch( pMsg->type() )

                        if( pMsgCon != nullptr )
                        {
                            POST_OR_DELETE_MESSAGE(pMsgCon);
                            pMsgCon = nullptr;
                        }
                    } // if( !CRequestExecTree::GetInstance()->isBlockingRequest(pMsgReq->getRequestId()) )
                } // if( errResultInfo.isErrorResult() )
            } // if( pMsg->type() != EMsgTypeReqConnect && pMsg->type() != EMsgTypeReqDisconnect && pMsg->type() != EMsgTypeReqReceiveData )
        } // if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeReq )

        else if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeCon )
        {
            CMsgCon* pMsgCon = dynamic_cast<CMsgCon*>(pMsg);

            if( pMsgCon == nullptr )
            {
                throw CException( __FILE__, __LINE__, EResultMessageTypeMismatch );
            }
            if( pMsgCon->getSender() != m_pDbClt )
            {
                throw CException( __FILE__, __LINE__, EResultInvalidSenderObjInMessage );
            }

            SErrResultInfo errResultInfo = ErrResultInfoSuccess(pMsgCon->msgTypeToStr());

            if( m_state != EStateConnected )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultMessageStateMismatch);
                errResultInfo.setAddErrInfoDscr( "Gateway is not connected" );
            }
            else if( m_reqDbClt != Database::ERequestNone )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultMessageStateMismatch);
                errResultInfo.setAddErrInfoDscr( "Gateway is busy" );
            }

            QByteArray byteArr;

            if( !errResultInfo.isErrorResult() )
            {
                byteArr = pMsgCon->serialize();

                if( byteArr.size() == 0 )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInternalProgramError);
                    errResultInfo.setAddErrInfoDscr( "Serializing message resulted in empty data buffer." );
                }
            }

            if( !errResultInfo.isErrorResult() )
            {
                if( m_pIpcSocketWrapper != nullptr )
                {
                    errResultInfo = writeDataBlock_L( m_pIpcSocketWrapper, byteArr );

                    // Please note that the socket may have been closed while sending data
                    // emitting the "error" or "disconnected" signals (see corresponding slots).
                    if( m_pIpcSocketWrapper->socketState() == Ipc::ESocketStateUnconnected )
                    {
                        onDisconnected(m_pIpcSocketWrapper);
                    }
                }
            } // if( !errResultInfo.isErrorResult() )

            if( errResultInfo.isErrorResult() )
            {
                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
            }
        } // if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeCon )
    } // if( pMsg != nullptr )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pEv);
    }
    return bEventHandled;

} // event
