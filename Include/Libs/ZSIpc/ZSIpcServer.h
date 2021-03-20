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

#ifndef ZSIpc_Server_h
#define ZSIpc_Server_h

#include <QtCore/qobject.h>

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysRequest.h"

class QMutex;
class QTimer;

namespace ZS
{
namespace System
{
class CErrLog;
class CRequest;
class CRequestQueue;
}
namespace Trace
{
class CTrcAdminObj;
class CTrcMthFile;
}

namespace Ipc
{
class CSrvCltBaseGatewayThread;

//******************************************************************************
class ZSIPCDLL_API CServer : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // overridables
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CServer"; }
public: // type definitions and constants
    enum EState {
        EStateIdle      = 0, // gateway thread is not created, server is not listening
        EStateListening = 1, // gateway thread is running, server is listening
        EStateCount
    };
    static QString State2Str( int i_iState, bool i_bDetailed = false );
    enum ERequest {
        ERequestNone                = 0,
        ERequestStartup             = 1,
        ERequestShutdown            = 2,
        ERequestChangeSettings      = 3,
        ERequestSendData            = 4,
        ERequestStartGatewayThread  = 5,
        ERequestStopGatewayThread   = 6,
        ERequestStartupGateway      = 7,
        ERequestShutdownGateway     = 8,
        ERequestCount,
        ERequestUndefined
    };
    static QString Request2Str( int i_iRequest );
public: // ctors and dtor
    CServer(
        const QString&      i_strObjName,
        bool                i_bMultiThreadedAccess = false,  // If true each access to member variables will be protected by a mutex (and the class becomes thread safe).
        Trace::CTrcMthFile* i_pTrcMthFile = nullptr,         // If != nullptr trace method file with detail level is used instead of trace admin object with Trace server.
        int                 i_iTrcMthFileDetailLevel = Trace::ETraceDetailLevelMethodArgs );
    virtual ~CServer();
signals: // of the remote connection
    void connected( QObject* i_pServer, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void disconnected( QObject* i_pServer, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void socketDscrChanged( QObject* i_pServer, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void settingsChanged( QObject* i_pServer );
    void stateChanged( QObject* i_pServer, int i_iState );
    void requestInProgressChanged( QObject* i_pServer, ZS::System::SRequestDscr i_reqDscr );
    void receivedData( QObject* i_pServer, int i_iSocketId, const QByteArray& i_byteArr );
public: // overridables
    virtual QString nameSpace() const { return CServer::NameSpace(); }
    virtual QString className() const { return CServer::ClassName(); }
public: // instance methods
    QString getName() const { return m_strObjName; }
public: // instance methods
    void setKeepReqDscrInExecTree( bool i_bKeep );
    bool keepReqDscrInExecTree() const;
public: // overridables of the remote connection
    virtual ZS::System::CRequest* startup( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
    virtual ZS::System::CRequest* shutdown( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 ); 
public: // overridables of the remote connection
    // Need to be called after changing settings to apply the settings.
    virtual ZS::System::CRequest* changeSettings( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
public: // overridables of the remote connection
    virtual ZS::System::CRequest* sendData( int i_iSocketId, const QByteArray& i_byteArr, int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
public: // instance methods
    QList<ESocketType> getSocketTypes() const;
    int getSocketTypesCount() const;
    ESocketType getSocketType( int i_idx = 0 ) const;
    bool isSocketTypeChangeable() const;
    bool isSocketTypeSupported( ESocketType i_socketType ) const;
public: // instance methods changing and reading the host settings
    SServerHostSettings getHostSettings() const;
    virtual void setHostSettings( const SServerHostSettings& i_hostSettings ); // The request method "changeSettings" need to be called afterwards.
public: // instance methods of the remote connection
    virtual void setBlkType( CBlkType* i_pBlkType ); // The class takes ownership of the data block. The request method "changeSettings" need to be called afterwards.
    CBlkType* getBlkType();
public: // instance methods of the remote connection
    int getSocketId( int i_idxSocket ) const;
    SSocketDscr getSocketDscr( int i_iSocketId ) const;
    SSocketDscr findSocketDscr( const QString& i_strRemoteHostName, unsigned int i_uRemotePort ) const;
    int getArrLenConnections() const;
    int getActiveConnections() const;
public: // instance methods of the remote connection
    virtual void setSocketName( int i_iSocketId, const QString& i_strSocketName );       // Emits signal "socketDscrChanged".
public: // instance methods (state machine)
    EState getState() const;
    QString state2Str() const;
    bool isListening() const;
    bool isStartingUp() const;
    bool isConnected( int i_iSocketId = ZS::Ipc::ESocketIdUndefined ) const;
public: // instance methods (state machine)
    bool isBusy() const;
    ERequest requestInProgress() const;
    QString requestInProgress2Str( bool i_bShort = true ) const;
    ZS::System::CRequest* getRequestInProgress() const;
public: // instance methods (aborting requests)
    virtual void abortRequest( qint64 i_iRequestId );
    virtual void abortRequestInProgress();
    virtual void abortAllRequests();
public: // instance methods to trace methods calls
    virtual void addTrcMsgLogObject( QObject* i_pObj );
    virtual void removeTrcMsgLogObject( QObject* i_pObj );
protected: // overridables
    virtual void onReceivedData( int i_iSocketId, const QByteArray& i_byteArr );
protected: // instance methods
    virtual void executeNextPostponedRequest();
protected: // overridables
    virtual void executeStartupRequest( ZS::System::CRequest* i_pReq );
    virtual void executeShutdownRequest( ZS::System::CRequest* i_pReq );
    virtual void executeChangeSettingsRequest( ZS::System::CRequest* i_pReq );
    virtual void executeSendDataRequest( ZS::System::CRequest* i_pReq );
protected: // overridables (auxiliary methods)
    virtual CSrvCltBaseGatewayThread* createGatewayThread();
    virtual ZS::System::SErrResultInfo startGatewayThread( int i_iTimeout_ms = 10000, qint64 i_iReqIdParent = -1 );
    virtual ZS::System::SErrResultInfo stopGatewayThread( int i_iTimeout_ms = 10000, qint64 i_iReqIdParent = -1 );
protected: // overridables (auxiliary methods)
    virtual ZS::System::CRequest* startupGateway( int i_iTimeout_ms = 10000, bool i_bWait = false, qint64 i_iReqIdParent = -1 );    // underscore appended to distinguish from QObject::connect (important to create python bindings)
    virtual ZS::System::CRequest* shutdownGateway( int i_iTimeout_ms = 10000, bool i_bWait = false, qint64 i_iReqIdParent = -1 );    // underscore appended to distinguish from QObject::connect (important to create python bindings)
protected slots:
    virtual void onRequestTimeout();
protected slots:
    virtual void onRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected: // instance methods
    ZS::System::SErrResultInfo checkSocket( int i_iSocketId ) const;
protected: // instance methods
    bool isMethodTraceActive( int i_iFilterDetailLevel ) const;
    int getMethodTraceDetailLevel() const;
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pEv ) override;
protected: // instance members
    QMutex*                    m_pMtx;
    QString                    m_strObjName;
    ZS::System::CErrLog*       m_pErrLog;
    // Connection settings
    QList<ESocketType>         m_arSocketTypes; // as default only socket type Tcp is supported
    SServerHostSettings        m_hostSettings;
    CBlkType*                  m_pBlkType;
    QVector<SSocketDscr*>      m_arpSocketDscr;
    // Gateway
    CSrvCltBaseGatewayThread*  m_pGatewayThread;
    QObject*                   m_pGateway;
    // State Machine
    EState                     m_state;
    bool                       m_bReqExecTreeCreated;
    ZS::System::CRequestQueue* m_pRequestQueue;
    ZS::System::SRequestDscr   m_reqDscrTimeout;
    QTimer*                    m_pTmrReqTimeout;
    bool                       m_bMsgReqContinuePending;
    bool                       m_bIsBeingDestroyed;
    // The methods of the Ipc server base class and the Ipc server gateway
    // are writing trace outputs to the local trace file. But the trace server
    // wants to send data to the trace client. This data should not be written
    // to the local trace file as that may just confuse the software developer.
    // For this the "tracing enabled" flag and methods have been introduced for
    // the Ipc server class and its gateway.
    QList<QObject*>            m_arpTrcMsgLogObjects;
    int                        m_iTrcMthFileDetailLevel;
    Trace::CTrcMthFile*        m_pTrcMthFile;   // Either trace method file with detail level is used or
    Trace::CTrcAdminObj*       m_pTrcAdminObj;  // trace admin object with IpcTrace server.

}; // class CServer

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_Server_h
