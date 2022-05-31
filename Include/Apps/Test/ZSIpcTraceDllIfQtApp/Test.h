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
namespace IpcTraceDllIfQtApp
{
class CMyClass1;
class CMyClass2;
class CMyClass3;

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
    void doTestStepTraceServerCreateInstance( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceServerReleaseInstance( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceServerStartup( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceServerShutdown( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceServerRecallAdminObjs( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceServerSaveAdminObjs( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceClientConnect( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceClientDisconnect( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceMethodCall( ZS::Test::CTestStep* i_pTestStep );
public slots: // test step methods
    void onTimerTestStepTimeout();
    void onClass1AboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName);
    void onClass2AboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName);
    void onClass3AboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName);
private: // instance auxiliary methods
    void splitMethodCallOperation(
        const QString& i_strOperation,
        QString& o_strClassName,
        QString& o_strSubClassName,
        QString& o_strObjName,
        QString& o_strMth,
        QStringList& o_strlstInArgs,
        QString& o_strMthRet ) const;
protected: // instance members
    ZS::Test::GUI::CDlgTestStep*         m_pDlgTestStep;
    QTimer*                              m_pTmrTestStepTimeout;
    ZS::Trace::DllIf::STrcServerSettings m_trcSettings;
    unsigned short                       m_uTrcServerPort;
    //int                                  m_iTrcDetailLevelTrcServer;
    ZS::Trace::DllIf::CIpcTrcServer*     m_pTrcServer;
    QHash<QString, CMyClass1*>           m_hshpMyClass1InstancesByName;
    QHash<QString, CMyClass2*>           m_hshpMyClass2InstancesByName;
    QHash<QString, CMyClass3*>           m_hshpMyClass3InstancesByName;
    QMultiHash<QString, CMyClass1*>      m_multihshpMyClass1InstancesByName;
    QMultiHash<QString, CMyClass2*>      m_multihshpMyClass2InstancesByName;
    QMultiHash<QString, CMyClass3*>      m_multihshpMyClass3InstancesByName;

}; // class CTest

} // namespace IpcTraceDllIfQtApp

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcTraceDllIfQtApp_Test_h
