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

#ifdef _WINDOWS
#include <windows.h>
#endif

#include <QtCore/qglobal.h>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;


/*******************************************************************************
Libraries depending on build configuration and used Qt version
*******************************************************************************/

#ifdef _WINDOWS

//#pragma comment(lib, "user32.lib")
//#pragma comment(lib, "advapi32.lib")

#ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

#pragma message("COMPILERLIBINFIX = " COMPILERLIBINFIX)
#pragma message("PLATFORMLIBINFIX = " PLATFORMLIBINFIX)
#pragma message("CONFIGLIBINFIX = " CONFIGLIBINFIX)

#pragma message("QTMAINLIB = " QTMAINLIB)
#pragma message("QTCORELIB = " QTCORELIB)
#pragma message("QTNETWORKLIB = " QTNETWORKLIB)
#pragma message("QTXMLLIB = " QTXMLLIB)
#pragma message("QTSQLLIB = " QTSQLLIB)
#pragma message("QTGUILIB = " QTGUILIB)
#pragma message("QTWIDGETSLIB = " QTWIDGETSLIB)

#pragma message("ZSSYSLIB = " ZSSYSLIB)
#pragma message("ZSSYSGUILIB = " ZSSYSGUILIB)
#pragma message("ZSIPCLIB = " ZSIPCLIB)
#pragma message("ZSIPCGUILIB = " ZSIPCGUILIB)
#pragma message("ZSIPCTRACELIB = " ZSIPCTRACELIB)
#pragma message("ZSIPCTRACEGUILIB = " ZSIPCTRACEGUILIB)

#pragma message(__FILE__ ": Linking against = " QTCORELIB)
#pragma comment(lib, QTCORELIB)
#pragma message(__FILE__ ": Linking against = " QTXMLLIB)
#pragma comment(lib, QTXMLLIB)

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

