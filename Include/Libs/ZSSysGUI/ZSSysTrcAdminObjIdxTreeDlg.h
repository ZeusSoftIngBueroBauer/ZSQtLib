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

#ifndef ZSSysGUI_TrcAdminObjIdxTreeDlg_h
#define ZSSysGUI_TrcAdminObjIdxTreeDlg_h

#include <QtCore/qglobal.h>

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSysGUI/ZSSysDialog.h"
#include "ZSSys/ZSSysAux.h"

class QVBoxLayout;

namespace ZS
{
namespace System
{
class CIdxTreeTrcAdminObjs;
class CTrcAdminObj;

namespace GUI
{
class CWdgtIdxTreeTrcAdminObjs;

//******************************************************************************
class ZSSYSGUIDLL_API CDlgIdxTreeTrcAdminObjs : public ZS::System::GUI::CDialog
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CDlgIdxTreeTrcAdminObjs"; }
public: // class CDlgTrcServer
    static CDlgIdxTreeTrcAdminObjs* CreateInstance(
        const QString& i_strDlgTitle,
        CIdxTreeTrcAdminObjs* i_pIdxTree,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
    static CDlgIdxTreeTrcAdminObjs* GetInstance( const QString& i_strObjName );
protected: // ctor
    CDlgIdxTreeTrcAdminObjs(
        const QString& i_strDlgTitle,
        CIdxTreeTrcAdminObjs* i_pIdxTree,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
public: // dtor
    virtual ~CDlgIdxTreeTrcAdminObjs();
public: // overridables of base class ZS::System::GUI::CDialog
    virtual QString nameSpace() override { return NameSpace(); }
    virtual QString className() override { return ClassName(); }
protected: // instance members
    CIdxTreeTrcAdminObjs* m_pIdxTree;
    QVBoxLayout* m_pLyt;
    CWdgtIdxTreeTrcAdminObjs* m_pWdgtIdxTree;
    /*!< Trace admin object to control trace outputs of the class.
         The object will not be created if the index tree's belongs to the trace server. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CDlgIdxTreeTrcAdminObjs

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_TrcAdminObjIdxTreeDlg_h
