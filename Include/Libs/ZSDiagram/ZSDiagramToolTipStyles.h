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

#ifndef ZSDiagramToolTipStyles_h
#define ZSDiagramToolTipStyles_h

#include <QtGui/qcolor.h>
#include <QtGui/qfont.h>
#include "ZSDiagram/ZSDiagramDllMain.h"

namespace ZS
{
namespace PhysVal
{
class CUnit;
}
namespace Diagram
{
struct SFrameStyle;

//******************************************************************************
class ZSDIAGRAMDLL_API CToolTipStyle
//******************************************************************************
{
public:  // type definitions and constants
    typedef enum {
        EToolTipPosMin          = 0,
        EToolTipPosRightTop     = 0,
        EToolTipPosRightBottom  = 1,
        EToolTipPosRightCenter  = 2,
        EToolTipPosLeftTop      = 3,
        EToolTipPosLeftBottom   = 4,
        EToolTipPosLeftCenter   = 5,
        EToolTipPosTopCenter    = 6,
        EToolTipPosBottomCenter = 7,
        EToolTipPosCenterCenter = 8,
        EToolTipPosMax          = 8,
        EToolTipPosCount,
        EToolTipPosUndefined,
    }   EToolTipPos;
public: // ctors and dtor
    CToolTipStyle();
    CToolTipStyle(
        const QColor&   i_colFg,
        const QColor&   i_colBg,
        const QFont&    i_fnt,
        int             i_cxOffs,
        int             i_cyOffs,
        SFrameStyle*    i_pFrameStyle,
        int             i_iMarginTop,
        int             i_iMarginBottom,
        int             i_iMarginLeft,
        int             i_iMarginRight,
        int             i_cxArrowWidth );
    CToolTipStyle( const CToolTipStyle& i_toolTipStyleOther );
    virtual ~CToolTipStyle();
public: // operators
    CToolTipStyle& operator = ( const CToolTipStyle& i_toolTipStyleOther );
public: // instance methods
    void setToolTip( const QString& i_strToolTip );
    QString getToolTip() const;
    void setArrowHeadPos( const QPoint& i_ptPos );
    QPoint getArrowHeadPos() const;
    void setToolTipPos( EToolTipPos i_toolTipPos );
    EToolTipPos getToolTipPos() const;
    //void setIsVisible( bool i_bVisible );
    //bool getIsVisible() const;
    void setColFg( const QColor& i_col );
    QColor getColFg() const;
    void setColBg( const QColor& i_col );
    QColor getColBg() const;
    void setFont( const QFont& i_fnt );
    QFont getFont() const;
    void setOffsHor( int i_iOffs );
    int getOffsHor() const;
    void setOffsVer( int i_iOffs );
    int getOffsVer() const;
    void setFrameStyle( SFrameStyle* i_pFrameStyle );
    SFrameStyle* getFrameStyle();
    void setMarginTop( int i_iMargin );
    int getMarginTop() const;
    void setMarginBottom( int i_iMargin );
    int getMarginBottom() const;
    void setMarginLeft( int i_iMargin );
    int getMarginLeft() const;
    void setMarginRight( int i_iMargin );
    int getMarginRight() const;
    void setArrowWidth( int i_cxWidth );
    int getArrowWidth() const;
    QRect boundingRect() const;
    void setRectToolTip( const QRect& i_rect );
    QRect getRectToolTip() const;
    void setRectArrow( const QRect& i_rect );
    QRect getRectArrow() const;
public: // overridables
    virtual void updateFrameBorderPoints();
    virtual void draw( QPainter* i_pPainter );
protected: // instance members
    QString         m_strToolTip;
    QColor          m_colFg;
    QColor          m_colBg;
    QFont           m_fnt;
    int             m_cxOffs;
    int             m_cyOffs;
    SFrameStyle*    m_pFrameStyle;
    int             m_iMarginTop;
    int             m_iMarginBottom;
    int             m_iMarginLeft;
    int             m_iMarginRight;
    int             m_cxArrowWidth;
    QPoint          m_ptArrowHeadPos;
    EToolTipPos     m_toolTipPos;
    QRect           m_rectToolTip;
    QRect           m_rectToolTipArrow;
    int             m_iPtArrToolTipLength;
    #if QT_VERSION >= 0x040100
    QPolygon        m_ptarrToolTip;
    QPolygon**      m_arpPtArrToolTip;
    #else
    QPointArray     m_ptarrToolTip;
    QPointArray**   m_arpPtArrToolTip;
    #endif

}; // class CToolTipStyle

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagramToolTipStyles_h
