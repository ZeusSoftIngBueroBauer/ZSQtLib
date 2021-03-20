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

#ifndef ZSApps_TestAsyncRequests_Database_h
#define ZSApps_TestAsyncRequests_Database_h

#include <QtCore/qstring.h>
#include <QtCore/qthread.h>

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcCommon.h"
#include "ZSSys/ZSSysRequestQueue.h"

class QMutex;
class QTimer;

namespace ZS
{
namespace System
{
class CRequest;
class CRequestSequencer;
}

namespace Trace
{
class CTrcAdminObj;
}

namespace Apps
{
namespace Test
{
namespace AsyncRequests
{
class CDbGateway;
class CDbGatewayThread;
class CMsgReqConnect;
class CMsgReqDisconnect;
class CMsgReqRegister;
class CMsgReqUnregister;
class CMsgReqSelectSchema;
class CMsgReqUpdateSchema;
class CMsgReqSelectData;
class CMsgReqUpdateData;
class CMsgIndConnected;
class CMsgIndDisconnected;

struct SDbCltSocketDscr;

/*******************************************************************************
Type definitions and constants
*******************************************************************************/

namespace Database
{
//==============================================================================
enum EState {
//==============================================================================
    EStateCreated      = 0,
    EStateRunning      = 1,
    EStateDisconnected = 2,
    EStateConnected    = 3,
    EStateRegistered   = 4,
    EStateCount
};
QString State2Str( int i_iState );

//==============================================================================
enum ERequest {
//==============================================================================
    ERequestNone              =  0,
    ERequestStartup           =  1,
    ERequestShutdown          =  2,
    ERequestReadSchema        =  3,
    ERequestSaveSchema        =  4,
    ERequestReadData          =  5,
    ERequestSaveData          =  6,
    ERequestSet2Default       =  7,
    ERequestChangeSettings    =  8,
    ERequestConnect           =  9,
    ERequestDisconnect        = 10,
    ERequestRegister          = 11,
    ERequestUnregister        = 12,
    ERequestSelectSchema      = 13,
    ERequestUpdateSchema      = 14,
    ERequestSelectData        = 15,
    ERequestUpdateData        = 16,
    // Internal Sequencer Requests (Child Requests)
    ERequestStartThread       = 17,
    ERequestStopThread        = 18,
    ERequestDbStartup         = 19,
    ERequestDbShutdown        = 20,
    ERequestDbConnect         = 21,
    ERequestDbDisconnect      = 22,
    ERequestDbUpdateSchema    = 23,
    ERequestDbCltUpdateSchema = 24,
    ERequestDbUpdateData      = 25,
    ERequestDbCltUpdateData   = 26,
    ERequestCount,
    ERequestUndefined
};
QString Request2Str( int i_iRequest );

//==============================================================================
enum ETimeout {
//==============================================================================
    ETimeoutInfinite,
    ETimeoutShort,
    ETimeoutMedium,
    ETimeoutLong,
    ETimeoutCount,
    ETimeoutUndefined
};

const int c_iTimeoutInfinite_ms =    -1;
const int c_iTimeoutShort_ms    =   200;
const int c_iTimeoutMedium_ms   =  2000;
const int c_iTimeoutLong_ms     = 20000;

QString timeout2Str( int i_iTimeout );
ETimeout str2Timeout( const QString& i_str );
int getTimeoutInMs( int i_iTimeout );
int getTimeoutInMs( const QString& i_str );

} // namespace Database

//******************************************************************************
class CDb : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::AsyncRequests::Database"; }
    static QString ClassName() { return "CDb"; }
public: // class methods (request sequencer execution functions)
    static ZS::System::CRequest* StartGatewayThread( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, QVariant& i_valExec );
    static ZS::System::CRequest* StopGatewayThread( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, QVariant& i_valExec );
    static ZS::System::CRequest* StartupGateway( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, QVariant& i_valExec );
    static ZS::System::CRequest* ShutdownGateway( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, QVariant& i_valExec );
public: // class methods (startup request sequencer execution functions)
    static ZS::System::CRequest* ReadSchema( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* ReadData( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* Set2Default( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
public: // class methods (shutdown request sequencer execution functions)
    static ZS::System::CRequest* SaveSchema( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* SaveData( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
public: // class methods (execute commands request sequencer execution functions)
    static ZS::System::CRequest* UpdateDbSchema( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* UpdateDbCltSchema( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
public: // class methods (execute commands request sequencer execution functions)
    static ZS::System::CRequest* UpdateDbData( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* UpdateDbCltData( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
public: // ctors and dtor
    CDb( const QString& i_strDbObjName );
    virtual  ~CDb();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
signals:
    void stateChanged( int i_iStateCurr, int i_iStatePrev );
public: // instance methods
    void setReqModeSingleShot( bool i_bModeSingleShot );
    bool isReqModeSingleShot() const { return m_bReqModeSingleShot; }
    void executeReqSingleShot();
public: // instance methods
    ZS::System::CRequest* startup( qint64 i_iReqIdParent = -1 );
    ZS::System::CRequest* shutdown( qint64 i_iReqIdParent = -1 );
public: // instance methods
    ZS::System::CRequest* readSchema( qint64 i_iReqIdParent = -1 );
    ZS::System::CRequest* saveSchema( qint64 i_iReqIdParent = -1 );
public: // instance methods
    ZS::System::CRequest* readData( qint64 i_iReqIdParent = -1 );
    ZS::System::CRequest* saveData( qint64 i_iReqIdParent = -1 );
public: // instance methods
    ZS::System::CRequest* set2Default( qint64 i_iReqIdParent = -1 );
protected: // instance methods
    ZS::System::CRequest* onReceivedMsgReqConnect( CMsgReqConnect* i_pMsgReq );
    ZS::System::CRequest* onReceivedMsgReqDisconnect( CMsgReqDisconnect* i_pMsgReq );
protected: // instance methods
    ZS::System::CRequest* onReceivedMsgReqRegister( CMsgReqRegister* i_pMsgReq );
    ZS::System::CRequest* onReceivedMsgReqUnregister( CMsgReqUnregister* i_pMsgReq );
protected: // instance methods
    ZS::System::CRequest* onReceivedMsgReqSelectSchema( CMsgReqSelectSchema* i_pMsgReq );
    ZS::System::CRequest* onReceivedMsgReqUpdateSchema( CMsgReqUpdateSchema* i_pMsgReq );
protected: // instance methods
    ZS::System::CRequest* onReceivedMsgReqSelectData( CMsgReqSelectData* i_pMsgReq );
    ZS::System::CRequest* onReceivedMsgReqUpdateData( CMsgReqUpdateData* i_pMsgReq );
protected: // instance methods
    void onReceivedMsgIndConnected( CMsgIndConnected* i_pMsgInd );
    void onReceivedMsgIndDisconnected( CMsgIndDisconnected* i_pMsgInd );
protected: // overridables (state machine)
    virtual void executeNextPostponedRequest();
protected: // instance methods
    void executeStartupRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
    void executeShutdownRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
protected: // instance methods
    void executeReadSchemaRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
    void executeSaveSchemaRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
protected: // instance methods
    void executeReadDataRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
    void executeSaveDataRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
protected: // instance methods
    void executeSet2DefaultRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
protected: // instance methods
    void executeConnectRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
    void executeDisconnectRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
protected: // instance methods
    void executeRegisterRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
    void executeUnregisterRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
protected: // instance methods
    void executeSelectSchemaRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
    void executeUpdateSchemaRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
protected: // instance methods
    void executeSelectDataRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
    void executeUpdateDataRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
protected slots: // instance methods
    void onExecuteReqSingleShotTimeout();
protected: // internal request sequencer methods
    ZS::System::CRequest* startGatewayThread( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, ZS::Ipc::ESocketType i_socketType );
    ZS::System::CRequest* stopGatewayThread( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, ZS::Ipc::ESocketType i_socketType );
    ZS::System::CRequest* startupGateway( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, ZS::Ipc::ESocketType i_socketType );
    ZS::System::CRequest* shutdownGateway( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, ZS::Ipc::ESocketType i_socketType );
protected: // internal request sequencer methods
    ZS::System::CRequest* updateDbSchema( qint64 i_iReqIdParent );
    ZS::System::CRequest* updateDbCltSchema( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, SDbCltSocketDscr* i_pDbCltSocketDscr );
protected: // internal request sequencer methods
    ZS::System::CRequest* updateDbData( qint64 i_iReqIdParent );
    ZS::System::CRequest* updateDbCltData( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, SDbCltSocketDscr* i_pDbCltSocketDscr );
protected slots:
    void onRequestTimeout();
protected slots:
    void onRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pEv );
private: // instance members
    // Direct message connections to clients (message dispatch):
    QList<QObject*>                         m_arpDbCltsConnected;
    QList<QObject*>                         m_arpDbCltsRegistered;
    // Connections to clients via gateways (e.g. through TCP/IP sockets):
    QVector<Ipc::SServerHostSettings>       m_arHostSettingsGtw;
    QVector<CDbGatewayThread*>              m_arpDbGtwThread;
    QVector<CDbGateway*>                    m_arpDbGtw;
    QVector<QHash<int,Ipc::SSocketDscr>>    m_arhshSocketDscrsDbCltsConnected;  // Hash of connected clients at the corresponding gateway. Key is socket id.
    QVector<QList<int>>                     m_arariSocketIdsDbCltsRegistered;
    // Requests:
    ZS::System::CRequestSequencer*          m_pReqSeqrStartup;
    ZS::System::CRequestSequencer*          m_pReqSeqrShutdown;
    ZS::System::CRequestSequencer*          m_pReqSeqrExecCmd;
    ZS::System::CRequestQueue               m_requestQueue;
    ZS::System::SRequestDscr                m_reqDscrTimeout;
    QTimer*                                 m_pTmrReqTimeout;
    bool                                    m_bMsgReqContinuePending;
    // Simulating asynchronous requests:
    Database::ERequest                      m_reqSimInProgress; // Request which is currently simulated. Range [None, Connect, Disconnect, SelectSchema, .., Set2Default] but !! NOT !! Startup and Shutdown
    int                                     m_iReqSimDurationMin_ms;
    int                                     m_iReqSimDurationMax_ms;
    int                                     m_iReqSimDurationStepsMax;
    int                                     m_iReqSimDurationStepsCurr;
    bool                                    m_bReqModeSingleShot;
    bool                                    m_bReqSingleShot;
    ZS::Trace::CTrcAdminObj*                m_pTrcAdminObj;

}; // class CDb

} // namespace AsyncRequests

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestAsyncRequests_Database_h
