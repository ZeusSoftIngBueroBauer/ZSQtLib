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

#include "QtGui/qbitmap.h"

#include "ZSDataGUI/ZSDataGUICommon.h"
#include "ZSData/ZSDataCommon.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::Data;
using namespace ZS::Data::GUI;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
Enum EMode
==============================================================================*/

//------------------------------------------------------------------------------
QPixmap ZS::Data::GUI::dataSetObjType2Pixmap( int i_iType, const QSize& i_sz )
//------------------------------------------------------------------------------
{
    QString strObjType = dataSetObjType2Str(i_iType);

    QPixmap pxm = QPixmap(":/ZS/Data/DSObjTypes/" + strObjType);

    pxm = pxm.scaled(i_sz.width(), i_sz.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pxm.setMask(pxm.createHeuristicMask());

    return pxm;

} // mode2Pixmap
