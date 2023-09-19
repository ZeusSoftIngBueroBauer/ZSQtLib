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

#include <QtNetwork/qhostinfo.h>
#include <QtNetwork/qtcpsocket.h>

#include "ZSIpc/ZSIpcSocketWrapperTcp.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;


/*******************************************************************************
class CTcpSocketWrapper : public CIpcSocketWrapper
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

struct STcpSocketIntVal2WrapperIntVal
{
    int m_iTcpSocketVal;
    int m_iWrapperVal;
};

/* Converting tcp socket errors into the error definitions of the wrapper
==============================================================================*/

//------------------------------------------------------------------------------
const STcpSocketIntVal2WrapperIntVal s_arTcpSocketError2WrapperError[] =
//------------------------------------------------------------------------------
{
    /*  0 */ { QAbstractSocket::ConnectionRefusedError,           EResultConnectionRefused },
    /*  1 */ { QAbstractSocket::RemoteHostClosedError,            EResultRemoteHostClosed },
    /*  2 */ { QAbstractSocket::HostNotFoundError,                EResultHostNotFound },
    /*  3 */ { QAbstractSocket::SocketAccessError,                EResultSocketAccessError },
    /*  4 */ { QAbstractSocket::SocketResourceError,              EResultSocketResourceError },
    /*  5 */ { QAbstractSocket::SocketTimeoutError,               EResultSocketTimeout },
    /*  6 */ { QAbstractSocket::DatagramTooLargeError,            EResultDatagramTooLarge },
    /*  7 */ { QAbstractSocket::NetworkError,                     EResultNetworkError },
    /*  8 */ { QAbstractSocket::AddressInUseError,                EResultAddressAlreadyInUse },
    /*  9 */ { QAbstractSocket::SocketAddressNotAvailableError,   EResultSocketAddressNotAvailable },
    /* 10 */ { QAbstractSocket::UnsupportedSocketOperationError,  EResultUnsupportedSocketOperation },
    #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
    /* 11 */ { QAbstractSocket::UnfinishedSocketOperationError,   EResultUnfinishedSocketOperation },
    /* 12 */ { QAbstractSocket::ProxyAuthenticationRequiredError, EResultProxyAuthenticationRequired },
    /* 13 */ { QAbstractSocket::SslHandshakeFailedError,          EResultSslHandshakeFailed },
    /* 14 */ { QAbstractSocket::ProxyConnectionRefusedError,      EResultProxyConnectionRefused },
    /* 15 */ { QAbstractSocket::ProxyConnectionClosedError,       EResultProxyConnectionClosed },
    /* 16 */ { QAbstractSocket::ProxyConnectionTimeoutError,      EResultProxyTimeout },
    /* 17 */ { QAbstractSocket::ProxyNotFoundError,               EResultProxyNotFound },
    /* 18 */ { QAbstractSocket::ProxyProtocolError,               EResultProxyProtocol },
    #endif
    /* 19 */ { QAbstractSocket::UnknownSocketError,               EResultUndefined }
};

//------------------------------------------------------------------------------
ZS::System::EResult CTcpSocketWrapper::TcpSocketError2Result( QAbstractSocket::SocketError i_socketError )
//------------------------------------------------------------------------------
{
    ZS::System::EResult result = EResultUndefined;

    if( i_socketError >= 0 && i_socketError < static_cast<int>(_ZSArrLen(s_arTcpSocketError2WrapperError)) )
    {
        if( i_socketError == s_arTcpSocketError2WrapperError[i_socketError].m_iTcpSocketVal )
        {
            result = static_cast<EResult>(s_arTcpSocketError2WrapperError[i_socketError].m_iWrapperVal);
        }
        else
        {
            for( int idx = 0; idx < static_cast<int>(_ZSArrLen(s_arTcpSocketError2WrapperError)); idx++ )
            {
                if( i_socketError == s_arTcpSocketError2WrapperError[idx].m_iTcpSocketVal )
                {
                    result = static_cast<EResult>(s_arTcpSocketError2WrapperError[idx].m_iWrapperVal);
                    break;
                }
            }
        }
    }
    return result;

} // TcpSocketError2Result

/* Converting tcp socket states into the socket state definitions of the wrapper
==============================================================================*/

//------------------------------------------------------------------------------
const STcpSocketIntVal2WrapperIntVal s_arTcpSocketState2WrapperState[] =
//------------------------------------------------------------------------------
{
    { QAbstractSocket::UnconnectedState, ESocketStateUnconnected },
    { QAbstractSocket::HostLookupState,  ESocketStateHostLookup },
    { QAbstractSocket::ConnectingState,  ESocketStateConnecting },
    { QAbstractSocket::ConnectedState,   ESocketStateConnected },
    { QAbstractSocket::BoundState,       ESocketStateBound },
    { QAbstractSocket::ListeningState,   ESocketStateListening },
    { QAbstractSocket::ClosingState,     ESocketStateClosing }
};

//------------------------------------------------------------------------------
ESocketState CTcpSocketWrapper::TcpSocketState2SocketState( QAbstractSocket::SocketState i_socketState )
//------------------------------------------------------------------------------
{
    ESocketState socketState = ESocketStateUndefined;

    if( i_socketState >= 0 && i_socketState < static_cast<int>(_ZSArrLen(s_arTcpSocketState2WrapperState)) )
    {
        if( i_socketState == s_arTcpSocketState2WrapperState[i_socketState].m_iTcpSocketVal )
        {
            socketState = static_cast<ESocketState>(s_arTcpSocketState2WrapperState[i_socketState].m_iWrapperVal);
        }
        else
        {
            for( int idx = 0; idx < static_cast<int>(_ZSArrLen(s_arTcpSocketState2WrapperState)); idx++ )
            {
                if( i_socketState == s_arTcpSocketState2WrapperState[i_socketState].m_iTcpSocketVal )
                {
                    socketState = static_cast<ESocketState>(s_arTcpSocketState2WrapperState[i_socketState].m_iWrapperVal);
                    break;
                }
            }
        }
    }
    return socketState;

} // TcpSocketState2SocketState

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTcpSocketWrapper::CTcpSocketWrapper(
    const QString& i_strObjName,
    ESrvCltType    i_srvCltType,
    int            i_iSocketId,
    QTcpSocket*    i_pTcpSocket,
    CTrcMthFile*   i_pTrcMthFile,
    EMethodTraceDetailLevel i_eTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    CIpcSocketWrapper(
        /* strObjName             */ i_strObjName,
        /* srvCltType             */ i_srvCltType,
        /* socketType             */ ESocketTypeTcp,
        /* iSocketId              */ i_iSocketId,
        /* pSocket                */ i_pTcpSocket,
        /* pTrcMthFile            */ i_pTrcMthFile,
        /* iTrcMthFileDetailLevel */ i_eTrcMthFileDetailLevel ),
    m_pTcpSocket(i_pTcpSocket)
{
    // The derived classes must instantiate the trace admin object and trace the ctor.
    if( m_pTrcMthFile == nullptr && !i_strObjName.contains("TrcServer") && !i_strObjName.contains("TrcClient") )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strAddInfo         */ "" );

    if( m_pTcpSocket == nullptr )
    {
        m_pSocket = m_pTcpSocket = new QTcpSocket(this);
    }

    m_socketDscr.m_socketState = TcpSocketState2SocketState( m_pTcpSocket->state() );
    m_socketDscr.m_strLocalHostName = QHostInfo::localHostName();
    m_socketDscr.m_hostAddrLocal = m_pTcpSocket->localAddress();
    m_socketDscr.m_uLocalPort = m_pTcpSocket->localPort();
    m_socketDscr.m_strRemoteHostName = m_pTcpSocket->peerName();
    m_socketDscr.m_hostAddrRemote = m_pTcpSocket->peerAddress();
    m_socketDscr.m_uRemotePort = m_pTcpSocket->peerPort();

    if( !QObject::connect(
        /* pObjSender   */ m_pTcpSocket,
        /* szSignal     */ SIGNAL(connected()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onConnected()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTcpSocket,
        /* szSignal     */ SIGNAL(disconnected()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDisconnected()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTcpSocket,
        /* szSignal     */ SIGNAL(error(QAbstractSocket::SocketError)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onError(QAbstractSocket::SocketError)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTcpSocket,
        /* szSignal     */ SIGNAL(stateChanged(QAbstractSocket::SocketState)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onStateChanged(QAbstractSocket::SocketState)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTcpSocket,
        /* szSignal     */ SIGNAL(aboutToClose()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onAboutToClose()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTcpSocket,
        /* szSignal     */ SIGNAL(bytesWritten(qint64)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBytesWritten(qint64)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTcpSocket,
        /* szSignal     */ SIGNAL(readyRead()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onReadyRead()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CTcpSocketWrapper::CTcpSocketWrapper(
    const QString&     i_strObjName,
    const SSocketDscr& i_socketDscr,
    QTcpSocket*        i_pTcpSocket,
    CTrcMthFile*       i_pTrcMthFile,
    EMethodTraceDetailLevel i_eTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    CIpcSocketWrapper(
        /* strObjName             */ i_strObjName,
        /* socketDscr             */ i_socketDscr,
        /* pSocket                */ i_pTcpSocket,
        /* pTrcMthFile            */ i_pTrcMthFile,
        /* iTrcMthFileDetailLevel */ i_eTrcMthFileDetailLevel ),
    m_pTcpSocket(i_pTcpSocket)
{
    // The derived classes must instantiate the trace admin object and trace the ctor.
    if( m_pTrcMthFile == nullptr && !i_strObjName.contains("TrcServer") && !i_strObjName.contains("TrcClient") )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strAddInfo         */ "" );

    if( m_pTcpSocket == nullptr )
    {
        m_pSocket = m_pTcpSocket = new QTcpSocket(this);
    }

    m_socketDscr.m_socketState = TcpSocketState2SocketState( m_pTcpSocket->state() );

    if( m_socketDscr.m_strLocalHostName.isEmpty() )
    {
        m_socketDscr.m_strLocalHostName = "127.0.0.1";
    }

    m_socketDscr.m_hostAddrLocal = m_pTcpSocket->localAddress();

    if( !QObject::connect(
        /* pObjSender   */ m_pTcpSocket,
        /* szSignal     */ SIGNAL(connected()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onConnected()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTcpSocket,
        /* szSignal     */ SIGNAL(disconnected()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDisconnected()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTcpSocket,
        /* szSignal     */ SIGNAL(error(QAbstractSocket::SocketError)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onError(QAbstractSocket::SocketError)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTcpSocket,
        /* szSignal     */ SIGNAL(stateChanged(QAbstractSocket::SocketState)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onStateChanged(QAbstractSocket::SocketState)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTcpSocket,
        /* szSignal     */ SIGNAL(aboutToClose()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onAboutToClose()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTcpSocket,
        /* szSignal     */ SIGNAL(bytesWritten(qint64)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBytesWritten(qint64)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTcpSocket,
        /* szSignal     */ SIGNAL(readyRead()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onReadyRead()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CTcpSocketWrapper::~CTcpSocketWrapper()
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
        if( m_pTcpSocket != nullptr )
        {
            QObject::disconnect(
                /* pObjSender   */ m_pTcpSocket,
                /* szSignal     */ SIGNAL(connected()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onConnected()) );
            QObject::disconnect(
                /* pObjSender   */ m_pTcpSocket,
                /* szSignal     */ SIGNAL(disconnected()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDisconnected()) );
            QObject::disconnect(
                /* pObjSender   */ m_pTcpSocket,
                /* szSignal     */ SIGNAL(error(QAbstractSocket::SocketError)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onError(QAbstractSocket::SocketError)) );
            QObject::disconnect(
                /* pObjSender   */ m_pTcpSocket,
                /* szSignal     */ SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onStateChanged(QAbstractSocket::SocketState)) );
            QObject::disconnect(
                /* pObjSender   */ m_pTcpSocket,
                /* szSignal     */ SIGNAL(aboutToClose()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onAboutToClose()) );
            QObject::disconnect(
                /* pObjSender   */ m_pTcpSocket,
                /* szSignal     */ SIGNAL(bytesWritten(qint64)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onBytesWritten(qint64)) );
            QObject::disconnect(
                /* pObjSender   */ m_pTcpSocket,
                /* szSignal     */ SIGNAL(readyRead()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onReadyRead()) );

            m_pTcpSocket->abort();
            m_pTcpSocket->deleteLater();
        }
    }
    catch(...)
    {
    }
    m_pTcpSocket = nullptr;

    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();

        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;
    }

} // dtor

/*==============================================================================
public: // must overridables of base class CIpcSocketWrapper
==============================================================================*/

//------------------------------------------------------------------------------
void CTcpSocketWrapper::connectToHost(
    const QString& i_strRemoteHostName,
    unsigned int   i_uRemotePort,
    unsigned int   /*i_uBufferSize*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = i_strRemoteHostName + ":" + QString::number(i_uRemotePort);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "connectToHost",
        /* strAddInfo         */ strAddTrcInfo );

    m_socketDscr.m_strRemoteHostName = i_strRemoteHostName;
    m_socketDscr.m_uServerListenPort = i_uRemotePort;

    m_pTcpSocket->connectToHost( m_socketDscr.m_strRemoteHostName, m_socketDscr.m_uServerListenPort );

} // connectToHost

//------------------------------------------------------------------------------
bool CTcpSocketWrapper::waitForConnected( int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "Timeout: " + QString::number(i_iTimeout_ms);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "waitForConnected",
        /* strAddInfo         */ strAddTrcInfo );

    return m_pTcpSocket->waitForConnected(i_iTimeout_ms);

} // waitForConnected

//------------------------------------------------------------------------------
void CTcpSocketWrapper::disconnectFromHost()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "disconnectFromHost",
        /* strAddInfo   */ "" );

    return m_pTcpSocket->disconnectFromHost();

} // disconnectFromHost

//------------------------------------------------------------------------------
bool CTcpSocketWrapper::waitForDisconnected( int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "Timeout: " + QString::number(i_iTimeout_ms);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "waitForDisconnected",
        /* strAddInfo         */ strAddTrcInfo );

    //m_bWaitForDisconnected = true;
    bool bDisconnected = m_pTcpSocket->waitForDisconnected(i_iTimeout_ms);
    //m_bWaitForDisconnected = false;
    return bDisconnected;

} // waitForDisconnected

//------------------------------------------------------------------------------
void CTcpSocketWrapper::abort()
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
        /* strMethod          */ "abort",
        /* strAddInfo         */ "" );

    return m_pTcpSocket->abort();

} // abort

////------------------------------------------------------------------------------
//qint64 CTcpSocketWrapper::write( const char* i_pcData, qint64 i_iMaxSize )
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj          */ m_pTrcAdminObj,
//        /* pTrcMthFile        */ m_pTrcMthFile,
//        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
//        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strNameSpace       */ NameSpace(),
//        /* strClassName       */ ClassName(),
//        /* strObjName         */ objectName(),
//        /* strMethod          */ "write",
//        /* strAddInfo         */ "" );
//
//    return m_pTcpSocket->write(i_pcData,i_iMaxSize);
//
//} // write

//------------------------------------------------------------------------------
qint64 CTcpSocketWrapper::write( const QByteArray& i_byteArray )
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
        /* strMethod          */ "write",
        /* strAddInfo         */ "" );

    return m_pTcpSocket->write(i_byteArray);

} // write

//------------------------------------------------------------------------------
bool CTcpSocketWrapper::waitForReadyRead( int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "Timeout: " + QString::number(i_iTimeout_ms);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "waitForReadyRead",
        /* strAddInfo         */ strAddTrcInfo );

    return m_pTcpSocket->waitForReadyRead(i_iTimeout_ms);

} // waitForReadyRead

//------------------------------------------------------------------------------
qint64 CTcpSocketWrapper::bytesAvailable() const
//------------------------------------------------------------------------------
{
    return m_pTcpSocket->bytesAvailable();
}

////------------------------------------------------------------------------------
//qint64 CTcpSocketWrapper::read( char* i_pcData, qint64 i_iMaxSize )
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj          */ m_pTrcAdminObj,
//        /* pTrcMthFile        */ m_pTrcMthFile,
//        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
//        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strNameSpace       */ NameSpace(),
//        /* strClassName       */ ClassName(),
//        /* strObjName         */ objectName(),
//        /* strMethod          */ "read",
//        /* strAddInfo         */ "" );
//
//    return m_pTcpSocket->read(i_pcData,i_iMaxSize);
//
//} // read

//------------------------------------------------------------------------------
QByteArray CTcpSocketWrapper::read( qint64 i_iMaxSize )
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
        /* strMethod          */ "read",
        /* strAddInfo         */ "" );

    return m_pTcpSocket->read(i_iMaxSize);

} // read

//------------------------------------------------------------------------------
QByteArray CTcpSocketWrapper::readAll()
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
        /* strMethod          */ "readAll",
        /* strAddInfo         */ "" );

    return m_pTcpSocket->readAll();

} // readAll

//------------------------------------------------------------------------------
ZS::System::EResult CTcpSocketWrapper::error() const
//------------------------------------------------------------------------------
{
    return TcpSocketError2Result(m_pTcpSocket->error());

} // error

//------------------------------------------------------------------------------
QString CTcpSocketWrapper::errorString() const
//------------------------------------------------------------------------------
{
    return m_pTcpSocket->errorString();

} // errorString

//------------------------------------------------------------------------------
SErrResultInfo CTcpSocketWrapper::errResultInfo() const
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo;

    errResultInfo.setSeverity(EResultSeverityInfo);
    errResultInfo.setResult(TcpSocketError2Result(m_pTcpSocket->error()));
    errResultInfo.setAddErrInfoDscr(m_pTcpSocket->errorString());

    switch( m_pTcpSocket->error() )
    {
        case QAbstractSocket::RemoteHostClosedError:
        case QAbstractSocket::HostNotFoundError:
        case QAbstractSocket::ConnectionRefusedError:
        case QAbstractSocket::AddressInUseError:
        case QAbstractSocket::SocketTimeoutError:
        {
            errResultInfo.setSeverity(EResultSeverityError);
            break;
        }
        case QAbstractSocket::SocketAccessError:
        case QAbstractSocket::SocketResourceError:
        case QAbstractSocket::DatagramTooLargeError:
        case QAbstractSocket::NetworkError:
        case QAbstractSocket::SocketAddressNotAvailableError:
        case QAbstractSocket::UnsupportedSocketOperationError:
        {
            errResultInfo.setSeverity(EResultSeverityError);
            break;
        }
        #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
        case QAbstractSocket::UnfinishedSocketOperationError:
        case QAbstractSocket::ProxyAuthenticationRequiredError:
        case QAbstractSocket::SslHandshakeFailedError:
        case QAbstractSocket::ProxyConnectionRefusedError:
        case QAbstractSocket::ProxyConnectionClosedError:
        case QAbstractSocket::ProxyConnectionTimeoutError:
        case QAbstractSocket::ProxyNotFoundError:
        case QAbstractSocket::ProxyProtocolError:
        #endif
        case QAbstractSocket::UnknownSocketError:
        default:
        {
            errResultInfo.setSeverity(EResultSeverityError);
            break;
        }
    }
    return errResultInfo;

} // errResultInfo

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTcpSocketWrapper::onConnected()
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
        /* strMethod          */ "onConnected",
        /* strAddInfo         */ "" );

    m_socketDscr.m_socketState = ESocketStateConnected;
    m_socketDscr.m_hostAddrLocal = m_pTcpSocket->localAddress();
    m_socketDscr.m_uLocalPort = m_pTcpSocket->localPort();
    m_socketDscr.m_strRemoteHostName = m_pTcpSocket->peerName();
    m_socketDscr.m_hostAddrRemote = m_pTcpSocket->peerAddress();
    m_socketDscr.m_uRemotePort = m_pTcpSocket->peerPort();

    emit CIpcSocketWrapper::connected(this);

} // onConnected

//------------------------------------------------------------------------------
void CTcpSocketWrapper::onDisconnected()
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
        /* strMethod          */ "onDisconnected",
        /* strAddInfo         */ "" );

    m_socketDscr.m_socketState = ESocketStateUnconnected;

    emit CIpcSocketWrapper::disconnected(this);

} // onDisconnected

//------------------------------------------------------------------------------
void CTcpSocketWrapper::onError( QAbstractSocket::SocketError i_socketError )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = result2Str( TcpSocketError2Result(i_socketError) );
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onError",
        /* strAddInfo         */ strAddTrcInfo );

    m_socketDscr.m_socketState = ESocketStateUnconnected;

    // Using local stack variable for errResultInfo because of wrong
    // gcc error "invalid initialization of non-const reference ...".
    SErrResultInfo errResultInfoTmp = errResultInfo();
    emit CIpcSocketWrapper::error(this, errResultInfoTmp);

} // onError

//------------------------------------------------------------------------------
void CTcpSocketWrapper::onStateChanged( QAbstractSocket::SocketState i_socketState )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = socketState2Str( TcpSocketState2SocketState(i_socketState) );
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onStateChanged",
        /* strAddInfo         */ strAddTrcInfo );

    m_socketDscr.m_socketState = TcpSocketState2SocketState(i_socketState);

    emit CIpcSocketWrapper::stateChanged( this, m_socketDscr.m_socketState );

} // onStateChanged

//------------------------------------------------------------------------------
void CTcpSocketWrapper::onAboutToClose()
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
        /* strMethod          */ "onAboutToClose",
        /* strAddInfo         */ "" );

    emit CIpcSocketWrapper::aboutToClose(this);

} // onAboutToClose

//------------------------------------------------------------------------------
void CTcpSocketWrapper::onBytesWritten( qint64 i_iBytes )
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
        /* strMethod          */ "onBytesWritten",
        /* strAddInfo         */ "" );

    emit CIpcSocketWrapper::bytesWritten( this, i_iBytes );

} // onBytesWritten

//------------------------------------------------------------------------------
void CTcpSocketWrapper::onReadyRead()
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
        /* strMethod          */ "onReadyRead",
        /* strAddInfo         */ "" );

    emit CIpcSocketWrapper::readyRead(this);

} // onReadyRead
