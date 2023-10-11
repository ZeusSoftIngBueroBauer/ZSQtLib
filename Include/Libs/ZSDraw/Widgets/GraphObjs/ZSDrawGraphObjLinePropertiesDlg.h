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

#ifndef ZSDraw_DrawGraphObjLinePropertiesDlg_h
#define ZSDraw_DrawGraphObjLinePropertiesDlg_h

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLinePropertiesWdgt.h"
#include "ZSSysGUI/ZSSysDialog.h"

class QPushButton;
class QScrollArea;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace Draw
{
class CDrawingView;

//******************************************************************************
class ZSDRAWDLL_API CDlgGraphObjLineProperties : public ZS::System::GUI::CDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CDlgGraphObjLineProperties"; }
public: // class methods
    static CDlgGraphObjLineProperties* CreateInstance(
        const QString&  i_strDlgTitle,
        CGraphObjLine*  i_pGraphObjLine,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags());
    static CDlgGraphObjLineProperties* GetInstance(CGraphObjLine* i_pGraphObjLine);
protected: // ctor
    CDlgGraphObjLineProperties(
        const QString& i_strDlgTitle,
        CGraphObjLine*  i_pGraphObjLine,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
public: // dtor
    virtual ~CDlgGraphObjLineProperties();
public: // instance methods
    void setCurrentWidget(CWdgtGraphObjLineProperties::EWidget i_widget);
protected slots: // instance methods
    void onBtnOkClicked(bool i_bChecked = false);
    void onBtnApplyClicked(bool i_bChecked = false);
    void onBtnResetClicked(bool i_bChecked = false);
    void onBtnCancelClicked(bool i_bChecked = false);
protected slots:
    void onWdgtLineSettingsContentChanged();
    void onGraphObjDestroyed(QObject*);
private: // instance members
    CGraphObjLine* m_pGraphObjLine;
    QVBoxLayout* m_pLyt;
    QScrollArea* m_pScrollArea;
    CWdgtGraphObjLineProperties* m_pWdgtLineSettings;
    // Buttons
    QHBoxLayout* m_pLytLineBtns;
    QPushButton* m_pBtnOk;
    QPushButton* m_pBtnApply;
    QPushButton* m_pBtnReset;
    QPushButton* m_pBtnCancel;

}; // class CDlgGraphObjLineProperties

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawGraphObjLinePropertiesDlg_h
