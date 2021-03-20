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

#include "Test.h"

#include "ZSPhysSizes/Electricity/ZSPhysSizes.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSPhysSizes/Kinematics/ZSPhysSizes.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
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
} // default ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class ZS::Test::CTest
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::start()
//------------------------------------------------------------------------------
{
    ZS::Test::CTestStep* pTestStep;
    QString              strActualValue;

    ZS::Test::CTestStepGroup* pTSGrp = getTestStepGroup( "Physical Values", nullptr );

    //==========================================================================
    // formatValue methods
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpFormatValue = getTestStepGroup( "formatValue", pTSGrp );

    EFormatResult formatResult;
    double        fValResult;
    QString       strValResult;
    CUnit*        pUnitValResult;
    int           iDigitsLeadingResult;
    int           iDigitsTrailingResult;
    int           iDigitsExponentResult;

    #define RESET_FORMAT_VALUE_RESULTs \
        fValResult = 0.0; \
        strValResult = ""; \
        pUnitValResult = nullptr; \
        iDigitsLeadingResult = 0; \
        iDigitsTrailingResult = 0; \
        iDigitsExponentResult = 0;

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "formatValue(123.45 mW, Res=0.02 mW, Round2Res)", pTSGrpFormatValue );

        pTestStep->setOperation( "formatValue(123.45 mW, Res=0.02 mW)" );
        pTestStep->setDescription( "Rounding to resolution" );
        pTestStep->setDesiredValue( "Ok: 123.46 mW (123.46,3,2,0)" );

        try
        {
            RESET_FORMAT_VALUE_RESULTs;

            pTestStep->testStarted();

            formatResult = formatValue(
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

            strActualValue = formatResult2Str(formatResult) + ": " + strValResult + " " + pUnitValResult->getSymbol() + " (" + QString::number(fValResult) + "," + QString::number(iDigitsLeadingResult) + "," + QString::number(iDigitsTrailingResult) + "," + QString::number(iDigitsExponentResult) + ")";
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "formatValue(123.45 mW, Res=0.02 mW, Overflow)", pTSGrpFormatValue );

        pTestStep->setOperation( "formatValue(123.45 mW, Res=0.02 mW)" );
        pTestStep->setDescription( "Not enough exponent digits for engineering format" );
        pTestStep->setDesiredValue( "ValueOverflow: --- mW (123.46,0,0,1)" );

        try
        {
            RESET_FORMAT_VALUE_RESULTs;

            pTestStep->testStarted();

            formatResult = formatValue(
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

            strActualValue = formatResult2Str(formatResult) + ": " + strValResult + " " + pUnitValResult->getSymbol() + " (" + QString::number(fValResult) + "," + QString::number(iDigitsLeadingResult) + "," + QString::number(iDigitsTrailingResult) + "," + QString::number(iDigitsExponentResult) + ")";
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "formatValue(123.45 mW, Res=0.02 mW, Eng.Format 1.Dig.Exp.)", pTSGrpFormatValue );

        pTestStep->setOperation( "formatValue(123.45 mW, Res=0.02 mW)" );
        pTestStep->setDescription( "Engineering format" );
        pTestStep->setDesiredValue( "Ok: 1.2346e+2 mW (123.46,1,4,1)" );

        try
        {
            RESET_FORMAT_VALUE_RESULTs;

            pTestStep->testStarted();

            formatResult = formatValue(
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

            strActualValue = formatResult2Str(formatResult) + ": " + strValResult + " " + pUnitValResult->getSymbol() + " (" + QString::number(fValResult) + "," + QString::number(iDigitsLeadingResult) + "," + QString::number(iDigitsTrailingResult) + "," + QString::number(iDigitsExponentResult) + ")";
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "formatValue(123.45 mW, Res=0.02 mW 2.Dig.Exp.)", pTSGrpFormatValue );

        pTestStep->setOperation( "formatValue(123.45 mW, Res=0.02 mW)" );
        pTestStep->setDescription( "Engineering format" );
        pTestStep->setDesiredValue( "Ok: 1.2346e+2 mW (123.46,1,4,1)" );

        try
        {
            RESET_FORMAT_VALUE_RESULTs;

            pTestStep->testStarted();

            formatResult = formatValue(
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

            strActualValue = formatResult2Str(formatResult) + ": " + strValResult + " " + pUnitValResult->getSymbol() + " (" + QString::number(fValResult) + "," + QString::number(iDigitsLeadingResult) + "," + QString::number(iDigitsTrailingResult) + "," + QString::number(iDigitsExponentResult) + ")";
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }


    //==========================================================================
    // Default constructor and conversion to string
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpDefaultCtor = getTestStepGroup( "Default ctor and convert2Str", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal().toString()", pTSGrpDefaultCtor );

        pTestStep->setOperation( "CPhysVal().toString()" );
        pTestStep->setDescription( "Default constructor creates invalid value" );
        pTestStep->setDesiredValue( "---" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal;
            strActualValue = physVal.toString();
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal().setVal(45.0)", pTSGrpDefaultCtor );

        pTestStep->setOperation( "CPhysVal().setVal(45.0)" );
        pTestStep->setDescription( "Value without unit" );
        pTestStep->setDesiredValue( "45.0" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal;
            physVal.setVal(45.0);
            strActualValue = physVal.toString();
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(Kinematics.Frequency) = 20k", pTSGrpDefaultCtor );

        pTestStep->setOperation( "CPhysVal(Kinematics.Frequency) = 20k" );
        pTestStep->setDescription( "String with Factor" );
        pTestStep->setDesiredValue( "20.0 kHz" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal(Kinematics::Frequency());
            physVal = "20k";
            strActualValue = physVal.toString();
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    //==========================================================================
    // Constructor with unit group or unit creates invalid value
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpCtorWithUnitGroup = getTestStepGroup( "ctor with Unit Group", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal( Kinematics.Time ).toString()", pTSGrpCtorWithUnitGroup );

        pTestStep->setOperation( "CPhysVal( Kinematics.Time ).toString()" );
        pTestStep->setDescription( "Constructor with unit group but no value creates invalid value" );
        pTestStep->setDesiredValue( "---" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( Kinematics::Time() );
            strActualValue = physVal.toString();
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal( Kinematics.Time ).setVal(12.0,h)", pTSGrpCtorWithUnitGroup );

        pTestStep->setOperation( "CPhysVal( Kinematics.Time ).setVal(12.0,h)" );
        pTestStep->setDescription( "Value with unit" );
        pTestStep->setDesiredValue( "12.0 h" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( Kinematics::Time() );
            physVal.setVal( 12.0, Kinematics::Time()->Hours() );
            strActualValue = physVal.toString();
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal( Kinematics.Time ).setVal(30.0)", pTSGrpCtorWithUnitGroup );

        pTestStep->setOperation( "CPhysVal( Kinematics.Time ).setVal(30.0)" );
        pTestStep->setDescription( "Constructor with unit group uses SI unit as default" );
        pTestStep->setDesiredValue( "30.0 s" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( Kinematics::Time() );
            physVal.setVal( 30.0 );
            strActualValue = physVal.toString();
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal( Kinematics.Time.Hours ).setVal(12.0)", pTSGrpCtorWithUnitGroup );

        pTestStep->setOperation( "CPhysVal( Kinematics.Time.Hours ).setVal(12.0)" );
        pTestStep->setDescription( "Constructor with unit, value set later" );
        pTestStep->setDesiredValue( "12.0 h" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( Kinematics::Time()->Hours() );
            physVal.setVal( 12.0 );
            strActualValue = physVal.toString();
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    //==========================================================================
    // Copy constructor
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpCopyCtor = getTestStepGroup( "Copy ctor", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal( physVal(3.456789e3,Kinematics.Frequency.MHz,100.0,kHz) )", pTSGrpCopyCtor );

        pTestStep->setOperation( "CPhysVal( physVal(3.456789e3,Kinematics.Frequency.MHz,100.0,kHz) )" );
        pTestStep->setDescription( "Copy constructor" );
        pTestStep->setDesiredValue( "3.45679 GHz" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 3.456789e3, Kinematics::Frequency()->MegaHertz(), 100.0, Kinematics::Frequency()->KiloHertz(), EResTypeAccuracy );
            CPhysVal physVal2( physVal1 );
            strActualValue = physVal2.toString(
                /* unitFindVal          */ EUnitFindBest,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    //==========================================================================
    // Constructors with double values
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpCtorWithDoubleValues = getTestStepGroup( "ctor with Double Values", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(1.0).toString()", pTSGrpCtorWithDoubleValues );

        pTestStep->setOperation( "CPhysVal(1.0).toString()" );
        pTestStep->setDescription( "Constructor with double value, one trailing digit" );
        pTestStep->setDesiredValue( "1.0" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal(1.0);
            strActualValue = physVal.toString();
            pTestStep->setActualValue( physVal.toString() );
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(10.0,%).convertValue(MHz)", pTSGrpCtorWithDoubleValues );

        pTestStep->setOperation( "CPhysVal(10.0,%).convertValue(MHz)" );
        pTestStep->setDescription( "Conversion not possible" );
        pTestStep->setDesiredValue( result2Str(EResultDifferentPhysSizes) );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 10.0, Ratio()->PerCent() );
            physVal.convertValue( Kinematics::Frequency()->MegaHertz() );
            pTestStep->setActualValue( physVal.toString() );
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(10.0,%).convertValue(%%)", pTSGrpCtorWithDoubleValues );

        pTestStep->setOperation( "CPhysVal(10.0,%).convertValue(%%)" );
        pTestStep->setDescription( "Conversion Ok" );
        pTestStep->setDesiredValue( "100.0 %%" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 10.0, Ratio()->PerCent() );
            physVal.convertValue( Ratio()->PerMille() );
            pTestStep->setActualValue( physVal.toString() );
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    //==========================================================================
    // Constructors with double value and resolutions
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpCtorWithDoubleValueAndRes = getTestStepGroup( "ctor with Double Values and Resolution", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(1.0,0.001).toString()", pTSGrpCtorWithDoubleValueAndRes );

        pTestStep->setOperation( "CPhysVal(1.0,0.001).toString()" );
        pTestStep->setDescription( "Constructor with double value and resolution" );
        pTestStep->setDesiredValue( "1.0000" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal(1.0,0.001,EResTypeAccuracy);
            strActualValue = physVal.toString();
            pTestStep->setActualValue( physVal.toString() );
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(1.0,%,0.5).toString()", pTSGrpCtorWithDoubleValueAndRes );

        pTestStep->setOperation( "CPhysVal(1.0,%,0.5).toString()" );
        pTestStep->setDescription( "Constructor with double value and resolution" );
        pTestStep->setDesiredValue( "1.0 %" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 1.0, Ratio()->PerCent(), 0.5, EResTypeAccuracy );
            strActualValue = physVal.toString();
            pTestStep->setActualValue( physVal.toString() );
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(1.2345678,%,5.0,%%).toString()", pTSGrpCtorWithDoubleValueAndRes );

        pTestStep->setOperation( "CPhysVal(1.2345678,%,5.0,%%).toString()" );
        pTestStep->setDescription( "Constructor with double value and resolution, both as ratio values" );
        pTestStep->setDesiredValue( "1.235 %" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 1.2345678, Ratio()->PerCent(), 5.0, Ratio()->PerMille(), EResTypeAccuracy );
            strActualValue = physVal.toString();
            pTestStep->setActualValue( physVal.toString() );
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(1.2345678,%,CPhysValRes(0.5,%%)).toString()", pTSGrpCtorWithDoubleValueAndRes );

        pTestStep->setOperation( "CPhysVal(1.2345678,%,CPhysValRes(0.5,%%)).toString()" );
        pTestStep->setDescription( "Constructor with double value and resolution, both as ratio values" );
        pTestStep->setDesiredValue( "1.2346 %" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 1.2345678, Ratio()->PerCent(), CPhysValRes(0.5,Ratio()->PerMille(),EResTypeAccuracy) );
            strActualValue = physVal.toString();
            pTestStep->setActualValue( physVal.toString() );
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(0.001234,dBm,0.0001).toString()", pTSGrpCtorWithDoubleValueAndRes );

        pTestStep->setOperation( "CPhysVal(0.001234,dBm,0.0001).toString()" );
        pTestStep->setDescription( "Constructor with double value and resolution" );
        pTestStep->setDesiredValue( "0.00123 dBm" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 0.001234, Electricity::Power()->dBMilliWatt(), 0.0001, EResTypeAccuracy );
            strActualValue = physVal.toString();
            pTestStep->setActualValue( physVal.toString() );
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(12.34567,km,2.0,%).toString()", pTSGrpCtorWithDoubleValueAndRes );

        pTestStep->setOperation( "CPhysVal(12.34567,km,2.0,%).toString()" );
        pTestStep->setDescription( "Constructor with double value and resolution" );
        pTestStep->setDesiredValue( "12.35 km" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 12.34567, Geometry::Length()->KiloMeter(), 2.0, Ratio()->PerCent(), EResTypeAccuracy );
            strActualValue = physVal.toString();
            pTestStep->setActualValue( physVal.toString() );
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(12.34567,km,2.0,%).toString(WithRes)", pTSGrpCtorWithDoubleValueAndRes );

        pTestStep->setOperation( "CPhysVal(12.34567,km,2.0,%).toString(WithRes)" );
        pTestStep->setDescription( "Constructor with double value and resolution" );
        pTestStep->setDesiredValue( "(12.35 " + c_strSymbolPlusMinus + " 0.25) km" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 12.34567, Geometry::Length()->KiloMeter(), 2.0, Ratio()->PerCent(), EResTypeAccuracy );
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue( strActualValue );
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(12.34567,km,25.0,m).toString()", pTSGrpCtorWithDoubleValueAndRes );

        pTestStep->setOperation( "CPhysVal(12.34567,km,25.0,m).toString()" );
        pTestStep->setDescription( "Constructor with double value and resolution" );
        pTestStep->setDesiredValue( "12.346 km" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 12.34567, Geometry::Length()->KiloMeter(), 25.0, Geometry::Length()->Meter(), EResTypeAccuracy );
            strActualValue = physVal.toString();
            pTestStep->setActualValue( physVal.toString() );
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(12.34567,km,CPhysValRes(100.0,m).toString()", pTSGrpCtorWithDoubleValueAndRes );

        pTestStep->setOperation( "CPhysVal(12.34567,km,CPhysValRes(100.0,m).toString()" );
        pTestStep->setDescription( "Constructor with double value and resolution" );
        pTestStep->setDesiredValue( "12.35 km" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 12.34567, Geometry::Length()->KiloMeter(), CPhysValRes(100.0,Geometry::Length()->Meter(),EResTypeAccuracy) );
            strActualValue = physVal.toString();
            pTestStep->setActualValue( physVal.toString() );
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    //==========================================================================
    // Constructors with string value and conversion into strings
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpWithStringValuesConvert2Str = getTestStepGroup( "ctors with String Values convert2Str", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(\"0.0\")", pTSGrpWithStringValuesConvert2Str );

        pTestStep->setOperation( "CPhysVal(\"0.0\")" );
        pTestStep->setDescription( "Constructor with string value" );
        pTestStep->setDesiredValue( "0.0" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( "0.0" );
            pTestStep->setActualValue( physVal.toString() );
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(\"1.2e-3\")", pTSGrpWithStringValuesConvert2Str );

        pTestStep->setOperation( "CPhysVal(\"1.2e-3\")" );
        pTestStep->setDescription( "Constructor with string value" );
        pTestStep->setDesiredValue( "0.0012" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( "1.2e-3" );
            strActualValue = physVal.toString();
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(\"1.234\")", pTSGrpWithStringValuesConvert2Str );

        pTestStep->setOperation( "CPhysVal(\"1.234\")" );
        pTestStep->setDescription( "Constructor with string value" );
        pTestStep->setDesiredValue( "1.234" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( "1.234" );
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindBest,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(\"(1.234 " + c_strSymbolPlusMinus + " 0.56) Kinematics.Time.s\")", pTSGrpWithStringValuesConvert2Str );

        pTestStep->setOperation( "CPhysVal(\"(1.234 " + c_strSymbolPlusMinus + " 0.56) Kinematics.Time.s\")" );
        pTestStep->setDescription( "Constructor with string value containing resolution" );
        pTestStep->setDesiredValue( "1.2 s" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( "(1.234 " + c_strSymbolPlusMinus + " 0.56) Kinematics.Time.s", EResTypeAccuracy );
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrNone );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(\"(1.234 " + c_strSymbolPlusMinus + " 0.26) Kinematics.Time.s\")", pTSGrpWithStringValuesConvert2Str );

        pTestStep->setOperation( "CPhysVal(\"(1.234 " + c_strSymbolPlusMinus + " 0.26) Kinematics.Time.s\")" );
        pTestStep->setDescription( "Constructor with string value containing resolution" );
        pTestStep->setDesiredValue( "(1.23 " + c_strSymbolPlusMinus + " 0.26) s" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( "(1.234 " + c_strSymbolPlusMinus + " 0.26) Kinematics.Time.s", EResTypeAccuracy );
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(\"1.234 Geometry.km " + c_strSymbolPlusMinus + " 56 m\")", pTSGrpWithStringValuesConvert2Str );

        pTestStep->setOperation( "CPhysVal(\"1.234 Geometry.km " + c_strSymbolPlusMinus + " 56 m\")" );
        pTestStep->setDescription( "Constructor with string value containing resolution" );
        pTestStep->setDesiredValue( "(1.23 " + c_strSymbolPlusMinus + " 0.056) km" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( "1.234 Geometry.km " + c_strSymbolPlusMinus + " 56 m", EResTypeAccuracy );
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(\"0.02 s\",Kinematics.Time)", pTSGrpWithStringValuesConvert2Str );

        pTestStep->setOperation( "CPhysVal(\"0.02 s\",Kinematics.Time)" );
        pTestStep->setDescription( "Constructor with string value" );
        pTestStep->setDesiredValue( "0.02 s" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( "0.02 s", Kinematics::Time() );
            strActualValue = physVal.toString();
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(\"(345.6789 " + c_strSymbolPlusMinus + " 0.003) Geometry.Length.km)\")", pTSGrpWithStringValuesConvert2Str );

        pTestStep->setOperation( "CPhysVal(\"(345.6789 " + c_strSymbolPlusMinus + " 0.003) Geometry.Length.km)\")" );
        pTestStep->setDescription( "Constructor with string value with format error (superfluous closing \")\" at end of string)" );
        pTestStep->setDesiredValue( "---" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( "(345.6789 " + c_strSymbolPlusMinus + " 0.003) Geometry.Length.km)", EResTypeAccuracy );
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(\"(345.6789 " + c_strSymbolPlusMinus + " 0.003) Geometry.Length.km\")", pTSGrpWithStringValuesConvert2Str );

        pTestStep->setOperation( "CPhysVal(\"(345.6789 " + c_strSymbolPlusMinus + " 0.003) Geometry.Length.km\")" );
        pTestStep->setDescription( "Constructor with string value containing resolution" );
        pTestStep->setDesiredValue( "(345.679 " + c_strSymbolPlusMinus + " 0.003) km" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( "(345.6789 " + c_strSymbolPlusMinus + " 0.003) Geometry.Length.km", EResTypeAccuracy );
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(\"(45 " + c_strSymbolPlusMinus + " 2) %\",Ratio)", pTSGrpWithStringValuesConvert2Str );

        pTestStep->setOperation( "CPhysVal(\"(45 " + c_strSymbolPlusMinus + " 2) %\",Ratio)" );
        pTestStep->setDescription( "Constructor with string value containing resolution in %" );
        pTestStep->setDesiredValue( "(45.0 " + c_strSymbolPlusMinus + " 2.0) %" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( "(45 " + c_strSymbolPlusMinus + " 2) %", Ratio(), EResTypeAccuracy );
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(\"8.9 " + QString::fromLatin1("µ") + "V" + c_strSymbolPlusMinus + " 0.05 %\",Electricity.Voltage,Ratio)", pTSGrpWithStringValuesConvert2Str );

        pTestStep->setOperation( "CPhysVal(\"8.9 " + QString::fromLatin1("µ") + "V" + c_strSymbolPlusMinus + " 0.05 %\",Electricity.Voltage,Ratio)" );
        pTestStep->setDescription( "Constructor with string value containing resolution in %" );
        pTestStep->setDesiredValue( "(8.900 " + c_strSymbolPlusMinus + " 0.0045) " + QString::fromLatin1("µ") + "V" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( "8.9 " + QString::fromLatin1("µ") + "V" + c_strSymbolPlusMinus + " 0.05 %", Electricity::Voltage(), Ratio(), EResTypeAccuracy );
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(\"8.9 " + QString::fromLatin1("µ") + "V" + c_strSymbolPlusMinus + " 0.05 %\",Electricity.Voltage,Ratio,OutResWithUnit)", pTSGrpWithStringValuesConvert2Str );

        pTestStep->setOperation( "CPhysVal(\"8.9 " + QString::fromLatin1("µ") + "V" + c_strSymbolPlusMinus + " 0.05 %\",Electricity.Voltage,Ratio,OutResWithUnit)" );
        pTestStep->setDescription( "Constructor with string value containing resolution in %" );
        pTestStep->setDesiredValue( "8.900 " + QString::fromLatin1("µ") + "V " + c_strSymbolPlusMinus + " 0.05 %" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( "8.9 " + QString::fromLatin1("µ") + "V " + c_strSymbolPlusMinus + " 0.05 %", Electricity::Voltage(), Ratio(), EResTypeAccuracy );
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(\"8.9 " + QString::fromLatin1("µ") + "V" + c_strSymbolPlusMinus + " 0.05 %\",Electricity.Voltage,Ratio).toString()", pTSGrpWithStringValuesConvert2Str );

        pTestStep->setOperation( "CPhysVal(\"8.9 " + QString::fromLatin1("µ") + "V" + c_strSymbolPlusMinus + " 0.05 %\",Electricity.Voltage,Ratio).toString()" );
        pTestStep->setDescription( "Constructor with string value containing resolution in %" );
        pTestStep->setDesiredValue( "8.900 " + QString::fromLatin1("µ") + "V " + c_strSymbolPlusMinus + " 4.5 nV" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( "8.9 " + QString::fromLatin1("µ") + "V" + c_strSymbolPlusMinus + " 0.05 %", Electricity::Voltage(), Ratio(), EResTypeAccuracy );
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitRes              */ Electricity::Voltage()->NanoVolt(),
                /* iResSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(\"8.9 mW" + c_strSymbolPlusMinus + " 4.5 dBm\",Electricity.Power).toString()", pTSGrpWithStringValuesConvert2Str );

        pTestStep->setOperation( "CPhysVal(\"8.9 mW" + c_strSymbolPlusMinus + " 4.5 dBm\",Electricity.Power).toString()" );
        pTestStep->setDescription( "Mixing linear and logarithmic units for value and resolution not allowed" );
        pTestStep->setDesiredValue( "MixedLinLogInValAndRes" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( "8.9 mW" + c_strSymbolPlusMinus + " 4.5 dBm", Electricity::Power(), EResTypeAccuracy );
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitRes              */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    //==========================================================================
    // Operators (equals, greater and less than) with CPhysVal as operand
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpOperatorsWithCPhysValOperand = getTestStepGroup( "Operators with CPhysVal Operand", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(8.9,V) == CPhysVal(8.9e3,mV)", pTSGrpOperatorsWithCPhysValOperand );

        pTestStep->setOperation( "CPhysVal(8.9,V) == CPhysVal(8.9e3,mV)");
        pTestStep->setDescription( "operator ==" );
        pTestStep->setDesiredValue( "true" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 8.9, Electricity::Voltage()->Volt() );
            CPhysVal physVal2( 8.9e3, Electricity::Voltage()->MilliVolt() );
            strActualValue = bool2Str( physVal1 == physVal2 );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(8.9e3,V) != CPhysVal(8.9e3,mV)", pTSGrpOperatorsWithCPhysValOperand );

        pTestStep->setOperation( "CPhysVal(8.9e3,V) != CPhysVal(8.9e3,mV)");
        pTestStep->setDescription( "operator !=" );
        pTestStep->setDesiredValue( "true" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 8.9e3, Electricity::Voltage()->Volt() );
            CPhysVal physVal2( 8.9e3, Electricity::Voltage()->MilliVolt() );
            strActualValue = bool2Str( physVal1 != physVal2 );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(8.9e3,V) < CPhysVal(8.9e3,mV)", pTSGrpOperatorsWithCPhysValOperand );

        pTestStep->setOperation( "CPhysVal(8.9e3,V) < CPhysVal(8.9e3,mV)");
        pTestStep->setDescription( "operator <" );
        pTestStep->setDesiredValue( "false" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 8.9e3, Electricity::Voltage()->Volt() );
            CPhysVal physVal2( 8.9e3, Electricity::Voltage()->MilliVolt() );
            strActualValue = bool2Str( physVal1 < physVal2 );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(8.9e3,V) > CPhysVal(8.9e3,mV)", pTSGrpOperatorsWithCPhysValOperand );

        pTestStep->setOperation( "CPhysVal(8.9e3,V) > CPhysVal(8.9e3,mV)");
        pTestStep->setDescription( "operator >" );
        pTestStep->setDesiredValue( "true" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 8.9e3, Electricity::Voltage()->Volt() );
            CPhysVal physVal2( 8.9e3, Electricity::Voltage()->MilliVolt() );
            strActualValue = bool2Str( physVal1 > physVal2 );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(8.9e3,V) <= CPhysVal(8.9e3,mV)", pTSGrpOperatorsWithCPhysValOperand );

        pTestStep->setOperation( "CPhysVal(8.9e3,V) <= CPhysVal(8.9e3,mV)");
        pTestStep->setDescription( "operator <=" );
        pTestStep->setDesiredValue( "false" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 8.9e3, Electricity::Voltage()->Volt() );
            CPhysVal physVal2( 8.9e3, Electricity::Voltage()->MilliVolt() );
            strActualValue = bool2Str( physVal1 <= physVal2 );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(8.9e3,V) >= CPhysVal(8.9e3,mV)", pTSGrpOperatorsWithCPhysValOperand );

        pTestStep->setOperation( "CPhysVal(8.9e3,V) >= CPhysVal(8.9e3,mV)");
        pTestStep->setDescription( "operator >=" );
        pTestStep->setDesiredValue( "true" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 8.9e3, Electricity::Voltage()->Volt() );
            CPhysVal physVal2( 8.9e3, Electricity::Voltage()->MilliVolt() );
            strActualValue = bool2Str( physVal1 >= physVal2 );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    //==========================================================================
    // Operators (assignment) to another CPhysVal
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpAssignmentOperator = getTestStepGroup( "Assignment Operator", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(Ratio) = CPhysVal(8.9e3,mV)", pTSGrpAssignmentOperator );

        pTestStep->setOperation( "physVal(Ratio) = CPhysVal(8.9e3,mV)");
        pTestStep->setDescription( "operator =" );
        pTestStep->setDesiredValue( "8900.0 mV" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 8.9e3, Electricity::Voltage()->MilliVolt() );
            CPhysVal physVal2( Ratio() );
            physVal2 = physVal1;
            strActualValue = physVal2.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    //==========================================================================
    // Operators (+ and -) with CPhysVal as operand
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpOperatorsAddSubWithCPhysValOperand = getTestStepGroup( "Operators + - with CPhysVal Operand", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal = physVal(7.5,dBm) + physVal(0.027,W)", pTSGrpOperatorsAddSubWithCPhysValOperand );

        pTestStep->setOperation( "physVal = physVal(7.5,dBm) + physVal(0.027,W)");
        pTestStep->setDescription( "operator + (logarithmic and linear values may not be added)" );
        pTestStep->setDesiredValue( "MixedLinLogInMathOp" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 7.5, Electricity::Power()->dBMilliWatt() );
            CPhysVal physVal2( 0.027, Electricity::Power()->Watt() );
            CPhysVal physVal3 = physVal1 + physVal2;
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal = physVal(7.5,dBm) - physVal(0.027,W)", pTSGrpOperatorsAddSubWithCPhysValOperand );

        pTestStep->setOperation( "physVal = physVal(7.5,dBm) - physVal(0.027,W)");
        pTestStep->setDescription( "operator + (logarithmic and linear values may not be added)" );
        pTestStep->setDesiredValue( "MixedLinLogInMathOp" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 7.5, Electricity::Power()->dBMilliWatt() );
            CPhysVal physVal2( 0.027, Electricity::Power()->Watt() );
            CPhysVal physVal3 = physVal1 - physVal2;
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal = physVal(0.027,W) + physVal(7.5,dBm)", pTSGrpOperatorsAddSubWithCPhysValOperand );

        pTestStep->setOperation( "physVal = physVal(0.027,W) + physVal(7.5,dBm)");
        pTestStep->setDescription( "operator + (logarithmic value added to linear value results in multiplication)" );
        pTestStep->setDesiredValue( "0.151832 W" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 0.027, Electricity::Power()->Watt() );
            CPhysVal physVal2( 7.5, Electricity::Power()->dBMilliWatt() );
            CPhysVal physVal3 = physVal1 + physVal2;
            strActualValue = physVal3.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal = physVal(0.027,W) - physVal(7.5,dBm)", pTSGrpOperatorsAddSubWithCPhysValOperand );

        pTestStep->setOperation( "physVal = physVal(0.027,W) - physVal(7.5,dBm)");
        pTestStep->setDescription( "operator + (logarithmic value subtracted from linear value results in division)" );
        pTestStep->setDesiredValue( "0.004801 W" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 0.027, Electricity::Power()->Watt() );
            CPhysVal physVal2( 7.5, Electricity::Power()->dBMilliWatt() );
            CPhysVal physVal3 = physVal1 - physVal2;
            strActualValue = physVal3.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal = physVal(0.027,W,0.2,mW) + physVal(10.0,dBm)", pTSGrpOperatorsAddSubWithCPhysValOperand );

        pTestStep->setOperation( "physVal = physVal(0.027,W,0.2,mW) + physVal(10.0,dBm)");
        pTestStep->setDescription( "operator + (logarithmic value added to linear value results in multiplication)" );
        pTestStep->setDesiredValue( "(0.2700 " + c_strSymbolPlusMinus + " 0.002) W" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
            CPhysVal physVal2( 10.0, Electricity::Power()->dBMilliWatt() );
            CPhysVal physVal3 = physVal1 + physVal2;
            strActualValue = physVal3.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal = physVal(0.027,W,0.2,mW) - physVal(10.0,dBm)", pTSGrpOperatorsAddSubWithCPhysValOperand );

        pTestStep->setOperation( "physVal = physVal(0.027,W,0.2,mW) - physVal(10.0,dBm)");
        pTestStep->setDescription( "operator + (logarithmic value subtracted from linear value results in division)" );
        pTestStep->setDesiredValue( "(0.0027 " + c_strSymbolPlusMinus + " 0.002) W" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
            CPhysVal physVal2( 10.0, Electricity::Power()->dBMilliWatt() );
            CPhysVal physVal3 = physVal1 - physVal2;
            strActualValue = physVal3.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal = physVal(0.027,W,0.2,mW) + physVal(0.027,W,0.2,mW)", pTSGrpOperatorsAddSubWithCPhysValOperand );

        pTestStep->setOperation( "physVal = physVal(0.027,W,0.2,mW) + physVal(0.027,W,0.2,mW)");
        pTestStep->setDescription( "operator + (adding two values with resolutions)" );
        pTestStep->setDesiredValue( "(0.0540 " + c_strSymbolPlusMinus + " 0.0004) W" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
            CPhysVal physVal2( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
            CPhysVal physVal3 = physVal1 + physVal2;
            strActualValue = physVal3.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal = physVal(0.027,W,0.2,mW) + physVal(0.0135,W,0.2,mW)", pTSGrpOperatorsAddSubWithCPhysValOperand );

        pTestStep->setOperation( "physVal = physVal(0.027,W,0.2,mW) + physVal(0.0135,W,0.2,mW)");
        pTestStep->setDescription( "operator + (subtracting two values with resolutions)" );
        pTestStep->setDesiredValue( "(0.0135 " + c_strSymbolPlusMinus + " 0.0004) W" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
            CPhysVal physVal2( 0.0135, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
            CPhysVal physVal3 = physVal1 - physVal2;
            strActualValue = physVal3.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    //==========================================================================
    // Operators (equals, greater and less than) with double as operand
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpOperatorsWithDoubleAsOperand = getTestStepGroup( "Operators with Double Operand", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(-23.45e-3) == -0.02345", pTSGrpOperatorsWithDoubleAsOperand );

        pTestStep->setOperation( "physVal(-23.45e-3) == -0.02345");
        pTestStep->setDescription( "operator == (comparing value without unit with double value)" );
        pTestStep->setDesiredValue( "true" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( -23.45e-3 );
            double   fVal2(-0.02345);
            strActualValue = bool2Str( physVal1 == fVal2 );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(-23.45e-3,s) == -0.02345", pTSGrpOperatorsWithDoubleAsOperand );

        pTestStep->setOperation( "physVal(-23.45e-3,s) == -0.02345");
        pTestStep->setDescription( "operator == (comparing value with unit with double value)" );
        pTestStep->setDesiredValue( "false" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( -23.45e-3, Kinematics::Time()->Seconds() );
            double   fVal2(-0.02345);
            strActualValue = bool2Str( physVal1 == fVal2 );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(-23.45e-3) != -0.02345", pTSGrpOperatorsWithDoubleAsOperand );

        pTestStep->setOperation( "physVal(-23.45e-3) != -0.02345");
        pTestStep->setDescription( "operator != (comparing value without unit with double value)" );
        pTestStep->setDesiredValue( "false" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( -23.45e-3 );
            double   fVal2(-0.02345);
            strActualValue = bool2Str( physVal1 != fVal2 );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(-23.45e-3) < -0.02345", pTSGrpOperatorsWithDoubleAsOperand );

        pTestStep->setOperation( "physVal(-23.45e-3) < -0.02345");
        pTestStep->setDescription( "operator < (comparing value without unit with double value)" );
        pTestStep->setDesiredValue( "false" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( -23.45e-3 );
            double   fVal2(-0.02345);
            strActualValue = bool2Str( physVal1 < fVal2 );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(-23.45e-3) > -0.02345", pTSGrpOperatorsWithDoubleAsOperand );

        pTestStep->setOperation( "physVal(-23.45e-3) > -0.02345");
        pTestStep->setDescription( "operator > (comparing value without unit with double value)" );
        pTestStep->setDesiredValue( "false" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( -23.45e-3 );
            double   fVal2(-0.02345);
            strActualValue = bool2Str( physVal1 > fVal2 );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(-23.45e-3) <= -0.02345", pTSGrpOperatorsWithDoubleAsOperand );

        pTestStep->setOperation( "physVal(-23.45e-3) <= -0.02345");
        pTestStep->setDescription( "operator <= (comparing value without unit with double value)" );
        pTestStep->setDesiredValue( "true" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( -23.45e-3 );
            double   fVal2(-0.02345);
            strActualValue = bool2Str( physVal1 <= fVal2 );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(-23.45e-3) >= -0.02345", pTSGrpOperatorsWithDoubleAsOperand );

        pTestStep->setOperation( "physVal(-23.45e-3) >= -0.02345");
        pTestStep->setDescription( "operator >= (comparing value without unit with double value)" );
        pTestStep->setDesiredValue( "true" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( -23.45e-3 );
            double   fVal2(-0.02345);
            strActualValue = bool2Str( physVal1 >= fVal2 );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    //==========================================================================
    // Operator (assignment) to another double
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpAssignToDouble = getTestStepGroup( "Assign to Double", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(mW) = 0.0135", pTSGrpAssignToDouble );

        pTestStep->setOperation( "physVal(mW) = 0.0135");
        pTestStep->setDescription( "operator = (to another double value)" );
        pTestStep->setDesiredValue( "0.0135 mW" );

        try
        {
            pTestStep->testStarted();
            double   fVal( 0.0135 );
            CPhysVal physVal1( Electricity::Power()->MilliWatt() );
            physVal1 = fVal;
            strActualValue = physVal1.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    //==========================================================================
    // Operators (+ and -) with double as operand
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpOperatorsAddSubWithDoubleOperand = getTestStepGroup( "Opeators + - with Double Operand", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal = physVal(0.027,W,0.2,mW) + 0.1", pTSGrpOperatorsAddSubWithDoubleOperand );

        pTestStep->setOperation( "physVal = physVal(0.027,W,0.2,mW) + 0.1");
        pTestStep->setDescription( "operator + (adding double value to physical value with unit not allowed)" );
        pTestStep->setDesiredValue( result2Str(EResultDifferentPhysSizes) );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
            double   fVal2( 0.1 );
            CPhysVal physVal3 = physVal1 - fVal2;
            strActualValue = physVal3.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal = physVal(0.027,0.0002) + 0.1", pTSGrpOperatorsAddSubWithDoubleOperand );

        pTestStep->setOperation( "physVal = physVal(0.027,0.0002) + 0.1");
        pTestStep->setDescription( "operator + (adding double value to physical value)" );
        pTestStep->setDesiredValue( "0.12700 " + c_strSymbolPlusMinus + " 0.0002" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 0.027, 0.0002, EResTypeAccuracy );
            double   fVal2( 0.1 );
            CPhysVal physVal3 = physVal1 + fVal2;
            strActualValue = physVal3.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(0.027,0.0003) += 0.1", pTSGrpOperatorsAddSubWithDoubleOperand );

        pTestStep->setOperation( "physVal(0.027,0.0003) += 0.1");
        pTestStep->setDescription( "operator += (adding double value to physical value)" );
        pTestStep->setDesiredValue( "0.1270 " + c_strSymbolPlusMinus + " 0.0003" );

        try
        {
            pTestStep->testStarted();
            double   fVal( 0.1 );
            CPhysVal physVal( 0.027, 0.0003, EResTypeAccuracy );
            physVal += fVal;
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal = physVal(0.027,0.002) - 0.1", pTSGrpOperatorsAddSubWithDoubleOperand );

        pTestStep->setOperation( "physVal = physVal(0.027,0.002) - 0.1");
        pTestStep->setDescription( "operator - (subtracting double value from physical value)" );
        pTestStep->setDesiredValue( "-0.0730 " + c_strSymbolPlusMinus + " 0.002" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 0.027, 0.002, EResTypeAccuracy );
            double   fVal2( 0.1 );
            CPhysVal physVal3 = physVal1 - fVal2;
            strActualValue = physVal3.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(0.027,0.003) -= 0.1", pTSGrpOperatorsAddSubWithDoubleOperand );

        pTestStep->setOperation( "physVal(0.027,0.003) -= 0.1");
        pTestStep->setDescription( "operator += (subtracting double value from physical value)" );
        pTestStep->setDesiredValue( "-0.073 " + c_strSymbolPlusMinus + " 0.003" );

        try
        {
            pTestStep->testStarted();
            double   fVal( 0.1 );
            CPhysVal physVal( 0.027, 0.003, EResTypeAccuracy );
            physVal -= fVal;
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    //==========================================================================
    // Operators (* and /) with double as operand
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpOperatorsMulDivWithDoubleOperand = getTestStepGroup( "Operators Mul and Div with Double Operand", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal = physVal(0.027,W,0.2,mW) * 5.0", pTSGrpOperatorsMulDivWithDoubleOperand );

        pTestStep->setOperation( "physVal = physVal(0.027,W,0.2,mW) * 5.0");
        pTestStep->setDescription( "operator * (multiplying physical value with resolution with double value)" );
        pTestStep->setDesiredValue( "(0.13500 " + c_strSymbolPlusMinus + " 0.0002) W" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
            double   fVal2( 5.0 );
            CPhysVal physVal3 = physVal1 * fVal2;
            strActualValue = physVal3.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(0.027,W,0.2,mW) *= 5.0", pTSGrpOperatorsMulDivWithDoubleOperand );

        pTestStep->setOperation( "physVal(0.027,W,0.2,mW) *= 5.0");
        pTestStep->setDescription( "operator * (multiplying physical value with resolution with double value)" );
        pTestStep->setDesiredValue( "(0.13500 " + c_strSymbolPlusMinus + " 0.0002) W" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
            physVal *= 5.0;
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal = physVal(0.027,W,0.2,mW) / 5.0", pTSGrpOperatorsMulDivWithDoubleOperand );

        pTestStep->setOperation( "physVal = physVal(0.027,W,0.2,mW) / 5.0");
        pTestStep->setDescription( "operator / (dividing physical value with resolution by double value)" );
        pTestStep->setDesiredValue( "(0.00540 " + c_strSymbolPlusMinus + " 0.0002) W" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
            double   fVal2( 5.0 );
            CPhysVal physVal3 = physVal1 / fVal2;
            strActualValue = physVal3.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal = physVal(0.027,W,0.2,mW) /= 5.0", pTSGrpOperatorsMulDivWithDoubleOperand );

        pTestStep->setOperation( "physVal = physVal(0.027,W,0.2,mW) /= 5.0");
        pTestStep->setDescription( "operator / (dividing physical value with resolution by double value)" );
        pTestStep->setDesiredValue( "(0.00540 " + c_strSymbolPlusMinus + " 0.0002) W" );

        try
        {
            pTestStep->testStarted();
            double   fVal( 5.0 );
            CPhysVal physVal( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
            physVal /= fVal;
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    //==========================================================================
    // Operators (equals, greater and less than) with string as operand
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpOperatorsWithStringOperand = getTestStepGroup( "Operators with String Operand", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(-23.45e-3) == \"-0.02345\"", pTSGrpOperatorsWithStringOperand );

        pTestStep->setOperation( "physVal(-23.45e-3) == \"-0.02345\"");
        pTestStep->setDescription( "operator == (comparing value without unit with string value)" );
        pTestStep->setDesiredValue( "true" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( -23.45e-3 );
            strActualValue = bool2Str( physVal1 == "-0.02345" );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(-23.45e-3,W) == \"-0.02345 W\"", pTSGrpOperatorsWithStringOperand );

        pTestStep->setOperation( "physVal(-23.45e-3,W) == \"-0.02345 W\"");
        pTestStep->setDescription( "operator == (comparing value with unit with string value)" );
        pTestStep->setDesiredValue( "true" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( -23.45e-3, Electricity::Power()->Watt() );
            strActualValue = bool2Str( physVal1 == "-0.02345 W" );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(-23.45e-3,W) != \"-0.02345 W\"", pTSGrpOperatorsWithStringOperand );

        pTestStep->setOperation( "physVal(-23.45e-3,W) != \"-0.02345 W\"");
        pTestStep->setDescription( "operator != (comparing value with unit with string value)" );
        pTestStep->setDesiredValue( "false" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( -23.45e-3, Electricity::Power()->Watt() );
            strActualValue = bool2Str( physVal1 != "-0.02345 W" );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(-23.45e-3) < \"-0.02345\"", pTSGrpOperatorsWithStringOperand );

        pTestStep->setOperation( "physVal(-23.45e-3) < \"-0.02345\"");
        pTestStep->setDescription( "operator < (comparing value without unit with string value)" );
        pTestStep->setDesiredValue( "false" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( -23.45e-3 );
            strActualValue = bool2Str( physVal1 < "-0.02345" );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(-23.45e-3) > \"-0.02345\"", pTSGrpOperatorsWithStringOperand );

        pTestStep->setOperation( "physVal(-23.45e-3) > \"-0.02345\"");
        pTestStep->setDescription( "operator > (comparing value without unit with string value)" );
        pTestStep->setDesiredValue( "false" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( -23.45e-3 );
            strActualValue = bool2Str( physVal1 > "-0.02345" );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(-23.45e-3) <= \"-0.02345\"", pTSGrpOperatorsWithStringOperand );

        pTestStep->setOperation( "physVal(-23.45e-3) <= \"-0.02345\"");
        pTestStep->setDescription( "operator <= (comparing value without unit with string value)" );
        pTestStep->setDesiredValue( "true" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( -23.45e-3 );
            strActualValue = bool2Str( physVal1 <= "-0.02345" );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(-23.45e-3) >= \"-0.02345\"", pTSGrpOperatorsWithStringOperand );

        pTestStep->setOperation( "physVal(-23.45e-3) >= \"-0.02345\"");
        pTestStep->setDescription( "operator >= (comparing value without unit with string value)" );
        pTestStep->setDesiredValue( "true" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( -23.45e-3 );
            strActualValue = bool2Str( physVal1 >= "-0.02345" );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    //==========================================================================
    // Operator (assignment) to another string
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpAssign2Str = getTestStepGroup( "Assign to String", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal = 0.0135 W " + c_strSymbolPlusMinus + " 0.2 mW", pTSGrpAssign2Str );

        pTestStep->setOperation( "physVal = 0.0135 W " + c_strSymbolPlusMinus + " 0.2 mW");
        pTestStep->setDescription( "operator = (assignment to string value)" );
        pTestStep->setDesiredValue( "(0.01350 " + c_strSymbolPlusMinus + " 0.0002) W" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( Electricity::Power(), EResTypeAccuracy );
            physVal = "0.0135 W " + c_strSymbolPlusMinus + " 0.2 mW";
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    //==========================================================================
    // Operators (+ and -) with string as operand
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpOperatorsAddSubWithStringOperand = getTestStepGroup( "Operators + - with String Operand", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal = physVal(0.027,W,0.2,mW) + \"(0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W)\"", pTSGrpOperatorsAddSubWithStringOperand );

        pTestStep->setOperation( "physVal = physVal(0.027,W,0.2,mW) + \"(0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W)\"");
        pTestStep->setDescription( "operator + (adding string value to physical value)" );
        pTestStep->setDesiredValue( "(0.0405 " + c_strSymbolPlusMinus + " 0.0004) W" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
            CPhysVal physVal3 = physVal1 + QString("0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W");
            strActualValue = physVal3.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(0.027,W,0.2,mW) += \"(0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W)\"", pTSGrpOperatorsAddSubWithStringOperand );

        pTestStep->setOperation( "physVal(0.027,W,0.2,mW) += \"(0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W)\"");
        pTestStep->setDescription( "operator += (adding string value to physical value)" );
        pTestStep->setDesiredValue( "(0.0405 " + c_strSymbolPlusMinus + " 0.0004) W" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
            physVal += QString("0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W");
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal = physVal(0.027,W,0.2,mW) - \"(0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W)\"", pTSGrpOperatorsAddSubWithStringOperand );

        pTestStep->setOperation( "physVal = physVal(0.027,W,0.2,mW) - \"(0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W)\"");
        pTestStep->setDescription( "operator - (subtracting string value from physical value)" );
        pTestStep->setDesiredValue( "(0.0135 " + c_strSymbolPlusMinus + " 0.0004) W" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal1( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
            CPhysVal physVal3 = physVal1 - QString("0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W");
            strActualValue = physVal3.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "physVal(0.027,W,0.2,mW) -= \"(0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W)\"", pTSGrpOperatorsAddSubWithStringOperand );

        pTestStep->setOperation( "physVal(0.027,W,0.2,mW) -= \"(0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W)\"");
        pTestStep->setDescription( "operator += (subtracting string value from physical value)" );
        pTestStep->setDesiredValue( "(0.0135 " + c_strSymbolPlusMinus + " 0.0004) W" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 0.027, Electricity::Power()->Watt(), 0.2, Electricity::Power()->MilliWatt(), EResTypeAccuracy );
            physVal -= QString("0.0135 W " + c_strSymbolPlusMinus + " 0.0002 W");
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindNone,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    //==========================================================================
    // Unit conversions
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpUnitConversions = getTestStepGroup( "Unit Conversions", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(0.0,dBm).convertValue(mW)", pTSGrpUnitConversions );

        pTestStep->setOperation( "CPhysVal(0.0,dBm).convertValue(mW)" );
        pTestStep->setDescription( "Conversion from logarithmic unit" );
        pTestStep->setDesiredValue( "1.0 mW" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 0.0, Electricity::Power()->dBMilliWatt() );
            physVal.convertValue( Electricity::Power()->MilliWatt() );
            pTestStep->setActualValue( physVal.toString() );
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(100.0,mW).convertValue(dBm)", pTSGrpUnitConversions );

        pTestStep->setOperation( "CPhysVal(100.0,mW).convertValue(dBm)" );
        pTestStep->setDescription( "Conversion into logarithmic unit" );
        pTestStep->setDesiredValue( "20.0 dBm" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 100.0, Electricity::Power()->MilliWatt() );
            physVal.convertValue( Electricity::Power()->dBMilliWatt() );
            pTestStep->setActualValue( physVal.toString() );
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    //==========================================================================
    // Accuracy digits
    //==========================================================================

    ZS::Test::CTestStepGroup* pTSGrpAccuracyDigits = getTestStepGroup( "Accuracy Digits", pTSGrp );

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(25.89453,km,0.12).toString()", pTSGrpAccuracyDigits );

        pTestStep->setOperation( "CPhysVal(25.89453,km,0.12).toString()" );
        pTestStep->setDescription( "Accuracy digits" );
        pTestStep->setDesiredValue( "(25.89 " + c_strSymbolPlusMinus + " 0.12) km" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 25.89453, Geometry::Length()->KiloMeter(), 0.12, EResTypeAccuracy );
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindBest,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(25.89453,km,0.4).toString()", pTSGrpAccuracyDigits );

        pTestStep->setOperation( "CPhysVal(25.89453,km,0.4).toString()" );
        pTestStep->setDescription( "Accuracy digits" );
        pTestStep->setDesiredValue( "(25.9 " + c_strSymbolPlusMinus + " 0.4) km" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 25.89453, Geometry::Length()->KiloMeter(), 0.4, EResTypeAccuracy );
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindBest,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(9876543210.12,Hz,0.05,kHz).toString()", pTSGrpAccuracyDigits );

        pTestStep->setOperation( "CPhysVal(9876543210.12,Hz,0.05,kHz).toString()" );
        pTestStep->setDescription( "Accuracy digits and best unit" );
        pTestStep->setDesiredValue( "(9.87654321 " + c_strSymbolPlusMinus + " 0.00000005) GHz" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 9876543210.12, Kinematics::Frequency()->Hertz(), 0.05, Kinematics::Frequency()->KiloHertz(), EResTypeAccuracy );
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindBest,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

    // Test: [description]
    //--------------------------------------------------------------------------
    {
        pTestStep = getTestStep( "CPhysVal(0.000009876543,GHz,120.0,Hz).toString()", pTSGrpAccuracyDigits );

        pTestStep->setOperation( "CPhysVal(0.000009876543,GHz,120.0,Hz).toString()" );
        pTestStep->setDescription( "Accuracy digits and best unit" );
        pTestStep->setDesiredValue( "(9.88 " + c_strSymbolPlusMinus + " 0.12) kHz" );

        try
        {
            pTestStep->testStarted();
            CPhysVal physVal( 0.000009876543, Kinematics::Frequency()->GigaHertz(), 120.0, Kinematics::Frequency()->Hertz(), EResTypeAccuracy );
            strActualValue = physVal.toString(
                /* unitFindVal          */ EUnitFindBest,
                /* iValSubStrVisibility */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
                /* unitFindRes          */ EUnitFindNone,
                /* iResSubStrVisibility */ EPhysValSubStrVal );
            pTestStep->setActualValue(strActualValue);
            pTestStep->testEnded();
        }
        catch( CException& exc )
        {
            pTestStep->setActualValue( exc.getResultStr() );
            pTestStep->testEnded();
        }
        catch(...)
        {
            pTestStep->setActualValue( "Unknown Exception thrown" );
            pTestStep->testEnded();
        }
    }

} // start
