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
    int idxStep = 0;

    ZS::Test::CTestStepGroup* pGrpRounding = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "Rounding",
        /* pGrpParent */ nullptr );

    ZS::Test::CTestStepGroup* pGrpRound2Nearest = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "Round2Nearest",
        /* pGrpParent */ pGrpRounding );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "1 = round2Nearest(1.1, 0)",
        /* pGrpParent      */ pGrpRound2Nearest,
        /* szDoTestStepFct */ SLOT(doTestStepRound2Nearest(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep),
        /* strOperation    */ "2 = round2Nearest(1.6, 0)",
        /* pGrpParent      */ pGrpRound2Nearest,
        /* szDoTestStepFct */ SLOT(doTestStepRound2Nearest(ZS::Test::CTestStep*)) );

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
