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

#ifndef ZSApps_TestIpcTraceDllIfQtApp_Test_h
#define ZSApps_TestIpcTraceDllIfQtApp_Test_h

#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStepIdxTreeEntry.h"
#include "ZSIpcTrace/ZSIpcTrcDllIf.h"

class QTimer;

namespace ZS
{
namespace Apps
{
namespace Test
{
namespace IpcTraceDllIfQtApp
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
public slots: // test step methods
    void doTestStepLoadDll( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepReleaseDll( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceServerCreateInstance( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceServerReleaseInstance( ZS::Test::CTestStep* i_pTestStep );
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
public slots: // test step methods
    void onTimerTestStepTimeout();
protected: // instance members
    QTimer*                              m_pTmrTestStepTimeout;
    ZS::Trace::DllIf::STrcServerSettings m_trcSettings;
    unsigned short                       m_uTrcServerPort;
    int                                  m_iTrcDetailLevelTrcServer;
    ZS::Trace::DllIf::CIpcTrcServer*     m_pTrcServer;
    CTestModule1*                        m_pTestModule1;
    CTestModule2*                        m_pTestModule2;

}; // class CTest

} // namespace IpcTraceDllIfQtApp

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcTraceDllIfQtApp_Test_h
