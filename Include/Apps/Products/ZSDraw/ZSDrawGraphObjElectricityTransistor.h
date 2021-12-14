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

#ifndef ZSDraw_GraphObjElectricityTransistor_h
#define ZSDraw_GraphObjElectricityTransistor_h

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
class CGraphObjTransistor : public CGraphObjElectricity
/*
  NPN Transistor          PNP Transistor
             +-+                     +-+
      CnctPt2| | C            CnctPt2| | E
             +-+                     +-+
CnctPt1       |         CnctPt1       |
 B     +--------+        B      ------+-
+-+    | |    | |       +-+    | |    | |
| |----|-|-- +  |       | |----|-|-- +  |
+-+    | |    | |       +-+    | |    | |
       +--------+               ------+-
              |                       |
             +-+                     +-+
      CnctPt3| | E            CnctPt3| | C
             +-+                     +-+
*******************************************************************************/
{
public: // type definitions and constants
    static const QSize   c_sizInitial;
    static const QString c_strKeyLabelDopingType;
    typedef enum {
        EDopingTypeNPN = 0,
        EDopingTypePNP = 1,
        EDopingTypeCount,
        EDopingTypeUndefined
    }   EDopingType;
    static QString DopingType2Str( int i_type );
    static EDopingType Str2DopingType( const QString& i_str );
public: // class methods
    // For sub system test used. Method call only allowed if currently no
    // graphical objects of this class are created.
    static void ResetCtorsDtorsCounters();
public: // ctors and dtor
    CGraphObjTransistor(
        CDrawingScene*       i_pDrawingScene,
        const CDrawSettings& i_drawSettings,
        EDopingType          i_dopingType = EDopingTypeNPN,
        const QString&       i_strObjName = "",
        const QString&       i_strObjId = "" );
    virtual ~CGraphObjTransistor();
public: // instance methods
    EDopingType getDopingType() const { return m_dopingType; }
    QString getDopingTypeAsString() const { return DopingType2Str(m_dopingType); }
    void setDopingType( EDopingType i_type );
public: // instance methods
    void showDopingType( ESelectionPoint i_selPtPos = ESelectionPointBottomCenter );
    void hideDopingType();
    bool isDopingTypeVisible() const;
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
    CGraphObjEllipse*         m_pEllBody;
    CGraphObjLine*            m_pLinBase;
    CGraphObjLine*            m_pLinBaseUp;
    CGraphObjLine*            m_pLinBaseDn;
    CGraphObjLine*            m_pLinCnctPt1;
    CGraphObjLine*            m_pLinCnctPt2;
    CGraphObjLine*            m_pLinCnctPt3;
    CGraphObjConnectionPoint* m_pCnctPt1;
    CGraphObjConnectionPoint* m_pCnctPt2;
    CGraphObjConnectionPoint* m_pCnctPt3;
    EDopingType               m_dopingType;

}; // class CGraphObjTransistor

} // namespace Electricity

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjElectricityTransistor_h
