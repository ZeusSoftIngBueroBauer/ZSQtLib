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

#ifndef ZSDraw_GraphObjElectricityInductor_h
#define ZSDraw_GraphObjElectricityInductor_h

#include "ZSDrawGraphObjElectricity.h"

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Draw
{
namespace Electricity
{
//******************************************************************************
class CGraphObjInductor : public CGraphObjElectricity
/*
CnctPt1        Body       CnctPt2
              |       |
+--+         | |     | |     +--+
|  |----|   |   |   |   |----|  |  <-- Line and Body as Polyline
+--+     | |     | |         +--+
          |       |
*******************************************************************************/
{
public: // type definitions and constants
public: // type definitions and constants
    static const QSize   c_sizInitial;
    static const QString c_strKeyLabelInductance;
public: // class methods
    // For sub system test used. Method call only allowed if currently no
    // graphical objects of this class are created.
    static void ResetCtorsDtorsCounters();
public: // ctors and dtor
    CGraphObjInductor(
        CDrawingScene*       i_pDrawingScene,
        const CDrawSettings& i_drawSettings,
        const QString&       i_strObjName = "",
        const QString&       i_strObjId = "" );
    virtual ~CGraphObjInductor();
public: // instance methods
    double getInductance() const { return m_fInductance_H; }
    void setInductance( double i_fInductance_H );
public: // instance methods
    void showInductance( ESelectionPoint i_selPtPos = ESelectionPointBottomCenter );
    void hideInductance();
    bool isInductanceVisible() const;
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone();
public: // overridables of base class CGraphObj
    virtual void onCreateAndExecDlgFormatGraphObjs();
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged();
protected: // overridables of base class CGraphObj
    virtual void updateToolTip();
protected: // class members
    static qint64                   s_iCtorsCount;
    static qint64                   s_iDtorsCount;
    static ZS::Trace::CTrcAdminObj* s_pTrcAdminObjCtorsAndDtor;
    static ZS::Trace::CTrcAdminObj* s_pTrcAdminObjItemChange;
protected: // instance members
    CGraphObjPolyline*        m_pLinBody;
    CGraphObjConnectionPoint* m_pCnctPt1;
    CGraphObjConnectionPoint* m_pCnctPt2;
    double                    m_fInductance_H; // Henries

}; // class CGraphObjInductor

} // namespace Electricity

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjElectricityInductor_h
