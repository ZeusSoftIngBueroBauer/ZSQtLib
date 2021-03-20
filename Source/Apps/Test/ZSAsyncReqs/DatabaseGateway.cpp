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

#include "DatabaseGateway.h"
#include "Msg.h"

#include "ZSIpc/ZSIpcBlkType.h"
#include "ZSIpc/ZSIpcServerWrapperInProcMsg.h"
#include "ZSIpc/ZSIpcServerWrapperShm.h"
#include "ZSIpc/ZSIpcServerWrapperTcp.h"
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
class CDbGatewayThread : public QThread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDbGatewayThread::CDbGatewayThread(
    const QString&       i_strObjName,
    ZS::Ipc::ESocketType i_socketType,
    CDb*                 i_pDb ) :
//------------------------------------------------------------------------------
    QThread(),
    m_pDb(i_pDb),
    m_socketType(i_socketType),
    m_pWaitCond(nullptr),
    m_pDbGtw(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::AsyncRequests", "CDbGatewayThread", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
CDbGatewayThread::~CDbGatewayThread()
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
    //    delete m_pDbGtw;
    //}
    //catch(...)
    //{
    //}

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDb = nullptr;
    m_pWaitCond = nullptr;
    m_pDbGtw = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
protected: // must overridables (overridables of base class QThread)
==============================================================================*/

//------------------------------------------------------------------------------
void CDbGatewayThread::start()
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
void CDbGatewayThread::start( QWaitCondition* i_pWaitCond )
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
void CDbGatewayThread::run()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "run",
        /* strAddInfo   */ "" );

    m_pDbGtw = new CDbGateway( objectName(), m_socketType, m_pDb );

    if( m_pWaitCond != nullptr )
    {
        m_pWaitCond->wakeAll();
    }

    exec();

    try
    {
        delete m_pDbGtw;
    }
    catch(...)
    {
    }
    m_pDbGtw = nullptr;

} // run


/*******************************************************************************
class CDbGateway : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDbGateway::CDbGateway(
    const QString&       i_strObjName,
    ZS::Ipc::ESocketType i_socketType,
    CDb*                 i_pDb ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pDb(i_pDb),
    m_hostSettings(i_socketType),
    m_pIpcServerWrapper(nullptr),
    m_hshpIpcSocketWrappers(),
    m_arpTrcMsgLogObjects(),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::AsyncRequests", "CDbGateway", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
CDbGateway::~CDbGateway()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    int iSocketId;

    if( m_hshpIpcSocketWrappers.size() > 0 )
    {
        for( iSocketId = m_hshpIpcSocketWrappers.size()-1; iSocketId >= 0; iSocketId-- )
        {
            try
            {
                delete m_hshpIpcSocketWrappers[iSocketId];
            }
            catch(...)
            {
            }
            m_hshpIpcSocketWrappers[iSocketId] = nullptr;
        }
    }
    m_hshpIpcSocketWrappers.clear();

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

    m_pDb = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDbGateway::onNewConnectionPending( QObject* /*i_pServerWrapper*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onNewConnectionPending",
        /* strAddInfo   */ strAddTrcInfo );

    Ipc::CIpcSocketWrapper* pIpcSocketWrapper = nullptr;

    bool bSocketIdExisting = true;
    int iSocketId = 0;

    while( bSocketIdExisting )
    {
        bSocketIdExisting = false;

        QHash<int,Ipc::CIpcSocketWrapper*>::const_iterator itSockets = m_hshpIpcSocketWrappers.begin();

        while( itSockets != m_hshpIpcSocketWrappers.end() )
        {
            pIpcSocketWrapper = itSockets.value();

            if( pIpcSocketWrapper->getSocketId() == iSocketId )
            {
                bSocketIdExisting = true;
                iSocketId++;
                break;
            }
            itSockets++;
        }
    } // while( !bSocketIdFound )

    pIpcSocketWrapper = m_pIpcServerWrapper->nextPendingConnection();

    if( pIpcSocketWrapper == nullptr )
    {
        QString strAddErrInfo = "newConnectionPending( ";
        strAddErrInfo += "pIpcSocketWrapper == nullptr )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddErrInfo);
    }

    m_hshpIpcSocketWrappers[iSocketId] = pIpcSocketWrapper;

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

    CMsgIndConnected* pMsgInd = new CMsgIndConnected(
        /* pObjSender          */ this,
        /* pObjReceiver        */ m_pDb,
        /* bMustBeAcknowledged */ false,
        /* iReqId              */ -1,
        /* iMsgId              */ -1 );
    pMsgInd->setSocketDscr( pIpcSocketWrapper->getSocketDscr() );
    POST_OR_DELETE_MESSAGE(pMsgInd);
    pMsgInd = nullptr;

} // onNewConnectionPending

//------------------------------------------------------------------------------
void CDbGateway::onDisconnected( QObject* i_pSocketWrapper )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDisconnected",
        /* strAddInfo   */ strAddTrcInfo );

    Ipc::CIpcSocketWrapper* pIpcSocketWrapper = dynamic_cast<Ipc::CIpcSocketWrapper*>(i_pSocketWrapper);

    if( pIpcSocketWrapper == nullptr )
    {
        QString strAddErrInfo = "disconnected( ";
        strAddErrInfo += "pIpcSocketWrapper == nullptr )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddErrInfo);
    }

    int iSocketId = pIpcSocketWrapper->getSocketId();

    if( !m_hshpIpcSocketWrappers.contains(iSocketId) )
    {
        QString strAddErrInfo = "disconnected( ";
        strAddErrInfo += "SocketId = " + QString::number(iSocketId) + " > " + QString::number(m_hshpIpcSocketWrappers.size()) + " )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddErrInfo);
    }
    if( m_hshpIpcSocketWrappers[iSocketId] != i_pSocketWrapper )
    {
        QString strAddErrInfo = "disconnected( ";
        strAddErrInfo += "m_hshpIpcSocketWrappers[" + QString::number(iSocketId) + "] != i_pSocketWrapper )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddErrInfo);
    }

    if( !QObject::disconnect(
        /* pObjSender   */ pIpcSocketWrapper,
        /* szSignal     */ SIGNAL(disconnected(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDisconnected(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::disconnect(
        /* pObjSender   */ pIpcSocketWrapper,
        /* szSignal     */ SIGNAL(readyRead(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onReadyRead(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::disconnect(
        /* pObjSender   */ pIpcSocketWrapper,
        /* szSignal     */ SIGNAL(error(QObject*,ZS::System::SErrResultInfo&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onError(QObject*,ZS::System::SErrResultInfo&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    Ipc::SSocketDscr socketDscr = pIpcSocketWrapper->getSocketDscr();

    m_hshpIpcSocketWrappers.remove(iSocketId);

    try
    {
        delete pIpcSocketWrapper;
    }
    catch(...)
    {
    }
    pIpcSocketWrapper = nullptr;

    CMsgIndDisconnected* pMsgInd = new CMsgIndDisconnected(
        /* pObjSender          */ this,
        /* pObjReceiver        */ m_pDb,
        /* bMustBeAcknowledged */ false,
        /* iReqId              */ -1,
        /* iMsgId              */ -1 );
    pMsgInd->setSocketDscr(socketDscr);
    POST_OR_DELETE_MESSAGE(pMsgInd);
    pMsgInd = nullptr;

} // onDisconnected

//------------------------------------------------------------------------------
void CDbGateway::onReadyRead( QObject* i_pSocketWrapper )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onReadyRead",
        /* strAddInfo   */ strAddTrcInfo );

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
    POST_OR_DELETE_MESSAGE(pMsgReq);
    pMsgReq = nullptr;

} // onReadyRead

//------------------------------------------------------------------------------
void CDbGateway::onError( QObject* i_pSocketWrapper, ZS::System::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_errResultInfo.getResultStr();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onError",
        /* strAddInfo   */ strAddTrcInfo );

    Ipc::CIpcSocketWrapper* pIpcSocketWrapper = dynamic_cast<Ipc::CIpcSocketWrapper*>(i_pSocketWrapper);

    if( pIpcSocketWrapper == nullptr )
    {
        QString strAddErrInfo = "onError( pIpcSocketWrapper == nullptr )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddErrInfo);
    }

    SErrResultInfo errResultInfo = i_errResultInfo;

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

    switch( errResultInfo.getResult() )
    {
        // If the remote client closed the connection ..
        case EResultRemoteHostClosed:
        {
            // .. its not a real error. The connection will be closed calling the disconnected slot.
            errResultInfo.setErrResult(ErrResultSuccess);
        }
        default:
        {
            break;
        }
    }

    if( errResultInfo.isErrorResult() )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

} // onError

/*==============================================================================
public: //instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CDbGateway::receiveDataBlock_L( ZS::Ipc::CIpcSocketWrapper* i_pSocketWrapper )
//------------------------------------------------------------------------------
{
    // +--------+----------+
    // | LENgth | Data ... |
    // +--------+----------+

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_pSocketWrapper->getRemoteHostName() + ":" + QString::number(i_pSocketWrapper->getServerListenPort()) + ":" + QString::number(i_pSocketWrapper->getRemotePort());
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
        qint32      iBlkSizeLen = sizeof(iBlkSize);
        QByteArray  byteArrBlkSize(iBlkSizeLen,0x00);
        bool        bReadyRead;
        const int   iReadTimeout_ms = 1000;
        double      fStartTime_ms;
        double      fCurrTime_ms;

        // Wait until the number of bytes containing the length of the block have been received ..
        fStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
        while( i_pSocketWrapper->bytesAvailable() < iBlkSizeLen )
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
            byteArrBlkSize = i_pSocketWrapper->read(byteArrBlkSize.size());

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
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CDbGateway::writeDataBlock_L( ZS::Ipc::CIpcSocketWrapper* i_pSocketWrapper, const QByteArray& i_byteArr )
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

        //const MsgProtocol::SMsgHeader* pMsgHdr = reinterpret_cast<const MsgProtocol::SMsgHeader*>(i_byteArr.data());

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
bool CDbGateway::event( QEvent* i_pEv )
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
            strAddTrcInfo = "Msg: " + pMsg->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDetailLevel */ ETraceDetailLevelMethodCalls,
            /* strMethod    */ "event",
            /* strAddInfo   */ strAddTrcInfo );

        if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq )
        {
            if( static_cast<int>(pMsg->type()) == EMsgTypeReqStartup )
            {
                CMsgReqStartup* pMsgReq = dynamic_cast<CMsgReqStartup*>(pMsg);
                if( pMsgReq == nullptr )
                {
                    throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                }
                if( pMsgReq->getSender() != m_pDb )
                {
                    throw CException( __FILE__, __LINE__, EResultInvalidSenderObjInMessage, "MsgReqStartup" );
                }

                SErrResultInfo errResultInfo = ErrResultInfoSuccess("MsgReqStartup");

                if( errResultInfo.isErrorResult() )
                {
                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                }
                else // if( !errResultInfo.isErrorResult() )
                {
                    m_hostSettings = pMsgReq->getHostSettings();

                    switch( m_hostSettings.m_socketType )
                    {
                        case Ipc::ESocketTypeTcp:
                        {
                            m_pIpcServerWrapper = new Ipc::CTcpServerWrapper(objectName());
                            break;
                        }
                        case Ipc::ESocketTypeShm:
                        {
                            m_pIpcServerWrapper = new Ipc::CShmServerWrapper(objectName());
                            break;
                        }
                        case Ipc::ESocketTypeInProcMsg:
                        {
                            m_pIpcServerWrapper = new Ipc::CInProcMsgServerWrapper(objectName());
                            break;
                        }
                        default:
                        {
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultInvalidMessageContent);
                            errResultInfo.setAddErrInfoDscr( "Socket type " + Ipc::socketType2Str(m_hostSettings.m_socketType) + " out of range." );
                            break;
                        }
                    }

                    if( m_pIpcServerWrapper != nullptr )
                    {
                        if( !QObject::connect(
                            /* pObjSender   */ m_pIpcServerWrapper,
                            /* szSignal     */ SIGNAL(newConnection(QObject*)),
                            /* pObjReceiver */ this,
                            /* szSlot       */ SLOT(onNewConnectionPending(QObject*)) ) )
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
                                errResultInfo.setErrSource(nameSpace(), className(), objectName(), pMsg->msgTypeToStr());
                            }
                            if( errResultInfo.getAddErrInfoDscr().isEmpty() )
                            {
                                errResultInfo.setAddErrInfoDscr( m_hostSettings.m_strLocalHostName + ":" + QString::number(m_hostSettings.m_uLocalPort) );
                            }
                        }
                    } // if( m_pIpcServerWrapper != nullptr )

                    if( CRequestExecTree::GetInstance()->isWaiting(pMsgReq->getRequestId()) )
                    {
                        CRequestExecTree::GetInstance()->wake(pMsgReq->getRequestId());
                    }
                    else
                    {
                        CMsgConStartup* pMsgCon = new CMsgConStartup(
                            /* pObjSender    */ this,
                            /* pObjReceiver  */ pMsgReq->getSender(),
                            /* iReqId        */ pMsgReq->getRequestId(),
                            /* iMsgIdReq     */ pMsgReq->getMsgId(),
                            /* errResultInfo */ errResultInfo,
                            /* iProgress     */ 100 );
                        pMsgCon->setHostSettings(m_hostSettings);
                        POST_OR_DELETE_MESSAGE(pMsgCon);
                        pMsgCon = nullptr;
                    }
                } // if( !errResultInfo.isErrorResult() )
            } // if( pMsg->type() == EMsgTypeReqStartup )

            else if( static_cast<int>(pMsg->type()) == EMsgTypeReqShutdown )
            {
                CMsgReqShutdown* pMsgReq = dynamic_cast<CMsgReqShutdown*>(pMsg);
                if( pMsgReq == nullptr )
                {
                    throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                }
                if( pMsgReq->getSender() != m_pDb )
                {
                    throw CException( __FILE__, __LINE__, EResultInvalidSenderObjInMessage, "MsgReqShutdown" );
                }

                SErrResultInfo errResultInfo = ErrResultInfoSuccess("MsgReqShutdown");

                if( errResultInfo.isErrorResult() )
                {
                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                }
                else // if( !errResultInfo.isErrorResult() )
                {
                    switch( m_hostSettings.m_socketType )
                    {
                        case Ipc::ESocketTypeTcp:
                        {
                            break;
                        }
                        case Ipc::ESocketTypeShm:
                        {
                            break;
                        }
                        case Ipc::ESocketTypeInProcMsg:
                        {
                            break;
                        }
                        default:
                        {
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultInvalidMessageContent);
                            errResultInfo.setAddErrInfoDscr( "Socket type " + Ipc::socketType2Str(m_hostSettings.m_socketType) + " out of range." );
                            break;
                        }
                    }

                    if( CRequestExecTree::GetInstance()->isWaiting(pMsgReq->getRequestId()) )
                    {
                        CRequestExecTree::GetInstance()->wake(pMsgReq->getRequestId());
                    }
                    else
                    {
                        CMsgConShutdown* pMsgCon = new CMsgConShutdown(
                            /* pObjSender    */ this,
                            /* pObjReceiver  */ pMsgReq->getSender(),
                            /* iReqId        */ pMsgReq->getRequestId(),
                            /* iMsgIdReq     */ pMsgReq->getMsgId(),
                            /* errResultInfo */ errResultInfo,
                            /* iProgress     */ 100 );
                        POST_OR_DELETE_MESSAGE(pMsgCon);
                        pMsgCon = nullptr;
                    }
                } // if( !errResultInfo.isErrorResult() )
            } // if( pMsg->type() == EMsgTypeReqShutdown )

            else if( static_cast<int>(pMsg->type()) == EMsgTypeReqReceiveData )
            {
                CMsgReqReceiveData* pMsgReq = dynamic_cast<CMsgReqReceiveData*>(pMsg);
                if( pMsgReq == nullptr )
                {
                    throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                }

                SErrResultInfo errResultInfo = ErrResultInfoSuccess("MsgReqReceiveData");

                int iSocketId = pMsgReq->getSocketId();

                if( !m_hshpIpcSocketWrappers.contains(iSocketId) )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultSocketIdOutOfRange);
                    errResultInfo.setAddErrInfoDscr( "SocketId:" + QString::number(iSocketId) );
                }

                if( !errResultInfo.isErrorResult() )
                {
                    Ipc::CIpcSocketWrapper* pIpcSocketWrapper = m_hshpIpcSocketWrappers[iSocketId];

                    if( pIpcSocketWrapper != nullptr )
                    {
                        if( pIpcSocketWrapper->socketState() == Ipc::ESocketStateConnected )
                        {
                            QByteArray byteArr = receiveDataBlock_L(pIpcSocketWrapper);

                            if( byteArr.size() > 0 )
                            {
                                CMsg* pMsg = CMsgClassFactory::CreateInstance(byteArr);

                                if( pMsg != nullptr )
                                {
                                    if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) )
                                    {
                                        mthTracer.trace( "Received: " + pMsg->getAddTrcInfoStr() );
                                    }

                                    pMsg->setSender(this);
                                    pMsg->setSenderId(pIpcSocketWrapper->getSocketId());
                                    pMsg->setReceiver(m_pDb);
                                    POST_OR_DELETE_MESSAGE(pMsg);
                                    pMsg = nullptr;
                                }
                            }

                            // Please note that the socket may have been closed while waiting for data
                            // emitting the "error" or "disconnected" signals (see corresponding slots).
                            if( pIpcSocketWrapper->socketState() == Ipc::ESocketStateUnconnected )
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
                                POST_OR_DELETE_MESSAGE(pMsgReq);
                                pMsgReq = nullptr;
                            }
                        } // if( pIpcSocketWrapper->socketState() == Ipc::ESocketStateConnected )
                    } // if( pIpcSocketWrapper != nullptr )
                } // if( !errResultInfo.isErrorResult() )
            } // else if( pMsg->type() == EMsgTypeReqReceiveData )

            else // if( pMsg->type() != EMsgTypeReqStartup && pMsg->type() != EMsgTypeReqShutdown && pMsg->type() != EMsgTypeReqReceiveData )
            {
                CMsgReq* pMsgReq = dynamic_cast<CMsgReq*>(pMsg);

                if( pMsgReq == nullptr )
                {
                    throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                }
                if( pMsgReq->getSender() != m_pDb )
                {
                    throw CException( __FILE__, __LINE__, EResultInvalidSenderObjInMessage );
                }

                SErrResultInfo errResultInfo = ErrResultInfoSuccess(pMsgReq->msgTypeToStr());

                int iSocketId = pMsgReq->getReceiverId();

                if( !m_hshpIpcSocketWrappers.contains(iSocketId) )
                {
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultSocketIdOutOfRange);
                    errResultInfo.setAddErrInfoDscr( "SocketId:" + QString::number(iSocketId) );
                }

                QByteArray byteArr;

                if( !errResultInfo.isErrorResult() )
                {
                    byteArr = pMsgReq->serialize();

                    if( byteArr.size() == 0 )
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultInternalProgramError);
                        errResultInfo.setAddErrInfoDscr( "Serializing message resulted in empty data buffer." );
                    }
                }

                if( !errResultInfo.isErrorResult() )
                {
                    Ipc::CIpcSocketWrapper* pIpcSocketWrapper = m_hshpIpcSocketWrappers[iSocketId];

                    if( pIpcSocketWrapper != nullptr )
                    {
                        errResultInfo = writeDataBlock_L( pIpcSocketWrapper, byteArr );

                        // Please note that the socket may have been closed while sending data
                        // emitting the "error" or "disconnected" signals (see corresponding slots).
                        if( pIpcSocketWrapper->socketState() == Ipc::ESocketStateUnconnected )
                        {
                            onDisconnected(pIpcSocketWrapper);
                        }
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
                                SErrResultInfo errResultInfo = ErrResultInfoSuccess(pMsg->msgTypeToStr());

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

                        if( pMsgCon != nullptr )
                        {
                            POST_OR_DELETE_MESSAGE(pMsgCon);
                            pMsgCon = nullptr;
                        }
                    } // if( !CRequestExecTree::GetInstance()->isBlockingRequest(pMsgReq->getRequestId()) )
                } // if( errResultInfo.isErrorResult() )
            } // if( pMsg->type() != EMsgTypeReqStartup && pMsg->type() != EMsgTypeReqShutdown && pMsg->type() != EMsgTypeReqReceiveData )
        } // if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq )

        else if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeCon )
        {
            CMsgCon* pMsgCon = dynamic_cast<CMsgCon*>(pMsg);

            if( pMsgCon == nullptr )
            {
                throw CException( __FILE__, __LINE__, EResultMessageTypeMismatch );
            }
            if( pMsgCon->getSender() != m_pDb )
            {
                throw CException( __FILE__, __LINE__, EResultInvalidSenderObjInMessage );
            }

            SErrResultInfo errResultInfo = ErrResultInfoSuccess(pMsgCon->msgTypeToStr());

            int iSocketId = pMsgCon->getReceiverId();

            if( !m_hshpIpcSocketWrappers.contains(iSocketId) )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultSocketIdOutOfRange);
                errResultInfo.setAddErrInfoDscr( "SocketId:" + QString::number(iSocketId) );
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
                Ipc::CIpcSocketWrapper* pIpcSocketWrapper = m_hshpIpcSocketWrappers[iSocketId];

                if( pIpcSocketWrapper != nullptr )
                {
                    errResultInfo = writeDataBlock_L( pIpcSocketWrapper, byteArr );

                    // Please note that the socket may have been closed while sending data
                    // emitting the "error" or "disconnected" signals (see corresponding slots).
                    if( pIpcSocketWrapper->socketState() == Ipc::ESocketStateUnconnected )
                    {
                        onDisconnected(pIpcSocketWrapper);
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
        } // if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeCon )
    } // if( pMsg != nullptr )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pEv);
    }
    return bEventHandled;

} // event
