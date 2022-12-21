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

#ifdef USE_ZS_IPCTRACE_DLL_IF

#include "ZSIpcTrcDllIf.h"

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
using namespace ZS::Trace;


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
namespace ZS::Trace::Dll
*******************************************************************************/

/*==============================================================================
type definitions and constants
==============================================================================*/

typedef char* (*TFctTrcAdminObj_getNameSpace)( const DllIf::CTrcAdminObj* i_pTrcAdminObj );
typedef char* (*TFctTrcAdminObj_getClassName)( const DllIf::CTrcAdminObj* i_pTrcAdminObj );
typedef char* (*TFctTrcAdminObj_getObjectName)( const DllIf::CTrcAdminObj* i_pTrcAdminObj );
typedef void (*TFctTrcAdminObj_setObjectThreadName)( DllIf::CTrcAdminObj* i_pTrcAdminObj, const char* i_szThreadName );
typedef char* (*TFctTrcAdminObj_getObjectThreadName)( const DllIf::CTrcAdminObj* i_pTrcAdminObj );
typedef int (*TFctTrcAdminObj_lock)( DllIf::CTrcAdminObj* i_pTrcAdminObj );
typedef int (*TFctTrcAdminObj_unlock)( DllIf::CTrcAdminObj* i_pTrcAdminObj );
typedef bool (*TFctTrcAdminObj_isLocked)( const DllIf::CTrcAdminObj* i_pTrcAdminObj );
typedef int (*TFctTrcAdminObj_getLockCount)( const DllIf::CTrcAdminObj* i_pTrcAdminObj );
typedef void (*TFctTrcAdminObj_setDeleteOnUnlock)( DllIf::CTrcAdminObj* i_pTrcAdminObj, bool i_bDelete );
typedef bool (*TFctTrcAdminObj_deleteOnUnlock)( const DllIf::CTrcAdminObj* i_pTrcAdminObj );
typedef void (*TFctTrcAdminObj_setEnabled)( DllIf::CTrcAdminObj* i_pTrcAdminObj, bool i_bEnabled );
typedef bool (*TFctTrcAdminObj_isEnabled)( const DllIf::CTrcAdminObj* i_pTrcAdminObj );
typedef void (*TFctTrcAdminObj_setMethodCallsTraceDetailLevel)( DllIf::CTrcAdminObj* i_pTrcAdminObj, DllIf::EMethodTraceDetailLevel i_eDetailLevel );
typedef DllIf::EMethodTraceDetailLevel (*TFctTrcAdminObj_getMethodCallsTraceDetailLevel)( const DllIf::CTrcAdminObj* i_pTrcAdminObj );
typedef bool (*TFctTrcAdminObj_areMethodCallsActive)( const DllIf::CTrcAdminObj* i_pTrcAdminObj, DllIf::EMethodTraceDetailLevel i_eDetailLevel );
typedef void (*TFctTrcAdminObj_setRuntimeInfoTraceDetailLevel)( DllIf::CTrcAdminObj* i_pTrcAdminObj, DllIf::ELogDetailLevel i_eDetailLevel );
typedef DllIf::ELogDetailLevel (*TFctTrcAdminObj_getRuntimeInfoTraceDetailLevel)( const DllIf::CTrcAdminObj* i_pTrcAdminObj );
typedef bool (*TFctTrcAdminObj_isRuntimeInfoActive)( const DllIf::CTrcAdminObj* i_pTrcAdminObj, DllIf::ELogDetailLevel i_eDetailLevel );
typedef void (*TFctTrcAdminObj_setTraceDataFilter)( DllIf::CTrcAdminObj* i_pTrcAdminObj, const char* i_szFilter );
typedef char* (*TFctTrcAdminObj_getTraceDataFilter)( const DllIf::CTrcAdminObj* i_pTrcAdminObj );
typedef bool (*TFctTrcAdminObj_isTraceDataSuppressedByFilter)( const DllIf::CTrcAdminObj* i_pTrcAdminObj, const char* i_szTraceData );
typedef void (*TFctTrcAdminObj_traceMethodEnter)( const DllIf::CTrcAdminObj* i_pTrcAdminObj, const char* i_szObjName, const char* i_szMethod, const char* i_szMethodInArgs );
typedef void (*TFctTrcAdminObj_traceMethodLeave)( const DllIf::CTrcAdminObj* i_pTrcAdminObj, const char* i_szObjName, const char* i_szMethod, const char* i_szMethodReturn, const char* i_szMethodOutArgs );
typedef void (*TFctTrcAdminObj_traceMethod)( const DllIf::CTrcAdminObj* i_pTrcAdminObj, const char* i_szObjName, const char* i_szMethod, const char* i_szMethodAddInfo );

typedef DllIf::CTrcAdminObj* (*TFctTrcServer_GetTraceAdminObj)(
    const char* i_szNameSpace, const char* i_szClassName, const char* i_szObjName, DllIf::EEnabled i_bEnabledAsDefault,
    DllIf::EMethodTraceDetailLevel i_eMethodCallsDefaultDetailLevel, DllIf::ELogDetailLevel i_eRuntimeInfoDefaultDetailLevel );
typedef void (*TFctTrcServer_RenameTraceAdminObj)( DllIf::CTrcAdminObj** io_ppTrcAdminObj, const char* i_szNewObjName );
typedef void (*TFctTrcServer_ReleaseTraceAdminObj)( DllIf::CTrcAdminObj* i_pTrcAdminObj );
typedef void (*TFctTrcServer_SetOrganizationName)( const char* i_szName );
typedef char* (*TFctTrcServer_GetOrganizationName)();
typedef void (*TFctTrcServer_SetApplicationName)( const char* i_szName );
typedef char* (*TFctTrcServer_GetApplicationName)();
typedef void (*TFctTrcServer_SetAdminObjFileAbsoluteFilePath)( const char* i_szAbsFilePath );
typedef char* (*TFctTrcServer_GetAdminObjFileAbsoluteFilePath)();
typedef char* (*TFctTrcServer_GetAdminObjFileCompleteBaseName)();
typedef char* (*TFctTrcServer_GetAdminObjFileAbsolutePath)();
typedef void (*TFctTrcServer_SetLocalTrcFileAbsoluteFilePath)( const char* i_szAbsFilePath );
typedef char* (*TFctTrcServer_GetLocalTrcFileAbsoluteFilePath)();
typedef char* (*TFctTrcServer_GetLocalTrcFileCompleteBaseName)();
typedef char* (*TFctTrcServer_GetLocalTrcFileAbsolutePath)();
typedef void (*TFctTrcServer_RegisterThread)( const char* i_szThreadName, void* i_pvThreadHandle );
typedef void (*TFctTrcServer_UnregisterThread)( void* i_pvThreadHandle );
typedef char* (*TFctTrcServer_GetThreadName)( void* i_pvThreadHandle );
typedef char* (*TFctTrcServer_GetCurrentThreadName)();
typedef bool (*TFctTrcServer_isActive)();
typedef void (*TFctTrcServer_setEnabled)( bool i_bEnabled );
typedef bool (*TFctTrcServer_isEnabled)();
typedef void (*TFctTrcServer_setNewTrcAdminObjsEnabledAsDefault)( bool i_bEnabled );
typedef bool (*TFctTrcServer_areNewTrcAdminObjsEnabledAsDefault)();
typedef void (*TFctTrcServer_setNewTrcAdminObjsMethodCallsDefaultDetailLevel)( DllIf::EMethodTraceDetailLevel i_eDetailLevel );
typedef DllIf::EMethodTraceDetailLevel (*TFctTrcServer_getNewTrcAdminObjsMethodCallsDefaultDetailLevel)();
typedef void (*TFctTrcServer_setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel)( DllIf::ELogDetailLevel i_eDetailLevel );
typedef DllIf::ELogDetailLevel (*TFctTrcServer_getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel)();
typedef bool (*TFctTrcServer_recallAdminObjs)();
typedef bool (*TFctTrcServer_saveAdminObjs)();
typedef void (*TFctTrcServer_setUseLocalTrcFile)( bool i_bUse );
typedef bool (*TFctTrcServer_isLocalTrcFileUsed)();
typedef bool (*TFctTrcServer_isLocalTrcFileActive)();
typedef void (*TFctTrcServer_setLocalTrcFileAutoSaveIntervalInMs)( int i_iAutoSaveInterval_ms );
typedef int (*TFctTrcServer_getLocalTrcFileAutoSaveIntervalInMs)();
typedef void (*TFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite)( bool i_bCloseFile );
typedef bool (*TFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite)();
typedef void (*TFctTrcServer_setLocalTrcFileSubFileCountMax)( int i_iCountMax );
typedef int (*TFctTrcServer_getLocalTrcFileSubFileCountMax)();
typedef void (*TFctTrcServer_setLocalTrcFileSubFileLineCountMax)( int i_iCountMax );
typedef int (*TFctTrcServer_getLocalTrcFileSubFileLineCountMax)();
typedef void (*TFctTrcServer_setUseIpcServer)( bool i_bUseIpcServer );
typedef bool (*TFctTrcServer_isIpcServerUsed)();
typedef void (*TFctTrcServer_setCacheTrcDataIfNotConnected)( bool i_bCacheData );
typedef bool (*TFctTrcServer_getCacheTrcDataIfNotConnected)();
typedef void (*TFctTrcServer_setCacheTrcDataMaxArrLen)( int i_iMaxArrLen );
typedef int (*TFctTrcServer_getCacheTrcDataMaxArrLen)();
typedef bool (*TFctTrcServer_setTraceSettings)( const DllIf::STrcServerSettings& i_settings );
typedef DllIf::STrcServerSettings (*TFctTrcServer_getTraceSettings)();
typedef void (*TFctTrcServer_clearLocalTrcFile)();

typedef DllIf::CIpcTrcServer* (*TFctIpcTrcServer_GetInstance)();
typedef DllIf::CIpcTrcServer* (*TFctIpcTrcServer_CreateInstance)(
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelDllIf,
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServer,
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerMutex,
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerIpcServer,
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerIpcServerMutex,
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerIpcServerGateway );
typedef void (*TFctIpcTrcServer_ReleaseInstance)( DllIf::CIpcTrcServer* i_pTrcServer );
typedef bool (*TFctIpcTrcServer_startup)( DllIf::CIpcTrcServer* i_pTrcServer, int i_iTimeout_ms, bool i_bWait );
typedef bool (*TFctIpcTrcServer_shutdown)( DllIf::CIpcTrcServer* i_pTrcServer, int i_iTimeout_ms, bool i_bWait );
typedef bool (*TFctIpcTrcServer_isListening)( const DllIf::CIpcTrcServer* i_pTrcServer );
typedef bool (*TFctIpcTrcServer_isConnected)( const DllIf::CIpcTrcServer* i_pTrcServer );
typedef bool (*TFctIpcTrcServer_setPort)( DllIf::CIpcTrcServer* i_pTrcServer, unsigned short i_uPort, int i_iTimeout_ms, bool i_bWait );
typedef unsigned short (*TFctIpcTrcServer_getPort)( const DllIf::CIpcTrcServer* i_pTrcServer );

/*==============================================================================
static instances
==============================================================================*/

static char* s_szDllFileName = nullptr;

#ifdef _WIN32
static HMODULE s_hndDllIf = NULL;
#else
static void* s_hndDllIf = NULL;
#endif

TFctTrcAdminObj_getNameSpace                                  s_pFctTrcAdminObj_getNameSpace                                  = NULL;
TFctTrcAdminObj_getClassName                                  s_pFctTrcAdminObj_getClassName                                  = NULL;
TFctTrcAdminObj_getObjectName                                 s_pFctTrcAdminObj_getObjectName                                 = NULL;
TFctTrcAdminObj_setObjectThreadName                           s_pFctTrcAdminObj_setObjectThreadName                           = NULL;
TFctTrcAdminObj_getObjectThreadName                           s_pFctTrcAdminObj_getObjectThreadName                           = NULL;
TFctTrcAdminObj_lock                                          s_pFctTrcAdminObj_lock                                          = NULL;
TFctTrcAdminObj_unlock                                        s_pFctTrcAdminObj_unlock                                        = NULL;
TFctTrcAdminObj_isLocked                                      s_pFctTrcAdminObj_isLocked                                      = NULL;
TFctTrcAdminObj_getLockCount                                  s_pFctTrcAdminObj_getLockCount                                  = NULL;
TFctTrcAdminObj_setDeleteOnUnlock                             s_pFctTrcAdminObj_setDeleteOnUnlock                             = NULL;
TFctTrcAdminObj_deleteOnUnlock                                s_pFctTrcAdminObj_deleteOnUnlock                                = NULL;
TFctTrcAdminObj_setEnabled                                    s_pFctTrcAdminObj_setEnabled                                    = NULL;
TFctTrcAdminObj_isEnabled                                     s_pFctTrcAdminObj_isEnabled                                     = NULL;
TFctTrcAdminObj_setMethodCallsTraceDetailLevel                s_pFctTrcAdminObj_setMethodCallsTraceDetailLevel                = NULL;
TFctTrcAdminObj_getMethodCallsTraceDetailLevel                s_pFctTrcAdminObj_getMethodCallsTraceDetailLevel                = NULL;
TFctTrcAdminObj_areMethodCallsActive                          s_pFctTrcAdminObj_areMethodCallsActive                          = NULL;
TFctTrcAdminObj_setRuntimeInfoTraceDetailLevel                s_pFctTrcAdminObj_setRuntimeInfoTraceDetailLevel                = NULL;
TFctTrcAdminObj_getRuntimeInfoTraceDetailLevel                s_pFctTrcAdminObj_getRuntimeInfoTraceDetailLevel                = NULL;
TFctTrcAdminObj_isRuntimeInfoActive                           s_pFctTrcAdminObj_isRuntimeInfoActive                           = NULL;
TFctTrcAdminObj_setTraceDataFilter                            s_pFctTrcAdminObj_setTraceDataFilter                            = NULL;
TFctTrcAdminObj_getTraceDataFilter                            s_pFctTrcAdminObj_getTraceDataFilter                            = NULL;
TFctTrcAdminObj_isTraceDataSuppressedByFilter                 s_pFctTrcAdminObj_isTraceDataSuppressedByFilter                 = NULL;
TFctTrcAdminObj_traceMethodEnter                              s_pFctTrcAdminObj_traceMethodEnter                              = NULL;
TFctTrcAdminObj_traceMethodLeave                              s_pFctTrcAdminObj_traceMethodLeave                              = NULL;
TFctTrcAdminObj_traceMethod                                   s_pFctTrcAdminObj_traceMethod                                   = NULL;

TFctTrcServer_GetTraceAdminObj                                s_pFctTrcServer_GetTraceAdminObj                                = NULL;
TFctTrcServer_RenameTraceAdminObj                             s_pFctTrcServer_RenameTraceAdminObj                             = NULL;
TFctTrcServer_ReleaseTraceAdminObj                            s_pFctTrcServer_ReleaseTraceAdminObj                            = NULL;
TFctTrcServer_SetOrganizationName                             s_pFctTrcServer_SetOrganizationName                             = NULL;
TFctTrcServer_GetOrganizationName                             s_pFctTrcServer_GetOrganizationName                             = NULL;
TFctTrcServer_SetApplicationName                              s_pFctTrcServer_SetApplicationName                              = NULL;
TFctTrcServer_GetApplicationName                              s_pFctTrcServer_GetApplicationName                              = NULL;
TFctTrcServer_SetAdminObjFileAbsoluteFilePath                 s_pFctTrcServer_SetAdminObjFileAbsoluteFilePath                 = NULL;
TFctTrcServer_GetAdminObjFileAbsoluteFilePath                 s_pFctTrcServer_GetAdminObjFileAbsoluteFilePath                 = NULL;
TFctTrcServer_GetAdminObjFileCompleteBaseName                 s_pFctTrcServer_GetAdminObjFileCompleteBaseName                 = NULL;
TFctTrcServer_GetAdminObjFileAbsolutePath                     s_pFctTrcServer_GetAdminObjFileAbsolutePath                     = NULL;
TFctTrcServer_SetLocalTrcFileAbsoluteFilePath                 s_pFctTrcServer_SetLocalTrcFileAbsoluteFilePath                 = NULL;
TFctTrcServer_GetLocalTrcFileAbsoluteFilePath                 s_pFctTrcServer_GetLocalTrcFileAbsoluteFilePath                 = NULL;
TFctTrcServer_GetLocalTrcFileCompleteBaseName                 s_pFctTrcServer_GetLocalTrcFileCompleteBaseName                 = NULL;
TFctTrcServer_GetLocalTrcFileAbsolutePath                     s_pFctTrcServer_GetLocalTrcFileAbsolutePath                     = NULL;
TFctTrcServer_RegisterThread                                  s_pFctTrcServer_RegisterThread                                  = NULL;
TFctTrcServer_UnregisterThread                                s_pFctTrcServer_UnregisterThread                                = NULL;
TFctTrcServer_GetThreadName                                   s_pFctTrcServer_GetThreadName                                   = NULL;
TFctTrcServer_GetCurrentThreadName                            s_pFctTrcServer_GetCurrentThreadName                            = NULL;
TFctTrcServer_isActive                                        s_pFctTrcServer_isActive                                        = NULL;
TFctTrcServer_setEnabled                                      s_pFctTrcServer_setEnabled                                      = NULL;
TFctTrcServer_isEnabled                                       s_pFctTrcServer_isEnabled                                       = NULL;
TFctTrcServer_setNewTrcAdminObjsEnabledAsDefault              s_pFctTrcServer_setNewTrcAdminObjsEnabledAsDefault              = NULL;
TFctTrcServer_areNewTrcAdminObjsEnabledAsDefault              s_pFctTrcServer_areNewTrcAdminObjsEnabledAsDefault              = NULL;
TFctTrcServer_setNewTrcAdminObjsMethodCallsDefaultDetailLevel s_pFctTrcServer_setNewTrcAdminObjsMethodCallsDefaultDetailLevel = NULL;
TFctTrcServer_getNewTrcAdminObjsMethodCallsDefaultDetailLevel s_pFctTrcServer_getNewTrcAdminObjsMethodCallsDefaultDetailLevel = NULL;
TFctTrcServer_setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel s_pFctTrcServer_setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel = NULL;
TFctTrcServer_getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel s_pFctTrcServer_getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel = NULL;
TFctTrcServer_recallAdminObjs                                 s_pFctTrcServer_recallAdminObjs                                 = NULL;
TFctTrcServer_saveAdminObjs                                   s_pFctTrcServer_saveAdminObjs                                   = NULL;
TFctTrcServer_setUseLocalTrcFile                              s_pFctTrcServer_setUseLocalTrcFile                              = NULL;
TFctTrcServer_isLocalTrcFileUsed                              s_pFctTrcServer_isLocalTrcFileUsed                              = NULL;
TFctTrcServer_isLocalTrcFileActive                            s_pFctTrcServer_isLocalTrcFileActive                            = NULL;
TFctTrcServer_setLocalTrcFileAutoSaveIntervalInMs             s_pFctTrcServer_setLocalTrcFileAutoSaveIntervalInMs             = NULL;
TFctTrcServer_getLocalTrcFileAutoSaveIntervalInMs             s_pFctTrcServer_getLocalTrcFileAutoSaveIntervalInMs             = NULL;
TFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite          s_pFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite          = NULL;
TFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite          s_pFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite          = NULL;
TFctTrcServer_setLocalTrcFileSubFileCountMax                  s_pFctTrcServer_setLocalTrcFileSubFileCountMax                  = NULL;
TFctTrcServer_getLocalTrcFileSubFileCountMax                  s_pFctTrcServer_getLocalTrcFileSubFileCountMax                  = NULL;
TFctTrcServer_setLocalTrcFileSubFileLineCountMax              s_pFctTrcServer_setLocalTrcFileSubFileLineCountMax              = NULL;
TFctTrcServer_getLocalTrcFileSubFileLineCountMax              s_pFctTrcServer_getLocalTrcFileSubFileLineCountMax              = NULL;
TFctTrcServer_setUseIpcServer                                 s_pFctTrcServer_setUseIpcServer                                 = NULL;
TFctTrcServer_isIpcServerUsed                                 s_pFctTrcServer_isIpcServerUsed                                 = NULL;
TFctTrcServer_setCacheTrcDataIfNotConnected                   s_pFctTrcServer_setCacheTrcDataIfNotConnected                   = NULL;
TFctTrcServer_getCacheTrcDataIfNotConnected                   s_pFctTrcServer_getCacheTrcDataIfNotConnected                   = NULL;
TFctTrcServer_setCacheTrcDataMaxArrLen                        s_pFctTrcServer_setCacheTrcDataMaxArrLen                        = NULL;
TFctTrcServer_getCacheTrcDataMaxArrLen                        s_pFctTrcServer_getCacheTrcDataMaxArrLen                        = NULL;
TFctTrcServer_setTraceSettings                                s_pFctTrcServer_setTraceSettings                                = NULL;
TFctTrcServer_getTraceSettings                                s_pFctTrcServer_getTraceSettings                                = NULL;
TFctTrcServer_clearLocalTrcFile                               s_pFctTrcServer_clearLocalTrcFile                               = NULL;

TFctIpcTrcServer_CreateInstance                               s_pFctIpcTrcServer_CreateInstance                               = NULL;
TFctIpcTrcServer_GetInstance                                  s_pFctIpcTrcServer_GetInstance                                  = NULL;
TFctIpcTrcServer_ReleaseInstance                              s_pFctIpcTrcServer_ReleaseInstance                              = NULL;
TFctIpcTrcServer_startup                                      s_pFctIpcTrcServer_startup                                      = NULL;
TFctIpcTrcServer_shutdown                                     s_pFctIpcTrcServer_shutdown                                     = NULL;
TFctIpcTrcServer_isListening                                  s_pFctIpcTrcServer_isListening                                  = NULL;
TFctIpcTrcServer_isConnected                                  s_pFctIpcTrcServer_isConnected                                  = NULL;
TFctIpcTrcServer_setPort                                      s_pFctIpcTrcServer_setPort                                      = NULL;
TFctIpcTrcServer_getPort                                      s_pFctIpcTrcServer_getPort                                      = NULL;


/*==============================================================================
Exported methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Loads the Remote Method Tracing Dlls.

    @ingroup _GRP_Namespace_ZS_Trace_DllIf

    @param i_configuration [in] Defines the config type of the build.
           Default: EBuildConfigurationAutoDetect
           The parameter defines the config type lib infix for naming the Qt and
           ZSQtLib Dlls. This may be either an empty string or "d" for debug builds.
           Usually the config type lib infix for the build configuration can be
           automatically detected by evaluationg the compiler directive _DEBUG.
           The macro CONFIGLIBINFIX is set via "ZSIpcTrcDllIf.h" to e.g.
           an empty string for Release and to "d" for Debug builds.

    @param i_iQtVersionMajor [in] Defines the major version of the Qt Dlls.
           Default: 5
           The parameter defines the Qt version major lib infix for naming the Qt and
           ZSQtLib Dlls. This parameter cannot be automatically detected as no Qt header
           files are included when using the Dll inteface files.

    @param i_szQtLibInfix [in] Defines the lib infix used when compiling the Qt Dlls.
           Default: null
           Qt may be compiled spcifying a QT_LIBINFIX. This QT_LIBINFIX is used to
           name the Qt dlls. Also the ZSQtLib Dlls are using this QT_LIBINFIX for
           naming the dlls.

    @Example
        ConfigType: Debug, QT_VERSION_MAJOR: 5, QT_LIBINFIX: "Urgh":
        - Qt5CoreUrghd.dll, ...
        - ZSSysQt5Urghd.dll, ...

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
bool ZS::Trace::DllIf::loadDll( EBuildConfiguration i_configuration, int i_iQtVersionMajor, const char* i_szQtLibInfix )
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
     *  "ZSIpcTraceQt5d"
     *  "libZSIpcTraceQt5d"
     * The GNU compiler (MinGW on Windows) are inserting "lib"
     * at the beginning of the file names (also on Windows machines).
     * We are going to try both versions of file names.
    */

    const char* szZSDllName1 = "ZSIpcTraceQt";
    const char* szZSDllName2 = "libZSIpcTraceQt";

    for( int iFileNameTries = 0; iFileNameTries < 2; ++iFileNameTries )
    {
        const char* szZSDllName = szZSDllName1;
        if( iFileNameTries == 1 )
        {
            szZSDllName = szZSDllName2;
        }

        delete s_szDllFileName;
        s_szDllFileName = nullptr;

        size_t iStrLenDllFileName = strlen(szZSDllName) + strlen(szQtVersionMajor) + strlen(szConfig) + strlen(i_szQtLibInfix) + 4;
        s_szDllFileName = new char[iStrLenDllFileName+1];
        memset(s_szDllFileName, 0x00, iStrLenDllFileName+1);

        size_t iStrPos = 0;
        memcpy(&s_szDllFileName[iStrPos], szZSDllName, strlen(szZSDllName));           // "ZSIpcTraceQt"
        iStrPos += strlen(szZSDllName);
        memcpy(&s_szDllFileName[iStrPos], szQtVersionMajor, strlen(szQtVersionMajor)); // "ZSIpcTraceQt5"
        iStrPos += strlen(szQtVersionMajor);
        if( strlen(i_szQtLibInfix) > 0 )
        {
            memcpy(&s_szDllFileName[iStrPos], i_szQtLibInfix, strlen(i_szQtLibInfix)); // "ZSIpcTraceQt5Urgh"
            iStrPos += strlen(i_szQtLibInfix);
        }
        if( strlen(szConfig) > 0 )
        {
            memcpy(&s_szDllFileName[iStrPos], szConfig, strlen(szConfig));             // "ZSIpcTraceQt5d"
            iStrPos += strlen(szConfig);
        }

        #ifdef _WIN32
        #ifdef UNICODE
        const wstring wstrDllFileName = ZS::System::s2ws(s_szDllFileName);
        s_hndDllIf = LoadLibrary(wstrDllFileName.c_str());
        #else
        s_hndDllIf = LoadLibrary(s_szDllFileName);
        #endif
        #else // !_WIN32
        memcpy(&s_szDllFileName[iStrPos], ".so", 3);                                 // "ZSIpcTraceQt5d.so"
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

        // TrcAdminObj
        //------------

        s_pFctTrcAdminObj_getNameSpace = (TFctTrcAdminObj_getNameSpace)GetProcAddress(s_hndDllIf, "TrcAdminObj_getNameSpace");
        if( s_pFctTrcAdminObj_getNameSpace == NULL ) bOk = false;

        s_pFctTrcAdminObj_getClassName = (TFctTrcAdminObj_getClassName)GetProcAddress(s_hndDllIf, "TrcAdminObj_getClassName");
        if( s_pFctTrcAdminObj_getClassName == NULL ) bOk = false;

        s_pFctTrcAdminObj_getObjectName = (TFctTrcAdminObj_getObjectName)GetProcAddress(s_hndDllIf, "TrcAdminObj_getObjectName");
        if( s_pFctTrcAdminObj_getObjectName == NULL ) bOk = false;

        s_pFctTrcAdminObj_setObjectThreadName = (TFctTrcAdminObj_setObjectThreadName)GetProcAddress(s_hndDllIf, "TrcAdminObj_setObjectThreadName");
        if( s_pFctTrcAdminObj_setObjectThreadName == NULL ) bOk = false;

        s_pFctTrcAdminObj_getObjectThreadName = (TFctTrcAdminObj_getObjectThreadName)GetProcAddress(s_hndDllIf, "TrcAdminObj_getObjectThreadName");
        if( s_pFctTrcAdminObj_getObjectThreadName == NULL ) bOk = false;

        s_pFctTrcAdminObj_lock = (TFctTrcAdminObj_lock)GetProcAddress(s_hndDllIf, "TrcAdminObj_lock");
        if( s_pFctTrcAdminObj_lock == NULL ) bOk = false;

        s_pFctTrcAdminObj_unlock = (TFctTrcAdminObj_unlock)GetProcAddress(s_hndDllIf, "TrcAdminObj_unlock");
        if( s_pFctTrcAdminObj_unlock == NULL ) bOk = false;

        s_pFctTrcAdminObj_isLocked = (TFctTrcAdminObj_isLocked)GetProcAddress(s_hndDllIf, "TrcAdminObj_isLocked");
        if( s_pFctTrcAdminObj_isLocked == NULL ) bOk = false;

        s_pFctTrcAdminObj_getLockCount = (TFctTrcAdminObj_getLockCount)GetProcAddress(s_hndDllIf, "TrcAdminObj_getLockCount");
        if( s_pFctTrcAdminObj_getLockCount == NULL ) bOk = false;

        s_pFctTrcAdminObj_setDeleteOnUnlock = (TFctTrcAdminObj_setDeleteOnUnlock)GetProcAddress(s_hndDllIf, "TrcAdminObj_setDeleteOnUnlock");
        if( s_pFctTrcAdminObj_setDeleteOnUnlock == NULL ) bOk = false;

        s_pFctTrcAdminObj_deleteOnUnlock = (TFctTrcAdminObj_deleteOnUnlock)GetProcAddress(s_hndDllIf, "TrcAdminObj_deleteOnUnlock");
        if( s_pFctTrcAdminObj_deleteOnUnlock == NULL ) bOk = false;

        s_pFctTrcAdminObj_setEnabled = (TFctTrcAdminObj_setEnabled)GetProcAddress(s_hndDllIf, "TrcAdminObj_setEnabled");
        if( s_pFctTrcAdminObj_setEnabled == NULL ) bOk = false;

        s_pFctTrcAdminObj_isEnabled = (TFctTrcAdminObj_isEnabled)GetProcAddress(s_hndDllIf, "TrcAdminObj_isEnabled");
        if( s_pFctTrcAdminObj_isEnabled == NULL ) bOk = false;

        s_pFctTrcAdminObj_setMethodCallsTraceDetailLevel = (TFctTrcAdminObj_setMethodCallsTraceDetailLevel)GetProcAddress(s_hndDllIf, "TrcAdminObj_setMethodCallsTraceDetailLevel");
        if( s_pFctTrcAdminObj_setMethodCallsTraceDetailLevel == NULL ) bOk = false;

        s_pFctTrcAdminObj_getMethodCallsTraceDetailLevel = (TFctTrcAdminObj_getMethodCallsTraceDetailLevel)GetProcAddress(s_hndDllIf, "TrcAdminObj_getMethodCallsTraceDetailLevel");
        if( s_pFctTrcAdminObj_getMethodCallsTraceDetailLevel == NULL ) bOk = false;

        s_pFctTrcAdminObj_areMethodCallsActive = (TFctTrcAdminObj_areMethodCallsActive)GetProcAddress(s_hndDllIf, "TrcAdminObj_areMethodCallsActive");
        if( s_pFctTrcAdminObj_areMethodCallsActive == NULL ) bOk = false;

        s_pFctTrcAdminObj_setRuntimeInfoTraceDetailLevel = (TFctTrcAdminObj_setRuntimeInfoTraceDetailLevel)GetProcAddress(s_hndDllIf, "TrcAdminObj_setRuntimeInfoTraceDetailLevel");
        if( s_pFctTrcAdminObj_setRuntimeInfoTraceDetailLevel == NULL ) bOk = false;

        s_pFctTrcAdminObj_getRuntimeInfoTraceDetailLevel = (TFctTrcAdminObj_getRuntimeInfoTraceDetailLevel)GetProcAddress(s_hndDllIf, "TrcAdminObj_getRuntimeInfoTraceDetailLevel");
        if( s_pFctTrcAdminObj_getRuntimeInfoTraceDetailLevel == NULL ) bOk = false;

        s_pFctTrcAdminObj_isRuntimeInfoActive = (TFctTrcAdminObj_isRuntimeInfoActive)GetProcAddress(s_hndDllIf, "TrcAdminObj_isRuntimeInfoActive");
        if( s_pFctTrcAdminObj_isRuntimeInfoActive == NULL ) bOk = false;

        s_pFctTrcAdminObj_setTraceDataFilter = (TFctTrcAdminObj_setTraceDataFilter)GetProcAddress(s_hndDllIf, "TrcAdminObj_setTraceDataFilter");
        if( s_pFctTrcAdminObj_setTraceDataFilter == NULL ) bOk = false;

        s_pFctTrcAdminObj_getTraceDataFilter = (TFctTrcAdminObj_getTraceDataFilter)GetProcAddress(s_hndDllIf, "TrcAdminObj_getTraceDataFilter");
        if( s_pFctTrcAdminObj_getTraceDataFilter == NULL ) bOk = false;

        s_pFctTrcAdminObj_isTraceDataSuppressedByFilter = (TFctTrcAdminObj_isTraceDataSuppressedByFilter)GetProcAddress(s_hndDllIf, "TrcAdminObj_isTraceDataSuppressedByFilter");
        if( s_pFctTrcAdminObj_isTraceDataSuppressedByFilter == NULL ) bOk = false;

        s_pFctTrcAdminObj_traceMethodEnter = (TFctTrcAdminObj_traceMethodEnter)GetProcAddress(s_hndDllIf, "TrcAdminObj_traceMethodEnter");
        if( s_pFctTrcAdminObj_traceMethodEnter == NULL ) bOk = false;

        s_pFctTrcAdminObj_traceMethodLeave = (TFctTrcAdminObj_traceMethodLeave)GetProcAddress(s_hndDllIf, "TrcAdminObj_traceMethodLeave");
        if( s_pFctTrcAdminObj_traceMethodLeave == NULL ) bOk = false;

        s_pFctTrcAdminObj_traceMethod = (TFctTrcAdminObj_traceMethod)GetProcAddress(s_hndDllIf, "TrcAdminObj_traceMethod");
        if( s_pFctTrcAdminObj_traceMethod == NULL ) bOk = false;

        // TrcServer
        //------------

        s_pFctTrcServer_GetTraceAdminObj = (TFctTrcServer_GetTraceAdminObj)GetProcAddress(s_hndDllIf, "TrcServer_GetTraceAdminObj");
        if( s_pFctTrcServer_GetTraceAdminObj == NULL ) bOk = false;

        s_pFctTrcServer_RenameTraceAdminObj = (TFctTrcServer_RenameTraceAdminObj)GetProcAddress(s_hndDllIf, "TrcServer_RenameTraceAdminObj");
        if( s_pFctTrcServer_RenameTraceAdminObj == NULL ) bOk = false;

        s_pFctTrcServer_ReleaseTraceAdminObj = (TFctTrcServer_ReleaseTraceAdminObj)GetProcAddress(s_hndDllIf, "TrcServer_ReleaseTraceAdminObj");
        if( s_pFctTrcServer_ReleaseTraceAdminObj == NULL ) bOk = false;

        s_pFctTrcServer_SetOrganizationName = (TFctTrcServer_SetOrganizationName)GetProcAddress(s_hndDllIf, "TrcServer_SetOrganizationName");
        if( s_pFctTrcServer_SetOrganizationName == NULL ) bOk = false;

        s_pFctTrcServer_GetOrganizationName = (TFctTrcServer_GetOrganizationName)GetProcAddress(s_hndDllIf, "TrcServer_GetOrganizationName");
        if( s_pFctTrcServer_GetOrganizationName == NULL ) bOk = false;

        s_pFctTrcServer_SetApplicationName = (TFctTrcServer_SetApplicationName)GetProcAddress(s_hndDllIf, "TrcServer_SetApplicationName");
        if( s_pFctTrcServer_SetApplicationName == NULL ) bOk = false;

        s_pFctTrcServer_GetApplicationName = (TFctTrcServer_GetApplicationName)GetProcAddress(s_hndDllIf, "TrcServer_GetApplicationName");
        if( s_pFctTrcServer_GetApplicationName == NULL ) bOk = false;

        s_pFctTrcServer_SetAdminObjFileAbsoluteFilePath = (TFctTrcServer_SetAdminObjFileAbsoluteFilePath)GetProcAddress(s_hndDllIf, "TrcServer_SetAdminObjFileAbsoluteFilePath");
        if( s_pFctTrcServer_SetAdminObjFileAbsoluteFilePath == NULL ) bOk = false;

        s_pFctTrcServer_GetAdminObjFileAbsoluteFilePath = (TFctTrcServer_GetAdminObjFileAbsoluteFilePath)GetProcAddress(s_hndDllIf, "TrcServer_GetAdminObjFileAbsoluteFilePath");
        if( s_pFctTrcServer_GetAdminObjFileAbsoluteFilePath == NULL ) bOk = false;

        s_pFctTrcServer_GetAdminObjFileCompleteBaseName = (TFctTrcServer_GetAdminObjFileCompleteBaseName)GetProcAddress(s_hndDllIf, "TrcServer_GetAdminObjFileCompleteBaseName");
        if( s_pFctTrcServer_GetAdminObjFileCompleteBaseName == NULL ) bOk = false;

        s_pFctTrcServer_GetAdminObjFileAbsolutePath = (TFctTrcServer_GetAdminObjFileAbsolutePath)GetProcAddress(s_hndDllIf, "TrcServer_GetAdminObjFileAbsolutePath");
        if( s_pFctTrcServer_GetAdminObjFileAbsolutePath == NULL ) bOk = false;

        s_pFctTrcServer_SetLocalTrcFileAbsoluteFilePath = (TFctTrcServer_SetLocalTrcFileAbsoluteFilePath)GetProcAddress(s_hndDllIf, "TrcServer_SetLocalTrcFileAbsoluteFilePath");
        if( s_pFctTrcServer_SetLocalTrcFileAbsoluteFilePath == NULL ) bOk = false;

        s_pFctTrcServer_GetLocalTrcFileAbsoluteFilePath = (TFctTrcServer_GetLocalTrcFileAbsoluteFilePath)GetProcAddress(s_hndDllIf, "TrcServer_GetLocalTrcFileAbsoluteFilePath");
        if( s_pFctTrcServer_GetLocalTrcFileAbsoluteFilePath == NULL ) bOk = false;

        s_pFctTrcServer_GetLocalTrcFileCompleteBaseName = (TFctTrcServer_GetLocalTrcFileCompleteBaseName)GetProcAddress(s_hndDllIf, "TrcServer_GetLocalTrcFileCompleteBaseName");
        if( s_pFctTrcServer_GetLocalTrcFileCompleteBaseName == NULL ) bOk = false;

        s_pFctTrcServer_GetLocalTrcFileAbsolutePath = (TFctTrcServer_GetLocalTrcFileAbsolutePath)GetProcAddress(s_hndDllIf, "TrcServer_GetLocalTrcFileAbsolutePath");
        if( s_pFctTrcServer_GetLocalTrcFileAbsolutePath == NULL ) bOk = false;

        s_pFctTrcServer_RegisterThread = (TFctTrcServer_RegisterThread)GetProcAddress(s_hndDllIf, "TrcServer_RegisterThread");
        if( s_pFctTrcServer_RegisterThread == NULL ) bOk = false;

        s_pFctTrcServer_UnregisterThread = (TFctTrcServer_UnregisterThread)GetProcAddress(s_hndDllIf, "TrcServer_UnregisterThread");
        if( s_pFctTrcServer_UnregisterThread == NULL ) bOk = false;

        s_pFctTrcServer_GetThreadName = (TFctTrcServer_GetThreadName)GetProcAddress(s_hndDllIf, "TrcServer_GetThreadName");
        if( s_pFctTrcServer_GetThreadName == NULL ) bOk = false;

        s_pFctTrcServer_GetCurrentThreadName = (TFctTrcServer_GetCurrentThreadName)GetProcAddress(s_hndDllIf, "TrcServer_GetCurrentThreadName");
        if( s_pFctTrcServer_GetCurrentThreadName == NULL ) bOk = false;

        s_pFctTrcServer_isActive = (TFctTrcServer_isActive)GetProcAddress(s_hndDllIf, "TrcServer_isActive");
        if( s_pFctTrcServer_isActive == NULL ) bOk = false;

        s_pFctTrcServer_setEnabled = (TFctTrcServer_setEnabled)GetProcAddress(s_hndDllIf, "TrcServer_setEnabled");
        if( s_pFctTrcServer_setEnabled == NULL ) bOk = false;

        s_pFctTrcServer_isEnabled = (TFctTrcServer_isEnabled)GetProcAddress(s_hndDllIf, "TrcServer_isEnabled");
        if( s_pFctTrcServer_isEnabled == NULL ) bOk = false;

        s_pFctTrcServer_setNewTrcAdminObjsEnabledAsDefault = (TFctTrcServer_setNewTrcAdminObjsEnabledAsDefault)GetProcAddress(s_hndDllIf, "TrcServer_setNewTrcAdminObjsEnabledAsDefault");
        if( s_pFctTrcServer_setNewTrcAdminObjsEnabledAsDefault == NULL ) bOk = false;

        s_pFctTrcServer_areNewTrcAdminObjsEnabledAsDefault = (TFctTrcServer_areNewTrcAdminObjsEnabledAsDefault)GetProcAddress(s_hndDllIf, "TrcServer_areNewTrcAdminObjsEnabledAsDefault");
        if( s_pFctTrcServer_areNewTrcAdminObjsEnabledAsDefault == NULL ) bOk = false;

        s_pFctTrcServer_setNewTrcAdminObjsMethodCallsDefaultDetailLevel = (TFctTrcServer_setNewTrcAdminObjsMethodCallsDefaultDetailLevel)GetProcAddress(s_hndDllIf, "TrcServer_setNewTrcAdminObjsMethodCallsDefaultDetailLevel");
        if( s_pFctTrcServer_setNewTrcAdminObjsMethodCallsDefaultDetailLevel == NULL ) bOk = false;

        s_pFctTrcServer_getNewTrcAdminObjsMethodCallsDefaultDetailLevel = (TFctTrcServer_getNewTrcAdminObjsMethodCallsDefaultDetailLevel)GetProcAddress(s_hndDllIf, "TrcServer_getNewTrcAdminObjsMethodCallsDefaultDetailLevel");
        if( s_pFctTrcServer_getNewTrcAdminObjsMethodCallsDefaultDetailLevel == NULL ) bOk = false;

        s_pFctTrcServer_getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel = (TFctTrcServer_getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel)GetProcAddress(s_hndDllIf, "TrcServer_getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel");
        if( s_pFctTrcServer_getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel == NULL ) bOk = false;

        s_pFctTrcServer_setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel = (TFctTrcServer_setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel)GetProcAddress(s_hndDllIf, "TrcServer_setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel");
        if( s_pFctTrcServer_setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel == NULL ) bOk = false;

        s_pFctTrcServer_recallAdminObjs = (TFctTrcServer_recallAdminObjs)GetProcAddress(s_hndDllIf, "TrcServer_recallAdminObjs");
        if( s_pFctTrcServer_recallAdminObjs == NULL ) bOk = false;

        s_pFctTrcServer_saveAdminObjs = (TFctTrcServer_saveAdminObjs)GetProcAddress(s_hndDllIf, "TrcServer_saveAdminObjs");
        if( s_pFctTrcServer_saveAdminObjs == NULL ) bOk = false;

        s_pFctTrcServer_setUseLocalTrcFile = (TFctTrcServer_setUseLocalTrcFile)GetProcAddress(s_hndDllIf, "TrcServer_setUseLocalTrcFile");
        if( s_pFctTrcServer_setUseLocalTrcFile == NULL ) bOk = false;

        s_pFctTrcServer_isLocalTrcFileUsed = (TFctTrcServer_isLocalTrcFileUsed)GetProcAddress(s_hndDllIf, "TrcServer_isLocalTrcFileUsed");
        if( s_pFctTrcServer_isLocalTrcFileUsed == NULL ) bOk = false;

        s_pFctTrcServer_isLocalTrcFileActive = (TFctTrcServer_isLocalTrcFileActive)GetProcAddress(s_hndDllIf, "TrcServer_isLocalTrcFileActive");
        if( s_pFctTrcServer_isLocalTrcFileActive == NULL ) bOk = false;

        s_pFctTrcServer_setLocalTrcFileAutoSaveIntervalInMs = (TFctTrcServer_setLocalTrcFileAutoSaveIntervalInMs)GetProcAddress(s_hndDllIf, "TrcServer_setLocalTrcFileAutoSaveIntervalInMs");
        if( s_pFctTrcServer_setLocalTrcFileAutoSaveIntervalInMs == NULL ) bOk = false;

        s_pFctTrcServer_getLocalTrcFileAutoSaveIntervalInMs = (TFctTrcServer_getLocalTrcFileAutoSaveIntervalInMs)GetProcAddress(s_hndDllIf, "TrcServer_getLocalTrcFileAutoSaveIntervalInMs");
        if( s_pFctTrcServer_getLocalTrcFileAutoSaveIntervalInMs == NULL ) bOk = false;

        s_pFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite = (TFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite)GetProcAddress(s_hndDllIf, "TrcServer_setLocalTrcFileCloseFileAfterEachWrite");
        if( s_pFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite == NULL ) bOk = false;

        s_pFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite = (TFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite)GetProcAddress(s_hndDllIf, "TrcServer_getLocalTrcFileCloseFileAfterEachWrite");
        if( s_pFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite == NULL ) bOk = false;

        s_pFctTrcServer_setLocalTrcFileSubFileCountMax = (TFctTrcServer_setLocalTrcFileSubFileCountMax)GetProcAddress(s_hndDllIf, "TrcServer_setLocalTrcFileSubFileCountMax");
        if( s_pFctTrcServer_setLocalTrcFileSubFileCountMax == NULL ) bOk = false;

        s_pFctTrcServer_getLocalTrcFileSubFileCountMax = (TFctTrcServer_getLocalTrcFileSubFileCountMax)GetProcAddress(s_hndDllIf, "TrcServer_getLocalTrcFileSubFileCountMax");
        if( s_pFctTrcServer_getLocalTrcFileSubFileCountMax == NULL ) bOk = false;

        s_pFctTrcServer_setLocalTrcFileSubFileLineCountMax = (TFctTrcServer_setLocalTrcFileSubFileLineCountMax)GetProcAddress(s_hndDllIf, "TrcServer_setLocalTrcFileSubFileLineCountMax");
        if( s_pFctTrcServer_setLocalTrcFileSubFileLineCountMax == NULL ) bOk = false;

        s_pFctTrcServer_getLocalTrcFileSubFileLineCountMax = (TFctTrcServer_getLocalTrcFileSubFileLineCountMax)GetProcAddress(s_hndDllIf, "TrcServer_getLocalTrcFileSubFileLineCountMax");
        if( s_pFctTrcServer_getLocalTrcFileSubFileLineCountMax == NULL ) bOk = false;

        s_pFctTrcServer_setUseIpcServer = (TFctTrcServer_setUseIpcServer)GetProcAddress(s_hndDllIf, "TrcServer_setUseIpcServer");
        if( s_pFctTrcServer_setUseIpcServer == NULL ) bOk = false;

        s_pFctTrcServer_isIpcServerUsed = (TFctTrcServer_isIpcServerUsed)GetProcAddress(s_hndDllIf, "TrcServer_isIpcServerUsed");
        if( s_pFctTrcServer_isIpcServerUsed == NULL ) bOk = false;

        s_pFctTrcServer_setCacheTrcDataIfNotConnected = (TFctTrcServer_setCacheTrcDataIfNotConnected)GetProcAddress(s_hndDllIf, "TrcServer_setCacheTrcDataIfNotConnected");
        if( s_pFctTrcServer_setCacheTrcDataIfNotConnected == NULL ) bOk = false;

        s_pFctTrcServer_getCacheTrcDataIfNotConnected = (TFctTrcServer_getCacheTrcDataIfNotConnected)GetProcAddress(s_hndDllIf, "TrcServer_getCacheTrcDataIfNotConnected");
        if( s_pFctTrcServer_getCacheTrcDataIfNotConnected == NULL ) bOk = false;

        s_pFctTrcServer_setCacheTrcDataMaxArrLen = (TFctTrcServer_setCacheTrcDataMaxArrLen)GetProcAddress(s_hndDllIf, "TrcServer_setCacheTrcDataMaxArrLen");
        if( s_pFctTrcServer_setCacheTrcDataMaxArrLen == NULL ) bOk = false;

        s_pFctTrcServer_getCacheTrcDataMaxArrLen = (TFctTrcServer_getCacheTrcDataMaxArrLen)GetProcAddress(s_hndDllIf, "TrcServer_getCacheTrcDataMaxArrLen");
        if( s_pFctTrcServer_getCacheTrcDataMaxArrLen == NULL ) bOk = false;

        s_pFctTrcServer_setTraceSettings = (TFctTrcServer_setTraceSettings)GetProcAddress(s_hndDllIf, "TrcServer_setTraceSettings");
        if( s_pFctTrcServer_setTraceSettings == NULL ) bOk = false;

        s_pFctTrcServer_getTraceSettings = (TFctTrcServer_getTraceSettings)GetProcAddress(s_hndDllIf, "TrcServer_getTraceSettings");
        if( s_pFctTrcServer_getTraceSettings == NULL ) bOk = false;

        s_pFctTrcServer_clearLocalTrcFile = (TFctTrcServer_clearLocalTrcFile)GetProcAddress(s_hndDllIf, "TrcServer_clearLocalTrcFile");
        if( s_pFctTrcServer_clearLocalTrcFile == NULL ) bOk = false;

        // IpcTrcServer
        //-------------

        s_pFctIpcTrcServer_GetInstance = (TFctIpcTrcServer_GetInstance)GetProcAddress(s_hndDllIf, "IpcTrcServer_GetInstance");
        if( s_pFctIpcTrcServer_GetInstance == NULL ) bOk = false;

        s_pFctIpcTrcServer_CreateInstance = (TFctIpcTrcServer_CreateInstance)GetProcAddress(s_hndDllIf, "IpcTrcServer_CreateInstance");
        if( s_pFctIpcTrcServer_CreateInstance == NULL ) bOk = false;

        s_pFctIpcTrcServer_ReleaseInstance = (TFctIpcTrcServer_ReleaseInstance)GetProcAddress(s_hndDllIf, "IpcTrcServer_ReleaseInstance");
        if( s_pFctIpcTrcServer_ReleaseInstance == NULL ) bOk = false;

        s_pFctIpcTrcServer_startup = (TFctIpcTrcServer_startup)GetProcAddress(s_hndDllIf, "IpcTrcServer_startup");
        if( s_pFctIpcTrcServer_startup == NULL ) bOk = false;

        s_pFctIpcTrcServer_shutdown = (TFctIpcTrcServer_shutdown)GetProcAddress(s_hndDllIf, "IpcTrcServer_shutdown");
        if( s_pFctIpcTrcServer_shutdown == NULL ) bOk = false;

        s_pFctIpcTrcServer_isListening = (TFctIpcTrcServer_isListening)GetProcAddress(s_hndDllIf, "IpcTrcServer_isListening");
        if( s_pFctIpcTrcServer_isListening == NULL ) bOk = false;

        s_pFctIpcTrcServer_isConnected = (TFctIpcTrcServer_isConnected)GetProcAddress(s_hndDllIf, "IpcTrcServer_isConnected");
        if( s_pFctIpcTrcServer_isConnected == NULL ) bOk = false;

        s_pFctIpcTrcServer_setPort = (TFctIpcTrcServer_setPort)GetProcAddress(s_hndDllIf, "IpcTrcServer_setPort");
        if( s_pFctIpcTrcServer_setPort == NULL ) bOk = false;

        s_pFctIpcTrcServer_getPort = (TFctIpcTrcServer_getPort)GetProcAddress(s_hndDllIf, "IpcTrcServer_getPort");
        if( s_pFctIpcTrcServer_getPort == NULL ) bOk = false;

    } // if( s_hndDllIf != NULL )

    return bOk;

} // loadDll

//------------------------------------------------------------------------------
/*! Returns the file name of the loaded ZSIpcTrace Dll.

    @ingroup _GRP_Namespace_ZS_Trace_DllIf

    @return name of the dll.
        ! Don't free this pointer as this is a const char pointer created during compile time !
*/
const char* ZS::Trace::DllIf::getDllFileName()
//------------------------------------------------------------------------------
{
    return s_szDllFileName;
}

//------------------------------------------------------------------------------
/*! Releases the Remote Method Trace Dlls.

    @ingroup _GRP_Namespace_ZS_Trace_DllIf

    @return true if dll was release.
            false in case of an error (if the dll was not loaded).
*/
bool ZS::Trace::DllIf::releaseDll()
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

        s_pFctTrcAdminObj_getNameSpace                   = NULL;
        s_pFctTrcAdminObj_getClassName                   = NULL;
        s_pFctTrcAdminObj_getObjectName                  = NULL;
        s_pFctTrcAdminObj_setObjectThreadName            = NULL;
        s_pFctTrcAdminObj_getObjectThreadName            = NULL;
        s_pFctTrcAdminObj_lock                           = NULL;
        s_pFctTrcAdminObj_unlock                         = NULL;
        s_pFctTrcAdminObj_isLocked                       = NULL;
        s_pFctTrcAdminObj_getLockCount                   = NULL;
        s_pFctTrcAdminObj_setDeleteOnUnlock              = NULL;
        s_pFctTrcAdminObj_deleteOnUnlock                 = NULL;
        s_pFctTrcAdminObj_setEnabled                     = NULL;
        s_pFctTrcAdminObj_isEnabled                      = NULL;
        s_pFctTrcAdminObj_setMethodCallsTraceDetailLevel = NULL;
        s_pFctTrcAdminObj_getMethodCallsTraceDetailLevel = NULL;
        s_pFctTrcAdminObj_areMethodCallsActive           = NULL;
        s_pFctTrcAdminObj_setRuntimeInfoTraceDetailLevel = NULL;
        s_pFctTrcAdminObj_getRuntimeInfoTraceDetailLevel = NULL;
        s_pFctTrcAdminObj_isRuntimeInfoActive            = NULL;
        s_pFctTrcAdminObj_setTraceDataFilter             = NULL;
        s_pFctTrcAdminObj_getTraceDataFilter             = NULL;
        s_pFctTrcAdminObj_isTraceDataSuppressedByFilter  = NULL;
        s_pFctTrcAdminObj_traceMethodEnter               = NULL;
        s_pFctTrcAdminObj_traceMethodLeave               = NULL;
        s_pFctTrcAdminObj_traceMethod                    = NULL;

        s_pFctTrcServer_GetTraceAdminObj                                = NULL;
        s_pFctTrcServer_RenameTraceAdminObj                             = NULL;
        s_pFctTrcServer_ReleaseTraceAdminObj                            = NULL;
        s_pFctTrcServer_SetOrganizationName                             = NULL;
        s_pFctTrcServer_GetOrganizationName                             = NULL;
        s_pFctTrcServer_SetApplicationName                              = NULL;
        s_pFctTrcServer_GetApplicationName                              = NULL;
        s_pFctTrcServer_SetAdminObjFileAbsoluteFilePath                 = NULL;
        s_pFctTrcServer_GetAdminObjFileAbsoluteFilePath                 = NULL;
        s_pFctTrcServer_GetAdminObjFileCompleteBaseName                 = NULL;
        s_pFctTrcServer_GetAdminObjFileAbsolutePath                     = NULL;
        s_pFctTrcServer_SetLocalTrcFileAbsoluteFilePath                 = NULL;
        s_pFctTrcServer_GetLocalTrcFileAbsoluteFilePath                 = NULL;
        s_pFctTrcServer_GetLocalTrcFileCompleteBaseName                 = NULL;
        s_pFctTrcServer_GetLocalTrcFileAbsolutePath                     = NULL;
        s_pFctTrcServer_RegisterThread                                  = NULL;
        s_pFctTrcServer_UnregisterThread                                = NULL;
        s_pFctTrcServer_GetThreadName                                   = NULL;
        s_pFctTrcServer_GetCurrentThreadName                            = NULL;
        s_pFctTrcServer_isActive                                        = NULL;
        s_pFctTrcServer_setEnabled                                      = NULL;
        s_pFctTrcServer_isEnabled                                       = NULL;
        s_pFctTrcServer_setNewTrcAdminObjsEnabledAsDefault              = NULL;
        s_pFctTrcServer_areNewTrcAdminObjsEnabledAsDefault              = NULL;
        s_pFctTrcServer_setNewTrcAdminObjsMethodCallsDefaultDetailLevel = NULL;
        s_pFctTrcServer_getNewTrcAdminObjsMethodCallsDefaultDetailLevel = NULL;
        s_pFctTrcServer_setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel = NULL;
        s_pFctTrcServer_getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel = NULL;
        s_pFctTrcServer_recallAdminObjs                                 = NULL;
        s_pFctTrcServer_saveAdminObjs                                   = NULL;
        s_pFctTrcServer_setUseLocalTrcFile                              = NULL;
        s_pFctTrcServer_isLocalTrcFileUsed                              = NULL;
        s_pFctTrcServer_isLocalTrcFileActive                            = NULL;
        s_pFctTrcServer_setLocalTrcFileAutoSaveIntervalInMs             = NULL;
        s_pFctTrcServer_getLocalTrcFileAutoSaveIntervalInMs             = NULL;
        s_pFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite          = NULL;
        s_pFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite          = NULL;
        s_pFctTrcServer_setLocalTrcFileSubFileCountMax                  = NULL;
        s_pFctTrcServer_getLocalTrcFileSubFileCountMax                  = NULL;
        s_pFctTrcServer_setLocalTrcFileSubFileLineCountMax              = NULL;
        s_pFctTrcServer_getLocalTrcFileSubFileLineCountMax              = NULL;
        s_pFctTrcServer_setUseIpcServer                                 = NULL;
        s_pFctTrcServer_isIpcServerUsed                                 = NULL;
        s_pFctTrcServer_setCacheTrcDataIfNotConnected                   = NULL;
        s_pFctTrcServer_getCacheTrcDataIfNotConnected                   = NULL;
        s_pFctTrcServer_setCacheTrcDataMaxArrLen                        = NULL;
        s_pFctTrcServer_getCacheTrcDataMaxArrLen                        = NULL;
        s_pFctTrcServer_setTraceSettings                                = NULL;
        s_pFctTrcServer_getTraceSettings                                = NULL;
        s_pFctTrcServer_clearLocalTrcFile                               = NULL;

        s_pFctIpcTrcServer_GetInstance     = NULL;
        s_pFctIpcTrcServer_CreateInstance  = NULL;
        s_pFctIpcTrcServer_ReleaseInstance = NULL;
        s_pFctIpcTrcServer_startup         = NULL;
        s_pFctIpcTrcServer_shutdown        = NULL;
        s_pFctIpcTrcServer_isListening     = NULL;
        s_pFctIpcTrcServer_isConnected     = NULL;
        s_pFctIpcTrcServer_setPort         = NULL;
        s_pFctIpcTrcServer_getPort         = NULL;
    }

    return bOk;

} // releaseDll


/*******************************************************************************
class CTrcAdminObj
*******************************************************************************/

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the name space of the module, class or instance referencing the
           trace admin object.

    See CTrcAdminObj::getNameSpace for more details.

    @return Character string pointing to the name space.
            The ownership is passed to the caller so it must be free by the caller.
*/
char* DllIf::CTrcAdminObj::getNameSpace() const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_getNameSpace != NULL )
    {
        return s_pFctTrcAdminObj_getNameSpace(this);
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! @brief Returns the class name of the module, class or instance referencing the
           trace admin object.

    See CTrcAdminObj::getClassName for more details.

    @return Character string pointing to the class name.
            The ownership is passed to the caller so it must be free by the caller.
*/
char* DllIf::CTrcAdminObj::getClassName() const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_getClassName != NULL )
    {
        return s_pFctTrcAdminObj_getClassName(this);
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! @brief Returns the object name of the instance creating this trace admin object.

    See CTrcAdminObj::getObjectName for more details.

    @return Character string pointing to the object name.
            The ownership is passed to the caller so it must be free by the caller.
*/
char* DllIf::CTrcAdminObj::getObjectName() const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_getObjectName != NULL )
    {
        return s_pFctTrcAdminObj_getObjectName(this);
    }
    return NULL;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the name of the thread in which context the trace admin object is created.

    See CTrcAdminObj::setObjectThreadName for more details.

    @param i_szThreadName [in]
        Name of the thread in which context the trace admin object was created.
        The trace admin object does not take ownership of the character string
        but will make a deep copy of the passed string.
*/
void DllIf::CTrcAdminObj::setObjectThreadName( const char* i_szThreadName )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_setObjectThreadName != NULL )
    {
        s_pFctTrcAdminObj_setObjectThreadName(this, i_szThreadName);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the name of the thread in which context the trace admin object is created.

    See CTrcAdminObj::getObjectThreadName for more details.

    @return Character string pointing to the thread name.
            The ownership is passed to the caller so it must be free by the caller.
*/
char* DllIf::CTrcAdminObj::getObjectThreadName() const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_getObjectThreadName != NULL )
    {
        return s_pFctTrcAdminObj_getObjectThreadName(this);
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! @brief Locks the trace admin object by incrementing the lock count.

    See CTrcAdminObj::lock for more details.

    @return Current lock count.
*/
int DllIf::CTrcAdminObj::lock()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_lock != NULL )
    {
        return s_pFctTrcAdminObj_lock(this);
    }
    return 0;
}

//------------------------------------------------------------------------------
/*! @brief Unlocks the object by decrementing the lock counter.

    See CTrcAdminObj::unlock for more details.

    @return Current lock count.
*/
int DllIf::CTrcAdminObj::unlock()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_unlock != NULL )
    {
        return s_pFctTrcAdminObj_unlock(this);
    }
    return 0;
}

//------------------------------------------------------------------------------
/*! Returns whether the admin object is locked.

    See CTrcAdminObj::isLocked for more details.

    @return true if the object is locked, false otherwise.
*/
bool DllIf::CTrcAdminObj::isLocked() const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_isLocked != NULL )
    {
        return s_pFctTrcAdminObj_isLocked(this);
    }
    return false;
}

//------------------------------------------------------------------------------
/*! Returns the current lock count.

    @return Current lock count.
*/
int DllIf::CTrcAdminObj::getLockCount() const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_getLockCount != NULL )
    {
        return s_pFctTrcAdminObj_getLockCount(this);
    }
    return 0;
}

//------------------------------------------------------------------------------
/*! @brief Sets the flag that the admin object is no longer need if unlocked.

    See CTrcAdminObj::setDeleteOnUnlock for more details.

    @param i_bDelete [in]
        Flag to indicate whether the object can be deleted if unlocked
        (and the reference counter is still 0).
*/
void DllIf::CTrcAdminObj::setDeleteOnUnlock( bool i_bDelete )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_setDeleteOnUnlock != NULL )
    {
        return s_pFctTrcAdminObj_setDeleteOnUnlock(this, i_bDelete);
    }
    return ;
}

//------------------------------------------------------------------------------
/*! @brief Returns the flag whether the admin object may be deleted if
           unlocked and no longer used.

    See CTrcAdminObj::deleteOnUnlock for more details.

    @return true if the flag is set, false otherwise.
*/
bool DllIf::CTrcAdminObj::deleteOnUnlock() const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_deleteOnUnlock != NULL )
    {
        return s_pFctTrcAdminObj_deleteOnUnlock(this);
    }
    return false;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Enables trace output for this object.

    See CTrcAdminObj::setEnabled for more details.

    @param i_bEnabled [in] Flag to enable or disable method trace output.
                          - true ... tracing is enabled
                          - false .. tracing is disabled
*/
void DllIf::CTrcAdminObj::setEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_setEnabled != NULL )
    {
        s_pFctTrcAdminObj_setEnabled(this, i_bEnabled);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns whether tracing is enabled or disabled.

    @return Flag indicating whether method trace output is enabled or disabled.
*/
bool DllIf::CTrcAdminObj::isEnabled() const
//------------------------------------------------------------------------------
{
    bool bEnabled = false;

    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_isEnabled != NULL )
    {
        bEnabled = s_pFctTrcAdminObj_isEnabled(this);
    }
    return bEnabled;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the detail level of trace output for this object.

    See CTrcAdminObj::setMethodCallsTraceDetailLevel for more details.

    @param i_eDetailLevel [in] Detail level.
*/
void DllIf::CTrcAdminObj::setMethodCallsTraceDetailLevel( EMethodTraceDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_setMethodCallsTraceDetailLevel != NULL )
    {
        s_pFctTrcAdminObj_setMethodCallsTraceDetailLevel(this, i_eDetailLevel);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the detail level of trace output for this object.

    @return Detail level.
*/
DllIf::EMethodTraceDetailLevel DllIf::CTrcAdminObj::getMethodCallsTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    EMethodTraceDetailLevel eDetailLevel = EMethodTraceDetailLevelNone;

    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_getMethodCallsTraceDetailLevel != NULL )
    {
        eDetailLevel = s_pFctTrcAdminObj_getMethodCallsTraceDetailLevel(this);
    }
    return eDetailLevel;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether tracing is activated or disactived.

    See CTrcAdminObj::areMethodCallsActive for more details.

    @param i_eFilterDetailLevel [in]
        Trace outputs should be generated if the given filter detail level
        is greater or equal than the current detail level set at the trace
        admin object or at the method tracer itself.

    @return Flag indicating whether method trace output is active or not.
*/
bool DllIf::CTrcAdminObj::areMethodCallsActive( EMethodTraceDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_areMethodCallsActive != NULL )
    {
        bIsActive = s_pFctTrcAdminObj_areMethodCallsActive(this, i_eFilterDetailLevel);
    }
    return bIsActive;
}

//------------------------------------------------------------------------------
/*! @brief Sets the detail level of trace output for this object.

    See CTrcAdminObj::setRuntimeInfoTraceDetailLevel for more details.

    @param i_eDetailLevel [in] Detail level.
*/
void DllIf::CTrcAdminObj::setRuntimeInfoTraceDetailLevel( ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_setRuntimeInfoTraceDetailLevel != NULL )
    {
        s_pFctTrcAdminObj_setRuntimeInfoTraceDetailLevel(this, i_eDetailLevel);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the detail level of trace output for this object.

    @return Detail level.
*/
 DllIf::ELogDetailLevel DllIf::CTrcAdminObj::getRuntimeInfoTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    ELogDetailLevel eDetailLevel = ELogDetailLevelNone;

    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_getRuntimeInfoTraceDetailLevel != NULL )
    {
        eDetailLevel = s_pFctTrcAdminObj_getRuntimeInfoTraceDetailLevel(this);
    }
    return eDetailLevel;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether tracing is activated or disactived.

    See CTrcAdminObj::isRuntimeInfoActive for more details.

    @param i_eFilterDetailLevel [in]
        Trace outputs should be generated if the given filter detail level
        is greater or equal than the current detail level set at the trace
        admin object or at the method tracer itself.

    @return Flag indicating whether method trace output is active or not.
*/
bool DllIf::CTrcAdminObj::isRuntimeInfoActive( ELogDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_isRuntimeInfoActive != NULL )
    {
        bIsActive = s_pFctTrcAdminObj_isRuntimeInfoActive(this, i_eFilterDetailLevel);
    }
    return bIsActive;
}

//------------------------------------------------------------------------------
/*! @brief Sets the trace data filter as a regular expression.

    The filter is a regular expression which allows to define a positive
    pattern where only the data will be traced which mets the expression
    or a negative pattern which supporessed the trace output if the
    filter does not match.

    Examples

    @param i_szFilter [in] Filter as regular expression.
*/
void DllIf::CTrcAdminObj::setTraceDataFilter( const char* i_szFilter )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_setTraceDataFilter != NULL )
    {
        s_pFctTrcAdminObj_setTraceDataFilter(this, i_szFilter);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the trace data filter.

    @return Trace data filter (string containing a regular expression).
            The ownership is passed to the caller of the method.
            The caller must delete the returned string.
*/
char* DllIf::CTrcAdminObj::getTraceDataFilter() const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_getTraceDataFilter != NULL )
    {
        return s_pFctTrcAdminObj_getTraceDataFilter(this);
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether given trace data should be suppressed by the data filter.

    @param i_szTraceData [in]
        Trace data to be checked against the filter string.

    @return true if the passed trace data should be suppressed, false otherwise.
*/
bool DllIf::CTrcAdminObj::isTraceDataSuppressedByFilter( const char* i_szTraceData ) const
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_isTraceDataSuppressedByFilter != NULL )
    {
        return s_pFctTrcAdminObj_isTraceDataSuppressedByFilter(this, i_szTraceData);
    }
    return false;
}


/*******************************************************************************
class CMethodTracer
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Instance tracer (name space, class and object name set at trace admin object).

    @param i_pTrcAdminObj [in]
        Trace admin object used to control the output detail level of the method trace.
    @param i_eFilterDetailLevel [in]
        Entering and leaving the method is traced if the admin objects detail level
        is greater or equal than the filter setting than the detail level.
    @param i_szMethod [in]
        Name of the method to be traced.
    @param i_szMethodInArgs [in]
        String describing the input arguments passed to the method to be traced.
*/
DllIf::CMethodTracer::CMethodTracer(
    CTrcAdminObj* i_pTrcAdminObj,
    EMethodTraceDetailLevel i_eFilterDetailLevel,
    const char* i_szMethod,
    const char* i_szMethodInArgs ) :
//------------------------------------------------------------------------------
    m_pTrcAdminObj(i_pTrcAdminObj),
    m_eEnterLeaveFilterDetailLevel(i_eFilterDetailLevel),
    m_bEnterTraced(false),
    m_szObjName(NULL),
    m_szMethod(NULL),
    m_szMethodReturn(NULL),
    m_szMethodOutArgs(NULL)
{
    size_t iStrLen = strlen(i_szMethod);
    m_szMethod = new char[iStrLen+1];
    memset(m_szMethod, 0x00, iStrLen+1);
    memcpy(m_szMethod, i_szMethod, iStrLen);

    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_areMethodCallsActive != NULL && s_pFctTrcAdminObj_traceMethodEnter != NULL )
    {
        if( m_pTrcAdminObj != NULL )
        {
            if( s_pFctTrcAdminObj_areMethodCallsActive(m_pTrcAdminObj, m_eEnterLeaveFilterDetailLevel) )
            {
                s_pFctTrcAdminObj_traceMethodEnter(m_pTrcAdminObj, m_szObjName, m_szMethod, i_szMethodInArgs);
                m_bEnterTraced = true;
            }
            m_pTrcAdminObj->lock();

            if( s_pFctTrcAdminObj_lock != NULL )
            {
                s_pFctTrcAdminObj_lock(m_pTrcAdminObj);
            }
        }
    }
} // ctor

//------------------------------------------------------------------------------
/*! Class tracer (name space and class name (but not object name) set at trace admin object).

    @param i_pTrcAdminObj [in]
        Trace admin object used to control the output detail level of the method trace.
    @param i_eFilterDetailLevel [in]
        Entering and leaving the method is traced if the admin objects detail level
        is greater or equal than the filter setting than the detail level.
    @param i_szObjName [in]
        Name of the object (instance) the method to be traced is applied to.
    @param i_szMethod [in]
        Name of the method to be traced.
    @param i_szMethodInArgs [in]
        String describing the input arguments passed to the method to be traced.
*/
DllIf::CMethodTracer::CMethodTracer(
    CTrcAdminObj* i_pTrcAdminObj,
    EMethodTraceDetailLevel i_eFilterDetailLevel,
    const char* i_szObjName,
    const char* i_szMethod,
    const char* i_szMethodInArgs ) :
//------------------------------------------------------------------------------
    m_pTrcAdminObj(i_pTrcAdminObj),
    m_eEnterLeaveFilterDetailLevel(i_eFilterDetailLevel),
    m_bEnterTraced(false),
    m_szObjName(NULL),
    m_szMethod(NULL),
    m_szMethodReturn(NULL),
    m_szMethodOutArgs(NULL)
{
    size_t iStrLen = strlen(i_szObjName);
    m_szObjName = new char[iStrLen+1];
    memset(m_szObjName, 0x00, iStrLen+1);
    memcpy(m_szObjName, i_szObjName, iStrLen);

    iStrLen = strlen(i_szMethod);
    m_szMethod = new char[iStrLen+1];
    memset(m_szMethod, 0x00, iStrLen+1);
    memcpy(m_szMethod, i_szMethod, iStrLen);

    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_areMethodCallsActive != NULL && s_pFctTrcAdminObj_traceMethodEnter != NULL )
    {
        if( m_pTrcAdminObj != NULL )
        {
            if( s_pFctTrcAdminObj_areMethodCallsActive(m_pTrcAdminObj, m_eEnterLeaveFilterDetailLevel) )
            {
                s_pFctTrcAdminObj_traceMethodEnter(m_pTrcAdminObj, m_szObjName, m_szMethod, i_szMethodInArgs);
                m_bEnterTraced = true;
            }
            m_pTrcAdminObj->lock();

            if( s_pFctTrcAdminObj_lock != NULL )
            {
                s_pFctTrcAdminObj_lock(m_pTrcAdminObj);
            }
        }
    }
} // ctor

//------------------------------------------------------------------------------
/*! Destroys the CMethodTracer and traces leaving the method.

    @note If the CMethodTracer is used in the destructor of a class and a trace
          admin object is used the trace admin object is usually released in this
          destructor. If the trace admin object would be destroyed when releasing
          the instance leaving the destructor may not be traced and the program
          may crash as the method tracer would access a dead instance.
          This should usually not happen as the trace server together with the
          tree of admin objects should be destroyed at the end of the program.
          But to be on safe side you can call "onAdminObjAboutToBeReleased"
          before releasing the trace admin object.
*/
DllIf::CMethodTracer::~CMethodTracer()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_traceMethodLeave != NULL )
    {
        if( m_pTrcAdminObj != NULL )
        {
            onAdminObjAboutToBeReleased();
        }
    }

    delete m_szObjName;
    delete m_szMethod;
    delete m_szMethodReturn;
    delete m_szMethodOutArgs;

    m_pTrcAdminObj = NULL;
    m_eEnterLeaveFilterDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
    m_bEnterTraced = false;
    m_szObjName = NULL;
    m_szMethod = NULL;
    m_szMethodReturn = NULL;
    m_szMethodOutArgs = NULL;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! This method may be called if the CMethodTracer is used in the destructor
    of a class right before the trace admin object is released.

    @see ~CMethodTracer for more details.
*/
void DllIf::CMethodTracer::onAdminObjAboutToBeReleased()
//------------------------------------------------------------------------------
{
    if( m_pTrcAdminObj != NULL && s_pFctTrcAdminObj_traceMethodLeave != NULL )
    {
        if( m_bEnterTraced )
        {
            s_pFctTrcAdminObj_traceMethodLeave(m_pTrcAdminObj, m_szObjName, m_szMethod, m_szMethodReturn, m_szMethodOutArgs);
        }
        m_pTrcAdminObj->unlock();

        if( s_pFctTrcAdminObj_unlock != NULL )
        {
            s_pFctTrcAdminObj_unlock(m_pTrcAdminObj);
        }
        if( m_pTrcAdminObj->deleteOnUnlock() )
        {
            CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        }
    }
    m_pTrcAdminObj = NULL;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the current detail level either set at the trace admin object or
    at the method tracer itself.

    @return Current trace detail level.
*/
DllIf::EMethodTraceDetailLevel DllIf::CMethodTracer::getMethodCallsTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    EMethodTraceDetailLevel eDetailLevel = EMethodTraceDetailLevelNone;

    if( m_pTrcAdminObj != NULL && s_pFctTrcAdminObj_getMethodCallsTraceDetailLevel != NULL )
    {
        eDetailLevel = s_pFctTrcAdminObj_getMethodCallsTraceDetailLevel(m_pTrcAdminObj);
    }
    return eDetailLevel;
}

//------------------------------------------------------------------------------
/*! Returns whether tracing is active for the given filter detail level.

    E.g. if or the trace admin object the detail level MethodArgs is set and
    "areMethodCallsActive" will return false for detail levels greater than MethodArgs.

    @param i_eFilterDetailLevel [in]
        Trace outputs should be generated if the given filter detail level
        is greater or equal than the current detail level set at the trace
        admin object or at the method tracer itself.

    @return true if tracing is active for the given filter detail level.
            false otherwise.
*/
bool DllIf::CMethodTracer::areMethodCallsActive( EMethodTraceDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( m_pTrcAdminObj != NULL && s_pFctTrcAdminObj_areMethodCallsActive != NULL )
    {
        bIsActive = s_pFctTrcAdminObj_areMethodCallsActive(m_pTrcAdminObj, i_eFilterDetailLevel);
    }
    return bIsActive;
}

//------------------------------------------------------------------------------
/*! Returns the current detail level either set at the trace admin object or
    at the method tracer itself.

    @return Current trace detail level.
*/
DllIf::ELogDetailLevel DllIf::CMethodTracer::getRuntimeInfoTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    ELogDetailLevel eDetailLevel = ELogDetailLevelNone;

    if( m_pTrcAdminObj != NULL && s_pFctTrcAdminObj_getRuntimeInfoTraceDetailLevel != NULL )
    {
        eDetailLevel = s_pFctTrcAdminObj_getRuntimeInfoTraceDetailLevel(m_pTrcAdminObj);
    }
    return eDetailLevel;
}

//------------------------------------------------------------------------------
/*! Returns whether tracing is active for the given filter detail level.

    E.g. if or the trace admin object the detail level MethodArgs is set and
    "isActive" will return false for detail levels greater than MethodArgs.

    @param i_eFilterDetailLevel [in]
        Trace outputs should be generated if the given filter detail level
        is greater or equal than the current detail level set at the trace
        admin object or at the method tracer itself.

    @return true if tracing is active for the given filter detail level.
            false otherwise.
*/
bool DllIf::CMethodTracer::isRuntimeInfoActive( ELogDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( m_pTrcAdminObj != NULL && s_pFctTrcAdminObj_isRuntimeInfoActive != NULL )
    {
        bIsActive = s_pFctTrcAdminObj_isRuntimeInfoActive(m_pTrcAdminObj, i_eFilterDetailLevel);
    }
    return bIsActive;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether given trace data should be suppressed by the data filter.

    Example

        if( !m_pTrcAdminObj->isTraceDataSuppressedByFilter("bla bla bla") )
        {
            strTrcOutData = "bla bla bla";
        }

    @param i_strTraceData [in]
        Trace data to be checked against the filter string.

    @return true if the passed trace data should be suppressed, false otherwise.
*/
bool DllIf::CMethodTracer::isTraceDataSuppressedByFilter( const char* i_szTraceData ) const
//------------------------------------------------------------------------------
{
    bool bSuppressed = false;

    if( m_pTrcAdminObj != nullptr && s_pFctTrcAdminObj_isTraceDataSuppressedByFilter )
    {
        bSuppressed = m_pTrcAdminObj->isTraceDataSuppressedByFilter(i_szTraceData);
    }
    return bSuppressed;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! If the destructor of the method tracer class is called leaving the method
    is traced. If the method returns a value and this return value should appear
    in the trace output the return value may be set by this method.

    @param i_bResult [in] Return value of the method to be traced.
*/
void DllIf::CMethodTracer::setMethodReturn( bool i_bResult )
//------------------------------------------------------------------------------
{
    delete m_szMethodReturn;
    m_szMethodReturn = NULL;

    const char* c_szTrue  = "true";  const int c_iStrLenTrue  = 4;
    const char* c_szFalse = "false"; const int c_iStrLenFalse = 5;

    if( i_bResult )
    {
        m_szMethodReturn = new char[c_iStrLenTrue+1];
        memset(m_szMethodReturn, 0x00, c_iStrLenTrue+1);
        memcpy(m_szMethodReturn, c_szTrue, c_iStrLenTrue);
    }
    else // if( !i_bResult )
    {
        m_szMethodReturn = new char[c_iStrLenFalse+1];
        memset(m_szMethodReturn, 0x00, c_iStrLenFalse+1);
        memcpy(m_szMethodReturn, c_szFalse, c_iStrLenFalse);
    }

} // setMethodReturn

//------------------------------------------------------------------------------
/*! If the destructor of the method tracer class is called leaving the method
    is traced. If the method returns a value and this return value should appear
    in the trace output the return value may be set by this method.

    @param i_iResult [in] Return value of the method to be traced.
*/
void DllIf::CMethodTracer::setMethodReturn( int i_iResult )
//------------------------------------------------------------------------------
{
    delete m_szMethodReturn;
    m_szMethodReturn = NULL;

    //                        1         2
    //               12345678901234567890123456
    // Max uint_32:               4.294.967.296
    // Max uint_64:  18.446.744.073.709.551.616
    const int c_iStrLenMax = 26;

    m_szMethodReturn = new char[c_iStrLenMax+1];
    memset(m_szMethodReturn, 0x00, c_iStrLenMax+1);

    #ifdef _WINDOWS
    _itoa_s(i_iResult, m_szMethodReturn, c_iStrLenMax+1, 10);
    #else
    sprintf(m_szMethodReturn, "%d", i_iResult);
    #endif
}

//------------------------------------------------------------------------------
/*! If the destructor of the method tracer class is called leaving the method
    is traced. If the method returns a value and this return value should appear
    in the trace output the return value may be set by this method.

    @param i_sz [in] Return value of the method to be traced.
*/
void DllIf::CMethodTracer::setMethodReturn( const char* i_sz )
//------------------------------------------------------------------------------
{
    delete m_szMethodReturn;
    m_szMethodReturn = NULL;

    size_t iStrLen = strlen(i_sz);
    m_szMethodReturn = new char[iStrLen+1];
    memset(m_szMethodReturn, 0x00, iStrLen+1);
    memcpy(m_szMethodReturn, i_sz, iStrLen);

} // setMethodReturn

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! If the destructor of the method tracer class is called leaving the method
    is traced. If the method returns values through arguments and those output
    arguments should appear in the trace output the output values may be set
    by invoking this method.

    @param i_sz [in] String describing the output arguments of the method to be traced.
*/
void DllIf::CMethodTracer::setMethodOutArgs( const char* i_sz )
//------------------------------------------------------------------------------
{
    delete m_szMethodOutArgs;
    m_szMethodOutArgs = NULL;

    size_t iStrLen = strlen(i_sz);
    m_szMethodOutArgs = new char[iStrLen+1];
    memset(m_szMethodOutArgs, 0x00, iStrLen+1);
    memcpy(m_szMethodOutArgs, i_sz, iStrLen);

} // setMethodOutArgs

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Adds a trace output string.

    This method is used to add additional runtime information to the trace output.

    @param i_szAddInfo [in]
        Additional runtime information to be output.
    @param i_eFilterDetailLevel [in]
        Trace outputs should be generated if the given filter detail level
        is greater or equal than the current detail level set at the trace
        admin object or at the method tracer itself.
*/
void DllIf::CMethodTracer::trace( const char* i_szAddInfo, ELogDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    if( m_pTrcAdminObj != NULL && s_pFctTrcAdminObj_isRuntimeInfoActive != NULL && s_pFctTrcAdminObj_traceMethod != NULL )
    {
        if( s_pFctTrcAdminObj_isRuntimeInfoActive(m_pTrcAdminObj, i_eFilterDetailLevel) )
        {
            s_pFctTrcAdminObj_traceMethod(m_pTrcAdminObj, m_szObjName, m_szMethod, i_szAddInfo);
        }
    }
} // trace


/*******************************************************************************
class CTrcServer
*******************************************************************************/

/*==============================================================================
public: // class methods to add, remove and modify admin objects
==============================================================================*/

//------------------------------------------------------------------------------
/*!
    @param i_szNameSpace [in]
           An empty string or nullptr is not a good choice.

    @param i_szClassName [in]
           An empty string or nullptr is not a good choice.

    @param i_szObjName [in]
           An empty string or nullptr is not a good choice.
           For admin objects which are class members the class name may be passed
           instead of an empty string.

    @param i_bEnabledAsDefault [in]
    @param i_iDefaultDetailLevel [in]
*/
DllIf::CTrcAdminObj* DllIf::CTrcServer::GetTraceAdminObj(
    const char* i_szNameSpace,
    const char* i_szClassName,
    const char* i_szObjName,
    EEnabled    i_bEnabledAsDefault,
    EMethodTraceDetailLevel i_eMethodCallsDefaultDetailLevel,
    ELogDetailLevel i_eRuntimeInfoDefaultDetailLevel )
//------------------------------------------------------------------------------
{
    DllIf::CTrcAdminObj* pTrcAdminObj = NULL;

    if( s_hndDllIf != NULL && s_pFctTrcServer_GetTraceAdminObj != NULL )
    {
        pTrcAdminObj = s_pFctTrcServer_GetTraceAdminObj(
            i_szNameSpace, i_szClassName, i_szObjName,
            i_bEnabledAsDefault, i_eMethodCallsDefaultDetailLevel, i_eRuntimeInfoDefaultDetailLevel);
    }
    return pTrcAdminObj;
}

//------------------------------------------------------------------------------
void DllIf::CTrcServer::RenameTraceAdminObj(
    CTrcAdminObj** io_ppTrcAdminObj,
    const char*    i_szNewObjName )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_RenameTraceAdminObj != NULL )
    {
        s_pFctTrcServer_RenameTraceAdminObj(io_ppTrcAdminObj, i_szNewObjName);
    }
}

//------------------------------------------------------------------------------
void DllIf::CTrcServer::ReleaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_ReleaseTraceAdminObj != NULL )
    {
        s_pFctTrcServer_ReleaseTraceAdminObj(i_pTrcAdminObj);
    }
}

/*==============================================================================
public: // class method to save/recall admin objects file
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the name of the organization.

    @param i_szName [in] Name of the organization.
*/
void DllIf::CTrcServer::SetOrganizationName( const char* i_szName )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_SetOrganizationName != NULL )
    {
        s_pFctTrcServer_SetOrganizationName(i_szName);
    }
}

//------------------------------------------------------------------------------
/*! Returns the name of the organization.

    @return Name of the organization. The string must be freed by the caller.
*/
char* DllIf::CTrcServer::GetOrganizationName()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_GetOrganizationName != NULL )
    {
        return s_pFctTrcServer_GetOrganizationName();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! Sets the name of the application.

    @param i_szName [in] Name of the application.
*/
void DllIf::CTrcServer::SetApplicationName( const char* i_szName )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_SetApplicationName != NULL )
    {
        s_pFctTrcServer_SetApplicationName(i_szName);
    }
}

//------------------------------------------------------------------------------
/*! Returns the name of the application.

    @return Name of the application. The string must be freed by the caller.
*/
char* DllIf::CTrcServer::GetApplicationName()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_GetApplicationName != NULL )
    {
        return s_pFctTrcServer_GetApplicationName();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! Sets the absolute file path for trace admin objects xml file.

    @param i_szAbsFilePath [in]
        Absolute file path including directory path, file name and suffix.
*/
void DllIf::CTrcServer::SetAdminObjFileAbsoluteFilePath( const char* i_szAbsFilePath )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_SetAdminObjFileAbsoluteFilePath != NULL )
    {
        s_pFctTrcServer_SetAdminObjFileAbsoluteFilePath(i_szAbsFilePath);
    }
}

//------------------------------------------------------------------------------
/*! Returns the path information of the trace admin objects xml file.

    The returned character pointer must be freed after calling the method.

    @return Character buffer containing the files absolute path.
            The caller must free this buffer.
*/
char* DllIf::CTrcServer::GetAdminObjFileAbsoluteFilePath()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_GetAdminObjFileAbsoluteFilePath != NULL )
    {
        return s_pFctTrcServer_GetAdminObjFileAbsoluteFilePath();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! Returns the base name of the trace admin objects xml file.

    The returned character pointer must be freed after calling the method.

    @return Character buffer containing the base name.
            The caller must free this buffer.
*/
char* DllIf::CTrcServer::GetAdminObjFileCompleteBaseName()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_GetAdminObjFileCompleteBaseName != NULL )
    {
        return s_pFctTrcServer_GetAdminObjFileCompleteBaseName();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! Returns the absolute path of the trace admin objects xml file.

    The returned character pointer must be freed after calling the method.

    @return Character buffer containing the files absolute path (without filename).
            The caller must free this buffer.
*/
char* DllIf::CTrcServer::GetAdminObjFileAbsolutePath()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_GetAdminObjFileAbsolutePath != NULL )
    {
        return s_pFctTrcServer_GetAdminObjFileAbsolutePath();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! Sets the absolute file path for trace method log file.

    @param i_szAbsFilePath [in]
        Absolute file path including directory path, file name and suffix.
*/
void DllIf::CTrcServer::SetLocalTrcFileAbsoluteFilePath( const char* i_szAbsFilePath )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_SetLocalTrcFileAbsoluteFilePath != NULL )
    {
        s_pFctTrcServer_SetLocalTrcFileAbsoluteFilePath(i_szAbsFilePath);
    }
}

//------------------------------------------------------------------------------
/*! Returns the path information for the trace method log file.

    The returned character pointer must be freed after calling the method.

    @return Character buffer containing the files absolute path. The caller
            must free this buffer.
*/
char* DllIf::CTrcServer::GetLocalTrcFileAbsoluteFilePath()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_GetLocalTrcFileAbsoluteFilePath != NULL )
    {
        return s_pFctTrcServer_GetLocalTrcFileAbsoluteFilePath();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! Returns the base name of the trace method log file.

    The returned character pointer must be freed after calling the method.

    @return Character buffer containing the base name.
            The caller must free this buffer.
*/
char* DllIf::CTrcServer::GetLocalTrcFileCompleteBaseName()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_GetLocalTrcFileCompleteBaseName != NULL )
    {
        return s_pFctTrcServer_GetLocalTrcFileCompleteBaseName();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/*! Returns the absolute path of the trace method log file.

    The returned character pointer must be freed after calling the method.

    @return Character buffer containing the files absolute path (without filename).
            The caller must free this buffer.
*/
char* DllIf::CTrcServer::GetLocalTrcFileAbsolutePath()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_GetLocalTrcFileAbsolutePath != NULL )
    {
        return s_pFctTrcServer_GetLocalTrcFileAbsolutePath();
    }
    return NULL;
}

//------------------------------------------------------------------------------
void DllIf::CTrcServer::RegisterThread( const char* i_szThreadName, void* i_pvThreadHandle )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_RegisterThread != NULL )
    {
        s_pFctTrcServer_RegisterThread(i_szThreadName, i_pvThreadHandle);
    }
}

//------------------------------------------------------------------------------
void DllIf::CTrcServer::UnregisterThread( void* i_pvThreadHandle )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_UnregisterThread != NULL )
    {
        s_pFctTrcServer_UnregisterThread(i_pvThreadHandle);
    }
}

//------------------------------------------------------------------------------
char* DllIf::CTrcServer::GetThreadName( void* i_pvThreadHandle )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_GetThreadName != NULL )
    {
        return s_pFctTrcServer_GetThreadName(i_pvThreadHandle);
    }
    return NULL;
}

//------------------------------------------------------------------------------
char* DllIf::CTrcServer::GetCurrentThreadName()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_GetCurrentThreadName != NULL )
    {
        return s_pFctTrcServer_GetCurrentThreadName();
    }
    return NULL;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::isActive() const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( s_hndDllIf != NULL && s_pFctTrcServer_isActive != NULL )
    {
        bIsActive = s_pFctTrcServer_isActive();
    }
    return bIsActive;
}

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_setEnabled != NULL )
    {
        s_pFctTrcServer_setEnabled(i_bEnabled);
    }
}

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::isEnabled() const
//------------------------------------------------------------------------------
{
    bool bEnabled = false;

    if( s_hndDllIf != NULL && s_pFctTrcServer_isEnabled != NULL )
    {
        bEnabled = s_pFctTrcServer_isEnabled();
    }
    return bEnabled;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setNewTrcAdminObjsEnabledAsDefault( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_setNewTrcAdminObjsEnabledAsDefault != NULL )
    {
        s_pFctTrcServer_setNewTrcAdminObjsEnabledAsDefault(i_bEnabled);
    }
}

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::areNewTrcAdminObjsEnabledAsDefault() const
//------------------------------------------------------------------------------
{
    bool bEnabled = false;

    if( s_hndDllIf != NULL && s_pFctTrcServer_areNewTrcAdminObjsEnabledAsDefault != NULL )
    {
        bEnabled = s_pFctTrcServer_areNewTrcAdminObjsEnabledAsDefault();
    }
    return bEnabled;
}

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setNewTrcAdminObjsMethodCallsDefaultDetailLevel( EMethodTraceDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_setNewTrcAdminObjsMethodCallsDefaultDetailLevel != NULL )
    {
        s_pFctTrcServer_setNewTrcAdminObjsMethodCallsDefaultDetailLevel(i_eDetailLevel);
    }
}

//------------------------------------------------------------------------------
DllIf::EMethodTraceDetailLevel DllIf::CTrcServer::getNewTrcAdminObjsMethodCallsDefaultDetailLevel() const
//------------------------------------------------------------------------------
{
    EMethodTraceDetailLevel eDetailLevel = EMethodTraceDetailLevelNone;

    if( s_hndDllIf != NULL && s_pFctTrcServer_getNewTrcAdminObjsMethodCallsDefaultDetailLevel != NULL )
    {
        eDetailLevel = s_pFctTrcServer_getNewTrcAdminObjsMethodCallsDefaultDetailLevel();
    }
    return eDetailLevel;
}

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel( ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel != NULL )
    {
        s_pFctTrcServer_setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel(i_eDetailLevel);
    }
}

//------------------------------------------------------------------------------
DllIf::ELogDetailLevel DllIf::CTrcServer::getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel() const
//------------------------------------------------------------------------------
{
    ELogDetailLevel eDetailLevel = ELogDetailLevelNone;

    if( s_hndDllIf != NULL && s_pFctTrcServer_getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel != NULL )
    {
        eDetailLevel = s_pFctTrcServer_getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel();
    }
    return eDetailLevel;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::recallAdminObjs()
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndDllIf != NULL && s_pFctTrcServer_recallAdminObjs != NULL )
    {
        bOk = s_pFctTrcServer_recallAdminObjs();
    }
    return bOk;
}

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::saveAdminObjs()
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndDllIf != NULL && s_pFctTrcServer_saveAdminObjs != NULL )
    {
        bOk = s_pFctTrcServer_saveAdminObjs();
    }
    return bOk;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setUseLocalTrcFile( bool i_bUse )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_setUseLocalTrcFile != NULL )
    {
        s_pFctTrcServer_setUseLocalTrcFile(i_bUse);
    }
}

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::isLocalTrcFileUsed() const
//------------------------------------------------------------------------------
{
    bool bUsed = false;

    if( s_hndDllIf != NULL && s_pFctTrcServer_isLocalTrcFileUsed != NULL )
    {
        bUsed = s_pFctTrcServer_isLocalTrcFileUsed();
    }
    return bUsed;
}

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::isLocalTrcFileActive() const
//------------------------------------------------------------------------------
{
    bool bUsed = false;

    if( s_hndDllIf != NULL && s_pFctTrcServer_isLocalTrcFileActive != NULL )
    {
        bUsed = s_pFctTrcServer_isLocalTrcFileActive();
    }
    return bUsed;
}

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setLocalTrcFileAutoSaveIntervalInMs( int i_iAutoSaveInterval_ms )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_setLocalTrcFileAutoSaveIntervalInMs != NULL )
    {
        s_pFctTrcServer_setLocalTrcFileAutoSaveIntervalInMs(i_iAutoSaveInterval_ms);
    }
}

//------------------------------------------------------------------------------
int DllIf::CTrcServer::getLocalTrcFileAutoSaveIntervalInMs() const
//------------------------------------------------------------------------------
{
    int iAutoSaveInterval_ms = 0;

    if( s_hndDllIf != NULL && s_pFctTrcServer_getLocalTrcFileAutoSaveIntervalInMs != NULL )
    {
        iAutoSaveInterval_ms = s_pFctTrcServer_getLocalTrcFileAutoSaveIntervalInMs();
    }
    return iAutoSaveInterval_ms;
}

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setLocalTrcFileCloseFileAfterEachWrite( bool i_bCloseFile )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite != NULL )
    {
        s_pFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite(i_bCloseFile);
    }
}

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::getLocalTrcFileCloseFileAfterEachWrite() const
//------------------------------------------------------------------------------
{
    bool bClose = false;

    if( s_hndDllIf != NULL && s_pFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite != NULL )
    {
        bClose = s_pFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite();
    }
    return bClose;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setLocalTrcFileSubFileCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_setLocalTrcFileSubFileCountMax != NULL )
    {
        s_pFctTrcServer_setLocalTrcFileSubFileCountMax(i_iCountMax);
    }
}

//------------------------------------------------------------------------------
int DllIf::CTrcServer::getLocalTrcFileSubFileCountMax() const
//------------------------------------------------------------------------------
{
    int iCountMax = 0;

    if( s_hndDllIf != NULL && s_pFctTrcServer_getLocalTrcFileSubFileCountMax != NULL )
    {
        iCountMax = s_pFctTrcServer_getLocalTrcFileSubFileCountMax();
    }
    return iCountMax;
}

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setLocalTrcFileSubFileLineCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_setLocalTrcFileSubFileLineCountMax != NULL )
    {
        s_pFctTrcServer_setLocalTrcFileSubFileLineCountMax(i_iCountMax);
    }
}

//------------------------------------------------------------------------------
int DllIf::CTrcServer::getLocalTrcFileSubFileLineCountMax() const
//------------------------------------------------------------------------------
{
    int iCountMax = 0;

    if( s_hndDllIf != NULL && s_pFctTrcServer_getLocalTrcFileSubFileLineCountMax != NULL )
    {
        iCountMax = s_pFctTrcServer_getLocalTrcFileSubFileLineCountMax();
    }
    return iCountMax;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setUseIpcServer( bool i_bUse )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_setUseIpcServer != NULL )
    {
        s_pFctTrcServer_setUseIpcServer(i_bUse);
    }
}

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::isIpcServerUsed() const
//------------------------------------------------------------------------------
{
    bool bUsed = false;

    if( s_hndDllIf != NULL && s_pFctTrcServer_isIpcServerUsed != NULL )
    {
        bUsed = s_pFctTrcServer_isIpcServerUsed();
    }
    return bUsed;
}

//------------------------------------------------------------------------------
/*! Defines whether caching of the method trace should be activated.

    If no trace client is connected, the trace ooutputs are not sent to the client.
    If caching is enabled the trace outputs are locally stored in a cache.
    If the client connect the cached trace outputs will be send to the client.
    In this way, for example, the startup of the application can also be made visible
    to the client if the clients connects later on.

    @param i_bCacheData [In] true to activate caching.
*/
void DllIf::CTrcServer::setCacheTrcDataIfNotConnected( bool i_bCacheData )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_setCacheTrcDataIfNotConnected != NULL )
    {
        s_pFctTrcServer_setCacheTrcDataIfNotConnected(i_bCacheData);
    }
}

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::getCacheTrcDataIfNotConnected() const
//------------------------------------------------------------------------------
{
    bool bCacheData = false;

    if( s_hndDllIf != NULL && s_pFctTrcServer_getCacheTrcDataIfNotConnected != NULL )
    {
        bCacheData = s_pFctTrcServer_getCacheTrcDataIfNotConnected();
    }
    return bCacheData;
}

//------------------------------------------------------------------------------
/*! Ist das Caching aktiviert wird hier die Anzahl der Zeilen definiert, die im
    Cache abgelegt werden.

    @param i_iMaxArrLen [In] Anzahl der Methoden Aufrufe, die im Cache gespeichert
           werden, bis sich ein Client verbindet.
*/
void DllIf::CTrcServer::setCacheTrcDataMaxArrLen( int i_iMaxArrLen )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_setCacheTrcDataMaxArrLen != NULL )
    {
        s_pFctTrcServer_setCacheTrcDataMaxArrLen(i_iMaxArrLen);
    }
}

//------------------------------------------------------------------------------
int DllIf::CTrcServer::getCacheTrcDataMaxArrLen() const
//------------------------------------------------------------------------------
{
    int iMaxArrLen = 0;

    if( s_hndDllIf != NULL && s_pFctTrcServer_getCacheTrcDataMaxArrLen != NULL )
    {
        iMaxArrLen = s_pFctTrcServer_getCacheTrcDataMaxArrLen();
    }
    return iMaxArrLen;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::setTraceSettings( const DllIf::STrcServerSettings& i_settings )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndDllIf != NULL && s_pFctTrcServer_setTraceSettings != NULL )
    {
        bOk = s_pFctTrcServer_setTraceSettings(i_settings);
    }
    return bOk;
}

//------------------------------------------------------------------------------
DllIf::STrcServerSettings DllIf::CTrcServer::getTraceSettings() const
//------------------------------------------------------------------------------
{
    DllIf::STrcServerSettings settings;

    STrcServerSettings_init(settings);

    if( s_hndDllIf != NULL && s_pFctTrcServer_getTraceSettings != NULL )
    {
        settings = s_pFctTrcServer_getTraceSettings();
    }
    return settings;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CTrcServer::clearLocalTrcFile()
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcServer_clearLocalTrcFile != NULL )
    {
        s_pFctTrcServer_clearLocalTrcFile();
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CTrcServer::traceMethodEnter(
    const CTrcAdminObj* i_pAdminObj,
    const char*         i_szMethod,
    const char*         i_szMethodInArgs )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_areMethodCallsActive != NULL && s_pFctTrcAdminObj_traceMethodEnter != NULL )
    {
        if( i_pAdminObj != NULL )
        {
            if( s_pFctTrcAdminObj_areMethodCallsActive(i_pAdminObj, EMethodTraceDetailLevelEnterLeave) )
            {
                s_pFctTrcAdminObj_traceMethodEnter(i_pAdminObj, "", i_szMethod, i_szMethodInArgs);
            }
        }
    }
}

//------------------------------------------------------------------------------
void DllIf::CTrcServer::traceMethodEnter(
    const CTrcAdminObj* i_pAdminObj,
    const char*         i_szObjName,
    const char*         i_szMethod,
    const char*         i_szMethodInArgs )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_areMethodCallsActive != NULL && s_pFctTrcAdminObj_traceMethodEnter != NULL )
    {
        if( i_pAdminObj != NULL )
        {
            if( s_pFctTrcAdminObj_areMethodCallsActive(i_pAdminObj, EMethodTraceDetailLevelEnterLeave) )
            {
                s_pFctTrcAdminObj_traceMethodEnter(i_pAdminObj, i_szObjName, i_szMethod, i_szMethodInArgs);
            }
        }
    }
}

//------------------------------------------------------------------------------
void DllIf::CTrcServer::traceMethod(
    const CTrcAdminObj* i_pAdminObj,
    const char*         i_szMethod,
    const char*         i_szAddInfo )
//------------------------------------------------------------------------------
{
    if( i_pAdminObj != NULL && s_pFctTrcAdminObj_isRuntimeInfoActive != NULL && s_pFctTrcAdminObj_traceMethod != NULL )
    {
        if( s_pFctTrcAdminObj_isRuntimeInfoActive(i_pAdminObj, ELogDetailLevelFatal) )
        {
            s_pFctTrcAdminObj_traceMethod(i_pAdminObj, "", i_szMethod, i_szAddInfo);
        }
    }
}

//------------------------------------------------------------------------------
void DllIf::CTrcServer::traceMethod(
    const CTrcAdminObj* i_pAdminObj,
    const char*         i_szObjName,
    const char*         i_szMethod,
    const char*         i_szAddInfo )
//------------------------------------------------------------------------------
{
    if( i_pAdminObj != NULL && s_pFctTrcAdminObj_isRuntimeInfoActive != NULL && s_pFctTrcAdminObj_traceMethod != NULL )
    {
        if( s_pFctTrcAdminObj_isRuntimeInfoActive(i_pAdminObj, ELogDetailLevelFatal) )
        {
            s_pFctTrcAdminObj_traceMethod(i_pAdminObj, i_szObjName, i_szMethod, i_szAddInfo);
        }
    }
}

//------------------------------------------------------------------------------
void DllIf::CTrcServer::traceMethodLeave(
    const CTrcAdminObj* i_pAdminObj,
    const char*         i_szMethod,
    const char*         i_szMethodReturn,
    const char*         i_szMethodOutArgs )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_areMethodCallsActive != NULL && s_pFctTrcAdminObj_traceMethodLeave != NULL )
    {
        if( i_pAdminObj != NULL )
        {
            if( s_pFctTrcAdminObj_areMethodCallsActive(i_pAdminObj, EMethodTraceDetailLevelEnterLeave) )
            {
                s_pFctTrcAdminObj_traceMethodLeave(i_pAdminObj, "", i_szMethod, i_szMethodReturn, i_szMethodOutArgs);
            }
        }
    }
}

//------------------------------------------------------------------------------
void DllIf::CTrcServer::traceMethodLeave(
    const CTrcAdminObj* i_pAdminObj,
    const char*         i_szObjName,
    const char*         i_szMethod,
    const char*         i_szMethodReturn,
    const char*         i_szMethodOutArgs )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctTrcAdminObj_areMethodCallsActive != NULL && s_pFctTrcAdminObj_traceMethodLeave != NULL )
    {
        if( i_pAdminObj != NULL )
        {
            if( s_pFctTrcAdminObj_areMethodCallsActive(i_pAdminObj, EMethodTraceDetailLevelEnterLeave) )
            {
                s_pFctTrcAdminObj_traceMethodLeave(i_pAdminObj, i_szObjName, i_szMethod, i_szMethodReturn, i_szMethodOutArgs);
            }
        }
    }
}


/*******************************************************************************
class CIpcTrcServer
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns a reference to the trace server with the given name.

    This method is used to access an already existing trace server.
    Neither a trace server may be created by this method call nor a reference
    counter of the trace server is affected.

    @return Reference to trace server or nullptr, if a trace server with the
            given name is not existing.

   @note This method does not incremente the reference counter of the trace server.
*/
DllIf::CIpcTrcServer* DllIf::CIpcTrcServer::GetInstance()
//------------------------------------------------------------------------------
{
    DllIf::CIpcTrcServer* pTrcServer = NULL;

    if( s_hndDllIf != NULL && s_pFctIpcTrcServer_GetInstance != NULL )
    {
        pTrcServer = s_pFctIpcTrcServer_GetInstance();
    }
    return pTrcServer;
}

//------------------------------------------------------------------------------
/*! Creates a trace server with the given name if a trace server with the given
    name is not already existing.

    If a trace server with the given name is already existing the reference to
    the existing trace server is returned and a reference counter is incremented.

    @param i_eTrcDetailLevelDllIf [in] For debugging purposes of the Dll interface
           the Dll interace methods may be traced itself by writing a log file.
           Default: ETraceDetailLevelNone
    @param i_eTrcDetailLevelTrcServer [in]
        If the methods of the trace server itself should be logged a value
        greater than 0 (ETraceDetailLevelNone) could be passed here.
    @param i_eTrcDetailLevelTrcServerMutex [in]
        If locking and unlocking the mutex of the trace server should be
        logged a value greater than 0 (ETraceDetailLevelNone) could be passed here.
    @param i_eTrcDetailLevelTrcServerIpcServer [in]
        If the methods of the trace server's Ipc Server should be logged
        a value greater than 0 (ETraceDetailLevelNone) could be passed here.
    @param i_eTrcDetailLevelTrcServerIpcServerMutex [in]
        If locking and unlocking the mutex of the trace server's Ipc Server should be
        logged a value greater than 0 (ETraceDetailLevelNone) could be passed here.
    @param i_eTrcDetailLevelTrcServerIpcServerGateway [in]
        If the methods of the trace server's Ipc Server's Gateway should
        be logged a value greater than 0 (ETraceDetailLevelNone) could be
        passed here.

    @return Reference to trace server or nullptr.
            nullptr is returned if i_bCreateIfNotExisting was set to false
            and a trace server with the given name is not existing.
*/
DllIf::CIpcTrcServer* DllIf::CIpcTrcServer::CreateInstance(
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelDllIf,
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServer,
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerMutex,
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerIpcServer,
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerIpcServerMutex,
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerIpcServerGateway )
//------------------------------------------------------------------------------
{
    DllIf::CIpcTrcServer* pTrcServer = NULL;

    if( s_hndDllIf != NULL && s_pFctIpcTrcServer_GetInstance != NULL )
    {
        pTrcServer = s_pFctIpcTrcServer_CreateInstance(
            i_eTrcDetailLevelDllIf,
            i_eTrcDetailLevelTrcServer,
            i_eTrcDetailLevelTrcServerMutex,
            i_eTrcDetailLevelTrcServerIpcServer,
            i_eTrcDetailLevelTrcServerIpcServerMutex,
            i_eTrcDetailLevelTrcServerIpcServerGateway);
    }
    return pTrcServer;
}

//------------------------------------------------------------------------------
/*! Releases the trace server by decrementing the reference counter. If the
    reference counter reaches 0 the trace server will be destroyed.

    @param i_pTrcServer [in] Reference to the trace server as returned by
           the GetInstance call.
*/
void DllIf::CIpcTrcServer::ReleaseInstance( DllIf::CIpcTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    if( s_hndDllIf != NULL && s_pFctIpcTrcServer_ReleaseInstance != NULL )
    {
        s_pFctIpcTrcServer_ReleaseInstance(i_pTrcServer);
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Starts the trace server.

    The Ipc server of the trace server is instantiated in an Gateway Thread
    and waits there to accept incoming connection requests.
    Starting the trace server is an asynchronous request.
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

    @return true if the trace server has been started.
*/
bool DllIf::CIpcTrcServer::startup( int i_iTimeout_ms, bool /*i_bWait*/ )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndDllIf != NULL && s_pFctIpcTrcServer_startup != NULL )
    {
        bOk = s_pFctIpcTrcServer_startup(this, i_iTimeout_ms, true);
    }
    return bOk;
}

//------------------------------------------------------------------------------
/*! Stops the trace server.

    The Ipc server of the trace server is instantiated in an Gateway Thread
    and waits there to accept incoming connection requests.
    Stopping the trace server is an asynchronous request.
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

    @return true if the trace server has been shutdown.
*/
bool DllIf::CIpcTrcServer::shutdown( int i_iTimeout_ms, bool /*i_bWait*/ )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndDllIf != NULL && s_pFctIpcTrcServer_shutdown != NULL )
    {
        bOk = s_pFctIpcTrcServer_shutdown(this, i_iTimeout_ms, true);
    }
    return bOk;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool DllIf::CIpcTrcServer::isListening() const
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndDllIf != NULL && s_pFctIpcTrcServer_isListening != NULL )
    {
        bOk = s_pFctIpcTrcServer_isListening(this);
    }
    return bOk;
}

//------------------------------------------------------------------------------
bool DllIf::CIpcTrcServer::isConnected() const
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndDllIf != NULL && s_pFctIpcTrcServer_isConnected != NULL )
    {
        bOk = s_pFctIpcTrcServer_isConnected(this);
    }
    return bOk;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool DllIf::CIpcTrcServer::setPort( unsigned short i_uPort, int i_iTimeout_ms, bool i_bWait )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndDllIf != NULL && s_pFctIpcTrcServer_setPort != NULL )
    {
        bOk = s_pFctIpcTrcServer_setPort(this, i_uPort, i_iTimeout_ms, i_bWait);
    }
    return bOk;
}

//------------------------------------------------------------------------------
unsigned short DllIf::CIpcTrcServer::getPort() const
//------------------------------------------------------------------------------
{
    unsigned short uPort = 0;

    if( s_hndDllIf != NULL && s_pFctIpcTrcServer_getPort != NULL )
    {
        uPort = s_pFctIpcTrcServer_getPort(this);
    }
    return uPort;
}

#endif // #ifdef USE_ZS_IPCTRACE_DLL_IF
