/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

This file may be used with no license reszictions for your needs. But it is not
allowed to resell any modules of the ZSQtLib veiling the original developer of
the modules. Therefore the copyright link to ZeusSoft, Ing. Buero Bauer must not
be removed from the header of the source code modules.

ZeusSoft, Ing. Buero Bauer provides the source code as is without any guarantee
that the code is written without faults.

ZeusSoft, Ing. Buero Bauer does not assume any liability for any damages which
may result in using the software modules.

*******************************************************************************/

#ifdef USE_ZS_IPCLOG_DLL_IF

#ifndef ZSIpcLog_DllIf_h
#define ZSIpcLog_DllIf_h

/*! This header file represents the DLL interface for the remote logging
    of the ZSQtLib. In order to be able to use it, the cpp file must also be
    compiled into the application or the dll in which the interface is to be used.
*/

/*******************************************************************************
public type definitions and constants
*******************************************************************************/

#define __STR__(x) #x
#define __STRING__(x) __STR__(x)

#ifndef QT_VERSION_MAJOR
// Align with definitions in ZSSysDllMain.h
#if QT_VERSION >= 0x050000
#define QT_VERSION_MAJOR 5
#elif QT_VERSION >= 0x040804
#define QT_VERSION_MAJOR 4
#endif
#endif

#ifndef QT_VERSION_MAJOR
#define QT_VERSION_MAJOR 5
#endif

#ifndef CONFIGLIBINFIX
// Align with definitions in ZSSysDllMain.h
#ifdef _DEBUG
#define CONFIGLIBINFIX "d"
#else
#define CONFIGLIBINFIX ""
#endif
#endif

extern "C"
{
namespace ZS
{
namespace Log
{
namespace DllIf
{
class CLogServer;

//==============================================================================
/*! Indicates whether an optional value is enabled or disabled.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum EEnabled
//==============================================================================
{
    EEnabledNo        = 0, /*! Indicates that the option is disabled (alias for false). */
    EEnabledYes       = 1, /*! Indicates that the option is enabled (alias for true). */
    EEnabledUndefined = 2  /*! The variable is not yet initialised and its value is unknown. */
};

//==============================================================================
/*! Log detail levels.

    Higher detail levels include lower detail levels.
*/
enum ELogDetailLevel
//==============================================================================
{
    ELogDetailLevelNone          =  0, /*!< Trace output of runtime info is disabled. */
    ELogDetailLevelCritical      =  1, /*!< Log critical runtime info (critical errors, critical exceptions). */
    ELogDetailLevelError         =  2, /*!< Log error runtime info (errors, exceptions). */
    ELogDetailLevelWarning       =  3, /*!< Log warning runtime info. */
    ELogDetailLevelInfoNormal    =  4, /*!< Log runtime info. */
    ELogDetailLevelInfoDetailed  =  5, /*!< Log detailed runtime info. */
    ELogDetailLevelInfoVerbose   =  6, /*!< Log very detailed runtime info. */
    ELogDetailLevelDebugNormal   =  7, /*!< Output debug runtime info. */
    ELogDetailLevelDebugDetailed =  8, /*!< Output detailed debug runtime info. */
    ELogDetailLevelDebugVerbose  =  9, /*!< Output very detailed debug runtime info. */
    ELogDetailLevelUndefined     = 10, /*!< Used e.g. to indicate that the trace level should not be used but the predefined detail level should be used. */
    ELogDetailLevelCount
};

//******************************************************************************
/*! Instances of this class are used to filter log outputs.

    @ref ZS::System::CLogger
*/
class CLogger
//******************************************************************************
{
public: // instance methods
    virtual const char* keyInTree() const = 0;
public: // instance methods
    char* getNameSpace() const;     // returned character string must be freed by caller
    char* getClassName() const;     // returned character string must be freed by caller
    char* getObjectName() const;    // returned character string must be freed by caller
public: // instance methods
    void setObjectThreadName( const char* i_szThreadName );
    char* getObjectThreadName() const;  // returned character string must be freed by caller
public: // instance methods
    virtual int lock() = 0;
    virtual int unlock() = 0;
    virtual bool isLocked() const = 0;
    virtual int getLockCount() const = 0;
    virtual void setDeleteOnUnlock( bool i_bDelete ) = 0;
    virtual bool deleteOnUnlock() const = 0;
public: // instance methods
    virtual int incrementRefCount() = 0;
    virtual int decrementRefCount() = 0;
    virtual void setRefCount( int i_iRefCount ) = 0;
    virtual int getRefCount() const = 0;
public: // instance methods
    void setEnabled( bool i_bEnabled );
    bool isEnabled() const;
public: // instance methods
    void setDetailLevel( ELogDetailLevel i_eTrcDetailLevel );
    ELogDetailLevel getDetailLevel() const;
    bool isActive( ELogDetailLevel i_eFilterDetailLevel ) const;
public: // instance methods
    void setDataFilter( const char* i_szFilter );
    char* getDataFilter() const;  // returned character string must be freed by caller
    bool isDataSuppressedByFilter( const char* i_szTraceData ) const;
protected: // ctors and dtor
    CLogger();
    ~CLogger();

}; // class CLogger


//******************************************************************************
/*! struct SLogServerSettings

    @ref ZS::System::SLogServerSettings
*/
struct SLogServerSettings
//******************************************************************************
{
    /*!< Logging may be enabled or disabled for both writing to local log file and sending data to remote client. */
    bool m_bEnabled;
    /*!< Absolute file path the tree of logger objects and their settings will be saved and recalled.
         May be initialized by CLogServer::GetLoggerFileAbsoluteFilePath after the applications
         organization and application name have been set. */
    char* m_szLoggerFileAbsFilePath;
    /*!< Defines whether newly created loggers should be enabled as default. */
    bool m_bNewLoggersEnabledAsDefault;
    /*!< Defines the log detail level for newly created loggers. */
    ELogDetailLevel m_eNewLoggersDefaultDetailLevel;
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
    char* m_szLocalLogFileAbsFilePath;
    /*!< Auto save interval for the local log file. */
    int m_iLocalLogFileAutoSaveInterval_ms;
    /*!< Number of sub files to be created for round robin. The oldest log file will be overwritten. */
    int m_iLocalLogFileSubFileCountMax;
    /*!< Number of lines which can be written to a log file before the file is closed and the next sub log file is created. */
    int m_iLocalLogFileSubFileLineCountMax;
    /*!< For hard to find errors the log file may be immediately closed after an entry has been written.
         Use with special care as enabling this feature extremely slows down the program. */
    bool m_bLocalLogFileCloseFileAfterEachWrite;
};

void SLogServerSettings_init( SLogServerSettings& i_logSettings );
void SLogServerSettings_release( SLogServerSettings& i_logSettings );


//******************************************************************************
/*! class CLogServer

    @ref ZS::Log::CLogServer
*/
class CLogServer
//******************************************************************************
{
public: // class methods to add, remove and modify logger
    static CLogger* GetLogger(
        const char* i_szNameSpace,
        const char* i_szClassName,
        const char* i_szObjName = "",
        EEnabled    i_bEnabledAsDefault = EEnabledUndefined,
        ELogDetailLevel i_eDefaultDetailLevel = ELogDetailLevelUndefined );
    static void RenameLogger( CLogger** io_ppLogger, const char* i_szNewObjName );
    static void ReleaseLogger( CLogger* i_pLogger );
public: // class method to save/recall loggers file
    // Set organization and application if the log server is used in a none Qt Application.
    // Only if the organization and application name is set the default file paths for the
    // loggers XML file and the log file may be correctly determined.
    static void SetOrganizationName( const char* i_szName );
    static char* GetOrganizationName(); // returned string must be freed by caller
    static void SetApplicationName( const char* i_szName );
    static char* GetApplicationName();  // returned string must be freed by caller
    static void SetLoggerFileAbsoluteFilePath( const char* i_szAbsFilePath );
    static char* GetLoggerFileAbsoluteFilePath();
    static char* GetLoggerFileCompleteBaseName();
    static char* GetLoggerFileAbsolutePath();
    static void SetLocalLogFileAbsoluteFilePath( const char* i_szAbsFilePath );
    static char* GetLocalLogFileAbsoluteFilePath();
    static char* GetLocalLogFileCompleteBaseName();
    static char* GetLocalLogFileAbsolutePath();
public: // class methods
    static void RegisterCurrentThread( const char* i_szThreadName );
    static void UnregisterCurrentThread();
    static char* GetCurrentThreadName();    // returned string must be freed by caller
public: // instance methods
    const char* name() const { return "ZSLogServer"; }
public: // instance methods
    bool isActive() const;
public: // instance methods
    void setEnabled( bool i_bEnabled );
    bool isEnabled() const;
public: // instance methods
    void setNewLoggersEnabledAsDefault( bool i_bEnabled );
    bool areNewLoggersEnabledAsDefault() const;
    void setNewLoggersDefaultDetailLevel( ELogDetailLevel i_eDetailLevel );
    ELogDetailLevel getNewLoggersDefaultDetailLevel() const;
public: // instance methods
    bool recallLoggers();
    bool saveLoggers();
public: // instance methods
    void setUseLocalLogFile( bool i_bUse );
    bool isLocalLogFileUsed() const;
    bool isLocalLogFileActive() const;
    void setLocalLogFileAutoSaveIntervalInMs( int i_iAutoSaveInterval_ms );
    int getLocalLogFileAutoSaveIntervalInMs() const;
    void setLocalLogFileCloseFileAfterEachWrite( bool i_bCloseFile );
    bool getLocalLogFileCloseFileAfterEachWrite() const;
public: // instance methods
    void setLocalLogFileSubFileCountMax( int i_iCountMax );
    int getLocalLogFileSubFileCountMax() const;
    void setLocalLogFileSubFileLineCountMax( int i_iCountMax );
    int getLocalLogFileSubFileLineCountMax() const;
public: // instance methods
    void setUseIpcServer( bool i_bUse );
    bool isIpcServerUsed() const;
    void setCacheLogDataIfNotConnected( bool i_bCacheData );
    bool getCacheLogDataIfNotConnected() const;
    void setCacheLogDataMaxArrLen( int i_iMaxArrLen );
    int getCacheLogDataMaxArrLen() const;
public: // instance methods
    bool setLogSettings( const SLogServerSettings& i_settings );
    SLogServerSettings getLogSettings() const;  // !! char pointers returned in the struct must be freed by caller !!
public: // instance methods
    void clearLocalLogFile();
public: // instance methods
    void log(
        const CLogger* i_pLogger,
        const char*    i_szMethod,
        const char*    i_szAddInfo );
    void log(
        const CLogger* i_pLogger,
        const char*    i_szObjName,
        const char*    i_szMethod,
        const char*    i_szAddInfo );
public: // ctors and dtor (declared public but for internal use only, implemented in ZSIpcLog::ZSIpcLogDllMain)
    CLogServer();
    ~CLogServer();

}; // class CLogServer


//******************************************************************************
/*! class CIpcLogServer

    @ref ZS::Log::CIpcLogServer
*/
class CIpcLogServer : public CLogServer
//******************************************************************************
{
public: // class methods
    static CIpcLogServer* GetInstance();
    static CIpcLogServer* CreateInstance();
    static void ReleaseInstance( CIpcLogServer* i_pLogServer );
public: // instance methods
    bool startup( int i_iTimeout_ms = 5000, bool i_bWait = true );
    bool shutdown( int i_iTimeout_ms = 5000, bool i_bWait = true );
public: // instance methods
    bool isListening() const;
    bool isConnected() const;
public: // instance methods changing and reading the host settings
    bool setPort( unsigned short i_uPort, int i_iTimeout_ms = 5000, bool i_bWait = true);
    unsigned short getPort() const;
public: // ctors and dtor (declared public but for internal use only, implemented in ZSIpcLog::ZSIpcLogDllMain)
    CIpcLogServer();
    ~CIpcLogServer();

}; // class CIpcLogServer

//==============================================================================
/*! Selects the config type (Debug or Release) which has been used to compile.
*/
enum EBuildConfiguration
//==============================================================================
{
    EBuildConfigurationRelease    =  0, /*! Debug built was used. */
    EBuildConfigurationDebug      =  1, /*! Release built was used. */
    EBuildConfigurationAutoDetect =  2  /*! The config type should be automatically detected. */
};

bool loadDll(
    EBuildConfiguration i_configuration = EBuildConfigurationAutoDetect,
    int i_iQtVersionMajor = QT_VERSION_MAJOR );
const char* getDllFileName();
bool releaseDll();

} // namespace DllIf

} // namespace Log

} // namespace ZS

} // extern "C"

#endif // #ifndef ZSIpcLog_DllIf_h

#endif // #ifdef USE_ZS_IPCLOG_DLL_IF
