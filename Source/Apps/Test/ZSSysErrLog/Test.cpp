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

#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStepIdxTreeEntry.h"
#include "ZSTest/ZSTestStepIdxTree.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysAux.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Apps::Test::SysErrLog;


/*******************************************************************************
class CTest
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest( const QString& i_strTestStepsFileName ) :
//------------------------------------------------------------------------------
    ZS::Test::CTest( "ZSErrLog", i_strTestStepsFileName )
{
    int idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " addEntries",
        /* strOperation    */ "ErrLog.addEntry",
        /* pGrpParent      */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepAddErrLogEntries(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ErrLog.clear",
        /* strOperation    */ "ErrLog.clear",
        /* pGrpParent      */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepClearErrLog(ZS::Test::CTestStep*)) );

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

/*==============================================================================
public: // test step methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepAddErrLogEntries( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    CErrLog* pErrLog = nullptr;

    SErrLogEntry* pErrLogEntry = nullptr;

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("doTestStepAddErrLogEntries");

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "ZSErrLog Existing";
        strlstExpectedValues.append(strExpectedValue);

        pErrLog = CErrLog::GetInstance();

        if( pErrLog == nullptr )
        {
            strResultValue = "ZSErrLog Not Existing";
        }
        else // if( pErrLogInst2 != nullptr )
        {
            strResultValue = "ZSErrLog Existing";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    catch(...)
    {
        strResultValue = "Unknown exception thrown";
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    if( pErrLog != nullptr )
    {
        try
        {
            errResultInfo.setSeverity(EResultSeveritySuccess);
            errResultInfo.setResult(EResultSuccess);
            errResultInfo.setAddErrInfoDscr("Just a test");

            strExpectedValue = errResultInfo.toString() + " not added to " + pErrLog->objectName();
            strlstExpectedValues.append(strExpectedValue);

            pErrLog->addEntry(errResultInfo);

            pErrLogEntry = pErrLog->findEntry(errResultInfo);

            if( pErrLogEntry != nullptr )
            {
                strResultValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            }
            else
            {
                strResultValue = errResultInfo.toString() + " not added to " + pErrLog->objectName();
            }
        }
        catch( CException& exc )
        {
            strResultValue = exc.getAddErrInfo();
        }
        catch(...)
        {
            strResultValue = "Unknown exception thrown";
        }
        strlstResultValues.append(strResultValue);

        try
        {
            SErrResultInfo errResultInfoTmp(
                /* errSource  */ pErrLog->nameSpace(), pErrLog->className(), pErrLog->objectName(), "addEntry",
                /* result     */ EResultListIsFull,
                /* severity   */ EResultSeverityError,
                /* strAddInfo */ "Maximum number of error log entries for severity " + resultSeverity2Str(errResultInfo.getSeverity()) + " reached." );

            strExpectedValue = errResultInfoTmp.toString() + " added to " + pErrLog->objectName();
            strlstExpectedValues.append(strExpectedValue);

            pErrLogEntry = pErrLog->findEntry(errResultInfoTmp);

            if( pErrLogEntry != nullptr )
            {
                strResultValue = errResultInfoTmp.toString() + " added to " + pErrLog->objectName();
            }
            else
            {
                strResultValue = errResultInfoTmp.toString() + " not added to " + pErrLog->objectName();
            }
        }
        catch( CException& exc )
        {
            strResultValue = exc.getAddErrInfo();
        }
        catch(...)
        {
            strResultValue = "Unknown exception thrown";
        }
        strlstResultValues.append(strResultValue);

        // ---------------------------------------------------------------------

        try
        {
            errResultInfo.setSeverity(EResultSeverityInfo);
            errResultInfo.setResult(EResultRequestAborted);
            errResultInfo.setAddErrInfoDscr("Just a test");

            strExpectedValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            strlstExpectedValues.append(strExpectedValue);

            pErrLog->addEntry(errResultInfo);

            pErrLogEntry = pErrLog->findEntry(errResultInfo);

            if( pErrLogEntry != nullptr )
            {
                strResultValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            }
            else
            {
                strResultValue = errResultInfo.toString() + " not added to " + pErrLog->objectName();
            }
        }
        catch( CException& exc )
        {
            strResultValue = exc.getAddErrInfo();
        }
        catch(...)
        {
            strResultValue = "Unknown exception thrown";
        }
        strlstResultValues.append(strResultValue);

        // ---------------------------------------------------------------------

        try
        {
            errResultInfo.setSeverity(EResultSeverityWarning);
            errResultInfo.setResult(EResultRequestAborted);
            errResultInfo.setAddErrInfoDscr("Just a test");

            strExpectedValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            strlstExpectedValues.append(strExpectedValue);

            pErrLog->addEntry(errResultInfo);

            pErrLogEntry = pErrLog->findEntry(errResultInfo);

            if( pErrLogEntry != nullptr )
            {
                strResultValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            }
            else
            {
                strResultValue = errResultInfo.toString() + " not added to " + pErrLog->objectName();
            }
        }
        catch( CException& exc )
        {
            strResultValue = exc.getAddErrInfo();
        }
        catch(...)
        {
            strResultValue = "Unknown exception thrown";
        }
        strlstResultValues.append(strResultValue);

        // ---------------------------------------------------------------------

        try
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestAborted);
            errResultInfo.setAddErrInfoDscr("Just a test");

            strExpectedValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            strlstExpectedValues.append(strExpectedValue);

            pErrLog->addEntry(errResultInfo);

            pErrLogEntry = pErrLog->findEntry(errResultInfo);

            if( pErrLogEntry != nullptr )
            {
                strResultValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            }
            else
            {
                strResultValue = errResultInfo.toString() + " not added to " + pErrLog->objectName();
            }
        }
        catch( CException& exc )
        {
            strResultValue = exc.getAddErrInfo();
        }
        catch(...)
        {
            strResultValue = "Unknown exception thrown";
        }
        strlstResultValues.append(strResultValue);

        // ---------------------------------------------------------------------

        try
        {
            errResultInfo.setSeverity(EResultSeverityCritical);
            errResultInfo.setResult(EResultRequestAborted);
            errResultInfo.setAddErrInfoDscr("Just a test");

            strExpectedValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            strlstExpectedValues.append(strExpectedValue);

            pErrLog->addEntry(errResultInfo);

            pErrLogEntry = pErrLog->findEntry(errResultInfo);

            if( pErrLogEntry != nullptr )
            {
                strResultValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            }
            else
            {
                strResultValue = errResultInfo.toString() + " not added to " + pErrLog->objectName();
            }
        }
        catch( CException& exc )
        {
            strResultValue = exc.getAddErrInfo();
        }
        catch(...)
        {
            strResultValue = "Unknown exception thrown";
        }
        strlstResultValues.append(strResultValue);

    } // if( pErrLog != nullptr )

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepAddErrLogEntries

//------------------------------------------------------------------------------
void CTest::doTestStepClearErrLog( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strExpectedValue;
    QString strResultValue;

    CErrLog* pErrLog = nullptr;

    // -------------------------------------------------------------------------

    try
    {
        strExpectedValue = "ZSErrLog Existing";
        strlstExpectedValues.append(strExpectedValue);

        pErrLog = CErrLog::GetInstance();

        if( pErrLog == nullptr )
        {
            strResultValue = "ZSErrLog Not Existing";
        }
        else // if( pErrLogInst2 != nullptr )
        {
            strResultValue = "ZSErrLog Existing";
        }
    }
    catch( CException& exc )
    {
        strResultValue = exc.getAddErrInfo();
    }
    catch(...)
    {
        strResultValue = "Unknown exception thrown";
    }
    strlstResultValues.append(strResultValue);

    // -------------------------------------------------------------------------

    if( pErrLog != nullptr )
    {
        try
        {
            strExpectedValue = "ZSErrLog Empty";
            strlstExpectedValues.append(strExpectedValue);

            pErrLog->clear();

            if( pErrLog->getEntryCount() > 0 )
            {
                strResultValue = "ZSErrLog Not Empty";
            }
            else // if( pErrLogInst2 != nullptr )
            {
                strResultValue = "ZSErrLog Empty";
            }
        }
        catch( CException& exc )
        {
            strResultValue = exc.getAddErrInfo();
        }
        catch(...)
        {
            strResultValue = "Unknown exception thrown";
        }
        strlstResultValues.append(strResultValue);

    } // if( pErrLog != nullptr )

    // -------------------------------------------------------------------------

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepClearErrLog
