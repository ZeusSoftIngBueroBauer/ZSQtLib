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

#ifndef ZSIpc_Client_h
#define ZSIpc_Client_h

#include <QtCore/qobject.h>

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysRequest.h"

class QTimer;

namespace ZS
{
namespace System
{
class CErrLog;
class CMutex;
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
class ZSIPCDLL_API CClient : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CClient"; }
public: // type definitions and constants
    enum EState {
        EStateUnconnected = 0, // gateway thread is not created, socket is not connected
        EStateConnected   = 1, // gateway thread is running, socket is connected
        EStateCount
    };
    static QString State2Str( int i_iState, bool i_bDetailed = false );
    enum ERequest {
        ERequestNone                = 0,
        ERequestConnect             = 1,
        ERequestDisconnect          = 2,
        ERequestChangeSettings      = 3,
        ERequestSendData            = 4,
        ERequestStartGatewayThread  = 5,
        ERequestStopGatewayThread   = 6,
        ERequestConnectGateway      = 7,
        ERequestDisconnectGateway   = 8,
        ERequestCount,
        ERequestUndefined
    };
    static QString Request2Str( int i_iRequest );
public: // ctors and dtor
    CClient(
        const QString& i_strObjName,
        bool           i_bMultiThreadedAccess = false,
        int            i_iTrcMthFileDetailLevel = ZS::Trace::ETraceDetailLevelNone,
        int            i_iTrcMthFileDetailLevelMutex = ZS::Trace::ETraceDetailLevelNone,
        int            i_iTrcMthFileDetailLevelGateway = ZS::Trace::ETraceDetailLevelNone );
    virtual ~CClient();
signals: // of the remote connection
    void connected( QObject* i_pClient );
    void disconnected( QObject* i_pClient );
    void settingsChanged( QObject* i_pClient );
    void stateChanged( QObject* i_pClient, int i_iState );
    void requestInProgressChanged( QObject* i_pClient, ZS::System::SRequestDscr i_reqDscr );
    void receivedData( QObject* i_pClient, const QByteArray& i_byteArr );
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    QString getName() const { return m_strObjName; }
public: // instance methods
    void setKeepReqDscrInExecTree( bool i_bKeep );
    bool keepReqDscrInExecTree() const;
public: // overridables of the remote connection
    virtual ZS::System::CRequest* connect_( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );    // underscore appended to distinguish from QObject::connect (important to create python bindings)
    virtual ZS::System::CRequest* disconnect_( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 ); // underscore appended to distinguish from QObject::disconnect(important to create python bindings)
public: // overridables of the remote connection
    // Need to be called after changing settings to apply the settings.
    virtual ZS::System::CRequest* changeSettings( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
public: // overridables of the remote connection
    virtual ZS::System::CRequest* sendData( const QByteArray& i_byteArr, int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
public: // instance methods
    virtual void setOnlyLocalHostConnectionsAreAllowed( bool i_bAllowOnlyLocalHostConnections );
public: // instance methods
    QList<ESocketType> getSocketTypes() const;
    int getSocketTypesCount() const;
    ESocketType getSocketType( int i_idx = 0 ) const;
    bool isSocketTypeChangeable() const;
    bool isSocketTypeSupported( ESocketType i_socketType ) const;
public: // instance methods changing and reading the host settings
    QString getConnectionString() const;
    SClientHostSettings getHostSettings() const;
    virtual void setHostSettings( const SClientHostSettings& i_hostSettings ); // The request method "changeSettings" need to be called afterwards.
public: // instance methods
    void setWatchDogTimerUsed( bool i_bUsed );
    bool isWatchDogTimerUsed() const { return m_bWatchDogTimerUsed; }
    bool isWatchDogTimerEnabled() const;
    int getWatchDogTimerIntervalInMs() const;
    int getWatchDogTimeoutInMs() const;
    STimerSettings getWatchDogSettings() const;
    virtual void setWatchDogSettings( const STimerSettings& i_settings ); // The request method "changeSettings" need to be called afterwards.
public: // instance methods of the remote connection
    virtual void setBlkType( CBlkType* i_pBlkType ); // The class takes ownership of the data block. The request method "changeSettings" need to be called afterwards.
    CBlkType* getBlkType();
public: // instance methods of the remote connection
    int getSocketId() const;
    SSocketDscr getSocketDscr() const;
    int getConnectTimeoutInMs() const;
public: // instance methods (state machine)
    EState getState() const;
    QString state2Str() const;
    bool isConnected() const;
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
public: // instance methods to trace methods calls
    void setMethodTraceDetailLevel( int i_iTrcDetailLevel );
protected: // overridables of the remote connection
    virtual void onReceivedData( const QByteArray& i_byteArr );
protected: // instance methods
    virtual void executeNextPostponedRequest();
protected: // overridables
    virtual void executeConnectRequest( ZS::System::CRequest* i_pReq );
    virtual void executeDisconnectRequest( ZS::System::CRequest* i_pReq );
    virtual void executeChangeSettingsRequest( ZS::System::CRequest* i_pReq );
    virtual void executeSendDataRequest( ZS::System::CRequest* i_pReq );
protected: // overridables (auxiliary methods)
    virtual CSrvCltBaseGatewayThread* createGatewayThread();
    virtual ZS::System::SErrResultInfo startGatewayThread( int i_iTimeout_ms = 10000, qint64 i_iReqIdParent = -1 );
    virtual ZS::System::SErrResultInfo stopGatewayThread( int i_iTimeout_ms = 10000, qint64 i_iReqIdParent = -1 );
protected: // overridables (auxiliary methods)
    virtual ZS::System::CRequest* connectGateway( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );    // underscore appended to distinguish from QObject::connect (important to create python bindings)
    virtual ZS::System::CRequest* disconnectGateway( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );    // underscore appended to distinguish from QObject::connect (important to create python bindings)
protected slots:
    virtual void onRequestTimeout();
protected slots:
    virtual void onRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected: // instance methods
    bool isMethodTraceActive( int i_iFilterDetailLevel ) const;
    int getMethodTraceDetailLevel() const;
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pEv ) override;
protected: // instance members
    ZS::System::CMutex*        m_pMtx;
    QString                    m_strObjName;
    ZS::System::CErrLog*       m_pErrLog;
    // Connection settings
    bool                       m_bOnlyLocalHostConnectionsAreAllowed;
    QList<ESocketType>         m_arSocketTypes; // as default only socket type Tcp is supported
    SClientHostSettings        m_hostSettings;
    SSocketDscr                m_socketDscr;  // valid if connected
    bool                       m_bWatchDogTimerUsed;
    STimerSettings             m_watchDogTimerSettings;
    CBlkType*                  m_pBlkType;
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
    /*!< The methods of the Ipc server base class and the Ipc server gateway
         are writing trace outputs to the local trace file. But the trace server
         wants to send data to the trace client. This data should not be written
         to the local trace file as that may just confuse the software developer.
         For this the "tracing enabled" flag and methods have been introduced for
         the Ipc server class and its gateway. */
    QList<QObject*>            m_arpTrcMsgLogObjects;
    /*<! Trace detail level used if the method trace of the client got to be output
         directly to a trace method file and not through the trace server. */
    int                        m_iTrcMthFileDetailLevel;
    /*<! Trace detail level used if the method trace of the client's gateway got to be output
         directly to a trace method file and not through the trace server. */
    int                        m_iTrcMthFileDetailLevelGateway;
    /*<! Reference to local trace method file. Used if the trace client itself got to be logged. */
    ZS::Trace::CTrcMthFile*    m_pTrcMthFile;
    /*!< Trace admin object with trace server. */
    ZS::Trace::CTrcAdminObj*   m_pTrcAdminObj;

}; // class CClient

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_Client
