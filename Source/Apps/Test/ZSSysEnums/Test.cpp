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

//#include <QtCore/qtimer.h>

#include "Test.h"

#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStepAdminObj.h"
#include "ZSTest/ZSTestStepAdminObjPool.h"
#include "ZSSys/ZSSysEnumTemplate.h"
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
CTest::CTest( const QString& i_strTestStepsFileName ) :
//------------------------------------------------------------------------------
    ZS::Test::CTest(
        /* strName              */ "ZS::System::Enums",
        /* strTestStepsFileName */ i_strTestStepsFileName,
        /* strNodeSeparator     */ "\\",
        /* iTestStepInterval_ms */ 0 )
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

    // Recall test admin object settings
    //----------------------------------

    m_pAdminObjPool->read_(i_strTestStepsFileName);

} // default ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    m_pAdminObjPool->save_();

} // dtor

/*==============================================================================
public: // test step methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsEnumerator2Str( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = "SEnumEntry::enumerator2Str(" + strEnumerator + "): " + strEnumerator;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "SEnumEntry::enumerator2Str(" + strEnumerator + "): ";
        strActualValue += SEnumEntry::enumerator2Str(pEnumArr, iEnumArrLen, static_cast<int>(enumerator));
        strlstActualValues.append(strActualValue);
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

            strDesiredValue = "SEnumEntry::enumerator2Str(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strDesiredValue += strResultExpected;
            strlstDesiredValues.append(strDesiredValue);

            strActualValue = "SEnumEntry::enumerator2Str(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strEnumerator = SEnumEntry::enumerator2Str(pEnumArr, iEnumArrLen, static_cast<int>(enumerator), idxAlias);
            strActualValue += strEnumerator;
            strlstActualValues.append(strActualValue);
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

        strDesiredValue  = "SEnumEntry::enumerator2Str(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
        strDesiredValue += "Enumerator " + strEnumerator + " does not have an alias string at index " + QString::number(idxAlias);
        strlstDesiredValues.append(strDesiredValue);

        try
        {
            strActualValue = "SEnumEntry::enumerator2Str(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strEnumerator = SEnumEntry::enumerator2Str(pEnumArr, iEnumArrLen, static_cast<int>(enumerator), idxAlias);
            strActualValue += strEnumerator;
        }
        catch( CException& exc )
        {
            strActualValue += exc.getAddErrInfo();
        }
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    strDesiredValue = "SEnumEntry::enumerator2Str(-1): Enumerator -1 not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        iEnumerator = -1;
        enumerator = static_cast<EProcessorClock>(iEnumerator);
        strActualValue = "SEnumEntry::enumerator2Str(" + QString::number(static_cast<int>(enumerator)) + "): ";
        strActualValue += SEnumEntry::enumerator2Str(pEnumArr, iEnumArrLen, static_cast<int>(enumerator));
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    strDesiredValue = "SEnumEntry::enumerator2Str(3): Enumerator 3 not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        iEnumerator = iEnumArrLen;
        enumerator = static_cast<EProcessorClock>(iEnumerator);
        strActualValue = "SEnumEntry::enumerator2Str(" + QString::number(static_cast<int>(enumerator)) + "): ";
        strActualValue += SEnumEntry::enumerator2Str(pEnumArr, iEnumArrLen, static_cast<int>(enumerator));
    }
    catch( CException& exc )
    {
        strActualValue = "SEnumEntry::enumerator2Str(" + QString::number(iEnumerator) + "): " + exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumEntryClassMethodsEnumerator2Str

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsEnumerator2Val( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = "SEnumEntry::enumerator2Val(" + strEnumerator + "): ";
        if( valExpected.canConvert(QVariant::Double) ) strDesiredValue += QString::number(valExpected.toDouble(),'g');
        strlstDesiredValues.append(strDesiredValue);

        valResult = SEnumEntry::enumerator2Val(pEnumArr, iEnumArrLen, static_cast<int>(enumerator));
        strActualValue = "SEnumEntry::enumerator2Val(" + strEnumerator + "): ";
        if( valResult.canConvert(QVariant::Double) ) strActualValue += QString::number(valResult.toDouble(),'g');
        strlstActualValues.append(strActualValue);
    }

    strDesiredValue = "SEnumEntry::enumerator2Val(-1): Enumerator -1 not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        iEnumerator = -1;
        enumerator = static_cast<EProcessorClock>(iEnumerator);
        strEnumerator = QString::number(static_cast<int>(enumerator));
        strActualValue = "SEnumEntry::enumerator2Val(" + strEnumerator + "): ";
        valResult = SEnumEntry::enumerator2Val(pEnumArr, iEnumArrLen, static_cast<int>(enumerator));
        if( valResult.canConvert(QVariant::Double) ) strActualValue += QString::number(valResult.toDouble(),'g');
    }
    catch( CException& exc )
    {
        strActualValue = "SEnumEntry::enumerator2Val(" + strEnumerator + "): " + exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    strDesiredValue = "SEnumEntry::enumerator2Val(3): Enumerator 3 not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        iEnumerator = iEnumArrLen;
        enumerator = static_cast<EProcessorClock>(iEnumerator);
        strEnumerator = QString::number(static_cast<int>(enumerator));
        strActualValue = "SEnumEntry::enumerator2Val(" + strEnumerator + "): ";
        valResult = SEnumEntry::enumerator2Val(pEnumArr, iEnumArrLen, static_cast<int>(enumerator));
        if( valResult.canConvert(QVariant::Double) ) strActualValue += QString::number(valResult.toDouble(),'g');
    }
    catch( CException& exc )
    {
        strActualValue = "SEnumEntry::enumerator2Val(" + strEnumerator + "): " + exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    {
        if( iEnumerator == 0 ) { enumerator = EProcessorClock::Low; strEnumerator = "Low"; strValResult = "5.12e+6"; }
        else if( iEnumerator == 1 ) { enumerator = EProcessorClock::Medium; strEnumerator = "Medium"; strValResult = "5.12e+8"; }
        else if( iEnumerator == 2 ) { enumerator = EProcessorClock::High; strEnumerator = "High"; strValResult = "5.12e+9"; }
        else { enumerator = static_cast<EProcessorClock>(iEnumerator); strEnumerator = QString::number(static_cast<int>(enumerator)); strValResult = ""; }

        strDesiredValue = "SEnumEntry::enumerator2Val(" + strEnumerator + ", Double, &bOk): " + strValResult + ", true";
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "SEnumEntry::enumerator2Val(" + strEnumerator + ", Double, &bOk): ";
        valResult = SEnumEntry::enumerator2Val(pEnumArr, iEnumArrLen, static_cast<int>(enumerator), QVariant::Double, &bOk);
        if( bOk ) strActualValue += QString::number(valResult.toDouble(),'g') + ", " + bool2Str(bOk);
        else strActualValue += valResult.toString() + ", " + bool2Str(bOk);
        strlstActualValues.append(strActualValue);
    }

    try
    {
        iEnumerator = -1;
        enumerator = static_cast<EProcessorClock>(iEnumerator);
        strEnumerator = QString::number(static_cast<int>(enumerator));

        strDesiredValue = "SEnumEntry::enumerator2Val(" + strEnumerator + ", Double, &bOk): ";
        strDesiredValue += "Enumerator " + strEnumerator + " not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "SEnumEntry::enumerator2Val(" + strEnumerator + ", Double, &bOk): ";
        valResult = SEnumEntry::enumerator2Val(pEnumArr, iEnumArrLen, static_cast<int>(enumerator), QVariant::Double, &bOk);
        if( bOk ) strActualValue += QString::number(valResult.toDouble(),'g') + ", " + bool2Str(bOk);
        else strActualValue += valResult.toString() + ", " + bool2Str(bOk);
    }
    catch( CException& exc )
    {
        strActualValue = "SEnumEntry::enumerator2Val(" + QString::number(iEnumerator) + ", Double, &bOk): " + exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        iEnumerator = iEnumArrLen;
        enumerator = static_cast<EProcessorClock>(iEnumerator);
        strEnumerator = QString::number(static_cast<int>(enumerator));

        strDesiredValue = "SEnumEntry::enumerator2Val(" + strEnumerator + ", Double, &bOk): ";
        strDesiredValue += "Enumerator " + strEnumerator + " not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "SEnumEntry::enumerator2Val(" + strEnumerator + ", Double, &bOk): ";
        valResult = SEnumEntry::enumerator2Val(pEnumArr, iEnumArrLen, static_cast<int>(enumerator), QVariant::Double, &bOk);
        if( bOk ) strActualValue += QString::number(valResult.toDouble(),'g') + ", " + bool2Str(bOk);
        else strActualValue += valResult.toString() + ", " + bool2Str(bOk);
    }
    catch( CException& exc )
    {
        strActualValue = "SEnumEntry::enumerator2Val(" + strEnumerator + ", Double, &bOk): " + exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumEntryClassMethodsEnumerator2Val

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsStr2Enumerator( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + "): ";
        strDesiredValue += strEnumeratorSource;
        strlstDesiredValues.append(strDesiredValue);

        enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource));

        if( enumerator == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumerator == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumerator == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumerator)); }

        strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + "): ";
        strActualValue += strEnumeratorResult;
        strlstActualValues.append(strActualValue);
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

            strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + "): ";
            strDesiredValue += strEnumeratorResult;
            strlstDesiredValues.append(strDesiredValue);

            strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + "): ";
            enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource, idxAlias));
            if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
            else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
            else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
            else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
            strActualValue += strEnumeratorResult;
            strlstActualValues.append(strActualValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias6;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(pEnumArr, iEnumArrLen, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumEntryClassMethodsStr2Enumerator

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsStr2Val( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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
        if( iEnumerator == 0 ) { strEnumeratorSource = "Low"; strValResult = "5.12e+6"; }
        else if( iEnumerator == 1 ) { strEnumeratorSource = "Medium"; strValResult = "5.12e+8"; }
        else if( iEnumerator == 2 ) { strEnumeratorSource = "High"; strValResult = "5.12e+9"; }
        else { strEnumeratorSource = ""; strValResult = ""; }

        strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + "): ";
        strDesiredValue += strValResult;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + "): ";
        valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource);
        if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'g');
        else strValResult = "";
        strActualValue += strValResult;
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )
    {
        strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));

        for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
        {
            if( iEnumerator == 0 )
            {
                strValResult = "5.12e+6";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Low";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "L";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Low (5.12 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "LOW";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "LOW";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 1 )
            {
                strValResult = "5.12e+8";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Medium";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "M";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Medium (512.0 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "MED";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "MEDium";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 2 )
            {
                strValResult = "5.12e+9";

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

            strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", Double, " + strAlias + "): ";
            strDesiredValue += strValResult;
            strlstDesiredValues.append(strDesiredValue);

            strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", Double, " + strAlias + "): ";
            valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Double, idxAlias);
            if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'g');
            else strValResult = "";
            strActualValue += strValResult;
            strlstActualValues.append(strActualValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "5.12e+8";

    strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strValResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'g');
    else strValResult = "";
    strActualValue += strValResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "";

    strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strValResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'g');
    else strValResult = "";
    strActualValue += strValResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "5.12e+8";

    strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strValResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'g');
    else strValResult = "";
    strActualValue += strValResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "512000000";

    strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Int, " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strValResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Int, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Int, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Int) ) strValResult = QString::number(valResult.toInt());
    else strValResult = "";
    strActualValue += strValResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "";

    strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strValResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'g');
    else strValResult = "";
    strActualValue += strValResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "5.12e+8";

    strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strValResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'g');
    else strValResult = "";
    strActualValue += strValResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias6;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "";

    strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strValResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'g');
    else strValResult = "";
    strActualValue += strValResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumEntryClassMethodsStr2Val

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsVal2Enumerator( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = "SEnumEntry::val2Enumerator(" + val.toString() + "): ";
        strDesiredValue += strEnumerator;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "SEnumEntry::val2Enumerator(" + val.toString() + "): ";
        enumerator = static_cast<EProcessorClock>(SEnumEntry::val2Enumerator(pEnumArr, iEnumArrLen, val));

        if( enumerator == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( enumerator == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( enumerator == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(enumerator)); }

        strActualValue += strEnumerator;
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    val = 5.12e8;

    strDesiredValue = "SEnumEntry::val2Enumerator(" + val.toString() + ", &bOk): ";
    strDesiredValue += "Medium, true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::val2Enumerator(" + val.toString() + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::val2Enumerator(pEnumArr, iEnumArrLen, val, &bOk));
    if( enumerator == EProcessorClock::Low ) { strEnumerator = "Low"; }
    else if( enumerator == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
    else if( enumerator == EProcessorClock::High ) { strEnumerator = "High"; }
    else { strEnumerator = QString::number(static_cast<int>(enumerator)); }
    strActualValue += strEnumerator + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    val = QVariant();

    strDesiredValue = "SEnumEntry::val2Enumerator(" + val.toString() + ", &bOk): ";
    strDesiredValue += "-1, false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::val2Enumerator(" + val.toString() + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::val2Enumerator(pEnumArr, iEnumArrLen, val, &bOk));
    if( enumerator == EProcessorClock::Low ) { strEnumerator = "Low"; }
    else if( enumerator == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
    else if( enumerator == EProcessorClock::High ) { strEnumerator = "High"; }
    else { strEnumerator = QString::number(static_cast<int>(enumerator)); }
    strActualValue += strEnumerator + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    val = 5.12e3;

    strDesiredValue = "SEnumEntry::val2Enumerator(" + val.toString() + ", &bOk): ";
    strDesiredValue += "-1, false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::val2Enumerator(" + val.toString() + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::val2Enumerator(pEnumArr, iEnumArrLen, val, &bOk));
    if( enumerator == EProcessorClock::Low ) { strEnumerator = "Low"; }
    else if( enumerator == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
    else if( enumerator == EProcessorClock::High ) { strEnumerator = "High"; }
    else { strEnumerator = QString::number(static_cast<int>(enumerator)); }
    strActualValue += strEnumerator + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumEntryClassMethodsVal2Enumerator

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsVal2Str( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = "SEnumEntry::val2Str(" + val.toString() + "): ";
        strDesiredValue += strValResult;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "SEnumEntry::val2Str(" + val.toString() + "): ";
        strValResult = SEnumEntry::val2Str(pEnumArr, iEnumArrLen, val);
        strActualValue += strValResult;
        strlstActualValues.append(strActualValue);
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

            strDesiredValue = "SEnumEntry::val2Str(" + val.toString() + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + ", &bOk): ";
            strDesiredValue += strValResult + ", " + bool2Str(bOk);
            strlstDesiredValues.append(strDesiredValue);

            strActualValue = "SEnumEntry::val2Str(" + val.toString() + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + ", &bOk): ";
            strValResult = SEnumEntry::val2Str(pEnumArr, iEnumArrLen, val, idxAlias, &bOk);
            strActualValue += strValResult + ", " + bool2Str(bOk);
            strlstActualValues.append(strActualValue);
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

        strDesiredValue = "SEnumEntry::val2Str(" + val.toString() + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + ", &bOk): ";
        if( iEnumerator <= 2 ) strDesiredValue += "Enumerator " + strEnumerator + " does not have an alias string at index " + QString::number(idxAlias);
        else strDesiredValue += ", false";
        strlstDesiredValues.append(strDesiredValue);

        try
        {
            strActualValue = "SEnumEntry::val2Str(" + val.toString() + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + ", &bOk): ";
            strValResult = SEnumEntry::val2Str(pEnumArr, iEnumArrLen, val, idxAlias, &bOk);
            strActualValue += strValResult + ", " + bool2Str(bOk);
        }
        catch( CException& exc )
        {
            strActualValue += exc.getAddErrInfo();
        }
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumEntryClassMethodsVal2Str

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsIsValidEnumerator( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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
        strDesiredValue = "SEnumEntry::isValidEnumerator(" + strEnumerator + "): " + bool2Str(bOk);
        strlstDesiredValues.append(strDesiredValue);

        bOk = SEnumEntry::isValidEnumerator(pEnumArr, iEnumArrLen, iEnumerator);
        strActualValue = "SEnumEntry::isValidEnumerator(" + strEnumerator + "): " + bool2Str(bOk);
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumerator = QString::number(iEnumerator);
    bOk = false;
    strDesiredValue = "SEnumEntry::isValidEnumerator(" + strEnumerator + "): " + bool2Str(bOk);
    strlstDesiredValues.append(strDesiredValue);

    bOk = SEnumEntry::isValidEnumerator(pEnumArr, iEnumArrLen, iEnumerator);
    strActualValue = "SEnumEntry::isValidEnumerator(" + strEnumerator + "): " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    iEnumerator = 5;
    strEnumerator = QString::number(iEnumerator);
    bOk = false;
    strDesiredValue = "SEnumEntry::isValidEnumerator(" + strEnumerator + "): " + bool2Str(bOk);
    strlstDesiredValues.append(strDesiredValue);

    bOk = SEnumEntry::isValidEnumerator(pEnumArr, iEnumArrLen, iEnumerator);
    strActualValue = "SEnumEntry::isValidEnumerator(" + strEnumerator + "): " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumEntryClassMethodsIsValidEnumerator

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsInitStr2EnumeratorMaps( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

    const SEnumEntry* pEnumArr = CEnumProcessorClock::s_arEnumEntries.data();
    const int         iEnumArrLen = CEnumProcessorClock::s_arEnumEntries.count();

    QString strEnumerator;

    int idxAlias;
    int iEnumerator;

    QVector<QHash<QString, int>> armapsStr2Enumerators;

    SEnumEntry::initStr2EnumeratorMaps(pEnumArr, iEnumArrLen, armapsStr2Enumerators, nullptr);

    strDesiredValue = "AliasCount = " + QString::number(EEnumEntryAliasStrSCPILong + 1);
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "AliasCount = " + QString::number(armapsStr2Enumerators.size());
    strlstActualValues.append(strActualValue);

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

            strDesiredValue = "armapsStr2Enumerators[" + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "][" + strEnumerator + "]: " + QString::number(iEnumerator);
            strlstDesiredValues.append(strDesiredValue);

            try
            {
                iEnumerator = armapsStr2Enumerators[idxAlias][strEnumerator];
                strActualValue = "armapsStr2Enumerators[" + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "][" + strEnumerator + "]: " + QString::number(iEnumerator);
            }
            catch(...)
            {
                strActualValue = "Unexpected exception thrown";
            }
            strlstActualValues.append(strActualValue);
        }
    }

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumEntryClassMethodsInitStr2EnumeratorMaps

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsMapStr2Enumerator( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + "): ";
        strDesiredValue += strEnumeratorResult;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + "): ";
        enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource));
        if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
        else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
        else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
        else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
        strActualValue += strEnumeratorResult;
        strlstActualValues.append(strActualValue);
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

            strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + "): ";
            strDesiredValue += strEnumeratorResult;
            strlstDesiredValues.append(strDesiredValue);

            strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + "): ";
            enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource, idxAlias));
            if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
            else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
            else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
            else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
            strActualValue += strEnumeratorResult;
            strlstActualValues.append(strActualValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias6;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strDesiredValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += "Map does not contain alias strings at index " + QString::number(idxAlias);
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "SEnumEntry::str2Enumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
        enumerator = static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(armapsStr2Enumerators, strEnumeratorSource, idxAlias, caseSensitivity, &bOk));
        if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
        else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
        else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
        else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
        strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumEntryClassMethodsMapStr2Enumerator

//------------------------------------------------------------------------------
void CTest::doTestStepEnumEntryClassMethodsMapStr2Val( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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
        if( iEnumerator == 0 ) { strEnumeratorSource = "Low"; strValResult = "5.12e+6"; }
        else if( iEnumerator == 1 ) { strEnumeratorSource = "Medium"; strValResult = "5.12e+8"; }
        else if( iEnumerator == 2 ) { strEnumeratorSource = "High"; strValResult = "5.12e+9"; }
        else { strEnumeratorSource = ""; strValResult = ""; }

        strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + "): ";
        strDesiredValue += strValResult;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + "): ";
        valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource);
        if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'g');
        else strValResult = "";
        strActualValue += strValResult;
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )
    {
        strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));

        for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
        {
            if( iEnumerator == 0 )
            {
                strValResult = "5.12e+6";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Low";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "L";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Low (5.12 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "LOW";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "LOW";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 1 )
            {
                strValResult = "5.12e+8";

                if( idxAlias == EEnumEntryAliasStrName ) strEnumeratorSource = "Medium";
                else if( idxAlias == EEnumEntryAliasStrSymbol ) strEnumeratorSource = "M";
                else if( idxAlias == EEnumEntryAliasStrText ) strEnumeratorSource = "Medium (512.0 MHz)";
                else if( idxAlias == EEnumEntryAliasStrSCPIShort ) strEnumeratorSource = "MED";
                else if( idxAlias == EEnumEntryAliasStrSCPILong ) strEnumeratorSource = "MEDium";
                else strEnumeratorSource = QString::number(iEnumerator);
            }
            else if( iEnumerator == 2 )
            {
                strValResult = "5.12e+9";

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

            strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", Double, " + strAlias + "): ";
            strDesiredValue += strValResult;
            strlstDesiredValues.append(strDesiredValue);

            strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", Double, " + strAlias + "): ";
            valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Double, idxAlias);
            if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'g');
            else strValResult = "";
            strActualValue += strValResult;
            strlstActualValues.append(strActualValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "5.12e+8";

    strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strValResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'g');
    else strValResult = "";
    strActualValue += strValResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "";

    strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strValResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'g');
    else strValResult = "";
    strActualValue += strValResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "5.12e+8";

    strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strValResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'g');
    else strValResult = "";
    strActualValue += strValResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "512000000";

    strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Int, " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strValResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Int, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Int, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Int) ) strValResult = QString::number(valResult.toInt());
    else strValResult = "";
    strActualValue += strValResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "";

    strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strValResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'g');
    else strValResult = "";
    strActualValue += strValResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "5.12e+8";

    strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strValResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'g');
    else strValResult = "";
    strActualValue += strValResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias6;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strValResult = "";

    strDesiredValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += "Map does not contain alias strings at index " + QString::number(idxAlias);
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "SEnumEntry::str2Val(" + strEnumeratorSource + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
        valResult = SEnumEntry::str2Val(armapsStr2Enumerators, pEnumArr, iEnumArrLen, strEnumeratorSource, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
        if( valResult.canConvert(QVariant::Double) ) strValResult = QString::number(valResult.toDouble(),'g');
        else strValResult = "";
        strActualValue += strValResult + ", " + bool2Str(bOk);
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumEntryClassMethodsMapStr2Val

/*==============================================================================
public: // test step methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeClassMethodToString( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = "CEnumMode::toString(" + strEnumerator + "): " + strEnumerator;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "CEnumMode::toString(" + strEnumerator + "): " + CEnumMode::toString(static_cast<EMode>(iEnumerator));
        strlstActualValues.append(strActualValue);
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

            strDesiredValue = "CEnumMode::toString(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strDesiredValue += strResultExpected;
            strlstDesiredValues.append(strDesiredValue);

            strActualValue = "CEnumMode::toString(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strEnumerator = CEnumMode::toString(enumerator, idxAlias);
            strActualValue += strEnumerator;
            strlstActualValues.append(strActualValue);
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

        strDesiredValue = "CEnumMode::toString(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
        strDesiredValue += "Enumerator " + strEnumerator + " does not have an alias string at index " + QString::number(idxAlias);
        strlstDesiredValues.append(strDesiredValue);

        try
        {
            strActualValue = "CEnumMode::toString(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strEnumerator = CEnumMode::toString(enumerator, idxAlias);
            strActualValue += strEnumerator;
        }
        catch( CException& exc )
        {
            strActualValue += exc.getAddErrInfo();
        }
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    strDesiredValue = "CEnumMode::toString(-1): Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        iEnumerator = -1;
        enumerator = static_cast<EMode>(iEnumerator);
        strActualValue = "CEnumMode::toString(" + QString::number(static_cast<int>(enumerator)) + "): ";
        strActualValue += CEnumMode::toString(enumerator);
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    strDesiredValue = "CEnumMode::toString(3): Enumerator 3 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        iEnumerator = CEnumMode::count();
        enumerator = static_cast<EMode>(iEnumerator);
        strActualValue = "CEnumMode::toString(" + QString::number(static_cast<int>(enumerator)) + "): ";
        strActualValue += CEnumMode::toString(enumerator);
    }
    catch( CException& exc )
    {
        strActualValue = "CEnumMode::toString(" + QString::number(iEnumerator) + "): " + exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateZSSysModeClassMethodToString

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeClassMethodToEnumerator( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + "): ";
        strDesiredValue += strEnumeratorSource;
        strlstDesiredValues.append(strDesiredValue);

        enumerator = CEnumMode::toEnumerator(strEnumeratorSource);

        if( enumerator == EMode::Edit ) { strEnumeratorResult = "Edit"; }
        else if( enumerator == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
        else if( enumerator == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumerator)); }

        strActualValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + "): ";
        strActualValue += strEnumeratorResult;
        strlstActualValues.append(strActualValue);
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

            strDesiredValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + "): ";
            strDesiredValue += strEnumeratorResult;
            strlstDesiredValues.append(strDesiredValue);

            enumerator = CEnumMode::toEnumerator(strEnumeratorSource, idxAlias);

            if( enumerator == EMode::Edit ) { strEnumeratorResult = "Edit"; }
            else if( enumerator == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
            else if( enumerator == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
            else { strEnumeratorResult = QString::number(static_cast<int>(enumerator)); }

            strActualValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + "): ";
            strActualValue += strEnumeratorResult;
            strlstActualValues.append(strActualValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrText; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Simulation";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Simulation";

    strDesiredValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    enumerator = CEnumMode::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumerator == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumerator == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "simULATION";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strDesiredValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    enumerator = CEnumMode::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumerator == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumerator == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "simULATION";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Simulation";

    strDesiredValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    enumerator = CEnumMode::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumerator == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumerator == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "simULATION";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strDesiredValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    enumerator = CEnumMode::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumerator == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumerator == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "simULATION";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Simulation";

    strDesiredValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    enumerator = CEnumMode::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumerator == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumerator == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "Simulation";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias8;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Map does not contain alias strings at index " + QString::number(idxAlias);

    strDesiredValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

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
    strActualValue = "CEnumMode::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateZSSysModeClassMethodToEnumerator

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeClassMethodFromString( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = "CEnumMode::fromString(" + strEnumeratorSource + "): ";
        strDesiredValue += strEnumeratorSource;
        strlstDesiredValues.append(strDesiredValue);

        enumInst = CEnumMode::fromString(strEnumeratorSource);

        if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
        else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
        else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }

        strActualValue = "CEnumMode::fromString(" + strEnumeratorSource + "): ";
        strActualValue += strEnumeratorResult;
        strlstActualValues.append(strActualValue);
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

            strDesiredValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + "): ";
            strDesiredValue += strEnumeratorResult;
            strlstDesiredValues.append(strDesiredValue);

            strActualValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + "): ";
            enumInst = CEnumMode::fromString(strEnumeratorSource, idxAlias);
            if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
            else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
            else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
            else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
            strActualValue += strEnumeratorResult;
            strlstActualValues.append(strActualValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrText; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Simulation";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Simulation";

    strDesiredValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumMode::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "simULATION";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strDesiredValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumMode::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "simULATION";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Simulation";

    strDesiredValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumMode::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "simULATION";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strDesiredValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumMode::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "simULATION";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Simulation";

    strDesiredValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumMode::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Simulation";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias8;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strDesiredValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += "Map does not contain alias strings at index " + QString::number(idxAlias);
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumMode::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
        enumInst = CEnumMode::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
        if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
        else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
        else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
        else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
        strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateZSSysModeClassMethodFromString

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeCtors( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

    CEnumMode* pEnumInst = nullptr;

    QString strEnumerator;
    QString strAlias;
    QString strCaseSensitivity;

    Qt::CaseSensitivity caseSensitivity;

    int iEnumerator;
    int idxAlias;

    // -------------------------------------------------------------------------

    strDesiredValue = "CEnumMode::ctor(): -1";
    strlstDesiredValues.append(strDesiredValue);

    pEnumInst = new CEnumMode();
    if( pEnumInst->enumerator() == EMode::Edit ) { strEnumerator = "Edit"; }
    else if( pEnumInst->enumerator() == EMode::Simulation ) { strEnumerator = "Simulation"; }
    else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
    else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
    strActualValue = "CEnumMode::ctor(): " + strEnumerator;
    strlstActualValues.append(strActualValue);
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

        strDesiredValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        strDesiredValue += strEnumerator;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumMode(static_cast<EMode>(iEnumerator));
        if( pEnumInst->enumerator() == EMode::Edit ) { strEnumerator = "Edit"; }
        else if( pEnumInst->enumerator() == EMode::Simulation ) { strEnumerator = "Simulation"; }
        else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strActualValue += strEnumerator;
        strlstActualValues.append(strActualValue);
        delete pEnumInst;
        pEnumInst = nullptr;
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumerator = QString::number(iEnumerator);

    strDesiredValue = "CEnumMode::ctor(" + strEnumerator + "): ";
    strDesiredValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumMode(static_cast<EMode>(iEnumerator));
        if( pEnumInst->enumerator() == EMode::Edit ) { strEnumerator = "Edit"; }
        else if( pEnumInst->enumerator() == EMode::Simulation ) { strEnumerator = "Simulation"; }
        else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strActualValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    iEnumerator = CEnumMode::count();
    strEnumerator = QString::number(iEnumerator);

    strDesiredValue = "CEnumMode::ctor(" + strEnumerator + "): ";
    strDesiredValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumMode(static_cast<EMode>(iEnumerator));
        if( pEnumInst->enumerator() == EMode::Edit ) { strEnumerator = "Edit"; }
        else if( pEnumInst->enumerator() == EMode::Simulation ) { strEnumerator = "Simulation"; }
        else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strActualValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);
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

        strDesiredValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        strDesiredValue += strEnumerator;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumMode(iEnumerator);
        if( pEnumInst->enumerator() == EMode::Edit ) { strEnumerator = "Edit"; }
        else if( pEnumInst->enumerator() == EMode::Simulation ) { strEnumerator = "Simulation"; }
        else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strActualValue += strEnumerator;
        strlstActualValues.append(strActualValue);
        delete pEnumInst;
        pEnumInst = nullptr;
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumerator = QString::number(iEnumerator);

    strDesiredValue = "CEnumMode::ctor(" + strEnumerator + "): ";
    strDesiredValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumMode(iEnumerator);
        if( pEnumInst->enumerator() == EMode::Edit ) { strEnumerator = "Edit"; }
        else if( pEnumInst->enumerator() == EMode::Simulation ) { strEnumerator = "Simulation"; }
        else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strActualValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    iEnumerator = CEnumMode::count();
    strEnumerator = QString::number(iEnumerator);

    strDesiredValue = "CEnumMode::ctor(" + strEnumerator + "): ";
    strDesiredValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumMode(iEnumerator);
        if( pEnumInst->enumerator() == EMode::Edit ) { strEnumerator = "Edit"; }
        else if( pEnumInst->enumerator() == EMode::Simulation ) { strEnumerator = "Simulation"; }
        else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strActualValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);
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

        strDesiredValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        strDesiredValue += strEnumerator;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "CEnumMode::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumMode(strEnumerator);
        if( pEnumInst->enumerator() == EMode::Edit ) { strEnumerator = "Edit"; }
        else if( pEnumInst->enumerator() == EMode::Simulation ) { strEnumerator = "Simulation"; }
        else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strActualValue += strEnumerator;
        strlstActualValues.append(strActualValue);
        delete pEnumInst;
        pEnumInst = nullptr;
    }

    // -------------------------------------------------------------------------

    strEnumerator = "Simulation";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);

    strDesiredValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strDesiredValue += strEnumerator;
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumMode(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EMode::Edit ) strEnumerator = "Edit";
    else if( pEnumInst->enumerator() == EMode::Simulation ) strEnumerator = "Simulation";
    else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strActualValue += strEnumerator;
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "-1";

    strDesiredValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strDesiredValue += strEnumerator;
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumMode(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EMode::Edit ) strEnumerator = "Edit";
    else if( pEnumInst->enumerator() == EMode::Simulation ) strEnumerator = "Simulation";
    else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strActualValue += strEnumerator;
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "Simulation";

    strDesiredValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strDesiredValue += strEnumerator;
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumMode(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EMode::Edit ) strEnumerator = "Edit";
    else if( pEnumInst->enumerator() == EMode::Simulation ) strEnumerator = "Simulation";
    else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strActualValue += strEnumerator;
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "Simulation";

    strDesiredValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strDesiredValue += strEnumerator;
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumMode(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EMode::Edit ) strEnumerator = "Edit";
    else if( pEnumInst->enumerator() == EMode::Simulation ) strEnumerator = "Simulation";
    else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strActualValue += strEnumerator;
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "-1";

    strDesiredValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strDesiredValue += strEnumerator;
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumMode(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EMode::Edit ) strEnumerator = "Edit";
    else if( pEnumInst->enumerator() == EMode::Simulation ) strEnumerator = "Simulation";
    else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strActualValue += strEnumerator;
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "Simulation";

    strDesiredValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strDesiredValue += strEnumerator;
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumMode(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EMode::Edit ) strEnumerator = "Edit";
    else if( pEnumInst->enumerator() == EMode::Simulation ) strEnumerator = "Simulation";
    else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strActualValue += strEnumerator;
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "Simulation";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias8;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "-1";

    strDesiredValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strDesiredValue += "Map does not contain alias strings at index " + QString::number(idxAlias);
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumMode::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
        pEnumInst = new CEnumMode(strEnumerator, idxAlias, caseSensitivity);
        if( pEnumInst->enumerator() == EMode::Edit ) strEnumerator = "Edit";
        else if( pEnumInst->enumerator() == EMode::Simulation ) strEnumerator = "Simulation";
        else if( pEnumInst->enumerator() == EMode::Undefined ) { strEnumerator = "Undefined"; }
        else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
        strActualValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateZSSysModeCtors

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeOperatorAssign( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = "CEnumMode = " + strEnumeratorSource + ": ";
        strDesiredValue += strEnumeratorSource;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "CEnumMode = " + strEnumeratorSource + ": ";
        enumInst = static_cast<EMode>(iEnumerator);
        if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
        else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
        else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strActualValue += strEnumeratorResult;
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumeratorSource = QString::number(iEnumerator);

    strDesiredValue = "CEnumMode = " + strEnumeratorSource + ": ";
    strDesiredValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumMode = " + strEnumeratorSource + ": ";
        enumInst = static_cast<EMode>(iEnumerator);
        if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
        else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
        else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strActualValue += strEnumeratorResult;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    iEnumerator = CEnumMode::count();
    strEnumeratorSource = QString::number(iEnumerator);

    strDesiredValue = "CEnumMode = " + strEnumeratorSource + ": ";
    strDesiredValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumMode = " + strEnumeratorSource + ": ";
        enumInst = static_cast<EMode>(iEnumerator);
        if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
        else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
        else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strActualValue += strEnumeratorResult;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumMode::count(); ++iEnumerator )
    {
        strEnumeratorSource = "-1";

        if( iEnumerator == 0 ) { strEnumeratorSource = "Edit"; }
        else if( iEnumerator == 1 ) { strEnumeratorSource = "Simulation"; }
        else if( iEnumerator == 2 ) { strEnumeratorSource = "Undefined"; }
        else { strEnumeratorSource = QString::number(iEnumerator); }

        strDesiredValue = "CEnumMode = " + strEnumeratorSource + ": ";
        strDesiredValue += strEnumeratorSource;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "CEnumMode = " + strEnumeratorSource + ": ";
        enumInst = iEnumerator;
        if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
        else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
        else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strActualValue += strEnumeratorResult;
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumeratorSource = QString::number(iEnumerator);

    strDesiredValue = "CEnumMode = " + strEnumeratorSource + ": ";
    strDesiredValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumMode = " + strEnumeratorSource + ": ";
        enumInst = iEnumerator;
        if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
        else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
        else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strActualValue += strEnumeratorResult;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    iEnumerator = CEnumMode::count();
    strEnumeratorSource = QString::number(iEnumerator);

    strDesiredValue = "CEnumMode = " + strEnumeratorSource + ": ";
    strDesiredValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumMode = " + strEnumeratorSource + ": ";
        enumInst = iEnumerator;
        if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
        else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
        else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strActualValue += strEnumeratorResult;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

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

            strDesiredValue = "CEnumMode = " + strEnumeratorSource + ": ";
            strDesiredValue += strEnumeratorResult;
            strlstDesiredValues.append(strDesiredValue);

            strActualValue = "CEnumMode = " + strEnumeratorSource + ": ";
            enumInst = strEnumeratorSource;
            if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
            else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
            else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
            else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
            strActualValue += strEnumeratorResult;
            strlstActualValues.append(strActualValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrText; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "simULATION";

    strDesiredValue = "CEnumMode = " + strEnumeratorSource + ": -1";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumMode = " + strEnumeratorSource + ": ";
    enumInst = strEnumeratorSource;
    if( enumInst.enumerator() == EMode::Edit ) strEnumeratorResult = "Edit";
    else if( enumInst.enumerator() == EMode::Simulation ) strEnumeratorResult = "Simulation";
    else if( enumInst.enumerator() == EMode::Undefined ) strEnumeratorResult = "Undefined";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strActualValue += strEnumeratorResult;
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateZSSysModeOperatorAssign

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeOperatorCompareWithEnumerator( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

    EMode enumerator;

    CEnumMode enumInst;

    // -------------------------------------------------------------------------

    enumerator = EMode::Simulation;
    enumInst = EMode::Simulation;

    try
    {
        strDesiredValue = "Simulation == Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == enumerator )
        {
            strActualValue = "Simulation == Simulation";
        }
        else
        {
            strActualValue = "Simulation != Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation == Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != enumerator )
        {
            strActualValue = "Simulation != Simulation";
        }
        else
        {
            strActualValue = "Simulation == Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Simulation >= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < enumerator )
        {
            strActualValue = "Simulation < Simulation";
        }
        else
        {
            strActualValue = "Simulation >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > enumerator )
        {
            strActualValue = "Simulation > Simulation";
        }
        else
        {
            strActualValue = "Simulation <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Simulation <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= enumerator )
        {
            strActualValue = "Simulation <= Simulation";
        }
        else
        {
            strActualValue = "Simulation > Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation >= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= enumerator )
        {
            strActualValue = "Simulation >= Simulation";
        }
        else
        {
            strActualValue = "Simulation < Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = EMode::Edit;
    enumerator = EMode::Simulation;

    try
    {
        strDesiredValue = "Edit != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == enumerator )
        {
            strActualValue = "Edit == Simulation";
        }
        else
        {
            strActualValue = "Edit != Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Edit != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != enumerator )
        {
            strActualValue = "Edit != Simulation";
        }
        else
        {
            strActualValue = "Edit == Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Edit < Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < enumerator )
        {
            strActualValue = "Edit < Simulation";
        }
        else
        {
            strActualValue = "Edit >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Edit <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > enumerator )
        {
            strActualValue = "Edit > Simulation";
        }
        else
        {
            strActualValue = "Edit <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Edit <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= enumerator )
        {
            strActualValue = "Edit <= Simulation";
        }
        else
        {
            strActualValue = "Edit > Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Edit < Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= enumerator )
        {
            strActualValue = "Edit >= Simulation";
        }
        else
        {
            strActualValue = "Edit < Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumMode(); // InvalidEnumerator
    enumerator = EMode::Simulation;

    try
    {
        strDesiredValue = "Invalid != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == enumerator )
        {
            strActualValue = "Invalid == Simulation";
        }
        else
        {
            strActualValue = "Invalid != Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != enumerator )
        {
            strActualValue = "Invalid != Simulation";
        }
        else
        {
            strActualValue = "Invalid == Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < enumerator )
        {
            strActualValue = "Invalid < Simulation";
        }
        else
        {
            strActualValue = "Invalid >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > enumerator )
        {
            strActualValue = "Invalid > Simulation";
        }
        else
        {
            strActualValue = "Invalid <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= enumerator )
        {
            strActualValue = "Invalid <= Simulation";
        }
        else
        {
            strActualValue = "Invalid > Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= enumerator )
        {
            strActualValue = "Invalid >= Simulation";
        }
        else
        {
            strActualValue = "Invalid < Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = EMode::Simulation;
    enumerator = static_cast<EMode>(EInvalidEnumerator);

    try
    {
        strDesiredValue = "Simulation != Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == enumerator )
        {
            strActualValue = "Simulation == Invalid";
        }
        else
        {
            strActualValue = "Simulation != Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation != Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != enumerator )
        {
            strActualValue = "Simulation != Invalid";
        }
        else
        {
            strActualValue = "Simulation == Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < enumerator )
        {
            strActualValue = "Simulation < Invalid";
        }
        else
        {
            strActualValue = "Simulation >= Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > enumerator )
        {
            strActualValue = "Simulation > Invalid";
        }
        else
        {
            strActualValue = "Simulation <= Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= enumerator )
        {
            strActualValue = "Simulation <= Invalid";
        }
        else
        {
            strActualValue = "Simulation > Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= enumerator )
        {
            strActualValue = "Simulation >= Invalid";
        }
        else
        {
            strActualValue = "Simulation < Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumMode(); // InvalidEnumerator
    enumerator = static_cast<EMode>(EInvalidEnumerator);

    try
    {
        strDesiredValue = "Invalid == Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == enumerator )
        {
            strActualValue = "Invalid == Invalid";
        }
        else
        {
            strActualValue = "Invalid != Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid == Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != enumerator )
        {
            strActualValue = "Invalid != Invalid";
        }
        else
        {
            strActualValue = "Invalid == Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < enumerator )
        {
            strActualValue = "Invalid < Invalid";
        }
        else
        {
            strActualValue = "Invalid >= Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > enumerator )
        {
            strActualValue = "Invalid > Invalid";
        }
        else
        {
            strActualValue = "Invalid <= Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= enumerator )
        {
            strActualValue = "Invalid <= Invalid";
        }
        else
        {
            strActualValue = "Invalid > Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= enumerator )
        {
            strActualValue = "Invalid >= Invalid";
        }
        else
        {
            strActualValue = "Invalid < Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateZSSysModeOperatorCompareWithEnumerator

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeOperatorCompareWithInt( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

    CEnumMode enumInst;

    int iEnumerator;

    // -------------------------------------------------------------------------

    enumInst = EMode::Simulation;
    iEnumerator = static_cast<int>(EMode::Simulation);

    try
    {
        strDesiredValue = "Simulation == Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == iEnumerator )
        {
            strActualValue = "Simulation == Simulation";
        }
        else
        {
            strActualValue = "Simulation != Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation == Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != iEnumerator )
        {
            strActualValue = "Simulation != Simulation";
        }
        else
        {
            strActualValue = "Simulation == Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Simulation >= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < iEnumerator )
        {
            strActualValue = "Simulation < Simulation";
        }
        else
        {
            strActualValue = "Simulation >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > iEnumerator )
        {
            strActualValue = "Simulation > Simulation";
        }
        else
        {
            strActualValue = "Simulation <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Simulation <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= iEnumerator )
        {
            strActualValue = "Simulation <= Simulation";
        }
        else
        {
            strActualValue = "Simulation > Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation >= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= iEnumerator )
        {
            strActualValue = "Simulation >= Simulation";
        }
        else
        {
            strActualValue = "Simulation < Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = EMode::Edit;
    iEnumerator = static_cast<int>(EMode::Simulation);

    try
    {
        strDesiredValue = "Edit != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == iEnumerator )
        {
            strActualValue = "Edit == Simulation";
        }
        else
        {
            strActualValue = "Edit != Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Edit != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != iEnumerator )
        {
            strActualValue = "Edit != Simulation";
        }
        else
        {
            strActualValue = "Edit == Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Edit < Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < iEnumerator )
        {
            strActualValue = "Edit < Simulation";
        }
        else
        {
            strActualValue = "Edit >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Edit <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > iEnumerator )
        {
            strActualValue = "Edit > Simulation";
        }
        else
        {
            strActualValue = "Edit <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Edit <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= iEnumerator )
        {
            strActualValue = "Edit <= Simulation";
        }
        else
        {
            strActualValue = "Edit > Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Edit < Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= iEnumerator )
        {
            strActualValue = "Edit >= Simulation";
        }
        else
        {
            strActualValue = "Edit < Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumMode(); // InvalidEnumerator
    iEnumerator = static_cast<int>(EMode::Simulation);

    try
    {
        strDesiredValue = "Invalid != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == iEnumerator )
        {
            strActualValue = "Invalid == Simulation";
        }
        else
        {
            strActualValue = "Invalid != Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != iEnumerator )
        {
            strActualValue = "Invalid != Simulation";
        }
        else
        {
            strActualValue = "Invalid == Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid < Simulation: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < iEnumerator )
        {
            strActualValue = "Invalid < Simulation";
        }
        else
        {
            strActualValue = "Invalid < Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid > Simulation: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > iEnumerator )
        {
            strActualValue = "Invalid > Simulation";
        }
        else
        {
            strActualValue = "Invalid > Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid <= Simulation: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= iEnumerator )
        {
            strActualValue = "Invalid <= Simulation";
        }
        else
        {
            strActualValue = "Invalid <= Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid >= Simulation: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= iEnumerator )
        {
            strActualValue = "Invalid >= Simulation";
        }
        else
        {
            strActualValue = "Invalid >= Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = EMode::Simulation;
    iEnumerator = EInvalidEnumerator;

    try
    {
        strDesiredValue = "Simulation != Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == iEnumerator )
        {
            strActualValue = "Simulation == Invalid";
        }
        else
        {
            strActualValue = "Simulation != Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation != Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != iEnumerator )
        {
            strActualValue = "Simulation != Invalid";
        }
        else
        {
            strActualValue = "Simulation == Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Simulation < Invalid: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < iEnumerator )
        {
            strActualValue = "Simulation < Invalid";
        }
        else
        {
            strActualValue = "Simulation < Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation > Invalid: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > iEnumerator )
        {
            strActualValue = "Simulation > Invalid";
        }
        else
        {
            strActualValue = "Simulation > Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Simulation <= Invalid: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= iEnumerator )
        {
            strActualValue = "Simulation <= Invalid";
        }
        else
        {
            strActualValue = "Simulation <= Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation >= Invalid: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= iEnumerator )
        {
            strActualValue = "Simulation >= Invalid";
        }
        else
        {
            strActualValue = "Simulation >= Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumMode(); // InvalidEnumerator
    iEnumerator = EInvalidEnumerator;

    try
    {
        strDesiredValue = "Invalid == Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == iEnumerator )
        {
            strActualValue = "Invalid == Invalid";
        }
        else
        {
            strActualValue = "Invalid != Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid == Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != iEnumerator )
        {
            strActualValue = "Invalid != Invalid";
        }
        else
        {
            strActualValue = "Invalid == Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid < Invalid: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < iEnumerator )
        {
            strActualValue = "Invalid < Invalid";
        }
        else
        {
            strActualValue = "Invalid < Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid > Invalid: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > iEnumerator )
        {
            strActualValue = "Invalid > Invalid";
        }
        else
        {
            strActualValue = "Invalid > Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid <= Invalid: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= iEnumerator )
        {
            strActualValue = "Invalid <= Invalid";
        }
        else
        {
            strActualValue = "Invalid <= Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid >= Invalid: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= iEnumerator )
        {
            strActualValue = "Invalid >= Invalid";
        }
        else
        {
            strActualValue = "Invalid >= Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateZSSysModeOperatorCompareWithInt

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeOperatorCompareWithCharPtr( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

    CEnumMode enumInst;

    // -------------------------------------------------------------------------

    enumInst = EMode::Simulation;
    const char* szEnumerator = "Simulation";

    try
    {
        strDesiredValue = "Simulation == Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == szEnumerator )
        {
            strActualValue = "Simulation == Simulation";
        }
        else
        {
            strActualValue = "Simulation != Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation == Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != szEnumerator )
        {
            strActualValue = "Simulation != Simulation";
        }
        else
        {
            strActualValue = "Simulation == Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Simulation >= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < szEnumerator )
        {
            strActualValue = "Simulation < Simulation";
        }
        else
        {
            strActualValue = "Simulation >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > szEnumerator )
        {
            strActualValue = "Simulation > Simulation";
        }
        else
        {
            strActualValue = "Simulation <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Simulation <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= szEnumerator )
        {
            strActualValue = "Simulation <= Simulation";
        }
        else
        {
            strActualValue = "Simulation > Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation >= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= szEnumerator )
        {
            strActualValue = "Simulation >= Simulation";
        }
        else
        {
            strActualValue = "Simulation < Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = EMode::Edit;
    szEnumerator = "Simulation";

    try
    {
        strDesiredValue = "Edit != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == szEnumerator )
        {
            strActualValue = "Edit == Simulation";
        }
        else
        {
            strActualValue = "Edit != Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Edit != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != szEnumerator )
        {
            strActualValue = "Edit != Simulation";
        }
        else
        {
            strActualValue = "Edit == Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Edit < Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < szEnumerator )
        {
            strActualValue = "Edit < Simulation";
        }
        else
        {
            strActualValue = "Edit >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Edit <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > szEnumerator )
        {
            strActualValue = "Edit > Simulation";
        }
        else
        {
            strActualValue = "Edit <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Edit <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= szEnumerator )
        {
            strActualValue = "Edit <= Simulation";
        }
        else
        {
            strActualValue = "Edit > Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Edit < Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= szEnumerator )
        {
            strActualValue = "Edit >= Simulation";
        }
        else
        {
            strActualValue = "Edit < Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumMode(); // InvalidEnumerator
    szEnumerator = "Simulation";

    try
    {
        strDesiredValue = "Invalid != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == szEnumerator )
        {
            strActualValue = "Invalid == Simulation";
        }
        else
        {
            strActualValue = "Invalid != Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != szEnumerator )
        {
            strActualValue = "Invalid != Simulation";
        }
        else
        {
            strActualValue = "Invalid == Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid < Simulation: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < szEnumerator )
        {
            strActualValue = "Invalid < Simulation";
        }
        else
        {
            strActualValue = "Invalid < Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid > Simulation: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > szEnumerator )
        {
            strActualValue = "Invalid > Simulation";
        }
        else
        {
            strActualValue = "Invalid > Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid <= Simulation: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= szEnumerator )
        {
            strActualValue = "Invalid <= Simulation";
        }
        else
        {
            strActualValue = "Invalid <= Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid >= Simulation: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= szEnumerator )
        {
            strActualValue = "Invalid >= Simulation";
        }
        else
        {
            strActualValue = "Invalid >= Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = EMode::Simulation;
    szEnumerator = "Unknown";

    try
    {
        strDesiredValue = "Simulation != Unknown";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == szEnumerator )
        {
            strActualValue = "Simulation == Unknown";
        }
        else
        {
            strActualValue = "Simulation != Unknown";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation != Unknown";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != szEnumerator )
        {
            strActualValue = "Simulation != Unknown";
        }
        else
        {
            strActualValue = "Simulation == Unknown";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Simulation < Unknown: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < szEnumerator )
        {
            strActualValue = "Simulation < Unknown";
        }
        else
        {
            strActualValue = "Simulation < Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation > Unknown: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > szEnumerator )
        {
            strActualValue = "Simulation > Unknown";
        }
        else
        {
            strActualValue = "Simulation > Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Simulation <= Unknown: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= szEnumerator )
        {
            strActualValue = "Simulation <= Unknown";
        }
        else
        {
            strActualValue = "Simulation <= Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation >= Unknown: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= szEnumerator )
        {
            strActualValue = "Simulation >= Unknown";
        }
        else
        {
            strActualValue = "Simulation >= Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumMode(); // InvalidEnumerator
    szEnumerator = "Unknown";

    try
    {
        strDesiredValue = "Invalid != Unknown";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == szEnumerator )
        {
            strActualValue = "Invalid == Unknown";
        }
        else
        {
            strActualValue = "Invalid != Unknown";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid != Unknown";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != szEnumerator )
        {
            strActualValue = "Invalid != Unknown";
        }
        else
        {
            strActualValue = "Invalid == Unknown";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid < Unknown: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < szEnumerator )
        {
            strActualValue = "Invalid < Unknown";
        }
        else
        {
            strActualValue = "Invalid < Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid > Unknown: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > szEnumerator )
        {
            strActualValue = "Invalid > Unknown";
        }
        else
        {
            strActualValue = "Invalid > Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid <= Unknown: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= szEnumerator )
        {
            strActualValue = "Invalid <= Unknown";
        }
        else
        {
            strActualValue = "Invalid <= Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid >= Unknown: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= szEnumerator )
        {
            strActualValue = "Invalid >= Unknown";
        }
        else
        {
            strActualValue = "Invalid >= Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateZSSysModeOperatorCompareWithCharPtr

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeOperatorCompareWithQString( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

    CEnumMode enumInst;

    QString strEnumerator;

    // -------------------------------------------------------------------------

    enumInst = EMode::Simulation;
    strEnumerator = "Simulation";

    try
    {
        strDesiredValue = "Simulation == Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == strEnumerator )
        {
            strActualValue = "Simulation == Simulation";
        }
        else
        {
            strActualValue = "Simulation != Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation == Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != strEnumerator )
        {
            strActualValue = "Simulation != Simulation";
        }
        else
        {
            strActualValue = "Simulation == Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Simulation >= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < strEnumerator )
        {
            strActualValue = "Simulation < Simulation";
        }
        else
        {
            strActualValue = "Simulation >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > strEnumerator )
        {
            strActualValue = "Simulation > Simulation";
        }
        else
        {
            strActualValue = "Simulation <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Simulation <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= strEnumerator )
        {
            strActualValue = "Simulation <= Simulation";
        }
        else
        {
            strActualValue = "Simulation > Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation >= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= strEnumerator )
        {
            strActualValue = "Simulation >= Simulation";
        }
        else
        {
            strActualValue = "Simulation < Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = EMode::Edit;
    strEnumerator = "Simulation";

    try
    {
        strDesiredValue = "Edit != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == strEnumerator )
        {
            strActualValue = "Edit == Simulation";
        }
        else
        {
            strActualValue = "Edit != Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Edit != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != strEnumerator )
        {
            strActualValue = "Edit != Simulation";
        }
        else
        {
            strActualValue = "Edit == Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Edit < Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < strEnumerator )
        {
            strActualValue = "Edit < Simulation";
        }
        else
        {
            strActualValue = "Edit >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Edit <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > strEnumerator )
        {
            strActualValue = "Edit > Simulation";
        }
        else
        {
            strActualValue = "Edit <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Edit <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= strEnumerator )
        {
            strActualValue = "Edit <= Simulation";
        }
        else
        {
            strActualValue = "Edit > Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Edit < Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= strEnumerator )
        {
            strActualValue = "Edit >= Simulation";
        }
        else
        {
            strActualValue = "Edit < Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumMode(); // InvalidEnumerator
    strEnumerator = "Simulation";

    try
    {
        strDesiredValue = "Invalid != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == strEnumerator )
        {
            strActualValue = "Invalid == Simulation";
        }
        else
        {
            strActualValue = "Invalid != Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != strEnumerator )
        {
            strActualValue = "Invalid != Simulation";
        }
        else
        {
            strActualValue = "Invalid == Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid < Simulation: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < strEnumerator )
        {
            strActualValue = "Invalid < Simulation";
        }
        else
        {
            strActualValue = "Invalid < Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid > Simulation: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > strEnumerator )
        {
            strActualValue = "Invalid > Simulation";
        }
        else
        {
            strActualValue = "Invalid > Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid <= Simulation: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= strEnumerator )
        {
            strActualValue = "Invalid <= Simulation";
        }
        else
        {
            strActualValue = "Invalid <= Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid >= Simulation: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= strEnumerator )
        {
            strActualValue = "Invalid >= Simulation";
        }
        else
        {
            strActualValue = "Invalid >= Simulation: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = EMode::Simulation;
    strEnumerator = "Unknown";

    try
    {
        strDesiredValue = "Simulation != Unknown";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == strEnumerator )
        {
            strActualValue = "Simulation == Unknown";
        }
        else
        {
            strActualValue = "Simulation != Unknown";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation != Unknown";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != strEnumerator )
        {
            strActualValue = "Simulation != Unknown";
        }
        else
        {
            strActualValue = "Simulation == Unknown";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Simulation < Unknown: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < strEnumerator )
        {
            strActualValue = "Simulation < Unknown";
        }
        else
        {
            strActualValue = "Simulation < Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation > Unknown: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > strEnumerator )
        {
            strActualValue = "Simulation > Unknown";
        }
        else
        {
            strActualValue = "Simulation > Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Simulation <= Unknown: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= strEnumerator )
        {
            strActualValue = "Simulation <= Unknown";
        }
        else
        {
            strActualValue = "Simulation <= Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation >= Unknown: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= strEnumerator )
        {
            strActualValue = "Simulation >= Unknown";
        }
        else
        {
            strActualValue = "Simulation >= Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumMode(); // InvalidEnumerator
    strEnumerator = "Unknown";

    try
    {
        strDesiredValue = "Invalid != Unknown";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == strEnumerator )
        {
            strActualValue = "Invalid == Unknown";
        }
        else
        {
            strActualValue = "Invalid != Unknown";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid != Unknown";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != strEnumerator )
        {
            strActualValue = "Invalid != Unknown";
        }
        else
        {
            strActualValue = "Invalid == Unknown";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid < Unknown: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < strEnumerator )
        {
            strActualValue = "Invalid < Unknown";
        }
        else
        {
            strActualValue = "Invalid < Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid > Unknown: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > strEnumerator )
        {
            strActualValue = "Invalid > Unknown";
        }
        else
        {
            strActualValue = "Invalid > Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid <= Unknown: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= strEnumerator )
        {
            strActualValue = "Invalid <= Unknown";
        }
        else
        {
            strActualValue = "Invalid <= Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid >= Unknown: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= strEnumerator )
        {
            strActualValue = "Invalid >= Unknown";
        }
        else
        {
            strActualValue = "Invalid >= Unknown: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateZSSysModeOperatorCompareWithQString

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeOperatorCompareWithEnumTemplate( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

    CEnumMode enumInst1;
    CEnumMode enumInst2;

    // -------------------------------------------------------------------------

    enumInst1 = EMode::Simulation;
    enumInst2 = EMode::Simulation;

    try
    {
        strDesiredValue = "Simulation == Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 == enumInst2 )
        {
            strActualValue = "Simulation == Simulation";
        }
        else
        {
            strActualValue = "Simulation != Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation == Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 != enumInst2 )
        {
            strActualValue = "Simulation != Simulation";
        }
        else
        {
            strActualValue = "Simulation == Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Simulation >= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 < enumInst2 )
        {
            strActualValue = "Simulation < Simulation";
        }
        else
        {
            strActualValue = "Simulation >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 > enumInst2 )
        {
            strActualValue = "Simulation > Simulation";
        }
        else
        {
            strActualValue = "Simulation <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Simulation <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 <= enumInst2 )
        {
            strActualValue = "Simulation <= Simulation";
        }
        else
        {
            strActualValue = "Simulation > Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation >= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 >= enumInst2 )
        {
            strActualValue = "Simulation >= Simulation";
        }
        else
        {
            strActualValue = "Simulation < Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst1 = EMode::Edit;
    enumInst2 = EMode::Simulation;

    try
    {
        strDesiredValue = "Edit != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 == enumInst2 )
        {
            strActualValue = "Edit == Simulation";
        }
        else
        {
            strActualValue = "Edit != Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Edit != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 != enumInst2 )
        {
            strActualValue = "Edit != Simulation";
        }
        else
        {
            strActualValue = "Edit == Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Edit < Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 < enumInst2 )
        {
            strActualValue = "Edit < Simulation";
        }
        else
        {
            strActualValue = "Edit >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Edit <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 > enumInst2 )
        {
            strActualValue = "Edit > Simulation";
        }
        else
        {
            strActualValue = "Edit <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Edit <= Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 <= enumInst2 )
        {
            strActualValue = "Edit <= Simulation";
        }
        else
        {
            strActualValue = "Edit > Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Edit < Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 >= enumInst2 )
        {
            strActualValue = "Edit >= Simulation";
        }
        else
        {
            strActualValue = "Edit < Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst1 = CEnumMode(); // InvalidEnumerator
    enumInst2 = EMode::Simulation;

    try
    {
        strDesiredValue = "Invalid != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 == enumInst2 )
        {
            strActualValue = "Invalid == Simulation";
        }
        else
        {
            strActualValue = "Invalid != Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid != Simulation";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 != enumInst2 )
        {
            strActualValue = "Invalid != Simulation";
        }
        else
        {
            strActualValue = "Invalid == Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 < enumInst2 )
        {
            strActualValue = "Invalid < Simulation";
        }
        else
        {
            strActualValue = "Invalid >= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 > enumInst2 )
        {
            strActualValue = "Invalid > Simulation";
        }
        else
        {
            strActualValue = "Invalid <= Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 <= enumInst2 )
        {
            strActualValue = "Invalid <= Simulation";
        }
        else
        {
            strActualValue = "Invalid > Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 >= enumInst2 )
        {
            strActualValue = "Invalid >= Simulation";
        }
        else
        {
            strActualValue = "Invalid < Simulation";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst1 = EMode::Simulation;
    enumInst2 = CEnumMode(); // InvalidEnumerator

    try
    {
        strDesiredValue = "Simulation != Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 == enumInst2 )
        {
            strActualValue = "Simulation == Invalid";
        }
        else
        {
            strActualValue = "Simulation != Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Simulation != Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 != enumInst2 )
        {
            strActualValue = "Simulation != Invalid";
        }
        else
        {
            strActualValue = "Simulation == Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 < enumInst2 )
        {
            strActualValue = "Simulation < Invalid";
        }
        else
        {
            strActualValue = "Simulation >= Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 > enumInst2 )
        {
            strActualValue = "Simulation > Invalid";
        }
        else
        {
            strActualValue = "Simulation <= Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 <= enumInst2 )
        {
            strActualValue = "Simulation <= Invalid";
        }
        else
        {
            strActualValue = "Simulation > Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 >= enumInst2 )
        {
            strActualValue = "Simulation >= Invalid";
        }
        else
        {
            strActualValue = "Simulation < Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst1 = CEnumMode(); // InvalidEnumerator
    enumInst2 = CEnumMode(); // InvalidEnumerator

    try
    {
        strDesiredValue = "Invalid == Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 == enumInst2 )
        {
            strActualValue = "Invalid == Invalid";
        }
        else
        {
            strActualValue = "Invalid != Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid == Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 != enumInst2 )
        {
            strActualValue = "Invalid != Invalid";
        }
        else
        {
            strActualValue = "Invalid == Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 < enumInst2 )
        {
            strActualValue = "Invalid < Invalid";
        }
        else
        {
            strActualValue = "Invalid >= Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 > enumInst2 )
        {
            strActualValue = "Invalid > Invalid";
        }
        else
        {
            strActualValue = "Invalid <= Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 <= enumInst2 )
        {
            strActualValue = "Invalid <= Invalid";
        }
        else
        {
            strActualValue = "Invalid > Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum ZS::System::EMode> not in range [0..2][Edit..Undefined]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 >= enumInst2 )
        {
            strActualValue = "Invalid >= Invalid";
        }
        else
        {
            strActualValue = "Invalid < Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateZSSysModeOperatorCompareWithEnumTemplate

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeOperatorIncDec( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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
            strDesiredValue = strEnumeratorSource + "++: " + strEnumeratorResult;
            strlstDesiredValues.append(strDesiredValue);

            enumInst++;
            if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
            else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
            else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
            else strEnumeratorResult = "Invalid";
            strActualValue = strEnumeratorSource + "++: " + strEnumeratorResult;
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        strlstActualValues.append(strActualValue);
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
            strDesiredValue = "++" + strEnumeratorSource + ": " + strEnumeratorResult;
            strlstDesiredValues.append(strDesiredValue);

            ++enumInst;
            if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
            else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
            else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
            else strEnumeratorResult = "Invalid";
            strActualValue = "++" + strEnumeratorSource + ": " + strEnumeratorResult;
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        strlstActualValues.append(strActualValue);
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
            strDesiredValue = strEnumeratorSource + "--: " + strEnumeratorResult;
            strlstDesiredValues.append(strDesiredValue);

            enumInst--;
            if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
            else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
            else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
            else strEnumeratorResult = "Invalid";
            strActualValue = strEnumeratorSource + "--: " + strEnumeratorResult;
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        strlstActualValues.append(strActualValue);
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
            strDesiredValue = "--" + strEnumeratorSource + ": " + strEnumeratorResult;
            strlstDesiredValues.append(strDesiredValue);

            --enumInst;
            if( enumInst.enumerator() == EMode::Edit ) { strEnumeratorResult = "Edit"; }
            else if( enumInst.enumerator() == EMode::Simulation ) { strEnumeratorResult = "Simulation"; }
            else if( enumInst.enumerator() == EMode::Undefined ) { strEnumeratorResult = "Undefined"; }
            else strEnumeratorResult = "Invalid";
            strActualValue = "--" + strEnumeratorSource + ": " + strEnumeratorResult;
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateZSSysModeOperatorIncDec

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeInstMethodToString( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

                strDesiredValue = strEnumeratorSource + ".toString(" + strAlias + "): " + strEnumeratorSource;
                strlstDesiredValues.append(strDesiredValue);

                strEnumeratorResult = enumInst.toString(idxAlias);
                strActualValue = strEnumeratorSource + ".toString(" + strAlias + "): " + strEnumeratorResult;
            }
            catch( CException& exc )
            {
                strActualValue = exc.getAddErrInfo();
            }
            strlstActualValues.append(strActualValue);
        }
    }

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateZSSysModeInstMethodToString

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateZSSysModeForLoop( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = strEnumerator + ".toString(): " + strEnumerator;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = strEnumerator + ".toString(): " + enumInst.toString();
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    for( iEnumerator = CEnumMode::count()-1, enumInst = CEnumMode::count()-1; enumInst >= 0; --iEnumerator, --enumInst )
    {
        if( iEnumerator == 0 ) { strEnumerator = "Edit"; }
        else if( iEnumerator == 1 ) { strEnumerator = "Simulation"; }
        else if( iEnumerator == 2 ) { strEnumerator = "Undefined"; }
        else { strEnumerator = QString::number(iEnumerator); }

        strDesiredValue = strEnumerator + ".toString(): " + strEnumerator;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = strEnumerator + ".toString(): " + enumInst.toString();
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateZSSysModeForLoop

/*==============================================================================
public: // test step methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedClassMethodToString( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = "CEnumProcessorClock::toString(" + strEnumerator + "): " + strEnumerator;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "CEnumProcessorClock::toString(" + strEnumerator + "): " + CEnumProcessorClock::toString(static_cast<EProcessorClock>(iEnumerator));
        strlstActualValues.append(strActualValue);
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

            strDesiredValue = "CEnumProcessorClock::toString(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strDesiredValue += strResultExpected;
            strlstDesiredValues.append(strDesiredValue);

            strActualValue = "CEnumProcessorClock::toString(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strEnumerator = CEnumProcessorClock::toString(enumerator, idxAlias);
            strActualValue += strEnumerator;
            strlstActualValues.append(strActualValue);
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

        strDesiredValue = "CEnumProcessorClock::toString(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
        strDesiredValue += "Enumerator " + strEnumerator + " does not have an alias string at index " + QString::number(idxAlias);
        strlstDesiredValues.append(strDesiredValue);

        try
        {
            strActualValue = "CEnumProcessorClock::toString(" + strEnumerator + ", " + enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias)) + "): ";
            strEnumerator = CEnumProcessorClock::toString(enumerator, idxAlias);
            strActualValue += strEnumerator;
        }
        catch( CException& exc )
        {
            strActualValue += exc.getAddErrInfo();
        }
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    strDesiredValue = "CEnumProcessorClock::toString(-1): Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        iEnumerator = -1;
        enumerator = static_cast<EProcessorClock>(iEnumerator);
        strActualValue = "CEnumProcessorClock::toString(" + QString::number(static_cast<int>(enumerator)) + "): ";
        strActualValue += CEnumProcessorClock::toString(enumerator);
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    strDesiredValue = "CEnumProcessorClock::toString(3): Enumerator 3 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        iEnumerator = CEnumProcessorClock::count();
        enumerator = static_cast<EProcessorClock>(iEnumerator);
        strActualValue = "CEnumProcessorClock::toString(" + QString::number(static_cast<int>(enumerator)) + "): ";
        strActualValue += CEnumProcessorClock::toString(enumerator);
    }
    catch( CException& exc )
    {
        strActualValue = "CEnumProcessorClock::toString(" + QString::number(iEnumerator) + "): " + exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateUserDefinedClassMethodToString

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedClassMethodToEnumerator( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + "): ";
        strDesiredValue += strEnumeratorSource;
        strlstDesiredValues.append(strDesiredValue);

        enumerator = CEnumProcessorClock::toEnumerator(strEnumeratorSource);

        if( enumerator == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumerator == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumerator == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumerator)); }

        strActualValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + "): ";
        strActualValue += strEnumeratorResult;
        strlstActualValues.append(strActualValue);
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

            strDesiredValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + "): ";
            strDesiredValue += strEnumeratorResult;
            strlstDesiredValues.append(strDesiredValue);

            enumerator = CEnumProcessorClock::toEnumerator(strEnumeratorSource, idxAlias);

            if( enumerator == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
            else if( enumerator == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
            else if( enumerator == EProcessorClock::High ) { strEnumeratorResult = "High"; }
            else { strEnumeratorResult = QString::number(static_cast<int>(enumerator)); }

            strActualValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + "): ";
            strActualValue += strEnumeratorResult;
            strlstActualValues.append(strActualValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strDesiredValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    enumerator = CEnumProcessorClock::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strDesiredValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    enumerator = CEnumProcessorClock::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strDesiredValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    enumerator = CEnumProcessorClock::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strDesiredValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    enumerator = CEnumProcessorClock::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strDesiredValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    enumerator = CEnumProcessorClock::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strDesiredValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    enumerator = CEnumProcessorClock::toEnumerator(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias6;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Map does not contain alias strings at index " + QString::number(idxAlias);

    strDesiredValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

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
    strActualValue = "CEnumProcessorClock::toEnumerator(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    valSource = 5.12e6;
    strEnumeratorResult = "Low";

    strDesiredValue = "CEnumProcessorClock::toEnumerator(" + valSource.toString() + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    enumerator = CEnumProcessorClock::toEnumerator(valSource, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue = "CEnumProcessorClock::toEnumerator(" + valSource.toString() + ", &bOk): ";
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    valSource = 5.12e8;
    strEnumeratorResult = "Medium";

    strDesiredValue = "CEnumProcessorClock::toEnumerator(" + valSource.toString() + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    enumerator = CEnumProcessorClock::toEnumerator(valSource, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue = "CEnumProcessorClock::toEnumerator(" + valSource.toString() + ", &bOk): ";
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    valSource = 5.12e9;
    strEnumeratorResult = "High";

    strDesiredValue = "CEnumProcessorClock::toEnumerator(" + valSource.toString() + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    enumerator = CEnumProcessorClock::toEnumerator(valSource, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue = "CEnumProcessorClock::toEnumerator(" + valSource.toString() + ", &bOk): ";
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    valSource = 5.12;
    strEnumeratorResult = "-1";

    strDesiredValue = "CEnumProcessorClock::toEnumerator(" + valSource.toString() + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    enumerator = CEnumProcessorClock::toEnumerator(valSource, &bOk);
    if( enumerator == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumerator == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumerator == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumerator));
    strActualValue = "CEnumProcessorClock::toEnumerator(" + valSource.toString() + ", &bOk): ";
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateUserDefinedClassMethodToEnumerator

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedClassMethodToValue( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = "CEnumProcessorClock::toValue(" + strEnumerator + "): ";
        if( valExpected.canConvert(QVariant::Double) ) strDesiredValue += QString::number(valExpected.toDouble(),'g');
        strlstDesiredValues.append(strDesiredValue);

        valResult = CEnumProcessorClock::toValue(enumerator);
        strActualValue = "CEnumProcessorClock::toValue(" + strEnumerator + "): ";
        if( valResult.canConvert(QVariant::Double) ) strActualValue += QString::number(valResult.toDouble(),'g');
        strlstActualValues.append(strActualValue);
    }

    iEnumerator = -1;
    enumerator = static_cast<EProcessorClock>(iEnumerator);
    strEnumerator = QString::number(static_cast<int>(enumerator));

    strDesiredValue = "CEnumProcessorClock::toValue(" + strEnumerator + "): ";
    strDesiredValue += "Enumerator " + strEnumerator + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumProcessorClock::toValue(" + strEnumerator + "): ";
        valResult = CEnumProcessorClock::toValue(enumerator);
        if( valResult.canConvert(QVariant::Double) ) strActualValue += QString::number(valResult.toDouble(),'g');
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    iEnumerator = CEnumProcessorClock::count();
    enumerator = static_cast<EProcessorClock>(iEnumerator);
    strEnumerator = QString::number(static_cast<int>(enumerator));

    strDesiredValue = "CEnumProcessorClock::toValue(" + strEnumerator + "): ";
    strDesiredValue += "Enumerator " + strEnumerator + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumProcessorClock::toValue(" + strEnumerator + "): ";
        valResult = CEnumProcessorClock::toValue(enumerator);
        if( valResult.canConvert(QVariant::Double) ) strActualValue += QString::number(valResult.toDouble(),'g');
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

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

        strDesiredValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", Double, &bOk): ";
        if( valExpected.canConvert(QVariant::Double) ) strDesiredValue += QString::number(valExpected.toDouble(),'g') + ", true";
        else strDesiredValue += valExpected.toString() + ", false";
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", Double, &bOk): ";
        valResult = CEnumProcessorClock::toValue(enumerator, QVariant::Double, &bOk);
        if( bOk ) strActualValue += QString::number(valResult.toDouble(),'g') + ", " + bool2Str(bOk);
        else strActualValue += valResult.toString() + ", " + bool2Str(bOk);
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    enumerator = static_cast<EProcessorClock>(iEnumerator);
    strEnumerator = QString::number(static_cast<int>(enumerator));

    strDesiredValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", Double, &bOk): ";
    strDesiredValue += "Enumerator " + strEnumerator + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", Double, &bOk): ";
        valResult = CEnumProcessorClock::toValue(enumerator, QVariant::Double, &bOk);
        if( bOk ) strActualValue += QString::number(valResult.toDouble(),'g') + ", " + bool2Str(bOk);
        else strActualValue += valResult.toString() + ", " + bool2Str(bOk);
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    iEnumerator = CEnumProcessorClock::count();
    enumerator = static_cast<EProcessorClock>(iEnumerator);
    strEnumerator = QString::number(static_cast<int>(enumerator));

    strDesiredValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", Double, &bOk): ";
    strDesiredValue += "Enumerator " + strEnumerator + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", Double, &bOk): ";
        valResult = CEnumProcessorClock::toValue(enumerator, QVariant::Double, &bOk);
        if( bOk ) strActualValue += QString::number(valResult.toDouble(),'g') + ", " + bool2Str(bOk);
        else strActualValue += valResult.toString() + ", " + bool2Str(bOk);
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

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

        strDesiredValue = "CEnumProcessorClock::toValue(" + strEnumerator + "): ";
        if( valExpected.canConvert(QVariant::Double) ) strDesiredValue += QString::number(valExpected.toDouble(),'g');
        else strDesiredValue += valExpected.toString();
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "CEnumProcessorClock::toValue(" + strEnumerator + "): ";
        valResult = CEnumProcessorClock::toValue(strEnumerator);
        if( valResult.canConvert(QVariant::Double) ) strActualValue += QString::number(valResult.toDouble(),'g');
        strlstActualValues.append(strActualValue);
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

            strDesiredValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", Double, " + strAlias + "): ";
            if( valExpected.canConvert(QVariant::Double) ) strDesiredValue += QString::number(valExpected.toDouble(),'g');
            else strDesiredValue += valExpected.toString();
            strlstDesiredValues.append(strDesiredValue);

            strActualValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", Double, " + strAlias + "): ";
            valResult = CEnumProcessorClock::toValue(strEnumerator, QVariant::Double, idxAlias);
            if( valResult.canConvert(QVariant::Double) ) strActualValue += QString::number(valResult.toDouble(),'g');
            else strActualValue += valResult.toString();
            strlstActualValues.append(strActualValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumerator = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    valExpected = 5.12e8;

    strDesiredValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    if( valExpected.canConvert(QVariant::Double) ) strDesiredValue += QString::number(valExpected.toDouble(),'g') + ", true";
    else strDesiredValue += valExpected.toString() + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = CEnumProcessorClock::toValue(strEnumerator, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strActualValue += QString::number(valResult.toDouble(),'g') + ", true";
    else strActualValue += valResult.toString() + ", false";
    strlstActualValues.append(strActualValue);

    strEnumerator = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    valExpected = QVariant();

    strDesiredValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    if( valExpected.canConvert(QVariant::Double) ) strDesiredValue += QString::number(valExpected.toDouble(),'g') + ", true";
    else strDesiredValue += valExpected.toString() + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = CEnumProcessorClock::toValue(strEnumerator, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strActualValue += QString::number(valResult.toDouble(),'g') + ", true";
    else strActualValue += valResult.toString() + ", false";
    strlstActualValues.append(strActualValue);

    strEnumerator = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    valExpected = 5.12e8;

    strDesiredValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    if( valExpected.canConvert(QVariant::Double) ) strDesiredValue += QString::number(valExpected.toDouble(),'g') + ", true";
    else strDesiredValue += valExpected.toString() + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = CEnumProcessorClock::toValue(strEnumerator, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strActualValue += QString::number(valResult.toDouble(),'g') + ", true";
    else strActualValue += valResult.toString() + ", false";
    strlstActualValues.append(strActualValue);

    strEnumerator = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    valExpected = 5.12e8;

    strDesiredValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Int, " + strCaseSensitivity + ", &bOk): ";
    if( valExpected.canConvert(QVariant::Double) ) strDesiredValue += QString::number(valExpected.toDouble(),'g') + ", true";
    else strDesiredValue += valExpected.toString() + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Int, " + strCaseSensitivity + ", &bOk): ";
    valResult = CEnumProcessorClock::toValue(strEnumerator, QVariant::Int, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strActualValue += QString::number(valResult.toDouble(),'g') + ", true";
    else strActualValue += valResult.toString() + ", false";
    strlstActualValues.append(strActualValue);

    strEnumerator = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    valExpected = QVariant();

    strDesiredValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    if( valExpected.canConvert(QVariant::Double) ) strDesiredValue += QString::number(valExpected.toDouble(),'g') + ", true";
    else strDesiredValue += valExpected.toString() + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = CEnumProcessorClock::toValue(strEnumerator, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strActualValue += QString::number(valResult.toDouble(),'g') + ", true";
    else strActualValue += valResult.toString() + ", false";
    strlstActualValues.append(strActualValue);

    strEnumerator = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    valExpected = 5.12e8;

    strDesiredValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    if( valExpected.canConvert(QVariant::Double) ) strDesiredValue += QString::number(valExpected.toDouble(),'g') + ", true";
    else strDesiredValue += valExpected.toString() + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    valResult = CEnumProcessorClock::toValue(strEnumerator, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
    if( valResult.canConvert(QVariant::Double) ) strActualValue += QString::number(valResult.toDouble(),'g') + ", true";
    else strActualValue += valResult.toString() + ", false";
    strlstActualValues.append(strActualValue);

    strEnumerator = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias6;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    valExpected = QVariant();

    strDesiredValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += "Map does not contain alias strings at index " + QString::number(idxAlias);
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumProcessorClock::toValue(" + strEnumerator + ", " + strAlias + ", Invalid, " + strCaseSensitivity + ", &bOk): ";
        valResult = CEnumProcessorClock::toValue(strEnumerator, QVariant::Invalid, idxAlias, caseSensitivity, &bOk);
        if( valResult.canConvert(QVariant::Double) ) strActualValue += QString::number(valResult.toDouble(),'g') + ", true";
        else strActualValue += valResult.toString() + ", false";
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateUserDefinedClassMethodToValue

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedClassMethodFromString( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + "): ";
        strDesiredValue += strEnumeratorSource;
        strlstDesiredValues.append(strDesiredValue);

        enumInst = CEnumProcessorClock::fromString(strEnumeratorSource);

        if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }

        strActualValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + "): ";
        strActualValue += strEnumeratorResult;
        strlstActualValues.append(strActualValue);
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

            strDesiredValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + "): ";
            strDesiredValue += strEnumeratorResult;
            strlstDesiredValues.append(strDesiredValue);

            strActualValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + "): ";
            enumInst = CEnumProcessorClock::fromString(strEnumeratorSource, idxAlias);
            if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
            else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
            else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
            else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
            strActualValue += strEnumeratorResult;
            strlstActualValues.append(strActualValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strDesiredValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strDesiredValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strDesiredValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strDesiredValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strDesiredValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "Medium";

    strDesiredValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += strEnumeratorResult + ", true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    strEnumeratorSource = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias6;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumeratorResult = "-1";

    strDesiredValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
    strDesiredValue += "Map does not contain alias strings at index " + QString::number(idxAlias);
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumProcessorClock::fromString(" + strEnumeratorSource + ", " + strAlias + ", " + strCaseSensitivity + ", &bOk): ";
        enumInst = CEnumProcessorClock::fromString(strEnumeratorSource, idxAlias, caseSensitivity, &bOk);
        if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
        else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
        else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
        else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
        strActualValue += strEnumeratorResult + ", " + bool2Str(bOk);
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateUserDefinedClassMethodFromString

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedClassMethodFromValue( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = "CEnumProcessorClock::fromValue(" + val.toString() + "): ";
        strDesiredValue += strEnumerator;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "CEnumProcessorClock::fromValue(" + val.toString() + "): ";
        enumInst = CEnumProcessorClock::fromValue(val);

        if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(enumInst.enumerator())); }

        strActualValue += strEnumerator;
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    val = 5.12e8;

    strDesiredValue = "CEnumProcessorClock::fromValue(" + val.toString() + ", &bOk): ";
    strDesiredValue += "Medium, true";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::fromValue(" + val.toString() + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromValue(val, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
    else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
    else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
    else { strEnumerator = QString::number(static_cast<int>(enumInst.enumerator())); }
    strActualValue += strEnumerator + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    val = QVariant();

    strDesiredValue = "CEnumProcessorClock::fromValue(" + val.toString() + ", &bOk): ";
    strDesiredValue += "-1, false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::fromValue(" + val.toString() + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromValue(val, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
    else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
    else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
    else { strEnumerator = QString::number(static_cast<int>(enumInst.enumerator())); }
    strActualValue += strEnumerator + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    val = 5.12e3;

    strDesiredValue = "CEnumProcessorClock::fromValue(" + val.toString() + ", &bOk): ";
    strDesiredValue += "-1, false";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::fromValue(" + val.toString() + ", &bOk): ";
    enumInst = CEnumProcessorClock::fromValue(val, &bOk);
    if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
    else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
    else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
    else { strEnumerator = QString::number(static_cast<int>(enumInst.enumerator())); }
    strActualValue += strEnumerator + ", " + bool2Str(bOk);
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateUserDefinedClassMethodFromValue

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedCtors( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

    CEnumProcessorClock* pEnumInst = nullptr;

    QString strEnumerator;
    QString strAlias;
    QString strCaseSensitivity;

    Qt::CaseSensitivity caseSensitivity;

    int iEnumerator;
    int idxAlias;

    // -------------------------------------------------------------------------

    strDesiredValue = "CEnumProcessorClock::ctor(): -1";
    strlstDesiredValues.append(strDesiredValue);

    pEnumInst = new CEnumProcessorClock();
    if( pEnumInst->enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
    else if( pEnumInst->enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
    else if( pEnumInst->enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
    else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
    strActualValue = "CEnumProcessorClock::ctor(): " + strEnumerator;
    strlstActualValues.append(strActualValue);
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

        strDesiredValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        strDesiredValue += strEnumerator;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumProcessorClock(static_cast<EProcessorClock>(iEnumerator));
        if( pEnumInst->enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( pEnumInst->enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( pEnumInst->enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strActualValue += strEnumerator;
        strlstActualValues.append(strActualValue);
        delete pEnumInst;
        pEnumInst = nullptr;
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumerator = QString::number(iEnumerator);

    strDesiredValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
    strDesiredValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumProcessorClock(static_cast<EProcessorClock>(iEnumerator));
        if( pEnumInst->enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( pEnumInst->enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( pEnumInst->enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strActualValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    iEnumerator = CEnumProcessorClock::count();
    strEnumerator = QString::number(iEnumerator);

    strDesiredValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
    strDesiredValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumProcessorClock(static_cast<EProcessorClock>(iEnumerator));
        if( pEnumInst->enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( pEnumInst->enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( pEnumInst->enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strActualValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);
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

        strDesiredValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        strDesiredValue += strEnumerator;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumProcessorClock(iEnumerator);
        if( pEnumInst->enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( pEnumInst->enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( pEnumInst->enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strActualValue += strEnumerator;
        strlstActualValues.append(strActualValue);
        delete pEnumInst;
        pEnumInst = nullptr;
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumerator = QString::number(iEnumerator);

    strDesiredValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
    strDesiredValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumProcessorClock(iEnumerator);
        if( pEnumInst->enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( pEnumInst->enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( pEnumInst->enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strActualValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    iEnumerator = CEnumProcessorClock::count();
    strEnumerator = QString::number(iEnumerator);

    strDesiredValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
    strDesiredValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumProcessorClock(iEnumerator);
        if( pEnumInst->enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( pEnumInst->enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( pEnumInst->enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strActualValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);
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

        strDesiredValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        strDesiredValue += strEnumerator;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "CEnumProcessorClock::ctor(" + strEnumerator + "): ";
        pEnumInst = new CEnumProcessorClock(strEnumerator);
        if( pEnumInst->enumerator() == EProcessorClock::Low ) { strEnumerator = "Low"; }
        else if( pEnumInst->enumerator() == EProcessorClock::Medium ) { strEnumerator = "Medium"; }
        else if( pEnumInst->enumerator() == EProcessorClock::High ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator())); }
        strActualValue += strEnumerator;
        strlstActualValues.append(strActualValue);
        delete pEnumInst;
        pEnumInst = nullptr;
    }

    // -------------------------------------------------------------------------

    strEnumerator = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);

    strDesiredValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strDesiredValue += strEnumerator;
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumProcessorClock(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EProcessorClock::Low ) strEnumerator = "Low";
    else if( pEnumInst->enumerator() == EProcessorClock::Medium ) strEnumerator = "Medium";
    else if( pEnumInst->enumerator() == EProcessorClock::High ) strEnumerator = "High";
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strActualValue += strEnumerator;
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "-1";

    strDesiredValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strDesiredValue += strEnumerator;
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumProcessorClock(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EProcessorClock::Low ) strEnumerator = "Low";
    else if( pEnumInst->enumerator() == EProcessorClock::Medium ) strEnumerator = "Medium";
    else if( pEnumInst->enumerator() == EProcessorClock::High ) strEnumerator = "High";
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strActualValue += strEnumerator;
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "medIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrName;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "Medium";

    strDesiredValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strDesiredValue += strEnumerator;
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumProcessorClock(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EProcessorClock::Low ) strEnumerator = "Low";
    else if( pEnumInst->enumerator() == EProcessorClock::Medium ) strEnumerator = "Medium";
    else if( pEnumInst->enumerator() == EProcessorClock::High ) strEnumerator = "High";
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strActualValue += strEnumerator;
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "MEDium";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "Medium";

    strDesiredValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strDesiredValue += strEnumerator;
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumProcessorClock(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EProcessorClock::Low ) strEnumerator = "Low";
    else if( pEnumInst->enumerator() == EProcessorClock::Medium ) strEnumerator = "Medium";
    else if( pEnumInst->enumerator() == EProcessorClock::High ) strEnumerator = "High";
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strActualValue += strEnumerator;
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseSensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "-1";

    strDesiredValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strDesiredValue += strEnumerator;
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumProcessorClock(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EProcessorClock::Low ) strEnumerator = "Low";
    else if( pEnumInst->enumerator() == EProcessorClock::Medium ) strEnumerator = "Medium";
    else if( pEnumInst->enumerator() == EProcessorClock::High ) strEnumerator = "High";
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strActualValue += strEnumerator;
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "meDIUM";
    idxAlias = ZS::System::EEnumEntryAliasStrUndefined;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "Medium";

    strDesiredValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strDesiredValue += strEnumerator;
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    pEnumInst = new CEnumProcessorClock(strEnumerator, idxAlias, caseSensitivity);
    if( pEnumInst->enumerator() == EProcessorClock::Low ) strEnumerator = "Low";
    else if( pEnumInst->enumerator() == EProcessorClock::Medium ) strEnumerator = "Medium";
    else if( pEnumInst->enumerator() == EProcessorClock::High ) strEnumerator = "High";
    else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
    strActualValue += strEnumerator;
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    strEnumerator = "Medium";
    idxAlias = ZS::System::EEnumEntryAliasStrAlias6;
    strAlias = enumEntryAlias2Str(static_cast<EEnumEntryAliasStr>(idxAlias));
    caseSensitivity = Qt::CaseInsensitive;
    strCaseSensitivity = qCaseSensitivity2Str(caseSensitivity);
    strEnumerator = "-1";

    strDesiredValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
    strDesiredValue += "Map does not contain alias strings at index " + QString::number(idxAlias);
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumProcessorClock::ctor(" + strEnumerator + ", " + strAlias + ", " + strCaseSensitivity + "): ";
        pEnumInst = new CEnumProcessorClock(strEnumerator, idxAlias, caseSensitivity);
        if( pEnumInst->enumerator() == EProcessorClock::Low ) strEnumerator = "Low";
        else if( pEnumInst->enumerator() == EProcessorClock::Medium ) strEnumerator = "Medium";
        else if( pEnumInst->enumerator() == EProcessorClock::High ) strEnumerator = "High";
        else strEnumerator = QString::number(static_cast<int>(pEnumInst->enumerator()));
        strActualValue += strEnumerator;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);
    delete pEnumInst;
    pEnumInst = nullptr;

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateUserDefinedCtors

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedOperatorAssign( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
        strDesiredValue += strEnumeratorSource;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
        enumInst = static_cast<EProcessorClock>(iEnumerator);
        if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strActualValue += strEnumeratorResult;
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumeratorSource = QString::number(iEnumerator);

    strDesiredValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
    strDesiredValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
        enumInst = static_cast<EProcessorClock>(iEnumerator);
        if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strActualValue += strEnumeratorResult;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    iEnumerator = CEnumProcessorClock::count();
    strEnumeratorSource = QString::number(iEnumerator);

    strDesiredValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
    strDesiredValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
        enumInst = static_cast<EProcessorClock>(iEnumerator);
        if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strActualValue += strEnumeratorResult;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    for( iEnumerator = 0; iEnumerator < CEnumProcessorClock::count(); ++iEnumerator )
    {
        strEnumeratorSource = "-1";

        if( iEnumerator == 0 ) { strEnumeratorSource = "Low"; }
        else if( iEnumerator == 1 ) { strEnumeratorSource = "Medium"; }
        else if( iEnumerator == 2 ) { strEnumeratorSource = "High"; }
        else { strEnumeratorSource = QString::number(iEnumerator); }

        strDesiredValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
        strDesiredValue += strEnumeratorSource;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
        enumInst = iEnumerator;
        if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strActualValue += strEnumeratorResult;
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    iEnumerator = -1;
    strEnumeratorSource = QString::number(iEnumerator);

    strDesiredValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
    strDesiredValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
        enumInst = iEnumerator;
        if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strActualValue += strEnumeratorResult;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    iEnumerator = CEnumProcessorClock::count();
    strEnumeratorSource = QString::number(iEnumerator);

    strDesiredValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
    strDesiredValue += "Enumerator " + QString::number(iEnumerator) + " of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
    strlstDesiredValues.append(strDesiredValue);

    try
    {
        strActualValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
        enumInst = iEnumerator;
        if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
        else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
        else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
        else { strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator())); }
        strActualValue += strEnumeratorResult;
    }
    catch( CException& exc )
    {
        strActualValue += exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

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

            strDesiredValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
            strDesiredValue += strEnumeratorResult;
            strlstDesiredValues.append(strDesiredValue);

            strActualValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
            enumInst = strEnumeratorSource;
            if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
            else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
            else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
            else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
            strActualValue += strEnumeratorResult;
            strlstActualValues.append(strActualValue);

        } // for( iEnumerator = 0; iEnumerator < iEnumArrLen; ++iEnumerator )
    } // for( idxAlias = 0; idxAlias <= EEnumEntryAliasStrSCPILong; ++idxAlias )

    // -------------------------------------------------------------------------

    strEnumeratorSource = "medIUM";

    strDesiredValue = "CEnumProcessorClock = " + strEnumeratorSource + ": -1";
    strlstDesiredValues.append(strDesiredValue);

    strActualValue = "CEnumProcessorClock = " + strEnumeratorSource + ": ";
    enumInst = strEnumeratorSource;
    if( enumInst.enumerator() == EProcessorClock::Low ) strEnumeratorResult = "Low";
    else if( enumInst.enumerator() == EProcessorClock::Medium ) strEnumeratorResult = "Medium";
    else if( enumInst.enumerator() == EProcessorClock::High ) strEnumeratorResult = "High";
    else strEnumeratorResult = QString::number(static_cast<int>(enumInst.enumerator()));
    strActualValue += strEnumeratorResult;
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateUserDefinedOperatorAssign

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedOperatorCompareWithEnumerator( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

    EProcessorClock enumerator;

    CEnumProcessorClock enumInst;

    // -------------------------------------------------------------------------

    enumerator = EProcessorClock::Medium;
    enumInst = EProcessorClock::Medium;

    try
    {
        strDesiredValue = "Medium == Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == enumerator )
        {
            strActualValue = "Medium == Medium";
        }
        else
        {
            strActualValue = "Medium != Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium == Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != enumerator )
        {
            strActualValue = "Medium != Medium";
        }
        else
        {
            strActualValue = "Medium == Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Medium >= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < enumerator )
        {
            strActualValue = "Medium < Medium";
        }
        else
        {
            strActualValue = "Medium >= Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium <= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > enumerator )
        {
            strActualValue = "Medium > Medium";
        }
        else
        {
            strActualValue = "Medium <= Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Medium <= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= enumerator )
        {
            strActualValue = "Medium <= Medium";
        }
        else
        {
            strActualValue = "Medium > Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium >= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= enumerator )
        {
            strActualValue = "Medium >= Medium";
        }
        else
        {
            strActualValue = "Medium < Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Low;
    enumerator = EProcessorClock::High;

    try
    {
        strDesiredValue = "Low != High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == enumerator )
        {
            strActualValue = "Low == High";
        }
        else
        {
            strActualValue = "Low != High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Low != High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != enumerator )
        {
            strActualValue = "Low != High";
        }
        else
        {
            strActualValue = "Low == High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Low < High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < enumerator )
        {
            strActualValue = "Low < High";
        }
        else
        {
            strActualValue = "Low >= High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Low <= High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > enumerator )
        {
            strActualValue = "Low > High";
        }
        else
        {
            strActualValue = "Low <= High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Low <= High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= enumerator )
        {
            strActualValue = "Low <= High";
        }
        else
        {
            strActualValue = "Low > High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Low < High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= enumerator )
        {
            strActualValue = "Low >= High";
        }
        else
        {
            strActualValue = "Low < High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumProcessorClock(); // InvalidEnumerator
    enumerator = EProcessorClock::Medium;

    try
    {
        strDesiredValue = "Invalid != Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == enumerator )
        {
            strActualValue = "Invalid == Medium";
        }
        else
        {
            strActualValue = "Invalid != Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid != Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != enumerator )
        {
            strActualValue = "Invalid != Medium";
        }
        else
        {
            strActualValue = "Invalid == Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < enumerator )
        {
            strActualValue = "Invalid < Medium";
        }
        else
        {
            strActualValue = "Invalid >= Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > enumerator )
        {
            strActualValue = "Invalid > Medium";
        }
        else
        {
            strActualValue = "Invalid <= Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= enumerator )
        {
            strActualValue = "Invalid <= Medium";
        }
        else
        {
            strActualValue = "Invalid > Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= enumerator )
        {
            strActualValue = "Invalid >= Medium";
        }
        else
        {
            strActualValue = "Invalid < Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Medium;
    enumerator = static_cast<EProcessorClock>(EInvalidEnumerator);

    try
    {
        strDesiredValue = "Medium != Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == enumerator )
        {
            strActualValue = "Medium == Invalid";
        }
        else
        {
            strActualValue = "Medium != Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium != Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != enumerator )
        {
            strActualValue = "Medium != Invalid";
        }
        else
        {
            strActualValue = "Medium == Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < enumerator )
        {
            strActualValue = "Medium < Invalid";
        }
        else
        {
            strActualValue = "Medium >= Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > enumerator )
        {
            strActualValue = "Medium > Invalid";
        }
        else
        {
            strActualValue = "Medium <= Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= enumerator )
        {
            strActualValue = "Medium <= Invalid";
        }
        else
        {
            strActualValue = "Medium > Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= enumerator )
        {
            strActualValue = "Medium >= Invalid";
        }
        else
        {
            strActualValue = "Medium < Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumProcessorClock(); // InvalidEnumerator
    enumerator = static_cast<EProcessorClock>(EInvalidEnumerator);

    try
    {
        strDesiredValue = "Invalid == Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == enumerator )
        {
            strActualValue = "Invalid == Invalid";
        }
        else
        {
            strActualValue = "Invalid != Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid == Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != enumerator )
        {
            strActualValue = "Invalid != Invalid";
        }
        else
        {
            strActualValue = "Invalid == Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < enumerator )
        {
            strActualValue = "Invalid < Invalid";
        }
        else
        {
            strActualValue = "Invalid >= Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > enumerator )
        {
            strActualValue = "Invalid > Invalid";
        }
        else
        {
            strActualValue = "Invalid <= Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= enumerator )
        {
            strActualValue = "Invalid <= Invalid";
        }
        else
        {
            strActualValue = "Invalid > Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= enumerator )
        {
            strActualValue = "Invalid >= Invalid";
        }
        else
        {
            strActualValue = "Invalid < Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateUserDefinedOperatorCompareWithEnumerator

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedOperatorCompareWithInt( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

    CEnumProcessorClock enumInst;

    int iEnumerator;

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Medium;
    iEnumerator = static_cast<int>(EProcessorClock::Medium);

    try
    {
        strDesiredValue = "Medium == Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == iEnumerator )
        {
            strActualValue = "Medium == Medium";
        }
        else
        {
            strActualValue = "Medium != Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium == Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != iEnumerator )
        {
            strActualValue = "Medium != Medium";
        }
        else
        {
            strActualValue = "Medium == Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Medium >= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < iEnumerator )
        {
            strActualValue = "Medium < Medium";
        }
        else
        {
            strActualValue = "Medium >= Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium <= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > iEnumerator )
        {
            strActualValue = "Medium > Medium";
        }
        else
        {
            strActualValue = "Medium <= Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Medium <= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= iEnumerator )
        {
            strActualValue = "Medium <= Medium";
        }
        else
        {
            strActualValue = "Medium > Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium >= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= iEnumerator )
        {
            strActualValue = "Medium >= Medium";
        }
        else
        {
            strActualValue = "Medium < Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Low;
    iEnumerator = static_cast<int>(EProcessorClock::High);

    try
    {
        strDesiredValue = "Low != High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == iEnumerator )
        {
            strActualValue = "Low == High";
        }
        else
        {
            strActualValue = "Low != High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Low != High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != iEnumerator )
        {
            strActualValue = "Low != High";
        }
        else
        {
            strActualValue = "Low == High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Low < High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < iEnumerator )
        {
            strActualValue = "Low < High";
        }
        else
        {
            strActualValue = "Low >= High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Low <= High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > iEnumerator )
        {
            strActualValue = "Low > High";
        }
        else
        {
            strActualValue = "Low <= High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Low <= High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= iEnumerator )
        {
            strActualValue = "Low <= High";
        }
        else
        {
            strActualValue = "Low > High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Low < High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= iEnumerator )
        {
            strActualValue = "Low >= High";
        }
        else
        {
            strActualValue = "Low < High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumProcessorClock(); // InvalidEnumerator
    iEnumerator = static_cast<int>(EProcessorClock::Medium);

    try
    {
        strDesiredValue = "Invalid != Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == iEnumerator )
        {
            strActualValue = "Invalid == Medium";
        }
        else
        {
            strActualValue = "Invalid != Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid != Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != iEnumerator )
        {
            strActualValue = "Invalid != Medium";
        }
        else
        {
            strActualValue = "Invalid == Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid < Medium: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < iEnumerator )
        {
            strActualValue = "Invalid < Medium";
        }
        else
        {
            strActualValue = "Invalid < Medium: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid > Medium: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > iEnumerator )
        {
            strActualValue = "Invalid > Medium";
        }
        else
        {
            strActualValue = "Invalid > Medium: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid <= Medium: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= iEnumerator )
        {
            strActualValue = "Invalid <= Medium";
        }
        else
        {
            strActualValue = "Invalid <= Medium: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid >= Medium: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= iEnumerator )
        {
            strActualValue = "Invalid >= Medium";
        }
        else
        {
            strActualValue = "Invalid >= Medium: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Medium;
    iEnumerator = EInvalidEnumerator;

    try
    {
        strDesiredValue = "Medium != Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == iEnumerator )
        {
            strActualValue = "Medium == Invalid";
        }
        else
        {
            strActualValue = "Medium != Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium != Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != iEnumerator )
        {
            strActualValue = "Medium != Invalid";
        }
        else
        {
            strActualValue = "Medium == Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Medium < Invalid: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < iEnumerator )
        {
            strActualValue = "Medium < Invalid";
        }
        else
        {
            strActualValue = "Medium < Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium > Invalid: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > iEnumerator )
        {
            strActualValue = "Medium > Invalid";
        }
        else
        {
            strActualValue = "Medium > Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Medium <= Invalid: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= iEnumerator )
        {
            strActualValue = "Medium <= Invalid";
        }
        else
        {
            strActualValue = "Medium <= Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium >= Invalid: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= iEnumerator )
        {
            strActualValue = "Medium >= Invalid";
        }
        else
        {
            strActualValue = "Medium >= Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumProcessorClock(); // InvalidEnumerator
    iEnumerator = EInvalidEnumerator;

    try
    {
        strDesiredValue = "Invalid == Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == iEnumerator )
        {
            strActualValue = "Invalid == Invalid";
        }
        else
        {
            strActualValue = "Invalid != Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid == Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != iEnumerator )
        {
            strActualValue = "Invalid != Invalid";
        }
        else
        {
            strActualValue = "Invalid == Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid < Invalid: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < iEnumerator )
        {
            strActualValue = "Invalid < Invalid";
        }
        else
        {
            strActualValue = "Invalid < Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid > Invalid: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > iEnumerator )
        {
            strActualValue = "Invalid > Invalid";
        }
        else
        {
            strActualValue = "Invalid > Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid <= Invalid: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= iEnumerator )
        {
            strActualValue = "Invalid <= Invalid";
        }
        else
        {
            strActualValue = "Invalid <= Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid >= Invalid: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= iEnumerator )
        {
            strActualValue = "Invalid >= Invalid";
        }
        else
        {
            strActualValue = "Invalid >= Invalid: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateUserDefinedOperatorCompareWithInt

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedOperatorCompareWithCharPtr( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

    CEnumProcessorClock enumInst;

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Medium;
    const char* szEnumerator = "Medium";

    try
    {
        strDesiredValue = "Medium == Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == szEnumerator )
        {
            strActualValue = "Medium == Medium";
        }
        else
        {
            strActualValue = "Medium != Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium == Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != szEnumerator )
        {
            strActualValue = "Medium != Medium";
        }
        else
        {
            strActualValue = "Medium == Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Medium >= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < szEnumerator )
        {
            strActualValue = "Medium < Medium";
        }
        else
        {
            strActualValue = "Medium >= Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium <= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > szEnumerator )
        {
            strActualValue = "Medium > Medium";
        }
        else
        {
            strActualValue = "Medium <= Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Medium <= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= szEnumerator )
        {
            strActualValue = "Medium <= Medium";
        }
        else
        {
            strActualValue = "Medium > Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium >= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= szEnumerator )
        {
            strActualValue = "Medium >= Medium";
        }
        else
        {
            strActualValue = "Medium < Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Low;
    szEnumerator = "High";

    try
    {
        strDesiredValue = "Low != High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == szEnumerator )
        {
            strActualValue = "Low == High";
        }
        else
        {
            strActualValue = "Low != High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Low != High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != szEnumerator )
        {
            strActualValue = "Low != High";
        }
        else
        {
            strActualValue = "Low == High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Low < High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < szEnumerator )
        {
            strActualValue = "Low < High";
        }
        else
        {
            strActualValue = "Low >= High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Low <= High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > szEnumerator )
        {
            strActualValue = "Low > High";
        }
        else
        {
            strActualValue = "Low <= High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Low <= High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= szEnumerator )
        {
            strActualValue = "Low <= High";
        }
        else
        {
            strActualValue = "Low > High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Low < High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= szEnumerator )
        {
            strActualValue = "Low >= High";
        }
        else
        {
            strActualValue = "Low < High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumProcessorClock(); // InvalidEnumerator
    szEnumerator = "Medium";

    try
    {
        strDesiredValue = "Invalid != Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == szEnumerator )
        {
            strActualValue = "Invalid == Medium";
        }
        else
        {
            strActualValue = "Invalid != Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid != Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != szEnumerator )
        {
            strActualValue = "Invalid != Medium";
        }
        else
        {
            strActualValue = "Invalid == Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid < Medium: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < szEnumerator )
        {
            strActualValue = "Invalid < Medium";
        }
        else
        {
            strActualValue = "Invalid < Medium: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid > Medium: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > szEnumerator )
        {
            strActualValue = "Invalid > Medium";
        }
        else
        {
            strActualValue = "Invalid > Medium: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid <= Medium: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= szEnumerator )
        {
            strActualValue = "Invalid <= Medium";
        }
        else
        {
            strActualValue = "Invalid <= Medium: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid >= Medium: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= szEnumerator )
        {
            strActualValue = "Invalid >= Medium";
        }
        else
        {
            strActualValue = "Invalid >= Medium: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Medium;
    szEnumerator = "Undefined";

    try
    {
        strDesiredValue = "Medium != Undefined";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == szEnumerator )
        {
            strActualValue = "Medium == Undefined";
        }
        else
        {
            strActualValue = "Medium != Undefined";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium != Undefined";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != szEnumerator )
        {
            strActualValue = "Medium != Undefined";
        }
        else
        {
            strActualValue = "Medium == Undefined";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Medium < Undefined: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < szEnumerator )
        {
            strActualValue = "Medium < Undefined";
        }
        else
        {
            strActualValue = "Medium < Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium > Undefined: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > szEnumerator )
        {
            strActualValue = "Medium > Undefined";
        }
        else
        {
            strActualValue = "Medium > Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Medium <= Undefined: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= szEnumerator )
        {
            strActualValue = "Medium <= Undefined";
        }
        else
        {
            strActualValue = "Medium <= Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium >= Undefined: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= szEnumerator )
        {
            strActualValue = "Medium >= Undefined";
        }
        else
        {
            strActualValue = "Medium >= Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumProcessorClock(); // InvalidEnumerator
    szEnumerator = "Undefined";

    try
    {
        strDesiredValue = "Invalid != Undefined";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == szEnumerator )
        {
            strActualValue = "Invalid == Undefined";
        }
        else
        {
            strActualValue = "Invalid != Undefined";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid != Undefined";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != szEnumerator )
        {
            strActualValue = "Invalid != Undefined";
        }
        else
        {
            strActualValue = "Invalid == Undefined";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid < Undefined: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < szEnumerator )
        {
            strActualValue = "Invalid < Undefined";
        }
        else
        {
            strActualValue = "Invalid < Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid > Undefined: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > szEnumerator )
        {
            strActualValue = "Invalid > Undefined";
        }
        else
        {
            strActualValue = "Invalid > Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid <= Undefined: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= szEnumerator )
        {
            strActualValue = "Invalid <= Undefined";
        }
        else
        {
            strActualValue = "Invalid <= Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid >= Undefined: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= szEnumerator )
        {
            strActualValue = "Invalid >= Undefined";
        }
        else
        {
            strActualValue = "Invalid >= Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateUserDefinedOperatorCompareWithCharPtr

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedOperatorCompareWithQString( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

    CEnumProcessorClock enumInst;

    QString strEnumerator;

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Medium;
    strEnumerator = "Medium";

    try
    {
        strDesiredValue = "Medium == Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == strEnumerator )
        {
            strActualValue = "Medium == Medium";
        }
        else
        {
            strActualValue = "Medium != Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium == Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != strEnumerator )
        {
            strActualValue = "Medium != Medium";
        }
        else
        {
            strActualValue = "Medium == Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Medium >= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < strEnumerator )
        {
            strActualValue = "Medium < Medium";
        }
        else
        {
            strActualValue = "Medium >= Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium <= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > strEnumerator )
        {
            strActualValue = "Medium > Medium";
        }
        else
        {
            strActualValue = "Medium <= Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Medium <= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= strEnumerator )
        {
            strActualValue = "Medium <= Medium";
        }
        else
        {
            strActualValue = "Medium > Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium >= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= strEnumerator )
        {
            strActualValue = "Medium >= Medium";
        }
        else
        {
            strActualValue = "Medium < Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Low;
    strEnumerator = "High";

    try
    {
        strDesiredValue = "Low != High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == strEnumerator )
        {
            strActualValue = "Low == High";
        }
        else
        {
            strActualValue = "Low != High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Low != High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != strEnumerator )
        {
            strActualValue = "Low != High";
        }
        else
        {
            strActualValue = "Low == High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Low < High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < strEnumerator )
        {
            strActualValue = "Low < High";
        }
        else
        {
            strActualValue = "Low >= High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Low <= High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > strEnumerator )
        {
            strActualValue = "Low > High";
        }
        else
        {
            strActualValue = "Low <= High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Low <= High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= strEnumerator )
        {
            strActualValue = "Low <= High";
        }
        else
        {
            strActualValue = "Low > High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Low < High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= strEnumerator )
        {
            strActualValue = "Low >= High";
        }
        else
        {
            strActualValue = "Low < High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumProcessorClock(); // InvalidEnumerator
    strEnumerator = "Medium";

    try
    {
        strDesiredValue = "Invalid != Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == strEnumerator )
        {
            strActualValue = "Invalid == Medium";
        }
        else
        {
            strActualValue = "Invalid != Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid != Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != strEnumerator )
        {
            strActualValue = "Invalid != Medium";
        }
        else
        {
            strActualValue = "Invalid == Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid < Medium: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < strEnumerator )
        {
            strActualValue = "Invalid < Medium";
        }
        else
        {
            strActualValue = "Invalid < Medium: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid > Medium: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > strEnumerator )
        {
            strActualValue = "Invalid > Medium";
        }
        else
        {
            strActualValue = "Invalid > Medium: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid <= Medium: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= strEnumerator )
        {
            strActualValue = "Invalid <= Medium";
        }
        else
        {
            strActualValue = "Invalid <= Medium: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid >= Medium: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= strEnumerator )
        {
            strActualValue = "Invalid >= Medium";
        }
        else
        {
            strActualValue = "Invalid >= Medium: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = EProcessorClock::Medium;
    strEnumerator = "Undefined";

    try
    {
        strDesiredValue = "Medium != Undefined";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == strEnumerator )
        {
            strActualValue = "Medium == Undefined";
        }
        else
        {
            strActualValue = "Medium != Undefined";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium != Undefined";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != strEnumerator )
        {
            strActualValue = "Medium != Undefined";
        }
        else
        {
            strActualValue = "Medium == Undefined";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Medium < Undefined: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < strEnumerator )
        {
            strActualValue = "Medium < Undefined";
        }
        else
        {
            strActualValue = "Medium < Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium > Undefined: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > strEnumerator )
        {
            strActualValue = "Medium > Undefined";
        }
        else
        {
            strActualValue = "Medium > Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Medium <= Undefined: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= strEnumerator )
        {
            strActualValue = "Medium <= Undefined";
        }
        else
        {
            strActualValue = "Medium <= Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium >= Undefined: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= strEnumerator )
        {
            strActualValue = "Medium >= Undefined";
        }
        else
        {
            strActualValue = "Medium >= Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst = CEnumProcessorClock(); // InvalidEnumerator
    strEnumerator = "Undefined";

    try
    {
        strDesiredValue = "Invalid != Undefined";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst == strEnumerator )
        {
            strActualValue = "Invalid == Undefined";
        }
        else
        {
            strActualValue = "Invalid != Undefined";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid != Undefined";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst != strEnumerator )
        {
            strActualValue = "Invalid != Undefined";
        }
        else
        {
            strActualValue = "Invalid == Undefined";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid < Undefined: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst < strEnumerator )
        {
            strActualValue = "Invalid < Undefined";
        }
        else
        {
            strActualValue = "Invalid < Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid > Undefined: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst > strEnumerator )
        {
            strActualValue = "Invalid > Undefined";
        }
        else
        {
            strActualValue = "Invalid > Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Invalid <= Undefined: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst <= strEnumerator )
        {
            strActualValue = "Invalid <= Undefined";
        }
        else
        {
            strActualValue = "Invalid <= Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid >= Undefined: false";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst >= strEnumerator )
        {
            strActualValue = "Invalid >= Undefined";
        }
        else
        {
            strActualValue = "Invalid >= Undefined: false";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateUserDefinedOperatorCompareWithQString

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedOperatorCompareWithEnumTemplate( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

    CEnumProcessorClock enumInst1;
    CEnumProcessorClock enumInst2;

    // -------------------------------------------------------------------------

    enumInst1 = EProcessorClock::Medium;
    enumInst2 = EProcessorClock::Medium;

    try
    {
        strDesiredValue = "Medium == Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 == enumInst2 )
        {
            strActualValue = "Medium == Medium";
        }
        else
        {
            strActualValue = "Medium != Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium == Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 != enumInst2 )
        {
            strActualValue = "Medium != Medium";
        }
        else
        {
            strActualValue = "Medium == Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Medium >= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 < enumInst2 )
        {
            strActualValue = "Medium < Medium";
        }
        else
        {
            strActualValue = "Medium >= Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium <= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 > enumInst2 )
        {
            strActualValue = "Medium > Medium";
        }
        else
        {
            strActualValue = "Medium <= Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Medium <= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 <= enumInst2 )
        {
            strActualValue = "Medium <= Medium";
        }
        else
        {
            strActualValue = "Medium > Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium >= Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 >= enumInst2 )
        {
            strActualValue = "Medium >= Medium";
        }
        else
        {
            strActualValue = "Medium < Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst1 = EProcessorClock::Low;
    enumInst2 = EProcessorClock::High;

    try
    {
        strDesiredValue = "Low != High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 == enumInst2 )
        {
            strActualValue = "Low == High";
        }
        else
        {
            strActualValue = "Low != High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Low != High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 != enumInst2 )
        {
            strActualValue = "Low != High";
        }
        else
        {
            strActualValue = "Low == High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Low < High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 < enumInst2 )
        {
            strActualValue = "Low < High";
        }
        else
        {
            strActualValue = "Low >= High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Low <= High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 > enumInst2 )
        {
            strActualValue = "Low > High";
        }
        else
        {
            strActualValue = "Low <= High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Low <= High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 <= enumInst2 )
        {
            strActualValue = "Low <= High";
        }
        else
        {
            strActualValue = "Low > High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Low < High";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 >= enumInst2 )
        {
            strActualValue = "Low >= High";
        }
        else
        {
            strActualValue = "Low < High";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst1 = CEnumProcessorClock(); // InvalidEnumerator
    enumInst2 = EProcessorClock::Medium;

    try
    {
        strDesiredValue = "Invalid != Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 == enumInst2 )
        {
            strActualValue = "Invalid == Medium";
        }
        else
        {
            strActualValue = "Invalid != Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid != Medium";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 != enumInst2 )
        {
            strActualValue = "Invalid != Medium";
        }
        else
        {
            strActualValue = "Invalid == Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 < enumInst2 )
        {
            strActualValue = "Invalid < Medium";
        }
        else
        {
            strActualValue = "Invalid >= Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 > enumInst2 )
        {
            strActualValue = "Invalid > Medium";
        }
        else
        {
            strActualValue = "Invalid <= Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 <= enumInst2 )
        {
            strActualValue = "Invalid <= Medium";
        }
        else
        {
            strActualValue = "Invalid > Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 >= enumInst2 )
        {
            strActualValue = "Invalid >= Medium";
        }
        else
        {
            strActualValue = "Invalid < Medium";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst1 = EProcessorClock::Medium;
    enumInst2 = CEnumProcessorClock(); // InvalidEnumerator

    try
    {
        strDesiredValue = "Medium != Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 == enumInst2 )
        {
            strActualValue = "Medium == Invalid";
        }
        else
        {
            strActualValue = "Medium != Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Medium != Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 != enumInst2 )
        {
            strActualValue = "Medium != Invalid";
        }
        else
        {
            strActualValue = "Medium == Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 < enumInst2 )
        {
            strActualValue = "Medium < Invalid";
        }
        else
        {
            strActualValue = "Medium >= Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 > enumInst2 )
        {
            strActualValue = "Medium > Invalid";
        }
        else
        {
            strActualValue = "Medium <= Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 <= enumInst2 )
        {
            strActualValue = "Medium <= Invalid";
        }
        else
        {
            strActualValue = "Medium > Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 >= enumInst2 )
        {
            strActualValue = "Medium >= Invalid";
        }
        else
        {
            strActualValue = "Medium < Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    enumInst1 = CEnumProcessorClock(); // InvalidEnumerator
    enumInst2 = CEnumProcessorClock(); // InvalidEnumerator

    try
    {
        strDesiredValue = "Invalid == Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 == enumInst2 )
        {
            strActualValue = "Invalid == Invalid";
        }
        else
        {
            strActualValue = "Invalid != Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Invalid == Invalid";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 != enumInst2 )
        {
            strActualValue = "Invalid != Invalid";
        }
        else
        {
            strActualValue = "Invalid == Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 < enumInst2 )
        {
            strActualValue = "Invalid < Invalid";
        }
        else
        {
            strActualValue = "Invalid >= Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 > enumInst2 )
        {
            strActualValue = "Invalid > Invalid";
        }
        else
        {
            strActualValue = "Invalid <= Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 <= enumInst2 )
        {
            strActualValue = "Invalid <= Invalid";
        }
        else
        {
            strActualValue = "Invalid > Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    try
    {
        strDesiredValue = "Enumerator -1 of enumeration class ZS::System::CEnum<enum EProcessorClock> not in range [0..2][Low..High]";
        strlstDesiredValues.append(strDesiredValue);

        if( enumInst1 >= enumInst2 )
        {
            strActualValue = "Invalid >= Invalid";
        }
        else
        {
            strActualValue = "Invalid < Invalid";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateUserDefinedOperatorCompareWithEnumTemplate

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedOperatorIncDec( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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
            strDesiredValue = strEnumeratorSource + "++: " + strEnumeratorResult;
            strlstDesiredValues.append(strDesiredValue);

            enumInst++;
            if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
            else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
            else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
            else strEnumeratorResult = "Invalid";
            strActualValue = strEnumeratorSource + "++: " + strEnumeratorResult;
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        strlstActualValues.append(strActualValue);
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
            strDesiredValue = "++" + strEnumeratorSource + ": " + strEnumeratorResult;
            strlstDesiredValues.append(strDesiredValue);

            ++enumInst;
            if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
            else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
            else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
            else strEnumeratorResult = "Invalid";
            strActualValue = "++" + strEnumeratorSource + ": " + strEnumeratorResult;
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        strlstActualValues.append(strActualValue);
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
            strDesiredValue = strEnumeratorSource + "--: " + strEnumeratorResult;
            strlstDesiredValues.append(strDesiredValue);

            enumInst--;
            if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
            else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
            else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
            else strEnumeratorResult = "Invalid";
            strActualValue = strEnumeratorSource + "--: " + strEnumeratorResult;
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        strlstActualValues.append(strActualValue);
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
            strDesiredValue = "--" + strEnumeratorSource + ": " + strEnumeratorResult;
            strlstDesiredValues.append(strDesiredValue);

            --enumInst;
            if( enumInst.enumerator() == EProcessorClock::Low ) { strEnumeratorResult = "Low"; }
            else if( enumInst.enumerator() == EProcessorClock::Medium ) { strEnumeratorResult = "Medium"; }
            else if( enumInst.enumerator() == EProcessorClock::High ) { strEnumeratorResult = "High"; }
            else strEnumeratorResult = "Invalid";
            strActualValue = "--" + strEnumeratorSource + ": " + strEnumeratorResult;
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateUserDefinedOperatorIncDec

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedInstMethodToString( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

                strDesiredValue = strEnumeratorSource + ".toString(" + strAlias + "): " + strEnumeratorSource;
                strlstDesiredValues.append(strDesiredValue);

                strEnumeratorResult = enumInst.toString(idxAlias);
                strActualValue = strEnumeratorSource + ".toString(" + strAlias + "): " + strEnumeratorResult;
            }
            catch( CException& exc )
            {
                strActualValue = exc.getAddErrInfo();
            }
            strlstActualValues.append(strActualValue);
        }
    }

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateUserDefinedInstMethodToString

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedInstMethodToValue( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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
            strDesiredValue = strEnumerator + ".toValue(QVariant::Double, &bOk): " + val.toString() + ", " + bool2Str(bOk);
            strlstDesiredValues.append(strDesiredValue);

            val = enumInst.toValue(QVariant::Double, &bOk);
            strActualValue = strEnumerator + ".toValue(QVariant::Double, &bOk): " + val.toString() + ", " + bool2Str(bOk);
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        strlstActualValues.append(strActualValue);
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
            strDesiredValue = strEnumerator + ".toValue(QVariant::Invalid, &bOk): " + val.toString() + ", " + bool2Str(bOk);
            strlstDesiredValues.append(strDesiredValue);

            val = enumInst.toValue(QVariant::Invalid, &bOk);
            strActualValue = strEnumerator + ".toValue(QVariant::Invalid, &bOk): " + val.toString() + ", " + bool2Str(bOk);
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateUserDefinedInstMethodToValue

//------------------------------------------------------------------------------
void CTest::doTestStepEnumClassTemplateUserDefinedForLoop( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

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

        strDesiredValue = strEnumerator + ".toString(): " + strEnumerator;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = strEnumerator + ".toString(): " + enumInst.toString();
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    for( iEnumerator = CEnumProcessorClock::count()-1, enumInst = CEnumProcessorClock::count()-1; enumInst >= 0; --iEnumerator, --enumInst )
    {
        if( iEnumerator == 0 ) { strEnumerator = "Low"; }
        else if( iEnumerator == 1 ) { strEnumerator = "Medium"; }
        else if( iEnumerator == 2 ) { strEnumerator = "High"; }
        else { strEnumerator = QString::number(iEnumerator); }

        strDesiredValue = strEnumerator + ".toString(): " + strEnumerator;
        strlstDesiredValues.append(strDesiredValue);

        strActualValue = strEnumerator + ".toString(): " + enumInst.toString();
        strlstActualValues.append(strActualValue);
    }

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepEnumClassTemplateUserDefinedForLoop

