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

#ifndef ZSApps_TestPhysVal_Test_h
#define ZSApps_TestPhysVal_Test_h

#include "ZSTest/ZSTest.h"

namespace ZS
{
namespace Apps
{
namespace Test
{
namespace PhysVal
{
//******************************************************************************
class CTest : public ZS::Test::CTest
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CTest( const QString& i_strTestStepsFileName );
    virtual ~CTest();
protected slots:
    void doTestStepFormatValue_1( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepFormatValue_2( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepFormatValue_3( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepFormatValue_4( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepDefaultCtor_1( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepDefaultCtor_2( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepDefaultCtor_3( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepCtorWithUnitGrp_1( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithUnitGrp_2( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithUnitGrp_3( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithUnitGrp_4( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepCopyCtor( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepCtorWithDblVals_1( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithDblVals_2( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithDblVals_3( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepCtorWithDblValsAndRes_1( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithDblValsAndRes_2( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithDblValsAndRes_3( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithDblValsAndRes_4( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithDblValsAndRes_5( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithDblValsAndRes_6( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithDblValsAndRes_7( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithDblValsAndRes_8( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithDblValsAndRes_9( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepCtorWithStrValsConvert2Str_1( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithStrValsConvert2Str_2( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithStrValsConvert2Str_3( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithStrValsConvert2Str_4( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithStrValsConvert2Str_5( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithStrValsConvert2Str_6( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithStrValsConvert2Str_7( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithStrValsConvert2Str_8( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithStrValsConvert2Str_9( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithStrValsConvert2Str_10( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithStrValsConvert2Str_11( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithStrValsConvert2Str_12( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithStrValsConvert2Str_13( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCtorWithStrValsConvert2Str_14( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepOperatorsWithCPhysValOperand_1( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsWithCPhysValOperand_2( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsWithCPhysValOperand_3( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsWithCPhysValOperand_4( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsWithCPhysValOperand_5( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsWithCPhysValOperand_6( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepAssignmentOperator( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepOperatorsAddSubWithCPhysValOperand_1( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsAddSubWithCPhysValOperand_2( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsAddSubWithCPhysValOperand_3( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsAddSubWithCPhysValOperand_4( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsAddSubWithCPhysValOperand_5( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsAddSubWithCPhysValOperand_6( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsAddSubWithCPhysValOperand_7( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsAddSubWithCPhysValOperand_8( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepOperatorsWithDoubleAsOperand_1( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsWithDoubleAsOperand_2( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsWithDoubleAsOperand_3( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsWithDoubleAsOperand_4( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsWithDoubleAsOperand_5( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsWithDoubleAsOperand_6( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsWithDoubleAsOperand_7( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepAssignToDouble( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepOperatorsAddSubWithDoubleOperand_1( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsAddSubWithDoubleOperand_2( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsAddSubWithDoubleOperand_3( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsAddSubWithDoubleOperand_4( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsAddSubWithDoubleOperand_5( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepOperatorsMulDivWithDoubleOperand_1( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsMulDivWithDoubleOperand_2( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsMulDivWithDoubleOperand_3( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsMulDivWithDoubleOperand_4( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepOperatorsWithStringOperand_1( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsWithStringOperand_2( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsWithStringOperand_3( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsWithStringOperand_4( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsWithStringOperand_5( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsWithStringOperand_6( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsWithStringOperand_7( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepAssign2Str( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepOperatorsAddSubWithStringOperand_1( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsAddSubWithStringOperand_2( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsAddSubWithStringOperand_3( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepOperatorsAddSubWithStringOperand_4( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepUnitConversions_1( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepUnitConversions_2( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void doTestStepAccuracyDigits_1( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepAccuracyDigits_2( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepAccuracyDigits_3( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepAccuracyDigits_4( ZS::Test::CTestStep* i_pTestStep );

}; // class CTest

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_Test_h
