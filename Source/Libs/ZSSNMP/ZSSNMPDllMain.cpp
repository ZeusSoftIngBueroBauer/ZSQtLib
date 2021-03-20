/*******************************************************************************

Copyright 2004 - 2010 by    ZeusSoft, Ing. Buero Bauer
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

--------------------------------------------------------------------------------

$Date: $

$Revision: $

$Author: $

$History: $

*******************************************************************************/

#ifdef _WINDOWS
#include <windows.h>
#endif
#include <QtCore/qglobal.h>
#include "ZSSNMP/Include/ZSSNMPDllMain.h"
#include "ZSSys/Include/ZSSysVersion.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"

/*******************************************************************************
Libraries depending on build configuration and used Qt version (Qt-Isar-Dll's)
*******************************************************************************/

#ifdef _WINDOWS

#ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

    // Qt-Dll's
    //--------------------------------------------------------------------------

    #ifdef USE_RS_QT_ISAR_DLLs
        #ifdef _DEBUG
            #pragma comment(lib, "QtCoreIsard4")
            #pragma comment(lib, "QtNetworkIsard4")
            #pragma comment(lib, "QtXmlIsard4")
            #ifndef NO_GUI
                #pragma comment(lib, "QtGuiIsard4")
            #endif
        #else
            #pragma comment(lib, "QtCoreIsar4")
            #pragma comment(lib, "QtNetworkIsar4")
            #pragma comment(lib, "QtXmlIsar4")
            #ifndef NO_GUI
                #pragma comment(lib, "QtGuiIsar4")
            #endif
        #endif
    #elif defined USE_RS_QT_RASRV_DLLs
        #ifdef _DEBUG
            #pragma comment(lib, "qtmainRasrvd")
            #pragma comment(lib, "QtCoreRasrvd4")
            #pragma comment(lib, "QtNetworkRasrvd4")
            #pragma comment(lib, "QtXmlRasrvd4")
            #ifndef NO_GUI
                #pragma comment(lib, "QtGuiRasrvd4")
            #endif
        #else
            #pragma comment(lib, "qtmainRasrv")
            #pragma comment(lib, "QtCoreRasrv4")
            #pragma comment(lib, "QtNetworkRasrv4")
            #pragma comment(lib, "QtXmlRasrv4")
            #ifndef NO_GUI
                #pragma comment(lib, "QtGuiRasrv4")
            #endif
        #endif
    // Room for further Qt renaming ideas ...
    #else
        #ifdef _DEBUG
            #pragma comment(lib, "QtCored4")
            #pragma comment(lib, "QtNetworkd4")
            #pragma comment(lib, "QtXmld4")
            #ifndef NO_GUI
                #pragma comment(lib, "QtGuid4")
            #endif
        #else
            #pragma comment(lib, "QtCore4")
            #pragma comment(lib, "QtNetwork4")
            #pragma comment(lib, "QtXml4")
            #ifndef NO_GUI
                #pragma comment(lib, "QtGui4")
            #endif
        #endif
    #endif

#endif // #ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

#endif // #ifdef _WINDOWS


/*******************************************************************************
Entry point for the DLL application.
*******************************************************************************/

#ifdef _WINDOWS
//------------------------------------------------------------------------------
BOOL APIENTRY DllMain(
    HANDLE /*i_hModule*/,
    DWORD  i_ul_reason_for_call,
    LPVOID /*i_lpReserved*/ )
//------------------------------------------------------------------------------
{
    switch( i_ul_reason_for_call )
    {
        case DLL_PROCESS_ATTACH:
        {
            break;
        }
        case DLL_THREAD_ATTACH:
        {
            break;
        }
        case DLL_THREAD_DETACH:
        {
            break;
        }
        case DLL_PROCESS_DETACH:
        {
            break;
        }
        default:
        {
            break;
        }
    }
    return TRUE;

} // DllMain
#endif
