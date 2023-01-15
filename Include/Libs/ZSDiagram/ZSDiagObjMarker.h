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

#ifndef ZSDiagObjMarker_h
#define ZSDiagObjMarker_h

#include "ZSDiagram/ZSDiagObj.h"
#include "ZSPhysVal/ZSPhysVal.h"

//#include <QtCore/qobject.h>

namespace ZS
{
namespace Diagram
{
class CLabelStyle;
class CImageStyle;
class CToolTipStyle;
struct SFrameStyle;
struct SLineStyle;

//******************************************************************************
class ZSDIAGRAMDLL_API CDiagObjMarker : public CDiagObj
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    typedef enum {
        EElementMin     = 0,
        EElementLineVer = 0,
        EElementLineHor = 1,
        EElementImage   = 2,
        EElementLabel   = 3,
        EElementToolTip = 4,
        EElementCursor  = 5,
        EElementMax     = 5,
        EElementCount
    }   EElement;
public: // ctors and dtor
    CDiagObjMarker( const QString& i_strObjName, CDiagTrace* i_pDiagTrace );
    virtual ~CDiagObjMarker();
signals:
    void posChanged( ZS::Diagram::CDiagObjMarker* );
public: // overridables to get and set the cursor position
    virtual void setCalculateCursorPos( bool i_bCalculate );
    virtual bool getCalculateCursorPos() const;
    virtual void setCursorMoveDir( EScaleDir i_moveDir );
    virtual EScaleDir getCursorMoveDir() const;
    virtual void setVal( EScaleDir i_scaleDir, double i_fXVal, PhysVal::CUnit* i_pUnit = nullptr );
    virtual QPoint getPos() const;
public: // overridables to modify the marker style
    virtual void showElement( EDiagObjState i_diagObjState, EElement i_element );
    virtual void hideElement( EDiagObjState i_diagObjState, EElement i_element );
    virtual bool isElementVisible( EDiagObjState i_diagObjStateIdx, EElement i_element ) const;
    virtual void setLineStyleVer( EDiagObjState i_diagObjState, SLineStyle* i_pLineStyle );
    virtual SLineStyle* getLineStyleVer( EDiagObjState i_diagObjState );
    virtual void setLineStyleHor( EDiagObjState i_diagObjState, SLineStyle* i_pLineStyle );
    virtual SLineStyle* getLineStyleHor( EDiagObjState i_diagObjState );
    virtual void setImageStyle( EDiagObjState i_diagObjState, CImageStyle* i_pImageStyle );
    virtual CImageStyle* getImageStyle( EDiagObjState i_diagObjState );
    virtual void setLabelStyle( EDiagObjState i_diagObjState, CLabelStyle* i_pLabelStyle, int i_iAligmentFlags = 0, int i_iOffset_px = 0 );
    virtual CLabelStyle* getLabelStyle( EDiagObjState i_diagObjState );
    virtual void setToolTipStyle( EDiagObjState i_diagObjState, CToolTipStyle* i_pToolTipStyle );
    virtual CToolTipStyle* getToolTipStyle( EDiagObjState i_diagObjState );
    virtual void setToolTipValueFormat( EScaleDir i_scaleDir, PhysVal::SValueFormatProvider* i_pValueFormat );
    virtual PhysVal::SValueFormatProvider* getToolTipValueFormat( EScaleDir i_scaleDir );
    virtual void setImageStyleCursor( EDiagObjState i_diagObjState, CImageStyle* i_pImageStyle );
    virtual CImageStyle* getImageStyleCursor( EDiagObjState i_diagObjState );
public: // overridables of base class CDiagObj
    virtual PhysVal::CPhysVal getVal( EScaleDir i_scaleDir ) const;
    virtual bool isFocusable() const;
    virtual bool isEditable() const;
    virtual bool isHit( const QPoint& i_pt ) const;
    virtual void moveEvent( CDiagObjMoveEvent* i_pEv, bool i_bInformDiagram = true );
public: // must overridables of base class CDiagObj
    virtual CDiagObj* clone( CDataDiagram* i_pDiagramTrg ) const;
    virtual void update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice = nullptr );
protected: // instance methods
    void updateLayout();
    void updateData();
    void updatePixmap( QPaintDevice* i_pPaintDevice );
    void updateWidget();
protected: // overridables
    virtual void ensureToolTipIsVisible( CToolTipStyle* i_pToolTipStyle );
private: // copy ctor not allowed
    CDiagObjMarker( const CDiagObjMarker& );
private: // assignment operator not allowed
    void operator=( const CDiagObjMarker& );
protected:  // instance members
    // Previous states (before data processing in "update" method)
    bool                           m_bVisiblePrev;
    EDiagObjState                  m_statePrev;
    // Calculated cursor position
    bool                           m_bCalculateCursorPos;
    EScaleDir                      m_scaleDirCursorMove;
    PhysVal::CPhysVal              m_arphysValPrev[EScaleDirCount];
    PhysVal::CPhysVal              m_arphysVal[EScaleDirCount];
    QPoint                         m_ptPosPrev;
    QPoint                         m_ptPos;
    // Graphical elements of the marker:
    bool                           m_arbShowElement[EDiagObjStateCount][EElementCount];
    // - value indication with vertical line
    SLineStyle*                    m_arpLineStyleVer[EDiagObjStateCount];
    QRect                          m_rectLineVerPrev;
    // - value indication with horizontal line
    SLineStyle*                    m_arpLineStyleHor[EDiagObjStateCount];
    QRect                          m_rectLineHorPrev;
    // - value indication with image
    CImageStyle*                   m_arpImageStyle[EDiagObjStateCount];
    QRect                          m_rectImageCurr;
    QRect                          m_rectImagePrev;
    // - marker label
    CLabelStyle*                   m_arpLabelStyle[EDiagObjStateCount];
    int                            m_ariLabelAligmentFlags[EDiagObjStateCount];
    int                            m_ariLabelOffset_px[EDiagObjStateCount];
    QRect                          m_rectLabelCurr;
    QRect                          m_rectLabelPrev;
    // - value indication with tool tip
    CToolTipStyle*                 m_arpToolTipStyle[EDiagObjStateCount];
    PhysVal::SValueFormatProvider* m_arpValueFormatToolTip[EScaleDirCount];
    QRect                          m_rectToolTipPrev;
    QRect                          m_rectToolTipArrowPrev;
    // - indication of focus and editing cursor (StateNormal not used)
    CImageStyle*                   m_arpImageStyleCursor[EDiagObjStateCount];
    QRect                          m_rectImageCursorCurr;
    QRect                          m_rectImageCursorPrev;

}; // class CDiagObjMarker

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagObjMarker_h
