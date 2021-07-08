/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSIpcTrace_DllIf_h
#define ZSIpcTrace_DllIf_h

/*******************************************************************************
public type definitions and constants
*******************************************************************************/

#ifndef QT_VERSION_MAJOR
#if QT_VERSION >= 0x050000
#define QT_VERSION_MAJOR 5
#elif QT_VERSION >= 0x040804
#define QT_VERSION_MAJOR 4
#endif
#endif

/*
Visual Studio version            | _MSC_VER
---------------------------------+---------------
Visual Studio 6.0                | 1200
Visual Studio .NET 2002 (7.0)    | 1300
Visual Studio .NET 2003 (7.1)    | 1310
Visual Studio 2005 (8.0)         | 1400
Visual Studio 2008 (9.0)         | 1500
Visual Studio 2010 (10.0)        | 1600
Visual Studio 2012 (11.0)        | 1700
Visual Studio 2013 (12.0)        | 1800
Visual Studio 2015 (14.0)        | 1900
Visual Studio 2017 RTW (15.0)    | 1910
Visual Studio 2017 version 15.3  | 1911
Visual Studio 2017 version 15.5  | 1912
Visual Studio 2017 version 15.6  | 1913
Visual Studio 2017 version 15.7  | 1914
Visual Studio 2017 version 15.8  | 1915
Visual Studio 2017 version 15.9  | 1916
Visual Studio 2019 RTW (16.0)    | 1920
Visual Studio 2019 version 16.1  | 1921
Visual Studio 2019 version 16.2  | 1922
Visual Studio 2019 version 16.3  | 1923
Visual Studio 2019 version 16.4  | 1924
Visual Studio 2019 version 16.5  | 1925
Visual Studio 2019 version 16.6  | 1926
Visual Studio 2019 version 16.7  | 1927
Visual Studio 2019 version 16.8  | 1928
Visual Studio 2019 version 16.10 | 1929
*/
#ifdef _WINDOWS
#ifndef COMPILERLIBINFIX
#if _MSC_VER <= 1200
#define __CXX_STANDARD__ 1
#define COMPILERLIBINFIX "msvc2000"
#elif _MSC_VER >= 1300 && _MSC_VER <= 1300
#define __CXX_STANDARD__ 1
#define COMPILERLIBINFIX "msvc2002"
#elif _MSC_VER >= 1310 && _MSC_VER <= 1310
#define __CXX_STANDARD__ 1
#define COMPILERLIBINFIX "msvc2003"
#elif _MSC_VER >= 1400 && _MSC_VER <= 1400
#define __CXX_STANDARD__ 1
#define COMPILERLIBINFIX "msvc2005"
#elif _MSC_VER >= 1500 && _MSC_VER <= 1500
#define __CXX_STANDARD__ 1
#define COMPILERLIBINFIX "msvc2008"
#elif _MSC_VER >= 1600 && _MSC_VER <= 1600
#define __CXX_STANDARD__ 1
#define COMPILERLIBINFIX "msvc2010"
#elif _MSC_VER >= 1700 && _MSC_VER <= 1700
#define __CXX_STANDARD__ 1
#define COMPILERLIBINFIX "msvc2012"
#elif _MSC_VER >= 1800 && _MSC_VER <= 1800
#define __CXX_STANDARD__ 201402
#define CXX_STANDARD CPP17
#define COMPILERLIBINFIX "msvc2013"
#elif _MSC_VER >= 1900 && _MSC_VER <= 1900
#define __CXX_STANDARD__ 201703
#define CXX_STANDARD CPP17
#define COMPILERLIBINFIX "msvc2015"
#elif _MSC_VER >= 1910 && _MSC_VER <= 1916
#define __CXX_STANDARD__ 201703
#define COMPILERLIBINFIX "msvc2017"
#elif _MSC_VER >= 1920 && _MSC_VER <= 1929
#define __CXX_STANDARD__ 201703
#define COMPILERLIBINFIX "msvc2019"
#else
#define __CXX_STANDARD__ 1
#endif
#endif
#endif // #ifdef _WINDOWS

#ifndef COMPILERLIBINFIX
#define COMPILERLIBINFIX ""
#endif

#ifdef _WINDOWS
#if (__CXX_STANDARD__ == 201703)
#define CXX_STANDARD 17
#elif (__CXX_STANDARD__ == 201402)
#define CXX_STANDARD 14
#elif (__CXX_STANDARD__ == 201103)
#define CXX_STANDARD 11
#elif (__CXX_STANDARD__ == 19971)
#define CXX_STANDARD 98
#elif (__CXX_STANDARD__ == 1)
#define CXX_STANDARD 1
#ifndef nullptr
#define nullptr 0
#endif
#endif
#endif // #ifdef _WINDOWS

// Some customers are configuring Qt to rename the Qt libraries to Qt*<infix>.
#define QTLIBINFIX ""
//#define QTLIBINFIX "Isar"
//#define QTLIBINFIX "SWP"

#ifndef PLATFORMLIBINFIX
#ifdef _WIN64
#define PLATFORMLIBINFIX "x64"
#else
#define PLATFORMLIBINFIX "Win32"
#endif
#endif

#ifndef PLATFORMLIBINFIX
#define PLATFORMLIBINFIX ""
#endif

#ifndef CONFIGLIBINFIX
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
/*! Predefined trace detail levels.

    @ingroup _GRP_Libs_ZSSys_MethodTracing

    Higher detail levels include lower detail levels.
    Please note that this enum is only a suggestion for trace detail levels
    which can be used. The detail level of the trace admin object is an integer
    type and user defined values can be used if necessary.
    To avoid type casts requested by the compiler this enum is not a class enum definition.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum ETraceDetailLevel
//==============================================================================
{
    ETraceDetailLevelUndefined      = -1,    /*!< Used e.g. to indicate that the trace level should not be used but the predefined detail level should be used. */
    ETraceDetailLevelNone           =  0,    /*!< Trace output is disabled. */
    ETraceDetailLevelMethodCalls    =  1,    /*!< Tracing method entries and leaves without tracing input and output arguments and without method return value. */
    ETraceDetailLevelMethodArgs     =  2,    /*!< Tracing method entries and leaves together with input and output arguemnts and method return value. */
    ETraceDetailLevelInternalStates =  3,    /*!< Tracing everything above and internal states (e.g. current state, current request in progress etc.. */
    ETraceDetailLevelRuntimeInfo    =  4,    /*!< Tracing everything above and additional runtime info (e.g. what is going to be done, what has been executed and how). */
    ETraceDetailLevelVerbose        =  5,    /*!< Tracing everything above and even more. */
    ETraceDetailLevelCount
};

//******************************************************************************
class CTrcAdminObj
//******************************************************************************
{
public: // instance methods
    const char* serverName() const { return m_szServerName; }
    const char* keyInTree() const { return m_szKeyInTree; }
public: // instance methods
    //char* getNameSpace() const;
    //char* getClassName() const;
    //char* getObjectName() const;
public: // instance methods
    void setObjectThreadName( const char* i_szThreadName );
    //char* getObjectThreadName() const;
public: // instance methods
    void setEnabled( bool i_bEnabled );
    bool isEnabled() const;
public: // instance methods
    void setTraceDetailLevel( int i_iTrcDetailLevel );
    int getTraceDetailLevel() const;
public: // instance methods
    bool isActive( int i_iDetailLevel ) const;
public: // ctors and dtor (declared public but for internal use only, implemented in ZSIpcTrace::ZSIpcTrcDllMain)
    CTrcAdminObj( const char* i_szServerName, const char* i_szKeyInTree );
    ~CTrcAdminObj();
protected: // instance members
    char* m_szServerName;
    char* m_szKeyInTree;

}; // class CTrcAdminObj


//******************************************************************************
class CMethodTracer
//******************************************************************************
{
public: // ctors and dtor
    CMethodTracer(  // instance tracer (name space, class and object name set at trace admin object)
        CTrcAdminObj* i_pTrcAdminObj,
        int           i_iFilterDetailLevel,    // Entering and leaving the method is traced if the admin objects detail level is greater or equal than the filter setting than the detail level.
        const char*   i_szMethod,
        const char*   i_szMethodInArgs );
    CMethodTracer(  // class tracer (name space and class name (but not object name) set at trace admin object)
        CTrcAdminObj* i_pTrcAdminObj,
        int           i_iFilterDetailLevel,    // Entering and leaving the method is traced if the admin objects detail level is greater or equal than the filter setting than the detail level.
        const char*   i_szObjName,
        const char*   i_szMethod,
        const char*   i_szMethodInArgs );
    ~CMethodTracer();
public: // instance methods
    void onAdminObjAboutToBeReleased();
public: // instance methods
    int getTraceDetailLevel() const;
    int getEnterLeaveFilterDetailLevel() const { return m_iEnterLeaveFilterDetailLevel; }
public: // instance methods
    //char* getNameSpace() const;
    //char* getClassName() const;
    //char* getObjectName() const;
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
    bool isActive( int i_iFilterDetailLevel ) const;
public: // instance methods
    void trace( const char* i_szAddInfo, int i_iFilterDetailLevel = ETraceDetailLevelInternalStates ) const;
protected: // instance members
    CTrcAdminObj* m_pTrcAdminObj;
    int           m_iEnterLeaveFilterDetailLevel;
    char*         m_szObjName;
    char*         m_szMethod;
    char*         m_szMethodReturn;
    char*         m_szMethodOutArgs;

}; // class CMethodTracer


//******************************************************************************
struct STrcServerSettings
//******************************************************************************
{
    bool  m_bEnabled;
    bool  m_bNewTrcAdminObjsEnabledAsDefault;
    int   m_iNewTrcAdminObjsDefaultDetailLevel;
    bool  m_bCacheDataIfNotConnected;
    int   m_iCacheDataMaxArrLen;
    char* m_szAdminObjFileAbsFilePath;
    bool  m_bUseLocalTrcFile;
    char* m_szLocalTrcFileAbsFilePath;
    int   m_iLocalTrcFileAutoSaveInterval_ms;
    int   m_iLocalTrcFileSubFileCountMax;
    int   m_iLocalTrcFileSubFileLineCountMax;
    bool  m_bLocalTrcFileCloseFileAfterEachWrite;

}; // struct STrcServerSettings

void STrcServerSettings_init( STrcServerSettings& i_trcSettings );
void STrcServerSettings_release( STrcServerSettings& i_trcSettings );


//******************************************************************************
class CTrcServer
//******************************************************************************
{
public: // class methods to add, remove and modify admin objects
    static CTrcAdminObj* GetTraceAdminObj(
        const char* i_szNameSpace,
        const char* i_szClassName,
        const char* i_szObjName,
        EEnabled    i_bEnabledAsDefault = EEnabledUndefined,
        int         i_iDefaultDetailLevel = ETraceDetailLevelUndefined,
        const char* i_szServerName = "ZSTrcServer" );
    static void ReleaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj, const char* i_szServerName = "ZSTrcServer" );
public: // class method to save/recall admin objects file
    // Set organization and application if the trace server is used in a none Qt Application.
    // Only if the organization and application name is set the default file paths for the
    // trace admin objects and the trace log files may be correctly determined.
    static void SetOrganizationName( const char* i_szName );
    static char* GetOrganizationName();
    static void SetApplicationName( const char* i_szName );
    static char* GetApplicationName();
    static char* GetDefaultAdminObjFileAbsoluteFilePath( const char* i_szIniFileScope = "System" );
    static char* GetDefaultLocalTrcFileAbsoluteFilePath( const char* i_szIniFileScope = "System" );
public: // class methods
    static void RegisterCurrentThread( const char* i_szThreadName );
    static void UnregisterCurrentThread();
    static char* GetCurrentThreadName();
public: // instance methods
    const char* name() const { return m_szName; }
public: // instance methods
    bool isActive() const;
public: // instance methods
    void setEnabled( bool i_bEnabled );
    bool isEnabled() const;
public: // instance methods
    void setNewTrcAdminObjsEnabledAsDefault( bool i_bEnabled );
    bool areNewTrcAdminObjsEnabledAsDefault() const;
    void setNewTrcAdminObjsDefaultDetailLevel( int i_iDetailLevel );
    int getNewTrcAdminObjsDefaultDetailLevel() const;
public: // instance methods
    void setAdminObjFileAbsoluteFilePath( const char* i_szAbsFilePath );
    char* getAdminObjFileAbsoluteFilePath() const;
public: // instance methods
    bool recallAdminObjs();
    bool saveAdminObjs();
public: // instance methods
    void setUseLocalTrcFile( bool i_bUse );
    bool isLocalTrcFileUsed() const;
    void setLocalTrcFileAbsoluteFilePath( const char* i_szAbsFilePath );
    char* getLocalTrcFileAbsoluteFilePath() const;
    char* getLocalTrcFileCompleteBaseName() const;
    char* getLocalTrcFileAbsolutePath() const;
    bool isLocalTrcFileActive() const;
    void setLocalTrcFileAutoSaveInterval( int i_iAutoSaveInterval_ms );
    int getLocalTrcFileAutoSaveInterval() const;
    void setLocalTrcFileCloseFileAfterEachWrite( bool i_bCloseFile );
    bool getLocalTrcFileCloseFileAfterEachWrite() const;
public: // instance methods
    void setLocalTrcFileSubFileCountMax( int i_iCountMax );
    int getLocalTrcFileSubFileCountMax() const;
    void setLocalTrcFileSubFileLineCountMax( int i_iCountMax );
    int getLocalTrcFileSubFileLineCountMax() const;
public: // instance methods
    void setCacheTrcDataIfNotConnected( bool i_bCacheData );
    bool getCacheTrcDataIfNotConnected() const;
    void setCacheTrcDataMaxArrLen( int i_iMaxArrLen );
    int getCacheTrcDataMaxArrLen() const;
public: // instance methods
    bool setTraceSettings( const STrcServerSettings& i_settings );
    STrcServerSettings getTraceSettings() const;  // !! char pointers returned in the struct must be freed by caller !!
public: // instance methods
    void clearLocalTrcFile();
public: // ctors and dtor (declared public but for internal use only, implemented in ZSIpcTrace::ZSIpcTrcDllMain)
    CTrcServer( const char* i_szName );
    ~CTrcServer();
protected: // instance members
    char* m_szName;

}; // class CTrcServer


//******************************************************************************
class CIpcTrcServer : public CTrcServer
//******************************************************************************
{
public: // class methods
    static CIpcTrcServer* GetInstance( const char* i_szName = "ZSTrcServer" );
    static CIpcTrcServer* CreateInstance(
        const char* i_szName = "ZSTrcServer",
        bool i_bCreateOnlyIfNotYetExisting = false,
        int i_iTrcDetailLevel = ETraceDetailLevelNone );
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
    CIpcTrcServer( const char* i_szName, int i_iTrcDetailLevel = ETraceDetailLevelNone );
    ~CIpcTrcServer();

}; // class CIpcTrcServer

//==============================================================================
enum EBuildConfiguration
//==============================================================================
{
    EBuildConfigurationRelease    =  0,
    EBuildConfigurationDebug      =  1,
    EBuildConfigurationAutoDetect =  2
};

bool loadDll( const char* i_szCompiler = nullptr, const char* i_szPlatform = nullptr, EBuildConfiguration i_configuration = EBuildConfigurationAutoDetect, int i_iQtVersionMajor = 5 );
const char* getDllFileName();
bool releaseDll();


} // namespace DllIf

} // namespace Trace

} // namespace ZS

} // extern "C"

#endif // #ifndef ZSIpcTrace_DllIf_h
