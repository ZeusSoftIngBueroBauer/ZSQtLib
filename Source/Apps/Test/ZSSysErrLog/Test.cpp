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
#include "ZSTest/ZSTestStepAdminObj.h"
#include "ZSTest/ZSTestStepAdminObjPool.h"
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
CTest::CTest() :
//------------------------------------------------------------------------------
    ZS::Test::CTest("ZSErrLog")
{
    int idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CErrLog::CreateInstance(ZS::Apps::Test::SysErrLog::ErrLog)",
        /* strOperation    */ "CErrLog::CreateInstance(ZS::Apps::Test::SysErrLog::ErrLog)",
        /* pGrpParent      */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepCreateErrLogInstance(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZS::Apps::Test::SysErrLog::ErrLog.addEntries",
        /* strOperation    */ "ZS::Apps::Test::SysErrLog::ErrLog.addEntry",
        /* pGrpParent      */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepAddErrLogEntries(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " ZS::Apps::Test::SysErrLog::ErrLog.clear",
        /* strOperation    */ "ZS::Apps::Test::SysErrLog::ErrLog.clear",
        /* pGrpParent      */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepClearErrLog(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " CErrLog::ReleaseInstance(ZS::Apps::Test::SysErrLog::ErrLog)",
        /* strOperation    */ "CErrLog::ReleaseInstance(ZS::Apps::Test::SysErrLog::ErrLog)",
        /* pGrpParent      */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepDestroyErrLogInstance(ZS::Test::CTestStep*)) );

    // Recall test admin object settings
    //----------------------------------

    m_pAdminObjPool->read_();

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
void CTest::doTestStepCreateErrLogInstance( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

    QString strOperation = i_pTestStep->getOperation();

    // Examples for strOperation:
    // - "CErrLog::CreateInstance(ZS::Apps::Test::SysErrLog::ErrLog)"

    QString strErrLogName;

    if( strOperation.startsWith("CErrLog::CreateInstance",Qt::CaseInsensitive) )
    {
        strErrLogName = strOperation;
        strErrLogName.remove("CErrLog::CreateInstance");
        strErrLogName.remove("(");
        strErrLogName.remove(")");

        strDesiredValue = strErrLogName + " Existing";
        strlstDesiredValues.append(strDesiredValue);
    }

    // -------------------------------------------------------------------------

    QString strAppNameNormalized = QCoreApplication::applicationName();

    // Range of IniFileScope: ["AppDir", "User", "System"]
    #ifdef __linux__
    // Using "System" on linux Mint ends up in directory "etc/xdg/<CompanyName>"
    // where the application has not write access rights. Stupid ...
    QString strIniFileScope = "User";
    #else
    QString strIniFileScope = "System"; // Default
    #endif

    // The application name may contain characters which are invalid in file names:
    strAppNameNormalized.remove(":");
    strAppNameNormalized.remove(" ");
    strAppNameNormalized.remove("\\");
    strAppNameNormalized.remove("/");
    strAppNameNormalized.remove("<");
    strAppNameNormalized.remove(">");

    QString strAppConfigDir = ZS::System::getAppConfigDir(strIniFileScope);
    QString strAppLogDir = ZS::System::getAppLogDir(strIniFileScope);

    QString strErrLogNameTmp = strErrLogName;

    strErrLogNameTmp.remove("::");
    if( strErrLogNameTmp.endsWith("ErrLogErrLog") )
    {
        strErrLogNameTmp.remove(strErrLogNameTmp.size()-7, 6);
    }

    QString strErrLogFileBaseName = strAppNameNormalized + "-" + strErrLogNameTmp;
    QString strErrLogFileSuffix = "xml";

    QString strErrLogFileAbsFilePath = strAppLogDir + "/" + strErrLogFileBaseName + "." + strErrLogFileSuffix;

    try
    {
        CErrLog::CreateInstance(true, strErrLogFileAbsFilePath, strErrLogName);

        if( CErrLog::GetInstance(strErrLogName) != nullptr )
        {
            strActualValue = strErrLogName + " Existing";

            emit errLogCreated(strErrLogName);
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    catch(...)
    {
        strActualValue = "Unknown exception thrown";
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepCreateErrLogInstance

//------------------------------------------------------------------------------
void CTest::doTestStepDestroyErrLogInstance( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

    QString strOperation = i_pTestStep->getOperation();

    // Examples for strOperation:
    // - "CErrLog::ReleaseInstance(ZS::Apps::Test::SysErrLog::ErrLog)"

    QString strErrLogName;

    if( strOperation.startsWith("CErrLog::ReleaseInstance",Qt::CaseInsensitive) )
    {
        strErrLogName = strOperation;
        strErrLogName.remove("CErrLog::ReleaseInstance");
        strErrLogName.remove("(");
        strErrLogName.remove(")");
    }

    // -------------------------------------------------------------------------

    CErrLog* pErrLog = nullptr;

    try
    {
        strDesiredValue = strErrLogName + " Existing";
        strlstDesiredValues.append(strDesiredValue);

        pErrLog = CErrLog::GetInstance(strErrLogName);

        if( pErrLog == nullptr )
        {
            strActualValue = strErrLogName + " Not Existing";
        }
        else
        {
            strActualValue = strErrLogName + " Existing";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    catch(...)
    {
        strActualValue = "Unknown exception thrown";
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    if( pErrLog != nullptr )
    {
        QString strErrLogAbsFilePath = pErrLog->getAbsFilePath();

        try
        {
            strDesiredValue = strErrLogAbsFilePath + " Existing";
            strlstDesiredValues.append(strDesiredValue);

            QFileInfo fileInfoErrLog(strErrLogAbsFilePath);

            if( fileInfoErrLog.exists() )
            {
                strActualValue = strErrLogAbsFilePath + " Existing";
            }
            else
            {
                strActualValue = strErrLogAbsFilePath + " Not Existing";
            }
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        catch(...)
        {
            strActualValue = "Unknown exception thrown";
        }
        strlstActualValues.append(strActualValue);

        // -------------------------------------------------------------------------

        try
        {
            strDesiredValue = strErrLogName + " File Existing";
            strlstDesiredValues.append(strDesiredValue);

            CErrLog::ReleaseInstance(strErrLogName);

            if( CErrLog::GetInstance(strErrLogName) == nullptr )
            {
                strActualValue = strErrLogName + " File Existing";
            }
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        catch(...)
        {
            strActualValue = "Unknown exception thrown";
        }
        strlstActualValues.append(strActualValue);

    } // if( pErrLog != nullptr )

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepDestroyErrLogInstance

//------------------------------------------------------------------------------
void CTest::doTestStepAddErrLogEntries( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

    QString strOperation = i_pTestStep->getOperation();

    // Examples for strOperation:
    // - "ZS::System::ErrLog.addEntry"

    QString strErrLogName;

    if( strOperation.endsWith(".addEntry",Qt::CaseInsensitive) )
    {
        strErrLogName = strOperation;
        strErrLogName.remove(".addEntry");
        strErrLogName.remove("(");
        strErrLogName.remove(")");
    }

    CErrLog* pErrLog = nullptr;

    SErrLogEntry* pErrLogEntry = nullptr;

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("doTestStepAddErrLogEntries");

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = strErrLogName + " Existing";
        strlstDesiredValues.append(strDesiredValue);

        pErrLog = CErrLog::GetInstance(strErrLogName);

        if( pErrLog == nullptr )
        {
            strActualValue = strErrLogName + " Not Existing";
        }
        else // if( pErrLogInst2 != nullptr )
        {
            strActualValue = strErrLogName + " Existing";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    catch(...)
    {
        strActualValue = "Unknown exception thrown";
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    if( pErrLog != nullptr )
    {
        try
        {
            errResultInfo.setSeverity(EResultSeveritySuccess);
            errResultInfo.setResult(EResultSuccess);
            errResultInfo.setAddErrInfoDscr("Just a test");

            strDesiredValue = errResultInfo.toString() + " not added to " + pErrLog->objectName();
            strlstDesiredValues.append(strDesiredValue);

            pErrLog->addEntry(errResultInfo);

            pErrLogEntry = pErrLog->findEntry(errResultInfo);

            if( pErrLogEntry != nullptr )
            {
                strActualValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            }
            else
            {
                strActualValue = errResultInfo.toString() + " not added to " + pErrLog->objectName();
            }
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        catch(...)
        {
            strActualValue = "Unknown exception thrown";
        }
        strlstActualValues.append(strActualValue);

        try
        {
            SErrResultInfo errResultInfoTmp(
                /* errSource  */ pErrLog->nameSpace(), pErrLog->className(), pErrLog->objectName(), "addEntry",
                /* result     */ EResultListIsFull,
                /* severity   */ EResultSeverityError,
                /* strAddInfo */ "Maximum number of error log entries for severity " + resultSeverity2Str(errResultInfo.getSeverity()) + " reached." );

            strDesiredValue = errResultInfoTmp.toString() + " added to " + pErrLog->objectName();
            strlstDesiredValues.append(strDesiredValue);

            pErrLogEntry = pErrLog->findEntry(errResultInfoTmp);

            if( pErrLogEntry != nullptr )
            {
                strActualValue = errResultInfoTmp.toString() + " added to " + pErrLog->objectName();
            }
            else
            {
                strActualValue = errResultInfoTmp.toString() + " not added to " + pErrLog->objectName();
            }
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        catch(...)
        {
            strActualValue = "Unknown exception thrown";
        }
        strlstActualValues.append(strActualValue);

        // ---------------------------------------------------------------------

        try
        {
            errResultInfo.setSeverity(EResultSeverityInfo);
            errResultInfo.setResult(EResultRequestAborted);
            errResultInfo.setAddErrInfoDscr("Just a test");

            strDesiredValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            strlstDesiredValues.append(strDesiredValue);

            pErrLog->addEntry(errResultInfo);

            pErrLogEntry = pErrLog->findEntry(errResultInfo);

            if( pErrLogEntry != nullptr )
            {
                strActualValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            }
            else
            {
                strActualValue = errResultInfo.toString() + " not added to " + pErrLog->objectName();
            }
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        catch(...)
        {
            strActualValue = "Unknown exception thrown";
        }
        strlstActualValues.append(strActualValue);

        // ---------------------------------------------------------------------

        try
        {
            errResultInfo.setSeverity(EResultSeverityWarning);
            errResultInfo.setResult(EResultRequestAborted);
            errResultInfo.setAddErrInfoDscr("Just a test");

            strDesiredValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            strlstDesiredValues.append(strDesiredValue);

            pErrLog->addEntry(errResultInfo);

            pErrLogEntry = pErrLog->findEntry(errResultInfo);

            if( pErrLogEntry != nullptr )
            {
                strActualValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            }
            else
            {
                strActualValue = errResultInfo.toString() + " not added to " + pErrLog->objectName();
            }
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        catch(...)
        {
            strActualValue = "Unknown exception thrown";
        }
        strlstActualValues.append(strActualValue);

        // ---------------------------------------------------------------------

        try
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestAborted);
            errResultInfo.setAddErrInfoDscr("Just a test");

            strDesiredValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            strlstDesiredValues.append(strDesiredValue);

            pErrLog->addEntry(errResultInfo);

            pErrLogEntry = pErrLog->findEntry(errResultInfo);

            if( pErrLogEntry != nullptr )
            {
                strActualValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            }
            else
            {
                strActualValue = errResultInfo.toString() + " not added to " + pErrLog->objectName();
            }
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        catch(...)
        {
            strActualValue = "Unknown exception thrown";
        }
        strlstActualValues.append(strActualValue);

        // ---------------------------------------------------------------------

        try
        {
            errResultInfo.setSeverity(EResultSeverityCritical);
            errResultInfo.setResult(EResultRequestAborted);
            errResultInfo.setAddErrInfoDscr("Just a test");

            strDesiredValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            strlstDesiredValues.append(strDesiredValue);

            pErrLog->addEntry(errResultInfo);

            pErrLogEntry = pErrLog->findEntry(errResultInfo);

            if( pErrLogEntry != nullptr )
            {
                strActualValue = errResultInfo.toString() + " added to " + pErrLog->objectName();
            }
            else
            {
                strActualValue = errResultInfo.toString() + " not added to " + pErrLog->objectName();
            }
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        catch(...)
        {
            strActualValue = "Unknown exception thrown";
        }
        strlstActualValues.append(strActualValue);

    } // if( pErrLog != nullptr )

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepAddErrLogEntries

//------------------------------------------------------------------------------
void CTest::doTestStepClearErrLog( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstDesiredValues;
    QStringList strlstActualValues;

    QString strDesiredValue;
    QString strActualValue;

    QString strOperation = i_pTestStep->getOperation();

    // Examples for strOperation:
    // - "ZS::Apps::Test::SysErrLog::ErrLog.clear"

    QString strErrLogName;

    if( strOperation.endsWith(".clear",Qt::CaseInsensitive) )
    {
        strErrLogName = strOperation;
        strErrLogName.remove(".clear");
        strErrLogName.remove("(");
        strErrLogName.remove(")");
    }

    CErrLog* pErrLog = nullptr;

    // -------------------------------------------------------------------------

    try
    {
        strDesiredValue = strErrLogName + " Existing";
        strlstDesiredValues.append(strDesiredValue);

        pErrLog = CErrLog::GetInstance(strErrLogName);

        if( pErrLog == nullptr )
        {
            strActualValue = strErrLogName + " Not Existing";
        }
        else // if( pErrLogInst2 != nullptr )
        {
            strActualValue = strErrLogName + " Existing";
        }
    }
    catch( CException& exc )
    {
        strActualValue = exc.getAddErrInfo();
    }
    catch(...)
    {
        strActualValue = "Unknown exception thrown";
    }
    strlstActualValues.append(strActualValue);

    // -------------------------------------------------------------------------

    if( pErrLog != nullptr )
    {
        try
        {
            strDesiredValue = strErrLogName + " Empty";
            strlstDesiredValues.append(strDesiredValue);

            pErrLog->clear();

            if( pErrLog->getEntryCount() > 0 )
            {
                strActualValue = strErrLogName + " Not Empty";
            }
            else // if( pErrLogInst2 != nullptr )
            {
                strActualValue = strErrLogName + " Empty";
            }
        }
        catch( CException& exc )
        {
            strActualValue = exc.getAddErrInfo();
        }
        catch(...)
        {
            strActualValue = "Unknown exception thrown";
        }
        strlstActualValues.append(strActualValue);

    } // if( pErrLog != nullptr )

    // -------------------------------------------------------------------------

    i_pTestStep->setDesiredValues(strlstDesiredValues);
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepClearErrLog
