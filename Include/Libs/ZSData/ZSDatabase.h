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

#ifndef ZSDatabase_h
#define ZSDatabase_h

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
class CDataSet;
class CDb;
class CDbGatewayThread;
class CDbGateway;

typedef CDataSet* (*TFctCreateDataSet)();
typedef void (*TFctAddDataSetObjects)( CDataSet* i_pDS );

//******************************************************************************
class ZSDATADLL_API CDb : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Data::Db"; }
    static QString ClassName() { return "CDb"; }
public: // type definitions and constants
    typedef enum {
        EStateIdle       = 0, // gateway thread(s) is (are) not created, server is not listening
        EStateListening  = 1, // gateway thread(s) is (are) running, server is listening
        EStateCount,
        EStateUndefined
    }   EState;
    static QString State2Str( int i_iState );
public: // type definitions and constants
    typedef enum {
        // Main Requests
        ERequestNone                =  0,
        ERequestStartup             =  1,
        ERequestShutdown            =  2,
        ERequestUpdateHostSettings  =  3,
        // Internal requests
        ERequestStartGatewayThreads =  4,
        ERequestStopGatewayThreads  =  5,
        ERequestStartGatewayThread  =  6,
        ERequestStopGatewayThread   =  7,
        ERequestStartupGateways     =  8,
        ERequestShutdownGateways    =  9,
        ERequestStartupGateway      = 10,
        ERequestShutdownGateway     = 11,
        ERequestCount,
        ERequestUndefined
    }   ERequest;
    static QString Request2Str( int i_iRequest );
public: // ctors and dtor
    CDb( const QString&  i_strObjName = "Db" ); // default ctor creates database with:
        // ObjectName:          "Db"
        // ProtocolTypes:       [ZSXML, ZSMsg]
        // SocketTypes:         {ZSXML: [Tcp], ZSMsg: [InProcMsg]}
        // MultiThreadedAccess: false
    virtual ~CDb();
signals:
    void stateChanged( ZS::Data::CDb* i_pDb, ZS::Data::CDb::EState i_state );
    void requestInProgressChanged( ZS::Data::CDb* i_pDb, ZS::System::SRequestDscr i_reqDscr );
signals:
    void defaultHostSettingsChanged(
        ZS::Data::CDb*                      i_pDb,
        ZS::Data::EProtocolType             i_protocolType,
        const ZS::Ipc::SServerHostSettings& i_hostSettings );
    void hostSettingsChanged(
        ZS::Data::CDb*                      i_pDb,
        ZS::Data::EProtocolType             i_protocolType,
        const ZS::Ipc::SServerHostSettings& i_hostSettings );
    void hostSettingsRowVersionChanged(
        ZS::Data::CDb*          i_pDb,
        ZS::Data::EProtocolType i_protocolType,
        ZS::Ipc::ESocketType    i_socketType );
signals:
    void connected( ZS::Data::CDb* i_pDb, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void disconnected( ZS::Data::CDb* i_pDb, const ZS::Ipc::SSocketDscr& i_socketDscr );
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setKeepReqDscrInExecTree( bool i_bKeep );
    bool keepReqDscrInExecTree() const;
public: // instance methods (on passing Count or Undefined take all socket types into account)
    int getProtocolTypesCount( ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeUndefined ) const;
    QList<EProtocolType> getProtocolTypes( ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeUndefined ) const;
    bool isProtocolTypeSupported( EProtocolType i_protocolType, ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeUndefined ) const;
public: // instance methods (on passing Count or Undefined take all protocol types into account)
    int getSocketTypesCount( EProtocolType i_protocolType = EProtocolTypeUndefined ) const;
    QList<ZS::Ipc::ESocketType> getSocketTypes( EProtocolType i_protocolType = EProtocolTypeUndefined ) const;
    bool isSocketTypeSupported( ZS::Ipc::ESocketType i_socketType, EProtocolType i_protocolType = EProtocolTypeUndefined ) const;
public: // instance methods (local connection parameters)
    QString getConnectionString( EProtocolType i_protocolType, ZS::Ipc::ESocketType i_socketType, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    ZS::Ipc::SServerHostSettings getHostSettings( EProtocolType i_protocolType, ZS::Ipc::ESocketType i_socketType, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    void setHostSettings( EProtocolType i_protocolType, const ZS::Ipc::SServerHostSettings& i_settings );
public: // instance methods (local connection parameters)
    QString getDefaultConnectionString( EProtocolType i_protocolType, ZS::Ipc::ESocketType i_socketType ) const;
    ZS::Ipc::SServerHostSettings* getDefaultHostSettings( EProtocolType i_protocolType, ZS::Ipc::ESocketType i_socketType ) const; // nullptr, if no default settings have been set
    void setDefaultHostSettings( EProtocolType i_protocolType, const ZS::Ipc::SServerHostSettings& i_settings ); 
public: // instance methods (on passing Count or Undefined take all protocol and/or socket types into account)
    void setHostSettings2Default( EProtocolType i_protocolType = EProtocolTypeUndefined, ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeUndefined ); // The request method "updateHostSettings" need to be called afterwards.
public: // instance methods (on passing Count or Undefined take all protocol and/or socket types into account)
    QHash<SCnctId, ZS::Ipc::SSocketDscr> getClientCncts( EProtocolType i_protocolType = EProtocolTypeUndefined, ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeUndefined ) const;
    QList<SCnctId> getClientCnctIds( EProtocolType i_protocolType = EProtocolTypeUndefined, ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeUndefined ) const;
    QList<ZS::Ipc::SSocketDscr> getClientCnctDscrs( EProtocolType i_protocolType = EProtocolTypeUndefined, ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeUndefined ) const;
    int getClientCnctsCount( EProtocolType i_protocolType = EProtocolTypeUndefined, ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeUndefined ) const;
    SCnctId getClientCnctId( int i_idxCnct, EProtocolType i_protocolType = EProtocolTypeUndefined, ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeUndefined ) const;
    ZS::Ipc::SSocketDscr getClientCnctDscr( int i_idxCnct, EProtocolType i_protocolType = EProtocolTypeUndefined, ZS::Ipc::ESocketType i_socketType = ZS::Ipc::ESocketTypeUndefined ) const;
public: // instance methods
    EState getState() const;
    QString state2Str() const;
public: // instance methods
    bool isListening( EProtocolType i_protocolType, ZS::Ipc::ESocketType i_socketType ) const;
public: // instance methods
    bool isConnected( EProtocolType i_protocolType, ZS::Ipc::ESocketType i_socketType, int i_iSocketId ) const;
    ZS::System::SErrResultInfo checkSocketId( EProtocolType i_protocolType, ZS::Ipc::ESocketType i_socketType, int i_iSocketId ) const;
public: // instance methods
    bool isBusy() const;
    bool isRequestInProgress( int i_iRequestId = -1, ERequest i_request = ERequestUndefined, bool i_bIgnorePostponedRequests = false ) const;      // -1 and Undefined to check whether any request is in progress
    ERequest getRequestInProgress() const;
    QString requestInProgress2Str( bool i_bShort = true ) const;
public: // instance methods
    void abortRequest( qint64 i_iRequestId );
    void abortRequestInProgress();
    void abortAllRequests();
public: // asynchronous request methods (may be queued)
    virtual ZS::System::CRequest* startup(
        int    i_iTimeout_ms  = 0,
        bool   i_bWait        = false,
        qint64 i_iReqIdParent = -1 );
    virtual ZS::System::CRequest* shutdown(
        int    i_iTimeout_ms  = 0,
        bool   i_bWait        = false,
        qint64 i_iReqIdParent = -1 );
public: // overridables
    // Need to be called after changing settings to apply the settings:
    virtual ZS::System::CRequest* updateHostSettings(
        int    i_iTimeout_ms  = 0,
        bool   i_bWait        = false,
        qint64 i_iReqIdParent = -1 );
public: // instance methods
    void addTrcMsgLogObject( QObject* i_pObj );
    void removeTrcMsgLogObject( QObject* i_pObj );
public: // instance methods
    CDataSet* getDataSet() const { return m_pDS; }
public: // overridables
    virtual void createDataSet();
protected: // overridables
    virtual void executeNextPostponedRequest();
protected: // overridables
    virtual void executeStartupRequest( ZS::System::CRequest* i_pReq );
    virtual void executeShutdownRequest( ZS::System::CRequest* i_pReq );
    virtual void executeUpdateHostSettingsRequest( ZS::System::CRequest* i_pReq );
protected: // overridables (auxiliary methods)
    virtual void createGatewayThreads();
    virtual void destroyGatewayThreads();
protected: // overridables (auxiliary methods)
    virtual ZS::System::CRequest* startGatewayThreads( int i_iTimeout_ms = 10000, qint64 i_iReqIdParent = -1 );
    virtual ZS::System::CRequest* stopGatewayThreads( int i_iTimeout_ms = 10000, qint64 i_iReqIdParent = -1 );
protected: // overridables (auxiliary methods)
    virtual ZS::System::CRequest* startupGateways( int i_iTimeout_ms = 10000, qint64 i_iReqIdParent = -1 );
    virtual ZS::System::CRequest* shutdownGateways( int i_iTimeout_ms = 10000, qint64 i_iReqIdParent = -1 );
protected: // auxiliary methods
    void acceptChangedSettings();
protected slots:
    virtual void onRequestTimeout();
protected slots:
    virtual void onRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots:
    virtual void onDSDestroyed( QObject* i_pDS );
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pMsg );
protected: // instance members
    QMutex*                                          m_pMtx;
    TFctCreateDataSet                                m_fctCreateDataSet;
    QList<TFctAddDataSetObjects>                     m_arFctAddDataSetObjects;
    CDataSet*                                        m_pDS;
    // Supported protocol and connection types
    QList<EProtocolType>                             m_arProtocolTypes;         // Only protocol types ZSXML and ZSMsg is supported yet (implementing a SQL parser is not that simple).
    QHash<EProtocolType,QList<ZS::Ipc::ESocketType>> m_hsharSocketTypes;        // As default only protocol types ZSXML and ZSMsg and connection types Tcp and InProcMsg are supported.
    QHash<SCnctType,ZS::Ipc::SServerHostSettings>    m_hshHostSettingsDefault;
    QHash<SCnctType,ZS::Ipc::SServerHostSettings>    m_hshHostSettingsOrig;
    QHash<SCnctType,ZS::Ipc::SServerHostSettings>    m_hshHostSettingsCurr;
    // Connected and registered clients
    QHash<SCnctType,CDbGatewayThread*>               m_hshpGatewayThreads;      // Key is built of protocol type and connection type.
    QHash<SCnctType,CDbGateway*>                     m_hshpGateways;            // Key is built of protocol type and connection type.
    QHash<SCnctId,Ipc::SSocketDscr>                  m_hshDbCltsConnected;      // Hash of connected clients. Key is built of protocol type and connection type and - depending on connection type - either socket id or pointer to client object 
    QHash<SCnctId,SDbClientRegSpec>                  m_hshDbCltsRegSpecs;       // Hash of registered clients. Key is built of protocol type, connection type and - depending on connection type - either socket id or pointer to client object 
    //QHash<SCnctId,CChangeDataSet*>                 m_hsharpDbCltsChgDS;       // For each registered client a change data set is updated with database changes. Key is built of protocol type, connection type and - depending on connection type - either socket id or pointer to client object 
    // State machine
    EState                                           m_state;
    ZS::System::CRequestQueue*                       m_pRequestQueue;
    ZS::System::SRequestDscr                         m_reqDscrTimeout;
    QTimer*                                          m_pTmrReqTimeout;
    bool                                             m_bMsgReqContinuePending;
    bool                                             m_bIsBeingDestroyed;
    // Tracing
    QList<QObject*>                                  m_arpTrcMsgLogObjects;
    ZS::Trace::CTrcAdminObj*                         m_pTrcAdminObj;

}; // class CDb

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSDatabase_h
