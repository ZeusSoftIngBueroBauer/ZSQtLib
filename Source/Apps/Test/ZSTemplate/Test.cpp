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

#include <QtCore/qrandom.h>
#include <QtCore/qthread.h>

#include "Test.h"

#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSys/ZSSysErrLog.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Apps::Test::Template;


/*******************************************************************************
class CTest
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest( const QString& i_strTestStepsFileName ) :
//------------------------------------------------------------------------------
    ZS::Test::CTest(
        /* strName              */ "ZS::Apps::Test::Template",
        /* strTestStepsFileName */ i_strTestStepsFileName,
        /* iTestStepInterval_ms */ 0 )
{
    int idxGroup = 0;

    ZS::Test::CTestStepGroup* pGrp1 = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "Group " + QString::number(++idxGroup),
        /* pGrpParent */ nullptr );

    int idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Test Something",
        /* strOperation    */ "Test Something",
        /* pGrpParent      */ pGrp1,
        /* szDoTestStepFct */ SLOT(doTestStepGrp1Step1(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Test Something",
        /* strOperation    */ "Test Something",
        /* pGrpParent      */ pGrp1,
        /* szDoTestStepFct */ SLOT(doTestStepGrp1Step2(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Test Something",
        /* strOperation    */ "Test Something",
        /* pGrpParent      */ pGrp1,
        /* szDoTestStepFct */ SLOT(doTestStepGrp1Step3(ZS::Test::CTestStep*)) );

    ZS::Test::CTestStepGroup* pGrp2 = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "Group " + QString::number(++idxGroup),
        /* pGrpParent */ nullptr );

    idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Test Something",
        /* strOperation    */ "Test Something",
        /* pGrpParent      */ pGrp2,
        /* szDoTestStepFct */ SLOT(doTestStepGrp2Step1(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Test Something",
        /* strOperation    */ "Test Something",
        /* pGrpParent      */ pGrp2,
        /* szDoTestStepFct */ SLOT(doTestStepGrp2Step2(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Test Something",
        /* strOperation    */ "Test Something",
        /* pGrpParent      */ pGrp2,
        /* szDoTestStepFct */ SLOT(doTestStepGrp2Step3(ZS::Test::CTestStep*)) );

    // Recall test step settings
    //--------------------------

    SErrResultInfo errResultInfo = recall(i_strTestStepsFileName);

    if(errResultInfo.isErrorResult())
    {
        if(CErrLog::GetInstance() != nullptr)
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

} // default ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo = save();

    if(errResultInfo.isErrorResult())
    {
        if(CErrLog::GetInstance() != nullptr)
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

} // dtor

//------------------------------------------------------------------------------
void CTest::doTestStepGrp1Step1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strExpectedValue  = "TestOutput";
    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Do Test Step
    //-------------

    QRandomGenerator randGen;
    unsigned long uTestStepDuration_ms = 100 + randGen.generate() % 900;
    QThread::msleep(uTestStepDuration_ms);

    // Result Values
    //---------------

    strResultValue  = "TestOutput";
    strlstResultValues.append(strResultValue);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepGrp1Step1

//------------------------------------------------------------------------------
void CTest::doTestStepGrp1Step2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strExpectedValue  = "TestOutput";
    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Do Test Step
    //-------------

    QRandomGenerator randGen;
    unsigned long uTestStepDuration_ms = 100 + randGen.generate() % 900;
    QThread::msleep(uTestStepDuration_ms);

    // Result Values
    //---------------

    strResultValue  = "TestOutput";
    strlstResultValues.append(strResultValue);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepGrp1Step2

//------------------------------------------------------------------------------
void CTest::doTestStepGrp1Step3( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strExpectedValue  = "TestOutput";
    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Do Test Step
    //-------------

    QRandomGenerator randGen;
    unsigned long uTestStepDuration_ms = 100 + randGen.generate() % 900;
    QThread::msleep(uTestStepDuration_ms);

    // Result Values
    //---------------

    strResultValue  = "TestOutput";
    strlstResultValues.append(strResultValue);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepGrp1Step3

//------------------------------------------------------------------------------
void CTest::doTestStepGrp2Step1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strExpectedValue  = "TestOutput";
    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Do Test Step
    //-------------

    QRandomGenerator randGen;
    unsigned long uTestStepDuration_ms = 100 + randGen.generate() % 900;
    QThread::msleep(uTestStepDuration_ms);

    // Result Values
    //---------------

    strResultValue  = "TestOutput";
    strlstResultValues.append(strResultValue);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepGrp2Step1

//------------------------------------------------------------------------------
void CTest::doTestStepGrp2Step2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strExpectedValue  = "TestOutput";
    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Do Test Step
    //-------------

    QRandomGenerator randGen;
    unsigned long uTestStepDuration_ms = 100 + randGen.generate() % 900;
    QThread::msleep(uTestStepDuration_ms);

    // Result Values
    //---------------

    strResultValue  = "TestOutputtt";
    strlstResultValues.append(strResultValue);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepGrp2Step2

//------------------------------------------------------------------------------
void CTest::doTestStepGrp2Step3( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    // Expected Values
    //---------------

    strExpectedValue  = "TestOutput";
    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Do Test Step
    //-------------

    QRandomGenerator randGen;
    unsigned long uTestStepDuration_ms = 100 + randGen.generate() % 900;
    QThread::msleep(uTestStepDuration_ms);

    // Result Values
    //---------------

    strResultValue  = "TestOutput";
    strlstResultValues.append(strResultValue);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepGrp2Step3
