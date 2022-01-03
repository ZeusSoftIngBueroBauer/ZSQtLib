/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSDraw_GraphObjElectricityDiode_h
#define ZSDraw_GraphObjElectricityDiode_h

#include "ZSDrawGraphObjElectricity.h"

namespace ZS
{
namespace Draw
{
class CGraphObjLine;
class CGraphObjPolygon;
class CGraphObjConnectionPoint;

namespace Electricity
{
//******************************************************************************
class CGraphObjDiode : public CGraphObjElectricity
/*
CnctPt1  Body     CnctPt2
        +     |
+--+    |  +  |   +--+
|  |----|    +----|  |  <-- Line (hidden below Body)
+--+    |  +  |   +--+
        +     |
*******************************************************************************/
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw::Electricity"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjDiode"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // type definitions and constants
    static const QSize   c_sizInitial;
    static const QString c_strKeyLabelCutOffVoltage;
public: // ctors and dtor
    CGraphObjDiode(
        CDrawingScene*       i_pDrawingScene,
        const CDrawSettings& i_drawSettings,
        const QString&       i_strObjName = "" );
    virtual ~CGraphObjDiode();
public: // overridables
    virtual QString nameSpace() { return NameSpace(); }
    virtual QString className() { return ClassName(); }
public: // instance methods
    double getCutOffVoltage() const { return m_fCutOffVoltage_V; }
    void setCutOffVoltage( double i_fCutOffVoltage_V );
public: // instance methods
    void showCutOffVoltage( ESelectionPoint i_selPtPos = ESelectionPoint::BottomCenter );
    void hideCutOffVoltage();
    bool isCutOffVoltageVisible() const;
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone();
public: // overridables of base class CGraphObj
    virtual void onCreateAndExecDlgFormatGraphObjs();
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged();
protected: // overridables of base class CGraphObj
    virtual void updateToolTip();
protected: // class members
    static qint64 s_iInstCount;
protected: // instance members
    CGraphObjLine*            m_pLinCnct;
    CGraphObjPolygon*         m_pPlgBody;
    CGraphObjLine*            m_pLinBody;
    CGraphObjConnectionPoint* m_pCnctPt1;
    CGraphObjConnectionPoint* m_pCnctPt2;
    double                    m_fCutOffVoltage_V;

}; // class CGraphObjDiode

} // namespace Electricity

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjElectricityDiode_h
