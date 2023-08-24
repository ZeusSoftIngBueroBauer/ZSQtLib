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

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#else
#include <QtWidgets/qlayout.h>
#endif

#include "ZSSysGUI/ZSSysPushButtonCloseWidget.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CPushButtonCloseWidget : public QPushButton
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

static const SEnumEntry s_arEnumStrSizes[] =
{
    /*  0 */ SEnumEntry( CPushButtonCloseWidget::ESize16x16,     "16x16"     ),
    /*  1 */ SEnumEntry( CPushButtonCloseWidget::ESize24x24,     "24x24"     ),
    /*  2 */ SEnumEntry( CPushButtonCloseWidget::ESizeCount,     "Count"     ),
    /*  3 */ SEnumEntry( CPushButtonCloseWidget::ESizeUndefined, "Undefined" )
};

//------------------------------------------------------------------------------
QString CPushButtonCloseWidget::size2Str( int i_size )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_size >= 0 && i_size < _ZSArrLen(s_arEnumStrSizes) )
    {
        str = SEnumEntry::enumerator2Str(s_arEnumStrSizes, _ZSArrLen(s_arEnumStrSizes), i_size);
    }
    else
    {
        str = "? (" + QString::number(i_size) + ")";
    }
    return str;

} // size2Str

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPushButtonCloseWidget::CPushButtonCloseWidget( ESize i_size, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QPushButton(i_pWdgtParent)
{
    QString strSize = size2Str(i_size);

    QIcon   iconBtnCloseWidget;
    QPixmap pxmBtnCloseWidget(":/ZS/Button/ButtonClose" + strSize);

    pxmBtnCloseWidget.setMask(pxmBtnCloseWidget.createHeuristicMask());
    iconBtnCloseWidget.addPixmap(pxmBtnCloseWidget);

    setIcon(iconBtnCloseWidget);

    setStyleSheet("border: none;");

} // ctor

//------------------------------------------------------------------------------
CPushButtonCloseWidget::~CPushButtonCloseWidget()
//------------------------------------------------------------------------------
{
} // dtor
