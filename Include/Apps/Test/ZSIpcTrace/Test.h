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
#include "ZSTest/ZSTestStepAdminObj.h"
#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysRequest.h"

class QTimer;

namespace ZS
{
namespace Trace
{
class CIpcTrcClient;
class CIpcTrcServer;
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
    CTest();
    ~CTest();
signals:
    void trcServerCreated(ZS::Trace::CIpcTrcServer*);
    void trcServerAboutToBeDestroyed(ZS::Trace::CIpcTrcServer*);
    void trcClientCreated(ZS::Trace::CIpcTrcClient*);
    void trcClientAboutToBeDestroyed(ZS::Trace::CIpcTrcClient*);
public: // instance methods
    ZS::Trace::CIpcTrcServer* getTestTrcServer() { return m_pTestTrcServer; }
    ZS::Trace::CIpcTrcClient* getTestTrcClient() { return m_pTestTrcClient; }
    CTestModule1* getTestModule1() const { return m_pTestModule1; }
    CTestModule2* getTestModule2() const { return m_pTestModule2; }
public slots: // test step methods
    void doTestStepTraceServerCreateInstance( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceServerReleaseInstance( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceServerRecallAdminObjs( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceServerSaveAdminObjs( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceServerStartup( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceServerShutdown( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceClientCreate( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceClientDestroy( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceClientConnect( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceClientDisconnect( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepCreateModule1( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepDeleteModule1( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepCreateModule2( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepDeleteModule2( ZS::Test::CTestStep* i_pTestStep );
private slots:
    void onRequestChanged( ZS::System::SRequestDscr i_reqDscr );
    void onZSTraceClientTraceAdminObjInserted( QObject* i_pTrcClient, const QString& i_strKeyInTree );
    void onZSTraceClientTrcMthListWdgtTextItemAdded( const QString& i_strText );
    void onTimerTestStepTimeout();
private: // instance members
    ZS::Trace::CIpcTrcServer*            m_pTestTrcServer;
    ZS::Trace::CIpcTrcClient*            m_pTestTrcClient;
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
