/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSApps_TestIpcLog_Test_h
#define ZSApps_TestIpcLog_Test_h

#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStepIdxTreeEntry.h"
#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysRequest.h"

class QTimer;

namespace ZS
{
namespace Log
{
class CIpcLogClient;
class CIpcLogServer;
}

namespace Apps
{
namespace Test
{
namespace IpcLog
{
//******************************************************************************
class CTest : public ZS::Test::CTest
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CTest();
    ~CTest();
public slots: // test step methods
    void doTestStepLogServerStartup( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepLogServerShutdown( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepLogClientConnect( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepLogClientDisconnect( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepLogServerGetLogger( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepModifyLogger( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepModifyLogServer( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepModifyLogServerLoggerFile( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepModifyLogServerLocalLogFile( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepLoggerAddLogEntry( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepLoggerAddLogEntryMyThread( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepLogServerAddLogEntry( ZS::Test::CTestStep* i_pTestStep );
private slots:
    void onRequestChanged( ZS::System::SRequestDscr i_reqDscr );
    void onLogClientLoggerInserted( QObject* i_pLogClient, const QString& i_strKeyInTree );
    void onLogClientLoggerChanged( QObject* i_pLogClient, const QString& i_strKeyInTree );
    void onLogClientLogWdgtTextItemAdded( const QString& i_strText );
    void onLogClientLogSettingsChanged( QObject* i_pLogClient );
    void onTimerTestStepTimeout();
    void onTimerCheckLogClientLogWdgtIsEmptyTimeout();
private: // instance auxiliary methods
    void splitMethodCallOperation(
        const QString& i_strOperation,
        QString& o_strClassName,
        QString& o_strSubClassName,
        QString& o_strObjName,
        QString& o_strMth,
        QStringList& o_strlstInArgs,
        QString& o_strMthRet ) const;
private: // instance members
    QTimer*                              m_pTmrTestStepTimeout;
    QTimer*                              m_pTmrCheckLogClientLogWdgtIsEmpty;
    QHash<qint64, ZS::System::CRequest*> m_hshReqsInProgress;

}; // class CTest

} // namespace IpcLog

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcLog_Test_h
