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

#ifndef ZSPhysValGUI_EditItemDelegate_h
#define ZSPhysValGUI_EditItemDelegate_h

#include "ZSPhysValGUI/ZSPhysValGUIDllMain.h"
#include "ZSPhysVal/ZSPhysVal.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qstyleditemdelegate.h>
#else
#include <QtWidgets/qstyleditemdelegate.h>
#endif

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}
namespace PhysVal
{
namespace GUI
{
class CWdgtEditPhysVal;

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
    combo box items via the data method as a list of SComboBoxItems.
    As default the combo box delegate will use Qt::AccessibleTextRole" to get the
    list of combo box items. The AccessibleTextRole is used as this role
    is rarely used and is a predefined role. This default role can be changed
    to any other role by invoking the "setItemDataRole".

    @note The data method for the column the combo box is assigned to, must return
          a QString for the value (the current text of the combo box) instead of
          an integer value (the current index of the combo box). Returning the
          current text is more flexible than returning an index value as the list of
          items may address the same enum but some of the enums may not be selectable.
*/
class ZSPHYSVALGUIDLL_API CEditPhysValtemDelegate : public QStyledItemDelegate
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CEditPhysValtemDelegate"; }
public: // ctors and dtor
    CEditPhysValtemDelegate(QAbstractItemModel* i_pModel, QWidget* i_pWdgtParent = nullptr);
    virtual ~CEditPhysValtemDelegate();
public: // instance methods
    void setItemDataRoleMinimumValue(int i_iRole);
    int itemDataRoleMinimumValue() const;
    void setItemDataRoleMaximumValue(int i_iRole);
    int itemDataRoleMaximumValue() const;
public: // overridables of base class QStyledItemDelegate
    QWidget* createEditor(QWidget* i_pWdgtParent, const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx) const override;
    void setEditorData(QWidget* i_pWdgtEditor, const QModelIndex& i_modelIdx) const override;
    void setModelData(QWidget* i_pWdgtEditor, QAbstractItemModel* i_pModel, const QModelIndex& i_modelIdx) const override;
    QSize sizeHint(const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx) const override;
    void updateEditorGeometry(QWidget* i_pWdgtEditor, const QStyleOptionViewItem& i_option, const QModelIndex& i_modelIdx) const override;
protected slots:
    void onCloseEditor(QWidget* i_pWdgtEditor, QAbstractItemDelegate::EndEditHint i_hint = NoHint);
    void onEditPhysValValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
protected: // instance members
    /*!< Pointer to the model the delegate is assigned to. Passed by the constructor. */
    QAbstractItemModel* m_pModel;
    /*!< Item data role to query the minimum value from the model.
         Defaults to Qt::UserRole. */
    int m_iItemDataRoleMinVal;
    /*!< Item data role to query the maximum value from the model.
         Defaults to Qt::UserRole + 1. */
    int m_iItemDataRoleMaxVal;
    /*!< Widget to edit the physical value. Created by "createEditor" method.
         Stored in member variable for the sizeHint method. */
    mutable CWdgtEditPhysVal* m_pWdgtEditPhysVal;
    /*!< Model index at which the editor was created.
         Needed to set the model data if the current value is changed. */
    mutable QModelIndex m_modelIdxEditorCreated;
    /*!< Trace admin object for method tracing. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    /*!< Trace admin object for noisy methods like "paint". */
    ZS::System::CTrcAdminObj* m_pTrcAdminObjNoisyMethods;

}; // class ComboBoxItemDelegate

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_EditItemDelegate_h
