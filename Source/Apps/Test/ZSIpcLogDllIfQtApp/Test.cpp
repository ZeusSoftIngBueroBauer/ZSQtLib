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
#include "MyThread.h"
#include "MainWindow.h"

#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStepIdxTree.h"
#include "ZSTestGUI/ZSTestStepDlg.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysLoggerIdxTree.h"
#include "ZSSys/ZSSysLogger.h"
#include "ZSSys/ZSSysLogServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Log;
using namespace ZS::Test::GUI;
using namespace ZS::Apps::Test::IpcLogDllIfQtApp;


/*******************************************************************************
class CTest
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest() :
//------------------------------------------------------------------------------
    ZS::Test::CTest("ZS::IpcLog::DllIfQtApp"),
    m_pDlgTestStep(nullptr),
    m_pTmrTestStepTimeout(nullptr),
    m_logSettings(),
    m_uLogServerPort(24762)
{
    m_pTmrTestStepTimeout = new QTimer(this);
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

    int idxGroup = 0;
    int idxStep = 0;

    // Test Step Group - Startup
    //--------------------------

    ZS::Test::CTestStepGroup* pTestGroupStartup = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " Startup",
        /* pTSGrpParent    */ nullptr );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Load ZSIpcLog Dll",
        /* strOperation    */ "loadDll",
        /* pTSGrpParent    */ pTestGroupStartup,
        /* szDoTestStepFct */ SLOT(doTestStepLoadDll(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Log Server Create Instance",
        /* strOperation    */ "CIpcLogServer::CreateInstance",
        /* pTSGrpParent    */ pTestGroupStartup,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerCreateInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Log Server Startup",
        /* strOperation    */ "CIpcLogServer::startup",
        /* pTSGrpParent    */ pTestGroupStartup,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerStartup(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Log Client Connect",
        /* strOperation    */ "CIpcLogClient::connect",
        /* pTSGrpParent    */ pTestGroupStartup,
        /* szDoTestStepFct */ SLOT(doTestStepLogClientConnect(ZS::Test::CTestStep*)) );

    // Test Step Group - GetLogger
    //----------------------------

    QString strLoggerDatabaseAccesses = "DatabaseAccesses";
    QString strLoggerUserEventsLogins = "UserEvents-Logins";
    QString strLoggerZSIpcCServer = "ZS-Ipc-CServer";
    QString strLoggerZSIpcCServerInst1 = "ZS-Ipc-CServer-Inst1";;

    ZS::Test::CTestStepGroup* pTestGroupGetLogger = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " GetLogger",
        /* pTSGrpParent    */ nullptr );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::GetLogger()",
        /* strOperation    */ "LogServer::GetLogger()",
        /* pTSGrpParent    */ pTestGroupGetLogger,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerGetLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::GetLogger(" + strLoggerDatabaseAccesses +")",
        /* strOperation    */ "LogServer::GetLogger(" + strLoggerDatabaseAccesses + ")",
        /* pTSGrpParent    */ pTestGroupGetLogger,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerGetLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::GetLogger(" + strLoggerUserEventsLogins + ")",
        /* strOperation    */ "LogServer::GetLogger(" + strLoggerUserEventsLogins + ")",
        /* pTSGrpParent    */ pTestGroupGetLogger,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerGetLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::GetLogger(" + strLoggerZSIpcCServer + ")",
        /* strOperation    */ "LogServer::GetLogger(" + strLoggerZSIpcCServer + ")",
        /* pTSGrpParent    */ pTestGroupGetLogger,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerGetLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::GetLogger(" + strLoggerZSIpcCServerInst1 + ")",
        /* strOperation    */ "LogServer::GetLogger(" + strLoggerZSIpcCServerInst1 + ")",
        /* pTSGrpParent    */ pTestGroupGetLogger,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerGetLogger(ZS::Test::CTestStep*)) );

    // Test Step Group - ModifyLoggers
    //--------------------------------

    ZS::Test::CTestStepGroup* pTestGroupModifyLoggers = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " ModifyLoggers",
        /* pTSGrpParent    */ nullptr );

    // Test Step Group - ModifyLoggers - SetLogLevel
    //----------------------------------------------

    ZS::Test::CTestStepGroup* pTestGroupModifyLoggersSetLogLevel = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " SetLogLevel",
        /* pTSGrpParent    */ pTestGroupModifyLoggers );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Info, Info Message)",
        /* strOperation    */ "LogServer::theInst.log(Info, Info Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetLogLevel,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-NoLogEntry");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setLogLevel(Info)",
        /* strOperation    */ "LogServer::theInst.setLogLevel(Info)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetLogLevel,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Info, Info Message)",
        /* strOperation    */ "LogServer::theInst.log(Info, Info Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetLogLevel,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-InfoMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setLogLevel(DebugDetailed)",
        /* strOperation    */ "LogServer::theInst.setLogLevel(DebugDetailed)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetLogLevel,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Debug, Debug Message)",
        /* strOperation    */ "LogServer::theInst.log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetLogLevel,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(DebugDetailed, Detailed Debug Message)",
        /* strOperation    */ "LogServer::theInst.log(DebugDetailed, Detailed Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetLogLevel,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-DetailedDebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(DebugVerbose, Verbose Debug Message)",
        /* strOperation    */ "LogServer::theInst.log(DebugVerbose, Verbose Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetLogLevel,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-NoLogEntry");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setLogLevel(None)",
        /* strOperation    */ "LogServer::theInst.setLogLevel(None)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetLogLevel,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Info, Info Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Info, Info Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetLogLevel,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-NoLogEntry");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(Info)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(Info)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetLogLevel,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Info, Info Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Info, Info Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetLogLevel,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-InfoMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(DebugDetailed)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(DebugDetailed)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetLogLevel,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetLogLevel,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(DebugDetailed, Detailed Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(DebugDetailed, Detailed Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetLogLevel,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-DetailedDebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(DebugVerbose, Verbose Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(DebugVerbose, Verbose Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetLogLevel,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-NoLogEntry");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetLogLevel,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    // Test Step Group - ModifyLoggers - SetEnabled
    //---------------------------------------------

    ZS::Test::CTestStepGroup* pTestGroupModifyLoggersSetEnabled = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " SetEnabled",
        /* pTSGrpParent    */ pTestGroupModifyLoggers );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setLogLevel(Debug)",
        /* strOperation    */ "LogServer::theInst.setLogLevel(Debug)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Info, Info Message)",
        /* strOperation    */ "LogServer::theInst.log(Info, Info Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-InfoMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setEnabled(No)",
        /* strOperation    */ "LogServer::theInst.setEnabled(No)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Info, Info Message)",
        /* strOperation    */ "LogServer::theInst.log(Info, Info Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-NoLogEntry");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setEnabled(Yes)",
        /* strOperation    */ "LogServer::theInst.setEnabled(Yes)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Debug, Debug Message)",
        /* strOperation    */ "LogServer::theInst.log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setLogLevel(None)",
        /* strOperation    */ "LogServer::theInst.setLogLevel(None)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(Debug)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(Debug)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Info, Info Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Info, Info Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-InfoMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setEnabled(No)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setEnabled(No)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Info, Info Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Info, Info Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-NoLogEntry");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setEnabled(Yes)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setEnabled(Yes)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    // Test Step Group - ModifyLoggers - SetDataFilter
    //------------------------------------------------

    ZS::Test::CTestStepGroup* pTestGroupModifyLoggersSetDataFilter = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " SetDataFilter",
        /* pTSGrpParent    */ pTestGroupModifyLoggers );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setLogLevel(Debug)",
        /* strOperation    */ "LogServer::theInst.setLogLevel(Debug)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Info, Info Message)",
        /* strOperation    */ "LogServer::theInst.log(Info, Info Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-InfoMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setDataFilter($I{Hello}I$)",
        /* strOperation    */ "LogServer::theInst.setDataFilter($I{Hello}I$)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Info, Hello World)",
        /* strOperation    */ "LogServer::theInst.log(Info, Hello World)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-HelloWorld");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Info, Hallo Welt)",
        /* strOperation    */ "LogServer::theInst.log(Info, Hallo Welt)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-NoLogEntry");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setDataFilter($I{Hello}I$$I{World}I$)",
        /* strOperation    */ "LogServer::theInst.setDataFilter($I{Hello}I$$I{World}I$)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Info, Hello World)",
        /* strOperation    */ "LogServer::theInst.log(Info, Hello World)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-HelloWorld");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Info, Hallo Welt)",
        /* strOperation    */ "LogServer::theInst.log(Info, Hallo Welt)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-NoLogEntry");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setDataFilter($!I{Hello}I!$)",
        /* strOperation    */ "LogServer::theInst.setDataFilter($!I{Hello}I!$)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Info, Hello World)",
        /* strOperation    */ "LogServer::theInst.log(Info, Hello World)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-NoLogEntry");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Info, Hallo Welt)",
        /* strOperation    */ "LogServer::theInst.log(Info, Hallo Welt)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-HalloWelt");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setDataFilter($!I{Hello}I!$$!I{World}I!$)",
        /* strOperation    */ "LogServer::theInst.setDataFilter($!I{Hello}I!$$!I{World}I!$)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Info, Hello World)",
        /* strOperation    */ "LogServer::theInst.log(Info, Hello World)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-NoLogEntry");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Info, Hallo Welt)",
        /* strOperation    */ "LogServer::theInst.log(Info, Hallo Welt)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-HalloWelt");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setDataFilter($I{Hello}I$$!I{World}I!$)",
        /* strOperation    */ "LogServer::theInst.setDataFilter($I{Hello}I$$!I{World}I!$)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Info, Hello World)",
        /* strOperation    */ "LogServer::theInst.log(Info, Hello World)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-NoLogEntry");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Info, Hello Welt)",
        /* strOperation    */ "LogServer::theInst.log(Info, Hello Welt)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-HelloWelt");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Info, Hallo World)",
        /* strOperation    */ "LogServer::theInst.log(Info, Hallo World)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-NoLogEntry");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.log(Info, Hello Welt)",
        /* strOperation    */ "LogServer::theInst.log(Info, Hello Welt)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-HelloWelt");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setDataFilter()",
        /* strOperation    */ "LogServer::theInst.setDataFilter()",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setLogLevel(None)",
        /* strOperation    */ "LogServer::theInst.setLogLevel(None)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetDataFilter,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    // Test Step Group - ModifyLoggers - AddThreadName
    //------------------------------------------------

    ZS::Test::CTestStepGroup* pTestGroupModifyLoggersAddThreadName = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " AddThreadName",
        /* pTSGrpParent    */ pTestGroupModifyLoggers );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(Debug)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(Debug)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddThreadName,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setAddThreadName(true)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setAddThreadName(true)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddThreadName,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddThreadName,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-GuiMainThread-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setAddThreadName(false)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setAddThreadName(false)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddThreadName,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddThreadName,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddThreadName,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    // Test Step Group - ModifyLoggers - AddDateTime
    //----------------------------------------------

    ZS::Test::CTestStepGroup* pTestGroupModifyLoggersAddDateTime = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " AddDateTime",
        /* pTSGrpParent    */ pTestGroupModifyLoggers );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(Debug)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(Debug)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddDateTime,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setAddDateTime(true)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setAddDateTime(true)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddDateTime,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddDateTime,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-DateTime-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setAddDateTime(false)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setAddDateTime(false)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddDateTime,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddDateTime,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddDateTime,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    // Test Step Group - ModifyLoggers - AddSystemTime
    //------------------------------------------------

    ZS::Test::CTestStepGroup* pTestGroupModifyLoggersAddSystemTime = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " AddSystemTime",
        /* pTSGrpParent    */ pTestGroupModifyLoggers );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(Debug)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(Debug)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddSystemTime,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setAddSystemTime(true)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setAddSystemTime(true)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddSystemTime,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddSystemTime,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-SystemTime-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setAddSystemTime(false)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setAddSystemTime(false)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddSystemTime,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddSystemTime,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersAddSystemTime,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    // Test Step Group - ModifyLoggers - SetNameSpace
    //-----------------------------------------------

    ZS::Test::CTestStepGroup* pTestGroupModifyLoggersSetNameSpace = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " SetNameSpace",
        /* pTSGrpParent    */ pTestGroupModifyLoggers );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(Debug)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(Debug)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetNameSpace,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setNameSpace(ZS-Db)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setNameSpace(ZS-Db)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetNameSpace,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetNameSpace,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-NameSpace-ZSDb-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setNameSpace()",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setNameSpace()",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetNameSpace,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetNameSpace,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetNameSpace,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    // Test Step Group - ModifyLoggers - SetClassName
    //-----------------------------------------------

    ZS::Test::CTestStepGroup* pTestGroupModifyLoggersSetClassName = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " SetClassName",
        /* pTSGrpParent    */ pTestGroupModifyLoggers );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(Debug)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(Debug)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetClassName,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setClassName(CDataTable)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setClassName(CDataTable)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetClassName,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetClassName,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-ClassName-DataTable-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setClassName()",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setClassName()",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetClassName,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetClassName,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetClassName,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    // Test Step Group - ModifyLoggers - SetObjectName
    //------------------------------------------------

    ZS::Test::CTestStepGroup* pTestGroupModifyLoggersSetObjectName = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " SetObjectName",
        /* pTSGrpParent    */ pTestGroupModifyLoggers );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(Debug)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(Debug)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetObjectName,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setObjectName(Customers)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setObjectName(Customers)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetObjectName,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetObjectName,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-ObjectName-Customers-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setObjectName()",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setObjectName()",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetObjectName,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetObjectName,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetObjectName,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    // Test Step Group - ModifyLoggers - SetEverything
    //------------------------------------------------

    ZS::Test::CTestStepGroup* pTestGroupModifyLoggersSetEverything = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " SetEverything",
        /* pTSGrpParent    */ pTestGroupModifyLoggers );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(Debug)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(Debug)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setAddThreadName(true)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setAddThreadName(true)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setAddDateTime(true)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setAddDateTime(true)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setAddSystemTime(true)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setAddSystemTime(true)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setNameSpace(ZS-Db)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setNameSpace(ZS-Db)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setClassName(CDataTable)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setClassName(CDataTable)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setObjectName(Customers)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setObjectName(Customers)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-Everything-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MyThread " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntryMyThread(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-MyThread-Everything-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setAddThreadName(false)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setAddThreadName(false)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setAddDateTime(false)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setAddDateTime(false)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setAddSystemTime(false)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setAddSystemTime(false)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setNameSpace()",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setNameSpace()",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setClassName()",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setClassName()",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setObjectName()",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setObjectName()",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " MyThread " + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepLoggerAddLogEntryMyThread(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " " + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* strOperation    */ "Logger::" + strLoggerDatabaseAccesses + ".setLogLevel(None)",
        /* pTSGrpParent    */ pTestGroupModifyLoggersSetEverything,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    // Test Step Group - ModifyLogServer
    //----------------------------------

    ZS::Test::CTestStepGroup* pTestGroupModifyLogServer = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " ModifyLogServer",
        /* pTSGrpParent    */ nullptr );

    // Test Step Group - LogServer - SetEnabled
    //-----------------------------------------

    ZS::Test::CTestStepGroup* pTestGroupModifyLogServerSetEnabled = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " SetEnabled",
        /* pTSGrpParent    */ pTestGroupModifyLogServer );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " logServer.setLogLevel(Debug)",
        /* strOperation    */ "LogServer::theInst.setLogLevel(Debug)",
        /* pTSGrpParent    */ pTestGroupModifyLogServerSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " logServer.log(Debug, Debug Message)",
        /* strOperation    */ "LogServer::theInst.log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLogServerSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " logServer.setEnabled(false)",
        /* strOperation    */ "LogServer::theInst.setEnabled(false)",
        /* pTSGrpParent    */ pTestGroupModifyLogServerSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogServer(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " logServer.log(Debug, Debug Message)",
        /* strOperation    */ "LogServer::theInst.log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLogServerSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-NoLogEntry");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " logServer.setEnabled(true)",
        /* strOperation    */ "LogServer::theInst.setEnabled(true)",
        /* pTSGrpParent    */ pTestGroupModifyLogServerSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogServer(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " logServer.log(Debug, Debug Message)",
        /* strOperation    */ "LogServer::theInst.log(Debug, Debug Message)",
        /* pTSGrpParent    */ pTestGroupModifyLogServerSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerAddLogEntry(ZS::Test::CTestStep*)) );
    pTestStep->setConfigValue("ExpectedResultsFileName", "ZSLogServer-DebugMessage");

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setLogLevel(None)",
        /* strOperation    */ "LogServer::theInst.setLogLevel(None)",
        /* pTSGrpParent    */ pTestGroupModifyLogServerSetEnabled,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogger(ZS::Test::CTestStep*)) );

    // Test Step Group - LogServer - NewLoggersDefaultSettings
    //--------------------------------------------------------

    ZS::Test::CTestStepGroup* pTestGroupModifyLogServerNewLoggersDefaultSettings = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " NewLoggersDefaultSettings",
        /* pTSGrpParent    */ pTestGroupModifyLogServer );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setNewLoggersEnabledAsDefault(false)",
        /* strOperation    */ "LogServer::theInst.setNewLoggersEnabledAsDefault(false)",
        /* pTSGrpParent    */ pTestGroupModifyLogServerNewLoggersDefaultSettings,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogServer(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::GetLogger(NewLoggerDisabledAsDefault)",
        /* strOperation    */ "LogServer::GetLogger(NewLoggerDisabledAsDefault)",
        /* pTSGrpParent    */ pTestGroupModifyLogServerNewLoggersDefaultSettings,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerGetLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setNewLoggersEnabledAsDefault(true)",
        /* strOperation    */ "LogServer::theInst.setNewLoggersEnabledAsDefault(true)",
        /* pTSGrpParent    */ pTestGroupModifyLogServerNewLoggersDefaultSettings,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogServer(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::GetLogger(NewLoggerEnabledAsDefault)",
        /* strOperation    */ "LogServer::GetLogger(NewLoggerEnabledAsDefault)",
        /* pTSGrpParent    */ pTestGroupModifyLogServerNewLoggersDefaultSettings,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerGetLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setNewLoggersDefaultDetailLevel(Debug)",
        /* strOperation    */ "LogServer::theInst.setNewLoggersDefaultDetailLevel(Debug)",
        /* pTSGrpParent    */ pTestGroupModifyLogServerNewLoggersDefaultSettings,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogServer(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::GetLogger(NewLoggerDefaultDetailLevelDebug)",
        /* strOperation    */ "LogServer::GetLogger(NewLoggerDefaultDetailLevelDebug)",
        /* pTSGrpParent    */ pTestGroupModifyLogServerNewLoggersDefaultSettings,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerGetLogger(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.setNewLoggersDefaultDetailLevel(None)",
        /* strOperation    */ "LogServer::theInst.setNewLoggersDefaultDetailLevel(None)",
        /* pTSGrpParent    */ pTestGroupModifyLogServerNewLoggersDefaultSettings,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogServer(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::GetLogger(NewLoggerDefaultDetailLevelNone)",
        /* strOperation    */ "LogServer::GetLogger(NewLoggerDefaultDetailLevelNone)",
        /* pTSGrpParent    */ pTestGroupModifyLogServerNewLoggersDefaultSettings,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerGetLogger(ZS::Test::CTestStep*)) );

    // Test Step Group - LogServer - LoggerFile
    //-----------------------------------------

    ZS::Test::CTestStepGroup* pTestGroupModifyLogServerLoggerFile = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " LoggerFile",
        /* pTSGrpParent    */ pTestGroupModifyLogServer );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.LoggerFileMethods",
        /* strOperation    */ "LogServer::theInst.LoggerFileMethods",
        /* pTSGrpParent    */ pTestGroupModifyLogServerLoggerFile,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogServerLoggerFile(ZS::Test::CTestStep*)) );

    // Test Step Group - LogServer - LocalLogFile
    //-------------------------------------------

    ZS::Test::CTestStepGroup* pTestGroupModifyLogServerLocalLogFile = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " LocalLogFile",
        /* pTSGrpParent    */ pTestGroupModifyLogServer );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " LogServer::theInst.LocalLogFileMethods",
        /* strOperation    */ "LogServer::theInst.LocalLogFileMethods",
        /* pTSGrpParent    */ pTestGroupModifyLogServerLocalLogFile,
        /* szDoTestStepFct */ SLOT(doTestStepModifyLogServerLocalLogFile(ZS::Test::CTestStep*)) );

    // Test Step Group - Shutdown
    //---------------------------

    ZS::Test::CTestStepGroup* pTestGroupShutdown = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxGroup) + " Shutdown",
        /* pTSGrpParent    */ nullptr );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Log Client Disconnect",
        /* strOperation    */ "CIpcLogClient::disconnect",
        /* pTSGrpParent    */ pTestGroupShutdown,
        /* szDoTestStepFct */ SLOT(doTestStepLogClientDisconnect(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Log Server Shutdown",
        /* strOperation    */ "CIpcLogServer::shutdown",
        /* pTSGrpParent    */ pTestGroupShutdown,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerShutdown(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Log Server Release Instance",
        /* strOperation    */ "CIpcLogServer::ReleaseInstance",
        /* pTSGrpParent    */ pTestGroupShutdown,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerReleaseInstance(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Release ZSIpcLog Dll",
        /* strOperation    */ "releaseDll",
        /* pTSGrpParent    */ pTestGroupShutdown,
        /* szDoTestStepFct */ SLOT(doTestStepReleaseDll(ZS::Test::CTestStep*)) );

    // Recall test step settings
    //--------------------------

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

    try
    {
        delete m_pDlgTestStep;
    }
    catch(...)
    {
    }

    DllIf::CIpcLogServer* pLogServer = DllIf::CIpcLogServer::GetInstance();

    if( pLogServer != nullptr )
    {
        DllIf::CIpcLogServer::ReleaseInstance(pLogServer);
    }

    m_pDlgTestStep = nullptr;
    m_pTmrTestStepTimeout = nullptr;
    SLogServerSettings_release(m_logSettings);
    m_uLogServerPort = 0;

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
    DllIf::EBuildConfiguration buildConfiguration = DllIf::EBuildConfigurationDebug;
    #else
    DllIf::EBuildConfiguration buildConfiguration = DllIf::EBuildConfigurationRelease;
    #endif

    bool bOk = DllIf::loadDll(buildConfiguration, QT_VERSION_MAJOR);

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

    bool bOk = DllIf::releaseDll();

    // Result Values
    //---------------

    strResultValue = "ReleaseDll: " + QString(bOk ? "Ok" : "Failed");

    strlstResultValues.append(strResultValue);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepReleaseDll

//------------------------------------------------------------------------------
void CTest::doTestStepLogServerCreateInstance( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strExpectedValue = "LogServer != nullptr";

    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    DllIf::CIpcLogServer* pLogServer = DllIf::CIpcLogServer::CreateInstance();

    // Result Values
    //---------------

    if( pLogServer == nullptr ) strResultValue = "LogServer == nullptr";
    else strResultValue = "LogServer != nullptr";

    strlstResultValues.append(strResultValue);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepLogServerCreateInstance

//------------------------------------------------------------------------------
void CTest::doTestStepLogServerReleaseInstance( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strExpectedValue = "LogServer == nullptr";

    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    DllIf::CIpcLogServer* pLogServer = DllIf::CIpcLogServer::GetInstance();
    DllIf::CIpcLogServer::ReleaseInstance(pLogServer);
    pLogServer = DllIf::CIpcLogServer::GetInstance();

    // Result Values
    //---------------

    if( pLogServer == nullptr ) strResultValue = "LogServer == nullptr";
    else strResultValue = "LogServer != nullptr";

    strlstResultValues.append(strResultValue);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepLogServerReleaseInstance

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

    strExpectedValue = "Listening";

    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    bool bOk = false;

    DllIf::CIpcLogServer* pLogServer = DllIf::CIpcLogServer::GetInstance();

    if( pLogServer != nullptr )
    {
        DllIf::SLogServerSettings_release(m_logSettings);
        m_logSettings = pLogServer->getLogSettings();
        m_logSettings.m_bLocalLogFileCloseFileAfterEachWrite = true;

        bOk = pLogServer->setLogSettings(m_logSettings);
        if( bOk ) bOk = pLogServer->setPort(m_uLogServerPort);

        if( bOk ) bOk = pLogServer->startup();
    }

    // Result Values
    //---------------

    strResultValue = "Not listening";

    if( pLogServer != nullptr )
    {
        if( pLogServer->isListening() ) strResultValue = "Listening";
    }

    strlstResultValues.append(strResultValue);

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

    strExpectedValue = "Idle";

    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    DllIf::CIpcLogServer* pLogServer = DllIf::CIpcLogServer::GetInstance();

    if( pLogServer != nullptr )
    {
        pLogServer->shutdown();
    }

    // Result Values
    //---------------

    strExpectedValue = "Not idle";

    if( pLogServer != nullptr )
    {
        if( !pLogServer->isListening() ) strResultValue = "Idle";
    }

    strlstResultValues.append(strResultValue);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepLogServerShutdown

//------------------------------------------------------------------------------
void CTest::doTestStepLogClientConnect( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setDescription("Check whether client can connect.");
    i_pTestStep->setInstruction(
        "Start and connect the log method client.<br/>"
        "Open Loggers Dialog and Log Client Settings Dialog.<br/>"
        "Compare the settings on tab \"Log Settings\" with the expected values.");

    DllIf::CIpcLogServer* pLogServer = DllIf::CIpcLogServer::GetInstance();

    char* szFilePath = DllIf::CIpcLogServer::GetLoggerFileAbsoluteFilePath();
    QString strLoggerFilePath(szFilePath);
    delete szFilePath; szFilePath = nullptr;

    szFilePath = DllIf::CIpcLogServer::GetLocalLogFileAbsoluteFilePath();
    QString strLogFilePath(szFilePath);
    delete szFilePath; szFilePath = nullptr;

    QStringList strlstExpectedValues;

    strlstExpectedValues.append("Log Client Connected");
    strlstExpectedValues.append("ApplicationName:                  " + QCoreApplication::applicationName());
    strlstExpectedValues.append("ServerName:                       theInst");
    strlstExpectedValues.append("LoggingEnabled:                   true");
    strlstExpectedValues.append("LoggersFile:                      " + strLoggerFilePath);
    strlstExpectedValues.append("New Loggers Enabled as Default:   true");
    strlstExpectedValues.append("New Loggers Default Detail Level: None");
    strlstExpectedValues.append("RemoteTracing:                    enabled");
    strlstExpectedValues.append("CacheData:                        disabled");
    strlstExpectedValues.append("MaxArrLen:                        1000");
    strlstExpectedValues.append("Use Local Log File:               true");
    strlstExpectedValues.append("Local Log File:                   " + strLogFilePath);
    strlstExpectedValues.append("Auto Save Interval:               1000");
    strlstExpectedValues.append("Max File Count:                   5");
    strlstExpectedValues.append("Max Line Count:                   2000");
    strlstExpectedValues.append("Close File After Each Write:      true");

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    m_pDlgTestStep = new CDlgTestStep(i_pTestStep);
    m_pDlgTestStep->exec();
    delete m_pDlgTestStep;
    m_pDlgTestStep = nullptr;

    // Check if really connected and correct test result if necessary.
    if( i_pTestStep->getTestResult() == ZS::Test::ETestResult::TestPassed )
    {
        if( pLogServer == nullptr )
        {
            i_pTestStep->setResultValue("No Log Server Created");
        }
        else if( !pLogServer->isConnected() )
        {
            i_pTestStep->setResultValue("Log Client NOT Connected");
        }
    }
} // doTestStepLogClientConnect

//------------------------------------------------------------------------------
void CTest::doTestStepLogClientDisconnect( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setDescription("Check whether client can disconnect.");
    i_pTestStep->setInstruction("Disconnect the log client.");
    i_pTestStep->setExpectedValue("Log Client Disconnected");

    m_pDlgTestStep = new CDlgTestStep(i_pTestStep);
    m_pDlgTestStep->exec();
    delete m_pDlgTestStep;
    m_pDlgTestStep = nullptr;

    DllIf::CIpcLogServer* pLogServer = DllIf::CIpcLogServer::GetInstance();

    // Check if really disconnected and correct test result if necessary.
    if( i_pTestStep->getTestResult() == ZS::Test::ETestResult::TestPassed )
    {
        if( pLogServer == nullptr )
        {
            i_pTestStep->setResultValue("No Log Server Created");
        }
        else if( pLogServer->isConnected() )
        {
            i_pTestStep->setResultValue("Log Client NOT Disconnected");
        }
    }
} // doTestStepLogClientDisconnect

//------------------------------------------------------------------------------
void CTest::doTestStepLogServerGetLogger( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTestGroup = i_pTestStep->getParentGroup();

    QString strTestGroupPath = pTestGroup == nullptr ? "" : pTestGroup->path();

    DllIf::CIpcLogServer* pLogServer = DllIf::CIpcLogServer::GetInstance();

    if( pLogServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Log server not existing");
    }

    // Test Step
    //----------

    QString strOperation = i_pTestStep->getOperation();

    QString strNameSpace;
    QString strClassName;
    QString strSubClassName;
    QString strObjName;
    QString strMth;
    QStringList strlstInArgs;
    QString strMthRet;

    splitMethodCallOperation(strOperation, strClassName, strSubClassName, strObjName, strMth, strlstInArgs, strMthRet);

    DllIf::CLogger* pLogger = nullptr;
    QString  strLoggerName;

    if( strMth.compare("getLogger", Qt::CaseInsensitive) == 0 )
    {
        if( strlstInArgs.size() == 0 )
        {
            pLogger = pLogServer->getLogger();
        }
        else if( strlstInArgs.size() == 1 )
        {
            strLoggerName = strlstInArgs[0].replace("-", "::");
            pLogger = DllIf::CLogServer::GetLogger(strLoggerName.toStdString().c_str());
        }
    }

    // Expected Values
    //----------------

    if( pLogger != nullptr && i_pTestStep->getExpectedValues().isEmpty() )
    {
        QStringList strlstExpectedValues = getExpectedValues(pLogger);
        i_pTestStep->setExpectedValues(strlstExpectedValues);
    }

    // Result Values
    //--------------

    i_pTestStep->setInstruction("Check whether logger is added to client with expected settings.");

    m_pDlgTestStep = new CDlgTestStep(i_pTestStep);
    m_pDlgTestStep->exec();
    delete m_pDlgTestStep;
    m_pDlgTestStep = nullptr;

} // doTestStepLogServerGetLogger

//------------------------------------------------------------------------------
void CTest::doTestStepModifyLogger( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTestGroup = i_pTestStep->getParentGroup();

    QString strTestGroupPath = pTestGroup == nullptr ? "" : pTestGroup->path();

    DllIf::CIpcLogServer* pLogServer = DllIf::CIpcLogServer::GetInstance();

    if( pLogServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Log server or log client not existing");
    }

    // Test Step
    //----------

    QString strOperation = i_pTestStep->getOperation();

    QString strNameSpace;
    QString strClassName;
    QString strSubClassName;
    QString strObjName;
    QString strMth;
    QStringList strlstInArgs;
    QString strMthRet;

    splitMethodCallOperation(strOperation, strClassName, strSubClassName, strObjName, strMth, strlstInArgs, strMthRet);

    DllIf::CLogger* pLogger = DllIf::CLogServer::GetLogger(strObjName.toStdString().c_str());

    if( strMth == "setLogLevel" ) {
        if( strlstInArgs.size() != 1 ) {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
        else {
            try {
                CEnumLogDetailLevel eDetailLevel = strlstInArgs[0];
                pLogger->setLogLevel(static_cast<DllIf::ELogDetailLevel>(eDetailLevel.enumerator()));
            }
            catch(CException&) {
                i_pTestStep->setExpectedValue("Invalid test step operation");
            }
        }
    }
    else if( strMth == "setEnabled" ) {
        if( strlstInArgs.size() != 1 ) {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
        else {
            try {
                CEnumEnabled eEnabled = strlstInArgs[0];
                pLogger->setEnabled(eEnabled == EEnabled::Yes ? true : false);
            }
            catch(CException&) {
                i_pTestStep->setExpectedValue("Invalid test step operation");
            }
        }
    }
    else if( strMth == "setDataFilter" ) {
        if( strlstInArgs.size() == 0 ) {
            pLogger->setDataFilter();
        }
        else if( strlstInArgs.size() == 1 ) {
            pLogger->setDataFilter(strlstInArgs[0].toStdString().c_str());
        }
        else {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
    }
    else if( strMth == "setAddThreadName" ) {
        if( strlstInArgs.size() == 1 ) {
            bool bAdd = str2Bool(strlstInArgs[0]);
            pLogger->setAddThreadName(bAdd);
        }
        else {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
    }
    else if( strMth == "setAddDateTime" ) {
        if( strlstInArgs.size() == 1 ) {
            bool bAdd = str2Bool(strlstInArgs[0]);
            pLogger->setAddDateTime(bAdd);
        }
        else {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
    }
    else if( strMth == "setAddSystemTime" ) {
        if( strlstInArgs.size() == 1 ) {
            bool bAdd = str2Bool(strlstInArgs[0]);
            pLogger->setAddSystemTime(bAdd);
        }
        else {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
    }
    else if( strMth == "setNameSpace" ) {
        if( strlstInArgs.size() == 0 ) {
            pLogger->setNameSpace();
        }
        else if( strlstInArgs.size() == 1 ) {
            pLogger->setNameSpace(strlstInArgs[0].toStdString().c_str());
        }
        else {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
    }
    else if( strMth == "setClassName" ) {
        if( strlstInArgs.size() == 0 ) {
            pLogger->setClassName();
        }
        else if( strlstInArgs.size() == 1 ) {
            pLogger->setClassName(strlstInArgs[0].toStdString().c_str());
        }
        else {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
    }
    else if( strMth == "setObjectName" ) {
        if( strlstInArgs.size() == 0 ) {
            pLogger->setObjectName();
        }
        else if( strlstInArgs.size() == 1 ) {
            pLogger->setObjectName(strlstInArgs[0].toStdString().c_str());
        }
        else {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
    }

    // Expected Values
    //----------------

    if( pLogger != nullptr && i_pTestStep->getExpectedValues().isEmpty() )
    {
        QStringList strlstExpectedValues = getExpectedValues(pLogger);
        i_pTestStep->setExpectedValues(strlstExpectedValues);
    }

    // Result Values
    //--------------

    i_pTestStep->setInstruction("Check whether settings of logger in client have been changed as expected.");

    m_pDlgTestStep = new CDlgTestStep(i_pTestStep);
    m_pDlgTestStep->exec();
    delete m_pDlgTestStep;
    m_pDlgTestStep = nullptr;

} // doTestStepModifyLogger

//------------------------------------------------------------------------------
void CTest::doTestStepModifyLogServer( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTestGroup = i_pTestStep->getParentGroup();

    QString strTestGroupPath = pTestGroup == nullptr ? "" : pTestGroup->path();

    DllIf::CIpcLogServer* pLogServer = DllIf::CIpcLogServer::GetInstance();

    if( pLogServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Log server or log client not existing");
    }

    // Test Step
    //----------

    QString strOperation = i_pTestStep->getOperation();

    QString strNameSpace;
    QString strClassName;
    QString strSubClassName;
    QString strObjName;
    QString strMth;
    QStringList strlstInArgs;
    QString strMthRet;

    splitMethodCallOperation(strOperation, strClassName, strSubClassName, strObjName, strMth, strlstInArgs, strMthRet);

    if( strMth == "setEnabled" ) {
        if( strlstInArgs.size() != 1 ) {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
        else {
            bool bConverted = false;
            bool bEnabled = str2Bool(strlstInArgs[0], &bConverted);
            if( !bConverted ) {
                i_pTestStep->setExpectedValue("Invalid test step operation");
            }
            else {
                pLogServer->setEnabled(bEnabled);
                if( pLogServer->isEnabled() != bEnabled ) {
                    i_pTestStep->setExpectedValue("isEnabled != " + bool2Str(bEnabled));
                }
                else {
                    i_pTestStep->setExpectedValue("Enabled: " + bool2Str(bEnabled));
                }
            }
        }
    }
    else if( strMth == "setNewLoggersEnabledAsDefault" ) {
        if( strlstInArgs.size() != 1 ) {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
        else {
            bool bConverted = false;
            bool bEnabled = str2Bool(strlstInArgs[0], &bConverted);
            if( !bConverted ) {
                i_pTestStep->setExpectedValue("Invalid test step operation");
            }
            else {
                pLogServer->setNewLoggersEnabledAsDefault(bEnabled);
                if( pLogServer->areNewLoggersEnabledAsDefault() != bEnabled ) {
                    i_pTestStep->setExpectedValue("areNewLoggersEnabledAsDefault != " + bool2Str(bEnabled));
                }
                else {
                    i_pTestStep->setExpectedValue("NewLoggersEnabledAsDefault: " + bool2Str(bEnabled));
                }
            }
        }
    }
    else if( strMth == "setNewLoggersDefaultDetailLevel" ) {
        if( strlstInArgs.size() != 1 ) {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
        else {
            bool bConverted = false;
            CEnumLogDetailLevel eDetailLevel = CEnumLogDetailLevel::fromString(strlstInArgs[0], &bConverted);
            if( !bConverted ) {
                i_pTestStep->setExpectedValue("Invalid test step operation");
            }
            else {
                pLogServer->setNewLoggersDefaultDetailLevel(static_cast<DllIf::ELogDetailLevel>(eDetailLevel.enumerator()));
                if( pLogServer->getNewLoggersDefaultDetailLevel() != static_cast<DllIf::ELogDetailLevel>(eDetailLevel.enumerator()) ) {
                    i_pTestStep->setExpectedValue("getNewLoggersDefaultDetailLevel != " + eDetailLevel.toString());
                }
                else {
                    i_pTestStep->setExpectedValue("NewLoggersDefaultDetailLevel: " + eDetailLevel.toString());
                }
            }
        }
    }

    // Result Values
    //--------------

    i_pTestStep->setInstruction("Check whether settings of log server in client have been changed as expected.");

    m_pDlgTestStep = new CDlgTestStep(i_pTestStep);
    m_pDlgTestStep->exec();
    delete m_pDlgTestStep;
    m_pDlgTestStep = nullptr;

} // doTestStepModifyLogServer

//------------------------------------------------------------------------------
void CTest::doTestStepModifyLogServerLoggerFile( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTestGroup = i_pTestStep->getParentGroup();

    QString strTestGroupPath = pTestGroup == nullptr ? "" : pTestGroup->path();

    DllIf::CIpcLogServer* pLogServer = DllIf::CIpcLogServer::GetInstance();

    if( pLogServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Log server or log client not existing");
    }

    // Test Step
    //----------

    QStringList strlstExpectedValues;
    QStringList strlstResultValues;
    QString strExpectedValue;
    QString strResultValue;

    char* szName = DllIf::CLogServer::GetLoggerFileAbsoluteFilePath();
    QString strAbsFilePathPrev(szName);
    delete szName; szName = nullptr;

    QString strAbsFilePathNew = "C:/Temp/ZSLogServer-Loggers.xml";

    QFileInfo fileInfoFile(strAbsFilePathNew);

    if( fileInfoFile.exists() )
    {
        QFile file(fileInfoFile.absoluteFilePath());
        file.remove();
    }

    strlstExpectedValues.append("LoggerFileAbsoluteFilePath: " + strAbsFilePathNew);
    strlstExpectedValues.append("LoggerFileCompleteBaseName: " + fileInfoFile.completeBaseName());
    strlstExpectedValues.append("LoggerFileAbsolutePath: " + fileInfoFile.absolutePath());
    strlstExpectedValues.append("LoggerFileExists: true");

    DllIf::CLogServer::SetLoggerFileAbsoluteFilePath(strAbsFilePathNew.toStdString().c_str());

    CIdxTreeLoggers* pIdxTreeServer = ZS::System::CLogServer::GetInstance()->getLoggersIdxTree();

    QVector<CIdxTreeEntry*> arpTreeEntriesServerSaved = pIdxTreeServer->treeEntriesVec();

    for( auto& pTreeEntry : arpTreeEntriesServerSaved )
    {
        if( pTreeEntry != nullptr && pTreeEntry->entryType() == EIdxTreeEntryType::Leave)
        {
            ZS::System::CLogger* pLogger = dynamic_cast<ZS::System::CLogger*>(pTreeEntry);
            strExpectedValue = pLogger->keyInTree() + ": ";
            strExpectedValue += "Enabled: " + CEnumEnabled(pLogger->getEnabled()).toString();
            strExpectedValue += ", LogLevel: " + CEnumLogDetailLevel(pLogger->getLogLevel()).toString();
            strlstExpectedValues.append(strExpectedValue);
        }
    }

    pLogServer->saveLoggers(strAbsFilePathNew.toStdString().c_str());

    szName = DllIf::CLogServer::GetLoggerFileAbsoluteFilePath();
    strlstResultValues.append("LoggerFileAbsoluteFilePath: " + QString(szName));
    delete szName; szName = nullptr;

    szName = DllIf::CLogServer::GetLoggerFileCompleteBaseName();
    strlstResultValues.append("LoggerFileCompleteBaseName: " + QString(szName));
    delete szName; szName = nullptr;

    szName = DllIf::CLogServer::GetLoggerFileAbsolutePath();
    strlstResultValues.append("LoggerFileAbsolutePath: " + QString(szName));
    delete szName; szName = nullptr;

    strlstResultValues.append("LoggerFileExists: " + bool2Str(fileInfoFile.exists(strAbsFilePathNew)));

    pIdxTreeServer->clear();

    QThread::msleep(1000);

    int iClearedIdxTreeEntries = pIdxTreeServer->treeEntriesVec().size();

    pLogServer->recallLoggers(strAbsFilePathNew.toStdString().c_str());

    QVector<CIdxTreeEntry*> arpTreeEntriesServerRecalled = pIdxTreeServer->treeEntriesVec();

    for( auto& pTreeEntry : arpTreeEntriesServerRecalled )
    {
        if( pTreeEntry != nullptr && pTreeEntry->entryType() == EIdxTreeEntryType::Leave)
        {
            ZS::System::CLogger* pLogger = dynamic_cast<ZS::System::CLogger*>(pTreeEntry);
            strResultValue = pLogger->keyInTree() + ": ";
            strResultValue += "Enabled: " + CEnumEnabled(pLogger->isEnabled()).toString();
            strResultValue += ", LogLevel: " + CEnumLogDetailLevel(pLogger->getLogLevel()).toString();
            strlstResultValues.append(strResultValue);
        }
    }

    strlstExpectedValues.append("ClearedIdxTreeEntries: 0");
    strlstResultValues.append("ClearedIdxTreeEntries: " + QString::number(iClearedIdxTreeEntries));

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

    DllIf::CLogServer::SetLoggerFileAbsoluteFilePath(strAbsFilePathPrev.toStdString().c_str());

} // doTestStepModifyLogServerLoggerFile

//------------------------------------------------------------------------------
void CTest::doTestStepModifyLogServerLocalLogFile( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTestGroup = i_pTestStep->getParentGroup();

    QString strTestGroupPath = pTestGroup == nullptr ? "" : pTestGroup->path();

    DllIf::CIpcLogServer* pLogServer = DllIf::CIpcLogServer::GetInstance();

    if( pLogServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Log server or log client not existing");
    }

    // Test Step
    //----------

    QStringList strlstExpectedValues;
    QStringList strlstResultValues;
    QString strExpectedValue;
    QString strResultValue;

    char* szName = DllIf::CLogServer::GetLocalLogFileAbsoluteFilePath();
    QString strAbsFilePathPrev(szName);
    delete szName; szName = nullptr;

    QString strAbsPathNew = "C:/Temp";
    QString strCompleteBaseName = "ZSLogServer";
    QString strAbsFilePathNew = strAbsPathNew + QDir::separator() + strCompleteBaseName + ".log";

    QFileInfo fileInfoLogFile(strAbsPathNew + QDir::separator() + strCompleteBaseName + "00.log");

    if( fileInfoLogFile.exists() )
    {
        QFile logFile(fileInfoLogFile.absoluteFilePath());
        logFile.remove();
    }

    strlstExpectedValues.append("LocalLogFileAbsoluteFilePath: " + strAbsFilePathNew);
    strlstExpectedValues.append("LocalLogFileCompleteBaseName: " + strCompleteBaseName);
    strlstExpectedValues.append("LocalLogFileAbsolutePath: " + fileInfoLogFile.absolutePath());
    strlstExpectedValues.append("LocalLogFileExists: true");
    strlstExpectedValues.append("LocalLogFileLines: Debug Message");

    DllIf::CLogServer::SetLocalLogFileAbsoluteFilePath(strAbsFilePathNew.toStdString().c_str());

    DllIf::CLogger* pLogger = pLogServer->getLogger();
    DllIf::ELogDetailLevel logLevelPrev = pLogger->getLogLevel();
    pLogger->setLogLevel(static_cast<DllIf::ELogDetailLevel>(ELogDetailLevel::Debug));

    pLogServer->setLocalLogFileCloseFileAfterEachWrite(true);
    pLogServer->log(static_cast<DllIf::ELogDetailLevel>(ELogDetailLevel::Debug), "Debug Message");
    pLogServer->setLocalLogFileCloseFileAfterEachWrite(false);

    QFile logFile(fileInfoLogFile.absoluteFilePath());
    QString strLogFileLines;

    if( logFile.open(QFile::ReadOnly) )
    {
        QTextStream txtStreamLogFile(&logFile);
        strLogFileLines = txtStreamLogFile.readAll().simplified();
    }

    szName = DllIf::CLogServer::GetLocalLogFileAbsoluteFilePath();
    strlstResultValues.append("LocalLogFileAbsoluteFilePath: " + QString(szName));
    delete szName; szName = nullptr;

    szName = DllIf::CLogServer::GetLocalLogFileCompleteBaseName();
    strlstResultValues.append("LocalLogFileCompleteBaseName: " + QString(szName));
    delete szName; szName = nullptr;

    szName = DllIf::CLogServer::GetLocalLogFileAbsolutePath();
    strlstResultValues.append("LocalLogFileAbsolutePath: " + QString(szName));
    delete szName; szName = nullptr;

    strlstResultValues.append("LocalLogFileExists: " + bool2Str(fileInfoLogFile.exists()));
    strlstResultValues.append("LocalLogFileLines: " + strLogFileLines);

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

    DllIf::CLogServer::SetLocalLogFileAbsoluteFilePath(strAbsFilePathPrev.toStdString().c_str());
    pLogger->setLogLevel(static_cast<DllIf::ELogDetailLevel>(logLevelPrev));

} // doTestStepModifyLogServerLocalLogFile

//------------------------------------------------------------------------------
void CTest::doTestStepLoggerAddLogEntry( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTestGroup = i_pTestStep->getParentGroup();

    QString strTestGroupPath = pTestGroup == nullptr ? "" : pTestGroup->path();

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
        strlstExpectedValues.append("ExpectedResultsFileName not set");
    }
    else
    {
        readExpectedTestResults(strExpectedResultsAbsFilePath, strlstExpectedValues);
    }
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    QString strOperation = i_pTestStep->getOperation();

    QString strNameSpace;
    QString strClassName;
    QString strSubClassName;
    QString strObjName;
    QString strMth;
    QStringList strlstInArgs;
    QString strMthRet;

    splitMethodCallOperation(strOperation, strClassName, strSubClassName, strObjName, strMth, strlstInArgs, strMthRet);

    DllIf::CIpcLogServer* pLogServer = DllIf::CIpcLogServer::GetInstance();

    if( pLogServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Log server not existing");
    }
    else
    {
        pLogServer->clearLocalLogFile();
    }

    DllIf::CLogger* pLogger = DllIf::CIpcLogServer::GetLogger(strObjName.toStdString().c_str());

    if( strlstInArgs.size() != 2 )
    {
        i_pTestStep->setExpectedValue("Invalid Test Step Operation");
    }
    else
    {
        try
        {
            CEnumLogDetailLevel logLevel = strlstInArgs[0];
            pLogger->log(static_cast<DllIf::ELogDetailLevel>(logLevel.enumerator()), strlstInArgs[1].toStdString().c_str());
        }
        catch(CException&)
        {
            i_pTestStep->setExpectedValue("Invalid Log Level");
        }
    }

    // Result Values
    //--------------

    QString strInstruction = "Check whether client received the expected log output.\n";
    if( strlstExpectedValues.isEmpty() ) {
        strInstruction += "--- No log output expected ---";
    }
    else {
        strInstruction += "Clear Log Output before continue with next test step.";
    }
    i_pTestStep->setInstruction(strInstruction);

    m_pDlgTestStep = new CDlgTestStep(i_pTestStep);
    m_pDlgTestStep->exec();
    delete m_pDlgTestStep;
    m_pDlgTestStep = nullptr;

} // doTestStepLoggerAddLogEntry

//------------------------------------------------------------------------------
void CTest::doTestStepLoggerAddLogEntryMyThread( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTestGroup = i_pTestStep->getParentGroup();

    QString strTestGroupPath = pTestGroup == nullptr ? "" : pTestGroup->path();

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
        strlstExpectedValues.append("ExpectedResultsFileName not set");
    }
    else
    {
        readExpectedTestResults(strExpectedResultsAbsFilePath, strlstExpectedValues);
    }
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    QString strOperation = i_pTestStep->getOperation();

    QString strNameSpace;
    QString strClassName;
    QString strSubClassName;
    QString strObjName;
    QString strMth;
    QStringList strlstInArgs;
    QString strMthRet;

    splitMethodCallOperation(strOperation, strClassName, strSubClassName, strObjName, strMth, strlstInArgs, strMthRet);

    DllIf::CIpcLogServer* pLogServer = DllIf::CIpcLogServer::GetInstance();

    if( pLogServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Log server not existing");
    }
    else
    {
        pLogServer->clearLocalLogFile();
    }

    if( strlstInArgs.size() != 2 )
    {
        i_pTestStep->setExpectedValue("Invalid Test Step Operation");
    }
    else
    {
        try
        {
            CEnumLogDetailLevel logLevel = strlstInArgs[0];
            CMyThread myThread(strObjName, static_cast<DllIf::ELogDetailLevel>(logLevel.enumerator()), strlstInArgs[1]);
            myThread.start();
            myThread.waitForWorkDone();
        }
        catch(CException&)
        {
            i_pTestStep->setExpectedValue("Invalid Log Level");
        }
    }

    // Result Values
    //--------------

    QString strInstruction = "Check whether client received the expected log output.\n";
    if( strlstExpectedValues.isEmpty() ) {
        strInstruction += "--- No log output expected ---";
    }
    else {
        strInstruction += "Clear Log Output before continue with next test step.";
    }
    i_pTestStep->setInstruction(strInstruction);

    m_pDlgTestStep = new CDlgTestStep(i_pTestStep);
    m_pDlgTestStep->exec();
    delete m_pDlgTestStep;
    m_pDlgTestStep = nullptr;

} // doTestStepLoggerAddLogEntryMyThread

//------------------------------------------------------------------------------
void CTest::doTestStepLogServerAddLogEntry( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTestGroup = i_pTestStep->getParentGroup();

    QString strTestGroupPath = pTestGroup == nullptr ? "" : pTestGroup->path();

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
        strlstExpectedValues.append("ExpectedResultsFileName not set");
    }
    else
    {
        readExpectedTestResults(strExpectedResultsAbsFilePath, strlstExpectedValues);
    }
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    QString strOperation = i_pTestStep->getOperation();

    QString strNameSpace;
    QString strClassName;
    QString strSubClassName;
    QString strObjName;
    QString strMth;
    QStringList strlstInArgs;
    QString strMthRet;

    splitMethodCallOperation(strOperation, strClassName, strSubClassName, strObjName, strMth, strlstInArgs, strMthRet);

    DllIf::CIpcLogServer* pLogServer = DllIf::CIpcLogServer::GetInstance();

    if( pLogServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Log server not existing");
    }
    else
    {
        pLogServer->clearLocalLogFile();
    }

    if( strlstInArgs.size() != 2 )
    {
        i_pTestStep->setExpectedValue("Invalid Test Step Operation");
    }
    else if( pLogServer != nullptr )
    {
        try
        {
            CEnumLogDetailLevel logLevel = strlstInArgs[0];
            pLogServer->log(static_cast<DllIf::ELogDetailLevel>(logLevel.enumerator()), strlstInArgs[1].toStdString().c_str());
        }
        catch(CException&)
        {
            i_pTestStep->setExpectedValue("Invalid Log Level");
        }
    }

    // Result Values
    //--------------

    QString strInstruction = "Check whether client received the expected log output.\n";
    if( strlstExpectedValues.isEmpty() ) {
        strInstruction += "--- No log output expected ---";
    }
    else {
        strInstruction += "Clear Log Output before continue with next test step.";
    }
    i_pTestStep->setInstruction(strInstruction);

    m_pDlgTestStep = new CDlgTestStep(i_pTestStep);
    m_pDlgTestStep->exec();
    delete m_pDlgTestStep;
    m_pDlgTestStep = nullptr;

} // doTestStepLogServerAddLogEntry

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

//------------------------------------------------------------------------------
QStringList CTest::getExpectedValues( ZS::Log::DllIf::CLogger* i_pLogger ) const
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;

    char* szName = i_pLogger->name();
    strlstExpectedValues += "Name:          " + QString(szName);
    delete szName; szName = nullptr;

    if( QString(szName) == "NewLoggerDisabledAsDefault" ) {
        strlstExpectedValues += "Enabled:       " + CEnumEnabled(EEnabled::No).toString();
    }
    else if( QString(szName) == "NewLoggerDisabledAsDefault" ) {
        strlstExpectedValues += "Enabled:       " + CEnumEnabled(EEnabled::Yes).toString();
    }
    else {
        strlstExpectedValues += "Enabled:       " + CEnumEnabled(i_pLogger->isEnabled()).toString();
    }
    if( QString(szName) == "NewLoggerDefaultDetailLevelDebug" ) {
        strlstExpectedValues += "LogLevel:      " + CEnumLogDetailLevel(ELogDetailLevel::Debug).toString();
    }
    else if( QString(szName) == "NewLoggerDefaultDetailLevelNone" ) {
        strlstExpectedValues += "LogLevel:      " + CEnumLogDetailLevel(ELogDetailLevel::None).toString();
    }
    else {
        strlstExpectedValues += "LogLevel:      " + CEnumLogDetailLevel(i_pLogger->getLogLevel()).toString();
    }

    szName = i_pLogger->getDataFilter();
    strlstExpectedValues += "DataFilter:    " + QString(szName);
    delete szName; szName = nullptr;

    strlstExpectedValues += "AddThreadName: " + bool2Str(i_pLogger->addThreadName());
    strlstExpectedValues += "AddDateTime:   " + bool2Str(i_pLogger->addDateTime());
    strlstExpectedValues += "AddSystemTime: " + bool2Str(i_pLogger->addSystemTime());

    szName = i_pLogger->getNameSpace();
    strlstExpectedValues += "NameSpace:     " + QString(szName);
    delete szName; szName = nullptr;

    szName = i_pLogger->getClassName();
    strlstExpectedValues += "ClassName:     " + QString(szName);
    delete szName; szName = nullptr;

    szName = i_pLogger->getObjectName();
    strlstExpectedValues += "ObjName:       " + QString(szName);
    delete szName; szName = nullptr;

    return strlstExpectedValues;
}
