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

#ifndef ZSApps_TestAsynRequests_App_h
#define ZSApps_TestAsynRequests_App_h

#include <QtCore/qglobal.h>

#include "Database.h"

#include "ZSSysGUI/ZSSysGUIApp.h"
#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysRequestQueue.h"

class QSettings;

namespace ZS
{
namespace System
{
class CErrLog;
class CRequest;
class CRequestExecTree;
class CRequestSequencer;
}

namespace Trace
{
class CTrcAdminObj;
class CIpcTrcServer;
}

namespace Apps
{
namespace Test
{
namespace AsyncRequests
{
class CDbThread;
class CDbClient;
class CDbClientThread;
class CMainWindow;

const QString c_strObjNameApp          = "App";
const QString c_strObjNameDb           = "Db";
const QString c_strObjNameDbCltGUI     = "DbCltGUI";
const QString c_strObjNameDbCltSigGen  = "DbCltSigGen";
const QString c_strObjNameDbCltCapture = "DbCltCapture";

//******************************************************************************
class CApplication : public ZS::System::GUI::CGUIApp
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    enum EState {
        EStateCreated = 0,
        EStateRunning = 1,
        EStateCount
    };
    QString State2Str( int i_iState );
public: // type definitions and constants
    enum ERequest {
        ERequestNone                    =  0,
        ERequestStartup                 =  1,
        ERequestStartupTrcServer        =  2,
        ERequestStartupDb               =  3,
        ERequestShutdown                =  4,
        ERequestShutdownTrcServer       =  5,
        ERequestShutdownDb              =  6,
        ERequestConnect                 =  7,
        ERequestConnectDbClients        =  8,
        ERequestConnectDbClientGUI      =  9,
        ERequestStartupDbClientSigGen   = 10,
        ERequestConnectDbClientSigGen   = 11,
        ERequestStartupDbClientCapture  = 12,
        ERequestConnectDbClientCapture  = 13,
        ERequestDisconnect              = 14,
        ERequestDisconnectDbClients     = 15,
        ERequestDisconnectDbClientGUI   = 16,
        ERequestShutdownDbClientSigGen  = 17,
        ERequestShutdownDbClientCapture = 18,
        ERequestDeleteDbClients         = 19,
        ERequestDeleteDb                = 20,
        ERequestSelectSchema            = 21,
        ERequestSaveSchema              = 22,
        ERequestSelectData              = 23,
        ERequestSaveData                = 24,
        ERequestSet2Default             = 25,
        ERequestCount,
        ERequestUndefined
    };
    static QString Request2Str( int i_iRequest, bool i_bShort = false );
public: // class methods
    static CApplication* GetInstance();
public: // class methods (startup request execution functions)
    static ZS::System::CRequest* StartupDb( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* ConnectDbClientGUI( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* StartupDbClientSigGen( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* ConnectDbClientSigGen( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* StartupDbClientCapture( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* ConnectDbClientCapture( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
public: // class methods (shutdown request execution functions)
    static ZS::System::CRequest* DisconnectDbClientGUI( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* ShutdownDbClientSigGen( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* ShutdownDbClientCapture( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* ShutdownDb( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
public: // ctors and dtor
    CApplication(
        int            i_argc,
        char*          i_argv[],
        const QString& i_strOrganizationName,
        const QString& i_strOrganizationDomain,
        const QString& i_strAppName,
        const QString& i_strWindowTitle );
    ~CApplication();
signals:
    void stateChanged( int i_iStateCurr, int i_iStatePrev );
    void requestStarted( ZS::System::CRequest* i_pReq );
public: // instance methods
    EState state() const { return m_state; }
public: // instance methods
    CDbThread* getDbThread() const { return m_pDbThread; }
    CDbClient* getDbClientGUI() const { return m_pDbCltGUI; }
    CDbClientThread* getDbClientThreadSigGen() const { return m_pDbCltThreadSigGen; }
    CDbClientThread* getDbClientThreadCapture() const { return m_pDbCltThreadCapture; }
public: // instance methods
    void readSettings();
    void saveSettings();
public: // instance methods
    ZS::System::CRequest* startup();
    ZS::System::CRequest* shutdown();
public: // instance methods
    bool useBlockingMethodCalls( const QString& i_strObjName ) const;                               // ObjNames: [Db, DbClt<GUI|SigGen|Capture>]
    void setUseBlockingMethodCalls( bool i_bUse, const QString& i_strObjName );                     // ObjNames: [Db, DbClt<GUI|SigGen|Capture>]
public: // instance methods
    void setDbReqModeSingleShot( bool i_bModeSingleShot );
    bool isDbReqModeSingleShot() const { return m_bDbReqModeSingleShot; }
    void executeDbReqSingleShot();
public: // instance methods
    Database::ETimeout getTimeout( const QString& i_strObjName, const QString& i_strRequests = "" ) const;             // ObjNames: [Db, DbClt<GUI|SigGen|Capture>]
    void setTimeout( Database::ETimeout i_timeout, const QString& i_strObjName, const QString& i_strRequests = "" );   // ObjNames: [Db, DbClt<GUI|SigGen|Capture>]
public: // instance methods
    Ipc::ESocketType getSocketType( const QString& i_strObjName ) const;               // ObjNames: [Db, DbClt<GUI|SigGen|Capture>]
    void setSocketType( Ipc::ESocketType i_socketType, const QString& i_strObjName );    // ObjNames: [Db, DbClt<GUI|SigGen|Capture>]
public slots: // instance methods of system shutdown
    void onLastWindowClosed();
protected: // internal request sequencer methods
    ZS::System::CRequest* startupDb( qint64 i_iReqIdParent );
    ZS::System::CRequest* connectDbClientGUI( qint64 i_iReqIdParent );
    ZS::System::CRequest* startupDbClientSigGen( qint64 i_iReqIdParent );
    ZS::System::CRequest* connectDbClientSigGen( qint64 i_iReqIdParent );
    ZS::System::CRequest* startupDbClientCapture( qint64 i_iReqIdParent );
    ZS::System::CRequest* connectDbClientCapture( qint64 i_iReqIdParent );
protected: // internal request sequencer methods
    ZS::System::CRequest* disconnectDbClientGUI( qint64 i_iReqIdParent );
    ZS::System::CRequest* shutdownDbClientSigGen( qint64 i_iReqIdParent );
    ZS::System::CRequest* shutdownDbClientCapture( qint64 i_iReqIdParent );
    ZS::System::CRequest* deleteDbClients( qint64 i_iReqIdParent );
    ZS::System::CRequest* shutdownDb( qint64 i_iReqIdParent );
    ZS::System::CRequest* deleteDb( qint64 i_iReqIdParent );
protected slots: // internal request sequencer methods
    void onStartupFinished( qint64 i_iReqId );
    void onShutdownFinished( qint64 i_iReqId );
protected slots:
    void onRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots:
    void onDbCreated( QObject* i_pDb );
protected: // instance methods (state machine)
    void executeNextPostponedRequest();
protected: // instance methods
    void executeStartupRequest( ZS::System::CRequest* i_pReq );
    void executeShutdownRequest( ZS::System::CRequest* i_pReq );
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pEv );
private: // instance members
    QSettings*                     m_pSettingsFile;
    QString                        m_strErrLogFileAbsFilePath;
    bool                           m_bReqExecTreeGarbageCollectorEnabled;
    double                         m_fReqExecTreeGarbageCollectorInterval_s;
    double                         m_fReqExecTreeGarbageCollectorElapsed_s;
    ZS::System::CRequestExecTree*  m_pReqExecTree;
    EState                         m_state;
    ZS::System::CRequestSequencer* m_pReqSeqrStartup;
    ZS::System::CRequestSequencer* m_pReqSeqrShutdown;
    ZS::System::CRequestQueue      m_requestQueue;
    bool                           m_bMsgReqContinuePending;
    CMainWindow*                   m_pMainWindow;
    bool                           m_bDbUseBlockingMethodCalls;
    bool                           m_bDbReqModeSingleShot;
    Database::ETimeout             m_timeoutDb;
    CDbThread*                     m_pDbThread;
    bool                           m_bDbCltGUIUseBlockingMethodCalls;
    Ipc::ESocketType               m_socketTypeDbCltGUI;
    Database::ETimeout             m_timeoutDbCltGUIConnect;
    Database::ETimeout             m_timeoutDbCltGUISchema;
    Database::ETimeout             m_timeoutDbCltGUIData1;
    Database::ETimeout             m_timeoutDbCltGUIData2;
    Database::ETimeout             m_timeoutDbCltGUIData3;
    CDbClient*                     m_pDbCltGUI;
    bool                           m_bDbCltSigGenUseBlockingMethodCalls;
    Ipc::ESocketType               m_socketTypeDbCltSigGen;
    Database::ETimeout             m_timeoutDbCltSigGenStartup;
    Database::ETimeout             m_timeoutDbCltSigGenConnect;
    CDbClientThread*               m_pDbCltThreadSigGen;
    bool                           m_bDbCltCaptureUseBlockingMethodCalls;
    Ipc::ESocketType               m_socketTypeDbCltCapture;
    Database::ETimeout             m_timeoutDbCltCaptureStartup;
    Database::ETimeout             m_timeoutDbCltCaptureConnect;
    CDbClientThread*               m_pDbCltThreadCapture;
    ZS::Ipc::SServerHostSettings   m_trcServerHostSettings;
    ZS::Trace::STrcServerSettings  m_trcServerSettings;
    ZS::Trace::CIpcTrcServer*      m_pTrcServer;
    ZS::Trace::CTrcAdminObj*       m_pTrcAdminObj;

}; // class CApplication

} // namespace AsyncRequests

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestAsynRequests_App_h
