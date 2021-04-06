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
#include <QtCore/qdatetime.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qmutex.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qthread.h>
#include <QtXml/qdom.h>

#include "ZSSys/ZSSysTrcMthFile.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;


/*******************************************************************************
class CTrcMthFile
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

QMutex CTrcMthFile::s_mtx(QMutex::Recursive);
QMap<QString, CTrcMthFile*> CTrcMthFile::s_mapTrcMthFiles;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns a reference to an instance of the class.

    Checks whether a trace method file instance with the desired absolute path
    (including file name and suffix) already exist.
    If the file name is already used, a reference to the already used trace
    method file instance is returned and a reference counter for this instance
    is incremented.
    If the file name is not yet used a new trace method file instance is created.

    \param i_strAbsFilePath [in] Absolute path including the file name and suffix.
    \return Pointer to trace method file instance.
*/
CTrcMthFile* CTrcMthFile::Alloc( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    // Trace method files may be accessed from within different thread contexts and
    // therefore accessing the files must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( i_strAbsFilePath.isEmpty() )
    {
        return nullptr;
    }

    CTrcMthFile* pTrcMthFile = FindFile(i_strAbsFilePath);

    // If the file name is already used ..
    if( pTrcMthFile != nullptr )
    {
        pTrcMthFile->incrementRefCount();;
    }
    // If the file name is not yet used ..
    else
    {
        // .. create a new log file and add it to the list of files:
        pTrcMthFile = new CTrcMthFile(i_strAbsFilePath);
        s_mapTrcMthFiles[i_strAbsFilePath] = pTrcMthFile;
        pTrcMthFile->incrementRefCount();
    }

    return pTrcMthFile;

} // Alloc

//------------------------------------------------------------------------------
/*! Frees the trace method file instance.

    The reference counter of the trace method file is decremented.
    If the reference counter becomes 0 the instance will be deleted.
    Please note that even if the reference counter is still greater than 0
    the trace method file instance should no longer be used be the caller of
    this method but the reference should be invalided (set to nullptr).
    If a trace method file with the specified file name does not exist the
    method just returns.

    \param i_strAbsFilePath [in] Absolute path including the file name and suffix.
*/
void CTrcMthFile::Free( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    // Trace method files may be accessed from within different thread contexts and
    // therefore accessing the files must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    CTrcMthFile* pTrcMthFile = FindFile(i_strAbsFilePath);

    if( pTrcMthFile != nullptr )
    {
        if( pTrcMthFile->refCount() > 0 )
        {
            pTrcMthFile->decrementRefCount();

            if( pTrcMthFile->refCount() == 0 )
            {
                s_mapTrcMthFiles.remove(pTrcMthFile->absoluteFilePath());
                delete pTrcMthFile;
                pTrcMthFile = nullptr;
            }
        }
        else
        {
            throw CException(__FILE__, __LINE__, EResultInternalProgramError, "Trace Method file was not allocated");
        }
    }

} // Free

//------------------------------------------------------------------------------
/*! Frees the trace method file instance.

    The reference counter of the instance is decremented. If the reference
    counter becomes 0 the trace method file instance will be deleted.
    Please note that even if the reference counter is still greater than 0
    the trace method file instance should no longer be used be the caller
    of this method but the reference should be invalided (set to nullptr).

    \param i_pTrcMthFile [in] Pointer to trace method file instance to be freed.
*/
void CTrcMthFile::Free( CTrcMthFile* i_pTrcMthFile )
//------------------------------------------------------------------------------
{
    // Trace method files may be accessed from within different thread contexts and
    // therefore accessing the files must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( i_pTrcMthFile == nullptr )
    {
        return;
    }

    Free( i_pTrcMthFile->absoluteFilePath() );

} // Free

//------------------------------------------------------------------------------
/*! Checks whether a trace method file with the specified absolute file path exists.

    \param i_strAbsFilePath [in] Absolute path including the file name and suffix.
    \return Pointer to trace method file instance.
*/
CTrcMthFile* CTrcMthFile::FindFile( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    // Trace method files may be accessed from within different thread contexts and
    // therefore accessing the files must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    CTrcMthFile* pTrcMthFile = nullptr;

    if( s_mapTrcMthFiles.contains(i_strAbsFilePath) )
    {
        pTrcMthFile = s_mapTrcMthFiles[i_strAbsFilePath];
    }

    return pTrcMthFile;

} // FindFile

//------------------------------------------------------------------------------
/*! Returns the number of trace method file instances.

    The reference counter is ignored by this method. This means that even
    if "alloc" has been called several times with the same file name the
    number of trace method file instances may be 1.

    \return Number of trace method file instances.
*/
int CTrcMthFile::GetFilesCount()
//------------------------------------------------------------------------------
{
    return s_mapTrcMthFiles.size();
}

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs a trace method file instance.

    Trace method file instances may only be constructed using the static alloc method.

    \param i_strAbsFilePath [in] Absolute path including the file name and suffix.
*/
CTrcMthFile::CTrcMthFile( const QString& i_strAbsFilePath ) :
//------------------------------------------------------------------------------
    m_bEnabled(true),
    m_pLogFile(nullptr),
    m_iRefCount(0),
    m_hashThreadCallDepths()
{
    m_pLogFile = CLogFile::Alloc(i_strAbsFilePath);

} // ctor

//------------------------------------------------------------------------------
/*! Destroys the trace method file instance closing the current active sub log file.

    Trace method file instances may only be destroyed using the static free method.
*/
CTrcMthFile::~CTrcMthFile()
//------------------------------------------------------------------------------
{
    CLogFile::Free(m_pLogFile);

    m_bEnabled = false;
    m_pLogFile = nullptr;
    m_iRefCount = 0;
    //m_hashThreadCallDepths.clear();

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Closes the current active sub log file.

    Please note that if an addEntry call follows the log file will be opened again.
*/
void CTrcMthFile::close()
//------------------------------------------------------------------------------
{
    if( m_pLogFile != nullptr )
    {
        m_pLogFile->close();
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! This method may be used to temporarily disable output to the log file.

    \param i_bEnabled [in] Flag to enable or disable the output to the log file.
*/
void CTrcMthFile::setEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    m_bEnabled = i_bEnabled;
}

//------------------------------------------------------------------------------
/*! Returns the flag whehter output to the log file has been temporarily disabled.

    \return Flag whether output is enabled or disabled.
*/
bool CTrcMthFile::isEnabled() const
//------------------------------------------------------------------------------
{
    return m_bEnabled;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the absolute path of the log file including the file name and suffix.

    Example:

        QString strAbsFilePath = "C:/ProgramData/ZeusSoft/TestTrcMthLog/TestTrcMthLog.log";
        CTrcMthFile* pTrcMthFile = CTrcMthFile::Alloc(strAbsPath);
        QString strAbsFilePath = pTrcMthFile->absoluteFilePath();  // strBaseName = "C:/ProgramData/ZeusSoft/TestTrcMthLog/TestTrcMthLog.log"

    \return Absolute path of the log file including the file name and suffix.
*/
QString CTrcMthFile::absoluteFilePath() const
//------------------------------------------------------------------------------
{
    if( m_pLogFile != nullptr )
    {
        return m_pLogFile->absoluteFilePath();
    }
    return "";
}

//------------------------------------------------------------------------------
/*! Returns the complete base name of the log file without the path.

    The complete base name consists of all characters in the file up
    to (but not including) the last '.' character.

    Example:

        QString strAbsFilePath = "C:/ProgramData/ZeusSoft/TestTrcMthLog/TestTrcMthLog.log";
        CTrcMthFile* pTrcMthFile = CTrcMthFile::Alloc(strAbsPath);
        QString strBaseName = pTrcMthFile->completeBaseName();  // strBaseName = "TestTrcMthLog"

    \return Complete base name of the file without the path.
*/
QString CTrcMthFile::completeBaseName() const
//------------------------------------------------------------------------------
{
    if( m_pLogFile != nullptr )
    {
        return m_pLogFile->completeBaseName();
    }
    return "";
}

//------------------------------------------------------------------------------
/*! Returns the log file's absolute path.

    The absolute path doesn't include the file name.

    Example:

        QString strAbsFilePath = "C:/ProgramData/ZeusSoft/TestTrcMthLog/TestTrcMthLog.log";
        CTrcMthFile* pTrcMthFile = CTrcMthFile::Alloc(strAbsPath);
        QString strAbsPath = pTrcMthFile->absolutePath();  // strAbsPath = "C:/ProgramData/ZeusSoft/TestTrcMthLog"

    \return Absolute path of the log file including the file name and suffix.
*/
QString CTrcMthFile::absolutePath() const
//------------------------------------------------------------------------------
{
    if( m_pLogFile != nullptr )
    {
        return m_pLogFile->absolutePath();
    }
    return "";
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the time interval for the auto save timer.

    If greater than 0 a timer is started which automatically saves the
    currently active sub log file within the defined intervals.
    Please note that an following addEntry call opens the log file again.

    \param i_iInterval_ms [in] Timer interval in milli seconds.
*/
void CTrcMthFile::setAutoSaveInterval( int i_iInterval_ms )
//------------------------------------------------------------------------------
{
    if( m_pLogFile != nullptr )
    {
        m_pLogFile->setAutoSaveInterval(i_iInterval_ms);
    }
}

//------------------------------------------------------------------------------
/*! Returns the time interval in milli seconds of the auto save timer.

    \return Timer interval in milli seconds.
*/
int CTrcMthFile::getAutoSaveInterval() const
//------------------------------------------------------------------------------
{
    int iInterval = 0;

    if( m_pLogFile != nullptr )
    {
        iInterval = m_pLogFile->getAutoSaveInterval();
    }
    return iInterval;
}

//------------------------------------------------------------------------------
/*! Defines whether the current active log file should be immediately closed
    after adding an entry.

    This may help finding the reason for abnormal program terminations. But
    please note that if the flag is set this will significantly slow down
    the execution of the program.

    \param i_bCloseFile [in] Flag defining whether the log file should be
                             immediately closed after adding an entry.
*/
void CTrcMthFile::setCloseFileAfterEachWrite( bool i_bCloseFile )
//------------------------------------------------------------------------------
{
    if( m_pLogFile != nullptr )
    {
        m_pLogFile->setCloseFileAfterEachWrite(i_bCloseFile);
    }
}

//------------------------------------------------------------------------------
/*! Returns whether the current active log file is immediately closed
    after adding an entry.

    \return Flag defining whether the log file should be immediately closed
            after adding an entry.
*/
bool CTrcMthFile::getCloseFileAfterEachWrite() const
//------------------------------------------------------------------------------
{
    bool bClose = false;

    if( m_pLogFile != nullptr )
    {
        bClose = m_pLogFile->getCloseFileAfterEachWrite();
    }
    return bClose;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the maximum number of sub log files.

    The maximum number of sub log file defaults to 2 which should be sufficient
    in most cases.

    If the maximum number of lines per sub file is reached, the current file is closed,
    saved under a different file name and a new, current log file is opened.

    If the maximum number of sub log files is reachead the oldest sub log file is deleted,
    the more recent sub log files are renamed increasing their counter and then the
    current log file is stored in counter file 1.

    If the maximum number of backup files is set to 3, there are e.g. the following
    log files on the hard disk:

    - "TestApp00.log" ... currently active log file (SubFile00)
    - "TestApp01.log" ... newest backup sub log file
    - "TestApp02.log" ... oldest backup sub log file

    \param i_iCountMax [in] Maximum number of sub log files.
*/
void CTrcMthFile::setSubFileCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    if( m_pLogFile != nullptr )
    {
        m_pLogFile->setSubFileCountMax(i_iCountMax);
    }
}

//------------------------------------------------------------------------------
/*! Returns the maximum number of sub log files used.

    \return Maximum number of used sub log files.
*/
int CTrcMthFile::getSubFileCountMax() const
//------------------------------------------------------------------------------
{
    int iFileCount = 0;

    if( m_pLogFile != nullptr )
    {
        iFileCount = m_pLogFile->getSubFileCountMax();
    }
    return iFileCount;
}

//------------------------------------------------------------------------------
/*! Sets the maximum number of lines for a sub log file.

    If the maximum number of lines per sub file is reached, the current file is closed,
    saved under a different file name and a new, current log file is opened.

    If the maximum number of sub log files is reachead the oldest sub log file is deleted,
    the more recent sub log files are renamed increasing their counter and then the
    current log file is stored in counter file 1.

    If the maximum number of backup files is set to 3, there are e.g. the following
    log files on the hard disk:

    - "TestApp00.log" ... currently active log file (SubFile00)
    - "TestApp01.log" ... newest backup sub log file
    - "TestApp02.log" ... oldest backup sub log file

    \param i_iCountMax [in] Maximum number of lines for a sub log file.
*/
void CTrcMthFile::setSubFileLineCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    if( m_pLogFile != nullptr )
    {
        m_pLogFile->setSubFileLineCountMax(i_iCountMax);
    }
}

//------------------------------------------------------------------------------
/*! Returns the maximum number of lines used for sub log files.

    \return Maximum number of lines for a sub log file.
*/
int CTrcMthFile::getSubFileLineCountMax() const
//------------------------------------------------------------------------------
{
    int iLineCount = 0;

    if( m_pLogFile != nullptr )
    {
        iLineCount = m_pLogFile->getSubFileLineCountMax();
    }
    return iLineCount;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Traces entry of a method.

    \param i_strMethod [in] Name of the method to be logged.
    \param i_strMethodInArgs [in] Input arguments.
*/
void CTrcMthFile::traceMethodEnter(
    const QString& i_strMethod,
    const QString& i_strMethodInArgs )
//------------------------------------------------------------------------------
{
    addEntry(
        currentThreadName(),
        QDateTime::currentDateTime(),
        Time::getProcTimeInSec(),
        EMethodDirEnter,
        i_strMethod,
        i_strMethodInArgs );

} // traceMethodEnter

//------------------------------------------------------------------------------
/*! Traces additional runtime info.

    \param i_strMethod [in] Name of the method to be logged.
    \param i_strAddInfo [in] Additional runtime info.
*/
void CTrcMthFile::traceMethod(
    const QString& i_strMethod,
    const QString& i_strAddInfo )
//------------------------------------------------------------------------------
{
    addEntry(
        currentThreadName(),
        QDateTime::currentDateTime(),
        Time::getProcTimeInSec(),
        EMethodDirNone,
        i_strMethod,
        i_strAddInfo );

} // traceMethod

//------------------------------------------------------------------------------
/*! Traces return of a method.

    \param i_strMethod [in] Name of the method to be logged.
    \param i_strMethdReturn [in] Return values of the method.
    \param i_strMethodOutArgs [in] Output arguments.
*/
void CTrcMthFile::traceMethodLeave(
    const QString& i_strMethod,
    const QString& i_strMethdReturn,
    const QString& i_strMethodOutArgs )
//------------------------------------------------------------------------------
{
    addEntry(
        currentThreadName(),
        QDateTime::currentDateTime(),
        Time::getProcTimeInSec(),
        EMethodDirLeave,
        i_strMethod,
        i_strMethdReturn,
        i_strMethodOutArgs );

} // traceMethodLeave

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Traces entry of a method.

    This method is used if the caller don't want to let the method detect the
    name of the thread and the time stamp but wants to provide both informations
    per arguments. This is useful if the trace output should be delayed for
    some reason.

    \param i_strMthThreadName [in] Name of the thread from which the method
                                   to be traced has been called.
    \param i_dt [in] Current date time at which the method has been called.
    \param i_fSysTimeInSec [in] Current CPU execution time in secondes elapsed
                                since application has been started. May be retrieved
                                by calling ZS::System::Time::getProcTimeInSec().
    \param i_strMethod [in] Name of the method to be logged.
    \param i_strMethodInArgs [in] Input arguments.
*/
void CTrcMthFile::traceMethodEnter(
    const QString&   i_strMthThreadName,
    const QDateTime& i_dt,
    double           i_fSysTimeInSec,
    const QString&   i_strMethod,
    const QString&   i_strMethodInArgs )
//------------------------------------------------------------------------------
{
    addEntry(
        i_strMthThreadName,
        i_dt,
        i_fSysTimeInSec,
        EMethodDirEnter,
        i_strMethod,
        i_strMethodInArgs );

} // traceMethodEnter

//------------------------------------------------------------------------------
/*! Traces additional runtime info.

    This method is used if the caller don't want to let the method detect the
    name of the thread and the time stamp but wants to provide both informations
    per arguments. This is useful if the trace output should be delayed for
    some reason.

    \param i_strMthThreadName [in] Name of the thread from which the method
                                   to be traced has been called.
    \param i_dt [in] Current date time at which the method has been called.
    \param i_fSysTimeInSec [in] Current CPU execution time in secondes elapsed
                                since application has been started. May be retrieved
                                by calling ZS::System::Time::getProcTimeInSec().
    \param i_strMethod [in] Name of the method to be logged.
    \param i_strAddInfo [in] Additional runtime info.
*/
void CTrcMthFile::traceMethod(
    const QString&   i_strMthThreadName,
    const QDateTime& i_dt,
    double           i_fSysTimeInSec,
    const QString&   i_strMethod,
    const QString&   i_strAddInfo )
//------------------------------------------------------------------------------
{
    addEntry(
        i_strMthThreadName,
        i_dt,
        i_fSysTimeInSec,
        EMethodDirNone,
        i_strMethod,
        i_strAddInfo );

} // traceMethod

//------------------------------------------------------------------------------
/*! Traces return of a method.

    This method is used if the caller don't want to let the method detect the
    name of the thread and the time stamp but wants to provide both informations
    per arguments. This is useful if the trace output should be delayed for
    some reason.

    \param i_strMthThreadName [in] Name of the thread from which the method
                                   to be traced has been called.
    \param i_dt [in] Current date time at which the method has been called.
    \param i_fSysTimeInSec [in] Current CPU execution time in secondes elapsed
                                since application has been started. May be retrieved
                                by calling ZS::System::Time::getProcTimeInSec().
    \param i_strMethod [in] Name of the method to be logged.
    \param i_strMethdReturn [in] Return values of the method.
    \param i_strMethodOutArgs [in] Output arguments.
*/
void CTrcMthFile::traceMethodLeave(
    const QString&   i_strMthThreadName,
    const QDateTime& i_dt,
    double           i_fSysTimeInSec,
    const QString&   i_strMethod,
    const QString&   i_strMethdReturn,
    const QString&   i_strMethodOutArgs )
//------------------------------------------------------------------------------
{
    addEntry(
        i_strMthThreadName,
        i_dt,
        i_fSysTimeInSec,
        EMethodDirLeave,
        i_strMethod,
        i_strMethdReturn,
        i_strMethodOutArgs );

} // traceMethodLeave

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! This function combines all traceMethod functions in one method call.

    \param i_strMthThreadName [in] Name of the thread from which the method
                                   to be traced has been called.
    \param i_dt [in] Current date time at which the method has been called.
    \param i_fSysTimeInSec [in] Current CPU execution time in secondes elapsed
                                since application has been started. May be retrieved
                                by calling ZS::System::Time::getProcTimeInSec().
    \param i_mthDir [in] Defines whether the method has been entered or leaved.
                         If 'None' is passed the current indentation for the current
                         thread is  not modified and the passed additional info
                         string is writte to the log file.
    \param i_strMethod [in] Name of the method to be logged.
    \param i_strAddInfo [in] Depending on mthDir:
                             - Enter .. traced as method input arguments
                             - Leave .. traced as method return
                             - None ... traced as additional runtime info
    \param i_strMethodOutArgs [in] Depending on mthDir:
                             - Enter .. ignored
                             - Leave .. traced as method output arguments
                             - None ... ignored
*/
void CTrcMthFile::addEntry(
    const QString&         i_strMthThreadName,
    const QDateTime&       i_dt,
    double                 i_fSysTimeInSec,
    ZS::System::EMethodDir i_mthDir,
    const QString&         i_strMethod,
    const QString&         i_strAddInfo,
    const QString&         i_strMethodOutArgs )
//------------------------------------------------------------------------------
{
    if( m_pLogFile == nullptr )
    {
        return;
    }

    // The method may be called from within different thread contexts and
    // must therefore be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( !m_bEnabled )
    {
        return;
    }

    QString str;
    int     iStrLen;
    int     iCallDepth = 0;
    int     iPos = 0;
    int     idx;

    if( m_hashThreadCallDepths.contains(i_strMthThreadName) )
    {
        iCallDepth = m_hashThreadCallDepths.value(i_strMthThreadName);
    }
    else
    {
        m_hashThreadCallDepths.insert(i_strMthThreadName, 0);
    }

    if( i_mthDir == EMethodDirEnter )
    {
        //        '<MthThreadName> '     + 'yyyy-mm-dd hh:mm:ss:zzz' + ' (12345.123456): '     + '-> '            + '()' + 'bla bla bla'
        iStrLen = (c_iStrLenThreadMax+3) + 23                        + (c_iStrLenSysTimeMax+5) + 3*(iCallDepth+1) + 2 + i_strMethod.length() + i_strAddInfo.length();
    }
    else
    {
        //        '<MthThreadName> '     + 'yyyy-mm-dd hh:mm:ss:zzz' + ' (12345.123456): '     + '<- '          + '()' + 'bla bla bla'
        iStrLen = (c_iStrLenThreadMax+3) + 23                        + (c_iStrLenSysTimeMax+5) + 3*(iCallDepth) + 2 + i_strMethod.length() + i_strAddInfo.length();
    }
    str.reserve(iStrLen);

    //str.fill(0x00);

    QString strPrintThreadName;
    strPrintThreadName.resize(c_iStrLenThreadMax+3);
    strPrintThreadName.fill(' ', c_iStrLenThreadMax+3);
    strPrintThreadName.replace(0, 1, "<");
    if( i_strMthThreadName.length() <= c_iStrLenThreadMax )
    {
        strPrintThreadName.replace(1, i_strMthThreadName.length(), i_strMthThreadName);
    }
    else
    {
        strPrintThreadName.replace(1, c_iStrLenThreadMax, i_strMthThreadName.data(), c_iStrLenThreadMax);
    }
    strPrintThreadName.replace(c_iStrLenThreadMax+1, 2, "> ");
    str.replace(0, strPrintThreadName.length(), strPrintThreadName);
    iPos += strPrintThreadName.length();

    QString strDateTime = i_dt.toString("yyyy-MM-dd hh:mm:ss:zzz");
    str.replace(iPos, strDateTime.length(), strDateTime);
    iPos += strDateTime.length();

    str.replace(iPos, 2, " (");
    iPos += 2;

    QString strSysTime = QString::number(i_fSysTimeInSec, 'f', 6);
    iStrLen = strSysTime.length();
    for( idx = 0; idx < c_iStrLenSysTimeMax-iStrLen; idx++ )
    {
        strSysTime.insert(0, ' ');
    }
    str.replace(iPos, strSysTime.length(), strSysTime);
    iPos += strSysTime.length();
    str.replace(iPos, 2, "): ");
    iPos += 3;

    if( i_mthDir == EMethodDirEnter )
    {
        for( idx = 0; idx < iCallDepth; idx++ )
        {
            str.replace(iPos, 3, "   ");
            iPos += 3;
        }
        str.replace(iPos, 3, "-> ");
        iPos += 3;

        iCallDepth++;
    }
    else if( i_mthDir == EMethodDirLeave )
    {
        for( idx = 0; idx < iCallDepth-1; idx++ )
        {
            str.replace(iPos, 3, "   ");
            iPos += 3;
        }
        str.replace(iPos, 3, "<- ");
        iPos += 3;

        if( iCallDepth > 0 )
        {
            iCallDepth--;
        }
    }
    else
    {
        for( idx = 0; idx < iCallDepth-1; idx++ )
        {
            str.replace(iPos, 3, "   ");
            iPos += 3;
        }
        str.replace(iPos, 3, "   ");
        iPos += 3;
    }

    m_hashThreadCallDepths[i_strMthThreadName] = iCallDepth;

    str.replace(iPos, i_strMethod.length() ,i_strMethod);
    iPos += i_strMethod.length();
    str.replace(iPos, 1, "(");
    iPos += 1;

    if( i_mthDir != EMethodDirLeave )
    {
        if( i_strAddInfo.length() > 0 )
        {
            str.replace(iPos, i_strAddInfo.length(), i_strAddInfo);
            iPos += i_strAddInfo.length();
        }
    }
    else // if( i_mthDir == EMethodDirLeave )
    {
        if( i_strMethodOutArgs.length() > 0 )
        {
            str.replace(iPos, i_strMethodOutArgs.length(), i_strMethodOutArgs);
            iPos += i_strMethodOutArgs.length();
        }
    }

    str.replace(iPos, 1, ")");
    iPos += 1;

    if( i_mthDir == EMethodDirLeave )
    {
        if( i_strAddInfo.length() > 0 )
        {
            str += ": " + i_strAddInfo;
            iPos += (2 + i_strAddInfo.length());
        }
    }

    m_pLogFile->addEntry(str);

} // addEntry

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the current reference counter.

    /return Current reference counter.
*/
int CTrcMthFile::refCount() const
//------------------------------------------------------------------------------
{
    return m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Increments the reference counter.

    /return Current reference counter.
*/
int CTrcMthFile::incrementRefCount()
//------------------------------------------------------------------------------
{
    return ++m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Decrements the reference counter.

    /return Current reference counter.
*/
int CTrcMthFile::decrementRefCount()
//------------------------------------------------------------------------------
{
    return --m_iRefCount;
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Evaluates the name of the current thread.

    /return Name of the current thread.
*/
//------------------------------------------------------------------------------
QString CTrcMthFile::currentThreadName() const
{
    QString strThreadName = "Undefined";

    try
    {
        QThread* pThread = QThread::currentThread();

        if( pThread != nullptr )
        {
            strThreadName = pThread->objectName();
            if( strThreadName.isEmpty() )
            {
                strThreadName = QString("Thread") + threadId2Str(QThread::currentThreadId());
            }
        }
    }
    catch(...)
    {
    }

    return strThreadName;

} // currentThreadName
