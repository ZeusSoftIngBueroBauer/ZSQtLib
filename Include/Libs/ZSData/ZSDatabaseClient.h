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

#ifndef ZSData_DatabaseClient_h
#define ZSData_DatabaseClient_h

#include "ZSData/ZSDataCommon.h"
#include "ZSSys/ZSSysRequestQueue.h"

class QMutex;
class QTimer;

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Data
{
class CDb;
class CDataSet;
class CDbClientGatewayThread;
class CDbClientGateway;
//class CMsgReqConnect;
//class CMsgConConnect;
//class CMsgReqDisconnect;
//class CMsgConDisconnect;
//class CMsgReqChangeSettings;
//class CMsgConChangeSettings;
//class CMsgReqRegister;
//class CMsgConRegister;
//class CMsgReqUnregister;
//class CMsgConUnregister;
//class CMsgReqSelectSchema;
//class CMsgConSelectSchema;
//class CMsgReqUpdateSchema;
//class CMsgConUpdateSchema;
//class CMsgReqSelectData;
//class CMsgConSelectData;
//class CMsgReqUpdateData;
//class CMsgConUpdateData;

//******************************************************************************
class ZSDATADLL_API CDbClient : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Data::DbClient"; }
    static QString ClassName() { return "CDbClient"; }
public: // type definitions and constants
    // Please note that a Connected Client may send commands and queries to the database.
    // But if not registered it will not automatically notified by schema or data changes.
    enum EState {               // GatewayThread | Socket       | Registered4NotificationsFromDb
                                //---------------+--------------+-------------------------------
        EStateIdle        =  0, // not existing  | not existing | Unregistered
        EStateUnconnected =  1, // Running       | Unconnected  | Unregistered
        EStateConnected   =  2, // Running       | Connected    | Unregistered
        EStateRegistered  =  3, // Running       | Connected    | Registered
        EStateCount,
        EStateUndefined
    };
    static QString State2Str( int i_iState );
public: // type definitions and constants
    enum ERequest {
        // Main request aborting queries in progress.
        ERequestNone                  =  0,
        ERequestStartup               =  1,
        ERequestShutdown              =  2,
        ERequestConnect               =  3,
        ERequestDisconnect            =  4,
        ERequestUpdateHostSettings    =  5,
        ERequestRegister              =  6,
        ERequestUnregister            =  7,
        ERequestSelectSchema          =  8,
        ERequestUpdateSchema          =  9,
        ERequestSelectData            = 10,
        ERequestUpdateData            = 11,
        // Internal requests
        ERequestStartGatewayThread    = 12,
        ERequestStopGatewayThread     = 13,
        ERequestStartupGateway        = 14,
        ERequestShutdownGateway       = 15,
        ERequestConnectGateway        = 16,
        ERequestDisconnectGateway     = 17,
        ERequestRegisterGateway       = 18,
        ERequestUnregisterGateway     = 19,
        ERequestCount,
        ERequestUndefined
    };
    static QString Request2Str( int i_iRequest );
public: // ctors and dtor
    CDbClient( const QString& i_strObjName = "DbClt" ); // default ctor creates database client with:
        // ObjectName:          "DbClt"
        // ProtocolTypes:       [ZSXML, ZSMsg]
        // SocketTypes:         {ZSXML: [Tcp], ZSMsg: [InProcMsg]}
        // UseCommandThreads:   false
        // ConnectionName:      ""
        // MultiThreadedAccess: false
    virtual ~CDbClient();
signals:
    void stateChanged( ZS::Data::CDbClient* i_pClient, ZS::Data::CDbClient::EState i_state );
    void requestInProgressChanged( ZS::Data::CDbClient* i_pClient, ZS::System::SRequestDscr i_reqDscr );
signals:
    void connected( ZS::Data::CDbClient* i_pClient, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void disconnected( ZS::Data::CDbClient* i_pClient, const ZS::Ipc::SSocketDscr& i_socketDscr );
signals:
    void defaultProtocolTypeChanged( ZS::Data::CDbClient* i_pClient, ZS::Data::EProtocolType i_protocolType );
    void protocolTypeChanged( ZS::Data::CDbClient* i_pClient, ZS::Data::EProtocolType i_protocolType );
    void protocolTypeRowVersionChanged( ZS::Data::CDbClient* i_pClient );
signals:
    void defaultSocketTypeChanged( ZS::Data::CDbClient* i_pClient, ZS::Ipc::ESocketType i_socketType );
    void socketTypeChanged( ZS::Data::CDbClient* i_pClient, ZS::Ipc::ESocketType i_socketType );
    void socketTypeRowVersionChanged( ZS::Data::CDbClient* i_pClient );
signals:
    void defaultHostSettingsChanged(
        ZS::Data::CDbClient*                i_pClient,
        ZS::Data::EProtocolType             i_protocolType,
        const ZS::Ipc::SClientHostSettings& i_hostSettings );
    void hostSettingsChanged(
        ZS::Data::CDbClient*                i_pClient,
        ZS::Data::EProtocolType             i_protocolType,
        const ZS::Ipc::SClientHostSettings& i_hostSettings );
    void hostSettingsRowVersionChanged(
        ZS::Data::CDbClient*    i_pClient,
        ZS::Data::EProtocolType i_protocolType,
        ZS::Ipc::ESocketType    i_socketType );
signals:
    void defaultWatchDogSettingsChanged(
        ZS::Data::CDbClient*           i_pClient,
        ZS::Data::EProtocolType        i_protocolType,
        ZS::Ipc::ESocketType           i_socketType,
        const ZS::Ipc::STimerSettings& i_watchDogSettings );
    void watchDogSettingsChanged(
        ZS::Data::CDbClient*           i_pClient,
        ZS::Data::EProtocolType        i_protocolType,
        ZS::Ipc::ESocketType           i_socketType,
        const ZS::Ipc::STimerSettings& i_watchDogSettings );
    void watchDogSettingsRowVersionChanged(
        ZS::Data::CDbClient*    i_pClient,
        ZS::Data::EProtocolType i_protocolType,
        ZS::Ipc::ESocketType    i_socketType );
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setKeepReqDscrInExecTree( bool i_bKeep );
    bool keepReqDscrInExecTree() const;
public: // instance methods
    CDataSet* getDataSet() const { return m_pDS; }
    void setDataSet( CDataSet* i_pDS );
public: // instance methods (on passing Count use all socket types, on passing Undefined use current socket type)
    int getProtocolTypesCount( ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeUndefined ) const;
    QList<EProtocolType> getProtocolTypes( ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeCount ) const;
    bool isProtocolTypeChangeable( ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeUndefined ) const;
    bool isProtocolTypeSupported( EProtocolType i_protocolType, ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeUndefined ) const;
public: // instance methods (on passing Count use all protocol types, on passing Undefined use current protocol type)
    int getSocketTypesCount( EProtocolType i_protocolType = EProtocolTypeUndefined ) const;
    QList<Ipc::ESocketType> getSocketTypes( EProtocolType i_protocolType = EProtocolTypeUndefined ) const;
    bool isSocketTypeChangeable( EProtocolType i_protocolType = EProtocolTypeUndefined ) const;
    bool isSocketTypeSupported( ZS::Ipc::ESocketType i_socketType, EProtocolType i_protocolType = EProtocolTypeUndefined ) const;
public: // instance methods
    //bool connectionNeedsDatabaseName( ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeTcp, EProtocolType i_protocolType = EProtocolTypeSQL ) const;
    //bool connectionNeedsUserName( ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeTcp, EProtocolType i_protocolType = EProtocolTypeSQL ) const;
    //bool connectionNeedsPassword( ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeTcp, EProtocolType i_protocolType = EProtocolTypeSQL ) const;
public: // instance methods (affecting all settings)
    void setHostSettings2Default(); // The request method "updateHostSettings" need to be called afterwards.
public: // instance methods
    EProtocolType getProtocolType( ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    void setProtocolType( EProtocolType i_protocolType );
public: // instance methods changing and reading the host settings
    ZS::Ipc::ESocketType getSocketType( ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    void setSocketType( ZS::Ipc::ESocketType i_socketType );
public: // instance methods changing and reading the host settings (for protocol type InProcMsg)
    CDb* getDb( ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    void setDb( CDb* i_pDb );
public: // instance methods changing and reading the host settings (on passing Undefined use current socket type and current protocol type)
    QString getConnectionString(
        EProtocolType           i_protocolType = EProtocolTypeUndefined,
        ZS::Ipc::ESocketType    i_socketType = ZS::Ipc::ESocketTypeUndefined,
        ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    ZS::Ipc::SClientHostSettings getHostSettings(
        EProtocolType           i_protocolType = EProtocolTypeUndefined,
        ZS::Ipc::ESocketType    i_socketType = ZS::Ipc::ESocketTypeUndefined,
        ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    int getConnectTimeoutInMs(
        EProtocolType           i_protocolType = EProtocolTypeUndefined,
        ZS::Ipc::ESocketType    i_socketType = ZS::Ipc::ESocketTypeUndefined,
        ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    void setHostSettings( const ZS::Ipc::SClientHostSettings& i_settings, EProtocolType i_protocolType = EProtocolTypeUndefined );
public: // instance methods
    EProtocolType getDefaultProtocolType() const;
    void setDefaultProtocolType( EProtocolType i_protocolType );
public: // instance methods changing and reading the host settings
    void setDefaultSocketType( ZS::Ipc::ESocketType i_socketType );
    ZS::Ipc::ESocketType getDefaultSocketType() const;
public: // instance methods (on passing Undefined use current default socket type and current default protocol type)
    QString getDefaultConnectionString( EProtocolType i_protocolType = EProtocolTypeUndefined, ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeUndefined ) const;
    ZS::Ipc::SClientHostSettings* getDefaultHostSettings( EProtocolType i_protocolType = EProtocolTypeUndefined, ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeUndefined ) const;
    void setDefaultHostSettings( const ZS::Ipc::SClientHostSettings& i_settings, EProtocolType i_protocolType = EProtocolTypeUndefined  );
public: // instance methods changing and reading the watch dog settings (on passing Undefined use current socket type and current protocol type)
    bool isWatchDogTimerEnabled(
        EProtocolType           i_protocolType = EProtocolTypeUndefined,
        ZS::Ipc::ESocketType    i_socketType = ZS::Ipc::ESocketTypeUndefined,
        ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    int getWatchDogTimerIntervalInMs(
        EProtocolType           i_protocolType = EProtocolTypeUndefined,
        ZS::Ipc::ESocketType    i_socketType = ZS::Ipc::ESocketTypeUndefined,
        ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    int getWatchDogTimeoutInMs(
        EProtocolType           i_protocolType = EProtocolTypeUndefined,
        ZS::Ipc::ESocketType    i_socketType = ZS::Ipc::ESocketTypeUndefined,
        ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    ZS::Ipc::STimerSettings getWatchDogSettings(
        EProtocolType           i_protocolType = EProtocolTypeUndefined,
        ZS::Ipc::ESocketType    i_socketType = ZS::Ipc::ESocketTypeUndefined,
        ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    void setWatchDogSettings( 
        const ZS::Ipc::STimerSettings& i_settings,
        EProtocolType                  i_protocolType = EProtocolTypeUndefined,
        ZS::Ipc::ESocketType           i_socketType = ZS::Ipc::ESocketTypeUndefined );
public: // instance methods changing and reading the watch dog settings (on passing Undefined use current default socket type and current default protocol type)
    ZS::Ipc::STimerSettings* getDefaultWatchDogSettings(
        EProtocolType        i_protocolType = EProtocolTypeUndefined,
        ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeUndefined ) const;
    void setDefaultWatchDogSettings(
        const ZS::Ipc::STimerSettings& i_settings,
        EProtocolType                  i_protocolType = EProtocolTypeUndefined,
        ZS::Ipc::ESocketType           i_socketType = ZS::Ipc::ESocketTypeUndefined );
public: // instance methods of the remote connection (only valid if connection is established)
    SCnctId getCnctId() const;
    ZS::Ipc::SSocketDscr getSocketDscr() const;
    int getSocketId() const;
public: // instance methods (Register and Select Requests)
    //SDbClientRegSpec getRegistrationSpec() const;
    //QVector<SDSNodeSpec> getRegisterNodeSpecs() const { return m_arDSNodeSpecsRegister; }
    //void setRegisterNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs );
    //QVector<SDSNodeSpec> getSelectSchemaNodeSpecs() const { return m_arDSNodeSpecsSelectSchema; }
    //void setSelectSchemaNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs );
    //QVector<SDSNodeSpec> getSelectDataNodeSpecs() const { return m_arDSNodeSpecsSelectData; }
    //void setSelectDataNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs );
public: // instance methods
    EState getState() const;
    QString state2Str() const;
public: // instance methods
    bool isConnected() const;
    bool isConnecting() const;
public: // instance methods
    //bool isRegistered() const;
public: // instance methods
    bool isBusy() const; // alias for "isRequestInProgress(-1,ERequestUndefined,false)"
    bool isRequestInProgress( int i_iRequestId = -1, ERequest i_request = ERequestUndefined, bool i_bIgnorePostponedRequests = false ) const;      // -1 and Undefined to check whether any request is in progress
    ERequest getRequestInProgress() const;
    QString requestInProgress2Str( bool i_bShort = true ) const;
public: // instance methods
    void abortRequest( qint64 i_iRequestId );
    void abortRequestInProgress();
    //void abortAllExecCommandRequests();
    void abortAllRequests();
public: // overridables
    virtual ZS::System::CRequest* startup(
        int    i_iTimeout_ms  = 0,
        bool   i_bWait        = false,
        qint64 i_iReqIdParent = -1 );
    virtual ZS::System::CRequest* shutdown(
        int    i_iTimeout_ms  = 0,
        bool   i_bWait        = false,
        qint64 i_iReqIdParent = -1 );
public: // overridables
    virtual ZS::System::CRequest* connect_(
        int    i_iTimeout_ms  = 0,
        bool   i_bWait        = false,
        qint64 i_iReqIdParent = -1 );
    virtual ZS::System::CRequest* disconnect_(
        int    i_iTimeout_ms  = 0,
        bool   i_bWait        = false,
        qint64 i_iReqIdParent = -1 );
public: // overridables
    // Need to be called after changing settings to apply the settings:
    virtual ZS::System::CRequest* updateHostSettings(
        int    i_iTimeout_ms  = 0,
        bool   i_bWait        = false,
        qint64 i_iReqIdParent = -1 );
public: // overridables
    //virtual ZS::System::CRequest* register_(
    //    int    i_iTimeout_ms  = 0,
    //    bool   i_bWait        = false,
    //    qint64 i_iReqIdParent = -1 );
    //virtual ZS::System::CRequest* unregister(
    //    int    i_iTimeout_ms  = 0,
    //    bool   i_bWait        = false,
    //    qint64 i_iReqIdParent = -1 );
public: // overridables
    //virtual ZS::System::CRequest* selectSchema(
    //    int    i_iTimeout_ms  = 0,
    //    bool   i_bWait        = false,
    //    qint64 i_iReqIdParent = -1 );
    //virtual ZS::System::CRequest* updateSchema(
    //    int    i_iTimeout_ms  = 0,
    //    bool   i_bWait        = false,
    //    qint64 i_iReqIdParent = -1 );
public: // overridables
    //virtual ZS::System::CRequest* selectData(
    //    int    i_iTimeout_ms  = 0,
    //    bool   i_bWait        = false,
    //    qint64 i_iReqIdParent = -1 );
    //virtual ZS::System::CRequest* updateData(
    //    int    i_iTimeout_ms  = 0,
    //    bool   i_bWait        = false,
    //    qint64 i_iReqIdParent = -1 );
public: // overridables
    //virtual ZS::System::CRequest* connectAndRegister(       // Tasks performed in this order: connect, selectSchema, selectData, register
    //    int    i_iTimeout_ms  = 0,
    //    bool   i_bWait        = false,
    //    qint64 i_iReqIdParent = -1 );
    //virtual ZS::System::CRequest* disconnectAndUnregister(  // Tasks performed in this order: unregister, disconnect
    //    int    i_iTimeout_ms  = 0,
    //    bool   i_bWait        = false,
    //    qint64 i_iReqIdParent = -1 );
public: // overridables
    // Attention !! Please note that the command object will be executed in another thread context and the
    // command object is not thread safe. So don't access (and don't delete) the command object after
    // calling this method if the request is executed asynchronously (which is very likely the case).
    // After the command has been executed the "request.changed" signal is emitted providing the result of
    // the command execution within the data portion of the request item. But don't delete the result command
    // object as the ownership has been taken over by the client. The client will delete the command object.
    //virtual ZS::System::CRequest* sendCommand( CCommand* i_pCmd, qint64 i_iReqIdParent = -1, bool i_bMustBeConfirmed = true ); // the client takes ownership of the command
    //virtual ZS::System::CRequest* sendCommands( const QList<CCommand*>& i_arpCmds, qint64 i_iReqIdParent = -1, bool i_bMustBeConfirmed = true ); // the client takes ownership of the command
public: // instance methods
    void addTrcMsgLogObject( QObject* i_pObj );
    void removeTrcMsgLogObject( QObject* i_pObj );
protected: // instance methods of internal state machine
    virtual void executeNextPostponedRequest();
protected: // overridables
    virtual void executeStartupRequest( ZS::System::CRequest* i_pReq );
    virtual void executeShutdownRequest( ZS::System::CRequest* i_pReq );
protected: // overridables
    virtual void executeConnectRequest( ZS::System::CRequest* i_pReq );
    virtual void executeDisconnectRequest( ZS::System::CRequest* i_pReq );
protected: // instance methods
    virtual void executeUpdateHostSettingsRequest( ZS::System::CRequest* i_pReq );
protected: // overridables
    //virtual void executeRegisterRequest( ZS::System::CRequest* i_pReq );
    //virtual void executeUnregisterRequest( ZS::System::CRequest* i_pReq );
protected: // overridables
    //virtual void executeSelectSchemaRequest( ZS::System::CRequest* i_pReq );
    //virtual void executeUpdateSchemaRequest( ZS::System::CRequest* i_pReq );
protected: // overridables
    //virtual void executeSelectDataRequest( ZS::System::CRequest* i_pReq );
    //virtual void executeUpdateDataRequest( ZS::System::CRequest* i_pReq );
protected: // overridables to parse and execute the incoming data stream
    //virtual ZS::System::CRequest* execCommandUpdate( CCommandUpdate* i_pCmd, qint64 i_iReqIdParent = -1, bool i_bMustBeConfirmed = true );
protected: // overridables
    //virtual void executeConnectAndRegisterRequest( ZS::System::CRequest* i_pReq );
    //virtual void executeDisconnectAndUnregisterRequest( ZS::System::CRequest* i_pReq );
protected: // overridables
    //virtual void onReceivedMsgConSelectSchema( CMsgConSelectSchema* i_pMsgCon );
    //virtual void onReceivedMsgConUpdateSchema( CMsgConUpdateSchema* i_pMsgCon );
protected: // overridables
    //virtual void onReceivedMsgConSelectData( CMsgConSelectData* i_pMsgCon );
    //virtual void onReceivedMsgConUpdateData( CMsgConUpdateData* i_pMsgCon );
protected: // overridables (auxiliary methods)
    virtual void createGatewayThread();
    virtual void destroyGatewayThread();
protected: // overridables (auxiliary methods, blocking (synchronous) methods)
    virtual ZS::System::CRequest* startGatewayThread( int i_iTimeout_ms = 10000, qint64 i_iReqIdParent = -1 );
    virtual ZS::System::CRequest* stopGatewayThread( int i_iTimeout_ms = 10000, qint64 i_iReqIdParent = -1 );
    virtual ZS::System::CRequest* startupGateway( int i_iTimeout_ms = 10000, qint64 i_iReqIdParent = -1 );
    virtual ZS::System::CRequest* shutdownGateway( int i_iTimeout_ms = 10000, qint64 i_iReqIdParent = -1 );
protected: // overridables (auxiliary methods, optional blocking (synchronous) or unsynchronous methods)
    virtual ZS::System::CRequest* connectGateway( int i_iTimeout_ms = 10000, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
    virtual ZS::System::CRequest* disconnectGateway( int i_iTimeout_ms = 10000, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
    virtual ZS::System::CRequest* registerGateway( int i_iTimeout_ms = 10000, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
    virtual ZS::System::CRequest* unregisterGateway( int i_iTimeout_ms = 10000, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
protected: // auxiliary methods
    void acceptChangedSettings();
protected slots:
    void onRequestTimeout();
protected slots:
    void onRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots:
    void onDbDestroyed( QObject* i_pDb );
protected slots:
    void onDSDestroyed( QObject* i_pDS );
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pMsg );
protected: // instance members
    QMutex*                                         m_pMtx;
    CDataSet*                                       m_pDS;
    // Supported protocol and connection types
    QList<EProtocolType>                            m_arProtocolTypes;    // As default only protocol type ZS is supported
    QHash<EProtocolType,QList<Ipc::ESocketType>>    m_hsharSocketTypes;   // As default only protocol type ZS and connection type InProcMsg is supported.
    // Default, current and new data protocol type to be adjusted
    EProtocolType                                   m_protocolTypeDefault;
    EProtocolType                                   m_protocolTypeOrig;
    EProtocolType                                   m_protocolTypeCurr;
    // Default, current and new socket type to be adjusted
    ZS::Ipc::ESocketType                            m_socketTypeDefault;
    ZS::Ipc::ESocketType                            m_socketTypeOrig;
    ZS::Ipc::ESocketType                            m_socketTypeCurr;
    // Default, current and new host settings to be adjusted
    QHash<SCnctType,Ipc::SClientHostSettings>       m_hshHostSettingsDefault;
    QHash<SCnctType,Ipc::SClientHostSettings>       m_hshHostSettingsOrig;
    QHash<SCnctType,Ipc::SClientHostSettings>       m_hshHostSettingsCurr;
    // Currently used connection Id and socket descriptor (only valid if client is connected)
    SCnctId                                         m_cnctId;
    ZS::Ipc::SSocketDscr                            m_socketDscr;
    // Default, current and new data watch dog settings to be adjusted
    QHash<SCnctType,Ipc::STimerSettings>            m_hshWatchDogSettingsDefault;
    QHash<SCnctType,Ipc::STimerSettings>            m_hshWatchDogSettingsOrig;
    QHash<SCnctType,Ipc::STimerSettings>            m_hshWatchDogSettingsCurr;
    // Gateway
    CDbClientGatewayThread*                         m_pGatewayThread;
    CDbClientGateway*                               m_pGateway;
    // State Machine
    EState                                          m_state;
    ZS::System::CRequestQueue*                      m_pRequestQueue;
    ZS::System::SRequestDscr                        m_reqDscrTimeout;
    QTimer*                                         m_pTmrReqTimeout;
    bool                                            m_bMsgReqContinuePending;
    bool                                            m_bIsBeingDestroyed;
    // Connect, disconnect and change settings requests are executed one after another.
    // Register Requests
    //QVector<SDSNodeSpec>                          m_arDSNodeSpecsRegister;
    //QVector<SDSNodeSpec>                          m_arDSNodeSpecsSelectSchema;
    //QVector<SDSNodeSpec>                          m_arDSNodeSpecsSelectData;
    // Requests received by the database:
    //qint64                                        m_iReqMsgIdDb;     // = pMsgReq->getMsgId()
    //qint64                                        m_iReqIdDb;        // = pMsgReq->getRequestId()
    //CRequest*                                     m_pReqDb;          // Request initiated by request message received from database.
    // Tracing
    QList<QObject*>                                 m_arpTrcMsgLogObjects;
    Trace::CTrcAdminObj*                            m_pTrcAdminObj;

}; // class CDbClient

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSData_DatabaseClient_h
