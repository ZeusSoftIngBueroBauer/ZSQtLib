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

#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSIpcTrace/ZSIpcTrcClient.h"
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
    m_eZSTrcServerTrcDetailLevel(EMethodTraceDetailLevel::None),
    m_eZSTrcServerTrcDetailLevelNoisyMethods(EMethodTraceDetailLevel::None),
    m_eZSTrcClientTrcDetailLevel(EMethodTraceDetailLevel::None),
    m_pZSTrcServer(nullptr),
    m_clientHostSettingsZSTrcClient("127.0.0.1", 24763, 5000),
    m_pZSTrcClient(nullptr),
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
        else if( strArg == "ZSTrcServerTraceDetailLevel" )
        {
            m_eZSTrcServerTrcDetailLevel = CEnumMethodTraceDetailLevel::fromString(strVal).enumerator();
        }
        else if( strArg == "ZSTrcServerTraceDetailLevelNoisyMethods" )
        {
            m_eZSTrcServerTrcDetailLevelNoisyMethods = CEnumMethodTraceDetailLevel::fromString(strVal).enumerator();
        }
        else if( strArg == "ZSTrcClientTraceDetailLevel" )
        {
            m_eZSTrcClientTrcDetailLevel = CEnumMethodTraceDetailLevel::fromString(strVal).enumerator();
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

    // Trace Server
    //-------------

    // Create trace server after test instance so that the test instance does not find
    // the trace server and does not create a trace admin object instance.
    // But create trace server before the main window so that the main window can connect
    // to the stateChanged signal of the trace server.

    m_pZSTrcServer = ZS::Trace::CIpcTrcServer::CreateInstance(
        /* iTrcDetailLevel                 */ m_eZSTrcServerTrcDetailLevel,
        /* iTrcDetailLevelMutex            */ EMethodTraceDetailLevel::None,
        /* iTrcDetailLevelIpcServer        */ EMethodTraceDetailLevel::None,
        /* iTrcDetailLevelIpcServerMutex   */ EMethodTraceDetailLevel::None,
        /* iTrcDetailLevelIpcServerGateway */ EMethodTraceDetailLevel::None );
    m_pZSTrcServer->setLocalTrcFileSubFileLineCountMax(10000);

    //m_pZSTrcServer->recallAdminObjs();

    // Trace client
    //-------------

    m_pZSTrcClient = new CIpcTrcClient(
        /* strName                       */ "ZSTrcClient",
        /* iTrcMthFileDetailLevel        */ m_eZSTrcClientTrcDetailLevel,
        /* iTrcMthFileDetailLevelMutex   */ EMethodTraceDetailLevel::None,
        /* iTrcMthFileDetailLevelGateway */ EMethodTraceDetailLevel::None );
    m_pZSTrcClient->setHostSettings(m_clientHostSettingsZSTrcClient);
    m_pZSTrcClient->changeSettings();

    // Test
    //----------------

    m_pTest = new CTest();

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

    CIpcTrcServer* pTrcServer = ZS::Trace::CIpcTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcServer->shutdown();
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
        delete m_pZSTrcClient;
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
    if( m_pZSTrcServer != nullptr )
    {
        //m_pZSTrcServer->saveAdminObjs();

        try
        {
            ZS::Trace::CIpcTrcServer::ReleaseInstance();
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
    m_eZSTrcServerTrcDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
    m_eZSTrcServerTrcDetailLevelNoisyMethods = static_cast<EMethodTraceDetailLevel>(0);
    m_eZSTrcClientTrcDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
    m_pZSTrcServer = nullptr;
    //m_clientHostSettingsZSTrcClient;
    m_pZSTrcClient = nullptr;
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
