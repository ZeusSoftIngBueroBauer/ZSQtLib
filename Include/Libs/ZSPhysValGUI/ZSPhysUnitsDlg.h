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

#ifndef ZSPhysValGUI_PhysUnitsDlg_h
#define ZSPhysValGUI_PhysUnitsDlg_h

#include <QtCore/qglobal.h>

#include "ZSPhysValGUI/ZSPhysValGUIDllMain.h"
#include "ZSSysGUI/ZSSysDialog.h"
#include "ZSSys/ZSSysAux.h"

class QVBoxLayout;

namespace ZS
{
namespace PhysVal
{
class CIdxTreeUnits;

namespace GUI
{
class CWdgtUnits;

//******************************************************************************
class ZSPHYSVALGUIDLL_API CDlgPhysUnits : public ZS::System::GUI::CDialog
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CDlgPhysUnits"; }
public: // class CDlgTrcServer
    static CDlgPhysUnits* CreateInstance(
        const QString& i_strDlgTitle,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
    static CDlgPhysUnits* GetInstance();
protected: // ctor
    CDlgPhysUnits(
        const QString& i_strDlgTitle,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
public: // dtor
    virtual ~CDlgPhysUnits();
public: // overridables of base class ZS::System::GUI::CDialog
    virtual QString nameSpace() override { return NameSpace(); }
    virtual QString className() override { return ClassName(); }
protected: // instance members
    QVBoxLayout* m_pLyt;
    CWdgtUnits* m_pWdgtUnits;

}; // class CDlgPhysUnits

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysUnitsDlg_h
