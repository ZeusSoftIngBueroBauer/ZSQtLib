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

#include <QtWidgets/qapplication.h>
#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#if QT_VERSION < 0x050000
#include <QtGui/qcombobox.h>
#else
#include <QtWidgets/qcombobox.h>
#endif

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
    QStyledItemDelegate(i_pWdgtParent),
    m_iItemDataRole(Qt::AccessibleTextRole)
{
} // ctor

//------------------------------------------------------------------------------
CComboBoxItemDelegate::~CComboBoxItemDelegate()
//------------------------------------------------------------------------------
{
    m_iItemDataRole = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CComboBoxItemDelegate::setItemDataRole(int i_iRole)
//------------------------------------------------------------------------------
{
    m_iItemDataRole = i_iRole;
}

//------------------------------------------------------------------------------
int CComboBoxItemDelegate::itemDataRole() const
//------------------------------------------------------------------------------
{
    return m_iItemDataRole;
}

/*==============================================================================
public: // overridables of base class QStyledItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
void CComboBoxItemDelegate::paint(
    QPainter* i_pPainter, const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    QStyleOptionViewItem option = i_option;
    option.text = i_modelIdx.data(Qt::DisplayRole).toString();
    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &option, i_pPainter);
}

//------------------------------------------------------------------------------
QWidget* CComboBoxItemDelegate::createEditor(
    QWidget* i_pWdgtParent, const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    QComboBox* pCmb = new QComboBox(i_pWdgtParent);
    QList<SComboBoxItem> arItems = i_modelIdx.data(m_iItemDataRole).value<QList<SComboBoxItem>>();
    for(const SComboBoxItem& item : arItems) {
        if (!item.m_icon.isNull()) {
            pCmb->addItem(item.m_icon, item.m_strText, item.m_userData);
        }
        else {
            pCmb->addItem(item.m_strText, item.m_userData);
        }
    }
    return pCmb;
}

//------------------------------------------------------------------------------
void CComboBoxItemDelegate::setEditorData(
    QWidget* i_pWdgtEditor, const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    QComboBox* pCmb = static_cast<QComboBox*>(i_pWdgtEditor);
    int idxCmb = i_modelIdx.model()->data(i_modelIdx, Qt::EditRole).toInt();
    pCmb->setCurrentIndex(idxCmb);
}

//------------------------------------------------------------------------------
void CComboBoxItemDelegate::setModelData(
    QWidget* i_pWdgtEditor, QAbstractItemModel* i_pModel, const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    QComboBox* pCmb = static_cast<QComboBox*>(i_pWdgtEditor);
    i_pModel->setData(i_modelIdx, pCmb->currentText(), Qt::EditRole);
}

//------------------------------------------------------------------------------
void CComboBoxItemDelegate::updateEditorGeometry(
    QWidget* i_pWdgtEditor, const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    i_pWdgtEditor->setGeometry(i_option.rect);
}
