/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrLog.h"

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
    ZS::Test::CTest("Libs-IpcTrace-DllIfQtApp"),
    m_pDlgTestStep(nullptr),
    m_pTmrTestStepTimeout(nullptr),
    m_trcSettings(),
    m_uTrcServerPort(24763),
    m_pTrcServer(nullptr),
    m_hshpMyClass1InstancesByName(),
    m_hshpMyClass2InstancesByName(),
    m_hshpMyClass2ThreadInstancesByName(),
    m_hshpMyClass3InstancesByName(),
    m_hshpMyClass3ThreadInstancesByName(),
    m_multihshpMyClass1InstancesByName(),
    m_multihshpMyClass2InstancesByName(),
    m_multihshpMyClass2ThreadInstancesByName(),
    m_multihshpMyClass3InstancesByName(),
    m_multihshpMyClass3ThreadInstancesByName()
{
    m_pTmrTestStepTimeout = new QTimer(this);
    m_pTmrTestStepTimeout->setSingleShot(true);

    QObject::connect(
        m_pTmrTestStepTimeout, &QTimer::timeout,
        this, &CTest::onTimerTestStepTimeout);

    ZS::Test::CTestStep* pTestStep = nullptr;

    // Test Step Group - One Thread
    //=============================

    int idxGroup = 0;
    int idxStep = 0;

    ZS::Test::CTestStepGroup* pTestGroupOneThread = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " One Thread",
        /* pTSGrpParent    */ nullptr );

    // Test Step Group - One Thread - Startup
    //----------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Load ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::loadDll",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLoadDll(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Create Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::CreateInstance",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerCreateInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Startup",
        /* strOperation    */ "DllIf::CIpcTrcServer::startup",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerStartup(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Recall Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::recallAdminObjs",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerRecallAdminObjs(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Connect",
        /* strOperation    */ "CIpcTrcClient::connect",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientConnect(ZS::Test::CTestStep*)) );

    // Test Step Group - One Thread - Method Tracing - Class 1
    //--------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::classMethod",
        /* strOperation    */ "CMyClass1::classMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    // Test Step Group - One Thread - Method Tracing - Class 2
    //--------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(Inst1, nullptr)",
        /* strOperation    */ "CMyClass2::ctor(Inst1, nullptr)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(Inst2, nullptr)",
        /* strOperation    */ "CMyClass2::ctor(Inst2, nullptr)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-ctor-Inst2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.instMethod",
        /* strOperation    */ "CMyClass2::Inst1.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst2.instMethod",
        /* strOperation    */ "CMyClass2::Inst2.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::dtor(Inst1)",
        /* strOperation    */ "CMyClass2::dtor(Inst1)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-dtor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::dtor(Inst2)",
        /* strOperation    */ "CMyClass2::dtor(Inst2)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-dtor-Inst2");

    // Test Step Group - One Thread - Method Tracing - Class 2 - Logically Grouped Objects
    //------------------------------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(SoundCard-PowerLevel, nullptr)",
        /* strOperation    */ "CMyClass2::ctor(SoundCard-PowerLevel, nullptr)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-ctor-SoundCard-PowerLevel");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(Equalizer-PowerLevel, nullptr)",
        /* strOperation    */ "CMyClass2::ctor(Equalizer-PowerLevel, nullptr)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-ctor-Equalizer-PowerLevel");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::SoundCard-PowerLevel.instMethod",
        /* strOperation    */ "CMyClass2::SoundCard-PowerLevel.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Equalizer-PowerLevel.instMethod",
        /* strOperation    */ "CMyClass2::Equalizer-PowerLevel.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::dtor(SoundCard-PowerLevel)",
        /* strOperation    */ "CMyClass2::dtor(SoundCard-PowerLevel)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-dtor-SoundCard-PowerLevel");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::dtor(Equalizer-PowerLevel)",
        /* strOperation    */ "CMyClass2::dtor(Equalizer-PowerLevel)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-dtor-Equalizer-PowerLevel");

    // Test Step Group - One Thread - Method Tracing - Class 3
    //---------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::classMethod",
        /* strOperation    */ "CMyClass3::classMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::noisyClassMethod",
        /* strOperation    */ "CMyClass3::NoisyMethods::noisyClassMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::veryNoisyClassMethod",
        /* strOperation    */ "CMyClass3::VeryNoisyMethods::veryNoisyClassMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::ctor(Inst1, nullptr)",
        /* strOperation    */ "CMyClass3::ctor(Inst1, nullptr)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass3-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::ctor(Inst2, nullptr)",
        /* strOperation    */ "CMyClass3::ctor(Inst2, nullptr)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass3-ctor-Inst2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst1.instMethod",
        /* strOperation    */ "CMyClass3::Inst1.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst2.instMethod",
        /* strOperation    */ "CMyClass3::Inst2.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst1.noisyInstMethod",
        /* strOperation    */ "CMyClass3::NoisyMethods::Inst1.noisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst2.noisyInstMethod",
        /* strOperation    */ "CMyClass3::NoisyMethods::Inst2.noisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst1.veryNoisyInstMethod",
        /* strOperation    */ "CMyClass3::VeryNoisyMethods::Inst1.veryNoisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst2.veryNoisyInstMethod",
        /* strOperation    */ "CMyClass3::VeryNoisyMethods::Inst2.veryNoisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::dtor(Inst1)",
        /* strOperation    */ "CMyClass3::dtor(Inst1)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass3-dtor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::dtor(Inst2)",
        /* strOperation    */ "CMyClass3::dtor(Inst2)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass3-dtor-Inst2");

    // Test Step Group - One Thread - Shutdown
    //----------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Disconnect",
        /* strOperation    */ "CIpcTrcClient::disconnect",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientDisconnect(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Save Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::saveAdminObjs",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerSaveAdminObjs(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Shutdown",
        /* strOperation    */ "DllIf::CIpcTrcServer::shutdown",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerShutdown(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Release Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::ReleaseInstance",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerReleaseInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::releaseDll",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepReleaseDll(ZS::Test::CTestStep*)) );

    // Test Step Group - Several Threads
    //==================================

    idxStep = 0;

    ZS::Test::CTestStepGroup* pTestGroupSeveralThreads = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " Several Threads",
        /* pTSGrpParent    */ nullptr );

    // Test Step Group - Several Threads - Startup
    //----------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Load ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::loadDll",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLoadDll(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Create Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::CreateInstance",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerCreateInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Startup",
        /* strOperation    */ "DllIf::CIpcTrcServer::startup",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerStartup(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Recall Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::recallAdminObjs",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerRecallAdminObjs(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Connect",
        /* strOperation    */ "CIpcTrcClient::connect",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientConnect(ZS::Test::CTestStep*)) );

    // Test Step Group - Several Threads - Method Tracing - Class 1 - StartClass2Thread
    //---------------------------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::ctor(Inst1)",
        /* strOperation    */ "CMyClass1::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::Inst1.startClass2Thread(Inst1)",
        /* strOperation    */ "CMyClass1::Inst1.startClass2Thread(Inst1): Inst1",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-Inst1-startClass2Thread-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.recursiveTraceMethod()",
        /* strOperation    */ "CMyClass2::Inst1.recursiveTraceMethod(): 0",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-Inst1-recursiveTraceMethod");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.startMessageTimer()",
        /* strOperation    */ "CMyClass2::Inst1.startMessageTimer()",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-Inst1-startMessageTimer");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.startClass3Thread(Inst1)",
        /* strOperation    */ "CMyClass2::Inst1.startClass3Thread(Inst1): Inst1",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-Inst1-startClass3Thread-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.stopClass3Thread()",
        /* strOperation    */ "CMyClass2::Inst1.stopClass3Thread()",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-Inst1-stopClass3Thread");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::Inst1.stopClass2Thread()",
        /* strOperation    */ "CMyClass1::Inst1.stopClass2Thread()",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-Inst1-stopClass2Thread");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass1::dtor(Inst1)",
        /* strOperation    */ "CMyClass1::dtor(Inst1)",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-dtor-Inst1");

    // Test Step Group - Several Threads - Shutdown
    //-----------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Disconnect",
        /* strOperation    */ "CIpcTrcClient::disconnect",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientDisconnect(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Save Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::saveAdminObjs",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerSaveAdminObjs(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Shutdown",
        /* strOperation    */ "DllIf::CIpcTrcServer::shutdown",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerShutdown(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Release Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::ReleaseInstance",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerReleaseInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::releaseDll",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepReleaseDll(ZS::Test::CTestStep*)) );

    // Test Step Group - RenameTraceAdminObj
    //======================================

    ZS::Test::CTestStepGroup* pTestGroupRenameTraceAdminObj = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " RenameTraceAdminObj",
        /* pTSGrpParent    */ nullptr );

    // Test Step Group - RenameTraceAdminObj - One Thread
    //===================================================

    idxStep = 0;

    ZS::Test::CTestStepGroup* pTestGroupRenameTraceAdminObjOneThread = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " One Thread",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObj );


    // Test Step Group - RenameTraceAdminObj - One Thread - Startup
    //-------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Load ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::loadDll",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLoadDll(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Create Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::CreateInstance",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerCreateInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Startup",
        /* strOperation    */ "DllIf::CIpcTrcServer::startup",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerStartup(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Recall Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::recallAdminObjs",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerRecallAdminObjs(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Connect",
        /* strOperation    */ "CIpcTrcClient::connect",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientConnect(ZS::Test::CTestStep*)) );

    // Test Step Group - RenameTraceAdminObj - One Thread - Several instances of class tracer
    //---------------------------------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::ctor(Inst1)",
        /* strOperation    */ "CMyClass1::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::ctor(Inst1)",
        /* strOperation    */ "CMyClass1::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::Inst1.setObjectName(NewName1)",
        /* strOperation    */ "CMyClass1::Inst1.setObjectName(NewName1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-Inst1-setObjectName-NewName1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::Inst1.setObjectName(NewName2)",
        /* strOperation    */ "CMyClass1::Inst1.setObjectName(NewName2)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-Inst1-setObjectName-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::NewName1.setObjectName(NewName2)",
        /* strOperation    */ "CMyClass1::NewName1.setObjectName(NewName2)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-NewName1-setObjectName-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass1::dtor(NewName2)",
        /* strOperation    */ "CMyClass1::dtor(NewName2)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-dtor-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass1::dtor(NewName2)",
        /* strOperation    */ "CMyClass1::dtor(NewName2)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-dtor-NewName2");

    // Test Step Group - RenameTraceAdminObj - One Thread - Several instances of instance tracer
    //------------------------------------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(Inst1)",
        /* strOperation    */ "CMyClass2::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(Inst1)",
        /* strOperation    */ "CMyClass2::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.setObjectName(NewName1)",
        /* strOperation    */ "CMyClass2::Inst1.setObjectName(NewName1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-Inst1-setObjectName-NewName1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.setObjectName(NewName2)",
        /* strOperation    */ "CMyClass2::Inst1.setObjectName(NewName2)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-Inst1-setObjectName-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::NewName1.setObjectName(NewName2)",
        /* strOperation    */ "CMyClass2::NewName1.setObjectName(NewName2)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-NewName1-setObjectName-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass2::dtor(NewName2)",
        /* strOperation    */ "CMyClass2::dtor(NewName2)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-dtor-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass2::dtor(NewName2)",
        /* strOperation    */ "CMyClass2::dtor(NewName2)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-dtor-NewName2");

    // Test Step Group - RenameTraceAdminObj - One Thread - Shutdown
    //--------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Disconnect",
        /* strOperation    */ "CIpcTrcClient::disconnect",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientDisconnect(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Save Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::saveAdminObjs",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerSaveAdminObjs(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Shutdown",
        /* strOperation    */ "DllIf::CIpcTrcServer::shutdown",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerShutdown(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Release Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::ReleaseInstance",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerReleaseInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::releaseDll",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepReleaseDll(ZS::Test::CTestStep*)) );

    // Test Step Group - RenameTraceAdminObj - Several Threads
    //========================================================

    idxStep = 0;

    ZS::Test::CTestStepGroup* pTestGroupRenameTraceAdminObjSeveralThreads = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " Several Threads",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObj );

    // Test Step Group - RenameTraceAdminObj - Several Threads - Startup
    //------------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Load ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::loadDll",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLoadDll(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Create Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::CreateInstance",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerCreateInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Startup",
        /* strOperation    */ "DllIf::CIpcTrcServer::startup",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerStartup(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Recall Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::recallAdminObjs",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerRecallAdminObjs(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Connect",
        /* strOperation    */ "CIpcTrcClient::connect",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientConnect(ZS::Test::CTestStep*)) );

    // Test Step Group - RenameTraceAdminObj - Several Threads - Several instances of class tracer
    //--------------------------------------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::ctor(Inst1)",
        /* strOperation    */ "CMyClass1::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::Inst1.startClass2Thread(Inst1)",
        /* strOperation    */ "CMyClass1::Inst1.startClass2Thread(Inst1): Inst1",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-Inst1-startClass2Thread-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.startClass3Thread(Inst1)",
        /* strOperation    */ "CMyClass2::Inst1.startClass3Thread(Inst1): Inst1",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-Inst1-startClass3Thread-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.instMethod",
        /* strOperation    */ "CMyClass2::Inst1.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-Inst1-instMethod-queued");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst1.noisyInstMethod",
        /* strOperation    */ "CMyClass3::NoisyMethods::Inst1.noisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass3-Inst1-noisyInstMethod-queued");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst1.veryNoisyInstMethod",
        /* strOperation    */ "CMyClass3::VeryNoisyMethods::Inst1.veryNoisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass3-Inst1-veryNoisyInstMethod-queued");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2Thread::Inst1.setObjectName(NewName1)",
        /* strOperation    */ "CMyClass2Thread::MyClass2ThreadInst1.setObjectName(MyClass2ThreadNewName1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2Thread-Inst1-setObjectName-NewName1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.setObjectName(NewName1)",
        /* strOperation    */ "CMyClass2::Inst1.setObjectName(NewName1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-Inst1-setObjectName-NewName1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3Thread::Inst1.setObjectName(NewName1)",
        /* strOperation    */ "CMyClass3Thread::MyClass3ThreadInst1.setObjectName(MyClass3ThreadNewName1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass3Thread-Inst1-setObjectName-NewName1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst1.setObjectName(NewName1)",
        /* strOperation    */ "CMyClass3::Inst1.setObjectName(NewName1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass3-Inst1-setObjectName-NewName1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::NewName1.instMethod",
        /* strOperation    */ "CMyClass2::NewName1.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-NewName1-instMethod-queued");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::NewName1.noisyInstMethod",
        /* strOperation    */ "CMyClass3::NoisyMethods::NewName1.noisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass3-NewName1-noisyInstMethod-queued");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::NewName1.veryNoisyInstMethod",
        /* strOperation    */ "CMyClass3::VeryNoisyMethods::NewName1.veryNoisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass3-NewName1-veryNoisyInstMethod-queued");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::NewName1.stopClass3Thread()",
        /* strOperation    */ "CMyClass2::NewName1.stopClass3Thread()",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass2-NewName1-stopClass3Thread");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::Inst1.stopClass2Thread()",
        /* strOperation    */ "CMyClass1::Inst1.stopClass2Thread()",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-Inst1-stopClass2Thread-NewName1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::dtor(Inst1)",
        /* strOperation    */ "CMyClass1::dtor(Inst1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-dtor-Inst1");

    // Test Step Group - RenameTraceAdminObj - Several Threads - Shutdown
    //-------------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Disconnect",
        /* strOperation    */ "CIpcTrcClient::disconnect",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientDisconnect(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Save Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::saveAdminObjs",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerSaveAdminObjs(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Shutdown",
        /* strOperation    */ "DllIf::CIpcTrcServer::shutdown",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerShutdown(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Release Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::ReleaseInstance",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerReleaseInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::releaseDll",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepReleaseDll(ZS::Test::CTestStep*)) );

    // Test Step Group - Much data
    //=============================

    idxStep = 0;

    ZS::Test::CTestStepGroup* pTestGroupMuchData = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " Much data",
        /* pTSGrpParent    */ nullptr );

    // Test Step Group - Much data - Startup
    //--------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Load ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::loadDll",
        /* pTSGrpParent    */ pTestGroupMuchData,
        /* szDoTestStepFct */ SLOT(doTestStepLoadDll(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Create Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::CreateInstance",
        /* pTSGrpParent    */ pTestGroupMuchData,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerCreateInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Startup",
        /* strOperation    */ "DllIf::CIpcTrcServer::startup",
        /* pTSGrpParent    */ pTestGroupMuchData,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerStartup(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Recall Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::recallAdminObjs",
        /* pTSGrpParent    */ pTestGroupMuchData,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerRecallAdminObjs(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Connect",
        /* strOperation    */ "CIpcTrcClient::connect",
        /* pTSGrpParent    */ pTestGroupMuchData,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientConnect(ZS::Test::CTestStep*)) );

    // Test Step Group - Much data - Send Data
    //----------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::ctor(Inst1)",
        /* strOperation    */ "CMyClass1::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroupMuchData,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::sendMuchData()",
        /* strOperation    */ "CMyClass1::Inst1.sendMuchData()",
        /* pTSGrpParent    */ pTestGroupMuchData,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::sendMuchData()",
        /* strOperation    */ "CMyClass1::Inst1.sendMuchData()",
        /* pTSGrpParent    */ pTestGroupMuchData,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::sendMuchData()",
        /* strOperation    */ "CMyClass1::Inst1.sendMuchData()",
        /* pTSGrpParent    */ pTestGroupMuchData,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::dtor(Inst1)",
        /* strOperation    */ "CMyClass1::dtor(Inst1)",
        /* pTSGrpParent    */ pTestGroupMuchData,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSTrcServer-CMyClass1-dtor-Inst1");

    // Test Step Group - Much data - Shutdown
    //---------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Disconnect",
        /* strOperation    */ "CIpcTrcClient::disconnect",
        /* pTSGrpParent    */ pTestGroupMuchData,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientDisconnect(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Save Admin Objects",
        /* strOperation    */ "DllIf::CIpcTrcServer::saveAdminObjs",
        /* pTSGrpParent    */ pTestGroupMuchData,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerSaveAdminObjs(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Shutdown",
        /* strOperation    */ "DllIf::CIpcTrcServer::shutdown",
        /* pTSGrpParent    */ pTestGroupMuchData,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerShutdown(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Release Instance",
        /* strOperation    */ "DllIf::CIpcTrcServer::ReleaseInstance",
        /* pTSGrpParent    */ pTestGroupMuchData,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerReleaseInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release ZSIpcTrace Dll",
        /* strOperation    */ "DllIf::releaseDll",
        /* pTSGrpParent    */ pTestGroupMuchData,
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
            pObj, &CMyClass1::aboutToBeDestroyed,
            this, &CTest::onClass1AboutToBeDestroyed);

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
            pObj, &CMyClass2::aboutToBeDestroyed,
            this, &CTest::onClass2AboutToBeDestroyed);

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

    for( auto& pObj : m_hshpMyClass2ThreadInstancesByName )
    {
        QObject::disconnect(
            pObj, &CMyClass2Thread::aboutToBeDestroyed,
            this, &CTest::onClass2ThreadAboutToBeDestroyed);

        strObjName = pObj->objectName();

        try
        {
            delete pObj;
        }
        catch(...)
        {
        }
        m_hshpMyClass2ThreadInstancesByName[strObjName] = nullptr;
    }

    for( auto& pObj : m_hshpMyClass3InstancesByName )
    {
        QObject::disconnect(
            pObj, &CMyClass3::aboutToBeDestroyed,
            this, &CTest::onClass3AboutToBeDestroyed);

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

    for( auto& pObj : m_hshpMyClass3ThreadInstancesByName )
    {
        QObject::disconnect(
            pObj, &CMyClass3Thread::aboutToBeDestroyed,
            this, &CTest::onClass3ThreadAboutToBeDestroyed);

        strObjName = pObj->objectName();

        try
        {
            delete pObj;
        }
        catch(...)
        {
        }
        m_hshpMyClass3ThreadInstancesByName[strObjName] = nullptr;
    }

    while( !m_multihshpMyClass1InstancesByName.isEmpty() )
    {
        QMultiHash<QString, CMyClass1*>::iterator it = m_multihshpMyClass1InstancesByName.begin();

        CMyClass1* pObj = it.value();

        QObject::disconnect(
            pObj, &CMyClass1::aboutToBeDestroyed,
            this, &CTest::onClass1AboutToBeDestroyed);

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
            pObj, &CMyClass2::aboutToBeDestroyed,
            this, &CTest::onClass2AboutToBeDestroyed);

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

    while( !m_multihshpMyClass2ThreadInstancesByName.isEmpty() )
    {
        QMultiHash<QString, CMyClass2Thread*>::iterator it = m_multihshpMyClass2ThreadInstancesByName.begin();

        CMyClass2Thread* pObj = it.value();

        QObject::disconnect(
            pObj, &CMyClass2Thread::aboutToBeDestroyed,
            this, &CTest::onClass2ThreadAboutToBeDestroyed);

        strObjName = pObj->objectName();
        m_multihshpMyClass2ThreadInstancesByName.remove(strObjName, pObj);

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
            pObj, &CMyClass3::aboutToBeDestroyed,
            this, &CTest::onClass3AboutToBeDestroyed);

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

    while( !m_multihshpMyClass3ThreadInstancesByName.isEmpty() )
    {
        QMultiHash<QString, CMyClass3Thread*>::iterator it = m_multihshpMyClass3ThreadInstancesByName.begin();

        CMyClass3Thread* pObj = it.value();

        QObject::disconnect(
            pObj, &CMyClass3Thread::aboutToBeDestroyed,
            this, &CTest::onClass3ThreadAboutToBeDestroyed);

        strObjName = pObj->objectName();
        m_multihshpMyClass3ThreadInstancesByName.remove(strObjName, pObj);

        try
        {
            delete pObj;
        }
        catch(...)
        {
        }
    }

    if( m_pTrcServer != nullptr )
    {
        ZS::Trace::DllIf::CIpcTrcServer::ReleaseInstance(m_pTrcServer);
    }

    m_pDlgTestStep = nullptr;
    m_pTmrTestStepTimeout = nullptr;
    ZS::Trace::DllIf::STrcServerSettings_release(m_trcSettings);
    m_uTrcServerPort = 0;
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
        /* eTrcDetailLevelDllIf                     */ ZS::Trace::DllIf::EMethodTraceDetailLevelArgsNormal,
        /* eTrcDetailLevelTrcServer                 */ ZS::Trace::DllIf::EMethodTraceDetailLevelArgsNormal,
        /* eTrcDetailLevelTrcServerMutex            */ ZS::Trace::DllIf::EMethodTraceDetailLevelNone,
        /* eTrcDetailLevelTrcServerIpcServer        */ ZS::Trace::DllIf::EMethodTraceDetailLevelNone,
        /* eTrcDetailLevelTrcServerIpcServerMutex   */ ZS::Trace::DllIf::EMethodTraceDetailLevelNone,
        /* eTrcDetailLevelTrcServerIpcServerGateway */ ZS::Trace::DllIf::EMethodTraceDetailLevelNone );

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

    QString strAppConfigDir = ZS::System::getAppConfigDir();

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
    i_pTestStep->setDescription("Check whether client can disconnect.");
    i_pTestStep->setInstruction("Disconnect the trace method client.");
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

    QString strTestGroupPath = pTestGroup == nullptr ? "" : pTestGroup->path();

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
        m_pTrcServer->setNewTrcAdminObjsMethodCallsDefaultDetailLevel(DllIf::EMethodTraceDetailLevelArgsNormal);
        m_pTrcServer->setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel(DllIf::ELogDetailLevelDebug);
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
        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        if( val.isValid() && val.canConvert(QVariant::String) )
        #else
        if( val.isValid() && val.canConvert(static_cast<QMetaType>(QMetaType::QString)) )
        #endif
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
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && m_hshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is already existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;

                CMyClass1* pObj = new CMyClass1(strlstInArgs[0]);

                if( strTestGroupPath.contains("RenameTraceAdminObj") )
                {
                    m_multihshpMyClass1InstancesByName.insert(strObjName, pObj);
                }
                else
                {
                    m_hshpMyClass1InstancesByName[strObjName] = pObj;
                }

                QObject::connect(
                    pObj, &CMyClass1::aboutToBeDestroyed,
                    this, &CTest::onClass1AboutToBeDestroyed,
                    Qt::DirectConnection);
            }
        }
        else if( strMth == "dtor" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 0 )
            {
                bValidTestStep = true;
                if( strTestGroupPath.contains("RenameTraceAdminObj") )
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
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                if( strTestGroupPath.contains("RenameTraceAdminObj") )
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
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                CMyClass1* pMyClass1 = nullptr;
                if( strTestGroupPath.contains("RenameTraceAdminObj") )
                {
                    pMyClass1 = m_multihshpMyClass1InstancesByName.value(strObjName);
                }
                else
                {
                    pMyClass1 = m_hshpMyClass1InstancesByName.value(strObjName);
                }
                pMyClass1->startClass2Thread(strlstInArgs[0]);

                CMyClass2Thread* pMyClass2Thread = pMyClass1->getMyClass2Thread();
                CMyClass2* pMyClass2 = pMyClass1->getMyClass2();

                if( pMyClass2Thread == nullptr )
                {
                    strResultValue = "CMyClass2Thread Instance not created";
                }
                else if( pMyClass2 == nullptr )
                {
                    strResultValue = "CMyClass2 Instance not created";
                }
                else
                {
                    bValidTestStep = true;

                    QString strClass2ThreadObjName = pMyClass2Thread->objectName();
                    QString strClass2ObjName = pMyClass2->objectName();

                    if( strTestGroupPath.contains("RenameTraceAdminObj") )
                    {
                        m_multihshpMyClass2ThreadInstancesByName.insert(strClass2ThreadObjName, pMyClass2Thread);
                        m_multihshpMyClass2InstancesByName.insert(strClass2ObjName, pMyClass2);
                    }
                    else
                    {
                        m_hshpMyClass2ThreadInstancesByName[strClass2ThreadObjName] = pMyClass2Thread;
                        m_hshpMyClass2InstancesByName[strClass2ObjName] = pMyClass2;
                    }

                    QObject::connect(
                        pMyClass2Thread, &CMyClass2Thread::aboutToBeDestroyed,
                        this, &CTest::onClass2ThreadAboutToBeDestroyed,
                        Qt::DirectConnection);
                    QObject::connect(
                        pMyClass2, &CMyClass2::aboutToBeDestroyed,
                        this, &CTest::onClass2AboutToBeDestroyed,
                        Qt::DirectConnection);
                }
            }
        }
        else if( strMth == "stopClass2Thread" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 0 )
            {
                CMyClass1* pMyClass1 = nullptr;
                CMyClass2* pMyClass2 = nullptr;
                QString    strObjNameClass2;

                if( strTestGroupPath.contains("RenameTraceAdminObj") )
                {
                    pMyClass1 = m_multihshpMyClass1InstancesByName.value(strObjName);
                }
                else
                {
                    pMyClass1 = m_hshpMyClass1InstancesByName.value(strObjName);
                }
                if( pMyClass1 != nullptr )
                {
                    pMyClass2 = pMyClass1->getMyClass2();
                }

                if( pMyClass1 == nullptr )
                {
                    strResultValue = "CMyClass1 Instance not created";
                }
                else if( pMyClass2 == nullptr )
                {
                    strResultValue = "CMyClass2 Instance not created";
                }
                else
                {
                    bValidTestStep = true;
                    // Slots onClass2AboutToBeDestroyed will remove the
                    // object from the hashes.
                    pMyClass1->stopClass2Thread();
                }
            }
        }
        else if( strMth == "sendMuchData" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 0 )
            {
                CMyClass1* pMyClass1 = nullptr;

                if( strTestGroupPath.contains("RenameTraceAdminObj") )
                {
                    pMyClass1 = m_multihshpMyClass1InstancesByName.value(strObjName);
                }
                else
                {
                    pMyClass1 = m_hshpMyClass1InstancesByName.value(strObjName);
                }

                if( pMyClass1 == nullptr )
                {
                    strResultValue = "CMyClass1 Instance not created";
                }
                else
                {
                    bValidTestStep = true;
                    pMyClass1->sendMuchData();
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
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && m_hshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is already existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;

                CMyClass2* pObj = new CMyClass2(strlstInArgs[0]);

                if( strTestGroupPath.contains("RenameTraceAdminObj") )
                {
                    m_multihshpMyClass2InstancesByName.insert(strObjName, pObj);
                }
                else
                {
                    m_hshpMyClass2InstancesByName[strObjName] = pObj;
                }

                QObject::connect(
                    pObj, &CMyClass2::aboutToBeDestroyed,
                    this, &CTest::onClass2AboutToBeDestroyed,
                    Qt::DirectConnection);
            }
            else if( strlstInArgs.size() == 2 )
            {
                if( strlstInArgs[1] == "nullptr")
                {
                    bValidTestStep = true;

                    CMyClass2* pObj = new CMyClass2(strlstInArgs[0], nullptr);

                    if( strTestGroupPath.contains("RenameTraceAdminObj") )
                    {
                        m_multihshpMyClass2InstancesByName.insert(strObjName, pObj);
                    }
                    else
                    {
                        m_hshpMyClass2InstancesByName[strObjName] = pObj;
                    }

                    QObject::connect(
                        pObj, &CMyClass2::aboutToBeDestroyed,
                        this, &CTest::onClass2AboutToBeDestroyed,
                        Qt::DirectConnection);
                }
            }
        }
        else if( strMth == "dtor" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 0 )
            {
                bValidTestStep = true;
                if( strTestGroupPath.contains("RenameTraceAdminObj") )
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
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                if( strTestGroupPath.contains("RenameTraceAdminObj") )
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
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                CMyClass2* pObj = nullptr;
                if( strTestGroupPath.contains("RenameTraceAdminObj") )
                {
                    pObj = m_multihshpMyClass2InstancesByName.value(strObjName);
                }
                else
                {
                    pObj = m_hshpMyClass2InstancesByName.value(strObjName);
                }
                pObj->instMethod(strlstInArgs[0]);
            }
        }
        else if( strMth == "recursiveTraceMethod" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 0 )
            {
                bValidTestStep = true;
                CMyClass2* pObj = nullptr;
                if( strTestGroupPath.contains("RenameTraceAdminObj") )
                {
                    pObj = m_multihshpMyClass2InstancesByName.value(strObjName);
                }
                else
                {
                    pObj = m_hshpMyClass2InstancesByName.value(strObjName);
                }
                pObj->recursiveTraceMethod();
            }
        }
        else if( strMth == "startMessageTimer" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 0 )
            {
                bValidTestStep = true;
                CMyClass2* pObj = nullptr;
                if( strTestGroupPath.contains("RenameTraceAdminObj") )
                {
                    pObj = m_multihshpMyClass2InstancesByName.value(strObjName);
                }
                else
                {
                    pObj = m_hshpMyClass2InstancesByName.value(strObjName);
                }
                pObj->startMessageTimer();
            }
        }
        else if( strMth == "startClass3Thread" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                CMyClass2* pMyClass2 = nullptr;
                if( strTestGroupPath.contains("RenameTraceAdminObj") )
                {
                    pMyClass2 = m_multihshpMyClass2InstancesByName.value(strObjName);
                }
                else
                {
                    pMyClass2 = m_hshpMyClass2InstancesByName.value(strObjName);
                }
                pMyClass2->startClass3Thread(strlstInArgs[0]);

                CMyClass3Thread* pMyClass3Thread = pMyClass2->getMyClass3Thread();
                CMyClass3* pMyClass3 = pMyClass2->getMyClass3();

                if( pMyClass3Thread == nullptr )
                {
                    strResultValue = "CMyClass3Thread Instance not created";
                }
                else if( pMyClass3 == nullptr )
                {
                    strResultValue = "CMyClass3 Instance not created";
                }
                else
                {
                    bValidTestStep = true;

                    QString strClass3ThreadObjName = pMyClass3Thread->objectName();
                    QString strClass3ObjName = pMyClass3->objectName();

                    if( strTestGroupPath.contains("RenameTraceAdminObj") )
                    {
                        m_multihshpMyClass3ThreadInstancesByName.insert(strClass3ThreadObjName, pMyClass3Thread);
                        m_multihshpMyClass3InstancesByName.insert(strClass3ObjName, pMyClass3);
                    }
                    else
                    {
                        m_hshpMyClass3ThreadInstancesByName[strClass3ThreadObjName] = pMyClass3Thread;
                        m_hshpMyClass3InstancesByName[strClass3ObjName] = pMyClass3;
                    }

                    QObject::connect(
                        pMyClass3Thread, &CMyClass3Thread::aboutToBeDestroyed,
                        this, &CTest::onClass3ThreadAboutToBeDestroyed,
                        Qt::DirectConnection);
                    QObject::connect(
                        pMyClass3, &CMyClass3::aboutToBeDestroyed,
                        this, &CTest::onClass3AboutToBeDestroyed,
                        Qt::DirectConnection);
                }
            }
        }
        else if( strMth == "stopClass3Thread" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 0 )
            {
                CMyClass2* pMyClass2 = nullptr;
                CMyClass3* pMyClass3 = nullptr;

                if( strTestGroupPath.contains("RenameTraceAdminObj") )
                {
                    pMyClass2 = m_multihshpMyClass2InstancesByName.value(strObjName);
                    pMyClass3 = m_multihshpMyClass3InstancesByName.value(strObjName);
                }
                else
                {
                    pMyClass2 = m_hshpMyClass2InstancesByName.value(strObjName);
                    pMyClass3 = m_hshpMyClass3InstancesByName.value(strObjName);
                }

                if( pMyClass2 == nullptr )
                {
                    strResultValue = "CMyClass2 Instance not created";
                }
                else if( pMyClass3 == nullptr )
                {
                    strResultValue = "CMyClass3 Instance not created";
                }
                else
                {
                    bValidTestStep = true;
                    // Slots onClass3ThreadAboutToBeDestroyed and onClass3AboutToBeDestroyed
                    // will remove the objects from the hashes.
                    pMyClass2->stopClass3Thread();
                }
            }
        }
    } // if( strClassName == CMyClass2::ClassName() )

    else if( strClassName == CMyClass2Thread::ClassName() )
    {
        if( strMth == "setObjectName" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass2ThreadInstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2Thread::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass2ThreadInstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2Thread::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                if( strTestGroupPath.contains("RenameTraceAdminObj") )
                {
                    CMyClass2Thread* pObj = m_multihshpMyClass2ThreadInstancesByName.take(strObjName);
                    pObj->setObjectName(strlstInArgs[0]);
                    m_multihshpMyClass2ThreadInstancesByName.insert(pObj->objectName(), pObj);
                }
                else
                {
                    CMyClass2Thread* pObj = m_hshpMyClass2ThreadInstancesByName.take(strObjName);
                    pObj->setObjectName(strlstInArgs[0]);
                    m_hshpMyClass2ThreadInstancesByName.insert(pObj->objectName(), pObj);
                }
            }
        }
    } // if( strClassName == CMyClass2Thread::ClassName() )

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
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && m_hshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is already existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;

                CMyClass3* pObj = new CMyClass3(strlstInArgs[0]);

                if( strTestGroupPath.contains("RenameTraceAdminObj") )
                {
                    m_multihshpMyClass3InstancesByName.insert(strObjName, pObj);
                }
                else
                {
                    m_hshpMyClass3InstancesByName[strObjName] = pObj;
                }

                QObject::connect(
                    pObj, &CMyClass3::aboutToBeDestroyed,
                    this, &CTest::onClass3AboutToBeDestroyed,
                    Qt::DirectConnection);
            }
            else if( strlstInArgs.size() == 2 )
            {
                if( strlstInArgs[1] == "nullptr")
                {
                    bValidTestStep = true;

                    CMyClass3* pObj = new CMyClass3(strlstInArgs[0], nullptr);

                    if( strTestGroupPath.contains("RenameTraceAdminObj") )
                    {
                        m_multihshpMyClass3InstancesByName.insert(strObjName, pObj);
                    }
                    else
                    {
                        m_hshpMyClass3InstancesByName[strObjName] = pObj;
                    }

                    QObject::connect(
                        pObj, &CMyClass3::aboutToBeDestroyed,
                        this, &CTest::onClass3AboutToBeDestroyed,
                        Qt::DirectConnection);
                }
            }
        }
        else if( strMth == "dtor" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 0 )
            {
                bValidTestStep = true;
                if( strTestGroupPath.contains("RenameTraceAdminObj") )
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
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                if( strTestGroupPath.contains("RenameTraceAdminObj") )
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
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                CMyClass3* pObj = nullptr;
                if( strTestGroupPath.contains("RenameTraceAdminObj") )
                {
                    pObj = m_multihshpMyClass3InstancesByName.value(strObjName);
                }
                else
                {
                    pObj = m_hshpMyClass3InstancesByName.value(strObjName);
                }
                pObj->instMethod(strlstInArgs[0]);
            }
        }
        else if( strMth == "noisyInstMethod" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                CMyClass3* pObj = nullptr;
                if( strTestGroupPath.contains("RenameTraceAdminObj") )
                {
                    pObj = m_multihshpMyClass3InstancesByName.value(strObjName);
                }
                else
                {
                    pObj = m_hshpMyClass3InstancesByName.value(strObjName);
                }
                pObj->noisyInstMethod(strlstInArgs[0]);
            }
        }
        else if( strMth == "veryNoisyInstMethod" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                CMyClass3* pObj = nullptr;
                if( strTestGroupPath.contains("RenameTraceAdminObj") )
                {
                    pObj = m_multihshpMyClass3InstancesByName.value(strObjName);
                }
                else
                {
                    pObj = m_hshpMyClass3InstancesByName.value(strObjName);
                }
                pObj->veryNoisyInstMethod(strlstInArgs[0]);
            }
        }
    } // if( strClassName == CMyClass3::ClassName() )

    else if( strClassName == CMyClass3Thread::ClassName() )
    {
        if( strMth == "setObjectName" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !strTestGroupPath.contains("RenameTraceAdminObj") && !m_hshpMyClass3ThreadInstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3Thread::" + strObjName + " is not existing";
            }
            else if( strTestGroupPath.contains("RenameTraceAdminObj") && !m_multihshpMyClass3ThreadInstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3Thread::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                if( strTestGroupPath.contains("RenameTraceAdminObj") )
                {
                    CMyClass3Thread* pObj = m_multihshpMyClass3ThreadInstancesByName.take(strObjName);
                    pObj->setObjectName(strlstInArgs[0]);
                    m_multihshpMyClass3ThreadInstancesByName.insert(pObj->objectName(), pObj);
                }
                else
                {
                    CMyClass3Thread* pObj = m_hshpMyClass3ThreadInstancesByName.take(strObjName);
                    pObj->setObjectName(strlstInArgs[0]);
                    m_hshpMyClass3ThreadInstancesByName.insert(pObj->objectName(), pObj);
                }
            }
        }
    } // if( strClassName == CMyClass2Thread::ClassName() )

    if( m_pTrcServer != nullptr )
    {
        m_pTrcServer->setNewTrcAdminObjsMethodCallsDefaultDetailLevel(DllIf::EMethodTraceDetailLevelNone);
        m_pTrcServer->setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel(DllIf::ELogDetailLevelNone);
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
        if( strMth == "sendMuchData" )
        {
            QString strInstruction = "Check whether client received many trace lines.\n";
            strInstruction = "If the client received too much data and disabled remote tracing\n";
            strInstruction += "enable remote tracing again afterwards.";
            i_pTestStep->setInstruction(strInstruction);
        }
        else
        {
            i_pTestStep->setInstruction("Check whether client received the expected trace output.");
        }

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
    }
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
void CTest::onClass2ThreadAboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    if( m_multihshpMyClass2ThreadInstancesByName.contains(i_strObjName) )
    {
        QList<CMyClass2Thread*> arpObjs = m_multihshpMyClass2ThreadInstancesByName.values(i_strObjName);

        for( auto& pObj : arpObjs )
        {
            if( pObj == i_pObj )
            {
                m_multihshpMyClass2ThreadInstancesByName.remove(i_strObjName, pObj);
            }
        }
    }

    if( m_hshpMyClass2ThreadInstancesByName.contains(i_strObjName) )
    {
        if( m_hshpMyClass2ThreadInstancesByName.value(i_strObjName) == i_pObj )
        {
            m_hshpMyClass2ThreadInstancesByName.remove(i_strObjName);
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

//------------------------------------------------------------------------------
void CTest::onClass3ThreadAboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    if( m_multihshpMyClass3ThreadInstancesByName.contains(i_strObjName) )
    {
        QList<CMyClass3Thread*> arpObjs = m_multihshpMyClass3ThreadInstancesByName.values(i_strObjName);

        for( auto& pObj : arpObjs )
        {
            if( pObj == i_pObj )
            {
                m_multihshpMyClass3ThreadInstancesByName.remove(i_strObjName, pObj);
            }
        }
    }

    if( m_hshpMyClass3ThreadInstancesByName.contains(i_strObjName) )
    {
        if( m_hshpMyClass3ThreadInstancesByName.value(i_strObjName) == i_pObj )
        {
            m_hshpMyClass3ThreadInstancesByName.remove(i_strObjName);
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

    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    strlst = i_strOperation.split("::", Qt::SkipEmptyParts);
    #else
    strlst = i_strOperation.split("::", QString::SkipEmptyParts);
    #endif

    if( strlst.size() == 3 )
    {
        o_strSubClassName = strlst[1];
        strlst.removeAt(1);
    }

    if( strlst.size() == 2 )
    {
        o_strClassName = strlst[0];
        #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        strlst = strlst[1].split(".", Qt::SkipEmptyParts);
        #else
        strlst = strlst[1].split(".", QString::SkipEmptyParts);
        #endif
        if( strlst.size() == 1 )
        {
            o_strMth = strlst[0];
        }
        else if( strlst.size() == 2 )
        {
            //strlst[0].replace("-", "::");
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
                #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
                strlst = strArgsRet.split(": ", Qt::SkipEmptyParts);
                #else
                strlst = strArgsRet.split(": ", QString::SkipEmptyParts);
                #endif
                if( strlst.size() == 1 )
                {
                    strlst[0].remove("(").remove(")");
                    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
                    o_strlstInArgs = strlst[0].split(", ", Qt::SkipEmptyParts);
                    #else
                    o_strlstInArgs = strlst[0].split(", ", QString::SkipEmptyParts);
                    #endif
                }
                else if( strlst.size() == 2 )
                {
                    strlst[0].remove("(").remove(")");
                    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
                    o_strlstInArgs = strlst[0].split(", ", Qt::SkipEmptyParts);
                    #else
                    o_strlstInArgs = strlst[0].split(", ", QString::SkipEmptyParts);
                    #endif
                    o_strMthRet = strlst[1];
                }
                if( o_strMth == "ctor" && o_strlstInArgs.size() >= 1 && o_strObjName.isEmpty() )
                {
                    //o_strlstInArgs[0].replace("-", "::");
                    o_strObjName = o_strlstInArgs[0];
                }
                else if( o_strMth == "dtor" && o_strlstInArgs.size() == 1 && o_strObjName.isEmpty() )
                {
                    //o_strlstInArgs[0].replace("-", "::");
                    o_strObjName = o_strlstInArgs[0];
                    o_strlstInArgs.clear();
                }
            }
        }
    }
} // splitMethodCallOperation
