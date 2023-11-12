/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include "ZSIpcLog/ZSIpcLogServer.h"
#include "ZSIpcLog/ZSIpcLogClient.h"
#include "ZSTest/ZSTestStepIdxTree.h"
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
using namespace ZS::Log;
using namespace ZS::Apps::Test::IpcLog;


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
        strClassName = "ZS::Apps::Test::IpcLog::CApplication";
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
    m_pLogServer(nullptr),
    m_clientHostSettingsLogClient("127.0.0.1", 24762, 5000),
    m_pLogClient(nullptr),
    m_pTest(nullptr),
    m_pMainWindow(nullptr),
    m_bAutoStartTest(false)
{
    setObjectName("theApp");

    if( thread()->objectName().length() == 0 )
    {
        thread()->setObjectName("GUIMain");
    }

    QCoreApplication::setOrganizationName(i_strOrganizationName);
    QCoreApplication::setOrganizationDomain(i_strOrganizationDomain);
    QCoreApplication::setApplicationName(i_strAppName);

    QIcon iconApp;

    QPixmap pxmApp32x32(":/ZS/App/ZeusSoft_32x32.png");
    QPixmap pxmApp48x48(":/ZS/App/ZeusSoft_48x48.png");
    QPixmap pxmApp64x64(":/ZS/App/ZeusSoft_64x64.png");

    iconApp.addPixmap(pxmApp32x32);
    iconApp.addPixmap(pxmApp48x48);
    iconApp.addPixmap(pxmApp64x64);

    QApplication::setWindowIcon(iconApp);

    // Parse command arguments
    //------------------------

    int         idxArg;
    QString     strArg;
    QString     strVal;
    QStringList strListArgsPar;
    QStringList strListArgsVal;

    parseAppArgs( i_argc, i_argv, strListArgsPar, strListArgsVal );

    #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
    for( idxArg = 0; idxArg < strListArgsPar.length() && idxArg < strListArgsVal.length(); idxArg++ )
    #else
    for( idxArg = 0; idxArg < strListArgsPar.size() && idxArg < strListArgsVal.size(); idxArg++ )
    #endif
    {
        strArg = strListArgsPar[idxArg];
        strVal = strListArgsVal[idxArg];

        if( strArg == "AutoStartTest" )
        {
            m_bAutoStartTest = true;
        }
    }

    // Create error manager
    //---------------------

    CErrLog::CreateInstance();

    // Request Execution Tree
    //------------------------

    m_pReqExecTree = CRequestExecTree::CreateInstance();

    m_pReqExecTree->setGarbageCollectorEnabled(m_bReqExecTreeGarbageCollectorEnabled);
    m_pReqExecTree->setGarbageCollectorIntervalInSec(m_fReqExecTreeGarbageCollectorInterval_s);
    m_pReqExecTree->setGarbageCollectorElapsedInSec(m_fReqExecTreeGarbageCollectorElapsed_s);

    // Test
    //----------------

    m_pTest = new CTest();

    // Log Server
    //-----------

    // Create log server before the main window so that the main window can connect
    // to the stateChanged signal of the log server.

    m_pLogServer = ZS::Log::CIpcLogServer::CreateInstance();
    m_pLogServer->setLocalLogFileSubFileLineCountMax(10000);

    //m_pLogServer->recallLoggers();

    // Log client
    //-----------

    m_pLogClient = new CIpcLogClient("LogClient");
    m_pLogClient->setHostSettings(m_clientHostSettingsLogClient);
    m_pLogClient->changeSettings();

    // Main Window
    //------------

    m_pMainWindow = new CMainWindow(i_strWindowTitle, m_pTest);
    m_pMainWindow->show();

    // Start test automatically if desired
    //------------------------------------

    if( m_bAutoStartTest )
    {
        m_pTest->start();

        QObject::connect(
            m_pTest, &CTest::testFinished,
            this, &CApplication::onTestFinished);
    }

} // ctor

//------------------------------------------------------------------------------
CApplication::~CApplication()
//------------------------------------------------------------------------------
{
    // Destroy objects created and controlled by the application
    //----------------------------------------------------------

    if( m_pLogServer != nullptr )
    {
        m_pLogServer->shutdown();
    }

    try
    {
        delete m_pMainWindow;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pLogClient;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pTest;
    }
    catch(...)
    {
    }

    // Destroy trace server
    if( m_pLogServer != nullptr )
    {
        //m_pLogServer->saveLoggers();

        try
        {
            ZS::Log::CIpcLogServer::ReleaseInstance();
        }
        catch(...)
        {
        }
    }

    CRequestExecTree::DestroyInstance();

    CErrLog::ReleaseInstance();

    //m_strErrLogFileAbsFilePath;
    m_bReqExecTreeGarbageCollectorEnabled = false;
    m_fReqExecTreeGarbageCollectorInterval_s = 0.0;
    m_fReqExecTreeGarbageCollectorElapsed_s = 0.0;
    m_pReqExecTree = nullptr;
    m_pLogServer = nullptr;
    //m_clientHostSettingsLogClient;
    m_pLogClient = nullptr;
    //m_strTestStepsFileAbsFilePath;
    m_pTest = nullptr;
    m_pMainWindow = nullptr;
    m_bAutoStartTest = false;

} // dtor

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onTestFinished( const ZS::Test::CEnumTestResult& i_result )
//------------------------------------------------------------------------------
{
    // This test is fragile concering the timing. Which thread is waken up at first
    // if a wait condition is signalled etc.. We try the test several times before
    // the test is reported as failed.
    if( i_result == ZS::Test::ETestResult::TestFailed && m_pTest->getNumberOfTestRuns() < 3 )
    {
        m_pTest->start();
    }
    else
    {
        exit(i_result == ZS::Test::ETestResult::TestPassed ? 0 : 1);
    }
}
