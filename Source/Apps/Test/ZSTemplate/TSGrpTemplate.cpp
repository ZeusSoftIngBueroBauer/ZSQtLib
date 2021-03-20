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

#include "TSGrpTemplate.h"
#include "Test.h"

#include "ZSTest/ZSTestStep.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Apps::Test::Template;


/*******************************************************************************
class CTSGrpTemplate : public ZS::Test::CTestStepGroup
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTSGrpTemplate::CTSGrpTemplate( CTest* i_pTest, ZS::Test::CTestStepGroup* i_pTSGrpParent ) :
//------------------------------------------------------------------------------
    ZS::Test::CTestStepGroup(i_pTest, "Template", i_pTSGrpParent)
{
    int idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ i_pTest,
        /* strName         */ "Step " + QString::number(++idxStep) + " Test Something",
        /* strOperation    */ "Test Something",
        /* pTSGrpParent    */ this,
        /* szDoTestStepFct */ SLOT(doTestStepGrpTemplateTestStep1(ZS::Test::CTestStep*)) );

} // ctor

//------------------------------------------------------------------------------
CTSGrpTemplate::~CTSGrpTemplate()
//------------------------------------------------------------------------------
{
} // dtor

//------------------------------------------------------------------------------
void CTest::doTestStepGrpTemplateTestStep1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    // Test Step
    //----------

    // Desired Values
    //---------------

    strDesiredValue  = "TestStep1TestOutput";
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Actual Values
    //---------------

    strActualValue  = "TestStep1TestOutput";
    strlstActualValues.append(strActualValue);

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepGrpTemplateTestStep1
