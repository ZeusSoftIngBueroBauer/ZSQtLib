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

#ifndef ZSApps_RemCmdClient_App_h
#define ZSApps_RemCmdClient_App_h

#include <QtCore/qglobal.h>

#include "ZSSysGUI/ZSSysGUIApp.h"
#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSIpc/ZSIpcBlkType.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysRequestQueue.h"

class QSettings;

namespace ZS
{
namespace System
{
class CErrLog;
class CRequestExecTree;
}

namespace Trace
{
class CIpcTrcServer;
class CTrcAdminObj;
}

namespace RemCmd
{
class CRemCmdClient;
}

namespace Apps
{
namespace Products
{
namespace RemCmdClient
{
class CMainWindow;

//******************************************************************************
class CApplication : public ZS::System::GUI::CGUIApp
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static CApplication* GetInstance();
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
    ZS::RemCmd::CRemCmdClient* getRemCmdClient() { return m_pRemCmdClient; }
public: // instance methods
    void readSettings();
    void saveSettings();
protected slots:
    void shutdown();
protected slots:
    void onLastWindowClosed();
protected: // instance members
    QSettings*                    m_pSettingsFile;
    // State Machine
    bool                          m_bReqExecTreeGarbageCollectorEnabled;
    double                        m_fReqExecTreeGarbageCollectorInterval_s;
    double                        m_fReqExecTreeGarbageCollectorElapsed_s;
    ZS::System::CRequestExecTree* m_pReqExecTree;
    // Trace Server
    bool                          m_bTrcServerStartTcpServer;
    ZS::Ipc::SServerHostSettings  m_trcServerHostSettings;
    ZS::Trace::STrcServerSettings m_trcServerSettings;
    ZS::Trace::CIpcTrcServer*     m_pTrcServer;
    // Remote Command Client
    ZS::Ipc::SClientHostSettings  m_remCmdClientHostSettings;
    ZS::Ipc::STimerSettings       m_remCmdClientWatchDogSettings;
    ZS::Ipc::EBlkType             m_blkTypeRemCmdClient;
    ZS::RemCmd::CRemCmdClient*    m_pRemCmdClient;
    // Main Window
    CMainWindow*                  m_pMainWindow;
    // Method Trace
    ZS::Trace::CTrcAdminObj*      m_pTrcAdminObj;

}; // class CApplication

} // namespace RemCmdClient

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_RemCmdClient_App_h
