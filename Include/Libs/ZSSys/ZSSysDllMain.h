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

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#define QT_VERSION_MAJOR 5
#elif QT_VERSION >= QT_VERSION_CHECK(4, 8, 4)
#define QT_VERSION_MAJOR 4
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

// Set the following compile switch to use the pragmas within the main modules
// for linking the different versions of the Qt dlls (Qt 4, Qt 5, Debug, Release).
#ifdef _WINDOWS
#ifndef DONT_USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES
#define USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES
#endif
#endif

// Some customers are configuring Qt to rename the Qt libraries to Qt*<infix>.
#define QTLIBINFIX ""
//#define QTLIBINFIX "Isar"
//#define QTLIBINFIX "SWP"

#ifdef _WINDOWS
#ifdef _WIN64
#define PLATFORMLIBINFIX "x64"
#else
#define PLATFORMLIBINFIX "Win32"
#endif
#endif // #ifdef _WINDOWS

#ifndef PLATFORMLIBINFIX
#define PLATFORMLIBINFIX ""
#endif

#ifdef _DEBUG
#define CONFIGLIBINFIX "d"
#else
#define CONFIGLIBINFIX ""
#endif

#ifdef _WINDOWS
#define QTMAINLIB "qtmain"QTLIBINFIX""CONFIGLIBINFIX
#if QT_VERSION >= 0x040000 && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#define QTCORELIB    "QtCore"QTLIBINFIX""CONFIGLIBINFIX""_ZSSYS_TOSTRING(QT_VERSION_MAJOR)
#define QTNETWORKLIB "QtNetwork"QTLIBINFIX""CONFIGLIBINFIX""_ZSSYS_TOSTRING(QT_VERSION_MAJOR)
#define QTXMLLIB     "QtXml"QTLIBINFIX""CONFIGLIBINFIX""_ZSSYS_TOSTRING(QT_VERSION_MAJOR)
#define QTSQLLIB     "QtSql"QTLIBINFIX""CONFIGLIBINFIX""_ZSSYS_TOSTRING(QT_VERSION_MAJOR)
#define QTGUILIB     "QtGui"QTLIBINFIX""CONFIGLIBINFIX""_ZSSYS_TOSTRING(QT_VERSION_MAJOR)
#elif QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define QTCORELIB    "Qt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)"Core"QTLIBINFIX""CONFIGLIBINFIX
#define QTNETWORKLIB "Qt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)"Network"QTLIBINFIX""CONFIGLIBINFIX
#define QTXMLLIB     "Qt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)"Xml"QTLIBINFIX""CONFIGLIBINFIX
#define QTSQLLIB     "Qt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)"Sql"QTLIBINFIX""CONFIGLIBINFIX
#define QTGUILIB     "Qt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)"Gui"QTLIBINFIX""CONFIGLIBINFIX
#define QTWIDGETSLIB "Qt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)"Widgets"QTLIBINFIX""CONFIGLIBINFIX
#endif
#ifdef _DEBUG
#define ZSSYSLIB                  "ZSSysQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX"_"CONFIGLIBINFIX
#define ZSSYSGUILIB               "ZSSysGUIQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX"_"CONFIGLIBINFIX
#define ZSPHYSVALLIB              "ZSPhysValQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX"_"CONFIGLIBINFIX
#define ZSPHYSVALGUILIB           "ZSPhysValGUIQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX"_"CONFIGLIBINFIX
#define ZSPHYSSIZESELECTRICITYLIB "ZSPhysSizesElectricityQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX"_"CONFIGLIBINFIX
#define ZSPHYSSIZESGEOMETRYLIB    "ZSPhysSizesGeometryQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX"_"CONFIGLIBINFIX
#define ZSPHYSSIZESKINEMATICSLIB  "ZSPhysSizesKinematicsQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX"_"CONFIGLIBINFIX
#define ZSIPCLIB                  "ZSIpcQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX"_"CONFIGLIBINFIX
#define ZSIPCGUILIB               "ZSIpcGUIQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX"_"CONFIGLIBINFIX
#define ZSIPCTRACELIB             "ZSIpcTraceQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX"_"CONFIGLIBINFIX
#define ZSIPCTRACEGUILIB          "ZSIpcTraceGUIQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX"_"CONFIGLIBINFIX
#define ZSTESTLIB                 "ZSTestQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX"_"CONFIGLIBINFIX
#define ZSTESTGUILIB              "ZSTestGUIQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX"_"CONFIGLIBINFIX
#define ZSREMCMDLIB               "ZSRemCmdQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX"_"CONFIGLIBINFIX
#define ZSREMCMDGUILIB            "ZSRemCmdGUIQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX"_"CONFIGLIBINFIX
#define ZSDRAWLIB                 "ZSDrawQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX"_"CONFIGLIBINFIX
#define ZSDIAGRAMLIB              "ZSDiagramQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX"_"CONFIGLIBINFIX
#else
#define ZSSYSLIB                  "ZSSysQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX""CONFIGLIBINFIX
#define ZSSYSGUILIB               "ZSSysGUIQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX""CONFIGLIBINFIX
#define ZSPHYSVALLIB              "ZSPhysValQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX""CONFIGLIBINFIX
#define ZSPHYSVALGUILIB           "ZSPhysValGUIQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX""CONFIGLIBINFIX
#define ZSPHYSSIZESELECTRICITYLIB "ZSPhysSizesElectricityQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX""CONFIGLIBINFIX
#define ZSPHYSSIZESGEOMETRYLIB    "ZSPhysSizesGeometryQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX""CONFIGLIBINFIX
#define ZSPHYSSIZESKINEMATICSLIB  "ZSPhysSizesKinematicsQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX""CONFIGLIBINFIX
#define ZSIPCLIB                  "ZSIpcQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX""CONFIGLIBINFIX
#define ZSIPCGUILIB               "ZSIpcGUIQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX""CONFIGLIBINFIX
#define ZSIPCTRACELIB             "ZSIpcTraceQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX""CONFIGLIBINFIX
#define ZSIPCTRACEGUILIB          "ZSIpcTraceGUIQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX""CONFIGLIBINFIX
#define ZSTESTLIB                 "ZSTestQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX""CONFIGLIBINFIX
#define ZSTESTGUILIB              "ZSTestGUIQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX""CONFIGLIBINFIX
#define ZSREMCMDLIB               "ZSRemCmdQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX""CONFIGLIBINFIX
#define ZSREMCMDGUILIB            "ZSRemCmdGUIQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX""CONFIGLIBINFIX
#define ZSDRAWLIB                 "ZSDrawQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX""CONFIGLIBINFIX
#define ZSDIAGRAMLIB              "ZSDiagramQt"_ZSSYS_TOSTRING(QT_VERSION_MAJOR)QTLIBINFIX"_"COMPILERLIBINFIX"_"PLATFORMLIBINFIX""CONFIGLIBINFIX
#endif
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#define QtKeepEmptyParts Qt::KeepEmptyParts
#define QtSkipEmptyParts Qt::SkipEmptyParts
#else
#define QtKeepEmptyParts QString::KeepEmptyParts
#define QtSkipEmptyParts QString::KeepEmptyParts
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
