/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSIpcTrace_DllMain_h
#define ZSIpcTrace_DllMain_h

#include "ZSIpc/ZSIpcDllMain.h"

// specific export defines
#ifdef ZSIPCTRACEDLL_EXPORTS
    #define ZSIPCTRACEDLL_API                   __API_EXPORT
    #define ZSIPCTRACEDLL_EXTERN_API extern "C" __API_EXPORT
#else
    #define ZSIPCTRACEDLL_API                   __API_IMPORT
    #define ZSIPCTRACEDLL_EXTERN_API extern "C" __API_IMPORT
#endif

#endif // #ifndef ZSIpcTrace_DllMain_h
