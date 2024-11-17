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

#ifndef ZSDraw_DrawGraphObjPolygonPropertiesDlg_h
#define ZSDraw_DrawGraphObjPolygonPropertiesDlg_h

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPolygonPropertiesWdgt.h"
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
class ZSDRAWDLL_API CDlgGraphObjPolygonProperties : public ZS::System::GUI::CDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CDlgGraphObjPolygonProperties"; }
public: // class methods
    static CDlgGraphObjPolygonProperties* CreateInstance(
        const QString&     i_strDlgTitle,
        CGraphObjPolyline* i_pGraphObjPolyline,
        QWidget*           i_pWdgtParent = nullptr,
        Qt::WindowFlags    i_wflags = Qt::WindowFlags());
    static CDlgGraphObjPolygonProperties* GetInstance(CGraphObjPolyline* i_pGraphObjPolyline);
protected: // ctor
    CDlgGraphObjPolygonProperties(
        const QString&     i_strDlgTitle,
        CGraphObjPolyline* i_pGraphObjPolyline,
        QWidget*           i_pWdgtParent = nullptr,
        Qt::WindowFlags    i_wFlags = Qt::WindowFlags());
public: // dtor
    virtual ~CDlgGraphObjPolygonProperties();
public: // instance methods
    void setCurrentWidget(CWdgtGraphObjPolygonProperties::EWidget i_widget);
protected slots: // instance methods
    void onBtnOkClicked(bool i_bChecked = false);
    void onBtnApplyClicked(bool i_bChecked = false);
    void onBtnResetClicked(bool i_bChecked = false);
    void onBtnCancelClicked(bool i_bChecked = false);
protected slots:
    void onWdgtPolygonSettingsContentChanged();
    void onGraphObjDestroyed(QObject*);
private: // instance members
    CGraphObjPolyline* m_pGraphObjPolyline;
    QVBoxLayout* m_pLyt;
    QScrollArea* m_pScrollArea;
    CWdgtGraphObjPolygonProperties* m_pWdgtPolygonSettings;
    // Buttons
    QHBoxLayout* m_pLytLineBtns;
    QPushButton* m_pBtnOk;
    QPushButton* m_pBtnApply;
    QPushButton* m_pBtnReset;
    QPushButton* m_pBtnCancel;

}; // class CDlgGraphObjPolygonProperties

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawGraphObjPolygonPropertiesDlg_h
