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

#ifndef ZSSys_ErrLog_h
#define ZSSys_ErrLog_h

#include <QtCore/qdatetime.h>
#include <QtCore/qobject.h>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysMsg.h"

#ifdef WIN32
#include <Windows.h>
#endif

class QFile;
class QMutex;
class QWaitCondition;
class QXmlStreamWriter;

namespace ZS
{
namespace System
{
class CTestCrashDumpThread;

//******************************************************************************
/*! @brief Each entry in the error log has this data type.

    The entries in the error log contain error codes, the source of the error,
    additional information about the error that has occurred but also the date, time,
    the elapsed time relative to the start of the program, how often one and the same
    error has occurred and a possibly a suggestion on how to fix this error.
*/
struct ZSSYSDLL_API SErrLogEntry
//******************************************************************************
{
public: // ctors
    SErrLogEntry();
    SErrLogEntry( const SErrLogEntry& i_other );
    SErrLogEntry(
        const QDateTime&      i_dateTime,
        double                i_fSysTime_us,
        const SErrResultInfo& i_errResultInfo,
        const QString&        i_strProposal,
        int                   i_iOccurrences );
public: // operators
    SErrLogEntry& operator = ( const SErrLogEntry& i_other );
public: // instance members
    int            m_iRowIdx;       /*!< Index in the Error Logs Table. For internal use only. May be changed on removing entries from the error log. */
    QDateTime      m_dateTime;      /*!< Date and time the error occurred or the entry was created. */
    double         m_fSysTime_us;   /*!< Time in micro seconds which have been passed since program start the error occured or the entry was created. */
    SErrResultInfo m_errResultInfo; /*!< Resultcode, severity, source and additional info about the error that occurred. */
    QString        m_strProposal;   /*!< Suggestion how to fix the error. */
    int            m_iOccurrences;  /*!< Number of occurrences the same error has occurred. */

}; // struct SErrLogEntry

//******************************************************************************
/*! @brief The class CErrLog offers multithreaded applications the ability to store
    error result infos in a xml file for later examination.

    Log files are often used to determine the causes of errors.

    Different components, modules, classes and instances within an application
    may determine the path to log files itself. But usually log files are named
    following a specific standard.

    - The path to the log file is usually a directory for log files of applications
      proposed by the respective operating system.
    - The file name itself will mostly reflect the name of the application and the
      suffix of the file extension specifies the conten type of the file.

    Under Windows often the directory "ProgramData/<CompanyName>/<AppName>" with a
    subfolder "Log" as the target for log files is used.

    If different sources (different modules, classes, instances) within the same
    application want to open the same error log file, this will only be successful
    on the first call.

    The errog log file class remedies this by declaring both constructor and destructor
    as protected methods. Instead of using the ctor and dtor the static class methods
    CErrLog::GetInstance, CErrLog::CreateInstance and CErrLog::ReleaseInstance
    must be used to open and close an error log file.

    Usually only one error log instance is created when the application is
    started using the CreateInstance class method. This error log instance
    may be accessed during runtime by the GetInstance class method.

    When creating an error log instance also an object name must be assigned beside
    the path to the xml file. For each different xml path a unique object name must
    be provided.

    The CErrLog::GetInstance method checks whether an instance with the given
    object name is already existing. If not the method returns nullptr.
    If no error log instance with the given name is existing but is needed the
    method CErrLog::CreateInstance must be invoked passing the object name of the
    error log instance together with the path to the xml file. The module, class,
    or instance creating the error log instance must destroy the error log instance
    if no longer needed to save the actual content of the error log instance,
    to close the xml file and to avoid memory leaks.

    As the error log instances may be accessed from within different thread contexts
    the methods of the class are internally serialized using mutexes.

    The CErrLog class offers methods to loop through the entries. To avoid that
    error log entries are added, removed or changed while looping through the list
    of entries and accessing entries which are no longer valid the error log instance
    may be temporarily locked. But don't forget to unlock the error log instance
    if no longer needed. Otherwise the appliation may be deadlocked.

    When invoking Qt methods also errors may occur. Qt offers the possibility
    to install a message handler for those error events. When creating the error
    log instance you can choose whether to install such an Qt error handler or not.
    If the error handler is installed (which is the default setting) all Qt errors
    will also be saved in the error log instance.

    You can determine how many entries can be saved in the error log file for
    each result severity.

    Example for using the class:

    @code
    QString strAppName = QCoreApplication::applicationName();

    QString strIniFileScope = "System";

    QString strAppLogDir = ZS::System::getAppLogDir(strIniFileScope);

    QString strErrLogFileBaseName = strAppNameNormalized + "-ErrLog.";
    QString strErrLogFileSuffix = "xml";

    QString strErrLogFileAbsFilePath = strAppLogDir + "/" + strErrLogFileBaseName + "." + strErrLogFileSuffix;

    CErrLog::CreateInstance(true, strErrLogFileAbsFilePath, "MyErrLogFile");
    @endcode

    If only a single error log instance is required in the system and the default
    settings are sufficient, you may call CreateInstance without arguments.
    @code
    CErrLog::CreateInstance();
    @endcode

    The following default settings are then used:

     - Qt error messages will be saved in the log file,
     - the xml log file will be saved in file path "<ProgramData>/<OrganizationName>/<ApplicationName>/Log"
     - using the file name "<ApplicationName> -ErrLog.xml" and
     - "ZSErrLog" is used as the object name of the error log instance.

    The ErrLog instance must be released again when the application is closed.
    On the one hand to save all actual error messages in the ErrLog file and, of course,
    because for a clean shutdown all created instances should also be released again so that
    no memory leaks are left behind. You do this by invoking ReleaseInstance. If ReleaseInstance
    is called without an object name, the name "ZSErrLog" is used by default.
    If you have used your own name, this must be passed as an argument to ReleaseInstance.

    @code
    CErrLog::ReleaseInstance();
    @endcode

    From anywhere in the program, you can use the static method GetInstance to access the
    the error log instance. The object name of the error log file must be given.
    If GetInstance is called without aguments, the default object name "ZSErrLog" is used.

    Error messages are added at runtime of the program with the call addEntry, as in the following
    example shown.

    @code
    if( CErrLog::GetInstance() != nullptr )
    {
        SErrResultInfo errResultInfo(
            ** errSource  ** "ZS::System", "CTest", "ErrLogTest", "testMethod",
            ** result     ** EResultListIsFull,
            ** severity   ** EResultSeverityError,
            ** strAddInfo ** "Maximum number of entries for reached." );
        CErrLog::GetInstance()->addEntry(errResultInfo);
    }
    @endcode

    @see _GRP_BasicConcepts_ErrorHandling
*/
class ZSSYSDLL_API CErrLog : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::System"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CErrLog"; }
public: // class methods
    static CErrLog* GetInstance( const QString& i_strName = "ZSErrLog" );
    static CErrLog* CreateInstance(
        bool           i_bInstallQtMsgHandler = true,
        bool           i_bInstallTerminateHandler = true,
        bool           i_bInstallFaultHandler = true,
        const QString& i_strAbsFilePath = "",
        const QString& i_strName = "ZSErrLog" );
    static void ReleaseInstance( const QString& i_strName = "ZSErrLog" );
    static void ReleaseInstance( CErrLog* i_pErrLog );
private: // class methods
    static void InstallQtMsgHandler();
    static void RemoveQtMsgHandler();
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    static void QtMsgHandler( QtMsgType i_msgType, const char* i_szMsg );
    #else
    static void QtMsgHandler( QtMsgType i_msgType, const QMessageLogContext& i_context, const QString& i_strMsg );
    #endif
    static void TerminateHandler();
    #ifdef WIN32
    static long ExceptionHandler(EXCEPTION_POINTERS* i_pExceptionPointers);
    #endif
protected: // ctors and dtor
    CErrLog(
        const QString& i_strName,
        const QString& i_strAbsFilePath,
        bool           i_bInstallQtMsgHandler,
        bool           i_bInstallTerminateHandler,
        bool           i_bInstallFaultHandler );
    virtual ~CErrLog();
signals:
    /*! Signal which will be emitted if an entry is added to the error log.
        Please note that the signal may be queued if the receiver is in another thread.
        You can't rely on that the error log entry still exists when receiving the signal.
        For this not a reference to the entry but to the ErrResultInfo structure is passed with the signal. */
    void entryAdded( const ZS::System::SErrResultInfo& i_errResultInfo );   // Will only be emitted for new entries.
    /*! Signal which will be emitted if an error log entry is changed.
        Please note that the signal may be queued if the receiver is in another thread.
        You can't rely on that the error log entry still exists when receiving the signal.
        For this not a reference to the entry but to the ErrResultInfo structure is passed with the signal. */
    void entryChanged( const ZS::System::SErrResultInfo& i_errResultInfo ); // Will be emitted for each occurrence.
    /*! Signal which will be emitted if an entry is removed from the error log.
        Please note that the signal may be queued if the receiver is in another thread.
        You can't rely on that the error log entry still exists when receiving the signal.
        For this not a reference to the entry but to the ErrResultInfo structure is passed with the signal. */
    void entryRemoved( const ZS::System::SErrResultInfo& i_errResultInfo );
public: // instance methods
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setEntriesCountMax( int i_iCount, EResultSeverity i_severity = EResultSeverityUndefined ); // Use Undefined (or Count) to set maximum numbers for all severities at once.
    int getEntriesCountMax( EResultSeverity i_severity = EResultSeverityUndefined ) const;          // Use Undefined (or Count) to get the sum of all maximum numbers for all severities.
public: // instance methods
    QString getAbsFilePath() const;
public: // instance methods
    SErrLogEntry* findEntry( const SErrResultInfo& i_errResultInfo );
    void addEntry(
        SErrResultInfo& i_errResultInfo,  // Not const as will be marked as "AddedToErrLogModel".
        const QString&  i_strProposal = "" );
    void addEntry(
        const QDateTime& i_dateTime,
        SErrResultInfo&  i_errResultInfo,  // Not const as will be marked as "AddedToErrLogModel".
        const QString&   i_strProposal = "" );
    void changeEntry(
        const SErrResultInfo& i_errResultInfo,
        bool                  i_bModifyDateTime = false,
        const QDateTime&      i_dateTime = QDateTime(),
        bool                  i_bModifyProposal = false,
        const QString&        i_strProposal = "" );
    void removeEntry( const SErrResultInfo& i_errResultInfo );
public: // instance methods
    void clear( EResultSeverity i_severity = EResultSeverityUndefined );            // Use Undefined (or Count) to clear all errors for all severities.
public: // instance methods
    void lock();     // !! Before "looping" through the list you need to lock the list !!
    void unlock();   // !! Don't forget to unlock the list after "looping" through the list !!
public: // instance methods
    int getEntryCount( EResultSeverity i_severity = EResultSeverityUndefined );     // Use Undefined (or Count) to get the sum of all errors for all severities.
    // Use Undefined (or Count) to count through all severities. In this case counting rows starts at highest severity (Critical).
    SErrLogEntry* getEntry( int i_iRowIdx, EResultSeverity i_severity = EResultSeverityUndefined );
    void removeEntry( int i_iRowIdx, EResultSeverity i_severity = EResultSeverityUndefined );
    SErrLogEntry takeEntry( int i_iRowIdx, EResultSeverity i_severity = EResultSeverityUndefined );
    SErrLogEntry takeFirstEntry( EResultSeverity i_severity = EResultSeverityUndefined );
    SErrLogEntry takeLastEntry( EResultSeverity i_severity = EResultSeverityUndefined );
public: // instance methods
    void testCrashDump();
protected: // instance methods
    void recall();
    void save();
protected: // instance methods
    void saveEntry( SErrLogEntry* i_pEntry, QXmlStreamWriter& i_xmlStreamWriter );
protected: // instance methods
    SErrLogEntry* addEntry_(
        const QDateTime&      i_dateTime,
        const SErrResultInfo& i_errResultInfo,
        const QString&        i_strProposal );
    void changeEntry_(
        SErrLogEntry*     i_pEntry,
        bool              i_bModifyOccurrences = false,
        int               i_iOccurrences = 0,
        bool              i_bModifyDateTime = false,
        const QDateTime&  i_dateTime = QDateTime(),
        bool              i_bModifyProposal = false,
        const QString&    i_strProposal = "" );
    void removeEntry_( int i_iRowIdx, EResultSeverity i_severity = EResultSeverityUndefined );
    #ifdef WIN32
    QString generateDump(EXCEPTION_POINTERS* i_pExceptionPointers) const;
    #endif
private: // copy ctor not allowed
    CErrLog( const CErrLog& );
private: // assignment operator not allowed
    CErrLog& operator = ( const CErrLog& );
protected: // class members
    /*!< Mutex to protect the class variables for multithreaded access. */
    static QMutex s_mtx;
    /*!< Hash with all created err log instances (key is name of instance). */
    static QHash<QString, CErrLog*> s_hshpInstances;
    /*!< Counts the number the class method InstallQtMsgHandler is called
         to ensure that the Qt message handler is installed and removed just once. */
    static int s_iMsgHandlerInstallCount;
protected: // instance members
    /*!< Mutex to protect the instance variables of the class for multithreaded access.
         Please note that entries may be added from within different thread contexts
         to the error log object and for this the list of entries of the error
         log object is protected by a mutex and entries will be "immediately" entered. */
    QMutex* m_pMtx;
    /*!< Absolute path including the file name and suffix of the error logs xml file. */
    QString m_strAbsFilePath;
    /*!< Xml file of the error log instance. */
    QFile* m_pFile;
    /*!< Flag for internal state machine to indicate that currently error log entries are
         added because the content of the error log file is read to avoid that the new
         entries are saved in the xml file again causing an endless recursion. */
    bool m_bRecallingModel;
    /*!< Flag for internal state machine to indicate that currently the content of the
         error log instance is cleared to avoid that for each removed entry a signal is emitted. */
    bool m_bClearingModel;
    /*!< The recursion counter avoids endless recursion if there is a problem
         when trying to write to the xml file (e.g. saving file not possible)
         whereupon the qt message handler is called invoking addEntry_ again
         whereupon again the save method would be called and so on. */
    int m_iAddEntryRecursionCounter;
    // Please note that shiboken crashes with error -1073741819 (access violation)
    // if array member variables are used defined by [] operators with fixed size.
    // Using vector initialized with fixed size in the ctor as a workaround.
    // Next problem: stupid gcc error: ">> should be > > within a nested template argument list".
    // But on using "> >" instead of ">>" shiboken reports an error on compiling the wrapper module.
    // As a workaround on linux to let shiboken generate the bindings the definition will be
    // temporarily changed to ">>". After shiboken has generated the bindings the definition
    // will be changed back to "> >" to compile the module with gcc on linux.
    /*!< Defines for each result severity the maximum number of log entries. */
    QVector<int> m_ariEntriesCountMax;
    /*!< For each severity list of error log entries. */
    QVector< QList<SErrLogEntry*> > m_ararpEntries;
    /*!< true, if this instance installed the Qt message hander and
         therefore must be removed again when the dtor is called. */
    bool m_bQtMsgHandlerInstalledByCtor;
    /*!< Pointer to thread for testing the generation of a crash dump file. */
    CTestCrashDumpThread* m_pTestCrashDumpThread;

}; // class CErrLog

//******************************************************************************
/*! @brief Internal thread class used to test creating a crash dump file.

    Added in this header so that the MOC compiler can find it.

    When calling CErrLog::testCrashDump an instance of this thread is created
    and the thread is started.
    A single shot timer is started in the run method of the thread invoking
    a lambda function which forces an access violation (nullptr assignment).
*/
class CTestCrashDumpThread : public QThread
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CTestCrashDumpThread();
    virtual ~CTestCrashDumpThread();
public: // instance methods
    bool waitForThreadRunning() const;
public: // overridables of base class QThread
    virtual void run() override;
private: // instance members
    QMutex*         m_pMtxWaitThreadRunning = nullptr;
    QWaitCondition* m_pWaitThreadRunning = nullptr;
};

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_ErrLog_h
