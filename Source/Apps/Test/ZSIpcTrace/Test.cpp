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

#include <QtCore/qfile.h>
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
#include "ZSTest/ZSTestStepAdminObjPool.h"
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
    m_pTestModule1(nullptr),
    m_pTestModule2(nullptr)
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

    // TODO Test modules with admin objects and tracing

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


    // TODO Test modules with admin objects and tracing

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
    //    /* strOperation    */ "new CTestModule1()",
    //    /* pTSGrpParent    */ nullptr,
    //    /* szDoTestStepFct */ SLOT(doTestStepCreateModule1(ZS::Test::CTestStep*)) );

    //new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(++idxStep) + " Create Module 2",
    //    /* strOperation    */ "new CTestModule2()",
    //    /* pTSGrpParent    */ nullptr,
    //    /* szDoTestStepFct */ SLOT(doTestStepCreateModule2(ZS::Test::CTestStep*)) );

    //new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(++idxStep) + " Delete Module 2",
    //    /* strOperation    */ "delete CTestModule2()",
    //    /* pTSGrpParent    */ nullptr,
    //    /* szDoTestStepFct */ SLOT(doTestStepDeleteModule2(ZS::Test::CTestStep*)) );

    //new ZS::Test::CTestStep(
    //    /* pTest           */ this,
    //    /* strName         */ "Step " + QString::number(++idxStep) + " Delete Module 1",
    //    /* strOperation    */ "delete CTestModule1()",
    //    /* pTSGrpParent    */ nullptr,
    //    /* szDoTestStepFct */ SLOT(doTestStepDeleteModule1(ZS::Test::CTestStep*)) );

    // Recall test admin object settings
    //----------------------------------

    m_pAdminObjPool->read_();

} // default ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
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

    m_pAdminObjPool->save_();

    m_pTestTrcServer = nullptr;
    m_pTestTrcClient = nullptr;
    m_pTmrTestStepTimeout = nullptr;
    m_hshReqsInProgress.clear();
    m_pTestModule1 = nullptr;
    m_pTestModule2 = nullptr;

} // dtor

/*==============================================================================
public slots: // test step methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerCreateInstance( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    i_pTestStep->setDesiredValue("TestTrcServer Created");

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
            strlstActualValues.append("Exception thrown on CIpcTrcServer::CreateInstance");
            pTrcServer = nullptr;
        }
    }

    if( pTrcServer != nullptr )
    {
        strlstActualValues.append("TestTrcServer Created");
    }
    else
    {
        strlstActualValues.append("TestTrcServer Not Created");
    }

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepTraceServerCreateInstance

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerReleaseInstance( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    i_pTestStep->setDesiredValue("TestTrcServer Released");

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
            strlstActualValues.append("Exception thrown on CIpcTrcServer::ReleaseInstance");
        }
        m_pTestTrcServer = nullptr;

        pTrcServer = ZS::Trace::CIpcTrcServer::GetInstance("TestTrcServer");
    }

    if( pTrcServer == nullptr )
    {
        strlstActualValues.append("TestTrcServer Released");
    }
    else
    {
        strlstActualValues.append("TestTrcServer Not Released");
    }

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepTraceServerReleaseInstance

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerRecallAdminObjs( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
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

    QString strAppNameNormalized = QCoreApplication::applicationName();

    // The application name may contain characters which are invalid in file names:
    strAppNameNormalized.remove(":");
    strAppNameNormalized.remove(" ");
    strAppNameNormalized.remove("\\");
    strAppNameNormalized.remove("/");
    strAppNameNormalized.remove("<");
    strAppNameNormalized.remove(">");

    QString strAppConfigDir = ZS::System::getAppConfigDir(strIniFileScope);

    QString strTrcAdminObjFileSuffix = "xml";
    QString strTrcAdminObjFileBaseName = strAppNameNormalized + "-TrcMthAdmObj";

    QString strTrcAdminObjFileAbsFilePath = strAppConfigDir + "/" + strTrcAdminObjFileBaseName + "." + strTrcAdminObjFileSuffix;

    QFile fileTrcAdminObjFile(strTrcAdminObjFileAbsFilePath);

    errResultInfo = SErrResultInfo(pTrcServer->nameSpace(), pTrcServer->className(), pTrcServer->objectName(), "recall");

    if( !fileTrcAdminObjFile.exists() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultFileOpenForRead);
        errResultInfo.setAddErrInfoDscr(strTrcAdminObjFileAbsFilePath);
    }

    strDesiredValue = errResultInfo.toString();
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    i_pTestStep->setDesiredValue(result2Str(ZS::System::EResultSuccess));

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
        strlstActualValues.append("Trace server not found");
    }
    else
    {
        SErrResultInfo errResultInfo = pTrcServer->recallAdminObjs();
        strlstActualValues.append(errResultInfo.getResultStr());
    }

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepTraceServerRecallAdminObjs

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerSaveAdminObjs( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strDesiredValue = errResultInfo.toString();
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValue(result2Str(ZS::System::EResultSuccess));

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
        strlstActualValues.append("Trace server not found");
    }
    else
    {
        SErrResultInfo errResultInfo = pTrcServer->saveAdminObjs();
        strlstActualValues.append(errResultInfo.getResultStr());
    }

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepTraceServerSaveAdminObjs

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
        i_pTestStep->setDesiredValue("Invalid test operation");
    }
    else
    {
        pTrcServer->clearLocalTrcFile();

        ZS::Ipc::SServerHostSettings hostSettings = pTrcServer->getHostSettings();

        strDesiredValue = ZS::Ipc::CServer::State2Str(Ipc::CServer::EStateListening) + " (";
        strDesiredValue += hostSettings.m_strLocalHostName;
        strDesiredValue += ":" + QString::number(hostSettings.m_uLocalPort);
        strDesiredValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
        strDesiredValue += ")";
        strlstDesiredValues.append(strDesiredValue);

        i_pTestStep->setDesiredValues(strlstDesiredValues);
    }

    // Test Step
    //----------

    if( pTrcServer == nullptr )
    {
        i_pTestStep->setActualValue("Trace server not found");
    }
    else
    {
        CRequest* pReq = pTrcServer->startup(5000);

        if( isAsynchronousRequest(pReq) )
        {
            strActualValue = "startupTrcServer not expected to be asynchronous";
        }
        else // if( isAsynchronousRequest(pReq) )
        {
            ZS::Ipc::SServerHostSettings hostSettings = pTrcServer->getHostSettings();

            strActualValue = ZS::Ipc::CServer::State2Str(pTrcServer->getState()) + " (";
            strActualValue += hostSettings.m_strLocalHostName;
            strActualValue += ":" + QString::number(hostSettings.m_uLocalPort);
            strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
            strActualValue += ")";
        }
        strlstActualValues.append(strActualValue);
    }

    i_pTestStep->setActualValues(strlstActualValues);

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
        i_pTestStep->setDesiredValue("Invalid test operation");
    }
    else
    {
        ZS::Ipc::SServerHostSettings hostSettings = pTrcServer->getHostSettings();

        strDesiredValue = ZS::Ipc::CServer::State2Str(Ipc::CServer::EStateIdle) + " (";
        strDesiredValue += hostSettings.m_strLocalHostName;
        strDesiredValue += ":" + QString::number(hostSettings.m_uLocalPort);
        strDesiredValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
        strDesiredValue += ")";
        strlstDesiredValues.append(strDesiredValue);

        i_pTestStep->setDesiredValues(strlstDesiredValues);
    }

    // Test Step
    //----------

    if( pTrcServer == nullptr )
    {
        i_pTestStep->setActualValue("Trace server not found");
    }
    else
    {
        CRequest* pReq = pTrcServer->shutdown();

        if( isAsynchronousRequest(pReq) )
        {
            strActualValue = "shutdownTrcServer not expected to be asynchronous";
        }
        else // if( isAsynchronousRequest(pReq) )
        {
            ZS::Ipc::SServerHostSettings hostSettings = pTrcServer->getHostSettings();

            strActualValue = ZS::Ipc::CServer::State2Str(pTrcServer->getState()) + " (";
            strActualValue += hostSettings.m_strLocalHostName;
            strActualValue += ":" + QString::number(hostSettings.m_uLocalPort);
            strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
            strActualValue += ")";
        }
        strlstActualValues.append(strActualValue);

        i_pTestStep->setActualValues(strlstActualValues);
    }

} // doTestStepTraceServerShutdown

//------------------------------------------------------------------------------
void CTest::doTestStepTraceClientCreate( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    i_pTestStep->setDesiredValue("TestTrcClient Created");

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
        strlstActualValues.append("TestTrcClient Created");
    }
    else
    {
        strlstActualValues.append("TestTrcClient Not Created");
    }

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepTraceClientCreate

//------------------------------------------------------------------------------
void CTest::doTestStepTraceClientDestroy( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    i_pTestStep->setDesiredValue("TestTrcClient Destroyed");

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
            strlstActualValues.append("Exception thrown on deleting TestTrcClient");
        }
        pTrcClient = m_pTestTrcClient = nullptr;
    }

    if( pTrcClient == nullptr )
    {
        strlstActualValues.append("TestTrcClient Destroyed");
    }
    else
    {
        strlstActualValues.append("TestTrcClient Not Destroyed");
    }

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepTraceClientDestroy

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

    QVector<CAbstractIdxTreeEntry*> arpTreeEntriesServer;

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
        i_pTestStep->setDesiredValue("Invalid test operation");
    }
    else
    {
        ZS::Ipc::SClientHostSettings hostSettings = pTrcClient->getHostSettings();

        strDesiredValue = ZS::Ipc::CClient::State2Str(Ipc::CClient::EStateConnected) + " (";
        strDesiredValue += hostSettings.m_strRemoteHostName;
        strDesiredValue += ":" + QString::number(hostSettings.m_uRemotePort);
        strDesiredValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
        strDesiredValue += ")";
        strlstDesiredValues.append(strDesiredValue);

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
                    strDesiredValue = pTrcAdminObj->keyInTree() + ": ";
                    strDesiredValue += "RefCount: " + QString::number(pTrcAdminObj->getRefCount());
                    strDesiredValue += ", Enabled: " + CEnumEnabled(pTrcAdminObj->getEnabled()).toString();
                    strDesiredValue += ", DetailLevel: " + QString::number(pTrcAdminObj->getTraceDetailLevel());
                    strlstDesiredValues.append(strDesiredValue);
                }
            }
        }
        i_pTestStep->setDesiredValues(strlstDesiredValues);
    }

    // Test Step
    //----------

    if( pTrcClient == nullptr )
    {
        i_pTestStep->setActualValue("Either no trace client or no trace server not found");
    }
    else
    {
        CRequest* pReq = pTrcClient->connect_();

        if( isAsynchronousRequest(pReq) )
        {
            m_pTmrTestStepTimeout->start(5000);

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
            strActualValue = "connectTrcClient expected to be asynchronous";
            strlstActualValues.append(strActualValue);
            i_pTestStep->setActualValues(strlstActualValues);
        }
    }

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
        i_pTestStep->setDesiredValue("Invalid test operation");
    }
    else
    {
        ZS::Ipc::SClientHostSettings hostSettings = pTrcClient->getHostSettings();

        strDesiredValue = ZS::Ipc::CClient::State2Str(Ipc::CClient::EStateUnconnected) + " (";
        strDesiredValue += hostSettings.m_strRemoteHostName;
        strDesiredValue += ":" + QString::number(hostSettings.m_uRemotePort);
        strDesiredValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
        strDesiredValue += ")";
        strlstDesiredValues.append(strDesiredValue);

        // We could also check whether the trace client cleared the index tree of admin objects.
        // But the order in which the onDisconnected slots are invoked when emitting the
        // IpcClient disconnected signal is unpredictable. The test may receive this signal before
        // the trace client and the trace admin object index tree may not be cleared at this time.

        //strDesiredValue = "TrcAdminObjIdxTree.isEmpty";
        //strlstDesiredValues.append(strDesiredValue);

        i_pTestStep->setDesiredValues(strlstDesiredValues);
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
            strActualValue = "disconnectTrcClient expected to be asynchronous";
            strlstActualValues.append(strActualValue);
            i_pTestStep->setActualValues(strlstActualValues);
        }
    }
    else
    {
        i_pTestStep->setActualValue("Trace client not found");
    }

} // doTestStepTraceClientDisconnect

#if 0
//------------------------------------------------------------------------------
void CTest::doTestStepCreateModule1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    QString strNameSpace  = CTestModule1::NameSpace();
    QString strClassName  = CTestModule1::ClassName();
    QString strObjName    = "TestModule1";
    QString strMthName    = "ctor";
    QString strMthInArgs  = "ObjName: TestModule1, TestModule2ObjName: TestModule2";
    QString strMthOutArgs = "";

    QString strTrcMethodEnter = "-> <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthInArgs + ")";
    QString strTrcMethodLeave = "<- <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthOutArgs + ")";

    strlstDesiredValues.append(strTrcMethodEnter);
    strlstDesiredValues.append(strTrcMethodLeave);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    m_pTestModule1 = new CTestModule1("TestModule1", "TestModule2");

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
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    QString strNameSpace  = CTestModule1::NameSpace();
    QString strClassName  = CTestModule1::ClassName();
    QString strObjName    = "TestModule1";
    QString strMthName    = "dtor";
    QString strMthInArgs  = "";
    QString strMthOutArgs = "";

    QString strTrcMethodEnter = "-> <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthInArgs + ")";
    QString strTrcMethodLeave = "<- <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthOutArgs + ")";

    strlstDesiredValues.append(strTrcMethodEnter);
    strlstDesiredValues.append(strTrcMethodLeave);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    delete m_pTestModule1;
    m_pTestModule1 = nullptr;

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
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    QString strNameSpace  = CTestModule2::NameSpace();
    QString strClassName  = CTestModule2::ClassName();
    QString strObjName    = "TestModule2";
    QString strMthName    = "event";
    QString strMthInArgs  = "Msg: ZS::Apps::Test::IpcTrace::ReqTest";
    QString strMthOutArgs = "";
    QString strMthRet     = "100";
    QString strMthTrace;

    QString strTrcMethodLeave = "<- <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthOutArgs + ")";
    if( !strMthRet.isEmpty() ) strTrcMethodLeave += ": " + strMthRet;
    strlstDesiredValues.append(strTrcMethodLeave);

    strMthTrace = "   -> <" + strNameSpace + "::" + strClassName + "> TestModule2.ctor(TestModule2Thread: TestModule2, ObjName: TestModule2)";
    strlstDesiredValues.append(strMthTrace);

    strMthTrace = "   <- <" + strNameSpace + "::" + strClassName + "> TestModule2.ctor()";
    strlstDesiredValues.append(strMthTrace);

    strMthTrace = "                                 <- <" + strNameSpace + "::" + strClassName + "> TestModule2.recursiveTraceMethod(): 10";
    strlstDesiredValues.append(strMthTrace);

    strMthTrace = "   <- <" + strNameSpace + "::" + strClassName + "> TestModule2.recursiveTraceMethod(): 0";
    strlstDesiredValues.append(strMthTrace);

    strMthTrace = "   -> <" + strNameSpace + "::" + strClassName + "> TestModule2.event(Msg: ZS::Apps::Test::IpcTrace::ReqTest)";
    strlstDesiredValues.append(strMthTrace);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    if( m_pTestModule1 != nullptr )
    {
        m_pTestModule2 = m_pTestModule1->createModule2();
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
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    QString strNameSpace  = CTestModule2::NameSpace();
    QString strClassName  = CTestModule2::ClassName();
    QString strObjName    = "TestModule2";
    QString strMthName    = "dtor";
    QString strMthInArgs  = "";
    QString strMthOutArgs = "";

    QString strTrcMethodLeave = "<- <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthOutArgs + ")";

    strlstDesiredValues.append(strTrcMethodLeave);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    if( m_pTestModule1 != nullptr )
    {
        m_pTestModule1->deleteModule2();
    }
    m_pTestModule2 = nullptr;

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
                QVector<CAbstractIdxTreeEntry*> arpTreeEntriesServer;

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
                            QString     strActualValue;
                            QStringList strlstActualValues;

                            ZS::Ipc::SClientHostSettings hostSettings = pTrcClient->getHostSettings();

                            strActualValue = ZS::Ipc::CClient::State2Str(pTrcClient->getState()) + " (";
                            strActualValue += hostSettings.m_strRemoteHostName;
                            strActualValue += ":" + QString::number(hostSettings.m_uRemotePort);
                            strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                            strActualValue += ")";
                            strlstActualValues.append(strActualValue);

                            pTestStep->setActualValues(strlstActualValues);
                        }
                    } // if( pTestStep->getOperation().contains("TrcClient::connect") )

                    else if( pTestStep->getOperation().contains("TrcClient::disconnect") )
                    {
                        QString     strActualValue;
                        QStringList strlstActualValues;

                        ZS::Ipc::SClientHostSettings hostSettings = pTrcClient->getHostSettings();

                        strActualValue = ZS::Ipc::CClient::State2Str(pTrcClient->getState()) + " (";
                        strActualValue += hostSettings.m_strRemoteHostName;
                        strActualValue += ":" + QString::number(hostSettings.m_uRemotePort);
                        strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                        strActualValue += ")";
                        strlstActualValues.append(strActualValue);

                        // We could also check whether the trace client cleared the index tree of admin objects.
                        // But the order in which the onDisconnected slots are invoked when emitting the
                        // IpcClient disconnected signal is unpredictable. The test may receive this signal before
                        // the trace client and the trace admin object index tree may not be cleared at this time.

                        pTestStep->setActualValues(strlstActualValues);

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
        QVector<CAbstractIdxTreeEntry*> arpTreeEntriesServer;

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

                QVector<CAbstractIdxTreeEntry*> arpTreeEntriesClient = pIdxTreeClient->treeEntriesVec();

                bool bTestStepFinished = false;

                if( pTrcClient->isConnected() && arpTreeEntriesClient.size() == arpTreeEntriesServer.size() )
                {
                    bTestStepFinished = true;

                    CAbstractIdxTreeEntry* pTreeEntryServer;
                    CAbstractIdxTreeEntry* pTreeEntryClient;

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

                    QString     strActualValue;
                    QStringList strlstActualValues;

                    ZS::Ipc::SClientHostSettings hostSettings = pTrcClient->getHostSettings();

                    strActualValue = ZS::Ipc::CClient::State2Str(pTrcClient->getState()) + " (";
                    strActualValue += hostSettings.m_strRemoteHostName;
                    strActualValue += ":" + QString::number(hostSettings.m_uRemotePort);
                    strActualValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                    strActualValue += ")";
                    strlstActualValues.append(strActualValue);

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
                                strActualValue = pTrcAdminObj->keyInTree() + ": ";
                                strActualValue += "RefCount: " + QString::number(pTrcAdminObj->getRefCount());
                                strActualValue += ", Enabled: " + CEnumEnabled(pTrcAdminObj->getEnabled()).toString();
                                strActualValue += ", DetailLevel: " + QString::number(pTrcAdminObj->getTraceDetailLevel());
                                strlstActualValues.append(strActualValue);
                            }
                        }
                    }

                    pTestStep->setActualValues(strlstActualValues);

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
        if( pTestStep->getOperation() == "new CTestModule1()" || pTestStep->getOperation() == "delete CTestModule1()" )
        {
            QString strText = i_strText;

            strText.replace("&lt;", "<");
            strText.replace("&gt;", ">");
            strText.replace("&nbsp;", "");

            // Two entries must have been added to finish the test step: Enter and Leave Method.
            // The test step is finished if the Method Leave is traced.
            if( strText.contains("<-") )
            {
                QString strNameSpace = "ZS::Apps::Test::IpcTrace";
                QString strClassName = "CTestModule1";
                QString strObjName   = "TestModule1";

                QString strMthName;
                QString strMthInArgs;
                QString strMthOutArgs;

                if( pTestStep->getOperation() == "new CTestModule1()" )
                {
                    strMthName   = "ctor";
                    strMthInArgs = "ObjName: TestModule1, TestModule2ObjName: TestModule2";
                }
                else if( pTestStep->getOperation() == "delete CTestModule1()" )
                {
                    strMthName = "dtor";
                }

                QString strTrcMethodEnter = "-> <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthInArgs + ")";
                QString strTrcMethodLeave = "<- <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthOutArgs + ")";

                if( strText.contains(strNameSpace) && strText.contains(strClassName) && strText.contains(strObjName) && strText.contains(strMthName) )
                {
                    QString     strActualValue;
                    QStringList strlstActualValues;

                    CWidgetCentral* pWdgtCentral = CWidgetCentral::GetInstance();

                    CWdgtTrcMthList* pWdgtTrcMthList = pWdgtCentral->getTrcMthListWdgt();

                    QTextEdit* pEdtTrcMthList = pWdgtTrcMthList->getTextEdit();

                    QTextDocument* pDocTrcMthList = pEdtTrcMthList->document();

                    if( m_pTmrTestStepTimeout->isActive() )
                    {
                        m_pTmrTestStepTimeout->stop();
                    }

                    QObject::disconnect(
                        /* pObjSender   */ pWdgtTrcMthList,
                        /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onZSTraceClientTrcMthListWdgtTextItemAdded(const QString&)) );

                    QTextCursor textCursor = pDocTrcMthList->find(strTrcMethodEnter);

                    if( !textCursor.isNull() ) strlstActualValues.append(strTrcMethodEnter);
                    else strlstActualValues.append(strTrcMethodEnter + ": not found in text edit widget");

                    textCursor = pDocTrcMthList->find(strTrcMethodLeave);

                    if( !textCursor.isNull() ) strlstActualValues.append(strTrcMethodLeave);
                    else strlstActualValues.append(strTrcMethodLeave + ": not found in text edit widget");

                    // Range of IniFileScope: ["AppDir", "User", "System"]
                    #ifdef __linux__
                    // Using "System" on linux Mint ends up in directory "etc/xdg/<CompanyName>"
                    // where the application has not write access rights. Stupid ...
                    QString strIniFileScope = "User";
                    #else
                    QString strIniFileScope = "System"; // Default
                    #endif

                    QString strAppNameNormalized = QCoreApplication::applicationName();

                    // The application name may contain characters which are invalid in file names:
                    strAppNameNormalized.remove(":");
                    strAppNameNormalized.remove(" ");
                    strAppNameNormalized.remove("\\");
                    strAppNameNormalized.remove("/");
                    strAppNameNormalized.remove("<");
                    strAppNameNormalized.remove(">");

                    QString strAppLogDir = ZS::System::getAppLogDir(strIniFileScope);

                    QString strTrcMthFileSuffix = "log";
                    QString strTrcMthFileBaseName = strAppNameNormalized + "-TrcMth00";

                    QString strTrcMthFileAbsFilePath = strAppLogDir + "/" + strTrcMthFileBaseName + "." + strTrcMthFileSuffix;

                    QFile fileTrcMthFile(strTrcMthFileAbsFilePath);

                    if( !fileTrcMthFile.open(QIODevice::ReadOnly|QIODevice::Text) )
                    {
                        strlstActualValues.append("Could not open file " + strTrcMthFileAbsFilePath);
                    }
                    else
                    {
                        QTextStream txtstrmLogFileAppDefault(&fileTrcMthFile);

                        QString strTrcMthFile = txtstrmLogFileAppDefault.readAll();

                        bool bTrcMethodEnterFound = strTrcMthFile.contains(strTrcMethodEnter);
                        bool bTrcMethodLeaveFound = strTrcMthFile.contains(strTrcMethodLeave);

                        if( !bTrcMethodEnterFound ) strlstActualValues.append(strTrcMethodEnter + ": not found in trace method file");
                        if( !bTrcMethodLeaveFound ) strlstActualValues.append(strTrcMethodLeave + ": not found in trace method file");
                    }

                    pTestStep->setActualValues(strlstActualValues);

                } // if( strText.contains(strNameSpace) && strText.contains(strClassName) && strText.contains(strObjName) && strText.contains(strMthName) )
            } // if( strText.contains("<-") )
        } // if( pTestStep->getOperation() == "new CTestModule1()" || pTestStep->getOperation() == "delete CTestModule1()" )

        else if( pTestStep->getOperation() == "new CTestModule2()" || pTestStep->getOperation() == "delete CTestModule2()" )
        {
            QString strText = i_strText;

            strText.replace("&lt;", "<");
            strText.replace("&gt;", ">");
            strText.replace("&nbsp;", "");

            // Two entries must have been added to finish the test step: Enter and Leave Method.
            // The test step is finished if the Method Leave is traced.
            if( strText.contains("<-") )
            {
                QString strNameSpace = "ZS::Apps::Test::IpcTrace";
                QString strClassName = "CTestModule2";
                QString strObjName   = "TestModule2";

                QString strMthName;
                QString strMthInArgs;
                QString strMthOutArgs;
                QString strMthRet;

                QString strTrc1;
                QString strTrc2;
                QString strTrc3;
                QString strTrc4;
                QString strTrc5;

                if( pTestStep->getOperation() == "new CTestModule2()" )
                {
                    strMthName   = "event";
                    strMthInArgs = "Msg: ZS::Apps::Test::IpcTrace::ReqTest";
                    strMthRet    = "100";

                    strTrc1 = "   -> <ZS::Apps::Test::IpcTrace::CTestModule2> TestModule2.ctor(TestModule2Thread: TestModule2, ObjName: TestModule2)";
                    strTrc2 = "   <- <ZS::Apps::Test::IpcTrace::CTestModule2> TestModule2.ctor()";
                    strTrc3 = "                                 <- <ZS::Apps::Test::IpcTrace::CTestModule2> TestModule2.recursiveTraceMethod(): 10";
                    strTrc4 = "   <- <ZS::Apps::Test::IpcTrace::CTestModule2> TestModule2.recursiveTraceMethod(): 0";
                    strTrc5 = "   -> <ZS::Apps::Test::IpcTrace::CTestModule2> TestModule2.event(Msg: ZS::Apps::Test::IpcTrace::ReqTest)";
                }
                else if( pTestStep->getOperation() == "delete CTestModule2()" )
                {
                    strMthName = "dtor";
                }

                QString strTrcMethodLeave = "<- <" + strNameSpace + "::" + strClassName + "> " + strObjName + "." + strMthName + "(" + strMthOutArgs + ")";

                if( !strMthRet.isEmpty() ) strTrcMethodLeave += ": " + strMthRet;

                if( strText.contains(strNameSpace) && strText.contains(strClassName) && strText.contains(strObjName) && strText.contains(strMthName) && strText.contains(strMthRet) )
                {
                    QString     strActualValue;
                    QStringList strlstActualValues;

                    CWidgetCentral* pWdgtCentral = CWidgetCentral::GetInstance();

                    CWdgtTrcMthList* pWdgtTrcMthList = pWdgtCentral->getTrcMthListWdgt();

                    QTextEdit* pEdtTrcMthList = pWdgtTrcMthList->getTextEdit();

                    QTextDocument* pDocTrcMthList = pEdtTrcMthList->document();

                    if( m_pTmrTestStepTimeout->isActive() )
                    {
                        m_pTmrTestStepTimeout->stop();
                    }

                    QObject::disconnect(
                        /* pObjSender   */ pWdgtTrcMthList,
                        /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onZSTraceClientTrcMthListWdgtTextItemAdded(const QString&)) );

                    QTextCursor textCursor = pDocTrcMthList->find(strTrcMethodLeave);

                    if( !textCursor.isNull() ) strlstActualValues.append(strTrcMethodLeave);
                    else strlstActualValues.append(strTrcMethodLeave + ": not found in text edit widget");

                    if( !strTrc1.isEmpty() )
                    {
                        textCursor = pDocTrcMthList->find(strTrc1);
                        if( !textCursor.isNull() ) strlstActualValues.append(strTrc1);
                        else strlstActualValues.append(strTrc1 + ": not found in text edit widget");
                    }
                    if( !strTrc2.isEmpty() )
                    {
                        textCursor = pDocTrcMthList->find(strTrc2);
                        if( !textCursor.isNull() ) strlstActualValues.append(strTrc2);
                        else strlstActualValues.append(strTrc2 + ": not found in text edit widget");
                    }
                    if( !strTrc3.isEmpty() )
                    {
                        textCursor = pDocTrcMthList->find(strTrc3);
                        if( !textCursor.isNull() ) strlstActualValues.append(strTrc3);
                        else strlstActualValues.append(strTrc3 + ": not found in text edit widget");
                    }
                    if( !strTrc4.isEmpty() )
                    {
                        textCursor = pDocTrcMthList->find(strTrc4);
                        if( !textCursor.isNull() ) strlstActualValues.append(strTrc4);
                        else strlstActualValues.append(strTrc4 + ": not found in text edit widget");
                    }
                    if( !strTrc5.isEmpty() )
                    {
                        textCursor = pDocTrcMthList->find(strTrc5);
                        if( !textCursor.isNull() ) strlstActualValues.append(strTrc5);
                        else strlstActualValues.append(strTrc5 + ": not found in text edit widget");
                    }

                    // Range of IniFileScope: ["AppDir", "User", "System"]
                    #ifdef __linux__
                    // Using "System" on linux Mint ends up in directory "etc/xdg/<CompanyName>"
                    // where the application has not write access rights. Stupid ...
                    QString strIniFileScope = "User";
                    #else
                    QString strIniFileScope = "System"; // Default
                    #endif

                    QString strAppNameNormalized = QCoreApplication::applicationName();

                    // The application name may contain characters which are invalid in file names:
                    strAppNameNormalized.remove(":");
                    strAppNameNormalized.remove(" ");
                    strAppNameNormalized.remove("\\");
                    strAppNameNormalized.remove("/");
                    strAppNameNormalized.remove("<");
                    strAppNameNormalized.remove(">");

                    QString strAppLogDir = ZS::System::getAppLogDir(strIniFileScope);

                    QString strTrcMthFileSuffix = "log";
                    QString strTrcMthFileBaseName = strAppNameNormalized + "-TrcMth00";

                    QString strTrcMthFileAbsFilePath = strAppLogDir + "/" + strTrcMthFileBaseName + "." + strTrcMthFileSuffix;

                    QFile fileTrcMthFile(strTrcMthFileAbsFilePath);

                    if( !fileTrcMthFile.open(QIODevice::ReadOnly|QIODevice::Text) )
                    {
                        strlstActualValues.append("Could not open file " + strTrcMthFileAbsFilePath);
                    }
                    else
                    {
                        QTextStream txtstrmLogFileAppDefault(&fileTrcMthFile);

                        QString strTrcMthFile = txtstrmLogFileAppDefault.readAll();

                        bool bStringFound = strTrcMthFile.contains(strTrcMethodLeave);

                        if( !bStringFound ) strlstActualValues.append(strTrcMethodLeave + ": not found in trace method file");

                        if( !strTrc1.isEmpty() )
                        {
                            bStringFound = strTrcMthFile.contains(strTrc1);
                            if( !bStringFound ) strlstActualValues.append(strTrc1 + ": not found in trace method file");
                        }
                        if( !strTrc2.isEmpty() )
                        {
                            bStringFound = strTrcMthFile.contains(strTrc2);
                            if( !bStringFound ) strlstActualValues.append(strTrc2 + ": not found in trace method file");
                        }
                        if( !strTrc3.isEmpty() )
                        {
                            bStringFound = strTrcMthFile.contains(strTrc3);
                            if( !bStringFound ) strlstActualValues.append(strTrc3 + ": not found in trace method file");
                        }
                        if( !strTrc4.isEmpty() )
                        {
                            bStringFound = strTrcMthFile.contains(strTrc4);
                            if( !bStringFound ) strlstActualValues.append(strTrc4 + ": not found in trace method file");
                        }
                        if( !strTrc5.isEmpty() )
                        {
                            bStringFound = strTrcMthFile.contains(strTrc5);
                            if( !bStringFound ) strlstActualValues.append(strTrc5 + ": not found in trace method file");
                        }
                    }

                    pTestStep->setActualValues(strlstActualValues);

                } // if( strText.contains(strNameSpace) && strText.contains(strClassName) && strText.contains(strObjName) && strText.contains(strMthName) )
            } // if( strText.contains("<-") )
        } // if( pTestStep->getOperation() == "new CTestModule2()" || pTestStep->getOperation() == "delete CTestModule2()" )
    } // if( pTestStep != nullptr )

} // onZSTraceClientTrcMthListWdgtTextItemAdded

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

        pTestStep->setActualValues(strlstActualValues);

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
