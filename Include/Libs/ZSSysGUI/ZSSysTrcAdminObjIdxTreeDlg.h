/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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
namespace Trace
{
class CIdxTreeTrcAdminObjs;

namespace GUI
{
class CWdgtIdxTreeTrcAdminObjs;

//******************************************************************************
class ZSSYSGUIDLL_API CDlgIdxTreeTrcAdminObjs : public ZS::System::GUI::CDialog
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Trace::GUI"; }
    static QString ClassName() { return "CDlgIdxTreeTrcAdminObjs"; }
public: // class CDlgTrcServer
    static CDlgIdxTreeTrcAdminObjs* CreateInstance(
        const QString& i_strDlgTitle,
        CIdxTreeTrcAdminObjs* i_pIdxTree,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags(),
        ZS::Trace::ETraceDetailLevelMethodCalls i_eTrcDetailLevel = ZS::Trace::ETraceDetailLevelMethodCalls::None );
    static CDlgIdxTreeTrcAdminObjs* GetInstance( const QString& i_strObjName );
protected: // ctor
    CDlgIdxTreeTrcAdminObjs(
        const QString& i_strDlgTitle,
        CIdxTreeTrcAdminObjs* i_pIdxTree,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags(),
        ZS::Trace::ETraceDetailLevelMethodCalls i_eTrcDetailLevel = ZS::Trace::ETraceDetailLevelMethodCalls::None );
public: // dtor
    virtual ~CDlgIdxTreeTrcAdminObjs();
public: // overridables of base class ZS::System::GUI::CDialog
    virtual QString nameSpace() override { return NameSpace(); }
    virtual QString className() override { return ClassName(); }
protected: // instance members
    CIdxTreeTrcAdminObjs* m_pIdxTree;
    QVBoxLayout* m_pLyt;
    CWdgtIdxTreeTrcAdminObjs* m_pWdgtIdxTree;
    ZS::Trace::ETraceDetailLevelMethodCalls m_eTrcDetailLevel;

}; // class CDlgIdxTreeTrcAdminObjs

} // namespace GUI

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSSysGUI_TrcAdminObjIdxTreeDlg_h
