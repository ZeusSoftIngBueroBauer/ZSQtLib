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

#ifndef ZSDraw_WdgtFormatGraphObjsElectricityResistor_h
#define ZSDraw_WdgtFormatGraphObjsElectricityResistor_h

#include <QtCore/qglobal.h>

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSDraw/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjs.h"

class QCheckBox;
class QDoubleSpinBox;
class QLabel;

class QLayout;
class QBoxLayout;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Draw
{
namespace Electricity
{
//******************************************************************************
class CWdgtFormatGraphObjsResistor : public CWdgtFormatGraphObjs
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw::Electricity"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtFormatGraphObjsResistor"; }
public: // ctors and dtor
    CWdgtFormatGraphObjsResistor(
        CDrawingScene* i_pDrawingScene,
        CGraphObj*     i_pGraphObj,
        QWidget*       i_pWdgtParent = nullptr );
    virtual ~CWdgtFormatGraphObjsResistor();
public: // must overridables of base class CWdgtFormatGraphObjs
    virtual void applyChanges() override;
    virtual void resetChanges() override;
    virtual bool hasChanges() const override;
protected: // must overridables of base class CWdgtFormatGraphObjs
    virtual void onGraphObjChanged() override;
private: // instance members
    QVBoxLayout*             m_pLyt;
    QLabel*                  m_pLblHeadLine;
    QHBoxLayout*             m_pLytResistance;
    QLabel*                  m_pLblResistance;
    QDoubleSpinBox*          m_pEdtResistance;
    QCheckBox*               m_pChkResistanceVisible;
    // Trace
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtFormatGraphObjsResistor

} // Electricity

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_WdgtFormatGraphObjsElectricityResistor_h
