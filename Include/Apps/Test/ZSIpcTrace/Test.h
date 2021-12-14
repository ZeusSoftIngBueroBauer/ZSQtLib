/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSApps_TestIpcTrace_Test_h
#define ZSApps_TestIpcTrace_Test_h

#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStepIdxTreeEntry.h"
#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysRequest.h"

class QTimer;

namespace ZS
{
namespace Ipc
{
class CClient;
class CServer;
}

namespace Apps
{
namespace Test
{
namespace IpcTrace
{
class CTestModule1;
class CTestModule2;

//******************************************************************************
class CTest : public ZS::Test::CTest
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CTest( const QString& i_strTestStepsFileName );
    ~CTest();
public: // instance methods
    CTestModule1* getTestModule1() const { return m_pTestModule1; }
    CTestModule2* getTestModule2() const { return m_pTestModule2; }
public slots: // test step methods
    void doTestStepTraceServerStartup( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceServerShutdown( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceServerRecallAdminObjs( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceServerSaveAdminObjs( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceClientConnect( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceClientDisconnect( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCreateModule1( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepDeleteModule1( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCreateModule2( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepDeleteModule2( ZS::Test::CTestStep* i_pTestStep );
private slots:
    void onRequestChanged( ZS::System::SRequestDscr i_reqDscr );
    void onTraceClientTraceAdminObjInserted( QObject* i_pTrcClient, const QString& i_strKeyInTree );
    void onTraceClientTrcMthListWdgtTextItemAdded( const QString& i_strText );
    void onTimerTestStepTimeout();
private: // instance members
    QTimer*                              m_pTmrTestStepTimeout;
    QHash<qint64, ZS::System::CRequest*> m_hshReqsInProgress;
    CTestModule1*                        m_pTestModule1;
    CTestModule2*                        m_pTestModule2;

}; // class CTest

} // namespace IpcTrace

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcTrace_Test_h
