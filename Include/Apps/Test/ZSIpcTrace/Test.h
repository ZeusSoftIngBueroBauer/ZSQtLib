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
class CMyClass1;
class CMyClass2;
class CMyClass2Thread;
class CMyClass3;
class CMyClass3Thread;

//******************************************************************************
class CTest : public ZS::Test::CTest
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::IpcTrace"; }
public: // ctors and dtor
    CTest();
    ~CTest();
public slots: // test step methods
    void doTestStepTraceServerStartup( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceServerShutdown( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceClientConnect( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceClientDisconnect( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceMethodCall( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTraceDataFilter( ZS::Test::CTestStep* i_pTestStep );
private slots:
    void onRequestChanged( ZS::System::SRequestDscr i_reqDscr );
    void onZSTraceClientTraceAdminObjInserted( QObject* i_pTrcClient, const QString& i_strKeyInTree );
    void onZSTraceClientTrcMthListWdgtTextItemAdded( const QString& i_strText );
    void onTimerTestStepTimeout();
    void onTimerTestStepTrcMthListWdgtTimeout();
    void onClass1AboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName);
    void onClass2AboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName);
    void onClass2ThreadAboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName);
    void onClass3AboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName);
    void onClass3ThreadAboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName);
private: // instance auxiliary methods
    void splitMethodCallOperation(
        const QString& i_strOperation,
        QString& o_strClassName,
        QString& o_strSubClassName,
        QString& o_strObjName,
        QString& o_strMth,
        QStringList& o_strlstInArgs,
        QString& o_strMthRet ) const;
    QStringList getResultValuesFromTrcMthListWdgt() const;
private: // instance members
    QTimer*                               m_pTmrTestStepTimeout;
    QTimer*                               m_pTmrTestStepTrcMthListWdgtTimeout;
    QHash<qint64, ZS::System::CRequest*>  m_hshReqsInProgress;
    QHash<QString, CMyClass1*>            m_hshpMyClass1InstancesByName;
    QHash<QString, CMyClass2*>            m_hshpMyClass2InstancesByName;
    QHash<QString, CMyClass2Thread*>      m_hshpMyClass2ThreadInstancesByName;
    QHash<QString, CMyClass3*>            m_hshpMyClass3InstancesByName;
    QHash<QString, CMyClass3Thread*>      m_hshpMyClass3ThreadInstancesByName;
    QMultiHash<QString, CMyClass1*>       m_multihshpMyClass1InstancesByName;
    QMultiHash<QString, CMyClass2*>       m_multihshpMyClass2InstancesByName;
    QMultiHash<QString, CMyClass2Thread*> m_multihshpMyClass2ThreadInstancesByName;
    QMultiHash<QString, CMyClass3*>       m_multihshpMyClass3InstancesByName;
    QMultiHash<QString, CMyClass3Thread*> m_multihshpMyClass3ThreadInstancesByName;

}; // class CTest

} // namespace IpcTrace

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcTrace_Test_h
