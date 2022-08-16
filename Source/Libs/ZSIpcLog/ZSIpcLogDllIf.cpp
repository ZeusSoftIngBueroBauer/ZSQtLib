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

#ifdef USE_ZS_IPCLOG_DLL_IF

#include "ZSIpcLog/ZSIpcLogDllIf.h"

#include <string>

#if defined(_WINDOWS) || defined (WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <tchar.h>
#include <windows.h>
#endif

#ifdef __linux__
#include <string.h>
#include <dlfcn.h>
#define GetProcAddress dlsym
#endif

using namespace std;
using namespace ZS::Log;


/*******************************************************************************
local auxiliary methods
*******************************************************************************/

namespace ZS
{
namespace System
{
#ifdef _WIN32
//------------------------------------------------------------------------------
wstring s2ws( const string& s )
//------------------------------------------------------------------------------
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}
#endif // #ifdef _WIN32

} // namespace System

} // namespace ZS


/*******************************************************************************
namespace ZS::Log::Dll
*******************************************************************************/

/*==============================================================================
type definitions and constants
==============================================================================*/

typedef void (*TFctLogger_log)( const DllIf::CLogger* i_pLogger, DllIf::ELogDetailLevel i_eFilterDetailLevel, const char* i_szLogEntry );
typedef void (*TFctLogger_setEnabled)( DllIf::CLogger* i_pLogger, bool i_bEnabled );
typedef bool (*TFctLogger_isEnabled)( const DllIf::CLogger* i_pLogger );
typedef void (*TFctLogger_setLogLevel)( DllIf::CLogger* i_pLogger, DllIf::ELogDetailLevel i_eDetailLevel );
typedef DllIf::ELogDetailLevel (*TFctLogger_getLogLevel)( const DllIf::CLogger* i_pLogger );
typedef bool (*TFctLogger_isActive)( const DllIf::CLogger* i_pLogger, DllIf::ELogDetailLevel i_eFilterDetailLevel );
typedef void (*TFctLogger_setDataFilter)( DllIf::CLogger* i_pLogger, const char* i_szFilter );
typedef char* (*TFctLogger_getDataFilter)( const DllIf::CLogger* i_pLogger );
typedef bool (*TFctLogger_isSuppressedByDataFilter)( const DllIf::CLogger* i_pLogger, const char* i_szLogEntry );
typedef void (*TFctLogger_setAddThreadName)( const DllIf::CLogger* i_pLogger, bool i_bAdd );
typedef bool (*TFctLogger_addThreadName)( const DllIf::CLogger* i_pLogger );
typedef void (*TFctLogger_setAddDateTime)( const DllIf::CLogger* i_pLogger, bool i_bAdd );
typedef bool (*TFctLogger_addDateTime)( const DllIf::CLogger* i_pLogger );
typedef void (*TFctLogger_setAddSystemTime)( const DllIf::CLogger* i_pLogger, bool i_bAdd );
typedef bool (*TFctLogger_addSystemTime)( const DllIf::CLogger* i_pLogger );
typedef void (*TFctLogger_setNameSpace)( const DllIf::CLogger* i_pLogger, const char* i_szNameSpace );
typedef char* (*TFctLogger_getNameSpace)( const DllIf::CLogger* i_pLogger );
typedef void (*TFctLogger_setClassName)( const DllIf::CLogger* i_pLogger, const char* i_szClassName );
typedef char* (*TFctLogger_getClassName)( const DllIf::CLogger* i_pLogger );
typedef void (*TFctLogger_setObjectName)( const DllIf::CLogger* i_pLogger, const char* i_szObjectName );
typedef char* (*TFctLogger_getObjectName)( const DllIf::CLogger* i_pLogger );
typedef char* (*TFctLogger_name)( const DllIf::CLogger* i_pLogger );

typedef void (*TFctLogServer_SetOrganizationName)( const char* i_szName );
typedef char* (*TFctLogServer_GetOrganizationName)();
typedef void (*TFctLogServer_SetApplicationName)( const char* i_szName );
typedef char* (*TFctLogServer_GetApplicationName)();
typedef void (*TFctLogServer_SetLoggerFileAbsoluteFilePath)( const char* i_szAbsFilePath );
typedef char* (*TFctLogServer_GetLoggerFileAbsoluteFilePath)();
typedef char* (*TFctLogServer_GetLoggerFileCompleteBaseName)();
typedef char* (*TFctLogServer_GetLoggerFileAbsolutePath)();
typedef void (*TFctLogServer_SetLocalLogFileAbsoluteFilePath)( const char* i_szAbsFilePath );
typedef char* (*TFctLogServer_GetLocalLogFileAbsoluteFilePath)();
typedef char* (*TFctLogServer_GetLocalLogFileCompleteBaseName)();
typedef char* (*TFctLogServer_GetLocalLogFileAbsolutePath)();
typedef void (*TFctLogServer_RegisterCurrentThread)( const char* i_szThreadName );
typedef void (*TFctLogServer_UnregisterCurrentThread)();
typedef char* (*TFctLogServer_GetCurrentThreadName)();
typedef DllIf::CLogger* (*TFctLogServer_GetLogger)( const char* i_szName, DllIf::EEnabled i_bEnabledAsDefault, DllIf::ELogDetailLevel i_eDefaultDetailLevel );
typedef void (*TFctLogServer_setEnabled)( bool i_bEnabled );
typedef bool (*TFctLogServer_isEnabled)();
typedef bool (*TFctLogServer_isActive)();
typedef void (*TFctLogServer_log)( const DllIf::CLogger* i_pLogger, DllIf::ELogDetailLevel i_eFilterDetailLevel, const char* i_szLogEntry);
typedef void (*TFctLogServer_setNewLoggersEnabledAsDefault)( bool i_bEnabled );
typedef bool (*TFctLogServer_areNewLoggersEnabledAsDefault)();
typedef void (*TFctLogServer_setNewLoggersDefaultDetailLevel)( DllIf::ELogDetailLevel i_eDetailLevel );
typedef DllIf::ELogDetailLevel (*TFctLogServer_getNewLoggersDefaultDetailLevel)();
typedef bool (*TFctLogServer_recallLoggers)();
typedef bool (*TFctLogServer_saveLoggers)();
typedef void (*TFctLogServer_setUseLocalLogFile)( bool i_bUse );
typedef bool (*TFctLogServer_isLocalLogFileUsed)();
typedef bool (*TFctLogServer_isLocalLogFileActive)();
typedef void (*TFctLogServer_setLocalLogFileAutoSaveIntervalInMs)( int i_iAutoSaveInterval_ms );
typedef int (*TFctLogServer_getLocalLogFileAutoSaveIntervalInMs)();
typedef void (*TFctLogServer_setLocalLogFileCloseFileAfterEachWrite)( bool i_bCloseFile );
typedef bool (*TFctLogServer_getLocalLogFileCloseFileAfterEachWrite)();
typedef void (*TFctLogServer_setLocalLogFileSubFileCountMax)( int i_iCountMax );
typedef int (*TFctLogServer_getLocalLogFileSubFileCountMax)();
typedef void (*TFctLogServer_setLocalLogFileSubFileLineCountMax)( int i_iCountMax );
typedef int (*TFctLogServer_getLocalLogFileSubFileLineCountMax)();
typedef void (*TFctLogServer_setUseIpcServer)( bool i_bUseIpcServer );
typedef bool (*TFctLogServer_isIpcServerUsed)();
typedef void (*TFctLogServer_setCacheLogDataIfNotConnected)( bool i_bCacheData );
typedef bool (*TFctLogServer_getCacheLogDataIfNotConnected)();
typedef void (*TFctLogServer_setCacheLogDataMaxArrLen)( int i_iMaxArrLen );
typedef int (*TFctLogServer_getCacheLogDataMaxArrLen)();
typedef bool (*TFctLogServer_setLogSettings)( const DllIf::SLogServerSettings& i_settings );
typedef DllIf::SLogServerSettings (*TFctLogServer_getLogSettings)();
typedef void (*TFctLogServer_clearLocalLogFile)();

typedef DllIf::CIpcLogServer* (*TFctIpcLogServer_GetInstance)();
typedef DllIf::CIpcLogServer* (*TFctIpcLogServer_CreateInstance)();
typedef void (*TFctIpcLogServer_ReleaseInstance)( DllIf::CIpcLogServer* i_pLogServer );
typedef bool (*TFctIpcLogServer_startup)( DllIf::CIpcLogServer* i_pLogServer, int i_iTimeout_ms, bool i_bWait );
typedef bool (*TFctIpcLogServer_shutdown)( DllIf::CIpcLogServer* i_pLogServer, int i_iTimeout_ms, bool i_bWait );
typedef bool (*TFctIpcLogServer_isListening)( const DllIf::CIpcLogServer* i_pLogServer );
typedef bool (*TFctIpcLogServer_isConnected)( const DllIf::CIpcLogServer* i_pLogServer );
typedef bool (*TFctIpcLogServer_setPort)( DllIf::CIpcLogServer* i_pLogServer, unsigned short i_uPort, int i_iTimeout_ms, bool i_bWait );
typedef unsigned short (*TFctIpcLogServer_getPort)( const DllIf::CIpcLogServer* i_pLogServer );

/*==============================================================================
static instances
==============================================================================*/

static char* s_szDllFileName = nullptr;

#ifdef _WIN32
static HMODULE s_hndDllIf = NULL;
#else
static void* s_hndDllIf = NULL;
#endif

TFctLogger_log                      s_pFctLogger_log                      = NULL;
TFctLogger_setEnabled               s_pFctLogger_setEnabled               = NULL;
TFctLogger_isEnabled                s_pFctLogger_isEnabled                = NULL;
TFctLogger_setLogLevel              s_pFctLogger_setLogLevel              = NULL;
TFctLogger_getLogLevel              s_pFctLogger_getLogLevel              = NULL;
TFctLogger_isActive                 s_pFctLogger_isActive                 = NULL;
TFctLogger_setDataFilter            s_pFctLogger_setDataFilter            = NULL;
TFctLogger_getDataFilter            s_pFctLogger_getDataFilter            = NULL;
TFctLogger_isSuppressedByDataFilter s_pFctLogger_isSuppressedByDataFilter = NULL;
TFctLogger_setAddThreadName         s_pFctLogger_setAddThreadName         = NULL;
TFctLogger_addThreadName            s_pFctLogger_addThreadName            = NULL;
TFctLogger_setAddDateTime           s_pFctLogger_setAddDateTime           = NULL;
TFctLogger_addDateTime              s_pFctLogger_addDateTime              = NULL;
TFctLogger_setAddSystemTime         s_pFctLogger_setAddSystemTime         = NULL;
TFctLogger_addSystemTime            s_pFctLogger_addSystemTime            = NULL;
TFctLogger_setNameSpace             s_pFctLogger_setNameSpace             = NULL;
TFctLogger_getNameSpace             s_pFctLogger_getNameSpace             = NULL;
TFctLogger_setClassName             s_pFctLogger_setClassName             = NULL;
TFctLogger_getClassName             s_pFctLogger_getClassName             = NULL;
TFctLogger_setObjectName            s_pFctLogger_setObjectName            = NULL;
TFctLogger_getObjectName            s_pFctLogger_getObjectName            = NULL;
TFctLogger_name                     s_pFctLogger_name                     = NULL;

TFctLogServer_SetOrganizationName                    s_pFctLogServer_SetOrganizationName                    = NULL;
TFctLogServer_GetOrganizationName                    s_pFctLogServer_GetOrganizationName                    = NULL;
TFctLogServer_SetApplicationName                     s_pFctLogServer_SetApplicationName                     = NULL;
TFctLogServer_GetApplicationName                     s_pFctLogServer_GetApplicationName                     = NULL;
TFctLogServer_SetLoggerFileAbsoluteFilePath          s_pFctLogServer_SetLoggerFileAbsoluteFilePath          = NULL;
TFctLogServer_GetLoggerFileAbsoluteFilePath          s_pFctLogServer_GetLoggerFileAbsoluteFilePath          = NULL;
TFctLogServer_GetLoggerFileCompleteBaseName          s_pFctLogServer_GetLoggerFileCompleteBaseName          = NULL;
TFctLogServer_GetLoggerFileAbsolutePath              s_pFctLogServer_GetLoggerFileAbsolutePath              = NULL;
TFctLogServer_SetLocalLogFileAbsoluteFilePath        s_pFctLogServer_SetLocalLogFileAbsoluteFilePath        = NULL;
TFctLogServer_GetLocalLogFileAbsoluteFilePath        s_pFctLogServer_GetLocalLogFileAbsoluteFilePath        = NULL;
TFctLogServer_GetLocalLogFileCompleteBaseName        s_pFctLogServer_GetLocalLogFileCompleteBaseName        = NULL;
TFctLogServer_GetLocalLogFileAbsolutePath            s_pFctLogServer_GetLocalLogFileAbsolutePath            = NULL;
TFctLogServer_RegisterCurrentThread                  s_pFctLogServer_RegisterCurrentThread                  = NULL;
TFctLogServer_UnregisterCurrentThread                s_pFctLogServer_UnregisterCurrentThread                = NULL;
TFctLogServer_GetCurrentThreadName                   s_pFctLogServer_GetCurrentThreadName                   = NULL;
TFctLogServer_GetLogger                              s_pFctLogServer_GetLogger                              = NULL;
TFctLogServer_setEnabled                             s_pFctLogServer_setEnabled                             = NULL;
TFctLogServer_isEnabled                              s_pFctLogServer_isEnabled                              = NULL;
TFctLogServer_isActive                               s_pFctLogServer_isActive                               = NULL;
TFctLogServer_log                                    s_pFctLogServer_log                                    = NULL;
TFctLogServer_setNewLoggersEnabledAsDefault          s_pFctLogServer_setNewLoggersEnabledAsDefault          = NULL;
TFctLogServer_areNewLoggersEnabledAsDefault          s_pFctLogServer_areNewLoggersEnabledAsDefault          = NULL;
TFctLogServer_setNewLoggersDefaultDetailLevel        s_pFctLogServer_setNewLoggersDefaultDetailLevel        = NULL;
TFctLogServer_getNewLoggersDefaultDetailLevel        s_pFctLogServer_getNewLoggersDefaultDetailLevel        = NULL;
TFctLogServer_recallLoggers                          s_pFctLogServer_recallLoggers                          = NULL;
TFctLogServer_saveLoggers                            s_pFctLogServer_saveLoggers                            = NULL;
TFctLogServer_setUseLocalLogFile                     s_pFctLogServer_setUseLocalLogFile                     = NULL;
TFctLogServer_isLocalLogFileUsed                     s_pFctLogServer_isLocalLogFileUsed                     = NULL;
TFctLogServer_isLocalLogFileActive                   s_pFctLogServer_isLocalLogFileActive                   = NULL;
TFctLogServer_setLocalLogFileAutoSaveIntervalInMs    s_pFctLogServer_setLocalLogFileAutoSaveIntervalInMs    = NULL;
TFctLogServer_getLocalLogFileAutoSaveIntervalInMs    s_pFctLogServer_getLocalLogFileAutoSaveIntervalInMs    = NULL;
TFctLogServer_setLocalLogFileCloseFileAfterEachWrite s_pFctLogServer_setLocalLogFileCloseFileAfterEachWrite = NULL;
TFctLogServer_getLocalLogFileCloseFileAfterEachWrite s_pFctLogServer_getLocalLogFileCloseFileAfterEachWrite = NULL;
TFctLogServer_setLocalLogFileSubFileCountMax         s_pFctLogServer_setLocalLogFileSubFileCountMax         = NULL;
TFctLogServer_getLocalLogFileSubFileCountMax         s_pFctLogServer_getLocalLogFileSubFileCountMax         = NULL;
TFctLogServer_setLocalLogFileSubFileLineCountMax     s_pFctLogServer_setLocalLogFileSubFileLineCountMax     = NULL;
TFctLogServer_getLocalLogFileSubFileLineCountMax     s_pFctLogServer_getLocalLogFileSubFileLineCountMax     = NULL;
TFctLogServer_setUseIpcServer                        s_pFctLogServer_setUseIpcServer                        = NULL;
TFctLogServer_isIpcServerUsed                        s_pFctLogServer_isIpcServerUsed                        = NULL;
TFctLogServer_setCacheLogDataIfNotConnected          s_pFctLogServer_setCacheLogDataIfNotConnected          = NULL;
TFctLogServer_getCacheLogDataIfNotConnected          s_pFctLogServer_getCacheLogDataIfNotConnected          = NULL;
TFctLogServer_setCacheLogDataMaxArrLen               s_pFctLogServer_setCacheLogDataMaxArrLen               = NULL;
TFctLogServer_getCacheLogDataMaxArrLen               s_pFctLogServer_getCacheLogDataMaxArrLen               = NULL;
TFctLogServer_setLogSettings                         s_pFctLogServer_setLogSettings                         = NULL;
TFctLogServer_getLogSettings                         s_pFctLogServer_getLogSettings                         = NULL;
TFctLogServer_clearLocalLogFile                      s_pFctLogServer_clearLocalLogFile                      = NULL;

TFctIpcLogServer_CreateInstance                      s_pFctIpcLogServer_CreateInstance                      = NULL;
TFctIpcLogServer_GetInstance                         s_pFctIpcLogServer_GetInstance                         = NULL;
TFctIpcLogServer_ReleaseInstance                     s_pFctIpcLogServer_ReleaseInstance                     = NULL;
TFctIpcLogServer_startup                             s_pFctIpcLogServer_startup                             = NULL;
TFctIpcLogServer_shutdown                            s_pFctIpcLogServer_shutdown                            = NULL;
TFctIpcLogServer_isListening                         s_pFctIpcLogServer_isListening                         = NULL;
TFctIpcLogServer_isConnected                         s_pFctIpcLogServer_isConnected                         = NULL;
TFctIpcLogServer_setPort                             s_pFctIpcLogServer_setPort                             = NULL;
TFctIpcLogServer_getPort                             s_pFctIpcLogServer_getPort                             = NULL;


/*==============================================================================
Exported methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Loads the Remote Logging Dlls.

    @ingroup _GRP_Namespace_ZS_Log_DllIf

    @param i_configuration [in] Spezifies the build configuration.
           Default: EBuildConfigurationAutoDetect
           The parameter defines the lib infix for the ZSQtLib Dlls used by
           the build configuration which may be either a Debug or Release Build.
           Usually the lib infix for the build configuration can be automatically
           detected by evaluationg the compiler directive _DEBUG and the
           the macro CONFIGLIBINFIX is set via "ZSIpcLogDllIf.h" to e.g.
           an empty string for Release and to "d" for Debug builds.
           If nullptr is passed  the automatically detected CONFIGLIBINFIX is used.
           The parameter must correspond to the config lib-infix
           as used when compiling and linking the ZSQtLib-Dlls.

    @param i_iQtVersionMajor [in] Spezifies the major version of the Qt Dlls.
           Default: 5
           The parameter defines the lib infix for the Qt Dlls.
           This parameter cannot be automatically detected as no Qt header files
           are included when using the Dll inteface files.
           The parameter must correspond to the lib-infix for the Qt major version
           as used when compiling and linking the ZSQtLib-Dlls.

    @return true, if loading the Dlls was successfull and the method address of each
            needed expertod method could be resolved.
            false, in case of an error.

    @note Possible reasons why the dlls could not be loaded are:
          - Dlls were not found
            - If necessary, expand PATH so that the operating system can find the Dlls.
            - File names are not correct (check all lib infixes).
          - Incompatible Versions (x64/Win32, Qt Version 4 statt 5, etc.).
          - Qt dlls could not be loaded
            - Were the Qt Dlls possibly compiled with a LibInfix?
            - Can the operating system find the Qt dlls?
*/
bool ZS::Log::DllIf::loadDll( EBuildConfiguration i_configuration, int i_iQtVersionMajor )
//------------------------------------------------------------------------------
{
    const char* szConfig = CONFIGLIBINFIX;

    if( i_configuration == EBuildConfigurationRelease )
    {
        szConfig = "";
    }
    else if( i_configuration == EBuildConfigurationDebug )
    {
        szConfig = "d";
    }

    char* szQtVersionMajor = new char[21]; memset(szQtVersionMajor, 0x00, 21);

    #ifdef _WINDOWS
    _itoa_s(i_iQtVersionMajor, szQtVersionMajor, 21, 10);
    #else
    sprintf(szQtVersionMajor, "%d", i_iQtVersionMajor);
    #endif

    char* szError = nullptr;

    /* Examples for library file names:
     *  "ZSIpcLogQt5d"
     *  "libZSIpcLogQt5d"
     * The GNU compiler (MinGW on Windows) are inserting "lib"
     * at the beginning of the file names (also on Windows machines).
     * We are going to try both versions of file names.
    */

    const char* szZSDllName1 = "ZSIpcLogQt";
    const char* szZSDllName2 = "libZSIpcLogQt";

    for( int iFileNameTries = 0; iFileNameTries < 2; ++iFileNameTries )
    {
        const char* szZSDllName = szZSDllName1;
        if( iFileNameTries == 1 )
        {
            szZSDllName = szZSDllName2;
        }

        delete s_szDllFileName;
        s_szDllFileName = nullptr;

        size_t iStrLenDllFileName = strlen(szZSDllName) + strlen(szQtVersionMajor) + strlen(szConfig) + 4;
        s_szDllFileName = new char[iStrLenDllFileName+1];
        memset(s_szDllFileName, 0x00, iStrLenDllFileName+1);

        size_t iStrPos = 0;
        memcpy(&s_szDllFileName[iStrPos], szZSDllName, strlen(szZSDllName));           // "ZSIpcLogQt"
        iStrPos += strlen(szZSDllName);
        memcpy(&s_szDllFileName[iStrPos], szQtVersionMajor, strlen(szQtVersionMajor)); // "ZSIpcLogQt5"
        iStrPos += strlen(szQtVersionMajor);
        if( strlen(szConfig) > 0 )
        {
            memcpy(&s_szDllFileName[iStrPos], szConfig, strlen(szConfig));             // "ZSIpcLogQt5d"
            iStrPos += strlen(szConfig);
        }

        #ifdef _WIN32
        #ifdef UNICODE
        const wstring wstrTrcDllFileName = ZS::System::s2ws(s_szDllFileName);
        s_hndDllIf = LoadLibrary(wstrTrcDllFileName.c_str());
        #else
        s_hndDllIf = LoadLibrary(s_szDllFileName);
        #endif
        #else // !_WIN32
        memcpy(&s_szDllFileName[iStrPos], ".so", 3);                                 // "ZSIpcLogQt5d.so"
        s_hndDllIf = dlopen(s_szDllFileName, RTLD_LAZY);
        if( s_hndDllIf == NULL )
        {
            // Helps to find the reason in debugger why shared library could not be loaded.
            if( szError == nullptr ) szError = new char[10000];
            memset(szError, 0x00, 10000);
            sprintf(szError, "%s", dlerror());
        }
        #endif

        if( s_hndDllIf != NULL )
        {
            break;
        }
    } // for( int iFileNameTries = 0; iFileNameTries < 2; ++iFileNameTries )

    delete [] szError;
    szError = nullptr;

    delete [] szQtVersionMajor;
    szQtVersionMajor = nullptr;

    bool bOk = false;

    if( s_hndDllIf != NULL )
    {
        bOk = true;

        // Logger
        //------------

        s_pFctLogger_log = (TFctLogger_log)GetProcAddress(s_hndDllIf, "Logger_log");
        if( s_pFctLogger_log == NULL ) bOk = false;

        s_pFctLogger_setEnabled = (TFctLogger_setEnabled)GetProcAddress(s_hndDllIf, "Logger_setEnabled");
        if( s_pFctLogger_setEnabled == NULL ) bOk = false;

        s_pFctLogger_isEnabled = (TFctLogger_isEnabled)GetProcAddress(s_hndDllIf, "Logger_isEnabled");
        if( s_pFctLogger_isEnabled == NULL ) bOk = false;

        s_pFctLogger_setLogLevel = (TFctLogger_setLogLevel)GetProcAddress(s_hndDllIf, "Logger_setLogLevel");
        if( s_pFctLogger_setLogLevel == NULL ) bOk = false;

        s_pFctLogger_getLogLevel = (TFctLogger_getLogLevel)GetProcAddress(s_hndDllIf, "Logger_getLogLevel");
        if( s_pFctLogger_getLogLevel == NULL ) bOk = false;

        s_pFctLogger_isActive = (TFctLogger_isActive)GetProcAddress(s_hndDllIf, "Logger_isActive");
        if( s_pFctLogger_isActive == NULL ) bOk = false;

        s_pFctLogger_setDataFilter = (TFctLogger_setDataFilter)GetProcAddress(s_hndDllIf, "Logger_setDataFilter");
        if( s_pFctLogger_setDataFilter == NULL ) bOk = false;

        s_pFctLogger_getDataFilter = (TFctLogger_getDataFilter)GetProcAddress(s_hndDllIf, "Logger_getDataFilter");
        if( s_pFctLogger_getDataFilter == NULL ) bOk = false;

        s_pFctLogger_isSuppressedByDataFilter = (TFctLogger_isSuppressedByDataFilter)GetProcAddress(s_hndDllIf, "Logger_isSuppressedByDataFilter");
        if( s_pFctLogger_isSuppressedByDataFilter == NULL ) bOk = false;

        s_pFctLogger_setAddThreadName = (TFctLogger_setAddThreadName)GetProcAddress(s_hndDllIf, "Logger_setAddThreadName");
        if( s_pFctLogger_setAddThreadName == NULL ) bOk = false;

        s_pFctLogger_addThreadName = (TFctLogger_addThreadName)GetProcAddress(s_hndDllIf, "Logger_addThreadName");
        if( s_pFctLogger_addThreadName == NULL ) bOk = false;

        s_pFctLogger_setAddDateTime = (TFctLogger_setAddDateTime)GetProcAddress(s_hndDllIf, "Logger_setAddDateTime");
        if( s_pFctLogger_setAddDateTime == NULL ) bOk = false;

        s_pFctLogger_addDateTime = (TFctLogger_addDateTime)GetProcAddress(s_hndDllIf, "Logger_addDateTime");
        if( s_pFctLogger_addDateTime == NULL ) bOk = false;

        s_pFctLogger_setAddSystemTime = (TFctLogger_setAddSystemTime)GetProcAddress(s_hndDllIf, "Logger_setAddSystemTime");
        if( s_pFctLogger_setAddSystemTime == NULL ) bOk = false;

        s_pFctLogger_addSystemTime = (TFctLogger_addSystemTime)GetProcAddress(s_hndDllIf, "Logger_addSystemTime");
        if( s_pFctLogger_addSystemTime == NULL ) bOk = false;

        s_pFctLogger_setNameSpace = (TFctLogger_setNameSpace)GetProcAddress(s_hndDllIf, "Logger_setNameSpace");
        if( s_pFctLogger_setNameSpace == NULL ) bOk = false;

        s_pFctLogger_getNameSpace = (TFctLogger_getNameSpace)GetProcAddress(s_hndDllIf, "Logger_getNameSpace");
        if( s_pFctLogger_getNameSpace == NULL ) bOk = false;

        s_pFctLogger_setClassName = (TFctLogger_setClassName)GetProcAddress(s_hndDllIf, "Logger_setClassName");
        if( s_pFctLogger_setClassName == NULL ) bOk = false;

        s_pFctLogger_getClassName = (TFctLogger_getClassName)GetProcAddress(s_hndDllIf, "Logger_getClassName");
        if( s_pFctLogger_getClassName == NULL ) bOk = false;

        s_pFctLogger_setObjectName = (TFctLogger_setObjectName)GetProcAddress(s_hndDllIf, "Logger_setObjectName");
        if( s_pFctLogger_setObjectName == NULL ) bOk = false;

        s_pFctLogger_getObjectName = (TFctLogger_getObjectName)GetProcAddress(s_hndDllIf, "Logger_getObjectName");
        if( s_pFctLogger_getObjectName == NULL ) bOk = false;

        s_pFctLogger_name = (TFctLogger_name)GetProcAddress(s_hndDllIf, "Logger_name");
        if( s_pFctLogger_name == NULL ) bOk = false;

        // LogServer
        //------------

        s_pFctLogServer_SetOrganizationName = (TFctLogServer_SetOrganizationName)GetProcAddress(s_hndDllIf, "LogServer_SetOrganizationName");
        if( s_pFctLogServer_SetOrganizationName == NULL ) bOk = false;

        s_pFctLogServer_GetOrganizationName = (TFctLogServer_GetOrganizationName)GetProcAddress(s_hndDllIf, "LogServer_GetOrganizationName");
        if( s_pFctLogServer_GetOrganizationName == NULL ) bOk = false;

        s_pFctLogServer_SetApplicationName = (TFctLogServer_SetApplicationName)GetProcAddress(s_hndDllIf, "LogServer_SetApplicationName");
        if( s_pFctLogServer_SetApplicationName == NULL ) bOk = false;

        s_pFctLogServer_GetApplicationName = (TFctLogServer_GetApplicationName)GetProcAddress(s_hndDllIf, "LogServer_GetApplicationName");
        if( s_pFctLogServer_GetApplicationName == NULL ) bOk = false;

        s_pFctLogServer_SetLoggerFileAbsoluteFilePath = (TFctLogServer_SetLoggerFileAbsoluteFilePath)GetProcAddress(s_hndDllIf, "LogServer_SetLoggerFileAbsoluteFilePath");
        if( s_pFctLogServer_SetLoggerFileAbsoluteFilePath == NULL ) bOk = false;

        s_pFctLogServer_GetLoggerFileAbsoluteFilePath = (TFctLogServer_GetLoggerFileAbsoluteFilePath)GetProcAddress(s_hndDllIf, "LogServer_GetLoggerFileAbsoluteFilePath");
        if( s_pFctLogServer_GetLoggerFileAbsoluteFilePath == NULL ) bOk = false;

        s_pFctLogServer_GetLoggerFileCompleteBaseName = (TFctLogServer_GetLoggerFileCompleteBaseName)GetProcAddress(s_hndDllIf, "LogServer_GetLoggerFileCompleteBaseName");
        if( s_pFctLogServer_GetLoggerFileCompleteBaseName == NULL ) bOk = false;

        s_pFctLogServer_GetLoggerFileAbsolutePath = (TFctLogServer_GetLoggerFileAbsolutePath)GetProcAddress(s_hndDllIf, "LogServer_GetLoggerFileAbsolutePath");
        if( s_pFctLogServer_GetLoggerFileAbsolutePath == NULL ) bOk = false;

        s_pFctLogServer_SetLocalLogFileAbsoluteFilePath = (TFctLogServer_SetLocalLogFileAbsoluteFilePath)GetProcAddress(s_hndDllIf, "LogServer_SetLocalLogFileAbsoluteFilePath");
        if( s_pFctLogServer_SetLocalLogFileAbsoluteFilePath == NULL ) bOk = false;

        s_pFctLogServer_GetLocalLogFileAbsoluteFilePath = (TFctLogServer_GetLocalLogFileAbsoluteFilePath)GetProcAddress(s_hndDllIf, "LogServer_GetLocalLogFileAbsoluteFilePath");
        if( s_pFctLogServer_GetLocalLogFileAbsoluteFilePath == NULL ) bOk = false;

        s_pFctLogServer_GetLocalLogFileCompleteBaseName = (TFctLogServer_GetLocalLogFileCompleteBaseName)GetProcAddress(s_hndDllIf, "LogServer_GetLocalLogFileCompleteBaseName");
        if( s_pFctLogServer_GetLocalLogFileCompleteBaseName == NULL ) bOk = false;

        s_pFctLogServer_GetLocalLogFileAbsolutePath = (TFctLogServer_GetLocalLogFileAbsolutePath)GetProcAddress(s_hndDllIf, "LogServer_GetLocalLogFileAbsolutePath");
        if( s_pFctLogServer_GetLocalLogFileAbsolutePath == NULL ) bOk = false;

        s_pFctLogServer_RegisterCurrentThread = (TFctLogServer_RegisterCurrentThread)GetProcAddress(s_hndDllIf, "LogServer_RegisterCurrentThread");
        if( s_pFctLogServer_RegisterCurrentThread == NULL ) bOk = false;

        s_pFctLogServer_UnregisterCurrentThread = (TFctLogServer_UnregisterCurrentThread)GetProcAddress(s_hndDllIf, "LogServer_UnregisterCurrentThread");
        if( s_pFctLogServer_UnregisterCurrentThread == NULL ) bOk = false;

        s_pFctLogServer_GetCurrentThreadName = (TFctLogServer_GetCurrentThreadName)GetProcAddress(s_hndDllIf, "LogServer_GetCurrentThreadName");
        if( s_pFctLogServer_GetCurrentThreadName == NULL ) bOk = false;

        s_pFctLogServer_GetLogger = (TFctLogServer_GetLogger)GetProcAddress(s_hndDllIf, "LogServer_GetLogger");
        if( s_pFctLogServer_GetLogger == NULL ) bOk = false;

        s_pFctLogServer_setEnabled = (TFctLogServer_setEnabled)GetProcAddress(s_hndDllIf, "LogServer_setEnabled");
        if( s_pFctLogServer_setEnabled == NULL ) bOk = false;

        s_pFctLogServer_isEnabled = (TFctLogServer_isEnabled)GetProcAddress(s_hndDllIf, "LogServer_isEnabled");
        if( s_pFctLogServer_isEnabled == NULL ) bOk = false;

        s_pFctLogServer_isActive = (TFctLogServer_isActive)GetProcAddress(s_hndDllIf, "LogServer_isActive");
        if( s_pFctLogServer_isActive == NULL ) bOk = false;

        s_pFctLogServer_log = (TFctLogServer_log)GetProcAddress(s_hndDllIf, "LogServer_log");
        if( s_pFctLogServer_log == NULL ) bOk = false;

        s_pFctLogServer_setNewLoggersEnabledAsDefault = (TFctLogServer_setNewLoggersEnabledAsDefault)GetProcAddress(s_hndDllIf, "LogServer_setNewLoggersEnabledAsDefault");
        if( s_pFctLogServer_setNewLoggersEnabledAsDefault == NULL ) bOk = false;

        s_pFctLogServer_areNewLoggersEnabledAsDefault = (TFctLogServer_areNewLoggersEnabledAsDefault)GetProcAddress(s_hndDllIf, "LogServer_areNewLoggersEnabledAsDefault");
        if( s_pFctLogServer_areNewLoggersEnabledAsDefault == NULL ) bOk = false;

        s_pFctLogServer_getNewLoggersDefaultDetailLevel = (TFctLogServer_getNewLoggersDefaultDetailLevel)GetProcAddress(s_hndDllIf, "LogServer_getNewLoggersDefaultDetailLevel");
        if( s_pFctLogServer_getNewLoggersDefaultDetailLevel == NULL ) bOk = false;

        s_pFctLogServer_setNewLoggersDefaultDetailLevel = (TFctLogServer_setNewLoggersDefaultDetailLevel)GetProcAddress(s_hndDllIf, "LogServer_setNewLoggersDefaultDetailLevel");
        if( s_pFctLogServer_setNewLoggersDefaultDetailLevel == NULL ) bOk = false;

        s_pFctLogServer_recallLoggers = (TFctLogServer_recallLoggers)GetProcAddress(s_hndDllIf, "LogServer_recallLoggers");
        if( s_pFctLogServer_recallLoggers == NULL ) bOk = false;

        s_pFctLogServer_saveLoggers = (TFctLogServer_saveLoggers)GetProcAddress(s_hndDllIf, "LogServer_saveLoggers");
        if( s_pFctLogServer_saveLoggers == NULL ) bOk = false;

        s_pFctLogServer_setUseLocalLogFile = (TFctLogServer_setUseLocalLogFile)GetProcAddress(s_hndDllIf, "LogServer_setUseLocalLogFile");
        if( s_pFctLogServer_setUseLocalLogFile == NULL ) bOk = false;

        s_pFctLogServer_isLocalLogFileUsed = (TFctLogServer_isLocalLogFileUsed)GetProcAddress(s_hndDllIf, "LogServer_isLocalLogFileUsed");
        if( s_pFctLogServer_isLocalLogFileUsed == NULL ) bOk = false;

        s_pFctLogServer_isLocalLogFileActive = (TFctLogServer_isLocalLogFileActive)GetProcAddress(s_hndDllIf, "LogServer_isLocalLogFileActive");
        if( s_pFctLogServer_isLocalLogFileActive == NULL ) bOk = false;

        s_pFctLogServer_setLocalLogFileAutoSaveIntervalInMs = (TFctLogServer_setLocalLogFileAutoSaveIntervalInMs)GetProcAddress(s_hndDllIf, "LogServer_setLocalLogFileAutoSaveIntervalInMs");
        if( s_pFctLogServer_setLocalLogFileAutoSaveIntervalInMs == NULL ) bOk = false;

        s_pFctLogServer_getLocalLogFileAutoSaveIntervalInMs = (TFctLogServer_getLocalLogFileAutoSaveIntervalInMs)GetProcAddress(s_hndDllIf, "LogServer_getLocalLogFileAutoSaveIntervalInMs");
        if( s_pFctLogServer_getLocalLogFileAutoSaveIntervalInMs == NULL ) bOk = false;

        s_pFctLogServer_setLocalLogFileCloseFileAfterEachWrite = (TFctLogServer_setLocalLogFileCloseFileAfterEachWrite)GetProcAddress(s_hndDllIf, "LogServer_setLocalLogFileCloseFileAfterEachWrite");
        if( s_pFctLogServer_setLocalLogFileCloseFileAfterEachWrite == NULL ) bOk = false;

        s_pFctLogServer_getLocalLogFileCloseFileAfterEachWrite = (TFctLogServer_getLocalLogFileCloseFileAfterEachWrite)GetProcAddress(s_hndDllIf, "LogServer_getLocalLogFileCloseFileAfterEachWrite");
        if( s_pFctLogServer_getLocalLogFileCloseFileAfterEachWrite == NULL ) bOk = false;

        s_pFctLogServer_setLocalLogFileSubFileCountMax = (TFctLogServer_setLocalLogFileSubFileCountMax)GetProcAddress(s_hndDllIf, "LogServer_setLocalLogFileSubFileCountMax");
        if( s_pFctLogServer_setLocalLogFileSubFileCountMax == NULL ) bOk = false;

        s_pFctLogServer_getLocalLogFileSubFileCountMax = (TFctLogServer_getLocalLogFileSubFileCountMax)GetProcAddress(s_hndDllIf, "LogServer_getLocalLogFileSubFileCountMax");
        if( s_pFctLogServer_getLocalLogFileSubFileCountMax == NULL ) bOk = false;

        s_pFctLogServer_setLocalLogFileSubFileLineCountMax = (TFctLogServer_setLocalLogFileSubFileLineCountMax)GetProcAddress(s_hndDllIf, "LogServer_setLocalLogFileSubFileLineCountMax");
        if( s_pFctLogServer_setLocalLogFileSubFileLineCountMax == NULL ) bOk = false;

        s_pFctLogServer_getLocalLogFileSubFileLineCountMax = (TFctLogServer_getLocalLogFileSubFileLineCountMax)GetProcAddress(s_hndDllIf, "LogServer_getLocalLogFileSubFileLineCountMax");
        if( s_pFctLogServer_getLocalLogFileSubFileLineCountMax == NULL ) bOk = false;

        s_pFctLogServer_setUseIpcServer = (TFctLogServer_setUseIpcServer)GetProcAddress(s_hndDllIf, "LogServer_setUseIpcServer");
        if( s_pFctLogServer_setUseIpcServer == NULL ) bOk = false;

        s_pFctLogServer_isIpcServerUsed = (TFctLogServer_isIpcServerUsed)GetProcAddress(s_hndDllIf, "LogServer_isIpcServerUsed");
        if( s_pFctLogServer_isIpcServerUsed == NULL ) bOk = false;

        s_pFctLogServer_setCacheLogDataIfNotConnected = (TFctLogServer_setCacheLogDataIfNotConnected)GetProcAddress(s_hndDllIf, "LogServer_setCacheLogDataIfNotConnected");
        if( s_pFctLogServer_setCacheLogDataIfNotConnected == NULL ) bOk = false;

        s_pFctLogServer_getCacheLogDataIfNotConnected = (TFctLogServer_getCacheLogDataIfNotConnected)GetProcAddress(s_hndDllIf, "LogServer_getCacheLogDataIfNotConnected");
        if( s_pFctLogServer_getCacheLogDataIfNotConnected == NULL ) bOk = false;

        s_pFctLogServer_setCacheLogDataMaxArrLen = (TFctLogServer_setCacheLogDataMaxArrLen)GetProcAddress(s_hndDllIf, "LogServer_setCacheLogDataMaxArrLen");
        if( s_pFctLogServer_setCacheLogDataMaxArrLen == NULL ) bOk = false;

        s_pFctLogServer_getCacheLogDataMaxArrLen = (TFctLogServer_getCacheLogDataMaxArrLen)GetProcAddress(s_hndDllIf, "LogServer_getCacheLogDataMaxArrLen");
        if( s_pFctLogServer_getCacheLogDataMaxArrLen == NULL ) bOk = false;

        s_pFctLogServer_setLogSettings = (TFctLogServer_setLogSettings)GetProcAddress(s_hndDllIf, "LogServer_setLogSettings");
        if( s_pFctLogServer_setLogSettings == NULL ) bOk = false;

        s_pFctLogServer_getLogSettings = (TFctLogServer_getLogSettings)GetProcAddress(s_hndDllIf, "LogServer_getLogSettings");
        if( s_pFctLogServer_getLogSettings == NULL ) bOk = false;

        s_pFctLogServer_clearLocalLogFile = (TFctLogServer_clearLocalLogFile)GetProcAddress(s_hndDllIf, "LogServer_clearLocalLogFile");
        if( s_pFctLogServer_clearLocalLogFile == NULL ) bOk = false;

        // IpcLogServer
        //-------------

        s_pFctIpcLogServer_GetInstance = (TFctIpcLogServer_GetInstance)GetProcAddress(s_hndDllIf, "IpcLogServer_GetInstance");
        if( s_pFctIpcLogServer_GetInstance == NULL ) bOk = false;

        s_pFctIpcLogServer_CreateInstance = (TFctIpcLogServer_CreateInstance)GetProcAddress(s_hndDllIf, "IpcLogServer_CreateInstance");
        if( s_pFctIpcLogServer_CreateInstance == NULL ) bOk = false;

        s_pFctIpcLogServer_ReleaseInstance = (TFctIpcLogServer_ReleaseInstance)GetProcAddress(s_hndDllIf, "IpcLogServer_ReleaseInstance");
        if( s_pFctIpcLogServer_ReleaseInstance == NULL ) bOk = false;

        s_pFctIpcLogServer_startup = (TFctIpcLogServer_startup)GetProcAddress(s_hndDllIf, "IpcLogServer_startup");
        if( s_pFctIpcLogServer_startup == NULL ) bOk = false;

        s_pFctIpcLogServer_shutdown = (TFctIpcLogServer_shutdown)GetProcAddress(s_hndDllIf, "IpcLogServer_shutdown");
        if( s_pFctIpcLogServer_shutdown == NULL ) bOk = false;

        s_pFctIpcLogServer_isListening = (TFctIpcLogServer_isListening)GetProcAddress(s_hndDllIf, "IpcLogServer_isListening");
        if( s_pFctIpcLogServer_isListening == NULL ) bOk = false;

        s_pFctIpcLogServer_isConnected = (TFctIpcLogServer_isConnected)GetProcAddress(s_hndDllIf, "IpcLogServer_isConnected");
        if( s_pFctIpcLogServer_isConnected == NULL ) bOk = false;

        s_pFctIpcLogServer_setPort = (TFctIpcLogServer_setPort)GetProcAddress(s_hndDllIf, "IpcLogServer_setPort");
        if( s_pFctIpcLogServer_setPort == NULL ) bOk = false;

        s_pFctIpcLogServer_getPort = (TFctIpcLogServer_getPort)GetProcAddress(s_hndDllIf, "IpcLogServer_getPort");
        if( s_pFctIpcLogServer_getPort == NULL ) bOk = false;

    } // if( s_hndDllIf != NULL )

    return bOk;

} // loadDll

//------------------------------------------------------------------------------
/*! Returns the file name of the loaded ZSIpcLog Dll.

    @ingroup _GRP_Namespace_ZS_Trace_DllIf

    @return name of the dll.
        ! Don't free this pointer as this is a const char pointer created during compile time !
*/
const char* ZS::Log::DllIf::getDllFileName()
//------------------------------------------------------------------------------
{
    return s_szDllFileName;
}

//------------------------------------------------------------------------------
/*! Releases the Remote Logging Dlls.

    @ingroup _GRP_Namespace_ZS_Trace_DllIf

    @return true if dll was release.
            false in case of an error (if the dll was not loaded).
*/
bool ZS::Log::DllIf::releaseDll()
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndDllIf != NULL )
    {
        bOk = true;

        #ifdef _WINDOWS
        FreeLibrary(s_hndDllIf);
        #elif defined __linux__
        dlclose(s_hndDllIf);
        #endif
        s_hndDllIf = NULL;

        s_pFctLogger_log                      = NULL;
        s_pFctLogger_setEnabled               = NULL;
        s_pFctLogger_isEnabled                = NULL;
        s_pFctLogger_setLogLevel              = NULL;
        s_pFctLogger_getLogLevel              = NULL;
        s_pFctLogger_isActive                 = NULL;
        s_pFctLogger_setDataFilter            = NULL;
        s_pFctLogger_getDataFilter            = NULL;
        s_pFctLogger_isSuppressedByDataFilter = NULL;
        s_pFctLogger_setAddThreadName         = NULL;
        s_pFctLogger_addThreadName            = NULL;
        s_pFctLogger_setAddDateTime           = NULL;
        s_pFctLogger_addDateTime              = NULL;
        s_pFctLogger_setAddSystemTime         = NULL;
        s_pFctLogger_addSystemTime            = NULL;
        s_pFctLogger_setNameSpace             = NULL;
        s_pFctLogger_getNameSpace             = NULL;
        s_pFctLogger_setClassName             = NULL;
        s_pFctLogger_getClassName             = NULL;
        s_pFctLogger_setObjectName            = NULL;
        s_pFctLogger_getObjectName            = NULL;
        s_pFctLogger_name                     = NULL;

        s_pFctLogServer_SetOrganizationName                    = NULL;
        s_pFctLogServer_GetOrganizationName                    = NULL;
        s_pFctLogServer_SetApplicationName                     = NULL;
        s_pFctLogServer_GetApplicationName                     = NULL;
        s_pFctLogServer_SetLoggerFileAbsoluteFilePath          = NULL;
        s_pFctLogServer_GetLoggerFileAbsoluteFilePath          = NULL;
        s_pFctLogServer_GetLoggerFileCompleteBaseName          = NULL;
        s_pFctLogServer_GetLoggerFileAbsolutePath              = NULL;
        s_pFctLogServer_SetLocalLogFileAbsoluteFilePath        = NULL;
        s_pFctLogServer_GetLocalLogFileAbsoluteFilePath        = NULL;
        s_pFctLogServer_GetLocalLogFileCompleteBaseName        = NULL;
        s_pFctLogServer_GetLocalLogFileAbsolutePath            = NULL;
        s_pFctLogServer_RegisterCurrentThread                  = NULL;
        s_pFctLogServer_UnregisterCurrentThread                = NULL;
        s_pFctLogServer_GetCurrentThreadName                   = NULL;
        s_pFctLogServer_GetLogger                              = NULL;
        s_pFctLogServer_setEnabled                             = NULL;
        s_pFctLogServer_isEnabled                              = NULL;
        s_pFctLogServer_isActive                               = NULL;
        s_pFctLogServer_log                                    = NULL;
        s_pFctLogServer_setNewLoggersEnabledAsDefault          = NULL;
        s_pFctLogServer_areNewLoggersEnabledAsDefault          = NULL;
        s_pFctLogServer_setNewLoggersDefaultDetailLevel        = NULL;
        s_pFctLogServer_getNewLoggersDefaultDetailLevel        = NULL;
        s_pFctLogServer_recallLoggers                          = NULL;
        s_pFctLogServer_saveLoggers                            = NULL;
        s_pFctLogServer_setUseLocalLogFile                     = NULL;
        s_pFctLogServer_isLocalLogFileUsed                     = NULL;
        s_pFctLogServer_isLocalLogFileActive                   = NULL;
        s_pFctLogServer_setLocalLogFileAutoSaveIntervalInMs    = NULL;
        s_pFctLogServer_getLocalLogFileAutoSaveIntervalInMs    = NULL;
        s_pFctLogServer_setLocalLogFileCloseFileAfterEachWrite = NULL;
        s_pFctLogServer_getLocalLogFileCloseFileAfterEachWrite = NULL;
        s_pFctLogServer_setLocalLogFileSubFileCountMax         = NULL;
        s_pFctLogServer_getLocalLogFileSubFileCountMax         = NULL;
        s_pFctLogServer_setLocalLogFileSubFileLineCountMax     = NULL;
        s_pFctLogServer_getLocalLogFileSubFileLineCountMax     = NULL;
        s_pFctLogServer_setUseIpcServer                        = NULL;
        s_pFctLogServer_isIpcServerUsed                        = NULL;
        s_pFctLogServer_setCacheLogDataIfNotConnected          = NULL;
        s_pFctLogServer_getCacheLogDataIfNotConnected          = NULL;
        s_pFctLogServer_setCacheLogDataMaxArrLen               = NULL;
        s_pFctLogServer_getCacheLogDataMaxArrLen               = NULL;
        s_pFctLogServer_setLogSettings                         = NULL;
        s_pFctLogServer_getLogSettings                         = NULL;
        s_pFctLogServer_clearLocalLogFile                      = NULL;

        s_pFctIpcLogServer_GetInstance     = NULL;
        s_pFctIpcLogServer_CreateInstance  = NULL;
        s_pFctIpcLogServer_ReleaseInstance = NULL;
        s_pFctIpcLogServer_startup         = NULL;
        s_pFctIpcLogServer_shutdown        = NULL;
        s_pFctIpcLogServer_isListening     = NULL;
        s_pFctIpcLogServer_isConnected     = NULL;
        s_pFctIpcLogServer_setPort         = NULL;
        s_pFctIpcLogServer_getPort         = NULL;
    }

    return bOk;

} // releaseDll


/*******************************************************************************
class CLogger
*******************************************************************************/

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a log entry for the given log level.

    See CLogger::log for more details.

    @param i_eFilterDetailLevel [in]
        If the given filter detail level is not None and is equal or greater
        than the current detail level of the log server the log entry will
        be added the log file.
    @param i_szLogEntry [in] String to be logged.
*/
void DllIf::CLogger::log( ELogDetailLevel i_eFilterDetailLevel, const char* i_szLogEntry )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_log != NULL )
    {
        s_pFctLogger_log(this, i_eFilterDetailLevel, i_szLogEntry);
    }
}

//------------------------------------------------------------------------------
/*! @brief Enables log output for this object.

    See CLogger::setEnabled for more details.

    @param i_bEnabled [in] Flag to enable or disable log outputs.
                          - true ... logging is enabled
                          - false .. logging is disabled
*/
void DllIf::CLogger::setEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_setEnabled != NULL )
    {
        s_pFctLogger_setEnabled(this, i_bEnabled);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns whether tracing is enabled or disabled.

    @return Flag indicating whether log output is enabled or disabled.
*/
bool DllIf::CLogger::isEnabled() const
//------------------------------------------------------------------------------
{
    bool bEnabled = false;

    if( s_hndDllIf != NULL && s_pFctLogger_isEnabled != NULL )
    {
        bEnabled = s_pFctLogger_isEnabled(this);
    }
    return bEnabled;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the detail level of log output for this object.

    See CLogger::setDetailLevel for more details.

    @param i_eDetailLevel [in] Detail level.
*/
void DllIf::CLogger::setLogLevel( ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_setLogLevel != NULL )
    {
        s_pFctLogger_setLogLevel(this, i_eDetailLevel);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the detail level of log output for this object.

    @return Detail level.
*/
 DllIf::ELogDetailLevel DllIf::CLogger::getLogLevel() const
//------------------------------------------------------------------------------
{
    ELogDetailLevel eDetailLevel = ELogDetailLevelNone;

    if( s_hndDllIf != NULL && s_pFctLogger_getLogLevel != NULL )
    {
        eDetailLevel = s_pFctLogger_getLogLevel(this);
    }
    return eDetailLevel;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether logging is activated or disactived.

    See CLogger::isActive for more details.

    @param i_eFilterDetailLevel [in]
        Log outputs should be generated if the given filter detail level
        is greater or equal than the current detail level set at the logger.

    @return Flag indicating whether log output is active or not.
*/
bool DllIf::CLogger::isActive( ELogDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( s_hndDllIf != NULL && s_pFctLogger_isActive != NULL )
    {
        bIsActive = s_pFctLogger_isActive(this, i_eFilterDetailLevel);
    }
    return bIsActive;
}

//------------------------------------------------------------------------------
/*! @brief Sets the log data filter as a regular expression.

    The filter is a regular expression which allows to define a positive
    pattern where only the data will be logged which mets the expression
    or a negative pattern which suppressed the log output if the
    filter does not match.

    Examples

    @param i_szFilter [in] Filter as regular expression.
*/
void DllIf::CLogger::setDataFilter( const char* i_szFilter )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_setDataFilter != NULL )
    {
        s_pFctLogger_setDataFilter(this, i_szFilter);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the log data filter.

    @return Data filter (string containing a regular expression).
            The ownership is passed to the caller of the method.
            The caller must delete the returned string.
*/
char* DllIf::CLogger::getDataFilter() const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_getDataFilter != NULL )
    {
        return s_pFctLogger_getDataFilter(this);
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether given log data should be suppressed by the data filter.

    @param i_szLogEntry [in]
        Log entry to be checked against the filter string.

    @return true if the passed log entry should be suppressed, false otherwise.
*/
bool DllIf::CLogger::isSuppressedByDataFilter( const char* i_szLogEntry ) const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_isSuppressedByDataFilter != NULL )
    {
        return s_pFctLogger_isSuppressedByDataFilter(this, i_szLogEntry);
    }
    return false;
}

//------------------------------------------------------------------------------
/*! @brief Sets the flag which indicates whether the current thread name
           should be added to the log entry.

    See CLogger::setAddThreadName for more details.

    @param i_bAdd [in]
        true if the current thread name should be added, false otherwise.
*/
void DllIf::CLogger::setAddThreadName( bool i_bAdd )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_setAddThreadName != NULL )
    {
        s_pFctLogger_setAddThreadName(this, i_bAdd);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the flag indicating whether the current thread name should be
           added to the log entry.

    See CLogger::addThreadName for more details.

    @return true if the current thread name should be added, false otherwise.
*/
bool DllIf::CLogger::addThreadName() const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_addThreadName != NULL )
    {
        return s_pFctLogger_addThreadName(this);
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! @brief Sets the flag which indicates whether the current date time
           should be added to the log entry.

    See CLogger::setAddDateTime for more details.

    @param i_bAdd [in]
        true if the current date time should be added, false otherwise.
*/
void DllIf::CLogger::setAddDateTime( bool i_bAdd )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_setAddDateTime != NULL )
    {
        s_pFctLogger_setAddDateTime(this, i_bAdd);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the flag indicating whether the current date time should be
           added to the log entry.

    See CLogger::addDateTime for more details.

    @return true if the current date time should be added, false otherwise.
*/
bool DllIf::CLogger::addDateTime() const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_addDateTime != NULL )
    {
        return s_pFctLogger_addDateTime(this);
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! @brief Sets the flag which indicates whether the current system time
           should be added to the log entry.

    See CLogger::setAddSystemTime for more details.

    @param i_bAdd [in]
        true if the current system time should be added, false otherwise.
*/
void DllIf::CLogger::setAddSystemTime( bool i_bAdd )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_setAddSystemTime != NULL )
    {
        s_pFctLogger_setAddSystemTime(this, i_bAdd);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the flag indicating whether the current system time should be
           added to the log entry.

    See CLogger::addSystemTime for more details.

    @return true if the current system time should be added, false otherwise.
*/
bool DllIf::CLogger::addSystemTime() const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_addSystemTime != NULL )
    {
        return s_pFctLogger_addSystemTime(this);
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! @brief Sets the name space of the logger.

    See CLogger::setNameSpace for more details.

    @param i_szNameSpace [in]
        Name space used by the logger to format the log entry string.
        Use empty string to remove the name space.
*/
void DllIf::CLogger::setNameSpace( const char* i_szNameSpace )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_setNameSpace != NULL )
    {
        s_pFctLogger_setNameSpace(this, i_szNameSpace);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the name space of the logger.

    See CLogger::getNameSpace for more details.

    @return Character string pointing to the name space.
            The ownership is passed to the caller so it must be free by the caller.
*/
char* DllIf::CLogger::getNameSpace() const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_getNameSpace != NULL )
    {
        return s_pFctLogger_getNameSpace(this);
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! @brief Sets the class name of the logger.

    See CLogger::setClassName for more details.

    @param i_szClassName [in]
        Class name used by the logger to format the log entry string.
        Use empty string to remove the class name.
*/
void DllIf::CLogger::setClassName( const char* i_szClassName )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_setClassName != NULL )
    {
        s_pFctLogger_setClassName(this, i_szClassName);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the class name of the logger.

    See CLogger::getClassName for more details.

    @return Character string pointing to the class name.
            The ownership is passed to the caller so it must be free by the caller.
*/
char* DllIf::CLogger::getClassName() const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_getClassName != NULL )
    {
        return s_pFctLogger_getClassName(this);
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! @brief Sets the object name of the logger.

    See CLogger::setObjectName for more details.

    @param i_szObjectName [in]
        Object name used by the logger to format the log entry string.
        Use empty string to remove the object name.
*/
void DllIf::CLogger::setObjectName( const char* i_szObjectName )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_setObjectName != NULL )
    {
        s_pFctLogger_setObjectName(this, i_szObjectName);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the object name of the logger.

    See CLogger::getObjectName for more details.

    @return Character string pointing to the object name.
            The ownership is passed to the caller so it must be free by the caller.
*/
char* DllIf::CLogger::getObjectName() const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_getObjectName != NULL )
    {
        return s_pFctLogger_getObjectName(this);
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! @brief Returns the name of the logger.

    The name of the logger corresponds to the path of the logger leave entry
    in the index tree.

    See CLogger::path for more details.

    @return Character string pointing to the name.
            The ownership is passed to the caller so it must be free by the caller.
*/
char* DllIf::CLogger::name() const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogger_name != NULL )
    {
        return s_pFctLogger_name(this);
    }
    return NULL;
}


/*******************************************************************************
class CLogServer
*******************************************************************************/

/*==============================================================================
public: // class method to save/recall loggers file
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the name of the organization.

    @param i_szName [in] Name of the organization.
*/
void DllIf::CLogServer::SetOrganizationName( const char* i_szName )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_SetOrganizationName != NULL )
    {
        s_pFctLogServer_SetOrganizationName(i_szName);
    }
}

//------------------------------------------------------------------------------
/*! Returns the name of the organization.

    @return Name of the organization. The string must be freed by the caller.
*/
char* DllIf::CLogServer::GetOrganizationName()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_GetOrganizationName != NULL )
    {
        return s_pFctLogServer_GetOrganizationName();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! Sets the name of the application.

    @param i_szName [in] Name of the application.
*/
void DllIf::CLogServer::SetApplicationName( const char* i_szName )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_SetApplicationName != NULL )
    {
        s_pFctLogServer_SetApplicationName(i_szName);
    }
}

//------------------------------------------------------------------------------
/*! Returns the name of the application.

    @return Name of the application. The string must be freed by the caller.
*/
char* DllIf::CLogServer::GetApplicationName()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_GetApplicationName != NULL )
    {
        return s_pFctLogServer_GetApplicationName();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! Sets the absolute file path for loggers xml file.

    @param i_szAbsFilePath [in]
        Absolute file path including directory path, file name and suffix.
*/
void DllIf::CLogServer::SetLoggerFileAbsoluteFilePath( const char* i_szAbsFilePath )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_SetLoggerFileAbsoluteFilePath != NULL )
    {
        s_pFctLogServer_SetLoggerFileAbsoluteFilePath(i_szAbsFilePath);
    }
}

//------------------------------------------------------------------------------
/*! Returns the path information of the loggers xml file.

    The returned character pointer must be freed after calling the method.

    @return Character buffer containing the files absolute path.
            The caller must free this buffer.
*/
char* DllIf::CLogServer::GetLoggerFileAbsoluteFilePath()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_GetLoggerFileAbsoluteFilePath != NULL )
    {
        return s_pFctLogServer_GetLoggerFileAbsoluteFilePath();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! Returns the base name of the loggers xml file.

    The returned character pointer must be freed after calling the method.

    @return Character buffer containing the base name.
            The caller must free this buffer.
*/
char* DllIf::CLogServer::GetLoggerFileCompleteBaseName()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_GetLoggerFileCompleteBaseName != NULL )
    {
        return s_pFctLogServer_GetLoggerFileCompleteBaseName();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! Returns the absolute path of the loggers xml file.

    The returned character pointer must be freed after calling the method.

    @return Character buffer containing the files absolute path (without filename).
            The caller must free this buffer.
*/
char* DllIf::CLogServer::GetLoggerFileAbsolutePath()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_GetLoggerFileAbsolutePath != NULL )
    {
        return s_pFctLogServer_GetLoggerFileAbsolutePath();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! Sets the absolute file path for the log file.

    @param i_szAbsFilePath [in]
        Absolute file path including directory path, file name and suffix.
*/
void DllIf::CLogServer::SetLocalLogFileAbsoluteFilePath( const char* i_szAbsFilePath )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_SetLocalLogFileAbsoluteFilePath != NULL )
    {
        s_pFctLogServer_SetLocalLogFileAbsoluteFilePath(i_szAbsFilePath);
    }
}

//------------------------------------------------------------------------------
/*! Returns the path information for the log file.

    The returned character pointer must be freed after calling the method.

    @return Character buffer containing the files absolute path. The caller
            must free this buffer.
*/
char* DllIf::CLogServer::GetLocalLogFileAbsoluteFilePath()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_GetLocalLogFileAbsoluteFilePath != NULL )
    {
        return s_pFctLogServer_GetLocalLogFileAbsoluteFilePath();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! Returns the base name of the log file.

    The returned character pointer must be freed after calling the method.

    @return Character buffer containing the base name.
            The caller must free this buffer.
*/
char* DllIf::CLogServer::GetLocalLogFileCompleteBaseName()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_GetLocalLogFileCompleteBaseName != NULL )
    {
        return s_pFctLogServer_GetLocalLogFileCompleteBaseName();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! Returns the absolute path of the log file.

    The returned character pointer must be freed after calling the method.

    @return Character buffer containing the files absolute path (without filename).
            The caller must free this buffer.
*/
char* DllIf::CLogServer::GetLocalLogFileAbsolutePath()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_GetLocalLogFileAbsolutePath != NULL )
    {
        return s_pFctLogServer_GetLocalLogFileAbsolutePath();
    }
    return NULL;
}

//------------------------------------------------------------------------------
void DllIf::CLogServer::RegisterCurrentThread( const char* i_szThreadName )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_RegisterCurrentThread != NULL )
    {
        s_pFctLogServer_RegisterCurrentThread(i_szThreadName);
    }
}

//------------------------------------------------------------------------------
void DllIf::CLogServer::UnregisterCurrentThread()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_UnregisterCurrentThread != NULL )
    {
        s_pFctLogServer_UnregisterCurrentThread();
    }
}

//------------------------------------------------------------------------------
char* DllIf::CLogServer::GetCurrentThreadName()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_GetCurrentThreadName != NULL )
    {
        return s_pFctLogServer_GetCurrentThreadName();
    }
    return NULL;
}

/*==============================================================================
public: // class methods to add, remove and modify loggers
==============================================================================*/

//------------------------------------------------------------------------------
/*!
*/
DllIf::CLogger* DllIf::CLogServer::GetLogger()
//------------------------------------------------------------------------------
{
    DllIf::CLogger* pLogger = NULL;

    if( s_hndDllIf != NULL && s_pFctLogServer_GetLogger != NULL )
    {
        pLogger = s_pFctLogServer_GetLogger("", DllIf::EEnabledUndefined, DllIf::ELogDetailLevelUndefined);
    }
    return pLogger;
}

//------------------------------------------------------------------------------
/*!
    @param i_szName [in]
    @param i_bEnabledAsDefault [in]
    @param i_iDefaultDetailLevel [in]
*/
DllIf::CLogger* DllIf::CLogServer::GetLogger(
    const char*     i_szName,
    EEnabled        i_bEnabledAsDefault,
    ELogDetailLevel i_eDefaultDetailLevel )
//------------------------------------------------------------------------------
{
    DllIf::CLogger* pLogger = NULL;

    if( s_hndDllIf != NULL && s_pFctLogServer_GetLogger != NULL )
    {
        pLogger = s_pFctLogServer_GetLogger(i_szName, i_bEnabledAsDefault, i_eDefaultDetailLevel);
    }
    return pLogger;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CLogServer::setEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_setEnabled != NULL )
    {
        s_pFctLogServer_setEnabled(i_bEnabled);
    }
}

//------------------------------------------------------------------------------
bool DllIf::CLogServer::isEnabled() const
//------------------------------------------------------------------------------
{
    bool bEnabled = false;

    if( s_hndDllIf != NULL && s_pFctLogServer_isEnabled != NULL )
    {
        bEnabled = s_pFctLogServer_isEnabled();
    }
    return bEnabled;
}

//------------------------------------------------------------------------------
bool DllIf::CLogServer::isActive() const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( s_hndDllIf != NULL && s_pFctLogServer_isActive != NULL )
    {
        bIsActive = s_pFctLogServer_isActive();
    }
    return bIsActive;
}

//------------------------------------------------------------------------------
/*!
*/
DllIf::CLogger* DllIf::CLogServer::getLogger()
//------------------------------------------------------------------------------
{
    DllIf::CLogger* pLogger = NULL;

    if( s_hndDllIf != NULL && s_pFctLogServer_GetLogger != NULL )
    {
        pLogger = s_pFctLogServer_GetLogger("", DllIf::EEnabledUndefined, DllIf::ELogDetailLevelUndefined);
    }
    return pLogger;
}

//------------------------------------------------------------------------------
/*!
    @param i_szName [in]
    @param i_bEnabledAsDefault [in]
    @param i_iDefaultDetailLevel [in]
*/
DllIf::CLogger* DllIf::CLogServer::getLogger(
    const char*     i_szName,
    EEnabled        i_bEnabledAsDefault,
    ELogDetailLevel i_eDefaultDetailLevel )
//------------------------------------------------------------------------------
{
    DllIf::CLogger* pLogger = NULL;

    if( s_hndDllIf != NULL && s_pFctLogServer_GetLogger != NULL )
    {
        pLogger = s_pFctLogServer_GetLogger(i_szName, i_bEnabledAsDefault, i_eDefaultDetailLevel);
    }
    return pLogger;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CLogServer::log( ELogDetailLevel i_eFilterDetailLevel, const char* i_szLogEntry)
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_log != NULL )
    {
        s_pFctLogServer_log(nullptr, i_eFilterDetailLevel, i_szLogEntry);
    }
}

//------------------------------------------------------------------------------
void DllIf::CLogServer::log(
    const CLogger*  i_pLogger,
    ELogDetailLevel i_eFilterDetailLevel,
    const char*     i_szLogEntry)
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_log != NULL )
    {
        s_pFctLogServer_log(i_pLogger, i_eFilterDetailLevel, i_szLogEntry);
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CLogServer::setNewLoggersEnabledAsDefault( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_setNewLoggersEnabledAsDefault != NULL )
    {
        s_pFctLogServer_setNewLoggersEnabledAsDefault(i_bEnabled);
    }
}

//------------------------------------------------------------------------------
bool DllIf::CLogServer::areNewLoggersEnabledAsDefault() const
//------------------------------------------------------------------------------
{
    bool bEnabled = false;

    if( s_hndDllIf != NULL && s_pFctLogServer_areNewLoggersEnabledAsDefault != NULL )
    {
        bEnabled = s_pFctLogServer_areNewLoggersEnabledAsDefault();
    }
    return bEnabled;
}

//------------------------------------------------------------------------------
void DllIf::CLogServer::setNewLoggersDefaultDetailLevel( ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_setNewLoggersDefaultDetailLevel != NULL )
    {
        s_pFctLogServer_setNewLoggersDefaultDetailLevel(i_eDetailLevel);
    }
}

//------------------------------------------------------------------------------
DllIf::ELogDetailLevel DllIf::CLogServer::getNewLoggersDefaultDetailLevel() const
//------------------------------------------------------------------------------
{
    ELogDetailLevel eDetailLevel = ELogDetailLevelNone;

    if( s_hndDllIf != NULL && s_pFctLogServer_getNewLoggersDefaultDetailLevel != NULL )
    {
        eDetailLevel = s_pFctLogServer_getNewLoggersDefaultDetailLevel();
    }
    return eDetailLevel;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool DllIf::CLogServer::recallLoggers()
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndDllIf != NULL && s_pFctLogServer_recallLoggers != NULL )
    {
        bOk = s_pFctLogServer_recallLoggers();
    }
    return bOk;
}

//------------------------------------------------------------------------------
bool DllIf::CLogServer::saveLoggers()
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndDllIf != NULL && s_pFctLogServer_saveLoggers != NULL )
    {
        bOk = s_pFctLogServer_saveLoggers();
    }
    return bOk;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CLogServer::setUseLocalLogFile( bool i_bUse )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_setUseLocalLogFile != NULL )
    {
        s_pFctLogServer_setUseLocalLogFile(i_bUse);
    }
}

//------------------------------------------------------------------------------
bool DllIf::CLogServer::isLocalLogFileUsed() const
//------------------------------------------------------------------------------
{
    bool bUsed = false;

    if( s_hndDllIf != NULL && s_pFctLogServer_isLocalLogFileUsed != NULL )
    {
        bUsed = s_pFctLogServer_isLocalLogFileUsed();
    }
    return bUsed;
}

//------------------------------------------------------------------------------
bool DllIf::CLogServer::isLocalLogFileActive() const
//------------------------------------------------------------------------------
{
    bool bUsed = false;

    if( s_hndDllIf != NULL && s_pFctLogServer_isLocalLogFileActive != NULL )
    {
        bUsed = s_pFctLogServer_isLocalLogFileActive();
    }
    return bUsed;
}

//------------------------------------------------------------------------------
void DllIf::CLogServer::setLocalLogFileAutoSaveIntervalInMs( int i_iAutoSaveInterval_ms )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_setLocalLogFileAutoSaveIntervalInMs != NULL )
    {
        s_pFctLogServer_setLocalLogFileAutoSaveIntervalInMs(i_iAutoSaveInterval_ms);
    }
}

//------------------------------------------------------------------------------
int DllIf::CLogServer::getLocalLogFileAutoSaveIntervalInMs() const
//------------------------------------------------------------------------------
{
    int iAutoSaveInterval_ms = 0;

    if( s_hndDllIf != NULL && s_pFctLogServer_getLocalLogFileAutoSaveIntervalInMs != NULL )
    {
        iAutoSaveInterval_ms = s_pFctLogServer_getLocalLogFileAutoSaveIntervalInMs();
    }
    return iAutoSaveInterval_ms;
}

//------------------------------------------------------------------------------
void DllIf::CLogServer::setLocalLogFileCloseFileAfterEachWrite( bool i_bCloseFile )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_setLocalLogFileCloseFileAfterEachWrite != NULL )
    {
        s_pFctLogServer_setLocalLogFileCloseFileAfterEachWrite(i_bCloseFile);
    }
}

//------------------------------------------------------------------------------
bool DllIf::CLogServer::getLocalLogFileCloseFileAfterEachWrite() const
//------------------------------------------------------------------------------
{
    bool bClose = false;

    if( s_hndDllIf != NULL && s_pFctLogServer_getLocalLogFileCloseFileAfterEachWrite != NULL )
    {
        bClose = s_pFctLogServer_getLocalLogFileCloseFileAfterEachWrite();
    }
    return bClose;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CLogServer::setLocalLogFileSubFileCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_setLocalLogFileSubFileCountMax != NULL )
    {
        s_pFctLogServer_setLocalLogFileSubFileCountMax(i_iCountMax);
    }
}

//------------------------------------------------------------------------------
int DllIf::CLogServer::getLocalLogFileSubFileCountMax() const
//------------------------------------------------------------------------------
{
    int iCountMax = 0;

    if( s_hndDllIf != NULL && s_pFctLogServer_getLocalLogFileSubFileCountMax != NULL )
    {
        iCountMax = s_pFctLogServer_getLocalLogFileSubFileCountMax();
    }
    return iCountMax;
}

//------------------------------------------------------------------------------
void DllIf::CLogServer::setLocalLogFileSubFileLineCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_setLocalLogFileSubFileLineCountMax != NULL )
    {
        s_pFctLogServer_setLocalLogFileSubFileLineCountMax(i_iCountMax);
    }
}

//------------------------------------------------------------------------------
int DllIf::CLogServer::getLocalLogFileSubFileLineCountMax() const
//------------------------------------------------------------------------------
{
    int iCountMax = 0;

    if( s_hndDllIf != NULL && s_pFctLogServer_getLocalLogFileSubFileLineCountMax != NULL )
    {
        iCountMax = s_pFctLogServer_getLocalLogFileSubFileLineCountMax();
    }
    return iCountMax;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CLogServer::setUseIpcServer( bool i_bUse )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_setUseIpcServer != NULL )
    {
        s_pFctLogServer_setUseIpcServer(i_bUse);
    }
}

//------------------------------------------------------------------------------
bool DllIf::CLogServer::isIpcServerUsed() const
//------------------------------------------------------------------------------
{
    bool bUsed = false;

    if( s_hndDllIf != NULL && s_pFctLogServer_isIpcServerUsed != NULL )
    {
        bUsed = s_pFctLogServer_isIpcServerUsed();
    }
    return bUsed;
}

//------------------------------------------------------------------------------
/*! Defines whether caching should be activated.

    If no client is connected, the outputs are not sent to the client.
    If caching is enabled the outputs are locally stored in a cache.
    If the client connects the cached outputs will be send to the client.
    In this way, for example, the startup of the application can also be made visible
    to the client if the clients connects later on.

    @param i_bCacheData [In] true to activate caching.
*/
void DllIf::CLogServer::setCacheLogDataIfNotConnected( bool i_bCacheData )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_setCacheLogDataIfNotConnected != NULL )
    {
        s_pFctLogServer_setCacheLogDataIfNotConnected(i_bCacheData);
    }
}

//------------------------------------------------------------------------------
bool DllIf::CLogServer::getCacheLogDataIfNotConnected() const
//------------------------------------------------------------------------------
{
    bool bCacheData = false;

    if( s_hndDllIf != NULL && s_pFctLogServer_getCacheLogDataIfNotConnected != NULL )
    {
        bCacheData = s_pFctLogServer_getCacheLogDataIfNotConnected();
    }
    return bCacheData;
}

//------------------------------------------------------------------------------
/*! Ist das Caching aktiviert wird hier die Anzahl der Zeilen definiert, die im
    Cache abgelegt werden.

    @param i_iMaxArrLen [In] Anzahl der Methoden Aufrufe, die im Cache gespeichert
           werden, bis sich ein Client verbindet.
*/
void DllIf::CLogServer::setCacheLogDataMaxArrLen( int i_iMaxArrLen )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_setCacheLogDataMaxArrLen != NULL )
    {
        s_pFctLogServer_setCacheLogDataMaxArrLen(i_iMaxArrLen);
    }
}

//------------------------------------------------------------------------------
int DllIf::CLogServer::getCacheLogDataMaxArrLen() const
//------------------------------------------------------------------------------
{
    int iMaxArrLen = 0;

    if( s_hndDllIf != NULL && s_pFctLogServer_getCacheLogDataMaxArrLen != NULL )
    {
        iMaxArrLen = s_pFctLogServer_getCacheLogDataMaxArrLen();
    }
    return iMaxArrLen;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool DllIf::CLogServer::setLogSettings( const DllIf::SLogServerSettings& i_settings )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndDllIf != NULL && s_pFctLogServer_setLogSettings != NULL )
    {
        bOk = s_pFctLogServer_setLogSettings(i_settings);
    }
    return bOk;
}

//------------------------------------------------------------------------------
DllIf::SLogServerSettings DllIf::CLogServer::getLogSettings() const
//------------------------------------------------------------------------------
{
    DllIf::SLogServerSettings settings;

    SLogServerSettings_init(settings);

    if( s_hndDllIf != NULL && s_pFctLogServer_getLogSettings != NULL )
    {
        settings = s_pFctLogServer_getLogSettings();
    }
    return settings;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CLogServer::clearLocalLogFile()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctLogServer_clearLocalLogFile != NULL )
    {
        s_pFctLogServer_clearLocalLogFile();
    }
}


/*******************************************************************************
class CIpcLogServer
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns a reference to the log server.

    This method is used to access an already existing log server.
    Neither a log server may be created by this method call nor a reference
    counter of the log server is affected.

    @return Reference to log server or nullptr, if a log server is not existing.

   @note This method does not increment the reference counter of the log server.
*/
DllIf::CIpcLogServer* DllIf::CIpcLogServer::GetInstance()
//------------------------------------------------------------------------------
{
    DllIf::CIpcLogServer* pLogServer = NULL;

    if( s_hndDllIf != NULL && s_pFctIpcLogServer_GetInstance != NULL )
    {
        pLogServer = s_pFctIpcLogServer_GetInstance();
    }
    return pLogServer;
}

//------------------------------------------------------------------------------
/*! Creates a log server if a log server is not already existing.

    If a log server is already existing the reference to the existing log server
    is returned and a reference counter is incremented.

    @return Reference to log server or nullptr.
*/
DllIf::CIpcLogServer* DllIf::CIpcLogServer::CreateInstance()
//------------------------------------------------------------------------------
{
    DllIf::CIpcLogServer* pLogServer = NULL;

    if( s_hndDllIf != NULL && s_pFctIpcLogServer_GetInstance != NULL )
    {
        pLogServer = s_pFctIpcLogServer_CreateInstance();
    }
    return pLogServer;
}

//------------------------------------------------------------------------------
/*! Releases the log server by decrementing the reference counter. If the
    reference counter reaches 0 the log server will be destroyed.

    @param i_pLogServer [in] Reference to the log server as returned by
           the GetInstance call.
*/
void DllIf::CIpcLogServer::ReleaseInstance( DllIf::CIpcLogServer* i_pLogServer )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctIpcLogServer_ReleaseInstance != NULL )
    {
        s_pFctIpcLogServer_ReleaseInstance(i_pLogServer);
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Starts the log server.

    The Ipc server of the log server is instantiated in an Gateway Thread
    and waits there to accept incoming connection requests.
    Starting the log server is an asynchronous request.
    By setting a corresponding timeout and the wait flag the method only returns
    after the Gateway Thread has been started and the Ipc Server is listening
    for incoming connection requests.

    @param i_iTimeout_ms [in] Timeout in ms the method waits for the Ipc server
           to enter the listen mode in the Gateway Thread.
           If the timeout is set to 0 ms and the wait flag is set the method
           waits for an infinite time.
           Default: 5000 ms

    @param i_bWait [in] If this flag is set to true the method only returns
           if the Gateway server has been started and the Ipc server has entered
           the listen mode to accept incoming connection requests.
           If this flag is false the method immediately returns.

    @return true if the log server has been started.
*/
bool DllIf::CIpcLogServer::startup( int i_iTimeout_ms, bool /*i_bWait*/ )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndDllIf != NULL && s_pFctIpcLogServer_startup != NULL )
    {
        bOk = s_pFctIpcLogServer_startup(this, i_iTimeout_ms, true);
    }
    return bOk;
}

//------------------------------------------------------------------------------
/*! Stops the log server.

    The Ipc server of the log server is instantiated in an Gateway Thread
    and waits there to accept incoming connection requests.
    Stopping the log server is an asynchronous request.
    By setting a corresponding timeout and the wait flag the method only returns
    after the Gateway Thread has been stopped and the Ipc Server has been destroyed.

    @param i_iTimeout_ms [in] Timeout in ms the method waits for the Gateway
           Thread to be stopped.
           If the timeout is set to 0 ms and the wait flag is set the method
           waits for an infinite time.
           Default: 5000 ms

    @param i_bWait [in] If this flag is set to true the method only returns
           if the Gateway thread has been stopped and the Ipc server has been
           destroyed. If this flag is false the method immediately returns.

    @return true if the log server has been shutdown.
*/
bool DllIf::CIpcLogServer::shutdown( int i_iTimeout_ms, bool /*i_bWait*/ )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndDllIf != NULL && s_pFctIpcLogServer_shutdown != NULL )
    {
        bOk = s_pFctIpcLogServer_shutdown(this, i_iTimeout_ms, true);
    }
    return bOk;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool DllIf::CIpcLogServer::isListening() const
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndDllIf != NULL && s_pFctIpcLogServer_isListening != NULL )
    {
        bOk = s_pFctIpcLogServer_isListening(this);
    }
    return bOk;
}

//------------------------------------------------------------------------------
bool DllIf::CIpcLogServer::isConnected() const
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndDllIf != NULL && s_pFctIpcLogServer_isConnected != NULL )
    {
        bOk = s_pFctIpcLogServer_isConnected(this);
    }
    return bOk;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool DllIf::CIpcLogServer::setPort( unsigned short i_uPort, int i_iTimeout_ms, bool i_bWait )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndDllIf != NULL && s_pFctIpcLogServer_setPort != NULL )
    {
        bOk = s_pFctIpcLogServer_setPort(this, i_uPort, i_iTimeout_ms, i_bWait);
    }
    return bOk;
}

//------------------------------------------------------------------------------
unsigned short DllIf::CIpcLogServer::getPort() const
//------------------------------------------------------------------------------
{
    unsigned short uPort = 0;

    if( s_hndDllIf != NULL && s_pFctIpcLogServer_getPort != NULL )
    {
        uPort = s_pFctIpcLogServer_getPort(this);
    }
    return uPort;
}

#endif // #ifdef USE_ZS_IPCLOG_DLL_IF
