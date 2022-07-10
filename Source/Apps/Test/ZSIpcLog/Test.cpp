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
#include <QtNetwork/qhostinfo.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qtextedit.h>
#else
#include <QtWidgets/qtextedit.h>
#endif

#include "Test.h"
#include "TestConfig.h"
#include "App.h"
#include "WidgetCentral.h"
#include "MyClass1.h"
#include "MyClass2.h"
#include "MyClass3.h"

#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStepIdxTree.h"
#include "ZSIpcLogGUI/ZSIpcLogWdgt.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysLogger.h"
#include "ZSSys/ZSSysLoggerIdxTree.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysAux.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Log;
using namespace ZS::Log::GUI;
using namespace ZS::Apps::Test::IpcLog;


/*******************************************************************************
class CTest
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest() :
//------------------------------------------------------------------------------
    ZS::Test::CTest("ZS::IpcLog"),
    m_pTmrTestStepTimeout(nullptr),
    m_hshReqsInProgress(),
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
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Startup",
        /* strOperation    */ "LogServer::startup",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerStartup(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Connect",
        /* strOperation    */ "LogClient::connect",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLogClientConnect(ZS::Test::CTestStep*)) );

    // Test Step Group - One Thread - Method Tracing - Class 1
    //--------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::classMethod",
        /* strOperation    */ "CMyClass1::classMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );

    // Test Step Group - One Thread - Method Tracing - Class 2
    //--------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(Inst1, nullptr)",
        /* strOperation    */ "CMyClass2::ctor(Inst1, nullptr)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(Inst2, nullptr)",
        /* strOperation    */ "CMyClass2::ctor(Inst2, nullptr)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-ctor-Inst2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.instMethod",
        /* strOperation    */ "CMyClass2::Inst1.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst2.instMethod",
        /* strOperation    */ "CMyClass2::Inst2.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::dtor(Inst1)",
        /* strOperation    */ "CMyClass2::dtor(Inst1)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-dtor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::dtor(Inst2)",
        /* strOperation    */ "CMyClass2::dtor(Inst2)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-dtor-Inst2");

    // Test Step Group - One Thread - Method Tracing - Class 2 - Logically Grouped Objects
    //------------------------------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(SoundCard-PowerLevel, nullptr)",
        /* strOperation    */ "CMyClass2::ctor(SoundCard-PowerLevel, nullptr)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-ctor-SoundCard-PowerLevel");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(Equalizer-PowerLevel, nullptr)",
        /* strOperation    */ "CMyClass2::ctor(Equalizer-PowerLevel, nullptr)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-ctor-Equalizer-PowerLevel");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::SoundCard-PowerLevel.instMethod",
        /* strOperation    */ "CMyClass2::SoundCard-PowerLevel.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Equalizer-PowerLevel.instMethod",
        /* strOperation    */ "CMyClass2::Equalizer-PowerLevel.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::dtor(SoundCard-PowerLevel)",
        /* strOperation    */ "CMyClass2::dtor(SoundCard-PowerLevel)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-dtor-SoundCard-PowerLevel");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::dtor(Equalizer-PowerLevel)",
        /* strOperation    */ "CMyClass2::dtor(Equalizer-PowerLevel)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-dtor-Equalizer-PowerLevel");

    // Test Step Group - One Thread - Method Tracing - Class 3
    //---------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::classMethod",
        /* strOperation    */ "CMyClass3::classMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::noisyClassMethod",
        /* strOperation    */ "CMyClass3::NoisyMethods::noisyClassMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::veryNoisyClassMethod",
        /* strOperation    */ "CMyClass3::VeryNoisyMethods::veryNoisyClassMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::ctor(Inst1, nullptr)",
        /* strOperation    */ "CMyClass3::ctor(Inst1, nullptr)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass3-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::ctor(Inst2, nullptr)",
        /* strOperation    */ "CMyClass3::ctor(Inst2, nullptr)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass3-ctor-Inst2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst1.instMethod",
        /* strOperation    */ "CMyClass3::Inst1.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst2.instMethod",
        /* strOperation    */ "CMyClass3::Inst2.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst1.noisyInstMethod",
        /* strOperation    */ "CMyClass3::NoisyMethods::Inst1.noisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst2.noisyInstMethod",
        /* strOperation    */ "CMyClass3::NoisyMethods::Inst2.noisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst1.veryNoisyInstMethod",
        /* strOperation    */ "CMyClass3::VeryNoisyMethods::Inst1.veryNoisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst2.veryNoisyInstMethod",
        /* strOperation    */ "CMyClass3::VeryNoisyMethods::Inst2.veryNoisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::dtor(Inst1)",
        /* strOperation    */ "CMyClass3::dtor(Inst1)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass3-dtor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::dtor(Inst2)",
        /* strOperation    */ "CMyClass3::dtor(Inst2)",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass3-dtor-Inst2");

    // Test Step Group - One Thread - Method Tracing - Data Filter
    //------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " DataFilter",
        /* strOperation    */ "CMyClass2::Inst1.Logger.setTraceDataFilters()",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepDataFilter(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-Inst1-Logger-setTraceDataFilters");

    // Test Step Group - One Thread - Shutdown
    //----------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Disconnect",
        /* strOperation    */ "LogClient::disconnect",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLogClientDisconnect(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Shutdown",
        /* strOperation    */ "LogServer::shutdown",
        /* pTSGrpParent    */ pTestGroupOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerShutdown(ZS::Test::CTestStep*)) );

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
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Startup",
        /* strOperation    */ "LogServer::startup",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerStartup(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Connect",
        /* strOperation    */ "LogClient::connect",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLogClientConnect(ZS::Test::CTestStep*)) );

    // Test Step Group - Several Threads - Method Tracing - Class 1 - StartClass2Thread
    //---------------------------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::ctor(Inst1)",
        /* strOperation    */ "CMyClass1::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass1-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::Inst1.startClass2Thread(Inst1)",
        /* strOperation    */ "CMyClass1::Inst1.startClass2Thread(Inst1): Inst1",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass1-Inst1-startClass2Thread-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.recursiveTraceMethod()",
        /* strOperation    */ "CMyClass2::Inst1.recursiveTraceMethod(): 0",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-Inst1-recursiveTraceMethod");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.startMessageTimer()",
        /* strOperation    */ "CMyClass2::Inst1.startMessageTimer()",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-Inst1-startMessageTimer");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.startClass3Thread(Inst1)",
        /* strOperation    */ "CMyClass2::Inst1.startClass3Thread(Inst1): Inst1",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-Inst1-startClass3Thread-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.stopClass3Thread()",
        /* strOperation    */ "CMyClass2::Inst1.stopClass3Thread()",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-Inst1-stopClass3Thread");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::Inst1.stopClass2Thread()",
        /* strOperation    */ "CMyClass1::Inst1.stopClass2Thread()",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass1-Inst1-stopClass2Thread");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::dtor(Inst1)",
        /* strOperation    */ "CMyClass1::dtor(Inst1)",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass1-dtor-Inst1");

    // Test Step Group - Several Threads - Shutdown
    //-----------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Disconnect",
        /* strOperation    */ "LogClient::disconnect",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLogClientDisconnect(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Shutdown",
        /* strOperation    */ "LogServer::shutdown",
        /* pTSGrpParent    */ pTestGroupSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerShutdown(ZS::Test::CTestStep*)) );

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
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Startup",
        /* strOperation    */ "LogServer::startup",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerStartup(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Connect",
        /* strOperation    */ "LogClient::connect",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLogClientConnect(ZS::Test::CTestStep*)) );

    // Test Step Group - RenameTraceAdminObj - One Thread - Several instances of class tracer
    //---------------------------------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::ctor(Inst1)",
        /* strOperation    */ "CMyClass1::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass1-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::ctor(Inst1)",
        /* strOperation    */ "CMyClass1::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass1-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::Inst1.setObjectName(NewName1)",
        /* strOperation    */ "CMyClass1::Inst1.setObjectName(NewName1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass1-Inst1-setObjectName-NewName1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::Inst1.setObjectName(NewName2)",
        /* strOperation    */ "CMyClass1::Inst1.setObjectName(NewName2)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass1-Inst1-setObjectName-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::NewName1.setObjectName(NewName2)",
        /* strOperation    */ "CMyClass1::NewName1.setObjectName(NewName2)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass1-NewName1-setObjectName-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::dtor(NewName2)",
        /* strOperation    */ "CMyClass1::dtor(NewName2)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass1-dtor-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::dtor(NewName2)",
        /* strOperation    */ "CMyClass1::dtor(NewName2)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass1-dtor-NewName2");

    // Test Step Group - RenameTraceAdminObj - One Thread - Several instances of instance tracer
    //------------------------------------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(Inst1)",
        /* strOperation    */ "CMyClass2::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::ctor(Inst1)",
        /* strOperation    */ "CMyClass2::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.setObjectName(NewName1)",
        /* strOperation    */ "CMyClass2::Inst1.setObjectName(NewName1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-Inst1-setObjectName-NewName1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.setObjectName(NewName2)",
        /* strOperation    */ "CMyClass2::Inst1.setObjectName(NewName2)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-Inst1-setObjectName-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::NewName1.setObjectName(NewName2)",
        /* strOperation    */ "CMyClass2::NewName1.setObjectName(NewName2)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-NewName1-setObjectName-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::dtor(NewName2)",
        /* strOperation    */ "CMyClass2::dtor(NewName2)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-dtor-NewName2");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::dtor(NewName2)",
        /* strOperation    */ "CMyClass2::dtor(NewName2)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-dtor-NewName2");

    // Test Step Group - RenameTraceAdminObj - One Thread - Shutdown
    //--------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Disconnect",
        /* strOperation    */ "LogClient::disconnect",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLogClientDisconnect(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Shutdown",
        /* strOperation    */ "LogServer::shutdown",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjOneThread,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerShutdown(ZS::Test::CTestStep*)) );

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
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Startup",
        /* strOperation    */ "LogServer::startup",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerStartup(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Connect",
        /* strOperation    */ "LogClient::connect",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLogClientConnect(ZS::Test::CTestStep*)) );

    // Test Step Group - RenameTraceAdminObj - Several Threads - Several instances of class tracer
    //--------------------------------------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::ctor(Inst1)",
        /* strOperation    */ "CMyClass1::ctor(Inst1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass1-ctor-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::Inst1.startClass2Thread(Inst1)",
        /* strOperation    */ "CMyClass1::Inst1.startClass2Thread(Inst1): Inst1",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass1-Inst1-startClass2Thread-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.startClass3Thread(Inst1)",
        /* strOperation    */ "CMyClass2::Inst1.startClass3Thread(Inst1): Inst1",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-Inst1-startClass3Thread-Inst1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.instMethod",
        /* strOperation    */ "CMyClass2::Inst1.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-Inst1-instMethod-queued");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst1.noisyInstMethod",
        /* strOperation    */ "CMyClass3::NoisyMethods::Inst1.noisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass3-Inst1-noisyInstMethod-queued");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst1.veryNoisyInstMethod",
        /* strOperation    */ "CMyClass3::VeryNoisyMethods::Inst1.veryNoisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass3-Inst1-veryNoisyInstMethod-queued");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2Thread::Inst1.setObjectName(NewName1)",
        /* strOperation    */ "CMyClass2Thread::MyClass2ThreadInst1.setObjectName(MyClass2ThreadNewName1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2Thread-Inst1-setObjectName-NewName1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::Inst1.setObjectName(NewName1)",
        /* strOperation    */ "CMyClass2::Inst1.setObjectName(NewName1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-Inst1-setObjectName-NewName1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3Thread::Inst1.setObjectName(NewName1)",
        /* strOperation    */ "CMyClass3Thread::MyClass3ThreadInst1.setObjectName(MyClass3ThreadNewName1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass3Thread-Inst1-setObjectName-NewName1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::Inst1.setObjectName(NewName1)",
        /* strOperation    */ "CMyClass3::Inst1.setObjectName(NewName1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass3-Inst1-setObjectName-NewName1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::NewName1.instMethod",
        /* strOperation    */ "CMyClass2::NewName1.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-NewName1-instMethod-queued");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::NewName1.noisyInstMethod",
        /* strOperation    */ "CMyClass3::NoisyMethods::NewName1.noisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-NewName1-noisyInstMethod-queued");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass3::NewName1.veryNoisyInstMethod",
        /* strOperation    */ "CMyClass3::VeryNoisyMethods::NewName1.veryNoisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-NewName1-veryNoisyInstMethod-queued");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass2::NewName1.stopClass3Thread()",
        /* strOperation    */ "CMyClass2::NewName1.stopClass3Thread()",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass2-NewName1-stopClass3Thread");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::Inst1.stopClass2Thread()",
        /* strOperation    */ "CMyClass1::Inst1.stopClass2Thread()",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass1-Inst1-stopClass2Thread-NewName1");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CMyClass1::dtor(Inst1)",
        /* strOperation    */ "CMyClass1::dtor(Inst1)",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLog(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "LogServer-CMyClass1-dtor-Inst1");

    // Test Step Group - RenameTraceAdminObj - Several Threads - Shutdown
    //-------------------------------------------------------------------

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Client Disconnect",
        /* strOperation    */ "LogClient::disconnect",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLogClientDisconnect(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Trace Server Shutdown",
        /* strOperation    */ "LogServer::shutdown",
        /* pTSGrpParent    */ pTestGroupRenameTraceAdminObjSeveralThreads,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerShutdown(ZS::Test::CTestStep*)) );

    // Recall test step settings
    //==========================

    recallTestSteps();

} // default ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo = saveTestSteps();

    if(errResultInfo.isErrorResult())
    {
        if(CErrLog::GetInstance() != nullptr)
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
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

    for( auto& pObj : m_hshpMyClass2ThreadInstancesByName )
    {
        QObject::disconnect(
            /* pObjSender   */ pObj,
            /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onClass2ThreadAboutToBeDestroyed(QObject*, const QString&)) );

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

    for( auto& pObj : m_hshpMyClass3ThreadInstancesByName )
    {
        QObject::disconnect(
            /* pObjSender   */ pObj,
            /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onClass3ThreadAboutToBeDestroyed(QObject*, const QString&)) );

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

    while( !m_multihshpMyClass2ThreadInstancesByName.isEmpty() )
    {
        QMultiHash<QString, CMyClass2Thread*>::iterator it = m_multihshpMyClass2ThreadInstancesByName.begin();

        CMyClass2Thread* pObj = it.value();

        QObject::disconnect(
            /* pObjSender   */ pObj,
            /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onClass2ThreadAboutToBeDestroyed(QObject*, const QString&)) );

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

    while( !m_multihshpMyClass3ThreadInstancesByName.isEmpty() )
    {
        QMultiHash<QString, CMyClass3Thread*>::iterator it = m_multihshpMyClass3ThreadInstancesByName.begin();

        CMyClass3Thread* pObj = it.value();

        QObject::disconnect(
            /* pObjSender   */ pObj,
            /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onClass3ThreadAboutToBeDestroyed(QObject*, const QString&)) );

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

    try
    {
        delete m_pTmrTestStepTimeout;
    }
    catch(...)
    {
    }

    m_pTmrTestStepTimeout = nullptr;
    m_hshReqsInProgress.clear();
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
void CTest::doTestStepLogServerStartup( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    CIpcLogServer* pLogServer = ZS::Log::CIpcLogServer::GetInstance();

    if( pLogServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Log server not existing");
    }
    else
    {
        pLogServer->clearLocalLogFile();

        ZS::Ipc::SServerHostSettings hostSettings = pLogServer->getHostSettings();

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

    if( pLogServer != nullptr )
    {
        CRequest* pReq = pLogServer->startup(5000);

        if( isAsynchronousRequest(pReq) )
        {
            strResultValue = "startupLogServer not expected to be asynchronous";
        }
        else // if( isAsynchronousRequest(pReq) )
        {
            ZS::Ipc::SServerHostSettings hostSettings = pLogServer->getHostSettings();

            strResultValue = ZS::Ipc::CServer::State2Str(pLogServer->getState()) + " (";
            strResultValue += hostSettings.m_strLocalHostName;
            strResultValue += ":" + QString::number(hostSettings.m_uLocalPort);
            strResultValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
            strResultValue += ")";
        }
        strlstResultValues.append(strResultValue);
    }

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepLogServerStartup

//------------------------------------------------------------------------------
void CTest::doTestStepLogServerShutdown( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    CIpcLogServer* pLogServer = ZS::Log::CIpcLogServer::GetInstance();

    if( pLogServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Trace server not existing");
    }
    else
    {
        ZS::Ipc::SServerHostSettings hostSettings = pLogServer->getHostSettings();

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

    if( pLogServer != nullptr )
    {
        CRequest* pReq = pLogServer->shutdown();

        if( isAsynchronousRequest(pReq) )
        {
            strResultValue = "shutdownLogServer not expected to be asynchronous";
        }
        else // if( isAsynchronousRequest(pReq) )
        {
            ZS::Ipc::SServerHostSettings hostSettings = pLogServer->getHostSettings();

            strResultValue = ZS::Ipc::CServer::State2Str(pLogServer->getState()) + " (";
            strResultValue += hostSettings.m_strLocalHostName;
            strResultValue += ":" + QString::number(hostSettings.m_uLocalPort);
            strResultValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
            strResultValue += ")";
        }
        strlstResultValues.append(strResultValue);

        i_pTestStep->setResultValues(strlstResultValues);
    }

} // doTestStepLogServerShutdown

//------------------------------------------------------------------------------
void CTest::doTestStepLogClientConnect( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    QVector<CIdxTreeEntry*> arpTreeEntriesServer;

    CIpcLogClient* pLogClient = CApplication::GetInstance()->getLogClient();
    CIpcLogServer* pLogServer = ZS::Log::CIpcLogServer::GetInstance();

    if( pLogClient == nullptr || pLogServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Trace server or trace client not existing");
    }
    else
    {
        ZS::Ipc::SClientHostSettings hostSettings = pLogClient->getHostSettings();

        strExpectedValue = ZS::Ipc::CClient::State2Str(Ipc::CClient::EStateConnected) + " (";
        strExpectedValue += hostSettings.m_strRemoteHostName;
        strExpectedValue += ":" + QString::number(hostSettings.m_uRemotePort);
        strExpectedValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
        strExpectedValue += ")";
        strlstExpectedValues.append(strExpectedValue);

        CIdxTreeLoggers* pIdxTreeServer = pLogServer->getLoggersIdxTree();

        arpTreeEntriesServer = pIdxTreeServer->treeEntriesVec();

        for( auto& pTreeEntry : arpTreeEntriesServer )
        {
            if( pTreeEntry != nullptr && pTreeEntry->entryType() == EIdxTreeEntryType::Leave)
            {
                CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);

                // Please note that the trace clients gateway admin objects (thread, sockets) will be created
                // after invoking the connect method. If the admin objects XML file did not exist those
                // trace admin objects are not yet added to the index tree but will be send to the trace client
                // as they will be created during the connect request.
                if( !pLogger->keyInTree().startsWith("L:ZS::Ipc") && !pLogger->keyInTree().startsWith("L:ZS::Trace") )
                {
                    strExpectedValue = pLogger->keyInTree() + ": ";
                    strExpectedValue += "RefCount: " + QString::number(pLogger->getRefCount());
                    strExpectedValue += ", Enabled: " + CEnumEnabled(pLogger->getEnabled()).toString();
                    strExpectedValue += ", DetailLevel: " + CEnumLogDetailLevel(pLogger->getDetailLevel()).toString();
                    strlstExpectedValues.append(strExpectedValue);
                }
            }
        }
        i_pTestStep->setExpectedValues(strlstExpectedValues);
    }

    // Test Step
    //----------

    if( pLogServer != nullptr && pLogClient != nullptr )
    {
        CRequest* pReq = pLogClient->connect_();

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
                    /* pObjSender   */ pLogClient,
                    /* szSignal     */ SIGNAL(traceAdminObjInserted(QObject*, const QString&)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onLogClientLoggerInserted(QObject*, const QString&)) ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            }
        }
        else // if( !isAsynchronousRequest(pReq) )
        {
            strResultValue = "connectLogClient expected to be asynchronous";
            strlstResultValues.append(strResultValue);
            i_pTestStep->setResultValues(strlstResultValues);
        }
    }

} // doTestStepLogClientConnect

//------------------------------------------------------------------------------
void CTest::doTestStepLogClientDisconnect( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    CIpcLogClient* pLogClient = CApplication::GetInstance()->getLogClient();

    if( pLogClient == nullptr )
    {
        i_pTestStep->setExpectedValue("Trace client not existing");
    }
    else
    {
        ZS::Ipc::SClientHostSettings hostSettings = pLogClient->getHostSettings();

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

        //strExpectedValue = "TrcLoggersIdxTree.isEmpty";
        //strlstExpectedValues.append(strExpectedValue);

        i_pTestStep->setExpectedValues(strlstExpectedValues);
    }

    // Test Step
    //----------

    if( pLogClient != nullptr )
    {
        CRequest* pReq = pLogClient->disconnect_();

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
            strResultValue = "disconnectLogClient expected to be asynchronous";
            strlstResultValues.append(strResultValue);
            i_pTestStep->setResultValues(strlstResultValues);
        }
    }

} // doTestStepLogClientDisconnect

//------------------------------------------------------------------------------
void CTest::doTestStepLog( ZS::Test::CTestStep* i_pTestStep )
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

    CIpcLogServer* pLogServer = ZS::Log::CIpcLogServer::GetInstance();

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

    if( pLogServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Trace server not existing");
    }
    else
    {
        pLogServer->setNewLoggersDefaultDetailLevel(ELogDetailLevel::DebugNormal);

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
        if( strMth == "ctor" )
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

                    if( !QObject::connect(
                        /* pObjSender   */ pMyClass2Thread,
                        /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onClass2ThreadAboutToBeDestroyed(QObject*, const QString&)),
                        /* cnctType     */ Qt::DirectConnection ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }
                    if( !QObject::connect(
                        /* pObjSender   */ pMyClass2,
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
                    // Slots onClass2ThreadAboutToBeDestroyed and onClass2AboutToBeDestroyed
                    // will remove the objects from the hashes.
                    pMyClass1->stopClass2Thread();
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

                    if( strTestGroupPath.contains("RenameTraceAdminObj") )
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

                    if( !QObject::connect(
                        /* pObjSender   */ pMyClass3Thread,
                        /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onClass3ThreadAboutToBeDestroyed(QObject*, const QString&)),
                        /* cnctType     */ Qt::DirectConnection ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }
                    if( !QObject::connect(
                        /* pObjSender   */ pMyClass3,
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
        if( strMth == "ctor" )
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

                    if( strTestGroupPath.contains("RenameTraceAdminObj") )
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

    if( pLogServer != nullptr )
    {
        pLogServer->setNewLoggersDefaultDetailLevel(ELogDetailLevel::None);
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
        CWidgetCentral* pWdgtCentral = CWidgetCentral::GetInstance();

        CWdgtLog* pWdgtLog = pWdgtCentral->getLogWdgt();

        if( pWdgtLog != nullptr )
        {
            pWdgtLog->getTextEdit()->clear();

            if( !QObject::connect(
                /* pObjSender   */ pWdgtLog,
                /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onLogClientLogWdgtTextItemAdded(const QString&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            m_pTmrTestStepTimeout->start(iTestStepTimeout_ms);
        }
        else
        {
            i_pTestStep->setResultValue("No trace method list widget");
        }
    }

} // doTestStepLog

//------------------------------------------------------------------------------
void CTest::doTestStepDataFilter( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    int iTestStepTimeout_ms = 5000;

    // Expected Values
    //----------------

    QStringList strlstExpectedValues;

    QString strExpectedResultsAbsFilePath;
    QVariant val = i_pTestStep->getConfigValue("ExpectedResultsFileName");
    if( val.isValid() && val.canConvert(QVariant::String) )
    {
        strExpectedResultsAbsFilePath = c_strExpectedResultsAbsDirPath + QDir::separator() + val.toString() + ".txt";
    }

    if( strExpectedResultsAbsFilePath.isEmpty() )
    {
        strlstExpectedValues.append("Missing ExpectedResultsFileName");
    }
    else
    {
        readExpectedTestResults(strExpectedResultsAbsFilePath, strlstExpectedValues);
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    QString strObjName = "Inst1";

    CMyClass2* pObj = new CMyClass2(strObjName);

    m_hshpMyClass2InstancesByName[strObjName] = pObj;

    if( !QObject::connect(
        /* pObjSender   */ pObj,
        /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onClass2AboutToBeDestroyed(QObject*, const QString&)),
        /* cnctType     */ Qt::DirectConnection ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    pObj->getLogger()->setDataFilter("");
    pObj->instMethod("No Filter Set");

    // Check if the string "abc" is contained:
    pObj->getLogger()->setDataFilter("abc");
    pObj->instMethod("1 abc bca cab"); // must occur in log output
    pObj->instMethod("2 xyz yzx zxy"); // must be suppressed in log output

    // Check if the strings "def" or "uvw" are contained:
    pObj->getLogger()->setDataFilter("def|uvw");
    pObj->instMethod("3 def efd fde"); // must occur in log output
    pObj->instMethod("4 uvw vwu wuv"); // must occur in log output

    // Check if the string "ghi" is NOT contained:
    pObj->getLogger()->setDataFilter("^((?!ghi).)*$");
    pObj->instMethod("5 ghi hig igh"); // must be suppressed in log output
    pObj->instMethod("6 rst str trs"); // must occur in log output

    pObj->getLogger()->setDataFilter("");
    pObj->instMethod("No Filter Set");

    // Remove filter so that the dtor is traced.
    pObj->getLogger()->setDataFilter("");
    delete pObj;
    pObj = nullptr;

    // Result Values
    //--------------

    CWidgetCentral* pWdgtCentral = CWidgetCentral::GetInstance();

    CWdgtLog* pWdgtLog = pWdgtCentral->getLogWdgt();

    if( pWdgtLog != nullptr )
    {
        pWdgtLog->getTextEdit()->clear();

        if( !QObject::connect(
            /* pObjSender   */ pWdgtLog,
            /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onLogClientLogWdgtTextItemAdded(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        m_pTmrTestStepTimeout->start(iTestStepTimeout_ms);
    }
    else
    {
        i_pTestStep->setResultValue("No trace method list widget");
    }

} // doTestStepDataFilter

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
                CIpcLogClient* pLogClient = nullptr;
                QVector<CIdxTreeEntry*> arpTreeEntriesServer;

                if( pTestStep->getOperation().contains("LogClient") )
                {
                    CIpcLogServer* pLogServer = CApplication::GetInstance()->getLogServer();
                    if( pLogServer != nullptr )
                    {
                        CIdxTreeLoggers* pIdxTreeServer = pLogServer->getLoggersIdxTree();
                        arpTreeEntriesServer = pIdxTreeServer->treeEntriesVec();
                    }
                    pLogClient = CApplication::GetInstance()->getLogClient();
                }

                if( pLogClient != nullptr )
                {
                    if( pTestStep->getOperation().contains("LogClient::connect") )
                    {
                        // If the trace server has already admin objects in the index tree the test step
                        // is not finished if the trace client is connected.
                        // The client got to query the trace admin objects after the connection
                        // has been established. We need to wait until all expected admin objects
                        // have been received by the client before finishing this test step.
                        // On receiving the admin objects the slot "onLogClientLoggerInserted"
                        // is called. If all expected admin objects are received this slot finishes
                        // the test step.

                        // If the trace server does not have any admin objects in the index tree the test step
                        // is finished after the trace client is connected. But this should not be the case.
                        if( arpTreeEntriesServer.size() == 0 )
                        {
                            QString     strResultValue;
                            QStringList strlstResultValues;

                            ZS::Ipc::SClientHostSettings hostSettings = pLogClient->getHostSettings();

                            strResultValue = ZS::Ipc::CClient::State2Str(pLogClient->getState()) + " (";
                            strResultValue += hostSettings.m_strRemoteHostName;
                            strResultValue += ":" + QString::number(hostSettings.m_uRemotePort);
                            strResultValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                            strResultValue += ")";
                            strlstResultValues.append(strResultValue);

                            pTestStep->setResultValues(strlstResultValues);
                        }
                    } // if( pTestStep->getOperation().contains("LogClient::connect") )

                    else if( pTestStep->getOperation().contains("LogClient::disconnect") )
                    {
                        QString     strResultValue;
                        QStringList strlstResultValues;

                        ZS::Ipc::SClientHostSettings hostSettings = pLogClient->getHostSettings();

                        strResultValue = ZS::Ipc::CClient::State2Str(pLogClient->getState()) + " (";
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

                    } // if( pTestStep->getOperation().contains("LogClient::disconnect") )
                } // if( pLogClient != nullptr )
            } // if( pTestStep != nullptr )
        } // if( errResultInfo.isErrorResult() || i_reqDscr.m_iProgress_perCent >= 100 )
    } // if( m_hshReqsInProgress.contains(i_reqDscr.m_iId) )

} // onRequestChanged

//------------------------------------------------------------------------------
void CTest::onLogClientLoggerInserted( QObject* /*i_pLogClient*/, const QString& /*i_strKeyInTree*/ )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStep* pTestStep = getCurrentTestStep();

    if( pTestStep != nullptr )
    {
        CIpcLogClient* pLogClient = CApplication::GetInstance()->getLogClient();
        QVector<CIdxTreeEntry*> arpTreeEntriesServer;

        CIpcLogServer* pLogServer = CApplication::GetInstance()->getLogServer();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTreeServer = pLogServer->getLoggersIdxTree();
            arpTreeEntriesServer = pIdxTreeServer->treeEntriesVec();
        }

        if( pLogClient != nullptr )
        {
            if( pTestStep->getOperation().contains("LogClient::connect") )
            {
                // Test step is finished if client is connected and received all trace admin objects from the server.

                // Actual Values
                //---------------

                CIdxTreeLoggers* pIdxTreeClient = pLogClient->getLoggersIdxTree();

                QVector<CIdxTreeEntry*> arpTreeEntriesClient = pIdxTreeClient->treeEntriesVec();

                bool bTestStepFinished = false;

                if( pLogClient->isConnected() && arpTreeEntriesClient.size() == arpTreeEntriesServer.size() )
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
                } // if( pLogClient->isConnected() && arpTreeEntriesClient.size() == arpTreeEntriesServer.size() )

                if( bTestStepFinished )
                {
                    if( m_pTmrTestStepTimeout->isActive() )
                    {
                        m_pTmrTestStepTimeout->stop();
                    }

                    QObject::disconnect(
                        /* pObjSender   */ pLogClient,
                        /* szSignal     */ SIGNAL(traceAdminObjInserted(QObject*, const QString&)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onLogClientLoggerInserted(QObject*, const QString&)) );

                    QString     strResultValue;
                    QStringList strlstResultValues;

                    ZS::Ipc::SClientHostSettings hostSettings = pLogClient->getHostSettings();

                    strResultValue = ZS::Ipc::CClient::State2Str(pLogClient->getState()) + " (";
                    strResultValue += hostSettings.m_strRemoteHostName;
                    strResultValue += ":" + QString::number(hostSettings.m_uRemotePort);
                    strResultValue += ":" + ZS::Ipc::socketType2Str(hostSettings.m_socketType);
                    strResultValue += ")";
                    strlstResultValues.append(strResultValue);

                    for( auto& pTreeEntry : arpTreeEntriesClient )
                    {
                        if( pTreeEntry != nullptr && pTreeEntry->entryType() == EIdxTreeEntryType::Leave)
                        {
                            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);

                            // Please note that the trace clients gateway admin objects (thread, sockets) will be created
                            // after invoking the connect method. If the admin objects XML file did not exist those
                            // trace admin objects are not yet added to the index tree but will be send to the trace client
                            // as they will be created during the connect request.
                            if( !pLogger->keyInTree().startsWith("L:ZS::Ipc") && !pLogger->keyInTree().startsWith("L:ZS::Trace") )
                            {
                                strResultValue = pLogger->keyInTree() + ": ";
                                strResultValue += "RefCount: " + QString::number(pLogger->getRefCount());
                                strResultValue += ", Enabled: " + CEnumEnabled(pLogger->getEnabled()).toString();
                                strResultValue += ", DetailLevel: " + CEnumLogDetailLevel(pLogger->getDetailLevel()).toString();
                                strlstResultValues.append(strResultValue);
                            }
                        }
                    }

                    pTestStep->setResultValues(strlstResultValues);

                } // if( bTestStepFinished )
            } // if( pTestStep->getOperation().contains("LogClient::connect") )
        } // if( pLogClient != nullptr )
    } // if( pTestStep != nullptr )

} // onLogClientLoggerInserted

//------------------------------------------------------------------------------
void CTest::onLogClientLogWdgtTextItemAdded( const QString& i_strText )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStep* pTestStep = getCurrentTestStep();

    if( pTestStep != nullptr )
    {
        QString strOperation = pTestStep->getOperation();

        QString strNameSpace;
        QString strClassName;
        QString strSubClassName;
        QString strObjName;
        QString strMth;
        QStringList strlstInArgs;
        QString strMthRet;

        splitMethodCallOperation(strOperation, strClassName, strSubClassName, strObjName, strMth, strlstInArgs, strMthRet);

        CLogServer* pLogServer = CLogServer::GetInstance();
        CWidgetCentral* pWdgtCentral = CWidgetCentral::GetInstance();
        CWdgtLog* pWdgtLog = pWdgtCentral->getLogWdgt();

        if( pWdgtLog != nullptr )
        {
            QString     strResultValue;
            QStringList strlstResultValues;
            QStringList strlstExpectedValues = pTestStep->getExpectedValues();

            QTextEdit* pEdtTrcMthList = pWdgtLog->getTextEdit();
            QTextDocument* pDocTrcMthList = pEdtTrcMthList->document();

            int iDocTrcMthListLineCount = pDocTrcMthList->lineCount();

            if( iDocTrcMthListLineCount == strlstExpectedValues.size() )
            {
                // Retrieve result values from trace method list widget
                //-----------------------------------------------------

                // The list widget must be cleared on starting a new test step.
                QString strAllLines = pEdtTrcMthList->toPlainText();
                QTextStream txtStream(&strAllLines, QIODevice::ReadOnly);
                while (!txtStream.atEnd())
                {
                    QString strLine = txtStream.readLine();
                    // Remove time information.
                    int idxEOfThread = strLine.indexOf("> ");
                    int idxMthTrc = strLine.indexOf("): ");
                    if( idxMthTrc >= 0 ) {
                        idxMthTrc += 3;
                    }
                    if( idxMthTrc < 0 ) {
                        idxMthTrc = strLine.indexOf("->");
                    }
                    if( idxMthTrc < 0 ) {
                        idxMthTrc = strLine.indexOf("<-");
                    }
                    if( idxEOfThread >= 0 && idxMthTrc >= 0 ) {
                        strLine = strLine.remove(idxEOfThread + 2, idxMthTrc - idxEOfThread - 2);
                    }
                    strlstResultValues << strLine;
                }

                // Check if entry added to log file.
                //----------------------------------

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
                QString strTrcMthFileBaseName = "LogServer-TrcMth00";

                QString strTrcMthFileAbsFilePath = strAppLogDir + "/" + strTrcMthFileBaseName + "." + strTrcMthFileSuffix;

                QFile fileTrcMthFile(strTrcMthFileAbsFilePath);

                // Temporarily disable trace output. This will close (flush buffer) the file
                // so that its content can be evaluated by onLogClientLogWdgtTextItemAdded.
                if( pLogServer != nullptr )
                {
                    pLogServer->setEnabled(false);
                    pLogServer->setEnabled(true);
                }

                if( !fileTrcMthFile.open(QIODevice::ReadOnly|QIODevice::Text) )
                {
                    strlstResultValues.append("Could not open file " + strTrcMthFileAbsFilePath);
                }
                else
                {
                    QTextStream txtstrmTrcMthFile(&fileTrcMthFile);
                    QString strTrcMthFile = txtstrmTrcMthFile.readAll();
                    fileTrcMthFile.close();

                    for( auto& strLine : strlstExpectedValues )
                    {
                        // Remove Thread Name.
                        int idxEOfThread = strLine.indexOf("> ");
                        if( idxEOfThread >= 0 ) {
                            strLine = strLine.remove(0, idxEOfThread + 2);
                        }
                        bool bEntryFound = strTrcMthFile.contains(strLine);
                        if( !bEntryFound )
                        {
                            strlstResultValues.append(strLine + ": not found in trace method file");
                        }
                    }
                }

                if( !strlstResultValues.isEmpty() || !strResultValue.isEmpty() )
                {
                    // Test step finished (only one entry expected).
                    if( m_pTmrTestStepTimeout->isActive() )
                    {
                        m_pTmrTestStepTimeout->stop();
                    }

                    QObject::disconnect(
                        /* pObjSender   */ pWdgtLog,
                        /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onLogClientLogWdgtTextItemAdded(const QString&)) );
                }

                if( !strlstResultValues.isEmpty() )
                {
                    pTestStep->setResultValues(strlstResultValues);
                }
                else if( !strResultValue.isEmpty() )
                {
                    pTestStep->setResultValue(strResultValue);
                }
            } // if( iDocTrcMthListLineCount == strlstExpectedValues.size() )
        } // if( pWdgtLog != nullptr )
    } // if( pTestStep != nullptr )

} // onLogClientLogWdgtTextItemAdded

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

        if( pTestStep->getOperation() == "CIpcLogClient::connect" )
        {
            CIpcLogClient* pLogClient = CApplication::GetInstance()->getLogClient();

            QObject::disconnect(
                /* pObjSender   */ pLogClient,
                /* szSignal     */ SIGNAL(traceAdminObjInserted(QObject*, const QString&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onLogClientLoggerInserted(QObject*, const QString&)) );
        }
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
