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

#include <QtGui/qimage.h>
#include <QtGui/qpainter.h>
#include "ZSDiagram/ZSDiagramImageStyles.h"
#include "ZSDiagram/ZSDiagramFrameStyles.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::Diagram;


/*******************************************************************************
class ZSDIAGRAMDLL_API CImageStyle
*******************************************************************************/

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CImageStyle::CImageStyle() :
//------------------------------------------------------------------------------
    m_ptPos(),
    m_size(),
    m_hotSpot(EImageHotSpotCenterCenter),
    m_pFrameStyle(nullptr),
    m_colBg(Qt::white),
    m_brushStyle(Qt::NoBrush),
    m_strText(),
    m_colText(Qt::black),
    m_fntText(),
    m_iTextMarginTop(0),
    m_iTextMarginBottom(0),
    m_iTextMarginLeft(0),
    m_iTextMarginRight(0),
    m_iTextAlignmentFlags(Qt::AlignVCenter|Qt::AlignHCenter)
{
} // ctor

//------------------------------------------------------------------------------
CImageStyle::CImageStyle( const CImageStyle& i_imageStyleOther ) :
//------------------------------------------------------------------------------
    m_ptPos(),
    m_size(i_imageStyleOther.m_size),
    m_hotSpot(i_imageStyleOther.m_hotSpot),
    m_pFrameStyle(nullptr),
    m_colBg(i_imageStyleOther.m_colBg),
    m_brushStyle(i_imageStyleOther.m_brushStyle),
    m_strText(i_imageStyleOther.m_strText),
    m_colText(i_imageStyleOther.m_colText),
    m_fntText(i_imageStyleOther.m_fntText),
    m_iTextMarginTop(i_imageStyleOther.m_iTextMarginTop),
    m_iTextMarginBottom(i_imageStyleOther.m_iTextMarginBottom),
    m_iTextMarginLeft(i_imageStyleOther.m_iTextMarginLeft),
    m_iTextMarginRight(i_imageStyleOther.m_iTextMarginRight),
    m_iTextAlignmentFlags(i_imageStyleOther.m_iTextAlignmentFlags)
{
    if( i_imageStyleOther.m_pFrameStyle != nullptr )
    {
        m_pFrameStyle = i_imageStyleOther.m_pFrameStyle->clone();
    }

} // copy ctor

//------------------------------------------------------------------------------
CImageStyle::~CImageStyle()
//------------------------------------------------------------------------------
{
    delete m_pFrameStyle;
    m_pFrameStyle = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CImageStyle::setPos( const QPoint& i_ptPos )
//------------------------------------------------------------------------------
{
    m_ptPos = i_ptPos;
}

//------------------------------------------------------------------------------
QPoint CImageStyle::getPos() const
//------------------------------------------------------------------------------
{
    return m_ptPos;
}

//------------------------------------------------------------------------------
void CImageStyle::setSize( int i_cxWidth, int i_cyHeight )
//------------------------------------------------------------------------------
{
    m_size.setWidth(i_cxWidth);
    m_size.setHeight(i_cyHeight);
}

//------------------------------------------------------------------------------
void CImageStyle::setSize( const QSize& i_size )
//------------------------------------------------------------------------------
{
    m_size = i_size;
}

//------------------------------------------------------------------------------
QSize CImageStyle::getSize() const
//------------------------------------------------------------------------------
{
    return m_size;
}

//------------------------------------------------------------------------------
void CImageStyle::setHotSpot( EImageHotSpot i_hotSpot )
//------------------------------------------------------------------------------
{
    m_hotSpot = i_hotSpot;
}

//------------------------------------------------------------------------------
CImageStyle::EImageHotSpot CImageStyle::getHotSpot() const
//------------------------------------------------------------------------------
{
    return m_hotSpot;
}

//------------------------------------------------------------------------------
QRect CImageStyle::boundingRect() const
//------------------------------------------------------------------------------
{
    QRect rect;

    if( m_size.isValid() )
    {
        // Cause of rounding inaccuracy (e.g. 13/2 = 6) the bounding rect will be increased.
        rect.setSize(m_size);

        switch( m_hotSpot )
        {
            case EImageHotSpotTopLeft:
            {
                rect.moveTop( m_ptPos.y() );
                rect.moveLeft( m_ptPos.x() );
                break;
            }
            case EImageHotSpotTopCenter:
            {
                rect.moveTop( m_ptPos.y() );
                rect.moveLeft( m_ptPos.x() - m_size.width()/2 );
                break;
            }
            case EImageHotSpotTopRight:
            {
                rect.moveTop( m_ptPos.y() );
                rect.moveLeft( m_ptPos.x() - m_size.width() );
                break;
            }
            case EImageHotSpotBottomLeft:
            {
                rect.moveTop( m_ptPos.y() - m_size.height() );
                rect.moveLeft( m_ptPos.x() );
                break;
            }
            case EImageHotSpotBottomCenter:
            {
                rect.moveTop( m_ptPos.y() - m_size.height() );
                rect.moveLeft( m_ptPos.x() - m_size.width()/2 );
                break;
            }
            case EImageHotSpotBottomRight:
            {
                rect.moveTop( m_ptPos.y() - m_size.height() );
                rect.moveLeft( m_ptPos.x() - m_size.width() );
                break;
            }
            case EImageHotSpotLeftCenter:
            {
                rect.moveTop( m_ptPos.y() - m_size.height()/2 );
                rect.moveLeft( m_ptPos.x() );
                break;
            }
            case EImageHotSpotRightCenter:
            {
                rect.moveTop( m_ptPos.y() - m_size.height()/2 );
                rect.moveLeft( m_ptPos.x() - m_size.width() );
                break;
            }
            case EImageHotSpotCenterCenter:
            default:
            {
                rect.moveTop( m_ptPos.y() - m_size.height()/2 );
                rect.moveLeft( m_ptPos.x() - m_size.width()/2 );
                break;
            }
        }
    }
    return rect;

} // boundingRect

//------------------------------------------------------------------------------
void CImageStyle::setFrameStyle( SFrameStyle* i_pFrameStyle )
//------------------------------------------------------------------------------
{
    delete m_pFrameStyle;
    m_pFrameStyle = i_pFrameStyle;
}

//------------------------------------------------------------------------------
SFrameStyle* CImageStyle::getFrameStyle()
//------------------------------------------------------------------------------
{
    return m_pFrameStyle;
}

//------------------------------------------------------------------------------
void CImageStyle::setColBg( const QColor& i_col )
//------------------------------------------------------------------------------
{
    m_colBg = i_col;
}

//------------------------------------------------------------------------------
QColor CImageStyle::getColBg() const
//------------------------------------------------------------------------------
{
    return m_colBg;
}

//------------------------------------------------------------------------------
void CImageStyle::setBrushStyle( const Qt::BrushStyle& i_brushStyle )
//------------------------------------------------------------------------------
{
    m_brushStyle = i_brushStyle;
}

//------------------------------------------------------------------------------
Qt::BrushStyle CImageStyle::getBrushStyle() const
//------------------------------------------------------------------------------
{
    return m_brushStyle;
}

//------------------------------------------------------------------------------
void CImageStyle::setText( const QString& i_strText )
//------------------------------------------------------------------------------
{
    m_strText = i_strText;
}

//------------------------------------------------------------------------------
QString CImageStyle::getText() const
//------------------------------------------------------------------------------
{
    return m_strText;
}

//------------------------------------------------------------------------------
void CImageStyle::setColText( const QColor& i_col )
//------------------------------------------------------------------------------
{
    m_colText = i_col;
}

//------------------------------------------------------------------------------
QColor CImageStyle::getColText() const
//------------------------------------------------------------------------------
{
    return m_colText;
}

//------------------------------------------------------------------------------
void CImageStyle::setFont( const QFont& i_fnt )
//------------------------------------------------------------------------------
{
    m_fntText = i_fnt;
}

//------------------------------------------------------------------------------
QFont CImageStyle::getFont() const
//------------------------------------------------------------------------------
{
    return m_fntText;
}

//------------------------------------------------------------------------------
void CImageStyle::setTextMarginTop( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iTextMarginTop = i_iMargin;
}

//------------------------------------------------------------------------------
int CImageStyle::getTextMarginTop() const
//------------------------------------------------------------------------------
{
    return m_iTextMarginTop;
}

//------------------------------------------------------------------------------
void CImageStyle::setTextMarginBottom( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iTextMarginBottom = i_iMargin;
}

//------------------------------------------------------------------------------
int CImageStyle::getTextMarginBottom() const
//------------------------------------------------------------------------------
{
    return m_iTextMarginBottom;
}

//------------------------------------------------------------------------------
void CImageStyle::setTextMarginLeft( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iTextMarginLeft = i_iMargin;
}

//------------------------------------------------------------------------------
int CImageStyle::getTextMarginLeft() const
//------------------------------------------------------------------------------
{
    return m_iTextMarginLeft;
}

//------------------------------------------------------------------------------
void CImageStyle::setTextMarginRight( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iTextMarginRight = i_iMargin;
}

//------------------------------------------------------------------------------
int CImageStyle::getTextMarginRight() const
//------------------------------------------------------------------------------
{
    return m_iTextMarginRight;
}

//------------------------------------------------------------------------------
void CImageStyle::setTextAlignment( int i_iAlignmentFlags )
//------------------------------------------------------------------------------
{
    m_iTextAlignmentFlags = i_iAlignmentFlags;
}

//------------------------------------------------------------------------------
int CImageStyle::getTextAlignment() const
//------------------------------------------------------------------------------
{
    return m_iTextAlignmentFlags;
}


/*******************************************************************************
class CImageStylePixmap : public CImageStyle
*******************************************************************************/

/*==============================================================================
public: ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CImageStylePixmap::CImageStylePixmap() :
//------------------------------------------------------------------------------
    CImageStyle(),
    m_pxm(),
    m_bmpMask()
{
} // default ctor

//------------------------------------------------------------------------------
CImageStylePixmap::CImageStylePixmap(
    const QImage& i_img,
    const QImage& i_imgMask,
    const QColor& i_colBg ) :
//------------------------------------------------------------------------------
    CImageStyle(),
    m_pxm(),
    m_bmpMask()
{
    m_pxm = QPixmap::fromImage(i_img);
    m_bmpMask = QBitmap::fromImage(i_imgMask);
    m_pxm.setMask(m_bmpMask);

    setSize( m_pxm.size() );
    setColBg(i_colBg);

} // ctor

//------------------------------------------------------------------------------
CImageStylePixmap::CImageStylePixmap( const CImageStylePixmap& i_imageStyleOther ) :
//------------------------------------------------------------------------------
    CImageStyle(i_imageStyleOther),
    m_pxm(i_imageStyleOther.m_pxm),
    m_bmpMask(i_imageStyleOther.m_bmpMask)
{
} // copy ctor

//------------------------------------------------------------------------------
CImageStylePixmap::~CImageStylePixmap()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CImageStylePixmap::setPixmap( const QPixmap& i_pxm )
//------------------------------------------------------------------------------
{
    m_pxm = i_pxm;
}

//------------------------------------------------------------------------------
void CImageStylePixmap::setMask( const QBitmap& i_bmpMask )
//------------------------------------------------------------------------------
{
    m_bmpMask = i_bmpMask;
}

/*==============================================================================
public: // must overridables of base class CImageStyle
==============================================================================*/

//------------------------------------------------------------------------------
void CImageStylePixmap::draw( QPainter* i_pPainter ) const
//------------------------------------------------------------------------------
{
    QRect rect;

    // Initializing image rectangle with HotSpot = CenterCenter.
    // All combinations marked as "very likely not needed" or "not yet implemented"
    // are using this initialization as long as not implemented.

    rect.setLeft( m_ptPos.x() - m_pxm.width()/2 );
    rect.setTop( m_ptPos.y() - m_pxm.height()/2 );
    rect.setWidth( m_pxm.width() );
    rect.setHeight( m_pxm.height() );

    switch( m_hotSpot )
    {
        case EImageHotSpotCenterCenter: // default (see above)
        {
            break;
        }
        case EImageHotSpotTopCenter:    // not yet implemented
        case EImageHotSpotBottomCenter: // not yet implemented
        case EImageHotSpotLeftCenter:   // not yet implemented
        case EImageHotSpotRightCenter:  // not yet implemented
        case EImageHotSpotTopLeft:      // not yet implemented
        case EImageHotSpotTopRight:     // not yet implemented
        case EImageHotSpotBottomLeft:   // not yet implemented
        case EImageHotSpotBottomRight:  // not yet implemented
        default:
        {
            break;
        }
    }

    i_pPainter->drawPixmap(rect,m_pxm);

} // draw

//------------------------------------------------------------------------------
CImageStyle* CImageStylePixmap::clone() const
//------------------------------------------------------------------------------
{
    return new CImageStylePixmap(*this);
}


/*******************************************************************************
class CImageStyleTriangle : public CImageStyle
*******************************************************************************/

/*==============================================================================
public: ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CImageStyleTriangle::CImageStyleTriangle() :
//------------------------------------------------------------------------------
    CImageStyle(),
    m_topDir(ETriangleTopDirDown)
{
    setSize(15,17);
    setTopDir(m_topDir);
    setBrushStyle(Qt::SolidPattern);

} // default ctor

//------------------------------------------------------------------------------
CImageStyleTriangle::CImageStyleTriangle(
    const QSize&    i_size,
    ETriangleTopDir i_topDir,
    const QColor&   i_colBg ) :
//------------------------------------------------------------------------------
    CImageStyle(),
    m_topDir(i_topDir)
{
    setSize(i_size);
    setTopDir(m_topDir);
    setColBg(i_colBg);
    setBrushStyle(Qt::SolidPattern);

} // ctor

//------------------------------------------------------------------------------
CImageStyleTriangle::CImageStyleTriangle( const CImageStyleTriangle& i_imageStyleOther ) :
//------------------------------------------------------------------------------
    CImageStyle(i_imageStyleOther),
    m_topDir(i_imageStyleOther.m_topDir)
{
} // copy ctor

//------------------------------------------------------------------------------
CImageStyleTriangle::~CImageStyleTriangle()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CImageStyleTriangle::setTopDir( ETriangleTopDir i_topDir )
//------------------------------------------------------------------------------
{
    m_topDir = i_topDir;

    switch( m_topDir )
    {
        case ETriangleTopDirUp:
        {
            m_hotSpot = EImageHotSpotTopCenter;
            break;
        }
        case ETriangleTopDirLeft:
        {
            m_hotSpot = EImageHotSpotLeftCenter;
            break;
        }
        case ETriangleTopDirRight:
        {
            m_hotSpot = EImageHotSpotRightCenter;
            break;
        }
        case ETriangleTopDirDown:
        default:
        {
            m_hotSpot = EImageHotSpotBottomCenter;
            break;
        }
    }

} // setTopDir

//------------------------------------------------------------------------------
CImageStyleTriangle::ETriangleTopDir CImageStyleTriangle::getTopDir() const
//------------------------------------------------------------------------------
{
    return m_topDir;
}

/*==============================================================================
public: // must overridables of base class CImageStyle
==============================================================================*/

//------------------------------------------------------------------------------
void CImageStyleTriangle::draw( QPainter* i_pPainter ) const
//------------------------------------------------------------------------------
{
    i_pPainter->save();

    #if QT_VERSION >= 0x040100
    QPolygon ptArr(3);
    #else
    QPointArray ptArr(3);
    #endif

    // Initializing point array with HotSpot = BottomCenter and TopDirDown.
    // All combinations marked as "very likely not needed" are using this
    // initialization as long as not implemented.
    ptArr[0].setX( m_ptPos.x() );
    ptArr[0].setY( m_ptPos.y() );
    ptArr[1].setX( m_ptPos.x() - m_size.width()/2 );
    ptArr[1].setY( m_ptPos.y() - m_size.height() );
    ptArr[2].setX( m_ptPos.x() + m_size.width()/2 );
    ptArr[2].setY( m_ptPos.y() - m_size.height() );

    switch( m_hotSpot )
    {
        case EImageHotSpotTopCenter:
        {
            switch( m_topDir )
            {
                case ETriangleTopDirUp:
                {
                    ptArr[0].setX( m_ptPos.x() );
                    ptArr[0].setY( m_ptPos.y() );
                    ptArr[1].setX( m_ptPos.x() + m_size.width()/2 );
                    ptArr[1].setY( m_ptPos.y() + m_size.height() );
                    ptArr[2].setX( m_ptPos.x() - m_size.width()/2 );
                    ptArr[2].setY( m_ptPos.y() + m_size.height() );
                    break;
                }
                case ETriangleTopDirDown:   // very likely not needed
                case ETriangleTopDirLeft:   // very likely not needed
                case ETriangleTopDirRight:  // very likely not needed
                default:
                {
                    break;
                }
            }
            break;
        }
        case EImageHotSpotBottomCenter:
        {
            switch( m_topDir )
            {
                case ETriangleTopDirDown:
                {
                    // The point array has already been initialized with
                    // HotSpot = BottomCenter and TopDirDown (see above).
                    break;
                }
                case ETriangleTopDirUp:     // very likely not needed
                case ETriangleTopDirLeft:   // very likely not needed
                case ETriangleTopDirRight:  // very likely not needed
                default:
                {
                    break;
                }
            }
            break;
        }
        case EImageHotSpotLeftCenter:
        {
            switch( m_topDir )
            {
                case ETriangleTopDirLeft:
                {
                    ptArr[0].setX( m_ptPos.x() );
                    ptArr[0].setY( m_ptPos.y() );
                    ptArr[1].setX( m_ptPos.x() + m_size.width() );
                    ptArr[1].setY( m_ptPos.y() - m_size.height()/2 );
                    ptArr[2].setX( m_ptPos.x() + m_size.width() );
                    ptArr[2].setY( m_ptPos.y() + m_size.height()/2 );
                    break;
                }
                case ETriangleTopDirRight:
                {
                    ptArr[0].setX( m_ptPos.x() + m_size.width() );
                    ptArr[0].setY( m_ptPos.y() );
                    ptArr[1].setX( m_ptPos.x() );
                    ptArr[1].setY( m_ptPos.y() + m_size.height()/2 );
                    ptArr[2].setX( m_ptPos.x() );
                    ptArr[2].setY( m_ptPos.y() - m_size.height()/2 );
                    break;
                }
                case ETriangleTopDirUp:     // very likely not needed
                case ETriangleTopDirDown:   // very likely not needed
                default:
                {
                    break;
                }
            }
            break;
        }
        case EImageHotSpotRightCenter:
        {
            switch( m_topDir )
            {
                case ETriangleTopDirRight:
                {
                    ptArr[0].setX( m_ptPos.x() );
                    ptArr[0].setY( m_ptPos.y() );
                    ptArr[1].setX( m_ptPos.x() - m_size.width() );
                    ptArr[1].setY( m_ptPos.y() + m_size.height()/2 );
                    ptArr[2].setX( m_ptPos.x() - m_size.width() );
                    ptArr[2].setY( m_ptPos.y() - m_size.height()/2 );
                    break;
                }
                case ETriangleTopDirLeft:   // very likely not needed
                {
                    ptArr[0].setX( m_ptPos.x() - m_size.width() );
                    ptArr[0].setY( m_ptPos.y() );
                    ptArr[1].setX( m_ptPos.x() + m_size.width() );
                    ptArr[1].setY( m_ptPos.y() - m_size.height()/2 );
                    ptArr[2].setX( m_ptPos.x() + m_size.width() );
                    ptArr[2].setY( m_ptPos.y() + m_size.height()/2 );
                    break;
                }
                case ETriangleTopDirUp:     // very likely not needed
                case ETriangleTopDirDown:   // very likely not needed
                default:
                {
                    break;
                }
            }
            break;
        }
        case EImageHotSpotCenterCenter: // very likely not needed
        case EImageHotSpotTopLeft:      // very likely not needed
        case EImageHotSpotTopRight:     // very likely not needed
        case EImageHotSpotBottomLeft:   // very likely not needed
        case EImageHotSpotBottomRight:  // very likely not needed
        default:
        {
            break;
        }
    }

    if( m_brushStyle != Qt::NoBrush )
    {
        i_pPainter->setBrush( QBrush(m_colBg,m_brushStyle) );
    }
    #if QT_VERSION >= 0x040100
    i_pPainter->drawPolygon(ptArr);
    #else
    i_pPainter->drawPolygon(ptArr,true);
    #endif

    if( m_pFrameStyle != nullptr )
    {
        m_pFrameStyle->draw(i_pPainter,ptArr);
    }
    else
    {
        i_pPainter->setPen( QPen(m_colBg,Qt::SolidLine) );

        i_pPainter->drawLine(ptArr.at(0),ptArr.at(1));
        i_pPainter->drawLine(ptArr.at(1),ptArr.at(2));
        i_pPainter->drawLine(ptArr.at(2),ptArr.at(0));
    }

    if( !m_strText.isEmpty() )
    {
        QRect rectText;
        QRect rectTextTmp;

        rectTextTmp = boundingRect();

        rectText.setLeft( rectTextTmp.left() + m_iTextMarginLeft );
        rectText.setRight( rectTextTmp.right() - m_iTextMarginRight );
        rectText.setTop( rectTextTmp.top() + m_iTextMarginTop );
        rectText.setBottom( rectTextTmp.bottom() - m_iTextMarginBottom );

        i_pPainter->setPen( QPen(m_colText) );
        i_pPainter->setFont(m_fntText);
        i_pPainter->drawText(rectText,m_iTextAlignmentFlags,m_strText);
    }

    i_pPainter->restore();

} // draw

//------------------------------------------------------------------------------
CImageStyle* CImageStyleTriangle::clone() const
//------------------------------------------------------------------------------
{
    return new CImageStyleTriangle(*this);
}


/*******************************************************************************
class CImageStyleHCursorFocused : public CImageStyle
*******************************************************************************/

/*==============================================================================
public: ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CImageStyleHCursorFocused::CImageStyleHCursorFocused() :
//------------------------------------------------------------------------------
    CImageStyle(),
    m_pxm(":/ZSDiagram/SplitHCursorFocused"),
    m_bmpMask(":/ZSDiagram/SplitHCursorFocusedMask")
{
    m_pxm.setMask(m_bmpMask);

    setSize( m_pxm.size() );

} // default ctor

//------------------------------------------------------------------------------
CImageStyleHCursorFocused::CImageStyleHCursorFocused( const CImageStyleHCursorFocused& i_imageStyleOther ) :
//------------------------------------------------------------------------------
    CImageStyle(i_imageStyleOther),
    m_pxm(i_imageStyleOther.m_pxm),
    m_bmpMask(i_imageStyleOther.m_bmpMask)
{
    m_pxm.setMask(m_bmpMask);

} // copy ctor

//------------------------------------------------------------------------------
CImageStyleHCursorFocused::~CImageStyleHCursorFocused()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class CImageStyle
==============================================================================*/

//------------------------------------------------------------------------------
void CImageStyleHCursorFocused::setColBg( const QColor& i_col )
//------------------------------------------------------------------------------
{
    CImageStyle::setColBg(i_col);
    m_pxm.fill(m_colBg);
    m_pxm.setMask(m_bmpMask);
}

/*==============================================================================
public: // must overridables of base class CImageStyle
==============================================================================*/

//------------------------------------------------------------------------------
void CImageStyleHCursorFocused::draw( QPainter* i_pPainter ) const
//------------------------------------------------------------------------------
{
    QRect rect;

    // Initializing image rectangle with HotSpot = CenterCenter.
    // All combinations marked as "very likely not needed" or "not yet implemented"
    // are using this initialization as long as not implemented.

    rect.setLeft( m_ptPos.x() - m_pxm.width()/2 );
    rect.setTop( m_ptPos.y() - m_pxm.height()/2 );
    rect.setWidth( m_pxm.width() );
    rect.setHeight( m_pxm.height() );

    switch( m_hotSpot )
    {
        case EImageHotSpotCenterCenter: // default (see above)
        {
            break;
        }
        case EImageHotSpotTopCenter:    // not yet implemented
        case EImageHotSpotBottomCenter: // not yet implemented
        case EImageHotSpotLeftCenter:   // not yet implemented
        case EImageHotSpotRightCenter:  // not yet implemented
        case EImageHotSpotTopLeft:      // not yet implemented
        case EImageHotSpotTopRight:     // not yet implemented
        case EImageHotSpotBottomLeft:   // not yet implemented
        case EImageHotSpotBottomRight:  // not yet implemented
        default:
        {
            break;
        }
    }

    i_pPainter->drawPixmap(rect,m_pxm);

} // draw

//------------------------------------------------------------------------------
CImageStyle* CImageStyleHCursorFocused::clone() const
//------------------------------------------------------------------------------
{
    return new CImageStyleHCursorFocused(*this);
}


/*******************************************************************************
class CImageStyleHCursorEditing : public CImageStyle
*******************************************************************************/

/*==============================================================================
public: ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CImageStyleHCursorEditing::CImageStyleHCursorEditing() :
//------------------------------------------------------------------------------
    CImageStyle(),
    m_pxm(":/ZSDiagram/SplitHCursorEditing"),
    m_bmpMask(":/ZSDiagram/SplitHCursorEditingMask")
{
    m_pxm.setMask(m_bmpMask);

    setSize( m_pxm.size() );

} // default ctor

//------------------------------------------------------------------------------
CImageStyleHCursorEditing::CImageStyleHCursorEditing( const CImageStyleHCursorEditing& i_imageStyleOther ) :
//------------------------------------------------------------------------------
    CImageStyle(i_imageStyleOther),
    m_pxm(i_imageStyleOther.m_pxm),
    m_bmpMask(i_imageStyleOther.m_bmpMask)
{
    m_pxm.setMask(m_bmpMask);

} // copy ctor

//------------------------------------------------------------------------------
CImageStyleHCursorEditing::~CImageStyleHCursorEditing()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class CImageStyle
==============================================================================*/

//------------------------------------------------------------------------------
void CImageStyleHCursorEditing::setColBg( const QColor& i_col )
//------------------------------------------------------------------------------
{
    CImageStyle::setColBg(i_col);
    m_pxm.fill(m_colBg);
    m_pxm.setMask(m_bmpMask);
}

/*==============================================================================
public: // must overridables of base class CImageStyle
==============================================================================*/

//------------------------------------------------------------------------------
void CImageStyleHCursorEditing::draw( QPainter* i_pPainter ) const
//------------------------------------------------------------------------------
{
    QRect rect;

    // Initializing image rectangle with HotSpot = CenterCenter.
    // All combinations marked as "very likely not needed" or "not yet implemented"
    // are using this initialization as long as not implemented.

    rect.setLeft( m_ptPos.x() - m_pxm.width()/2 );
    rect.setTop( m_ptPos.y() - m_pxm.height()/2 );
    rect.setWidth( m_pxm.width() );
    rect.setHeight( m_pxm.height() );

    switch( m_hotSpot )
    {
        case EImageHotSpotCenterCenter: // default (see above)
        {
            break;
        }
        case EImageHotSpotTopCenter:    // not yet implemented
        case EImageHotSpotBottomCenter: // not yet implemented
        case EImageHotSpotLeftCenter:   // not yet implemented
        case EImageHotSpotRightCenter:  // not yet implemented
        case EImageHotSpotTopLeft:      // not yet implemented
        case EImageHotSpotTopRight:     // not yet implemented
        case EImageHotSpotBottomLeft:   // not yet implemented
        case EImageHotSpotBottomRight:  // not yet implemented
        default:
        {
            break;
        }
    }

    i_pPainter->drawPixmap(rect,m_pxm);

} // draw

//------------------------------------------------------------------------------
CImageStyle* CImageStyleHCursorEditing::clone() const
//------------------------------------------------------------------------------
{
    return new CImageStyleHCursorEditing(*this);
}


/*******************************************************************************
class CImageStyleVCursorFocused : public CImageStyle
*******************************************************************************/

/*==============================================================================
public: ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CImageStyleVCursorFocused::CImageStyleVCursorFocused() :
//------------------------------------------------------------------------------
    CImageStyle(),
    m_pxm(":/ZSDiagram/SplitVCursorFocused"),
    m_bmpMask(":/ZSDiagram/SplitVCursorFocusedMask")
{
    m_pxm.setMask(m_bmpMask);

    setSize( m_pxm.size() );

} // default ctor

//------------------------------------------------------------------------------
CImageStyleVCursorFocused::CImageStyleVCursorFocused( const CImageStyleVCursorFocused& i_imageStyleOther ) :
//------------------------------------------------------------------------------
    CImageStyle(i_imageStyleOther),
    m_pxm(i_imageStyleOther.m_pxm),
    m_bmpMask(i_imageStyleOther.m_bmpMask)
{
} // copy ctor

//------------------------------------------------------------------------------
CImageStyleVCursorFocused::~CImageStyleVCursorFocused()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class CImageStyle
==============================================================================*/

//------------------------------------------------------------------------------
void CImageStyleVCursorFocused::setColBg( const QColor& i_col )
//------------------------------------------------------------------------------
{
    CImageStyle::setColBg(i_col);
    m_pxm.fill(m_colBg);
}

/*==============================================================================
public: // must overridables of base class CImageStyle
==============================================================================*/

//------------------------------------------------------------------------------
void CImageStyleVCursorFocused::draw( QPainter* i_pPainter ) const
//------------------------------------------------------------------------------
{
    QRect rect;

    // Initializing image rectangle with HotSpot = CenterCenter.
    // All combinations marked as "very likely not needed" or "not yet implemented"
    // are using this initialization as long as not implemented.

    rect.setLeft( m_ptPos.x() - m_pxm.width()/2 );
    rect.setTop( m_ptPos.y() - m_pxm.height()/2 );
    rect.setWidth( m_pxm.width() );
    rect.setHeight( m_pxm.height() );

    switch( m_hotSpot )
    {
        case EImageHotSpotCenterCenter: // default (see above)
        {
            break;
        }
        case EImageHotSpotTopCenter:    // not yet implemented
        case EImageHotSpotBottomCenter: // not yet implemented
        case EImageHotSpotLeftCenter:   // not yet implemented
        case EImageHotSpotRightCenter:  // not yet implemented
        case EImageHotSpotTopLeft:      // not yet implemented
        case EImageHotSpotTopRight:     // not yet implemented
        case EImageHotSpotBottomLeft:   // not yet implemented
        case EImageHotSpotBottomRight:  // not yet implemented
        default:
        {
            break;
        }
    }

    i_pPainter->drawPixmap(rect,m_pxm);

} // draw

//------------------------------------------------------------------------------
CImageStyle* CImageStyleVCursorFocused::clone() const
//------------------------------------------------------------------------------
{
    return new CImageStyleVCursorFocused(*this);
}


/*******************************************************************************
class CImageStyleVCursorEditing : public CImageStyle
*******************************************************************************/

/*==============================================================================
public: ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CImageStyleVCursorEditing::CImageStyleVCursorEditing() :
//------------------------------------------------------------------------------
    CImageStyle(),
    m_pxm(":/ZSDiagram/SplitVCursorEditing"),
    m_bmpMask(":/ZSDiagram/SplitVCursorEditingMask")
{
    m_pxm.setMask(m_bmpMask);

    setSize( m_pxm.size() );

} // default ctor

//------------------------------------------------------------------------------
CImageStyleVCursorEditing::CImageStyleVCursorEditing( const CImageStyleVCursorEditing& i_imageStyleOther ) :
//------------------------------------------------------------------------------
    CImageStyle(i_imageStyleOther),
    m_pxm(i_imageStyleOther.m_pxm),
    m_bmpMask(i_imageStyleOther.m_bmpMask)
{
} // copy ctor

//------------------------------------------------------------------------------
CImageStyleVCursorEditing::~CImageStyleVCursorEditing()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class CImageStyle
==============================================================================*/

//------------------------------------------------------------------------------
void CImageStyleVCursorEditing::setColBg( const QColor& i_col )
//------------------------------------------------------------------------------
{
    CImageStyle::setColBg(i_col);
    m_pxm.fill(m_colBg);
}

/*==============================================================================
public: // must overridables of base class CImageStyle
==============================================================================*/

//------------------------------------------------------------------------------
void CImageStyleVCursorEditing::draw( QPainter* i_pPainter ) const
//------------------------------------------------------------------------------
{
    QRect rect;

    // Initializing image rectangle with HotSpot = CenterCenter.
    // All combinations marked as "very likely not needed" or "not yet implemented"
    // are using this initialization as long as not implemented.

    rect.setLeft( m_ptPos.x() - m_pxm.width()/2 );
    rect.setTop( m_ptPos.y() - m_pxm.height()/2 );
    rect.setWidth( m_pxm.width() );
    rect.setHeight( m_pxm.height() );

    switch( m_hotSpot )
    {
        case EImageHotSpotCenterCenter: // default (see above)
        {
            break;
        }
        case EImageHotSpotTopCenter:    // not yet implemented
        case EImageHotSpotBottomCenter: // not yet implemented
        case EImageHotSpotLeftCenter:   // not yet implemented
        case EImageHotSpotRightCenter:  // not yet implemented
        case EImageHotSpotTopLeft:      // not yet implemented
        case EImageHotSpotTopRight:     // not yet implemented
        case EImageHotSpotBottomLeft:   // not yet implemented
        case EImageHotSpotBottomRight:  // not yet implemented
        default:
        {
            break;
        }
    }

    i_pPainter->drawPixmap(rect,m_pxm);

} // draw

//------------------------------------------------------------------------------
CImageStyle* CImageStyleVCursorEditing::clone() const
//------------------------------------------------------------------------------
{
    return new CImageStyleVCursorEditing(*this);
}
