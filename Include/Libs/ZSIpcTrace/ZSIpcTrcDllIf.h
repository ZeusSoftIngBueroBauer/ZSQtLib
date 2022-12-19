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

#ifdef USE_ZS_IPCTRACE_DLL_IF

#ifndef ZSIpcTrace_DllIf_h
#define ZSIpcTrace_DllIf_h

#pragma warning( push )
#pragma warning( disable : 4505 )

/*! This header file represents the DLL interface for the remote method tracing
    of the ZSQtLib. In order to be able to use it, the cpp file must also be
    compiled into the application or the dll in which the interface is to be used.

    @ref _PAGE_Overview_MethodTracing
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
namespace Trace
{
namespace DllIf
{
class CTrcServer;

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
/*! Trace detail levels for tracing method calls.

    @ingroup _GRP_Libs_ZSSys_MethodTracing

    Higher detail levels include lower detail levels.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum EMethodTraceDetailLevel
//==============================================================================
{
    EMethodTraceDetailLevelNone         = 0, /*!< Method trace output is disabled. */
    EMethodTraceDetailLevelEnterLeave   = 1, /*!< Tracing entering and leaving methods only
                                                  (without tracing input and output arguments and
                                                  without method return value). */
    EMethodTraceDetailLevelArgsNormal   = 2, /*!< Tracing entering and leaving methods together with
                                                  input and output arguemnts and method return value. */
    EMethodTraceDetailLevelArgsDetailed = 3, /*!< Tracing entering and leaving methods together with
                                                  input and output arguements and method return value
                                                  with more details than normal (content of arrays). */
    EMethodTraceDetailLevelArgsVerbose  = 4, /*!< Tracing entering and leaving methods together with
                                                  input and output arguemnts and method return value
                                                  in a very detailed level (content of arrays). */
    EMethodTraceDetailLevelUndefined    = 5, /*!< Used e.g. to indicate that the trace level should not be
                                                  used but the predefined detail level should be used. */
    EMethodTraceDetailLevelCount
};

//==============================================================================
/*! Trace detail levels for tracing runtime info.

    @ingroup _GRP_Libs_ZSSys_MethodTracing

    Higher detail levels include lower detail levels.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum ELogDetailLevel
//==============================================================================
{
    ELogDetailLevelNone          = 0, /*!< Log outputs are disabled. */
    ELogDetailLevelFatal         = 1, /*!< Log fatal errors (critical errors, critical exceptions). */
    ELogDetailLevelError         = 2, /*!< At this level all error conditions should be logged. */
    ELogDetailLevelWarning       = 3, /*!< At the Warning level all events that could potentially
                                           become an error should be logged. */
    ELogDetailLevelNotice        = 4, /*!< At this level all events should be logged which are
                                           considered to be notable but are not an error. */
    ELogDetailLevelInfo          = 5, /*!< The Info level is usually used to log all actions that
                                           are user-driven or system specific. */
    ELogDetailLevelDebug         = 6, /*!< The Debug level is usually used to log anything that
                                           happens in the program for debugging purposes. */
    ELogDetailLevelDebugDetailed = 7, /*!< Output detailed debug runtime info. */
    ELogDetailLevelDebugVerbose  = 8, /*!< Output very detailed debug runtime info. */
    ELogDetailLevelUndefined     = 9, /*!< Used e.g. to indicate that the log level should not be used
                                           but the predefined detail level should be used. */
    ELogDetailLevelCount
};

//******************************************************************************
/*! Instances of this class are used to filter method tracing.

    @ref ZS::System::CTrcAdminObj
*/
class CTrcAdminObj
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
    void setMethodCallsTraceDetailLevel( EMethodTraceDetailLevel i_eTrcDetailLevel );
    EMethodTraceDetailLevel getMethodCallsTraceDetailLevel() const;
    bool areMethodCallsActive( EMethodTraceDetailLevel i_eFilterDetailLevel ) const;
    void setRuntimeInfoTraceDetailLevel( ELogDetailLevel i_eTrcDetailLevel );
    ELogDetailLevel getRuntimeInfoTraceDetailLevel() const;
    bool isRuntimeInfoActive( ELogDetailLevel i_eFilterDetailLevel ) const;
public: // instance methods
    void setTraceDataFilter( const char* i_szFilter );
    char* getTraceDataFilter() const;  // returned character string must be freed by caller
    bool isTraceDataSuppressedByFilter( const char* i_szTraceData ) const;
protected: // ctors and dtor
    CTrcAdminObj();
    ~CTrcAdminObj();

}; // class CTrcAdminObj


//******************************************************************************
/*! class CMethodTracer

    @ref ZS::System::CMethodTracer
*/
class CMethodTracer
//******************************************************************************
{
public: // ctors and dtor
    CMethodTracer(  // instance tracer (name space, class and object name set at trace admin object)
        CTrcAdminObj* i_pTrcAdminObj,
        EMethodTraceDetailLevel i_eFilterDetailLevel,    // Entering and leaving the method is traced if the admin objects detail level is greater or equal than the filter setting than the detail level.
        const char* i_szMethod,
        const char* i_szMethodInArgs );
    CMethodTracer(  // class tracer (name space and class name (but not object name) set at trace admin object)
        CTrcAdminObj* i_pTrcAdminObj,
        EMethodTraceDetailLevel i_eFilterDetailLevel,    // Entering and leaving the method is traced if the admin objects detail level is greater or equal than the filter setting than the detail level.
        const char* i_szObjName,
        const char* i_szMethod,
        const char* i_szMethodInArgs );
    ~CMethodTracer();
public: // instance methods
    void onAdminObjAboutToBeReleased();
    bool isTraceDataSuppressedByFilter( const char* i_szTraceData ) const;
public: // instance methods
    EMethodTraceDetailLevel getMethodCallsTraceDetailLevel() const;
    bool areMethodCallsActive( EMethodTraceDetailLevel i_eFilterDetailLevel ) const;
    EMethodTraceDetailLevel getEnterLeaveFilterDetailLevel() const { return m_eEnterLeaveFilterDetailLevel; }
    ELogDetailLevel getRuntimeInfoTraceDetailLevel() const;
    bool isRuntimeInfoActive( ELogDetailLevel i_eFilterDetailLevel ) const;
public: // instance methods
    const char* getObjectName() const { return m_szObjName; } // This is NOT the object name of the trace admin object
    const char* getMethod() const { return m_szMethod; }
public: // instance methods
    void setMethodReturn( bool i_bResult );
    void setMethodReturn( int i_iResult );
    void setMethodReturn( const char* i_sz );
    char* getMethodReturn() const { return m_szMethodReturn; }
public: // instance methods
    void setMethodOutArgs( const char* i_sz );
    char* getMethodOutArgs() const { return m_szMethodOutArgs; }
public: // instance methods
    void trace( const char* i_szAddInfo, ELogDetailLevel i_eFilterDetailLevel = ELogDetailLevelFatal ) const;
protected: // instance members
    CTrcAdminObj* m_pTrcAdminObj;
    EMethodTraceDetailLevel m_eEnterLeaveFilterDetailLevel;
    bool m_bEnterTraced;
    char* m_szObjName;
    char* m_szMethod;
    char* m_szMethodReturn;
    char* m_szMethodOutArgs;

}; // class CMethodTracer


//******************************************************************************
/*! struct STrcServerSettings

    @ref ZS::System::STrcServerSettings
*/
struct STrcServerSettings
//******************************************************************************
{
    bool  m_bEnabled;                               /*!< Tracing may be enabled or disabled for both writing to local trace file and sending data to remote client. */
    char* m_szAdminObjFileAbsFilePath;              /*!< Absolute file path the tree of trace admin objects and their settings will be saved and recalled. */
    bool  m_bNewTrcAdminObjsEnabledAsDefault;       /*!< Defines whether newly created trace admin objects should be enabled as default. */
    int   m_iNewTrcAdminObjsMethodCallsDefaultDetailLevel;     /*!< Defines the trace detail level for method calls of newly created trace admin objects. */
    int   m_iNewTrcAdminObjsRuntimeInfoDefaultDetailLevel;     /*!< Defines the trace detail level for runtime info of newly created trace admin objects. */
    bool  m_bUseIpcServer;                          /*!< Defines whether trace output should be send to remote client. */
    bool  m_bCacheDataIfNotConnected;               /*!< If a trace client is not connect the flag defines whether trace data should be internally cached until a client connects. */
    int   m_iCacheDataMaxArrLen;                    /*!< If caching is enabled defines the maximum number of trace entries which should be locally cached. */
    bool  m_bUseLocalTrcFile;                       /*!< Defines whether trace output should be written to a local trace file. */
    char* m_szLocalTrcFileAbsFilePath;              /*!< If a local log file is used defines the absolute file path for the log file. */
    int   m_iLocalTrcFileAutoSaveInterval_ms;       /*!< Auto save interval for the local log file. */
    int   m_iLocalTrcFileSubFileCountMax;           /*!< Number of sub files to be created for round robin. The oldest log file will be overwritten. */
    int   m_iLocalTrcFileSubFileLineCountMax;       /*!< Number of lines which can be written to a log file before the file is closed and the next sub log file is created. */
    bool  m_bLocalTrcFileCloseFileAfterEachWrite;   /*!< For hard to find errors the log file may be immediately closed after an entry has been written.
                                                         Use with special care as enabling this feature extremely slows down the program. */
}; // struct STrcServerSettings

//------------------------------------------------------------------------------
/*! @brief Initializes the trace settings with default values.

    @param i_trcSettings [in] Struct to be initialized.
*/
static void STrcServerSettings_init( DllIf::STrcServerSettings& i_trcSettings )
//------------------------------------------------------------------------------
{
    i_trcSettings.m_bEnabled = true;
    i_trcSettings.m_szAdminObjFileAbsFilePath = 0;
    i_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault = false;
    i_trcSettings.m_iNewTrcAdminObjsMethodCallsDefaultDetailLevel = 0;
    i_trcSettings.m_iNewTrcAdminObjsRuntimeInfoDefaultDetailLevel = 0;
    i_trcSettings.m_bUseIpcServer = true;
    i_trcSettings.m_bCacheDataIfNotConnected = false;
    i_trcSettings.m_iCacheDataMaxArrLen = 1000;
    i_trcSettings.m_bUseLocalTrcFile = true;
    i_trcSettings.m_szLocalTrcFileAbsFilePath = 0;
    i_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms = 1000;
    i_trcSettings.m_iLocalTrcFileSubFileCountMax = 5;
    i_trcSettings.m_iLocalTrcFileSubFileLineCountMax = 2000;
    i_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite = false;
}

//------------------------------------------------------------------------------
/*! @brief Resets the trace settings. The character buffers containing file names
           will be freed.

    @param i_trcSettings [in] Struct to be reset.
*/
static void STrcServerSettings_release( DllIf::STrcServerSettings& i_trcSettings )
//------------------------------------------------------------------------------
{
    i_trcSettings.m_bEnabled = false;
    delete i_trcSettings.m_szAdminObjFileAbsFilePath;
    i_trcSettings.m_szAdminObjFileAbsFilePath = 0;
    i_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault = false;
    i_trcSettings.m_iNewTrcAdminObjsMethodCallsDefaultDetailLevel = 0;
    i_trcSettings.m_iNewTrcAdminObjsRuntimeInfoDefaultDetailLevel = 1;
    i_trcSettings.m_bUseIpcServer = false;
    i_trcSettings.m_bCacheDataIfNotConnected = false;
    i_trcSettings.m_iCacheDataMaxArrLen = 0;
    i_trcSettings.m_bUseLocalTrcFile = false;
    delete i_trcSettings.m_szLocalTrcFileAbsFilePath;
    i_trcSettings.m_szLocalTrcFileAbsFilePath = 0;
    i_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms = 0;
    i_trcSettings.m_iLocalTrcFileSubFileCountMax = 0;
    i_trcSettings.m_iLocalTrcFileSubFileLineCountMax = 0;
    i_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite = false;
}


//******************************************************************************
/*! class CTrcServer

    @ref ZS::System::CTrcServer
*/
class CTrcServer
//******************************************************************************
{
public: // class methods to add, remove and modify admin objects
    static CTrcAdminObj* GetTraceAdminObj(
        const char* i_szNameSpace,
        const char* i_szClassName,
        const char* i_szObjName = "",
        EEnabled    i_bEnabledAsDefault = EEnabledUndefined,
        EMethodTraceDetailLevel i_eMethodCallsDefaultDetailLevel = EMethodTraceDetailLevelUndefined,
        ELogDetailLevel i_eRuntimeInfoDefaultDetailLevel = ELogDetailLevelUndefined );
    static void RenameTraceAdminObj( CTrcAdminObj** io_ppTrcAdminObj, const char* i_szNewObjName );
    static void ReleaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj );
public: // class method to save/recall admin objects file
    // Set organization and application if the trace server is used in a none Qt Application.
    // Only if the organization and application name is set the default file paths for the
    // trace admin objects and the trace log files may be correctly determined.
    static void SetOrganizationName( const char* i_szName );
    static char* GetOrganizationName(); // returned string must be freed by caller
    static void SetApplicationName( const char* i_szName );
    static char* GetApplicationName();  // returned string must be freed by caller
    static void SetAdminObjFileAbsoluteFilePath( const char* i_szAbsFilePath );
    static char* GetAdminObjFileAbsoluteFilePath();
    static char* GetAdminObjFileCompleteBaseName();
    static char* GetAdminObjFileAbsolutePath();
    static void SetLocalTrcFileAbsoluteFilePath( const char* i_szAbsFilePath );
    static char* GetLocalTrcFileAbsoluteFilePath();
    static char* GetLocalTrcFileCompleteBaseName();
    static char* GetLocalTrcFileAbsolutePath();
public: // class methods
    static void RegisterCurrentThread( const char* i_szThreadName );
    static void UnregisterCurrentThread();
    static char* GetCurrentThreadName();    // returned string must be freed by caller
public: // instance methods
    const char* name() const { return "ZSTrcServer"; }
public: // instance methods
    bool isActive() const;
public: // instance methods
    void setEnabled( bool i_bEnabled );
    bool isEnabled() const;
public: // instance methods
    void setNewTrcAdminObjsEnabledAsDefault( bool i_bEnabled );
    bool areNewTrcAdminObjsEnabledAsDefault() const;
    void setNewTrcAdminObjsMethodCallsDefaultDetailLevel( EMethodTraceDetailLevel i_eDetailLevel );
    EMethodTraceDetailLevel getNewTrcAdminObjsMethodCallsDefaultDetailLevel() const;
    void setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel( ELogDetailLevel i_eDetailLevel );
    ELogDetailLevel getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel() const;
public: // instance methods
    bool recallAdminObjs();
    bool saveAdminObjs();
public: // instance methods
    void setUseLocalTrcFile( bool i_bUse );
    bool isLocalTrcFileUsed() const;
    bool isLocalTrcFileActive() const;
    void setLocalTrcFileAutoSaveIntervalInMs( int i_iAutoSaveInterval_ms );
    int getLocalTrcFileAutoSaveIntervalInMs() const;
    void setLocalTrcFileCloseFileAfterEachWrite( bool i_bCloseFile );
    bool getLocalTrcFileCloseFileAfterEachWrite() const;
public: // instance methods
    void setLocalTrcFileSubFileCountMax( int i_iCountMax );
    int getLocalTrcFileSubFileCountMax() const;
    void setLocalTrcFileSubFileLineCountMax( int i_iCountMax );
    int getLocalTrcFileSubFileLineCountMax() const;
public: // instance methods
    void setUseIpcServer( bool i_bUse );
    bool isIpcServerUsed() const;
    void setCacheTrcDataIfNotConnected( bool i_bCacheData );
    bool getCacheTrcDataIfNotConnected() const;
    void setCacheTrcDataMaxArrLen( int i_iMaxArrLen );
    int getCacheTrcDataMaxArrLen() const;
public: // instance methods
    bool setTraceSettings( const STrcServerSettings& i_settings );
    STrcServerSettings getTraceSettings() const;  // !! char pointers returned in the struct must be freed by caller !!
public: // instance methods
    void clearLocalTrcFile();
public: // instance methods
    void traceMethodEnter(
        const CTrcAdminObj* i_pAdminObj,
        const char*         i_szMethod,
        const char*         i_szMethodInArgs );
    void traceMethodEnter(
        const CTrcAdminObj* i_pAdminObj,
        const char*         i_szObjName,
        const char*         i_szMethod,
        const char*         i_szMethodInArgs );
    void traceMethod(
        const CTrcAdminObj* i_pAdminObj,
        const char*         i_szMethod,
        const char*         i_szAddInfo );
    void traceMethod(
        const CTrcAdminObj* i_pAdminObj,
        const char*         i_szObjName,
        const char*         i_szMethod,
        const char*         i_szAddInfo );
    void traceMethodLeave(
        const CTrcAdminObj* i_pAdminObj,
        const char*         i_szMethod,
        const char*         i_szMethodReturn,
        const char*         i_szMethodOutArgs );
    void traceMethodLeave(
        const CTrcAdminObj* i_pAdminObj,
        const char*         i_szObjName,
        const char*         i_szMethod,
        const char*         i_szMethodReturn,
        const char*         i_szMethodOutArgs );
public: // ctors and dtor (declared public but for internal use only, implemented in ZSIpcTrace::ZSIpcTrcDllMain)
    CTrcServer();
    ~CTrcServer();

}; // class CTrcServer


//******************************************************************************
/*! class CIpcTrcServer

    @ref ZS::Trace::CIpcTrcServer
*/
class CIpcTrcServer : public CTrcServer
//******************************************************************************
{
public: // class methods
    static CIpcTrcServer* GetInstance();
    static CIpcTrcServer* CreateInstance(
        EMethodTraceDetailLevel i_eTrcDetailLevelDllIf = EMethodTraceDetailLevelNone,
        EMethodTraceDetailLevel i_eTrcDetailLevelTrcServer = EMethodTraceDetailLevelNone,
        EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerMutex = EMethodTraceDetailLevelNone,
        EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerIpcServer = EMethodTraceDetailLevelNone,
        EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerIpcServerMutex = EMethodTraceDetailLevelNone,
        EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerIpcServerGateway = EMethodTraceDetailLevelNone );
    static void ReleaseInstance( CIpcTrcServer* i_pTrcServer );
public: // instance methods
    bool startup( int i_iTimeout_ms = 5000, bool i_bWait = true );
    bool shutdown( int i_iTimeout_ms = 5000, bool i_bWait = true );
public: // instance methods
    bool isListening() const;
    bool isConnected() const;
public: // instance methods changing and reading the host settings
    bool setPort( unsigned short i_uPort, int i_iTimeout_ms = 5000, bool i_bWait = true);
    unsigned short getPort() const;
public: // ctors and dtor (declared public but for internal use only, implemented in ZSIpcTrace::ZSIpcTrcDllMain)
    CIpcTrcServer();
    ~CIpcTrcServer();

}; // class CIpcTrcServer

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
    int i_iQtVersionMajor = QT_VERSION_MAJOR,
    const char* i_szQtLibInfix = nullptr);
const char* getDllFileName();
bool releaseDll();


} // namespace DllIf

} // namespace Trace

} // namespace ZS

} // extern "C"

#pragma warning( pop )

#endif // #ifndef ZSIpcTrace_DllIf_h

#endif // #ifdef USE_ZS_IPCTRACE_DLL_IF
