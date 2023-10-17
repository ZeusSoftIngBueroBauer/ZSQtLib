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

#ifndef ZSDraw_GraphObjElectricitySwitch_h
#define ZSDraw_GraphObjElectricitySwitch_h

#include "ZSDrawGraphObjElectricity.h"

namespace ZS
{
namespace Draw
{
class CGraphObjConnectionPoint;
class CGraphObjLine;

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
public: // class methods
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw::Electricity"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjSwitch"; }
public: // type definitions and constants
    static const QSize c_sizInitial;
    typedef enum {
        EStateOpened = 0,
        EStateClosed = 1,
        EStateCount,
        EStateUndefined
    }   EState;
    static QString State2Str( int i_state );
    static EState Str2State( const QString& i_str );
public: // ctors and dtor
    CGraphObjSwitch(
        const CDrawSettings& i_drawSettings,
        EState i_state = EStateOpened,
        const QString& i_strObjName = "");
    virtual ~CGraphObjSwitch();
public: // instance methods
    EState getState() const { return m_state; }
    QString getStateAsString() const { return State2Str(m_state); }
    void setState( EState i_state );
    void toggleState();
public: // instance methods
    void showState( ESelectionPoint i_selPtPos = ESelectionPoint::BottomCenter );
    void hideState();
    bool isStateVisible( ESelectionPoint i_selPtPos = ESelectionPoint::BottomCenter ) const;
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone() override;
public: // overridables of base class CGraphObj
    virtual void onCreateAndExecDlgFormatGraphObjs() override;
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld) override;
protected: // overridables of base class CGraphObjGroup
    virtual void mousePressEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
protected: // overridables of base class CGraphObjGroup
    virtual void keyPressEvent( QKeyEvent* i_pEv ) override;
    virtual void keyReleaseEvent( QKeyEvent* i_pEv ) override;
protected: // overridables of base class CGraphObj
    virtual void updateToolTip() override;
protected: // class members
    /*!< Needed to set an initial unique name when creating a new instance. */
    static qint64 s_iInstCount;
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
