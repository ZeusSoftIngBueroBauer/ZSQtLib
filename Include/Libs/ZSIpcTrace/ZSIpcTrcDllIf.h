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
enum ETraceDetailLevelMethodCalls
//==============================================================================
{
    ETraceDetailLevelMethodCallsNone         = 0, /*!< Trace output is disabled. */
    ETraceDetailLevelMethodCallsEnterLeave   = 1, /*!< Tracing method entries and leaves only
                            (without tracing input and output arguments and without method return value). */
    ETraceDetailLevelMethodCallsArgsNormal   = 2, /*!< Tracing method entries and leaves together with
                            input and output arguemnts and method return value. */
    ETraceDetailLevelMethodCallsArgsDetailed = 3, /*!< Tracing method entries and leaves together with
                            input and output arguements and method return value
                            with more details than normal (content of arrays). */
    ETraceDetailLevelMethodCallsArgsVerbose  = 4, /*!< Tracing method entries and leaves together with
                            input and output arguemnts and method return value
                            in a very detailed level (content of arrays). */
    ETraceDetailLevelMethodCallsUndefined    = 5,  /*!< Used e.g. to indicate that the trace level should not be used but the predefined detail level should be used. */
    ETraceDetailLevelMethodCallsCount
};

//==============================================================================
/*! Trace detail levels for tracing runtime info.

    @ingroup _GRP_Libs_ZSSys_MethodTracing

    Higher detail levels include lower detail levels.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum ETraceDetailLevelRuntimeInfo
//==============================================================================
{
    ETraceDetailLevelRuntimeInfoNone          =  0, /*!< Trace output of runtime info is disabled. */
    ETraceDetailLevelRuntimeInfoCritical      =  1, /*!< Log critical runtime info (critical errors, critical exceptions). */
    ETraceDetailLevelRuntimeInfoError         =  2, /*!< Log error runtime info (errors, exceptions). */
    ETraceDetailLevelRuntimeInfoWarning       =  3, /*!< Log warning runtime info. */
    ETraceDetailLevelRuntimeInfoInfoNormal    =  4, /*!< Log runtime info. */
    ETraceDetailLevelRuntimeInfoInfoDetailed  =  5, /*!< Log detailed runtime info. */
    ETraceDetailLevelRuntimeInfoInfoVerbose   =  6, /*!< Log very detailed runtime info. */
    ETraceDetailLevelRuntimeInfoDebugNormal   =  7, /*!< Output debug runtime info. */
    ETraceDetailLevelRuntimeInfoDebugDetailed =  8, /*!< Output detailed debug runtime info. */
    ETraceDetailLevelRuntimeInfoDebugVerbose  =  9, /*!< Output very detailed debug runtime info. */
    ETraceDetailLevelRuntimeInfoUndefined     = 10, /*!< Used e.g. to indicate that the trace level should not be used but the predefined detail level should be used. */
    ETraceDetailLevelRuntimeInfoCount
};

//******************************************************************************
/*! Instances of this class are used to filter method tracing.

    @ref ZS::Trace::CTrcAdminObj
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
    void setMethodCallsTraceDetailLevel( ETraceDetailLevelMethodCalls i_eTrcDetailLevel );
    ETraceDetailLevelMethodCalls getMethodCallsTraceDetailLevel() const;
    bool areMethodCallsActive( ETraceDetailLevelMethodCalls i_eFilterDetailLevel ) const;
    void setRuntimeInfoTraceDetailLevel( ETraceDetailLevelRuntimeInfo i_eTrcDetailLevel );
    ETraceDetailLevelRuntimeInfo getRuntimeInfoTraceDetailLevel() const;
    bool isRuntimeInfoActive( ETraceDetailLevelRuntimeInfo i_eFilterDetailLevel ) const;
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

    @ref ZS::Trace::CMethodTracer
*/
class CMethodTracer
//******************************************************************************
{
public: // ctors and dtor
    CMethodTracer(  // instance tracer (name space, class and object name set at trace admin object)
        CTrcAdminObj* i_pTrcAdminObj,
        ETraceDetailLevelMethodCalls i_eFilterDetailLevel,    // Entering and leaving the method is traced if the admin objects detail level is greater or equal than the filter setting than the detail level.
        const char* i_szMethod,
        const char* i_szMethodInArgs );
    CMethodTracer(  // class tracer (name space and class name (but not object name) set at trace admin object)
        CTrcAdminObj* i_pTrcAdminObj,
        ETraceDetailLevelMethodCalls i_eFilterDetailLevel,    // Entering and leaving the method is traced if the admin objects detail level is greater or equal than the filter setting than the detail level.
        const char* i_szObjName,
        const char* i_szMethod,
        const char* i_szMethodInArgs );
    ~CMethodTracer();
public: // instance methods
    void onAdminObjAboutToBeReleased();
    bool isTraceDataSuppressedByFilter( const char* i_szTraceData ) const;
public: // instance methods
    ETraceDetailLevelMethodCalls getMethodCallsTraceDetailLevel() const;
    bool areMethodCallsActive( ETraceDetailLevelMethodCalls i_eFilterDetailLevel ) const;
    ETraceDetailLevelMethodCalls getEnterLeaveFilterDetailLevel() const { return m_eEnterLeaveFilterDetailLevel; }
    ETraceDetailLevelRuntimeInfo getRuntimeInfoTraceDetailLevel() const;
    bool isRuntimeInfoActive( ETraceDetailLevelRuntimeInfo i_eFilterDetailLevel ) const;
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
    void trace( const char* i_szAddInfo, ETraceDetailLevelRuntimeInfo i_eFilterDetailLevel = ETraceDetailLevelRuntimeInfo::ETraceDetailLevelRuntimeInfoCritical ) const;
protected: // instance members
    CTrcAdminObj* m_pTrcAdminObj;
    ETraceDetailLevelMethodCalls m_eEnterLeaveFilterDetailLevel;
    bool m_bEnterTraced;
    char* m_szObjName;
    char* m_szMethod;
    char* m_szMethodReturn;
    char* m_szMethodOutArgs;

}; // class CMethodTracer


//******************************************************************************
/*! struct STrcServerSettings

    @ref ZS::Trace::STrcServerSettings
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

void STrcServerSettings_init( STrcServerSettings& i_trcSettings );
void STrcServerSettings_release( STrcServerSettings& i_trcSettings );


//******************************************************************************
/*! class CTrcServer

    @ref ZS::Trace::CTrcServer
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
        ETraceDetailLevelMethodCalls i_eMethodCallsDefaultDetailLevel = ETraceDetailLevelMethodCallsUndefined,
        ETraceDetailLevelRuntimeInfo i_eRuntimeInfoDefaultDetailLevel = ETraceDetailLevelRuntimeInfoUndefined );
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
    void setNewTrcAdminObjsMethodCallsDefaultDetailLevel( ETraceDetailLevelMethodCalls i_eDetailLevel );
    ETraceDetailLevelMethodCalls getNewTrcAdminObjsMethodCallsDefaultDetailLevel() const;
    void setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel( ETraceDetailLevelRuntimeInfo i_eDetailLevel );
    ETraceDetailLevelRuntimeInfo getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel() const;
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

    @ref ZS::Trace::CTrcServer
*/
class CIpcTrcServer : public CTrcServer
//******************************************************************************
{
public: // class methods
    static CIpcTrcServer* GetInstance();
    static CIpcTrcServer* CreateInstance(
        ETraceDetailLevelMethodCalls i_eTrcDetailLevelDllIf = ETraceDetailLevelMethodCallsNone,
        ETraceDetailLevelMethodCalls i_eTrcDetailLevelTrcServer = ETraceDetailLevelMethodCallsNone,
        ETraceDetailLevelMethodCalls i_eTrcDetailLevelTrcServerMutex = ETraceDetailLevelMethodCallsNone,
        ETraceDetailLevelMethodCalls i_eTrcDetailLevelTrcServerIpcServer = ETraceDetailLevelMethodCallsNone,
        ETraceDetailLevelMethodCalls i_eTrcDetailLevelTrcServerIpcServerMutex = ETraceDetailLevelMethodCallsNone,
        ETraceDetailLevelMethodCalls i_eTrcDetailLevelTrcServerIpcServerGateway = ETraceDetailLevelMethodCallsNone );
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
    int i_iQtVersionMajor = QT_VERSION_MAJOR );
const char* getDllFileName();
bool releaseDll();


} // namespace DllIf

} // namespace Trace

} // namespace ZS

} // extern "C"

#endif // #ifndef ZSIpcTrace_DllIf_h

#endif // #ifdef USE_ZS_IPCTRACE_DLL_IF
