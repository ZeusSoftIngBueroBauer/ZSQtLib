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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qdir.h>
#include <QtCore/qtimer.h>
#include <QtCore/qtextstream.h>

#include "Test.h"

#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepAdminObj.h"
#include "ZSTest/ZSTestStepAdminObjPool.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysLogFile.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Apps::Test::ZSSysLogFile;


/*******************************************************************************
class CTest
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest( const QString& i_strTestStepsFileName ) :
//------------------------------------------------------------------------------
    ZS::Test::CTest(
        /* strName              */ "ZS::System::LogFile",
        /* strTestStepsFileName */ i_strTestStepsFileName,
        /* strNodeSeparator     */ "\\",
        /* iTestStepInterval_ms */ 0 )
{
    ZS::Test::CTestStep* pTestStep;

    QString strDescription;

    int idxStep = 0;

    strDescription =  "Clear log file directory.";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Clear Log File Directory",
        /* strOperation    */ "clearLogFileDir",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepClearLogFileDir(ZS::Test::CTestStep*)) );
    pTestStep->setDescription(strDescription);

    strDescription =  "Create log files with same and different paths.";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Alloc Log Files",
        /* strOperation    */ "alloc()",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepAlloc(ZS::Test::CTestStep*)) );
    pTestStep->setDescription(strDescription);

    strDescription =  "Add entries to log file exceeding max line and max file count.";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Entries",
        /* strOperation    */ "addEntry()",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntries(ZS::Test::CTestStep*)) );
    pTestStep->setDescription(strDescription);

    strDescription =  "Free all log files.";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Free Log Files",
        /* strOperation    */ "free()",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepFree(ZS::Test::CTestStep*)) );
    pTestStep->setDescription(strDescription);

    strDescription =  "Create log files with same and different paths. Previous log files must be backuped.";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Alloc Log Files",
        /* strOperation    */ "alloc()",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepRealloc(ZS::Test::CTestStep*)) );
    pTestStep->setDescription(strDescription);

    strDescription =  "Add entries to log file exceeding max line and max file count.";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Add Entries",
        /* strOperation    */ "addEntry()",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepAddEntries(ZS::Test::CTestStep*)) );
    pTestStep->setDescription(strDescription);

    strDescription =  "Free all log files.";
    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + " Free Log Files",
        /* strOperation    */ "free()",
        /* pTSGrpParent    */ nullptr,
        /* szDoTestStepFct */ SLOT(doTestStepFree(ZS::Test::CTestStep*)) );
    pTestStep->setDescription(strDescription);

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
public slots: // test step methods (GrpZSSysLogFile)
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepClearLogFileDir( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    QString strAppNameNormalized = QCoreApplication::applicationName();

    // The application name may contain characters which are invalid in file names:
    strAppNameNormalized.remove(":");
    strAppNameNormalized.remove(" ");
    strAppNameNormalized.remove("\\");
    strAppNameNormalized.remove("/");
    strAppNameNormalized.remove("<");
    strAppNameNormalized.remove(">");

    QString strAppLogDir = ZS::System::getAppLogDir("System");

    dir_removeRecursively(strAppLogDir);

    // Desired Values
    //---------------

    strlstDesiredValues << strDesiredValue;
    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Actual Values
    //---------------

    strlstActualValues << strActualValue;
    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepClearLogFileDir

//------------------------------------------------------------------------------
void CTest::doTestStepAlloc( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    QString strAppNameNormalized = QCoreApplication::applicationName();

    // The application name may contain characters which are invalid in file names:
    strAppNameNormalized.remove(":");
    strAppNameNormalized.remove(" ");
    strAppNameNormalized.remove("\\");
    strAppNameNormalized.remove("/");
    strAppNameNormalized.remove("<");
    strAppNameNormalized.remove(">");

    QString strAppLogDir = ZS::System::getAppLogDir("System");

    QString strLogFileSuffix = "log";

    QString strLogFilePathAppDefault = strAppLogDir + "/" + strAppNameNormalized + "." + strLogFileSuffix;

    CLogFile* pLogFileAppDefault1 = CLogFile::Alloc(strLogFilePathAppDefault);
    CLogFile* pLogFileAppDefault2 = CLogFile::Alloc(strLogFilePathAppDefault);

    QString strLogFilePathTest = strAppLogDir + "/Test." + strLogFileSuffix;

    CLogFile* pLogFileTest = CLogFile::Alloc(strLogFilePathTest);

    int iLogFileAppDefaultFileCountMax = 2;
    int iLogFileAppDefaultLineCountMax = 5;
    int iLogFileTestFileCountMax       = 2;
    int iLogFileTestLineCountMax       = 2;

    pLogFileAppDefault1->setSubFileCountMax(iLogFileAppDefaultFileCountMax);
    pLogFileAppDefault1->setSubFileLineCountMax(iLogFileAppDefaultLineCountMax);

    pLogFileAppDefault2->setSubFileCountMax(iLogFileAppDefaultFileCountMax);
    pLogFileAppDefault2->setSubFileLineCountMax(iLogFileAppDefaultLineCountMax);

    pLogFileTest->setSubFileCountMax(iLogFileTestFileCountMax);
    pLogFileTest->setSubFileLineCountMax(iLogFileTestLineCountMax);

    // Desired Values
    //---------------

    strDesiredValue = "pLogFileAppDefault1 == pLogFileAppDefault2";
    strlstDesiredValues.append(strDesiredValue);

    strDesiredValue = "pLogFileAppDefault1 != pLogFileTest";
    strlstDesiredValues.append(strDesiredValue);

    strDesiredValue = "CLogFile::FindFile(AppDefault) == pLogFileAppDefault1";
    strlstDesiredValues.append(strDesiredValue);

    strDesiredValue = "CLogFile::FindFile(Test) == pLogFileTest";
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Actual Values
    //---------------

    if( pLogFileAppDefault1 == pLogFileAppDefault2 )
    {
        strActualValue = "pLogFileAppDefault1 == pLogFileAppDefault2";
        strlstActualValues.append(strActualValue);
    }
    else
    {
        strActualValue = "pLogFileAppDefault1 != pLogFileAppDefault2";
        strlstActualValues.append(strActualValue);
    }

    if( pLogFileAppDefault1 == pLogFileTest )
    {
        strActualValue = "pLogFileAppDefault1 == pLogFileTest";
        strlstActualValues.append(strActualValue);
    }
    else
    {
        strActualValue = "pLogFileAppDefault1 != pLogFileTest";
        strlstActualValues.append(strActualValue);
    }

    if( CLogFile::FindFile(strLogFilePathAppDefault) == pLogFileAppDefault1 )
    {
        strActualValue = "CLogFile::FindFile(AppDefault) == pLogFileAppDefault1";
        strlstActualValues.append(strActualValue);
    }
    else
    {
        strActualValue = "CLogFile::FindFile(AppDefault) != pLogFileAppDefault1";
        strlstActualValues.append(strActualValue);
    }

    if( CLogFile::FindFile(strLogFilePathTest) == pLogFileTest )
    {
        strActualValue = "CLogFile::FindFile(Test) == pLogFileTest";
        strlstActualValues.append(strActualValue);
    }
    else
    {
        strActualValue = "CLogFile::FindFile(Test) != pLogFileTest";
        strlstActualValues.append(strActualValue);
    }

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues << "";
    }

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepAlloc

//------------------------------------------------------------------------------
void CTest::doTestStepRealloc( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    QString strAppNameNormalized = QCoreApplication::applicationName();

    // The application name may contain characters which are invalid in file names:
    strAppNameNormalized.remove(":");
    strAppNameNormalized.remove(" ");
    strAppNameNormalized.remove("\\");
    strAppNameNormalized.remove("/");
    strAppNameNormalized.remove("<");
    strAppNameNormalized.remove(">");

    QString strAppLogDir = ZS::System::getAppLogDir("System");

    QString strLogFileSuffix = "log";

    QString strLogFilePathAppDefault = strAppLogDir + "/" + strAppNameNormalized + "." + strLogFileSuffix;

    CLogFile* pLogFileAppDefault1 = CLogFile::Alloc(strLogFilePathAppDefault);
    CLogFile* pLogFileAppDefault2 = CLogFile::Alloc(strLogFilePathAppDefault);

    QString strLogFilePathTest = strAppLogDir + "/Test." + strLogFileSuffix;

    CLogFile* pLogFileTest = CLogFile::Alloc(strLogFilePathTest);

    int iLogFileAppDefaultFileCountMax = 2;
    int iLogFileAppDefaultLineCountMax = 5;
    int iLogFileTestFileCountMax       = 2;
    int iLogFileTestLineCountMax       = 2;

    pLogFileAppDefault1->setSubFileCountMax(iLogFileAppDefaultFileCountMax);
    pLogFileAppDefault1->setSubFileLineCountMax(iLogFileAppDefaultLineCountMax);

    pLogFileAppDefault2->setSubFileCountMax(iLogFileAppDefaultFileCountMax);
    pLogFileAppDefault2->setSubFileLineCountMax(iLogFileAppDefaultLineCountMax);

    pLogFileTest->setSubFileCountMax(iLogFileTestFileCountMax);
    pLogFileTest->setSubFileLineCountMax(iLogFileTestLineCountMax);

    // Desired Values
    //---------------

    int idxFile;

    strDesiredValue = "pLogFileAppDefault1 == pLogFileAppDefault2";
    strlstDesiredValues.append(strDesiredValue);

    strDesiredValue = "pLogFileAppDefault1 != pLogFileTest";
    strlstDesiredValues.append(strDesiredValue);

    strDesiredValue = "CLogFile::FindFile(AppDefault) == pLogFileAppDefault1";
    strlstDesiredValues.append(strDesiredValue);

    strDesiredValue = "CLogFile::FindFile(Test) == pLogFileTest";
    strlstDesiredValues.append(strDesiredValue);

    strDesiredValue = strAppLogDir + "/Log.bak.exist(): true";
    strlstDesiredValues.append(strDesiredValue);

    for( idxFile = 0; idxFile < iLogFileAppDefaultFileCountMax; ++idxFile )
    {
        QString strLogFileNr = QString("%1").arg(idxFile, 2, 10, QChar('0'));
        QString strLogFileTmp = strAppLogDir + "/Log.bak/" + strAppNameNormalized + strLogFileNr + "." + strLogFileSuffix;

        strlstDesiredValues << strLogFileTmp + ".exist(): true";
    }

    for( idxFile = 0; idxFile < iLogFileTestFileCountMax; ++idxFile )
    {
        QString strLogFileNr = QString("%1").arg(idxFile, 2, 10, QChar('0'));
        QString strLogFileTmp = strAppLogDir + "/Log.bak/Test" + strLogFileNr + "." + strLogFileSuffix;

        strlstDesiredValues << strLogFileTmp + ".exist(): true";
    }

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Actual Values
    //---------------

    if( pLogFileAppDefault1 == pLogFileAppDefault2 )
    {
        strActualValue = "pLogFileAppDefault1 == pLogFileAppDefault2";
        strlstActualValues.append(strActualValue);
    }
    else
    {
        strActualValue = "pLogFileAppDefault1 != pLogFileAppDefault2";
        strlstActualValues.append(strActualValue);
    }

    if( pLogFileAppDefault1 == pLogFileTest )
    {
        strActualValue = "pLogFileAppDefault1 == pLogFileTest";
        strlstActualValues.append(strActualValue);
    }
    else
    {
        strActualValue = "pLogFileAppDefault1 != pLogFileTest";
        strlstActualValues.append(strActualValue);
    }

    if( CLogFile::FindFile(strLogFilePathAppDefault) == pLogFileAppDefault1 )
    {
        strActualValue = "CLogFile::FindFile(AppDefault) == pLogFileAppDefault1";
        strlstActualValues.append(strActualValue);
    }
    else
    {
        strActualValue = "CLogFile::FindFile(AppDefault) != pLogFileAppDefault1";
        strlstActualValues.append(strActualValue);
    }

    if( CLogFile::FindFile(strLogFilePathTest) == pLogFileTest )
    {
        strActualValue = "CLogFile::FindFile(Test) == pLogFileTest";
        strlstActualValues.append(strActualValue);
    }
    else
    {
        strActualValue = "CLogFile::FindFile(Test) != pLogFileTest";
        strlstActualValues.append(strActualValue);
    }

    QString strAppLogDirBak = ZS::System::getAppLogDir("System") + "/Log.bak";

    QDir dirAppLogBak(strAppLogDirBak);

    if( dirAppLogBak.exists() )
    {
        strlstActualValues << strAppLogDirBak + ".exist(): true";
    }
    else
    {
        strlstActualValues << strAppLogDirBak + ".exist(): false";
    }

    for( idxFile = 0; idxFile < iLogFileAppDefaultFileCountMax; ++idxFile )
    {
        QString strLogFileNr = QString("%1").arg(idxFile, 2, 10, QChar('0'));
        QString strLogFileTmp = strAppLogDir + "/Log.bak/" + strAppNameNormalized + strLogFileNr + "." + strLogFileSuffix;

        QFile fileLogFileAppDefault(strLogFileTmp);

        if( fileLogFileAppDefault.exists() )
        {
            strlstActualValues << strLogFileTmp + ".exist(): true";
        }
        else
        {
            strlstActualValues << strLogFileTmp + ".exist(): false";
        }
    }

    for( idxFile = 0; idxFile < iLogFileTestFileCountMax; ++idxFile )
    {
        QString strLogFileNr = QString("%1").arg(idxFile, 2, 10, QChar('0'));
        QString strLogFileTmp = strAppLogDir + "/Log.bak/Test" + strLogFileNr + "." + strLogFileSuffix;

        QFile fileLogFileAppDefault(strLogFileTmp);

        if( fileLogFileAppDefault.exists() )
        {
            strlstActualValues << strLogFileTmp + ".exist(): true";
        }
        else
        {
            strlstActualValues << strLogFileTmp + ".exist(): false";
        }
    }

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues << "";
    }

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepRealloc

//------------------------------------------------------------------------------
void CTest::doTestStepAddEntries( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    QString strAppNameNormalized = QCoreApplication::applicationName();

    // The application name may contain characters which are invalid in file names:
    strAppNameNormalized.remove(":");
    strAppNameNormalized.remove(" ");
    strAppNameNormalized.remove("\\");
    strAppNameNormalized.remove("/");
    strAppNameNormalized.remove("<");
    strAppNameNormalized.remove(">");

    QString strAppLogDir = ZS::System::getAppLogDir("System");

    QString strLogFileSuffix = "log";

    QString strLogFilePathAppDefault = strAppLogDir + "/" + strAppNameNormalized + "." + strLogFileSuffix;
    QString strLogFilePathTest = strAppLogDir + "/Test." + strLogFileSuffix;

    CLogFile* pLogFileAppDefault1 = CLogFile::FindFile(strLogFilePathAppDefault);
    CLogFile* pLogFileAppDefault2 = CLogFile::FindFile(strLogFilePathAppDefault);
    CLogFile* pLogFileTest = CLogFile::FindFile(strLogFilePathTest);

    int iLogFileAppDefaultFileCountMax = pLogFileAppDefault1->getSubFileCountMax();
    int iLogFileAppDefaultLineCountMax = pLogFileAppDefault1->getSubFileLineCountMax();
    int iLogFileTestFileCountMax       = pLogFileTest->getSubFileCountMax();
    int iLogFileTestLineCountMax       = pLogFileTest->getSubFileLineCountMax();

    QString strLogEntry;
    int     iLineNr;
    int     idxFile;
    int     iLines;

    iLines = (iLogFileAppDefaultFileCountMax+1) * iLogFileAppDefaultLineCountMax + 1;

    for( iLineNr = 1; iLineNr <= iLines; ++iLineNr )
    {
        strLogEntry = strAppNameNormalized + ".Line" + QString::number(iLineNr);

        if( (iLineNr % 2) != 0 )
        {
            pLogFileAppDefault1->addEntry(strLogEntry);
        }
        else
        {
            pLogFileAppDefault2->addEntry(strLogEntry);
        }
    }

    pLogFileAppDefault1->close();
    pLogFileAppDefault2->close();

    iLines = (iLogFileTestFileCountMax+1) * iLogFileTestLineCountMax + 1;

    for( iLineNr = 1; iLineNr <= iLines; ++iLineNr )
    {
        strLogEntry = "Test.Line" + QString::number(iLineNr);
        pLogFileTest->addEntry(strLogEntry);
    }

    pLogFileTest->close();

    // Desired Values
    //---------------

    // Number of entries which could be stored in all sub log files instead of latest (current) sub log file:
    int iLinesFilledSubFiles;
    // Number of entries which are stored in latest (current) sub log file:
    int iLinesCurrentSubFile;
    // Number of lines actually stored in the log file (sum over all sub log files):
    int iLinesLogFile;

    // (iLogFileAppDefaultFileCountMax+1) * iLogFileAppDefaultLineCountMax + 1 = (2+1) * 5 + 1 = 16
    // 16 lines have been written to ZSAppTestZSSysLogFile.
    // 2 files should exist: ZSAppTestZSSysLogFile00 and ZSAppTestZSSysLogFile01.
    // The older file "01" should contain "LineCountMax" entries (=5).
    // The newest file "00" should contain just one entry.

    // Number of lines which have been written:
    iLines = (iLogFileAppDefaultFileCountMax+1) * iLogFileAppDefaultLineCountMax + 1;

    // Number of entries which could be stored in all sub log files instead of latest (current) sub log file:
    iLinesFilledSubFiles = (iLogFileAppDefaultFileCountMax-1) * iLogFileAppDefaultLineCountMax; // 1 * 5 = 5

    // Number of entries which are stored in latest (current) sub log file:
    iLinesCurrentSubFile = iLines % iLinesFilledSubFiles; // 16 % 5 = 1

    // Number of lines actually stored in the log file (sum over all sub log files):
    iLinesLogFile = iLinesFilledSubFiles + iLinesCurrentSubFile; // 5 + 1 = 6;

    for( iLineNr = (iLines - iLinesLogFile + 1); iLineNr <= iLines; ++iLineNr )
    {
        strLogEntry = strAppNameNormalized + ".Line" + QString::number(iLineNr);
        strlstDesiredValues << strLogEntry;
    }

    // (iLogFileTestFileCountMax+1) * iLogFileTestLineCountMax + 1 = (2+1) * 2 + 1 = 7
    // 7 lines have been written to ZSAppTestZSSysLogFile.
    // 2 files should exist: Test00 and Test01.
    // The older file "01" should contain "LineCountMax" entries (=2).
    // The newest file "00" should contain just one entry.

    // Number of lines which have been written:
    iLines = (iLogFileTestFileCountMax+1) * iLogFileTestLineCountMax + 1;

    // Number of entries which could be stored in all sub log files instead of latest (current) sub log file:
    iLinesFilledSubFiles = (iLogFileTestFileCountMax-1) * iLogFileTestLineCountMax; // 1 * 5 = 5

    // Number of entries which are stored in latest (current) sub log file:
    iLinesCurrentSubFile = iLines % iLinesFilledSubFiles; // 16 % 5 = 1

    // Number of lines actually stored in the log file (sum over all sub log files):
    iLinesLogFile = iLinesFilledSubFiles + iLinesCurrentSubFile; // 5 + 1 = 6;

    for( iLineNr = (iLines - iLinesLogFile + 1); iLineNr <= iLines; ++iLineNr )
    {
        strLogEntry = "Test.Line" + QString::number(iLineNr);
        strlstDesiredValues << strLogEntry;
    }

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Actual Values
    //---------------

    for( idxFile = iLogFileAppDefaultFileCountMax-1; idxFile >= 0; --idxFile )
    {
        QString strLogFileNr = QString("%1").arg(idxFile, 2, 10, QChar('0'));
        QString strLogFileTmp = strAppLogDir + "/" + strAppNameNormalized + strLogFileNr + "." + strLogFileSuffix;

        QFile fileLogFileAppDefault(strLogFileTmp);

        if( !fileLogFileAppDefault.open(QIODevice::ReadOnly|QIODevice::Text) )
        {
            strlstActualValues << "Could not open file " + strLogFileTmp;
        }
        else
        {
            QTextStream txtstrmLogFileAppDefault(&fileLogFileAppDefault);

            while( !txtstrmLogFileAppDefault.atEnd() )
            {
                strlstActualValues << txtstrmLogFileAppDefault.readLine();
            }
        }
    }

    for( idxFile = iLogFileTestFileCountMax-1; idxFile >= 0; --idxFile )
    {
        QString strLogFileNr = QString("%1").arg(idxFile, 2, 10, QChar('0'));
        QString strLogFileTmp = strAppLogDir + "/Test" + strLogFileNr + "." + strLogFileSuffix;

        QFile fileLogFileTest(strLogFileTmp);

        if( !fileLogFileTest.open(QIODevice::ReadOnly|QIODevice::Text) )
        {
            strlstActualValues << "Could not open file " + strLogFileTmp;
        }
        else
        {
            QTextStream txtstrmLogFileTest(&fileLogFileTest);

            while( !txtstrmLogFileTest.atEnd() )
            {
                strlstActualValues << txtstrmLogFileTest.readLine();
            }
        }
    }

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues << "";
    }

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepAddEntries

//------------------------------------------------------------------------------
void CTest::doTestStepFree( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strDesiredValue;
    QStringList strlstDesiredValues;
    QString     strActualValue;
    QStringList strlstActualValues;

    QString strAppNameNormalized = QCoreApplication::applicationName();

    // The application name may contain characters which are invalid in file names:
    strAppNameNormalized.remove(":");
    strAppNameNormalized.remove(" ");
    strAppNameNormalized.remove("\\");
    strAppNameNormalized.remove("/");
    strAppNameNormalized.remove("<");
    strAppNameNormalized.remove(">");

    QString strAppLogDir = ZS::System::getAppLogDir("System");

    QString strLogFileSuffix = "log";

    QString strLogFilePathAppDefault = strAppLogDir + "/" + strAppNameNormalized + "." + strLogFileSuffix;

    CLogFile::Free(strLogFilePathAppDefault);
    CLogFile::Free(strLogFilePathAppDefault);

    QString strLogFilePathTest = strAppLogDir + "/Test." + strLogFileSuffix;

    CLogFile* pLogFileTest = CLogFile::FindFile(strLogFilePathTest);

    CLogFile::Free(pLogFileTest);

    // Desired Values
    //---------------

    strDesiredValue = "LogFilesCount == 0";
    strlstDesiredValues.append(strDesiredValue);

    i_pTestStep->setDesiredValues(strlstDesiredValues);

    // Actual Values
    //---------------

    int iLogFilesCount = CLogFile::GetFilesCount();

    if( iLogFilesCount == 0 )
    {
        strActualValue = "LogFilesCount == 0";
        strlstActualValues.append(strActualValue);
    }
    else
    {
        strActualValue = "LogFilesCount != 0 (=" + QString::number(iLogFilesCount) + ")";
        strlstActualValues.append(strActualValue);
    }

    // Please note that to finish a test step the list of actual values may not be empty.
    if( strlstActualValues.size() == 0 )
    {
        strlstActualValues << "";
    }

    i_pTestStep->setActualValues(strlstActualValues);

} // doTestStepFree
