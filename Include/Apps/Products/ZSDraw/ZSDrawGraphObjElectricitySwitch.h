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

#ifndef ZSDraw_GraphObjElectricitySwitch_h
#define ZSDraw_GraphObjElectricitySwitch_h

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
class CGraphObjSwitch : public CGraphObjElectricity
/*
CnctPt1 SwitchLine CnctPt2
   +--+            +--+
   |  +============+  |
   +--+            +--+
*******************************************************************************/
{
public: // type definitions and constants
    static const QSize   c_sizInitial;
    static const QString c_strKeyLabelState;
    typedef enum {
        EStateOpened = 0,
        EStateClosed = 1,
        EStateCount,
        EStateUndefined
    }   EState;
    static QString State2Str( int i_state );
    static EState Str2State( const QString& i_str );
public: // class methods
    // For sub system test used. Method call only allowed if currently no
    // graphical objects of this class are created.
    static void ResetCtorsDtorsCounters();
public: // ctors and dtor
    CGraphObjSwitch(
        CDrawingScene*       i_pDrawingScene,
        const CDrawSettings& i_drawSettings,
        EState               i_state = EStateOpened,
        const QString&       i_strObjName = "",
        const QString&       i_strObjId = "" );
    virtual ~CGraphObjSwitch();
public: // instance methods
    EState getState() const { return m_state; }
    QString getStateAsString() const { return State2Str(m_state); }
    void setState( EState i_state );
    void toggleState();
public: // instance methods
    void showState( ESelectionPoint i_selPtPos = ESelectionPointBottomCenter );
    void hideState();
    bool isStateVisible() const;
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone();
public: // overridables of base class CGraphObj
    virtual void onCreateAndExecDlgFormatGraphObjs();
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged();
protected: // overridables of base class CGraphObjGroup
    virtual void mousePressEvent( QGraphicsSceneMouseEvent* i_pEv );
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv );
    virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv );
protected: // overridables of base class CGraphObjGroup
    virtual void keyPressEvent( QKeyEvent* i_pEv );
    virtual void keyReleaseEvent( QKeyEvent* i_pEv );
protected: // overridables of base class CGraphObj
    virtual void updateToolTip();
protected: // class members
    static qint64                   s_iCtorsCount;
    static qint64                   s_iDtorsCount;
    static ZS::Trace::CTrcAdminObj* s_pTrcAdminObjCtorsAndDtor;
    static ZS::Trace::CTrcAdminObj* s_pTrcAdminObjItemChange;
    static ZS::Trace::CTrcAdminObj* s_pTrcAdminObjMouseEvents;
    static ZS::Trace::CTrcAdminObj* s_pTrcAdminObjKeyEvents;
protected: // instance members
    CGraphObjConnectionPoint* m_pCnctPt1;
    CGraphObjConnectionPoint* m_pCnctPt2;
    CGraphObjLine*            m_pLinSwitch;
    int                       m_idxLinSwitchAlignmentTop;
    EState                    m_state;

}; // class CGraphObjSwitch

} // namespace Electricity

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjElectricitySwitch_h
