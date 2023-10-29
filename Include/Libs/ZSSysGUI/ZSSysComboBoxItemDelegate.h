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

#ifndef ZSSysGUI_ComboBoxItemDelegate_h
#define ZSSysGUI_ComboBoxItemDelegate_h

#include "ZSSysGUI/ZSSysGUIDllMain.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qstyleditemdelegate.h>
#else
#include <QtWidgets/qstyleditemdelegate.h>
#endif

namespace ZS
{
namespace System
{
namespace GUI
{
//******************************************************************************
struct SComboBoxItem
//******************************************************************************
{
    SComboBoxItem() {}
    SComboBoxItem(const QString& i_strText, const QVariant& i_userData = QVariant()) :
        m_strText(i_strText), m_icon(), m_userData(i_userData)
    {}
    SComboBoxItem(const QIcon& i_icon, const QString& i_strText, const QVariant& i_userData = QVariant()) :
        m_strText(i_strText), m_icon(i_icon), m_userData(i_userData)
    {}
    QString m_strText;
    QIcon m_icon;
    QVariant m_userData;
};

Q_DECLARE_METATYPE(SComboBoxItem);
Q_DECLARE_METATYPE(QList<SComboBoxItem>);

//******************************************************************************
/*! @brief Delegate for a combo box in an item view.

    To use this delegate the model assigned to the item view must return the
    combo box items via the data method. The item data role for which the list
    items has to be returned must be set by invoking the "setItemDataRole".
    As default the combo box delegate will use Qt::AccessibleTextRole".
*/
class ZSSYSGUIDLL_API CComboBoxItemDelegate : public QStyledItemDelegate
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CComboBoxItemDelegate( QWidget* i_pWdgtParent = nullptr );
    virtual ~CComboBoxItemDelegate();
public: // instance methods
    void setItemDataRole(int i_iRole);
    int itemDataRole() const;
public: // overridables of base class QStyledItemDelegate
    void paint(QPainter* i_pPainter, const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx) const override;
    QWidget* createEditor(QWidget* i_pWdgtParent, const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx) const override;
    void setEditorData(QWidget* i_pWdgtEditor, const QModelIndex& i_modelIdx) const override;
    void setModelData(QWidget* i_pWdgtEditor, QAbstractItemModel* i_pModel, const QModelIndex& i_modelIdx) const override;
    void updateEditorGeometry(QWidget* i_pWdgtEditor, const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx) const override;
protected: // instance members
    int m_iItemDataRole;

}; // class ComboBoxItemDelegate

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_ComboBoxItemDelegate_h
