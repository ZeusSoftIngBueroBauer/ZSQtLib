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
#include <QtCore/qdatetime.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qmutex.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qtimer.h>

#include "ZSSys/ZSSysLogFile.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;

/*******************************************************************************
class ZS::System::CLogFile
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

QMutex CLogFile::s_mtx(QMutex::Recursive);
QMap<QString, CLogFile*> CLogFile::s_mapLogFiles;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns a reference to an instance of the class.

    Checks whether a log file instance with the desired absolute path (including
    file name and suffix) already exist.
    If the file name is already used, a reference to the already used log file
    instance is returned and a reference counter for this log file instance is
    incremented.
    If the file name is not yet used a new log file instance is created.

    @param i_strAbsFilePath [in] Absolute path including the file name and suffix.
    @return Pointer to log file instance.
*/
CLogFile* CLogFile::Alloc( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    // Log files may be accessed from within different thread contexts and
    // therefore accessing the files must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( i_strAbsFilePath.isEmpty() )
    {
        return nullptr;
    }

    CLogFile* pLogFile = FindFile(i_strAbsFilePath);

    // If the file name is already used ..
    if( pLogFile != nullptr )
    {
        pLogFile->incrementRefCount();;
    }
    // If the file name is not yet used ..
    else
    {
        // .. create a new log file and add it to the list of files:
        pLogFile = new CLogFile();
        pLogFile->setAbsoluteFilePath(i_strAbsFilePath);
        s_mapLogFiles[i_strAbsFilePath] = pLogFile;
        pLogFile->incrementRefCount();
    }

    return pLogFile;

} // Alloc

//------------------------------------------------------------------------------
/*! Frees the log file instance.

    The reference counter of the log file is decremented. If the reference
    counter becomes 0 the log file instance will be deleted.
    Please note that even if the reference counter is still greater than 0
    the log file instance should no longer be used be the caller of this
    method but the reference should be invalided (set to nullptr).
    If a log file with the specified file name does not exist the method
    just returns.

    @param i_strAbsFilePath [in] Absolute path including the file name and suffix.
*/
void CLogFile::Free( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    // Log files may be accessed from within different thread contexts and
    // therefore accessing the files must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    CLogFile* pLogFile = FindFile(i_strAbsFilePath);

    if( pLogFile != nullptr )
    {
        if( pLogFile->refCount() > 0 )
        {
            pLogFile->decrementRefCount();

            if( pLogFile->refCount() == 0 )
            {
                s_mapLogFiles.remove(pLogFile->absoluteFilePath());
                delete pLogFile;
                pLogFile = nullptr;
            }
        }
        else
        {
            throw CException(__FILE__, __LINE__, EResultInternalProgramError, "Log file was not allocated");
        }
    }

} // Free

//------------------------------------------------------------------------------
/*! Frees the log file instance.

    The reference counter of the log file is decremented. If the reference
    counter becomes 0 the log file instance will be deleted.
    Please note that even if the reference counter is still greater than 0
    the log file instance should no longer be used be the caller of this
    method but the reference should be invalided (set to nullptr).

    @param i_pLogFile [in] Pointer to log file instance to be freed.
*/
void CLogFile::Free( CLogFile* i_pLogFile )
//------------------------------------------------------------------------------
{
    // Log files may be accessed from within different thread contexts and
    // therefore accessing the files must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( i_pLogFile == nullptr )
    {
        return;
    }

    Free( i_pLogFile->absoluteFilePath() );

} // Free

//------------------------------------------------------------------------------
/*! Checks whether a log file with the specified absolute file path exists.

    @param i_strAbsFilePath [in] Absolute path including the file name and suffix.
    @return Pointer to log file instance.
*/
CLogFile* CLogFile::FindFile( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    // Log files may be accessed from within different thread contexts and
    // therefore accessing the files must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    CLogFile* pLogFile = nullptr;

    if( s_mapLogFiles.contains(i_strAbsFilePath) )
    {
        pLogFile = s_mapLogFiles[i_strAbsFilePath];
    }

    return pLogFile;

} // FindFile

//------------------------------------------------------------------------------
/*! Returns the number of log file instances.

    The reference counter is ignored by this method. This means that even
    if "alloc" has been called several times with the same file name the
    number of log file instances may be 1.

    @return Number of log file instances.
*/
int CLogFile::GetFilesCount()
//------------------------------------------------------------------------------
{
    return s_mapLogFiles.size();
}

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs a log file instance.

    Log file instances may only be constructed using the static alloc method.
*/
CLogFile::CLogFile() :
//------------------------------------------------------------------------------
    QObject(),
    m_strAbsFilePath(),
    m_strAbsPath(),
    m_strCompleteBaseName(),
    m_strSuffix(),
    m_strAbsFilePathSubFile00(),
    m_pFile(nullptr),
    m_iRefCount(0),
    m_bCloseFileAfterEachWrite(false),
    m_iSubFileCountMax(5),
    m_iSubFileLineCountMax(2000),
    m_iSubFileLineCountCurr(0),
    m_iAutoSaveInterval_ms(1000),
    m_pTimerAutoSave(nullptr)
{
    m_pFile = new QFile;

    m_pTimerAutoSave = new QTimer(this);

    if( !QObject::connect(
        /* pObjSender   */ m_pTimerAutoSave,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTimeoutAutoSaveFile()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
} // ctor

//------------------------------------------------------------------------------
/*! Destroys the log file instance closing the current active sub log file.

    Log file instances may only be destroyed using the static free method.
*/
CLogFile::~CLogFile()
//------------------------------------------------------------------------------
{
    if( m_pFile != nullptr && m_pFile->isOpen() )
    {
        m_pFile->close();
    }

    try
    {
        delete m_pFile;
    }
    catch(...)
    {
    }

    //m_strAbsFilePath.clear();
    //m_strAbsPath.clear();
    //m_strCompleteBaseName.clear();
    //m_strSuffix.clear();
    //m_strAbsFilePathSubFile00.clear();
    m_pFile = nullptr;
    m_iRefCount = 0;
    m_bCloseFileAfterEachWrite = false;
    m_iSubFileCountMax = 0;
    m_iSubFileLineCountMax = 0;
    m_iSubFileLineCountCurr = 0;
    m_iAutoSaveInterval_ms = 0;
    m_pTimerAutoSave = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Closes the current active sub log file.

    Please note that if an addEntry call follows the log file will be opened again.
*/
void CLogFile::close()
//------------------------------------------------------------------------------
{
    // Log files may be accessed from within different thread contexts and
    // therefore accessing the files must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( m_pFile != nullptr && m_pFile->isOpen() )
    {
        m_pFile->close();
    }

} // close

//------------------------------------------------------------------------------
/*! Clears the log file.

    All log files will be removed. The log files will not be moved to the backup directory.
*/
void CLogFile::clear()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_pFile != nullptr && m_pFile->isOpen() )
    {
        m_pFile->close();
    }

    // Remove all log files.
    //----------------------

    QDir dirLog;
    QFile logFile;

    QFileInfoList fileInfoList;

    QStringList strLstFilters;
    strLstFilters << m_strCompleteBaseName + "*.log";

    dirLog.setPath(m_strAbsPath);
    fileInfoList = dirLog.entryInfoList(strLstFilters, QDir::Files);

    for( int idxFile = 0; idxFile < fileInfoList.size(); ++idxFile )
    {
        QFileInfo fileInfo = fileInfoList.at(idxFile);

        QString strLogFileName = fileInfo.fileName();

        if( strLogFileName.startsWith(m_strCompleteBaseName,Qt::CaseInsensitive) )
        {
            logFile.setFileName(fileInfo.filePath());

            if( logFile.exists() )
            {
                logFile.remove();
            }
        }
    }

    // Reset line counter and file name.
    //----------------------------------

    m_iSubFileLineCountCurr = 0;

    m_strAbsFilePathSubFile00 = m_strAbsPath;
    if( !m_strAbsFilePathSubFile00.isEmpty() )
    {
        m_strAbsFilePathSubFile00 += "/";
    }
    m_strAbsFilePathSubFile00 += m_strCompleteBaseName + "00";

    if( !m_strSuffix.isEmpty() )
    {
        m_strAbsFilePathSubFile00 += "." + m_strSuffix;
    }
    if( !m_strAbsFilePathSubFile00.isEmpty() )
    {
        if( m_pFile != nullptr )
        {
            m_pFile->setFileName(m_strAbsFilePathSubFile00);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the absolute file path include path, file name and suffix.

    The absolute file path is parsed and the absolute path, the complete base
    name and file suffix is extracted. If a log file with the same file path
    is already existing all sub log files will be moved to the log backup directory.

    @param i_strAbsFilePath [in] Absolute path including the file name and suffix.
*/
void CLogFile::setAbsoluteFilePath( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    // Log files may be accessed from within different thread contexts and
    // therefore accessing the files must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( m_pFile != nullptr && m_pFile->isOpen() )
    {
        m_pFile->close();
    }

    if( !i_strAbsFilePath.isEmpty() && !m_strAbsFilePath.isEmpty() )
    {
        backup();
    }

    if( s_mapLogFiles.contains(m_strAbsFilePath) )
    {
        s_mapLogFiles.remove(m_strAbsFilePath);
    }

    m_iSubFileLineCountCurr = 0;
    m_strAbsPath = "";
    m_strCompleteBaseName = "";
    m_strSuffix = "";
    m_strAbsFilePathSubFile00 = "";

    m_strAbsFilePath = i_strAbsFilePath;

    if( !m_strAbsFilePath.isEmpty() )
    {
        s_mapLogFiles[m_strAbsFilePath] = this;

        QFileInfo fileInfoFile(m_strAbsFilePath);
        QString   strFileUdx;

        m_strAbsPath = fileInfoFile.absolutePath();
        m_strCompleteBaseName = fileInfoFile.completeBaseName();
        m_strSuffix = fileInfoFile.suffix();

        QDir dirLogFile;
        if( !dirLogFile.exists(m_strAbsPath) )
        {
            dirLogFile.mkpath(m_strAbsPath);
        }

        backup();

        m_strAbsFilePathSubFile00 = m_strAbsPath;
        if( !m_strAbsFilePathSubFile00.isEmpty() )
        {
            m_strAbsFilePathSubFile00 += "/";
        }
        m_strAbsFilePathSubFile00 += m_strCompleteBaseName + "00";

        if( !m_strSuffix.isEmpty() )
        {
            m_strAbsFilePathSubFile00 += "." + m_strSuffix;
        }
        if( !m_strAbsFilePathSubFile00.isEmpty() )
        {
            if( m_pFile != nullptr )
            {
                m_pFile->setFileName(m_strAbsFilePathSubFile00);
            }
        }
    } // if( !m_strAbsFilePath.isEmpty() )

} // setAbsoluteFilePath

//------------------------------------------------------------------------------
/*! Returns the absolute path of the log file including the file name and suffix.

    Example:

        QString strAbsFilePath = "C:/ProgramData/ZeusSoft/TestSysLog/TestSysLog.log";
        CLogFile* pLogFile = CLogFile::Alloc(strAbsPath);
        QString strAbsFilePath = pLogFile->absoluteFilePath();  // strBaseName = "C:/ProgramData/ZeusSoft/TestSysLog/TestSysLog.log"

    @return Absolute path of the log file including the file name and suffix.
*/
QString CLogFile::absoluteFilePath() const
//------------------------------------------------------------------------------
{
    return m_strAbsFilePath;
}

//------------------------------------------------------------------------------
/*! Returns the complete base name of the log file without the path.

    The complete base name consists of all characters in the file up
    to (but not including) the last '.' character.

    Example:

        QString strAbsFilePath = "C:/ProgramData/ZeusSoft/TestSysLog/TestSysLog.log";
        CLogFile* pLogFile = CLogFile::Alloc(strAbsPath);
        QString strBaseName = pLogFile->completeBaseName();  // strBaseName = "TestSysLog"

    @return Complete base name of the file without the path.
*/
QString CLogFile::completeBaseName() const
//------------------------------------------------------------------------------
{
    return m_strCompleteBaseName;
}

//------------------------------------------------------------------------------
/*! Returns the log file's absolute path.

    The absolute path doesn't include the file name.

    Example:

        QString strAbsFilePath = "C:/ProgramData/ZeusSoft/TestSysLog/TestSysLog.log";
        CLogFile* pLogFile = CLogFile::Alloc(strAbsPath);
        QString strAbsPath = pLogFile->absolutePath();  // strAbsPath = "C:/ProgramData/ZeusSoft/TestSysLog"

    @return Absolute path of the log file including the file name and suffix.
*/
QString CLogFile::absolutePath() const
//------------------------------------------------------------------------------
{
    return m_strAbsPath;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the time interval for the auto save timer.

    If greater than 0 a timer is started which automatically saves the
    currently active sub log file within the defined intervals.
    Please note that an following addEntry call opens the log file again.

    @param i_iInterval_ms [in] Timer interval in milli seconds.
*/
void CLogFile::setAutoSaveInterval( int i_iInterval_ms )
//------------------------------------------------------------------------------
{
    // Log files may be accessed from within different thread contexts and
    // therefore accessing the files must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( m_iAutoSaveInterval_ms != i_iInterval_ms )
    {
        m_iAutoSaveInterval_ms = i_iInterval_ms;

        if( m_pTimerAutoSave->thread() == QThread::currentThread() )
        {
            if( m_pTimerAutoSave->isActive() )
            {
                m_pTimerAutoSave->stop();
            }
            if( m_iAutoSaveInterval_ms > 0 )
            {
                m_pTimerAutoSave->start(m_iAutoSaveInterval_ms);
            }
        }
        else
        {
            CMsgReqStopTimer* pMsgReqStopTimer = new CMsgReqStopTimer(
                /* pObjSender   */ this,
                /* pObjReceiver */ this,
                /* iTimerId     */ -1 );
            QCoreApplication::postEvent(this, pMsgReqStopTimer);
            pMsgReqStopTimer = NULL;
            CMsgReqStartTimer* pMsgReqStartTimer = new CMsgReqStartTimer(
                /* pObjSender   */ this,
                /* pObjReceiver */ this,
                /* iTimerId     */ -1,
                /* bSingleShot  */ false,
                /* iInterval_ms */ m_iAutoSaveInterval_ms );
            QCoreApplication::postEvent(this, pMsgReqStartTimer);
            pMsgReqStartTimer = NULL;
        }
    }
} // setAutoSaveInterval

//------------------------------------------------------------------------------
/*! Returns the time interval in milli seconds of the auto save timer.

    @return Timer interval in milli seconds.
*/
int CLogFile::getAutoSaveInterval() const // returns time in milli seconds
//------------------------------------------------------------------------------
{
    return m_iAutoSaveInterval_ms;
}

//------------------------------------------------------------------------------
/*! Defines whether the current active log file should be immediately closed
    after adding an entry.

    This may help finding the reason for abnormal program terminations. But
    please note that if the flag is set this will significantly slow down
    the execution of the program.

    @param i_bCloseFile [in] Flag defining whether the log file should be
                             immediately closed after adding an entry.
*/
void CLogFile::setCloseFileAfterEachWrite( bool i_bCloseFile )
//------------------------------------------------------------------------------
{
    // Log files may be accessed from within different thread contexts and
    // therefore accessing the files must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( m_bCloseFileAfterEachWrite != i_bCloseFile )
    {
        m_bCloseFileAfterEachWrite = i_bCloseFile;

        if( m_bCloseFileAfterEachWrite )
        {
            if( m_pFile != nullptr && m_pFile->isOpen() )
            {
                m_pFile->close();
            }
            if( m_pTimerAutoSave->thread() == QThread::currentThread() )
            {
                if( m_pTimerAutoSave->isActive() )
                {
                    m_pTimerAutoSave->stop();
                }
            }
            else
            {
                CMsgReqStopTimer* pMsgReqStopTimer = new CMsgReqStopTimer(
                    /* pObjSender   */ this,
                    /* pObjReceiver */ this,
                    /* iTimerId     */ -1 );
                QCoreApplication::postEvent(this, pMsgReqStopTimer);
                pMsgReqStopTimer = NULL;
            }
        }
        else
        {
            if( m_iAutoSaveInterval_ms > 0 && !m_pTimerAutoSave->isActive() )
            {
                if( m_pTimerAutoSave->thread() == QThread::currentThread() )
                {
                    m_pTimerAutoSave->start(m_iAutoSaveInterval_ms);
                }
                else
                {
                    CMsgReqStartTimer* pMsgReqStartTimer = new CMsgReqStartTimer(
                        /* pObjSender   */ this,
                        /* pObjReceiver */ this,
                        /* iTimerId     */ -1,
                        /* bSingleShot  */ false,
                        /* iInterval_ms */ m_iAutoSaveInterval_ms );
                    QCoreApplication::postEvent(this, pMsgReqStartTimer);
                    pMsgReqStartTimer = NULL;
                }
            }
        }
    }
} // setCloseFileAfterEachWrite

//------------------------------------------------------------------------------
/*! Returns whether the current active log file is immediately closed
    after adding an entry.

    @return Flag defining whether the log file should be immediately closed
            after adding an entry.
*/
bool CLogFile::getCloseFileAfterEachWrite() const
//------------------------------------------------------------------------------
{
    return m_bCloseFileAfterEachWrite;
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

    @param i_iCountMax [in] Maximum number of sub log files.
*/
void CLogFile::setSubFileCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    // Log files may be accessed from within different thread contexts and
    // therefore accessing the files must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( m_iSubFileCountMax != i_iCountMax )
    {
        m_iSubFileCountMax = i_iCountMax;

        if( m_iSubFileCountMax == 100 )
        {
            m_iSubFileCountMax = 2;
        }
        else if( m_iSubFileCountMax > 100 )
        {
            m_iSubFileCountMax = 100;
        }
    }
} // setSubFileCountMax

//------------------------------------------------------------------------------
/*! Returns the maximum number of sub log files used.

    @return Maximum number of used sub log files.
*/
int CLogFile::getSubFileCountMax() const
//------------------------------------------------------------------------------
{
    return m_iSubFileCountMax;
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

    @param i_iCountMax [in] Maximum number of lines for a sub log file.
*/
void CLogFile::setSubFileLineCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    m_iSubFileLineCountMax = i_iCountMax;
}

//------------------------------------------------------------------------------
/*! Returns the maximum number of lines used for sub log files.

    @return Maximum number of lines for a sub log file.
*/
int CLogFile::getSubFileLineCountMax() const
//------------------------------------------------------------------------------
{
    return m_iSubFileLineCountMax;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Writes a line to the log file.

    @param i_strEntry [in] Log file entry.
*/
void CLogFile::addEntry( const QString& i_strEntry )
//------------------------------------------------------------------------------
{
    // Log files may be accessed from within different thread contexts and
    // therefore accessing the files must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( !m_strAbsFilePath.isEmpty() )
    {
        if( m_pFile == nullptr )
        {
            m_pFile = new QFile(m_strAbsFilePathSubFile00);
        }

        if( !m_pFile->isOpen() )
        {
            if( m_pFile->exists() )
            {
                m_pFile->open(QIODevice::Append|QIODevice::Text);
            }
            else
            {
                m_pFile->open(QIODevice::WriteOnly|QIODevice::Text);
            }
        }

        if( m_pFile->isOpen() )
        {
            // PATCH and WORKAROUND:
            // Using QTextStream did not work correctly. Lines were somehow lost (Qt version 4.8.2).
            // Using "QFile.write" instead seem to work properly.

            //QTextStream textStream(m_pFile);
            //textStream << i_strEntry << "\n";
            m_pFile->write(i_strEntry.toUtf8());
            m_pFile->write("\n");
            m_iSubFileLineCountCurr++;
        }

        if( m_bCloseFileAfterEachWrite && m_pFile->isOpen() )
        {
            m_pFile->flush();
            m_pFile->close();
        }

        if( m_iSubFileLineCountCurr >= m_iSubFileLineCountMax )
        {
            if( m_pFile->isOpen() )
            {
                m_pFile->close();
            }

            QString strUdxFileCurr;
            QString strFileCurrAbsFilePath;
            QFile   fileCurr;
            QString strUdxFilePrev;
            QString strFilePrevAbsFilePath;
            QFile   filePrev;
            int     idxFile;

            for( idxFile = static_cast<int>(m_iSubFileCountMax-1); idxFile > 0; idxFile-- )
            {
                strUdxFileCurr = QString::number(idxFile);
                if( idxFile < 10 )
                {
                    strUdxFileCurr.insert(0,"0");
                }

                strFileCurrAbsFilePath = m_strAbsPath;
                if( !strFileCurrAbsFilePath.isEmpty() )
                {
                    strFileCurrAbsFilePath += "/";
                }
                strFileCurrAbsFilePath += m_strCompleteBaseName;
                strFileCurrAbsFilePath += strUdxFileCurr;
                strFileCurrAbsFilePath += "." + m_strSuffix;

                if( fileCurr.isOpen() )
                {
                    fileCurr.close();
                }
                fileCurr.setFileName(strFileCurrAbsFilePath);

                if( fileCurr.exists() )
                {
                    fileCurr.remove();
                }

                strUdxFilePrev = QString::number(idxFile-1);
                if( (idxFile-1) < 10 )
                {
                    strUdxFilePrev.insert(0,"0");
                }

                strFilePrevAbsFilePath = m_strAbsPath;
                if( !strFilePrevAbsFilePath.isEmpty() )
                {
                    strFilePrevAbsFilePath += "/";
                }
                strFilePrevAbsFilePath += m_strCompleteBaseName;
                strFilePrevAbsFilePath += strUdxFilePrev;
                strFilePrevAbsFilePath += "." + m_strSuffix;

                if( filePrev.isOpen() )
                {
                    filePrev.close();
                }
                filePrev.setFileName(strFilePrevAbsFilePath);

                if( filePrev.exists() )
                {
                    filePrev.copy(strFileCurrAbsFilePath);
                }
            } // for( idxFile = static_cast<int>(m_iSubFileCountMax-1); idxFile > 0; idxFile-- )

            m_pFile->remove();
            m_iSubFileLineCountCurr = 0;

        } // if( m_iSubFileLineCountCurr >= m_iSubFileLineCountMax )

        // PATCH and WORKAROUND:
        // Using QTextStream did not work correctly. Lines were somehow lost (Qt version 4.8.2).
        // After trying a lot delete the QFile instance (if m_bCloseFileAfterEachWrite is set)
        // and creating the QFile instance again worked. This is strange ...
        if( m_bCloseFileAfterEachWrite )
        {
            delete m_pFile;
            m_pFile = nullptr;
        }

    } // if( !m_strAbsFilePath.isEmpty() )

} // addEntry

/*==============================================================================
public: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CLogFile::event( QEvent* i_pMsg )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    // Timers cannot be started and stopped from another thread.
    if( i_pMsg->type() >= QEvent::User )
    {
        QString strAddTrcInfo;
        QString strAddErrInfo;

        CMsg* pMsg = nullptr;

        try
        {
            pMsg = dynamic_cast<CMsg*>(i_pMsg);
        }
        catch(...)
        {
            pMsg = nullptr;
        }

        if( pMsg != nullptr )
        {
            bEventHandled = true;

            QMutexLocker mtxLocker(&s_mtx);

            if( pMsg->getMsgType() == EBaseMsgTypeReqStartTimer )
            {
                if( m_iAutoSaveInterval_ms > 0 )
                {
                    m_pTimerAutoSave->start(m_iAutoSaveInterval_ms);
                }
            }
            else if( pMsg->getMsgType() == EBaseMsgTypeReqStopTimer )
            {
                if( m_pTimerAutoSave->isActive() )
                {
                    m_pTimerAutoSave->stop();
                }
            }
        }
    }
    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pMsg);
    }
    return bEventHandled;
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Moves all already existing sub log files to the log backup directory.
*/
void CLogFile::backup()
//------------------------------------------------------------------------------
{
    // Log files may be accessed from within different thread contexts and
    // therefore accessing the files must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    QString strLogBakDirPath;
    QString strLogBakFilePath;
    QString strLogFileName;
    QDir    dirLog;
    QDir    dirLogBak;
    QFile   fileOldBak;
    QFile   fileOld;
    int     idxFile;

    QFileInfoList fileInfoList;

    QStringList strLstFilters;
    strLstFilters << m_strCompleteBaseName + "*.log";

    // Before we start backing up the sub log files we ensure that currently no sub log file is open:
    //-----------------------------------------------------------------------------------------------

    if( m_pFile != nullptr && m_pFile->isOpen() )
    {
        m_pFile->close();
    }

    // Create the backup directory (if not yet created)
    //-------------------------------------------------

    strLogBakDirPath = m_strAbsPath;
    if( !strLogBakDirPath.isEmpty() )
    {
        strLogBakDirPath += "/";
    }
    strLogBakDirPath += "Log.bak";

    bool bLogBakDirExists = dirLogBak.exists(strLogBakDirPath);

    if( !bLogBakDirExists )
    {
        bLogBakDirExists = dirLogBak.mkpath(strLogBakDirPath);
    }

    // Copy all "*.log" files into the backup directory
    //-------------------------------------------------

    if( bLogBakDirExists )
    {
        // Copy found log file into backup directory
        //------------------------------------------

        dirLog.setPath(m_strAbsPath);
        fileInfoList = dirLog.entryInfoList(strLstFilters, QDir::Files);

        for( idxFile = 0; idxFile < fileInfoList.size(); ++idxFile )
        {
            QFileInfo fileInfo = fileInfoList.at(idxFile);

            strLogFileName = fileInfo.fileName();

            if( strLogFileName.startsWith(m_strCompleteBaseName,Qt::CaseInsensitive) )
            {
                fileOld.setFileName(fileInfo.filePath());

                if( fileOld.exists() )
                {
                    strLogBakFilePath = m_strAbsPath;
                    if( !strLogBakFilePath.isEmpty() )
                    {
                        strLogBakFilePath += "/";
                    }
                    strLogBakFilePath += "Log.bak/" + fileInfo.fileName();

                    fileOld.copy(strLogBakFilePath);
                    fileOld.remove();
                }
            } // if( strLogFileName.startsWith(m_strCompleteBaseName,Qt::CaseInsensitive) )
        } // for( idxFile = 0; idxFile < fileInfoList.size(); ++idxFile )
    } // if( bLogBakDirExists )

} // backup

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the current reference counter.

    /return Current reference counter.
*/
int CLogFile::refCount() const
//------------------------------------------------------------------------------
{
    return m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Increment the reference counter.

    /return Current reference counter.
*/
int CLogFile::incrementRefCount()
//------------------------------------------------------------------------------
{
    return ++m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Decrements the reference counter.

    /return Current reference counter.
*/
int CLogFile::decrementRefCount()
//------------------------------------------------------------------------------
{
    return --m_iRefCount;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
/*! Timer call back method for automatically saving (closing) the current active
    sub log file.
*/
void CLogFile::onTimeoutAutoSaveFile()
//------------------------------------------------------------------------------
{
    // Log files may be accessed from within different thread contexts and
    // therefore accessing the files must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( m_pFile != nullptr && m_pFile->isOpen() )
    {
        m_pFile->close();
    }
}
