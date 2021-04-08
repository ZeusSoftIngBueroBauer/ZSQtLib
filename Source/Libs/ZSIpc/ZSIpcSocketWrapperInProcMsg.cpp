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
#include <QtCore/qmutex.h>
#include <QtCore/qwaitcondition.h>
#include <QtNetwork/qhostinfo.h>
#include <QtNetwork/qtcpsocket.h>

#include "ZSIpc/ZSIpcSocketWrapperInProcMsg.h"
#include "ZSIpc/ZSIpcInProcMsgSocket.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysSleeperThread.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;


/*******************************************************************************
class CInProcMsgSocketThread : public QThread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CInProcMsgSocketThread::CInProcMsgSocketThread(
    const QString& i_strObjName,
    ESrvCltType    i_srvCltType,
    CTrcMthFile*   i_pTrcMthFile,
    int            i_iTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    QThread(),
    m_srvCltType(i_srvCltType),
    m_pInProcMsgSocket(nullptr),
    m_iTrcMthFileDetailLevel(i_iTrcMthFileDetailLevel),
    m_pTrcMthFile(i_pTrcMthFile),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    if( m_pTrcMthFile == nullptr )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strAddInfo         */ "" );

} // ctor

//------------------------------------------------------------------------------
CInProcMsgSocketThread::~CInProcMsgSocketThread()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strAddInfo         */ "" );

    quit();
    wait();

    try
    {
        delete m_pInProcMsgSocket;
    }
    catch(...)
    {
    }

    m_pInProcMsgSocket = nullptr;

    if( m_pTrcAdminObj != nullptr )
    {
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;
    }

} // dtor

/*==============================================================================
public: // overridables of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
void CInProcMsgSocketThread::run()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "run",
        /* strAddInfo         */ "" );

    m_pInProcMsgSocket = new CInProcMsgSocket(objectName(), m_srvCltType);

    emit socketCreated(this,m_pInProcMsgSocket);

    exec();

    try
    {
        delete m_pInProcMsgSocket;
    }
    catch(...)
    {
    }
    m_pInProcMsgSocket = nullptr;

} // run


/*******************************************************************************
class CInProcMsgSocketWrapper : public CIpcSocketWrapper
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CInProcMsgSocketWrapper::CInProcMsgSocketWrapper(
    const QString&    i_strObjName,
    ESrvCltType       i_srvCltType,
    int               i_iSocketId,
    CInProcMsgSocket* i_pInProcMsgSocket,
    CTrcMthFile*      i_pTrcMthFile,
    int               i_iTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    CIpcSocketWrapper(
        /* strObjName             */ i_strObjName,
        /* srvCltType             */ i_srvCltType,
        /* socketType             */ ESocketTypeInProcMsg,
        /* iSocketId              */ i_iSocketId,
        /* pSocket                */ i_pInProcMsgSocket,
        /* pTrcMthFile            */ i_pTrcMthFile,
        /* iTrcMthFileDetailLevel */ i_iTrcMthFileDetailLevel ),
    m_pInProcMsgSocketThread(nullptr),
    m_pInProcMsgSocket(i_pInProcMsgSocket),
    m_copyDepthMsgReadBuff(ECopyDepthFlatReleaseOwnwership),
    m_bSlotOnConnectedConnected(false),
    m_bSlotOnDisconnectedConnected(false),
    m_bSlotOnErrorConnected(false),
    m_bSlotOnReadyReadConnected(false),
    m_pWaitCondSocketCreated(nullptr),
    m_pWaitCondConnected(nullptr),
    m_pWaitCondDisconnected(nullptr),
    m_pWaitCondReadyRead(nullptr)
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

    if( m_pInProcMsgSocket == nullptr )
    {
        m_pWaitCondSocketCreated = new QWaitCondition();

        m_pInProcMsgSocketThread = new CInProcMsgSocketThread(i_strObjName, i_srvCltType);

        // The slot "onSocketCreated" will be connected using DirectConnection to the
        // "socketCreated" signal. This way the slot is called directly from within the
        // IpcSocket thread context signaling the end of the wait condition.
        if( !QObject::connect(
            /* pObjSender   */ m_pInProcMsgSocketThread,
            /* szSignal     */ SIGNAL(socketCreated(QObject*,QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onSocketCreated(QObject*,QObject*)),
            /* connectType  */ Qt::DirectConnection ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        m_pInProcMsgSocketThread->start();

        QMutex mtx;
        mtx.lock();
        m_pWaitCondSocketCreated->wait(&mtx);
        mtx.unlock();

        QObject::disconnect(
            /* pObjSender   */ m_pInProcMsgSocketThread,
            /* szSignal     */ SIGNAL(socketCreated(QObject*,QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onSocketCreated(QObject*,QObject*)) );

    } // if( m_pInProcMsgSocket == nullptr )

    if( m_pInProcMsgSocket == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, "m_pInProcMsgSocket == nullptr" );
    }

    m_socketDscr.m_socketState = m_pInProcMsgSocket->state();
    m_socketDscr.m_uServerListenPort = m_pInProcMsgSocket->getServerListenPort();
    m_socketDscr.m_strLocalHostName = m_pInProcMsgSocket->getLocalHostName();
    m_socketDscr.m_uLocalPort = m_pInProcMsgSocket->getLocalPort();
    m_socketDscr.m_strRemoteHostName = m_pInProcMsgSocket->getRemoteHostName();
    m_socketDscr.m_uRemotePort = m_pInProcMsgSocket->getRemotePort();

    m_pInProcMsgSocket->setMsgReadBuffCopyDepth(m_copyDepthMsgReadBuff);

    // Please note that if the socket is created in another thread than the wrapper
    // slots using "AutoConnection" may have already been queued to the wrapper while
    // (or even before) processing the "waitFor.." methods.

    // To work with wait conditions slots must be connected to signals using
    // "DirectConnection" to allow the socket to wake the wrappers thread on
    // applying "wakeAll" to the wait condition (the wrapper thread's event loop
    // is blocked and cannot receive and process queued events while waiting on a
    // wait condition).

    // If we would "reconnect" the wrappers slots to the sockets signals within the
    // "waitFor.." methods it may happen that the thread execution changes to the
    // socket thread right after the slot has been disconnected but before the
    // socket emits the signal and the wrappers slot has been connected again
    // to the sockets signal. In this case the wrapper's slot method will not been
    // called and the wrapper would run in the timeout while waiting on the wait
    // condition.

    // That's why "DirectConnection" is always used for connecting the wrappers
    // slots to the sockets signals. If a slot is called and if no wait condition
    // is used the signals from the socket are queued by the wrapper itself in the
    // way that the events will be created and sent to the wrappers event method
    // on executing the slot methods.

    if( !QObject::connect(
        /* pObjSender   */ m_pInProcMsgSocket,
        /* szSignal     */ SIGNAL(destroyed(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onSocketDestroyed(QObject*)),
        /* connectType  */ Qt::DirectConnection ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_bSlotOnConnectedConnected = QObject::connect(
        /* pObjSender   */ m_pInProcMsgSocket,
        /* szSignal     */ SIGNAL(connected()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onConnected()),
        /* connectType  */ Qt::DirectConnection );
    if( !m_bSlotOnConnectedConnected )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_bSlotOnDisconnectedConnected = QObject::connect(
        /* pObjSender   */ m_pInProcMsgSocket,
        /* szSignal     */ SIGNAL(disconnected()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDisconnected()),
        /* connectType  */ Qt::DirectConnection );
    if( !m_bSlotOnDisconnectedConnected )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_bSlotOnReadyReadConnected = QObject::connect(
        /* pObjSender   */ m_pInProcMsgSocket,
        /* szSignal     */ SIGNAL(readyRead()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onReadyRead()),
        /* connectType  */ Qt::DirectConnection );
    if( !m_bSlotOnReadyReadConnected )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // We never wait for errors so the "onError" slot may use AutoConnection:
    m_bSlotOnErrorConnected = QObject::connect(
        /* pObjSender   */ m_pInProcMsgSocket,
        /* szSignal     */ SIGNAL(error(ZS::System::EResult)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onError(ZS::System::EResult)) );
    if( !m_bSlotOnErrorConnected )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CInProcMsgSocketWrapper::~CInProcMsgSocketWrapper()
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

    if( m_pInProcMsgSocket != nullptr )
    {
        if( m_bSlotOnConnectedConnected )
        {
            QObject::disconnect(
                /* pObjSender   */ m_pInProcMsgSocket,
                /* szSignal     */ SIGNAL(connected()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onConnected()) );
        }
        if( m_bSlotOnDisconnectedConnected )
        {
            QObject::disconnect(
                /* pObjSender   */ m_pInProcMsgSocket,
                /* szSignal     */ SIGNAL(disconnected()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDisconnected()) );
        }
        if( m_bSlotOnErrorConnected )
        {
            QObject::disconnect(
                /* pObjSender   */ m_pInProcMsgSocket,
                /* szSignal     */ SIGNAL(error(ZS::System::EResult)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onError(ZS::System::EResult)) );
        }
        if( m_bSlotOnReadyReadConnected )
        {
            QObject::disconnect(
                /* pObjSender   */ m_pInProcMsgSocket,
                /* szSignal     */ SIGNAL(readyRead()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onReadyRead()) );
        }
    } // if( m_pInProcMsgSocket != nullptr )

    m_bSlotOnConnectedConnected = false;
    m_bSlotOnDisconnectedConnected = false;
    m_bSlotOnErrorConnected = false;
    m_bSlotOnReadyReadConnected = false;

    try
    {
        delete m_pWaitCondSocketCreated;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pWaitCondConnected;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pWaitCondDisconnected;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pWaitCondReadyRead;
    }
    catch(...)
    {
    }

    if( m_pInProcMsgSocket != nullptr )
    {
        m_pInProcMsgSocket->abort();
    }

    // For sockets created on server's side ..
    if( m_pInProcMsgSocketThread == nullptr )
    {
        // .. no thread has been created and the
        // wrapper took ownership of the socket.
        try
        {
            delete m_pInProcMsgSocket;
        }
        catch(...)
        {
        }
    }
    // For sockets created on client's side ..
    else // if( m_pInProcMsgSocketThread != nullptr )
    {
        // .. a thread has been created hosting the socket and the
        // socket will be destroyed on leaving the thread.
        if( m_pInProcMsgSocketThread->isRunning() )
        {
            m_pInProcMsgSocketThread->quit();
            m_pInProcMsgSocketThread->wait(10000+m_socketDscr.m_iConnectTimeout_ms);
        }
        if( m_pInProcMsgSocketThread->isRunning() )
        {
            m_pInProcMsgSocketThread->deleteLater();
        }
        else
        {
            try
            {
                delete m_pInProcMsgSocketThread;
            }
            catch(...)
            {
            }
        }
    }

    m_pInProcMsgSocketThread = nullptr;
    m_pInProcMsgSocket = nullptr;
    m_pWaitCondSocketCreated = nullptr;
    m_pWaitCondConnected = nullptr;
    m_pWaitCondDisconnected = nullptr;
    m_pWaitCondReadyRead = nullptr;

    if( m_pTrcAdminObj != nullptr )
    {
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;
    }

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
ECopyDepth CInProcMsgSocketWrapper::getMsgReadBuffCopyDepth() const
//------------------------------------------------------------------------------
{
    ECopyDepth copyDepth = m_copyDepthMsgReadBuff;

    if( m_pInProcMsgSocket != nullptr )
    {
        copyDepth = m_pInProcMsgSocket->getMsgReadBuffCopyDepth();
    }
    return copyDepth;

} // getMsgReadBuffCopyDepth

//------------------------------------------------------------------------------
void CInProcMsgSocketWrapper::setMsgReadBuffCopyDepth( ECopyDepth i_copyDepth )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = CEnumCopyDepth::toString(i_copyDepth);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setMsgReadBuffCopyDepth",
        /* strAddInfo         */ strAddTrcInfo );

    m_copyDepthMsgReadBuff = i_copyDepth;

    if( m_pInProcMsgSocket != nullptr )
    {
        m_pInProcMsgSocket->setMsgReadBuffCopyDepth(m_copyDepthMsgReadBuff);
    }

} // setMsgReadBuffCopyDepth

/*==============================================================================
public: // must overridables of base class CIpcSocketWrapper
==============================================================================*/

//------------------------------------------------------------------------------
int CInProcMsgSocketWrapper::messagesAvailable() const
//------------------------------------------------------------------------------
{
    int iMsgCount = 0;

    if( m_pInProcMsgSocket != nullptr )
    {
        iMsgCount = m_pInProcMsgSocket->messagesAvailable();
    }
    return iMsgCount;

} // messagesAvailable

//------------------------------------------------------------------------------
void CInProcMsgSocketWrapper::writeMessage( CMsg* i_pMsg )
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
        /* strMethod          */ "writeMessage",
        /* strAddInfo         */ "" );

    if( m_pInProcMsgSocket != nullptr )
    {
        m_pInProcMsgSocket->writeMessage(i_pMsg);
    }

} // writeMessage

//------------------------------------------------------------------------------
CMsg* CInProcMsgSocketWrapper::readMessage()
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
        /* strMethod          */ "readMessage",
        /* strAddInfo         */ "" );

    CMsg* pMsg = nullptr;

    if( m_pInProcMsgSocket != nullptr )
    {
        pMsg = m_pInProcMsgSocket->readMessage();
    }
    return pMsg;

} // readMessage

/*==============================================================================
public: // must overridables of base class CIpcSocketWrapper
==============================================================================*/

//------------------------------------------------------------------------------
void CInProcMsgSocketWrapper::connectToHost(
    const QString& i_strRemoteHostName,
    unsigned int   i_uRemotePort,
    unsigned int   /*i_uBufferSize*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_strRemoteHostName + ":" + QString::number(i_uRemotePort);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "connectToHost",
        /* strAddInfo         */ strAddTrcInfo );

    if( m_socketDscr.m_srvCltType != ESrvCltTypeClient )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "connectToHost may only be called on client's side" );
    }

    if( m_pInProcMsgSocket == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, "m_pInProcMsgSocket == nullptr" );
    }

    m_pInProcMsgSocket->setLocalHostName(m_socketDscr.m_strLocalHostName);

    m_socketDscr.m_strRemoteHostName = i_strRemoteHostName;
    m_socketDscr.m_uServerListenPort = i_uRemotePort;

    if( m_socketDscr.m_socketState == ESocketStateUnconnected )
    {
        m_socketDscr.m_socketState = ESocketStateConnecting;

        m_pInProcMsgSocket->connectToServer(m_socketDscr.m_strRemoteHostName,m_socketDscr.m_uServerListenPort);
    }

} // connectToHost

//------------------------------------------------------------------------------
bool CInProcMsgSocketWrapper::waitForConnected( int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Timeout: " + QString::number(i_iTimeout_ms);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "waitForConnected",
        /* strAddInfo         */ strAddTrcInfo );

    if( m_socketDscr.m_srvCltType != ESrvCltTypeClient )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "waitForConnected may only be called on client's side" );
    }
    if( m_pInProcMsgSocket->thread() == QThread::currentThread() )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, "waitCondition can only be used if socket uses another thread" );
    }
    if( m_pInProcMsgSocket == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, "m_pInProcMsgSocket == nullptr" );
    }

    bool bConnected = (m_socketDscr.m_socketState == ESocketStateConnected);

    if( m_socketDscr.m_socketState == ESocketStateConnecting && m_pInProcMsgSocket->state() == ESocketStateConnecting )
    {
        int iTimeout_ms = i_iTimeout_ms;

        if( iTimeout_ms <= 0 )
        {
            iTimeout_ms = INT_MAX;
        }

        m_pWaitCondConnected = new QWaitCondition();

        QMutex mtx;
        mtx.lock();
        m_pWaitCondConnected->wait(&mtx,iTimeout_ms);
        mtx.unlock();

        delete m_pWaitCondConnected;
        m_pWaitCondConnected = nullptr;

        if( m_pInProcMsgSocket != nullptr ) // may have been deleted while I was waiting
        {
            if( m_pInProcMsgSocket->state() != ESocketStateConnected )
            {
                m_pInProcMsgSocket->abort(); // emits disconnected signal
            }
            else
            {
                m_socketDscr.m_socketState = ESocketStateConnected;
                m_socketDscr.m_uRemotePort = m_pInProcMsgSocket->getRemotePort();

                emit connected(this);
            }
        }

    } // if( m_socketDscr.m_socketState == ESocketStateConnecting && m_pInProcMsgSocket->state() == ESocketStateConnecting )

    bConnected = (m_socketDscr.m_socketState == ESocketStateConnected);

    return bConnected;

} // waitForConnected

//------------------------------------------------------------------------------
void CInProcMsgSocketWrapper::disconnectFromHost()
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
        /* strMethod          */ "disconnectFromHost",
        /* strAddInfo         */ "" );

    if( m_socketDscr.m_srvCltType != ESrvCltTypeClient )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "disconnectFromHost may only be called on client's side" );
    }

    if( m_socketDscr.m_socketState == ESocketStateConnected )
    {
        if( m_pInProcMsgSocket != nullptr )
        {
            m_socketDscr.m_socketState = ESocketStateClosing;

            m_pInProcMsgSocket->disconnectFromServer();
        }
        else
        {
            m_socketDscr.m_socketState = ESocketStateUnconnected;
        }
    }

} // disconnectFromHost

//------------------------------------------------------------------------------
bool CInProcMsgSocketWrapper::waitForDisconnected( int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Timeout: " + QString::number(i_iTimeout_ms);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "waitForDisconnected",
        /* strAddInfo         */ strAddTrcInfo );

    if( m_socketDscr.m_srvCltType != ESrvCltTypeClient )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "waitForDisconnected may only be called on client's side" );
    }
    if( m_pInProcMsgSocket->thread() == QThread::currentThread() )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, "waitCondition can only be used if socket uses another thread" );
    }
    if( m_pInProcMsgSocket == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, "m_pInProcMsgSocket == nullptr" );
    }

    bool bDisconnected = (m_socketDscr.m_socketState == ESocketStateUnconnected);

    if( m_socketDscr.m_socketState == ESocketStateClosing && m_pInProcMsgSocket->state() != ESocketStateUnconnected )
    {
        int iTimeout_ms = i_iTimeout_ms;

        if( iTimeout_ms <= 0 )
        {
            iTimeout_ms = INT_MAX;
        }

        m_pWaitCondDisconnected = new QWaitCondition();

        QMutex mtx;
        mtx.lock();
        m_pWaitCondDisconnected->wait(&mtx,iTimeout_ms);
        mtx.unlock();

        delete m_pWaitCondDisconnected;
        m_pWaitCondDisconnected = nullptr;

        if( m_pInProcMsgSocket != nullptr ) // may have been deleted while I was waiting
        {
            if( m_pInProcMsgSocket->state() != ESocketStateUnconnected )
            {
                m_pInProcMsgSocket->abort(); // emits disconnected signal
            }
            else
            {
                m_socketDscr.m_socketState = ESocketStateUnconnected;

                emit disconnected(this);
            }
        }

    } // if( m_socketDscr.m_socketState == ESocketStateClosing && m_pInProcMsgSocket->state() != ESocketStateUnconnected )

    bDisconnected = (m_socketDscr.m_socketState == ESocketStateUnconnected);

    return bDisconnected;

} // waitForDisconnected

//------------------------------------------------------------------------------
void CInProcMsgSocketWrapper::abort()
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
        /* strMethod          */ "abort",
        /* strAddInfo         */ "" );

    if( m_pInProcMsgSocket != nullptr )
    {
        switch( m_pInProcMsgSocket->state() )
        {
            case ESocketStateHostLookup:
            case ESocketStateConnecting:
            case ESocketStateListening:
            case ESocketStateClosing:
            {
                m_pInProcMsgSocket->abort();
                break;
            }
            case ESocketStateUnconnected:
            case ESocketStateConnected:
            case ESocketStateBound:
            default:
            {
                break;
            }
        }
    }
    else
    {
        switch( m_socketDscr.m_socketState )
        {
            case ESocketStateUnconnected:
            case ESocketStateHostLookup:
            case ESocketStateConnecting:
            case ESocketStateConnected:
            case ESocketStateBound:
            case ESocketStateListening:
            case ESocketStateClosing:
            default:
            {
                break;
            }
        }
    }

    if( m_pInProcMsgSocketThread != nullptr )
    {
        if( m_pInProcMsgSocketThread->isRunning() )
        {
            m_pInProcMsgSocketThread->quit();
            m_pInProcMsgSocketThread->wait(10000+m_socketDscr.m_iConnectTimeout_ms);
        }
        if( m_pInProcMsgSocketThread->isRunning() )
        {
            m_pInProcMsgSocketThread->deleteLater();
        }
        else
        {
            try
            {
                delete m_pInProcMsgSocketThread;
            }
            catch(...)
            {
            }
        }
    }
    m_pInProcMsgSocketThread = nullptr;
    m_pInProcMsgSocket = nullptr;

    m_bSlotOnConnectedConnected = false;
    m_bSlotOnDisconnectedConnected = false;
    m_bSlotOnErrorConnected = false;
    m_bSlotOnReadyReadConnected = false;

} // abort

////------------------------------------------------------------------------------
//qint64 CInProcMsgSocketWrapper::write( const char* i_pcData, qint64 i_iMaxSize )
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj          */ m_pTrcAdminObj,
//        /* pTrcMthFile        */ m_pTrcMthFile,
//        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
//        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
//        /* strNameSpace       */ nameSpace(),
//        /* strClassName       */ className(),
//        /* strObjName         */ objectName(),
//        /* strMethod          */ "write",
//        /* strAddInfo         */ "" );
//
//    // Use "writeMessage" instead.
//    return 0;
//
//} // write

//------------------------------------------------------------------------------
qint64 CInProcMsgSocketWrapper::write( const QByteArray& /*i_byteArray*/ )
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
        /* strMethod          */ "write",
        /* strAddInfo         */ "" );

    // Use "writeMessage" instead.
    return 0;

} // write

//------------------------------------------------------------------------------
bool CInProcMsgSocketWrapper::waitForReadyRead( int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Timeout: " + QString::number(i_iTimeout_ms);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "waitForReadyRead",
        /* strAddInfo         */ strAddTrcInfo );

    if( m_pInProcMsgSocket->thread() == QThread::currentThread() )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, "waitCondition can only be used if socket uses another thread" );
    }
    if( m_pInProcMsgSocket == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, "m_pInProcMsgSocket == nullptr" );
    }

    bool bReadyRead = false;

    if( m_pInProcMsgSocket != nullptr )
    {
        int iTimeout_ms = i_iTimeout_ms;

        if( iTimeout_ms <= 0 )
        {
            iTimeout_ms = INT_MAX;
        }

        m_pWaitCondReadyRead = new QWaitCondition();

        QMutex mtx;
        mtx.lock();
        bReadyRead = m_pWaitCondReadyRead->wait(&mtx,iTimeout_ms);
        mtx.unlock();

        delete m_pWaitCondReadyRead;
        m_pWaitCondReadyRead = nullptr;

        if( m_pInProcMsgSocket != nullptr ) // may have been deleted while I was waiting
        {
            if( m_pInProcMsgSocket->messagesAvailable() )
            {
                emit CIpcSocketWrapper::readyRead(this);
            }
        }

    } // if( m_pInProcMsgSocket != nullptr )

    return bReadyRead;

} // waitForReadyRead

//------------------------------------------------------------------------------
qint64 CInProcMsgSocketWrapper::bytesAvailable() const
//------------------------------------------------------------------------------
{
    qint64 iBytes = 0;

    if( m_pInProcMsgSocket != nullptr )
    {
        iBytes = m_pInProcMsgSocket->messagesAvailable();
    }
    return iBytes;

} // bytesAvailable

////------------------------------------------------------------------------------
//qint64 CInProcMsgSocketWrapper::read( char* i_pcData, qint64 i_iMaxSize )
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj          */ m_pTrcAdminObj,
//        /* pTrcMthFile        */ m_pTrcMthFile,
//        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
//        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
//        /* strNameSpace       */ nameSpace(),
//        /* strClassName       */ className(),
//        /* strObjName         */ objectName(),
//        /* strMethod          */ "read",
//        /* strAddInfo         */ "" );
//
//    // Use "readMessage" instead.
//    return 0;
//
//} // read

//------------------------------------------------------------------------------
QByteArray CInProcMsgSocketWrapper::read( qint64 /*i_iMaxSize*/ )
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
        /* strMethod          */ "read",
        /* strAddInfo         */ "" );

    // Use "readMessage" instead.
    return QByteArray();

} // read

//------------------------------------------------------------------------------
QByteArray CInProcMsgSocketWrapper::readAll()
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
        /* strMethod          */ "readAll",
        /* strAddInfo         */ "" );

    // Use "readMessage" instead.
    return QByteArray();

} // readAll

//------------------------------------------------------------------------------
ZS::System::EResult CInProcMsgSocketWrapper::error() const
//------------------------------------------------------------------------------
{
    ZS::System::EResult result = EResultSuccess;

    if( m_pInProcMsgSocket != nullptr )
    {
        result = m_pInProcMsgSocket->error();
    }
    return result;

} // error

//------------------------------------------------------------------------------
QString CInProcMsgSocketWrapper::errorString() const
//------------------------------------------------------------------------------
{
    QString strError;

    if( m_pInProcMsgSocket != nullptr )
    {
        strError = m_pInProcMsgSocket->errorString();
    }
    return strError;

} // errorString

//------------------------------------------------------------------------------
SErrResultInfo CInProcMsgSocketWrapper::errResultInfo() const
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo;

    if( m_pInProcMsgSocket != nullptr )
    {
        errResultInfo = m_pInProcMsgSocket->errResultInfo();
    }
    return errResultInfo;

} // errResultInfo

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CInProcMsgSocketWrapper::onSocketCreated( QObject* /*i_pThread*/, QObject* i_pSocket )
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
        /* strMethod          */ "onSocketCreated",
        /* strAddInfo         */ "" );

    m_pInProcMsgSocket = dynamic_cast<CInProcMsgSocket*>(i_pSocket);

    if( m_pWaitCondSocketCreated != nullptr )
    {
        m_pWaitCondSocketCreated->wakeAll();
    }

} // onSocketCreated

//------------------------------------------------------------------------------
void CInProcMsgSocketWrapper::onSocketDestroyed( QObject* /*i_pSocket*/ )
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
        /* strMethod          */ "onSocketDestroyed",
        /* strAddInfo         */ "" );

    m_pInProcMsgSocket = nullptr;

    m_bSlotOnConnectedConnected = false;
    m_bSlotOnDisconnectedConnected = false;
    m_bSlotOnErrorConnected = false;
    m_bSlotOnReadyReadConnected = false;

    m_socketDscr.m_socketState = ESocketStateUnconnected;

    emit CIpcSocketWrapper::disconnected(this);

} // onSocketDestroyed

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CInProcMsgSocketWrapper::onConnected()
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
        /* strMethod          */ "onConnected",
        /* strAddInfo         */ "" );

    if( m_socketDscr.m_socketState != ESocketStateConnected )
    {
        m_socketDscr.m_socketState = ESocketStateConnected;
        m_socketDscr.m_uRemotePort = m_pInProcMsgSocket->getRemotePort();

        if( m_pWaitCondConnected != nullptr )
        {
            // The "connected" signal is emitted in the "waitForConnected" method.
            m_pWaitCondConnected->wakeAll();
        }
        else
        {
            QEvent* pEv = new QEvent( static_cast<QEvent::Type>(EMsgTypeIndConnected) );
            QCoreApplication::postEvent(this,pEv);
            pEv = nullptr;
        }
    }

} // onConnected

//------------------------------------------------------------------------------
void CInProcMsgSocketWrapper::onDisconnected()
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
        /* strMethod          */ "onDisconnected",
        /* strAddInfo         */ "" );

    if( m_socketDscr.m_socketState != ESocketStateUnconnected )
    {
        m_socketDscr.m_socketState = ESocketStateUnconnected;

        if( m_pWaitCondDisconnected != nullptr )
        {
            // The "disconnected" signal is emitted in the "waitForDisconnected" method.
            m_pWaitCondDisconnected->wakeAll();
        }
        else
        {
            QEvent* pEv = new QEvent( static_cast<QEvent::Type>(EMsgTypeIndDisconnected) );
            QCoreApplication::postEvent(this,pEv);
            pEv = nullptr;
        }
    }

} // onDisconnected

//------------------------------------------------------------------------------
void CInProcMsgSocketWrapper::onError( ZS::System::EResult i_result )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = result2Str(i_result);
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

    // Using local stack variable for errResultInfo because of wrong
    // gcc error "invalid initialization of non-const reference ...".
    SErrResultInfo errResultInfoTmp = errResultInfo();
    emit CIpcSocketWrapper::error(this, errResultInfoTmp);

} // onError

//------------------------------------------------------------------------------
void CInProcMsgSocketWrapper::onReadyRead()
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

    if( m_pWaitCondReadyRead != nullptr )
    {
        // The "readyRead" signal is emitted in the "waitForReadyRead" method.
        m_pWaitCondReadyRead->wakeAll();
    }
    else
    {
        QEvent* pEv = new QEvent( static_cast<QEvent::Type>(EMsgTypeIndReceivedData) );
        QCoreApplication::postEvent(this,pEv);
        pEv = nullptr;
    }

} // onReadyRead

/*==============================================================================
protected: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CInProcMsgSocketWrapper::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    if( i_pEv != nullptr )
    {
        CMethodTracer mthTracer(
            /* pAdminObj          */ m_pTrcAdminObj,
            /* pTrcMthFile        */ m_pTrcMthFile,
            /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ nameSpace(),
            /* strClassName       */ className(),
            /* strObjName         */ objectName(),
            /* strMethod          */ "event",
            /* strAddInfo         */ "" );

        switch( static_cast<int>(i_pEv->type()) )
        {
            case EMsgTypeIndConnected:
            {
                emit CIpcSocketWrapper::connected(this);
                bEventHandled = true;
                break;
            }
            case EMsgTypeIndDisconnected:
            {
                emit CIpcSocketWrapper::disconnected(this);
                bEventHandled = true;
                break;
            }
            case EMsgTypeIndReceivedData:
            {
                emit CIpcSocketWrapper::readyRead(this);
                bEventHandled = true;
                break;
            }
            default:
            {
                break;
            }
        }
    }

    return bEventHandled;

} // event
