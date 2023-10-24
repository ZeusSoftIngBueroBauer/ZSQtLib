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

#include "ZSSysGUI/ZSSysCheckBoxItemDelegate.h"

#include <QtWidgets/qapplication.h>
#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CheckBoxItemDelegate : public QStyledItemDelegate
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CCheckBoxItemDelegate::CCheckBoxItemDelegate( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QStyledItemDelegate(i_pWdgtParent)
{
} // ctor

//------------------------------------------------------------------------------
CCheckBoxItemDelegate::~CCheckBoxItemDelegate()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class QStyledItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
void CCheckBoxItemDelegate::paint(
    QPainter* i_pPainter, const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    QStyleOptionButton cbOpt;

    // Centered as default. If this should be changed a call to i_modelIdx.data
    // with Qt::TextAlignmentRole should be added and the model must provide the
    // desired alignment flags.
    cbOpt.rect = QRect(
        /* x      */ i_option.rect.center().x() - cbOpt.rect.height() / 2 - 2,
        /* y      */ i_option.rect.center().y() - cbOpt.rect.height() / 2 + 2,
        /* width  */ cbOpt.rect.height() / 2 - 4,
        /* height */ cbOpt.rect.height() / 2 - 4);

    bool isChecked = i_modelIdx.data(Qt::DisplayRole).toBool();
    if (isChecked) {
        cbOpt.state |= QStyle::State_On;
    }
    else {
        cbOpt.state |= QStyle::State_Off;
    }
    QApplication::style()->drawControl(QStyle::CE_CheckBox, &cbOpt, i_pPainter);
}

/*==============================================================================
protected: // overridables of base class QStyledItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
bool CCheckBoxItemDelegate::editorEvent(
    QEvent* i_pEv, QAbstractItemModel* i_pModel, const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx)
//------------------------------------------------------------------------------
{
    bool bDataSet = false;
    if (i_pEv->type() == QEvent::MouseButtonRelease) {
        bool bVal = i_modelIdx.data(Qt::DisplayRole).toBool();
        // invert checkbox state
        bDataSet = i_pModel->setData(i_modelIdx, !bVal, Qt::EditRole);
    }
    return bDataSet;
}
