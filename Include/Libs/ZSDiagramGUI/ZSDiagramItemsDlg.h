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

#ifndef ZSDiagram_ItemsDlg_h
#define ZSDiagram_ItemsDlg_h

#include "ZSDiagramGUI/ZSDiagramGUIDllMain.h"
#include "ZSSysGUI/ZSSysDialog.h"

class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace Diagram
{
class CWdgtDiagram;

namespace GUI
{
class CWdgtDiagramItems;

//******************************************************************************
class ZSDIAGRAMGUIDLL_API CDlgDiagramItems : public ZS::System::GUI::CDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Diagram::GUI"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CDlgDiagramItems"; }
public: // class methods
    static CDlgDiagramItems* CreateInstance(
        const QString&  i_strDlgTitle,
        CWdgtDiagram*   i_pDiagram,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags());
    static CDlgDiagramItems* GetInstance( CWdgtDiagram* i_pDiagram );
protected: // ctor
    CDlgDiagramItems(
        const QString&  i_strDlgTitle,
        CWdgtDiagram*   i_pDiagram,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
public: // dtor
    virtual ~CDlgDiagramItems();
protected slots:
    virtual void onDiagramAboutToBeDestroyed(const QString& i_strObjName);
private: // instance members
    CWdgtDiagram* m_pDiagram;
    QVBoxLayout* m_pLyt;
    CWdgtDiagramItems* m_pWdgtDiagramObjs;

}; // CDlgDiagramItems

} // namespace GUI

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagram_ItemsDlg_h
