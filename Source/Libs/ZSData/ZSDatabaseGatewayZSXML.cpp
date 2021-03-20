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

#include "ZSData/ZSDatabaseGatewayZSXML.h"
#include "ZSData/ZSDatabase.h"
#include "ZSData/ZSDataMsg.h"
#include "ZSIpc/ZSIpcBlkTypeSL.h"
#include "ZSIpc/ZSIpcServerWrapperTcp.h"
#include "ZSIpc/ZSIpcSocketWrapperTcp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysSleeperThread.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
//#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Data;
using namespace ZS::Trace;


/*******************************************************************************
class CDbGatewayZSXML : public CDbGateway
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDbGatewayZSXML::CDbGatewayZSXML(
    ZS::Ipc::ESocketType i_socketType,
    CDb*                 i_pDb,
    CDbGatewayThread*    i_pGatewayThread ) :
//------------------------------------------------------------------------------
    CDbGateway(
        /* protocolType   */ EProtocolTypeZSXML,
        /* socketType     */ i_socketType,
        /* pDb            */ i_pDb,
        /* pGatewayThread */ i_pGatewayThread ),
    m_pIpcServerWrapper(nullptr),
    m_arpIpcSocketWrappers()
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
CDbGatewayZSXML::~CDbGatewayZSXML()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ strAddTrcInfo );

    int iSocketId;

    if( m_arpIpcSocketWrappers.size() > 0 )
    {
        for( iSocketId = m_arpIpcSocketWrappers.size()-1; iSocketId >= 0; iSocketId-- )
        {
            try
            {
                delete m_arpIpcSocketWrappers[iSocketId];
            }
            catch(...)
            {
            }
            m_arpIpcSocketWrappers[iSocketId] = nullptr;
        }
    }
    m_arpIpcSocketWrappers.clear();

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

} // dtor

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDbGatewayZSXML::onNewConnection( QObject* /*i_pServerWrapper*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onNewConnection",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "ReqInProgress: " + Request2Str(m_requestInProgress);
        strAddTrcInfo += ", ReqIdInProgress: " + QString::number(m_iReqIdInProgress);
        strAddTrcInfo += ", MsgReqIdProgress: " + QString::number(m_iMsgIdReqInProgress);
        mthTracer.trace(strAddTrcInfo);
    }

    Ipc::CIpcSocketWrapper* pIpcSocketWrapper = m_pIpcServerWrapper->nextPendingConnection();

    if( pIpcSocketWrapper == nullptr )
    {
        QString strAddErrInfo = "IpcServerWrapper->nextPendingConnection() returned nullptr";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddErrInfo);
    }

    int iSocketId;

    // Search free entry in list of sockets ..
    for( iSocketId = 0; iSocketId < m_arpIpcSocketWrappers.size(); iSocketId++ )
    {
        if( m_arpIpcSocketWrappers[iSocketId] == nullptr )
        {
            break;
        }
    }
    if( iSocketId >= m_arpIpcSocketWrappers.size() )
    {
        m_arpIpcSocketWrappers.append(pIpcSocketWrapper);
    }
    else
    {
        m_arpIpcSocketWrappers[iSocketId] = pIpcSocketWrapper;
    }

    pIpcSocketWrapper->setSocketId(iSocketId);

    if( !QObject::connect(
        /* pObjSender   */ pIpcSocketWrapper,
        /* szSignal     */ SIGNAL(disconnected(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDisconnected(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ pIpcSocketWrapper,
        /* szSignal     */ SIGNAL(readyRead(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onReadyRead(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ pIpcSocketWrapper,
        /* szSignal     */ SIGNAL(error(QObject*,ZS::System::SErrResultInfo&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onError(QObject*,ZS::System::SErrResultInfo&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    ZS::Ipc::SSocketDscr socketDscr = pIpcSocketWrapper->getSocketDscr();

    CMsgIndConnected* pMsgInd = new CMsgIndConnected(
        /* pObjSender   */ this,
        /* pObjReceiver */ m_pDb,
        /* protocolType */ m_protocolType,
        /* cnctSettings */ socketDscr );
    POST_OR_DELETE_MESSAGE(pMsgInd,&mthTracer);
    pMsgInd = nullptr;

} // onNewConnection

/*==============================================================================
protected slots: // must overridables of base class CSrvCltBaseGateway
==============================================================================*/

//------------------------------------------------------------------------------
void CDbGatewayZSXML::onDisconnected( QObject* i_pSocketWrapper )
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

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "ReqInProgress: " + Request2Str(m_requestInProgress);
        strAddTrcInfo += ", ReqIdInProgress: " + QString::number(m_iReqIdInProgress);
        strAddTrcInfo += ", MsgReqIdProgress: " + QString::number(m_iMsgIdReqInProgress);
        mthTracer.trace(strAddTrcInfo);
    }

    Ipc::CIpcSocketWrapper* pIpcSocketWrapper = dynamic_cast<Ipc::CIpcSocketWrapper*>(i_pSocketWrapper);

    if( pIpcSocketWrapper == nullptr )
    {
        QString strAddErrInfo = "disconnected( ";
        strAddErrInfo += "pIpcSocketWrapper == nullptr )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddErrInfo);
    }

    int iSocketId = pIpcSocketWrapper->getSocketId();

    if( iSocketId < 0 || iSocketId >= m_arpIpcSocketWrappers.size() )
    {
        QString strAddErrInfo = "disconnected( ";
        strAddErrInfo += "SocketId = " + QString::number(iSocketId) + " > " + QString::number(m_arpIpcSocketWrappers.size()) + " )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddErrInfo);
    }
    if( m_arpIpcSocketWrappers[iSocketId] != i_pSocketWrapper )
    {
        QString strAddErrInfo = "disconnected( ";
        strAddErrInfo += "m_arpIpcSocketWrappers[" + QString::number(iSocketId) + "] != i_pSocketWrapper )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddErrInfo);
    }

    switch( m_requestInProgress )
    {
        case ERequestNone:
        case ERequestUpdateHostSettings:
        {
            if( !QObject::disconnect(
                /* pObjSender   */ m_arpIpcSocketWrappers[iSocketId],
                /* szSignal     */ SIGNAL(disconnected(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDisconnected(QObject*)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::disconnect(
                /* pObjSender   */ m_arpIpcSocketWrappers[iSocketId],
                /* szSignal     */ SIGNAL(readyRead(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onReadyRead(QObject*)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::disconnect(
                /* pObjSender   */ m_arpIpcSocketWrappers[iSocketId],
                /* szSignal     */ SIGNAL(error(QObject*,ZS::System::SErrResultInfo&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onError(QObject*,ZS::System::SErrResultInfo&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            Ipc::SSocketDscr socketDscr = pIpcSocketWrapper->getSocketDscr();

            try
            {
                delete m_arpIpcSocketWrappers[iSocketId];
            }
            catch(...)
            {
            }
            m_arpIpcSocketWrappers[iSocketId] = nullptr;

            CMsgIndDisconnected* pMsgInd = new CMsgIndDisconnected(
                /* pObjSender   */ this,
                /* pObjReceiver */ m_pDb,
                /* protocolType */ m_protocolType,
                /* cnctSettings */ socketDscr );
            POST_OR_DELETE_MESSAGE(pMsgInd,&mthTracer);
            pMsgInd = nullptr;
            break;
        } // case ERequestUpdateHostSettings

        case ERequestStartup:
        case ERequestShutdown:
        {
            // The socket state will be checked after the "receiveDataBlock" method returns.
            break;
        }
        //case ERequestExecCommand:
        {
            // The socket state will be checked after the "receiveDataBlock" method returns.
            break;
        }
        default:
        {
            throw CException(__FILE__,__LINE__,EResultMethodStateMismatch,Request2Str(m_requestInProgress));
        }
    }

} // onDisconnected

//------------------------------------------------------------------------------
void CDbGatewayZSXML::onReadyRead( QObject* i_pSocketWrapper )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onReadyRead",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "ReqInProgress: " + Request2Str(m_requestInProgress);
        strAddTrcInfo += ", ReqIdInProgress: " + QString::number(m_iReqIdInProgress);
        strAddTrcInfo += ", MsgReqIdProgress: " + QString::number(m_iMsgIdReqInProgress);
        mthTracer.trace(strAddTrcInfo);
    }

    Ipc::CIpcSocketWrapper* pIpcSocketWrapper = dynamic_cast<Ipc::CIpcSocketWrapper*>(i_pSocketWrapper);

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
    POST_OR_DELETE_MESSAGE(pMsgReq,&mthTracer);
    pMsgReq = nullptr;

} // onReadyRead

//------------------------------------------------------------------------------
void CDbGatewayZSXML::onError( QObject* i_pSocketWrapper, ZS::System::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onError",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "ReqInProgress: " + Request2Str(m_requestInProgress);
        strAddTrcInfo += ", ReqIdInProgress: " + QString::number(m_iReqIdInProgress);
        strAddTrcInfo += ", MsgReqIdProgress: " + QString::number(m_iMsgIdReqInProgress);
        mthTracer.trace(strAddTrcInfo);
    }

    Ipc::CIpcSocketWrapper* pIpcSocketWrapper = dynamic_cast<Ipc::CIpcSocketWrapper*>(i_pSocketWrapper);

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
        case ERequestUpdateHostSettings:
        {
            break;
        }
        default:
        {
            throw CException(__FILE__,__LINE__,EResultInternalProgramError,Request2Str(m_requestInProgress));
        }
    }

    if( result != EResultSuccess )
    {
        CMsgErr* pMsgErr = new CMsgErr(
            /* pObjSender    */ this,
            /* pObjReceiver  */ m_pDb,
            /* errResultInfo */ errResultInfo );
        POST_OR_DELETE_MESSAGE(pMsgErr,&mthTracer);
        pMsgErr = nullptr;
    }

} // onError

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CDbGatewayZSXML::event( QEvent* i_pMsg )
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

            } // if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )

            CRequestExecTree* pReqExecTree = CRequestExecTree::GetInstance();

            if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeReq )
            {
                //------------------------------------------------------------------
                if( static_cast<int>(pMsg->type()) == EMsgTypeReqStartup )
                //------------------------------------------------------------------
                {
                    CMsgReqStartup* pMsgReq = dynamic_cast<CMsgReqStartup*>(i_pMsg);

                    if( pMsgReq == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReq->getSender() != m_pDb )
                    {
                        throw CException( __FILE__, __LINE__, EResultInvalidSenderObjInMessage, pMsg->msgTypeToStr() );
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

                        m_hostSettings = pMsgReq->getServerHostSettings();

                        delete m_pIpcServerWrapper;
                        m_pIpcServerWrapper = nullptr;

                        m_pIpcServerWrapper = new Ipc::CTcpServerWrapper(objectName());

                        if( !QObject::connect(
                            /* pObjSender   */ m_pIpcServerWrapper,
                            /* szSignal     */ SIGNAL(newConnection(QObject*)),
                            /* pObjReceiver */ this,
                            /* szSlot       */ SLOT(onNewConnection(QObject*)) ) )
                        {
                            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                        }

                        m_pIpcServerWrapper->setMaxPendingConnections( static_cast<int>(m_hostSettings.m_uMaxPendingConnections) );
                        m_pIpcServerWrapper->setLocalHostName(m_hostSettings.m_strLocalHostName);
                        m_pIpcServerWrapper->setLocalPort(m_hostSettings.m_uLocalPort);

                        if( !m_pIpcServerWrapper->listen() )
                        {
                            errResultInfo = m_pIpcServerWrapper->errResultInfo();

                            if( errResultInfo.getErrSource().isObjectPathEmpty() )
                            {
                                errResultInfo.setErrSource(nameSpace(), className(), objectName(), strMth);
                            }
                            if( errResultInfo.getAddErrInfoDscr().isEmpty() )
                            {
                                errResultInfo.setAddErrInfoDscr( m_hostSettings.getConnectionString() );
                            }
                        }
                    } // if( m_requestInProgress == ERequestNone )

                    if( m_requestInProgress == ERequestStartup )
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

                        m_requestInProgress   = ERequestNone;
                        m_iReqIdInProgress    = -1;
                        m_iMsgIdReqInProgress = -1;

                    } // if( m_requestInProgress == ERequestStartup )
                } // if( pMsg->type() == EMsgTypeReqStartup )

                //------------------------------------------------------------------
                else if( static_cast<int>(pMsg->type()) == EMsgTypeReqShutdown )
                //------------------------------------------------------------------
                {
                    ZS::Data::CMsgReqShutdown* pMsgReq = dynamic_cast<ZS::Data::CMsgReqShutdown*>(i_pMsg);

                    if( pMsgReq == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReq->getSender() != m_pDb )
                    {
                        throw CException( __FILE__, __LINE__, EResultInvalidSenderObjInMessage, pMsg->msgTypeToStr() );
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

                    if( m_arpIpcSocketWrappers.size() > 0 )
                    {
                        int iSocketId;

                        for( iSocketId = m_arpIpcSocketWrappers.size()-1; iSocketId >= 0; iSocketId-- )
                        {
                            try
                            {
                                delete m_arpIpcSocketWrappers[iSocketId];
                            }
                            catch(...)
                            {
                            }
                            m_arpIpcSocketWrappers[iSocketId] = nullptr;
                        }
                        m_arpIpcSocketWrappers.clear();
                    }

                    if( m_pIpcServerWrapper != nullptr )
                    {
                        m_pIpcServerWrapper->close();

                        try
                        {
                            delete m_pIpcServerWrapper;
                        }
                        catch(...)
                        {
                        }
                        m_pIpcServerWrapper = nullptr;

                    } // if( m_pIpcServerWrapper != nullptr )

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

                } // if( pMsg->type() == EMsgTypeReqShutdown )

                //------------------------------------------------------------------
                else if( static_cast<int>(pMsg->type()) == EMsgTypeReqChangeSettings )
                //------------------------------------------------------------------
                {
                    ZS::Data::CMsgReqChangeSettings* pMsgReq = dynamic_cast<ZS::Data::CMsgReqChangeSettings*>(i_pMsg);

                    if( pMsgReq == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReq->getSender() != this && pMsgReq->getSender() != m_pDb )
                    {
                        throw CException( __FILE__, __LINE__, EResultInvalidSenderObjInMessage, "MsgReqStartup" );
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
                        m_requestInProgress   = ERequestUpdateHostSettings;
                        m_iReqIdInProgress    = pMsgReq->getRequestId();
                        m_iMsgIdReqInProgress = pMsgReq->getMsgId();

                        Ipc::SServerHostSettings hostSettingsNew = pMsgReq->getServerHostSettings();

                        if( hostSettingsNew != m_hostSettings )
                        {
                            int iSocketId;

                            if( m_arpIpcSocketWrappers.size() > 0 )
                            {
                                for( iSocketId = m_arpIpcSocketWrappers.size()-1; iSocketId >= 0; iSocketId-- )
                                {
                                    if( m_arpIpcSocketWrappers[iSocketId] != nullptr )
                                    {
                                        onDisconnected(m_arpIpcSocketWrappers[iSocketId]);
                                    }

                                    try
                                    {
                                        delete m_arpIpcSocketWrappers[iSocketId];
                                    }
                                    catch(...)
                                    {
                                    }
                                    m_arpIpcSocketWrappers[iSocketId] = nullptr;
                                }
                                m_arpIpcSocketWrappers.clear();
                            }

                            m_hostSettings = hostSettingsNew;

                            if( m_pIpcServerWrapper != nullptr )
                            {
                                m_pIpcServerWrapper->close();

                                m_pIpcServerWrapper->setMaxPendingConnections( static_cast<int>(m_hostSettings.m_uMaxPendingConnections) );
                                m_pIpcServerWrapper->setLocalHostName(m_hostSettings.m_strLocalHostName);
                                m_pIpcServerWrapper->setLocalPort(m_hostSettings.m_uLocalPort);

                            } // if( m_pIpcServerWrapper != nullptr )
                        } // if( hostSettingsNew != m_hostSettings )

                        if( m_pIpcServerWrapper != nullptr )
                        {
                            if( !m_pIpcServerWrapper->isListening() )
                            {
                                if( !m_pIpcServerWrapper->listen() )
                                {
                                    errResultInfo = m_pIpcServerWrapper->errResultInfo();

                                    if( errResultInfo.getErrSource().isObjectPathEmpty() )
                                    {
                                        errResultInfo.setErrSource(nameSpace(), className(), objectName(), strMth);
                                    }
                                    if( errResultInfo.getAddErrInfoDscr().isEmpty() )
                                    {
                                        errResultInfo.setAddErrInfoDscr( m_hostSettings.getConnectionString() );
                                    }
                                }
                            }
                        } // if( m_pIpcServerWrapper != nullptr )
                    } // if( m_requestInProgress == ERequestNone )

                    if( m_requestInProgress == ERequestUpdateHostSettings )
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
                                mthTracer.trace( "-+ ReqUpdateHostSettings.WaitCondition.wakeAll()" );
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
                                mthTracer.trace( "+- ReqUpdateHostSettings.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
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

                    } // if( m_requestInProgress == ERequestUpdateHostSettings )
                } // if( pMsg->type() == EMsgTypeReqChangeSettings )

                //------------------------------------------------------------------
                //else if( pMsg->type() == EMsgTypeReqReceiveData )
                //------------------------------------------------------------------
                //{
                //    ZS::Data::CMsgReqReceiveData* pMsgReq = dynamic_cast<ZS::Data::CMsgReqReceiveData*>(i_pMsg);

                //    if( pMsgReq == nullptr )
                //    {
                //        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                //    }

                //    Ipc::CIpcSocketWrapper* pIpcSocketWrapper = nullptr;
                //    int                     iSocketId = pMsgReq->getSocketId();
                //    bool                    bSocketIdIsValid = true;

                //    if( iSocketId < 0 || iSocketId >= m_arpIpcSocketWrappers.size() )
                //    {
                //        bSocketIdIsValid = false;
                //    }
                //    else if( m_arpIpcSocketWrappers[iSocketId] == nullptr )
                //    {
                //        bSocketIdIsValid = false;
                //    }
                //    else if( iSocketId != m_arpIpcSocketWrappers[iSocketId]->getSocketId() )
                //    {
                //        bSocketIdIsValid = false;
                //    }
                //    if( bSocketIdIsValid )
                //    {
                //        pIpcSocketWrapper = m_arpIpcSocketWrappers[iSocketId];
                //    }
                //    if( pIpcSocketWrapper != nullptr && pIpcSocketWrapper->socketState() == Ipc::ESocketStateConnected )
                //    {
                //        CMsg*   pMsgRcvd = nullptr;
                //        QString strErrSource;
                //        QString strAddErrInfo;

                //        strErrSource  = objectName();
                //        strErrSource += ", MsgSender: " + pIpcSocketWrapper->getRemoteHostName();

                //        errResultInfo.setErrSource(strErrSource);

                //        if( pIpcSocketWrapper->socketType() == Ipc::ESocketTypeInProcMsg )
                //        {
                //            Ipc::CInProcMsgSocketWrapper* pInProcMsgSocketWrapper = dynamic_cast<Ipc::CInProcMsgSocketWrapper*>(pIpcSocketWrapper);

                //            pMsgRcvd = pInProcMsgSocketWrapper->readMessage();

                //            if( pMsgRcvd != nullptr )
                //            {
                //                if( mthTracer.isActive() )
                //                {
                //                    mthTracer.trace( "Received: " + pMsgRcvd->getAddTrcInfoStr() );
                //                }

                //                pMsgRcvd->setSender(this);
                //                pMsgRcvd->setSenderId(pIpcSocketWrapper->getSocketId());
                //                pMsgRcvd->setReceiver(m_pDb);
                //                POST_OR_DELETE_MESSAGE(pMsgRcvd,&mthTracer);
                //                pMsgRcvd = nullptr;

                //            } // if( pMsgRcvd != nullptr )
                //        } // if( m_pIpcSocketWrapper->socketType() == ESocketTypeInProcMsg )

                //        else // if( m_pIpcSocketWrapper->socketType() == Ipc::ESocketTypeTcp || m_pIpcSocketWrapper->socketType() == Ipc::ESocketTypeShm )
                //        {
                //            Ipc::CBlkTypeSL blkType = ZS::Data::getIpcBlockType();

                //            QList<QByteArray> arByteArrs = blkType.receiveDataBlocks(
                //                /* srvCltType          */ ZS::Ipc::ESrvCltTypeServer,
                //                /* pObjGtw             */ this,
                //                /* pObjSrvClt          */ m_pDb,
                //                /* pSocketWrapper      */ pIpcSocketWrapper,
                //                /* pByteArrWatchDog    */ &ZS::Data::getIpcWatchDogBlock(),
                //                /* pTrcAdminObj        */ m_pTrcAdminObj,
                //                /* arpTrcMsgLogObjects */ m_arpTrcMsgLogObjects );

                //            QByteArray byteArr;

                //            for( int idxBlk = 0; idxBlk < arByteArrs.size(); idxBlk++ )
                //            {
                //                byteArr = arByteArrs[idxBlk];

                //                if( byteArr.size() > 0 )
                //                {
                //                    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
                //                    {
                //                        strAddTrcInfo  = "-+ MsgClassFactory::CreateInstance";
                //                        mthTracer.trace(strAddTrcInfo);
                //                    }

                //                    pMsgRcvd = CMsgClassFactory::CreateInstance(byteArr,pIpcSocketWrapper);

                //                    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
                //                    {
                //                        strAddTrcInfo  = "+- MsgClassFactory::CreateInstance: ";
                //                        strAddTrcInfo += QString( pMsgRcvd == nullptr ? "{nullptr}" : "{" + pMsgRcvd->getAddTrcInfoStr(mthTracer.getDetailLevel()) + "}");
                //                        mthTracer.trace(strAddTrcInfo);
                //                    }

                //                    if( pMsgRcvd != nullptr )
                //                    {
                //                        if( mthTracer.isActive() )
                //                        {
                //                            mthTracer.trace( "Received: " + pMsgRcvd->getAddTrcInfoStr() );
                //                        }

                //                        pMsgRcvd->setSender(this);
                //                        pMsgRcvd->setSenderId(pIpcSocketWrapper->getSocketId());
                //                        pMsgRcvd->setReceiver(m_pDb);
                //                        POST_OR_DELETE_MESSAGE(pMsgRcvd,&mthTracer);
                //                        pMsgRcvd = nullptr;

                //                    } // if( pMsgRcvd != nullptr )
                //                } // if( byteArr.size() > 0 )
                //            } // for( int idxBlk = 0; idxBlk < arByteArrs.size(); idxBlk++ )
                //        } // if( m_pIpcSocketWrapper->socketType() == Ipc::ESocketTypeTcp || m_pIpcSocketWrapper->socketType() == Ipc::ESocketTypeShm )

                //        if( errResultInfo.isErrorResult() )
                //        {
                //            CMsgErr* pMsgErr = new CMsgErr(
                //                /* pObjSender    */ this,
                //                /* pObjReceiver  */ m_pDb,
                //                /* errResultInfo */ errResultInfo );
                //            POST_OR_DELETE_MESSAGE(pMsgErr,&mthTracer);
                //            pMsgErr = nullptr;
                //        }

                //        // Please note that the socket may have been closed while waiting for data
                //        // emitting the "error" or "disconnected" signals (see corresponding slots).
                //        if( pIpcSocketWrapper->socketState() == Ipc::ESocketStateUnconnected )
                //        {
                //            onDisconnected(pIpcSocketWrapper);
                //        }
                //        else if( pIpcSocketWrapper->bytesAvailable() > 0 )
                //        {
                //            ZS::Data::CMsgReqReceiveData* pMsgReq = new ZS::Data::CMsgReqReceiveData(
                //                /* pObjSender       */ this,
                //                /* pObjReceiver     */ this,
                //                /* iSocketId        */ pIpcSocketWrapper->getSocketId(),
                //                /* bMustBeConfirmed */ false,
                //                /* iReqId           */ -1 );
                //            POST_OR_DELETE_MESSAGE(pMsgReq,&mthTracer);
                //            pMsgReq = nullptr;
                //        }
                //    } // if( pIpcSocketWrapper != nullptr && pIpcSocketWrapper->socketState() == Ipc::ESocketStateConnected )
                //} // else if( pMsg->type() == EMsgTypeReqReceiveData )

                //------------------------------------------------------------------
                //else if( pMsg->type() == EMsgTypeReqRegister )
                //------------------------------------------------------------------
                //{
                //    errResultInfo.setSeverity(EResultSeverityError);
                //    errResultInfo.setResult(EResultInvalidMessage);
                //    errResultInfo.setAddErrInfoDscr( pMsg->getAddTrcInfoStr() );

                //} // if( pMsg->type() == EMsgTypeReqRegister )

                //------------------------------------------------------------------
                //else if( pMsg->type() == EMsgTypeReqUnregister )
                //------------------------------------------------------------------
                //{
                //    errResultInfo.setSeverity(EResultSeverityError);
                //    errResultInfo.setResult(EResultInvalidMessage);
                //    errResultInfo.setAddErrInfoDscr( pMsg->getAddTrcInfoStr() );

                //} // if( pMsg->type() == EMsgTypeReqUnregister )

                //------------------------------------------------------------------
                //else if( pMsg->type() == EMsgTypeReqReadSchema )
                //------------------------------------------------------------------
                //{
                //    errResultInfo.setSeverity(EResultSeverityError);
                //    errResultInfo.setResult(EResultInvalidMessage);
                //    errResultInfo.setAddErrInfoDscr( pMsg->getAddTrcInfoStr() );

                //} // if( pMsg->type() == EMsgTypeReqReadSchema )

                //------------------------------------------------------------------
                //else if( pMsg->type() == EMsgTypeReqSaveSchema )
                //------------------------------------------------------------------
                //{
                //    errResultInfo.setSeverity(EResultSeverityError);
                //    errResultInfo.setResult(EResultInvalidMessage);
                //    errResultInfo.setAddErrInfoDscr( pMsg->getAddTrcInfoStr() );

                //} // if( pMsg->type() == EMsgTypeReqSaveSchema )

                //------------------------------------------------------------------
                //else if( pMsg->type() == EMsgTypeReqSelectSchema )
                //------------------------------------------------------------------
                //{
                //    errResultInfo.setSeverity(EResultSeverityError);
                //    errResultInfo.setResult(EResultInvalidMessage);
                //    errResultInfo.setAddErrInfoDscr( pMsg->getAddTrcInfoStr() );

                //} // if( pMsg->type() == EMsgTypeReqSelectSchema )

                //------------------------------------------------------------------
                //else if( pMsg->type() == EMsgTypeReqUpdateSchema )
                //------------------------------------------------------------------
                //{
                //    ZS::Data::CMsgReqUpdateSchema* pMsgReq = dynamic_cast<ZS::Data::CMsgReqUpdateSchema*>(i_pMsg);

                //    if( pMsgReq == nullptr )
                //    {
                //        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                //    }
                //    if( pMsgReq->getSender() != this && pMsgReq->getSender() != m_pDb )
                //    {
                //        throw CException( __FILE__, __LINE__, EResultInvalidSenderObjInMessage, pMsgReq->getAddTrcInfoStr() );
                //    }
                //    if( m_requestInProgress != ERequestNone )
                //    {
                //        throw CException( __FILE__, __LINE__, EResultMessageStateMismatch, "MsgReqChangeSettings, RequestInProgress: " + Request2Str(m_requestInProgress) );
                //    }

                //    Ipc::CIpcSocketWrapper* pIpcSocketWrapper = nullptr;
                //    SCnctId                 cnctId = pMsgReq->getCnctId();
                //    bool                    bSocketIdIsValid = true;

                //    if( cnctId.m_iSocketId < 0 || cnctId.m_iSocketId >= m_arpIpcSocketWrappers.size() )
                //    {
                //        bSocketIdIsValid = false;
                //    }
                //    else if( m_arpIpcSocketWrappers[cnctId.m_iSocketId] == nullptr )
                //    {
                //        bSocketIdIsValid = false;
                //    }
                //    else if( cnctId.m_iSocketId != m_arpIpcSocketWrappers[cnctId.m_iSocketId]->getSocketId() )
                //    {
                //        bSocketIdIsValid = false;
                //    }
                //    if( bSocketIdIsValid )
                //    {
                //        pIpcSocketWrapper = m_arpIpcSocketWrappers[cnctId.m_iSocketId];
                //    }

                //    if( pIpcSocketWrapper == nullptr || pIpcSocketWrapper->socketState() != Ipc::ESocketStateConnected )
                //    {
                //        errResultInfo.setSeverity(EResultSeverityError);
                //        errResultInfo.setResult(EResultMessageStateMismatch);
                //        errResultInfo.setAddErrInfoDscr( "Gateway is not connected or invalid socket id" );
                //    }

                //    if( !errResultInfo.isErrorResult() )
                //    {
                //        if( bTrcDetailLevel1Active || m_arpTrcMsgLogObjects.size() > 0 )
                //        {
                //            strAddTrcInfo = "-+ sendMessage( " + pMsgReq->getAddTrcInfoStr(0) + " )";

                //            if( mthTracer.isActive() )
                //            {
                //                mthTracer.trace(strAddTrcInfo);
                //            }
                //            for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                //            {
                //                CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                //                    /* pObjSender   */ this,
                //                    /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                //                    /* transmitDir  */ ETransmitDir::Undefined,
                //                    /* bBold        */ false,
                //                    /* strMsg       */ strAddTrcInfo );
                //                POST_OR_DELETE_MESSAGE(pMsgLogItem);
                //                pMsgLogItem = nullptr;
                //            }
                //        }

                //        QByteArray byteArrMsg = pMsgReq->serialize();

                //        if( byteArrMsg.size() == 0 )
                //        {
                //            errResultInfo.setSeverity(EResultSeverityError);
                //            errResultInfo.setResult(EResultInternalProgramError);
                //            errResultInfo.setAddErrInfoDscr( "Serializing message resulted in empty data buffer." );
                //        }
                //        else // if( byteArrMsg.size() > 0 )
                //        {
                //            // Forward confirmaton message to client:
                //            Ipc::CBlkTypeSL blkType = ZS::Data::getIpcBlockType();

                //            bool bDataSent = blkType.writeDataBlock(
                //                /* srvCltType          */ ZS::Ipc::ESrvCltTypeClient,
                //                /* pObjGtw             */ this,
                //                /* pObjSrvClt          */ m_pDb,
                //                /* pSocketWrapper      */ pIpcSocketWrapper,
                //                /* byteArr             */ byteArrMsg,
                //                /* bIsWatchDogBlock    */ false,
                //                /* pTrcAdminObj        */ m_pTrcAdminObj,
                //                /* arpTrcMsgLogObjects */ m_arpTrcMsgLogObjects );

                //            // Please note that the socket may have been closed while sending data
                //            // emitting the "error" or "disconnected" signals (see corresponding slots).
                //            if( pIpcSocketWrapper->socketState() == Ipc::ESocketStateUnconnected )
                //            {
                //                onDisconnected(pIpcSocketWrapper);
                //            }
                //        } // if( byteArrMsg.size() > 0 )

                //        if( bTrcDetailLevel1Active || m_arpTrcMsgLogObjects.size() > 0 )
                //        {
                //            if( errResultInfo.isErrorResult() )
                //            {
                //                strAddTrcInfo = "+- sendMessage( ERROR: " + errResultInfo.getAddErrInfoDscr() + " )";
                //            }
                //            else
                //            {
                //                strAddTrcInfo = "+- sendMessage( SUCCESS )";
                //            }
                //            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
                //            {
                //                mthTracer.trace(strAddTrcInfo);
                //            }
                //            for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                //            {
                //                CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                //                    /* pObjSender   */ this,
                //                    /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
                //                    /* transmitDir  */ ETransmitDir::Undefined,
                //                    /* bBold        */ false,
                //                    /* strMsg       */ strAddTrcInfo );
                //                POST_OR_DELETE_MESSAGE(pMsgLogItem);
                //                pMsgLogItem = nullptr;
                //            }
                //        } // if( bTrcDetailLevel1Active || m_arpTrcMsgLogObjects.size() > 0 )
                //    } // if( !errResultInfo.isErrorResult() )

                //    if( pMsgReq->mustBeConfirmed() )
                //    {
                //        ZS::Data::CMsgConUpdateSchema* pMsgCon = new ZS::Data::CMsgConUpdateSchema(
                //            /* pObjSender    */ this,
                //            /* pObjReceiver  */ pMsgReq->getSender(),
                //            /* cnctId        */ cnctId,
                //            /* iReqId        */ pMsgReq->getRequestId(),
                //            /* iMsgIdReq     */ pMsgReq->getMsgId(),
                //            /* errResultInfo */ errResultInfo,
                //            /* iProgress     */ 10 );
                //        POST_OR_DELETE_MESSAGE(pMsgCon,&mthTracer);
                //        pMsgCon = nullptr;
                //    }
                //} // if( pMsg->type() == EMsgTypeReqUpdateSchema )

                //------------------------------------------------------------------
                //else if( pMsg->type() == EMsgTypeReqReadData )
                //------------------------------------------------------------------
                //{
                //    errResultInfo.setSeverity(EResultSeverityError);
                //    errResultInfo.setResult(EResultInvalidMessage);
                //    errResultInfo.setAddErrInfoDscr( pMsg->getAddTrcInfoStr() );

                //} // if( pMsg->type() == EMsgTypeReqReadData )

                //------------------------------------------------------------------
                //else if( pMsg->type() == EMsgTypeReqSaveData )
                //------------------------------------------------------------------
                //{
                //    errResultInfo.setSeverity(EResultSeverityError);
                //    errResultInfo.setResult(EResultInvalidMessage);
                //    errResultInfo.setAddErrInfoDscr( pMsg->getAddTrcInfoStr() );

                //} // if( pMsg->type() == EMsgTypeReqSaveData )

                //------------------------------------------------------------------
                //else if( pMsg->type() == EMsgTypeReqSelectData )
                //------------------------------------------------------------------
                //{
                //    errResultInfo.setSeverity(EResultSeverityError);
                //    errResultInfo.setResult(EResultInvalidMessage);
                //    errResultInfo.setAddErrInfoDscr( pMsg->getAddTrcInfoStr() );

                //} // if( pMsg->type() == EMsgTypeReqSelectData )

                //------------------------------------------------------------------
                //else if( pMsg->type() == EMsgTypeReqUpdateData )
                //------------------------------------------------------------------
                //{
                //    errResultInfo.setSeverity(EResultSeverityError);
                //    errResultInfo.setResult(EResultInvalidMessage);
                //    errResultInfo.setAddErrInfoDscr( pMsg->getAddTrcInfoStr() );

                //} // if( pMsg->type() == EMsgTypeReqUpdateData )

                //------------------------------------------------------------------
                //else if( pMsg->type() == EMsgTypeReqSet2Default )
                //------------------------------------------------------------------
                //{
                //    errResultInfo.setSeverity(EResultSeverityError);
                //    errResultInfo.setResult(EResultInvalidMessage);
                //    errResultInfo.setAddErrInfoDscr( pMsg->getAddTrcInfoStr() );

                //} // if( pMsg->type() == EMsgTypeReqSet2Default )
            } // if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeReq )

            //else if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeCon )
            //{
            //    ZS::Data::CMsgCon* pMsgCon = dynamic_cast<ZS::Data::CMsgCon*>(pMsg);

            //    if( pMsgCon == nullptr )
            //    {
            //        throw CException( __FILE__, __LINE__, EResultMessageTypeMismatch );
            //    }
            //    if( pMsgCon->getSender() != m_pDb )
            //    {
            //        throw CException( __FILE__, __LINE__, EResultInvalidSenderObjInMessage );
            //    }

            //    if( m_requestInProgress != ERequestNone )
            //    {
            //        errResultInfo.setSeverity(EResultSeverityError);
            //        errResultInfo.setResult(EResultMessageStateMismatch);
            //        errResultInfo.setAddErrInfoDscr( "Gateway is busy" );
            //    }

            //    Ipc::CIpcSocketWrapper* pIpcSocketWrapper = nullptr;
            //    int                     iSocketId = pMsgCon->getSocketId();
            //    bool                    bSocketIdIsValid = true;

            //    if( iSocketId < 0 || iSocketId >= m_arpIpcSocketWrappers.size() )
            //    {
            //        bSocketIdIsValid = false;
            //    }
            //    else if( m_arpIpcSocketWrappers[iSocketId] == nullptr )
            //    {
            //        bSocketIdIsValid = false;
            //    }
            //    else if( iSocketId != m_arpIpcSocketWrappers[iSocketId]->getSocketId() )
            //    {
            //        bSocketIdIsValid = false;
            //    }
            //    if( bSocketIdIsValid )
            //    {
            //        pIpcSocketWrapper = m_arpIpcSocketWrappers[iSocketId];
            //    }

            //    if( pIpcSocketWrapper == nullptr || pIpcSocketWrapper->socketState() != Ipc::ESocketStateConnected )
            //    {
            //        errResultInfo.setSeverity(EResultSeverityError);
            //        errResultInfo.setResult(EResultMessageStateMismatch);
            //        errResultInfo.setAddErrInfoDscr( "Gateway is not connected or invalid socket id" );
            //    }

            //    if( !errResultInfo.isErrorResult() )
            //    {
            //        if( bTrcDetailLevel1Active || m_arpTrcMsgLogObjects.size() > 0 )
            //        {
            //            strAddTrcInfo = "-+ sendMessage( " + pMsgCon->getAddTrcInfoStr(0) + " )";

            //            if( mthTracer.isActive() )
            //            {
            //                mthTracer.trace(strAddTrcInfo);
            //            }
            //            for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
            //            {
            //                CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
            //                    /* pObjSender   */ this,
            //                    /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
            //                    /* transmitDir  */ ETransmitDir::Undefined,
            //                    /* bBold        */ false,
            //                    /* strMsg       */ strAddTrcInfo );
            //                POST_OR_DELETE_MESSAGE(pMsgLogItem);
            //                pMsgLogItem = nullptr;
            //            }
            //        }

            //        QByteArray byteArrMsg = pMsgCon->serialize();

            //        if( byteArrMsg.size() == 0 )
            //        {
            //            errResultInfo.setSeverity(EResultSeverityError);
            //            errResultInfo.setResult(EResultInternalProgramError);
            //            errResultInfo.setAddErrInfoDscr( "Serializing message resulted in empty data buffer." );
            //        }
            //        else // if( byteArrMsg.size() > 0 )
            //        {
            //            // Forward confirmaton message to client:
            //            Ipc::CBlkTypeSL blkType = ZS::Data::getIpcBlockType();

            //            bool bDataSent = blkType.writeDataBlock(
            //                /* srvCltType          */ ZS::Ipc::ESrvCltTypeClient,
            //                /* pObjGtw             */ this,
            //                /* pObjSrvClt          */ m_pDb,
            //                /* pSocketWrapper      */ pIpcSocketWrapper,
            //                /* byteArr             */ byteArrMsg,
            //                /* bIsWatchDogBlock    */ false,
            //                /* pTrcAdminObj        */ m_pTrcAdminObj,
            //                /* arpTrcMsgLogObjects */ m_arpTrcMsgLogObjects );

            //            // Please note that the socket may have been closed while sending data
            //            // emitting the "error" or "disconnected" signals (see corresponding slots).
            //            if( pIpcSocketWrapper->socketState() == Ipc::ESocketStateUnconnected )
            //            {
            //                onDisconnected(pIpcSocketWrapper);
            //            }
            //        } // if( byteArrMsg.size() > 0 )

            //        if( bTrcDetailLevel1Active || m_arpTrcMsgLogObjects.size() > 0 )
            //        {
            //            if( errResultInfo.isErrorResult() )
            //            {
            //                strAddTrcInfo = "+- sendMessage( ERROR: " + errResultInfo.getAddErrInfoDscr() + " )";
            //            }
            //            else
            //            {
            //                strAddTrcInfo = "+- sendMessage( SUCCESS )";
            //            }
            //            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
            //            {
            //                mthTracer.trace(strAddTrcInfo);
            //            }
            //            for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
            //            {
            //                CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
            //                    /* pObjSender   */ this,
            //                    /* pObjReceiver */ m_arpTrcMsgLogObjects[idxObj],
            //                    /* transmitDir  */ ETransmitDir::Undefined,
            //                    /* bBold        */ false,
            //                    /* strMsg       */ strAddTrcInfo );
            //                POST_OR_DELETE_MESSAGE(pMsgLogItem);
            //                pMsgLogItem = nullptr;
            //            }
            //        } // if( bTrcDetailLevel1Active || m_arpTrcMsgLogObjects.size() > 0 )
            //    } // if( !errResultInfo.isErrorResult() )
            //} // if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeCon )

            if( errResultInfo.isErrorResult() )
            {
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
