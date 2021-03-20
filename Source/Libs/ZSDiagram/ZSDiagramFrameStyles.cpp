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
#include "ZSDiagram/ZSDiagramFrameStyles.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::Diagram;


/*******************************************************************************
struct ZSDIAGRAMDLL_API SFrameStyle
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SFrameStyle::SFrameStyle(
    int                 i_iFrameWidth,
    const QColor&       i_col,
    const Qt::PenStyle& i_penStyle ) :
//------------------------------------------------------------------------------
    m_iFrameWidth(i_iFrameWidth),
    m_col(i_col),
    m_penStyle(i_penStyle)
{
} // ctor

//------------------------------------------------------------------------------
SFrameStyle::SFrameStyle( const SFrameStyle& i_frameStyleOther ) :
//------------------------------------------------------------------------------
    m_iFrameWidth(i_frameStyleOther.m_iFrameWidth),
    m_col(i_frameStyleOther.m_col),
    m_penStyle(i_frameStyleOther.m_penStyle)
{
} // copy ctor

//------------------------------------------------------------------------------
SFrameStyle::~SFrameStyle()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
SFrameStyle* SFrameStyle::clone() const
//------------------------------------------------------------------------------
{
    return new SFrameStyle(*this);
}

//------------------------------------------------------------------------------
void SFrameStyle::setFrameWidth( int i_iFrameWidth )
//------------------------------------------------------------------------------
{
    m_iFrameWidth = i_iFrameWidth;
}

//------------------------------------------------------------------------------
int SFrameStyle::getFrameWidth() const
//------------------------------------------------------------------------------
{
    return m_iFrameWidth;
}

//------------------------------------------------------------------------------
void SFrameStyle::setCol( const QColor& i_col )
//------------------------------------------------------------------------------
{
    m_col = i_col;
}

//------------------------------------------------------------------------------
QColor SFrameStyle::getCol() const
//------------------------------------------------------------------------------
{
    return m_col;
}

//------------------------------------------------------------------------------
void SFrameStyle::setPenStyle( const Qt::PenStyle& i_penStyle )
//------------------------------------------------------------------------------
{
    m_penStyle = i_penStyle;
}

//------------------------------------------------------------------------------
Qt::PenStyle SFrameStyle::getPenStyle() const
//------------------------------------------------------------------------------
{
    return m_penStyle;
}

//------------------------------------------------------------------------------
void SFrameStyle::draw( QPainter* i_pPainter, const QRect& i_rect )
//------------------------------------------------------------------------------
{
    QRect rectFrame;
    QPen  pen;
    int   idxFrame;
    int   xLeft;
    int   xRight;
    int   yTop;
    int   yBottom;

    i_pPainter->save();

    pen.setColor(m_col);
    pen.setStyle(m_penStyle);
    i_pPainter->setPen(pen);

    xLeft   = i_rect.left();
    yTop    = i_rect.top();
    xRight  = i_rect.right();
    yBottom = i_rect.bottom();

    for( idxFrame = 0; idxFrame < m_iFrameWidth; idxFrame++, xLeft++, yTop++, xRight--, yBottom-- )
    {
        i_pPainter->drawLine(xLeft,yTop,xRight,yTop);
        i_pPainter->drawLine(xLeft,yTop,xLeft,yBottom);
        i_pPainter->drawLine(xLeft,yBottom,xRight,yBottom);
        i_pPainter->drawLine(xRight,yTop,xRight,yBottom);
    }
    i_pPainter->restore();

} // draw

//------------------------------------------------------------------------------
#if QT_VERSION >= 0x040100
void SFrameStyle::draw( QPainter* i_pPainter, const QPolygon& i_ptArr )
#else
void SFrameStyle::draw( QPainter* i_pPainter, const QPointArray& i_ptArr )
#endif
//------------------------------------------------------------------------------
{
    QPen pen;

    i_pPainter->save();

    pen.setColor(m_col);
    pen.setStyle(m_penStyle);
    pen.setWidth(m_iFrameWidth);

    i_pPainter->setPen(pen);
    i_pPainter->drawPolygon(i_ptArr);

    i_pPainter->restore();

} // draw


/*******************************************************************************
struct SFrameStyle3DSunken : public SFrameStyle
*******************************************************************************/

/*==============================================================================
public: ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SFrameStyle3DSunken::SFrameStyle3DSunken(
    const QColor& i_colLight,
    const QColor& i_colDark,
    int           i_iFrameWidth ) :
//------------------------------------------------------------------------------
    SFrameStyle(i_iFrameWidth),
    m_colLight(i_colLight),
    m_colDark(i_colDark)
{
} // ctor

//------------------------------------------------------------------------------
SFrameStyle3DSunken::SFrameStyle3DSunken( const SFrameStyle3DSunken& i_frameStyleOther ) :
//------------------------------------------------------------------------------
    SFrameStyle(i_frameStyleOther), //lint !e1738
    m_colLight(i_frameStyleOther.m_colLight),
    m_colDark(i_frameStyleOther.m_colDark)
{
} // copy ctor

//------------------------------------------------------------------------------
SFrameStyle3DSunken::~SFrameStyle3DSunken()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void SFrameStyle3DSunken::setColLight( const QColor& i_colLight )
//------------------------------------------------------------------------------
{
    m_colLight = i_colLight;
}

//------------------------------------------------------------------------------
QColor SFrameStyle3DSunken::getColLight() const
//------------------------------------------------------------------------------
{
    return m_colLight;
}

//------------------------------------------------------------------------------
void SFrameStyle3DSunken::setColDark( const QColor& i_colDark )
//------------------------------------------------------------------------------
{
    m_colDark = i_colDark;
}

//------------------------------------------------------------------------------
QColor SFrameStyle3DSunken::getColDark() const
//------------------------------------------------------------------------------
{
    return m_colDark;
}

/*==============================================================================
public: // overridables of base class SFrameStyle
==============================================================================*/

//lint -e1961
//------------------------------------------------------------------------------
void SFrameStyle3DSunken::draw( QPainter* i_pPainter, const QRect& i_rect )
//------------------------------------------------------------------------------
{
    QPen  penDark(m_colDark);
    QPen  penLight(m_colLight);
    int   idxFrame;
    int   xLeft;
    int   xRight;
    int   yTop;
    int   yBottom;

    i_pPainter->save();

    xLeft   = i_rect.left();
    yTop    = i_rect.top();
    xRight  = i_rect.right();
    yBottom = i_rect.bottom();

    for( idxFrame = 0; idxFrame < m_iFrameWidth; idxFrame++, xLeft++, yTop++, xRight--, yBottom-- )
    {
        i_pPainter->setPen(penDark);
        i_pPainter->drawLine(xLeft,yTop,xRight,yTop);
        i_pPainter->drawLine(xLeft,yTop,xLeft,yBottom);
        i_pPainter->setPen(penLight);
        i_pPainter->drawLine(xLeft,yBottom,xRight,yBottom);
        i_pPainter->drawLine(xRight,yTop,xRight,yBottom);
    }
    i_pPainter->restore();

} // draw
//lint +e1961

//------------------------------------------------------------------------------
SFrameStyle* SFrameStyle3DSunken::clone()  const
//------------------------------------------------------------------------------
{
    return new SFrameStyle3DSunken(*this);
}


/*******************************************************************************
struct SFrameStyle3DRaised : public SFrameStyle
*******************************************************************************/

/*==============================================================================
public: ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SFrameStyle3DRaised::SFrameStyle3DRaised(
    const QColor& i_colLight,
    const QColor& i_colDark,
    int           i_iFrameWidth ) :
//------------------------------------------------------------------------------
    SFrameStyle(i_iFrameWidth),
    m_colLight(i_colLight),
    m_colDark(i_colDark)
{
} // ctor

//------------------------------------------------------------------------------
SFrameStyle3DRaised::SFrameStyle3DRaised( const SFrameStyle3DRaised& i_frameStyleOther ) :
//------------------------------------------------------------------------------
    SFrameStyle(i_frameStyleOther), //lint !e1738
    m_colLight(i_frameStyleOther.m_colLight),
    m_colDark(i_frameStyleOther.m_colDark)
{
} // copy ctor

//------------------------------------------------------------------------------
SFrameStyle3DRaised::~SFrameStyle3DRaised()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void SFrameStyle3DRaised::setColLight( const QColor& i_colLight )
//------------------------------------------------------------------------------
{
    m_colLight = i_colLight;
}

//------------------------------------------------------------------------------
QColor SFrameStyle3DRaised::getColLight() const
//------------------------------------------------------------------------------
{
    return m_colLight;
}

//------------------------------------------------------------------------------
void SFrameStyle3DRaised::setColDark( const QColor& i_colDark )
//------------------------------------------------------------------------------
{
    m_colDark = i_colDark;
}

//------------------------------------------------------------------------------
QColor SFrameStyle3DRaised::getColDark() const
//------------------------------------------------------------------------------
{
    return m_colDark;
}

/*==============================================================================
public: // overridables of base class SFrameStyle
==============================================================================*/

//lint -e1961
//------------------------------------------------------------------------------
void SFrameStyle3DRaised::draw( QPainter* i_pPainter, const QRect& i_rect )
//------------------------------------------------------------------------------
{
    QPen  penDark(m_colDark);
    QPen  penLight(m_colLight);
    int   idxFrame;
    int   xLeft;
    int   xRight;
    int   yTop;
    int   yBottom;

    i_pPainter->save();

    xLeft   = i_rect.left();
    yTop    = i_rect.top();
    xRight  = i_rect.right();
    yBottom = i_rect.bottom();

    for( idxFrame = 0; idxFrame < m_iFrameWidth; idxFrame++, xLeft++, yTop++, xRight--, yBottom-- )
    {
        i_pPainter->setPen(penLight);
        i_pPainter->drawLine(xLeft,yTop,xRight,yTop);
        i_pPainter->drawLine(xLeft,yTop,xLeft,yBottom);
        i_pPainter->setPen(penDark);
        i_pPainter->drawLine(xLeft,yBottom,xRight,yBottom);
        i_pPainter->drawLine(xRight,yTop,xRight,yBottom);
    }
    i_pPainter->restore();

} // draw
//lint +e1961

//------------------------------------------------------------------------------
SFrameStyle* SFrameStyle3DRaised::clone() const
//------------------------------------------------------------------------------
{
    return new SFrameStyle3DRaised(*this);
}

