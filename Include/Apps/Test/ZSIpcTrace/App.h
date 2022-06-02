/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSApps_TestIpcTrace_App_h
#define ZSApps_TestIpcTrace_App_h

#include <QtCore/qglobal.h>

#include "ZSTest/ZSTestDllMain.h"
#include "ZSSysGUI/ZSSysGUIApp.h"
#include "ZSIpcTrace/ZSIpcTrcDllMain.h"
#include "ZSIpcTrace/ZSIpcTrcClient.h"
#include "ZSIpcTrace/ZSIpcTrcServer.h"

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
class CIpcTrcClient;
}

namespace Apps
{
namespace Test
{
namespace IpcTrace
{
class CMainWindow;
class CTest;

//******************************************************************************
class CApplication : public ZS::System::GUI::CGUIApp
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString ClassName( bool i_bIncludeNameSpace = false );
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
    CTest* getTest() { return m_pTest; }
    ZS::Trace::CIpcTrcServer* getTrcServer() { return m_pZSTrcServer; }
    ZS::Trace::CIpcTrcClient* getTrcClient() { return m_pZSTrcClient; }
    ZS::Trace::ETraceDetailLevelMethodCalls getTraceDetailLevelTrcServer() const { return m_eZSTrcServerTrcDetailLevel; }
    ZS::Trace::ETraceDetailLevelMethodCalls getTraceDetailLevelTrcClient() const { return m_eZSTrcClientTrcDetailLevel; }
protected slots:
    void onTestFinished( const ZS::Test::CEnumTestResult& i_result );
private: // instance members
    bool                                    m_bReqExecTreeGarbageCollectorEnabled;
    double                                  m_fReqExecTreeGarbageCollectorInterval_s;
    double                                  m_fReqExecTreeGarbageCollectorElapsed_s;
    ZS::System::CRequestExecTree*           m_pReqExecTree;
    ZS::Trace::ETraceDetailLevelMethodCalls m_eZSTrcServerTrcDetailLevel;
    ZS::Trace::ETraceDetailLevelMethodCalls m_eZSTrcClientTrcDetailLevel;
    ZS::Trace::CIpcTrcServer*               m_pZSTrcServer;
    ZS::Ipc::SClientHostSettings            m_clientHostSettingsZSTrcClient;
    ZS::Trace::CIpcTrcClient*               m_pZSTrcClient;
    CTest*                                  m_pTest;
    CMainWindow*                            m_pMainWindow;
    bool                                    m_bAutoStartTest;

}; // class CApplication

} // namespace IpcTrace

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcTrace_App_h
