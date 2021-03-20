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

/*==============================================================================

Thread pool:
------------

                              +- Thread ------------+
                              |                     |
            +-----------------+ RemCmdServerGateway |
            |                 |                     |
+- Thread --+-----------+     +----------+----------+     +- Thread ------------+
|                       |                |                |                     |
| GUIMain               |                |                | SigGen              |
|                       |                |                |                     |
| - RemCmdServer        |     +- Thread -+----------+     +-----------+---------+
| - DatabaseThread      |     |                     |                 |
| - SigGenThread        +-----+ Database            |-----------------+
| - SigAnaThread        |     |                     |                 |
|                       |     +---------------------+     +- Thread --+---------+
| - TestModule1         |                                 |                     |
|   - TestModule2Thread |                                 | SigAna              |
|                       |                                 |                     |
+-----------+-----------+     +- Thread ------------+     +---------------------+
            |                 |                     |
            +-----------------+ TestModule2         |
                              |                     |
                              +---------------------+


From within the GUIMainThread (by this application instance)

- the RemCmdServer,
- the DatabaseThread

are created. On starting the DatabaseThread the Database instance is
created within the run method of the thread.

The RemCmdServer instance creates and starts a gateway thread in which the
Ipc server is created communicating with remote clients.

The SigGen- and the SigAna-Threads are also created by the GUIMainThread but
only after the Database instance has been created. On starting the SigGen-
and SigAna-Threads the SigGen- and SigAna- instances are created within the
thread's run methods.

Settings and measure values of the signal generator and signal analyzer
are not accessed directly but through database parameters. This applies for
both the GUI controls as well as for the remote command objects. Remote
command objects to read/write settings and measure values of the signal
generator or signal analyzer are therefore created within the context of
the database thread.

The signal analyzer queries the Y value of the signal generator by sending
a GetValue request message to the signal generator in the configured sampling
rate.

Communication and data exchange between objects living in different threads
is done by means of message transfer (request, confirmation, indication and
acknowledge messages).

Please note that the RemCmdServer object also communicates with the
RemCmdObjects living in different thread by means of message transfer. E.g.
to execute a method within the database a message will be sent from the
RemCmdServer to the corresponding RemCmdObject living in the database thread.
This ensures that methods will be called from within the thread context in
which the RemCmdObjects have been created.

TestModule1 is created in the GUIMainThread context on clicking on the
corresponding button within the test widget or by executing the remote
command object "Module1::Create".

TestModule2 is created in the TestModule2Thread context on clicking on the
corresponding button within the test widget or by executing the remote command
object "Module2::Create". The TestModule2Thread object is created as a child
of TestModule1 on calling the startup method of TestModule1.

==============================================================================*/

#ifndef ZSApps_TestRemCmdServer_App_h
#define ZSApps_TestRemCmdServer_App_h

#include <QtCore/qglobal.h>

#include "ZSSysGUI/ZSSysGUIApp.h"
#include "ZSIpc/ZSIpcBlkType.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysRequestQueue.h"
#include "ZSSys/ZSSysTrcServer.h"

class QSettings;

namespace ZS
{
namespace System
{
class CErrLog;
class CRequest;
class CRequestExecTree;
struct SErrResultInfo;
}
namespace Ipc
{
struct SSocketDscr;
}
namespace Trace
{
class CTrcAdminObj;
class CIpcTrcServer;
}

namespace RemCmd
{
class CRemCmdServer;
class CRemCmdObj;
}

namespace Apps
{
namespace Test
{
namespace RemCmdServer
{
class CDatabase;
class CDatabaseThread;
class CMainWindow;
class CSigGen;
class CSigGenThread;
class CSigAna;
class CSigAnaThread;
class CTestModule1;
class CTestModule2Thread;
class CTestModule2;

//******************************************************************************
class CApplication : public ZS::System::GUI::CGUIApp
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    enum EState {
        EStateIdle                     =  0,
        EStateStartingRemCmdServer     =  1,
        EStateShuttingDownRemCmdServer =  2,
        EStateStartingDbThread         =  3,
        EStateShuttingDownDbThread     =  4,
        EStateStartingSigGenThread     =  5,
        EStateShuttingDownSigGenThread =  6,
        EStateStartingSigAnaThread     =  7,
        EStateShuttingDownSigAnaThread =  8,
        EStateCreatingModule1          =  9,
        EStateDeletingModule1          = 10,
        EStateCreatingModule2          = 11,
        EStateDeletingModule2          = 12,
        EStateCount,
        EStateUndefined
    };
    static QString State2Str( int i_state );
    enum ERequest {
        ERequestNone     = 0,
        ERequestStartup  = 1,
        ERequestShutdown = 2,
        ERequestCount,
        ERequestUndefined
    };
    static QString Request2Str( int i_request );
public: // class methods
    static CApplication* GetInstance();
public: // class methods (remote commands)
    static ZS::System::CRequest* OnExecuteRemCmdCreateModule1(
        QObject*                    i_pObjFctExecute,
        const ZS::Ipc::SSocketDscr& i_socketDscr,
        bool                        i_bIsQuery,
        const QByteArray&           i_byteArrArgs,
        QByteArray&                 o_byteArrResults,
        qint64                      i_iReqIdParent );
    static ZS::System::CRequest* OnExecuteRemCmdDeleteModule1(
        QObject*                    i_pObjFctExecute,
        const ZS::Ipc::SSocketDscr& i_socketDscr,
        bool                        i_bIsQuery,
        const QByteArray&           i_byteArrArgs,
        QByteArray&                 o_byteArrResults,
        qint64                      i_iReqIdParent );
    static ZS::System::CRequest* OnExecuteRemCmdCreateModule2(
        QObject*                    i_pObjFctExecute,
        const ZS::Ipc::SSocketDscr& i_socketDscr,
        bool                        i_bIsQuery,
        const QByteArray&           i_byteArrArgs,
        QByteArray&                 o_byteArrResults,
        qint64                      i_iReqIdParent );
    static ZS::System::CRequest* OnExecuteRemCmdDeleteModule2(
        QObject*                    i_pObjFctExecute,
        const ZS::Ipc::SSocketDscr& i_socketDscr,
        bool                        i_bIsQuery,
        const QByteArray&           i_byteArrArgs,
        QByteArray&                 o_byteArrResults,
        qint64                      i_iReqIdParent );
public: // ctors and dtor
    CApplication(
        int            i_argc,
        char*          i_argv[],
        const QString& i_strOrganizationName,
        const QString& i_strOrganizationDomain,
        const QString& i_strAppName,
        const QString& i_strWindowTitle );
    ~CApplication();
public: // instance methods
    QSettings* getSettingsFile() const { return m_pSettingsFile; }
public: // instance methods
    void readSettings();
    void saveSettings();
public: // instance methods
    CTestModule1* getTestModule1() const { return m_pTestModule1; }
    CTestModule2* getTestModule2() const { return m_pTestModule2; }
    CTestModule2Thread* getTestModule2Thread() const { return m_pTestModule2Thread; }
public: // instance methods
    ZS::System::CRequest* createModule1( qint64 i_iReqIdParent = -1 );
    ZS::System::CRequest* deleteModule1( qint64 i_iReqIdParent = -1 );
public: // instance methods
    ZS::System::CRequest* createModule2( qint64 i_iReqIdParent = -1 );
    ZS::System::CRequest* deleteModule2( qint64 i_iReqIdParent = -1 );
public: // instance methods
    QString requestInProgress2Str() const;
public: // instance methods (system startup and system shutdown)
    void startup();
    void shutdown();
protected slots: // of system startup and system shutdown
    void onLastWindowClosed();
    void onStartupDbThread();
    void onStartupSigGenThread();
    void onStartupSigAnaThread();
protected slots:
    void onRemCmdServerPendingRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots:
    void onDbThreadPendingRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots:
    void onSigGenThreadPendingRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots:
    void onSigAnaThreadPendingRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots:
    void onTestModule1PendingRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots:
    void onObjectDestroyed( QObject* i_pObj );
private: // instance members
    QSettings*                          m_pSettingsFile;
    QString                             m_strErrLogFileAbsFilePath;
    // Trace Server
    bool                                m_bTrcServerStartTcpServer;
    ZS::Ipc::SServerHostSettings        m_trcServerHostSettings;
    ZS::Trace::STrcServerSettings       m_trcServerSettings;
    ZS::Trace::CIpcTrcServer*           m_pTrcServer;
    // Remote Command Server
    ZS::Ipc::SServerHostSettings        m_remCmdServerHostSettings;
    ZS::Ipc::EBlkType                   m_blkTypeRemCmdServer;
    ZS::RemCmd::CRemCmdServer*          m_pRemCmdServer;
    ZS::RemCmd::CRemCmdObj*             m_pRemCmdObjCreateModule1;
    ZS::RemCmd::CRemCmdObj*             m_pRemCmdObjDeleteModule1;
    ZS::RemCmd::CRemCmdObj*             m_pRemCmdObjCreateModule2;
    ZS::RemCmd::CRemCmdObj*             m_pRemCmdObjDeleteModule2;
    // Test Modules
    CDatabaseThread*                    m_pDbThread;
    CDatabase*                          m_pDb;
    CSigGenThread*                      m_pSigGenThread;
    CSigGen*                            m_pSigGen;
    CSigAnaThread*                      m_pSigAnaThread;
    CSigAna*                            m_pSigAna;
    CTestModule1*                       m_pTestModule1;
    CTestModule2Thread*                 m_pTestModule2Thread;
    CTestModule2*                       m_pTestModule2;
    // State Machine
    bool                                m_bReqExecTreeGarbageCollectorEnabled;
    double                              m_fReqExecTreeGarbageCollectorInterval_s;
    double                              m_fReqExecTreeGarbageCollectorElapsed_s;
    ZS::System::CRequestExecTree*       m_pReqExecTree;
    EState                              m_state;
    ZS::System::CRequest*               m_pReqInProgress;
    QHash<qint64,ZS::System::CRequest*> m_hshReqsInProgress;
    // Main Window
    CMainWindow*                        m_pMainWindow;
    // Method Trace
    ZS::Trace::CTrcAdminObj*            m_pTrcAdminObj;

}; // class CApplication

} // namespace RemCmdServer

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestRemCmdServer_App_h
