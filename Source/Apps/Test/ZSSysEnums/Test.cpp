/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStepIdxTreeEntry.h"
#include "ZSTest/ZSTestStepIdxTree.h"
#include "ZSSys/ZSSysEnumTemplate.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Apps::Test::SysEnums;



/*******************************************************************************
class CTest
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//==============================================================================
enum class EProcessorClock
//==============================================================================
{
    Low    = 0,   // = 5.12 MHz
    Medium = 1,   // = 512.0 MHz
    High   = 2    // = 5.12 GHz
};

typedef CEnum<EProcessorClock> CEnumProcessorClock;

template<> QMutex CEnum<EProcessorClock>::s_mtxArMapsStr2Enumerators(QMutex::NonRecursive);
template<> QVector<QHash<QString, int>> CEnum<EProcessorClock>::s_armapsStr2Enumerators = QVector<QHash<QString, int>>();

template<> const QVector<SEnumEntry> CEnum<EProcessorClock>::s_arEnumEntries =
{   // idxRow,           Enumerator,                                Name,  Symbol, Text,                SCIPShort, SCPILong, Value
    /*  0 */ SEnumEntry( static_cast<int>(EProcessorClock::Low),    "Low",    "L", "Low (5.12 MHz)",     "LOW",    "LOW",    5.12e6  ),
    /*  1 */ SEnumEntry( static_cast<int>(EProcessorClock::Medium), "Medium", "M", "Medium (512.0 MHz)", "MED",    "MEDium", 512.0e6 ),
    /*  2 */ SEnumEntry( static_cast<int>(EProcessorClock::High),   "High",   "H", "High (5.12 GHz)",    "HIGH",   "HIGH",   5.12e9  )
};


/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest() :
//------------------------------------------------------------------------------
    ZS::Test::CTest("ZS::System::Enums")
{
    int idxStep = 0;

    ZS::Test::CTestStepGroup* pTstGrp;

    // Enum Entry Class Methods
    //-------------------------

    pTstGrp = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "Enum Entry Class Methods",
        /* pGrpParent */ nullptr );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SEnumEntry::enumerator2Str",
        /* strOperation    */ "SEnumEntry::enumerator2Str",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumEntryClassMethodsEnumerator2Str(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SEnumEntry::enumerator2Val",
        /* strOperation    */ "SEnumEntry::enumerator2Val",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumEntryClassMethodsEnumerator2Val(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SEnumEntry::str2Enumerator",
        /* strOperation    */ "SEnumEntry::str2Enumerator",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumEntryClassMethodsStr2Enumerator(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SEnumEntry::str2Val",
        /* strOperation    */ "SEnumEntry::str2Val",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumEntryClassMethodsStr2Val(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SEnumEntry::val2Enumerator",
        /* strOperation    */ "SEnumEntry::val2Enumerator",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumEntryClassMethodsVal2Enumerator(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SEnumEntry::val2Str",
        /* strOperation    */ "SEnumEntry::val2Str",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumEntryClassMethodsVal2Str(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SEnumEntry::isValidEnumerator",
        /* strOperation    */ "SEnumEntry::isValidEnumerator",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumEntryClassMethodsIsValidEnumerator(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SEnumEntry::initStr2EnumeratorMaps",
        /* strOperation    */ "SEnumEntry::initStr2EnumeratorMaps",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumEntryClassMethodsInitStr2EnumeratorMaps(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SEnumEntry::str2Enumerator(Maps)",
        /* strOperation    */ "SEnumEntry::str2Enumerator(Maps)",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumEntryClassMethodsMapStr2Enumerator(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " SEnumEntry::str2Val(Maps)",
        /* strOperation    */ "SEnumEntry::str2Val(Maps)",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumEntryClassMethodsMapStr2Val(ZS::Test::CTestStep*)) );

    // Enum Class Template ZS::System::Mode
    //-------------------------------------

    // Export of explicit instantiation of enum template from another Dll (ZSSys.dll).

    pTstGrp = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "Enum Class Template ZS::System::Mode",
        /* pGrpParent */ nullptr );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassZSSysMode::toString",
        /* strOperation    */ "EnumClassZSSysMode::toString",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateZSSysModeClassMethodToString(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassZSSysMode::toEnumerator",
        /* strOperation    */ "EnumClassZSSysMode::toEnumerator",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateZSSysModeClassMethodToEnumerator(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassZSSysMode::fromString",
        /* strOperation    */ "EnumClassZSSysMode::fromString",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateZSSysModeClassMethodFromString(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassZSSysMode::ctors",
        /* strOperation    */ "EnumClassZSSysMode::ctors",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateZSSysModeCtors(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassZSSysMode::operator=",
        /* strOperation    */ "EnumClassZSSysMode::operator=",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateZSSysModeOperatorAssign(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassZSSysMode::operator == != < > <= >= Enumerator",
        /* strOperation    */ "EnumClassZSSysMode::operator == != < > <= >= Enumerator",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateZSSysModeOperatorCompareWithEnumerator(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassZSSysMode::operator == != < > <= >= int",
        /* strOperation    */ "EnumClassZSSysMode::operator == != < > <= >= int",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateZSSysModeOperatorCompareWithInt(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassZSSysMode::operator == != < > <= >= const char*",
        /* strOperation    */ "EnumClassZSSysMode::operator == != < > <= >= const char*",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateZSSysModeOperatorCompareWithCharPtr(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassZSSysMode::operator == != < > <= >= QString",
        /* strOperation    */ "EnumClassZSSysMode::operator == != < > <= >= QString",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateZSSysModeOperatorCompareWithQString(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassZSSysMode::operator == != < > <= >= CEnumTemplate",
        /* strOperation    */ "EnumClassZSSysMode::operator == != < > <= >= CEnumTemplate",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateZSSysModeOperatorCompareWithEnumTemplate(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassZSSysMode::operator++--",
        /* strOperation    */ "EnumClassZSSysMode::operator++--",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateZSSysModeOperatorIncDec(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassZSSysMode.toString",
        /* strOperation    */ "EnumClassZSSysMode.toString",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateZSSysModeInstMethodToString(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassZSSysMode.Loops",
        /* strOperation    */ "EnumClassZSSysMode.Loops",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateZSSysModeForLoop(ZS::Test::CTestStep*)) );

    // Enum Class Template User Defined
    //---------------------------------

    pTstGrp = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "Enum Class Template User Defined",
        /* pGrpParent */ nullptr );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassTemplate::toString",
        /* strOperation    */ "EnumClassTemplate::toString",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateUserDefinedClassMethodToString(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassTemplate::toEnumerator",
        /* strOperation    */ "EnumClassTemplate::toEnumerator",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateUserDefinedClassMethodToEnumerator(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassTemplate::toValue",
        /* strOperation    */ "EnumClassTemplate::toValue",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateUserDefinedClassMethodToValue(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassTemplate::fromString",
        /* strOperation    */ "EnumClassTemplate::fromString",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateUserDefinedClassMethodFromString(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassTemplate::fromValue",
        /* strOperation    */ "EnumClassTemplate::fromValue",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateUserDefinedClassMethodFromValue(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassTemplate::ctors",
        /* strOperation    */ "EnumClassTemplate::ctors",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateUserDefinedCtors(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassTemplate::operator=",
        /* strOperation    */ "EnumClassTemplate::operator=",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateUserDefinedOperatorAssign(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassTemplate::operator == != < > <= >= Enumerator",
        /* strOperation    */ "EnumClassTemplate::operator == != < > <= >= Enumerator",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateUserDefinedOperatorCompareWithEnumerator(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassTemplate::operator == != < > <= >= int",
        /* strOperation    */ "EnumClassTemplate::operator == != < > <= >= int",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateUserDefinedOperatorCompareWithInt(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassTemplate::operator == != < > <= >= const char*",
        /* strOperation    */ "EnumClassTemplate::operator == != < > <= >= const char*",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateUserDefinedOperatorCompareWithCharPtr(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassTemplate::operator == != < > <= >= QString",
        /* strOperation    */ "EnumClassTemplate::operator == != < > <= >= QString",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateUserDefinedOperatorCompareWithQString(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassTemplate::operator == != < > <= >= CEnumTemplate",
        /* strOperation    */ "EnumClassTemplate::operator == != < > <= >= CEnumTemplate",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateUserDefinedOperatorCompareWithEnumTemplate(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassTemplate::operator++--",
        /* strOperation    */ "EnumClassTemplate::operator++--",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateUserDefinedOperatorIncDec(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassTemplate.toString",
        /* strOperation    */ "EnumClassTemplate.toString",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateUserDefinedInstMethodToString(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassTemplate.toVal",
        /* strOperation    */ "EnumClassTemplate.toVal",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateUserDefinedInstMethodToValue(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " EnumClassTemplate.Loops",
        /* strOperation    */ "EnumClassTemplate.Loops",
        /* pGrpParent      */ pTstGrp,
        /* szDoTestStepFct */ SLOT(doTestStepEnumClassTemplateUserDefinedForLoop(ZS::Test::CTestStep*)) );

    // Recall test step settings
    //--------------------------

    recallTestSteps();

} // default ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo = saveTestSteps();

    if(errResultInfo.isErrorResult())
    {
        if(CErrLog::GetInstance() != nullptr)
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

} // dtor

/*==============================================================================
public: // test step methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsEnumerator2Str( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    EProcessorClock enumerator;
    QString         strEnumerator;
    QString         strResultExpected;

    int  iEnumerator;
    int  idxAlias;

    const SEnumEntry* pEnumArr = CEnumProcessorClock::s_arEnumEntries.data();
    const int         iEnumArrLen = CEnumProcessorClock::s_arEnumEntries.count();

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    {
        if( iEnumerator == 0 ) { enumerator = EProcessorClock::Low; strEnumerator = "Low"; }
        else if( iEnumerator == 1 ) { enumerator = EProcessorClock::Medium; strEnumerator = "Medium"; }
        else if( iEnumerator == 2 ) { enumerator = EProcessorClock::High; strEnumerator = "High"; }
        else { enumerator = static_cast<EProcessorClock>(iEnumerator); strEnumerator = QString::number(static_cast<int>(enumerator)); }

        strExpectedValue = "SEnumEntry::enumerator2Str(" + strEnumerator + "): " + strEnumerator;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "SEnumEntry::enumerator2Str(" + strEnumerator + "): ";
        strResultValue += SEnumEntry::enumerator2Str(pEnumArr, iEnumArrLen, static_cast<int>(enumerator));
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )
    {
        for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
        {
            if( iEnumerator == 0 )
            {
                enumerator = EProcessorClock::Low;
                strEnumerator = "Low";
                if( idxAlias == EEnumEntryAliasStrName ) strResultExpected = "Low";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strResultExpected = "L";
                else if( idxAlias == EEnumEntryAliasStrText ) strResultExpected = "Low (5.12 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strResultExpected = "LOW";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strResultExpected = "LOW";
            }
            else if( iEnumerator == 1 )
            {
                enumerator = EProcessorClock::Medium;
                strEnumerator = "Medium";
                if( idxAlias == EEnumEntryAliasStrName ) strResultExpected = "Medium";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strResultExpected = "M";
                else if( idxAlias == EEnumEntryAliasStrText ) strResultExpected = "Medium (512.0 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strResultExpected = "MED";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strResultExpected = "MEDium";
            }
            else if( iEnumerator == 2 )
            {
                enumerator = EProcessorClock::High;
                strEnumerator = "High";
                if( idxAlias == EEnumEntryAliasStrName ) strResultExpected = "High";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strResultExpected = "H";
                else if( idxAlias == EEnumEntryAliasStrText ) strResultExpected = "High (5.12 GHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strResultExpected = "HIGH";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strResultExpected = "HIGH";
            }
            else
            {
                enumerator = static_cast<EProcessorClock>(iEnumerator);
                strEnumerator = QString::number(static_cast<int>(enumerator));
                strResultExpected = "?, ?";
            }

            strExpectedValue = "SEnumEntry::enumerator2Str(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strExpectedValue += strResultExpected;
            strlstExpectedValues.append(strExpectedValue);

            strResultValue = "SEnumEntry::enumerator2Str(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strEnumerator = SEnumEntry::enumerator2Str(pEnumArr, iEnumArrLen, static_cast<int>(enumerator), idxAlias);
            strResultValue += strEnumerator;
            strlstResultValues.append(strResultValue);
        }
    }

    // -------------------------------------------------------------------------

    idxAlias = EEnumEntryAliasStrAlias6;

    for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    {
        if( iEnumerator == 0 ) { enumerator = EProcessorClock::Low; strEnumerator = "Low"; }
        else if( iEnumerator == 1 ) { enumerator = EProcessorClock::Medium; strEnumerator = "Medium"; }
        else if( iEnumerator == 2 ) { enumerator = EProcessorClock::High; strEnumerator = "High"; }
        else { enumerator = static_cast<EProcessorClock>(iEnumerator); strEnumerator = QString::number(static_cast<int>(enumerator)); }

        strExpectedValue  = "SEnumEntry::enumerator2Str(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
        strExpectedValue += "Enumerator " + strEnumerator + " does not have an alias string at index " + QString::number(idxAlias);
        strlstExpectedValues.append(strExpectedValue);

        try
        {
            strResultValue = "SEnumEntry::enumerator2Str(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strEnumerator = SEnumEntry::enumerator2Str(pEnumArr, iEnumArrLen, static_cast<int>(enumerator), idxAlias);
            strResultValue += strEnumerator;
        }
        catch( CException& exc )
        {
            strResultValue += exc.getAddErrInfo();
        }
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    strExpectedValue = "SEnumEntry::enumerator2Str(-1): Enumerator -1 not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        iEnumerator = -1;
        enumerator = static_cast<EProcessorClock>(iEnumerator);
        strResultValue = "SEnumEntry::enumerator2Str(" + QString::number(static_cast<int>(enumerator)) + "): ";
        strResultValue += SEnumEntry::enumerator2Str(pEnumArr, iEnumArrLen, static_cast<int>(enumerator));
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    strExpectedValue = "SEnumEntry::enumerator2Str(3): Enumerator 3 not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        iEnumerator = iEnumArrLen;
        enumerator = static_cast<EProcessorClock>(iEnumerator);
        strResultValue = "SEnumEntry::enumerator2Str(" + QString::number(static_cast<int>(enumerator)) + "): ";
        strResultValue += SEnumEntry::enumerator2Str(pEnumArr, iEnumArrLen, static_cast<int>(enumerator));
    }
    catch( CException& exc )
    {
        strResultValue = "SEnumEntry::enumerator2Str(" + QString::number(iEnumerator) + "): " + exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumEntryClassMethodsEnumerator2Str

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsEnumerator2Val( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    EProcessorClock enumerator;
    QString         strEnumerator;

    int      iEnumerator;
    bool     bOk;
    QVariant valExpected;
    QVariant valResult;
    QString  strValResult;

    const SEnumEntry* pEnumArr = CEnumProcessorClock::s_arEnumEntries.data();
    const int         iEnumArrLen = CEnumProcessorClock::s_arEnumEntries.count();

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    {
        if( iEnumerator == 0 )
        {
            enumerator = EProcessorClock::Low;
            strEnumerator = "Low";
            valExpected = 5.12e6;
        }
        else if( iEnumerator == 1 )
        {
            enumerator = EProcessorClock::Medium;
            strEnumerator = "Medium";
            valExpected = 5.12e8;
        }
        else if( iEnumerator == 2 )
        {
            enumerator = EProcessorClock::High;
            strEnumerator = "High";
            valExpected = 5.12e9;
        }
        else
        {
            enumerator = static_cast<EProcessorClock>(iEnumerator);
            strEnumerator = QString::number(static_cast<int>(enumerator));
            valExpected = QVariant();
        }

        strExpectedValue = "SEnumEntry::enumerator2Val(" + strEnumerator + "): ";
        if( valExpected.canConvert(QVariant::Double) ) strExpectedValue += QString::number(valExpected.toDouble(),'f',1);
        strlstExpectedValues.append(strExpectedValue);

        valResult = SEnumEntry::enumerator2Val(pEnumArr, iEnumArrLen, static_cast<int>(enumerator));
        strResultValue = "SEnumEntry::enumerator2Val(" + strEnumerator + "): ";
        if( valResult.canConvert(QVariant::Double) ) strResultValue += QString::number(valResult.toDouble(),'f',1);
        strlstResultValues.append(strResultValue);
    }

    strExpectedValue = "SEnumEntry::enumerator2Val(-1): Enumerator -1 not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        iEnumerator = -1;
        enumerator = static_cast<EProcessorClock>(iEnumerator);
        strEnumerator = QString::number(static_cast<int>(enumerator));
        strResultValue = "SEnumEntry::enumerator2Val(" + strEnumerator + "): ";
        valResult = SEnumEntry::enumerator2Val(pEnumArr, iEnumArrLen, static_cast<int>(enumerator));
        if( valResult.canConvert(QVariant::Double) ) strResultValue += QString::number(valResult.toDouble(),'f',1);
    }
    catch( CException& exc )
    {
        strResultValue = "SEnumEntry::enumerator2Val(" + strEnumerator + "): " + exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    strExpectedValue = "SEnumEntry::enumerator2Val(3): Enumerator 3 not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        iEnumerator = iEnumArrLen;
        enumerator = static_cast<EProcessorClock>(iEnumerator);
        strEnumerator = QString::number(static_cast<int>(enumerator));
        strResultValue = "SEnumEntry::enumerator2Val(" + strEnumerator + "): ";
        valResult = SEnumEntry::enumerator2Val(pEnumArr, iEnumArrLen, static_cast<int>(enumerator));
        if( valResult.canConvert(QVariant::Double) ) strResultValue += QString::number(valResult.toDouble(),'f',1);
    }
    catch( CException& exc )
    {
        strResultValue = "SEnumEntry::enumerator2Val(" + strEnumerator + "): " + exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    {
        if( iEnumerator == 0 ) { enumerator = EProcessorClock::Low; strEnumerator = "Low"; strValResult = "5120000.0"; }
        else if( iEnumerator == 1 ) { enumerator = EProcessorClock::Medium; strEnumerator = "Medium"; strValResult = "512000000.0"; }
        else if( iEnumerator == 2 ) { enumerator = EProcessorClock::High; strEnumerator = "High"; strValResult = "5120000000.0"; }
        else { enumerator = static_cast<EProcessorClock>(iEnumerator); strEnumerator = QString::number(static_cast<int>(enumerator)); strValResult = ""; }

        strExpectedValue = "SEnumEntry::enumerator2Val(" + strEnumerator + ", Double, &bOk): " + strValResult + ", true";
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "SEnumEntry::enumerator2Val(" + strEnumerator + ", Double, &bOk): ";
        valResult = SEnumEntry::enumerator2Val(pEnumArr, iEnumArrLen, static_cast<int>(enumerator), QVariant::Double, &bOk);
        if( bOk ) strResultValue += QString::number(valResult.toDouble(),'f',1) + ", " + bool2Str(bOk);
        else strResultValue += valResult.toString() + ", " + bool2Str(bOk);
        strlstResultValues.append(strResultValue);
    }

    try
    {
        iEnumerator = -1;
        enumerator = static_cast<EProcessorClock>(iEnumerator);
        strEnumerator = QString::number(static_cast<int>(enumerator));

        strExpectedValue = "SEnumEntry::enumerator2Val(" + strEnumerator + ", Double, &bOk): ";
        strExpectedValue += "Enumerator " + strEnumerator + " not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "SEnumEntry::enumerator2Val(" + strEnumerator + ", Double, &bOk): ";
        valResult = SEnumEntry::enumerator2Val(pEnumArr, iEnumArrLen, static_cast<int>(enumerator), QVariant::Double, &bOk);
        if( bOk ) strResultValue += QString::number(valResult.toDouble(),'f',1) + ", " + bool2Str(bOk);
        else strResultValue += valResult.toString() + ", " + bool2Str(bOk);
    }
    catch( CException& exc )
    {
        strResultValue = "SEnumEntry::enumerator2Val(" + QString::number(iEnumerator) + ", Double, &bOk): " + exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        iEnumerator = iEnumArrLen;
        enumerator = static_cast<EProcessorClock>(iEnumerator);
        strEnumerator = QString::number(static_cast<int>(enumerator));

        strExpectedValue = "SEnumEntry::enumerator2Val(" + strEnumerator + ", Double, &bOk): ";
        strExpectedValue += "Enumerator " + strEnumerator + " not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "SEnumEntry::enumerator2Val(" + strEnumerator + ", Double, &bOk): ";
        valResult = SEnumEntry::enumerator2Val(pEnumArr, iEnumArrLen, static_cast<int>(enumerator), QVariant::Double, &bOk);
        if( bOk ) strResultValue += QString::number(valResult.toDouble(),'f',1) + ", " + bool2Str(bOk);
        else strResultValue += valResult.toString() + ", " + bool2Str(bOk);
    }
    catch( CException& exc )
    {
        strResultValue = "SEnumEntry::enumerator2Val(" + strEnumerator + ", Double, &bOk): " + exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumEntryClassMethodsEnumerator2Val

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsStr2Enumerator( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    EProcessorClock     enumerator;
    QString             strEnumeratorSource;
    QString             strEnumeratorResult;
    QString             strAlias;
    Qt::CaseSensitivity caseSensitivity;
    QString             strCaseSensitivity;

    int  iEnumerator;
    int  idxAlias;
    bool bOk;

    const SEnumEntry* pEnumArr = CEnumProcessorClock::s_arEnumEntries.data();
    const int         iEnumArrLen = CEnumProcessorClock::s_arEnumEntries.count();

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    {
        if( iEnumerator == 0 ) { strEnumeratorSource = "Low"; }
        else if( iEnumerator == 1 ) { strEnumeratorSource = "Medium"; }
        else if( iEnumerator == 2 ) { strEnumeratorSource = "High"; }
        else { strEnumeratorSource = QString::number(iEnumerator); }

        strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + "): ";
        strExpectedValue += strEnumeratorSource;
        strlstExpectedValues.append(strExpectedValue);

        enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource));

        if( enumerator == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumerator == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumerator == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumerator)); }

        strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + "): ";
        strResultValue += strEnumeratorResult;
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )
    {
        strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));

        for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
        {
            if( iEnumerator == 0 )
            {
                enumerator = EProcessorClock::Low;
                strEnumeratorResult = "Low";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Low";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "L";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Low (5.12 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "LOW";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "LOW";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 1 )
            {
                enumerator = EProcessorClock::Medium;
                strEnumeratorResult = "Medium";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Medium";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "M";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Medium (512.0 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "MED";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "MEDium";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 2 )
            {
                enumerator = EProcessorClock::High;
                strEnumeratorResult = "High";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "High";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "H";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "High (5.12 GHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "HIGH";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "HIGH";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else
            {
                enumerator = static_cast<EProcessorClock>(iEnumerator);
                strEnumeratorResult = QString::number(iEnumerator);
                strEnumeratorSource = QString::number(iEnumerator);
            }

            strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + "): ";
            strExpectedValue += strEnumeratorResult;
            strlstExpectedValues.append(strExpectedValue);

            strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + "): ";
            enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource, idxAlias));
            if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
            else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
            else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
            else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
            strResultValue += strEnumeratorResult;
            strlstResultValues.append(strResultValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias6;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumEntryClassMethodsStr2Enumerator

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsStr2Val( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    QString             strEnumeratorSource;
    QVariant            valResult;
    QString             strValResult;
    QString             strAlias;
    Qt::CaseSensitivity caseSensitivity;
    QString             strCaseSensitivity;

    int  iEnumerator;
    int  idxAlias;
    bool bOk;

    const SEnumEntry* pEnumArr = CEnumProcessorClock::s_arEnumEntries.data();
    const int         iEnumArrLen = CEnumProcessorClock::s_arEnumEntries.count();

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    {
        if( iEnumerator == 0 ) { strEnumeratorSource = "Low"; strValResult = "5120000.0"; }
        else if( iEnumerator == 1 ) { strEnumeratorSource = "Medium"; strValResult = "512000000.0"; }
        else if( iEnumerator == 2 ) { strEnumeratorSource = "High"; strValResult = "5120000000.0"; }
        else { strEnumeratorSource = ""; strValResult = ""; }

        strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + "): ";
        strExpectedValue += strValResult;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + "): ";
        valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource);
        if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'f',1);
        else strValResult = "";
        strResultValue += strValResult;
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )
    {
        strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));

        for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
        {
            if( iEnumerator == 0 )
            {
                strValResult = QString::number(5.12e6,'f',1);

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Low";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "L";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Low (5.12 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "LOW";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "LOW";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 1 )
            {
                strValResult = QString::number(5.12e8,'f',1);

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Medium";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "M";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Medium (512.0 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "MED";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "MEDium";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 2 )
            {
                strValResult = QString::number(5.12e9,'f',1);

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "High";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "H";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "High (5.12 GHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "HIGH";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "HIGH";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else
            {
                strValResult = "";
            }

            strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", Double, " + strAlias + "): ";
            strExpectedValue += strValResult;
            strlstExpectedValues.append(strExpectedValue);

            strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", Double, " + strAlias + "): ";
            valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Double, idxAlias);
            if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'f',1);
            else strValResult = "";
            strResultValue += strValResult;
            strlstResultValues.append(strResultValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = QString::number(5.12e8,'f',1);

    strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strValResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'f',1);
    else strValResult = "";
    strResultValue += strValResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "";

    strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strValResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'f',1);
    else strValResult = "";
    strResultValue += strValResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = QString::number(5.12e8,'f',1);

    strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strValResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'f',1);
    else strValResult = "";
    strResultValue += strValResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "512000000";

    strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Int, " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strValResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Int, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Int, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Int) ) strValResult = QString::number(valResult.toInt());
    else strValResult = "";
    strResultValue += strValResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "";

    strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strValResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'f',1);
    else strValResult = "";
    strResultValue += strValResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = QString::number(5.12e8,'f',1);

    strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strValResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'f',1);
    else strValResult = "";
    strResultValue += strValResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias6;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "";

    strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strValResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'f',1);
    else strValResult = "";
    strResultValue += strValResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumEntryClassMethodsStr2Val

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsVal2Enumerator( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    EProcessorClock enumerator;
    QString         strEnumerator;
    QVariant        val;

    int  iEnumerator;
    bool bOk;

    const SEnumEntry* pEnumArr = CEnumProcessorClock::s_arEnumEntries.data();
    const int         iEnumArrLen = CEnumProcessorClock::s_arEnumEntries.count();

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    {
        val = QVariant();
        enumerator = static_cast<EProcessorClock>(EInvalidEnumerator);
        strEnumerator = "-1";

        if( iEnumerator == 0 ) { val = 5.12e6; strEnumerator = "Low"; }
        else if( iEnumerator == 1 ) { val = 5.12e8; strEnumerator = "Medium"; }
        else if( iEnumerator == 2 ) { val = 5.12e9; strEnumerator = "High"; }

        strExpectedValue = "SEnumEntry::val2Enumerator(" + val.toString() + "): ";
        strExpectedValue += strEnumerator;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "SEnumEntry::val2Enumerator(" + val.toString() + "): ";
        enumerator = static_cast<EProcessorClock>(SEnumEntry::val2Enumerator(pEnumArr, iEnumArrLen, val));

        if( enumerator == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( enumerator == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( enumerator == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(enumerator)); }

        strResultValue += strEnumerator;
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    val = 5.12e8;

    strExpectedValue = "SEnumEntry::val2Enumerator(" + val.toString() + ", &bOk): ";
    strExpectedValue += "Medium, true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::val2Enumerator(" + val.toString() + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::val2Enumerator(pEnumArr, iEnumArrLen, val, &bOk));
    if( enumerator == EProcessorClock::Low ) { strEnumerator = "Low"; }
    else if( enumerator == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
    else if( enumerator == EProcessorClock::High ) { strEnumerator = "High"; }
    else { strEnumerator = QString::number(static_cast<int>(enumerator)); }
    strResultValue += strEnumerator + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    val = QVariant();

    strExpectedValue = "SEnumEntry::val2Enumerator(" + val.toString() + ", &bOk): ";
    strExpectedValue += "-1, false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::val2Enumerator(" + val.toString() + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::val2Enumerator(pEnumArr, iEnumArrLen, val, &bOk));
    if( enumerator == EProcessorClock::Low ) { strEnumerator = "Low"; }
    else if( enumerator == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
    else if( enumerator == EProcessorClock::High ) { strEnumerator = "High"; }
    else { strEnumerator = QString::number(static_cast<int>(enumerator)); }
    strResultValue += strEnumerator + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    val = 5.12e3;

    strExpectedValue = "SEnumEntry::val2Enumerator(" + val.toString() + ", &bOk): ";
    strExpectedValue += "-1, false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::val2Enumerator(" + val.toString() + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::val2Enumerator(pEnumArr, iEnumArrLen, val, &bOk));
    if( enumerator == EProcessorClock::Low ) { strEnumerator = "Low"; }
    else if( enumerator == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
    else if( enumerator == EProcessorClock::High ) { strEnumerator = "High"; }
    else { strEnumerator = QString::number(static_cast<int>(enumerator)); }
    strResultValue += strEnumerator + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumEntryClassMethodsVal2Enumerator

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsVal2Str( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    QString  strEnumerator;
    QString  strValResult;
    QVariant val;

    int  iEnumerator;
    int  idxAlias;
    bool bOk;

    const SEnumEntry* pEnumArr = CEnumProcessorClock::s_arEnumEntries.data();
    const int         iEnumArrLen = CEnumProcessorClock::s_arEnumEntries.count();

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    {
        val = QVariant();
        strValResult = "";

        if( iEnumerator == 0 ) { val = 5.12e6; strValResult = "Low"; }
        else if( iEnumerator == 1 ) { val = 5.12e8; strValResult = "Medium"; }
        else if( iEnumerator == 2 ) { val = 5.12e9; strValResult = "High"; }

        strExpectedValue = "SEnumEntry::val2Str(" + val.toString() + "): ";
        strExpectedValue += strValResult;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "SEnumEntry::val2Str(" + val.toString() + "): ";
        strValResult = SEnumEntry::val2Str(pEnumArr, iEnumArrLen, val);
        strResultValue += strValResult;
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )
    {
        for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
        {
            val = QVariant();
            strValResult = "";
            bOk = false;

            if( iEnumerator == 0 )
            {
                val = 5.12e6;
                bOk = true;
                if( idxAlias == EEnumEntryAliasStrName ) { strValResult = "Low"; }
                else if( idxAlias == EEnumEntryAliasStrSymbol ) { strValResult = "L"; }
                else if( idxAlias == EEnumEntryAliasStrText ) { strValResult = "Low (5.12 MHz)"; }
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) { strValResult = "LOW"; }
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) { strValResult = "LOW"; }
            }
            else if( iEnumerator == 1 )
            {
                val = 5.12e8;
                bOk = true;
                if( idxAlias == EEnumEntryAliasStrName ) { strValResult = "Medium"; }
                else if( idxAlias == EEnumEntryAliasStrSymbol ) { strValResult = "M"; }
                else if( idxAlias == EEnumEntryAliasStrText ) { strValResult = "Medium (512.0 MHz)"; }
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) { strValResult = "MED"; }
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) { strValResult = "MEDium"; }
            }
            else if( iEnumerator == 2 )
            {
                val = 5.12e9;
                bOk = true;
                if( idxAlias == EEnumEntryAliasStrName ) { strValResult = "High"; }
                else if( idxAlias == EEnumEntryAliasStrSymbol ) { strValResult = "H"; }
                else if( idxAlias == EEnumEntryAliasStrText ) { strValResult = "High (5.12 GHz)"; }
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) { strValResult = "HIGH"; }
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) { strValResult = "HIGH"; }
            }

            strExpectedValue = "SEnumEntry::val2Str(" + val.toString() + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + ", &bOk): ";
            strExpectedValue += strValResult + ", " + bool2Str(bOk);
            strlstExpectedValues.append(strExpectedValue);

            strResultValue = "SEnumEntry::val2Str(" + val.toString() + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + ", &bOk): ";
            strValResult = SEnumEntry::val2Str(pEnumArr, iEnumArrLen, val, idxAlias, &bOk);
            strResultValue += strValResult + ", " + bool2Str(bOk);
            strlstResultValues.append(strResultValue);
        }
    }

    idxAlias = EEnumEntryAliasStrAlias6;

    for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    {
        val = QVariant();
        strValResult = "";
        bOk = false;

        if( iEnumerator == 0 ) { val = 5.12e6; strEnumerator = "Low"; }
        else if( iEnumerator == 1 ) { val = 5.12e8; strEnumerator = "Medium"; }
        else if( iEnumerator == 2 ) { val = 5.12e9; strEnumerator = "High"; }

        strExpectedValue = "SEnumEntry::val2Str(" + val.toString() + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + ", &bOk): ";
        if( iEnumerator <= 2 ) strExpectedValue += "Enumerator " + strEnumerator + " does not have an alias string at index " + QString::number(idxAlias);
        else strExpectedValue += ", false";
        strlstExpectedValues.append(strExpectedValue);

        try
        {
            strResultValue = "SEnumEntry::val2Str(" + val.toString() + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + ", &bOk): ";
            strValResult = SEnumEntry::val2Str(pEnumArr, iEnumArrLen, val, idxAlias, &bOk);
            strResultValue += strValResult + ", " + bool2Str(bOk);
        }
        catch( CException& exc )
        {
            strResultValue += exc.getAddErrInfo();
        }
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumEntryClassMethodsVal2Str

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsIsValidEnumerator( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    QString strEnumerator;
    bool    bOk;

    int iEnumerator;

    const SEnumEntry* pEnumArr = CEnumProcessorClock::s_arEnumEntries.data();
    const int         iEnumArrLen = CEnumProcessorClock::s_arEnumEntries.count();

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    {
        strEnumerator = "";
        bOk = false;
        if( iEnumerator == 0 ) { strEnumerator = "Low"; bOk = true; }
        if( iEnumerator == 1 ) { strEnumerator = "Medium"; bOk = true; }
        if( iEnumerator == 2 ) { strEnumerator = "High"; bOk = true; }
        strExpectedValue = "SEnumEntry::isValidEnumerator(" + strEnumerator + "): " + bool2Str(bOk);
        strlstExpectedValues.append(strExpectedValue);

        bOk = SEnumEntry::isValidEnumerator(pEnumArr, iEnumArrLen, iEnumerator);
        strResultValue = "SEnumEntry::isValidEnumerator(" + strEnumerator + "): " + bool2Str(bOk);
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumerator = QString::number(iEnumerator);
    bOk = false;
    strExpectedValue = "SEnumEntry::isValidEnumerator(" + strEnumerator + "): " + bool2Str(bOk);
    strlstExpectedValues.append(strExpectedValue);

    bOk = SEnumEntry::isValidEnumerator(pEnumArr, iEnumArrLen, iEnumerator);
    strResultValue = "SEnumEntry::isValidEnumerator(" + strEnumerator + "): " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    iEnumerator = 5;
    strEnumerator = QString::number(iEnumerator);
    bOk = false;
    strExpectedValue = "SEnumEntry::isValidEnumerator(" + strEnumerator + "): " + bool2Str(bOk);
    strlstExpectedValues.append(strExpectedValue);

    bOk = SEnumEntry::isValidEnumerator(pEnumArr, iEnumArrLen, iEnumerator);
    strResultValue = "SEnumEntry::isValidEnumerator(" + strEnumerator + "): " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumEntryClassMethodsIsValidEnumerator

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsInitStr2EnumeratorMaps( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    const SEnumEntry* pEnumArr = CEnumProcessorClock::s_arEnumEntries.data();
    const int         iEnumArrLen = CEnumProcessorClock::s_arEnumEntries.count();

    QString strEnumerator;

    int idxAlias;
    int iEnumerator;

    QVector<QHash<QString, int>> armapsStr2Enumerators;

    SEnumEntry::initStr2EnumeratorMaps(pEnumArr, iEnumArrLen, armapsStr2Enumerators, nullptr);

    strExpectedValue = "AliasCount = " + QString::number(EEnumEntryAliasStrSCPILong + 1);
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "AliasCount = " + QString::number(armapsStr2Enumerators.size());
    strlstResultValues.append(strResultValue);

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )
    {
        for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
        {
            if( iEnumerator == 0 )
            {
                if( idxAlias == EEnumEntryAliasStrName ) { strEnumerator = "Low"; }
                else if( idxAlias == EEnumEntryAliasStrSymbol ) { strEnumerator = "L"; }
                else if( idxAlias == EEnumEntryAliasStrText ) { strEnumerator = "Low (5.12 MHz)"; }
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) { strEnumerator = "LOW"; }
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) { strEnumerator = "LOW"; }
            }
            else if( iEnumerator == 1 )
            {
                if( idxAlias == EEnumEntryAliasStrName ) { strEnumerator = "Medium"; }
                else if( idxAlias == EEnumEntryAliasStrSymbol ) { strEnumerator = "M"; }
                else if( idxAlias == EEnumEntryAliasStrText ) { strEnumerator = "Medium (512.0 MHz)"; }
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) { strEnumerator = "MED"; }
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) { strEnumerator = "MEDium"; }
            }
            else if( iEnumerator == 2 )
            {
                if( idxAlias == EEnumEntryAliasStrName ) { strEnumerator = "High"; }
                else if( idxAlias == EEnumEntryAliasStrSymbol ) { strEnumerator = "H"; }
                else if( idxAlias == EEnumEntryAliasStrText ) { strEnumerator = "High (5.12 GHz)"; }
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) { strEnumerator = "HIGH"; }
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) { strEnumerator = "HIGH"; }
            }

            strExpectedValue = "armapsStr2Enumerators[" + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "][" + strEnumerator + "]: " + QString::number(iEnumerator);
            strlstExpectedValues.append(strExpectedValue);

            try
            {
                iEnumerator = armapsStr2Enumerators[idxAlias][strEnumerator];
                strResultValue = "armapsStr2Enumerators[" + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "][" + strEnumerator + "]: " + QString::number(iEnumerator);
            }
            catch(...)
            {
                strResultValue = "Unexpected exception thrown";
            }
            strlstResultValues.append(strResultValue);
        }
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumEntryClassMethodsInitStr2EnumeratorMaps

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsMapStr2Enumerator( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    EProcessorClock     enumerator;
    QString             strEnumeratorSource;
    QString             strEnumeratorResult;
    QString             strAlias;
    Qt::CaseSensitivity caseSensitivity;
    QString             strCaseSensitivity;

    int  iEnumerator;
    int  idxAlias;
    bool bOk;

    const SEnumEntry* pEnumArr = CEnumProcessorClock::s_arEnumEntries.data();
    const int         iEnumArrLen = CEnumProcessorClock::s_arEnumEntries.count();

    QVector<QHash<QString, int>> armapsStr2Enumerators;

    SEnumEntry::initStr2EnumeratorMaps(pEnumArr, iEnumArrLen, armapsStr2Enumerators, nullptr);

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    {
        if( iEnumerator == 0 ) { strEnumeratorSource = "Low"; strEnumeratorResult = "Low"; }
        else if( iEnumerator == 1 ) { strEnumeratorSource = "Medium"; strEnumeratorResult = "Medium"; }
        else if( iEnumerator == 2 ) { strEnumeratorSource = "High"; strEnumeratorResult = "High"; }
        else { strEnumeratorSource = QString::number(iEnumerator); }

        strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + "): ";
        strExpectedValue += strEnumeratorResult;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + "): ";
        enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource));
        if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
        else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
        else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
        else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
        strResultValue += strEnumeratorResult;
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )
    {
        strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));

        for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
        {
            if( iEnumerator == 0 )
            {
                enumerator = EProcessorClock::Low;
                strEnumeratorResult = "Low";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Low";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "L";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Low (5.12 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "LOW";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "LOW";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 1 )
            {
                enumerator = EProcessorClock::Medium;
                strEnumeratorResult = "Medium";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Medium";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "M";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Medium (512.0 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "MED";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "MEDium";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 2 )
            {
                enumerator = EProcessorClock::High;
                strEnumeratorResult = "High";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "High";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "H";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "High (5.12 GHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "HIGH";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "HIGH";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else
            {
                enumerator = static_cast<EProcessorClock>(iEnumerator);
                strEnumeratorResult = QString::number(iEnumerator);
                strEnumeratorSource = QString::number(iEnumerator);
            }

            strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + "): ";
            strExpectedValue += strEnumeratorResult;
            strlstExpectedValues.append(strExpectedValue);

            strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + "): ";
            enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource, idxAlias));
            if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
            else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
            else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
            else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
            strResultValue += strEnumeratorResult;
            strlstResultValues.append(strResultValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias6;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strExpectedValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += "Map does not contain alias strings at index " + QString::number(idxAlias);
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
        enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
        if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
        else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
        else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
        else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
        strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumEntryClassMethodsMapStr2Enumerator

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsMapStr2Val( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    QString             strEnumeratorSource;
    QVariant            valResult;
    QString             strValResult;
    QString             strAlias;
    Qt::CaseSensitivity caseSensitivity;
    QString             strCaseSensitivity;

    int  iEnumerator;
    int  idxAlias;
    bool bOk;

    const SEnumEntry* pEnumArr = CEnumProcessorClock::s_arEnumEntries.data();
    const int         iEnumArrLen = CEnumProcessorClock::s_arEnumEntries.count();

    QVector<QHash<QString, int>> armapsStr2Enumerators;

    SEnumEntry::initStr2EnumeratorMaps(pEnumArr, iEnumArrLen, armapsStr2Enumerators, nullptr);

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    {
        if( iEnumerator == 0 ) { strEnumeratorSource = "Low"; strValResult = QString::number(5.12e6,'f',1); }
        else if( iEnumerator == 1 ) { strEnumeratorSource = "Medium"; strValResult = QString::number(5.12e8,'f',1); }
        else if( iEnumerator == 2 ) { strEnumeratorSource = "High"; strValResult = QString::number(5.12e9,'f',1); }
        else { strEnumeratorSource = ""; strValResult = ""; }

        strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + "): ";
        strExpectedValue += strValResult;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + "): ";
        valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource);
        if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'f',1);
        else strValResult = "";
        strResultValue += strValResult;
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )
    {
        strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));

        for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
        {
            if( iEnumerator == 0 )
            {
                strValResult = QString::number(5.12e6,'f',1);

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Low";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "L";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Low (5.12 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "LOW";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "LOW";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 1 )
            {
                strValResult = QString::number(5.12e8,'f',1);

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Medium";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "M";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Medium (512.0 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "MED";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "MEDium";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 2 )
            {
                strValResult = QString::number(5.12e9,'f',1);

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "High";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "H";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "High (5.12 GHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "HIGH";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "HIGH";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else
            {
                strValResult = "";
            }

            strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", Double, " + strAlias + "): ";
            strExpectedValue += strValResult;
            strlstExpectedValues.append(strExpectedValue);

            strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", Double, " + strAlias + "): ";
            valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Double, idxAlias);
            if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'f',1);
            else strValResult = "";
            strResultValue += strValResult;
            strlstResultValues.append(strResultValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = QString::number(5.12e8,'f',1);

    strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strValResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'f',1);
    else strValResult = "";
    strResultValue += strValResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "";

    strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strValResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'f',1);
    else strValResult = "";
    strResultValue += strValResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = QString::number(5.12e8,'f',1);

    strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strValResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'f',1);
    else strValResult = "";
    strResultValue += strValResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "512000000";

    strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Int, " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strValResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Int, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Int, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Int) ) strValResult = QString::number(valResult.toInt());
    else strValResult = "";
    strResultValue += strValResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "";

    strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strValResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'f',1);
    else strValResult = "";
    strResultValue += strValResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = QString::number(5.12e8,'f',1);

    strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strValResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'f',1);
    else strValResult = "";
    strResultValue += strValResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias6;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "";

    strExpectedValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += "Map does not contain alias strings at index " + QString::number(idxAlias);
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
        valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
        if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'f',1);
        else strValResult = "";
        strResultValue += strValResult + ", " + bool2Str(bOk);
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumEntryClassMethodsMapStr2Val

/*==============================================================================
public: // test step methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeClassMethodToString( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    EMode enumerator;

    QString strEnumerator;
    QString strResultExpected;

    int  iEnumerator;
    int  idxAlias;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumMode::count(); ++iEnumerator )
    {
        if( iEnumerator == 0 ) { enumerator = EMode::Edit; strEnumerator = "Edit"; }
        else if( iEnumerator == 1 ) { enumerator = EMode::Simulation; strEnumerator = "Simulation"; }
        else if( iEnumerator == 2 ) { enumerator = EMode::Simulation; strEnumerator = "Undefined"; }
        else { enumerator = static_cast<EMode>(iEnumerator); strEnumerator = QString::number(iEnumerator); }

        strExpectedValue = "CEnumMode::toString(" + strEnumerator + "): " + strEnumerator;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "CEnumMode::toString(" + strEnumerator + "): " + CEnumMode::toString(static_cast<EMode>(iEnumerator));
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrText; ++idxAlias )
    {
        for( iEnumerator = 0; iEnumerator < CEnumMode::count(); ++iEnumerator )
        {
            if( iEnumerator == 0 )
            {
                enumerator = EMode::Edit;
                strEnumerator = "Edit";
                if( idxAlias == EEnumEntryAliasStrName ) strResultExpected = "Edit";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strResultExpected = "E";
                else if( idxAlias == EEnumEntryAliasStrText ) strResultExpected = "Edit";
            }
            else if( iEnumerator == 1 )
            {
                enumerator = EMode::Simulation;
                strEnumerator = "Simulation";
                if( idxAlias == EEnumEntryAliasStrName ) strResultExpected = "Simulation";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strResultExpected = "S";
                else if( idxAlias == EEnumEntryAliasStrText ) strResultExpected = "Simulation";
            }
            else if( iEnumerator == 2 )
            {
                enumerator = EMode::Undefined;
                strEnumerator = "Undefined";
                if( idxAlias == EEnumEntryAliasStrName ) strResultExpected = "Undefined";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strResultExpected = "?";
                else if( idxAlias == EEnumEntryAliasStrText ) strResultExpected = "Undefined";
            }
            else
            {
                enumerator = static_cast<EMode>(iEnumerator);
                strEnumerator = QString::number(static_cast<int>(enumerator));
                strResultExpected = "?, ?";
            }

            strExpectedValue = "CEnumMode::toString(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strExpectedValue += strResultExpected;
            strlstExpectedValues.append(strExpectedValue);

            strResultValue = "CEnumMode::toString(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strEnumerator = CEnumMode::toString(enumerator, idxAlias);
            strResultValue += strEnumerator;
            strlstResultValues.append(strResultValue);
        }
    }

    // -------------------------------------------------------------------------

    idxAlias = EEnumEntryAliasStrAlias8;

    for( iEnumerator = 0; iEnumerator < CEnumMode::count(); ++iEnumerator )
    {
        if( iEnumerator == 0 ) { enumerator = EMode::Edit; strEnumerator = "Edit"; }
        else if( iEnumerator == 1 ) { enumerator = EMode::Simulation; strEnumerator = "Simulation"; }
        else if( iEnumerator == 2 ) { enumerator = EMode::Undefined; strEnumerator = "Undefined"; }
        else { enumerator = static_cast<EMode>(iEnumerator); strEnumerator = QString::number(static_cast<int>(enumerator)); }

        strExpectedValue = "CEnumMode::toString(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
        strExpectedValue += "Enumerator " + strEnumerator + " does not have an alias string at index " + QString::number(idxAlias);
        strlstExpectedValues.append(strExpectedValue);

        try
        {
            strResultValue = "CEnumMode::toString(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strEnumerator = CEnumMode::toString(enumerator, idxAlias);
            strResultValue += strEnumerator;
        }
        catch( CException& exc )
        {
            strResultValue += exc.getAddErrInfo();
        }
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    strExpectedValue = "CEnumMode::toString(-1): Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        iEnumerator = -1;
        enumerator = static_cast<EMode>(iEnumerator);
        strResultValue = "CEnumMode::toString(" + QString::number(static_cast<int>(enumerator)) + "): ";
        strResultValue += CEnumMode::toString(enumerator);
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    strExpectedValue = "CEnumMode::toString(3): Enumerator 3 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        iEnumerator = CEnumMode::count();
        enumerator = static_cast<EMode>(iEnumerator);
        strResultValue = "CEnumMode::toString(" + QString::number(static_cast<int>(enumerator)) + "): ";
        strResultValue += CEnumMode::toString(enumerator);
    }
    catch( CException& exc )
    {
        strResultValue = "CEnumMode::toString(" + QString::number(iEnumerator) + "): " + exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateZSSysModeClassMethodToString

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeClassMethodToEnumerator( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    EMode               enumerator;
    QString             strEnumeratorSource;
    QString             strEnumeratorResult;
    QString             strAlias;
    Qt::CaseSensitivity caseSensitivity;
    QString             strCaseSensitivity;

    int  iEnumerator;
    int  idxAlias;
    bool bOk;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumMode::count(); ++iEnumerator )
    {
        if( iEnumerator == 0 ) { strEnumeratorSource = "Edit"; }
        else if( iEnumerator == 1 ) { strEnumeratorSource = "Simulation"; }
        else if( iEnumerator == 2 ) { strEnumeratorSource = "Undefined"; }
        else { strEnumeratorSource = QString::number(iEnumerator); }

        strExpectedValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + "): ";
        strExpectedValue += strEnumeratorSource;
        strlstExpectedValues.append(strExpectedValue);

        enumerator = CEnumMode::toEnumerator(strEnumeratorSource);

        if( enumerator == EMode::Edit ) { strEnumeratorResult = "Edit"; }
        else if( enumerator == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
        else if( enumerator == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumerator)); }

        strResultValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + "): ";
        strResultValue += strEnumeratorResult;
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrText; ++idxAlias )
    {
        strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));

        for( iEnumerator = 0; iEnumerator < CEnumMode::count(); ++iEnumerator )
        {
            if( iEnumerator == 0 )
            {
                enumerator = EMode::Edit;
                strEnumeratorResult = "Edit";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Edit";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "E";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Edit";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 1 )
            {
                enumerator = EMode::Simulation;
                strEnumeratorResult = "Simulation";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Simulation";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "S";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Simulation";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 2 )
            {
                enumerator = EMode::Undefined;
                strEnumeratorResult = "Undefined";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Undefined";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "?";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Undefined";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else
            {
                enumerator = static_cast<EMode>(iEnumerator);
                strEnumeratorResult = QString::number(iEnumerator);
                strEnumeratorSource = QString::number(iEnumerator);
            }

            strExpectedValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + "): ";
            strExpectedValue += strEnumeratorResult;
            strlstExpectedValues.append(strExpectedValue);

            enumerator = CEnumMode::toEnumerator(strEnumeratorSource, idxAlias);

            if( enumerator == EMode::Edit ) { strEnumeratorResult = "Edit"; }
            else if( enumerator == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
            else if( enumerator == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
            else { strEnumeratorResult = QString::number(static_cast<int>(enumerator)); }

            strResultValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + "): ";
            strResultValue += strEnumeratorResult;
            strlstResultValues.append(strResultValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrText; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Simulation";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Simulation";

    strExpectedValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    enumerator = CEnumMode::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumerator == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumerator == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "simULATION";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strExpectedValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    enumerator = CEnumMode::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumerator == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumerator == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "simULATION";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Simulation";

    strExpectedValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    enumerator = CEnumMode::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumerator == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumerator == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "simULATION";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strExpectedValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    enumerator = CEnumMode::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumerator == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumerator == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "simULATION";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Simulation";

    strExpectedValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    enumerator = CEnumMode::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumerator == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumerator == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "Simulation";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias8;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Map does not contain alias strings at index " + QString::number(idxAlias);

    strExpectedValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        bOk = false;
        enumerator = CEnumMode::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
        if( enumerator == EMode::Edit ) strEnumeratorResult = "Edit";
        else if( enumerator == EMode::Simulation ) strEnumeratorResult = "Simulation";
        else if( enumerator == EMode::Undefined ) strEnumeratorResult = "Undefined";
        else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    }
    catch( CException& exc )
    {
        strEnumeratorResult = exc.getAddErrInfo();
    }
    strResultValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateZSSysModeClassMethodToEnumerator

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeClassMethodFromString( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    CEnumMode           enumInst;
    QString             strEnumeratorSource;
    QString             strEnumeratorResult;
    QString             strAlias;
    Qt::CaseSensitivity caseSensitivity;
    QString             strCaseSensitivity;

    int  iEnumerator;
    int  idxAlias;
    bool bOk;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumMode::count(); ++iEnumerator )
    {
        if( iEnumerator == 0 ) { strEnumeratorSource = "Edit"; }
        else if( iEnumerator == 1 ) { strEnumeratorSource = "Simulation"; }
        else if( iEnumerator == 2 ) { strEnumeratorSource = "Undefined"; }
        else { strEnumeratorSource = QString::number(iEnumerator); }

        strExpectedValue = "CEnumMode::fromString(" + strEnumeratorSource + "): ";
        strExpectedValue += strEnumeratorSource;
        strlstExpectedValues.append(strExpectedValue);

        enumInst = CEnumMode::fromString(strEnumeratorSource);

        if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
        else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
        else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }

        strResultValue = "CEnumMode::fromString(" + strEnumeratorSource + "): ";
        strResultValue += strEnumeratorResult;
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrText; ++idxAlias )
    {
        strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));

        for( iEnumerator = 0; iEnumerator < CEnumMode::count(); ++iEnumerator )
        {
            if( iEnumerator == 0 )
            {
                strEnumeratorResult = "Edit";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Edit";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "E";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Edit";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 1 )
            {
                strEnumeratorResult = "Simulation";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Simulation";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "S";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Simulation";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 2 )
            {
                strEnumeratorResult = "Undefined";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Undefined";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "?";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Undefined";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else
            {
                strEnumeratorResult = QString::number(iEnumerator);
                strEnumeratorSource = QString::number(iEnumerator);
            }

            strExpectedValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + "): ";
            strExpectedValue += strEnumeratorResult;
            strlstExpectedValues.append(strExpectedValue);

            strResultValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + "): ";
            enumInst = CEnumMode::fromString(strEnumeratorSource, idxAlias);
            if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
            else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
            else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
            else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
            strResultValue += strEnumeratorResult;
            strlstResultValues.append(strResultValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrText; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Simulation";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Simulation";

    strExpectedValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumMode::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "simULATION";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strExpectedValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumMode::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "simULATION";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Simulation";

    strExpectedValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumMode::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "simULATION";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strExpectedValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumMode::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "simULATION";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Simulation";

    strExpectedValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumMode::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Simulation";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias8;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strExpectedValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += "Map does not contain alias strings at index " + QString::number(idxAlias);
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
        enumInst = CEnumMode::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
        if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
        else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
        else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
        else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
        strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateZSSysModeClassMethodFromString

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeCtors( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    CEnumMode* pEnumInst = nullptr;

    QString strEnumerator;
    QString strAlias;
    QString strCaseSensitivity;

    Qt::CaseSensitivity caseSensitivity;

    int iEnumerator;
    int idxAlias;

    // -------------------------------------------------------------------------

    strExpectedValue = "CEnumMode::ctor(): -1";
    strlstExpectedValues.append(strExpectedValue);

    pEnumInst = new CEnumMode();
    if( pEnumInst->enumerator() == EMode::Edit ) { strEnumerator = "Edit"; }
    else if( pEnumInst->enumerator() == EMode::Simulation ) { strEnumerator = "Simulation"; }
    else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
    else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
    strResultValue = "CEnumMode::ctor(): " + strEnumerator;
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumMode::count(); ++iEnumerator )
    {
        strEnumerator = "-1";

        if( iEnumerator == 0 ) { strEnumerator = "Edit"; }
        else if( iEnumerator == 1 ) { strEnumerator = "Simulation"; }
        else if( iEnumerator == 2 ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(iEnumerator); }

        strExpectedValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        strExpectedValue += strEnumerator;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumMode(static_cast<EMode>(iEnumerator));
        if( pEnumInst->enumerator() == EMode::Edit ) { strEnumerator = "Edit"; }
        else if( pEnumInst->enumerator() == EMode::Simulation ) { strEnumerator = "Simulation"; }
        else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strResultValue += strEnumerator;
        strlstResultValues.append(strResultValue);
        delete pEnumInst;
        pEnumInst = nullptr;
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumerator = QString::number(iEnumerator);

    strExpectedValue = "CEnumMode::ctor(" + strEnumerator + "): ";
    strExpectedValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumMode(static_cast<EMode>(iEnumerator));
        if( pEnumInst->enumerator() == EMode::Edit ) { strEnumerator = "Edit"; }
        else if( pEnumInst->enumerator() == EMode::Simulation ) { strEnumerator = "Simulation"; }
        else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strResultValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    iEnumerator = CEnumMode::count();
    strEnumerator = QString::number(iEnumerator);

    strExpectedValue = "CEnumMode::ctor(" + strEnumerator + "): ";
    strExpectedValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumMode(static_cast<EMode>(iEnumerator));
        if( pEnumInst->enumerator() == EMode::Edit ) { strEnumerator = "Edit"; }
        else if( pEnumInst->enumerator() == EMode::Simulation ) { strEnumerator = "Simulation"; }
        else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strResultValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumMode::count(); ++iEnumerator )
    {
        strEnumerator = "-1";

        if( iEnumerator == 0 ) { strEnumerator = "Edit"; }
        else if( iEnumerator == 1 ) { strEnumerator = "Simulation"; }
        else if( iEnumerator == 2 ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(iEnumerator); }

        strExpectedValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        strExpectedValue += strEnumerator;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumMode(iEnumerator);
        if( pEnumInst->enumerator() == EMode::Edit ) { strEnumerator = "Edit"; }
        else if( pEnumInst->enumerator() == EMode::Simulation ) { strEnumerator = "Simulation"; }
        else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strResultValue += strEnumerator;
        strlstResultValues.append(strResultValue);
        delete pEnumInst;
        pEnumInst = nullptr;
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumerator = QString::number(iEnumerator);

    strExpectedValue = "CEnumMode::ctor(" + strEnumerator + "): ";
    strExpectedValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumMode(iEnumerator);
        if( pEnumInst->enumerator() == EMode::Edit ) { strEnumerator = "Edit"; }
        else if( pEnumInst->enumerator() == EMode::Simulation ) { strEnumerator = "Simulation"; }
        else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strResultValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    iEnumerator = CEnumMode::count();
    strEnumerator = QString::number(iEnumerator);

    strExpectedValue = "CEnumMode::ctor(" + strEnumerator + "): ";
    strExpectedValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumMode(iEnumerator);
        if( pEnumInst->enumerator() == EMode::Edit ) { strEnumerator = "Edit"; }
        else if( pEnumInst->enumerator() == EMode::Simulation ) { strEnumerator = "Simulation"; }
        else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strResultValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumMode::count(); ++iEnumerator )
    {
        strEnumerator = "-1";

        if( iEnumerator == 0 ) { strEnumerator = "Edit"; }
        else if( iEnumerator == 1 ) { strEnumerator = "Simulation"; }
        else if( iEnumerator == 2 ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(iEnumerator); }

        strExpectedValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        strExpectedValue += strEnumerator;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumMode(strEnumerator);
        if( pEnumInst->enumerator() == EMode::Edit ) { strEnumerator = "Edit"; }
        else if( pEnumInst->enumerator() == EMode::Simulation ) { strEnumerator = "Simulation"; }
        else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strResultValue += strEnumerator;
        strlstResultValues.append(strResultValue);
        delete pEnumInst;
        pEnumInst = nullptr;
    }

    // -------------------------------------------------------------------------

    strEnumerator = "Simulation";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);

    strExpectedValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strExpectedValue += strEnumerator;
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumMode(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EMode::Edit ) strEnumerator = "Edit";
    else if( pEnumInst->enumerator() == EMode::Simulation ) strEnumerator = "Simulation";
    else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strResultValue += strEnumerator;
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "-1";

    strExpectedValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strExpectedValue += strEnumerator;
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumMode(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EMode::Edit ) strEnumerator = "Edit";
    else if( pEnumInst->enumerator() == EMode::Simulation ) strEnumerator = "Simulation";
    else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strResultValue += strEnumerator;
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "Simulation";

    strExpectedValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strExpectedValue += strEnumerator;
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumMode(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EMode::Edit ) strEnumerator = "Edit";
    else if( pEnumInst->enumerator() == EMode::Simulation ) strEnumerator = "Simulation";
    else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strResultValue += strEnumerator;
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "Simulation";

    strExpectedValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strExpectedValue += strEnumerator;
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumMode(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EMode::Edit ) strEnumerator = "Edit";
    else if( pEnumInst->enumerator() == EMode::Simulation ) strEnumerator = "Simulation";
    else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strResultValue += strEnumerator;
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "-1";

    strExpectedValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strExpectedValue += strEnumerator;
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumMode(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EMode::Edit ) strEnumerator = "Edit";
    else if( pEnumInst->enumerator() == EMode::Simulation ) strEnumerator = "Simulation";
    else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strResultValue += strEnumerator;
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "Simulation";

    strExpectedValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strExpectedValue += strEnumerator;
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumMode(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EMode::Edit ) strEnumerator = "Edit";
    else if( pEnumInst->enumerator() == EMode::Simulation ) strEnumerator = "Simulation";
    else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strResultValue += strEnumerator;
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "Simulation";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias8;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "-1";

    strExpectedValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strExpectedValue += "Map does not contain alias strings at index " + QString::number(idxAlias);
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
        pEnumInst = new CEnumMode(strEnumerator, idxAlias, caseSensitivity);
        if( pEnumInst->enumerator() == EMode::Edit ) strEnumerator = "Edit";
        else if( pEnumInst->enumerator() == EMode::Simulation ) strEnumerator = "Simulation";
        else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
        else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
        strResultValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateZSSysModeCtors

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeOperatorAssign( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    CEnumMode enumInst;

    QString  strEnumeratorSource;
    QString  strEnumeratorResult;
    QVariant val;
    QString  strAlias;

    int iEnumerator;
    int idxAlias;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumMode::count(); ++iEnumerator )
    {
        strEnumeratorSource = "-1";

        if( iEnumerator == 0 ) { strEnumeratorSource = "Edit"; }
        else if( iEnumerator == 1 ) { strEnumeratorSource = "Simulation"; }
        else if( iEnumerator == 2 ) { strEnumeratorSource = "Undefined"; }
        else { strEnumeratorSource = QString::number(iEnumerator); }

        strExpectedValue = "CEnumMode = " + strEnumeratorSource + ": ";
        strExpectedValue += strEnumeratorSource;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "CEnumMode = " + strEnumeratorSource + ": ";
        enumInst = static_cast<EMode>(iEnumerator);
        if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
        else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
        else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strResultValue += strEnumeratorResult;
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumeratorSource = QString::number(iEnumerator);

    strExpectedValue = "CEnumMode = " + strEnumeratorSource + ": ";
    strExpectedValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumMode = " + strEnumeratorSource + ": ";
        enumInst = static_cast<EMode>(iEnumerator);
        if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
        else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
        else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strResultValue += strEnumeratorResult;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    iEnumerator = CEnumMode::count();
    strEnumeratorSource = QString::number(iEnumerator);

    strExpectedValue = "CEnumMode = " + strEnumeratorSource + ": ";
    strExpectedValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumMode = " + strEnumeratorSource + ": ";
        enumInst = static_cast<EMode>(iEnumerator);
        if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
        else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
        else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strResultValue += strEnumeratorResult;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumMode::count(); ++iEnumerator )
    {
        strEnumeratorSource = "-1";

        if( iEnumerator == 0 ) { strEnumeratorSource = "Edit"; }
        else if( iEnumerator == 1 ) { strEnumeratorSource = "Simulation"; }
        else if( iEnumerator == 2 ) { strEnumeratorSource = "Undefined"; }
        else { strEnumeratorSource = QString::number(iEnumerator); }

        strExpectedValue = "CEnumMode = " + strEnumeratorSource + ": ";
        strExpectedValue += strEnumeratorSource;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "CEnumMode = " + strEnumeratorSource + ": ";
        enumInst = iEnumerator;
        if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
        else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
        else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strResultValue += strEnumeratorResult;
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumeratorSource = QString::number(iEnumerator);

    strExpectedValue = "CEnumMode = " + strEnumeratorSource + ": ";
    strExpectedValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumMode = " + strEnumeratorSource + ": ";
        enumInst = iEnumerator;
        if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
        else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
        else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strResultValue += strEnumeratorResult;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    iEnumerator = CEnumMode::count();
    strEnumeratorSource = QString::number(iEnumerator);

    strExpectedValue = "CEnumMode = " + strEnumeratorSource + ": ";
    strExpectedValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumMode = " + strEnumeratorSource + ": ";
        enumInst = iEnumerator;
        if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
        else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
        else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strResultValue += strEnumeratorResult;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrText; ++idxAlias )
    {
        strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));

        for( iEnumerator = 0; iEnumerator < CEnumMode::count(); ++iEnumerator )
        {
            if( iEnumerator == 0 )
            {
                strEnumeratorResult = "Edit";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Edit";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "E";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Edit";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 1 )
            {
                strEnumeratorResult = "Simulation";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Simulation";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "S";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Simulation";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 2 )
            {
                strEnumeratorResult = "Undefined";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Undefined";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "?";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Undefined";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else
            {
                strEnumeratorResult = QString::number(iEnumerator);
                strEnumeratorSource = QString::number(iEnumerator);
            }

            strExpectedValue = "CEnumMode = " + strEnumeratorSource + ": ";
            strExpectedValue += strEnumeratorResult;
            strlstExpectedValues.append(strExpectedValue);

            strResultValue = "CEnumMode = " + strEnumeratorSource + ": ";
            enumInst = strEnumeratorSource;
            if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
            else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
            else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
            else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
            strResultValue += strEnumeratorResult;
            strlstResultValues.append(strResultValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrText; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "simULATION";

    strExpectedValue = "CEnumMode = " + strEnumeratorSource + ": -1";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumMode = " + strEnumeratorSource + ": ";
    enumInst = strEnumeratorSource;
    if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strResultValue += strEnumeratorResult;
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateZSSysModeOperatorAssign

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeOperatorCompareWithEnumerator( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    EMode enumerator;

    CEnumMode enumInst;

    // -------------------------------------------------------------------------

    enumerator = EMode::Simulation;
    enumInst = EMode::Simulation;

    try
    {
        strExpectedValue = "Simulation == Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == enumerator )
        {
            strResultValue = "Simulation == Simulation";
        }
        else
        {
            strResultValue = "Simulation != Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation == Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != enumerator )
        {
            strResultValue = "Simulation != Simulation";
        }
        else
        {
            strResultValue = "Simulation == Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Simulation >= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < enumerator )
        {
            strResultValue = "Simulation < Simulation";
        }
        else
        {
            strResultValue = "Simulation >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > enumerator )
        {
            strResultValue = "Simulation > Simulation";
        }
        else
        {
            strResultValue = "Simulation <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Simulation <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= enumerator )
        {
            strResultValue = "Simulation <= Simulation";
        }
        else
        {
            strResultValue = "Simulation > Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation >= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= enumerator )
        {
            strResultValue = "Simulation >= Simulation";
        }
        else
        {
            strResultValue = "Simulation < Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = EMode::Edit;
    enumerator = EMode::Simulation;

    try
    {
        strExpectedValue = "Edit != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == enumerator )
        {
            strResultValue = "Edit == Simulation";
        }
        else
        {
            strResultValue = "Edit != Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Edit != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != enumerator )
        {
            strResultValue = "Edit != Simulation";
        }
        else
        {
            strResultValue = "Edit == Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Edit < Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < enumerator )
        {
            strResultValue = "Edit < Simulation";
        }
        else
        {
            strResultValue = "Edit >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Edit <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > enumerator )
        {
            strResultValue = "Edit > Simulation";
        }
        else
        {
            strResultValue = "Edit <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Edit <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= enumerator )
        {
            strResultValue = "Edit <= Simulation";
        }
        else
        {
            strResultValue = "Edit > Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Edit < Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= enumerator )
        {
            strResultValue = "Edit >= Simulation";
        }
        else
        {
            strResultValue = "Edit < Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumMode(); // InvalidEnumerator
    enumerator = EMode::Simulation;

    try
    {
        strExpectedValue = "Invalid != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == enumerator )
        {
            strResultValue = "Invalid == Simulation";
        }
        else
        {
            strResultValue = "Invalid != Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != enumerator )
        {
            strResultValue = "Invalid != Simulation";
        }
        else
        {
            strResultValue = "Invalid == Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < enumerator )
        {
            strResultValue = "Invalid < Simulation";
        }
        else
        {
            strResultValue = "Invalid >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > enumerator )
        {
            strResultValue = "Invalid > Simulation";
        }
        else
        {
            strResultValue = "Invalid <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= enumerator )
        {
            strResultValue = "Invalid <= Simulation";
        }
        else
        {
            strResultValue = "Invalid > Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= enumerator )
        {
            strResultValue = "Invalid >= Simulation";
        }
        else
        {
            strResultValue = "Invalid < Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = EMode::Simulation;
    enumerator = static_cast<EMode>(EInvalidEnumerator);

    try
    {
        strExpectedValue = "Simulation != Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == enumerator )
        {
            strResultValue = "Simulation == Invalid";
        }
        else
        {
            strResultValue = "Simulation != Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation != Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != enumerator )
        {
            strResultValue = "Simulation != Invalid";
        }
        else
        {
            strResultValue = "Simulation == Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < enumerator )
        {
            strResultValue = "Simulation < Invalid";
        }
        else
        {
            strResultValue = "Simulation >= Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > enumerator )
        {
            strResultValue = "Simulation > Invalid";
        }
        else
        {
            strResultValue = "Simulation <= Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= enumerator )
        {
            strResultValue = "Simulation <= Invalid";
        }
        else
        {
            strResultValue = "Simulation > Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= enumerator )
        {
            strResultValue = "Simulation >= Invalid";
        }
        else
        {
            strResultValue = "Simulation < Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumMode(); // InvalidEnumerator
    enumerator = static_cast<EMode>(EInvalidEnumerator);

    try
    {
        strExpectedValue = "Invalid == Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == enumerator )
        {
            strResultValue = "Invalid == Invalid";
        }
        else
        {
            strResultValue = "Invalid != Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid == Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != enumerator )
        {
            strResultValue = "Invalid != Invalid";
        }
        else
        {
            strResultValue = "Invalid == Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < enumerator )
        {
            strResultValue = "Invalid < Invalid";
        }
        else
        {
            strResultValue = "Invalid >= Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > enumerator )
        {
            strResultValue = "Invalid > Invalid";
        }
        else
        {
            strResultValue = "Invalid <= Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= enumerator )
        {
            strResultValue = "Invalid <= Invalid";
        }
        else
        {
            strResultValue = "Invalid > Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= enumerator )
        {
            strResultValue = "Invalid >= Invalid";
        }
        else
        {
            strResultValue = "Invalid < Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateZSSysModeOperatorCompareWithEnumerator

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeOperatorCompareWithInt( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    CEnumMode enumInst;

    int iEnumerator;

    // -------------------------------------------------------------------------

    enumInst = EMode::Simulation;
    iEnumerator = static_cast<int>(EMode::Simulation);

    try
    {
        strExpectedValue = "Simulation == Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == iEnumerator )
        {
            strResultValue = "Simulation == Simulation";
        }
        else
        {
            strResultValue = "Simulation != Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation == Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != iEnumerator )
        {
            strResultValue = "Simulation != Simulation";
        }
        else
        {
            strResultValue = "Simulation == Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Simulation >= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < iEnumerator )
        {
            strResultValue = "Simulation < Simulation";
        }
        else
        {
            strResultValue = "Simulation >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > iEnumerator )
        {
            strResultValue = "Simulation > Simulation";
        }
        else
        {
            strResultValue = "Simulation <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Simulation <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= iEnumerator )
        {
            strResultValue = "Simulation <= Simulation";
        }
        else
        {
            strResultValue = "Simulation > Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation >= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= iEnumerator )
        {
            strResultValue = "Simulation >= Simulation";
        }
        else
        {
            strResultValue = "Simulation < Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = EMode::Edit;
    iEnumerator = static_cast<int>(EMode::Simulation);

    try
    {
        strExpectedValue = "Edit != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == iEnumerator )
        {
            strResultValue = "Edit == Simulation";
        }
        else
        {
            strResultValue = "Edit != Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Edit != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != iEnumerator )
        {
            strResultValue = "Edit != Simulation";
        }
        else
        {
            strResultValue = "Edit == Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Edit < Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < iEnumerator )
        {
            strResultValue = "Edit < Simulation";
        }
        else
        {
            strResultValue = "Edit >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Edit <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > iEnumerator )
        {
            strResultValue = "Edit > Simulation";
        }
        else
        {
            strResultValue = "Edit <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Edit <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= iEnumerator )
        {
            strResultValue = "Edit <= Simulation";
        }
        else
        {
            strResultValue = "Edit > Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Edit < Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= iEnumerator )
        {
            strResultValue = "Edit >= Simulation";
        }
        else
        {
            strResultValue = "Edit < Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumMode(); // InvalidEnumerator
    iEnumerator = static_cast<int>(EMode::Simulation);

    try
    {
        strExpectedValue = "Invalid != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == iEnumerator )
        {
            strResultValue = "Invalid == Simulation";
        }
        else
        {
            strResultValue = "Invalid != Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != iEnumerator )
        {
            strResultValue = "Invalid != Simulation";
        }
        else
        {
            strResultValue = "Invalid == Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid < Simulation: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < iEnumerator )
        {
            strResultValue = "Invalid < Simulation";
        }
        else
        {
            strResultValue = "Invalid < Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid > Simulation: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > iEnumerator )
        {
            strResultValue = "Invalid > Simulation";
        }
        else
        {
            strResultValue = "Invalid > Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid <= Simulation: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= iEnumerator )
        {
            strResultValue = "Invalid <= Simulation";
        }
        else
        {
            strResultValue = "Invalid <= Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid >= Simulation: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= iEnumerator )
        {
            strResultValue = "Invalid >= Simulation";
        }
        else
        {
            strResultValue = "Invalid >= Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = EMode::Simulation;
    iEnumerator = EInvalidEnumerator;

    try
    {
        strExpectedValue = "Simulation != Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == iEnumerator )
        {
            strResultValue = "Simulation == Invalid";
        }
        else
        {
            strResultValue = "Simulation != Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation != Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != iEnumerator )
        {
            strResultValue = "Simulation != Invalid";
        }
        else
        {
            strResultValue = "Simulation == Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Simulation < Invalid: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < iEnumerator )
        {
            strResultValue = "Simulation < Invalid";
        }
        else
        {
            strResultValue = "Simulation < Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation > Invalid: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > iEnumerator )
        {
            strResultValue = "Simulation > Invalid";
        }
        else
        {
            strResultValue = "Simulation > Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Simulation <= Invalid: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= iEnumerator )
        {
            strResultValue = "Simulation <= Invalid";
        }
        else
        {
            strResultValue = "Simulation <= Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation >= Invalid: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= iEnumerator )
        {
            strResultValue = "Simulation >= Invalid";
        }
        else
        {
            strResultValue = "Simulation >= Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumMode(); // InvalidEnumerator
    iEnumerator = EInvalidEnumerator;

    try
    {
        strExpectedValue = "Invalid == Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == iEnumerator )
        {
            strResultValue = "Invalid == Invalid";
        }
        else
        {
            strResultValue = "Invalid != Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid == Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != iEnumerator )
        {
            strResultValue = "Invalid != Invalid";
        }
        else
        {
            strResultValue = "Invalid == Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid < Invalid: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < iEnumerator )
        {
            strResultValue = "Invalid < Invalid";
        }
        else
        {
            strResultValue = "Invalid < Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid > Invalid: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > iEnumerator )
        {
            strResultValue = "Invalid > Invalid";
        }
        else
        {
            strResultValue = "Invalid > Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid <= Invalid: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= iEnumerator )
        {
            strResultValue = "Invalid <= Invalid";
        }
        else
        {
            strResultValue = "Invalid <= Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid >= Invalid: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= iEnumerator )
        {
            strResultValue = "Invalid >= Invalid";
        }
        else
        {
            strResultValue = "Invalid >= Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateZSSysModeOperatorCompareWithInt

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeOperatorCompareWithCharPtr( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    CEnumMode enumInst;

    // -------------------------------------------------------------------------

    enumInst = EMode::Simulation;
    const char* szEnumerator = "Simulation";

    try
    {
        strExpectedValue = "Simulation == Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == szEnumerator )
        {
            strResultValue = "Simulation == Simulation";
        }
        else
        {
            strResultValue = "Simulation != Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation == Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != szEnumerator )
        {
            strResultValue = "Simulation != Simulation";
        }
        else
        {
            strResultValue = "Simulation == Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Simulation >= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < szEnumerator )
        {
            strResultValue = "Simulation < Simulation";
        }
        else
        {
            strResultValue = "Simulation >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > szEnumerator )
        {
            strResultValue = "Simulation > Simulation";
        }
        else
        {
            strResultValue = "Simulation <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Simulation <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= szEnumerator )
        {
            strResultValue = "Simulation <= Simulation";
        }
        else
        {
            strResultValue = "Simulation > Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation >= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= szEnumerator )
        {
            strResultValue = "Simulation >= Simulation";
        }
        else
        {
            strResultValue = "Simulation < Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = EMode::Edit;
    szEnumerator = "Simulation";

    try
    {
        strExpectedValue = "Edit != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == szEnumerator )
        {
            strResultValue = "Edit == Simulation";
        }
        else
        {
            strResultValue = "Edit != Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Edit != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != szEnumerator )
        {
            strResultValue = "Edit != Simulation";
        }
        else
        {
            strResultValue = "Edit == Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Edit < Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < szEnumerator )
        {
            strResultValue = "Edit < Simulation";
        }
        else
        {
            strResultValue = "Edit >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Edit <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > szEnumerator )
        {
            strResultValue = "Edit > Simulation";
        }
        else
        {
            strResultValue = "Edit <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Edit <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= szEnumerator )
        {
            strResultValue = "Edit <= Simulation";
        }
        else
        {
            strResultValue = "Edit > Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Edit < Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= szEnumerator )
        {
            strResultValue = "Edit >= Simulation";
        }
        else
        {
            strResultValue = "Edit < Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumMode(); // InvalidEnumerator
    szEnumerator = "Simulation";

    try
    {
        strExpectedValue = "Invalid != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == szEnumerator )
        {
            strResultValue = "Invalid == Simulation";
        }
        else
        {
            strResultValue = "Invalid != Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != szEnumerator )
        {
            strResultValue = "Invalid != Simulation";
        }
        else
        {
            strResultValue = "Invalid == Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid < Simulation: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < szEnumerator )
        {
            strResultValue = "Invalid < Simulation";
        }
        else
        {
            strResultValue = "Invalid < Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid > Simulation: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > szEnumerator )
        {
            strResultValue = "Invalid > Simulation";
        }
        else
        {
            strResultValue = "Invalid > Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid <= Simulation: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= szEnumerator )
        {
            strResultValue = "Invalid <= Simulation";
        }
        else
        {
            strResultValue = "Invalid <= Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid >= Simulation: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= szEnumerator )
        {
            strResultValue = "Invalid >= Simulation";
        }
        else
        {
            strResultValue = "Invalid >= Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = EMode::Simulation;
    szEnumerator = "Unknown";

    try
    {
        strExpectedValue = "Simulation != Unknown";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == szEnumerator )
        {
            strResultValue = "Simulation == Unknown";
        }
        else
        {
            strResultValue = "Simulation != Unknown";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation != Unknown";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != szEnumerator )
        {
            strResultValue = "Simulation != Unknown";
        }
        else
        {
            strResultValue = "Simulation == Unknown";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Simulation < Unknown: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < szEnumerator )
        {
            strResultValue = "Simulation < Unknown";
        }
        else
        {
            strResultValue = "Simulation < Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation > Unknown: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > szEnumerator )
        {
            strResultValue = "Simulation > Unknown";
        }
        else
        {
            strResultValue = "Simulation > Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Simulation <= Unknown: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= szEnumerator )
        {
            strResultValue = "Simulation <= Unknown";
        }
        else
        {
            strResultValue = "Simulation <= Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation >= Unknown: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= szEnumerator )
        {
            strResultValue = "Simulation >= Unknown";
        }
        else
        {
            strResultValue = "Simulation >= Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumMode(); // InvalidEnumerator
    szEnumerator = "Unknown";

    try
    {
        strExpectedValue = "Invalid != Unknown";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == szEnumerator )
        {
            strResultValue = "Invalid == Unknown";
        }
        else
        {
            strResultValue = "Invalid != Unknown";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid != Unknown";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != szEnumerator )
        {
            strResultValue = "Invalid != Unknown";
        }
        else
        {
            strResultValue = "Invalid == Unknown";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid < Unknown: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < szEnumerator )
        {
            strResultValue = "Invalid < Unknown";
        }
        else
        {
            strResultValue = "Invalid < Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid > Unknown: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > szEnumerator )
        {
            strResultValue = "Invalid > Unknown";
        }
        else
        {
            strResultValue = "Invalid > Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid <= Unknown: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= szEnumerator )
        {
            strResultValue = "Invalid <= Unknown";
        }
        else
        {
            strResultValue = "Invalid <= Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid >= Unknown: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= szEnumerator )
        {
            strResultValue = "Invalid >= Unknown";
        }
        else
        {
            strResultValue = "Invalid >= Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateZSSysModeOperatorCompareWithCharPtr

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeOperatorCompareWithQString( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    CEnumMode enumInst;

    QString strEnumerator;

    // -------------------------------------------------------------------------

    enumInst = EMode::Simulation;
    strEnumerator = "Simulation";

    try
    {
        strExpectedValue = "Simulation == Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == strEnumerator )
        {
            strResultValue = "Simulation == Simulation";
        }
        else
        {
            strResultValue = "Simulation != Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation == Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != strEnumerator )
        {
            strResultValue = "Simulation != Simulation";
        }
        else
        {
            strResultValue = "Simulation == Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Simulation >= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < strEnumerator )
        {
            strResultValue = "Simulation < Simulation";
        }
        else
        {
            strResultValue = "Simulation >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > strEnumerator )
        {
            strResultValue = "Simulation > Simulation";
        }
        else
        {
            strResultValue = "Simulation <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Simulation <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= strEnumerator )
        {
            strResultValue = "Simulation <= Simulation";
        }
        else
        {
            strResultValue = "Simulation > Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation >= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= strEnumerator )
        {
            strResultValue = "Simulation >= Simulation";
        }
        else
        {
            strResultValue = "Simulation < Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = EMode::Edit;
    strEnumerator = "Simulation";

    try
    {
        strExpectedValue = "Edit != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == strEnumerator )
        {
            strResultValue = "Edit == Simulation";
        }
        else
        {
            strResultValue = "Edit != Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Edit != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != strEnumerator )
        {
            strResultValue = "Edit != Simulation";
        }
        else
        {
            strResultValue = "Edit == Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Edit < Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < strEnumerator )
        {
            strResultValue = "Edit < Simulation";
        }
        else
        {
            strResultValue = "Edit >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Edit <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > strEnumerator )
        {
            strResultValue = "Edit > Simulation";
        }
        else
        {
            strResultValue = "Edit <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Edit <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= strEnumerator )
        {
            strResultValue = "Edit <= Simulation";
        }
        else
        {
            strResultValue = "Edit > Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Edit < Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= strEnumerator )
        {
            strResultValue = "Edit >= Simulation";
        }
        else
        {
            strResultValue = "Edit < Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumMode(); // InvalidEnumerator
    strEnumerator = "Simulation";

    try
    {
        strExpectedValue = "Invalid != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == strEnumerator )
        {
            strResultValue = "Invalid == Simulation";
        }
        else
        {
            strResultValue = "Invalid != Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != strEnumerator )
        {
            strResultValue = "Invalid != Simulation";
        }
        else
        {
            strResultValue = "Invalid == Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid < Simulation: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < strEnumerator )
        {
            strResultValue = "Invalid < Simulation";
        }
        else
        {
            strResultValue = "Invalid < Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid > Simulation: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > strEnumerator )
        {
            strResultValue = "Invalid > Simulation";
        }
        else
        {
            strResultValue = "Invalid > Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid <= Simulation: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= strEnumerator )
        {
            strResultValue = "Invalid <= Simulation";
        }
        else
        {
            strResultValue = "Invalid <= Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid >= Simulation: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= strEnumerator )
        {
            strResultValue = "Invalid >= Simulation";
        }
        else
        {
            strResultValue = "Invalid >= Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = EMode::Simulation;
    strEnumerator = "Unknown";

    try
    {
        strExpectedValue = "Simulation != Unknown";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == strEnumerator )
        {
            strResultValue = "Simulation == Unknown";
        }
        else
        {
            strResultValue = "Simulation != Unknown";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation != Unknown";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != strEnumerator )
        {
            strResultValue = "Simulation != Unknown";
        }
        else
        {
            strResultValue = "Simulation == Unknown";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Simulation < Unknown: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < strEnumerator )
        {
            strResultValue = "Simulation < Unknown";
        }
        else
        {
            strResultValue = "Simulation < Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation > Unknown: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > strEnumerator )
        {
            strResultValue = "Simulation > Unknown";
        }
        else
        {
            strResultValue = "Simulation > Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Simulation <= Unknown: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= strEnumerator )
        {
            strResultValue = "Simulation <= Unknown";
        }
        else
        {
            strResultValue = "Simulation <= Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation >= Unknown: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= strEnumerator )
        {
            strResultValue = "Simulation >= Unknown";
        }
        else
        {
            strResultValue = "Simulation >= Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumMode(); // InvalidEnumerator
    strEnumerator = "Unknown";

    try
    {
        strExpectedValue = "Invalid != Unknown";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == strEnumerator )
        {
            strResultValue = "Invalid == Unknown";
        }
        else
        {
            strResultValue = "Invalid != Unknown";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid != Unknown";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != strEnumerator )
        {
            strResultValue = "Invalid != Unknown";
        }
        else
        {
            strResultValue = "Invalid == Unknown";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid < Unknown: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < strEnumerator )
        {
            strResultValue = "Invalid < Unknown";
        }
        else
        {
            strResultValue = "Invalid < Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid > Unknown: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > strEnumerator )
        {
            strResultValue = "Invalid > Unknown";
        }
        else
        {
            strResultValue = "Invalid > Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid <= Unknown: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= strEnumerator )
        {
            strResultValue = "Invalid <= Unknown";
        }
        else
        {
            strResultValue = "Invalid <= Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid >= Unknown: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= strEnumerator )
        {
            strResultValue = "Invalid >= Unknown";
        }
        else
        {
            strResultValue = "Invalid >= Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateZSSysModeOperatorCompareWithQString

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeOperatorCompareWithEnumTemplate( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    CEnumMode enumInst1;
    CEnumMode enumInst2;

    // -------------------------------------------------------------------------

    enumInst1 = EMode::Simulation;
    enumInst2 = EMode::Simulation;

    try
    {
        strExpectedValue = "Simulation == Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 == enumInst2 )
        {
            strResultValue = "Simulation == Simulation";
        }
        else
        {
            strResultValue = "Simulation != Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation == Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 != enumInst2 )
        {
            strResultValue = "Simulation != Simulation";
        }
        else
        {
            strResultValue = "Simulation == Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Simulation >= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 < enumInst2 )
        {
            strResultValue = "Simulation < Simulation";
        }
        else
        {
            strResultValue = "Simulation >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 > enumInst2 )
        {
            strResultValue = "Simulation > Simulation";
        }
        else
        {
            strResultValue = "Simulation <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Simulation <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 <= enumInst2 )
        {
            strResultValue = "Simulation <= Simulation";
        }
        else
        {
            strResultValue = "Simulation > Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation >= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 >= enumInst2 )
        {
            strResultValue = "Simulation >= Simulation";
        }
        else
        {
            strResultValue = "Simulation < Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst1 = EMode::Edit;
    enumInst2 = EMode::Simulation;

    try
    {
        strExpectedValue = "Edit != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 == enumInst2 )
        {
            strResultValue = "Edit == Simulation";
        }
        else
        {
            strResultValue = "Edit != Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Edit != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 != enumInst2 )
        {
            strResultValue = "Edit != Simulation";
        }
        else
        {
            strResultValue = "Edit == Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Edit < Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 < enumInst2 )
        {
            strResultValue = "Edit < Simulation";
        }
        else
        {
            strResultValue = "Edit >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Edit <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 > enumInst2 )
        {
            strResultValue = "Edit > Simulation";
        }
        else
        {
            strResultValue = "Edit <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Edit <= Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 <= enumInst2 )
        {
            strResultValue = "Edit <= Simulation";
        }
        else
        {
            strResultValue = "Edit > Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Edit < Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 >= enumInst2 )
        {
            strResultValue = "Edit >= Simulation";
        }
        else
        {
            strResultValue = "Edit < Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst1 = CEnumMode(); // InvalidEnumerator
    enumInst2 = EMode::Simulation;

    try
    {
        strExpectedValue = "Invalid != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 == enumInst2 )
        {
            strResultValue = "Invalid == Simulation";
        }
        else
        {
            strResultValue = "Invalid != Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid != Simulation";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 != enumInst2 )
        {
            strResultValue = "Invalid != Simulation";
        }
        else
        {
            strResultValue = "Invalid == Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 < enumInst2 )
        {
            strResultValue = "Invalid < Simulation";
        }
        else
        {
            strResultValue = "Invalid >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 > enumInst2 )
        {
            strResultValue = "Invalid > Simulation";
        }
        else
        {
            strResultValue = "Invalid <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 <= enumInst2 )
        {
            strResultValue = "Invalid <= Simulation";
        }
        else
        {
            strResultValue = "Invalid > Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 >= enumInst2 )
        {
            strResultValue = "Invalid >= Simulation";
        }
        else
        {
            strResultValue = "Invalid < Simulation";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst1 = EMode::Simulation;
    enumInst2 = CEnumMode(); // InvalidEnumerator

    try
    {
        strExpectedValue = "Simulation != Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 == enumInst2 )
        {
            strResultValue = "Simulation == Invalid";
        }
        else
        {
            strResultValue = "Simulation != Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Simulation != Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 != enumInst2 )
        {
            strResultValue = "Simulation != Invalid";
        }
        else
        {
            strResultValue = "Simulation == Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 < enumInst2 )
        {
            strResultValue = "Simulation < Invalid";
        }
        else
        {
            strResultValue = "Simulation >= Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 > enumInst2 )
        {
            strResultValue = "Simulation > Invalid";
        }
        else
        {
            strResultValue = "Simulation <= Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 <= enumInst2 )
        {
            strResultValue = "Simulation <= Invalid";
        }
        else
        {
            strResultValue = "Simulation > Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 >= enumInst2 )
        {
            strResultValue = "Simulation >= Invalid";
        }
        else
        {
            strResultValue = "Simulation < Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst1 = CEnumMode(); // InvalidEnumerator
    enumInst2 = CEnumMode(); // InvalidEnumerator

    try
    {
        strExpectedValue = "Invalid == Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 == enumInst2 )
        {
            strResultValue = "Invalid == Invalid";
        }
        else
        {
            strResultValue = "Invalid != Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid == Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 != enumInst2 )
        {
            strResultValue = "Invalid != Invalid";
        }
        else
        {
            strResultValue = "Invalid == Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 < enumInst2 )
        {
            strResultValue = "Invalid < Invalid";
        }
        else
        {
            strResultValue = "Invalid >= Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 > enumInst2 )
        {
            strResultValue = "Invalid > Invalid";
        }
        else
        {
            strResultValue = "Invalid <= Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 <= enumInst2 )
        {
            strResultValue = "Invalid <= Invalid";
        }
        else
        {
            strResultValue = "Invalid > Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 >= enumInst2 )
        {
            strResultValue = "Invalid >= Invalid";
        }
        else
        {
            strResultValue = "Invalid < Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateZSSysModeOperatorCompareWithEnumTemplate

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeOperatorIncDec( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    QString strEnumeratorResult;
    QString strEnumeratorSource;

    int iEnumerator;

    CEnumMode enumInst;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumMode::count(); ++iEnumerator )
    {
        try
        {
            enumInst = static_cast<EMode>(iEnumerator);
            if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorSource = "Edit"; strEnumeratorResult = "Simulation"; }
            else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorSource = "Simulation"; strEnumeratorResult = "Undefined"; }
            else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorSource = "Undefined"; strEnumeratorResult = "Invalid"; }
            else strEnumeratorSource = "Invalid";
            strExpectedValue = strEnumeratorSource + "++: " + strEnumeratorResult;
            strlstExpectedValues.append(strExpectedValue);

            enumInst++;
            if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
            else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
            else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
            else strEnumeratorResult = "Invalid";
            strResultValue = strEnumeratorSource + "++: " + strEnumeratorResult;
        }
        catch( CException& exc )
        {
            strResultValue = exc.getAddErrInfo();
        }
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumMode::count(); ++iEnumerator )
    {
        try
        {
            enumInst = static_cast<EMode>(iEnumerator);
            if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorSource = "Edit"; strEnumeratorResult = "Simulation"; }
            else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorSource = "Simulation"; strEnumeratorResult = "Undefined"; }
            else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorSource = "Undefined"; strEnumeratorResult = "Invalid"; }
            else strEnumeratorSource = "Invalid";
            strExpectedValue = "++" + strEnumeratorSource + ": " + strEnumeratorResult;
            strlstExpectedValues.append(strExpectedValue);

            ++enumInst;
            if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
            else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
            else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
            else strEnumeratorResult = "Invalid";
            strResultValue = "++" + strEnumeratorSource + ": " + strEnumeratorResult;
        }
        catch( CException& exc )
        {
            strResultValue = exc.getAddErrInfo();
        }
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( iEnumerator = CEnumMode::count()-1; iEnumerator >= 0; --iEnumerator )
    {
        try
        {
            enumInst = static_cast<EMode>(iEnumerator);
            if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorSource = "Edit"; strEnumeratorResult = "Invalid"; }
            else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorSource = "Simulation"; strEnumeratorResult = "Edit"; }
            else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorSource = "Undefined"; strEnumeratorResult = "Simulation"; }
            else strEnumeratorSource = "Invalid";
            strExpectedValue = strEnumeratorSource + "--: " + strEnumeratorResult;
            strlstExpectedValues.append(strExpectedValue);

            enumInst--;
            if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
            else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
            else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
            else strEnumeratorResult = "Invalid";
            strResultValue = strEnumeratorSource + "--: " + strEnumeratorResult;
        }
        catch( CException& exc )
        {
            strResultValue = exc.getAddErrInfo();
        }
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( iEnumerator = CEnumMode::count()-1; iEnumerator >= 0; --iEnumerator )
    {
        try
        {
            enumInst = static_cast<EMode>(iEnumerator);
            if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorSource = "Edit"; strEnumeratorResult = "Invalid"; }
            else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorSource = "Simulation"; strEnumeratorResult = "Edit"; }
            else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorSource = "Undefined"; strEnumeratorResult = "Simulation"; }
            else strEnumeratorSource = "Invalid";
            strExpectedValue = "--" + strEnumeratorSource + ": " + strEnumeratorResult;
            strlstExpectedValues.append(strExpectedValue);

            --enumInst;
            if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
            else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
            else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
            else strEnumeratorResult = "Invalid";
            strResultValue = "--" + strEnumeratorSource + ": " + strEnumeratorResult;
        }
        catch( CException& exc )
        {
            strResultValue = exc.getAddErrInfo();
        }
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateZSSysModeOperatorIncDec

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeInstMethodToString( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    QString strEnumeratorSource;
    QString strEnumeratorResult;
    QString strAlias;

    int  iEnumerator;
    int  idxAlias;

    CEnumMode enumInst;

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrText; ++idxAlias )
    {
        strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));

        for( iEnumerator = 0; iEnumerator < CEnumMode::count(); ++iEnumerator )
        {
            try
            {
                enumInst = static_cast<EMode>(iEnumerator);

                if( iEnumerator == static_cast<int>(EMode::Edit) )
                {
                    if( idxAlias == EEnumEntryAliasStrName ) { strEnumeratorSource = "Edit"; }
                    else if( idxAlias == EEnumEntryAliasStrSymbol ) { strEnumeratorSource = "E"; }
                    else if( idxAlias == EEnumEntryAliasStrText ) { strEnumeratorSource = "Edit"; }
                    else { strEnumeratorSource = ""; }
                }
                else if( iEnumerator == static_cast<int>(EMode::Simulation) )
                {
                    if( idxAlias == EEnumEntryAliasStrName ) { strEnumeratorSource = "Simulation"; }
                    else if( idxAlias == EEnumEntryAliasStrSymbol ) { strEnumeratorSource = "S"; }
                    else if( idxAlias == EEnumEntryAliasStrText ) { strEnumeratorSource = "Simulation"; }
                    else { strEnumeratorSource = ""; }
                }
                else if( iEnumerator == static_cast<int>(EMode::Undefined) )
                {
                    if( idxAlias == EEnumEntryAliasStrName ) { strEnumeratorSource = "Undefined"; }
                    else if( idxAlias == EEnumEntryAliasStrSymbol ) { strEnumeratorSource = "?"; }
                    else if( idxAlias == EEnumEntryAliasStrText ) { strEnumeratorSource = "Undefined"; }
                    else { strEnumeratorSource = ""; }
                }
                else
                {
                    strEnumeratorSource = "Invalid";
                }

                strExpectedValue = strEnumeratorSource + ".toString(" + strAlias + "): " + strEnumeratorSource;
                strlstExpectedValues.append(strExpectedValue);

                strEnumeratorResult = enumInst.toString(idxAlias);
                strResultValue = strEnumeratorSource + ".toString(" + strAlias + "): " + strEnumeratorResult;
            }
            catch( CException& exc )
            {
                strResultValue = exc.getAddErrInfo();
            }
            strlstResultValues.append(strResultValue);
        }
    }

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateZSSysModeInstMethodToString

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeForLoop( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    QString strEnumerator;

    CEnumMode enumInst;

    int iEnumerator;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0, enumInst = 0; enumInst < CEnumMode::count(); ++iEnumerator, ++enumInst )
    {
        if( iEnumerator == 0 ) { strEnumerator = "Edit"; }
        else if( iEnumerator == 1 ) { strEnumerator = "Simulation"; }
        else if( iEnumerator == 2 ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(iEnumerator); }

        strExpectedValue = strEnumerator + ".toString(): " + strEnumerator;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = strEnumerator + ".toString(): " + enumInst.toString();
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( iEnumerator = CEnumMode::count()-1, enumInst = CEnumMode::count()-1; enumInst >= 0; --iEnumerator, --enumInst )
    {
        if( iEnumerator == 0 ) { strEnumerator = "Edit"; }
        else if( iEnumerator == 1 ) { strEnumerator = "Simulation"; }
        else if( iEnumerator == 2 ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(iEnumerator); }

        strExpectedValue = strEnumerator + ".toString(): " + strEnumerator;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = strEnumerator + ".toString(): " + enumInst.toString();
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateZSSysModeForLoop

/*==============================================================================
public: // test step methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedClassMethodToString( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    EProcessorClock enumerator;
    QString         strEnumerator;
    QString         strResultExpected;

    int  iEnumerator;
    int  idxAlias;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        if( iEnumerator == 0 ) { enumerator = EProcessorClock::Low; strEnumerator = "Low"; }
        else if( iEnumerator == 1 ) { enumerator = EProcessorClock::Medium; strEnumerator = "Medium"; }
        else if( iEnumerator == 2 ) { enumerator = EProcessorClock::High; strEnumerator = "High"; }
        else { enumerator = static_cast<EProcessorClock>(iEnumerator); strEnumerator = QString::number(iEnumerator); }

        strExpectedValue = "CEnumProcessorClock::toString(" + strEnumerator + "): " + strEnumerator;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "CEnumProcessorClock::toString(" + strEnumerator + "): " + CEnumProcessorClock::toString(static_cast<EProcessorClock>(iEnumerator));
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )
    {
        for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
        {
            if( iEnumerator == 0 )
            {
                enumerator = EProcessorClock::Low;
                strEnumerator = "Low";
                if( idxAlias == EEnumEntryAliasStrName ) strResultExpected = "Low";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strResultExpected = "L";
                else if( idxAlias == EEnumEntryAliasStrText ) strResultExpected = "Low (5.12 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strResultExpected = "LOW";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strResultExpected = "LOW";
            }
            else if( iEnumerator == 1 )
            {
                enumerator = EProcessorClock::Medium;
                strEnumerator = "Medium";
                if( idxAlias == EEnumEntryAliasStrName ) strResultExpected = "Medium";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strResultExpected = "M";
                else if( idxAlias == EEnumEntryAliasStrText ) strResultExpected = "Medium (512.0 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strResultExpected = "MED";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strResultExpected = "MEDium";
            }
            else if( iEnumerator == 2 )
            {
                enumerator = EProcessorClock::High;
                strEnumerator = "High";
                if( idxAlias == EEnumEntryAliasStrName ) strResultExpected = "High";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strResultExpected = "H";
                else if( idxAlias == EEnumEntryAliasStrText ) strResultExpected = "High (5.12 GHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strResultExpected = "HIGH";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strResultExpected = "HIGH";
            }
            else
            {
                enumerator = static_cast<EProcessorClock>(iEnumerator);
                strEnumerator = QString::number(static_cast<int>(enumerator));
                strResultExpected = "?, ?";
            }

            strExpectedValue = "CEnumProcessorClock::toString(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strExpectedValue += strResultExpected;
            strlstExpectedValues.append(strExpectedValue);

            strResultValue = "CEnumProcessorClock::toString(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strEnumerator = CEnumProcessorClock::toString(enumerator, idxAlias);
            strResultValue += strEnumerator;
            strlstResultValues.append(strResultValue);
        }
    }

    // -------------------------------------------------------------------------

    idxAlias = EEnumEntryAliasStrAlias6;

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        if( iEnumerator == 0 ) { enumerator = EProcessorClock::Low; strEnumerator = "Low"; }
        else if( iEnumerator == 1 ) { enumerator = EProcessorClock::Medium; strEnumerator = "Medium"; }
        else if( iEnumerator == 2 ) { enumerator = EProcessorClock::High; strEnumerator = "High"; }
        else { enumerator = static_cast<EProcessorClock>(iEnumerator); strEnumerator = QString::number(static_cast<int>(enumerator)); }

        strExpectedValue = "CEnumProcessorClock::toString(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
        strExpectedValue += "Enumerator " + strEnumerator + " does not have an alias string at index " + QString::number(idxAlias);
        strlstExpectedValues.append(strExpectedValue);

        try
        {
            strResultValue = "CEnumProcessorClock::toString(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strEnumerator = CEnumProcessorClock::toString(enumerator, idxAlias);
            strResultValue += strEnumerator;
        }
        catch( CException& exc )
        {
            strResultValue += exc.getAddErrInfo();
        }
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    strExpectedValue = "CEnumProcessorClock::toString(-1): Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        iEnumerator = -1;
        enumerator = static_cast<EProcessorClock>(iEnumerator);
        strResultValue = "CEnumProcessorClock::toString(" + QString::number(static_cast<int>(enumerator)) + "): ";
        strResultValue += CEnumProcessorClock::toString(enumerator);
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    strExpectedValue = "CEnumProcessorClock::toString(3): Enumerator 3 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        iEnumerator = CEnumProcessorClock::count();
        enumerator = static_cast<EProcessorClock>(iEnumerator);
        strResultValue = "CEnumProcessorClock::toString(" + QString::number(static_cast<int>(enumerator)) + "): ";
        strResultValue += CEnumProcessorClock::toString(enumerator);
    }
    catch( CException& exc )
    {
        strResultValue = "CEnumProcessorClock::toString(" + QString::number(iEnumerator) + "): " + exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateUserDefinedClassMethodToString

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedClassMethodToEnumerator( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    EProcessorClock     enumerator;
    QString             strEnumeratorSource;
    QString             strEnumeratorResult;
    QString             strAlias;
    Qt::CaseSensitivity caseSensitivity;
    QString             strCaseSensitivity;
    QVariant            valSource;

    int  iEnumerator;
    int  idxAlias;
    bool bOk;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        if( iEnumerator == 0 ) { strEnumeratorSource = "Low"; }
        else if( iEnumerator == 1 ) { strEnumeratorSource = "Medium"; }
        else if( iEnumerator == 2 ) { strEnumeratorSource = "High"; }
        else { strEnumeratorSource = QString::number(iEnumerator); }

        strExpectedValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + "): ";
        strExpectedValue += strEnumeratorSource;
        strlstExpectedValues.append(strExpectedValue);

        enumerator = CEnumProcessorClock::toEnumerator(strEnumeratorSource);

        if( enumerator == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumerator == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumerator == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumerator)); }

        strResultValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + "): ";
        strResultValue += strEnumeratorResult;
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )
    {
        strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));

        for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
        {
            if( iEnumerator == 0 )
            {
                enumerator = EProcessorClock::Low;
                strEnumeratorResult = "Low";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Low";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "L";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Low (5.12 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "LOW";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "LOW";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 1 )
            {
                enumerator = EProcessorClock::Medium;
                strEnumeratorResult = "Medium";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Medium";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "M";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Medium (512.0 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "MED";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "MEDium";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 2 )
            {
                enumerator = EProcessorClock::High;
                strEnumeratorResult = "High";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "High";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "H";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "High (5.12 GHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "HIGH";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "HIGH";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else
            {
                enumerator = static_cast<EProcessorClock>(iEnumerator);
                strEnumeratorResult = QString::number(iEnumerator);
                strEnumeratorSource = QString::number(iEnumerator);
            }

            strExpectedValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + "): ";
            strExpectedValue += strEnumeratorResult;
            strlstExpectedValues.append(strExpectedValue);

            enumerator = CEnumProcessorClock::toEnumerator(strEnumeratorSource, idxAlias);

            if( enumerator == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
            else if( enumerator == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
            else if( enumerator == EProcessorClock::High ) { strEnumeratorResult = "High"; }
            else { strEnumeratorResult = QString::number(static_cast<int>(enumerator)); }

            strResultValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + "): ";
            strResultValue += strEnumeratorResult;
            strlstResultValues.append(strResultValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strExpectedValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    enumerator = CEnumProcessorClock::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strExpectedValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    enumerator = CEnumProcessorClock::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strExpectedValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    enumerator = CEnumProcessorClock::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strExpectedValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    enumerator = CEnumProcessorClock::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strExpectedValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    enumerator = CEnumProcessorClock::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strExpectedValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    enumerator = CEnumProcessorClock::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias6;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Map does not contain alias strings at index " + QString::number(idxAlias);

    strExpectedValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        bOk = false;
        enumerator = CEnumProcessorClock::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
        if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
        else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
        else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
        else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    }
    catch( CException& exc )
    {
        strEnumeratorResult = exc.getAddErrInfo();
    }
    strResultValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    valSource = 5.12e6;
    strEnumeratorResult = "Low";

    strExpectedValue = "CEnumProcessorClock::toEnumerator(" + valSource.toString() + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    enumerator = CEnumProcessorClock::toEnumerator(valSource, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue = "CEnumProcessorClock::toEnumerator(" + valSource.toString() + ", &bOk): ";
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    valSource = 5.12e8;
    strEnumeratorResult = "Medium";

    strExpectedValue = "CEnumProcessorClock::toEnumerator(" + valSource.toString() + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    enumerator = CEnumProcessorClock::toEnumerator(valSource, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue = "CEnumProcessorClock::toEnumerator(" + valSource.toString() + ", &bOk): ";
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    valSource = 5.12e9;
    strEnumeratorResult = "High";

    strExpectedValue = "CEnumProcessorClock::toEnumerator(" + valSource.toString() + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    enumerator = CEnumProcessorClock::toEnumerator(valSource, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue = "CEnumProcessorClock::toEnumerator(" + valSource.toString() + ", &bOk): ";
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    valSource = 5.12;
    strEnumeratorResult = "-1";

    strExpectedValue = "CEnumProcessorClock::toEnumerator(" + valSource.toString() + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    enumerator = CEnumProcessorClock::toEnumerator(valSource, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strResultValue = "CEnumProcessorClock::toEnumerator(" + valSource.toString() + ", &bOk): ";
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateUserDefinedClassMethodToEnumerator

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedClassMethodToValue( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    EProcessorClock enumerator;

    QString strEnumerator;
    QString strAlias;
    QString strCaseSensitivity;

    QVariant valExpected;
    QVariant valResult;

    int  iEnumerator;
    int  idxAlias;
    bool bOk;

    Qt::CaseSensitivity caseSensitivity;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        if( iEnumerator == 0 )
        {
            enumerator = EProcessorClock::Low;
            strEnumerator = "Low";
            valExpected = 5.12e6;
        }
        else if( iEnumerator == 1 )
        {
            enumerator = EProcessorClock::Medium;
            strEnumerator = "Medium";
            valExpected = 5.12e8;
        }
        else if( iEnumerator == 2 )
        {
            enumerator = EProcessorClock::High;
            strEnumerator = "High";
            valExpected = 5.12e9;
        }
        else
        {
            enumerator = static_cast<EProcessorClock>(iEnumerator);
            strEnumerator = QString::number(static_cast<int>(enumerator));
            valExpected = QVariant();
        }

        strExpectedValue = "CEnumProcessorClock::toValue(" + strEnumerator + "): ";
        if( valExpected.canConvert(QVariant::Double) ) strExpectedValue += QString::number(valExpected.toDouble(),'f',1);
        strlstExpectedValues.append(strExpectedValue);

        valResult = CEnumProcessorClock::toValue(enumerator);
        strResultValue = "CEnumProcessorClock::toValue(" + strEnumerator + "): ";
        if( valResult.canConvert(QVariant::Double) ) strResultValue += QString::number(valResult.toDouble(),'f',1);
        strlstResultValues.append(strResultValue);
    }

    iEnumerator = -1;
    enumerator = static_cast<EProcessorClock>(iEnumerator);
    strEnumerator = QString::number(static_cast<int>(enumerator));

    strExpectedValue = "CEnumProcessorClock::toValue(" + strEnumerator + "): ";
    strExpectedValue += "Enumerator " + strEnumerator + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumProcessorClock::toValue(" + strEnumerator + "): ";
        valResult = CEnumProcessorClock::toValue(enumerator);
        if( valResult.canConvert(QVariant::Double) ) strResultValue += QString::number(valResult.toDouble(),'f',1);
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    iEnumerator = CEnumProcessorClock::count();
    enumerator = static_cast<EProcessorClock>(iEnumerator);
    strEnumerator = QString::number(static_cast<int>(enumerator));

    strExpectedValue = "CEnumProcessorClock::toValue(" + strEnumerator + "): ";
    strExpectedValue += "Enumerator " + strEnumerator + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumProcessorClock::toValue(" + strEnumerator + "): ";
        valResult = CEnumProcessorClock::toValue(enumerator);
        if( valResult.canConvert(QVariant::Double) ) strResultValue += QString::number(valResult.toDouble(),'f',1);
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        if( iEnumerator == 0 )
        {
            enumerator = EProcessorClock::Low;
            strEnumerator = "Low";
            valExpected = 5.12e6;
        }
        else if( iEnumerator == 1 )
        {
            enumerator = EProcessorClock::Medium;
            strEnumerator = "Medium";
            valExpected = 5.12e8;
        }
        else if( iEnumerator == 2 )
        {
            enumerator = EProcessorClock::High;
            strEnumerator = "High";
            valExpected = 5.12e9;
        }
        else
        {
            enumerator = static_cast<EProcessorClock>(iEnumerator);
            strEnumerator = QString::number(static_cast<int>(enumerator));
            valExpected = QVariant();
        }

        strExpectedValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", Double, &bOk): ";
        if( valExpected.canConvert(QVariant::Double) ) strExpectedValue += QString::number(valExpected.toDouble(),'f',1) + ", true";
        else strExpectedValue += valExpected.toString() + ", false";
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", Double, &bOk): ";
        valResult = CEnumProcessorClock::toValue(enumerator, QVariant::Double, &bOk);
        if( bOk ) strResultValue += QString::number(valResult.toDouble(),'f',1) + ", " + bool2Str(bOk);
        else strResultValue += valResult.toString() + ", " + bool2Str(bOk);
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    enumerator = static_cast<EProcessorClock>(iEnumerator);
    strEnumerator = QString::number(static_cast<int>(enumerator));

    strExpectedValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", Double, &bOk): ";
    strExpectedValue += "Enumerator " + strEnumerator + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", Double, &bOk): ";
        valResult = CEnumProcessorClock::toValue(enumerator, QVariant::Double, &bOk);
        if( bOk ) strResultValue += QString::number(valResult.toDouble(),'f',1) + ", " + bool2Str(bOk);
        else strResultValue += valResult.toString() + ", " + bool2Str(bOk);
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    iEnumerator = CEnumProcessorClock::count();
    enumerator = static_cast<EProcessorClock>(iEnumerator);
    strEnumerator = QString::number(static_cast<int>(enumerator));

    strExpectedValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", Double, &bOk): ";
    strExpectedValue += "Enumerator " + strEnumerator + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", Double, &bOk): ";
        valResult = CEnumProcessorClock::toValue(enumerator, QVariant::Double, &bOk);
        if( bOk ) strResultValue += QString::number(valResult.toDouble(),'f',1) + ", " + bool2Str(bOk);
        else strResultValue += valResult.toString() + ", " + bool2Str(bOk);
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        if( iEnumerator == 0 )
        {
            enumerator = EProcessorClock::Low;
            strEnumerator = "Low";
            valExpected = 5.12e6;
        }
        else if( iEnumerator == 1 )
        {
            enumerator = EProcessorClock::Medium;
            strEnumerator = "Medium";
            valExpected = 5.12e8;
        }
        else if( iEnumerator == 2 )
        {
            enumerator = EProcessorClock::High;
            strEnumerator = "High";
            valExpected = 5.12e9;
        }
        else
        {
            enumerator = static_cast<EProcessorClock>(iEnumerator);
            strEnumerator = QString::number(static_cast<int>(enumerator));
            valExpected = QVariant();
        }

        strExpectedValue = "CEnumProcessorClock::toValue(" + strEnumerator + "): ";
        if( valExpected.canConvert(QVariant::Double) ) strExpectedValue += QString::number(valExpected.toDouble(),'f',1);
        else strExpectedValue += valExpected.toString();
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "CEnumProcessorClock::toValue(" + strEnumerator + "): ";
        valResult = CEnumProcessorClock::toValue(strEnumerator);
        if( valResult.canConvert(QVariant::Double) ) strResultValue += QString::number(valResult.toDouble(),'f',1);
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )
    {
        strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));

        for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
        {
            if( iEnumerator == 0 )
            {
                valExpected = 5.12e6;

                if( idxAlias == EEnumEntryAliasStrName ) strEnumerator = "Low";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumerator = "L";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumerator = "Low (5.12 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumerator = "LOW";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumerator = "LOW";
                else strEnumerator = QString::number(iEnumerator);
            }
            else if( iEnumerator == 1 )
            {
                valExpected = 5.12e8;

                if( idxAlias == EEnumEntryAliasStrName ) strEnumerator = "Medium";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumerator = "M";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumerator = "Medium (512.0 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumerator = "MED";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumerator = "MEDium";
                else strEnumerator = QString::number(iEnumerator);
            }
            else if( iEnumerator == 2 )
            {
                valExpected = 5.12e9;

                if( idxAlias == EEnumEntryAliasStrName ) strEnumerator = "High";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumerator = "H";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumerator = "High (5.12 GHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumerator = "HIGH";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumerator = "HIGH";
                else strEnumerator = QString::number(iEnumerator);
            }
            else
            {
                valExpected = QVariant();
                strEnumerator = QString::number(iEnumerator);
            }

            strExpectedValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", Double, " + strAlias + "): ";
            if( valExpected.canConvert(QVariant::Double) ) strExpectedValue += QString::number(valExpected.toDouble(),'f',1);
            else strExpectedValue += valExpected.toString();
            strlstExpectedValues.append(strExpectedValue);

            strResultValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", Double, " + strAlias + "): ";
            valResult = CEnumProcessorClock::toValue(strEnumerator, QVariant::Double, idxAlias);
            if( valResult.canConvert(QVariant::Double) ) strResultValue += QString::number(valResult.toDouble(),'f',1);
            else strResultValue += valResult.toString();
            strlstResultValues.append(strResultValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumerator = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    valExpected = 5.12e8;

    strExpectedValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    if( valExpected.canConvert(QVariant::Double) ) strExpectedValue += QString::number(valExpected.toDouble(),'f',1) + ", true";
    else strExpectedValue += valExpected.toString() + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = CEnumProcessorClock::toValue(strEnumerator, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strResultValue += QString::number(valResult.toDouble(),'f',1) + ", true";
    else strResultValue += valResult.toString() + ", false";
    strlstResultValues.append(strResultValue);

    strEnumerator = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    valExpected = QVariant();

    strExpectedValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    if( valExpected.canConvert(QVariant::Double) ) strExpectedValue += QString::number(valExpected.toDouble(),'f',1) + ", true";
    else strExpectedValue += valExpected.toString() + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = CEnumProcessorClock::toValue(strEnumerator, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strResultValue += QString::number(valResult.toDouble(),'f',1) + ", true";
    else strResultValue += valResult.toString() + ", false";
    strlstResultValues.append(strResultValue);

    strEnumerator = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    valExpected = 5.12e8;

    strExpectedValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    if( valExpected.canConvert(QVariant::Double) ) strExpectedValue += QString::number(valExpected.toDouble(),'f',1) + ", true";
    else strExpectedValue += valExpected.toString() + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = CEnumProcessorClock::toValue(strEnumerator, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strResultValue += QString::number(valResult.toDouble(),'f',1) + ", true";
    else strResultValue += valResult.toString() + ", false";
    strlstResultValues.append(strResultValue);

    strEnumerator = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    valExpected = 5.12e8;

    strExpectedValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Int, " + strCaseSensitivity + ", &bOk): ";
    if( valExpected.canConvert(QVariant::Double) ) strExpectedValue += QString::number(valExpected.toDouble(),'f',1) + ", true";
    else strExpectedValue += valExpected.toString() + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Int, " + strCaseSensitivity + ", &bOk): ";
    valResult = CEnumProcessorClock::toValue(strEnumerator, QVariant::Int, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strResultValue += QString::number(valResult.toDouble(),'f',1) + ", true";
    else strResultValue += valResult.toString() + ", false";
    strlstResultValues.append(strResultValue);

    strEnumerator = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    valExpected = QVariant();

    strExpectedValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    if( valExpected.canConvert(QVariant::Double) ) strExpectedValue += QString::number(valExpected.toDouble(),'f',1) + ", true";
    else strExpectedValue += valExpected.toString() + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = CEnumProcessorClock::toValue(strEnumerator, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strResultValue += QString::number(valResult.toDouble(),'f',1) + ", true";
    else strResultValue += valResult.toString() + ", false";
    strlstResultValues.append(strResultValue);

    strEnumerator = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    valExpected = 5.12e8;

    strExpectedValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    if( valExpected.canConvert(QVariant::Double) ) strExpectedValue += QString::number(valExpected.toDouble(),'f',1) + ", true";
    else strExpectedValue += valExpected.toString() + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = CEnumProcessorClock::toValue(strEnumerator, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strResultValue += QString::number(valResult.toDouble(),'f',1) + ", true";
    else strResultValue += valResult.toString() + ", false";
    strlstResultValues.append(strResultValue);

    strEnumerator = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias6;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    valExpected = QVariant();

    strExpectedValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += "Map does not contain alias strings at index " + QString::number(idxAlias);
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
        valResult = CEnumProcessorClock::toValue(strEnumerator, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
        if( valResult.canConvert(QVariant::Double) ) strResultValue += QString::number(valResult.toDouble(),'f',1) + ", true";
        else strResultValue += valResult.toString() + ", false";
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateUserDefinedClassMethodToValue

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedClassMethodFromString( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    CEnumProcessorClock enumInst;
    QString             strEnumeratorSource;
    QString             strEnumeratorResult;
    QString             strAlias;
    Qt::CaseSensitivity caseSensitivity;
    QString             strCaseSensitivity;

    int  iEnumerator;
    int  idxAlias;
    bool bOk;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        if( iEnumerator == 0 ) { strEnumeratorSource = "Low"; }
        else if( iEnumerator == 1 ) { strEnumeratorSource = "Medium"; }
        else if( iEnumerator == 2 ) { strEnumeratorSource = "High"; }
        else { strEnumeratorSource = QString::number(iEnumerator); }

        strExpectedValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + "): ";
        strExpectedValue += strEnumeratorSource;
        strlstExpectedValues.append(strExpectedValue);

        enumInst = CEnumProcessorClock::fromString(strEnumeratorSource);

        if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }

        strResultValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + "): ";
        strResultValue += strEnumeratorResult;
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )
    {
        strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));

        for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
        {
            if( iEnumerator == 0 )
            {
                strEnumeratorResult = "Low";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Low";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "L";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Low (5.12 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "LOW";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "LOW";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 1 )
            {
                strEnumeratorResult = "Medium";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Medium";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "M";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Medium (512.0 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "MED";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "MEDium";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 2 )
            {
                strEnumeratorResult = "High";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "High";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "H";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "High (5.12 GHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "HIGH";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "HIGH";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else
            {
                strEnumeratorResult = QString::number(iEnumerator);
                strEnumeratorSource = QString::number(iEnumerator);
            }

            strExpectedValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + "): ";
            strExpectedValue += strEnumeratorResult;
            strlstExpectedValues.append(strExpectedValue);

            strResultValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + "): ";
            enumInst = CEnumProcessorClock::fromString(strEnumeratorSource, idxAlias);
            if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
            else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
            else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
            else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
            strResultValue += strEnumeratorResult;
            strlstResultValues.append(strResultValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strExpectedValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strExpectedValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strExpectedValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strExpectedValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strExpectedValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strExpectedValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += strEnumeratorResult + ", true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias6;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strExpectedValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strExpectedValue += "Map does not contain alias strings at index " + QString::number(idxAlias);
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
        enumInst = CEnumProcessorClock::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
        if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
        else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
        else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
        else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
        strResultValue += strEnumeratorResult + ", " + bool2Str(bOk);
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateUserDefinedClassMethodFromString

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedClassMethodFromValue( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    CEnumProcessorClock enumInst;

    QString  strEnumerator;
    QVariant val;

    int  iEnumerator;
    bool bOk;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        val = QVariant();
        strEnumerator = "-1";

        if( iEnumerator == 0 ) { val = 5.12e6; strEnumerator = "Low"; }
        else if( iEnumerator == 1 ) { val = 5.12e8; strEnumerator = "Medium"; }
        else if( iEnumerator == 2 ) { val = 5.12e9; strEnumerator = "High"; }

        strExpectedValue = "CEnumProcessorClock::fromValue(" + val.toString() + "): ";
        strExpectedValue += strEnumerator;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "CEnumProcessorClock::fromValue(" + val.toString() + "): ";
        enumInst = CEnumProcessorClock::fromValue(val);

        if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(enumInst.enumerator())); }

        strResultValue += strEnumerator;
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    val = 5.12e8;

    strExpectedValue = "CEnumProcessorClock::fromValue(" + val.toString() + ", &bOk): ";
    strExpectedValue += "Medium, true";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::fromValue(" + val.toString() + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromValue(val, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
    else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
    else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
    else { strEnumerator = QString::number(static_cast<int>(enumInst.enumerator())); }
    strResultValue += strEnumerator + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    val = QVariant();

    strExpectedValue = "CEnumProcessorClock::fromValue(" + val.toString() + ", &bOk): ";
    strExpectedValue += "-1, false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::fromValue(" + val.toString() + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromValue(val, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
    else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
    else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
    else { strEnumerator = QString::number(static_cast<int>(enumInst.enumerator())); }
    strResultValue += strEnumerator + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    val = 5.12e3;

    strExpectedValue = "CEnumProcessorClock::fromValue(" + val.toString() + ", &bOk): ";
    strExpectedValue += "-1, false";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::fromValue(" + val.toString() + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromValue(val, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
    else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
    else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
    else { strEnumerator = QString::number(static_cast<int>(enumInst.enumerator())); }
    strResultValue += strEnumerator + ", " + bool2Str(bOk);
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateUserDefinedClassMethodFromValue

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedCtors( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    CEnumProcessorClock* pEnumInst = nullptr;

    QString strEnumerator;
    QString strAlias;
    QString strCaseSensitivity;

    Qt::CaseSensitivity caseSensitivity;

    int iEnumerator;
    int idxAlias;

    // -------------------------------------------------------------------------

    strExpectedValue = "CEnumProcessorClock::ctor(): -1";
    strlstExpectedValues.append(strExpectedValue);

    pEnumInst = new CEnumProcessorClock();
    if( pEnumInst->enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
    else if( pEnumInst->enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
    else if( pEnumInst->enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
    else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
    strResultValue = "CEnumProcessorClock::ctor(): " + strEnumerator;
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        strEnumerator = "-1";

        if( iEnumerator == 0 ) { strEnumerator = "Low"; }
        else if( iEnumerator == 1 ) { strEnumerator = "Medium"; }
        else if( iEnumerator == 2 ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(iEnumerator); }

        strExpectedValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        strExpectedValue += strEnumerator;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumProcessorClock(static_cast<EProcessorClock>(iEnumerator));
        if( pEnumInst->enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( pEnumInst->enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( pEnumInst->enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strResultValue += strEnumerator;
        strlstResultValues.append(strResultValue);
        delete pEnumInst;
        pEnumInst = nullptr;
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumerator = QString::number(iEnumerator);

    strExpectedValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
    strExpectedValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumProcessorClock(static_cast<EProcessorClock>(iEnumerator));
        if( pEnumInst->enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( pEnumInst->enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( pEnumInst->enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strResultValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    iEnumerator = CEnumProcessorClock::count();
    strEnumerator = QString::number(iEnumerator);

    strExpectedValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
    strExpectedValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumProcessorClock(static_cast<EProcessorClock>(iEnumerator));
        if( pEnumInst->enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( pEnumInst->enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( pEnumInst->enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strResultValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        strEnumerator = "-1";

        if( iEnumerator == 0 ) { strEnumerator = "Low"; }
        else if( iEnumerator == 1 ) { strEnumerator = "Medium"; }
        else if( iEnumerator == 2 ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(iEnumerator); }

        strExpectedValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        strExpectedValue += strEnumerator;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumProcessorClock(iEnumerator);
        if( pEnumInst->enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( pEnumInst->enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( pEnumInst->enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strResultValue += strEnumerator;
        strlstResultValues.append(strResultValue);
        delete pEnumInst;
        pEnumInst = nullptr;
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumerator = QString::number(iEnumerator);

    strExpectedValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
    strExpectedValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumProcessorClock(iEnumerator);
        if( pEnumInst->enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( pEnumInst->enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( pEnumInst->enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strResultValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    iEnumerator = CEnumProcessorClock::count();
    strEnumerator = QString::number(iEnumerator);

    strExpectedValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
    strExpectedValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumProcessorClock(iEnumerator);
        if( pEnumInst->enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( pEnumInst->enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( pEnumInst->enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strResultValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        strEnumerator = "-1";

        if( iEnumerator == 0 ) { strEnumerator = "Low"; }
        else if( iEnumerator == 1 ) { strEnumerator = "Medium"; }
        else if( iEnumerator == 2 ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(iEnumerator); }

        strExpectedValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        strExpectedValue += strEnumerator;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumProcessorClock(strEnumerator);
        if( pEnumInst->enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( pEnumInst->enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( pEnumInst->enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strResultValue += strEnumerator;
        strlstResultValues.append(strResultValue);
        delete pEnumInst;
        pEnumInst = nullptr;
    }

    // -------------------------------------------------------------------------

    strEnumerator = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);

    strExpectedValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strExpectedValue += strEnumerator;
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumProcessorClock(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EProcessorClock::Low ) strEnumerator = "Low";
    else if( pEnumInst->enumerator() == EProcessorClock::Medium ) strEnumerator = "Medium";
    else if( pEnumInst->enumerator() == EProcessorClock::High ) strEnumerator = "High";
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strResultValue += strEnumerator;
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "-1";

    strExpectedValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strExpectedValue += strEnumerator;
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumProcessorClock(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EProcessorClock::Low ) strEnumerator = "Low";
    else if( pEnumInst->enumerator() == EProcessorClock::Medium ) strEnumerator = "Medium";
    else if( pEnumInst->enumerator() == EProcessorClock::High ) strEnumerator = "High";
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strResultValue += strEnumerator;
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "Medium";

    strExpectedValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strExpectedValue += strEnumerator;
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumProcessorClock(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EProcessorClock::Low ) strEnumerator = "Low";
    else if( pEnumInst->enumerator() == EProcessorClock::Medium ) strEnumerator = "Medium";
    else if( pEnumInst->enumerator() == EProcessorClock::High ) strEnumerator = "High";
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strResultValue += strEnumerator;
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "Medium";

    strExpectedValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strExpectedValue += strEnumerator;
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumProcessorClock(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EProcessorClock::Low ) strEnumerator = "Low";
    else if( pEnumInst->enumerator() == EProcessorClock::Medium ) strEnumerator = "Medium";
    else if( pEnumInst->enumerator() == EProcessorClock::High ) strEnumerator = "High";
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strResultValue += strEnumerator;
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "-1";

    strExpectedValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strExpectedValue += strEnumerator;
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumProcessorClock(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EProcessorClock::Low ) strEnumerator = "Low";
    else if( pEnumInst->enumerator() == EProcessorClock::Medium ) strEnumerator = "Medium";
    else if( pEnumInst->enumerator() == EProcessorClock::High ) strEnumerator = "High";
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strResultValue += strEnumerator;
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "Medium";

    strExpectedValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strExpectedValue += strEnumerator;
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumProcessorClock(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EProcessorClock::Low ) strEnumerator = "Low";
    else if( pEnumInst->enumerator() == EProcessorClock::Medium ) strEnumerator = "Medium";
    else if( pEnumInst->enumerator() == EProcessorClock::High ) strEnumerator = "High";
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strResultValue += strEnumerator;
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias6;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "-1";

    strExpectedValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strExpectedValue += "Map does not contain alias strings at index " + QString::number(idxAlias);
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
        pEnumInst = new CEnumProcessorClock(strEnumerator, idxAlias, caseSensitivity);
        if( pEnumInst->enumerator() == EProcessorClock::Low ) strEnumerator = "Low";
        else if( pEnumInst->enumerator() == EProcessorClock::Medium ) strEnumerator = "Medium";
        else if( pEnumInst->enumerator() == EProcessorClock::High ) strEnumerator = "High";
        else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
        strResultValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateUserDefinedCtors

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedOperatorAssign( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    CEnumProcessorClock enumInst;

    QString  strEnumeratorSource;
    QString  strEnumeratorResult;
    QVariant val;
    QString  strAlias;

    int iEnumerator;
    int idxAlias;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        strEnumeratorSource = "-1";

        if( iEnumerator == 0 ) { strEnumeratorSource = "Low"; }
        else if( iEnumerator == 1 ) { strEnumeratorSource = "Medium"; }
        else if( iEnumerator == 2 ) { strEnumeratorSource = "High"; }
        else { strEnumeratorSource = QString::number(iEnumerator); }

        strExpectedValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
        strExpectedValue += strEnumeratorSource;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
        enumInst = static_cast<EProcessorClock>(iEnumerator);
        if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strResultValue += strEnumeratorResult;
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumeratorSource = QString::number(iEnumerator);

    strExpectedValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
    strExpectedValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
        enumInst = static_cast<EProcessorClock>(iEnumerator);
        if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strResultValue += strEnumeratorResult;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    iEnumerator = CEnumProcessorClock::count();
    strEnumeratorSource = QString::number(iEnumerator);

    strExpectedValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
    strExpectedValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
        enumInst = static_cast<EProcessorClock>(iEnumerator);
        if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strResultValue += strEnumeratorResult;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        strEnumeratorSource = "-1";

        if( iEnumerator == 0 ) { strEnumeratorSource = "Low"; }
        else if( iEnumerator == 1 ) { strEnumeratorSource = "Medium"; }
        else if( iEnumerator == 2 ) { strEnumeratorSource = "High"; }
        else { strEnumeratorSource = QString::number(iEnumerator); }

        strExpectedValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
        strExpectedValue += strEnumeratorSource;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
        enumInst = iEnumerator;
        if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strResultValue += strEnumeratorResult;
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumeratorSource = QString::number(iEnumerator);

    strExpectedValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
    strExpectedValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
        enumInst = iEnumerator;
        if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strResultValue += strEnumeratorResult;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    iEnumerator = CEnumProcessorClock::count();
    strEnumeratorSource = QString::number(iEnumerator);

    strExpectedValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
    strExpectedValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstExpectedValues.append(strExpectedValue);

    try
    {
        strResultValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
        enumInst = iEnumerator;
        if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strResultValue += strEnumeratorResult;
    }
    catch( CException& exc )
    {
        strResultValue += exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )
    {
        strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));

        for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
        {
            if( iEnumerator == 0 )
            {
                strEnumeratorResult = "Low";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Low";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "L";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Low (5.12 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "LOW";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "LOW";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 1 )
            {
                strEnumeratorResult = "Medium";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Medium";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "M";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Medium (512.0 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "MED";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "MEDium";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 2 )
            {
                strEnumeratorResult = "High";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "High";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "H";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "High (5.12 GHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "HIGH";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "HIGH";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else
            {
                strEnumeratorResult = QString::number(iEnumerator);
                strEnumeratorSource = QString::number(iEnumerator);
            }

            strExpectedValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
            strExpectedValue += strEnumeratorResult;
            strlstExpectedValues.append(strExpectedValue);

            strResultValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
            enumInst = strEnumeratorSource;
            if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
            else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
            else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
            else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
            strResultValue += strEnumeratorResult;
            strlstResultValues.append(strResultValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "medIUM";

    strExpectedValue = "CEnumProcessorClock = " + strEnumeratorSource + ": -1";
    strlstExpectedValues.append(strExpectedValue);

    strResultValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
    enumInst = strEnumeratorSource;
    if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strResultValue += strEnumeratorResult;
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateUserDefinedOperatorAssign

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedOperatorCompareWithEnumerator( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    EProcessorClock enumerator;

    CEnumProcessorClock enumInst;

    // -------------------------------------------------------------------------

    enumerator = EProcessorClock::Medium;
    enumInst = EProcessorClock::Medium;

    try
    {
        strExpectedValue = "Medium == Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == enumerator )
        {
            strResultValue = "Medium == Medium";
        }
        else
        {
            strResultValue = "Medium != Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium == Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != enumerator )
        {
            strResultValue = "Medium != Medium";
        }
        else
        {
            strResultValue = "Medium == Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Medium >= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < enumerator )
        {
            strResultValue = "Medium < Medium";
        }
        else
        {
            strResultValue = "Medium >= Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium <= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > enumerator )
        {
            strResultValue = "Medium > Medium";
        }
        else
        {
            strResultValue = "Medium <= Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Medium <= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= enumerator )
        {
            strResultValue = "Medium <= Medium";
        }
        else
        {
            strResultValue = "Medium > Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium >= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= enumerator )
        {
            strResultValue = "Medium >= Medium";
        }
        else
        {
            strResultValue = "Medium < Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Low;
    enumerator = EProcessorClock::High;

    try
    {
        strExpectedValue = "Low != High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == enumerator )
        {
            strResultValue = "Low == High";
        }
        else
        {
            strResultValue = "Low != High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Low != High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != enumerator )
        {
            strResultValue = "Low != High";
        }
        else
        {
            strResultValue = "Low == High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Low < High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < enumerator )
        {
            strResultValue = "Low < High";
        }
        else
        {
            strResultValue = "Low >= High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Low <= High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > enumerator )
        {
            strResultValue = "Low > High";
        }
        else
        {
            strResultValue = "Low <= High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Low <= High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= enumerator )
        {
            strResultValue = "Low <= High";
        }
        else
        {
            strResultValue = "Low > High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Low < High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= enumerator )
        {
            strResultValue = "Low >= High";
        }
        else
        {
            strResultValue = "Low < High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumProcessorClock(); // InvalidEnumerator
    enumerator = EProcessorClock::Medium;

    try
    {
        strExpectedValue = "Invalid != Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == enumerator )
        {
            strResultValue = "Invalid == Medium";
        }
        else
        {
            strResultValue = "Invalid != Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid != Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != enumerator )
        {
            strResultValue = "Invalid != Medium";
        }
        else
        {
            strResultValue = "Invalid == Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < enumerator )
        {
            strResultValue = "Invalid < Medium";
        }
        else
        {
            strResultValue = "Invalid >= Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > enumerator )
        {
            strResultValue = "Invalid > Medium";
        }
        else
        {
            strResultValue = "Invalid <= Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= enumerator )
        {
            strResultValue = "Invalid <= Medium";
        }
        else
        {
            strResultValue = "Invalid > Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= enumerator )
        {
            strResultValue = "Invalid >= Medium";
        }
        else
        {
            strResultValue = "Invalid < Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Medium;
    enumerator = static_cast<EProcessorClock>(EInvalidEnumerator);

    try
    {
        strExpectedValue = "Medium != Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == enumerator )
        {
            strResultValue = "Medium == Invalid";
        }
        else
        {
            strResultValue = "Medium != Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium != Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != enumerator )
        {
            strResultValue = "Medium != Invalid";
        }
        else
        {
            strResultValue = "Medium == Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < enumerator )
        {
            strResultValue = "Medium < Invalid";
        }
        else
        {
            strResultValue = "Medium >= Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > enumerator )
        {
            strResultValue = "Medium > Invalid";
        }
        else
        {
            strResultValue = "Medium <= Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= enumerator )
        {
            strResultValue = "Medium <= Invalid";
        }
        else
        {
            strResultValue = "Medium > Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= enumerator )
        {
            strResultValue = "Medium >= Invalid";
        }
        else
        {
            strResultValue = "Medium < Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumProcessorClock(); // InvalidEnumerator
    enumerator = static_cast<EProcessorClock>(EInvalidEnumerator);

    try
    {
        strExpectedValue = "Invalid == Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == enumerator )
        {
            strResultValue = "Invalid == Invalid";
        }
        else
        {
            strResultValue = "Invalid != Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid == Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != enumerator )
        {
            strResultValue = "Invalid != Invalid";
        }
        else
        {
            strResultValue = "Invalid == Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < enumerator )
        {
            strResultValue = "Invalid < Invalid";
        }
        else
        {
            strResultValue = "Invalid >= Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > enumerator )
        {
            strResultValue = "Invalid > Invalid";
        }
        else
        {
            strResultValue = "Invalid <= Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= enumerator )
        {
            strResultValue = "Invalid <= Invalid";
        }
        else
        {
            strResultValue = "Invalid > Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= enumerator )
        {
            strResultValue = "Invalid >= Invalid";
        }
        else
        {
            strResultValue = "Invalid < Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateUserDefinedOperatorCompareWithEnumerator

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedOperatorCompareWithInt( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    CEnumProcessorClock enumInst;

    int iEnumerator;

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Medium;
    iEnumerator = static_cast<int>(EProcessorClock::Medium);

    try
    {
        strExpectedValue = "Medium == Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == iEnumerator )
        {
            strResultValue = "Medium == Medium";
        }
        else
        {
            strResultValue = "Medium != Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium == Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != iEnumerator )
        {
            strResultValue = "Medium != Medium";
        }
        else
        {
            strResultValue = "Medium == Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Medium >= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < iEnumerator )
        {
            strResultValue = "Medium < Medium";
        }
        else
        {
            strResultValue = "Medium >= Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium <= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > iEnumerator )
        {
            strResultValue = "Medium > Medium";
        }
        else
        {
            strResultValue = "Medium <= Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Medium <= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= iEnumerator )
        {
            strResultValue = "Medium <= Medium";
        }
        else
        {
            strResultValue = "Medium > Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium >= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= iEnumerator )
        {
            strResultValue = "Medium >= Medium";
        }
        else
        {
            strResultValue = "Medium < Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Low;
    iEnumerator = static_cast<int>(EProcessorClock::High);

    try
    {
        strExpectedValue = "Low != High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == iEnumerator )
        {
            strResultValue = "Low == High";
        }
        else
        {
            strResultValue = "Low != High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Low != High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != iEnumerator )
        {
            strResultValue = "Low != High";
        }
        else
        {
            strResultValue = "Low == High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Low < High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < iEnumerator )
        {
            strResultValue = "Low < High";
        }
        else
        {
            strResultValue = "Low >= High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Low <= High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > iEnumerator )
        {
            strResultValue = "Low > High";
        }
        else
        {
            strResultValue = "Low <= High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Low <= High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= iEnumerator )
        {
            strResultValue = "Low <= High";
        }
        else
        {
            strResultValue = "Low > High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Low < High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= iEnumerator )
        {
            strResultValue = "Low >= High";
        }
        else
        {
            strResultValue = "Low < High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumProcessorClock(); // InvalidEnumerator
    iEnumerator = static_cast<int>(EProcessorClock::Medium);

    try
    {
        strExpectedValue = "Invalid != Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == iEnumerator )
        {
            strResultValue = "Invalid == Medium";
        }
        else
        {
            strResultValue = "Invalid != Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid != Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != iEnumerator )
        {
            strResultValue = "Invalid != Medium";
        }
        else
        {
            strResultValue = "Invalid == Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid < Medium: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < iEnumerator )
        {
            strResultValue = "Invalid < Medium";
        }
        else
        {
            strResultValue = "Invalid < Medium: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid > Medium: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > iEnumerator )
        {
            strResultValue = "Invalid > Medium";
        }
        else
        {
            strResultValue = "Invalid > Medium: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid <= Medium: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= iEnumerator )
        {
            strResultValue = "Invalid <= Medium";
        }
        else
        {
            strResultValue = "Invalid <= Medium: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid >= Medium: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= iEnumerator )
        {
            strResultValue = "Invalid >= Medium";
        }
        else
        {
            strResultValue = "Invalid >= Medium: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Medium;
    iEnumerator = EInvalidEnumerator;

    try
    {
        strExpectedValue = "Medium != Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == iEnumerator )
        {
            strResultValue = "Medium == Invalid";
        }
        else
        {
            strResultValue = "Medium != Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium != Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != iEnumerator )
        {
            strResultValue = "Medium != Invalid";
        }
        else
        {
            strResultValue = "Medium == Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Medium < Invalid: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < iEnumerator )
        {
            strResultValue = "Medium < Invalid";
        }
        else
        {
            strResultValue = "Medium < Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium > Invalid: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > iEnumerator )
        {
            strResultValue = "Medium > Invalid";
        }
        else
        {
            strResultValue = "Medium > Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Medium <= Invalid: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= iEnumerator )
        {
            strResultValue = "Medium <= Invalid";
        }
        else
        {
            strResultValue = "Medium <= Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium >= Invalid: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= iEnumerator )
        {
            strResultValue = "Medium >= Invalid";
        }
        else
        {
            strResultValue = "Medium >= Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumProcessorClock(); // InvalidEnumerator
    iEnumerator = EInvalidEnumerator;

    try
    {
        strExpectedValue = "Invalid == Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == iEnumerator )
        {
            strResultValue = "Invalid == Invalid";
        }
        else
        {
            strResultValue = "Invalid != Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid == Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != iEnumerator )
        {
            strResultValue = "Invalid != Invalid";
        }
        else
        {
            strResultValue = "Invalid == Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid < Invalid: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < iEnumerator )
        {
            strResultValue = "Invalid < Invalid";
        }
        else
        {
            strResultValue = "Invalid < Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid > Invalid: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > iEnumerator )
        {
            strResultValue = "Invalid > Invalid";
        }
        else
        {
            strResultValue = "Invalid > Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid <= Invalid: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= iEnumerator )
        {
            strResultValue = "Invalid <= Invalid";
        }
        else
        {
            strResultValue = "Invalid <= Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid >= Invalid: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= iEnumerator )
        {
            strResultValue = "Invalid >= Invalid";
        }
        else
        {
            strResultValue = "Invalid >= Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateUserDefinedOperatorCompareWithInt

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedOperatorCompareWithCharPtr( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    CEnumProcessorClock enumInst;

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Medium;
    const char* szEnumerator = "Medium";

    try
    {
        strExpectedValue = "Medium == Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == szEnumerator )
        {
            strResultValue = "Medium == Medium";
        }
        else
        {
            strResultValue = "Medium != Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium == Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != szEnumerator )
        {
            strResultValue = "Medium != Medium";
        }
        else
        {
            strResultValue = "Medium == Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Medium >= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < szEnumerator )
        {
            strResultValue = "Medium < Medium";
        }
        else
        {
            strResultValue = "Medium >= Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium <= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > szEnumerator )
        {
            strResultValue = "Medium > Medium";
        }
        else
        {
            strResultValue = "Medium <= Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Medium <= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= szEnumerator )
        {
            strResultValue = "Medium <= Medium";
        }
        else
        {
            strResultValue = "Medium > Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium >= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= szEnumerator )
        {
            strResultValue = "Medium >= Medium";
        }
        else
        {
            strResultValue = "Medium < Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Low;
    szEnumerator = "High";

    try
    {
        strExpectedValue = "Low != High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == szEnumerator )
        {
            strResultValue = "Low == High";
        }
        else
        {
            strResultValue = "Low != High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Low != High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != szEnumerator )
        {
            strResultValue = "Low != High";
        }
        else
        {
            strResultValue = "Low == High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Low < High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < szEnumerator )
        {
            strResultValue = "Low < High";
        }
        else
        {
            strResultValue = "Low >= High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Low <= High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > szEnumerator )
        {
            strResultValue = "Low > High";
        }
        else
        {
            strResultValue = "Low <= High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Low <= High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= szEnumerator )
        {
            strResultValue = "Low <= High";
        }
        else
        {
            strResultValue = "Low > High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Low < High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= szEnumerator )
        {
            strResultValue = "Low >= High";
        }
        else
        {
            strResultValue = "Low < High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumProcessorClock(); // InvalidEnumerator
    szEnumerator = "Medium";

    try
    {
        strExpectedValue = "Invalid != Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == szEnumerator )
        {
            strResultValue = "Invalid == Medium";
        }
        else
        {
            strResultValue = "Invalid != Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid != Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != szEnumerator )
        {
            strResultValue = "Invalid != Medium";
        }
        else
        {
            strResultValue = "Invalid == Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid < Medium: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < szEnumerator )
        {
            strResultValue = "Invalid < Medium";
        }
        else
        {
            strResultValue = "Invalid < Medium: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid > Medium: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > szEnumerator )
        {
            strResultValue = "Invalid > Medium";
        }
        else
        {
            strResultValue = "Invalid > Medium: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid <= Medium: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= szEnumerator )
        {
            strResultValue = "Invalid <= Medium";
        }
        else
        {
            strResultValue = "Invalid <= Medium: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid >= Medium: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= szEnumerator )
        {
            strResultValue = "Invalid >= Medium";
        }
        else
        {
            strResultValue = "Invalid >= Medium: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Medium;
    szEnumerator = "Undefined";

    try
    {
        strExpectedValue = "Medium != Undefined";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == szEnumerator )
        {
            strResultValue = "Medium == Undefined";
        }
        else
        {
            strResultValue = "Medium != Undefined";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium != Undefined";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != szEnumerator )
        {
            strResultValue = "Medium != Undefined";
        }
        else
        {
            strResultValue = "Medium == Undefined";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Medium < Undefined: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < szEnumerator )
        {
            strResultValue = "Medium < Undefined";
        }
        else
        {
            strResultValue = "Medium < Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium > Undefined: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > szEnumerator )
        {
            strResultValue = "Medium > Undefined";
        }
        else
        {
            strResultValue = "Medium > Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Medium <= Undefined: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= szEnumerator )
        {
            strResultValue = "Medium <= Undefined";
        }
        else
        {
            strResultValue = "Medium <= Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium >= Undefined: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= szEnumerator )
        {
            strResultValue = "Medium >= Undefined";
        }
        else
        {
            strResultValue = "Medium >= Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumProcessorClock(); // InvalidEnumerator
    szEnumerator = "Undefined";

    try
    {
        strExpectedValue = "Invalid != Undefined";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == szEnumerator )
        {
            strResultValue = "Invalid == Undefined";
        }
        else
        {
            strResultValue = "Invalid != Undefined";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid != Undefined";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != szEnumerator )
        {
            strResultValue = "Invalid != Undefined";
        }
        else
        {
            strResultValue = "Invalid == Undefined";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid < Undefined: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < szEnumerator )
        {
            strResultValue = "Invalid < Undefined";
        }
        else
        {
            strResultValue = "Invalid < Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid > Undefined: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > szEnumerator )
        {
            strResultValue = "Invalid > Undefined";
        }
        else
        {
            strResultValue = "Invalid > Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid <= Undefined: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= szEnumerator )
        {
            strResultValue = "Invalid <= Undefined";
        }
        else
        {
            strResultValue = "Invalid <= Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid >= Undefined: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= szEnumerator )
        {
            strResultValue = "Invalid >= Undefined";
        }
        else
        {
            strResultValue = "Invalid >= Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateUserDefinedOperatorCompareWithCharPtr

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedOperatorCompareWithQString( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    CEnumProcessorClock enumInst;

    QString strEnumerator;

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Medium;
    strEnumerator = "Medium";

    try
    {
        strExpectedValue = "Medium == Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == strEnumerator )
        {
            strResultValue = "Medium == Medium";
        }
        else
        {
            strResultValue = "Medium != Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium == Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != strEnumerator )
        {
            strResultValue = "Medium != Medium";
        }
        else
        {
            strResultValue = "Medium == Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Medium >= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < strEnumerator )
        {
            strResultValue = "Medium < Medium";
        }
        else
        {
            strResultValue = "Medium >= Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium <= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > strEnumerator )
        {
            strResultValue = "Medium > Medium";
        }
        else
        {
            strResultValue = "Medium <= Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Medium <= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= strEnumerator )
        {
            strResultValue = "Medium <= Medium";
        }
        else
        {
            strResultValue = "Medium > Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium >= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= strEnumerator )
        {
            strResultValue = "Medium >= Medium";
        }
        else
        {
            strResultValue = "Medium < Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Low;
    strEnumerator = "High";

    try
    {
        strExpectedValue = "Low != High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == strEnumerator )
        {
            strResultValue = "Low == High";
        }
        else
        {
            strResultValue = "Low != High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Low != High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != strEnumerator )
        {
            strResultValue = "Low != High";
        }
        else
        {
            strResultValue = "Low == High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Low < High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < strEnumerator )
        {
            strResultValue = "Low < High";
        }
        else
        {
            strResultValue = "Low >= High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Low <= High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > strEnumerator )
        {
            strResultValue = "Low > High";
        }
        else
        {
            strResultValue = "Low <= High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Low <= High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= strEnumerator )
        {
            strResultValue = "Low <= High";
        }
        else
        {
            strResultValue = "Low > High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Low < High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= strEnumerator )
        {
            strResultValue = "Low >= High";
        }
        else
        {
            strResultValue = "Low < High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumProcessorClock(); // InvalidEnumerator
    strEnumerator = "Medium";

    try
    {
        strExpectedValue = "Invalid != Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == strEnumerator )
        {
            strResultValue = "Invalid == Medium";
        }
        else
        {
            strResultValue = "Invalid != Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid != Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != strEnumerator )
        {
            strResultValue = "Invalid != Medium";
        }
        else
        {
            strResultValue = "Invalid == Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid < Medium: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < strEnumerator )
        {
            strResultValue = "Invalid < Medium";
        }
        else
        {
            strResultValue = "Invalid < Medium: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid > Medium: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > strEnumerator )
        {
            strResultValue = "Invalid > Medium";
        }
        else
        {
            strResultValue = "Invalid > Medium: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid <= Medium: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= strEnumerator )
        {
            strResultValue = "Invalid <= Medium";
        }
        else
        {
            strResultValue = "Invalid <= Medium: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid >= Medium: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= strEnumerator )
        {
            strResultValue = "Invalid >= Medium";
        }
        else
        {
            strResultValue = "Invalid >= Medium: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Medium;
    strEnumerator = "Undefined";

    try
    {
        strExpectedValue = "Medium != Undefined";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == strEnumerator )
        {
            strResultValue = "Medium == Undefined";
        }
        else
        {
            strResultValue = "Medium != Undefined";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium != Undefined";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != strEnumerator )
        {
            strResultValue = "Medium != Undefined";
        }
        else
        {
            strResultValue = "Medium == Undefined";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Medium < Undefined: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < strEnumerator )
        {
            strResultValue = "Medium < Undefined";
        }
        else
        {
            strResultValue = "Medium < Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium > Undefined: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > strEnumerator )
        {
            strResultValue = "Medium > Undefined";
        }
        else
        {
            strResultValue = "Medium > Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Medium <= Undefined: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= strEnumerator )
        {
            strResultValue = "Medium <= Undefined";
        }
        else
        {
            strResultValue = "Medium <= Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium >= Undefined: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= strEnumerator )
        {
            strResultValue = "Medium >= Undefined";
        }
        else
        {
            strResultValue = "Medium >= Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumProcessorClock(); // InvalidEnumerator
    strEnumerator = "Undefined";

    try
    {
        strExpectedValue = "Invalid != Undefined";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst == strEnumerator )
        {
            strResultValue = "Invalid == Undefined";
        }
        else
        {
            strResultValue = "Invalid != Undefined";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid != Undefined";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst != strEnumerator )
        {
            strResultValue = "Invalid != Undefined";
        }
        else
        {
            strResultValue = "Invalid == Undefined";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid < Undefined: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst < strEnumerator )
        {
            strResultValue = "Invalid < Undefined";
        }
        else
        {
            strResultValue = "Invalid < Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid > Undefined: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst > strEnumerator )
        {
            strResultValue = "Invalid > Undefined";
        }
        else
        {
            strResultValue = "Invalid > Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Invalid <= Undefined: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst <= strEnumerator )
        {
            strResultValue = "Invalid <= Undefined";
        }
        else
        {
            strResultValue = "Invalid <= Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid >= Undefined: false";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst >= strEnumerator )
        {
            strResultValue = "Invalid >= Undefined";
        }
        else
        {
            strResultValue = "Invalid >= Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateUserDefinedOperatorCompareWithQString

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedOperatorCompareWithEnumTemplate( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    CEnumProcessorClock enumInst1;
    CEnumProcessorClock enumInst2;

    // -------------------------------------------------------------------------

    enumInst1 = EProcessorClock::Medium;
    enumInst2 = EProcessorClock::Medium;

    try
    {
        strExpectedValue = "Medium == Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 == enumInst2 )
        {
            strResultValue = "Medium == Medium";
        }
        else
        {
            strResultValue = "Medium != Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium == Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 != enumInst2 )
        {
            strResultValue = "Medium != Medium";
        }
        else
        {
            strResultValue = "Medium == Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Medium >= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 < enumInst2 )
        {
            strResultValue = "Medium < Medium";
        }
        else
        {
            strResultValue = "Medium >= Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium <= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 > enumInst2 )
        {
            strResultValue = "Medium > Medium";
        }
        else
        {
            strResultValue = "Medium <= Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Medium <= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 <= enumInst2 )
        {
            strResultValue = "Medium <= Medium";
        }
        else
        {
            strResultValue = "Medium > Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium >= Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 >= enumInst2 )
        {
            strResultValue = "Medium >= Medium";
        }
        else
        {
            strResultValue = "Medium < Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst1 = EProcessorClock::Low;
    enumInst2 = EProcessorClock::High;

    try
    {
        strExpectedValue = "Low != High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 == enumInst2 )
        {
            strResultValue = "Low == High";
        }
        else
        {
            strResultValue = "Low != High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Low != High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 != enumInst2 )
        {
            strResultValue = "Low != High";
        }
        else
        {
            strResultValue = "Low == High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Low < High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 < enumInst2 )
        {
            strResultValue = "Low < High";
        }
        else
        {
            strResultValue = "Low >= High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Low <= High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 > enumInst2 )
        {
            strResultValue = "Low > High";
        }
        else
        {
            strResultValue = "Low <= High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Low <= High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 <= enumInst2 )
        {
            strResultValue = "Low <= High";
        }
        else
        {
            strResultValue = "Low > High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Low < High";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 >= enumInst2 )
        {
            strResultValue = "Low >= High";
        }
        else
        {
            strResultValue = "Low < High";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst1 = CEnumProcessorClock(); // InvalidEnumerator
    enumInst2 = EProcessorClock::Medium;

    try
    {
        strExpectedValue = "Invalid != Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 == enumInst2 )
        {
            strResultValue = "Invalid == Medium";
        }
        else
        {
            strResultValue = "Invalid != Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid != Medium";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 != enumInst2 )
        {
            strResultValue = "Invalid != Medium";
        }
        else
        {
            strResultValue = "Invalid == Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 < enumInst2 )
        {
            strResultValue = "Invalid < Medium";
        }
        else
        {
            strResultValue = "Invalid >= Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 > enumInst2 )
        {
            strResultValue = "Invalid > Medium";
        }
        else
        {
            strResultValue = "Invalid <= Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 <= enumInst2 )
        {
            strResultValue = "Invalid <= Medium";
        }
        else
        {
            strResultValue = "Invalid > Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 >= enumInst2 )
        {
            strResultValue = "Invalid >= Medium";
        }
        else
        {
            strResultValue = "Invalid < Medium";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst1 = EProcessorClock::Medium;
    enumInst2 = CEnumProcessorClock(); // InvalidEnumerator

    try
    {
        strExpectedValue = "Medium != Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 == enumInst2 )
        {
            strResultValue = "Medium == Invalid";
        }
        else
        {
            strResultValue = "Medium != Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Medium != Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 != enumInst2 )
        {
            strResultValue = "Medium != Invalid";
        }
        else
        {
            strResultValue = "Medium == Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 < enumInst2 )
        {
            strResultValue = "Medium < Invalid";
        }
        else
        {
            strResultValue = "Medium >= Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 > enumInst2 )
        {
            strResultValue = "Medium > Invalid";
        }
        else
        {
            strResultValue = "Medium <= Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 <= enumInst2 )
        {
            strResultValue = "Medium <= Invalid";
        }
        else
        {
            strResultValue = "Medium > Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 >= enumInst2 )
        {
            strResultValue = "Medium >= Invalid";
        }
        else
        {
            strResultValue = "Medium < Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    enumInst1 = CEnumProcessorClock(); // InvalidEnumerator
    enumInst2 = CEnumProcessorClock(); // InvalidEnumerator

    try
    {
        strExpectedValue = "Invalid == Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 == enumInst2 )
        {
            strResultValue = "Invalid == Invalid";
        }
        else
        {
            strResultValue = "Invalid != Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Invalid == Invalid";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 != enumInst2 )
        {
            strResultValue = "Invalid != Invalid";
        }
        else
        {
            strResultValue = "Invalid == Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 < enumInst2 )
        {
            strResultValue = "Invalid < Invalid";
        }
        else
        {
            strResultValue = "Invalid >= Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 > enumInst2 )
        {
            strResultValue = "Invalid > Invalid";
        }
        else
        {
            strResultValue = "Invalid <= Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 <= enumInst2 )
        {
            strResultValue = "Invalid <= Invalid";
        }
        else
        {
            strResultValue = "Invalid > Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    try
    {
        strExpectedValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstExpectedValues.append(strExpectedValue);

        if( enumInst1 >= enumInst2 )
        {
            strResultValue = "Invalid >= Invalid";
        }
        else
        {
            strResultValue = "Invalid < Invalid";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateUserDefinedOperatorCompareWithEnumTemplate

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedOperatorIncDec( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    QString strEnumeratorResult;
    QString strEnumeratorSource;

    int iEnumerator;

    CEnumProcessorClock enumInst;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        try
        {
            enumInst = static_cast<EProcessorClock>(iEnumerator);
            if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorSource = "Low"; strEnumeratorResult = "Medium"; }
            else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorSource = "Medium"; strEnumeratorResult = "High"; }
            else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorSource = "High"; strEnumeratorResult = "Invalid"; }
            else strEnumeratorSource = "Invalid";
            strExpectedValue = strEnumeratorSource + "++: " + strEnumeratorResult;
            strlstExpectedValues.append(strExpectedValue);

            enumInst++;
            if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
            else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
            else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
            else strEnumeratorResult = "Invalid";
            strResultValue = strEnumeratorSource + "++: " + strEnumeratorResult;
        }
        catch( CException& exc )
        {
            strResultValue = exc.getAddErrInfo();
        }
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        try
        {
            enumInst = static_cast<EProcessorClock>(iEnumerator);
            if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorSource = "Low"; strEnumeratorResult = "Medium"; }
            else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorSource = "Medium"; strEnumeratorResult = "High"; }
            else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorSource = "High"; strEnumeratorResult = "Invalid"; }
            else strEnumeratorSource = "Invalid";
            strExpectedValue = "++" + strEnumeratorSource + ": " + strEnumeratorResult;
            strlstExpectedValues.append(strExpectedValue);

            ++enumInst;
            if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
            else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
            else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
            else strEnumeratorResult = "Invalid";
            strResultValue = "++" + strEnumeratorSource + ": " + strEnumeratorResult;
        }
        catch( CException& exc )
        {
            strResultValue = exc.getAddErrInfo();
        }
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( iEnumerator = CEnumProcessorClock::count()-1; iEnumerator >= 0; --iEnumerator )
    {
        try
        {
            enumInst = static_cast<EProcessorClock>(iEnumerator);
            if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorSource = "Low"; strEnumeratorResult = "Invalid"; }
            else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorSource = "Medium"; strEnumeratorResult = "Low"; }
            else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorSource = "High"; strEnumeratorResult = "Medium"; }
            else strEnumeratorSource = "Invalid";
            strExpectedValue = strEnumeratorSource + "--: " + strEnumeratorResult;
            strlstExpectedValues.append(strExpectedValue);

            enumInst--;
            if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
            else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
            else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
            else strEnumeratorResult = "Invalid";
            strResultValue = strEnumeratorSource + "--: " + strEnumeratorResult;
        }
        catch( CException& exc )
        {
            strResultValue = exc.getAddErrInfo();
        }
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( iEnumerator = CEnumProcessorClock::count()-1; iEnumerator >= 0; --iEnumerator )
    {
        try
        {
            enumInst = static_cast<EProcessorClock>(iEnumerator);
            if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorSource = "Low"; strEnumeratorResult = "Invalid"; }
            else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorSource = "Medium"; strEnumeratorResult = "Low"; }
            else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorSource = "High"; strEnumeratorResult = "Medium"; }
            else strEnumeratorSource = "Invalid";
            strExpectedValue = "--" + strEnumeratorSource + ": " + strEnumeratorResult;
            strlstExpectedValues.append(strExpectedValue);

            --enumInst;
            if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
            else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
            else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
            else strEnumeratorResult = "Invalid";
            strResultValue = "--" + strEnumeratorSource + ": " + strEnumeratorResult;
        }
        catch( CException& exc )
        {
            strResultValue = exc.getAddErrInfo();
        }
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateUserDefinedOperatorIncDec

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedInstMethodToString( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    QString strEnumeratorSource;
    QString strEnumeratorResult;
    QString strAlias;

    int  iEnumerator;
    int  idxAlias;

    CEnumProcessorClock enumInst;

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )
    {
        strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));

        for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
        {
            try
            {
                enumInst = static_cast<EProcessorClock>(iEnumerator);

                if( iEnumerator == static_cast<int>(EProcessorClock::Low) )
                {
                    if( idxAlias == EEnumEntryAliasStrName ) { strEnumeratorSource = "Low"; }
                    else if( idxAlias == EEnumEntryAliasStrSymbol ) { strEnumeratorSource = "L"; }
                    else if( idxAlias == EEnumEntryAliasStrText ) { strEnumeratorSource = "Low (5.12 MHz)"; }
                    else if( idxAlias == EEnumEntryAliasStrSCPIShort ) { strEnumeratorSource = "LOW"; }
                    else if( idxAlias == EEnumEntryAliasStrSCPILong ) { strEnumeratorSource = "LOW"; }
                    else { strEnumeratorSource = ""; }
                }
                else if( iEnumerator == static_cast<int>(EProcessorClock::Medium) )
                {
                    if( idxAlias == EEnumEntryAliasStrName ) { strEnumeratorSource = "Medium"; }
                    else if( idxAlias == EEnumEntryAliasStrSymbol ) { strEnumeratorSource = "M"; }
                    else if( idxAlias == EEnumEntryAliasStrText ) { strEnumeratorSource = "Medium (512.0 MHz)"; }
                    else if( idxAlias == EEnumEntryAliasStrSCPIShort ) { strEnumeratorSource = "MED"; }
                    else if( idxAlias == EEnumEntryAliasStrSCPILong ) { strEnumeratorSource = "MEDium"; }
                    else { strEnumeratorSource = ""; }
                }
                else if( iEnumerator == static_cast<int>(EProcessorClock::High) )
                {
                    if( idxAlias == EEnumEntryAliasStrName ) { strEnumeratorSource = "High"; }
                    else if( idxAlias == EEnumEntryAliasStrSymbol ) { strEnumeratorSource = "H"; }
                    else if( idxAlias == EEnumEntryAliasStrText ) { strEnumeratorSource = "High (5.12 GHz)"; }
                    else if( idxAlias == EEnumEntryAliasStrSCPIShort ) { strEnumeratorSource = "HIGH"; }
                    else if( idxAlias == EEnumEntryAliasStrSCPILong ) { strEnumeratorSource = "HIGH"; }
                    else { strEnumeratorSource = ""; }
                }
                else
                {
                    strEnumeratorSource = "Invalid";
                }

                strExpectedValue = strEnumeratorSource + ".toString(" + strAlias + "): " + strEnumeratorSource;
                strlstExpectedValues.append(strExpectedValue);

                strEnumeratorResult = enumInst.toString(idxAlias);
                strResultValue = strEnumeratorSource + ".toString(" + strAlias + "): " + strEnumeratorResult;
            }
            catch( CException& exc )
            {
                strResultValue = exc.getAddErrInfo();
            }
            strlstResultValues.append(strResultValue);
        }
    }

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateUserDefinedInstMethodToString

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedInstMethodToValue( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    QString  strEnumerator;
    QVariant val;

    int  iEnumerator;
    bool bOk;

    CEnumProcessorClock enumInst;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        try
        {
            enumInst = static_cast<EProcessorClock>(iEnumerator);

            if( iEnumerator == static_cast<int>(EProcessorClock::Low) ) { strEnumerator = "Low"; val = 5.12e6; bOk = true; }
            else if( iEnumerator == static_cast<int>(EProcessorClock::Medium) ) { strEnumerator = "Medium"; val = 5.12e8; bOk = true; }
            else if( iEnumerator == static_cast<int>(EProcessorClock::High) ) { strEnumerator = "High"; val = 5.12e9; bOk = true; }
            else { strEnumerator = "Invalid"; val = QVariant(); bOk = false; }
            strExpectedValue = strEnumerator + ".toValue(QVariant::Double, &bOk): " + val.toString() + ", " + bool2Str(bOk);
            strlstExpectedValues.append(strExpectedValue);

            val = enumInst.toValue(QVariant::Double, &bOk);
            strResultValue = strEnumerator + ".toValue(QVariant::Double, &bOk): " + val.toString() + ", " + bool2Str(bOk);
        }
        catch( CException& exc )
        {
            strResultValue = exc.getAddErrInfo();
        }
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        try
        {
            enumInst = static_cast<EProcessorClock>(iEnumerator);

            if( iEnumerator == static_cast<int>(EProcessorClock::Low) ) { strEnumerator = "Low"; val = 5.12e6; bOk = true; }
            else if( iEnumerator == static_cast<int>(EProcessorClock::Medium) ) { strEnumerator = "Medium"; val = 5.12e8; bOk = true; }
            else if( iEnumerator == static_cast<int>(EProcessorClock::High) ) { strEnumerator = "High"; val = 5.12e9; bOk = true; }
            else { strEnumerator = "Invalid"; val = QVariant(); bOk = false; }
            strExpectedValue = strEnumerator + ".toValue(QVariant::Invalid, &bOk): " + val.toString() + ", " + bool2Str(bOk);
            strlstExpectedValues.append(strExpectedValue);

            val = enumInst.toValue(QVariant::Invalid, &bOk);
            strResultValue = strEnumerator + ".toValue(QVariant::Invalid, &bOk): " + val.toString() + ", " + bool2Str(bOk);
        }
        catch( CException& exc )
        {
            strResultValue = exc.getAddErrInfo();
        }
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateUserDefinedInstMethodToValue

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedForLoop( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    QString strEnumerator;

    CEnumProcessorClock enumInst;

    int iEnumerator;

    // -------------------------------------------------------------------------

    for( iEnumerator = 0, enumInst = 0; enumInst < CEnumProcessorClock::count(); ++iEnumerator, ++enumInst )
    {
        if( iEnumerator == 0 ) { strEnumerator = "Low"; }
        else if( iEnumerator == 1 ) { strEnumerator = "Medium"; }
        else if( iEnumerator == 2 ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(iEnumerator); }

        strExpectedValue = strEnumerator + ".toString(): " + strEnumerator;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = strEnumerator + ".toString(): " + enumInst.toString();
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    for( iEnumerator = CEnumProcessorClock::count()-1, enumInst = CEnumProcessorClock::count()-1; enumInst >= 0; --iEnumerator, --enumInst )
    {
        if( iEnumerator == 0 ) { strEnumerator = "Low"; }
        else if( iEnumerator == 1 ) { strEnumerator = "Medium"; }
        else if( iEnumerator == 2 ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(iEnumerator); }

        strExpectedValue = strEnumerator + ".toString(): " + strEnumerator;
        strlstExpectedValues.append(strExpectedValue);

        strResultValue = strEnumerator + ".toString(): " + enumInst.toString();
        strlstResultValues.append(strResultValue);
    }

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepEnumClassTemplateUserDefinedForLoop

