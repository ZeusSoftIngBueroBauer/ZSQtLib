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

#ifndef ZSSys_DllMain_h
#define ZSSys_DllMain_h

#include <QtCore/qglobal.h>

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

#endif // #ifndef ZSSys_DllMain_h
