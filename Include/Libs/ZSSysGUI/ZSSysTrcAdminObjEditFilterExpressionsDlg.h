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

#ifndef ZSSysGUI_TrcAdminObjEditFilterExpressionsDlg_h
#define ZSSysGUI_TrcAdminObjEditFilterExpressionsDlg_h

#include "ZSSysGUI/ZSSysDialog.h"
#include "ZSSys/ZSSysCommon.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#else
#endif

class QHBoxLayout;
class QVBoxLayout;
class QPushButton;

namespace ZS
{
namespace System
{
class CTrcAdminObj;

namespace GUI
{
class CWdgtTrcAdminObjEditFilterExpressions;

//******************************************************************************
class ZSSYSGUIDLL_API CDlgTrcAdminObjEditFilterExpressions : public CDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CDlgTrcAdminObjEditFilterExpressions"; }
public: // class methods
    static CDlgTrcAdminObjEditFilterExpressions* CreateInstance(
        const QString& i_strDlgTitle,
        const QString& i_strObjName,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
    static CDlgTrcAdminObjEditFilterExpressions* GetInstance( const QString& i_strObjName );
protected: // ctor
    CDlgTrcAdminObjEditFilterExpressions(
        const QString& i_strDlgTitle,
        const QString& i_strObjName,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
signals:
    //void applied();
    //void accepted(); inherited from QDialog
    //void rejected(); inherited from QDialog
public: // dtor
    virtual ~CDlgTrcAdminObjEditFilterExpressions();
public: // instance methods
    void setTraceAdminObj(CTrcAdminObj* i_pTrcAdminObj);
    void setFilterToEdit(EMethodTraceFilterProperty i_filter);
protected slots:
    void onBtnOkClicked( bool i_bChecked );
    void onBtnApplyClicked( bool i_bChecked );
    void onBtnResetClicked( bool i_bChecked );
    void onBtnCancelClicked( bool i_bChecked );
protected slots:
    void onWdgtEditFilterExpressionsContentChanged();
protected: // instance members
    CTrcAdminObj* m_pTrcAdminObj;
    EMethodTraceFilterProperty m_filter;
    QVBoxLayout* m_pLyt;
    CWdgtTrcAdminObjEditFilterExpressions* m_pWdgtEditFilterExpressions;
    QHBoxLayout* m_pLytLineBtns;
    QPushButton* m_pBtnOk;
    QPushButton* m_pBtnApply;
    QPushButton* m_pBtnReset;
    QPushButton* m_pBtnCancel;

}; // class CDlgTrcAdminObjEditFilterExpressions

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_TrcAdminObjEditFilterExpressionsDlg_h
