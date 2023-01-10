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

#ifndef ZSPhysValGUI_PhysSizesIdxTreeDlg_h
#define ZSPhysValGUI_PhysSizesIdxTreeDlg_h

#include <QtCore/qglobal.h>

#include "ZSPhysValGUI/ZSPhysValGUIDllMain.h"
#include "ZSSysGUI/ZSSysDialog.h"
#include "ZSSys/ZSSysAux.h"

class QVBoxLayout;

namespace ZS
{
namespace PhysVal
{
class CIdxTreePhysSizes;

namespace GUI
{
class CWdgtPhysSizes;

//******************************************************************************
class ZSPHYSVALGUIDLL_API CDlgIdxTreePhysSizes : public ZS::System::GUI::CDialog
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CDlgIdxTreePhysSizes"; }
public: // class CDlgTrcServer
    static CDlgIdxTreePhysSizes* CreateInstance(
        const QString& i_strDlgTitle,
        CIdxTreePhysSizes* i_pIdxTree,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
    static CDlgIdxTreePhysSizes* GetInstance( const QString& i_strObjName );
protected: // ctor
    CDlgIdxTreePhysSizes(
        const QString& i_strDlgTitle,
        CIdxTreePhysSizes* i_pIdxTree,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
public: // dtor
    virtual ~CDlgIdxTreePhysSizes();
public: // overridables of base class ZS::System::GUI::CDialog
    virtual QString nameSpace() override { return NameSpace(); }
    virtual QString className() override { return ClassName(); }
protected: // instance members
    CIdxTreePhysSizes* m_pIdxTree;
    QVBoxLayout* m_pLyt;
    CWdgtPhysSizes* m_pWdgtIdxTree;

}; // class CDlgIdxTreePhysSizes

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysSizesIdxTreeDlg_h
