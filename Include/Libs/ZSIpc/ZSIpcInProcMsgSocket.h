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

#ifndef ZSIpc_InProcMsgSocket_h
#define ZSIpc_InProcMsgSocket_h

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcCommon.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"

class QRecursiveMutex;
class QTimer;

namespace ZS
{
namespace System
{
class CMsg;
class CTrcAdminObj;
}

namespace Ipc
{
class CInProcMsgServer;
class CInProcMsgSocket;

//******************************************************************************
class ZSIPCDLL_API CInProcMsgSocketsAdminObj : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static CInProcMsgSocketsAdminObj* GetInstance(); // singleton class
public: // ctors and dtor
    CInProcMsgSocketsAdminObj( bool i_bTracingEnabled = true );
    virtual ~CInProcMsgSocketsAdminObj();
public: // instance methods
    CInProcMsgSocket* find( unsigned int i_uPort ) const;
    QList<unsigned int> getUsedPorts() const;
    unsigned int GetUniquePort() const;
public: // instance methods
    void onSocketCreated( CInProcMsgSocket* i_pSocket );
    void onSocketDestroyed( CInProcMsgSocket* i_pSocket );
protected: // instance members
    QMutex*                              m_pMutex;
    QMap<unsigned int,CInProcMsgSocket*> m_mapSockets; // Map with created sockets (the key corresponds to the port)
    bool                                 m_bDestroying;
    bool                                 m_bTracingEnabled;
    ZS::System::CTrcAdminObj*            m_pTrcAdminObj;

}; // class CInProcMsgSocketsAdminObj

//******************************************************************************
class ZSIPCDLL_API CInProcMsgSocket : public QObject
//******************************************************************************
{
friend class CInProcMsgServer;
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CInProcMsgSocket"; }
public: // class methods
    static CInProcMsgSocketsAdminObj* GetAdminObjInstance();
public: // class methods
    static CInProcMsgSocket* Find( unsigned int i_uPort );
    static QList<unsigned int> GetUsedPorts();
    static unsigned int GetUniquePort();
public: // ctors and dtor
    CInProcMsgSocket(
        const QString& i_strObjName,
        ESrvCltType    i_srvCltType,
        bool           i_bTracingEnabled = true );
    CInProcMsgSocket( // ctor only used on server's side
        const QString&    i_strObjName,
        CInProcMsgServer* i_pServer,
        CInProcMsgSocket* i_pSocketClient,
        bool              i_bTracingEnabled = true );
    virtual ~CInProcMsgSocket();
signals: // AbstractSocket
    void connected();
    void disconnected();
    //void error( QAbstractSocket::SocketError i_socketError );
    void error( ZS::System::EResult i_result );
    //void hostFound();
    //void proxyAuthenticationRequired( const QNetworkProxy& i_proxy, QAuthenticator* i_pAuthenticator );
    //void stateChanged( QAbstractSocket::SocketState socketState );
signals: // IODevice
    //void aboutToClose();
    //void bytesWritten( qint64 i_bytes );
    //void readChannelFinished();
    void readyRead();
public: // instance methods
    ZS::System::ECopyDepth getMsgReadBuffCopyDepth() const;
    void setMsgReadBuffCopyDepth( ZS::System::ECopyDepth i_copyDepth );
public: // instance methods
    ESrvCltType getSrvCltType() const;
public: // instance methods
    virtual unsigned int getServerListenPort() const;
public: // instance methods
    virtual void setLocalHostName( const QString& i_strLocalHostName );
    virtual QString getLocalHostName() const;
    virtual unsigned int getLocalPort() const;
public: // instance methods
    virtual QString getRemoteHostName() const;
    virtual unsigned int getRemotePort() const;
public: // instance methods
    virtual ESocketState state() const;
public: // overridables
    virtual void connectToServer( const QString& i_strRemoteHostName, unsigned int i_uRemotePort, int i_iTimeout_ms = 0 ); // 0 or less than 0 means infinite wait time
    virtual void disconnectFromServer( int i_iTimeout_ms = 0 );                        // 0 or less than 0 means infinite wait time
    virtual void abort();
    virtual int messagesAvailable() const;
    virtual void writeMessage( ZS::System::CMsg* i_pMsg );
    virtual ZS::System::CMsg* readMessage();
    virtual ZS::System::EResult error() const;
    virtual QString errorString() const;
    virtual ZS::System::SErrResultInfo errResultInfo() const;
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pEv );
protected slots:
    virtual void onTimerTimeout();
    virtual void onServerDestroyed( QObject* i_pServer );
    virtual void onSocketPeerDestroyed( QObject* i_pSocketPeer );
protected: // instance methods
    QRecursiveMutex*           m_pMutex;
    ESrvCltType                m_srvCltType;           // Defines on which side of the connection the socket is existing
    CInProcMsgServer*          m_pInProcMsgServer;     // On client's side the server to which the socket should be connected to, on server's side the server which created the socket for the new connection.
    CInProcMsgSocket*          m_pInProcMsgSocketPeer; // "Peer" to which this socket is connected (socket on the other side of the connection).
    ESocketState               m_socketState;
    int                        m_iReqMsgId;
    unsigned int               m_uServerListenPort;
    QString                    m_strLocalHostName;
    unsigned int               m_uLocalPort;
    QString                    m_strRemoteHostName;
    unsigned int               m_uRemotePort;
    QTimer*                    m_pTimer;
    bool                       m_bOnTimerTimeoutConnected;
    double                     m_fTimerStartTime_ms;
    int                        m_iConnectTimeout_ms;
    int                        m_iDisconnectTimeout_ms;
    ZS::System::SErrResultInfo m_errResultInfo;
    ZS::System::ECopyDepth     m_copyDepthMsgReadBuff;
    QList<ZS::System::CMsg*>   m_arpMsgReadBuff;
    bool                       m_bTracingEnabled;
    ZS::System::CTrcAdminObj*  m_pTrcAdminObj;

}; // class CInProcMsgSocket

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_InProcMsgSocket_h
