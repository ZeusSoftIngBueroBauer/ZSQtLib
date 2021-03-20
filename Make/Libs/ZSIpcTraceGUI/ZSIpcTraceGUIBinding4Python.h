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

#ifndef ZSTraceGUI_Binding4Python_h
#define ZSTraceGUI_Binding4Python_h

#undef QT_NO_STL
#undef QT_NO_STL_WCHAR

#ifndef NULL
#define NULL 0
#endif

#include <pyside_global.h>

#include "ZSSys/ZSSysBinding4Python.h"
#include "ZSIpc/ZSIpcBinding4Python.h"
#include "ZSTrace/ZSTraceBinding4Python.h"

#include "ZSSysGUI/ZSSysGUIBinding4Python.h"
#include "ZSIpcGUI/ZSIpcGUIBinding4Python.h"

#include "ZSTraceGUI/ZSTraceGUIBinding4PythonGlobalFunctionsWrapper.h"

#include "ZSTraceGUI/ZSTrcGUIDllMain.h"
#include "ZSTraceGUI/ZSTrcAdminObjPoolDlg.h"
#include "ZSTraceGUI/ZSTrcAdminObjPoolModel.h"
#include "ZSTraceGUI/ZSTrcAdminObjPoolWdgt.h"
#include "ZSTraceGUI/ZSTrcAdminObjPoolWdgtNameSpaceContent.h"
#include "ZSTraceGUI/ZSTrcAdminObjPoolWdgtNameSpaces.h"
#include "ZSTraceGUI/ZSTrcClientDlg.h"
#include "ZSTraceGUI/ZSTrcServerDlg.h"
#include "ZSTraceGUI/ZSTrcCtxModel.h"
#include "ZSTraceGUI/ZSTrcCtxWdgt.h"
#include "ZSTraceGUI/ZSTrcMsgModel.h"
#include "ZSTraceGUI/ZSTrcMsgWdgt.h"
#include "ZSTraceGUI/ZSTrcMthModel.h"
#include "ZSTraceGUI/ZSTrcMthWdgt.h"
#include "ZSTraceGUI/ZSTrcSettingsWdgt.h"

#endif // #ifndef ZSTraceGUI_Binding4Python_h
