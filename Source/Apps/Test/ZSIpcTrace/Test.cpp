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
#include "TestModule3.h"

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
    m_hshpMyClass1InstancesByName(),
    m_hshpMyClass2InstancesByName(),
    m_hshpMyClass3InstancesByName()
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
    //===================================

    int idxGroup = 0;
    int idxStep = 0;

    #if 0

    pGrp = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " One Trace Server",
        /* pTSGrpParent    */ nullptr );

    // Test Step Group - One Trace Server - Startup
    //----------------------------------------------

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer::startup",
        /* strOperation    */ "ZSTrcServer::startup",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerStartup(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcClient::connect",
        /* strOperation    */ "ZSTrcClient::connect",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientConnect(ZS::Test::CTestStep*)) );

    // Test Step Group - One Trace Server - Method Tracing - Class 1
    //--------------------------------------------------------------

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass1::classMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass1::classMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    // Test Step Group - One Trace Server - Method Tracing - Class 2
    //--------------------------------------------------------------

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass2::ctor(Inst1, nullptr)",
        /* strOperation    */ "ZSTrcServer/CMyClass2::ctor(Inst1, nullptr)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass2::ctor(Inst2, nullptr)",
        /* strOperation    */ "ZSTrcServer/CMyClass2::ctor(Inst2, nullptr)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass2::Inst1.instMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass2::Inst1.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass2::Inst2.instMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass2::Inst2.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass2::dtor(Inst1)",
        /* strOperation    */ "ZSTrcServer/CMyClass2::dtor(Inst1)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass2::dtor(Inst2)",
        /* strOperation    */ "ZSTrcServer/CMyClass2::dtor(Inst2)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    // Test Step Group - One Trace Server - Method Tracing - Class 3
    //--------------------------------------------------------------

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::classMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::classMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::noisyClassMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::NoisyMethods::noisyClassMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::veryNoisyClassMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::VeryNoisyMethods::veryNoisyClassMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::ctor(Inst1, nullptr)",
        /* strOperation    */ "ZSTrcServer/CMyClass3::ctor(Inst1, nullptr)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::ctor(Inst2, nullptr)",
        /* strOperation    */ "ZSTrcServer/CMyClass3::ctor(Inst2, nullptr)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::Inst1.instMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::Inst1.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::Inst2.instMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::Inst2.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::Inst1.noisyInstMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::NoisyMethods::Inst1.noisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::Inst2.noisyInstMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::NoisyMethods::Inst2.noisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::Inst1.veryNoisyInstMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::VeryNoisyMethods::Inst1.veryNoisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::Inst2.veryNoisyInstMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::VeryNoisyMethods::Inst2.veryNoisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::dtor(Inst1)",
        /* strOperation    */ "ZSTrcServer/CMyClass3::dtor(Inst1)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::dtor(Inst2)",
        /* strOperation    */ "ZSTrcServer/CMyClass3::dtor(Inst2)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    // Test Step Group - One Trace Server - Shutdown
    //----------------------------------------------

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcClient::disconnect",
        /* strOperation    */ "ZSTrcClient::disconnect",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientDisconnect(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer::shutdown",
        /* strOperation    */ "ZSTrcServer::shutdown",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerShutdown(ZS::Test::CTestStep*)) );

    #endif

    // Test Step Group - Two Trace Servers
    //====================================

    pGrp = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " Two Trace Servers",
        /* pTSGrpParent    */ nullptr );

    // Test Step Group - Two Trace Servers - Startup
    //----------------------------------------------

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer::CreateInstance",
        /* strOperation    */ "TestTrcServer::CreateInstance",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerCreateInstance(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer:recallAdminObjs",
        /* strOperation    */ "TestTrcServer::recallAdminObjs",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerRecallAdminObjs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer::startup",
        /* strOperation    */ "ZSTrcServer::startup",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerStartup(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer::startup",
        /* strOperation    */ "TestTrcServer::startup",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerStartup(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcClient::create",
        /* strOperation    */ "TestTrcClient::create",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientCreate(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcClient::connect",
        /* strOperation    */ "ZSTrcClient::connect",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientConnect(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcClient::connect",
        /* strOperation    */ "TestTrcClient::connect",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientConnect(ZS::Test::CTestStep*)) );

    #if 0

    // Test Step Group - Two Trace Servers - Method Tracing - Class 1
    //---------------------------------------------------------------

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass1::classMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass1::classMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass1::classMethod",
        /* strOperation    */ "TestTrcServer/CMyClass1::classMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    // Test Step Group - Two Trace Servers - Method Tracing - ZSTrcServer - Class 2
    //-----------------------------------------------------------------------------

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass2::ctor(Inst1, nullptr)",
        /* strOperation    */ "ZSTrcServer/CMyClass2::ctor(Inst1, nullptr)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass2::ctor(Inst2, nullptr)",
        /* strOperation    */ "ZSTrcServer/CMyClass2::ctor(Inst2, nullptr)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass2::Inst1.instMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass2::Inst1.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass2::Inst2.instMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass2::Inst2.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass2::dtor(Inst1)",
        /* strOperation    */ "ZSTrcServer/CMyClass2::dtor(Inst1)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass2::dtor(Inst2)",
        /* strOperation    */ "ZSTrcServer/CMyClass2::dtor(Inst2)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    // Test Step Group - Two Trace Servers - Method Tracing - TestTrcServer - Class 2
    //-------------------------------------------------------------------------------

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass2::ctor(Inst1, nullptr)",
        /* strOperation    */ "TestTrcServer/CMyClass2::ctor(Inst1, nullptr)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass2::ctor(Inst2, nullptr)",
        /* strOperation    */ "TestTrcServer/CMyClass2::ctor(Inst2, nullptr)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass2::Inst1.instMethod",
        /* strOperation    */ "TestTrcServer/CMyClass2::Inst1.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass2::Inst2.instMethod",
        /* strOperation    */ "TestTrcServer/CMyClass2::Inst2.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass2::dtor(Inst1)",
        /* strOperation    */ "TestTrcServer/CMyClass2::dtor(Inst1)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass2::dtor(Inst2)",
        /* strOperation    */ "TestTrcServer/CMyClass2::dtor(Inst2)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    // Test Step Group - Two Trace Servers - Method Tracing - ZSTrcServer - Class 3
    //-----------------------------------------------------------------------------

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::classMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::classMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::noisyClassMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::NoisyMethods::noisyClassMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::veryNoisyClassMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::VeryNoisyMethods::veryNoisyClassMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::ctor(Inst1, nullptr)",
        /* strOperation    */ "ZSTrcServer/CMyClass3::ctor(Inst1, nullptr)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::ctor(Inst2, nullptr)",
        /* strOperation    */ "ZSTrcServer/CMyClass3::ctor(Inst2, nullptr)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::Inst1.instMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::Inst1.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::Inst2.instMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::Inst2.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::Inst1.noisyInstMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::NoisyMethods::Inst1.noisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::Inst2.noisyInstMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::NoisyMethods::Inst2.noisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::Inst1.veryNoisyInstMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::VeryNoisyMethods::Inst1.veryNoisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::Inst2.veryNoisyInstMethod",
        /* strOperation    */ "ZSTrcServer/CMyClass3::VeryNoisyMethods::Inst2.veryNoisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::dtor(Inst1)",
        /* strOperation    */ "ZSTrcServer/CMyClass3::dtor(Inst1)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass3::dtor(Inst2)",
        /* strOperation    */ "ZSTrcServer/CMyClass3::dtor(Inst2)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    // Test Step Group - Two Trace Servers - Method Tracing - TestTrcServer - Class 3
    //-------------------------------------------------------------------------------

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass3::classMethod",
        /* strOperation    */ "TestTrcServer/CMyClass3::classMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass3::noisyClassMethod",
        /* strOperation    */ "TestTrcServer/CMyClass3::NoisyMethods::noisyClassMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass3::veryNoisyClassMethod",
        /* strOperation    */ "TestTrcServer/CMyClass3::VeryNoisyMethods::veryNoisyClassMethod(Hello Class): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass3::ctor(Inst1, nullptr)",
        /* strOperation    */ "TestTrcServer/CMyClass3::ctor(Inst1, nullptr)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass3::ctor(Inst2, nullptr)",
        /* strOperation    */ "TestTrcServer/CMyClass3::ctor(Inst2, nullptr)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass3::Inst1.instMethod",
        /* strOperation    */ "TestTrcServer/CMyClass3::Inst1.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass3::Inst2.instMethod",
        /* strOperation    */ "TestTrcServer/CMyClass3::Inst2.instMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass3::Inst1.noisyInstMethod",
        /* strOperation    */ "TestTrcServer/CMyClass3::NoisyMethods::Inst1.noisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass3::Inst2.noisyInstMethod",
        /* strOperation    */ "TestTrcServer/CMyClass3::NoisyMethods::Inst2.noisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass3::Inst1.veryNoisyInstMethod",
        /* strOperation    */ "TestTrcServer/CMyClass3::VeryNoisyMethods::Inst1.veryNoisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass3::Inst2.veryNoisyInstMethod",
        /* strOperation    */ "TestTrcServer/CMyClass3::VeryNoisyMethods::Inst2.veryNoisyInstMethod(Hello Instance): Hello World",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass3::dtor(Inst1)",
        /* strOperation    */ "TestTrcServer/CMyClass3::dtor(Inst1)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer/CMyClass3::dtor(Inst2)",
        /* strOperation    */ "TestTrcServer/CMyClass3::dtor(Inst2)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    #endif

    // Test Step Group - Two Trace Servers - Method Tracing - ZSTrcServer - Class 1 - StartClass2Thread
    //-------------------------------------------------------------------------------------------------

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass1::ctor(Inst1)",
        /* strOperation    */ "ZSTrcServer/CMyClass1::ctor(Inst1)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass1::Inst1.startClass2Thread(Inst1)",
        /* strOperation    */ "ZSTrcServer/CMyClass1::Inst1.startClass2Thread(Inst1): Inst1",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass2::Inst1.recursiveTraceMethod()",
        /* strOperation    */ "ZSTrcServer/CMyClass2::Inst1.recursiveTraceMethod(): 0",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass1::Inst1.stopClass2Thread()",
        /* strOperation    */ "ZSTrcServer/CMyClass1::Inst1.stopClass2Thread()",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer/CMyClass1::dtor(Inst1)",
        /* strOperation    */ "ZSTrcServer/CMyClass1::dtor(Inst1)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceMethodCall(ZS::Test::CTestStep*)) );

    // Test Step Group - Two Trace Servers - Shutdown
    //-----------------------------------------------

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcClient::disconnect",
        /* strOperation    */ "TestTrcClient::disconnect",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientDisconnect(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcClient::disconnect",
        /* strOperation    */ "ZSTrcClient::disconnect",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientDisconnect(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcClient::destroy",
        /* strOperation    */ "TestTrcClient::destroy",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceClientDestroy(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer::shutdown",
        /* strOperation    */ "TestTrcServer::shutdown",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerShutdown(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZSTrcServer::shutdown",
        /* strOperation    */ "ZSTrcServer::shutdown",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerShutdown(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer::saveAdminObjs",
        /* strOperation    */ "TestTrcServer::saveAdminObjs",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerSaveAdminObjs(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " TestTrcServer::ReleaseInstance",
        /* strOperation    */ "TestTrcServer::ReleaseInstance",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepTraceServerReleaseInstance(ZS::Test::CTestStep*)) );

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

    QString strObjName;

    for( auto& pObj : m_hshpMyClass1InstancesByName )
    {
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
    m_hshpMyClass1InstancesByName.clear();
    m_hshpMyClass2InstancesByName.clear();
    m_hshpMyClass3InstancesByName.clear();

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
            SServerHostSettings hostSettings(24764);
            pTrcServer->setHostSettings(hostSettings);
            pTrcServer->changeSettings();
            emit trcServerCreated(pTrcServer);
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
        SClientHostSettings hostSettings("127.0.0.1", 24764);
        pTrcClient->setHostSettings(hostSettings);
        pTrcClient->changeSettings();
        emit trcClientCreated(pTrcClient);
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
    QString strClientName;
    QString strNameSpace;
    QString strClassName;
    QString strSubClassName;
    QString strObjName;
    QString strMth;
    QStringList strlstInArgs;
    QString strMthRet;

    splitMethodCallOperation(strOperation, strServerName, strClassName, strSubClassName, strObjName, strMth, strlstInArgs, strMthRet);

    CIpcTrcServer* pTrcServer = nullptr;

    if( strServerName == "ZSTrcServer" )
    {
        strClientName = "ZSTrcClient";
        pTrcServer = ZS::Trace::CIpcTrcServer::GetInstance("ZSTrcServer");
        CMyClass1::setTraceServerName("ZSTrcServer");
        CMyClass2::setTraceServerName("ZSTrcServer");
        CMyClass2Thread::setTraceServerName("ZSTrcServer");
        CMyClass3::setTraceServerName("ZSTrcServer");
        CMyClass3Thread::setTraceServerName("ZSTrcServer");
    }
    else if( strServerName == "TestTrcServer" )
    {
        strClientName = "TestTrcClient";
        pTrcServer = ZS::Trace::CIpcTrcServer::GetInstance("TestTrcServer");
        CMyClass1::setTraceServerName("TestTrcServer");
        CMyClass2::setTraceServerName("TestTrcServer");
        CMyClass2Thread::setTraceServerName("TestTrcServer");
        CMyClass3::setTraceServerName("TestTrcServer");
        CMyClass3Thread::setTraceServerName("TestTrcServer");
    }

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

    if( pTrcServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Invalid test operation");
    }
    else
    {
        pTrcServer->setNewTrcAdminObjsDefaultDetailLevel(ETraceDetailLevelMethodArgs);
        pTrcServer->setLocalTrcFileCloseFileAfterEachWrite(true);

        QString strTrcMethodEnter = "-> <" + strNameSpace + "::" + strClassName;
        QString strTrcMethodLeave = "<- <" + strNameSpace + "::" + strClassName;

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

        if( strClassName == CMyClass1::ClassName() && strMth == "startClass2Thread" && strlstInArgs.size() == 1 )
        {
            QString strClass2ObjName = strlstInArgs[0];
            strlstExpectedValues.append(strTrcMethodEnter);
            strlstExpectedValues.append("   -> <" + strNameSpace + "::" + CMyClass2Thread::ClassName() + "> " + strClass2ObjName + ".ctor(" + strClass2ObjName + ", " + strObjName + ")");
            strlstExpectedValues.append("   <- <" + strNameSpace + "::" + CMyClass2Thread::ClassName() + "> " + strClass2ObjName + ".ctor()");
            strlstExpectedValues.append("   -> <" + strNameSpace + "::" + CMyClass2Thread::ClassName() + "> " + strClass2ObjName + ".start(InheritPriority)");
            strlstExpectedValues.append("   <- <" + strNameSpace + "::" + CMyClass2Thread::ClassName() + "> " + strClass2ObjName + ".start()");
            strlstExpectedValues.append("-> <" + strNameSpace + "::" + CMyClass2Thread::ClassName() + "> " + strClass2ObjName + ".run()");
            strlstExpectedValues.append("   -> <" + strNameSpace + "::" + CMyClass2::ClassName() + "> " + strClass2ObjName + ".ctor(" + strClass2ObjName + ", " + strObjName + ")");
            strlstExpectedValues.append("   <- <" + strNameSpace + "::" + CMyClass2::ClassName() + "> " + strClass2ObjName + ".ctor()");
            strlstExpectedValues.append("   -> <" + strNameSpace + "::" + CMyClass2Thread::ClassName() + "> " + strClass2ObjName + ".exec()");
            strlstExpectedValues.append(strTrcMethodLeave);
        }
        else if( strClassName == CMyClass1::ClassName() && strMth == "stopClass2Thread" && strlstInArgs.size() == 0 )
        {
            QString strClass2ObjName = "Inst1";
            strlstExpectedValues.append(strTrcMethodEnter);
            strlstExpectedValues.append("   -> <" + strNameSpace + "::" + CMyClass2Thread::ClassName() + "> " + strClass2ObjName + ".quit()");
            strlstExpectedValues.append("   <- <" + strNameSpace + "::" + CMyClass2Thread::ClassName() + "> " + strClass2ObjName + ".quit()");
            strlstExpectedValues.append("   <- <" + strNameSpace + "::" + CMyClass2Thread::ClassName() + "> " + strClass2ObjName + ".exec(): 0");
            strlstExpectedValues.append("   -> <" + strNameSpace + "::" + CMyClass2Thread::ClassName() + "> " + strClass2ObjName + ".wait(Forever)");
            strlstExpectedValues.append("   -> <" + strNameSpace + "::" + CMyClass2::ClassName() + "> " + strClass2ObjName + ".dtor()");
            strlstExpectedValues.append("   <- <" + strNameSpace + "::" + CMyClass2::ClassName() + "> " + strClass2ObjName + ".dtor()");
            strlstExpectedValues.append("<- <" + strNameSpace + "::" + CMyClass2Thread::ClassName() + "> " + strClass2ObjName + ".run()");
            strlstExpectedValues.append("   <- <" + strNameSpace + "::" + CMyClass2Thread::ClassName() + "> " + strClass2ObjName + ".wait(): true");
            strlstExpectedValues.append("   -> <" + strNameSpace + "::" + CMyClass2Thread::ClassName() + "> " + strClass2ObjName + ".dtor()");
            strlstExpectedValues.append("   <- <" + strNameSpace + "::" + CMyClass2Thread::ClassName() + "> " + strClass2ObjName + ".dtor()");
            strlstExpectedValues.append(strTrcMethodLeave);
        }
        else if( strClassName == CMyClass2::ClassName() && strMth == "recursiveTraceMethod" && strlstInArgs.size() == 0 )
        {
            strlstExpectedValues.append(strTrcMethodEnter);
            strlstExpectedValues.append("   <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod: postEvent( {ZS::Apps::Test::IpcTrace::ReqTest, recursiveTraceMethod} )");
            strlstExpectedValues.append(strTrcMethodLeave);
            strlstExpectedValues.append("      -> <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.event({ZS::Apps::Test::IpcTrace::ReqTest, recursiveTraceMethod})");
            strlstExpectedValues.append("         -> <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod()");
            strlstExpectedValues.append("            <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod: RecursionCount=0");
            strlstExpectedValues.append("            -> <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod()");
            strlstExpectedValues.append("               <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod: RecursionCount=1");
            strlstExpectedValues.append("               -> <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod()");
            strlstExpectedValues.append("                  <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod: RecursionCount=2");
            strlstExpectedValues.append("                  -> <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod()");
            strlstExpectedValues.append("                     <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod: RecursionCount=3");
            strlstExpectedValues.append("                     -> <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod()");
            strlstExpectedValues.append("                        <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod: RecursionCount=4");
            strlstExpectedValues.append("                        -> <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod()");
            strlstExpectedValues.append("                           <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod: RecursionCount=5");
            strlstExpectedValues.append("                           -> <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod()");
            strlstExpectedValues.append("                              <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod: RecursionCount=6");
            strlstExpectedValues.append("                              -> <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod()");
            strlstExpectedValues.append("                                 <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod: RecursionCount=7");
            strlstExpectedValues.append("                                 -> <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod()");
            strlstExpectedValues.append("                                    <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod: RecursionCount=8");
            strlstExpectedValues.append("                                    -> <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod()");
            strlstExpectedValues.append("                                       <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod: RecursionCount=9");
            strlstExpectedValues.append("                                       -> <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod()");
            strlstExpectedValues.append("                                          <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod: RecursionCount=10");
            strlstExpectedValues.append("                                       <- <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod(): 10");
            strlstExpectedValues.append("                                    <- <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod(): 9");
            strlstExpectedValues.append("                                 <- <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod(): 8");
            strlstExpectedValues.append("                              <- <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod(): 7");
            strlstExpectedValues.append("                           <- <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod(): 6");
            strlstExpectedValues.append("                        <- <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod(): 5");
            strlstExpectedValues.append("                     <- <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod(): 4");
            strlstExpectedValues.append("                  <- <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod(): 3");
            strlstExpectedValues.append("               <- <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod(): 2");
            strlstExpectedValues.append("            <- <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod(): 1");
            strlstExpectedValues.append("         <- <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.recursiveTraceMethod(): 0");
            strlstExpectedValues.append("      <- <ZS::Apps::Test::IpcTrace::CMyClass2> Inst1.event()");
        }
        else
        {
            strlstExpectedValues.append(strTrcMethodEnter);
            strlstExpectedValues.append(strTrcMethodLeave);
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
            else if( m_hshpMyClass1InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass1::" + strObjName + " is already existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                m_hshpMyClass1InstancesByName[strObjName] = new CMyClass1(strlstInArgs[0]);
            }
        }
        else if( strMth == "dtor" )
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
                bValidTestStep = true;
                delete m_hshpMyClass1InstancesByName[strObjName];
                m_hshpMyClass1InstancesByName.remove(strObjName);
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
                    QString strClass2ObjName = myClass2->objectName();
                    m_hshpMyClass2InstancesByName.remove(strClass2ObjName);
                    m_hshpMyClass1InstancesByName[strObjName]->stopClass2Thread();
                }
            }
        }
    }
    else if( strClassName == CMyClass2::ClassName() )
    {
        if( strMth == "ctor" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( m_hshpMyClass2InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass2::" + strObjName + " is already existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                m_hshpMyClass2InstancesByName[strObjName] = new CMyClass2(strlstInArgs[0]);
            }
            else if( strlstInArgs.size() == 2 )
            {
                if( strlstInArgs[1] == "nullptr")
                {
                    bValidTestStep = true;
                    m_hshpMyClass2InstancesByName[strObjName] = new CMyClass2(strlstInArgs[0], nullptr);
                }
            }
        }
        else if( strMth == "dtor" )
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
                delete m_hshpMyClass2InstancesByName[strObjName];
                m_hshpMyClass2InstancesByName.remove(strObjName);
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
            else if( m_hshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is already existing";
            }
            else if( strlstInArgs.size() == 1 )
            {
                bValidTestStep = true;
                m_hshpMyClass3InstancesByName[strObjName] = new CMyClass3(strlstInArgs[0]);
            }
            else if( strlstInArgs.size() == 2 )
            {
                if( strlstInArgs[1] == "nullptr")
                {
                    bValidTestStep = true;
                    m_hshpMyClass3InstancesByName[strObjName] = new CMyClass3(strlstInArgs[0], nullptr);
                }
            }
        }
        else if( strMth == "dtor" )
        {
            if( strObjName.isEmpty() )
            {
                strResultValue = "Invalid test step: ObjName not defined";
            }
            else if( !m_hshpMyClass3InstancesByName.contains(strObjName) )
            {
                strResultValue = "CMyClass3::" + strObjName + " is not existing";
            }
            else if( strlstInArgs.size() == 0 )
            {
                bValidTestStep = true;
                delete m_hshpMyClass3InstancesByName[strObjName];
                m_hshpMyClass3InstancesByName.remove(strObjName);
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

    if( pTrcServer != nullptr )
    {
        pTrcServer->setNewTrcAdminObjsDefaultDetailLevel(ETraceDetailLevelNone);
        // No: The file must be closed so that the content can be evaluated in onZSTraceClientTrcMthListWdgtTextItemAdded.
        //pTrcServer->setLocalTrcFileCloseFileAfterEachWrite(false);
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
    if( !strResultValue.isEmpty() )
    {
        i_pTestStep->setResultValue(strResultValue);
    }
    else
    {
        CWidgetCentral* pWdgtCentral = CWidgetCentral::GetInstance();

        CWdgtTrcMthList* pWdgtTrcMthList = pWdgtCentral->getTrcMthListWdgt(strClientName);

        if( pWdgtTrcMthList != nullptr )
        {
            pWdgtTrcMthList->getTextEdit()->clear();

            if( !QObject::connect(
                /* pObjSender   */ pWdgtTrcMthList,
                /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onZSTraceClientTrcMthListWdgtTextItemAdded(const QString&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            m_pTmrTestStepTimeout->start(5000);
        }
        else
        {
            i_pTestStep->setResultValue("No trace method list widget for " + strServerName);
        }
    }

} // doTestStepTraceMethodCall

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
        QString strClientName;
        QString strNameSpace;
        QString strClassName;
        QString strSubClassName;
        QString strObjName;
        QString strMth;
        QStringList strlstInArgs;
        QString strMthRet;

        splitMethodCallOperation(strOperation, strServerName, strClassName, strSubClassName, strObjName, strMth, strlstInArgs, strMthRet);

        if( strServerName == "ZSTrcServer" )
        {
            strClientName = "ZSTrcClient";
        }
        else if( strServerName == "TestTrcServer" )
        {
            strClientName = "TestTrcClient";
        }

        CWidgetCentral* pWdgtCentral = CWidgetCentral::GetInstance();
        CWdgtTrcMthList* pWdgtTrcMthList = pWdgtCentral->getTrcMthListWdgt(strClientName);

        if( pWdgtTrcMthList != nullptr )
        {
            QString     strResultValue;
            QStringList strlstResultValues;
            QStringList strlstExpectedValues = pTestStep->getExpectedValues();

            QTextEdit* pEdtTrcMthList = pWdgtTrcMthList->getTextEdit();
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
                    int idxMthTrc = strLine.indexOf("): ");
                    if( idxMthTrc >= 0 )
                    {
                        idxMthTrc += 3;
                    }
                    if( idxMthTrc < 0 )
                    {
                        idxMthTrc = strLine.indexOf("->");
                    }
                    if( idxMthTrc < 0 )
                    {
                        idxMthTrc = strLine.indexOf("<-");
                    }
                    if( idxMthTrc >= 0 )
                    {
                        strLine = strLine.remove(0, idxMthTrc);
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
                QString strTrcMthFileBaseName = strServerName + "-TrcMth00";

                QString strTrcMthFileAbsFilePath = strAppLogDir + "/" + strTrcMthFileBaseName + "." + strTrcMthFileSuffix;

                QFile fileTrcMthFile(strTrcMthFileAbsFilePath);

                if( !fileTrcMthFile.open(QIODevice::ReadOnly|QIODevice::Text) )
                {
                    strlstResultValues.append("Could not open file " + strTrcMthFileAbsFilePath);
                }
                else
                {
                    QTextStream txtstrmLogFileAppDefault(&fileTrcMthFile);
                    QString strTrcMthFile = txtstrmLogFileAppDefault.readAll();
                    fileTrcMthFile.close();

                    for( const auto& strLine : strlstExpectedValues )
                    {
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
                        /* pObjSender   */ pWdgtTrcMthList,
                        /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onZSTraceClientTrcMthListWdgtTextItemAdded(const QString&)) );
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
        } // if( pWdgtTrcMthList != nullptr )
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

    - "ZSTrcServer/CMyClass1::classMethod(Arg1)"
    - "ZSTrcServer/CMyClass1::classMethod(Arg1, Arg2): Ret"
    - "ZSTrcServer/CMyClass1::ctor(Inst1)",
    - "ZSTrcServer/CMyClass1::Inst1.startClass2Thread(Inst1): Inst1",
    - "ZSTrcServer/CMyClass1::Inst1.stopClass2Thread()",
    - "ZSTrcServer/CMyClass1::dtor(Inst1)",

    - "ZSTrcServer/CMyClass2::ctor(Inst1)"
    - "ZSTrcServer/CMyClass2::Inst1.instMethod(Arg1, Arg2)"
    - "ZSTrcServer/CMyClass2::dtor(Inst1)"
    - "ZSTrcServer/CMyClass2::Inst1.instMethod(Arg1, Arg2): Ret"
    - "ZSTrcServer/CMyClass2::Inst1.recursiveTraceMethod(): %N%",

    - "ZSTrcServer/CMyClass3::classMethod(Arg1, Arg2)"
    - "ZSTrcServer/CMyClass3::NoisyMethods::noisyClassMethod(Arg1, Arg2)"
    - "ZSTrcServer/CMyClass3::VeryNoisyMethods::veryNoisyClassMethod(Arg1, Arg2)"
    - "ZSTrcServer/CMyClass3::ctor(Inst1)"
    - "ZSTrcServer/CMyClass3::Inst1.instMethod(Arg1, Arg2)"
    - "ZSTrcServer/CMyClass3::Inst1.instMethod(Arg1, Arg2): Ret"
    - "ZSTrcServer/CMyClass3::NoisyMethods::Inst1.noisyInstMethod(Arg1, Arg2)"
    - "ZSTrcServer/CMyClass3::VeryNoisyMethods::Inst1.veryNoisyInstMethod(Arg1, Arg2)"
    - "ZSTrcServer/CMyClass3::dtor(Inst1)"
*/
void CTest::splitMethodCallOperation(
    const QString& i_strOperation,
    QString& o_strServerName,
    QString& o_strClassName,
    QString& o_strSubClassName,
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
                        o_strObjName = o_strlstInArgs[0];
                    }
                    else if( o_strMth == "dtor" && o_strlstInArgs.size() == 1 && o_strObjName.isEmpty() )
                    {
                        o_strObjName = o_strlstInArgs[0];
                        o_strlstInArgs.clear();
                    }
                }
            }
        }
    }
} // splitMethodCallOperation
