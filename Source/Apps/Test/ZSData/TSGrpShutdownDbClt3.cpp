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

#include "TSGrpShutdownDbClt3.h"
#include "Test.h"

#include "ZSData/ZSDatabaseClient.h"
#include "ZSData/ZSDatabase.h"
#include "ZSData/ZSDataSet.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepAdminObjPool.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Data;
using namespace ZS::Ipc;
using namespace ZS::PhysVal;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::Data;


/*******************************************************************************
class CTSGrpShutdownDbClt3 : public ZS::Test::CTestStepGroup
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTSGrpShutdownDbClt3::CTSGrpShutdownDbClt3( CTest* i_pTest, ZS::Test::CTestStepGroup* i_pTSGrpParent ) :
//------------------------------------------------------------------------------
    ZS::Test::CTestStepGroup(i_pTest, "Shutdown", i_pTSGrpParent)
{
    //new ZS::Test::CTestStep(
    //    /* pTest           */ i_pTest,
    //    /* strName         */ "Unregister",
    //    /* strOperation    */ "CDbClient.unregister",
    //    /* pTSGrpParent    */ this,
    //    /* szDoTestStepFct */ SLOT(doTestStepGrpShutdownDbClt3Unregister(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ i_pTest,
        /* strName         */ "Disconnect",
        /* strOperation    */ "CDbClient.disconnect",
        /* pTSGrpParent    */ this,
        /* szDoTestStepFct */ SLOT(doTestStepGrpShutdownDbClt3Disconnect(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ i_pTest,
        /* strName         */ "ShutdownClient",
        /* strOperation    */ "CDbClient.shutdown",
        /* pTSGrpParent    */ this,
        /* szDoTestStepFct */ SLOT(doTestStepGrpShutdownDbClt3ShutdownClient(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ i_pTest,
        /* strName         */ "DestroyDataSet",
        /* strOperation    */ "delete CDataSet",
        /* pTSGrpParent    */ this,
        /* szDoTestStepFct */ SLOT(doTestStepGrpShutdownDbClt3DestroyDataSet(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ i_pTest,
        /* strName         */ "DestroyClient",
        /* strOperation    */ "delete CDbClient",
        /* pTSGrpParent    */ this,
        /* szDoTestStepFct */ SLOT(doTestStepGrpShutdownDbClt3DestroyClient(ZS::Test::CTestStep*)) );

} // ctor

//------------------------------------------------------------------------------
CTSGrpShutdownDbClt3::~CTSGrpShutdownDbClt3()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
protected: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepGrpShutdownDbClt3Unregister( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "TestStep: " + QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->getName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepGrpShutdownDbClt3Unregister",
        /* strAddInfo   */ strAddTrcInfo );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    // Test Step
    //----------

    // Actual Values
    //---------------

    strActualValue = "__TODO__";
    strlstActualValues.append(strActualValue);

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGrpShutdownDbClt3Unregister

//------------------------------------------------------------------------------
void CTest::doTestStepGrpShutdownDbClt3Disconnect( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "TestStep: " + QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->getName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepGrpShutdownDbClt3Disconnect",
        /* strAddInfo   */ strAddTrcInfo );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    CRequest reqDesired(
        /* strCreatorNameSpace */ CDbClient::NameSpace(),
        /* strCreatorClassName      */ CDbClient::ClassName(),
        /* pObjCreator              */ m_pDbClt3,
        /* request                  */ CDbClient::ERequestDisconnect,
        /* strRequest               */ CDbClient::Request2Str(CDbClient::ERequestDisconnect),
        /* iParentId                */ -1,
        /* bKeepReqDscrInExecTree   */ false,
        /* iTimeout_ms              */ m_iReqTimeout_ms,
        /* bIsBlocking              */ false );
    reqDesired.setProgressInPerCent(100);
    reqDesired.setErrResult(ErrResultSuccess);

    strDesiredValue = "Req {" + reqDesired.getAddTrcInfoStr() + "}";
    strlstDesiredValues.append(strDesiredValue);
    strDesiredValue = "State: " + CDbClient::State2Str(CDbClient::EStateUnconnected);
    strlstDesiredValues.append(strDesiredValue);
    strDesiredValue = m_pDbClt3->objectName() + " in DbCnctList: false";
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    CRequest* pReq = nullptr;

    if( m_pDbClt3 != nullptr )
    {
        try
        {
            pReq = m_pDbClt3->disconnect_(
                /* iTimeout_ms  */ m_iReqTimeout_ms,
                /* bWait        */ false,
                /* iReqIdParent */ -1 );

            pReq->setExecutionData( QString::number(pReq->getId()), i_pTestStep );

            m_hshClientRequestQueueReqsInProgress[pReq->getId()] = pReq;

            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onTestStepGrpShutdownDbClt3RequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
        }
        catch( CException& exc )
        {
            strActualValue = exc.toString();
            strlstActualValues.append(strActualValue);
        }
        catch(...)
        {
            strActualValue = "Critical Error: Unknown Exception Occurred";
            strlstActualValues.append(strActualValue);
        }
    } // if( m_pDbClt3 != nullptr )

    // Actual Values
    //---------------

    // If an exception was thrown ..
    if( !strlstActualValues.isEmpty() )
    {
        strlstActualValues.append(strActualValue);
        i_pTestStep->setActualValues(strlstActualValues);
    }

} // doTestStepGrpShutdownDbClt3Disconnect

//------------------------------------------------------------------------------
void CTest::doTestStepGrpShutdownDbClt3ShutdownClient( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "TestStep: " + QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->getName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepGrpShutdownDbClt3ShutdownClient",
        /* strAddInfo   */ strAddTrcInfo );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    CRequest reqDesired(
        /* strCreatorNameSpace */ CDbClient::NameSpace(),
        /* strCreatorClassName      */ CDbClient::ClassName(),
        /* pObjCreator              */ m_pDbClt3,
        /* request                  */ CDbClient::ERequestShutdown,
        /* strRequest               */ CDbClient::Request2Str(CDbClient::ERequestShutdown),
        /* iParentId                */ -1,
        /* bKeepReqDscrInExecTree   */ false,
        /* iTimeout_ms              */ m_iReqTimeout_ms,
        /* bIsBlocking              */ true );
    reqDesired.setProgressInPerCent(100);
    reqDesired.setErrResult(ErrResultSuccess);

    strDesiredValue = "Req {" + reqDesired.getAddTrcInfoStr() + "}";
    strlstDesiredValues.append(strDesiredValue);
    strDesiredValue = "State: " + CDbClient::State2Str(CDbClient::EStateIdle);
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    CRequest* pReq = nullptr;

    if( m_pDbClt3 != nullptr )
    {
        try
        {
            pReq = m_pDbClt3->shutdown(
                /* iTimeout_ms  */ m_iReqTimeout_ms,
                /* bWait        */ true,
                /* iReqIdParent */ -1 );
        }
        catch( CException& exc )
        {
            strActualValue = exc.toString();
            strlstActualValues.append(strActualValue);
        }
        catch(...)
        {
            strActualValue = "Critical Error: Unknown Exception Occurred";
            strlstActualValues.append(strActualValue);
        }
    } // if( m_pDbClt3 != nullptr )

    // Actual Values
    //---------------

    if( m_pDbClt3 != nullptr )
    {
        strActualValue = "Req {" + QString(pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr()) + "}";
        strlstActualValues.append(strActualValue);
        strActualValue = "State: " + CDbClient::State2Str(m_pDbClt3->getState());
        strlstActualValues.append(strActualValue);

    } // if( m_pDbClt3 != nullptr )

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGrpShutdownDbClt3ShutdownClient

//------------------------------------------------------------------------------
void CTest::doTestStepGrpShutdownDbClt3DestroyDataSet( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "TestStep: " + QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->getName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepGrpShutdownDbClt3DestroyDataSet",
        /* strAddInfo   */ strAddTrcInfo );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strDesiredValue = "DS:nullptr";
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    try
    {
        emit dbClt3DataSetAboutToBeDestroyed(m_pDSDbClt3);

        delete m_pDSDbClt3;
        m_pDSDbClt3 = nullptr;
    }
    catch( CException& exc )
    {
        strActualValue = exc.toString();
        strlstActualValues.append(strActualValue);
    }
    catch(...)
    {
        strActualValue = "Critical Error: Unknown Exception Occurred";
        strlstActualValues.append(strActualValue);
    }

    // Actual Values
    //---------------

    strActualValue = "DS:" + QString(m_pDbClt3->getDataSet() == nullptr ? "nullptr" : m_pDbClt3->getDataSet()->getDSObjKey() );
    strlstActualValues.append(strActualValue);

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGrpShutdownDbClt3DestroyDataSet

//------------------------------------------------------------------------------
void CTest::doTestStepGrpShutdownDbClt3DestroyClient( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "TestStep: " + QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->getName(true));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepGrpShutdownDbClt3DestroyClient",
        /* strAddInfo   */ strAddTrcInfo );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    strDesiredValue = "";
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    try
    {
        emit dbClt3AboutToBeDestroyed(m_pDbClt3);

        delete m_pDbClt3;
        m_pDbClt3 = nullptr;
    }
    catch( CException& exc )
    {
        strActualValue = exc.toString();
        strlstActualValues.append(strActualValue);
    }
    catch(...)
    {
        strActualValue = "Critical Error: Unknown Exception Occurred";
        strlstActualValues.append(strActualValue);
    }
    // Actual Values
    //---------------

    strActualValue = "";
    strlstActualValues.append(strActualValue);

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGrpShutdownDbClt3DestroyClient

/*==============================================================================
protected slots: // future callbacks (GrpStartupDbClt3)
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::onTestStepGrpShutdownDbClt3RequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Req {" + i_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onTestStepGrpShutdownDbClt3RequestChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_hshClientRequestQueueReqsInProgress.contains(i_reqDscr.m_iId) )
    {
        CRequest* pReq = m_hshClientRequestQueueReqsInProgress[i_reqDscr.m_iId];

        ZS::Test::CTestStep* pTestStep = reinterpret_cast<ZS::Test::CTestStep*>(pReq->takeExecutionData(QString::number(i_reqDscr.m_iId)));

        // If the request has been finished ...
        if( i_reqDscr.m_errResultInfo.isErrorResult() || i_reqDscr.m_iProgress_perCent >= 100 )
        {
            QObject::disconnect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onTestStepGrpShutdownDbClt3RequestChanged(ZS::System::SRequestDscr)) );

            m_hshClientRequestQueueReqsInProgress.remove(i_reqDscr.m_iId);

            if( pTestStep != nullptr )
            {
                QString     strActualValue;
                QStringList strlstActualValues;

                if( pTestStep->getName() == "Disconnect" )
                {
                    strActualValue = "Req {" + QString(i_reqDscr.getAddTrcInfoStr()) + "}";
                    strlstActualValues.append(strActualValue);
                    strActualValue = "State: " + CDbClient::State2Str(m_pDbClt3->getState());
                    strlstActualValues.append(strActualValue);

                    m_strlstActualValuesTmp = strlstActualValues;

                    // It may take a while until the server receives the connect indication message from its TCP gateway.
                    m_iTestStepTimeoutSlotInterval_ms = 100;
                    m_iTestStepTimeoutSlotCalled = 0;
                    QTimer::singleShot(m_iTestStepTimeoutSlotInterval_ms, this, SLOT(onTestStepGrpShutdownDbClt3Timeout()));

                } // if( pTestStep->getName() == "Disconnect" )
            } // if( pTestStep != nullptr )
        } // if( i_reqDscr.m_errResultInfo.isErrorResult() || i_reqDscr.m_iProgress_perCent >= 100 )
    } // if( m_requestQueue.isRequestInProgress(i_reqDscr.m_iId) )

} // onTestStepGrpShutdownDbClt3RequestChanged

//------------------------------------------------------------------------------
void CTest::onTestStepGrpShutdownDbClt3Timeout()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onTestStepGrpShutdownDbClt3Timeout",
        /* strAddInfo   */ strAddTrcInfo );

    m_iTestStepTimeoutSlotCalled++;

    if( m_pTestStepCurr != nullptr )
    {
        if( m_pTestStepCurr->getName() == "Disconnect" )
        {
            QList<SSocketDscr> arCltCnctDscrs = m_pDb->getClientCnctDscrs();

            bool bCltInDbCnctList = false;

            for( int idxCnct = 0; idxCnct < arCltCnctDscrs.size(); idxCnct++ )
            {
                if( arCltCnctDscrs[idxCnct].m_socketType == m_socketDscrDbClt3Tmp.m_socketType )
                {
                    if( m_socketDscrDbClt3Tmp.m_socketType == ESocketTypeInProcMsg )
                    {
                        if( arCltCnctDscrs[idxCnct].m_pObjRemote == m_socketDscrDbClt3Tmp.m_pObjLocal )
                        {
                            bCltInDbCnctList = true;
                            break;
                        }
                    }
                    else
                    {
                        if( arCltCnctDscrs[idxCnct].m_uRemotePort == m_socketDscrDbClt3Tmp.m_uLocalPort )
                        {
                            bCltInDbCnctList = true;
                            break;
                        }
                    }
                } // if( arCltCnctDscrs[idxCnct].m_socketType == m_socketDscrDbClt3Tmp.m_socketType )
            } // for( int idxCnct = 0; idxCnct < arCltCnctDscrs.size(); idxCnct++ )

            m_socketDscrDbClt3Tmp = SSocketDscr();

            if( !bCltInDbCnctList || m_iTestStepTimeoutSlotCalled >= 10 )
            {
                QString strActualValue = m_pDbClt3->objectName() + " in DbCnctList: " + bool2Str(bCltInDbCnctList);
                m_strlstActualValuesTmp.append(strActualValue);
                m_pTestStepCurr->setActualValues(m_strlstActualValuesTmp);
                m_strlstActualValuesTmp.clear();
            }
            else if( m_iTestStepTimeoutSlotCalled < 10 )
            {
                // It may take a while until the server receives the connect indication message from its TCP gateway.
                QTimer::singleShot(m_iTestStepTimeoutSlotInterval_ms, this, SLOT(onTestStepGrpShutdownDbClt3Timeout()));
            }
        } // if( m_pTestStepCurr->getName() == "Disconnect" )
    } // if( m_pTestStepCurr != nullptr )

} // onTestStepGrpShutdownDbClt3Timeout
