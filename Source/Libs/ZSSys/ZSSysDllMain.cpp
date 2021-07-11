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

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")

#if _MSC_VER <= 1200
#pragma message("Visual Studio 6.0")
#elif _MSC_VER >= 1300 && _MSC_VER <= 1300
#pragma message("Visual Studio .NET 2002 (7.0)")
#elif _MSC_VER >= 1310 && _MSC_VER <= 1310
#pragma message("Visual Studio .NET 2003 (7.1)")
#elif _MSC_VER >= 1400 && _MSC_VER <= 1400
#pragma message("Visual Studio 2005 (8.0)")
#elif _MSC_VER >= 1500 && _MSC_VER <= 1500
#pragma message("Visual Studio 2008 (9.0)")
#elif _MSC_VER >= 1600 && _MSC_VER <= 1600
#pragma message("Visual Studio 2010 (10.0)")
#elif _MSC_VER >= 1700 && _MSC_VER <= 1700
#pragma message("Visual Studio 2012 (11.0)")
#elif _MSC_VER >= 1800 && _MSC_VER <= 1800
#pragma message("Visual Studio 2013 (12.0)")
#elif _MSC_VER >= 1900 && _MSC_VER <= 1900
#pragma message("Visual Studio 2015 (14.0)")
#elif _MSC_VER >= 1910 && _MSC_VER <= 1910
#pragma message("Visual Studio 2017 RTW (15.0)")
#elif _MSC_VER >= 1911 && _MSC_VER <= 1911
#pragma message("Visual Studio 2017 version 15.3")
#elif _MSC_VER >= 1912 && _MSC_VER <= 1912
#pragma message("Visual Studio 2017 version 15.5")
#elif _MSC_VER >= 1913 && _MSC_VER <= 1913
#pragma message("Visual Studio 2017 version 15.6")
#elif _MSC_VER >= 1914 && _MSC_VER <= 1914
#pragma message("Visual Studio 2017 version 15.7")
#elif _MSC_VER >= 1915 && _MSC_VER <= 1915
#pragma message("Visual Studio 2017 version 15.8")
#elif _MSC_VER >= 1916 && _MSC_VER <= 1916
#pragma message("Visual Studio 2017 version 15.9")
#elif _MSC_VER >= 1920 && _MSC_VER <= 1928
#pragma message("Visual Studio 2019 RTW (16.0)")
#elif _MSC_VER >= 1921 && _MSC_VER <= 1921
#pragma message("Visual Studio 2019 version 16.1")
#elif _MSC_VER >= 1922 && _MSC_VER <= 1922
#pragma message("Visual Studio 2019 version 16.2")
#elif _MSC_VER >= 1923 && _MSC_VER <= 1923
#pragma message("Visual Studio 2019 version 16.3")
#elif _MSC_VER >= 1924 && _MSC_VER <= 1924
#pragma message("Visual Studio 2019 version 16.4")
#elif _MSC_VER >= 1925 && _MSC_VER <= 1925
#pragma message("Visual Studio 2019 version 16.5")
#elif _MSC_VER >= 1926 && _MSC_VER <= 1926
#pragma message("Visual Studio 2019 version 16.6")
#elif _MSC_VER >= 1927 && _MSC_VER <= 1927
#pragma message("Visual Studio 2019 version 16.7")
#elif _MSC_VER >= 1928 && _MSC_VER <= 1928
#pragma message("Visual Studio 2019 version 16.8")
#elif _MSC_VER >= 1929 && _MSC_VER <= 1929
#pragma message("Visual Studio 2019 version 16.10")
#else
#error _MSC_VERSION not defined or not yet added to the ifdefs above
#endif

#if (CXX_STANDARD == 17)
#pragma message("CXX_STANDARD: 201703 (C++17)")
#elif (CXX_STANDARD == 14)
#pragma message("CXX_STANDARD: 201402 (C++14")
#elif (CXX_STANDARD == 11)
#pragma message("CXX_STANDARD: 201103 (C++11")
#elif (CXX_STANDARD == 98)
#pragma message("CXX_STANDARD: 19971 (C++98")
#elif (CXX_STANDARD == 1)
#pragma message("CXX_STANDARD: 1 (pre C++98)")
#else
#error CXX_STANDARD must be defined in ZSSysDllMain.h
#endif

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
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#pragma message("QTWIDGETSLIB = " QTWIDGETSLIB)
#endif

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

