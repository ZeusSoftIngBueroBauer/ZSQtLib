/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSIpcTrace/ZSIpcTrcDllIf.h"

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
typedef void (*TFctTrcAdminObj_setEnabled)( DllIf::CTrcAdminObj* i_pTrcAdminObj, bool i_bEnabled );
typedef bool (*TFctTrcAdminObj_isEnabled)( const DllIf::CTrcAdminObj* i_pTrcAdminObj );
typedef void (*TFctTrcAdminObj_setTraceDetailLevel)( DllIf::CTrcAdminObj* i_pTrcAdminObj, int i_iDetailLevel );
typedef int (*TFctTrcAdminObj_getTraceDetailLevel)( const DllIf::CTrcAdminObj* i_pTrcAdminObj );
typedef bool (*TFctTrcAdminObj_isActive)( const DllIf::CTrcAdminObj* i_pTrcAdminObj, int i_iDetailLevel );
typedef void (*TFctTrcAdminObj_traceMethodEnter)( const DllIf::CTrcAdminObj* i_pTrcAdminObj, const char* i_szObjName, const char* i_szMethod, const char* i_szMethodInArgs );
typedef void (*TFctTrcAdminObj_traceMethodLeave)( const DllIf::CTrcAdminObj* i_pTrcAdminObj, const char* i_szObjName, const char* i_szMethod, const char* i_szMethodReturn, const char* i_szMethodOutArgs );
typedef void (*TFctTrcAdminObj_traceMethod)( const DllIf::CTrcAdminObj* i_pTrcAdminObj, const char* i_szObjName, const char* i_szMethod, const char* i_szMethodAddInfo );

typedef DllIf::CTrcAdminObj* (*TFctTrcServer_GetTraceAdminObj)( const char* i_szServerName, const char* i_szNameSpace, const char* i_szClassName, const char* i_szObjName, DllIf::EEnabled i_bEnabledAsDefault, int i_iDefaultDetailLevel );
typedef void (*TFctTrcServer_ReleaseTraceAdminObj)( const char* i_szServerName, DllIf::CTrcAdminObj* i_pTrcAdminObj );
typedef void (*TFctTrcServer_SetOrganizationName)( const char* i_szName );
typedef char* (*TFctTrcServer_GetOrganizationName)();
typedef void (*TFctTrcServer_SetApplicationName)( const char* i_szName );
typedef char* (*TFctTrcServer_GetApplicationName)();
typedef char* (*TFctTrcServer_GetDefaultAdminObjFileAbsoluteFilePath)( const char* i_szIniFileScope );
typedef char* (*TFctTrcServer_GetDefaultLocalTrcFileAbsoluteFilePath)( const char* i_szIniFileScope );
typedef void (*TFctTrcServer_RegisterCurrentThread)( const char* i_szThreadName );
typedef void (*TFctTrcServer_UnregisterCurrentThread)();
typedef char* (*TFctTrcServer_GetCurrentThreadName)();
typedef bool (*TFctTrcServer_isActive)( const DllIf::CTrcServer* i_pTrcServer );
typedef void (*TFctTrcServer_setEnabled)( DllIf::CTrcServer* i_pTrcServer, bool i_bEnabled );
typedef bool (*TFctTrcServer_isEnabled)( const DllIf::CTrcServer* i_pTrcServer );
typedef void (*TFctTrcServer_setNewTrcAdminObjsEnabledAsDefault)( DllIf::CTrcServer* i_pTrcServer, bool i_bEnabled );
typedef bool (*TFctTrcServer_areNewTrcAdminObjsEnabledAsDefault)( const DllIf::CTrcServer* i_pTrcServer );
typedef void (*TFctTrcServer_setNewTrcAdminObjsDefaultDetailLevel)( DllIf::CTrcServer* i_pTrcServer, int i_iDetailLevel );
typedef int (*TFctTrcServer_getNewTrcAdminObjsDefaultDetailLevel)( const DllIf::CTrcServer* i_pTrcServer );
typedef void (*TFctTrcServer_setAdminObjFileAbsoluteFilePath)( DllIf::CTrcServer* i_pTrcServer, const char* i_szAbsFilePath );
typedef char* (*TFctTrcServer_getAdminObjFileAbsoluteFilePath)( const DllIf::CTrcServer* i_pTrcServer );
typedef bool (*TFctTrcServer_recallAdminObjs)( DllIf::CTrcServer* i_pTrcServer );
typedef bool (*TFctTrcServer_saveAdminObjs)( DllIf::CTrcServer* i_pTrcServer );
typedef void (*TFctTrcServer_setUseLocalTrcFile)( DllIf::CTrcServer* i_pTrcServer, bool i_bUse );
typedef bool (*TFctTrcServer_isLocalTrcFileUsed)( const DllIf::CTrcServer* i_pTrcServer );
typedef void (*TFctTrcServer_setLocalTrcFileAbsoluteFilePath)( DllIf::CTrcServer* i_pTrcServer, const char* i_szAbsFilePath );
typedef char* (*TFctTrcServer_getLocalTrcFileAbsoluteFilePath)( const DllIf::CTrcServer* i_pTrcServer );
typedef char* (*TFctTrcServer_getLocalTrcFileCompleteBaseName)( const DllIf::CTrcServer* i_pTrcServer );
typedef char* (*TFctTrcServer_getLocalTrcFileAbsolutePath)( const DllIf::CTrcServer* i_pTrcServer );
typedef bool (*TFctTrcServer_isLocalTrcFileActive)( const DllIf::CTrcServer* i_pTrcServer );
typedef void (*TFctTrcServer_setLocalTrcFileAutoSaveInterval)( DllIf::CTrcServer* i_pTrcServer, int i_iAutoSaveInterval_ms );
typedef int (*TFctTrcServer_getLocalTrcFileAutoSaveInterval)( const DllIf::CTrcServer* i_pTrcServer );
typedef void (*TFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite)( DllIf::CTrcServer* i_pTrcServer, bool i_bCloseFile );
typedef bool (*TFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite)( const DllIf::CTrcServer* i_pTrcServer );
typedef void (*TFctTrcServer_setLocalTrcFileSubFileCountMax)( DllIf::CTrcServer* i_pTrcServer, int i_iCountMax );
typedef int (*TFctTrcServer_getLocalTrcFileSubFileCountMax)( const DllIf::CTrcServer* i_pTrcServer );
typedef void (*TFctTrcServer_setLocalTrcFileSubFileLineCountMax)( DllIf::CTrcServer* i_pTrcServer, int i_iCountMax );
typedef int (*TFctTrcServer_getLocalTrcFileSubFileLineCountMax)( const DllIf::CTrcServer* i_pTrcServer );
typedef void (*TFctTrcServer_setCacheTrcDataIfNotConnected)( DllIf::CTrcServer* i_pTrcServer, bool i_bCacheData );
typedef bool (*TFctTrcServer_getCacheTrcDataIfNotConnected)( const DllIf::CTrcServer* i_pTrcServer );
typedef void (*TFctTrcServer_setCacheTrcDataMaxArrLen)( DllIf::CTrcServer* i_pTrcServer, int i_iMaxArrLen );
typedef int (*TFctTrcServer_getCacheTrcDataMaxArrLen)( const DllIf::CTrcServer* i_pTrcServer );
typedef bool (*TFctTrcServer_setTraceSettings)( DllIf::CTrcServer* i_pTrcServer, const DllIf::STrcServerSettings& i_settings );
typedef DllIf::STrcServerSettings (*TFctTrcServer_getTraceSettings)( const DllIf::CTrcServer* i_pTrcServer );
typedef void (*TFctTrcServer_clearLocalTrcFile)( DllIf::CTrcServer* i_pTrcServer );

typedef DllIf::CIpcTrcServer* (*TFctIpcTrcServer_GetInstance)( const char* i_szName );
typedef DllIf::CIpcTrcServer* (*TFctIpcTrcServer_CreateInstance)( const char* i_szName, int i_iTrcDetailLevel );
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

static char* s_szTrcDllFileName = nullptr;

#ifdef _WIN32
static HMODULE s_hndIpcTrcDllIf = NULL;
#else
static void* s_hndIpcTrcDllIf = NULL;
#endif

TFctTrcAdminObj_getNameSpace                         s_pFctTrcAdminObj_getNameSpace                         = NULL;
TFctTrcAdminObj_getClassName                         s_pFctTrcAdminObj_getClassName                         = NULL;
TFctTrcAdminObj_getObjectName                        s_pFctTrcAdminObj_getObjectName                        = NULL;
TFctTrcAdminObj_setObjectThreadName                  s_pFctTrcAdminObj_setObjectThreadName                  = NULL;
TFctTrcAdminObj_getObjectThreadName                  s_pFctTrcAdminObj_getObjectThreadName                  = NULL;
TFctTrcAdminObj_setEnabled                           s_pFctTrcAdminObj_setEnabled                           = NULL;
TFctTrcAdminObj_isEnabled                            s_pFctTrcAdminObj_isEnabled                            = NULL;
TFctTrcAdminObj_setTraceDetailLevel                  s_pFctTrcAdminObj_setTraceDetailLevel                  = NULL;
TFctTrcAdminObj_getTraceDetailLevel                  s_pFctTrcAdminObj_getTraceDetailLevel                  = NULL;
TFctTrcAdminObj_isActive                             s_pFctTrcAdminObj_isActive                             = NULL;
TFctTrcAdminObj_traceMethodEnter                     s_pFctTrcAdminObj_traceMethodEnter                     = NULL;
TFctTrcAdminObj_traceMethodLeave                     s_pFctTrcAdminObj_traceMethodLeave                     = NULL;
TFctTrcAdminObj_traceMethod                          s_pFctTrcAdminObj_traceMethod                          = NULL;

TFctTrcServer_GetTraceAdminObj                       s_pFctTrcServer_GetTraceAdminObj                       = NULL;
TFctTrcServer_ReleaseTraceAdminObj                   s_pFctTrcServer_ReleaseTraceAdminObj                   = NULL;
TFctTrcServer_SetOrganizationName                    s_pFctTrcServer_SetOrganizationName                    = NULL;
TFctTrcServer_GetOrganizationName                    s_pFctTrcServer_GetOrganizationName                    = NULL;
TFctTrcServer_SetApplicationName                     s_pFctTrcServer_SetApplicationName                     = NULL;
TFctTrcServer_GetApplicationName                     s_pFctTrcServer_GetApplicationName                     = NULL;
TFctTrcServer_GetDefaultAdminObjFileAbsoluteFilePath s_pFctTrcServer_GetDefaultAdminObjFileAbsoluteFilePath = NULL;
TFctTrcServer_GetDefaultLocalTrcFileAbsoluteFilePath s_pFctTrcServer_GetDefaultLocalTrcFileAbsoluteFilePath = NULL;
TFctTrcServer_RegisterCurrentThread                  s_pFctTrcServer_RegisterCurrentThread                  = NULL;
TFctTrcServer_UnregisterCurrentThread                s_pFctTrcServer_UnregisterCurrentThread                = NULL;
TFctTrcServer_GetCurrentThreadName                   s_pFctTrcServer_GetCurrentThreadName                   = NULL;
TFctTrcServer_isActive                               s_pFctTrcServer_isActive                               = NULL;
TFctTrcServer_setEnabled                             s_pFctTrcServer_setEnabled                             = NULL;
TFctTrcServer_isEnabled                              s_pFctTrcServer_isEnabled                              = NULL;
TFctTrcServer_setNewTrcAdminObjsEnabledAsDefault     s_pFctTrcServer_setNewTrcAdminObjsEnabledAsDefault     = NULL;
TFctTrcServer_areNewTrcAdminObjsEnabledAsDefault     s_pFctTrcServer_areNewTrcAdminObjsEnabledAsDefault     = NULL;
TFctTrcServer_setNewTrcAdminObjsDefaultDetailLevel   s_pFctTrcServer_setNewTrcAdminObjsDefaultDetailLevel   = NULL;
TFctTrcServer_getNewTrcAdminObjsDefaultDetailLevel   s_pFctTrcServer_getNewTrcAdminObjsDefaultDetailLevel   = NULL;
TFctTrcServer_setAdminObjFileAbsoluteFilePath        s_pFctTrcServer_setAdminObjFileAbsoluteFilePath        = NULL;
TFctTrcServer_getAdminObjFileAbsoluteFilePath        s_pFctTrcServer_getAdminObjFileAbsoluteFilePath        = NULL;
TFctTrcServer_recallAdminObjs                        s_pFctTrcServer_recallAdminObjs                        = NULL;
TFctTrcServer_saveAdminObjs                          s_pFctTrcServer_saveAdminObjs                          = NULL;
TFctTrcServer_setUseLocalTrcFile                     s_pFctTrcServer_setUseLocalTrcFile                     = NULL;
TFctTrcServer_isLocalTrcFileUsed                     s_pFctTrcServer_isLocalTrcFileUsed                     = NULL;
TFctTrcServer_setLocalTrcFileAbsoluteFilePath        s_pFctTrcServer_setLocalTrcFileAbsoluteFilePath        = NULL;
TFctTrcServer_getLocalTrcFileAbsoluteFilePath        s_pFctTrcServer_getLocalTrcFileAbsoluteFilePath        = NULL;
TFctTrcServer_getLocalTrcFileCompleteBaseName        s_pFctTrcServer_getLocalTrcFileCompleteBaseName        = NULL;
TFctTrcServer_getLocalTrcFileAbsolutePath            s_pFctTrcServer_getLocalTrcFileAbsolutePath            = NULL;
TFctTrcServer_isLocalTrcFileActive                   s_pFctTrcServer_isLocalTrcFileActive                   = NULL;
TFctTrcServer_setLocalTrcFileAutoSaveInterval        s_pFctTrcServer_setLocalTrcFileAutoSaveInterval        = NULL;
TFctTrcServer_getLocalTrcFileAutoSaveInterval        s_pFctTrcServer_getLocalTrcFileAutoSaveInterval        = NULL;
TFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite s_pFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite = NULL;
TFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite s_pFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite = NULL;
TFctTrcServer_setLocalTrcFileSubFileCountMax         s_pFctTrcServer_setLocalTrcFileSubFileCountMax         = NULL;
TFctTrcServer_getLocalTrcFileSubFileCountMax         s_pFctTrcServer_getLocalTrcFileSubFileCountMax         = NULL;
TFctTrcServer_setLocalTrcFileSubFileLineCountMax     s_pFctTrcServer_setLocalTrcFileSubFileLineCountMax     = NULL;
TFctTrcServer_getLocalTrcFileSubFileLineCountMax     s_pFctTrcServer_getLocalTrcFileSubFileLineCountMax     = NULL;
TFctTrcServer_setCacheTrcDataIfNotConnected          s_pFctTrcServer_setCacheTrcDataIfNotConnected          = NULL;
TFctTrcServer_getCacheTrcDataIfNotConnected          s_pFctTrcServer_getCacheTrcDataIfNotConnected          = NULL;
TFctTrcServer_setCacheTrcDataMaxArrLen               s_pFctTrcServer_setCacheTrcDataMaxArrLen               = NULL;
TFctTrcServer_getCacheTrcDataMaxArrLen               s_pFctTrcServer_getCacheTrcDataMaxArrLen               = NULL;
TFctTrcServer_setTraceSettings                       s_pFctTrcServer_setTraceSettings                       = NULL;
TFctTrcServer_getTraceSettings                       s_pFctTrcServer_getTraceSettings                       = NULL;
TFctTrcServer_clearLocalTrcFile                      s_pFctTrcServer_clearLocalTrcFile                      = NULL;

TFctIpcTrcServer_CreateInstance                      s_pFctIpcTrcServer_CreateInstance                      = NULL;
TFctIpcTrcServer_GetInstance                         s_pFctIpcTrcServer_GetInstance                         = NULL;
TFctIpcTrcServer_ReleaseInstance                     s_pFctIpcTrcServer_ReleaseInstance                     = NULL;
TFctIpcTrcServer_startup                             s_pFctIpcTrcServer_startup                             = NULL;
TFctIpcTrcServer_shutdown                            s_pFctIpcTrcServer_shutdown                            = NULL;
TFctIpcTrcServer_isListening                         s_pFctIpcTrcServer_isListening                         = NULL;
TFctIpcTrcServer_isConnected                         s_pFctIpcTrcServer_isConnected                         = NULL;
TFctIpcTrcServer_setPort                             s_pFctIpcTrcServer_setPort                             = NULL;
TFctIpcTrcServer_getPort                             s_pFctIpcTrcServer_getPort                             = NULL;


/*==============================================================================
Exported methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Loads the Remote Method Tracing Dlls.

    @param i_szCompiler [in] Spezifies the used compiler.
           Default: nullptr
           For Visual Studio Compilers this parameter is automatically detected
           and the macro COMPILERLIBINFIX is set via "ZSIpcTrcDllIf.h" to e.g.
           "msvc2013" by evaluating the preprozessor define _MSC_VER.
           If nullptr is passed  the automatically detected COMPILERLIBINFIX is used.
           This parameter must correspond to the compilier lib-infix
           as used when compiling and linking the ZSQtLib-Dlls and may also
           be an empty string e.g. on linux machines.

    @param i_szPlatform [in] Spezifies the used platform.
           Default: nullptr
           For Visual Studio Compilers this parameter is automatically detected
           and the macro PLATFORMLIBINFIX is set via "ZSIpcTrcDllIf.h" to e.g.
           "x64" or "Win32" by evaluating the preprozessor define _WIN64.
           If nullptr is passed  the automatically detected PLATFORMLIBINFIX is used.
           The parameter must correspond to the platform lib-infix
           as used when compiling and linking the ZSQtLib-Dlls and may also
           be an empty string e.g. on linux machines.

    @param i_configuration [in] Spezifies the build configuration.
           Default: EBuildConfigurationAutoDetect
           The parameter defines the lib infix for the ZSQtLib Dlls used by
           the build configuration which may be either a Debug or Release Build.
           Usually the lib infix for the build configuration can be automatically
           detected by evaluationg the compiler directive _DEBUG and the
           the macro CONFIGLIBINFIX is set via "ZSIpcTrcDllIf.h" to e.g.
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

    @note Moegliche Ursachen dafuer, dass die Dlls nicht geladen werden konnten, sind:
          - Dlls wurden nicht gefunden
            - Ggf. PATH erweitern, so dass das Betriebssystem die Dlls finden koennen.
            - Dateinamen sind nicht korrekt (alle Lib-Infixes pruefen).
          - Incompatible Versionen (x64/Win32, Qt Version 4 statt 5, etc.).
          - Qt Dlls konnten nicht geladen werden
            - Wurden die Qt Dlls ggf. mit einem LibInfix compiliert?
            - Kann das Betriebssystem die Qt-Dlls finden?
*/
bool ZS::Trace::DllIf::loadDll(
    const char* i_szCompiler,
    const char* i_szPlatform,
    EBuildConfiguration i_configuration,
    int i_iQtVersionMajor )
//------------------------------------------------------------------------------
{
    const char* szCompiler = COMPILERLIBINFIX;

    if( i_szCompiler != nullptr )
    {
        szCompiler = i_szCompiler;
    }

    const char* szPlatform = PLATFORMLIBINFIX;

    if( i_szPlatform != nullptr )
    {
        szPlatform = i_szPlatform;
    }

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
     *  "ZSIpcTraceQt5_msvc2015_x64_d"
     *  "ZSIpcTraceQt5_mingw81_x64_d"
     *  "libZSIpcTraceQt5_mingw81_x64_d"
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

        delete s_szTrcDllFileName;
        s_szTrcDllFileName = nullptr;

        size_t iStrLenDllFileName = strlen(szZSDllName) + strlen(szQtVersionMajor) + 1 + strlen(szCompiler) + 1 + strlen(szPlatform) + 1 + strlen(szConfig) + 4;
        s_szTrcDllFileName = new char[iStrLenDllFileName+1];
        memset(s_szTrcDllFileName, 0x00, iStrLenDllFileName+1);

        size_t iStrPos = 0;
        memcpy(&s_szTrcDllFileName[iStrPos], szZSDllName, strlen(szZSDllName));           // "ZSIpcTraceQt"
        iStrPos += strlen(szZSDllName);
        memcpy(&s_szTrcDllFileName[iStrPos], szQtVersionMajor, strlen(szQtVersionMajor)); // "ZSIpcTraceQt5"
        iStrPos += strlen(szQtVersionMajor);
        memcpy(&s_szTrcDllFileName[iStrPos], "_", 1);                                     // "ZSIpcTraceQt5_"
        iStrPos += 1;
        memcpy(&s_szTrcDllFileName[iStrPos], szCompiler, strlen(szCompiler));             // "ZSIpcTraceQt5_msvc2015"
        iStrPos += strlen(szCompiler);
        memcpy(&s_szTrcDllFileName[iStrPos], "_", 1);                                     // "ZSIpcTraceQt5_msvc2015_"
        iStrPos += 1;
        memcpy(&s_szTrcDllFileName[iStrPos], szPlatform, strlen(szPlatform));             // "ZSIpcTraceQt5_msvc2015_x64"
        iStrPos += strlen(szPlatform);
        if( strlen(szConfig) > 0 )
        {
            memcpy(&s_szTrcDllFileName[iStrPos], "_", 1);                                 // "ZSIpcTraceQt5_msvc2015_x64_"
            iStrPos += 1;
            memcpy(&s_szTrcDllFileName[iStrPos], szConfig, strlen(szConfig));             // "ZSIpcTraceQt5_msvc2015_x64_d"
            iStrPos += strlen(szConfig);
        }

        #ifdef _WIN32
        const wstring wstrTrcDllFileName = ZS::System::s2ws(s_szTrcDllFileName);
        s_hndIpcTrcDllIf = LoadLibrary(wstrTrcDllFileName.c_str());
        #else
        memcpy(&s_szTrcDllFileName[iStrPos], ".so", 3);                                 // "ZSIpcTraceQt5_msvc2015_x64_d.so"
        s_hndIpcTrcDllIf = dlopen(s_szTrcDllFileName, RTLD_LAZY);
        if( s_hndIpcTrcDllIf == NULL )
        {
            // Helps to find the reason in debugger why shared library could not be loaded.
            if( szError == nullptr ) szError = new char[10000];
            memset(szError, 0x00, 10000);
            sprintf(szError, "%s", dlerror());
        }
        #endif

        if( s_hndIpcTrcDllIf != NULL )
        {
            break;
        }
    } // for( int iFileNameTries = 0; iFileNameTries < 2; ++iFileNameTries )

    delete [] szError;
    szError = nullptr;

    delete [] szQtVersionMajor;
    szQtVersionMajor = nullptr;

    bool bOk = false;

    if( s_hndIpcTrcDllIf != NULL )
    {
        bOk = true;

        // TrcAdminObj
        //------------

        s_pFctTrcAdminObj_getNameSpace = (TFctTrcAdminObj_getNameSpace)GetProcAddress(s_hndIpcTrcDllIf, "TrcAdminObj_getNameSpace");
        if( s_pFctTrcAdminObj_getNameSpace == NULL ) bOk = false;

        s_pFctTrcAdminObj_getClassName = (TFctTrcAdminObj_getClassName)GetProcAddress(s_hndIpcTrcDllIf, "TrcAdminObj_getClassName");
        if( s_pFctTrcAdminObj_getClassName == NULL ) bOk = false;

        s_pFctTrcAdminObj_getObjectName = (TFctTrcAdminObj_getObjectName)GetProcAddress(s_hndIpcTrcDllIf, "TrcAdminObj_getObjectName");
        if( s_pFctTrcAdminObj_getObjectName == NULL ) bOk = false;

        s_pFctTrcAdminObj_setObjectThreadName = (TFctTrcAdminObj_setObjectThreadName)GetProcAddress(s_hndIpcTrcDllIf, "TrcAdminObj_setObjectThreadName");
        if( s_pFctTrcAdminObj_setObjectThreadName == NULL ) bOk = false;

        s_pFctTrcAdminObj_getObjectThreadName = (TFctTrcAdminObj_getObjectThreadName)GetProcAddress(s_hndIpcTrcDllIf, "TrcAdminObj_getObjectThreadName");
        if( s_pFctTrcAdminObj_getObjectThreadName == NULL ) bOk = false;

        s_pFctTrcAdminObj_setEnabled = (TFctTrcAdminObj_setEnabled)GetProcAddress(s_hndIpcTrcDllIf, "TrcAdminObj_setEnabled");
        if( s_pFctTrcAdminObj_setEnabled == NULL ) bOk = false;

        s_pFctTrcAdminObj_isEnabled = (TFctTrcAdminObj_isEnabled)GetProcAddress(s_hndIpcTrcDllIf, "TrcAdminObj_isEnabled");
        if( s_pFctTrcAdminObj_isEnabled == NULL ) bOk = false;

        s_pFctTrcAdminObj_setTraceDetailLevel = (TFctTrcAdminObj_setTraceDetailLevel)GetProcAddress(s_hndIpcTrcDllIf, "TrcAdminObj_setTraceDetailLevel");
        if( s_pFctTrcAdminObj_setTraceDetailLevel == NULL ) bOk = false;

        s_pFctTrcAdminObj_getTraceDetailLevel = (TFctTrcAdminObj_getTraceDetailLevel)GetProcAddress(s_hndIpcTrcDllIf, "TrcAdminObj_getTraceDetailLevel");
        if( s_pFctTrcAdminObj_getTraceDetailLevel == NULL ) bOk = false;

        s_pFctTrcAdminObj_isActive = (TFctTrcAdminObj_isActive)GetProcAddress(s_hndIpcTrcDllIf, "TrcAdminObj_isActive");
        if( s_pFctTrcAdminObj_isActive == NULL ) bOk = false;

        s_pFctTrcAdminObj_traceMethodEnter = (TFctTrcAdminObj_traceMethodEnter)GetProcAddress(s_hndIpcTrcDllIf, "TrcAdminObj_traceMethodEnter");
        if( s_pFctTrcAdminObj_traceMethodEnter == NULL ) bOk = false;

        s_pFctTrcAdminObj_traceMethodLeave = (TFctTrcAdminObj_traceMethodLeave)GetProcAddress(s_hndIpcTrcDllIf, "TrcAdminObj_traceMethodLeave");
        if( s_pFctTrcAdminObj_traceMethodLeave == NULL ) bOk = false;

        s_pFctTrcAdminObj_traceMethod = (TFctTrcAdminObj_traceMethod)GetProcAddress(s_hndIpcTrcDllIf, "TrcAdminObj_traceMethod");
        if( s_pFctTrcAdminObj_traceMethod == NULL ) bOk = false;

        // TrcServer
        //------------

        s_pFctTrcServer_GetTraceAdminObj = (TFctTrcServer_GetTraceAdminObj)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_GetTraceAdminObj");
        if( s_pFctTrcServer_GetTraceAdminObj == NULL ) bOk = false;

        s_pFctTrcServer_ReleaseTraceAdminObj = (TFctTrcServer_ReleaseTraceAdminObj)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_ReleaseTraceAdminObj");
        if( s_pFctTrcServer_ReleaseTraceAdminObj == NULL ) bOk = false;

        s_pFctTrcServer_SetOrganizationName = (TFctTrcServer_SetOrganizationName)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_SetOrganizationName");
        if( s_pFctTrcServer_SetOrganizationName == NULL ) bOk = false;

        s_pFctTrcServer_GetOrganizationName = (TFctTrcServer_GetOrganizationName)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_GetOrganizationName");
        if( s_pFctTrcServer_GetOrganizationName == NULL ) bOk = false;

        s_pFctTrcServer_SetApplicationName = (TFctTrcServer_SetApplicationName)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_SetApplicationName");
        if( s_pFctTrcServer_SetApplicationName == NULL ) bOk = false;

        s_pFctTrcServer_GetApplicationName = (TFctTrcServer_GetApplicationName)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_GetApplicationName");
        if( s_pFctTrcServer_GetApplicationName == NULL ) bOk = false;

        s_pFctTrcServer_GetDefaultAdminObjFileAbsoluteFilePath = (TFctTrcServer_GetDefaultAdminObjFileAbsoluteFilePath)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_GetDefaultAdminObjFileAbsoluteFilePath");
        if( s_pFctTrcServer_GetDefaultAdminObjFileAbsoluteFilePath == NULL ) bOk = false;

        s_pFctTrcServer_GetDefaultLocalTrcFileAbsoluteFilePath = (TFctTrcServer_GetDefaultLocalTrcFileAbsoluteFilePath)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_GetDefaultLocalTrcFileAbsoluteFilePath");
        if( s_pFctTrcServer_GetDefaultLocalTrcFileAbsoluteFilePath == NULL ) bOk = false;

        s_pFctTrcServer_RegisterCurrentThread = (TFctTrcServer_RegisterCurrentThread)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_RegisterCurrentThread");
        if( s_pFctTrcServer_RegisterCurrentThread == NULL ) bOk = false;

        s_pFctTrcServer_UnregisterCurrentThread = (TFctTrcServer_UnregisterCurrentThread)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_UnregisterCurrentThread");
        if( s_pFctTrcServer_UnregisterCurrentThread == NULL ) bOk = false;

        s_pFctTrcServer_GetCurrentThreadName = (TFctTrcServer_GetCurrentThreadName)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_GetCurrentThreadName");
        if( s_pFctTrcServer_GetCurrentThreadName == NULL ) bOk = false;

        s_pFctTrcServer_isActive = (TFctTrcServer_isActive)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_isActive");
        if( s_pFctTrcServer_isActive == NULL ) bOk = false;

        s_pFctTrcServer_setEnabled = (TFctTrcServer_setEnabled)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_setEnabled");
        if( s_pFctTrcServer_setEnabled == NULL ) bOk = false;

        s_pFctTrcServer_isEnabled = (TFctTrcServer_isEnabled)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_isEnabled");
        if( s_pFctTrcServer_isEnabled == NULL ) bOk = false;

        s_pFctTrcServer_setNewTrcAdminObjsEnabledAsDefault = (TFctTrcServer_setNewTrcAdminObjsEnabledAsDefault)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_setNewTrcAdminObjsEnabledAsDefault");
        if( s_pFctTrcServer_setNewTrcAdminObjsEnabledAsDefault == NULL ) bOk = false;

        s_pFctTrcServer_areNewTrcAdminObjsEnabledAsDefault = (TFctTrcServer_areNewTrcAdminObjsEnabledAsDefault)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_areNewTrcAdminObjsEnabledAsDefault");
        if( s_pFctTrcServer_areNewTrcAdminObjsEnabledAsDefault == NULL ) bOk = false;

        s_pFctTrcServer_setNewTrcAdminObjsDefaultDetailLevel = (TFctTrcServer_setNewTrcAdminObjsDefaultDetailLevel)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_setNewTrcAdminObjsDefaultDetailLevel");
        if( s_pFctTrcServer_setNewTrcAdminObjsDefaultDetailLevel == NULL ) bOk = false;

        s_pFctTrcServer_getNewTrcAdminObjsDefaultDetailLevel = (TFctTrcServer_getNewTrcAdminObjsDefaultDetailLevel)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_getNewTrcAdminObjsDefaultDetailLevel");
        if( s_pFctTrcServer_getNewTrcAdminObjsDefaultDetailLevel == NULL ) bOk = false;

        s_pFctTrcServer_setAdminObjFileAbsoluteFilePath = (TFctTrcServer_setAdminObjFileAbsoluteFilePath)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_setAdminObjFileAbsoluteFilePath");
        if( s_pFctTrcServer_setAdminObjFileAbsoluteFilePath == NULL ) bOk = false;

        s_pFctTrcServer_getAdminObjFileAbsoluteFilePath = (TFctTrcServer_getAdminObjFileAbsoluteFilePath)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_getAdminObjFileAbsoluteFilePath");
        if( s_pFctTrcServer_getAdminObjFileAbsoluteFilePath == NULL ) bOk = false;

        s_pFctTrcServer_recallAdminObjs = (TFctTrcServer_recallAdminObjs)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_recallAdminObjs");
        if( s_pFctTrcServer_recallAdminObjs == NULL ) bOk = false;

        s_pFctTrcServer_saveAdminObjs = (TFctTrcServer_saveAdminObjs)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_saveAdminObjs");
        if( s_pFctTrcServer_saveAdminObjs == NULL ) bOk = false;

        s_pFctTrcServer_setUseLocalTrcFile = (TFctTrcServer_setUseLocalTrcFile)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_setUseLocalTrcFile");
        if( s_pFctTrcServer_setUseLocalTrcFile == NULL ) bOk = false;

        s_pFctTrcServer_isLocalTrcFileUsed = (TFctTrcServer_isLocalTrcFileUsed)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_isLocalTrcFileUsed");
        if( s_pFctTrcServer_isLocalTrcFileUsed == NULL ) bOk = false;

        s_pFctTrcServer_setLocalTrcFileAbsoluteFilePath = (TFctTrcServer_setLocalTrcFileAbsoluteFilePath)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_setLocalTrcFileAbsoluteFilePath");
        if( s_pFctTrcServer_setLocalTrcFileAbsoluteFilePath == NULL ) bOk = false;

        s_pFctTrcServer_getLocalTrcFileAbsoluteFilePath = (TFctTrcServer_getLocalTrcFileAbsoluteFilePath)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_getLocalTrcFileAbsoluteFilePath");
        if( s_pFctTrcServer_getLocalTrcFileAbsoluteFilePath == NULL ) bOk = false;

        s_pFctTrcServer_getLocalTrcFileCompleteBaseName = (TFctTrcServer_getLocalTrcFileCompleteBaseName)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_getLocalTrcFileCompleteBaseName");
        if( s_pFctTrcServer_getLocalTrcFileCompleteBaseName == NULL ) bOk = false;

        s_pFctTrcServer_getLocalTrcFileAbsolutePath = (TFctTrcServer_getLocalTrcFileAbsolutePath)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_getLocalTrcFileAbsolutePath");
        if( s_pFctTrcServer_getLocalTrcFileAbsolutePath == NULL ) bOk = false;

        s_pFctTrcServer_isLocalTrcFileActive = (TFctTrcServer_isLocalTrcFileActive)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_isLocalTrcFileActive");
        if( s_pFctTrcServer_isLocalTrcFileActive == NULL ) bOk = false;

        s_pFctTrcServer_setLocalTrcFileAutoSaveInterval = (TFctTrcServer_setLocalTrcFileAutoSaveInterval)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_setLocalTrcFileAutoSaveInterval");
        if( s_pFctTrcServer_setLocalTrcFileAutoSaveInterval == NULL ) bOk = false;

        s_pFctTrcServer_getLocalTrcFileAutoSaveInterval = (TFctTrcServer_getLocalTrcFileAutoSaveInterval)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_getLocalTrcFileAutoSaveInterval");
        if( s_pFctTrcServer_getLocalTrcFileAutoSaveInterval == NULL ) bOk = false;

        s_pFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite = (TFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_setLocalTrcFileCloseFileAfterEachWrite");
        if( s_pFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite == NULL ) bOk = false;

        s_pFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite = (TFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_getLocalTrcFileCloseFileAfterEachWrite");
        if( s_pFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite == NULL ) bOk = false;

        s_pFctTrcServer_setLocalTrcFileSubFileCountMax = (TFctTrcServer_setLocalTrcFileSubFileCountMax)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_setLocalTrcFileSubFileCountMax");
        if( s_pFctTrcServer_setLocalTrcFileSubFileCountMax == NULL ) bOk = false;

        s_pFctTrcServer_getLocalTrcFileSubFileCountMax = (TFctTrcServer_getLocalTrcFileSubFileCountMax)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_getLocalTrcFileSubFileCountMax");
        if( s_pFctTrcServer_getLocalTrcFileSubFileCountMax == NULL ) bOk = false;

        s_pFctTrcServer_setLocalTrcFileSubFileLineCountMax = (TFctTrcServer_setLocalTrcFileSubFileLineCountMax)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_setLocalTrcFileSubFileLineCountMax");
        if( s_pFctTrcServer_setLocalTrcFileSubFileLineCountMax == NULL ) bOk = false;

        s_pFctTrcServer_getLocalTrcFileSubFileLineCountMax = (TFctTrcServer_getLocalTrcFileSubFileLineCountMax)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_getLocalTrcFileSubFileLineCountMax");
        if( s_pFctTrcServer_getLocalTrcFileSubFileLineCountMax == NULL ) bOk = false;

        s_pFctTrcServer_setCacheTrcDataIfNotConnected = (TFctTrcServer_setCacheTrcDataIfNotConnected)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_setCacheTrcDataIfNotConnected");
        if( s_pFctTrcServer_setCacheTrcDataIfNotConnected == NULL ) bOk = false;

        s_pFctTrcServer_getCacheTrcDataIfNotConnected = (TFctTrcServer_getCacheTrcDataIfNotConnected)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_getCacheTrcDataIfNotConnected");
        if( s_pFctTrcServer_getCacheTrcDataIfNotConnected == NULL ) bOk = false;

        s_pFctTrcServer_setCacheTrcDataMaxArrLen = (TFctTrcServer_setCacheTrcDataMaxArrLen)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_setCacheTrcDataMaxArrLen");
        if( s_pFctTrcServer_setCacheTrcDataMaxArrLen == NULL ) bOk = false;

        s_pFctTrcServer_getCacheTrcDataMaxArrLen = (TFctTrcServer_getCacheTrcDataMaxArrLen)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_getCacheTrcDataMaxArrLen");
        if( s_pFctTrcServer_getCacheTrcDataMaxArrLen == NULL ) bOk = false;

        s_pFctTrcServer_setTraceSettings = (TFctTrcServer_setTraceSettings)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_setTraceSettings");
        if( s_pFctTrcServer_setTraceSettings == NULL ) bOk = false;

        s_pFctTrcServer_getTraceSettings = (TFctTrcServer_getTraceSettings)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_getTraceSettings");
        if( s_pFctTrcServer_getTraceSettings == NULL ) bOk = false;

        s_pFctTrcServer_clearLocalTrcFile = (TFctTrcServer_clearLocalTrcFile)GetProcAddress(s_hndIpcTrcDllIf, "TrcServer_clearLocalTrcFile");
        if( s_pFctTrcServer_clearLocalTrcFile == NULL ) bOk = false;

        // IpcTrcServer
        //-------------

        s_pFctIpcTrcServer_GetInstance = (TFctIpcTrcServer_GetInstance)GetProcAddress(s_hndIpcTrcDllIf, "IpcTrcServer_GetInstance");
        if( s_pFctIpcTrcServer_GetInstance == NULL ) bOk = false;

        s_pFctIpcTrcServer_CreateInstance = (TFctIpcTrcServer_CreateInstance)GetProcAddress(s_hndIpcTrcDllIf, "IpcTrcServer_CreateInstance");
        if( s_pFctIpcTrcServer_CreateInstance == NULL ) bOk = false;

        s_pFctIpcTrcServer_ReleaseInstance = (TFctIpcTrcServer_ReleaseInstance)GetProcAddress(s_hndIpcTrcDllIf, "IpcTrcServer_ReleaseInstance");
        if( s_pFctIpcTrcServer_ReleaseInstance == NULL ) bOk = false;

        s_pFctIpcTrcServer_startup = (TFctIpcTrcServer_startup)GetProcAddress(s_hndIpcTrcDllIf, "IpcTrcServer_startup");
        if( s_pFctIpcTrcServer_startup == NULL ) bOk = false;

        s_pFctIpcTrcServer_shutdown = (TFctIpcTrcServer_shutdown)GetProcAddress(s_hndIpcTrcDllIf, "IpcTrcServer_shutdown");
        if( s_pFctIpcTrcServer_shutdown == NULL ) bOk = false;

        s_pFctIpcTrcServer_isListening = (TFctIpcTrcServer_isListening)GetProcAddress(s_hndIpcTrcDllIf, "IpcTrcServer_isListening");
        if( s_pFctIpcTrcServer_isListening == NULL ) bOk = false;

        s_pFctIpcTrcServer_isConnected = (TFctIpcTrcServer_isConnected)GetProcAddress(s_hndIpcTrcDllIf, "IpcTrcServer_isConnected");
        if( s_pFctIpcTrcServer_isConnected == NULL ) bOk = false;

        s_pFctIpcTrcServer_setPort = (TFctIpcTrcServer_setPort)GetProcAddress(s_hndIpcTrcDllIf, "IpcTrcServer_setPort");
        if( s_pFctIpcTrcServer_setPort == NULL ) bOk = false;

        s_pFctIpcTrcServer_getPort = (TFctIpcTrcServer_getPort)GetProcAddress(s_hndIpcTrcDllIf, "IpcTrcServer_getPort");
        if( s_pFctIpcTrcServer_getPort == NULL ) bOk = false;

    } // if( s_hndIpcTrcDllIf != NULL )

    return bOk;

} // loadDll

//------------------------------------------------------------------------------
/*! Returns the file name of the loaded ZSIpcTrace Dll.

    @return name of the dll.
*/
const char* ZS::Trace::DllIf::getDllFileName()
//------------------------------------------------------------------------------
{
    return s_szTrcDllFileName;
}

//------------------------------------------------------------------------------
/*! Returns Releases the Remote Method Trace Dlls.

    @return true if dll was release.
            false in case of an error (if the dll was not loaded).
*/
bool ZS::Trace::DllIf::releaseDll()
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndIpcTrcDllIf != NULL )
    {
        bOk = true;

        #ifdef _WINDOWS
        FreeLibrary(s_hndIpcTrcDllIf);
        #elif defined __linux__
        dlclose(s_hndIpcTrcDllIf);
        #endif
        s_hndIpcTrcDllIf = NULL;

        s_pFctTrcAdminObj_getNameSpace                         = NULL;
        s_pFctTrcAdminObj_getClassName                         = NULL;
        s_pFctTrcAdminObj_getObjectName                        = NULL;
        s_pFctTrcAdminObj_setObjectThreadName                  = NULL;
        s_pFctTrcAdminObj_getObjectThreadName                  = NULL;
        s_pFctTrcAdminObj_setEnabled                           = NULL;
        s_pFctTrcAdminObj_isEnabled                            = NULL;
        s_pFctTrcAdminObj_setTraceDetailLevel                  = NULL;
        s_pFctTrcAdminObj_isActive                             = NULL;
        s_pFctTrcAdminObj_traceMethodEnter                     = NULL;
        s_pFctTrcAdminObj_traceMethodLeave                     = NULL;
        s_pFctTrcAdminObj_traceMethod                          = NULL;

        s_pFctTrcServer_GetTraceAdminObj                       = NULL;
        s_pFctTrcServer_ReleaseTraceAdminObj                   = NULL;
        s_pFctTrcServer_SetOrganizationName                    = NULL;
        s_pFctTrcServer_GetOrganizationName                    = NULL;
        s_pFctTrcServer_SetApplicationName                     = NULL;
        s_pFctTrcServer_GetApplicationName                     = NULL;
        s_pFctTrcServer_GetDefaultAdminObjFileAbsoluteFilePath = NULL;
        s_pFctTrcServer_GetDefaultLocalTrcFileAbsoluteFilePath = NULL;
        s_pFctTrcServer_RegisterCurrentThread                  = NULL;
        s_pFctTrcServer_UnregisterCurrentThread                = NULL;
        s_pFctTrcServer_GetCurrentThreadName                   = NULL;
        s_pFctTrcServer_isActive                               = NULL;
        s_pFctTrcServer_setEnabled                             = NULL;
        s_pFctTrcServer_isEnabled                              = NULL;
        s_pFctTrcServer_setNewTrcAdminObjsEnabledAsDefault     = NULL;
        s_pFctTrcServer_areNewTrcAdminObjsEnabledAsDefault     = NULL;
        s_pFctTrcServer_setNewTrcAdminObjsDefaultDetailLevel   = NULL;
        s_pFctTrcServer_getNewTrcAdminObjsDefaultDetailLevel   = NULL;
        s_pFctTrcServer_setAdminObjFileAbsoluteFilePath        = NULL;
        s_pFctTrcServer_getAdminObjFileAbsoluteFilePath        = NULL;
        s_pFctTrcServer_recallAdminObjs                        = NULL;
        s_pFctTrcServer_saveAdminObjs                          = NULL;
        s_pFctTrcServer_setUseLocalTrcFile                     = NULL;
        s_pFctTrcServer_isLocalTrcFileUsed                     = NULL;
        s_pFctTrcServer_setLocalTrcFileAbsoluteFilePath        = NULL;
        s_pFctTrcServer_getLocalTrcFileAbsoluteFilePath        = NULL;
        s_pFctTrcServer_getLocalTrcFileCompleteBaseName        = NULL;
        s_pFctTrcServer_getLocalTrcFileAbsolutePath            = NULL;
        s_pFctTrcServer_isLocalTrcFileActive                   = NULL;
        s_pFctTrcServer_setLocalTrcFileAutoSaveInterval        = NULL;
        s_pFctTrcServer_getLocalTrcFileAutoSaveInterval        = NULL;
        s_pFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite = NULL;
        s_pFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite = NULL;
        s_pFctTrcServer_setLocalTrcFileSubFileCountMax         = NULL;
        s_pFctTrcServer_getLocalTrcFileSubFileCountMax         = NULL;
        s_pFctTrcServer_setLocalTrcFileSubFileLineCountMax     = NULL;
        s_pFctTrcServer_getLocalTrcFileSubFileLineCountMax     = NULL;
        s_pFctTrcServer_setCacheTrcDataIfNotConnected          = NULL;
        s_pFctTrcServer_getCacheTrcDataIfNotConnected          = NULL;
        s_pFctTrcServer_setCacheTrcDataMaxArrLen               = NULL;
        s_pFctTrcServer_getCacheTrcDataMaxArrLen               = NULL;
        s_pFctTrcServer_setTraceSettings                       = NULL;
        s_pFctTrcServer_getTraceSettings                       = NULL;
        s_pFctTrcServer_clearLocalTrcFile                      = NULL;

        s_pFctIpcTrcServer_GetInstance                         = NULL;
        s_pFctIpcTrcServer_CreateInstance                      = NULL;
        s_pFctIpcTrcServer_ReleaseInstance                     = NULL;
        s_pFctIpcTrcServer_startup                             = NULL;
        s_pFctIpcTrcServer_shutdown                            = NULL;
        s_pFctIpcTrcServer_isListening                         = NULL;
        s_pFctIpcTrcServer_isConnected                         = NULL;
        s_pFctIpcTrcServer_setPort                             = NULL;
        s_pFctIpcTrcServer_getPort                             = NULL;
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
char* DllIf::CTrcAdminObj::getNameSpace() const
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcAdminObj_getNameSpace != NULL )
    {
        return s_pFctTrcAdminObj_getNameSpace(this);
    }
    return NULL;

} // getNameSpace

//------------------------------------------------------------------------------
char* DllIf::CTrcAdminObj::getClassName() const
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcAdminObj_getClassName != NULL )
    {
        return s_pFctTrcAdminObj_getClassName(this);
    }
    return NULL;

} // getClassName

//------------------------------------------------------------------------------
char* DllIf::CTrcAdminObj::getObjectName() const
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcAdminObj_getObjectName != NULL )
    {
        return s_pFctTrcAdminObj_getObjectName(this);
    }
    return NULL;

} // getObjectName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CTrcAdminObj::setObjectThreadName( const char* i_szThreadName )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcAdminObj_setObjectThreadName != NULL )
    {
        s_pFctTrcAdminObj_setObjectThreadName(this, i_szThreadName);
    }
} // setObjectThreadName

//------------------------------------------------------------------------------
char* DllIf::CTrcAdminObj::getObjectThreadName() const
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcAdminObj_getObjectThreadName != NULL )
    {
        return s_pFctTrcAdminObj_getObjectThreadName(this);
    }
    return NULL;

} // getObjectThreadName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CTrcAdminObj::setEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcAdminObj_setEnabled != NULL )
    {
        s_pFctTrcAdminObj_setEnabled(this, i_bEnabled);
    }
} // setEnabled

//------------------------------------------------------------------------------
bool DllIf::CTrcAdminObj::isEnabled() const
//------------------------------------------------------------------------------
{
    bool bEnabled = false;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcAdminObj_isEnabled != NULL )
    {
        bEnabled = s_pFctTrcAdminObj_isEnabled(this);
    }
    return bEnabled;

} // getEnabled

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CTrcAdminObj::setTraceDetailLevel( int i_iDetailLevel )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcAdminObj_setTraceDetailLevel != NULL )
    {
        s_pFctTrcAdminObj_setTraceDetailLevel(this, i_iDetailLevel);
    }
} // setTraceDetailLevel

//------------------------------------------------------------------------------
int DllIf::CTrcAdminObj::getTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    int iDetailLevel = ETraceDetailLevelNone;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcAdminObj_getTraceDetailLevel != NULL )
    {
        iDetailLevel = s_pFctTrcAdminObj_getTraceDetailLevel(this);
    }
    return iDetailLevel;

} // getTraceDetailLevel

//------------------------------------------------------------------------------
bool DllIf::CTrcAdminObj::isActive( int i_iDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcAdminObj_isActive != NULL )
    {
        bIsActive = s_pFctTrcAdminObj_isActive(this, i_iDetailLevel);
    }
    return bIsActive;

} // isActive


/*******************************************************************************
class CMethodTracer
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
DllIf::CMethodTracer::CMethodTracer(
    CTrcAdminObj* i_pTrcAdminObj,
    int           i_iFilterDetailLevel,
    const char*   i_szMethod,
    const char*   i_szMethodInArgs ) :
//------------------------------------------------------------------------------
    m_pTrcAdminObj(i_pTrcAdminObj),
    m_iEnterLeaveFilterDetailLevel(i_iFilterDetailLevel),
    m_szObjName(NULL),
    m_szMethod(NULL),
    m_szMethodReturn(NULL),
    m_szMethodOutArgs(NULL)
{
    size_t iStrLen = strlen(i_szMethod);
    m_szMethod = new char[iStrLen+1];
    memset(m_szMethod, 0x00, iStrLen+1);
    memcpy(m_szMethod, i_szMethod, iStrLen);

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcAdminObj_isActive != NULL && s_pFctTrcAdminObj_traceMethodEnter != NULL )
    {
        if( m_pTrcAdminObj != NULL )
        {
            if( s_pFctTrcAdminObj_isActive(m_pTrcAdminObj, m_iEnterLeaveFilterDetailLevel) )
            {
                s_pFctTrcAdminObj_traceMethodEnter(m_pTrcAdminObj, m_szObjName, m_szMethod, i_szMethodInArgs);
            }
        }
    }
} // ctor

//------------------------------------------------------------------------------
DllIf::CMethodTracer::CMethodTracer(
    CTrcAdminObj* i_pTrcAdminObj,
    int           i_iFilterDetailLevel,
    const char*   i_szObjName,
    const char*   i_szMethod,
    const char*   i_szMethodInArgs ) :
//------------------------------------------------------------------------------
    m_pTrcAdminObj(i_pTrcAdminObj),
    m_iEnterLeaveFilterDetailLevel(i_iFilterDetailLevel),
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

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcAdminObj_isActive != NULL && s_pFctTrcAdminObj_traceMethodEnter != NULL )
    {
        if( m_pTrcAdminObj != NULL )
        {
            if( s_pFctTrcAdminObj_isActive(m_pTrcAdminObj, m_iEnterLeaveFilterDetailLevel) )
            {
                s_pFctTrcAdminObj_traceMethodEnter(m_pTrcAdminObj, m_szObjName, m_szMethod, i_szMethodInArgs);
            }
        }
    }
} // ctor

//------------------------------------------------------------------------------
DllIf::CMethodTracer::~CMethodTracer()
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcAdminObj_isActive != NULL && s_pFctTrcAdminObj_traceMethodLeave != NULL )
    {
        if( m_pTrcAdminObj != NULL )
        {
            if( s_pFctTrcAdminObj_isActive(m_pTrcAdminObj, m_iEnterLeaveFilterDetailLevel) )
            {
                s_pFctTrcAdminObj_traceMethodLeave(m_pTrcAdminObj, m_szObjName, m_szMethod, m_szMethodReturn, m_szMethodOutArgs);
            }
        }
    }

    delete m_szObjName;
    delete m_szMethod;
    delete m_szMethodReturn;
    delete m_szMethodOutArgs;

    m_pTrcAdminObj = NULL;
    m_iEnterLeaveFilterDetailLevel = 0;
    m_szObjName = NULL;
    m_szMethod = NULL;
    m_szMethodReturn = NULL;
    m_szMethodOutArgs = NULL;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CMethodTracer::onAdminObjAboutToBeReleased( bool i_bTraceMethodLeave )
//------------------------------------------------------------------------------
{
    if( m_pTrcAdminObj != NULL && s_pFctTrcAdminObj_isActive != NULL && s_pFctTrcAdminObj_traceMethodLeave != NULL )
    {
        if( i_bTraceMethodLeave && s_pFctTrcAdminObj_isActive(m_pTrcAdminObj, m_iEnterLeaveFilterDetailLevel) )
        {
            s_pFctTrcAdminObj_traceMethodLeave(m_pTrcAdminObj, m_szObjName, m_szMethod, m_szMethodReturn, m_szMethodOutArgs);
        }
        m_pTrcAdminObj = NULL;
    }
} // onAdminObjAboutToBeReleased

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int DllIf::CMethodTracer::getTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    int iDetailLevel = ETraceDetailLevelNone;

    if( m_pTrcAdminObj != NULL && s_pFctTrcAdminObj_getTraceDetailLevel != NULL )
    {
        iDetailLevel = s_pFctTrcAdminObj_getTraceDetailLevel(m_pTrcAdminObj);
    }
    return iDetailLevel;

} // getTraceDetailLevel

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
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

} // setMethodReturn

//------------------------------------------------------------------------------
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
bool DllIf::CMethodTracer::isActive( int i_iFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( m_pTrcAdminObj != NULL && s_pFctTrcAdminObj_isActive != NULL )
    {
        bIsActive = s_pFctTrcAdminObj_isActive(m_pTrcAdminObj, i_iFilterDetailLevel);
    }
    return bIsActive;

} // isActive

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CMethodTracer::trace( const char* i_szAddInfo, int i_iFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    if( m_pTrcAdminObj != NULL && s_pFctTrcAdminObj_isActive != NULL && s_pFctTrcAdminObj_traceMethod != NULL )
    {
        if( s_pFctTrcAdminObj_isActive(m_pTrcAdminObj, i_iFilterDetailLevel) )
        {
            s_pFctTrcAdminObj_traceMethod(m_pTrcAdminObj, m_szObjName, m_szMethod, i_szAddInfo);
        }
    }
} // trace


/*******************************************************************************
struct STrcServerSettings
*******************************************************************************/

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::STrcServerSettings_init( DllIf::STrcServerSettings&i_trcSettings )
//------------------------------------------------------------------------------
{
    i_trcSettings.m_bEnabled = true;
    i_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault = false;
    i_trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel = 0;
    i_trcSettings.m_bCacheDataIfNotConnected = false;
    i_trcSettings.m_iCacheDataMaxArrLen = 1000;
    i_trcSettings.m_szAdminObjFileAbsFilePath = NULL;
    i_trcSettings.m_bUseLocalTrcFile = true;
    i_trcSettings.m_szLocalTrcFileAbsFilePath = NULL;
    i_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms = 1000;
    i_trcSettings.m_iLocalTrcFileSubFileCountMax = 5;
    i_trcSettings.m_iLocalTrcFileSubFileLineCountMax = 2000;
    i_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite = false;

} // STrcServerSettings_init

//------------------------------------------------------------------------------
void DllIf::STrcServerSettings_release( DllIf::STrcServerSettings&i_trcSettings )
//------------------------------------------------------------------------------
{
    i_trcSettings.m_bEnabled = false;
    i_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault = false;
    i_trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel = 0;
    i_trcSettings.m_bCacheDataIfNotConnected = false;
    i_trcSettings.m_iCacheDataMaxArrLen = 0;
    delete i_trcSettings.m_szAdminObjFileAbsFilePath;
    i_trcSettings.m_szAdminObjFileAbsFilePath = NULL;
    i_trcSettings.m_bUseLocalTrcFile = false;
    delete i_trcSettings.m_szLocalTrcFileAbsFilePath;
    i_trcSettings.m_szLocalTrcFileAbsFilePath = NULL;
    i_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms = 0;
    i_trcSettings.m_iLocalTrcFileSubFileCountMax = 0;
    i_trcSettings.m_iLocalTrcFileSubFileLineCountMax = 0;
    i_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite = false;

} // STrcServerSettings_release


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
*/
DllIf::CTrcAdminObj* DllIf::CTrcServer::GetTraceAdminObj(
    const char* i_szNameSpace,
    const char* i_szClassName,
    const char* i_szObjName,
    EEnabled    i_bEnabledAsDefault,
    int         i_iDefaultDetailLevel,
    const char* i_szServerName )
//------------------------------------------------------------------------------
{
    DllIf::CTrcAdminObj* pTrcAdminObj = NULL;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_GetTraceAdminObj != NULL )
    {
        pTrcAdminObj = s_pFctTrcServer_GetTraceAdminObj(i_szServerName, i_szNameSpace, i_szClassName, i_szObjName, i_bEnabledAsDefault, i_iDefaultDetailLevel);
    }

    return pTrcAdminObj;

} // GetTraceAdminObj

//------------------------------------------------------------------------------
void DllIf::CTrcServer::ReleaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj, const char* i_szServerName )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_ReleaseTraceAdminObj != NULL )
    {
        s_pFctTrcServer_ReleaseTraceAdminObj(i_szServerName, i_pTrcAdminObj);
    }
} // ReleaseTraceAdminObj

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
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_SetOrganizationName != NULL )
    {
        s_pFctTrcServer_SetOrganizationName(i_szName);
    }
} // SetOrganizationName

//------------------------------------------------------------------------------
/*! Returns the name of the organization.

    @return Name of the organization. The string must be freed by the caller.
*/
char* DllIf::CTrcServer::GetOrganizationName()
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_GetOrganizationName != NULL )
    {
        return s_pFctTrcServer_GetOrganizationName();
    }
    return NULL;

} // GetOrganizationName

//------------------------------------------------------------------------------
/*! Sets the name of the application.

    @param i_szName [in] Name of the application.
*/
void DllIf::CTrcServer::SetApplicationName( const char* i_szName )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_SetApplicationName != NULL )
    {
        s_pFctTrcServer_SetApplicationName(i_szName);
    }
} // SetApplicationName

//------------------------------------------------------------------------------
/*! Returns the name of the application.

    @return Name of the application. The string must be freed by the caller.
*/
char* DllIf::CTrcServer::GetApplicationName()
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_GetApplicationName != NULL )
    {
        return s_pFctTrcServer_GetApplicationName();
    }
    return NULL;

} // GetApplicationName

//------------------------------------------------------------------------------
/*! Returns the path information for the trace admin objects xml file
    for the defined scope.

    The returned character pointer must be freed after calling the method.

    @return Character buffer containing the files absolute path. The caller
            must free this buffer.
*/
char* DllIf::CTrcServer::GetDefaultAdminObjFileAbsoluteFilePath( const char* i_szIniFileScope )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_GetDefaultAdminObjFileAbsoluteFilePath != NULL )
    {
        return s_pFctTrcServer_GetDefaultAdminObjFileAbsoluteFilePath(i_szIniFileScope);
    }
    return NULL;

} // GetDefaultAdminObjFileAbsoluteFilePath

//------------------------------------------------------------------------------
/*! Returns the path information for the trace method log file for the defined scope.

    The returned character pointer must be freed after calling the method.

    @param i_szIniFileScope [In]

    @return Character buffer containing the files absolute path. The caller
            must free this buffer.
*/
char* DllIf::CTrcServer::GetDefaultLocalTrcFileAbsoluteFilePath( const char* i_szIniFileScope )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_GetDefaultLocalTrcFileAbsoluteFilePath != NULL )
    {
        return s_pFctTrcServer_GetDefaultLocalTrcFileAbsoluteFilePath(i_szIniFileScope);
    }
    return NULL;

} // GetDefaultLocalTrcFileAbsoluteFilePath

//------------------------------------------------------------------------------
void DllIf::CTrcServer::RegisterCurrentThread( const char* i_szThreadName )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_RegisterCurrentThread != NULL )
    {
        s_pFctTrcServer_RegisterCurrentThread(i_szThreadName);
    }

} // RegisterCurrentThread

//------------------------------------------------------------------------------
void DllIf::CTrcServer::UnregisterCurrentThread()
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_UnregisterCurrentThread != NULL )
    {
        s_pFctTrcServer_UnregisterCurrentThread();
    }

} // RegisterCurrentThread

//------------------------------------------------------------------------------
char* DllIf::CTrcServer::GetCurrentThreadName()
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_GetCurrentThreadName != NULL )
    {
        return s_pFctTrcServer_GetCurrentThreadName();
    }
    return NULL;

} // GetCurrentThreadName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::isActive() const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_isActive != NULL )
    {
        bIsActive = s_pFctTrcServer_isActive(this);
    }
    return bIsActive;

} // isActive

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_setEnabled != NULL )
    {
        s_pFctTrcServer_setEnabled(this, i_bEnabled);
    }
} // setEnabled

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::isEnabled() const
//------------------------------------------------------------------------------
{
    bool bEnabled = false;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_isEnabled != NULL )
    {
        bEnabled = s_pFctTrcServer_isEnabled(this);
    }
    return bEnabled;

} // isEnabled

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setNewTrcAdminObjsEnabledAsDefault( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_setNewTrcAdminObjsEnabledAsDefault != NULL )
    {
        s_pFctTrcServer_setNewTrcAdminObjsEnabledAsDefault(this, i_bEnabled);
    }
} // setNewTrcAdminObjsEnabledAsDefault

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::areNewTrcAdminObjsEnabledAsDefault() const
//------------------------------------------------------------------------------
{
    bool bEnabled = false;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_areNewTrcAdminObjsEnabledAsDefault != NULL )
    {
        bEnabled = s_pFctTrcServer_areNewTrcAdminObjsEnabledAsDefault(this);
    }
    return bEnabled;

} // areNewTrcAdminObjsEnabledAsDefault

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setNewTrcAdminObjsDefaultDetailLevel( int i_iDetailLevel )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_setNewTrcAdminObjsDefaultDetailLevel != NULL )
    {
        s_pFctTrcServer_setNewTrcAdminObjsDefaultDetailLevel(this, i_iDetailLevel);
    }
} // setNewTrcAdminObjsDefaultDetailLevel

//------------------------------------------------------------------------------
int DllIf::CTrcServer::getNewTrcAdminObjsDefaultDetailLevel() const
//------------------------------------------------------------------------------
{
    int iDetailLevel = ETraceDetailLevelNone;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_getNewTrcAdminObjsDefaultDetailLevel != NULL )
    {
        iDetailLevel = s_pFctTrcServer_getNewTrcAdminObjsDefaultDetailLevel(this);
    }
    return iDetailLevel;

} // getNewTrcAdminObjsDefaultDetailLevel

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setAdminObjFileAbsoluteFilePath( const char* i_szAbsFilePath )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_setAdminObjFileAbsoluteFilePath != NULL )
    {
        s_pFctTrcServer_setAdminObjFileAbsoluteFilePath(this, i_szAbsFilePath);
    }
} // setAdminObjFileAbsoluteFilePath

//------------------------------------------------------------------------------
char* DllIf::CTrcServer::getAdminObjFileAbsoluteFilePath() const
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_getAdminObjFileAbsoluteFilePath != NULL )
    {
        return s_pFctTrcServer_getAdminObjFileAbsoluteFilePath(this);
    }
    return NULL;

} // getAdminObjFileAbsoluteFilePath

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::recallAdminObjs()
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_recallAdminObjs != NULL )
    {
        bOk = s_pFctTrcServer_recallAdminObjs(this);
    }
    return bOk;

} // recallAdminObjs

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::saveAdminObjs()
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_saveAdminObjs != NULL )
    {
        bOk = s_pFctTrcServer_saveAdminObjs(this);
    }
    return bOk;

} // saveAdminObjs

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setUseLocalTrcFile( bool i_bUse )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_setUseLocalTrcFile != NULL )
    {
        s_pFctTrcServer_setUseLocalTrcFile(this, i_bUse);
    }
} // setUseLocalTrcFile

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::isLocalTrcFileUsed() const
//------------------------------------------------------------------------------
{
    bool bUsed = false;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_isLocalTrcFileUsed != NULL )
    {
        bUsed = s_pFctTrcServer_isLocalTrcFileUsed(this);
    }
    return bUsed;

} // isLocalTrcFileUsed

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setLocalTrcFileAbsoluteFilePath( const char* i_szAbsFilePath )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_setLocalTrcFileAbsoluteFilePath != NULL )
    {
        s_pFctTrcServer_setLocalTrcFileAbsoluteFilePath(this, i_szAbsFilePath);
    }
} // setLocalTrcFileAbsoluteFilePath

//------------------------------------------------------------------------------
char* DllIf::CTrcServer::getLocalTrcFileAbsoluteFilePath() const
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_getLocalTrcFileAbsoluteFilePath != NULL )
    {
        return s_pFctTrcServer_getLocalTrcFileAbsoluteFilePath(this);
    }
    return NULL;

} // getLocalTrcFileAbsoluteFilePath

//------------------------------------------------------------------------------
char* DllIf::CTrcServer::getLocalTrcFileCompleteBaseName() const
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_getLocalTrcFileCompleteBaseName != NULL )
    {
        return s_pFctTrcServer_getLocalTrcFileCompleteBaseName(this);
    }
    return NULL;

} // getLocalTrcFileCompleteBaseName

//------------------------------------------------------------------------------
char* DllIf::CTrcServer::getLocalTrcFileAbsolutePath() const
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_getLocalTrcFileAbsolutePath != NULL )
    {
        return s_pFctTrcServer_getLocalTrcFileAbsolutePath(this);
    }
    return NULL;

} // getLocalTrcFileAbsolutePath

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::isLocalTrcFileActive() const
//------------------------------------------------------------------------------
{
    bool bUsed = false;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_isLocalTrcFileActive != NULL )
    {
        bUsed = s_pFctTrcServer_isLocalTrcFileActive(this);
    }
    return bUsed;

} // isLocalTrcFileActive

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setLocalTrcFileAutoSaveInterval( int i_iAutoSaveInterval_ms )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_setLocalTrcFileAutoSaveInterval != NULL )
    {
        s_pFctTrcServer_setLocalTrcFileAutoSaveInterval(this, i_iAutoSaveInterval_ms);
    }
} // setLocalTrcFileAutoSaveInterval

//------------------------------------------------------------------------------
int DllIf::CTrcServer::getLocalTrcFileAutoSaveInterval() const
//------------------------------------------------------------------------------
{
    int iAutoSaveInterval_ms = 0;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_getLocalTrcFileAutoSaveInterval != NULL )
    {
        iAutoSaveInterval_ms = s_pFctTrcServer_getLocalTrcFileAutoSaveInterval(this);
    }
    return iAutoSaveInterval_ms;

} // getLocalTrcFileAutoSaveInterval

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setLocalTrcFileCloseFileAfterEachWrite( bool i_bCloseFile )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite != NULL )
    {
        s_pFctTrcServer_setLocalTrcFileCloseFileAfterEachWrite(this, i_bCloseFile);
    }
} // setLocalTrcFileCloseFileAfterEachWrite

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::getLocalTrcFileCloseFileAfterEachWrite() const
//------------------------------------------------------------------------------
{
    bool bClose = false;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite != NULL )
    {
        bClose = s_pFctTrcServer_getLocalTrcFileCloseFileAfterEachWrite(this);
    }
    return bClose;

} // getLocalTrcFileCloseFileAfterEachWrite

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setLocalTrcFileSubFileCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_setLocalTrcFileSubFileCountMax != NULL )
    {
        s_pFctTrcServer_setLocalTrcFileSubFileCountMax(this, i_iCountMax);
    }
} // setLocalTrcFileSubFileCountMax

//------------------------------------------------------------------------------
int DllIf::CTrcServer::getLocalTrcFileSubFileCountMax() const
//------------------------------------------------------------------------------
{
    int iCountMax = ETraceDetailLevelNone;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_getLocalTrcFileSubFileCountMax != NULL )
    {
        iCountMax = s_pFctTrcServer_getLocalTrcFileSubFileCountMax(this);
    }
    return iCountMax;

} // getLocalTrcFileSubFileCountMax

//------------------------------------------------------------------------------
void DllIf::CTrcServer::setLocalTrcFileSubFileLineCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_setLocalTrcFileSubFileLineCountMax != NULL )
    {
        s_pFctTrcServer_setLocalTrcFileSubFileLineCountMax(this, i_iCountMax);
    }
} // setLocalTrcFileSubFileLineCountMax

//------------------------------------------------------------------------------
int DllIf::CTrcServer::getLocalTrcFileSubFileLineCountMax() const
//------------------------------------------------------------------------------
{
    int iCountMax = ETraceDetailLevelNone;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_getLocalTrcFileSubFileLineCountMax != NULL )
    {
        iCountMax = s_pFctTrcServer_getLocalTrcFileSubFileLineCountMax(this);
    }
    return iCountMax;

} // getLocalTrcFileSubFileLineCountMax

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Definiert, ob das Caching des Methoden Traces aktivert werden soll.

    Ist kein Trace Client verbunden, werden die Methoden Traces nicht an den
    Client geschickt. Das Caching ermoeglicht es z.B. auch die Methoden Traces
    zum Client zu schicken, nachdem sich dieser verbunden hat. Damit kann z.B.
    auch der Startup der Applikation fuer den Client sichtbar gemacht werden.

    @param i_bCacheData [In] true um das caching zu aktivieren.
*/
void DllIf::CTrcServer::setCacheTrcDataIfNotConnected( bool i_bCacheData )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_setCacheTrcDataIfNotConnected != NULL )
    {
        s_pFctTrcServer_setCacheTrcDataIfNotConnected(this, i_bCacheData);
    }
} // setCacheTrcDataIfNotConnected

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::getCacheTrcDataIfNotConnected() const
//------------------------------------------------------------------------------
{
    bool bCacheData = false;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_getCacheTrcDataIfNotConnected != NULL )
    {
        bCacheData = s_pFctTrcServer_getCacheTrcDataIfNotConnected(this);
    }
    return bCacheData;

} // getCacheTrcDataIfNotConnected

//------------------------------------------------------------------------------
/*! Ist das Caching aktiviert wird hier die Anzahl der Zeilen definiert, die im
    Cache abgelegt werden.

    @param i_iMaxArrLen [In] Anzahl der Methoden Aufrufe, die im Cache gespeichert
           werden, bis sich ein Client verbindet.
*/
void DllIf::CTrcServer::setCacheTrcDataMaxArrLen( int i_iMaxArrLen )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_setCacheTrcDataMaxArrLen != NULL )
    {
        s_pFctTrcServer_setCacheTrcDataMaxArrLen(this, i_iMaxArrLen);
    }
} // setCacheTrcDataMaxArrLen

//------------------------------------------------------------------------------
int DllIf::CTrcServer::getCacheTrcDataMaxArrLen() const
//------------------------------------------------------------------------------
{
    int iMaxArrLen = 0;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_getCacheTrcDataMaxArrLen != NULL )
    {
        iMaxArrLen = s_pFctTrcServer_getCacheTrcDataMaxArrLen(this);
    }
    return iMaxArrLen;

} // getCacheTrcDataMaxArrLen

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool DllIf::CTrcServer::setTraceSettings( const DllIf::STrcServerSettings& i_settings )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_setTraceSettings != NULL )
    {
        bOk = s_pFctTrcServer_setTraceSettings(this, i_settings);
    }
    return bOk;

} // setTraceSettings

//------------------------------------------------------------------------------
DllIf::STrcServerSettings DllIf::CTrcServer::getTraceSettings() const
//------------------------------------------------------------------------------
{
    DllIf::STrcServerSettings settings;

    STrcServerSettings_init(settings);

    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_getTraceSettings != NULL )
    {
        settings = s_pFctTrcServer_getTraceSettings(this);
    }
    return settings;

} // getTraceSettings

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CTrcServer::clearLocalTrcFile()
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcServer_clearLocalTrcFile != NULL )
    {
        s_pFctTrcServer_clearLocalTrcFile(this);
    }
} // clearLocalTrcFile

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
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcAdminObj_isActive != NULL && s_pFctTrcAdminObj_traceMethodEnter != NULL )
    {
        if( i_pAdminObj != NULL )
        {
            if( s_pFctTrcAdminObj_isActive(i_pAdminObj, ETraceDetailLevelMethodCalls) )
            {
                s_pFctTrcAdminObj_traceMethodEnter(i_pAdminObj, "", i_szMethod, i_szMethodInArgs);
            }
        }
    }
} // traceMethodEnter

//------------------------------------------------------------------------------
void DllIf::CTrcServer::traceMethodEnter(
    const CTrcAdminObj* i_pAdminObj,
    const char*         i_szObjName,
    const char*         i_szMethod,
    const char*         i_szMethodInArgs )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcAdminObj_isActive != NULL && s_pFctTrcAdminObj_traceMethodEnter != NULL )
    {
        if( i_pAdminObj != NULL )
        {
            if( s_pFctTrcAdminObj_isActive(i_pAdminObj, ETraceDetailLevelMethodCalls) )
            {
                s_pFctTrcAdminObj_traceMethodEnter(i_pAdminObj, i_szObjName, i_szMethod, i_szMethodInArgs);
            }
        }
    }
} // traceMethodEnter

//------------------------------------------------------------------------------
void DllIf::CTrcServer::traceMethod(
    const CTrcAdminObj* i_pAdminObj,
    const char*         i_szMethod,
    const char*         i_szAddInfo )
//------------------------------------------------------------------------------
{
    if( i_pAdminObj != NULL && s_pFctTrcAdminObj_isActive != NULL && s_pFctTrcAdminObj_traceMethod != NULL )
    {
        if( s_pFctTrcAdminObj_isActive(i_pAdminObj, ETraceDetailLevelMethodCalls) )
        {
            s_pFctTrcAdminObj_traceMethod(i_pAdminObj, "", i_szMethod, i_szAddInfo);
        }
    }
} // traceMethod

//------------------------------------------------------------------------------
void DllIf::CTrcServer::traceMethod(
    const CTrcAdminObj* i_pAdminObj,
    const char*         i_szObjName,
    const char*         i_szMethod,
    const char*         i_szAddInfo )
//------------------------------------------------------------------------------
{
    if( i_pAdminObj != NULL && s_pFctTrcAdminObj_isActive != NULL && s_pFctTrcAdminObj_traceMethod != NULL )
    {
        if( s_pFctTrcAdminObj_isActive(i_pAdminObj, ETraceDetailLevelMethodCalls) )
        {
            s_pFctTrcAdminObj_traceMethod(i_pAdminObj, i_szObjName, i_szMethod, i_szAddInfo);
        }
    }
} // traceMethod

//------------------------------------------------------------------------------
void DllIf::CTrcServer::traceMethodLeave(
    const CTrcAdminObj* i_pAdminObj,
    const char*         i_szMethod,
    const char*         i_szMethodReturn,
    const char*         i_szMethodOutArgs )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcAdminObj_isActive != NULL && s_pFctTrcAdminObj_traceMethodLeave != NULL )
    {
        if( i_pAdminObj != NULL )
        {
            if( s_pFctTrcAdminObj_isActive(i_pAdminObj, ETraceDetailLevelMethodCalls) )
            {
                s_pFctTrcAdminObj_traceMethodLeave(i_pAdminObj, "", i_szMethod, i_szMethodReturn, i_szMethodOutArgs);
            }
        }
    }
} // traceMethodLeave

//------------------------------------------------------------------------------
void DllIf::CTrcServer::traceMethodLeave(
    const CTrcAdminObj* i_pAdminObj,
    const char*         i_szObjName,
    const char*         i_szMethod,
    const char*         i_szMethodReturn,
    const char*         i_szMethodOutArgs )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctTrcAdminObj_isActive != NULL && s_pFctTrcAdminObj_traceMethodLeave != NULL )
    {
        if( i_pAdminObj != NULL )
        {
            if( s_pFctTrcAdminObj_isActive(i_pAdminObj, ETraceDetailLevelMethodCalls) )
            {
                s_pFctTrcAdminObj_traceMethodLeave(i_pAdminObj, i_szObjName, i_szMethod, i_szMethodReturn, i_szMethodOutArgs);
            }
        }
    }
} // traceMethodLeave


/*******************************************************************************
class CIpcTrcServer
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns a reference to the trace server with the given name.

    This method is used to access an already existing trace server.
    Neiter a trace server may be created by this method call nor a reference
    counter of the trace server is affected.

    @return Reference to trace server or nullptr, if a trace server with the
            given name is not existing.

   @note This method does not incremente the reference counter of the trace server.
*/
DllIf::CIpcTrcServer* DllIf::CIpcTrcServer::GetInstance( const char* i_szName )
//------------------------------------------------------------------------------
{
    DllIf::CIpcTrcServer* pTrcServer = NULL;

    if( s_hndIpcTrcDllIf != NULL && s_pFctIpcTrcServer_GetInstance != NULL )
    {
        pTrcServer = s_pFctIpcTrcServer_GetInstance(i_szName);
    }
    return pTrcServer;

} // GetInstance

//------------------------------------------------------------------------------
/*! Creates a trace server with the given name if a trace server with the given
    name is not already existing.

    If a trace server with the given name is already existing the reference to
    the existing trace server is returned and a reference counter is incremented.

    @param i_szName [in] Name of the trace server.
           Default: "ZSTrcServer"

    @param i_iTrcDetailLevel [in] For debugging purposes of the Dll interface
           the Dll interace methods may be traced itself by writing a log file.
           Default: ETraceDetailLevelNone

    @return Reference to trace server or nullptr.
            nullptr is returned if i_bCreateIfNotExisting was set to false
            and a trace server with the given name is not existing.
*/
DllIf::CIpcTrcServer* DllIf::CIpcTrcServer::CreateInstance(
    const char* i_szName,
    int i_iTrcDetailLevel )
//------------------------------------------------------------------------------
{
    DllIf::CIpcTrcServer* pTrcServer = NULL;

    if( s_hndIpcTrcDllIf != NULL && s_pFctIpcTrcServer_GetInstance != NULL )
    {
        pTrcServer = s_pFctIpcTrcServer_CreateInstance(i_szName, i_iTrcDetailLevel);
    }
    return pTrcServer;

} // CreateInstance

//------------------------------------------------------------------------------
/*! Releases the trace server by decrementing the reference counter. If the
    reference counter reaches 0 the trace server will be destroyed.

    @param i_pTrcServer [in] Reference to the trace server as returned by
           the GetInstance call.
*/
void DllIf::CIpcTrcServer::ReleaseInstance( DllIf::CIpcTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    if( s_hndIpcTrcDllIf != NULL && s_pFctIpcTrcServer_ReleaseInstance != NULL )
    {
        s_pFctIpcTrcServer_ReleaseInstance(i_pTrcServer);
    }

} // ReleaseInstance

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

    if( s_hndIpcTrcDllIf != NULL && s_pFctIpcTrcServer_startup != NULL )
    {
        bOk = s_pFctIpcTrcServer_startup(this, i_iTimeout_ms, true);
    }
    return bOk;

} // startupServer

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

    if( s_hndIpcTrcDllIf != NULL && s_pFctIpcTrcServer_shutdown != NULL )
    {
        bOk = s_pFctIpcTrcServer_shutdown(this, i_iTimeout_ms, true);
    }
    return bOk;

} // shutdown

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool DllIf::CIpcTrcServer::isListening() const
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndIpcTrcDllIf != NULL && s_pFctIpcTrcServer_isListening != NULL )
    {
        bOk = s_pFctIpcTrcServer_isListening(this);
    }
    return bOk;

} // isListening

//------------------------------------------------------------------------------
bool DllIf::CIpcTrcServer::isConnected() const
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndIpcTrcDllIf != NULL && s_pFctIpcTrcServer_isConnected != NULL )
    {
        bOk = s_pFctIpcTrcServer_isConnected(this);
    }
    return bOk;

} // isConnected

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool DllIf::CIpcTrcServer::setPort( unsigned short i_uPort, int i_iTimeout_ms, bool i_bWait )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( s_hndIpcTrcDllIf != NULL && s_pFctIpcTrcServer_setPort != NULL )
    {
        bOk = s_pFctIpcTrcServer_setPort(this, i_uPort, i_iTimeout_ms, i_bWait);
    }
    return bOk;

} // setPort

//------------------------------------------------------------------------------
unsigned short DllIf::CIpcTrcServer::getPort() const
//------------------------------------------------------------------------------
{
    unsigned short uPort = 0;

    if( s_hndIpcTrcDllIf != NULL && s_pFctIpcTrcServer_getPort != NULL )
    {
        uPort = s_pFctIpcTrcServer_getPort(this);
    }
    return uPort;

} // getPort
