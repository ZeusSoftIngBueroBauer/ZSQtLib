/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSApps_TestIpcLogDllIfQtApp_Test_h
#define ZSApps_TestIpcLogDllIfQtApp_Test_h

#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStepIdxTreeEntry.h"
#include "ZSIpcLog/ZSIpcLogDllIf.h"

class QTimer;

namespace ZS
{
namespace Test
{
namespace GUI
{
class CDlgTestStep;
}
}

namespace Apps
{
namespace Test
{
namespace IpcLogDllIfQtApp
{
class CMyClass1;

//******************************************************************************
class CTest : public ZS::Test::CTest
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CTest();
    ~CTest();
public slots: // test step methods
    void doTestStepLoadDll( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepReleaseDll( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepLogServerCreateInstance( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepLogServerReleaseInstance( ZS::Test::CTestStep* i_pTestStep );
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
    void doTestStepLogMethodCall( ZS::Test::CTestStep* i_pTestStep );
private slots:
    void onTimerTestStepTimeout();
    void onClass1AboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName);
private: // instance auxiliary methods
    void splitMethodCallOperation(
        const QString& i_strOperation,
        QString& o_strClassName,
        QString& o_strSubClassName,
        QString& o_strObjName,
        QString& o_strMth,
        QStringList& o_strlstInArgs,
        QString& o_strMthRet ) const;
    QStringList getExpectedValues( ZS::Log::DllIf::CLogger* i_pLogger ) const;
private: // instance members
    ZS::Test::GUI::CDlgTestStep*       m_pDlgTestStep;
    QTimer*                            m_pTmrTestStepTimeout;
    ZS::Log::DllIf::SLogServerSettings m_logSettings;
    unsigned short                     m_uLogServerPort;
    QHash<QString, CMyClass1*>         m_hshpMyClass1InstancesByName;

}; // class CTest

} // namespace IpcLogDllIfQtApp

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcLogDllIfQtApp_Test_h
