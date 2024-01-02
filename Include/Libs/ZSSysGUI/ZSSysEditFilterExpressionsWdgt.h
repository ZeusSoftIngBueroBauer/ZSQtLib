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

#ifndef ZSSysGUI_EditFilterExpressionsWdgt_h
#define ZSSysGUI_EditFilterExpressionsWdgt_h

#include "ZSSysGUI/ZSSysGUIDllMain.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

class QHBoxLayout;
class QVBoxLayout;
class QPushButton;

namespace ZS
{
namespace System
{
namespace GUI
{
class CTableView;

//******************************************************************************
class ZSSYSGUIDLL_API CWdgtEditFilterExpressions : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::System::GUI"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtEditFilterExpressions"; }
public: // ctors and dtor
    CWdgtEditFilterExpressions(QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtEditFilterExpressions();
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    bool hasErrors() const;
    bool hasChanges() const;
    void acceptChanges();
    void rejectChanges();
protected slots:
    void onBtnResizeRowsAndColumnsToContentsClicked(bool i_bChecked = false);
    void onBtnAddFilterExpressionClicked(bool i_bChecked = false);
    void onBtnRemoveFilterExpressionClicked(bool i_bChecked = false);
private: // instance members
    QVBoxLayout* m_pLyt;
    QWidget* m_pWdgtFilterExpressions;
    QVBoxLayout* m_pLytWdgtFilterExpressions;
    QHBoxLayout* m_pLytLineEditButtons;
    QPushButton* m_pBtnResizeRowsAndColumnsToContents;
    QPushButton* m_pBtnAddLabel;
    QPushButton* m_pBtnRemoveLabels;
    QVBoxLayout* m_pLytTableView;
    CTableView* m_pTableView;

}; // class CWdgtEditFilterExpressions

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_EditFilterExpressionsWdgt_h
