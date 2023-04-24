/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
#include <QtCore/qrandom.h>
#endif
#include <QtCore/qthread.h>

#include "Test.h"

#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

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
CTest::CTest() :
//------------------------------------------------------------------------------
    ZS::Test::CTest("ZSSysMath")
{
    ZS::Test::CTestStep* pTestStep;

    int idxStep = 0;

    ZS::Test::CTestStepGroup* pGrpRounding = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "Rounding",
        /* pGrpParent */ nullptr );

    ZS::Test::CTestStepGroup* pGrpRound2Nearest = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "Round2Nearest",
        /* pGrpParent */ pGrpRounding );

    idxStep = 0;

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "1 = round2Nearest(1.1, 0)",
        /* pGrpParent      */ pGrpRound2Nearest,
        /* szDoTestStepFct */ SLOT(doTestStepRound2Nearest(ZS::Test::CTestStep*)) );
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "2 = round2Nearest(1.6, 0)",
        /* pGrpParent      */ pGrpRound2Nearest,
        /* szDoTestStepFct */ SLOT(doTestStepRound2Nearest(ZS::Test::CTestStep*)) );

    ZS::Test::CTestStepGroup* pGrpDivLines = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "Division Lines",
        /* pGrpParent */ nullptr );

    idxStep = 0;

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "calculateDivLines4LinSpacing()",
        /* pGrpParent      */ pGrpDivLines,
        /* szDoTestStepFct */ SLOT(doTestStepCalculateDivLines4LinSpacing(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"ScaleMinVal", 0}, {"ScaleMaxVal", 0}, {"ScaleRangePix", 0},
        {"DivLineDistMinVal", 0}, {"DivLineDistMinPix", 0}, {"UseDivLineDistValDecimalFactor25", false},
        {"DivLineFirstVal", 0}, {"DivLineDistFirstPix", 0}, {"DivLineDistVal", 0}, {"DivLineDistPix", 0},
        {"DivLinesCount", 0}});

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "calculateDivLines4LinSpacing()",
        /* pGrpParent      */ pGrpDivLines,
        /* szDoTestStepFct */ SLOT(doTestStepCalculateDivLines4LinSpacing(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"ScaleMinVal", 0}, {"ScaleMaxVal", 100.0}, {"ScaleRangePix", 1000},
        {"DivLineDistMinVal", 10.0}, {"DivLineDistMinPix", 50}, {"UseDivLineDistValDecimalFactor25", false},
        {"DivLineFirstVal", 0.0}, {"DivLineDistFirstPix", 0.0}, {"DivLineDistVal", 10.0}, {"DivLineDistPix", 100},
        {"DivLinesCount", 11}});

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
public slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepRound2Nearest( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strExpectedValue;
    QString strResultValue;
    QString strMth;
    QStringList strlstInArgs;

    QString strOperation = i_pTestStep->getOperation();

    splitMethodCallOperation(strOperation, strMth, strlstInArgs, strExpectedValue);

    i_pTestStep->setExpectedValue(strExpectedValue);

    bool bIsValidOperation = true;

    if( strMth == "round2Nearest") {
        double fVal = 0.0;
        int iTrailingDigits = -1;
        if( strlstInArgs.size() == 1 ) {
            fVal = strlstInArgs[0].toDouble();
        }
        else if( strlstInArgs.size() == 2 ) {
            fVal = strlstInArgs[0].toDouble();
            iTrailingDigits = strlstInArgs[1].toInt();
        }
        else {
            bIsValidOperation = false;
            strResultValue = "Invalid number of arguments";
        }
        if( bIsValidOperation ) {
            double fResult = Math::round2Nearest(fVal, iTrailingDigits);
            strResultValue = QString::number(
                fResult, 'f', iTrailingDigits >= 0 ? iTrailingDigits : 6);
        }
    }

    i_pTestStep->setResultValue(strResultValue);

} // doTestStepRound2Nearest

//------------------------------------------------------------------------------
void CTest::doTestStepCalculateDivLines4LinSpacing( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow)
    {
        double  fScaleMinVal = 0.0;
        double  fScaleMaxVal = 0.0;
        int     iScaleRangePix = 0;
        double  fDivLineDistMinVal = 0.0;
        int     iDivLineDistMinPix = 0;
        bool    bUseDivLineDistValDecimalFactor25 = false;
        double  fDivLineFirstValExpected = 0.0;
        double  fDivLineDistFirstPixExpected = 0.0;
        double  fDivLineDistValExpected = 0.0;
        double  fDivLineDistPixExpected = 0.0;
        int     iDivLineCountExpected = 0;

        QMap<QString, QString> mapExpectedValues;
        QMap<QString, QString> mapResultValues;

        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);

        for (const QString& strKey : dataRow.keys())
        {
            if (strKey == "ScaleMinVal") {
                fScaleMinVal = dataRow["ScaleMinVal"].toDouble();
            }
            else if (strKey == "ScaleMaxVal") {
                fScaleMaxVal = dataRow["ScaleMaxVal"].toDouble();
            }
            else if (strKey == "ScaleRangePix") {
                iScaleRangePix = dataRow["ScaleRangePix"].toInt();
            }
            else if (strKey == "DivLineDistMinVal") {
                fDivLineDistMinVal = dataRow["DivLineDistMinVal"].toDouble();
            }
            else if (strKey == "DivLineDistMinPix") {
                iDivLineDistMinPix = dataRow["DivLineDistMinPix"].toInt();
            }
            else if (strKey == "UseDivLineDistValDecimalFactor25") {
                bUseDivLineDistValDecimalFactor25 = dataRow["UseDivLineDistValDecimalFactor25"].toBool();
            }
            else if (strKey == "DivLineFirstVal") {
                fDivLineFirstValExpected = dataRow["DivLineFirstVal"].toDouble();
                mapExpectedValues[strKey] = dataRow["DivLineFirstVal"].toString();
            }
            else if (strKey == "DivLineDistFirstPix") {
                fDivLineDistFirstPixExpected = dataRow["DivLineDistFirstPix"].toDouble();
                mapExpectedValues[strKey] = dataRow["DivLineDistFirstPix"].toString();
            }
            else if (strKey == "DivLineDistVal") {
                fDivLineDistValExpected = dataRow["DivLineDistVal"].toDouble();
                mapExpectedValues[strKey] = dataRow["DivLineDistVal"].toString();
            }
            else if (strKey == "DivLineDistPix") {
                fDivLineDistPixExpected = dataRow["DivLineDistPix"].toDouble();
                mapExpectedValues[strKey] = dataRow["DivLineDistPix"].toString();
            }
            else if (strKey == "DivLinesCount") {
                iDivLineCountExpected = dataRow["DivLinesCount"].toInt();
                mapExpectedValues[strKey] = dataRow["DivLinesCount"].toString();
            }
        }

        double fDivLineFirstValResult = 0.0;
        double fDivLineDistFirstPixResult = 0.0;
        double fDivLineDistValResult = 0.0;
        double fDivLineDistPixResult = 0.0;

        int iDivLineCountResult = Math::calculateDivLines4LinSpacing(
            /* fScaleMinVal          */ fScaleMinVal,
            /* fScaleMaxVal          */ fScaleMaxVal,
            /* iScaleRangePix        */ iScaleRangePix,
            /* fDivLineDistMinVal    */ fDivLineDistMinVal,
            /* iDivLineDistMinPix    */ iDivLineDistMinPix,
            /* bUseDivLineFactor25   */ bUseDivLineDistValDecimalFactor25,
            /* pfDivLineFirstVal     */ &fDivLineFirstValResult,
            /* pfDivLineDistFirstPix */ &fDivLineDistFirstPixResult,
            /* pfDivLineDistVal      */ &fDivLineDistValResult,
            /* pfDivLineDistPix      */ &fDivLineDistPixResult );

        mapResultValues["DivLineFirstVal"] = QString::number(fDivLineFirstValResult);
        mapResultValues["DivLineDistFirstPix"] = QString::number(fDivLineDistFirstPixResult);
        mapResultValues["DivLineDistVal"] = QString::number(fDivLineDistValResult);
        mapResultValues["DivLineDistPix"] = QString::number(fDivLineDistPixResult);
        mapResultValues["DivLinesCount"] = QString::number(iDivLineCountResult);

        QString strExpectedValues;
        for (const QString& strKey : mapExpectedValues.keys())
        {
            if (!strExpectedValues.isEmpty()) strExpectedValues += ", ";
            strExpectedValues += strKey + "=" + mapExpectedValues[strKey];
        }
        strlstExpectedValues.append(strExpectedValues);

        QString strResultValues;
        for (const QString& strKey : mapResultValues.keys())
        {
            if (!strResultValues.isEmpty()) strResultValues += ", ";
            strResultValues += strKey + "=" + mapResultValues[strKey];
        }
        strlstResultValues.append(strResultValues);
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepCalculateDivLines4LinSpacing

/*==============================================================================
private: // instance auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Splits the operation string into sections.

    @param [in] i_strOperation
        Examples for strOperation:
        - "1 = round2Nearest(1.1, 0)"
    @param [in] o_strMth
        Method to be called (e.g. "round2Nearest")
    @param [in] o_strlstInArgs
        Arguments to be passed to the method call (e.g. "1.1", "0")
    @param [in] o_strMthRet
        Result of the method call (e.g. "1")
*/
void CTest::splitMethodCallOperation(
    const QString& i_strOperation,
    QString& o_strMth,
    QStringList& o_strlstInArgs,
    QString& o_strMthRet ) const
//------------------------------------------------------------------------------
{
    o_strMth = "";
    o_strlstInArgs.clear();
    o_strMthRet = "";

    // 1 = round2Nearest(1.1, 0)
    QStringList strlst;
    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    strlst = i_strOperation.split("=", Qt::SkipEmptyParts);
    #else
    strlst = i_strOperation.split("=", QString::SkipEmptyParts);
    #endif
    if( strlst.size() == 2 ) {
        o_strMthRet = strlst[0].trimmed();    // 1
        QString strMth = strlst[1].trimmed(); // round2Nearest(1.1, 0)
        int idx1 = strMth.indexOf("(");
        int idx2 = strMth.indexOf(")");
        if( idx1 > 0 && idx2 > idx1 ) {
            o_strMth = strMth.mid(0, idx1).trimmed();
            QString strArgs = strMth.mid(idx1+1, idx2-idx1-1).trimmed();
            o_strlstInArgs = strArgs.split(",");
            for( int idxStr = 0; idxStr < o_strlstInArgs.size(); ++idxStr ) {
                o_strlstInArgs[idxStr] = o_strlstInArgs[idxStr].trimmed();
            }
        }
    }
} // splitMethodCallOperation
