/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qtimer.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qmessagebox.h>
#else
#include <QtWidgets/qmessagebox.h>
#endif

#include "Test.h"
#include "TestModule1.h"
#include "TestModule2.h"
#include "MainWindow.h"

#include "ZSTest/ZSTestStepIdxTree.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Apps::Test::IpcTraceDllIfQtApp;


/*******************************************************************************
class CTest
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest() :
//------------------------------------------------------------------------------
    ZS::Test::CTest("ZS::IpcTrace::DllIfQtApp"),
    m_pTmrTestStepTimeout(nullptr),
    m_trcSettings(),
    m_uTrcServerPort(24763),
    m_iTrcDetailLevelTrcServer(ZS::Trace::ETraceDetailLevelNone),
    m_pTrcServer(nullptr),
    m_pTestModule1(nullptr),
    m_pTestModule2(nullptr)
{
    ZS::Trace::DllIf::STrcServerSettings_init(m_trcSettings);

    m_pTmrTestStepTimeout = new QTimer();
    m_pTmrTestStepTimeout->setSingleShot(true);

    if( !QObject::connect(
        /* pObjSender   */ m_pTmrTestStepTimeout,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTimerTestStepTimeout()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Test Steps
    //-----------

    int idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Load ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::loadDll",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepLoadDll(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Create Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::CreateInstance",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerCreateInstance(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Startup",
        /* strOperation    */ "DllIf::CIpcTrcServer::startup",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerStartup(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Recall Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::recallAdminObjs",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerRecallAdminObjs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Connect",
        /* strOperation    */ "CIpcTrcClient::connect",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientConnect(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Create Module 1",
        /* strOperation    */ "new CTestModule1()",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepCreateModule1(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Create Module 2",
        /* strOperation    */ "new CTestModule2()",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepCreateModule2(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Delete Module 2",
        /* strOperation    */ "delete CTestModule2()",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepDeleteModule2(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Delete Module 1",
        /* strOperation    */ "delete CTestModule1()",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepDeleteModule1(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Disconnect",
        /* strOperation    */ "CIpcTrcClient::disconnect",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientDisconnect(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Save Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::saveAdminObjs",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerSaveAdminObjs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Shutdown",
        /* strOperation    */ "DllIf::CIpcTrcServer::shutdown",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerShutdown(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Release Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::ReleaseInstance",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerReleaseInstance(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::releaseDll",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepReleaseDll(ZS::Test::CTestStep*)) );

    // Recall test step settings
    //--------------------------

    recall();

} // default ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo = save();

    if( errResultInfo.isErrorResult() )
    {
        if(CErrLog::GetInstance() != nullptr)
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    try
    {
        delete m_pTestModule1;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pTmrTestStepTimeout;
    }
    catch(...)
    {
    }

    if( m_pTrcServer != nullptr )
    {
        ZS::Trace::DllIf::CIpcTrcServer::ReleaseInstance(m_pTrcServer);
    }

    m_pTmrTestStepTimeout = nullptr;
    ZS::Trace::DllIf::STrcServerSettings_release(m_trcSettings);
    m_uTrcServerPort = 0;
    m_iTrcDetailLevelTrcServer = 0;
    m_pTrcServer = nullptr;
    m_pTestModule1 = nullptr;
    m_pTestModule2 = nullptr;

} // dtor

/*==============================================================================
public slots: // test step methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepLoadDll( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strDesiredValue = "LoadDll: Ok";

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setExpectedValues(strlstDesiredValues);

    // Test Step
    //----------

    #ifdef _DEBUG
    ZS::Trace::DllIf::EBuildConfiguration buildConfiguration = ZS::Trace::DllIf::EBuildConfigurationDebug;
    #else
    ZS::Trace::DllIf::EBuildConfiguration buildConfiguration = ZS::Trace::DllIf::EBuildConfigurationRelease;
    #endif

    const char* szCompiler = COMPILERLIBINFIX;  // from "ZSSysDllMain.h"
    const char* szPlatform = PLATFORMLIBINFIX;  // from "ZSSysDllMain.h"

    bool bOk = ZS::Trace::DllIf::loadDll(szCompiler, szPlatform, buildConfiguration, QT_VERSION_MAJOR);

    // Actual Values
    //---------------

    strActualValue = "LoadDll: " + QString(bOk ? "Ok" : "Failed");

    strlstActualValues.append(strActualValue);

    i_pTestStep->setResultValues(strlstActualValues);

} // doTestStepLoadDll

//------------------------------------------------------------------------------
void CTest::doTestStepReleaseDll( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strDesiredValue = "ReleaseDll: Ok";

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setExpectedValues(strlstDesiredValues);

    // Test Step
    //----------

    bool bOk = ZS::Trace::DllIf::releaseDll();

    // Actual Values
    //---------------

    strActualValue = "ReleaseDll: " + QString(bOk ? "Ok" : "Failed");

    strlstActualValues.append(strActualValue);

    i_pTestStep->setResultValues(strlstActualValues);

} // doTestStepReleaseDll

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerCreateInstance( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strDesiredValue = "TrcServer != nullptr";

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setExpectedValues(strlstDesiredValues);

    // Test Step
    //----------

    m_pTrcServer = ZS::Trace::DllIf::CIpcTrcServer::CreateInstance(
        "ZSTrcServer", ZS::Trace::DllIf::ETraceDetailLevelMethodArgs);

    // Actual Values
    //---------------

    if( m_pTrcServer == nullptr ) strActualValue = "TrcServer == nullptr";
    else strActualValue = "TrcServer != nullptr";

    strlstActualValues.append(strActualValue);

    i_pTestStep->setResultValues(strlstActualValues);

} // doTestStepTraceServerCreateInstance

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerReleaseInstance( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strDesiredValue = "TrcServer == nullptr";

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setExpectedValues(strlstDesiredValues);

    // Test Step
    //----------

    ZS::Trace::DllIf::CIpcTrcServer::ReleaseInstance(m_pTrcServer);
    m_pTrcServer = ZS::Trace::DllIf::CIpcTrcServer::GetInstance("ZSTrcServer");

    // Actual Values
    //---------------

    if( m_pTrcServer == nullptr ) strActualValue = "TrcServer == nullptr";
    else strActualValue = "TrcServer != nullptr";

    strlstActualValues.append(strActualValue);

    i_pTestStep->setResultValues(strlstActualValues);

} // doTestStepTraceServerReleaseInstance

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerStartup( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strDesiredValue = "Listening";

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setExpectedValues(strlstDesiredValues);

    // Test Step
    //----------

    bool bOk = false;

    if( m_pTrcServer != nullptr )
    {
        m_pTrcServer->clearLocalTrcFile();

        STrcServerSettings_release(m_trcSettings);
        m_trcSettings = m_pTrcServer->getTraceSettings();
        m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite = true;

        bOk = m_pTrcServer->setTraceSettings(m_trcSettings);
        if( bOk ) bOk = m_pTrcServer->setPort(m_uTrcServerPort);

        if( bOk ) bOk = m_pTrcServer->startup();
    }

    // Actual Values
    //---------------

    strActualValue = "Not listening";

    if( m_pTrcServer != nullptr )
    {
        if( m_pTrcServer->isListening() ) strActualValue = "Listening";
    }

    strlstActualValues.append(strActualValue);

    i_pTestStep->setResultValues(strlstActualValues);

} // doTestStepTraceServerStartup

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerShutdown( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strDesiredValue = "Idle";

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setExpectedValues(strlstDesiredValues);

    // Test Step
    //----------

    if( m_pTrcServer != nullptr )
    {
        m_pTrcServer->shutdown();
    }

    // Actual Values
    //---------------

    strDesiredValue = "Not idle";

    if( m_pTrcServer != nullptr )
    {
        if( !m_pTrcServer->isListening() ) strActualValue = "Idle";
    }

    strlstActualValues.append(strActualValue);

    i_pTestStep->setResultValues(strlstActualValues);

} // doTestStepTraceServerShutdown

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerRecallAdminObjs( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    // Range of IniFileScope: ["AppDir", "User", "System"]
    #ifdef __linux__
    // Using "System" on linux Mint ends up in directory "etc/xdg/<CompanyName>"
    // where the application has not write access rights. Stupid ...
    QString strIniFileScope = "User";
    #else
    QString strIniFileScope = "System"; // Default
    #endif

    QString strAppConfigDir = ZS::System::getAppConfigDir(strIniFileScope);

    QString strTrcAdminObjFileSuffix = "xml";
    QString strTrcAdminObjFileBaseName = QString(m_pTrcServer->name()) + "-TrcMthAdmObj";

    QString strTrcAdminObjFileAbsFilePath = strAppConfigDir + "/" + strTrcAdminObjFileBaseName + "." + strTrcAdminObjFileSuffix;

    QFile fileTrcAdminObjFile(strTrcAdminObjFileAbsFilePath);

    bool bOk = false;

    if( fileTrcAdminObjFile.exists() )
    {
        bOk = true;
    }

    strDesiredValue = bool2Str(bOk);
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setExpectedValues(strlstDesiredValues);

    // Test Step
    //----------

    bOk = !bOk;

    if( m_pTrcServer != nullptr )
    {
        bOk = m_pTrcServer->recallAdminObjs();
    }

    // Actual Values
    //---------------

    strActualValue = bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    i_pTestStep->setResultValues(strlstActualValues);

} // doTestStepTraceServerRecallAdminObjs

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerSaveAdminObjs( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    SErrResultInfo errResultInfo;

    // Desired Values
    //---------------

    bool bOk = true;

    strDesiredValue = bool2Str(bOk);
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setExpectedValues(strlstDesiredValues);

    // Test Step
    //----------

    bOk = !bOk;

    if( m_pTrcServer != nullptr )
    {
        bOk = m_pTrcServer->saveAdminObjs();
    }

    // Actual Values
    //---------------

    strActualValue = bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    i_pTestStep->setResultValues(strlstActualValues);

} // doTestStepTraceServerSaveAdminObjs

//------------------------------------------------------------------------------
void CTest::doTestStepTraceClientConnect( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strDesiredValue = "Connected";

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setExpectedValues(strlstDesiredValues);

    // Test Step
    //----------

    QMessageBox msgBox(CMainWindow::GetInstance());

    msgBox.setText("Please start and connect the trace method client.");
    msgBox.setInformativeText("Is the trace client connected?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setMinimumWidth(200);
    msgBox.setMinimumHeight(200);

    int iRet = msgBox.exec();

    // Actual Values
    //---------------

    strActualValue = "Not connected";

    if( iRet == QMessageBox::Yes )
    {
        if( m_pTrcServer != nullptr )
        {
            if( m_pTrcServer->isConnected() ) strActualValue = "Connected";
        }
    }

    strlstActualValues.append(strActualValue);

    i_pTestStep->setResultValues(strlstActualValues);

} // doTestStepTraceClientConnect

//------------------------------------------------------------------------------
void CTest::doTestStepTraceClientDisconnect( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strDesiredValue = "Disconnected";

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setExpectedValues(strlstDesiredValues);

    // Test Step
    //----------

    QMessageBox msgBox(CMainWindow::GetInstance());

    msgBox.setText("Please disconnect the trace method client.");
    msgBox.setInformativeText("Is the trace client disconnected?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setMinimumWidth(200);
    msgBox.setMinimumHeight(200);

    int iRet = msgBox.exec();

    // Actual Values
    //---------------

    strActualValue = "Not disconnected";

    if( iRet == QMessageBox::Yes )
    {
        if( m_pTrcServer != nullptr )
        {
            if( !m_pTrcServer->isConnected() ) strActualValue = "Disconnected";
        }
    }

    strlstActualValues.append(strActualValue);

    i_pTestStep->setResultValues(strlstActualValues);

} // doTestStepTraceClientDisconnect

//------------------------------------------------------------------------------
void CTest::doTestStepCreateModule1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;
    QStringList strlstMthTraces;

    QString strNameSpace  = CTestModule1::NameSpace();
    QString strClassName  = CTestModule1::ClassName();
    QString strObjName    = "TestModule1";
    QString strMthName    = "ctor";
    QString strMthInArgs  = "ObjName: TestModule1, TestModule2ObjName: TestModule2";
    QString strMthOutArgs = "";

    strlstMthTraces << "-> <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthInArgs + ")";
    strlstMthTraces << "<- <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthOutArgs + ")";

    bool bOk = true;

    // Desired Values
    //---------------

    strDesiredValue = bool2Str(bOk);

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setExpectedValues(strlstDesiredValues);

    // Test Step
    //----------

    m_pTestModule1 = new CTestModule1("TestModule1", "TestModule2");

    QString strInfoText;

    QMessageBox msgBox(CMainWindow::GetInstance());

    msgBox.setText("Please have a look at the trace output widget of the method trace client.");

    strInfoText  = "Do you see the following lines?";

    foreach( const QString& strTrc, strlstMthTraces )
    {
        strInfoText += "\n" + strTrc;
    }

    msgBox.setInformativeText(strInfoText);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setMinimumWidth(800);
    msgBox.setMinimumHeight(200);

    int iRet = msgBox.exec();

    // Actual Values
    //---------------

    bOk = !bOk;

    if( iRet == QMessageBox::Yes )
    {
        bOk = true;
    }

    strActualValue = bool2Str(bOk);

    strlstActualValues.append(strActualValue);

    i_pTestStep->setResultValues(strlstActualValues);

} // doTestStepCreateModule1

//------------------------------------------------------------------------------
void CTest::doTestStepDeleteModule1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;
    QStringList strlstMthTraces;

    QString strNameSpace  = CTestModule1::NameSpace();
    QString strClassName  = CTestModule1::ClassName();
    QString strObjName    = "TestModule1";
    QString strMthName    = "dtor";
    QString strMthInArgs  = "";
    QString strMthOutArgs = "";

    strlstMthTraces << "-> <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthInArgs + ")";
    strlstMthTraces << "<- <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthOutArgs + ")";

    bool bOk = true;

    // Desired Values
    //---------------

    strDesiredValue = bool2Str(bOk);

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setExpectedValues(strlstDesiredValues);

    // Test Step
    //----------

    delete m_pTestModule1;
    m_pTestModule1 = nullptr;

    QString strInfoText;

    QMessageBox msgBox(CMainWindow::GetInstance());

    msgBox.setText("Please have a look at the trace output widget of the method trace client.");

    strInfoText  = "Do you see the following lines?";

    foreach( const QString& strTrc, strlstMthTraces )
    {
        strInfoText += "\n" + strTrc;
    }

    msgBox.setInformativeText(strInfoText);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setMinimumWidth(800);
    msgBox.setMinimumHeight(200);

    int iRet = msgBox.exec();

    // Actual Values
    //---------------

    bOk = !bOk;

    if( iRet == QMessageBox::Yes )
    {
        bOk = true;
    }

    strActualValue = bool2Str(bOk);

    strlstActualValues.append(strActualValue);

    i_pTestStep->setResultValues(strlstActualValues);

} // doTestStepDeleteModule1

//------------------------------------------------------------------------------
void CTest::doTestStepCreateModule2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;
    QStringList strlstMthTraces;

    QString strNameSpace = CTestModule2::NameSpace();
    QString strClassName = CTestModule2::ClassName();

    strlstMthTraces << "   -> <" + strNameSpace + "::" + strClassName + "> TestModule2.ctor(TestModule2Thread: TestModule2, ObjName: TestModule2)";
    strlstMthTraces << "   <- <" + strNameSpace + "::" + strClassName + "> TestModule2.ctor()";
    strlstMthTraces << "                                 <- <" + strNameSpace + "::" + strClassName + "> TestModule2.recursiveTraceMethod(): 10";
    strlstMthTraces << "   <- <" + strNameSpace + "::" + strClassName + "> TestModule2.recursiveTraceMethod(): 0";
    strlstMthTraces << "   -> <" + strNameSpace + "::" + strClassName + "> TestModule2.event(Msg: ZS::Apps::Test::IpcTrace::ReqTest)";
    strlstMthTraces << "   <- <" + strNameSpace + "::" + strClassName + "> TestModule2.event(): 100";

    bool bOk = true;

    // Desired Values
    //---------------

    strDesiredValue = bool2Str(bOk);

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setExpectedValues(strlstDesiredValues);

    // Test Step
    //----------

    if( m_pTestModule1 != nullptr )
    {
        m_pTestModule2 = m_pTestModule1->createModule2();
    }

    QString strInfoText;

    QMessageBox msgBox(CMainWindow::GetInstance());

    msgBox.setText("Please have a look at the trace output widget of the method trace client.");

    strInfoText  = "Do you see the following lines?";

    foreach( const QString& strTrc, strlstMthTraces )
    {
        strInfoText += "\n" + strTrc;
    }

    msgBox.setInformativeText(strInfoText);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setMinimumWidth(800);
    msgBox.setMinimumHeight(200);

    int iRet = msgBox.exec();

    // Actual Values
    //---------------

    bOk = !bOk;

    if( iRet == QMessageBox::Yes )
    {
        bOk = true;
    }

    strActualValue = bool2Str(bOk);

    strlstActualValues.append(strActualValue);

    i_pTestStep->setResultValues(strlstActualValues);

} // doTestStepCreateModule2

//------------------------------------------------------------------------------
void CTest::doTestStepDeleteModule2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;
    QStringList strlstMthTraces;

    QString strNameSpace  = CTestModule2::NameSpace();
    QString strClassName  = CTestModule2::ClassName();
    QString strObjName    = "TestModule2";
    QString strMthName    = "dtor";
    QString strMthInArgs  = "";
    QString strMthOutArgs = "";

    strlstMthTraces << "-> <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthInArgs + ")";
    strlstMthTraces << "<- <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthOutArgs + ")";

    bool bOk = true;

    // Desired Values
    //---------------

    strDesiredValue = bool2Str(bOk);

    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setExpectedValues(strlstDesiredValues);

    // Test Step
    //----------

    if( m_pTestModule1 != nullptr )
    {
        m_pTestModule1->deleteModule2();
    }
    m_pTestModule2 = nullptr;

    QString strInfoText;

    QMessageBox msgBox(CMainWindow::GetInstance());

    msgBox.setText("Please have a look at the trace output widget of the method trace client.");

    strInfoText  = "Do you see the following lines?";

    foreach( const QString& strTrc, strlstMthTraces )
    {
        strInfoText += "\n" + strTrc;
    }

    msgBox.setInformativeText(strInfoText);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setMinimumWidth(800);
    msgBox.setMinimumHeight(200);

    int iRet = msgBox.exec();

    // Actual Values
    //---------------

    bOk = !bOk;

    if( iRet == QMessageBox::Yes )
    {
        bOk = true;
    }

    strActualValue = bool2Str(bOk);

    strlstActualValues.append(strActualValue);

    i_pTestStep->setResultValues(strlstActualValues);

} // doTestStepDeleteModule2

/*==============================================================================
protected: // slots
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::onTimerTestStepTimeout()
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStep* pTestStep = getCurrentTestStep();

    if( pTestStep != nullptr )
    {
        QString     strActualValue;
        QStringList strlstActualValues;

        strActualValue = "Test step not finished in time";
        strlstActualValues.append(strActualValue);

        pTestStep->setResultValues(strlstActualValues);

    } // if( pTestStep != nullptr )

} // onTimerTestStepTimeout()
