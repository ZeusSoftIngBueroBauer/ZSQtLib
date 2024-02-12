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

#ifndef ZSSys_LogFile_h
#define ZSSys_LogFile_h

#include <QtCore/qstring.h>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"

class QDateTime;
class QFile;
class QTimer;

namespace ZS
{
namespace System
{
//******************************************************************************
/*! @brief The class CLogFile offers multithreaded applications the ability to write
    logging output to text files limited to a maximum number of lines.

    Log files are often used to determine the causes of errors.

    Different components, modules, classes and instances within an application
    may determine the path to log files itself. But usually log files are named
    following a specific standard.

    - The path to the log file is usually a directory for log files of applications
      proposed by the respective operating system.
    - The file name itself will mostly reflect the name of the application and as
      file extension the suffix "log" may be used.

    Under Windows often the directory "ProgramData/<CompanyName>/<AppName>" with a
    subfolder "Log" as the target for log files is used.

    If different sources (different modules, classes, instances) within the same
    application want to open the same log file, this will only be successful on the
    first call.

    The log file class remedies this by declaring both constructor and destructor
    as protected methods. Instead of using the ctor and dtor the static class methods
    CLogFile::alloc and CLogFile::free must be used to open and close a log file.

    The CLogFile::alloc method checks whether a file with the desired path is already
    in use. If so, a reference to the log file instance already used is returned and a
    reference counter for this log file instance is increased. If not, a new log
    file instance is created.

    The reference counter is decremented with the CLogFile::free method. If the
    reference counter reaches the value 0, the log file is no longer required and
    the file is closed.

    The opening and closing of files takes a relatively long time, which can have
    a negative impact on the execution speed of the application.

    For this reason, log files are often only opened once when the program is started
    and only closed again when the program is closed.
    But this has other negative effects.
    The log file can become very large and possibly use up the hard disk space.
    Secondly, the file is not closed correctly if the program crashes.

    In this case, the log file would like to have the history of the program
    execution up to the call that caused the application to crash.

    If the file is not closed correctly, the most interesting places are usually missing.
    The log file class also helps here.

    On the one hand, there is a flag that can ensure that the file is always closed
    immediately after an entry is written and saved on disk.
    This slows down the program considerably, but it can ensure that the instructions
    that caused the program to crash still appear in the log file.

    On the other hand, a log file instance may handle several sub log files.
    One sub log file is the current active file where logs are actually written to.
    All other sub log files keep the history of recently written log file entries.

    You can determine how many entries can be written to a sub log file and how many
    sub log files (including the current active file) should be used.
    The file names of the sub log files end with a counter for the sub files.

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

    If a log file file name is used for the first time, it is also checked whether
    a file with this exact name already exists. If so, the existing log file with
    all the existing sub log backup files is copied to a backup directory.
    This ensures that when the application is restarted, not all log files from the
    previous program run are overwritten.

    An AutoSave timer was defined as another feature.
    If this is active, the current log file is closed at the specified time interval
    and thus written to the hard disk.

    Example for using the class:

    @code
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

    QString strLogFileAbsPath = strAppLogDir + "/" + strAppNameNormalized + "." + strLogFileSuffix;

    CLogFile* pLogFile = CLogFile::Alloc(strLogFileAbsPath);

    pLogFile->setSubFileCountMax(2);
    pLogFile->setSubFileLineCountMax(500);

    for( int iLineNr = 1; iLineNr <= 1234; ++iLineNr )
    {
        strLogEntry = strAppNameNormalized + ".Line" + QString::number(iLineNr);
        pLogFile->addEntry(strLogEntry);
    }

    CLogFile::Free(strLogFileAbsPath);
    @endcode

*/
class ZSSYSDLL_API CLogFile : public QObject
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CLogFile"; }
public: // class methods
    static CLogFile* Alloc( const QString& i_strAbsFilePath );
    static void Free( const QString& i_strAbsFilePath );
    static void Free( CLogFile* i_pLogFile );
    static CLogFile* FindFile( const QString& i_strAbsFilePath );
    static int GetFilesCount();
protected: // ctors and dtor
    CLogFile();
    virtual ~CLogFile();
public: // instance methods
    void close();
    void clear();
public: // instance methods
    void setAbsoluteFilePath( const QString& i_strAbsFilePath );
    QString absoluteFilePath() const;
    QString completeBaseName() const;
    QString absolutePath() const;
public: // instance methods
    void setAutoSaveInterval( int i_iInterval_ms );
    int getAutoSaveInterval() const;
    void setCloseFileAfterEachWrite( bool i_bCloseFile );
    bool getCloseFileAfterEachWrite() const;
public: // instance methods
    void setSubFileCountMax( int i_iCountMax );
    int getSubFileCountMax() const;
    void setSubFileLineCountMax( int i_iCountMax );
    int getSubFileLineCountMax() const;
public: // instance methods
    void addEntry( const QString& i_strEntry );
public: // overridables of base class QObject
    virtual bool event( QEvent* i_pMsg );
protected: // instance methods
    void backup();
protected: // instance methods
    int refCount() const;
    int incrementRefCount();
    int decrementRefCount();
protected slots:
    void onTimeoutAutoSaveFile();
protected: // class members
    /*!< Mutex to protect the static and instance methods of the class for multithreaded access. */
    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    static QRecursiveMutex s_mtx;
    #else
    static QMutex s_mtx;
    #endif
    /*!< Map containing all existing (allocated) log file instances.
         The key of the entries is the absolute file path of the log files.
         The value is the pointer to the log file instance. */
    static QMap<QString, CLogFile*> s_mapLogFiles;
protected: // instance members
    /*!< Absolute path including the file name and suffix. */
    QString m_strAbsFilePath;
    /*!< Absolute path without the file name and suffix. */
    QString m_strAbsPath;
    /*!< Base name of the file without the path excluding the file suffix.
         The base name consists of all characters up to (but not including) the last '.' character. */
    QString m_strCompleteBaseName;
    /*!< Suffix (extension) of the file. The suffix consists of the characters after (but not including) the last '.'. */
    QString m_strSuffix;
    /*!< Absolute path including the file name and suffix of the currently active sub log file to which the lines are added (SubFile00). */
    QString m_strAbsFilePathSubFile00;
    /*!< Text file of the currently active sub log file to which the lines are added (SubFile00). */
    QFile* m_pFile;
    /*!< Number of allocs for this log file.
         Will be decreased with each "free" call. If the ref count becomes 0 the log file instance will be
         deleted (but the file of course remains on disk). */
    int m_iRefCount;
    /*!< If set the log file will be closed after adding a line.
         Attention! This significantly slows down the execution of the program. */
    bool m_bCloseFileAfterEachWrite;
    /*!< Maximum number of sub log files.
         Defaults to 2 which should be sufficient in most cases. */
    int m_iSubFileCountMax;
    /*!< Maximum number of lines written to the currently active sub log file (SubFile00) before the file
         will be saved, renamed to SubFile01 and a new empty currently active sub log file will be opened.
         Defaults to 1000. */
    int m_iSubFileLineCountMax;
    /*!< Current number of lines written to the latest sub log file (SubFile00).
         If the current number of lines exceeds "SubFleLineCountMax" the content
         will be saved, renamed to SubFile01 and a new empty currently active sub log file will be opened. */
    int m_iSubFileLineCountCurr;
    /*!< If greater than 0 a timer is started which automatically saves the currently active sub log file
         within the defined intervals. */
    int m_iAutoSaveInterval_ms;
    /*!< Timer triggering the auto save. */
    QTimer* m_pTimerAutoSave;

}; // class CLogFile

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_LogFile_h
