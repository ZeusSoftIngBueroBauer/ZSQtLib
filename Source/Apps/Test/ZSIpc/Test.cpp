/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/qtimer.h>
#include <QtNetwork/qhostinfo.h>

#include "Test.h"

#include "ZSTest/ZSTestStepAdminObjPool.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSIpc/ZSIpcClient.h"
#include "ZSIpc/ZSIpcServer.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::Ipc;


/*******************************************************************************
class CTest
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest( const QString& i_strTestStepsFileName ) :
//------------------------------------------------------------------------------
    ZS::Test::CTest("ZS::Ipc", i_strTestStepsFileName),
    m_iReqTimeout_ms(10000),
    // Ipc Network
    // Please note that in the test steps the client host settings will be set to
    // the server host settings. Clients may not connect to the server if using
    // the host name instead of the IP address in the connection string. Especially
    // in virtual private networks. Maybe because a DNS server is not accessible?
    m_serverHostSettingsDefault(
        /* strLocalHostName       */ QHostInfo::localHostName(),
        /* uLocalPort             */ 24763,
        /* uMaxPendingConnections */ 30 ),
    m_serverHostSettings(
        /* strLocalHostName       */ QHostInfo::localHostName(),
        /* uLocalPort             */ 24763,
        /* uMaxPendingConnections */ 30 ),
    m_pServer(nullptr),
    m_hshServerRequestQueueReqsInProgress(),
    m_clientHostSettingsDefault(
        /* strRemoteHostName  */ QHostInfo::localHostName(),
        /* uRemotePort        */ 24763,
        /* iConnectTimeout_ms */ 5000 ),
    m_clientHostSettings(
        /* strRemoteHostName  */ QHostInfo::localHostName(),
        /* uRemotePort        */ 24763,
        /* iConnectTimeout_ms */ 5000 ),
    m_clientWatchDogSettingsDefault(
        /* bEnabled     */ false,
        /* iInterval_ms */ 10000,
        /* iTimeout_ms  */ 5000 ),
    m_clientWatchDogSettings(
        /* bEnabled     */ false,
        /* iInterval_ms */ 10000,
        /* iTimeout_ms  */ 5000 ),
    m_pClient(nullptr),
    //m_arClientHostSettings(),
    //m_arpClients(),
    m_hshClientRequestQueueReqsInProgress(),
    // Method Trace
    m_pTrcAdminObj(nullptr)
{
    // Get trace admin object
    //-----------------------

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::Ipc", "CTest", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    int idxSubGroup;
    int idxStep;

    // Sub-Group: Server
    //==================

    idxSubGroup = 0;

    ZS::Test::CTestStepGroup* pGrpServer = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Server",
        /* pTSGrpParent    */ nullptr );

    // Sub-Group: Server - Blocking Calls
    //===================================

    ZS::Test::CTestStepGroup* pGrpServerBlockingCalls = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Blocking Calls",
        /* pTSGrpParent    */ pGrpServer );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Create Server",
        /* strOperation    */ "new Ipc::CServer()",
        /* pTSGrpParent    */ pGrpServerBlockingCalls,
        /* szDoTestStepFct */ SLOT(doTestStepGroupServerBlockingCallsCreateServer(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Init Settings",
        /* strOperation    */ "Ipc::CServer.changeSettings()",
        /* pTSGrpParent    */ pGrpServerBlockingCalls,
        /* szDoTestStepFct */ SLOT(doTestStepGroupServerBlockingCallsInitSettings(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Startup",
        /* strOperation    */ "Ipc::CServer.startup()",
        /* pTSGrpParent    */ pGrpServerBlockingCalls,
        /* szDoTestStepFct */ SLOT(doTestStepGroupServerBlockingCallsStartup(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Change Port",
        /* strOperation    */ "Ipc::CServer.changeSettings()",
        /* pTSGrpParent    */ pGrpServerBlockingCalls,
        /* szDoTestStepFct */ SLOT(doTestStepGroupServerBlockingCallsChangePort(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Shutdown",
        /* strOperation    */ "Ipc::CServer.shutdown()",
        /* pTSGrpParent    */ pGrpServerBlockingCalls,
        /* szDoTestStepFct */ SLOT(doTestStepGroupServerBlockingCallsShutdown(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Delete Server",
        /* strOperation    */ "delete CIpcServer()",
        /* pTSGrpParent    */ pGrpServerBlockingCalls,
        /* szDoTestStepFct */ SLOT(doTestStepGroupServerBlockingCallsDeleteServer(ZS::Test::CTestStep*)) );

    // Sub-Group: Server - Request Queue
    //==================================

    ZS::Test::CTestStepGroup* pGrpServerRequestQueue = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Request Queue",
        /* pTSGrpParent    */ pGrpServer );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Create Server",
        /* strOperation    */ "new Ipc::CServer()",
        /* pTSGrpParent    */ pGrpServerRequestQueue,
        /* szDoTestStepFct */ SLOT(doTestStepGroupServerRequestQueueCreateServer(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Init Settings",
        /* strOperation    */ "Ipc::CServer.changeSettings()",
        /* pTSGrpParent    */ pGrpServerRequestQueue,
        /* szDoTestStepFct */ SLOT(doTestStepGroupServerRequestQueueInitSettings(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Startup",
        /* strOperation    */ "Ipc::CServer.startup()",
        /* pTSGrpParent    */ pGrpServerRequestQueue,
        /* szDoTestStepFct */ SLOT(doTestStepGroupServerRequestQueueStartup(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Change Port",
        /* strOperation    */ "Ipc::CServer.changeSettings()",
        /* pTSGrpParent    */ pGrpServerRequestQueue,
        /* szDoTestStepFct */ SLOT(doTestStepGroupServerRequestQueueChangePort(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Shutdown",
        /* strOperation    */ "Ipc::CServer.shutdown()",
        /* pTSGrpParent    */ pGrpServerRequestQueue,
        /* szDoTestStepFct */ SLOT(doTestStepGroupServerRequestQueueShutdown(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Delete Server",
        /* strOperation    */ "delete CIpcServer()",
        /* pTSGrpParent    */ pGrpServerRequestQueue,
        /* szDoTestStepFct */ SLOT(doTestStepGroupServerRequestQueueDeleteServer(ZS::Test::CTestStep*)) );

    // Sub-Group: Client
    //==================

    ZS::Test::CTestStepGroup* pGrpClient = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Client",
        /* pTSGrpParent    */ nullptr );

    // Sub-Group: Client - Blocking Calls
    //===================================

    ZS::Test::CTestStepGroup* pGrpClientBlockingCalls = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Blocking Calls",
        /* pTSGrpParent    */ pGrpClient );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Create Server",
        /* strOperation    */ "new CIpcServer()",
        /* pTSGrpParent    */ pGrpClientBlockingCalls,
        /* szDoTestStepFct */ SLOT(doTestStep__NotYetImplemented__(ZS::Test::CTestStep*)) );

    // Sub-Group: Client - Request Queue
    //==================================

    ZS::Test::CTestStepGroup* pGrpClientRequestQueue = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Request Queue",
        /* pTSGrpParent    */ pGrpClient );

    //m_arfctsDoTestStepGroups << FctDoTestStepGroupClientRequestQueue;

    // Sub-Group: Send Data
    //=====================

    ZS::Test::CTestStepGroup* pGrpSendData = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Send Data",
        /* pTSGrpParent    */ nullptr );

    // Sub-Group: Send Data - Tcp
    //===========================

    ZS::Test::CTestStepGroup* pGrpSendDataTcp = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Send Tcp",
        /* pTSGrpParent    */ pGrpSendData );

    // Sub-Group: Send Data - Tcp - Block Type E
    //==========================================

    ZS::Test::CTestStepGroup* pGrpSendDataTcpBlkTypeE = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Block Type E",
        /* pTSGrpParent    */ pGrpSendDataTcp );

    //m_arfctsDoTestStepGroups << FctDoTestStepGroupSendDataTcpBlkTypeE;

    // Sub-Group: Send Data - Tcp - Block Type L
    //==========================================

    ZS::Test::CTestStepGroup* pGrpSendDataTcpBlkTypeL = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Block Type L",
        /* pTSGrpParent    */ pGrpSendDataTcp );

    //m_arfctsDoTestStepGroups << FctDoTestStepGroupSendDataTcpBlkTypeL;

    // Sub-Group: Send Data - Tcp - Block Type SL
    //===========================================

    ZS::Test::CTestStepGroup* pGrpSendDataTcpBlkTypeSL = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Block Type SL",
        /* pTSGrpParent    */ pGrpSendDataTcp );

    //m_arfctsDoTestStepGroups << FctDoTestStepGroupSendDataTcpBlkTypeSL;

    // Sub-Group: Send Data - Tcp - Block Type TLV
    //============================================

    ZS::Test::CTestStepGroup* pGrpSendDataTcpBlkTypeTLV = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++idxSubGroup) + " Block Type TLV",
        /* pTSGrpParent    */ pGrpSendDataTcp );

    //m_arfctsDoTestStepGroups << FctDoTestStepGroupSendDataTcpBlkTypeTLV;

    // Recall test admin object settings
    //----------------------------------

    m_pAdminObjPool->read_(i_strTestStepsFileName);

} // default ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pAdminObjPool->save_();

    delete m_pServer;
    delete m_pClient;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_iReqTimeout_ms = 0;
    // Ipc Network
    //m_serverHostSettingsDefault;
    //m_serverHostSettings;
    m_pServer = nullptr;
    m_hshServerRequestQueueReqsInProgress.clear();
    //m_clientHostSettingsDefault;
    //m_clientHostSettings;
    //m_clientWatchDogSettingsDefault;
    //m_clientWatchDogSettings;
    m_pClient = nullptr;
    // Method Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class ZS::Test::CTest
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::setReqTimeoutInMs( int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setReqTimeoutInMs",
        /* strAddInfo   */ "" );

    m_iReqTimeout_ms = i_iTimeout_ms;
}

//------------------------------------------------------------------------------
void CTest::doTestStep__NotYetImplemented__( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_pTestStep->getName(true);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStep__NotYetImplemented__",
        /* strAddInfo   */ strMthInArgs );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    if( strlstDesiredValues.size() == 0 )
    {
        strlstDesiredValues << "Test Step not implemented";
    }
    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues << "";
    }
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStep__NotYetImplemented__
