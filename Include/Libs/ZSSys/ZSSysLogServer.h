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

#ifndef ZSSys_LogServer_h
#define ZSSys_LogServer_h

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysCommon.h"
#include "ZSSys/ZSSysErrResult.h"

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>


namespace ZS
{
namespace System
{
class CMsg;
class CMutex;
class CLogFile;
class CLogger;
class CIdxTreeLoggers;

//******************************************************************************
/*! struct SLogServerSettings
*/
struct ZSSYSDLL_API SLogServerSettings
//******************************************************************************
{
public: // ctor
    SLogServerSettings(
        bool i_bEnabled = true,
        bool i_bNewLoggersEnabledAsDefault = true,
        ZS::System::ELogDetailLevel i_eNewLoggersDefaultDetailLevel = ZS::System::ELogDetailLevel::None,
        bool i_bUseIpcServer = true,
        bool i_bCacheDataIfNotConnected = false,
        int  i_iCacheDataMaxArrLen = 1000,
        bool i_bUseLocalLogFile = true,
        int  i_iLocalLogFileAutoSaveInterval_ms = 1000,
        int  i_iLocalLogFileSubFileCountMax = 5,
        int  i_iLocalLogFileSubFileLineCountMax = 2000,
        bool i_bLocalLogFileCloseFileAfterEachWrite = false );
public: // operators
    bool operator == ( const SLogServerSettings& i_settingsOther ) const;
    bool operator != ( const SLogServerSettings& i_settingsOther ) const;
public: // struct methods
    QString toString() const;
public: // struct members
    /*!< Logging may be enabled or disabled for both writing to local log file and sending data to remote client. */
    bool m_bEnabled;
    /*!< Absolute file path the tree of logger objects and their settings will be saved and recalled.
         May be initialized by CLogServer::GetLoggerFileAbsoluteFilePath after the applications
         organization and application name have been set. */
    QString m_strLoggerFileAbsFilePath;
    /*!< Defines whether newly created loggers should be enabled as default. */
    bool m_bNewLoggersEnabledAsDefault;
    /*!< Defines the log detail level for newly created loggers. */
    ZS::System::ELogDetailLevel m_eNewLoggersDefaultDetailLevel;
    /*!< Defines whether log output should be send to remote client. */
    bool m_bUseIpcServer;
    /*!< If a log client is not connected the flag defines whether log data should be internally cached until a client connects. */
    bool m_bCacheDataIfNotConnected;
    /*!< If caching is enabled defines the maximum number of log entries which should be locally cached. */
    int m_iCacheDataMaxArrLen;
    /*!< Defines whether log output should be written to a local log file. */
    bool m_bUseLocalLogFile;
    /*!< If a local log file is used defines the absolute file path for the log file.
         May be initialized by CLogServer::GetLocalLogFileAbsoluteFilePath after the applications
         organization and application name have been set. */
    QString m_strLocalLogFileAbsFilePath;
    /*!< Auto save interval for the local log file. */
    int m_iLocalLogFileAutoSaveInterval_ms;
    /*!< Number of sub files to be created for round robin. The oldest log file will be overwritten. */
    int m_iLocalLogFileSubFileCountMax;
    /*!< Number of lines which can be written to a log file before the file is closed and the next sub log file is created. */
    int m_iLocalLogFileSubFileLineCountMax;
    /*!< For hard to find errors the log file may be immediately closed after an entry has been written.
         Use with special care as enabling this feature extremely slows down the program. */
    bool m_bLocalLogFileCloseFileAfterEachWrite;

}; // struct SLogServerSettings


//******************************************************************************
/*! @brief All log outputs should be made via the CLogServer class.

The class manages a tree of loggers, which can be used to activate and
deactivate the logging for modules, classes and instances and to specify the
level of detail of the output.

These Logger objects can be asked whether a log output should take place
and which log outputs should be created.

There is only one Log Server instance per application, which is created when
the application is started by calling the "CreateInstance" class method.

During program execution, a reference to the instance can be obtained via
"GetInstance" and parameters can be changed or log outputs can be made in the
log file.

Before exiting the application, the Log Server instance must be deleted again
with "ReleaseInstance".
*/
class ZSSYSDLL_API CLogServer : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    static const int c_iStrLenThreadMax  = 24; /*!< Maximum number of characters used for thread names. */
    static const int c_iStrLenSysTimeMax = 15; /*!< Maximum number of characters used for the system time.
                                                    The maximum value is 99999999.999999 seconds (ca. 75 years). */
public: // class methods
    /*! Returns the namespace of the class.
        @note The static class functions name must be different from the instance method "nameSpace". */
    static QString NameSpace() { return "ZS::Log"; }
    /*! Returns the class name.
        @note The static class functions name must be different from the instance method "className". */
    static QString ClassName() { return "CLogServer"; }
public: // class methods
    static CLogServer* GetInstance();
    static CLogServer* CreateInstance();
    static void ReleaseInstance();
public: // class methods to register thread names
    static void RegisterCurrentThread(const QString& i_strThreadName);
    static void UnregisterCurrentThread();
    static QString GetCurrentThreadName();
public: // class methods to add, remove and modify admin objects
    static CIdxTreeLoggers* GetLoggersIdxTree();
    static CLogger* GetLogger( int i_idxInTree );
    static CLogger* GetLogger(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName = "" );
    static CLogger* GetLogger(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        ZS::System::EEnabled i_bEnabledAsDefault,
        ZS::System::ELogDetailLevel i_eDefaultDetailLevel );
    static void RenameLogger( CLogger** io_ppLogger, const QString& i_strNewObjName );
    static void ReleaseLogger( CLogger* i_pLogger );
public: // class methods to get default file paths
    static void SetLoggerFileAbsoluteFilePath( const QString& i_strAbsFilePath );
    static QString GetLoggerFileAbsoluteFilePath();
    static QString GetLoggerFileCompleteBaseName();
    static QString GetLoggerFileAbsolutePath();
    static void SetLocalLogFileAbsoluteFilePath( const QString& i_strAbsFilePath );
    static QString GetLocalLogFileAbsoluteFilePath();
    static QString GetLocalLogFileCompleteBaseName();
    static QString GetLocalLogFileAbsolutePath();
protected: // ctors and dtor
    CLogServer();
    virtual ~CLogServer();
signals:
    /*! Signal which is emitted if a log setting has been changed.
        @param i_pLogServer [in] Pointer to object emitting the signal. */
    void logSettingsChanged( QObject* i_pLogServer );
public: // instance methods
    /*! Returns the namespace of the class. May be overriden to return the namespace of the derived class. */
    QString nameSpace() const { return NameSpace(); }
    /*! Returns the class name. May be overriden to return the class name of the derived class. */
    QString className() const { return ClassName(); }
public: // instance methods
    CIdxTreeLoggers* getLoggersIdxTree();
public: // instance methods to add, remove and modify admin objects
    virtual CLogger* getLogger( int i_idxInTree );
    virtual CLogger* getLogger(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        ZS::System::EEnabled i_bEnabledAsDefault,
        ZS::System::ELogDetailLevel i_eDefaultDetailLevel );
    virtual void renameLogger( CLogger** io_ppLogger, const QString& i_strNewObjName );
    virtual void releaseLogger( CLogger* i_pLogger );
public: // instance methods
    virtual void log(
        const CLogger* i_pLogger,
        const QString& i_strMethod,
        const QString& i_strAddInfo );
    virtual void log(
        const CLogger* i_pLogger,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strAddInfo );
    virtual void log(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strAddInfo );
public: // instance methods
    virtual bool isActive() const;
    virtual bool isActive( const CLogger* i_pLogger ) const;
public: // instance methods
    virtual void setEnabled( bool i_bEnabled );
    virtual bool isEnabled() const;
public: // instance methods
    virtual void setNewLoggersEnabledAsDefault( bool i_bEnabled );
    virtual bool areNewLoggersEnabledAsDefault() const;
    virtual void setNewLoggersDefaultDetailLevel( ZS::System::ELogDetailLevel i_eDetailLevel );
    virtual ZS::System::ELogDetailLevel getNewLoggersDefaultDetailLevel() const;
protected: // instance methods
    virtual void setLoggerFileAbsoluteFilePath( const QString& i_strAbsFilePath );
    virtual QString getLoggerFileCompleteBaseName() const;
    virtual QString getLoggerFileAbsolutePath() const;
public: // instance methods
    virtual ZS::System::SErrResultInfo recallLoggers( const QString& i_strAbsFilePath = QString() );
    virtual ZS::System::SErrResultInfo saveLoggers( const QString& i_strAbsFilePath = QString() );
protected: // instance methods
    virtual void setLocalLogFileAbsoluteFilePath( const QString& i_strAbsFilePath );
    virtual QString getLocalLogFileCompleteBaseName() const;
    virtual QString getLocalLogFileAbsolutePath() const;
public: // instance methods
    virtual void setUseLocalLogFile( bool i_bUse );
    virtual bool isLocalLogFileUsed() const;
    virtual bool isLocalLogFileActive() const;
    virtual CLogFile* getLocalLogFile();
    virtual void setLocalLogFileAutoSaveIntervalInMs( int i_iAutoSaveInterval_ms );
    virtual int getLocalLogFileAutoSaveIntervalInMs() const;
    virtual void setLocalLogFileCloseFileAfterEachWrite( bool i_bCloseFile );
    virtual bool getLocalLogFileCloseFileAfterEachWrite() const;
public: // instance methods
    virtual void setLocalLogFileSubFileCountMax( int i_iCountMax );
    virtual int getLocalLogFileSubFileCountMax() const;
    virtual void setLocalLogFileSubFileLineCountMax( int i_iCountMax );
    virtual int getLocalLogFileSubFileLineCountMax() const;
public: // instance methods
    virtual void setUseIpcServer( bool i_bUse );
    virtual bool isIpcServerUsed() const;
    virtual void setCacheLogDataIfNotConnected( bool i_bCacheData );
    virtual bool getCacheLogDataIfNotConnected() const;
    virtual void setCacheLogDataMaxArrLen( int i_iMaxArrLen );
    virtual int getCacheLogDataMaxArrLen() const;
public: // instance methods
    virtual void setLogSettings( const SLogServerSettings& i_settings );
    virtual SLogServerSettings getLogSettings() const;
public: // instance methods
    virtual void clearLocalLogFile();
protected: // instance methods
    virtual void addEntry(
        const QString&   i_strThreadName,
        const QDateTime& i_dt,
        double           i_fSysTimeInSec,
        const QString&   i_strNameSpace,
        const QString&   i_strClassName,
        const QString&   i_strObjName,
        const QString&   i_strMethod,
        const QString&   i_strAddInfo = "" );
protected: // auxiliary instance methods
    static QString currentThreadName();
protected: // reference counter
    int getRefCount() const;
    int incrementRefCount();
    int decrementRefCount();
protected: // class members
    static QMutex      s_mtx;       /*!< Mutex to protect the class and instance members of the class for multithreaded access. */
    static CLogServer* s_pTheInst;  /*!< Pointer to singleton instance. */
    static QHash<Qt::HANDLE, QString>  s_hshThreadNames; /*!< Hash with registered threads (key is thread id, value is name of thread). */
    static QHash<QString, Qt::HANDLE>  s_hshThreadIds;   /*!< Hash with registered threads (key name of thread, value is thread id). */
    static QString s_strLoggerFileAbsFilePath;         /*!< Absolute file path the tree of loggers and their settings will be saved and recalled. */
    static QString s_strLocalLogFileAbsFilePath;         /*!< If a local log file is used defines the absolute file path for the log file. */
protected: // instance members
    /*<! Index tree containg a hierarchically ordered tree of the logger objects. */
    CIdxTreeLoggers* m_pLoggersIdxTree;
    /*<! Currently used log settings. */
    SLogServerSettings m_logSettings;
    /*<! Reference to local log file. */
    CLogFile* m_pLogFile;
    /*<! Reference counter for createInstance and releaseInstance. */
    int m_iRefCount;

}; // class CLogServer

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_LogServer_h
