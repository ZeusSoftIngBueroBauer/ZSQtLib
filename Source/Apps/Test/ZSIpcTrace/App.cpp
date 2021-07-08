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

#include <QtCore/qdir.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qsettings.h>
#include <QtCore/qthread.h>
#include <QtCore/qtimer.h>
#include <QtGui/qbitmap.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qmessagebox.h>
#else
#include <QtWidgets/qmessagebox.h>
#endif

#include "App.h"
#include "MainWindow.h"
#include "Test.h"

#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSIpcTrace/ZSIpcTrcClient.h"
#include "ZSTest/ZSTestStepAdminObjPool.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::IpcTrace;


/*******************************************************************************
class CApplication : public ZS::System::GUI::CGUIApp
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CApplication::ClassName( bool i_bIncludeNameSpace )
//------------------------------------------------------------------------------
{
    QString strClassName;

    if( i_bIncludeNameSpace )
    {
        strClassName = "ZS::Apps::Test::IpcTrace::CApplication";
    }
    else
    {
        strClassName = "CApplication";
    }
    return strClassName;

} // ClassName

//------------------------------------------------------------------------------
CApplication* CApplication::GetInstance()
//------------------------------------------------------------------------------
{
    return dynamic_cast<CApplication*>(QApplication::instance());
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CApplication::CApplication(
    int            i_argc,
    char*          i_argv[],
    const QString& i_strOrganizationName,
    const QString& i_strOrganizationDomain,
    const QString& i_strAppName,
    const QString& i_strWindowTitle ) :
//------------------------------------------------------------------------------
    CGUIApp(i_argc,i_argv),
    m_bReqExecTreeGarbageCollectorEnabled(true),
    m_fReqExecTreeGarbageCollectorInterval_s(5.0),
    m_fReqExecTreeGarbageCollectorElapsed_s(60.0),
    m_pReqExecTree(nullptr),
    m_trcServerHostSettings(
        /* strLocalHostName */ "127.0.0.1",
        /* uLocalPort       */ 24763,
        /* uMaxConnections  */ 30 ),
    m_trcServerSettings(),
    m_pTrcServer(nullptr),
    m_iTrcDetailLevelTrcServer(ETraceDetailLevelNone),
    m_trcClientHostSettings(
        /* strRemoteHost      */ "127.0.0.1",
        /* uRemotePort        */ 24763,
        /* iConnectTimeout_ms */ 5000 ),
    m_pTrcClient(nullptr),
    m_strThreadClrFileAbsFilePath(),
    m_strTestStepsFileAbsFilePath(),
    m_pTest(nullptr),
    m_pMainWindow(nullptr)
{
    m_trcServerHostSettings = SServerHostSettings(ZS::Ipc::ESocketTypeTcp);

    m_trcServerHostSettings.m_uLocalPort = 24763;
    m_trcServerHostSettings.m_uMaxPendingConnections = 5000;

    m_trcServerSettings = STrcServerSettings();

    m_trcServerSettings.m_bLocalTrcFileCloseFileAfterEachWrite = true;

    setObjectName("theApp");

    if( thread()->objectName().length() == 0 )
    {
        thread()->setObjectName("GUIMain");
    }

    QCoreApplication::setOrganizationName(i_strOrganizationName);
    QCoreApplication::setOrganizationDomain(i_strOrganizationDomain);
    QCoreApplication::setApplicationName(i_strAppName);

    QIcon iconApp;

    QPixmap pxmApp16x16(":/ZS/App/Zeus16x16.bmp");
    QPixmap pxmApp32x32(":/ZS/App/Zeus32x32.bmp");

    pxmApp16x16.setMask(pxmApp16x16.createHeuristicMask());
    pxmApp32x32.setMask(pxmApp32x32.createHeuristicMask());

    iconApp.addPixmap(pxmApp16x16);
    iconApp.addPixmap(pxmApp32x32);

    QApplication::setWindowIcon(iconApp);

    // Parse command arguments
    //------------------------

    int         idxArg;
    QString     strArg;
    QString     strVal;
    int         iVal;
    bool        bConverted;
    QStringList strListArgsPar;
    QStringList strListArgsVal;

    // Range of IniFileScope: ["AppDir", "User", "System"]
    #ifdef __linux__
    // Using "System" on linux Mint ends up in directory "etc/xdg/<CompanyName>"
    // where the application has not write access rights. Stupid ...
    QString strIniFileScope = "User";
    #else
    QString strIniFileScope = "System"; // Default
    #endif

    parseAppArgs( i_argc, i_argv, strListArgsPar, strListArgsVal );

    #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
    for( idxArg = 0; idxArg < strListArgsPar.length() && idxArg < strListArgsVal.length(); idxArg++ )
    #else
    for( idxArg = 0; idxArg < strListArgsPar.size() && idxArg < strListArgsVal.size(); idxArg++ )
    #endif
    {
        strArg = strListArgsPar[idxArg];
        strVal = strListArgsVal[idxArg];

        // Here only the command arguments concerning the location of the ini file are parsed.
        // Other arguments (e.g. mode) are parsed further below.
        if( strArg.compare("IniFileScope",Qt::CaseInsensitive) == 0 )
        {
            strIniFileScope = strVal;
        }
        else if( strArg == "TrcServerTraceDetailLevel" )
        {
            m_iTrcDetailLevelTrcServer = str2TraceDetailLevel(strVal);
        }
        else if( strArg == "TrcServerSettingsTrcFileLineCountMax" )
        {
            iVal = strVal.toInt(&bConverted);
            if( bConverted ) m_trcServerSettings.m_iLocalTrcFileSubFileLineCountMax = iVal;
        }
    }

    // Calculate default file paths
    //-----------------------------

    QString strAppNameNormalized = QCoreApplication::applicationName();

    // The application name may contain characters which are invalid in file names:
    strAppNameNormalized.remove(":");
    strAppNameNormalized.remove(" ");
    strAppNameNormalized.remove("\\");
    strAppNameNormalized.remove("/");
    strAppNameNormalized.remove("<");
    strAppNameNormalized.remove(">");

    QString strAppConfigDir = ZS::System::getAppConfigDir(strIniFileScope);
    QString strAppLogDir = ZS::System::getAppLogDir(strIniFileScope);

    QString strErrLogFileBaseName = strAppNameNormalized + "-Error";
    QString strErrLogFileSuffix = "xml";

    m_strErrLogFileAbsFilePath = strAppLogDir + "/" + strErrLogFileBaseName + "." + strErrLogFileSuffix;

    QString strTrcAdminObjFileSuffix = "xml";
    QString strTrcAdminObjFileBaseName = strAppNameNormalized + "-TrcMthAdmObj";

    QString strTrcLogFileSuffix = "log";
    QString strTrcLogFileBaseName = strAppNameNormalized + "-TrcMth";

    m_trcServerSettings.m_strAdminObjFileAbsFilePath = strAppConfigDir + "/" + strTrcAdminObjFileBaseName + "." + strTrcAdminObjFileSuffix;
    m_trcServerSettings.m_strLocalTrcFileAbsFilePath = strAppLogDir + "/" + strTrcLogFileBaseName + "." + strTrcLogFileSuffix;

    QString strTestStepsFileBaseName = strAppNameNormalized + "-TestSteps";
    QString strTestStepsFileSuffix = "xml";

    m_strTestStepsFileAbsFilePath = strAppConfigDir + "/" + strTestStepsFileBaseName + "." + strTestStepsFileSuffix;

    QString strThreadClrFileBaseName = strAppNameNormalized + "-ThreadColors";
    QString strThreadClrFileSuffix = "xml";

    m_strThreadClrFileAbsFilePath = strAppConfigDir + "/" + strThreadClrFileBaseName + "." + strThreadClrFileSuffix;

    // Create error manager
    //---------------------

    CErrLog::CreateInstance(true, m_strErrLogFileAbsFilePath);

    // Request Execution Tree
    //------------------------

    m_pReqExecTree = CRequestExecTree::CreateInstance();

    m_pReqExecTree->setGarbageCollectorEnabled(m_bReqExecTreeGarbageCollectorEnabled);
    m_pReqExecTree->setGarbageCollectorIntervalInSec(m_fReqExecTreeGarbageCollectorInterval_s);
    m_pReqExecTree->setGarbageCollectorElapsedInSec(m_fReqExecTreeGarbageCollectorElapsed_s);

    // Test
    //----------------

    m_pTest = new CTest(m_strTestStepsFileAbsFilePath);

    // Trace Server
    //-------------

    // Create trace server after test instance so that the test instance does not find
    // the trace server and does not create a trace admin object instance.
    // But create trace server before the main window so that the main window can connect
    // to the stateChanged signal of the trace server.

    m_pTrcServer = CIpcTrcServer::CreateInstance("ZSTrcServer", false, m_iTrcDetailLevelTrcServer);

    m_pTrcServer->setHostSettings(m_trcServerHostSettings);
    m_pTrcServer->setTraceSettings(m_trcServerSettings);
    m_pTrcServer->changeSettings();

    m_pTrcServer->recallAdminObjs();
    m_pTrcServer->saveAdminObjs();

    // Trace client
    //-------------

    m_pTrcClient = new CIpcTrcClient("TrcMthClient");

    m_pTrcClient->setWatchDogTimerUsed(false);
    m_pTrcClient->setHostSettings(m_trcClientHostSettings);
    m_pTrcClient->changeSettings();

    // Main Window
    //------------

    m_pMainWindow = new CMainWindow(i_strWindowTitle, m_pTest);
    m_pMainWindow->show();

} // ctor

//------------------------------------------------------------------------------
CApplication::~CApplication()
//------------------------------------------------------------------------------
{
    //saveSettings();

    // Destroy objects created and controlled by the application
    //----------------------------------------------------------

    try
    {
        delete m_pMainWindow;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pTrcClient;
    }
    catch(...)
    {
    }

    // Destroy trace server
    if( m_pTrcServer != nullptr )
    {
        try
        {
            ZS::Trace::CTrcServer::ReleaseInstance(m_pTrcServer);
        }
        catch(...)
        {
        }
    }

    try
    {
        delete m_pTest;
    }
    catch(...)
    {
    }

    //try
    //{
    //    delete m_pSettingsFile;
    //}
    //catch(...)
    //{
    //}

    CRequestExecTree::DestroyInstance();

    CErrLog::ReleaseInstance();

    //m_bTrcDetailLevelTrcServerSetViaProgArgs = false;
    //m_bTrcServerSettingsTrcFileLineCountMaxSetViaProgArgs = false;
    //m_pSettingsFile = nullptr;
    //m_strErrLogFileAbsFilePath;
    m_bReqExecTreeGarbageCollectorEnabled = false;
    m_fReqExecTreeGarbageCollectorInterval_s = 0.0;
    m_fReqExecTreeGarbageCollectorElapsed_s = 0.0;
    m_pReqExecTree = nullptr;
    //m_trcServerHostSettings;
    //m_trcServerSettings;
    m_pTrcServer = nullptr;
    m_iTrcDetailLevelTrcServer = 0;
    //m_strThreadClrFileAbsFilePath;
    //m_trcClientHostSettings;
    m_pTrcClient = nullptr;
    //m_state = static_cast<EState>(0);
    //m_pReqInProgress = nullptr;
    //m_strTestStepsFileAbsFilePath;
    m_pTest = nullptr;
    m_pMainWindow = nullptr;

} // dtor

