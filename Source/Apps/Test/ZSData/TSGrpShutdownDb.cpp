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

#include "TSGrpShutdownDb.h"
#include "Test.h"

#include "ZSData/ZSDatabase.h"
#include "ZSData/ZSDataSet.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Data;
using namespace ZS::PhysVal;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::Data;


/*******************************************************************************
class CTSGrpShutdownDb : public ZS::Test::CTestStepGroup
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTSGrpShutdownDb::CTSGrpShutdownDb( CTest* i_pTest, ZS::Test::CTestStepGroup* i_pTSGrpParent ) :
//------------------------------------------------------------------------------
    ZS::Test::CTestStepGroup(i_pTest, "Db", i_pTSGrpParent)
{
    new ZS::Test::CTestStep(
        /* pTest           */ i_pTest,
        /* strName         */ "ShutdownDatabase",
        /* strOperation    */ "Db.Shutdown",
        /* pTSGrpParent    */ this,
        /* szDoTestStepFct */ SLOT(doTestStepGrpShutdownDbShutdownDatabase(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ i_pTest,
        /* strName         */ "DestroyDataSet",
        /* strOperation    */ "delete CDataSet",
        /* pTSGrpParent    */ this,
        /* szDoTestStepFct */ SLOT(doTestStepGrpShutdownDbDestroyDataSet(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ i_pTest,
        /* strName         */ "DestroyDatabase",
        /* strOperation    */ "delete CDb",
        /* pTSGrpParent    */ this,
        /* szDoTestStepFct */ SLOT(doTestStepGrpShutdownDbDestroyDatabase(ZS::Test::CTestStep*)) );

} // ctor

//------------------------------------------------------------------------------
CTSGrpShutdownDb::~CTSGrpShutdownDb()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
protected: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepGrpShutdownDbShutdownDatabase( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGrpShutdownDbShutdownDatabase",
        /* strAddInfo   */ strAddTrcInfo );

    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Desired Values
    //---------------

    CRequest reqDesired(
        /* strCreatorNameSpace */ CDb::NameSpace(),
        /* strCreatorClassName      */ CDb::ClassName(),
        /* pObjCreator              */ m_pDb,
        /* request                  */ CDb::ERequestShutdown,
        /* strRequest               */ CDb::Request2Str(CDb::ERequestShutdown),
        /* iParentId                */ -1,
        /* bKeepReqDscrInExecTree   */ false,
        /* iTimeout_ms              */ m_iReqTimeout_ms,
        /* bIsBlocking              */ true );
    reqDesired.setProgressInPerCent(100);
    reqDesired.setErrResult(ErrResultSuccess);

    strDesiredValue = "Req {" + reqDesired.getAddTrcInfoStr() + "}";
    strlstDesiredValues.append(strDesiredValue);
    strDesiredValue = "State: " + CDb::State2Str(CDb::EStateIdle);
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Test Step
    //----------

    CRequest* pReq = nullptr;

    if( m_pDb != nullptr )
    {
        try
        {
            pReq = m_pDb->shutdown(
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
    } // if( m_pDb != nullptr )

    // Actual Values
    //---------------

    if( m_pDb != nullptr )
    {
        strActualValue = "Req {" + QString(pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr()) + "}";
        strlstActualValues.append(strActualValue);
        strActualValue = "State: " + CDb::State2Str(m_pDb->getState());
        strlstActualValues.append(strActualValue);

    } // if( m_pDb != nullptr )

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGrpShutdownDbShutdownDatabase

//------------------------------------------------------------------------------
void CTest::doTestStepGrpShutdownDbDestroyDataSet( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGrpShutdownDbDestroyDataSet",
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
        emit dbDataSetAboutToBeDestroyed(m_pDSDb);

        delete m_pDSDb;
        m_pDSDb = nullptr;
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

    strActualValue = "DS:" + QString(m_pDb->getDataSet() == nullptr ? "nullptr" : m_pDb->getDataSet()->getDSObjKey() );
    strlstActualValues.append(strActualValue);

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGrpShutdownDbDestroyDataSet

//------------------------------------------------------------------------------
void CTest::doTestStepGrpShutdownDbDestroyDatabase( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepGrpShutdownDbDestroyDatabase",
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
        emit dbAboutToBeDestroyed(m_pDb);

        delete m_pDb;
        m_pDb = nullptr;
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

} // doTestStepGrpShutdownDbDestroyDatabase
