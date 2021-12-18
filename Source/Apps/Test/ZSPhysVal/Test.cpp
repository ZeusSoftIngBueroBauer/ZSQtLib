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

#include <QtCore/qfileinfo.h>

#include "Test.h"

#include "ZSPhysSizes/Electricity/ZSPhysSizes.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSPhysSizes/Kinematics/ZSPhysSizes.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysAux.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CTest
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest( const QString& i_strTestStepsFileName ) :
//------------------------------------------------------------------------------
    ZS::Test::CTest( "ZS::PhysVal", i_strTestStepsFileName )
{
    ZS::Test::CTestStep* pTestStep;
    QString              strResultValue;

    int idxStep = 0;

    ZS::Test::CTestStepGroup* pTSGrp = new ZS::Test::CTestStepGroup( this, "Physical Values", nullptr );

    //==========================================================================
    // formatValue methods
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpFormatValue = new ZS::Test::CTestStepGroup( this, "formatValue", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "formatValue(123.45 mW, Res=0.02 mW, Round2Res)",
        /* pGrpParent      */ pTSGrpFormatValue,
        /* szDoTestStepFct */ SLOT(doTestStepFormatValue_1(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Rounding to resolution" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "formatValue(123.45 mW, Res=0.02 mW, Overflow)",
        /* pGrpParent      */ pTSGrpFormatValue,
        /* szDoTestStepFct */ SLOT(doTestStepFormatValue_2(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Not enough exponent digits for engineering format" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "formatValue(123.45 mW, Res=0.02 mW, Eng.Format 1.Dig.Exp.)",
        /* pGrpParent      */ pTSGrpFormatValue,
        /* szDoTestStepFct */ SLOT(doTestStepFormatValue_3(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Engineering format" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "formatValue(123.45 mW, Res=0.02 mW 2.Dig.Exp.)",
        /* pGrpParent      */ pTSGrpFormatValue,
        /* szDoTestStepFct */ SLOT(doTestStepFormatValue_4(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Engineering format" );

    //==========================================================================
    // Default constructor and conversion to string
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpDefaultCtor = new ZS::Test::CTestStepGroup( this, "Default ctor and convert2Str", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal().toString()",
        /* pGrpParent      */ pTSGrpDefaultCtor,
        /* szDoTestStepFct */ SLOT(doTestStepDefaultCtor_1(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Default constructor creates invalid value" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal().setVal(45.0)",
        /* pGrpParent      */ pTSGrpDefaultCtor,
        /* szDoTestStepFct */ SLOT(doTestStepDefaultCtor_2(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Value without unit" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(Kinematics.Frequency) = 20k",
        /* pGrpParent      */ pTSGrpDefaultCtor,
        /* szDoTestStepFct */ SLOT(doTestStepDefaultCtor_3(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "String with Factor" );

    //==========================================================================
    // Constructor with unit group or unit creates invalid value
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpCtorWithUnitGroup = new ZS::Test::CTestStepGroup( this, "ctor with Unit Group", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal( Kinematics.Time ).toString()",
        /* pGrpParent      */ pTSGrpCtorWithUnitGroup,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithUnitGrp_1(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with unit group but no value creates invalid value" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal( Kinematics.Time ).setVal(12.0,h)",
        /* pGrpParent      */ pTSGrpCtorWithUnitGroup,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithUnitGrp_2(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Value with unit" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal( Kinematics.Time ).setVal(30.0)",
        /* pGrpParent      */ pTSGrpCtorWithUnitGroup,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithUnitGrp_3(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with unit group uses SI unit as default" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal( Kinematics.Time.Hours ).setVal(12.0)",
        /* pGrpParent      */ pTSGrpCtorWithUnitGroup,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithUnitGrp_4(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with unit, value set later" );

    //==========================================================================
    // Copy constructor
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpCopyCtor = new ZS::Test::CTestStepGroup( this, "Copy ctor", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal( physVal(3.456789e3,Kinematics.Frequency.MHz,100.0,kHz) )",
        /* pGrpParent      */ pTSGrpCopyCtor,
        /* szDoTestStepFct */ SLOT(doTestStepCopyCtor(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Copy constructor" );

    //==========================================================================
    // Constructors with double values
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpCtorWithDoubleValues = new ZS::Test::CTestStepGroup( this, "ctor with Double Values", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(1.0).toString()",
        /* pGrpParent      */ pTSGrpCtorWithDoubleValues,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithDblVals_1(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with double value, one trailing digit" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(10.0,%).convertValue(MHz)",
        /* pGrpParent      */ pTSGrpCtorWithDoubleValues,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithDblVals_2(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Conversion not possible" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(10.0,%).convertValue(%%)",
        /* pGrpParent      */ pTSGrpCtorWithDoubleValues,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithDblVals_3(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Conversion Ok" );

    //==========================================================================
    // Constructors with double value and resolutions
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpCtorWithDoubleValueAndRes = new ZS::Test::CTestStepGroup( this, "ctor with Double Values and Resolution", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(1.0,0.001).toString()",
        /* pGrpParent      */ pTSGrpCtorWithDoubleValueAndRes,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithDblValsAndRes_1(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with double value and resolution" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(1.0,%,0.5).toString()",
        /* pGrpParent      */ pTSGrpCtorWithDoubleValueAndRes,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithDblValsAndRes_2(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with double value and resolution" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(1.2345678,%,5.0,%%).toString()",
        /* pGrpParent      */ pTSGrpCtorWithDoubleValueAndRes,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithDblValsAndRes_3(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with double value and resolution, both as ratio values" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(1.2345678,%,CPhysValRes(0.5,%%)).toString()",
        /* pGrpParent      */ pTSGrpCtorWithDoubleValueAndRes,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithDblValsAndRes_4(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with double value and resolution, both as ratio values" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(0.001234,dBm,0.0001).toString()",
        /* pGrpParent      */ pTSGrpCtorWithDoubleValueAndRes,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithDblValsAndRes_5(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with double value and resolution" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(12.34567,km,2.0,%).toString()",
        /* pGrpParent      */ pTSGrpCtorWithDoubleValueAndRes,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithDblValsAndRes_6(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with double value and resolution" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(12.34567,km,2.0,%).toString(WithRes)",
        /* pGrpParent      */ pTSGrpCtorWithDoubleValueAndRes,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithDblValsAndRes_7(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with double value and resolution" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(12.34567,km,25.0,m).toString()",
        /* pGrpParent      */ pTSGrpCtorWithDoubleValueAndRes,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithDblValsAndRes_8(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with double value and resolution" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(12.34567,km,CPhysValRes(100.0,m).toString()",
        /* pGrpParent      */ pTSGrpCtorWithDoubleValueAndRes,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithDblValsAndRes_9(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with double value and resolution" );

    //==========================================================================
    // Constructors with string value and conversion into strings
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpWithStringValuesConvert2Str = new ZS::Test::CTestStepGroup( this, "ctors with String Values convert2Str", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(\"0.0\")",
        /* pGrpParent      */ pTSGrpWithStringValuesConvert2Str,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithStrValsConvert2Str_1(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with string value" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(\"1.2e-3\")",
        /* pGrpParent      */ pTSGrpWithStringValuesConvert2Str,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithStrValsConvert2Str_2(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with string value" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(\"1.234\")",
        /* pGrpParent      */ pTSGrpWithStringValuesConvert2Str,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithStrValsConvert2Str_3(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with string value" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(\"(1.234 " + c_strSymbolPlusMinus + " 0.56) Kinematics.Time.s\")",
        /* pGrpParent      */ pTSGrpWithStringValuesConvert2Str,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithStrValsConvert2Str_4(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with string value containing resolution" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(\"(1.234 " + c_strSymbolPlusMinus + " 0.26) Kinematics.Time.s\")",
        /* pGrpParent      */ pTSGrpWithStringValuesConvert2Str,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithStrValsConvert2Str_5(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with string value containing resolution" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(\"1.234 Geometry.km " + c_strSymbolPlusMinus + " 56 m\")",
        /* pGrpParent      */ pTSGrpWithStringValuesConvert2Str,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithStrValsConvert2Str_6(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with string value containing resolution" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(\"0.02 s\",Kinematics.Time)",
        /* pGrpParent      */ pTSGrpWithStringValuesConvert2Str,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithStrValsConvert2Str_7(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with string value" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(\"(345.6789 " + c_strSymbolPlusMinus + " 0.003) Geometry.Length.km)\")",
        /* pGrpParent      */ pTSGrpWithStringValuesConvert2Str,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithStrValsConvert2Str_8(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with string value with format error (superfluous closing \")\" at end of string)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(\"(345.6789 " + c_strSymbolPlusMinus + " 0.003) Geometry.Length.km\")",
        /* pGrpParent      */ pTSGrpWithStringValuesConvert2Str,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithStrValsConvert2Str_9(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with string value containing resolution" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(\"(45 " + c_strSymbolPlusMinus + " 2) %\",Ratio)",
        /* pGrpParent      */ pTSGrpWithStringValuesConvert2Str,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithStrValsConvert2Str_10(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with string value containing resolution in %" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(\"8.9 " + QString::fromLatin1("µ") + "V" + c_strSymbolPlusMinus + " 0.05 %\",Electricity.Voltage,Ratio)",
        /* pGrpParent      */ pTSGrpWithStringValuesConvert2Str,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithStrValsConvert2Str_11(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with string value containing resolution in %" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(\"8.9 " + QString::fromLatin1("µ") + "V" + c_strSymbolPlusMinus + " 0.05 %\",Electricity.Voltage,Ratio,OutResWithUnit)",
        /* pGrpParent      */ pTSGrpWithStringValuesConvert2Str,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithStrValsConvert2Str_12(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with string value containing resolution in %" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(\"8.9 " + QString::fromLatin1("µ") + "V" + c_strSymbolPlusMinus + " 0.05 %\",Electricity.Voltage,Ratio).toString()",
        /* pGrpParent      */ pTSGrpWithStringValuesConvert2Str,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithStrValsConvert2Str_13(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Constructor with string value containing resolution in %" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(\"8.9 mW" + c_strSymbolPlusMinus + " 4.5 dBm\",Electricity.Power).toString()",
        /* pGrpParent      */ pTSGrpWithStringValuesConvert2Str,
        /* szDoTestStepFct */ SLOT(doTestStepCtorWithStrValsConvert2Str_14(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Mixing linear and logarithmic units for value and resolution not allowed" );

    //==========================================================================
    // Operators (equals, greater and less than) with CPhysVal as operand
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpOperatorsWithCPhysValOperand = new ZS::Test::CTestStepGroup( this, "Operators with CPhysVal Operand", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(8.9,V) == CPhysVal(8.9e3,mV)",
        /* pGrpParent      */ pTSGrpOperatorsWithCPhysValOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithCPhysValOperand_1(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator ==" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(8.9e3,V) != CPhysVal(8.9e3,mV)",
        /* pGrpParent      */ pTSGrpOperatorsWithCPhysValOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithCPhysValOperand_2(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator !=" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(8.9e3,V) < CPhysVal(8.9e3,mV)",
        /* pGrpParent      */ pTSGrpOperatorsWithCPhysValOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithCPhysValOperand_3(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator <" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(8.9e3,V) > CPhysVal(8.9e3,mV)",
        /* pGrpParent      */ pTSGrpOperatorsWithCPhysValOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithCPhysValOperand_4(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator >" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(8.9e3,V) <= CPhysVal(8.9e3,mV)",
        /* pGrpParent      */ pTSGrpOperatorsWithCPhysValOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithCPhysValOperand_5(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator <=" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(8.9e3,V) >= CPhysVal(8.9e3,mV)",
        /* pGrpParent      */ pTSGrpOperatorsWithCPhysValOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithCPhysValOperand_6(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator >=" );

    //==========================================================================
    // Operators (assignment) to another CPhysVal
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpAssignmentOperator = new ZS::Test::CTestStepGroup( this, "Assignment Operator", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(Ratio) = CPhysVal(8.9e3,mV)",
        /* pGrpParent      */ pTSGrpAssignmentOperator,
        /* szDoTestStepFct */ SLOT(doTestStepAssignmentOperator(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator =" );

    //==========================================================================
    // Operators (+ and -) with CPhysVal as operand
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpOperatorsAddSubWithCPhysValOperand = new ZS::Test::CTestStepGroup( this, "Operators + - with CPhysVal Operand", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal = physVal(7.5,dBm) + physVal(0.027,W)",
        /* pGrpParent      */ pTSGrpOperatorsAddSubWithCPhysValOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsAddSubWithCPhysValOperand_1(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator + (logarithmic and linear values may not be added)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal = physVal(7.5,dBm) - physVal(0.027,W)",
        /* pGrpParent      */ pTSGrpOperatorsAddSubWithCPhysValOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsAddSubWithCPhysValOperand_2(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator + (logarithmic and linear values may not be added)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal = physVal(0.027,W) + physVal(7.5,dBm)",
        /* pGrpParent      */ pTSGrpOperatorsAddSubWithCPhysValOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsAddSubWithCPhysValOperand_3(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator + (logarithmic value added to linear value results in multiplication)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal = physVal(0.027,W) - physVal(7.5,dBm)",
        /* pGrpParent      */ pTSGrpOperatorsAddSubWithCPhysValOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsAddSubWithCPhysValOperand_4(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator + (logarithmic value subtracted from linear value results in division)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal = physVal(0.027,W,0.2,mW) + physVal(10.0,dBm)",
        /* pGrpParent      */ pTSGrpOperatorsAddSubWithCPhysValOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsAddSubWithCPhysValOperand_5(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator + (logarithmic value added to linear value results in multiplication)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal = physVal(0.027,W,0.2,mW) - physVal(10.0,dBm)",
        /* pGrpParent      */ pTSGrpOperatorsAddSubWithCPhysValOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsAddSubWithCPhysValOperand_6(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator + (logarithmic value subtracted from linear value results in division)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal = physVal(0.027,W,0.2,mW) + physVal(0.027,W,0.2,mW)",
        /* pGrpParent      */ pTSGrpOperatorsAddSubWithCPhysValOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsAddSubWithCPhysValOperand_7(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator + (adding two values with resolutions)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal = physVal(0.027,W,0.2,mW) + physVal(0.0135,W,0.2,mW)",
        /* pGrpParent      */ pTSGrpOperatorsAddSubWithCPhysValOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsAddSubWithCPhysValOperand_8(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator + (subtracting two values with resolutions)" );

    //==========================================================================
    // Operators (equals, greater and less than) with double as operand
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpOperatorsWithDoubleAsOperand = new ZS::Test::CTestStepGroup( this, "Operators with Double Operand", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(-23.45e-3) == -0.02345",
        /* pGrpParent      */ pTSGrpOperatorsWithDoubleAsOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithDoubleAsOperand_1(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator == (comparing value without unit with double value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(-23.45e-3,s) == -0.02345",
        /* pGrpParent      */ pTSGrpOperatorsWithDoubleAsOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithDoubleAsOperand_2(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator == (comparing value with unit with double value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(-23.45e-3) != -0.02345",
        /* pGrpParent      */ pTSGrpOperatorsWithDoubleAsOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithDoubleAsOperand_3(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator != (comparing value without unit with double value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(-23.45e-3) < -0.02345",
        /* pGrpParent      */ pTSGrpOperatorsWithDoubleAsOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithDoubleAsOperand_4(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator < (comparing value without unit with double value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(-23.45e-3) > -0.02345",
        /* pGrpParent      */ pTSGrpOperatorsWithDoubleAsOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithDoubleAsOperand_5(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator > (comparing value without unit with double value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(-23.45e-3) <= -0.02345",
        /* pGrpParent      */ pTSGrpOperatorsWithDoubleAsOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithDoubleAsOperand_6(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator <= (comparing value without unit with double value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(-23.45e-3) >= -0.02345",
        /* pGrpParent      */ pTSGrpOperatorsWithDoubleAsOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithDoubleAsOperand_7(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator >= (comparing value without unit with double value)" );

    //==========================================================================
    // Operator (assignment) to another double
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpAssignToDouble = new ZS::Test::CTestStepGroup( this, "Assign to Double", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(mW) = 0.0135",
        /* pGrpParent      */ pTSGrpAssignToDouble,
        /* szDoTestStepFct */ SLOT(doTestStepAssignToDouble(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator = (to another double value)" );

    //==========================================================================
    // Operators (+ and -) with double as operand
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpOperatorsAddSubWithDoubleOperand = new ZS::Test::CTestStepGroup( this, "Opeators + - with Double Operand", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal = physVal(0.027,W,0.2,mW) + 0.1",
        /* pGrpParent      */ pTSGrpOperatorsAddSubWithDoubleOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsAddSubWithDoubleOperand_1(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator + (adding double value to physical value with unit not allowed)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal = physVal(0.027,0.0002) + 0.1",
        /* pGrpParent      */ pTSGrpOperatorsAddSubWithDoubleOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsAddSubWithDoubleOperand_2(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator + (adding double value to physical value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(0.027,0.0003) += 0.1",
        /* pGrpParent      */ pTSGrpOperatorsAddSubWithDoubleOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsAddSubWithDoubleOperand_3(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator += (adding double value to physical value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal = physVal(0.027,0.002) - 0.1",
        /* pGrpParent      */ pTSGrpOperatorsAddSubWithDoubleOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsAddSubWithDoubleOperand_4(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator - (subtracting double value from physical value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(0.027,0.003) -= 0.1",
        /* pGrpParent      */ pTSGrpOperatorsAddSubWithDoubleOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsAddSubWithDoubleOperand_5(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator += (subtracting double value from physical value)" );

    //==========================================================================
    // Operators (* and /) with double as operand
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpOperatorsMulDivWithDoubleOperand = new ZS::Test::CTestStepGroup( this, "Operators Mul and Div with Double Operand", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal = physVal(0.027,W,0.2,mW) * 5.0",
        /* pGrpParent      */ pTSGrpOperatorsMulDivWithDoubleOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsMulDivWithDoubleOperand_1(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator * (multiplying physical value with resolution with double value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(0.027,W,0.2,mW) *= 5.0",
        /* pGrpParent      */ pTSGrpOperatorsMulDivWithDoubleOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsMulDivWithDoubleOperand_2(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator * (multiplying physical value with resolution with double value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal = physVal(0.027,W,0.2,mW) / 5.0",
        /* pGrpParent      */ pTSGrpOperatorsMulDivWithDoubleOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsMulDivWithDoubleOperand_3(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator / (dividing physical value with resolution by double value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal = physVal(0.027,W,0.2,mW) /= 5.0",
        /* pGrpParent      */ pTSGrpOperatorsMulDivWithDoubleOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsMulDivWithDoubleOperand_4(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator / (dividing physical value with resolution by double value)" );

    //==========================================================================
    // Operators (equals, greater and less than) with string as operand
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpOperatorsWithStringOperand = new ZS::Test::CTestStepGroup( this, "Operators with String Operand", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(-23.45e-3) == \"-0.02345\"",
        /* pGrpParent      */ pTSGrpOperatorsWithStringOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithStringOperand_1(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator == (comparing value without unit with string value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(-23.45e-3,W) == \"-0.02345 W\"",
        /* pGrpParent      */ pTSGrpOperatorsWithStringOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithStringOperand_2(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator == (comparing value with unit with string value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(-23.45e-3,W) != \"-0.02345 W\"",
        /* pGrpParent      */ pTSGrpOperatorsWithStringOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithStringOperand_3(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator != (comparing value with unit with string value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(-23.45e-3) < \"-0.02345\"",
        /* pGrpParent      */ pTSGrpOperatorsWithStringOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithStringOperand_4(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator < (comparing value without unit with string value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(-23.45e-3) > \"-0.02345\"",
        /* pGrpParent      */ pTSGrpOperatorsWithStringOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithStringOperand_5(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator > (comparing value without unit with string value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(-23.45e-3) <= \"-0.02345\"",
        /* pGrpParent      */ pTSGrpOperatorsWithStringOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithStringOperand_6(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator <= (comparing value without unit with string value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(-23.45e-3) >= \"-0.02345\"",
        /* pGrpParent      */ pTSGrpOperatorsWithStringOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsWithStringOperand_7(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator >= (comparing value without unit with string value)" );

    //==========================================================================
    // Operator (assignment) to another string
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpAssign2Str = new ZS::Test::CTestStepGroup( this, "Assign to String", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal = 0.0135 W " + c_strSymbolPlusMinus + " 0.2 mW",
        /* pGrpParent      */ pTSGrpAssign2Str,
        /* szDoTestStepFct */ SLOT(doTestStepAssign2Str(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator = (assignment to string value)" );

    //==========================================================================
    // Operators (+ and -) with string as operand
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpOperatorsAddSubWithStringOperand = new ZS::Test::CTestStepGroup( this, "Operators + - with String Operand", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal = physVal(0.027,W,0.2,mW) + \"(0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W)\"",
        /* pGrpParent      */ pTSGrpOperatorsAddSubWithStringOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsAddSubWithStringOperand_1(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator + (adding string value to physical value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(0.027,W,0.2,mW) += \"(0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W)\"",
        /* pGrpParent      */ pTSGrpOperatorsAddSubWithStringOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsAddSubWithStringOperand_2(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator += (adding string value to physical value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal = physVal(0.027,W,0.2,mW) - \"(0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W)\"",
        /* pGrpParent      */ pTSGrpOperatorsAddSubWithStringOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsAddSubWithStringOperand_3(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator - (subtracting string value from physical value)" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "physVal(0.027,W,0.2,mW) -= \"(0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W)\"",
        /* pGrpParent      */ pTSGrpOperatorsAddSubWithStringOperand,
        /* szDoTestStepFct */ SLOT(doTestStepOperatorsAddSubWithStringOperand_4(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "operator += (subtracting string value from physical value)" );

    //==========================================================================
    // Unit conversions
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpUnitConversions = new ZS::Test::CTestStepGroup( this, "Unit Conversions", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(0.0,dBm).convertValue(mW)",
        /* pGrpParent      */ pTSGrpUnitConversions,
        /* szDoTestStepFct */ SLOT(doTestStepUnitConversions_1(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Conversion from logarithmic unit" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(100.0,mW).convertValue(dBm)",
        /* pGrpParent      */ pTSGrpUnitConversions,
        /* szDoTestStepFct */ SLOT(doTestStepUnitConversions_2(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Conversion into logarithmic unit" );

    //==========================================================================
    // Accuracy digits
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpAccuracyDigits = new ZS::Test::CTestStepGroup( this, "Accuracy Digits", pTSGrp );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(25.89453,km,0.12).toString()",
        /* pGrpParent      */ pTSGrpAccuracyDigits,
        /* szDoTestStepFct */ SLOT(doTestStepAccuracyDigits_1(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Accuracy digits" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(25.89453,km,0.4).toString()",
        /* pGrpParent      */ pTSGrpAccuracyDigits,
        /* szDoTestStepFct */ SLOT(doTestStepAccuracyDigits_2(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Accuracy digits" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(9876543210.12,Hz,0.05,kHz).toString()",
        /* pGrpParent      */ pTSGrpAccuracyDigits,
        /* szDoTestStepFct */ SLOT(doTestStepAccuracyDigits_3(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Accuracy digits and best unit" );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "CPhysVal(0.000009876543,GHz,120.0,Hz).toString()",
        /* pGrpParent      */ pTSGrpAccuracyDigits,
        /* szDoTestStepFct */ SLOT(doTestStepAccuracyDigits_4(ZS::Test::CTestStep*)) );
    pTestStep->setDescription( "Accuracy digits and best unit" );

    // Recall test step settings
    //--------------------------

    QFileInfo fileInfo(i_strTestStepsFileName);

    if( fileInfo.exists() )
    {
        SErrResultInfo errResultInfo = recall(i_strTestStepsFileName);

        if(errResultInfo.isErrorResult())
        {
            if(CErrLog::GetInstance() != nullptr)
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
    }

} // ctor

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

/*==============================================================================
protected slots:
==============================================================================*/

//==========================================================================
// formatValue methods
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepFormatValue_1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "Ok: 123.46 mW (123.46,3,2,0)" );

    try
    {
        double  fValResult = 0.0;
        QString strValResult;
        CUnit*  pUnitValResult = nullptr;
        int     iDigitsLeadingResult = 0;
        int     iDigitsTrailingResult = 0;
        int     iDigitsExponentResult = 0;

        EFormatResult formatResult = formatValue(
            /* fVal                */ 123.45,
            /* pUnitVal            */ Electricity::Power()->MilliWatt(),
            /* fRes                */ 0.02,
            /* pUnitRes            */ nullptr,
            /* resType             */ EResTypeResolution,
            /* iDigitsMantissa     */ 0,
            /* iDigitsExponent     */ 0,
            /* bUseEngineeringForm */ false,
            /* pfVal               */ &fValResult,
            /* pstrVal             */ &strValResult,
            /* ppUnitVal           */ &pUnitValResult,
            /* piDigitsLeading     */ &iDigitsLeadingResult,
            /* piDigitsTrailing    */ &iDigitsTrailingResult,
            /* piDigitsExponent    */ &iDigitsExponentResult );

        QString strResultValue = formatResult2Str(formatResult) + ": "
            + strValResult + " " + pUnitValResult->getSymbol()
            + " (" + QString::number(fValResult) + "," + QString::number(iDigitsLeadingResult)
            + "," + QString::number(iDigitsTrailingResult) + "," + QString::number(iDigitsExponentResult) + ")";
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepFormatValue_1

//------------------------------------------------------------------------------
void CTest::doTestStepFormatValue_2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "ValueOverflow: --- mW (123.46,0,0,1)" );

    try
    {
        double  fValResult = 0.0;
        QString strValResult;
        CUnit*  pUnitValResult = nullptr;
        int     iDigitsLeadingResult = 0;
        int     iDigitsTrailingResult = 0;
        int     iDigitsExponentResult = 0;

        EFormatResult formatResult = formatValue(
            /* fVal                */ 123.45,
            /* pUnitVal            */ Electricity::Power()->MilliWatt(),
            /* fRes                */ 0.02,
            /* pUnitRes            */ nullptr,
            /* resType             */ EResTypeResolution,
            /* iDigitsMantissa     */ 0,
            /* iDigitsExponent     */ 0,
            /* bUseEngineeringForm */ true,
            /* pfVal               */ &fValResult,
            /* pstrVal             */ &strValResult,
            /* ppUnitVal           */ &pUnitValResult,
            /* piDigitsLeading     */ &iDigitsLeadingResult,
            /* piDigitsTrailing    */ &iDigitsTrailingResult,
            /* piDigitsExponent    */ &iDigitsExponentResult );

        QString strResultValue = formatResult2Str(formatResult) + ": "
            + strValResult + " " + pUnitValResult->getSymbol()
            + " (" + QString::number(fValResult) + "," + QString::number(iDigitsLeadingResult)
            + "," + QString::number(iDigitsTrailingResult) + "," + QString::number(iDigitsExponentResult) + ")";
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepFormatValue_2

//------------------------------------------------------------------------------
void CTest::doTestStepFormatValue_3( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "Ok: 1.2346e+2 mW (123.46,1,4,1)" );

    try
    {
        double  fValResult = 0.0;
        QString strValResult;
        CUnit*  pUnitValResult = nullptr;
        int     iDigitsLeadingResult = 0;
        int     iDigitsTrailingResult = 0;
        int     iDigitsExponentResult = 0;

        EFormatResult formatResult = formatValue(
            /* fVal                */ 123.45,
            /* pUnitVal            */ Electricity::Power()->MilliWatt(),
            /* fRes                */ 0.02,
            /* pUnitRes            */ nullptr,
            /* resType             */ EResTypeResolution,
            /* iDigitsMantissa     */ 0,
            /* iDigitsExponent     */ 1,
            /* bUseEngineeringForm */ true,
            /* pfVal               */ &fValResult,
            /* pstrVal             */ &strValResult,
            /* ppUnitVal           */ &pUnitValResult,
            /* piDigitsLeading     */ &iDigitsLeadingResult,
            /* piDigitsTrailing    */ &iDigitsTrailingResult,
            /* piDigitsExponent    */ &iDigitsExponentResult );

        QString strResultValue = formatResult2Str(formatResult) + ": "
            + strValResult + " " + pUnitValResult->getSymbol()
            + " (" + QString::number(fValResult) + "," + QString::number(iDigitsLeadingResult)
            + "," + QString::number(iDigitsTrailingResult) + "," + QString::number(iDigitsExponentResult) + ")";
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepFormatValue_3

//------------------------------------------------------------------------------
void CTest::doTestStepFormatValue_4( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "Ok: 1.2346e+2 mW (123.46,1,4,1)" );

    try
    {
        double  fValResult = 0.0;
        QString strValResult;
        CUnit*  pUnitValResult = nullptr;
        int     iDigitsLeadingResult = 0;
        int     iDigitsTrailingResult = 0;
        int     iDigitsExponentResult = 0;

        EFormatResult formatResult = formatValue(
            /* fVal                */ 123.45,
            /* pUnitVal            */ Electricity::Power()->MilliWatt(),
            /* fRes                */ 0.02,
            /* pUnitRes            */ nullptr,
            /* resType             */ EResTypeResolution,
            /* iDigitsMantissa     */ 0,
            /* iDigitsExponent     */ 2,
            /* bUseEngineeringForm */ true,
            /* pfVal               */ &fValResult,
            /* pstrVal             */ &strValResult,
            /* ppUnitVal           */ &pUnitValResult,
            /* piDigitsLeading     */ &iDigitsLeadingResult,
            /* piDigitsTrailing    */ &iDigitsTrailingResult,
            /* piDigitsExponent    */ &iDigitsExponentResult );

        QString strResultValue = formatResult2Str(formatResult) + ": "
            + strValResult + " " + pUnitValResult->getSymbol()
            + " (" + QString::number(fValResult) + "," + QString::number(iDigitsLeadingResult)
            + "," + QString::number(iDigitsTrailingResult) + "," + QString::number(iDigitsExponentResult) + ")";
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepFormatValue_4

//==========================================================================
// Default constructor and conversion to string
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepDefaultCtor_1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "---" );

    try
    {
        CPhysVal physVal;
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepDefaultCtor_1

//------------------------------------------------------------------------------
void CTest::doTestStepDefaultCtor_2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "45.0" );

    try
    {
        CPhysVal physVal;
        physVal.setVal(45.0);
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepDefaultCtor_2

//------------------------------------------------------------------------------
void CTest::doTestStepDefaultCtor_3( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "20.0 kHz" );

    try
    {
        CPhysVal physVal(Kinematics::Frequency());
        physVal = "20k";
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepDefaultCtor_3

//==========================================================================
// Constructor with unit group or unit creates invalid value
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithUnitGrp_1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "---" );

    try
    {
        CPhysVal physVal( Kinematics::Time() );
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithUnitGrp_1

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithUnitGrp_2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "12.0 h" );

    try
    {
        CPhysVal physVal( Kinematics::Time() );
        physVal.setVal( 12.0, Kinematics::Time()->Hours() );
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithUnitGrp_2

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithUnitGrp_3( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "30.0 s" );

    try
    {
        CPhysVal physVal( Kinematics::Time() );
        physVal.setVal( 30.0 );
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithUnitGrp_3

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithUnitGrp_4( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "12.0 h" );

    try
    {
        CPhysVal physVal( Kinematics::Time()->Hours() );
        physVal.setVal( 12.0 );
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithUnitGrp_4

//==========================================================================
// Copy constructor
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepCopyCtor( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "3.45679 GHz" );

    try
    {
        CPhysVal physVal1( 3.456789e3, Kinematics::Frequency()->MegaHertz(), 100.0, Kinematics::Frequency()->KiloHertz(), EResTypeAccuracy );
        CPhysVal physVal2( physVal1 );
        QString strResultValue = physVal2.toString(
            /* unitFindVal          */ EUnitFindBest,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCopyCtor

//==========================================================================
// Constructors with double values
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithDblVals_1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "1.0" );

    try
    {
        CPhysVal physVal(1.0);
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue( physVal.toString() );
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithDblVals_1

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithDblVals_2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( result2Str(EResultDifferentPhysSizes) );

    try
    {
        CPhysVal physVal( 10.0, Ratio()->PerCent() );
        physVal.convertValue( Kinematics::Frequency()->MegaHertz() );
        i_pTestStep->setResultValue( physVal.toString() );
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
   }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithDblVals_2

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithDblVals_3( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "100.0 %%" );

    try
    {
        CPhysVal physVal( 10.0, Ratio()->PerCent() );
        physVal.convertValue( Ratio()->PerMille() );
        i_pTestStep->setResultValue( physVal.toString() );
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithDblVals_3

//==========================================================================
// Constructors with double value and resolutions
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithDblValsAndRes_1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "1.0000" );

    try
    {
        CPhysVal physVal(1.0,0.001,EResTypeAccuracy);
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue( physVal.toString() );
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithDblValsAndRes_1

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithDblValsAndRes_2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "1.0 %" );

    try
    {
        CPhysVal physVal( 1.0, Ratio()->PerCent(), 0.5, EResTypeAccuracy );
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue( physVal.toString() );
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithDblValsAndRes_2

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithDblValsAndRes_3( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "1.235 %" );

    try
    {
        CPhysVal physVal( 1.2345678, Ratio()->PerCent(), 5.0, Ratio()->PerMille(), EResTypeAccuracy );
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue( physVal.toString() );
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithDblValsAndRes_3

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithDblValsAndRes_4( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "1.2346 %" );

    try
    {
        CPhysVal physVal( 1.2345678, Ratio()->PerCent(), CPhysValRes(0.5,Ratio()->PerMille(),EResTypeAccuracy) );
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue( physVal.toString() );
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithDblValsAndRes_4

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithDblValsAndRes_5( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "0.00123 dBm" );

    try
    {
        CPhysVal physVal( 0.001234, Electricity::Power()->dBMilliWatt(), 0.0001, EResTypeAccuracy );
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue( physVal.toString() );
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithDblValsAndRes_5

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithDblValsAndRes_6( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "12.35 km" );

    try
    {
        CPhysVal physVal( 12.34567, Geometry::Length()->KiloMeter(), 2.0, Ratio()->PerCent(), EResTypeAccuracy );
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue( physVal.toString() );
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithDblValsAndRes_6

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithDblValsAndRes_7( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(12.35 " + c_strSymbolPlusMinus + " 0.25) km" );

    try
    {
        CPhysVal physVal( 12.34567, Geometry::Length()->KiloMeter(), 2.0, Ratio()->PerCent(), EResTypeAccuracy );
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue( strResultValue );
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithDblValsAndRes_7

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithDblValsAndRes_8( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "12.346 km" );

    try
    {
        CPhysVal physVal( 12.34567, Geometry::Length()->KiloMeter(), 25.0, Geometry::Length()->Meter(), EResTypeAccuracy );
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue( physVal.toString() );
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithDblValsAndRes_8

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithDblValsAndRes_9( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "12.35 km" );

    try
    {
        CPhysVal physVal( 12.34567, Geometry::Length()->KiloMeter(), CPhysValRes(100.0,Geometry::Length()->Meter(),EResTypeAccuracy) );
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue( physVal.toString() );
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithDblValsAndRes_9

//==========================================================================
// Constructors with string value and conversion into strings
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithStrValsConvert2Str_1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "0.0" );

    try
    {
        CPhysVal physVal( "0.0" );
        i_pTestStep->setResultValue( physVal.toString() );
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithStrValsConvert2Str_1

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithStrValsConvert2Str_2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "0.0012" );

    try
    {
        CPhysVal physVal( "1.2e-3" );
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithStrValsConvert2Str_2

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithStrValsConvert2Str_3( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "1.234" );

    try
    {
        CPhysVal physVal( "1.234" );
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindBest,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithStrValsConvert2Str_3

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithStrValsConvert2Str_4( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "1.2 s" );

    try
    {
        CPhysVal physVal( "(1.234 " + c_strSymbolPlusMinus + " 0.56) Kinematics.Time.s", EResTypeAccuracy );
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrNone );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithStrValsConvert2Str_4

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithStrValsConvert2Str_5( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(1.23 " + c_strSymbolPlusMinus + " 0.26) s" );

    try
    {
        CPhysVal physVal( "(1.234 " + c_strSymbolPlusMinus + " 0.26) Kinematics.Time.s", EResTypeAccuracy );
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithStrValsConvert2Str_5

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithStrValsConvert2Str_6( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(1.23 " + c_strSymbolPlusMinus + " 0.056) km" );

    try
    {
        CPhysVal physVal( "1.234 Geometry.km " + c_strSymbolPlusMinus + " 56 m", EResTypeAccuracy );
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithStrValsConvert2Str_6

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithStrValsConvert2Str_7( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "0.02 s" );

    try
    {
        CPhysVal physVal( "0.02 s", Kinematics::Time() );
        QString strResultValue = physVal.toString();
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithStrValsConvert2Str_7

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithStrValsConvert2Str_8( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "---" );

    try
    {
        CPhysVal physVal( "(345.6789 " + c_strSymbolPlusMinus + " 0.003) Geometry.Length.km)", EResTypeAccuracy );
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithStrValsConvert2Str_8

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithStrValsConvert2Str_9( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(345.679 " + c_strSymbolPlusMinus + " 0.003) km" );

    try
    {
        CPhysVal physVal( "(345.6789 " + c_strSymbolPlusMinus + " 0.003) Geometry.Length.km", EResTypeAccuracy );
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithStrValsConvert2Str_9

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithStrValsConvert2Str_10( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(45.0 " + c_strSymbolPlusMinus + " 2.0) %" );

    try
    {
        CPhysVal physVal( "(45 " + c_strSymbolPlusMinus + " 2) %", Ratio(), EResTypeAccuracy );
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithStrValsConvert2Str_10

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithStrValsConvert2Str_11( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(8.900 " + c_strSymbolPlusMinus + " 0.0045) " + QString::fromLatin1("µ") + "V" );

    try
    {
        CPhysVal physVal( "8.9 " + QString::fromLatin1("µ") + "V" + c_strSymbolPlusMinus + " 0.05 %", Electricity::Voltage(), Ratio(), EResTypeAccuracy );
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithStrValsConvert2Str_11

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithStrValsConvert2Str_12( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "8.900 " + QString::fromLatin1("µ") + "V " + c_strSymbolPlusMinus + " 0.05 %" );

    try
    {
        CPhysVal physVal( "8.9 " + QString::fromLatin1("µ") + "V " + c_strSymbolPlusMinus + " 0.05 %", Electricity::Voltage(), Ratio(), EResTypeAccuracy );
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithStrValsConvert2Str_12

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithStrValsConvert2Str_13( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "8.900 " + QString::fromLatin1("µ") + "V " + c_strSymbolPlusMinus + " 4.5 nV" );

    try
    {
        CPhysVal physVal( "8.9 " + QString::fromLatin1("µ") + "V" + c_strSymbolPlusMinus + " 0.05 %", Electricity::Voltage(), Ratio(), EResTypeAccuracy );
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitRes              */ Electricity::Voltage()->NanoVolt(),
            /* iResSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithStrValsConvert2Str_13

//------------------------------------------------------------------------------
void CTest::doTestStepCtorWithStrValsConvert2Str_14( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "MixedLinLogInValAndRes" );

    try
    {
        CPhysVal physVal( "8.9 mW" + c_strSymbolPlusMinus + " 4.5 dBm", Electricity::Power(), EResTypeAccuracy );
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitRes              */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepCtorWithStrValsConvert2Str_14

//==========================================================================
// Operators (equals, greater and less than) with CPhysVal as operand
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithCPhysValOperand_1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "true" );

    try
    {
        CPhysVal physVal1( 8.9, Electricity::Voltage()->Volt() );
        CPhysVal physVal2( 8.9e3, Electricity::Voltage()->MilliVolt() );
        QString strResultValue = bool2Str( physVal1 == physVal2 );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithCPhysValOperand_1

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithCPhysValOperand_2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "true" );

    try
    {
        CPhysVal physVal1( 8.9e3, Electricity::Voltage()->Volt() );
        CPhysVal physVal2( 8.9e3, Electricity::Voltage()->MilliVolt() );
        QString strResultValue = bool2Str( physVal1 != physVal2 );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithCPhysValOperand_2

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithCPhysValOperand_3( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "false" );

    try
    {
        CPhysVal physVal1( 8.9e3, Electricity::Voltage()->Volt() );
        CPhysVal physVal2( 8.9e3, Electricity::Voltage()->MilliVolt() );
        QString strResultValue = bool2Str( physVal1 < physVal2 );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithCPhysValOperand_3

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithCPhysValOperand_4( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "true" );

    try
    {
        CPhysVal physVal1( 8.9e3, Electricity::Voltage()->Volt() );
        CPhysVal physVal2( 8.9e3, Electricity::Voltage()->MilliVolt() );
        QString strResultValue = bool2Str( physVal1 > physVal2 );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithCPhysValOperand_4

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithCPhysValOperand_5( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "false" );

    try
    {
        CPhysVal physVal1( 8.9e3, Electricity::Voltage()->Volt() );
        CPhysVal physVal2( 8.9e3, Electricity::Voltage()->MilliVolt() );
        QString strResultValue = bool2Str( physVal1 <= physVal2 );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithCPhysValOperand_5

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithCPhysValOperand_6( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "true" );

    try
    {
        CPhysVal physVal1( 8.9e3, Electricity::Voltage()->Volt() );
        CPhysVal physVal2( 8.9e3, Electricity::Voltage()->MilliVolt() );
        QString strResultValue = bool2Str( physVal1 >= physVal2 );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithCPhysValOperand_6

//==========================================================================
// Operators (assignment) to another CPhysVal
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepAssignmentOperator( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "8900.0 mV" );

    try
    {
        CPhysVal physVal1( 8.9e3, Electricity::Voltage()->MilliVolt() );
        CPhysVal physVal2( Ratio() );
        physVal2 = physVal1;
        QString strResultValue = physVal2.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepAssignmentOperator

//==========================================================================
// Operators (+ and -) with CPhysVal as operand
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsAddSubWithCPhysValOperand_1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "MixedLinLogInMathOp" );

    try
    {
        CPhysVal physVal1( 7.5, Electricity::Power()->dBMilliWatt() );
        CPhysVal physVal2( 0.027, Electricity::Power()->Watt() );
        CPhysVal physVal3 = physVal1 + physVal2;
        QString strResultValue = physVal3.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsAddSubWithCPhysValOperand_1

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsAddSubWithCPhysValOperand_2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "MixedLinLogInMathOp" );

    try
    {
        CPhysVal physVal1( 7.5, Electricity::Power()->dBMilliWatt() );
        CPhysVal physVal2( 0.027, Electricity::Power()->Watt() );
        CPhysVal physVal3 = physVal1 - physVal2;
        QString strResultValue = physVal3.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsAddSubWithCPhysValOperand_2

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsAddSubWithCPhysValOperand_3( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "0.151832 W" );

    try
    {
        CPhysVal physVal1( 0.027, Electricity::Power()->Watt() );
        CPhysVal physVal2( 7.5, Electricity::Power()->dBMilliWatt() );
        CPhysVal physVal3 = physVal1 + physVal2;
        QString strResultValue = physVal3.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsAddSubWithCPhysValOperand_3

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsAddSubWithCPhysValOperand_4( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "0.004801 W" );

    try
    {
        CPhysVal physVal1( 0.027, Electricity::Power()->Watt() );
        CPhysVal physVal2( 7.5, Electricity::Power()->dBMilliWatt() );
        CPhysVal physVal3 = physVal1 - physVal2;
        QString strResultValue = physVal3.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsAddSubWithCPhysValOperand_4

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsAddSubWithCPhysValOperand_5( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(0.2700 " + c_strSymbolPlusMinus + " 0.002) W" );

    try
    {
        CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
        CPhysVal physVal2( 10.0, Electricity::Power()->dBMilliWatt() );
        CPhysVal physVal3 = physVal1 + physVal2;
        QString strResultValue = physVal3.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsAddSubWithCPhysValOperand_5

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsAddSubWithCPhysValOperand_6( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(0.0027 " + c_strSymbolPlusMinus + " 0.002) W" );

    try
    {
        CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
        CPhysVal physVal2( 10.0, Electricity::Power()->dBMilliWatt() );
        CPhysVal physVal3 = physVal1 - physVal2;
        QString strResultValue = physVal3.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsAddSubWithCPhysValOperand_6

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsAddSubWithCPhysValOperand_7( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(0.0540 " + c_strSymbolPlusMinus + " 0.0004) W" );

    try
    {
        CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
        CPhysVal physVal2( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
        CPhysVal physVal3 = physVal1 + physVal2;
        QString strResultValue = physVal3.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsAddSubWithCPhysValOperand_7

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsAddSubWithCPhysValOperand_8( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(0.0135 " + c_strSymbolPlusMinus + " 0.0004) W" );

    try
    {
        CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
        CPhysVal physVal2( 0.0135, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
        CPhysVal physVal3 = physVal1 - physVal2;
        QString strResultValue = physVal3.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsAddSubWithCPhysValOperand_8

//==========================================================================
// Operators (equals, greater and less than) with double as operand
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithDoubleAsOperand_1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "true" );

    try
    {
        CPhysVal physVal1( -23.45e-3 );
        double   fVal2(-0.02345);
        QString strResultValue = bool2Str( physVal1 == fVal2 );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithDoubleAsOperand_1

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithDoubleAsOperand_2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "false" );

    try
    {
        CPhysVal physVal1( -23.45e-3, Kinematics::Time()->Seconds() );
        double   fVal2(-0.02345);
        QString strResultValue = bool2Str( physVal1 == fVal2 );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithDoubleAsOperand_2

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithDoubleAsOperand_3( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "false" );

    try
    {
        CPhysVal physVal1( -23.45e-3 );
        double   fVal2(-0.02345);
        QString strResultValue = bool2Str( physVal1 != fVal2 );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }

} // doTestStepOperatorsWithDoubleAsOperand_3

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithDoubleAsOperand_4( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "false" );

    try
    {
        CPhysVal physVal1( -23.45e-3 );
        double   fVal2(-0.02345);
        QString strResultValue = bool2Str( physVal1 < fVal2 );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithDoubleAsOperand_4

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithDoubleAsOperand_5( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "false" );

    try
    {
        CPhysVal physVal1( -23.45e-3 );
        double   fVal2(-0.02345);
        QString strResultValue = bool2Str( physVal1 > fVal2 );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithDoubleAsOperand_5

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithDoubleAsOperand_6( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "true" );

    try
    {
        CPhysVal physVal1( -23.45e-3 );
        double   fVal2(-0.02345);
        QString strResultValue = bool2Str( physVal1 <= fVal2 );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithDoubleAsOperand_6

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithDoubleAsOperand_7( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "true" );

    try
    {
        CPhysVal physVal1( -23.45e-3 );
        double   fVal2(-0.02345);
        QString strResultValue = bool2Str( physVal1 >= fVal2 );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithDoubleAsOperand_7

//==========================================================================
// Operator (assignment) to another double
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepAssignToDouble( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "0.0135 mW" );

    try
    {
        double   fVal( 0.0135 );
        CPhysVal physVal1( Electricity::Power()->MilliWatt() );
        physVal1 = fVal;
        QString strResultValue = physVal1.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepAssignToDouble

//==========================================================================
// Operators (+ and -) with double as operand
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsAddSubWithDoubleOperand_1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( result2Str(EResultDifferentPhysSizes) );

    try
    {
        CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
        double   fVal2( 0.1 );
        CPhysVal physVal3 = physVal1 - fVal2;
        QString strResultValue = physVal3.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsAddSubWithDoubleOperand_1

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsAddSubWithDoubleOperand_2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "0.12700 " + c_strSymbolPlusMinus + " 0.0002" );

    try
    {
        CPhysVal physVal1( 0.027, 0.0002, EResTypeAccuracy );
        double   fVal2( 0.1 );
        CPhysVal physVal3 = physVal1 + fVal2;
        QString strResultValue = physVal3.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsAddSubWithDoubleOperand_2

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsAddSubWithDoubleOperand_3( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "0.1270 " + c_strSymbolPlusMinus + " 0.0003" );

    try
    {
        double   fVal( 0.1 );
        CPhysVal physVal( 0.027, 0.0003, EResTypeAccuracy );
        physVal += fVal;
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsAddSubWithDoubleOperand_3

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsAddSubWithDoubleOperand_4( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "-0.0730 " + c_strSymbolPlusMinus + " 0.002" );

    try
    {
        CPhysVal physVal1( 0.027, 0.002, EResTypeAccuracy );
        double   fVal2( 0.1 );
        CPhysVal physVal3 = physVal1 - fVal2;
        QString strResultValue = physVal3.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsAddSubWithDoubleOperand_4

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsAddSubWithDoubleOperand_5( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "-0.073 " + c_strSymbolPlusMinus + " 0.003" );

    try
    {
        double   fVal( 0.1 );
        CPhysVal physVal( 0.027, 0.003, EResTypeAccuracy );
        physVal -= fVal;
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsAddSubWithDoubleOperand_5


//==========================================================================
// Operators (* and /) with double as operand
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsMulDivWithDoubleOperand_1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(0.13500 " + c_strSymbolPlusMinus + " 0.0002) W" );

    try
    {
        CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
        double   fVal2( 5.0 );
        CPhysVal physVal3 = physVal1 * fVal2;
        QString strResultValue = physVal3.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsMulDivWithDoubleOperand_1

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsMulDivWithDoubleOperand_2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(0.13500 " + c_strSymbolPlusMinus + " 0.0002) W" );

    try
    {
        CPhysVal physVal( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
        physVal *= 5.0;
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsMulDivWithDoubleOperand_2

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsMulDivWithDoubleOperand_3( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(0.00540 " + c_strSymbolPlusMinus + " 0.0002) W" );

    try
    {
        CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
        double   fVal2( 5.0 );
        CPhysVal physVal3 = physVal1 / fVal2;
        QString strResultValue = physVal3.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsMulDivWithDoubleOperand_3

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsMulDivWithDoubleOperand_4( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(0.00540 " + c_strSymbolPlusMinus + " 0.0002) W" );

    try
    {
        double   fVal( 5.0 );
        CPhysVal physVal( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
        physVal /= fVal;
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsMulDivWithDoubleOperand_4

//==========================================================================
// Operators (equals, greater and less than) with string as operand
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithStringOperand_1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "true" );

    try
    {
        CPhysVal physVal1( -23.45e-3 );
        QString strResultValue = bool2Str( physVal1 == "-0.02345" );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithStringOperand_1

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithStringOperand_2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "true" );

    try
    {
        CPhysVal physVal1( -23.45e-3, Electricity::Power()->Watt() );
        QString strResultValue = bool2Str( physVal1 == "-0.02345 W" );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithStringOperand_2

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithStringOperand_3( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "false" );

    try
    {
        CPhysVal physVal1( -23.45e-3, Electricity::Power()->Watt() );
        QString strResultValue = bool2Str( physVal1 != "-0.02345 W" );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithStringOperand_3

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithStringOperand_4( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "false" );

    try
    {
        CPhysVal physVal1( -23.45e-3 );
        QString strResultValue = bool2Str( physVal1 < "-0.02345" );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithStringOperand_4

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithStringOperand_5( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "false" );

    try
    {
        CPhysVal physVal1( -23.45e-3 );
        QString strResultValue = bool2Str( physVal1 > "-0.02345" );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithStringOperand_5

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithStringOperand_6( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "true" );

    try
    {
        CPhysVal physVal1( -23.45e-3 );
        QString strResultValue = bool2Str( physVal1 <= "-0.02345" );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithStringOperand_6

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsWithStringOperand_7( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "true" );

    try
    {
        CPhysVal physVal1( -23.45e-3 );
        QString strResultValue = bool2Str( physVal1 >= "-0.02345" );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsWithStringOperand_7

//==========================================================================
// Operator (assignment) to another string
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepAssign2Str( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(0.01350 " + c_strSymbolPlusMinus + " 0.0002) W" );

    try
    {
        CPhysVal physVal( Electricity::Power(), EResTypeAccuracy );
        physVal = "0.0135 W " + c_strSymbolPlusMinus + " 0.2 mW";
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepAssign2Str

//==========================================================================
// Operators (+ and -) with string as operand
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsAddSubWithStringOperand_1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(0.0405 " + c_strSymbolPlusMinus + " 0.0004) W" );

    try
    {
        CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
        CPhysVal physVal3 = physVal1 + QString("0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W");
        QString strResultValue = physVal3.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsAddSubWithStringOperand_1

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsAddSubWithStringOperand_2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(0.0405 " + c_strSymbolPlusMinus + " 0.0004) W" );

    try
    {
        CPhysVal physVal( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
        physVal += QString("0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W");
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsAddSubWithStringOperand_2

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsAddSubWithStringOperand_3( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(0.0135 " + c_strSymbolPlusMinus + " 0.0004) W" );

    try
    {
        CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
        CPhysVal physVal3 = physVal1 - QString("0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W");
        QString strResultValue = physVal3.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsAddSubWithStringOperand_3

//------------------------------------------------------------------------------
void CTest::doTestStepOperatorsAddSubWithStringOperand_4( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(0.0135 " + c_strSymbolPlusMinus + " 0.0004) W" );

    try
    {
        CPhysVal physVal( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
        physVal -= QString("0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W");
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindNone,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepOperatorsAddSubWithStringOperand_4

//==========================================================================
// Unit conversions
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepUnitConversions_1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "1.0 mW" );

    try
    {
        CPhysVal physVal( 0.0, Electricity::Power()->dBMilliWatt() );
        physVal.convertValue( Electricity::Power()->MilliWatt() );
        i_pTestStep->setResultValue( physVal.toString() );
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepUnitConversions_1

//------------------------------------------------------------------------------
void CTest::doTestStepUnitConversions_2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "20.0 dBm" );

    try
    {
        CPhysVal physVal( 100.0, Electricity::Power()->MilliWatt() );
        physVal.convertValue( Electricity::Power()->dBMilliWatt() );
        i_pTestStep->setResultValue( physVal.toString() );
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepUnitConversions_2

//==========================================================================
// Accuracy digits
//==========================================================================

//------------------------------------------------------------------------------
void CTest::doTestStepAccuracyDigits_1( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(25.89 " + c_strSymbolPlusMinus + " 0.12) km" );

    try
    {
        CPhysVal physVal( 25.89453, Geometry::Length()->KiloMeter(), 0.12, EResTypeAccuracy );
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindBest,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepAccuracyDigits_1

//------------------------------------------------------------------------------
void CTest::doTestStepAccuracyDigits_2( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(25.9 " + c_strSymbolPlusMinus + " 0.4) km" );

    try
    {
        CPhysVal physVal( 25.89453, Geometry::Length()->KiloMeter(), 0.4, EResTypeAccuracy );
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindBest,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepAccuracyDigits_2

//------------------------------------------------------------------------------
void CTest::doTestStepAccuracyDigits_3( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(9.87654321 " + c_strSymbolPlusMinus + " 0.00000005) GHz" );

    try
    {
        CPhysVal physVal( 9876543210.12, Kinematics::Frequency()->Hertz(), 0.05, Kinematics::Frequency()->KiloHertz(), EResTypeAccuracy );
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindBest,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepAccuracyDigits_3

//------------------------------------------------------------------------------
void CTest::doTestStepAccuracyDigits_4( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    i_pTestStep->setExpectedValue( "(9.88 " + c_strSymbolPlusMinus + " 0.12) kHz" );

    try
    {
        CPhysVal physVal( 0.000009876543, Kinematics::Frequency()->GigaHertz(), 120.0, Kinematics::Frequency()->Hertz(), EResTypeAccuracy );
        QString strResultValue = physVal.toString(
            /* unitFindVal          */ EUnitFindBest,
            /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
            /* unitFindRes          */ EUnitFindNone,
            /* iResSubStrVisibility */ EPhysValSubStrVal );
        i_pTestStep->setResultValue(strResultValue);
    }
    catch( CException& exc )
    {
        i_pTestStep->setResultValue( exc.getResultStr() );
    }
    catch(...)
    {
        i_pTestStep->setResultValue( "Unknown Exception thrown" );
    }
} // doTestStepAccuracyDigits_4
