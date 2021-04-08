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

#ifndef ZSSys_DllMain_h
#define ZSSys_DllMain_h

#include <QtCore/QString>

// generic export defines
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define __API_EXPORT __declspec(dllexport)
    #define __API_IMPORT __declspec(dllimport)
#elif defined(__linux__)
    #if (__GNUC__ >= 4)
        #define __API_EXPORT __attribute((visibility("default")))
    #else
        #define __API_EXPORT
    #endif
    #define  __API_IMPORT
#else
    #error Missing export macro
#endif

// specific export defines
#ifdef ZSSYSDLL_EXPORTS
    #define ZSSYSDLL_API __API_EXPORT
#else
    #define ZSSYSDLL_API __API_IMPORT
#endif


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

#define _ZSArrLen(_arr) static_cast<int>((sizeof(_arr)/sizeof(_arr[0])))

#define __STR__(x) #x
#define __STRING__(x) __STR__(x)
#define __FILELINE__ __FILE__ "(" __STRING__(__LINE__) "): "
#define __INFO__ __FILELINE__ "Info: "
#define __WARNING__ __FILELINE__ "Warning: "
#define __ERROR__ __FILELINE__ "Error: "
#define __TODO__ __FILELINE__ "TODO: "

/*
Visual Studio version           | _MSC_VER
--------------------------------+---------------
Visual Studio 6.0               | 1200
Visual Studio .NET 2002 (7.0)   | 1300
Visual Studio .NET 2003 (7.1)   | 1310
Visual Studio 2005 (8.0)        | 1400
Visual Studio 2008 (9.0)        | 1500
Visual Studio 2010 (10.0)       | 1600
Visual Studio 2012 (11.0)       | 1700
Visual Studio 2013 (12.0)       | 1800
Visual Studio 2015 (14.0)       | 1900
Visual Studio 2017 RTW (15.0)   | 1910
Visual Studio 2017 version 15.3 | 1911
Visual Studio 2017 version 15.5 | 1912
Visual Studio 2017 version 15.6 | 1913
Visual Studio 2017 version 15.7 | 1914
Visual Studio 2017 version 15.8 | 1915
Visual Studio 2017 version 15.9 | 1916
Visual Studio 2019 RTW (16.0)   | 1920
Visual Studio 2019 version 16.1 | 1921
Visual Studio 2019 version 16.2 | 1922
Visual Studio 2019 version 16.3 | 1923
Visual Studio 2019 version 16.4 | 1924
Visual Studio 2019 version 16.5 | 1925
Visual Studio 2019 version 16.6 | 1926
Visual Studio 2019 version 16.7 | 1927
Visual Studio 2019 version 16.8 | 1928
*/
#if _MSC_VER <= 1200
#define __CXX_STANDARD__ 1
#elif _MSC_VER >= 1300 && _MSC_VER <= 1300
#define __CXX_STANDARD__ 1
#elif _MSC_VER >= 1310 && _MSC_VER <= 1310
#define __CXX_STANDARD__ 1
#elif _MSC_VER >= 1400 && _MSC_VER <= 1400
#define __CXX_STANDARD__ 1
#elif _MSC_VER >= 1500 && _MSC_VER <= 1500
#define __CXX_STANDARD__ 1
#elif _MSC_VER >= 1600 && _MSC_VER <= 1600
#define __CXX_STANDARD__ 1
#elif _MSC_VER >= 1700 && _MSC_VER <= 1700
#define __CXX_STANDARD__ 1
#elif _MSC_VER >= 1800 && _MSC_VER <= 1800
#define __CXX_STANDARD__ 201402
#define CXX_STANDARD CPP17
#elif _MSC_VER >= 1900 && _MSC_VER <= 1900
#define __CXX_STANDARD__ 201703
#define CXX_STANDARD CPP17
#elif _MSC_VER >= 1910 && _MSC_VER <= 1916
#define __CXX_STANDARD__ 201703
#elif _MSC_VER >= 1920 && _MSC_VER <= 1928
#define __CXX_STANDARD__ 201703
#else
#define __CXX_STANDARD__ 1
#endif

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
#endif

#ifndef nullptr
#define nullptr NULL
#endif

namespace ZS
{
namespace System
{
}
namespace Trace
{
}
} // namespace ZS

#endif // #ifndef ZSSys_DllMain_h
