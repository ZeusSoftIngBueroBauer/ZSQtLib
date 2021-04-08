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

#ifndef nullptr
#define nullptr 0
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
    char* getNameSpace() const;
    char* getClassName() const;
    char* getObjectName() const;
public: // instance methods
    void setObjectThreadName( const char* i_szThreadName );
    char* getObjectThreadName() const;
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
    char* getNameSpace() const;
    char* getClassName() const;
    char* getObjectName() const;
    char* getMethod() const { return m_szMethod; }
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
        bool        i_bEnabledAsDefault = true,
        int         i_iDefaultDetailLevel = ETraceDetailLevelNone,
        const char* i_szServerName = "ZSTrcServer" );
    static void ReleaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj, const char* i_szServerName = "ZSTrcServer" );
public: // class method to save/recall admin objects file
    // Set organization and application if the trace server is used in a none Qt Application.
    // Only if the organization and application name is set the default file paths for the
    // trace admin objects and the trace log files may be correctly determined.
    static void SetOrganizationName( const char* i_szName );
    static char* GetOrganizationName();     // returned char pointer must be freed by caller
    static void SetApplicationName( const char* i_szName );
    static char* GetApplicationName();     // returned char pointer must be freed by caller
    static void GetDefaultFilePaths(
        char**      o_pszAdminObjFileAbsFilePath,    // must be freed by caller
        char**      o_pszLocalTrcFileAbsFilePath,    // must be freed by caller
        const char* i_szIniFileScope = "System" );
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
    char* getAdminObjFileAbsoluteFilePath() const; // return char pointer must be freed by caller
public: // instance methods
    bool recallAdminObjs();
    bool saveAdminObjs();
public: // instance methods
    void setUseLocalTrcFile( bool i_bUse );
    bool isLocalTrcFileUsed() const;
    void setLocalTrcFileAbsoluteFilePath( const char* i_szAbsFilePath );
    char* getLocalTrcFileAbsoluteFilePath() const;   // returned char pointer must be freed by caller
    char* getLocalTrcFileCompleteBaseName() const;   // returned char pointer must be freed by caller
    char* getLocalTrcFileAbsolutePath() const;       // returned char pointer must be freed by caller
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
    STrcServerSettings getTraceSettings() const;  // returned char pointers must be freed by caller
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
    static CIpcTrcServer* CreateInstance( const char* i_szName = "ZSTrcServer", int i_iTrcDetailLevel = ETraceDetailLevelNone );
    static void DestroyInstance( CIpcTrcServer* i_pTrcServer );
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
