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

#ifndef ZSIpc_SocketWrapperTcp_h
#define ZSIpc_SocketWrapperTcp_h

#include <QtNetwork/qtcpsocket.h>
#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcSocketWrapper.h"

namespace ZS
{
namespace Ipc
{
//******************************************************************************
class ZSIPCDLL_API CTcpSocketWrapper : public CIpcSocketWrapper
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CTcpSocketWrapper"; }
public: // class methods
    static ZS::System::EResult TcpSocketError2Result( QAbstractSocket::SocketError i_socketError );
    static ESocketState TcpSocketState2SocketState( QAbstractSocket::SocketState i_socketState );
public: // ctors and dtor
    CTcpSocketWrapper(
        const QString&      i_strObjName,
        ESrvCltType         i_srvCltType,
        int                 i_iSocketId = ZS::Ipc::ESocketIdUndefined,
        QTcpSocket*         i_pTcpSocket = nullptr,
        Trace::CTrcMthFile* i_pTrcMthFile = nullptr,         // If != nullptr trace method file with detail level is used instead of trace admin object with Trace server.
        ZS::Trace::ETraceDetailLevelMethodCalls i_eTrcMthFileDetailLevel = Trace::ETraceDetailLevelMethodCalls::None );
    CTcpSocketWrapper(
        const QString&      i_strObjName,
        const SSocketDscr&  i_socketDscr,
        QTcpSocket*         i_pTcpSocket,
        Trace::CTrcMthFile* i_pTrcMthFile = nullptr,         // If != nullptr trace method file with detail level is used instead of trace admin object with Trace server.
        ZS::Trace::ETraceDetailLevelMethodCalls i_eTrcMthFileDetailLevel = Trace::ETraceDetailLevelMethodCalls::None );
    virtual ~CTcpSocketWrapper();
public: // overridables of base class CIpcSocketWrapper
    virtual QString nameSpace() const override { return NameSpace(); }
    virtual QString className() const override { return ClassName(); }
public: // must overridables of base class CIpcSocketWrapper
    virtual void connectToHost( const QString& i_strRemoteHostName, unsigned int i_uRemotePort, unsigned int i_uBufferSize = 0 ) override;
    virtual bool waitForConnected( int i_iTimeout_ms = 30000 ) override;
    virtual void disconnectFromHost() override;
    virtual bool waitForDisconnected( int i_iTimeout_ms = 30000 ) override;
    virtual void abort() override;
    //virtual qint64 write( const char* i_pcData, qint64 i_iMaxSize );
    virtual qint64 write( const QByteArray& i_byteArray ) override;
    virtual bool waitForReadyRead( int i_iTimeout_ms = 30000 ) override;
    virtual qint64 bytesAvailable() const override;
    //virtual qint64 read( char* i_pcData, qint64 i_iMaxSize );
    virtual QByteArray read( qint64 i_iMaxSize ) override;
    virtual QByteArray readAll() override;
    virtual ZS::System::EResult error() const override;
    virtual QString errorString() const override;
    virtual ZS::System::SErrResultInfo errResultInfo() const override;
protected slots: // overridables
    virtual void onConnected();
    virtual void onDisconnected();
    virtual void onError( QAbstractSocket::SocketError i_socketError );
    virtual void onStateChanged( QAbstractSocket::SocketState i_socketState );
    virtual void onAboutToClose();
    virtual void onBytesWritten( qint64 bytes );
    virtual void onReadyRead();
protected: // instance methods
    QTcpSocket* m_pTcpSocket;

}; // class CTcpSocketWrapper

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_SocketWrapperTcp_h
