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

#ifndef ZSSys_Binding4Python_h
#define ZSSys_Binding4Python_h

#undef QT_NO_STL
#undef QT_NO_STL_WCHAR

#ifndef NULL
#define NULL 0
#endif

#include <pyside_global.h>

#include "ZSSys/ZSSysBinding4PythonGlobalFunctionsWrapper.h"

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnum.h"
#include "ZSSys/ZSSysEnumVal.h"
#include "ZSSys/ZSSysEnumValArr.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysLogFile.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysMsgProtocol.h"
#include "ZSSys/ZSSysObjPool.h"
#include "ZSSys/ZSSysRequest.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysRequestQueue.h"
#include "ZSSys/ZSSysRequestSequencer.h"
#include "ZSSys/ZSSysSleeperThread.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcAdminObjPool.h"
#include "ZSSys/ZSSysTrcData.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcMthFile.h"
#include "ZSSys/ZSSysTrcServer.h"
#include "ZSSys/ZSSysVersion.h"

#endif // #ifndef ZSSys_Binding4Python_h
