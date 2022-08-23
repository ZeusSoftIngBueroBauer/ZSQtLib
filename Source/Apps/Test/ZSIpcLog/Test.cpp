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
#include "MyThread.h"
#include "WidgetCentral.h"

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
    m_pTmrCheckLogClientLogWdgtIsEmpty(nullptr),
    m_hshReqsInProgress()
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

    m_pTmrCheckLogClientLogWdgtIsEmpty = new QTimer(this);
    m_pTmrCheckLogClientLogWdgtIsEmpty->setSingleShot(true);

    if( !QObject::connect(
        /* pObjSender   */ m_pTmrCheckLogClientLogWdgtIsEmpty,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTimerCheckLogClientLogWdgtIsEmptyTimeout()) ) )
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
        /* strName         */ "Step " + QString::number(++idxStep) + " Log Server Startup",
        /* strOperation    */ "LogServer::startup",
        /* pTSGrpParent    */ pTestGroupStartup,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerStartup(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Log Client Connect",
        /* strOperation    */ "LogClient::connect",
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
        /* strOperation    */ "LogClient::disconnect",
        /* pTSGrpParent    */ pTestGroupShutdown,
        /* szDoTestStepFct */ SLOT(doTestStepLogClientDisconnect(ZS::Test::CTestStep*)) );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Log Server Shutdown",
        /* strOperation    */ "LogServer::shutdown",
        /* pTSGrpParent    */ pTestGroupShutdown,
        /* szDoTestStepFct */ SLOT(doTestStepLogServerShutdown(ZS::Test::CTestStep*)) );

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

    m_pTmrTestStepTimeout = nullptr;
    m_pTmrCheckLogClientLogWdgtIsEmpty = nullptr;
    m_hshReqsInProgress.clear();

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
        CRequest* pReq = pLogServer->startup(1000);

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
        i_pTestStep->setExpectedValue("Log server not existing");
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
        i_pTestStep->setExpectedValue("Log server or log client not existing");
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

                strExpectedValue = pLogger->keyInTree() + ": ";
                strExpectedValue += "Enabled: " + CEnumEnabled(pLogger->getEnabled()).toString();
                strExpectedValue += ", LogLevel: " + CEnumLogDetailLevel(pLogger->getLogLevel()).toString();
                strlstExpectedValues.append(strExpectedValue);
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
                m_pTmrTestStepTimeout->start(1000);

                if( !QObject::connect(
                    /* pObjSender   */ pLogClient,
                    /* szSignal     */ SIGNAL(loggerInserted(QObject*, const QString&)),
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
        i_pTestStep->setExpectedValue("Log client not existing");
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

        // We could also check whether the log client cleared the index tree of loggers.
        // But the order in which the onDisconnected slots are invoked when emitting the
        // IpcClient disconnected signal is unpredictable. The test may receive this signal before
        // the log client and the loggers index tree may not be cleared at this time.

        //strExpectedValue = "LoggersIdxTree.isEmpty";
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
void CTest::doTestStepLogServerGetLogger( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTestGroup = i_pTestStep->getParentGroup();

    QString strTestGroupPath = pTestGroup == nullptr ? "" : pTestGroup->path();

    CIpcLogServer* pLogServer = ZS::Log::CIpcLogServer::GetInstance();
    CIpcLogClient* pLogClient = CApplication::GetInstance()->getLogClient();

    if( pLogServer == nullptr || pLogClient == nullptr )
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

    CLogger* pLoggerServer = nullptr;
    CLogger* pLoggerClient = nullptr;
    QString  strLoggerName;

    if( strMth.compare("getLogger", Qt::CaseInsensitive) == 0 )
    {
        if( strlstInArgs.size() == 0 )
        {
            pLoggerServer = pLogServer->getLogger();
        }
        else if( strlstInArgs.size() == 1 )
        {
            strLoggerName = strlstInArgs[0].replace("-", "::");
            pLoggerServer = CLogServer::GetLogger(strLoggerName);
        }
    }

    // Expected Values
    //----------------

    if( pLoggerServer != nullptr && pLogClient != nullptr && i_pTestStep->getExpectedValues().isEmpty() )
    {
        CIdxTreeLoggers* pIdxTreeClient = pLogClient->getLoggersIdxTree();

        QString     strExpectedValue;
        QStringList strlstExpectedValues;

        pLoggerClient = dynamic_cast<CLogger*>(pIdxTreeClient->findEntry(pLoggerServer->keyInTree()));

        strExpectedValue = pLoggerServer->keyInTree() + ": ";
        if( strLoggerName == "NewLoggerDisabledAsDefault" ) {
            strExpectedValue += "Enabled: " + CEnumEnabled(EEnabled::No).toString();
        }
        else if( strLoggerName == "NewLoggerDisabledAsDefault" ) {
            strExpectedValue += "Enabled: " + CEnumEnabled(EEnabled::Yes).toString();
        }
        else {
            strExpectedValue += "Enabled: " + CEnumEnabled(pLoggerServer->getEnabled()).toString();
        }
        if( strLoggerName == "NewLoggerDefaultDetailLevelDebug" ) {
            strExpectedValue += ", LogLevel: " + CEnumLogDetailLevel(ELogDetailLevel::Debug).toString();
        }
        else if( strLoggerName == "NewLoggerDefaultDetailLevelNone" ) {
            strExpectedValue += ", LogLevel: " + CEnumLogDetailLevel(ELogDetailLevel::None).toString();
        }
        else {
            strExpectedValue += ", LogLevel: " + CEnumLogDetailLevel(pLoggerServer->getLogLevel()).toString();
        }
        strExpectedValue += ", DataFilter: " + pLoggerServer->getDataFilter();
        strExpectedValue += ", AddThreadName: " + bool2Str(pLoggerServer->addThreadName());
        strExpectedValue += ", AddDateTime: " + bool2Str(pLoggerServer->addDateTime());
        strExpectedValue += ", AddSystemTime: " + bool2Str(pLoggerServer->addSystemTime());
        strExpectedValue += ", NameSpace: " + pLoggerServer->getNameSpace();
        strExpectedValue += ", ClassName: " + pLoggerServer->getClassName();
        strExpectedValue += ", ObjName: " + pLoggerServer->getObjectName();
        strlstExpectedValues.append(strExpectedValue);

        i_pTestStep->setExpectedValues(strlstExpectedValues);
    }

    // Result Values
    //--------------

    if( pLogServer != nullptr && pLogClient != nullptr )
    {
        if( pLoggerClient == nullptr )
        {
            m_pTmrTestStepTimeout->start(1000);

            if( !QObject::connect(
                /* pObjSender   */ pLogClient,
                /* szSignal     */ SIGNAL(loggerInserted(QObject*, const QString&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onLogClientLoggerInserted(QObject*, const QString&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
        }
        else
        {
            QString     strResultValue;
            QStringList strlstResultValues;

            strResultValue = pLoggerClient->keyInTree() + ": ";
            strResultValue += "Enabled: " + CEnumEnabled(pLoggerClient->getEnabled()).toString();
            strResultValue += ", LogLevel: " + CEnumLogDetailLevel(pLoggerClient->getLogLevel()).toString();
            strResultValue += ", DataFilter: " + pLoggerClient->getDataFilter();
            strResultValue += ", AddThreadName: " + bool2Str(pLoggerClient->addThreadName());
            strResultValue += ", AddDateTime: " + bool2Str(pLoggerClient->addDateTime());
            strResultValue += ", AddSystemTime: " + bool2Str(pLoggerClient->addSystemTime());
            strResultValue += ", NameSpace: " + pLoggerClient->getNameSpace();
            strResultValue += ", ClassName: " + pLoggerClient->getClassName();
            strResultValue += ", ObjName: " + pLoggerClient->getObjectName();
            strlstResultValues.append(strResultValue);

            i_pTestStep->setResultValues(strlstResultValues);
        }
    }

} // doTestStepLogServerGetLogger

//------------------------------------------------------------------------------
void CTest::doTestStepModifyLogger( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTestGroup = i_pTestStep->getParentGroup();

    QString strTestGroupPath = pTestGroup == nullptr ? "" : pTestGroup->path();

    CIpcLogServer* pLogServer = ZS::Log::CIpcLogServer::GetInstance();
    CIpcLogClient* pLogClient = CApplication::GetInstance()->getLogClient();

    if( pLogServer == nullptr || pLogClient == nullptr )
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

    //QString strLoggerName = strObjName.replace("-", "::");
    CLogger* pLoggerServer = CLogServer::GetLogger(strObjName);
    CLogger* pLoggerClient = nullptr;

    if( strMth == "setLogLevel" ) {
        if( strlstInArgs.size() != 1 ) {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
        else {
            try {
                CEnumLogDetailLevel eDetailLevel = strlstInArgs[0];
                pLoggerServer->setLogLevel(eDetailLevel.enumerator());
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
                pLoggerServer->setEnabled(eEnabled.enumerator());
            }
            catch(CException&) {
                i_pTestStep->setExpectedValue("Invalid test step operation");
            }
        }
    }
    else if( strMth == "setDataFilter" ) {
        if( strlstInArgs.size() == 0 ) {
            pLoggerServer->setDataFilter();
        }
        else if( strlstInArgs.size() == 1 ) {
            pLoggerServer->setDataFilter(strlstInArgs[0]);
        }
        else {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
    }
    else if( strMth == "setAddThreadName" ) {
        if( strlstInArgs.size() == 1 ) {
            bool bAdd = str2Bool(strlstInArgs[0]);
            pLoggerServer->setAddThreadName(bAdd);
        }
        else {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
    }
    else if( strMth == "setAddDateTime" ) {
        if( strlstInArgs.size() == 1 ) {
            bool bAdd = str2Bool(strlstInArgs[0]);
            pLoggerServer->setAddDateTime(bAdd);
        }
        else {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
    }
    else if( strMth == "setAddSystemTime" ) {
        if( strlstInArgs.size() == 1 ) {
            bool bAdd = str2Bool(strlstInArgs[0]);
            pLoggerServer->setAddSystemTime(bAdd);
        }
        else {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
    }
    else if( strMth == "setNameSpace" ) {
        if( strlstInArgs.size() == 0 ) {
            pLoggerServer->setNameSpace();
        }
        else if( strlstInArgs.size() == 1 ) {
            pLoggerServer->setNameSpace(strlstInArgs[0]);
        }
        else {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
    }
    else if( strMth == "setClassName" ) {
        if( strlstInArgs.size() == 0 ) {
            pLoggerServer->setClassName();
        }
        else if( strlstInArgs.size() == 1 ) {
            pLoggerServer->setClassName(strlstInArgs[0]);
        }
        else {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
    }
    else if( strMth == "setObjectName" ) {
        if( strlstInArgs.size() == 0 ) {
            pLoggerServer->setObjectName();
        }
        else if( strlstInArgs.size() == 1 ) {
            pLoggerServer->setObjectName(strlstInArgs[0]);
        }
        else {
            i_pTestStep->setExpectedValue("Invalid test step operation");
        }
    }

    // Expected Values
    //----------------

    if( pLogServer != nullptr && i_pTestStep->getExpectedValues().isEmpty() )
    {
        CIdxTreeLoggers* pIdxTreeServer = pLogServer->GetLoggersIdxTree();
        QVector<CIdxTreeEntry*> arpTreeEntriesServer = pIdxTreeServer->treeEntriesVec();

        QString     strExpectedValue;
        QStringList strlstExpectedValues;

        for( auto& pTreeEntry : arpTreeEntriesServer )
        {
            if( pTreeEntry != nullptr && pTreeEntry->entryType() == EIdxTreeEntryType::Leave)
            {
                CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);

                strExpectedValue = pLogger->keyInTree() + ": ";
                strExpectedValue += "Enabled: " + CEnumEnabled(pLogger->getEnabled()).toString();
                strExpectedValue += ", LogLevel: " + CEnumLogDetailLevel(pLogger->getLogLevel()).toString();
                strExpectedValue += ", DataFilter: " + pLogger->getDataFilter();
                strExpectedValue += ", AddThreadName: " + bool2Str(pLogger->addThreadName());
                strExpectedValue += ", AddDateTime: " + bool2Str(pLogger->addDateTime());
                strExpectedValue += ", AddSystemTime: " + bool2Str(pLogger->addSystemTime());
                strExpectedValue += ", NameSpace: " + pLogger->getNameSpace();
                strExpectedValue += ", ClassName: " + pLogger->getClassName();
                strExpectedValue += ", ObjName: " + pLogger->getObjectName();
                strlstExpectedValues.append(strExpectedValue);
            }
        }
        i_pTestStep->setExpectedValues(strlstExpectedValues);
    }

    // Result Values
    //--------------

    if( pLogServer != nullptr && pLogClient != nullptr )
    {
        m_pTmrTestStepTimeout->start(1000);

        if( !QObject::connect(
            /* pObjSender   */ pLogClient,
            /* szSignal     */ SIGNAL(loggerChanged(QObject*, const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onLogClientLoggerChanged(QObject*, const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }

} // doTestStepModifyLogger

//------------------------------------------------------------------------------
void CTest::doTestStepModifyLogServer( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTestGroup = i_pTestStep->getParentGroup();

    QString strTestGroupPath = pTestGroup == nullptr ? "" : pTestGroup->path();

    CIpcLogServer* pLogServer = ZS::Log::CIpcLogServer::GetInstance();
    CIpcLogClient* pLogClient = CApplication::GetInstance()->getLogClient();

    if( pLogServer == nullptr || pLogClient == nullptr )
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
                pLogServer->setNewLoggersDefaultDetailLevel(eDetailLevel.enumerator());
                if( pLogServer->getNewLoggersDefaultDetailLevel() != eDetailLevel.enumerator() ) {
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

    m_pTmrTestStepTimeout->start(1000);

    if( !QObject::connect(
        /* pObjSender   */ pLogClient,
        /* szSignal     */ SIGNAL(logSettingsChanged(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onLogClientLogSettingsChanged(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // doTestStepModifyLogServer

//------------------------------------------------------------------------------
void CTest::doTestStepModifyLogServerLoggerFile( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTestGroup = i_pTestStep->getParentGroup();

    QString strTestGroupPath = pTestGroup == nullptr ? "" : pTestGroup->path();

    CIpcLogServer* pLogServer = ZS::Log::CIpcLogServer::GetInstance();
    CIpcLogClient* pLogClient = CApplication::GetInstance()->getLogClient();

    if( pLogServer == nullptr || pLogClient == nullptr )
    {
        i_pTestStep->setExpectedValue("Log server or log client not existing");
    }

    // Test Step
    //----------

    QStringList strlstExpectedValues;
    QStringList strlstResultValues;
    QString strExpectedValue;
    QString strResultValue;

    QString strAbsFilePathPrev = CLogServer::GetLoggerFileAbsoluteFilePath();

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

    CLogServer::SetLoggerFileAbsoluteFilePath(strAbsFilePathNew);

    CIdxTreeLoggers* pIdxTreeServer = pLogServer->getLoggersIdxTree();

    QVector<CIdxTreeEntry*> arpTreeEntriesServerSaved = pIdxTreeServer->treeEntriesVec();

    for( auto& pTreeEntry : arpTreeEntriesServerSaved )
    {
        if( pTreeEntry != nullptr && pTreeEntry->entryType() == EIdxTreeEntryType::Leave)
        {
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            strExpectedValue = pLogger->keyInTree() + ": ";
            strExpectedValue += "Enabled: " + CEnumEnabled(pLogger->getEnabled()).toString();
            strExpectedValue += ", LogLevel: " + CEnumLogDetailLevel(pLogger->getLogLevel()).toString();
            strlstExpectedValues.append(strExpectedValue);
        }
    }

    pLogServer->saveLoggers(strAbsFilePathNew);

    strlstResultValues.append("LoggerFileAbsoluteFilePath: " + CLogServer::GetLoggerFileAbsoluteFilePath());
    strlstResultValues.append("LoggerFileCompleteBaseName: " + CLogServer::GetLoggerFileCompleteBaseName());
    strlstResultValues.append("LoggerFileAbsolutePath: " + CLogServer::GetLoggerFileAbsolutePath());
    strlstResultValues.append("LoggerFileExists: " + bool2Str(fileInfoFile.exists(strAbsFilePathNew)));

    pIdxTreeServer->clear();

    int iClearedIdxTreeEntries = pIdxTreeServer->treeEntriesVec().size();

    pLogServer->recallLoggers(strAbsFilePathNew);

    QVector<CIdxTreeEntry*> arpTreeEntriesServerRecalled = pIdxTreeServer->treeEntriesVec();

    for( auto& pTreeEntry : arpTreeEntriesServerRecalled )
    {
        if( pTreeEntry != nullptr && pTreeEntry->entryType() == EIdxTreeEntryType::Leave)
        {
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            strResultValue = pLogger->keyInTree() + ": ";
            strResultValue += "Enabled: " + CEnumEnabled(pLogger->getEnabled()).toString();
            strResultValue += ", LogLevel: " + CEnumLogDetailLevel(pLogger->getLogLevel()).toString();
            strlstResultValues.append(strResultValue);
        }
    }

    strlstExpectedValues.append("ClearedIdxTreeEntries: 0");
    strlstResultValues.append("ClearedIdxTreeEntries: " + QString::number(iClearedIdxTreeEntries));

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

    CLogServer::SetLoggerFileAbsoluteFilePath(strAbsFilePathPrev);

} // doTestStepModifyLogServerLoggerFile

//------------------------------------------------------------------------------
void CTest::doTestStepModifyLogServerLocalLogFile( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStepGroup* pTestGroup = i_pTestStep->getParentGroup();

    QString strTestGroupPath = pTestGroup == nullptr ? "" : pTestGroup->path();

    CIpcLogServer* pLogServer = ZS::Log::CIpcLogServer::GetInstance();
    CIpcLogClient* pLogClient = CApplication::GetInstance()->getLogClient();

    if( pLogServer == nullptr || pLogClient == nullptr )
    {
        i_pTestStep->setExpectedValue("Log server or log client not existing");
    }

    // Test Step
    //----------

    QStringList strlstExpectedValues;
    QStringList strlstResultValues;
    QString strExpectedValue;
    QString strResultValue;

    QString strAbsFilePathPrev = CLogServer::GetLocalLogFileAbsoluteFilePath();

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

    CLogServer::SetLocalLogFileAbsoluteFilePath(strAbsFilePathNew);

    CLogger* pLogger = pLogServer->getLogger();
    ELogDetailLevel logLevelPrev = pLogger->getLogLevel();
    pLogger->setLogLevel(ELogDetailLevel::Debug);

    pLogServer->setLocalLogFileCloseFileAfterEachWrite(true);
    pLogServer->log(ELogDetailLevel::Debug, "Debug Message");
    pLogServer->setLocalLogFileCloseFileAfterEachWrite(false);

    QFile logFile(fileInfoLogFile.absoluteFilePath());
    QString strLogFileLines;

    if( logFile.open(QFile::ReadOnly) )
    {
        QTextStream txtStreamLogFile(&logFile);
        strLogFileLines = txtStreamLogFile.readAll().simplified();
    }

    strlstResultValues.append("LocalLogFileAbsoluteFilePath: " + CLogServer::GetLocalLogFileAbsoluteFilePath());
    strlstResultValues.append("LocalLogFileCompleteBaseName: " + CLogServer::GetLocalLogFileCompleteBaseName());
    strlstResultValues.append("LocalLogFileAbsolutePath: " + CLogServer::GetLocalLogFileAbsolutePath());
    strlstResultValues.append("LocalLogFileExists: " + bool2Str(fileInfoLogFile.exists()));
    strlstResultValues.append("LocalLogFileLines: " + strLogFileLines);

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

    CLogServer::SetLocalLogFileAbsoluteFilePath(strAbsFilePathPrev);
    pLogger->setLogLevel(logLevelPrev);

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

    CIpcLogServer* pLogServer = ZS::Log::CIpcLogServer::GetInstance();

    if( pLogServer == nullptr )
    {
        i_pTestStep->setExpectedValue("Log server not existing");
    }
    else
    {
        pLogServer->clearLocalLogFile();
    }

    CLogger* pLogger = CIpcLogServer::GetLogger(strObjName);

    if( strlstInArgs.size() != 2 )
    {
        i_pTestStep->setExpectedValue("Invalid Test Step Operation");
    }
    else
    {
        try
        {
            CEnumLogDetailLevel logLevel = strlstInArgs[0];
            pLogger->log(logLevel.enumerator(), strlstInArgs[1]);
        }
        catch(CException&)
        {
            i_pTestStep->setExpectedValue("Invalid Log Level");
        }
    }

    // Result Values
    //--------------

    CWidgetCentral* pWdgtCentral = CWidgetCentral::GetInstance();

    CWdgtLog* pWdgtLog = pWdgtCentral->getLogWdgt();

    if( pWdgtLog != nullptr )
    {
        pWdgtLog->getTextEdit()->clear();

        if( strlstExpectedValues.isEmpty() )
        {
            m_pTmrCheckLogClientLogWdgtIsEmpty->start(100);
        }
        else
        {
            if( !QObject::connect(
                /* pObjSender   */ pWdgtLog,
                /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onLogClientLogWdgtTextItemAdded(const QString&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            m_pTmrTestStepTimeout->start(1000);
        }
    }
    else
    {
        i_pTestStep->setResultValue("No log widget");
    }

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

    CIpcLogServer* pLogServer = ZS::Log::CIpcLogServer::GetInstance();

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
            CMyThread myThread(strObjName, logLevel.enumerator(), strlstInArgs[1]);
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

    CWidgetCentral* pWdgtCentral = CWidgetCentral::GetInstance();

    CWdgtLog* pWdgtLog = pWdgtCentral->getLogWdgt();

    if( pWdgtLog != nullptr )
    {
        pWdgtLog->getTextEdit()->clear();

        if( strlstExpectedValues.isEmpty() )
        {
            m_pTmrCheckLogClientLogWdgtIsEmpty->start(100);
        }
        else
        {
            if( !QObject::connect(
                /* pObjSender   */ pWdgtLog,
                /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onLogClientLogWdgtTextItemAdded(const QString&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            m_pTmrTestStepTimeout->start(1000);
        }
    }
    else
    {
        i_pTestStep->setResultValue("No log widget");
    }

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

    CIpcLogServer* pLogServer = ZS::Log::CIpcLogServer::GetInstance();

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
            pLogServer->log(logLevel.enumerator(), strlstInArgs[1]);
        }
        catch(CException&)
        {
            i_pTestStep->setExpectedValue("Invalid Log Level");
        }
    }

    // Result Values
    //--------------

    CWidgetCentral* pWdgtCentral = CWidgetCentral::GetInstance();

    CWdgtLog* pWdgtLog = pWdgtCentral->getLogWdgt();

    if( pWdgtLog != nullptr )
    {
        pWdgtLog->getTextEdit()->clear();

        if( strlstExpectedValues.isEmpty() )
        {
            m_pTmrCheckLogClientLogWdgtIsEmpty->start(100);
        }
        else
        {
            if( !QObject::connect(
                /* pObjSender   */ pWdgtLog,
                /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onLogClientLogWdgtTextItemAdded(const QString&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            m_pTmrTestStepTimeout->start(1000);
        }
    }
    else
    {
        i_pTestStep->setResultValue("No log widget");
    }

} // doTestStepLogServerAddLogEntry

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
                        // If the log server has already loggers in the index tree the test step
                        // is not finished if the log client is connected.
                        // The client got to query the loggers after the connection
                        // has been established. We need to wait until all expected loggers
                        // have been received by the client before finishing this test step.
                        // On receiving a logger the slot "onLogClientLoggerInserted" is called.
                        // If all expected loggers have been received te slot finishes the test step.

                        // If the log server does not have any loggers in the index tree the test step is
                        // finished after the log client is connected. But this should never be the case.
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

                        // We could also check whether the log client cleared the index tree of loggers.
                        // But the order in which the onDisconnected slots are invoked when emitting the
                        // IpcClient disconnected signal is unpredictable. The test may receive this signal before
                        // the log client and the loggers index tree may not be cleared at this time.

                        pTestStep->setResultValues(strlstResultValues);

                    } // if( pTestStep->getOperation().contains("LogClient::disconnect") )
                } // if( pLogClient != nullptr )
            } // if( pTestStep != nullptr )
        } // if( errResultInfo.isErrorResult() || i_reqDscr.m_iProgress_perCent >= 100 )
    } // if( m_hshReqsInProgress.contains(i_reqDscr.m_iId) )

} // onRequestChanged

//------------------------------------------------------------------------------
void CTest::onLogClientLoggerInserted( QObject* /*i_pLogClient*/, const QString& i_strKeyInTree )
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
            if( pTestStep->getOperation().contains("LogClient::connect", Qt::CaseInsensitive) )
            {
                // Test step is finished if client is connected and received all loggers from the server.

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
                        /* szSignal     */ SIGNAL(loggerInserted(QObject*, const QString&)),
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
                        if( pTreeEntry != nullptr && pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
                        {
                            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);

                            strResultValue = pLogger->keyInTree() + ": ";
                            strResultValue += "Enabled: " + CEnumEnabled(pLogger->getEnabled()).toString();
                            strResultValue += ", LogLevel: " + CEnumLogDetailLevel(pLogger->getLogLevel()).toString();
                            strlstResultValues.append(strResultValue);
                        }
                    }

                    pTestStep->setResultValues(strlstResultValues);

                } // if( bTestStepFinished )
            } // if( pTestStep->getOperation().contains("LogClient::connect") )

            else if( pTestStep->getOperation().contains("LogServer::getLogger", Qt::CaseInsensitive) )
            {
                // Test step is finished if client received the new logger entry.

                if( m_pTmrTestStepTimeout->isActive() )
                {
                    m_pTmrTestStepTimeout->stop();
                }

                QObject::disconnect(
                    /* pObjSender   */ pLogClient,
                    /* szSignal     */ SIGNAL(loggerInserted(QObject*, const QString&)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onLogClientLoggerInserted(QObject*, const QString&)) );

                // Actual Values
                //---------------

                CIdxTreeLoggers* pIdxTreeClient = pLogClient->getLoggersIdxTree();

                CIdxTreeEntry* pTreeEntryClient = pIdxTreeClient->findEntry(i_strKeyInTree);

                QString     strResultValue;
                QStringList strlstResultValues;

                CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntryClient);

                if( pLogger == nullptr )
                {
                    strResultValue = i_strKeyInTree + " not found in client's index tree";
                }
                else
                {
                    strResultValue = pLogger->keyInTree() + ": ";
                    strResultValue += "Enabled: " + CEnumEnabled(pLogger->getEnabled()).toString();
                    strResultValue += ", LogLevel: " + CEnumLogDetailLevel(pLogger->getLogLevel()).toString();
                    strResultValue += ", DataFilter: " + pLogger->getDataFilter();
                    strResultValue += ", AddThreadName: " + bool2Str(pLogger->addThreadName());
                    strResultValue += ", AddDateTime: " + bool2Str(pLogger->addDateTime());
                    strResultValue += ", AddSystemTime: " + bool2Str(pLogger->addSystemTime());
                    strResultValue += ", NameSpace: " + pLogger->getNameSpace();
                    strResultValue += ", ClassName: " + pLogger->getClassName();
                    strResultValue += ", ObjName: " + pLogger->getObjectName();
                    strlstResultValues.append(strResultValue);
                }

                pTestStep->setResultValues(strlstResultValues);

            } // if( pTestStep->getOperation().contains("LogServer::getLogger") )
        } // if( pLogClient != nullptr )
    } // if( pTestStep != nullptr )
} // onLogClientLoggerInserted

//------------------------------------------------------------------------------
void CTest::onLogClientLoggerChanged( QObject* /*i_pLogClient*/, const QString& /*i_strKeyInTree*/ )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStep* pTestStep = getCurrentTestStep();

    if( pTestStep != nullptr )
    {
        CIpcLogClient* pLogClient = CApplication::GetInstance()->getLogClient();
        QVector<CIdxTreeEntry*> arpTreeEntriesClient;
        if( pLogClient != nullptr )
        {
            CIdxTreeLoggers* pIdxTreeClient = pLogClient->getLoggersIdxTree();
            arpTreeEntriesClient = pIdxTreeClient->treeEntriesVec();
        }

        CIpcLogServer* pLogServer = CApplication::GetInstance()->getLogServer();
        QVector<CIdxTreeEntry*> arpTreeEntriesServer;
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTreeServer = pLogServer->getLoggersIdxTree();
            arpTreeEntriesServer = pIdxTreeServer->treeEntriesVec();
        }

        if( arpTreeEntriesClient.size() == arpTreeEntriesServer.size() )
        {
            if( m_pTmrTestStepTimeout->isActive() )
            {
                m_pTmrTestStepTimeout->stop();
            }

            QObject::disconnect(
                /* pObjSender   */ pLogClient,
                /* szSignal     */ SIGNAL(loggerChanged(QObject*, const QString&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onLogClientLoggerChanged(QObject*, const QString&)) );

            QString     strResultValue;
            QStringList strlstResultValues;

            for( auto& pTreeEntry : arpTreeEntriesClient )
            {
                if( pTreeEntry != nullptr && pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
                {
                    CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);

                    strResultValue = pLogger->keyInTree() + ": ";
                    strResultValue += "Enabled: " + CEnumEnabled(pLogger->getEnabled()).toString();
                    strResultValue += ", LogLevel: " + CEnumLogDetailLevel(pLogger->getLogLevel()).toString();
                    strResultValue += ", DataFilter: " + pLogger->getDataFilter();
                    strResultValue += ", AddThreadName: " + bool2Str(pLogger->addThreadName());
                    strResultValue += ", AddDateTime: " + bool2Str(pLogger->addDateTime());
                    strResultValue += ", AddSystemTime: " + bool2Str(pLogger->addSystemTime());
                    strResultValue += ", NameSpace: " + pLogger->getNameSpace();
                    strResultValue += ", ClassName: " + pLogger->getClassName();
                    strResultValue += ", ObjName: " + pLogger->getObjectName();
                    strlstResultValues.append(strResultValue);
                }
            }

            pTestStep->setResultValues(strlstResultValues);

        } // if( arpTreeEntriesClient.size() == arpTreeEntriesServer.size() )
    } // if( pTestStep != nullptr )
} // onLogClientLoggerChanged

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
            QStringList strlstResultValuesLogWdgt;
            QStringList strlstResultValuesLogFile;
            QStringList strlstResultValues;
            QStringList strlstExpectedValues = pTestStep->getExpectedValues();

            QTextEdit* pEdtLog = pWdgtLog->getTextEdit();
            QTextDocument* pDocLog = pEdtLog->document();

            int iDocLogLineCount = pDocLog->lineCount();

            if( iDocLogLineCount == strlstExpectedValues.size() )
            {
                if( m_pTmrTestStepTimeout->isActive() )
                {
                    m_pTmrTestStepTimeout->stop();
                }

                QObject::disconnect(
                    /* pObjSender   */ pWdgtLog,
                    /* szSignal     */ SIGNAL(textItemAdded(const QString&)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onLogClientLogWdgtTextItemAdded(const QString&)) );

                // Retrieve result values from log widget
                //---------------------------------------

                // The list log must be cleared on starting a new test step.
                QString strAllLines = pEdtLog->toPlainText();
                QTextStream txtStream(&strAllLines, QIODevice::ReadOnly);
                while (!txtStream.atEnd())
                {
                    QString strLine = txtStream.readLine();
                    strlstResultValuesLogWdgt << strLine;
                }

                // Retrieve result values from log file
                //-------------------------------------

                // Range of IniFileScope: ["AppDir", "User", "System"]
                #ifdef __linux__
                // Using "System" on linux Mint ends up in directory "etc/xdg/<CompanyName>"
                // where the application has not write access rights. Stupid ...
                QString strIniFileScope = "User";
                #else
                QString strIniFileScope = "System"; // Default
                #endif

                QString strAppLogDir = ZS::System::getAppLogDir(strIniFileScope);

                QString strLogFileSuffix = "log";
                QString strLogFileBaseName = "ZSLogServer00";

                QString strLogFileAbsFilePath = strAppLogDir + "/" + strLogFileBaseName + "." + strLogFileSuffix;

                QFile fileLogFile(strLogFileAbsFilePath);

                // Temporarily disable log output. This will close (flush buffer) the file
                // so that its content can be evaluated by onLogClientLogWdgtTextItemAdded.
                if( pLogServer != nullptr )
                {
                    pLogServer->setEnabled(false);
                    pLogServer->setEnabled(true);
                }

                if( !fileLogFile.open(QIODevice::ReadOnly|QIODevice::Text) )
                {
                    strlstResultValuesLogFile.append("Could not open file " + strLogFileAbsFilePath);
                }
                else
                {
                    QTextStream txtstrmLogFile(&fileLogFile);
                    strAllLines = txtstrmLogFile.readAll();
                    fileLogFile.close();

                    QTextStream txtStreamAllLines(&strAllLines, QIODevice::ReadOnly);
                    while (!txtStreamAllLines.atEnd())
                    {
                        QString strLine = txtStreamAllLines.readLine();
                        strlstResultValuesLogFile << strLine;
                    }
                }

                if( strlstExpectedValues.size() != strlstResultValuesLogWdgt.size() )
                {
                    strlstResultValues.append("Number of lines in log widget not as expected");
                    strlstResultValues = strlstResultValuesLogWdgt;
                }
                else if( strlstExpectedValues.size() != strlstResultValuesLogFile.size() )
                {
                    strlstResultValues.append("Number of lines in log file not as expected");
                    strlstResultValues = strlstResultValuesLogFile;
                }
                else
                {
                    for( int idxLine = 0; idxLine < strlstExpectedValues.size(); ++idxLine )
                    {
                        QString strExpectedValue = strlstExpectedValues[idxLine];
                        QString strResultValueLogWdgt = strlstResultValuesLogWdgt[idxLine];
                        QString strResultValueLogFile = strlstResultValuesLogFile[idxLine];
                        QString strTmpLogWdgt;
                        QString strTmpLogFile;

                        int idxBegLogWdgt = -1;
                        int idxEndLogWdgt = -1;
                        int idxBegLogFile = -1;
                        int idxEndLogFile = -1;

                                                                  //01234567890123456789012
                        int idxExpected = strExpectedValue.indexOf("yyyy-MM-dd hh:mm:ss.zzz");
                        if( idxExpected >= 0 )
                        {
                            QRegExp regExp("\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}:\\d{3}");
                            idxBegLogWdgt = regExp.indexIn(strResultValueLogWdgt);
                            idxBegLogFile = regExp.indexIn(strResultValueLogFile);
                            if( idxBegLogWdgt == idxExpected && idxBegLogFile == idxExpected ) {
                                strTmpLogWdgt = strResultValueLogWdgt.mid(idxExpected, 23);
                                strTmpLogFile = strResultValueLogFile.mid(idxExpected, 23);
                                if( strTmpLogWdgt == strTmpLogFile ) {
                                    strExpectedValue = strExpectedValue.replace("yyyy-MM-dd hh:mm:ss.zzz", strTmpLogWdgt);
                                    strlstExpectedValues[idxLine] = strExpectedValue;
                                }
                            }
                        }

                        idxExpected = strExpectedValue.indexOf("(SysTime)");
                        if( idxExpected >= 0 )
                        {
                            QRegExp regExp("\\( {1,}\\d{1,}.\\d{6}\\)");
                            idxBegLogWdgt = regExp.indexIn(strResultValueLogWdgt);
                            idxBegLogFile = regExp.indexIn(strResultValueLogFile);
                            if( idxBegLogWdgt == idxExpected && idxBegLogFile == idxExpected ) {
                                idxEndLogWdgt = strResultValueLogWdgt.indexOf(")", idxBegLogWdgt);
                                idxEndLogFile = strResultValueLogWdgt.indexOf(")", idxBegLogFile);
                                strTmpLogWdgt = strResultValueLogWdgt.mid(idxBegLogWdgt, idxEndLogWdgt - idxBegLogWdgt + 1);
                                strTmpLogFile = strResultValueLogFile.mid(idxBegLogFile, idxEndLogFile - idxBegLogWdgt + 1);
                                if( strTmpLogWdgt == strTmpLogFile ) {
                                    strExpectedValue = strExpectedValue.replace("(SysTime)", strTmpLogWdgt);
                                    strlstExpectedValues[idxLine] = strExpectedValue;
                                }
                            }
                        }
                    }

                    pTestStep->setExpectedValues(strlstExpectedValues);

                    for( int idxLine = 0; idxLine < strlstExpectedValues.size(); ++idxLine )
                    {
                        QString strResultValueLogWdgt = strlstResultValuesLogWdgt[idxLine];
                        QString strResultValueLogFile = strlstResultValuesLogFile[idxLine];

                        if( strResultValueLogWdgt != strResultValueLogFile )
                        {
                            strlstResultValues.append("Entry in log widget different from entry in log file");
                        }
                        else
                        {
                            strlstResultValues.append(strResultValueLogWdgt);
                        }
                    }
                }

                pTestStep->setResultValues(strlstResultValues);

            } // if( iDocLogMthListLineCount == strlstExpectedValues.size() )
        } // if( pWdgtLog != nullptr )
    } // if( pTestStep != nullptr )

} // onLogClientLogWdgtTextItemAdded

//------------------------------------------------------------------------------
void CTest::onLogClientLogSettingsChanged( QObject* /*i_pLogClient*/ )
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStep* pTestStep = getCurrentTestStep();

    if( pTestStep != nullptr )
    {
        if( m_pTmrTestStepTimeout->isActive() )
        {
            m_pTmrTestStepTimeout->stop();
        }

        CIpcLogClient* pLogClient = CApplication::GetInstance()->getLogClient();

        QObject::disconnect(
            /* pObjSender   */ pLogClient,
            /* szSignal     */ SIGNAL(logSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onLogClientLogSettingsChanged(QObject*)) );

        SLogServerSettings logSettings = pLogClient->getLogSettings();

        QStringList strlstExpectedValues = pTestStep->getExpectedValues();

        QString     strResultValue;
        QStringList strlstResultValues;

        for( const QString& strExpectedValue : strlstExpectedValues )
        {
            if( strExpectedValue.startsWith("Enabled:") ) {
                strResultValue = "Enabled: " + bool2Str(logSettings.m_bEnabled);
                strlstResultValues.append(strResultValue);
            }
            else if( strExpectedValue.startsWith("NewLoggersEnabledAsDefault:") ) {
                strResultValue = "NewLoggersEnabledAsDefault: " + bool2Str(logSettings.m_bNewLoggersEnabledAsDefault);
                strlstResultValues.append(strResultValue);
            }
            else if( strExpectedValue.startsWith("NewLoggersDefaultDetailLevel:") ) {
                strResultValue = "NewLoggersDefaultDetailLevel: " + CEnumLogDetailLevel(logSettings.m_eNewLoggersDefaultDetailLevel).toString();
                strlstResultValues.append(strResultValue);
            }
            else if( strExpectedValue.startsWith("LoggerFileAbsFilePath:") ) {
                strResultValue = "LoggerFileAbsFilePath: " + logSettings.m_strLoggerFileAbsFilePath;
                strlstResultValues.append(strResultValue);
            }
            else if( strExpectedValue.startsWith("UseLocalLogFile:") ) {
                strResultValue = "UseLocalLogFile: " + bool2Str(logSettings.m_bUseLocalLogFile);
                strlstResultValues.append(strResultValue);
            }
            else if( strExpectedValue.startsWith("LocalLogFileAbsFilePath:") ) {
                strResultValue = "LocalLogFileAbsFilePath: " + logSettings.m_strLocalLogFileAbsFilePath;
                strlstResultValues.append(strResultValue);
            }
            else if( strExpectedValue.startsWith("LocalLogFileAutoSaveInterval_ms:") ) {
                strResultValue = "LocalLogFileAutoSaveInterval_ms: " + QString::number(logSettings.m_iLocalLogFileAutoSaveInterval_ms);
                strlstResultValues.append(strResultValue);
            }
            else if( strExpectedValue.startsWith("LocalLogFileSubFileCountMax:") ) {
                strResultValue = "LocalLogFileSubFileCountMax: " + QString::number(logSettings.m_iLocalLogFileSubFileCountMax);
                strlstResultValues.append(strResultValue);
            }
            else if( strExpectedValue.startsWith("LocalLogFileSubFileLineCountMax:") ) {
                strResultValue = "LocalLogFileSubFileLineCountMax: " + QString::number(logSettings.m_iLocalLogFileSubFileLineCountMax);
                strlstResultValues.append(strResultValue);
            }
            else if( strExpectedValue.startsWith("LocalLogFileCloseAfterEachWrite:") ) {
                strResultValue = "LocalLogFileCloseAfterEachWrite: " + bool2Str(logSettings.m_bLocalLogFileCloseFileAfterEachWrite);
                strlstResultValues.append(strResultValue);
            }
            else if( strExpectedValue.startsWith("CacheDataIfNotConnected:") ) {
                strResultValue = "CacheDataIfNotConnected: " + bool2Str(logSettings.m_bCacheDataIfNotConnected);
                strlstResultValues.append(strResultValue);
            }
            else if( strExpectedValue.startsWith("CacheDataMaxArrLen:") ) {
                strResultValue = "CacheDataMaxArrLen: " + QString::number(logSettings.m_iCacheDataMaxArrLen);
                strlstResultValues.append(strResultValue);
            }
        }

        pTestStep->setResultValues(strlstResultValues);

    } // if( pTestStep != nullptr )
} // onLogClientLogSettingsChanged

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

        if( pTestStep->getOperation().contains("LogClient::connect", Qt::CaseInsensitive)
         || pTestStep->getOperation().contains("CIpcLogServer::GetLogger", Qt::CaseInsensitive) )
        {
            CIpcLogClient* pLogClient = CApplication::GetInstance()->getLogClient();

            QObject::disconnect(
                /* pObjSender   */ pLogClient,
                /* szSignal     */ SIGNAL(loggerInserted(QObject*, const QString&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onLogClientLoggerInserted(QObject*, const QString&)) );
            QObject::disconnect(
                /* pObjSender   */ pLogClient,
                /* szSignal     */ SIGNAL(loggerChanged(QObject*, const QString&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onLogClientLoggerChanged(QObject*, const QString&)) );
        }
    } // if( pTestStep != nullptr )

} // onTimerTestStepTimeout()

//------------------------------------------------------------------------------
void CTest::onTimerCheckLogClientLogWdgtIsEmptyTimeout()
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStep* pTestStep = getCurrentTestStep();

    if( pTestStep != nullptr )
    {
        CWidgetCentral* pWdgtCentral = CWidgetCentral::GetInstance();
        CWdgtLog* pWdgtLog = pWdgtCentral->getLogWdgt();

        if( pWdgtLog != nullptr )
        {
            QTextEdit* pEdtLog = pWdgtLog->getTextEdit();
            QTextDocument* pDocLog = pEdtLog->document();

            int iDocLogLineCount = pDocLog->lineCount();

            if( iDocLogLineCount > 0 )
            {
                QString strAllLines = pEdtLog->toPlainText();

                if( m_pTmrTestStepTimeout->isActive() )
                {
                    m_pTmrTestStepTimeout->stop();
                }
                if( !strAllLines.isEmpty() )
                {
                    pTestStep->setResultValue("Clients log widget is not empty");
                }
                else
                {
                    pTestStep->setResultValue("");
                }
            }
        }
    } // if( pTestStep != nullptr )

} // onTimerCheckLogClientLogWdgtIsEmptyTimeout()

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
