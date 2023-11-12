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

#ifndef ZSIpc_SocketWrapper_h
#define ZSIpc_SocketWrapper_h

#include <QtCore/qobject.h>

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcCommon.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"

namespace ZS
{
namespace System
{
class CTrcAdminObj;
class CTrcMthFile;
}

namespace Ipc
{
//******************************************************************************
class ZSIPCDLL_API CIpcSocketWrapper : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CIpcSocketWrapper"; }
protected: // ctor
    CIpcSocketWrapper(
        const QString&      i_strObjName,
        ESrvCltType         i_srvCltType,
        ESocketType         i_socketType,
        int                 i_iSocketId = ZS::Ipc::ESocketIdUndefined,
        QObject*            i_pSocket = nullptr,
        ZS::System::CTrcMthFile* i_pTrcMthFile = nullptr,         // If != nullptr trace method file with detail level is used instead of trace admin object with Trace server.
        ZS::System::EMethodTraceDetailLevel i_eTrcMthFileDetailLevel = ZS::System::EMethodTraceDetailLevel::None );
    CIpcSocketWrapper(
        const QString&      i_strObjName,
        const SSocketDscr&  i_socketDscr,
        QObject*            i_pSocket = nullptr,
        ZS::System::CTrcMthFile* i_pTrcMthFile = nullptr,         // If != nullptr trace method file with detail level is used instead of trace admin object with Trace server.
        ZS::System::EMethodTraceDetailLevel i_eTrcMthFileDetailLevel = ZS::System::EMethodTraceDetailLevel::None );
public: // dtor
    virtual ~CIpcSocketWrapper();
signals:
    void connected( QObject* i_pSocketWrapper );
    void disconnected( QObject* i_pSocketWrapper );
    void error( QObject* i_pSocketWrapper, const ZS::System::SErrResultInfo& i_errResultInfo );
    void stateChanged( QObject* i_pSocketWrapper, int i_iState );
    void aboutToClose( QObject* i_pSocketWrapper );
    void bytesWritten( QObject* i_pSocketWrapper, qint64 i_iBytes );
    void readyRead( QObject* i_pSocketWrapper );
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    SSocketDscr getSocketDscr() const { return m_socketDscr; }
    ESrvCltType srvCltType() const { return m_socketDscr.m_srvCltType; }
    ESocketType socketType() const { return m_socketDscr.m_socketType; }
    void setSocketId( int i_iId ); // call it only once right after creating an instance of this class
    int getSocketId() const { return m_socketDscr.m_iSocketId; }
    ESocketState socketState() const { return m_socketDscr.m_socketState; }
public: // overridables
    virtual void setServerListenPort( unsigned int i_uPort );
    virtual unsigned int getServerListenPort() const { return m_socketDscr.m_uServerListenPort; }
public: // overridables
    virtual void setLocalHostName( const QString& i_strName );
    virtual QString getLocalHostName() const { return m_socketDscr.m_strLocalHostName; }
    virtual void setLocalHostAddress( const QHostAddress& i_hostAddr );
    virtual QHostAddress getLocalHostAddress() const { return m_socketDscr.m_hostAddrLocal; }
    virtual void setLocalPort( unsigned int i_uPort );
    virtual unsigned int getLocalPort() const { return m_socketDscr.m_uLocalPort; }
public: // overridables
    virtual void setRemoteHostName( const QString& i_strName );
    virtual QString getRemoteHostName() const { return m_socketDscr.m_strRemoteHostName; }
    virtual void setRemoteHostAddress( const QHostAddress& i_hostAddr );
    virtual QHostAddress getRemoteHostAddress() const { return m_socketDscr.m_hostAddrRemote; }
    virtual void setRemotePort( unsigned int i_uPort );
    virtual unsigned int getRemotePort() const { return m_socketDscr.m_uRemotePort; }
public: // overridables
    virtual void setBufferSize( unsigned int i_uBufferSize );
    virtual unsigned int getBufferSize() const { return m_socketDscr.m_uBufferSize; }
    virtual double getLastTimeDataBlockReceivedInMs() const { return m_fLastTimeDataBlockReceived_ms; }
    virtual void setLastTimeDataBlockReceivedInMs( double i_fTime_ms );
public: // must overridables
    virtual void connectToHost( const QString& i_strRemoteHostName, unsigned int i_uRemotePort, unsigned int i_uBufferSize = 0 ) = 0;
    virtual bool waitForConnected( int i_iTimeout_ms = 30000 ) = 0;
    virtual void disconnectFromHost() = 0;
    virtual bool waitForDisconnected( int i_iTimeout_ms = 30000 ) = 0;
    virtual void abort() = 0;
    //virtual qint64 write( const char* i_pcData, qint64 i_iMaxSize ) = 0;
    virtual qint64 write( const QByteArray& i_byteArray ) = 0;
    virtual bool waitForReadyRead( int i_iTimeout_ms = 30000 ) = 0;
    virtual qint64 bytesAvailable() const = 0;
    //virtual qint64 read( char* i_pcData, qint64 i_iMaxSize ) = 0;
    virtual QByteArray read( qint64 i_iMaxSize ) = 0;
    virtual QByteArray readAll() = 0;
    virtual ZS::System::EResult error() const = 0;
    virtual QString errorString() const = 0;
    virtual ZS::System::SErrResultInfo errResultInfo() const = 0;
protected: // instance methods
    bool areTraceMethodCallsActive( ZS::System::EMethodTraceDetailLevel i_eFilterDetailLevel ) const;
    ZS::System::EMethodTraceDetailLevel getMethodCallsTraceDetailLevel() const;
    bool isTraceRuntimeInfoActive( ZS::System::ELogDetailLevel i_eFilterDetailLevel ) const;
    ZS::System::ELogDetailLevel getRuntimeInfoTraceDetailLevel() const;
protected: // instance members
    SSocketDscr          m_socketDscr;
    double               m_fLastTimeDataBlockReceived_ms; // must be set by the gateway on receiving a complete data block
    QObject*             m_pSocket;
    ZS::System::EMethodTraceDetailLevel m_eTrcMthFileDetailLevel;
    ZS::System::CTrcMthFile*  m_pTrcMthFile;   // Either trace method file with detail level is used or
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;  // trace admin object with IpcTrace server.

}; // class CIpcSocketWrapper

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_SocketWrapper_h
