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

#ifndef ZSIpcTracePy_DllMain_h
#define ZSIpcTracePy_DllMain_h

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
#ifdef ZSIPCTRACEPYDLL_EXPORTS
    #define ZSIPCTRACEPYDLL_API                   __API_EXPORT
    #define ZSIPCTRACEPYDLL_EXTERN_API extern "C" __API_EXPORT
#else
    #define ZSIPCTRACEPYDLL_API                   __API_IMPORT
    #define ZSIPCTRACEPYDLL_EXTERN_API extern "C" __API_IMPORT
#endif

#endif // #ifndef ZSIpcTracePy_DllMain_h
