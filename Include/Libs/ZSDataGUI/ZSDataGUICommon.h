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

#ifndef ZSDataGUICommon_DllMain_h
#define ZSDataGUICommon_DllMain_h

#include <QtCore/qsize.h>

#include "ZSDataGUI/ZSDataGUIDllMain.h"


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace Data
{
namespace GUI
{
ZSDATAGUIDLL_API QPixmap dataSetObjType2Pixmap( int i_iType, const QSize& i_sz = QSize(16,16) );

} // namespace GUI

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSDataGUICommon_DllMain_h
