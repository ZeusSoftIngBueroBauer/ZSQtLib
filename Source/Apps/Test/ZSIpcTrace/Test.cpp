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
#include <QtNetwork/qhostinfo.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qtextedit.h>
#else
#include <QtWidgets/qtextedit.h>
#endif

#include "Test.h"
#include "App.h"
#include "WidgetCentral.h"
#include "TestModule1.h"
#include "TestModule2.h"

#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepIdxTree.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSIpcTraceGUI/ZSIpcTrcMthWdgt.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysAux.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Trace::GUI;
using namespace ZS::Apps::Test::IpcTrace;


/*******************************************************************************
class CTest
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest() :
//------------------------------------------------------------------------------
    ZS::Test::CTest("ZS::IpcTrace"),
    m_pTestTrcServer(nullptr),
    m_pTestTrcClient(nullptr),
    m_pTmrTestStepTimeout(nullptr),
    m_hshReqsInProgress(),
    m_pMyClass1(nullptr),
    m_pMyClass2(nullptr)
{
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

    ZS::Test::CTestStepGroup* pGrp = nullptr;

    // Test Step Group - One Trace Server
    //-----------------------------------

    int idxGroup = 0;
    int idxStep = 0;

    pGrp = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " One Trace Server",
        /* pTSGrpParent    */ nullptr );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZS Trace Server Startup",
        /* strOperation    */ "ZSTrcServer::startup",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerStartup(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZS Trace Client Connect",
        /* strOperation    */ "ZSTrcClient::connect",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientConnect(ZS::Test::CTestStep*)) );

    #pragma message(__TODO__ "Test modules with admin objects and tracing")

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::classMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass1::classMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZS Trace Client Disconnect",
        /* strOperation    */ "ZSTrcClient::disconnect",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientDisconnect(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZS Trace Server Shutdown",
        /* strOperation    */ "ZSTrcServer::shutdown",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerShutdown(ZS::Test::CTestStep*)) );


    // Test Step Group - Two Trace Servers
    //------------------------------------

    pGrp = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " Two Trace Servers",
        /* pTSGrpParent    */ nullptr );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Test Trace Server Create Instance",
        /* strOperation    */ "TestTrcServer::CreateInstance",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerCreateInstance(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Test Trace Server Recall Admin Objects",
        /* strOperation    */ "TestTrcServer::recallAdminObjs",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerRecallAdminObjs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZS Trace Server Startup",
        /* strOperation    */ "ZSTrcServer::startup",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerStartup(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Test Trace Server Startup",
        /* strOperation    */ "TestTrcServer::startup",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerStartup(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Test Trace Client Create",
        /* strOperation    */ "TestTrcClient::create",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientCreate(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZS Trace Client Connect",
        /* strOperation    */ "ZSTrcClient::connect",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientConnect(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Test Trace Client Connect",
        /* strOperation    */ "TestTrcClient::connect",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientConnect(ZS::Test::CTestStep*)) );


    #pragma message(__TODO__ "Test modules with admin objects and tracing")


    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Test Trace Client Disconnect",
        /* strOperation    */ "TestTrcClient::disconnect",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientDisconnect(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZS Trace Client Disconnect",
        /* strOperation    */ "ZSTrcClient::disconnect",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientDisconnect(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Test Trace Client Destroy",
        /* strOperation    */ "TestTrcClient::destroy",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientDestroy(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Test Trace Server Shutdown",
        /* strOperation    */ "TestTrcServer::shutdown",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerShutdown(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZS Trace Server Shutdown",
        /* strOperation    */ "ZSTrcServer::shutdown",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerShutdown(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Test Trace Server Save Admin Objects",
        /* strOperation    */ "TestTrcServer::saveAdminObjs",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerSaveAdminObjs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Test Trace Server Release Instance",
        /* strOperation    */ "TestTrcServer::ReleaseInstance",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerReleaseInstance(ZS::Test::CTestStep*)) );

    //new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(++idxStep) + " Create Module 1",
    //    /* strOperation    */ "new CMyClass1()",
    //    /* pTSGrpParent    */ nullptr,
    //    /* szDoTestStepFct */ SLOT(doTestStepCreateModule1(ZS::Test::CTestStep*)) );

    //new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(++idxStep) + " Create Module 2",
    //    /* strOperation    */ "new CMyClass2()",
    //    /* pTSGrpParent    */ nullptr,
    //    /* szDoTestStepFct */ SLOT(doTestStepCreateModule2(ZS::Test::CTestStep*)) );

    //new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(++idxStep) + " Delete Module 2",
    //    /* strOperation    */ "delete CMyClass2()",
    //    /* pTSGrpParent    */ nullptr,
    //    /* szDoTestStepFct */ SLOT(doTestStepDeleteModule2(ZS::Test::CTestStep*)) );

    //new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(++idxStep) + " Delete Module 1",
    //    /* strOperation    */ "delete CMyClass1()",
    //    /* pTSGrpParent    */ nullptr,
    //    /* szDoTestStepFct */ SLOT(doTestStepDeleteModule1(ZS::Test::CTestStep*)) );

    // Recall test step settings
    //--------------------------

    recall();

} // default ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo = save();

    if(errResultInfo.isErrorResult())
    {
        if(CErrLog::GetInstance() != nullptr)
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    try
    {
        delete m_pMyClass1;
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

    try
    {
        delete m_pTestTrcClient;
    }
    catch(...)
    {
    }

    if( m_pTestTrcServer != nullptr )
    {
        ZS::Trace::CIpcTrcServer::ReleaseInstance(m_pTestTrcServer);
    }

    m_pTestTrcServer = nullptr;
    m_pTestTrcClient = nullptr;
    m_pTmrTestStepTimeout = nullptr;
    m_hshReqsInProgress.clear();
    m_pMyClass1 = nullptr;
    m_pMyClass2 = nullptr;

} // dtor

/*==============================================================================
public slots: // test step methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerCreateInstance( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    i_pTestStep->setExpectedValue("TestTrcServer Created");

    // Test Step
    //----------

    CIpcTrcServer* pTrcServer = nullptr;

    if( i_pTestStep->getOperation().startsWith("TestTrcServer") )
    {
        try
        {
            pTrcServer = m_pTestTrcServer = ZS::Trace::CIpcTrcServer::CreateInstance("TestTrcServer");
            emit trcServerCreated(pTrcServer);
            SServerHostSettings hostSettings(24764);
            pTrcServer->setHostSettings(hostSettings);
            pTrcServer->changeSettings();
        }
        catch(...)
        {
            strlstResultValues.append("Exception thrown on CIpcTrcServer::CreateInstance");
            pTrcServer = nullptr;
        }
    }

    if( pTrcServer != nullptr )
    {
        strlstResultValues.append("TestTrcServer Created");
    }
    else
    {
        strlstResultValues.append("TestTrcServer Not Created");
    }

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepTraceServerCreateInstance

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerReleaseInstance( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    i_pTestStep->setExpectedValue("TestTrcServer Released");

    // Test Step
    //----------

    CIpcTrcServer* pTrcServer = nullptr;

    if( i_pTestStep->getOperation().startsWith("TestTrcServer") )
    {
        if( m_pTestTrcServer )
        {
            emit trcServerAboutToBeDestroyed(m_pTestTrcServer);
        }

        try
        {
            ZS::Trace::CIpcTrcServer::ReleaseInstance(m_pTestTrcServer);
        }
        catch(...)
        {
            strlstResultValues.append("Exception thrown on CIpcTrcServer::ReleaseInstance");
        }
        m_pTestTrcServer = nullptr;

        pTrcServer = ZS::Trace::CIpcTrcServer::GetInstance("TestTrcServer");
    }

    if( pTrcServer == nullptr )
    {
        strlstResultValues.append("TestTrcServer Released");
    }
    else
    {
        strlstResultValues.append("TestTrcServer Not Released");
    }

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepTraceServerReleaseInstance

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerRecallAdminObjs( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    QString strServerName;

    if( i_pTestStep->getOperation().startsWith("ZSTrcServer") )
    {
        strServerName = "ZSTrcServer";
    }
    else if( i_pTestStep->getOperation().startsWith("TestTrcServer") )
    {
        strServerName = "TestTrcServer";
    }

    CIpcTrcServer* pTrcServer = ZS::Trace::CIpcTrcServer::GetInstance(strServerName);

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
    QString strTrcAdminObjFileBaseName = strServerName + "-TrcMthAdmObj";

    QString strTrcAdminObjFileAbsFilePath = strAppConfigDir + "/" + strTrcAdminObjFileBaseName + "." + strTrcAdminObjFileSuffix;

    QFile fileTrcAdminObjFile(strTrcAdminObjFileAbsFilePath);

    SErrResultInfo errResultInfo;

    if( !fileTrcAdminObjFile.exists() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultFileOpenForRead);
        errResultInfo.setAddErrInfoDscr(strTrcAdminObjFileAbsFilePath);
    }

    strlstExpectedValues.append(errResultInfo.getResultStr());

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( pTrcServer == nullptr )
    {
        strlstResultValues.append("Trace server not found");
    }
    else
    {
        SErrResultInfo errResultInfo = pTrcServer->recallAdminObjs();
        strlstResultValues.append(errResultInfo.getResultStr());
    }

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepTraceServerRecallAdminObjs

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerSaveAdminObjs( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    i_pTestStep->setExpectedValue(result2Str(ZS::System::EResultSuccess));

    // Test Step
    //----------

    CIpcTrcServer* pTrcServer = nullptr;

    if( i_pTestStep->getOperation().startsWith("ZSTrcServer") )
    {
        pTrcServer = ZS::Trace::CIpcTrcServer::GetInstance("ZSTrcServer");
    }
    else if( i_pTestStep->getOperation().startsWith("TestTrcServer") )
    {
        pTrcServer = ZS::Trace::CIpcTrcServer::GetInstance("TestTrcServer");
    }

    if( pTrcServer == nullptr )
    {
        strlstResultValues.append("Trace server not found");
    }
    else
    {
        SErrResultInfo errResultInfo = pTrcServer->saveAdminObjs();
        strlstResultValues.append(errResultInfo.getResultStr());
    }

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepTraceServerSaveAdminObjs

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerStartup( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    CIpcTrcServer* pTrcServer = nullptr;

    if( i_pTestStep->getOperation().startsWith("ZSTrcServer") )
    {
        pTrcServer = ZS::Trace::CIpcTrcServer::GetInstance("ZSTrcServer");
    }
    else if( i_pTestStep->getOperation().startsWith("TestTrcServer") )
    {
        pTrcServer = ZS::Trace::CIpcTrcServer::GetInstance("TestTrcServer");
    }

    if( pTrcServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Invalid test operation");
    }
    else
    {
        pTrcServer->clearLocalTrcFile();

        ZS::Ipc::SServerHostSettings hostSettings = pTrcServer->getHostSettings();

        strExpectedValue = ZS::Ipc::CServer::State2Str(Ipc::CServer::EStateListening) + " (";
        strExpectedValue += hostSettings.m_strLocalHostName;
        strExpectedValue += ":" + QString::number(hostSettings.m_uLocalPort);
        strExpectedValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
        strExpectedValue += ")";
        strlstExpectedValues.append(strExpectedValue);

        i_pTestStep->setExpectedValues(strlstExpectedValues);
    }

    // Test Step
    //----------

    if( pTrcServer == nullptr )
    {
        i_pTestStep->setResultValue("Trace server not found");
    }
    else
    {
        CRequest* pReq = pTrcServer->startup(5000);

        if( isAsynchronousRequest(pReq) )
        {
            strResultValue = "startupTrcServer not expected to be asynchronous";
        }
        else // if( isAsynchronousRequest(pReq) )
        {
            ZS::Ipc::SServerHostSettings hostSettings = pTrcServer->getHostSettings();

            strResultValue = ZS::Ipc::CServer::State2Str(pTrcServer->getState()) + " (";
            strResultValue += hostSettings.m_strLocalHostName;
            strResultValue += ":" + QString::number(hostSettings.m_uLocalPort);
            strResultValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
            strResultValue += ")";
        }
        strlstResultValues.append(strResultValue);
    }

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepTraceServerStartup

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerShutdown( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    CIpcTrcServer* pTrcServer = nullptr;

    if( i_pTestStep->getOperation().startsWith("ZSTrcServer") )
    {
        pTrcServer = ZS::Trace::CIpcTrcServer::GetInstance("ZSTrcServer");
    }
    else if( i_pTestStep->getOperation().startsWith("TestTrcServer") )
    {
        pTrcServer = ZS::Trace::CIpcTrcServer::GetInstance("TestTrcServer");
    }

    if( pTrcServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Invalid test operation");
    }
    else
    {
        ZS::Ipc::SServerHostSettings hostSettings = pTrcServer->getHostSettings();

        strExpectedValue = ZS::Ipc::CServer::State2Str(Ipc::CServer::EStateIdle) + " (";
        strExpectedValue += hostSettings.m_strLocalHostName;
        strExpectedValue += ":" + QString::number(hostSettings.m_uLocalPort);
        strExpectedValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
        strExpectedValue += ")";
        strlstExpectedValues.append(strExpectedValue);

        i_pTestStep->setExpectedValues(strlstExpectedValues);
    }

    // Test Step
    //----------

    if( pTrcServer == nullptr )
    {
        i_pTestStep->setResultValue("Trace server not found");
    }
    else
    {
        CRequest* pReq = pTrcServer->shutdown();

        if( isAsynchronousRequest(pReq) )
        {
            strResultValue = "shutdownTrcServer not expected to be asynchronous";
        }
        else // if( isAsynchronousRequest(pReq) )
        {
            ZS::Ipc::SServerHostSettings hostSettings = pTrcServer->getHostSettings();

            strResultValue = ZS::Ipc::CServer::State2Str(pTrcServer->getState()) + " (";
            strResultValue += hostSettings.m_strLocalHostName;
            strResultValue += ":" + QString::number(hostSettings.m_uLocalPort);
            strResultValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
            strResultValue += ")";
        }
        strlstResultValues.append(strResultValue);

        i_pTestStep->setResultValues(strlstResultValues);
    }

} // doTestStepTraceServerShutdown

//------------------------------------------------------------------------------
void CTest::doTestStepTraceClientCreate( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    i_pTestStep->setExpectedValue("TestTrcClient Created");

    // Test Step
    //----------

    ZS::Trace::CIpcTrcClient* pTrcClient = nullptr;

    if( i_pTestStep->getOperation().startsWith("TestTrcClient") )
    {
        pTrcClient = m_pTestTrcClient = new ZS::Trace::CIpcTrcClient("TestTrcClient");
        emit trcClientCreated(pTrcClient);
        SClientHostSettings hostSettings("127.0.0.1", 24764);
        pTrcClient->setHostSettings(hostSettings);
        pTrcClient->changeSettings();
    }

    if( pTrcClient != nullptr )
    {
        strlstResultValues.append("TestTrcClient Created");
    }
    else
    {
        strlstResultValues.append("TestTrcClient Not Created");
    }

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepTraceClientCreate

//------------------------------------------------------------------------------
void CTest::doTestStepTraceClientDestroy( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    i_pTestStep->setExpectedValue("TestTrcClient Destroyed");

    // Test Step
    //----------

    CIpcTrcClient* pTrcClient = nullptr;

    if( i_pTestStep->getOperation().startsWith("TestTrcClient") )
    {
        if( m_pTestTrcClient )
        {
            emit trcClientAboutToBeDestroyed(m_pTestTrcClient);
        }

        try
        {
            delete m_pTestTrcClient;
        }
        catch(...)
        {
            strlstResultValues.append("Exception thrown on deleting TestTrcClient");
        }
        pTrcClient = m_pTestTrcClient = nullptr;
    }

    if( pTrcClient == nullptr )
    {
        strlstResultValues.append("TestTrcClient Destroyed");
    }
    else
    {
        strlstResultValues.append("TestTrcClient Not Destroyed");
    }

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepTraceClientDestroy

//------------------------------------------------------------------------------
void CTest::doTestStepTraceClientConnect( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    QVector<CIdxTreeEntry*> arpTreeEntriesServer;

    CIpcTrcClient* pTrcClient = nullptr;
    CIpcTrcServer* pTrcServer = nullptr;

    if( i_pTestStep->getOperation().startsWith("ZSTrcClient") )
    {
        pTrcClient = CApplication::GetInstance()->getTrcClient();
        pTrcServer = ZS::Trace::CIpcTrcServer::GetInstance("ZSTrcServer");
    }
    else if( i_pTestStep->getOperation().startsWith("TestTrcClient") )
    {
        pTrcClient = m_pTestTrcClient;
        pTrcServer = ZS::Trace::CIpcTrcServer::GetInstance("TestTrcServer");
    }

    if( pTrcClient == nullptr || pTrcServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Invalid test operation");
    }
    else
    {
        ZS::Ipc::SClientHostSettings hostSettings = pTrcClient->getHostSettings();

        strExpectedValue = ZS::Ipc::CClient::State2Str(Ipc::CClient::EStateConnected) + " (";
        strExpectedValue += hostSettings.m_strRemoteHostName;
        strExpectedValue += ":" + QString::number(hostSettings.m_uRemotePort);
        strExpectedValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
        strExpectedValue += ")";
        strlstExpectedValues.append(strExpectedValue);

        CIdxTreeTrcAdminObjs* pIdxTreeServer = pTrcServer->getTraceAdminObjIdxTree();

        arpTreeEntriesServer = pIdxTreeServer->treeEntriesVec();

        for( auto& pTreeEntry : arpTreeEntriesServer )
        {
            if( pTreeEntry != nullptr && pTreeEntry->entryType() == EIdxTreeEntryType::Leave)
            {
                CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

                // Please note that the trace clients gateway admin objects (thread, sockets) will be created
                // after invoking the connect method. If the admin objects XML file did not exist those
                // trace admin objects are not yet added to the index tree but will be send to the trace client
                // as they will be created during the connect request.
                if( !pTrcAdminObj->keyInTree().contains("TrcClient") )
                {
                    strExpectedValue = pTrcAdminObj->keyInTree() + ": ";
                    strExpectedValue += "RefCount: " + QString::number(pTrcAdminObj->getRefCount());
                    strExpectedValue += ", Enabled: " + CEnumEnabled(pTrcAdminObj->getEnabled()).toString();
                    strExpectedValue += ", DetailLevel: " + QString::number(pTrcAdminObj->getTraceDetailLevel());
                    strlstExpectedValues.append(strExpectedValue);
                }
            }
        }
        i_pTestStep->setExpectedValues(strlstExpectedValues);
    }

    // Test Step
    //----------

    if( pTrcClient == nullptr )
    {
        i_pTestStep->setResultValue("Either no trace client or no trace server not found");
    }
    else
    {
        CRequest* pReq = pTrcClient->connect_();

        if( isAsynchronousRequest(pReq) )
        {
            pReq->setExecutionData(QString::number(pReq->getId()), i_pTestStep);

            m_hshReqsInProgress[pReq->getId()] = pReq;

            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            if( arpTreeEntriesServer.size() > 0 )
            {
                m_pTmrTestStepTimeout->start(5000);

                if( !QObject::connect(
                    /* pObjSender   */ pTrcClient,
                    /* szSignal     */ SIGNAL(traceAdminObjInserted(QObject*, const QString&)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onZSTraceClientTraceAdminObjInserted(QObject*, const QString&)) ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            }
        }
        else // if( !isAsynchronousRequest(pReq) )
        {
            strResultValue = "connectTrcClient expected to be asynchronous";
            strlstResultValues.append(strResultValue);
            i_pTestStep->setResultValues(strlstResultValues);
        }
    }

} // doTestStepTraceClientConnect

//------------------------------------------------------------------------------
void CTest::doTestStepTraceClientDisconnect( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    CIpcTrcClient* pTrcClient = nullptr;

    if( i_pTestStep->getOperation().startsWith("ZSTrcClient") )
    {
        pTrcClient = CApplication::GetInstance()->getTrcClient();
    }
    else if( i_pTestStep->getOperation().startsWith("TestTrcClient") )
    {
        pTrcClient = m_pTestTrcClient;
    }

    if( pTrcClient == nullptr )
    {
        i_pTestStep->setExpectedValue("Invalid test operation");
    }
    else
    {
        ZS::Ipc::SClientHostSettings hostSettings = pTrcClient->getHostSettings();

        strExpectedValue = ZS::Ipc::CClient::State2Str(Ipc::CClient::EStateUnconnected) + " (";
        strExpectedValue += hostSettings.m_strRemoteHostName;
        strExpectedValue += ":" + QString::number(hostSettings.m_uRemotePort);
        strExpectedValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
        strExpectedValue += ")";
        strlstExpectedValues.append(strExpectedValue);

        // We could also check whether the trace client cleared the index tree of admin objects.
        // But the order in which the onDisconnected slots are invoked when emitting the
        // IpcClient disconnected signal is unpredictable. The test may receive this signal before
        // the trace client and the trace admin object index tree may not be cleared at this time.

        //strExpectedValue = "TrcAdminObjIdxTree.isEmpty";
        //strlstExpectedValues.append(strExpectedValue);

        i_pTestStep->setExpectedValues(strlstExpectedValues);
    }

    // Test Step
    //----------

    if( pTrcClient != nullptr )
    {
        CRequest* pReq = pTrcClient->disconnect_();

        if( isAsynchronousRequest(pReq) )
        {
            pReq->setExecutionData(QString::number(pReq->getId()), i_pTestStep);

            m_hshReqsInProgress[pReq->getId()] = pReq;

            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
        }
        else // if( isAsynchronousRequest(pReq) )
        {
            strResultValue = "disconnectTrcClient expected to be asynchronous";
            strlstResultValues.append(strResultValue);
            i_pTestStep->setResultValues(strlstResultValues);
        }
    }
    else
    {
        i_pTestStep->setResultValue("Trace client not found");
    }

} // doTestStepTraceClientDisconnect

//------------------------------------------------------------------------------
void CTest::doTestStepTraceMethodCall( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //----------------

    QString strOperation = i_pTestStep->getOperation();

    QString strServerName;
    QString strNameSpace;
    QString strClassName;
    QString strObjName;
    QString strMth;
    QStringList strlstInArgs;
    QString strMthRet;

    splitMethodCallOperation(strOperation, strServerName, strClassName, strObjName, strMth, strlstInArgs, strMthRet);

    CIpcTrcServer* pTrcServer = nullptr;

    if( strServerName == "ZSTrcServer" )
    {
        pTrcServer = ZS::Trace::CIpcTrcServer::GetInstance("ZSTrcServer");
        CMyClass1::setTraceServerName("ZSTrcServer");
    }
    else if( strServerName == "TestTrcServer" )
    {
        pTrcServer = ZS::Trace::CIpcTrcServer::GetInstance("TestTrcServer");
        CMyClass1::setTraceServerName("TestTrcServer");
    }

    if( strClassName == CMyClass1::ClassName() )
    {
        strNameSpace = CMyClass1::NameSpace();
    }
    else if( strClassName == CMyClass2::ClassName() )
    {
        strNameSpace = CMyClass1::NameSpace();
    }

    if( pTrcServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Invalid test operation");
    }
    else
    {
        pTrcServer->setNewTrcAdminObjsDefaultDetailLevel(ETraceDetailLevelMethodArgs);
        pTrcServer->setLocalTrcFileCloseFileAfterEachWrite(true);

        QString strTrcMethodEnter = "-> <" + strNameSpace + "::" + strClassName + "> ";
        QString strTrcMethodLeave = "<- <" + strNameSpace + "::" + strClassName + "> ";

        if( !strObjName.isEmpty() )
        {
            strTrcMethodEnter += strObjName + ".";
            strTrcMethodLeave += strObjName + ".";
        }
        strTrcMethodEnter += strMth + "(" + strlstInArgs.join(", ") + ")";

        if( !strMthRet.isEmpty() )
        {
            strTrcMethodLeave += strMth + "(): " + strMthRet;
        }

        strlstExpectedValues.append(strTrcMethodEnter);
        strlstExpectedValues.append(strTrcMethodLeave);

        i_pTestStep->setExpectedValues(strlstExpectedValues);
    }

    // Test Step
    //----------

    if( strlstInArgs.size() == 0 )
    {
    }
    else if( strlstInArgs.size() == 1 )
    {
        if( strClassName == CMyClass1::ClassName() )
        {
            if( strMth == "classMethod")
            {
                CMyClass1::classMethod(strlstInArgs[0]);
            }
        }
    }
    if( strlstInArgs.size() == 2 )
    {
    }

    if( pTrcServer != nullptr )
    {
        pTrcServer->setNewTrcAdminObjsDefaultDetailLevel(ETraceDetailLevelNone);
        pTrcServer->setLocalTrcFileCloseFileAfterEachWrite(false);
    }

    CWidgetCentral* pWdgtCentral = CWidgetCentral::GetInstance();

    CWdgtTrcMthList* pWdgtTrcMthList = pWdgtCentral->getTrcMthListWdgt();

    if( !QObject::connect(
        /* pObjSender   */ pWdgtTrcMthList,
        /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onZSTraceClientTrcMthListWdgtTextItemAdded(const QString&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pTmrTestStepTimeout->start(5000);

} // doTestStepTraceMethodCall

#if 0
//------------------------------------------------------------------------------
void CTest::doTestStepCreateModule1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    QString strNameSpace  = CMyClass1::NameSpace();
    QString strClassName  = CMyClass1::ClassName();
    QString strObjName    = "MyClass1";
    QString strMthName    = "ctor";
    QString strMthInArgs  = "ObjName: MyClass1, MyClass2ObjName: MyClass2";
    QString strMthOutArgs = "";

    QString strTrcMethodEnter = "-> <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthInArgs + ")";
    QString strTrcMethodLeave = "<- <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthOutArgs + ")";

    strlstExpectedValues.append(strTrcMethodEnter);
    strlstExpectedValues.append(strTrcMethodLeave);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    m_pMyClass1 = new CMyClass1("MyClass1", "MyClass2");

    CWidgetCentral* pWdgtCentral = CWidgetCentral::GetInstance();

    CWdgtTrcMthList* pWdgtTrcMthList = pWdgtCentral->getTrcMthListWdgt();

    if( !QObject::connect(
        /* pObjSender   */ pWdgtTrcMthList,
        /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onZSTraceClientTrcMthListWdgtTextItemAdded(const QString&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pTmrTestStepTimeout->start(5000);

} // doTestStepCreateModule1
#endif // #if 0

#if 0
//------------------------------------------------------------------------------
void CTest::doTestStepDeleteModule1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    QString strNameSpace  = CMyClass1::NameSpace();
    QString strClassName  = CMyClass1::ClassName();
    QString strObjName    = "MyClass1";
    QString strMthName    = "dtor";
    QString strMthInArgs  = "";
    QString strMthOutArgs = "";

    QString strTrcMethodEnter = "-> <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthInArgs + ")";
    QString strTrcMethodLeave = "<- <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthOutArgs + ")";

    strlstExpectedValues.append(strTrcMethodEnter);
    strlstExpectedValues.append(strTrcMethodLeave);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    delete m_pMyClass1;
    m_pMyClass1 = nullptr;

    CWidgetCentral* pWdgtCentral = CWidgetCentral::GetInstance();

    CWdgtTrcMthList* pWdgtTrcMthList = pWdgtCentral->getTrcMthListWdgt();

    if( !QObject::connect(
        /* pObjSender   */ pWdgtTrcMthList,
        /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onZSTraceClientTrcMthListWdgtTextItemAdded(const QString&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pTmrTestStepTimeout->start(5000);

} // doTestStepDeleteModule1
#endif // #if 0

#if 0
//------------------------------------------------------------------------------
void CTest::doTestStepCreateModule2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    QString strNameSpace  = CMyClass2::NameSpace();
    QString strClassName  = CMyClass2::ClassName();
    QString strObjName    = "MyClass2";
    QString strMthName    = "event";
    QString strMthInArgs  = "Msg: ZS::Apps::Test::IpcTrace::ReqTest";
    QString strMthOutArgs = "";
    QString strMthRet     = "100";
    QString strMthTrace;

    QString strTrcMethodLeave = "<- <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthOutArgs + ")";
    if( !strMthRet.isEmpty() ) strTrcMethodLeave += ": " + strMthRet;
    strlstExpectedValues.append(strTrcMethodLeave);

    strMthTrace = "   -> <" + strNameSpace + "::" + strClassName + "> MyClass2.ctor(MyClass2Thread: MyClass2, ObjName: MyClass2)";
    strlstExpectedValues.append(strMthTrace);

    strMthTrace = "   <- <" + strNameSpace + "::" + strClassName + "> MyClass2.ctor()";
    strlstExpectedValues.append(strMthTrace);

    strMthTrace = "                                 <- <" + strNameSpace + "::" + strClassName + "> MyClass2.recursiveTraceMethod(): 10";
    strlstExpectedValues.append(strMthTrace);

    strMthTrace = "   <- <" + strNameSpace + "::" + strClassName + "> MyClass2.recursiveTraceMethod(): 0";
    strlstExpectedValues.append(strMthTrace);

    strMthTrace = "   -> <" + strNameSpace + "::" + strClassName + "> MyClass2.event(Msg: ZS::Apps::Test::IpcTrace::ReqTest)";
    strlstExpectedValues.append(strMthTrace);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pMyClass1 != nullptr )
    {
        m_pMyClass2 = m_pMyClass1->createModule2();
    }

    CWidgetCentral* pWdgtCentral = CWidgetCentral::GetInstance();

    CWdgtTrcMthList* pWdgtTrcMthList = pWdgtCentral->getTrcMthListWdgt();

    if( !QObject::connect(
        /* pObjSender   */ pWdgtTrcMthList,
        /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onZSTraceClientTrcMthListWdgtTextItemAdded(const QString&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pTmrTestStepTimeout->start(30000);

} // doTestStepCreateModule2
#endif // #if 0

#if 0
//------------------------------------------------------------------------------
void CTest::doTestStepDeleteModule2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    QString strNameSpace  = CMyClass2::NameSpace();
    QString strClassName  = CMyClass2::ClassName();
    QString strObjName    = "MyClass2";
    QString strMthName    = "dtor";
    QString strMthInArgs  = "";
    QString strMthOutArgs = "";

    QString strTrcMethodLeave = "<- <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthOutArgs + ")";

    strlstExpectedValues.append(strTrcMethodLeave);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pMyClass1 != nullptr )
    {
        m_pMyClass1->deleteModule2();
    }
    m_pMyClass2 = nullptr;

    CWidgetCentral* pWdgtCentral = CWidgetCentral::GetInstance();

    CWdgtTrcMthList* pWdgtTrcMthList = pWdgtCentral->getTrcMthListWdgt();

    if( !QObject::connect(
        /* pObjSender   */ pWdgtTrcMthList,
        /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onZSTraceClientTrcMthListWdgtTextItemAdded(const QString&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pTmrTestStepTimeout->start(5000);

} // doTestStepDeleteModule2
#endif // #if 0

/*==============================================================================
protected: // slots
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::onRequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    if( CErrLog::GetInstance() != nullptr && i_reqDscr.m_errResultInfo.isErrorResult() )
    {
        if( !i_reqDscr.m_errResultInfo.isAddedToErrLogModel() )
        {
            CErrLog::GetInstance()->addEntry(i_reqDscr.m_errResultInfo);
            i_reqDscr.m_errResultInfo.setAddedToErrLogModel(true);
        }
    }

    if( m_hshReqsInProgress.contains(i_reqDscr.m_iId) )
    {
        CRequest* pReq = m_hshReqsInProgress[i_reqDscr.m_iId];

        SErrResultInfo errResultInfo = i_reqDscr.m_errResultInfo;

        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        // If the request has been finished ...
        if( errResultInfo.isErrorResult() || i_reqDscr.m_iProgress_perCent >= 100 )
        {
            m_hshReqsInProgress.remove(i_reqDscr.m_iId);

            // On updating the request this slot method would be invoked as a reentry.
            // As the request is finished we don't want this reentry call.
            QObject::disconnect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

            ZS::Test::CTestStep* pTestStep = reinterpret_cast<ZS::Test::CTestStep*>(pReq->takeExecutionData(QString::number(i_reqDscr.m_iId)));

            if( pTestStep != nullptr )
            {
                CIpcTrcClient* pTrcClient = nullptr;
                QVector<CIdxTreeEntry*> arpTreeEntriesServer;

                if( pTestStep->getOperation().contains("ZSTrcClient") )
                {
                    CIpcTrcServer* pTrcServer = CApplication::GetInstance()->getTrcServer();
                    if( pTrcServer != nullptr )
                    {
                        CIdxTreeTrcAdminObjs* pIdxTreeServer = pTrcServer->getTraceAdminObjIdxTree();
                        arpTreeEntriesServer = pIdxTreeServer->treeEntriesVec();
                    }
                    pTrcClient = CApplication::GetInstance()->getTrcClient();
                }
                else if( pTestStep->getOperation().contains("TestTrcClient") )
                {
                    if( m_pTestTrcServer != nullptr )
                    {
                        CIdxTreeTrcAdminObjs* pIdxTreeServer = m_pTestTrcServer->getTraceAdminObjIdxTree();
                        arpTreeEntriesServer = pIdxTreeServer->treeEntriesVec();
                    }
                    pTrcClient = m_pTestTrcClient;
                }

                if( pTrcClient != nullptr )
                {
                    if( pTestStep->getOperation().contains("TrcClient::connect") )
                    {
                        // If the trace server has already admin objects in the index tree the test step
                        // is not finished if the trace client is connected.
                        // The client got to query the trace admin objects after the connection
                        // has been established. We need to wait until all expected admin objects
                        // have been received by the client before finishing this test step.
                        // On receiving the admin objects the slot "onZSTraceClientTraceAdminObjInserted"
                        // is called. If all expected admin objects are received this slot finishes
                        // the test step.

                        // If the trace server does not have any admin objects in the index tree the test step
                        // is finished after the trace client is connected. But this should not be the case.
                        if( arpTreeEntriesServer.size() == 0 )
                        {
                            QString     strResultValue;
                            QStringList strlstResultValues;

                            ZS::Ipc::SClientHostSettings hostSettings = pTrcClient->getHostSettings();

                            strResultValue = ZS::Ipc::CClient::State2Str(pTrcClient->getState()) + " (";
                            strResultValue += hostSettings.m_strRemoteHostName;
                            strResultValue += ":" + QString::number(hostSettings.m_uRemotePort);
                            strResultValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                            strResultValue += ")";
                            strlstResultValues.append(strResultValue);

                            pTestStep->setResultValues(strlstResultValues);
                        }
                    } // if( pTestStep->getOperation().contains("TrcClient::connect") )

                    else if( pTestStep->getOperation().contains("TrcClient::disconnect") )
                    {
                        QString     strResultValue;
                        QStringList strlstResultValues;

                        ZS::Ipc::SClientHostSettings hostSettings = pTrcClient->getHostSettings();

                        strResultValue = ZS::Ipc::CClient::State2Str(pTrcClient->getState()) + " (";
                        strResultValue += hostSettings.m_strRemoteHostName;
                        strResultValue += ":" + QString::number(hostSettings.m_uRemotePort);
                        strResultValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                        strResultValue += ")";
                        strlstResultValues.append(strResultValue);

                        // We could also check whether the trace client cleared the index tree of admin objects.
                        // But the order in which the onDisconnected slots are invoked when emitting the
                        // IpcClient disconnected signal is unpredictable. The test may receive this signal before
                        // the trace client and the trace admin object index tree may not be cleared at this time.

                        pTestStep->setResultValues(strlstResultValues);

                    } // if( pTestStep->getOperation().contains("TrcClient::disconnect") )
                } // if( pTrcClient != nullptr )
            } // if( pTestStep != nullptr )
        } // if( errResultInfo.isErrorResult() || i_reqDscr.m_iProgress_perCent >= 100 )
    } // if( m_hshReqsInProgress.contains(i_reqDscr.m_iId) )

} // onRequestChanged

//------------------------------------------------------------------------------
void CTest::onZSTraceClientTraceAdminObjInserted( QObject* /*i_pTrcClient*/, const QString& /*i_strKeyInTree*/ )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStep* pTestStep = getCurrentTestStep();

    if( pTestStep != nullptr )
    {
        CIpcTrcClient* pTrcClient = nullptr;
        QVector<CIdxTreeEntry*> arpTreeEntriesServer;

        if( pTestStep->getOperation().contains("ZSTrcClient") )
        {
            CIpcTrcServer* pTrcServer = CApplication::GetInstance()->getTrcServer();
            if( pTrcServer != nullptr )
            {
                CIdxTreeTrcAdminObjs* pIdxTreeServer = pTrcServer->getTraceAdminObjIdxTree();
                arpTreeEntriesServer = pIdxTreeServer->treeEntriesVec();
            }
            pTrcClient = CApplication::GetInstance()->getTrcClient();
        }
        else if( pTestStep->getOperation().contains("TestTrcClient") )
        {
            if( m_pTestTrcServer != nullptr )
            {
                CIdxTreeTrcAdminObjs* pIdxTreeServer = m_pTestTrcServer->getTraceAdminObjIdxTree();
                arpTreeEntriesServer = pIdxTreeServer->treeEntriesVec();
            }
            pTrcClient = m_pTestTrcClient;
        }

        if( pTrcClient != nullptr )
        {
            if( pTestStep->getOperation().contains("TrcClient::connect") )
            {
                // Test step is finished if client is connected and received all trace admin objects from the server.

                // Actual Values
                //---------------

                CIdxTreeTrcAdminObjs* pIdxTreeClient = pTrcClient->getTraceAdminObjIdxTree();

                QVector<CIdxTreeEntry*> arpTreeEntriesClient = pIdxTreeClient->treeEntriesVec();

                bool bTestStepFinished = false;

                if( pTrcClient->isConnected() && arpTreeEntriesClient.size() == arpTreeEntriesServer.size() )
                {
                    bTestStepFinished = true;

                    CIdxTreeEntry* pTreeEntryServer;
                    CIdxTreeEntry* pTreeEntryClient;

                    for( int idxEntry = 0; idxEntry < arpTreeEntriesServer.size(); idxEntry++ )
                    {
                        pTreeEntryServer = arpTreeEntriesServer[idxEntry];
                        pTreeEntryClient = arpTreeEntriesClient[idxEntry];

                        if( pTreeEntryServer == nullptr && pTreeEntryClient != nullptr )
                        {
                            bTestStepFinished = false;
                            break;
                        }
                        else if( pTreeEntryServer != nullptr && pTreeEntryClient == nullptr )
                        {
                            bTestStepFinished = false;
                            break;
                        }
                    }
                } // if( pTrcClient->isConnected() && arpTreeEntriesClient.size() == arpTreeEntriesServer.size() )

                if( bTestStepFinished )
                {
                    if( m_pTmrTestStepTimeout->isActive() )
                    {
                        m_pTmrTestStepTimeout->stop();
                    }

                    QObject::disconnect(
                        /* pObjSender   */ pTrcClient,
                        /* szSignal     */ SIGNAL(traceAdminObjInserted(QObject*, const QString&)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onZSTraceClientTraceAdminObjInserted(QObject*, const QString&)) );

                    QString     strResultValue;
                    QStringList strlstResultValues;

                    ZS::Ipc::SClientHostSettings hostSettings = pTrcClient->getHostSettings();

                    strResultValue = ZS::Ipc::CClient::State2Str(pTrcClient->getState()) + " (";
                    strResultValue += hostSettings.m_strRemoteHostName;
                    strResultValue += ":" + QString::number(hostSettings.m_uRemotePort);
                    strResultValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                    strResultValue += ")";
                    strlstResultValues.append(strResultValue);

                    for( auto& pTreeEntry : arpTreeEntriesClient )
                    {
                        if( pTreeEntry != nullptr && pTreeEntry->entryType() == EIdxTreeEntryType::Leave)
                        {
                            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

                            // Please note that the trace clients gateway admin objects (thread, sockets) will be created
                            // after invoking the connect method. If the admin objects XML file did not exist those
                            // trace admin objects are not yet added to the index tree but will be send to the trace client
                            // as they will be created during the connect request.
                            if( !pTrcAdminObj->keyInTree().contains("TrcClient") )
                            {
                                strResultValue = pTrcAdminObj->keyInTree() + ": ";
                                strResultValue += "RefCount: " + QString::number(pTrcAdminObj->getRefCount());
                                strResultValue += ", Enabled: " + CEnumEnabled(pTrcAdminObj->getEnabled()).toString();
                                strResultValue += ", DetailLevel: " + QString::number(pTrcAdminObj->getTraceDetailLevel());
                                strlstResultValues.append(strResultValue);
                            }
                        }
                    }

                    pTestStep->setResultValues(strlstResultValues);

                } // if( bTestStepFinished )
            } // if( pTestStep->getOperation().contains("TrcClient::connect") )
        } // if( pTrcClient != nullptr )
    } // if( pTestStep != nullptr )

} // onZSTraceClientTraceAdminObjInserted

//------------------------------------------------------------------------------
void CTest::onZSTraceClientTrcMthListWdgtTextItemAdded( const QString& i_strText )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStep* pTestStep = getCurrentTestStep();

    if( pTestStep != nullptr )
    {
        QString strOperation = pTestStep->getOperation();

        QString strServerName;
        QString strNameSpace;
        QString strClassName;
        QString strObjName;
        QString strMth;
        QStringList strlstInArgs;
        QString strMthRet;

        splitMethodCallOperation(strOperation, strServerName, strClassName, strObjName, strMth, strlstInArgs, strMthRet);

        if( strClassName == CMyClass1::ClassName() )
        {
            strNameSpace = CMyClass1::NameSpace();
        }
        else if( strClassName == CMyClass2::ClassName() )
        {
            strNameSpace = CMyClass1::NameSpace();
        }

        QString     strResultValue;
        QStringList strlstResultValues;

        QString strText = i_strText;

        strText.replace("&lt;", "<");
        strText.replace("&gt;", ">");
        strText.replace("&nbsp;", "");

        // Two entries must have been added to finish the test step: Enter and Leave Method.
        // The test step is finished if the Method Leave is traced.
        if( strText.contains("<-") )
        {
            CWidgetCentral* pWdgtCentral = CWidgetCentral::GetInstance();
            CWdgtTrcMthList* pWdgtTrcMthList = pWdgtCentral->getTrcMthListWdgt();
            QTextEdit* pEdtTrcMthList = pWdgtTrcMthList->getTextEdit();
            QTextDocument* pDocTrcMthList = pEdtTrcMthList->document();

            // Range of IniFileScope: ["AppDir", "User", "System"]
            #ifdef __linux__
            // Using "System" on linux Mint ends up in directory "etc/xdg/<CompanyName>"
            // where the application has not write access rights. Stupid ...
            QString strIniFileScope = "User";
            #else
            QString strIniFileScope = "System"; // Default
            #endif

            QString strAppLogDir = ZS::System::getAppLogDir(strIniFileScope);

            QString strTrcMthFileSuffix = "log";
            QString strTrcMthFileBaseName = strServerName + "-TrcMth00";

            QString strTrcMthFileAbsFilePath = strAppLogDir + "/" + strTrcMthFileBaseName + "." + strTrcMthFileSuffix;

            QString strTrcMethodEnter = "-> <" + strNameSpace + "::" + strClassName + "> ";
            QString strTrcMethodLeave = "<- <" + strNameSpace + "::" + strClassName + "> ";

            if( !strObjName.isEmpty() )
            {
                strTrcMethodEnter += strObjName + ".";
                strTrcMethodLeave += strObjName + ".";
            }
            strTrcMethodEnter += strMth + "(" + strlstInArgs.join(", ") + ")";

            if( !strMthRet.isEmpty() )
            {
                strTrcMethodLeave += strMth + "(): " + strMthRet;
            }

            if( strClassName == "CMyClass1" && strMth == "classMethod" )
            {
                // Test step finished (only one entry expected).
                if( m_pTmrTestStepTimeout->isActive() )
                {
                    m_pTmrTestStepTimeout->stop();
                }

                QObject::disconnect(
                    /* pObjSender   */ pWdgtTrcMthList,
                    /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onZSTraceClientTrcMthListWdgtTextItemAdded(const QString&)) );

                // Check if entry added to trace method widget.
                //---------------------------------------------

                QTextCursor textCursor = pDocTrcMthList->find(strTrcMethodEnter);

                if( !textCursor.isNull() ) strlstResultValues.append(strTrcMethodEnter);
                else strlstResultValues.append(strTrcMethodEnter + ": not found in text edit widget");

                textCursor = pDocTrcMthList->find(strTrcMethodLeave);

                if( !textCursor.isNull() ) strlstResultValues.append(strTrcMethodLeave);
                else strlstResultValues.append(strTrcMethodLeave + ": not found in text edit widget");

                // Check if entry added to log file.
                //----------------------------------

                QFile fileTrcMthFile(strTrcMthFileAbsFilePath);

                if( !fileTrcMthFile.open(QIODevice::ReadOnly|QIODevice::Text) )
                {
                    strlstResultValues.append("Could not open file " + strTrcMthFileAbsFilePath);
                }
                else
                {
                    QTextStream txtstrmLogFileAppDefault(&fileTrcMthFile);

                    QString strTrcMthFile = txtstrmLogFileAppDefault.readAll();

                    bool bTrcMethodEnterFound = strTrcMthFile.contains(strTrcMethodEnter);
                    bool bTrcMethodLeaveFound = strTrcMthFile.contains(strTrcMethodLeave);

                    if( !bTrcMethodEnterFound ) strlstResultValues.append(strTrcMethodEnter + ": not found in trace method file");
                    if( !bTrcMethodLeaveFound ) strlstResultValues.append(strTrcMethodLeave + ": not found in trace method file");
                }

                pTestStep->setResultValues(strlstResultValues);

            } // if( pTestStep->getOperation().contains("CMyClass1::classMethod") )

            else if( pTestStep->getOperation() == "new CMyClass1()" || pTestStep->getOperation() == "delete CMyClass1()" )
            {
                // Test step finished (only one entry expected).
                if( m_pTmrTestStepTimeout->isActive() )
                {
                    m_pTmrTestStepTimeout->stop();
                }

                QObject::disconnect(
                    /* pObjSender   */ pWdgtTrcMthList,
                    /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onZSTraceClientTrcMthListWdgtTextItemAdded(const QString&)) );

                // Check if entry added to trace method widget.
                //---------------------------------------------

                QTextCursor textCursor = pDocTrcMthList->find(strTrcMethodEnter);

                if( !textCursor.isNull() ) strlstResultValues.append(strTrcMethodEnter);
                else strlstResultValues.append(strTrcMethodEnter + ": not found in text edit widget");

                textCursor = pDocTrcMthList->find(strTrcMethodLeave);

                if( !textCursor.isNull() ) strlstResultValues.append(strTrcMethodLeave);
                else strlstResultValues.append(strTrcMethodLeave + ": not found in text edit widget");

                // Check if entry added to log file.
                //----------------------------------

                QFile fileTrcMthFile(strTrcMthFileAbsFilePath);

                if( !fileTrcMthFile.open(QIODevice::ReadOnly|QIODevice::Text) )
                {
                    strlstResultValues.append("Could not open file " + strTrcMthFileAbsFilePath);
                }
                else
                {
                    QTextStream txtstrmLogFileAppDefault(&fileTrcMthFile);

                    QString strTrcMthFile = txtstrmLogFileAppDefault.readAll();

                    bool bTrcMethodEnterFound = strTrcMthFile.contains(strTrcMethodEnter);
                    bool bTrcMethodLeaveFound = strTrcMthFile.contains(strTrcMethodLeave);

                    if( !bTrcMethodEnterFound ) strlstResultValues.append(strTrcMethodEnter + ": not found in trace method file");
                    if( !bTrcMethodLeaveFound ) strlstResultValues.append(strTrcMethodLeave + ": not found in trace method file");
                }

                pTestStep->setResultValues(strlstResultValues);

            } // if( pTestStep->getOperation() == "new CMyClass1()" || pTestStep->getOperation() == "delete CMyClass1()" )

            else if( pTestStep->getOperation() == "new CMyClass2()" || pTestStep->getOperation() == "delete CMyClass2()" )
            {
                QString strInArgs;
                QString strOutArgs;

                QString strTrc1;
                QString strTrc2;
                QString strTrc3;
                QString strTrc4;
                QString strTrc5;

                if( pTestStep->getOperation() == "new CMyClass2()" )
                {
                    strMth    = "event";
                    strInArgs = "Msg: ZS::Apps::Test::IpcTrace::ReqTest";
                    strMthRet = "100";

                    strTrc1 = "   -> <ZS::Apps::Test::IpcTrace::CMyClass2> MyClass2.ctor(MyClass2Thread: MyClass2, ObjName: MyClass2)";
                    strTrc2 = "   <- <ZS::Apps::Test::IpcTrace::CMyClass2> MyClass2.ctor()";
                    strTrc3 = "                                 <- <ZS::Apps::Test::IpcTrace::CMyClass2> MyClass2.recursiveTraceMethod(): 10";
                    strTrc4 = "   <- <ZS::Apps::Test::IpcTrace::CMyClass2> MyClass2.recursiveTraceMethod(): 0";
                    strTrc5 = "   -> <ZS::Apps::Test::IpcTrace::CMyClass2> MyClass2.event(Msg: ZS::Apps::Test::IpcTrace::ReqTest)";
                }
                else if( pTestStep->getOperation() == "delete CMyClass2()" )
                {
                    strMth = "dtor";
                }

                QString strTrcMethodLeave = "<- <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMth + "(" + strOutArgs + ")";

                if( !strMthRet.isEmpty() ) strTrcMethodLeave += ": " + strMthRet;

                if( strText.contains(strNameSpace) && strText.contains(strClassName) && strText.contains(strObjName) && strText.contains(strMth) && strText.contains(strMthRet) )
                {
                    if( m_pTmrTestStepTimeout->isActive() )
                    {
                        m_pTmrTestStepTimeout->stop();
                    }

                    QObject::disconnect(
                        /* pObjSender   */ pWdgtTrcMthList,
                        /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onZSTraceClientTrcMthListWdgtTextItemAdded(const QString&)) );

                    // Check if entry added to trace method widget.
                    //---------------------------------------------

                    QTextCursor textCursor = pDocTrcMthList->find(strTrcMethodLeave);

                    if( !textCursor.isNull() ) strlstResultValues.append(strTrcMethodLeave);
                    else strlstResultValues.append(strTrcMethodLeave + ": not found in text edit widget");

                    if( !strTrc1.isEmpty() )
                    {
                        textCursor = pDocTrcMthList->find(strTrc1);
                        if( !textCursor.isNull() ) strlstResultValues.append(strTrc1);
                        else strlstResultValues.append(strTrc1 + ": not found in text edit widget");
                    }
                    if( !strTrc2.isEmpty() )
                    {
                        textCursor = pDocTrcMthList->find(strTrc2);
                        if( !textCursor.isNull() ) strlstResultValues.append(strTrc2);
                        else strlstResultValues.append(strTrc2 + ": not found in text edit widget");
                    }
                    if( !strTrc3.isEmpty() )
                    {
                        textCursor = pDocTrcMthList->find(strTrc3);
                        if( !textCursor.isNull() ) strlstResultValues.append(strTrc3);
                        else strlstResultValues.append(strTrc3 + ": not found in text edit widget");
                    }
                    if( !strTrc4.isEmpty() )
                    {
                        textCursor = pDocTrcMthList->find(strTrc4);
                        if( !textCursor.isNull() ) strlstResultValues.append(strTrc4);
                        else strlstResultValues.append(strTrc4 + ": not found in text edit widget");
                    }
                    if( !strTrc5.isEmpty() )
                    {
                        textCursor = pDocTrcMthList->find(strTrc5);
                        if( !textCursor.isNull() ) strlstResultValues.append(strTrc5);
                        else strlstResultValues.append(strTrc5 + ": not found in text edit widget");
                    }

                    // Check if entry added to log file.
                    //----------------------------------

                    QFile fileTrcMthFile(strTrcMthFileAbsFilePath);

                    if( !fileTrcMthFile.open(QIODevice::ReadOnly|QIODevice::Text) )
                    {
                        strlstResultValues.append("Could not open file " + strTrcMthFileAbsFilePath);
                    }
                    else
                    {
                        QTextStream txtstrmLogFileAppDefault(&fileTrcMthFile);

                        QString strTrcMthFile = txtstrmLogFileAppDefault.readAll();

                        bool bStringFound = strTrcMthFile.contains(strTrcMethodLeave);

                        if( !bStringFound ) strlstResultValues.append(strTrcMethodLeave + ": not found in trace method file");

                        if( !strTrc1.isEmpty() )
                        {
                            bStringFound = strTrcMthFile.contains(strTrc1);
                            if( !bStringFound ) strlstResultValues.append(strTrc1 + ": not found in trace method file");
                        }
                        if( !strTrc2.isEmpty() )
                        {
                            bStringFound = strTrcMthFile.contains(strTrc2);
                            if( !bStringFound ) strlstResultValues.append(strTrc2 + ": not found in trace method file");
                        }
                        if( !strTrc3.isEmpty() )
                        {
                            bStringFound = strTrcMthFile.contains(strTrc3);
                            if( !bStringFound ) strlstResultValues.append(strTrc3 + ": not found in trace method file");
                        }
                        if( !strTrc4.isEmpty() )
                        {
                            bStringFound = strTrcMthFile.contains(strTrc4);
                            if( !bStringFound ) strlstResultValues.append(strTrc4 + ": not found in trace method file");
                        }
                        if( !strTrc5.isEmpty() )
                        {
                            bStringFound = strTrcMthFile.contains(strTrc5);
                            if( !bStringFound ) strlstResultValues.append(strTrc5 + ": not found in trace method file");
                        }
                    }

                    pTestStep->setResultValues(strlstResultValues);

                } // if( strText.contains(strNameSpace) && strText.contains(strClassName) && strText.contains(strObjName) && strText.contains(strMthName) )
            } // if( pTestStep->getOperation() == "new CMyClass2()" || pTestStep->getOperation() == "delete CMyClass2()" )
        } // if( strText.contains("<-") )
    } // if( pTestStep != nullptr )

} // onZSTraceClientTrcMthListWdgtTextItemAdded

//------------------------------------------------------------------------------
void CTest::onTimerTestStepTimeout()
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStep* pTestStep = getCurrentTestStep();

    if( pTestStep != nullptr )
    {
        QString     strResultValue;
        QStringList strlstResultValues;

        strResultValue = "Test step not finished in time";
        strlstResultValues.append(strResultValue);

        pTestStep->setResultValues(strlstResultValues);

        if( pTestStep->getOperation() == "CIpcTrcClient::connect" )
        {
            CIpcTrcClient* pTrcClient = CApplication::GetInstance()->getTrcClient();

            QObject::disconnect(
                /* pObjSender   */ pTrcClient,
                /* szSignal     */ SIGNAL(traceAdminObjInserted(QObject*, const QString&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onZSTraceClientTraceAdminObjInserted(QObject*, const QString&)) );
        }
    } // if( pTestStep != nullptr )

} // onTimerTestStepTimeout()

/*==============================================================================
private: // instance auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Splits the operation string into sections.

    Examples for strOperation:

    - "ZSTrcServer/CMyClass1::method(Arg1)",
    - "ZSTrcServer/CMyClass1::method(Arg1, Arg2): Ret",
    - "ZSTrcServer/CMyClass2::theInst.method(Arg1, Arg2)",
    - "ZSTrcServer/CMyClass2::theInst.method(Arg1, Arg2): Ret",

*/
void CTest::splitMethodCallOperation(
    const QString& i_strOperation,
    QString& o_strServerName,
    QString& o_strClassName,
    QString& o_strObjName,
    QString& o_strMth,
    QStringList& o_strlstInArgs,
    QString& o_strMthRet ) const
//------------------------------------------------------------------------------
{
    o_strServerName = "";
    o_strClassName = "";
    o_strObjName = "";
    o_strMth = "";
    o_strlstInArgs.clear();
    o_strMthRet = "";

    QStringList strlst;

    strlst = i_strOperation.split("/", Qt::SkipEmptyParts);

    if( strlst.size() == 2 )
    {
        o_strServerName = strlst[0];
        strlst = strlst[1].split("::", Qt::SkipEmptyParts);
        if( strlst.size() == 2 )
        {
            o_strClassName = strlst[0];
            strlst = strlst[1].split(".", Qt::SkipEmptyParts);
            if( strlst.size() == 1 )
            {
                o_strMth = strlst[0];
            }
            else if( strlst.size() == 2 )
            {
                o_strObjName = strlst[0];
                o_strMth = strlst[1];
            }
            if( !o_strMth.isEmpty() )
            {
                int idx1 = o_strMth.indexOf("(");
                int idx2 = o_strMth.indexOf(")");
                if( idx1 >= 0 && idx2 > 1 )
                {
                    QString strArgsRet = o_strMth.mid(idx1);
                    o_strMth = o_strMth.mid(0, idx1);
                    strlst = strArgsRet.split(": ", Qt::SkipEmptyParts);
                    if( strlst.size() == 1 )
                    {
                        strlst[0].remove("(").remove(")");
                        o_strlstInArgs = strlst[0].split(",", Qt::SkipEmptyParts);
                    }
                    else if( strlst.size() == 2 )
                    {
                        strlst[0].remove("(").remove(")");
                        o_strlstInArgs = strlst[0].split(",", Qt::SkipEmptyParts);
                        o_strMthRet = strlst[1];
                    }
                }
            }
        }
    }
} // splitMethodCallOperation
