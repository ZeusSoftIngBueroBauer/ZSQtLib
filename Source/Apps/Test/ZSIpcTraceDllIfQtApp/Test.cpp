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

#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qtimer.h>

#include "Test.h"
#include "TestConfig.h"
#include "MyClass1.h"
#include "MyClass2.h"
#include "MyClass3.h"
#include "MainWindow.h"

#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStepIdxTree.h"
#include "ZSTestGUI/ZSTestStepDlg.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Test::GUI;
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
    m_pDlgTestStep(nullptr),
    m_pTmrTestStepTimeout(nullptr),
    m_trcSettings(),
    m_uTrcServerPort(24763),
    //m_iTrcDetailLevelTrcServer(ZS::Trace::ETraceDetailLevelNone),
    m_pTrcServer(nullptr),
    m_hshpMyClass1InstancesByName(),
    m_hshpMyClass2InstancesByName(),
    m_hshpMyClass3InstancesByName(),
    m_multihshpMyClass1InstancesByName(),
    m_multihshpMyClass2InstancesByName(),
    m_multihshpMyClass3InstancesByName()
{
    //ZS::Trace::DllIf::STrcServerSettings_init(m_trcSettings);

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

    ZS::Test::CTestStepGroup* pTestGroup = nullptr;
    ZS::Test::CTestStep* pTestStep = nullptr;

    // Test Step Group - One Thread
    //=============================

    int idxGroup = 0;
    int idxStep = 0;

    pTestGroup = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " One Thread",
        /* pTSGrpParent    */ nullptr );

    // Test Step Group - One Thread - Startup
    //----------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Load ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::loadDll",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepLoadDll(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Create Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::CreateInstance",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerCreateInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Startup",
        /* strOperation    */ "DllIf::CIpcTrcServer::startup",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerStartup(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Recall Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::recallAdminObjs",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerRecallAdminObjs(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Connect",
        /* strOperation    */ "CIpcTrcClient::connect",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientConnect(ZS::Test::CTestStep*)) );

    // Test Step Group - One Thread - Method Tracing - Class 1
    //--------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::classMethod",
        /* strOperation    */ "CMyClass1::classMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    // Test Step Group - One Thread - Method Tracing - Class 2
    //--------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(Inst1, nullptr)",
        /* strOperation    */ "CMyClass2::ctor(Inst1, nullptr)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(Inst2, nullptr)",
        /* strOperation    */ "CMyClass2::ctor(Inst2, nullptr)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-ctor-Inst2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.instMethod",
        /* strOperation    */ "CMyClass2::Inst1.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst2.instMethod",
        /* strOperation    */ "CMyClass2::Inst2.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::dtor(Inst1)",
        /* strOperation    */ "CMyClass2::dtor(Inst1)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-dtor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::dtor(Inst2)",
        /* strOperation    */ "CMyClass2::dtor(Inst2)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-dtor-Inst2");

    // Test Step Group - One Thread - Method Tracing - Class 2 - Logically Grouped Objects
    //------------------------------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(SoundCard-PowerLevel, nullptr)",
        /* strOperation    */ "CMyClass2::ctor(SoundCard-PowerLevel, nullptr)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-ctor-SoundCard-PowerLevel");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(Equalizer-PowerLevel, nullptr)",
        /* strOperation    */ "CMyClass2::ctor(Equalizer-PowerLevel, nullptr)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-ctor-Equalizer-PowerLevel");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::SoundCard-PowerLevel.instMethod",
        /* strOperation    */ "CMyClass2::SoundCard-PowerLevel.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Equalizer-PowerLevel.instMethod",
        /* strOperation    */ "CMyClass2::Equalizer-PowerLevel.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::dtor(SoundCard-PowerLevel)",
        /* strOperation    */ "CMyClass2::dtor(SoundCard-PowerLevel)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-dtor-SoundCard-PowerLevel");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::dtor(Equalizer-PowerLevel)",
        /* strOperation    */ "CMyClass2::dtor(Equalizer-PowerLevel)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-dtor-Equalizer-PowerLevel");

    // Test Step Group - One Thread - Method Tracing - Class 3
    //---------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::classMethod",
        /* strOperation    */ "CMyClass3::classMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::noisyClassMethod",
        /* strOperation    */ "CMyClass3::NoisyMethods::noisyClassMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::veryNoisyClassMethod",
        /* strOperation    */ "CMyClass3::VeryNoisyMethods::veryNoisyClassMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::ctor(Inst1, nullptr)",
        /* strOperation    */ "CMyClass3::ctor(Inst1, nullptr)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass3-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::ctor(Inst2, nullptr)",
        /* strOperation    */ "CMyClass3::ctor(Inst2, nullptr)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass3-ctor-Inst2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst1.instMethod",
        /* strOperation    */ "CMyClass3::Inst1.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst2.instMethod",
        /* strOperation    */ "CMyClass3::Inst2.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst1.noisyInstMethod",
        /* strOperation    */ "CMyClass3::NoisyMethods::Inst1.noisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst2.noisyInstMethod",
        /* strOperation    */ "CMyClass3::NoisyMethods::Inst2.noisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst1.veryNoisyInstMethod",
        /* strOperation    */ "CMyClass3::VeryNoisyMethods::Inst1.veryNoisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst2.veryNoisyInstMethod",
        /* strOperation    */ "CMyClass3::VeryNoisyMethods::Inst2.veryNoisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::dtor(Inst1)",
        /* strOperation    */ "CMyClass3::dtor(Inst1)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass3-dtor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::dtor(Inst2)",
        /* strOperation    */ "CMyClass3::dtor(Inst2)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass3-dtor-Inst2");

    // Test Step Group - One Thread - Shutdown
    //----------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Disconnect",
        /* strOperation    */ "CIpcTrcClient::disconnect",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientDisconnect(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Save Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::saveAdminObjs",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerSaveAdminObjs(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Shutdown",
        /* strOperation    */ "DllIf::CIpcTrcServer::shutdown",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerShutdown(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Release Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::ReleaseInstance",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerReleaseInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::releaseDll",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepReleaseDll(ZS::Test::CTestStep*)) );

    // Test Step Group - Several Threads
    //==================================

    idxStep = 0;

    pTestGroup = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " Several Threads",
        /* pTSGrpParent    */ nullptr );

    // Test Step Group - Several Threads - Startup
    //----------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Load ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::loadDll",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepLoadDll(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Create Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::CreateInstance",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerCreateInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Startup",
        /* strOperation    */ "DllIf::CIpcTrcServer::startup",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerStartup(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Recall Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::recallAdminObjs",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerRecallAdminObjs(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Connect",
        /* strOperation    */ "CIpcTrcClient::connect",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientConnect(ZS::Test::CTestStep*)) );

    // Test Step Group - Several Threads - Method Tracing - Class 1 - StartClass2Thread
    //---------------------------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::ctor(Inst1)",
        /* strOperation    */ "CMyClass1::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::Inst1.startClass2Thread(Inst1)",
        /* strOperation    */ "CMyClass1::Inst1.startClass2Thread(Inst1): Inst1",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-Inst1-startClass2Thread-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.recursiveTraceMethod()",
        /* strOperation    */ "CMyClass2::Inst1.recursiveTraceMethod(): 0",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-Inst1-recursiveTraceMethod");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.startMessageTimer()",
        /* strOperation    */ "CMyClass2::Inst1.startMessageTimer()",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-Inst1-startMessageTimer");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.startClass3Thread(Inst1)",
        /* strOperation    */ "CMyClass2::Inst1.startClass3Thread(Inst1): Inst1",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-Inst1-startClass3Thread-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.stopClass3Thread()",
        /* strOperation    */ "CMyClass2::Inst1.stopClass3Thread()",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-Inst1-stopClass3Thread");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::Inst1.stopClass2Thread()",
        /* strOperation    */ "CMyClass1::Inst1.stopClass2Thread()",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-Inst1-stopClass2Thread");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass1::dtor(Inst1)",
        /* strOperation    */ "CMyClass1::dtor(Inst1)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-dtor-Inst1");

    // Test Step Group - Several Threads - Shutdown
    //-----------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Disconnect",
        /* strOperation    */ "CIpcTrcClient::disconnect",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientDisconnect(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Save Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::saveAdminObjs",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerSaveAdminObjs(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Shutdown",
        /* strOperation    */ "DllIf::CIpcTrcServer::shutdown",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerShutdown(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Release Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::ReleaseInstance",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerReleaseInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::releaseDll",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepReleaseDll(ZS::Test::CTestStep*)) );

    // Test Step Group - RenameTraceAdminObj
    //======================================

    idxStep = 0;

    pTestGroup = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " RenameTraceAdminObj",
        /* pTSGrpParent    */ nullptr );

    // Test Step Group - RenameTraceAdminObj - Startup
    //------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Load ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::loadDll",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepLoadDll(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Create Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::CreateInstance",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerCreateInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Startup",
        /* strOperation    */ "DllIf::CIpcTrcServer::startup",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerStartup(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Recall Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::recallAdminObjs",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerRecallAdminObjs(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Connect",
        /* strOperation    */ "CIpcTrcClient::connect",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientConnect(ZS::Test::CTestStep*)) );

    // Test Step Group - RenameTraceAdminObj - Several instances of class tracer
    //--------------------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::ctor(Inst1)",
        /* strOperation    */ "CMyClass1::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::ctor(Inst1)",
        /* strOperation    */ "CMyClass1::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::Inst1.setObjectName(NewName1)",
        /* strOperation    */ "CMyClass1::Inst1.setObjectName(NewName1)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-Inst1-setObjectName-NewName1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::Inst1.setObjectName(NewName2)",
        /* strOperation    */ "CMyClass1::Inst1.setObjectName(NewName2)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-Inst1-setObjectName-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::NewName1.setObjectName(NewName2)",
        /* strOperation    */ "CMyClass1::NewName1.setObjectName(NewName2)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-NewName1-setObjectName-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass1::dtor(NewName2)",
        /* strOperation    */ "CMyClass1::dtor(NewName2)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-dtor-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass1::dtor(NewName2)",
        /* strOperation    */ "CMyClass1::dtor(NewName2)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-dtor-NewName2");

    // Test Step Group - RenameTraceAdminObj - Several instances of instance tracer
    //-----------------------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(Inst1)",
        /* strOperation    */ "CMyClass2::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(Inst1)",
        /* strOperation    */ "CMyClass2::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.setObjectName(NewName1)",
        /* strOperation    */ "CMyClass2::Inst1.setObjectName(NewName1)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-Inst1-setObjectName-NewName1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.setObjectName(NewName2)",
        /* strOperation    */ "CMyClass2::Inst1.setObjectName(NewName2)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-Inst1-setObjectName-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::NewName1.setObjectName(NewName2)",
        /* strOperation    */ "CMyClass2::NewName1.setObjectName(NewName2)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-NewName1-setObjectName-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass2::dtor(NewName2)",
        /* strOperation    */ "CMyClass2::dtor(NewName2)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-dtor-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass2::dtor(NewName2)",
        /* strOperation    */ "CMyClass2::dtor(NewName2)",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-dtor-NewName2");

    // Test Step Group - RenameTraceAdminObj - Shutdown
    //--------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Disconnect",
        /* strOperation    */ "CIpcTrcClient::disconnect",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientDisconnect(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Save Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::saveAdminObjs",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerSaveAdminObjs(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Shutdown",
        /* strOperation    */ "DllIf::CIpcTrcServer::shutdown",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerShutdown(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Release Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::ReleaseInstance",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerReleaseInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::releaseDll",
        /* pTSGrpParent    */ pTestGroup,
        /* szDoTestStepFct */ SLOT(doTestStepReleaseDll(ZS::Test::CTestStep*)) );

    // Recall test step settings
    //==========================

    recallTestSteps();

} // default ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo = saveTestSteps();

    if( errResultInfo.isErrorResult() )
    {
        if(CErrLog::GetInstance() != nullptr)
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    try
    {
        delete m_pDlgTestStep;
    }
    catch(...)
    {
    }

    QString strObjName;

    for( auto& pObj : m_hshpMyClass1InstancesByName )
    {
        QObject::disconnect(
            /* pObjSender   */ pObj,
            /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onClass1AboutToBeDestroyed(QObject*, const QString&)) );

        strObjName = pObj->objectName();

        try
        {
            delete pObj;
        }
        catch(...)
        {
        }
        m_hshpMyClass1InstancesByName[strObjName] = nullptr;
    }

    for( auto& pObj : m_hshpMyClass2InstancesByName )
    {
        QObject::disconnect(
            /* pObjSender   */ pObj,
            /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onClass2AboutToBeDestroyed(QObject*, const QString&)) );

        strObjName = pObj->objectName();

        try
        {
            delete pObj;
        }
        catch(...)
        {
        }
        m_hshpMyClass2InstancesByName[strObjName] = nullptr;
    }

    for( auto& pObj : m_hshpMyClass3InstancesByName )
    {
        QObject::disconnect(
            /* pObjSender   */ pObj,
            /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onClass3AboutToBeDestroyed(QObject*, const QString&)) );

        strObjName = pObj->objectName();

        try
        {
            delete pObj;
        }
        catch(...)
        {
        }
        m_hshpMyClass3InstancesByName[strObjName] = nullptr;
    }

    while( !m_multihshpMyClass1InstancesByName.isEmpty() )
    {
        QMultiHash<QString, CMyClass1*>::iterator it = m_multihshpMyClass1InstancesByName.begin();

        CMyClass1* pObj = it.value();

        QObject::disconnect(
            /* pObjSender   */ pObj,
            /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onClass1AboutToBeDestroyed(QObject*, const QString&)) );

        strObjName = pObj->objectName();
        m_multihshpMyClass1InstancesByName.remove(strObjName, pObj);

        try
        {
            delete pObj;
        }
        catch(...)
        {
        }
    }

    while( !m_multihshpMyClass2InstancesByName.isEmpty() )
    {
        QMultiHash<QString, CMyClass2*>::iterator it = m_multihshpMyClass2InstancesByName.begin();

        CMyClass2* pObj = it.value();

        QObject::disconnect(
            /* pObjSender   */ pObj,
            /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onClass2AboutToBeDestroyed(QObject*, const QString&)) );

        strObjName = pObj->objectName();
        m_multihshpMyClass2InstancesByName.remove(strObjName, pObj);

        try
        {
            delete pObj;
        }
        catch(...)
        {
        }
    }

    while( !m_multihshpMyClass3InstancesByName.isEmpty() )
    {
        QMultiHash<QString, CMyClass3*>::iterator it = m_multihshpMyClass3InstancesByName.begin();

        CMyClass3* pObj = it.value();

        QObject::disconnect(
            /* pObjSender   */ pObj,
            /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onClass3AboutToBeDestroyed(QObject*, const QString&)) );

        strObjName = pObj->objectName();
        m_multihshpMyClass3InstancesByName.remove(strObjName, pObj);

        try
        {
            delete pObj;
        }
        catch(...)
        {
        }
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

    m_pDlgTestStep = nullptr;
    m_pTmrTestStepTimeout = nullptr;
    ZS::Trace::DllIf::STrcServerSettings_release(m_trcSettings);
    m_uTrcServerPort = 0;
    //m_iTrcDetailLevelTrcServer = 0;
    m_pTrcServer = nullptr;
    m_hshpMyClass1InstancesByName.clear();
    m_hshpMyClass2InstancesByName.clear();
    m_hshpMyClass3InstancesByName.clear();
    m_multihshpMyClass1InstancesByName.clear();
    m_multihshpMyClass2InstancesByName.clear();
    m_multihshpMyClass3InstancesByName.clear();

} // dtor

/*==============================================================================
public slots: // test step methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepLoadDll( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strExpectedValue = "LoadDll: Ok";

    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    #ifdef _DEBUG
    ZS::Trace::DllIf::EBuildConfiguration buildConfiguration = ZS::Trace::DllIf::EBuildConfigurationDebug;
    #else
    ZS::Trace::DllIf::EBuildConfiguration buildConfiguration = ZS::Trace::DllIf::EBuildConfigurationRelease;
    #endif

    bool bOk = ZS::Trace::DllIf::loadDll(buildConfiguration, QT_VERSION_MAJOR);

    // Result Values
    //---------------

    strResultValue = "LoadDll: " + QString(bOk ? "Ok" : "Failed");

    strlstResultValues.append(strResultValue);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepLoadDll

//------------------------------------------------------------------------------
void CTest::doTestStepReleaseDll( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strExpectedValue = "ReleaseDll: Ok";

    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    bool bOk = ZS::Trace::DllIf::releaseDll();

    // Result Values
    //---------------

    strResultValue = "ReleaseDll: " + QString(bOk ? "Ok" : "Failed");

    strlstResultValues.append(strResultValue);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepReleaseDll

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerCreateInstance( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strExpectedValue = "TrcServer != nullptr";

    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    m_pTrcServer = ZS::Trace::DllIf::CIpcTrcServer::CreateInstance(
        /* iTrcDetailLevelDllIf                     */ ZS::Trace::DllIf::ETraceDetailLevelMethodArgs,
        /* iTrcDetailLevelTrcServer                 */ ZS::Trace::DllIf::ETraceDetailLevelMethodArgs,
        /* iTrcDetailLevelTrcServerMutex            */ ZS::Trace::DllIf::ETraceDetailLevelNone,
        /* iTrcDetailLevelTrcServerIpcServer        */ ZS::Trace::DllIf::ETraceDetailLevelNone,
        /* iTrcDetailLevelTrcServerIpcServerMutex   */ ZS::Trace::DllIf::ETraceDetailLevelNone,
        /* iTrcDetailLevelTrcServerIpcServerGateway */ ZS::Trace::DllIf::ETraceDetailLevelNone );

    // Result Values
    //---------------

    if( m_pTrcServer == nullptr ) strResultValue = "TrcServer == nullptr";
    else strResultValue = "TrcServer != nullptr";

    strlstResultValues.append(strResultValue);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepTraceServerCreateInstance

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerReleaseInstance( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strExpectedValue = "TrcServer == nullptr";

    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    ZS::Trace::DllIf::CIpcTrcServer::ReleaseInstance(m_pTrcServer);
    m_pTrcServer = ZS::Trace::DllIf::CIpcTrcServer::GetInstance();

    // Result Values
    //---------------

    if( m_pTrcServer == nullptr ) strResultValue = "TrcServer == nullptr";
    else strResultValue = "TrcServer != nullptr";

    strlstResultValues.append(strResultValue);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepTraceServerReleaseInstance

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

    strExpectedValue = "Listening";

    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    bool bOk = false;

    if( m_pTrcServer != nullptr )
    {
        STrcServerSettings_release(m_trcSettings);
        m_trcSettings = m_pTrcServer->getTraceSettings();
        m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite = true;

        bOk = m_pTrcServer->setTraceSettings(m_trcSettings);
        if( bOk ) bOk = m_pTrcServer->setPort(m_uTrcServerPort);

        if( bOk ) bOk = m_pTrcServer->startup();
    }

    // Result Values
    //---------------

    strResultValue = "Not listening";

    if( m_pTrcServer != nullptr )
    {
        if( m_pTrcServer->isListening() ) strResultValue = "Listening";
    }

    strlstResultValues.append(strResultValue);

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

    strExpectedValue = "Idle";

    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    if( m_pTrcServer != nullptr )
    {
        m_pTrcServer->shutdown();
    }

    // Result Values
    //---------------

    strExpectedValue = "Not idle";

    if( m_pTrcServer != nullptr )
    {
        if( !m_pTrcServer->isListening() ) strResultValue = "Idle";
    }

    strlstResultValues.append(strResultValue);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepTraceServerShutdown

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerRecallAdminObjs( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
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

    strExpectedValue = bool2Str(bOk);
    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    bOk = !bOk;

    if( m_pTrcServer != nullptr )
    {
        bOk = m_pTrcServer->recallAdminObjs();
    }

    // Result Values
    //---------------

    strResultValue = bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepTraceServerRecallAdminObjs

//------------------------------------------------------------------------------
void CTest::doTestStepTraceServerSaveAdminObjs( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    SErrResultInfo errResultInfo;

    // Expected Values
    //---------------

    bool bOk = true;

    strExpectedValue = bool2Str(bOk);
    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    bOk = !bOk;

    if( m_pTrcServer != nullptr )
    {
        bOk = m_pTrcServer->saveAdminObjs();
    }

    // Result Values
    //---------------

    strResultValue = bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepTraceServerSaveAdminObjs

//------------------------------------------------------------------------------
void CTest::doTestStepTraceClientConnect( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setDescription("Check whether client can connect.");
    i_pTestStep->setInstruction(
        "Start and connect the trace method client.<br/>"
        "Hide time info in trace method client to be able to compare the expected with the actual result values.<br/>"
        "By hiding the time info you may copy and paste the trace method output from the trace client "
        "to the result values edit widget of this dialog.<br/>"
        "After copy and paste you may press the button 'Compare Expected With Result Values' below.");
    i_pTestStep->setExpectedValue("Method Trace Client Connected");

    m_pDlgTestStep = new CDlgTestStep(i_pTestStep);
    m_pDlgTestStep->exec();
    delete m_pDlgTestStep;
    m_pDlgTestStep = nullptr;

    // Check if really connected and correct test result if necessary.
    if( i_pTestStep->getTestResult() == ZS::Test::ETestResult::TestPassed )
    {
        if( m_pTrcServer == nullptr )
        {
            i_pTestStep->setResultValue("No Trace Server Created");
        }
        else if( !m_pTrcServer->isConnected() )
        {
            i_pTestStep->setResultValue("Method Trace Client NOT Connected");
        }
    }

} // doTestStepTraceClientConnect

//------------------------------------------------------------------------------
void CTest::doTestStepTraceClientDisconnect( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setDescription("Check whether client can connect.");
    i_pTestStep->setInstruction(
        "Start and connect the trace method client.<br/>"
        "Hide time info in trace method client to be able to compare the expected with the actual result values.<br/>"
        "By hiding the time info you may copy and paste the trace method output from the trace client "
        "to the result values edit widget of this dialog.<br/>"
        "After copy and paste you may press the button 'Compare Expected With Result Values' below.");
    i_pTestStep->setExpectedValue("Method Trace Client Disconnected");

    m_pDlgTestStep = new CDlgTestStep(i_pTestStep);
    m_pDlgTestStep->exec();
    delete m_pDlgTestStep;
    m_pDlgTestStep = nullptr;

    // Check if really disconnected and correct test result if necessary.
    if( i_pTestStep->getTestResult() == ZS::Test::ETestResult::TestPassed )
    {
        if( m_pTrcServer == nullptr )
        {
            i_pTestStep->setResultValue("No Trace Server Created");
        }
        else if( m_pTrcServer->isConnected() )
        {
            i_pTestStep->setResultValue("Method Trace Client NOT Disconnected");
        }
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

    int iTestStepTimeout_ms = 5000;

    ZS::Test::CTestStepGroup* pTestGroup = i_pTestStep->getParentGroup();

    QString strTestGroupName = pTestGroup == nullptr ? "" : pTestGroup->name();

    // Expected Values
    //----------------

    QString strOperation = i_pTestStep->getOperation();

    QString strNameSpace;
    QString strClassName;
    QString strSubClassName;
    QString strObjName;
    QString strMth;
    QStringList strlstInArgs;
    QString strMthRet;

    splitMethodCallOperation(strOperation, strClassName, strSubClassName, strObjName, strMth, strlstInArgs, strMthRet);

    if( strClassName == CMyClass1::ClassName() )
    {
        strNameSpace = CMyClass1::NameSpace();
    }
    else if( strClassName == CMyClass2::ClassName() )
    {
        strNameSpace = CMyClass2::NameSpace();
    }
    else if( strClassName == CMyClass3::ClassName() )
    {
        strNameSpace = CMyClass3::NameSpace();
    }

    if( m_pTrcServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Trace server not existing");
    }
    else
    {
        m_pTrcServer->setNewTrcAdminObjsDefaultDetailLevel(ETraceDetailLevelMethodArgs);
        // The trace output may have been disabled to close (flush buffer) the file
        // so that its content can be evaluated by onZSTraceClientTrcMthListWdgtTextItemAdded.
        // We need to enable the trace output again.
        m_pTrcServer->setEnabled(true);

        QString strTrcMethodEnter = "<GUIMain                 > -> <" + strNameSpace + "::" + strClassName;
        QString strTrcMethodLeave = "<GUIMain                 > <- <" + strNameSpace + "::" + strClassName;

        if( !strSubClassName.isEmpty() )
        {
            strTrcMethodEnter += "::" + strSubClassName;
            strTrcMethodLeave += "::" + strSubClassName;
        }
        strTrcMethodEnter += "> ";
        strTrcMethodLeave += "> ";

        if( !strObjName.isEmpty() )
        {
            strTrcMethodEnter += strObjName + ".";
            strTrcMethodLeave += strObjName + ".";
        }
        strTrcMethodEnter += strMth + "(" + strlstInArgs.join(", ") + ")";
        strTrcMethodLeave += strMth + "()";

        if( !strMthRet.isEmpty() )
        {
            strTrcMethodLeave += ": " + strMthRet;
        }

        QString strExpectedResultsAbsFilePath;
        QVariant val = i_pTestStep->getConfigValue("ExpectedResultsFileName");
        if( val.isValid() && val.canConvert(QVariant::String) )
        {
            strExpectedResultsAbsFilePath = c_strExpectedResultsAbsDirPath + QDir::separator() + val.toString() + ".txt";
        }

        if( strExpectedResultsAbsFilePath.isEmpty() )
        {
            strlstExpectedValues.append(strTrcMethodEnter);
            strlstExpectedValues.append(strTrcMethodLeave);
        }
        else
        {
            readExpectedTestResults(strExpectedResultsAbsFilePath, strlstExpectedValues);
        }

        i_pTestStep->setExpectedValues(strlstExpectedValues);
    }

    // Test Step
    //----------

    bool bValidTestStep = false;

    if( strClassName == CMyClass1::ClassName() )
    {
        if( strMth == "classMethod")
        {
            if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                CMyClass1::classMethod(strlstInArgs[0]);
            }
        }
        else if( strMth == "ctor" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupName.contains("RenameTraceAdminObj") && m_hshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is already existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;

                CMyClass1* pObj = new CMyClass1(strlstInArgs[0]);

                if( strTestGroupName.contains("RenameTraceAdminObj") )
                {
                    m_multihshpMyClass1InstancesByName.insert(strObjName, pObj);
                }
                else
                {
                    m_hshpMyClass1InstancesByName[strObjName] = pObj;
                }

                if( !QObject::connect(
                    /* pObjSender   */ pObj,
                    /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onClass1AboutToBeDestroyed(QObject*, const QString&)),
                    /* cnctType     */ Qt::DirectConnection ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            }
        }
        else if( strMth == "dtor" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupName.contains("RenameTraceAdminObj") && !m_hshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is not existing";
            }
            else if( strTestGroupName.contains("RenameTraceAdminObj") && !m_multihshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 0 )
            {
                bValidTestStep = true;
                if( strTestGroupName.contains("RenameTraceAdminObj") )
                {
                    delete m_multihshpMyClass1InstancesByName.value(strObjName);
                    // Slot onClass1AboutToBeDestroyed will remove the object from the hash.
                    //m_multihshpMyClass1InstancesByName.remove(strObjName, pObj);
                }
                else
                {
                    delete m_hshpMyClass1InstancesByName[strObjName];
                    // Slot onClass1AboutToBeDestroyed will remove the object from the hash.
                    //m_hshpMyClass1InstancesByName.remove(strObjName);
                }
            }
        }
        else if( strMth == "setObjectName" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupName.contains("RenameTraceAdminObj") && !m_hshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is not existing";
            }
            else if( strTestGroupName.contains("RenameTraceAdminObj") && !m_multihshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                if( strTestGroupName.contains("RenameTraceAdminObj") )
                {
                    CMyClass1* pObj = m_multihshpMyClass1InstancesByName.take(strObjName);
                    pObj->setObjectName(strlstInArgs[0]);
                    m_multihshpMyClass1InstancesByName.insert(pObj->objectName(), pObj);
                }
                else
                {
                    CMyClass1* pObj = m_hshpMyClass1InstancesByName.take(strObjName);
                    pObj->setObjectName(strlstInArgs[0]);
                    m_hshpMyClass1InstancesByName.insert(pObj->objectName(), pObj);
                }
            }
        }
        else if( strMth == "startClass2Thread" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !m_hshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                m_hshpMyClass1InstancesByName[strObjName]->startClass2Thread(strlstInArgs[0]);
                CMyClass2* myClass2 = m_hshpMyClass1InstancesByName[strObjName]->getMyClass2();
                if( myClass2 == nullptr )
                {
                    strResultValue = "CMyClass2 Instance not created";
                }
                else
                {
                    bValidTestStep = true;
                    QString strClass2ObjName = myClass2->objectName();
                    m_hshpMyClass2InstancesByName[strClass2ObjName] = myClass2;

                    if( !QObject::connect(
                        /* pObjSender   */ m_hshpMyClass2InstancesByName[strClass2ObjName],
                        /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onClass2AboutToBeDestroyed(QObject*, const QString&)),
                        /* cnctType     */ Qt::DirectConnection ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }
                }
            }
        }
        else if( strMth == "stopClass2Thread" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !m_hshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 0 )
            {
                CMyClass2* myClass2 = m_hshpMyClass1InstancesByName[strObjName]->getMyClass2();
                if( myClass2 == nullptr )
                {
                    strResultValue = "CMyClass2 Instance not created";
                }
                else
                {
                    bValidTestStep = true;
                    // Slot onClass3AboutToBeDestroyed will remove the object from the hash.
                    //QString strClass2ObjName = myClass2->objectName();
                    //m_hshpMyClass2InstancesByName.remove(strClass2ObjName);
                    m_hshpMyClass1InstancesByName[strObjName]->stopClass2Thread();
                }
            }
        }
    } // if( strClassName == CMyClass1::ClassName() )

    else if( strClassName == CMyClass2::ClassName() )
    {
        if( strMth == "ctor" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupName.contains("RenameTraceAdminObj") && m_hshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is already existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;

                CMyClass2* pObj = new CMyClass2(strlstInArgs[0]);

                if( strTestGroupName.contains("RenameTraceAdminObj") )
                {
                    m_multihshpMyClass2InstancesByName.insert(strObjName, pObj);
                }
                else
                {
                    m_hshpMyClass2InstancesByName[strObjName] = pObj;
                }

                if( !QObject::connect(
                    /* pObjSender   */ pObj,
                    /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onClass2AboutToBeDestroyed(QObject*, const QString&)),
                    /* cnctType     */ Qt::DirectConnection ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            }
            else if( strlstInArgs.size() == 2 )
            {
                if( strlstInArgs[1] == "nullptr")
                {
                    bValidTestStep = true;

                    CMyClass2* pObj = new CMyClass2(strlstInArgs[0], nullptr);

                    if( strTestGroupName.contains("RenameTraceAdminObj") )
                    {
                        m_multihshpMyClass2InstancesByName.insert(strObjName, pObj);
                    }
                    else
                    {
                        m_hshpMyClass2InstancesByName[strObjName] = pObj;
                    }

                    if( !QObject::connect(
                        /* pObjSender   */ pObj,
                        /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onClass2AboutToBeDestroyed(QObject*, const QString&)),
                        /* cnctType     */ Qt::DirectConnection ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }
                }
            }
        }
        else if( strMth == "dtor" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupName.contains("RenameTraceAdminObj") && !m_hshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strTestGroupName.contains("RenameTraceAdminObj") && !m_multihshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 0 )
            {
                bValidTestStep = true;
                if( strTestGroupName.contains("RenameTraceAdminObj") )
                {
                    delete m_multihshpMyClass2InstancesByName.value(strObjName);
                    // Slot onClass2AboutToBeDestroyed will remove the object from the hash.
                    //m_multihshpMyClass2InstancesByName.remove(strObjName, pObj);
                }
                else
                {
                    delete m_hshpMyClass2InstancesByName[strObjName];
                    // Slot onClass2AboutToBeDestroyed will remove the object from the hash.
                    //m_hshpMyClass2InstancesByName.remove(strObjName);
                }
            }
        }
        else if( strMth == "setObjectName" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupName.contains("RenameTraceAdminObj") && !m_hshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strTestGroupName.contains("RenameTraceAdminObj") && !m_multihshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                if( strTestGroupName.contains("RenameTraceAdminObj") )
                {
                    CMyClass2* pObj = m_multihshpMyClass2InstancesByName.take(strObjName);
                    pObj->setObjectName(strlstInArgs[0]);
                    m_multihshpMyClass2InstancesByName.insert(pObj->objectName(), pObj);
                }
                else
                {
                    CMyClass2* pObj = m_hshpMyClass2InstancesByName.take(strObjName);
                    pObj->setObjectName(strlstInArgs[0]);
                    m_hshpMyClass2InstancesByName.insert(pObj->objectName(), pObj);
                }
            }
        }
        else if( strMth == "instMethod" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !m_hshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                m_hshpMyClass2InstancesByName[strObjName]->instMethod(strlstInArgs[0]);
            }
        }
        else if( strMth == "recursiveTraceMethod" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !m_hshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 0 )
            {
                bValidTestStep = true;
                m_hshpMyClass2InstancesByName[strObjName]->recursiveTraceMethod();
            }
        }
        else if( strMth == "startMessageTimer" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !m_hshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 0 )
            {
                bValidTestStep = true;
                m_hshpMyClass2InstancesByName[strObjName]->startMessageTimer();
            }
        }
        else if( strMth == "startClass3Thread" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !m_hshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                m_hshpMyClass2InstancesByName[strObjName]->startClass3Thread(strlstInArgs[0]);
                CMyClass3* myClass3 = m_hshpMyClass2InstancesByName[strObjName]->getMyClass3();
                if( myClass3 == nullptr )
                {
                    strResultValue = "CMyClass3 Instance not created";
                }
                else
                {
                    bValidTestStep = true;
                    QString strClass3ObjName = myClass3->objectName();
                    m_hshpMyClass3InstancesByName[strClass3ObjName] = myClass3;

                    if( !QObject::connect(
                        /* pObjSender   */ m_hshpMyClass3InstancesByName[strClass3ObjName],
                        /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onClass3AboutToBeDestroyed(QObject*, const QString&)),
                        /* cnctType     */ Qt::DirectConnection ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }
                }
            }
        }
        else if( strMth == "stopClass3Thread" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !m_hshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 0 )
            {
                CMyClass3* myClass3 = m_hshpMyClass2InstancesByName[strObjName]->getMyClass3();
                if( myClass3 == nullptr )
                {
                    strResultValue = "CMyClass3 Instance not created";
                }
                else
                {
                    bValidTestStep = true;
                    // Slot onClass3AboutToBeDestroyed will remove the object from the hash.
                    //QString strClass3ObjName = myClass3->objectName();
                    //m_hshpMyClass3InstancesByName.remove(strClass3ObjName);
                    m_hshpMyClass2InstancesByName[strObjName]->stopClass3Thread();
                }
            }
        }
    } // if( strClassName == CMyClass2::ClassName() )

    else if( strClassName == CMyClass3::ClassName() )
    {
        if( strMth == "classMethod")
        {
            if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                CMyClass3::classMethod(strlstInArgs[0]);
            }
        }
        else if( strMth == "noisyClassMethod")
        {
            if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                CMyClass3::noisyClassMethod(strlstInArgs[0]);
            }
        }
        else if( strMth == "veryNoisyClassMethod")
        {
            if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                CMyClass3::veryNoisyClassMethod(strlstInArgs[0]);
            }
        }
        else if( strMth == "ctor" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupName.contains("RenameTraceAdminObj") && m_hshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is already existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;

                CMyClass3* pObj = new CMyClass3(strlstInArgs[0]);

                if( strTestGroupName.contains("RenameTraceAdminObj") )
                {
                    m_multihshpMyClass3InstancesByName.insert(strObjName, pObj);
                }
                else
                {
                    m_hshpMyClass3InstancesByName[strObjName] = pObj;
                }

                if( !QObject::connect(
                    /* pObjSender   */ pObj,
                    /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onClass3AboutToBeDestroyed(QObject*, const QString&)),
                    /* cnctType     */ Qt::DirectConnection ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            }
            else if( strlstInArgs.size() == 2 )
            {
                if( strlstInArgs[1] == "nullptr")
                {
                    bValidTestStep = true;

                    CMyClass3* pObj = new CMyClass3(strlstInArgs[0], nullptr);

                    if( strTestGroupName.contains("RenameTraceAdminObj") )
                    {
                        m_multihshpMyClass3InstancesByName.insert(strObjName, pObj);
                    }
                    else
                    {
                        m_hshpMyClass3InstancesByName[strObjName] = pObj;
                    }

                    if( !QObject::connect(
                        /* pObjSender   */ pObj,
                        /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onClass3AboutToBeDestroyed(QObject*, const QString&)),
                        /* cnctType     */ Qt::DirectConnection ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }
                }
            }
        }
        else if( strMth == "dtor" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupName.contains("RenameTraceAdminObj") && !m_hshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strTestGroupName.contains("RenameTraceAdminObj") && !m_multihshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 0 )
            {
                bValidTestStep = true;
                if( strTestGroupName.contains("RenameTraceAdminObj") )
                {
                    delete m_multihshpMyClass3InstancesByName.value(strObjName);
                    // Slot onClass3AboutToBeDestroyed will remove the object from the hash.
                    //m_multihshpMyClass3InstancesByName.remove(strObjName, pObj);
                }
                else
                {
                    delete m_hshpMyClass3InstancesByName[strObjName];
                    // Slot onClass3AboutToBeDestroyed will remove the object from the hash.
                    //m_hshpMyClass3InstancesByName.remove(strObjName);
                }
            }
        }
        else if( strMth == "setObjectName" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupName.contains("RenameTraceAdminObj") && !m_hshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strTestGroupName.contains("RenameTraceAdminObj") && !m_multihshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                if( strTestGroupName.contains("RenameTraceAdminObj") )
                {
                    CMyClass3* pObj = m_multihshpMyClass3InstancesByName.take(strObjName);
                    pObj->setObjectName(strlstInArgs[0]);
                    m_multihshpMyClass3InstancesByName.insert(pObj->objectName(), pObj);
                }
                else
                {
                    CMyClass3* pObj = m_hshpMyClass3InstancesByName.take(strObjName);
                    pObj->setObjectName(strlstInArgs[0]);
                    m_hshpMyClass3InstancesByName.insert(pObj->objectName(), pObj);
                }
            }
        }
        else if( strMth == "instMethod" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !m_hshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                m_hshpMyClass3InstancesByName[strObjName]->instMethod(strlstInArgs[0]);
            }
        }
        else if( strMth == "noisyInstMethod" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !m_hshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                m_hshpMyClass3InstancesByName[strObjName]->noisyInstMethod(strlstInArgs[0]);
            }
        }
        else if( strMth == "veryNoisyInstMethod" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !m_hshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                m_hshpMyClass3InstancesByName[strObjName]->veryNoisyInstMethod(strlstInArgs[0]);
            }
        }
    } // if( strClassName == CMyClass3::ClassName() )

    if( m_pTrcServer != nullptr )
    {
        m_pTrcServer->setNewTrcAdminObjsDefaultDetailLevel(ETraceDetailLevelNone);
    }

    if( !bValidTestStep )
    {
        if( !strResultValue.isEmpty() )
        {
            strlstResultValues.append(strResultValue);
            strResultValue = "";
        }
        strlstResultValues.append("Invalid test step operation");
    }

    if( !strlstResultValues.isEmpty() )
    {
        i_pTestStep->setResultValues(strlstResultValues);
    }
    else if( !strResultValue.isEmpty() )
    {
        i_pTestStep->setResultValue(strResultValue);
    }
    else
    {
        i_pTestStep->setInstruction("Check whether client received the expected trace output.");

        m_pDlgTestStep = new CDlgTestStep(i_pTestStep);
        m_pDlgTestStep->exec();
        delete m_pDlgTestStep;
        m_pDlgTestStep = nullptr;
    }

} // doTestStepTraceMethodCall

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
        QString     strResultValue;
        QStringList strlstResultValues;

        strResultValue = "Test step not finished in time";
        strlstResultValues.append(strResultValue);

        pTestStep->setResultValues(strlstResultValues);

    } // if( pTestStep != nullptr )

} // onTimerTestStepTimeout()

//------------------------------------------------------------------------------
void CTest::onClass1AboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    if( m_multihshpMyClass1InstancesByName.contains(i_strObjName) )
    {
        QList<CMyClass1*> arpObjs = m_multihshpMyClass1InstancesByName.values(i_strObjName);

        for( auto& pObj : arpObjs )
        {
            if( pObj == i_pObj )
            {
                m_multihshpMyClass1InstancesByName.remove(i_strObjName, pObj);
            }
        }
    }

    if( m_hshpMyClass1InstancesByName.contains(i_strObjName) )
    {
        if( m_hshpMyClass1InstancesByName.value(i_strObjName) == i_pObj )
        {
            m_hshpMyClass1InstancesByName.remove(i_strObjName);
        }
    }
}

//------------------------------------------------------------------------------
void CTest::onClass2AboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    if( m_multihshpMyClass2InstancesByName.contains(i_strObjName) )
    {
        QList<CMyClass2*> arpObjs = m_multihshpMyClass2InstancesByName.values(i_strObjName);

        for( auto& pObj : arpObjs )
        {
            if( pObj == i_pObj )
            {
                m_multihshpMyClass2InstancesByName.remove(i_strObjName, pObj);
            }
        }
    }

    if( m_hshpMyClass2InstancesByName.contains(i_strObjName) )
    {
        if( m_hshpMyClass2InstancesByName.value(i_strObjName) == i_pObj )
        {
            m_hshpMyClass2InstancesByName.remove(i_strObjName);
        }
    }
}

//------------------------------------------------------------------------------
void CTest::onClass3AboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    if( m_multihshpMyClass3InstancesByName.contains(i_strObjName) )
    {
        QList<CMyClass3*> arpObjs = m_multihshpMyClass3InstancesByName.values(i_strObjName);

        for( auto& pObj : arpObjs )
        {
            if( pObj == i_pObj )
            {
                m_multihshpMyClass3InstancesByName.remove(i_strObjName, pObj);
            }
        }
    }

    if( m_hshpMyClass3InstancesByName.contains(i_strObjName) )
    {
        if( m_hshpMyClass3InstancesByName.value(i_strObjName) == i_pObj )
        {
            m_hshpMyClass3InstancesByName.remove(i_strObjName);
        }
    }
}

/*==============================================================================
private: // instance auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Splits the operation string into sections.

    Examples for strOperation:

    - "CMyClass1::classMethod(Arg1)"
    - "CMyClass1::classMethod(Arg1, Arg2): Ret"
    - "CMyClass1::ctor(Inst1)",
    - "CMyClass1::Inst1.startClass2Thread(Inst1): Inst1",
    - "CMyClass1::Inst1.stopClass2Thread()",
    - "CMyClass1::dtor(Inst1)",

    - "CMyClass2::ctor(Inst1)"
    - "CMyClass2::ctor(SoundCard-PowerLevel)"
    - "CMyClass2::Inst1.instMethod(Arg1, Arg2)"
    - "CMyClass2::Inst1.instMethod(Arg1, Arg2): Ret"
    - "CMyClass2::Inst1.recursiveTraceMethod(): %N%",
    - "CMyClass2::SoundCard-PowerLevel.instMethod(Arg1, Arg2)"
    - "CMyClass2::dtor(SoundCard-PowerLevel)"
    - "CMyClass2::dtor(Inst1)"

    - "CMyClass3::classMethod(Arg1, Arg2)"
    - "CMyClass3::NoisyMethods::noisyClassMethod(Arg1, Arg2)"
    - "CMyClass3::VeryNoisyMethods::veryNoisyClassMethod(Arg1, Arg2)"
    - "CMyClass3::ctor(Inst1)"
    - "CMyClass3::Inst1.instMethod(Arg1, Arg2)"
    - "CMyClass3::Inst1.instMethod(Arg1, Arg2): Ret"
    - "CMyClass3::NoisyMethods::Inst1.noisyInstMethod(Arg1, Arg2)"
    - "CMyClass3::VeryNoisyMethods::Inst1.veryNoisyInstMethod(Arg1, Arg2)"
    - "CMyClass3::dtor(Inst1)"

    If an object name contains the delimiter "-" this delimiter will be replaced by "::"
    to create a grouped object name.
*/
void CTest::splitMethodCallOperation(
    const QString& i_strOperation,
    QString& o_strClassName,
    QString& o_strSubClassName,
    QString& o_strObjName,
    QString& o_strMth,
    QStringList& o_strlstInArgs,
    QString& o_strMthRet ) const
//------------------------------------------------------------------------------
{
    o_strClassName = "";
    o_strObjName = "";
    o_strMth = "";
    o_strlstInArgs.clear();
    o_strMthRet = "";

    QStringList strlst;

    strlst = i_strOperation.split("::", Qt::SkipEmptyParts);

    if( strlst.size() == 3 )
    {
        o_strSubClassName = strlst[1];
        strlst.removeAt(1);
    }

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
            strlst[0].replace("-", "::");
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
                    o_strlstInArgs = strlst[0].split(", ", Qt::SkipEmptyParts);
                }
                else if( strlst.size() == 2 )
                {
                    strlst[0].remove("(").remove(")");
                    o_strlstInArgs = strlst[0].split(", ", Qt::SkipEmptyParts);
                    o_strMthRet = strlst[1];
                }
                if( o_strMth == "ctor" && o_strlstInArgs.size() >= 1 && o_strObjName.isEmpty() )
                {
                    o_strlstInArgs[0].replace("-", "::");
                    o_strObjName = o_strlstInArgs[0];
                }
                else if( o_strMth == "dtor" && o_strlstInArgs.size() == 1 && o_strObjName.isEmpty() )
                {
                    o_strlstInArgs[0].replace("-", "::");
                    o_strObjName = o_strlstInArgs[0];
                    o_strlstInArgs.clear();
                }
            }
        }
    }
} // splitMethodCallOperation
