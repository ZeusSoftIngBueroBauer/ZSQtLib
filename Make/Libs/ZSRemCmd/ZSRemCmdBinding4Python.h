/*******************************************************************************

Copyright 2004 - 2017 by    ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSRemCmd_Binding4Python_h
#define ZSRemCmd_Binding4Python_h

#undef QT_NO_STL
#undef QT_NO_STL_WCHAR

#ifndef NULL
#define NULL 0
#endif

#include <pyside_global.h>

#include "ZSSys/ZSSysBinding4Python.h"
#include "ZSIpc/ZSIpcBinding4Python.h"
#include "ZSTrace/ZSTraceBinding4Python.h"

#include "ZSRemCmd/ZSRemCmdBinding4PythonGlobalFunctionsWrapper.h"

#include "ZSRemCmd/ZSRemCmdDllMain.h"
#include "ZSRemCmd/ZSRemCmdErrNumbers.h"
#include "ZSRemCmd/ZSRemCmdMsg.h"
#include "ZSRemCmd/ZSRemCmdObj.h"
#include "ZSRemCmd/ZSRemCmdSubSysNode.h"
#include "ZSRemCmd/ZSRemCmdObjPool.h"
#include "ZSRemCmd/ZSRemCmdParser.h"
#include "ZSRemCmd/ZSRemCmdClient.h"
#include "ZSRemCmd/ZSRemCmdServer.h"

#endif // #ifndef ZSRemCmd_Binding4Python_h
