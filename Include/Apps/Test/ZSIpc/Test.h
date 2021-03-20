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

#ifndef ZSApps_TestIpc_Test_h
#define ZSApps_TestIpc_Test_h

#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStepAdminObj.h"
#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysRequest.h"

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Ipc
{
class CClient;
class CServer;
}

namespace Apps
{
namespace Test
{
namespace Ipc
{
//******************************************************************************
class CTest : public ZS::Test::CTest
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CTest( const QString& i_strTestStepsFileName );
    ~CTest();
public: // instance methods
    void setReqTimeoutInMs( int i_iTimeout_ms );
    int getReqTimeoutInMs() const { return m_iReqTimeout_ms; }
public slots: // test step methods
    void doTestStep__NotYetImplemented__( ZS::Test::CTestStep* i_pTestStep );
public slots: // test step methods (Group Server - Blocking Calls)
    void doTestStepGroupServerBlockingCallsCreateServer( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGroupServerBlockingCallsInitSettings( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGroupServerBlockingCallsStartup( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGroupServerBlockingCallsChangePort( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGroupServerBlockingCallsShutdown( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGroupServerBlockingCallsDeleteServer( ZS::Test::CTestStep* i_pTestStep );
public slots: // test step methods (Group Server - Request Queue)
    void doTestStepGroupServerRequestQueueCreateServer( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGroupServerRequestQueueInitSettings( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGroupServerRequestQueueStartup( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGroupServerRequestQueueChangePort( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGroupServerRequestQueueShutdown( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGroupServerRequestQueueDeleteServer( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void onTestStepGroupServerRequestQueueReqInProgressChanged( ZS::System::SRequestDscr i_reqDscr );
public slots: // test step methods
    //void doTestStepGroupServerRequestQueue( ZS::Test::CTestStepGroup* i_pTestStepGrpParent );
    //void doTestStepGroupClientBlockingCalls( ZS::Test::CTestStepGroup* i_pTestStepGrpParent );
    //void doTestStepGroupClientRequestQueue( ZS::Test::CTestStepGroup* i_pTestStepGrpParent );
    //void doTestStepGroupSendDataTcpBlkTypeE( ZS::Test::CTestStepGroup* i_pTestStepGrpParent );
    //void doTestStepGroupSendDataTcpBlkTypeL( ZS::Test::CTestStepGroup* i_pTestStepGrpParent );
    //void doTestStepGroupSendDataTcpBlkTypeSL( ZS::Test::CTestStepGroup* i_pTestStepGrpParent );
    //void doTestStepGroupSendDataTcpBlkTypeTLV( ZS::Test::CTestStepGroup* i_pTestStepGrpParent );
    //void onTestStepGroupClientRequestQueueReqInProgressChanged( ZS::System::SRequestDscr i_reqDscr );
    //void onTestStepGroupClientRequestQueueReconnectTimeout();
    //void onTestStepGroupSendDataTcpBlkTypeEServerReceivedData( QObject* i_pServer, int i_iSocketId, const QByteArray& i_byteArr );
    //void onTestStepGroupSendDataTcpBlkTypeEClientReceivedData( QObject* i_pServer, const QByteArray& i_byteArr );
    //void onTestStepGroupSendDataTcpBlkTypeEClientDisconnected( QObject* i_pClient );
    //void onTestStepGroupSendDataTcpBlkTypeLServerReceivedData( QObject* i_pServer, int i_iSocketId, const QByteArray& i_byteArr );
    //void onTestStepGroupSendDataTcpBlkTypeLClientReceivedData( QObject* i_pServer, const QByteArray& i_byteArr );
    //void onTestStepGroupSendDataTcpBlkTypeLClientDisconnected( QObject* i_pClient );
    //void onTestStepGroupSendDataTcpBlkTypeSLServerReceivedData( QObject* i_pServer, int i_iSocketId, const QByteArray& i_byteArr );
    //void onTestStepGroupSendDataTcpBlkTypeSLClientReceivedData( QObject* i_pServer, const QByteArray& i_byteArr );
    //void onTestStepGroupSendDataTcpBlkTypeSLClientDisconnected( QObject* i_pClient );
    //void onTestStepGroupSendDataTcpBlkTypeTLVServerReceivedData( QObject* i_pServer, int i_iSocketId, const QByteArray& i_byteArr );
    //void onTestStepGroupSendDataTcpBlkTypeTLVClientReceivedData( QObject* i_pServer, const QByteArray& i_byteArr );
    //void onTestStepGroupSendDataTcpBlkTypeTLVClientDisconnected( QObject* i_pClient );
protected: // auxiliary methods
    QString exception2Str( const ZS::System::CException& i_exc ) const;
protected: // instance members
    int                                 m_iReqTimeout_ms;
    // Ipc Network
    ZS::Ipc::SServerHostSettings        m_serverHostSettingsDefault;
    ZS::Ipc::SServerHostSettings        m_serverHostSettings;
    ZS::Ipc::CServer*                   m_pServer;
    QHash<qint64,ZS::System::CRequest*> m_hshServerRequestQueueReqsInProgress;
    ZS::Ipc::SClientHostSettings        m_clientHostSettingsDefault;
    ZS::Ipc::SClientHostSettings        m_clientHostSettings;
    ZS::Ipc::STimerSettings             m_clientWatchDogSettingsDefault;
    ZS::Ipc::STimerSettings             m_clientWatchDogSettings;
    ZS::Ipc::CClient*                   m_pClient;
    QHash<qint64,ZS::System::CRequest*> m_hshClientRequestQueueReqsInProgress;
    // Method Trace
    ZS::Trace::CTrcAdminObj*            m_pTrcAdminObj;

}; // class CTest

} // namespace Ipc

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpc_Test_h
