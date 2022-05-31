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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qdir.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qtimer.h>
#include <QtCore/qtextstream.h>

#include "Test.h"

#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepIdxTreeEntry.h"
#include "ZSTest/ZSTestStepIdxTree.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysErrLog.h"
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
CTest::CTest() :
//------------------------------------------------------------------------------
    ZS::Test::CTest("ZS::System::LogFile"),
    m_iInitialLogFilesCount(0)
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

    // Recall test step settings
    //--------------------------

    m_iInitialLogFilesCount = CLogFile::GetFilesCount();

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
public slots: // test step methods (GrpZSSysLogFile)
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepClearLogFileDir( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

    QString strAppNameNormalized = QCoreApplication::applicationName();

    // The application name may contain characters which are invalid in file names:
    strAppNameNormalized.remove(":");
    strAppNameNormalized.remove(" ");
    strAppNameNormalized.remove("\\");
    strAppNameNormalized.remove("/");
    strAppNameNormalized.remove("<");
    strAppNameNormalized.remove(">");

    QString strAppLogDir = ZS::System::getAppLogDir("System");

    QDir dirAppLog(strAppLogDir);

    dirAppLog.removeRecursively();

    // Expected Values
    //---------------

    strlstExpectedValues << strExpectedValue;
    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Result Values
    //---------------

    strlstResultValues << strResultValue;
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepClearLogFileDir

//------------------------------------------------------------------------------
void CTest::doTestStepAlloc( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

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

    // Expected Values
    //---------------

    strExpectedValue = "pLogFileAppDefault1 == pLogFileAppDefault2";
    strlstExpectedValues.append(strExpectedValue);

    strExpectedValue = "pLogFileAppDefault1 != pLogFileTest";
    strlstExpectedValues.append(strExpectedValue);

    strExpectedValue = "CLogFile::FindFile(AppDefault) == pLogFileAppDefault1";
    strlstExpectedValues.append(strExpectedValue);

    strExpectedValue = "CLogFile::FindFile(Test) == pLogFileTest";
    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Result Values
    //---------------

    if( pLogFileAppDefault1 == pLogFileAppDefault2 )
    {
        strResultValue = "pLogFileAppDefault1 == pLogFileAppDefault2";
        strlstResultValues.append(strResultValue);
    }
    else
    {
        strResultValue = "pLogFileAppDefault1 != pLogFileAppDefault2";
        strlstResultValues.append(strResultValue);
    }

    if( pLogFileAppDefault1 == pLogFileTest )
    {
        strResultValue = "pLogFileAppDefault1 == pLogFileTest";
        strlstResultValues.append(strResultValue);
    }
    else
    {
        strResultValue = "pLogFileAppDefault1 != pLogFileTest";
        strlstResultValues.append(strResultValue);
    }

    if( CLogFile::FindFile(strLogFilePathAppDefault) == pLogFileAppDefault1 )
    {
        strResultValue = "CLogFile::FindFile(AppDefault) == pLogFileAppDefault1";
        strlstResultValues.append(strResultValue);
    }
    else
    {
        strResultValue = "CLogFile::FindFile(AppDefault) != pLogFileAppDefault1";
        strlstResultValues.append(strResultValue);
    }

    if( CLogFile::FindFile(strLogFilePathTest) == pLogFileTest )
    {
        strResultValue = "CLogFile::FindFile(Test) == pLogFileTest";
        strlstResultValues.append(strResultValue);
    }
    else
    {
        strResultValue = "CLogFile::FindFile(Test) != pLogFileTest";
        strlstResultValues.append(strResultValue);
    }

    // Please note that to finish a test step the list of result values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues << "";
    }

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepAlloc

//------------------------------------------------------------------------------
void CTest::doTestStepRealloc( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

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

    // Expected Values
    //---------------

    int idxFile;

    strExpectedValue = "pLogFileAppDefault1 == pLogFileAppDefault2";
    strlstExpectedValues.append(strExpectedValue);

    strExpectedValue = "pLogFileAppDefault1 != pLogFileTest";
    strlstExpectedValues.append(strExpectedValue);

    strExpectedValue = "CLogFile::FindFile(AppDefault) == pLogFileAppDefault1";
    strlstExpectedValues.append(strExpectedValue);

    strExpectedValue = "CLogFile::FindFile(Test) == pLogFileTest";
    strlstExpectedValues.append(strExpectedValue);

    strExpectedValue = strAppLogDir + "/Log.bak.exist(): true";
    strlstExpectedValues.append(strExpectedValue);

    for( idxFile = 0; idxFile < iLogFileAppDefaultFileCountMax; ++idxFile )
    {
        QString strLogFileNr = QString("%1").arg(idxFile, 2, 10, QChar('0'));
        QString strLogFileTmp = strAppLogDir + "/Log.bak/" + strAppNameNormalized + strLogFileNr + "." + strLogFileSuffix;

        strlstExpectedValues << strLogFileTmp + ".exist(): true";
    }

    for( idxFile = 0; idxFile < iLogFileTestFileCountMax; ++idxFile )
    {
        QString strLogFileNr = QString("%1").arg(idxFile, 2, 10, QChar('0'));
        QString strLogFileTmp = strAppLogDir + "/Log.bak/Test" + strLogFileNr + "." + strLogFileSuffix;

        strlstExpectedValues << strLogFileTmp + ".exist(): true";
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Result Values
    //---------------

    if( pLogFileAppDefault1 == pLogFileAppDefault2 )
    {
        strResultValue = "pLogFileAppDefault1 == pLogFileAppDefault2";
        strlstResultValues.append(strResultValue);
    }
    else
    {
        strResultValue = "pLogFileAppDefault1 != pLogFileAppDefault2";
        strlstResultValues.append(strResultValue);
    }

    if( pLogFileAppDefault1 == pLogFileTest )
    {
        strResultValue = "pLogFileAppDefault1 == pLogFileTest";
        strlstResultValues.append(strResultValue);
    }
    else
    {
        strResultValue = "pLogFileAppDefault1 != pLogFileTest";
        strlstResultValues.append(strResultValue);
    }

    if( CLogFile::FindFile(strLogFilePathAppDefault) == pLogFileAppDefault1 )
    {
        strResultValue = "CLogFile::FindFile(AppDefault) == pLogFileAppDefault1";
        strlstResultValues.append(strResultValue);
    }
    else
    {
        strResultValue = "CLogFile::FindFile(AppDefault) != pLogFileAppDefault1";
        strlstResultValues.append(strResultValue);
    }

    if( CLogFile::FindFile(strLogFilePathTest) == pLogFileTest )
    {
        strResultValue = "CLogFile::FindFile(Test) == pLogFileTest";
        strlstResultValues.append(strResultValue);
    }
    else
    {
        strResultValue = "CLogFile::FindFile(Test) != pLogFileTest";
        strlstResultValues.append(strResultValue);
    }

    QString strAppLogDirBak = ZS::System::getAppLogDir("System") + "/Log.bak";

    QDir dirAppLogBak(strAppLogDirBak);

    if( dirAppLogBak.exists() )
    {
        strlstResultValues << strAppLogDirBak + ".exist(): true";
    }
    else
    {
        strlstResultValues << strAppLogDirBak + ".exist(): false";
    }

    for( idxFile = 0; idxFile < iLogFileAppDefaultFileCountMax; ++idxFile )
    {
        QString strLogFileNr = QString("%1").arg(idxFile, 2, 10, QChar('0'));
        QString strLogFileTmp = strAppLogDir + "/Log.bak/" + strAppNameNormalized + strLogFileNr + "." + strLogFileSuffix;

        QFile fileLogFileAppDefault(strLogFileTmp);

        if( fileLogFileAppDefault.exists() )
        {
            strlstResultValues << strLogFileTmp + ".exist(): true";
        }
        else
        {
            strlstResultValues << strLogFileTmp + ".exist(): false";
        }
    }

    for( idxFile = 0; idxFile < iLogFileTestFileCountMax; ++idxFile )
    {
        QString strLogFileNr = QString("%1").arg(idxFile, 2, 10, QChar('0'));
        QString strLogFileTmp = strAppLogDir + "/Log.bak/Test" + strLogFileNr + "." + strLogFileSuffix;

        QFile fileLogFileAppDefault(strLogFileTmp);

        if( fileLogFileAppDefault.exists() )
        {
            strlstResultValues << strLogFileTmp + ".exist(): true";
        }
        else
        {
            strlstResultValues << strLogFileTmp + ".exist(): false";
        }
    }

    // Please note that to finish a test step the list of result values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues << "";
    }

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepRealloc

//------------------------------------------------------------------------------
void CTest::doTestStepAddEntries( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

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

    // Expected Values
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
        strlstExpectedValues << strLogEntry;
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
        strlstExpectedValues << strLogEntry;
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Result Values
    //---------------

    for( idxFile = iLogFileAppDefaultFileCountMax-1; idxFile >= 0; --idxFile )
    {
        QString strLogFileNr = QString("%1").arg(idxFile, 2, 10, QChar('0'));
        QString strLogFileTmp = strAppLogDir + "/" + strAppNameNormalized + strLogFileNr + "." + strLogFileSuffix;

        QFile fileLogFileAppDefault(strLogFileTmp);

        if( !fileLogFileAppDefault.open(QIODevice::ReadOnly|QIODevice::Text) )
        {
            strlstResultValues << "Could not open file " + strLogFileTmp;
        }
        else
        {
            QTextStream txtstrmLogFileAppDefault(&fileLogFileAppDefault);

            while( !txtstrmLogFileAppDefault.atEnd() )
            {
                strlstResultValues << txtstrmLogFileAppDefault.readLine();
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
            strlstResultValues << "Could not open file " + strLogFileTmp;
        }
        else
        {
            QTextStream txtstrmLogFileTest(&fileLogFileTest);

            while( !txtstrmLogFileTest.atEnd() )
            {
                strlstResultValues << txtstrmLogFileTest.readLine();
            }
        }
    }

    // Please note that to finish a test step the list of result values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues << "";
    }

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepAddEntries

//------------------------------------------------------------------------------
void CTest::doTestStepFree( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString     strExpectedValue;
    QStringList strlstExpectedValues;
    QString     strResultValue;
    QStringList strlstResultValues;

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

    // Expected Values
    //---------------

    strExpectedValue = "LogFilesCount == " + QString::number(m_iInitialLogFilesCount);
    strlstExpectedValues.append(strExpectedValue);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Result Values
    //---------------

    int iLogFilesCount = CLogFile::GetFilesCount();

    strResultValue = "LogFilesCount == " + QString::number(iLogFilesCount);
    strlstResultValues.append(strResultValue);

    // Please note that to finish a test step the list of result values may not be empty.
    if( strlstResultValues.size() == 0 )
    {
        strlstResultValues << "";
    }

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepFree
