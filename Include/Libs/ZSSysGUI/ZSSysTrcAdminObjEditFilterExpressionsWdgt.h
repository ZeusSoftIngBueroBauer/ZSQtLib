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

#ifndef ZSSysGUI_TrcAdminObjEditFilterExpressionsWdgt_h
#define ZSSysGUI_TrcAdminObjEditFilterExpressionsWdgt_h

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysCommon.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

class QLabel;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
class CTrcAdminObj;

namespace GUI
{
class CSepLine;
class CTableView;

//******************************************************************************
class ZSSYSGUIDLL_API CWdgtTrcAdminObjEditFilterExpressions : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::System::GUI"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtTrcAdminObjEditFilterExpressions"; }
public: // ctors and dtor
    CWdgtTrcAdminObjEditFilterExpressions(QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtTrcAdminObjEditFilterExpressions();
public: // instance methods
    void setTraceAdminObj(CTrcAdminObj* i_pTrcAdminObj);
    void setFilterToEdit(EMethodTraceFilterProperty i_filter);
public: // instance methods
    bool hasErrors() const;
    bool hasChanges() const;
    void acceptChanges();
    void rejectChanges();
protected slots:
    void onBtnResizeRowsAndColumnsToContentsClicked(bool i_bChecked = false);
    void onBtnAddFilterExpressionClicked(bool i_bChecked = false);
    void onBtnRemoveFilterExpressionClicked(bool i_bChecked = false);
private: // auxiliary instance methods
    void setHeadlineText();
private: // instance members
    CTrcAdminObj* m_pTrcAdminObj;
    CEnumMethodTraceFilterProperty m_eFilter;
    QVBoxLayout* m_pLyt;
    QHBoxLayout* m_pLytLineTrcAdminObjPath;
    QLabel* m_pLblTrcAdminObjPath;
    ZS::System::GUI::CSepLine* m_pSepLineTrcAdminObjPath;
    QWidget* m_pWdgtFilterExpressions;
    QVBoxLayout* m_pLytWdgtFilterExpressions;
    QHBoxLayout* m_pLytLineEditButtons;
    QPushButton* m_pBtnResizeRowsAndColumnsToContents;
    QPushButton* m_pBtnAddLabel;
    QPushButton* m_pBtnRemoveLabels;
    QLabel* m_pLblFilterProperty;
    QVBoxLayout* m_pLytTableView;
    CTableView* m_pTableView;

}; // class CWdgtTrcAdminObjEditFilterExpressions

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_TrcAdminObjEditFilterExpressionsWdgt_h
