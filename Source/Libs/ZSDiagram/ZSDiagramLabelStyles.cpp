/*******************************************************************************

Copyright 2004 - 2009 by    ZeusSoft, Ing. Buero Bauer
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

#include <QtGui/qpainter.h>
#include "ZSDiagram/ZSDiagramLabelStyles.h"
#include "ZSDiagram/ZSDiagramFrameStyles.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::Diagram;


/*******************************************************************************
struct ZSDIAGRAMDLL_API CLabelStyle
*******************************************************************************/

/*==============================================================================
public: ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CLabelStyle::CLabelStyle() :
//------------------------------------------------------------------------------
    m_strLabel(),
    m_brushStyle(Qt::SolidPattern),
    m_colFg(Qt::black),
    m_colBg(Qt::black),
    m_fntLabel(),
    m_iMarginTop(0),
    m_iMarginBottom(0),
    m_iMarginLeft(0),
    m_iMarginRight(0),
    m_iAlignmentFlags(Qt::AlignVCenter|Qt::AlignHCenter), //lint !e655
    m_pFrameStyle(nullptr)
{
} // default ctor

//------------------------------------------------------------------------------
CLabelStyle::CLabelStyle( const QString& i_strLabel ) :
//------------------------------------------------------------------------------
    m_strLabel(i_strLabel),
    m_brushStyle(Qt::SolidPattern),
    m_colFg(Qt::black),
    m_colBg(Qt::black),
    m_fntLabel(),
    m_iMarginTop(0),
    m_iMarginBottom(0),
    m_iMarginLeft(0),
    m_iMarginRight(0),
    m_iAlignmentFlags(Qt::AlignVCenter|Qt::AlignHCenter), //lint !e655
    m_pFrameStyle(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
CLabelStyle::CLabelStyle(
    const QString&        i_strLabel,
    const Qt::BrushStyle& i_brushStyle,
    const QColor&         i_colFg,
    const QColor&         i_colBg,
    const QFont&          i_fntLabel,
    int                   i_iMarginTop,
    int                   i_iMarginBottom,
    int                   i_iMarginLeft,
    int                   i_iMarginRight,
    int                   i_iAlignmentFlags,
    SFrameStyle*          i_pFrameStyle ) :
//------------------------------------------------------------------------------
    m_strLabel(i_strLabel),
    m_brushStyle(i_brushStyle),
    m_colFg(i_colFg),
    m_colBg(i_colBg),
    m_fntLabel(i_fntLabel),
    m_iMarginTop(i_iMarginTop),
    m_iMarginBottom(i_iMarginBottom),
    m_iMarginLeft(i_iMarginLeft),
    m_iMarginRight(i_iMarginRight),
    m_iAlignmentFlags(i_iAlignmentFlags),
    m_pFrameStyle(i_pFrameStyle)
{
} // ctor

//------------------------------------------------------------------------------
CLabelStyle::CLabelStyle( const CLabelStyle& i_labelStyleOther ) :
//------------------------------------------------------------------------------
    m_strLabel(i_labelStyleOther.m_strLabel),
    m_brushStyle(i_labelStyleOther.m_brushStyle),
    m_colFg(i_labelStyleOther.m_colFg),
    m_colBg(i_labelStyleOther.m_colBg),
    m_fntLabel(i_labelStyleOther.m_fntLabel),
    m_iMarginTop(i_labelStyleOther.m_iMarginTop),
    m_iMarginBottom(i_labelStyleOther.m_iMarginBottom),
    m_iMarginLeft(i_labelStyleOther.m_iMarginLeft),
    m_iMarginRight(i_labelStyleOther.m_iMarginRight),
    m_iAlignmentFlags(i_labelStyleOther.m_iAlignmentFlags),
    m_pFrameStyle(nullptr)
{
    if( i_labelStyleOther.m_pFrameStyle != nullptr )
    {
        m_pFrameStyle = i_labelStyleOther.m_pFrameStyle->clone();
    }

} // copy ctor

//------------------------------------------------------------------------------
CLabelStyle::~CLabelStyle()
//------------------------------------------------------------------------------
{
    delete m_pFrameStyle;
    m_pFrameStyle = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CLabelStyle::setLabel( const QString& i_strLabel )
//------------------------------------------------------------------------------
{
    m_strLabel = i_strLabel;
}

//------------------------------------------------------------------------------
QString CLabelStyle::getLabel() const
//------------------------------------------------------------------------------
{
    return m_strLabel;
}

//------------------------------------------------------------------------------
void CLabelStyle::setBrushStyle( const Qt::BrushStyle& i_brushStyle )
//------------------------------------------------------------------------------
{
    m_brushStyle = i_brushStyle;
}

//------------------------------------------------------------------------------
Qt::BrushStyle CLabelStyle::getBrushStyle() const
//------------------------------------------------------------------------------
{
    return m_brushStyle;
}

//------------------------------------------------------------------------------
void CLabelStyle::setColFg( const QColor& i_col )
//------------------------------------------------------------------------------
{
    m_colFg = i_col;
}

//------------------------------------------------------------------------------
QColor CLabelStyle::getColFg() const
//------------------------------------------------------------------------------
{
    return m_colFg;
}

//------------------------------------------------------------------------------
void CLabelStyle::setColBg( const QColor& i_col )
//------------------------------------------------------------------------------
{
    m_colBg = i_col;
}

//------------------------------------------------------------------------------
QColor CLabelStyle::getColBg() const
//------------------------------------------------------------------------------
{
    return m_colBg;
}

//------------------------------------------------------------------------------
void CLabelStyle::setFont( const QFont& i_fnt )
//------------------------------------------------------------------------------
{
    m_fntLabel = i_fnt;
}

//------------------------------------------------------------------------------
QFont CLabelStyle::getFont() const
//------------------------------------------------------------------------------
{
    return m_fntLabel;
}

//------------------------------------------------------------------------------
void CLabelStyle::setMarginTop( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iMarginTop = i_iMargin;
}

//------------------------------------------------------------------------------
int CLabelStyle::getMarginTop() const
//------------------------------------------------------------------------------
{
    return m_iMarginTop;
}

//------------------------------------------------------------------------------
void CLabelStyle::setMarginBottom( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iMarginBottom = i_iMargin;
}

//------------------------------------------------------------------------------
int CLabelStyle::getMarginBottom() const
//------------------------------------------------------------------------------
{
    return m_iMarginBottom;
}

//------------------------------------------------------------------------------
void CLabelStyle::setMarginLeft( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iMarginLeft = i_iMargin;
}

//------------------------------------------------------------------------------
int CLabelStyle::getMarginLeft() const
//------------------------------------------------------------------------------
{
    return m_iMarginLeft;
}

//------------------------------------------------------------------------------
void CLabelStyle::setMarginRight( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iMarginRight = i_iMargin;
}

//------------------------------------------------------------------------------
int CLabelStyle::getMarginRight() const
//------------------------------------------------------------------------------
{
    return m_iMarginRight;
}

//------------------------------------------------------------------------------
void CLabelStyle::setAlignment( int i_iAlignmentFlags )
//------------------------------------------------------------------------------
{
    m_iAlignmentFlags = i_iAlignmentFlags;
}

//------------------------------------------------------------------------------
int CLabelStyle::getAlignment() const
//------------------------------------------------------------------------------
{
    return m_iAlignmentFlags;
}

//------------------------------------------------------------------------------
void CLabelStyle::setFrameStyle( SFrameStyle* i_pFrameStyle )
//------------------------------------------------------------------------------
{
    delete m_pFrameStyle;
    m_pFrameStyle = i_pFrameStyle;
}

//------------------------------------------------------------------------------
SFrameStyle* CLabelStyle::getFrameStyle()
//------------------------------------------------------------------------------
{
    return m_pFrameStyle;
}

//------------------------------------------------------------------------------
QRect CLabelStyle::boundingRect() const
//------------------------------------------------------------------------------
{
    // Unter Linux funktioniert die Berechnung der Text-Breite nicht korrekt.
    #ifdef LINUX
    int cxInnerBorderCorrValForLinux = m_strLabel.length();
    int cyInnerBorderCorrValForLinux = 4;
    #else
    int cxInnerBorderCorrValForLinux = 0;
    int cyInnerBorderCorrValForLinux = 0;
    #endif

    QFontMetrics fntmtr(m_fntLabel);

    QRect rectLabel = fntmtr.boundingRect(m_strLabel);

    rectLabel.setWidth(
        rectLabel.width()
      + m_iMarginLeft
      + m_iMarginRight
      + cxInnerBorderCorrValForLinux );
    rectLabel.setHeight(
        rectLabel.height()
      + m_iMarginTop
      + m_iMarginBottom
      + cyInnerBorderCorrValForLinux );

    if( m_pFrameStyle != nullptr )
    {
        rectLabel.setWidth( rectLabel.width() + 2*m_pFrameStyle->getFrameWidth() );
        rectLabel.setHeight( rectLabel.height() + 2*m_pFrameStyle->getFrameWidth() );
    }
    return rectLabel;

} // boundingRect

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CLabelStyle::draw( QPainter* i_pPainter, const QRect& i_rect ) const
//------------------------------------------------------------------------------
{
    i_pPainter->save();

    QPen pen;

    pen.setColor(m_colFg);
    pen.setStyle(Qt::SolidLine);
    i_pPainter->setPen(pen);

    if( m_brushStyle != Qt::NoBrush )
    {
        QBrush brush(m_colBg,m_brushStyle);
        i_pPainter->fillRect( i_pPainter->boundingRect(i_rect,m_iAlignmentFlags,m_strLabel), brush );
    }
    if( m_pFrameStyle != nullptr )
    {
        m_pFrameStyle->draw(i_pPainter,i_rect);
    }
    i_pPainter->setFont(m_fntLabel);
    i_pPainter->drawText(i_rect,m_iAlignmentFlags,m_strLabel);

    i_pPainter->restore();

} // draw
