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

#ifndef ZSDraw_DlgDrawingViewSetup_h
#define ZSDraw_DlgDrawingViewSetup_h

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSSysGUI/ZSSysDialog.h"

class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace Draw
{
class CDrawGridSettings;
class CDrawingSize;
class CDrawingView;
class CWdgtDrawingViewProperties;

//******************************************************************************
class ZSDRAWDLL_API CDlgDrawingViewSetup : public ZS::System::GUI::CDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CDlgDrawingViewSetup"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // class methods
    static CDlgDrawingViewSetup* CreateInstance(
        const QString&  i_strDlgTitle,
        CDrawingView*   i_pDrawingView,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags());
    static CDlgDrawingViewSetup* GetInstance( CDrawingView* i_pDrawingView );
protected: // ctor
    CDlgDrawingViewSetup(
        const QString&  i_strDlgTitle,
        CDrawingView*   i_pDrawingView,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
public: // dtor
    virtual ~CDlgDrawingViewSetup();
public: // overridables
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const { return ClassName(); }
protected slots: // instance methods
    void onBtnOkClicked(bool i_bChecked = false);
    void onBtnApplyClicked(bool i_bChecked = false);
    void onBtnResetClicked(bool i_bChecked = false);
    void onBtnCancelClicked(bool i_bChecked = false);
protected slots:
    void onWdgtDrawingViewPropertiesContentChanged();
private: // instance members
    CDrawingView* m_pDrawingView;
    QVBoxLayout* m_pLyt;
    CWdgtDrawingViewProperties* m_pWdgtDrawingViewProperties;
    QHBoxLayout* m_pLytLineBtns;
    QPushButton* m_pBtnOk;
    QPushButton* m_pBtnApply;
    QPushButton* m_pBtnReset;
    QPushButton* m_pBtnCancel;

}; // CDlgDrawingViewSetup

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DlgDrawingViewSetup_h
