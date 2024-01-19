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

#ifndef ZSDiagram_ItemPropertiesDlg_h
#define ZSDiagram_ItemPropertiesDlg_h

#include "ZSDiagramGUI/ZSDiagramGUIDllMain.h"
#include "ZSSysGUI/ZSSysDialog.h"

class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace Diagram
{
class CDiagObj;
class CDiagScale;
class CDiagTrace;
class CWdgtDiagram;

namespace GUI
{
class CWdgtDiagramItemPropertiesBase;

//******************************************************************************
class ZSDIAGRAMGUIDLL_API CDlgDiagramItemProperties : public ZS::System::GUI::CDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Diagram::GUI"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CDlgDiagramItemProperties"; }
public: // class methods
    static CDlgDiagramItemProperties* CreateInstance(
        const QString&  i_strDlgTitle,
        CWdgtDiagram*   i_pDiagram,
        const QString&  i_strDiagItemClassName,
        const QString&  i_strDiagItemObjName,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags());
    static CDlgDiagramItemProperties* GetInstance(
        const QString& i_strDiagItemClassName,
        const QString& i_strDiagItemObjName);
protected: // ctor
    CDlgDiagramItemProperties(
        const QString&  i_strDlgTitle,
        CWdgtDiagram*   i_pDiagram,
        const QString&  i_strDiagItemClassName,
        const QString&  i_strDiagItemObjName,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
public: // dtor
    virtual ~CDlgDiagramItemProperties();
protected slots: // instance methods
    void onBtnOkClicked(bool i_bChecked = false);
    void onBtnApplyClicked(bool i_bChecked = false);
    void onBtnResetClicked(bool i_bChecked = false);
    void onBtnCancelClicked(bool i_bChecked = false);
protected slots:
    void onWdgtModeChanged();
    void onWdgtDiagItemDestroyed(QObject* i_pObj);
    void onWdgtDiagItemObjNameChanged();
    void onWdgtDiagItemPropertyChanged();
protected: // auxiliary instance methods
    CWdgtDiagramItemPropertiesBase* createDiagItemPropertiesWdgt();
private: // instance members
    CWdgtDiagram* m_pDiagram;
    QString m_strDiagItemClassName;
    QString m_strDiagItemObjName;
    QVBoxLayout* m_pLyt;
    CWdgtDiagramItemPropertiesBase* m_pWdgtDiagItemProperties;
    QHBoxLayout* m_pLytLineBtns;
    QPushButton* m_pBtnOk;
    QPushButton* m_pBtnApply;
    QPushButton* m_pBtnReset;
    QPushButton* m_pBtnCancel;

}; // CDlgDiagramItemProperties

} // namespace GUI

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagram_ItemPropertiesDlg_h
