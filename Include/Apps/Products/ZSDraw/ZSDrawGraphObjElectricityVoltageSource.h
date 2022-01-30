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

#ifndef ZSDraw_GraphObjElectricityVoltageSource_h
#define ZSDraw_GraphObjElectricityVoltageSource_h

#include "ZSDrawGraphObjElectricity.h"

namespace ZS
{
namespace Draw
{
class CGraphObjEllipse;
class CGraphObjLine;
class CGraphObjConnectionPoint;

namespace Electricity
{
//******************************************************************************
class CGraphObjVoltageSource : public CGraphObjElectricity
/*
CnctPt1  Body   CnctPt2
          +-+
+--+    +     +    +--+
|  |---+ -   + +---|  |  <-- CnctLine (hidden below body)
+--+    +     +    +--+
          +-+
*******************************************************************************/
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw::Electricity"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjVoltageSource"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // type definitions and constants
    static const QSize c_sizInitial;
public: // ctors and dtor
    CGraphObjVoltageSource(
        CDrawingScene*       i_pDrawingScene,
        const CDrawSettings& i_drawSettings,
        const QString&       i_strObjName = "" );
    virtual ~CGraphObjVoltageSource();
public: // overridables
    virtual QString nameSpace() { return NameSpace(); }
    virtual QString className() { return ClassName(); }
public: // instance methods
    double getVoltage() const { return m_fVoltage_V; }
    void setVoltage( double i_fVoltage_V );
public: // instance methods
    void showVoltage( ESelectionPoint i_selPtPos = ESelectionPoint::BottomCenter );
    void hideVoltage();
    bool isVoltageVisible( ESelectionPoint i_selPtPos = ESelectionPoint::BottomCenter ) const;
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
    CGraphObjEllipse*         m_pEllBody;
    CGraphObjLine*            m_pLinSignMinus;
    CGraphObjLine*            m_pLinSignPlusHor;
    CGraphObjLine*            m_pLinSignPlusVer;
    CGraphObjConnectionPoint* m_pCnctPt1;
    CGraphObjConnectionPoint* m_pCnctPt2;
    double                    m_fVoltage_V;

}; // class CGraphObjVoltageSource

} // namespace Electricity

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjElectricityVoltageSource_h
