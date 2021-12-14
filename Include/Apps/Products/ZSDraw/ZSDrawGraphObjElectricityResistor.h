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

#ifndef ZSDraw_GraphObjElectricityResistor_h
#define ZSDraw_GraphObjElectricityResistor_h

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
class CGraphObjResistor : public CGraphObjElectricity
/*
CnctPt1    Body    CnctPt2
        +--------+
+--+    |        |    +--+
|  |----|  -  -  |----|  |  <-- Line (hidden below Body)
+--+    |        |    +--+
        +--------+
*******************************************************************************/
{
public: // type definitions and constants
    static const QSize   c_sizInitial;
    static const QString c_strKeyLabelResistance;
public: // class methods
    // For sub system test used. Method call only allowed if currently no
    // graphical objects of this class are created.
    static void ResetCtorsDtorsCounters();
public: // ctors and dtor
    CGraphObjResistor(
        CDrawingScene*       i_pDrawingScene,
        const CDrawSettings& i_drawSettings,
        const QString&       i_strObjName = "",
        const QString&       i_strObjId = "" );
    virtual ~CGraphObjResistor();
public: // instance methods
    double getResistance() const { return m_fResistance_Ohm; }
    void setResistance( double i_fResistance_Ohm );
public: // instance methods
    void showResistance( ESelectionPoint i_selPtPos = ESelectionPointBottomCenter );
    void hideResistance();
    bool isResistanceVisible() const;
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
    CGraphObjLine*            m_pLinCnct;
    CGraphObjRect*            m_pRctBody;
    CGraphObjConnectionPoint* m_pCnctPt1;
    CGraphObjConnectionPoint* m_pCnctPt2;
    double                    m_fResistance_Ohm;

}; // class CGraphObjResistor

} // namespace Electricity

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjElectricityResistor_h
