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

#include "ZSSysGUI/ZSSysComboBoxItemDelegate.h"

#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CComboBoxItemDelegate : public QStyledItemDelegate
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CComboBoxItemDelegate::CComboBoxItemDelegate( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QStyledItemDelegate(i_pWdgtParent)
{
} // ctor

//------------------------------------------------------------------------------
CComboBoxItemDelegate::~CComboBoxItemDelegate()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class QStyledItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
void CComboBoxItemDelegate::paint(
    QPainter* i_pPainter, const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{

}

/*==============================================================================
protected: // overridables of base class QStyledItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
bool CComboBoxItemDelegate::editorEvent(
    QEvent* i_pEv, QAbstractItemModel* i_pModel, const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx)
//------------------------------------------------------------------------------
{
    if (i_pEv->type() == QEvent::MouseButtonRelease) {
        i_pModel->setData(i_modelIdx, !i_pModel->data(i_modelIdx).toBool());
        i_pEv->accept();
    }
    return QStyledItemDelegate::editorEvent(i_pEv, i_pModel, i_option, i_modelIdx);
}
