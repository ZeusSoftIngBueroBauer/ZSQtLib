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

#ifndef ZSDiagramImageStyles_h
#define ZSDiagramImageStyles_h

#include <QtCore/qglobal.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qcolor.h>
#include <QtGui/qpixmap.h>
#include "ZSDiagram/ZSDiagramDllMain.h"

namespace ZS
{
namespace Diagram
{
struct SFrameStyle;

//******************************************************************************
class ZSDIAGRAMDLL_API CImageStyle
//******************************************************************************
{
public: // type definitions and constants
    // Please note that it depends on the image style if a hot spot position is
    // meaningful or not. E.g. on using a triangle with top direction up a hot
    // spot position "LeftCenter" would not really be useful. Thats why the
    // derived image style classes may sometimes implicitly set the hot spot
    // position according to their needs.
    // Please also note if using a centered horizontal hot spot positions
    // (TopCenter or BottomCenter) the image width should be an odd number
    // and if using a centered vertical hot spot positions (LeftCenter or
    // RightCenter) the image height should be an odd number. Otherwise the
    // image would not really be centered to the image position.
    typedef enum {
        EImageHotSpotTopLeft      = 0,
        EImageHotSpotTopCenter    = 1,
        EImageHotSpotTopRight     = 2,
        EImageHotSpotBottomLeft   = 3,
        EImageHotSpotBottomCenter = 4,
        EImageHotSpotBottomRight  = 5,
        EImageHotSpotLeftCenter   = 6,
        EImageHotSpotRightCenter  = 7,
        EImageHotSpotCenterCenter = 8
    }   EImageHotSpot;
public: // dtor
    virtual ~CImageStyle();
protected: // ctors
    CImageStyle();
    CImageStyle( const CImageStyle& i_imageStyleOther );
public: // overridables
    virtual void setPos( const QPoint& i_ptPos );
    virtual QPoint getPos() const;
    virtual void setSize( int i_cxWidth, int i_cyHeight );
    virtual void setSize( const QSize& i_size );
    virtual QSize getSize() const;
    virtual void setHotSpot( EImageHotSpot i_hotSpot );
    virtual EImageHotSpot getHotSpot() const;
    virtual QRect boundingRect() const;
    virtual void setFrameStyle( SFrameStyle* i_pFrameStyle );
    virtual SFrameStyle* getFrameStyle();
    virtual void setColBg( const QColor& i_col );
    virtual QColor getColBg() const;
    virtual void setBrushStyle( const Qt::BrushStyle& i_brushStyle );
    virtual Qt::BrushStyle getBrushStyle() const;
    virtual void setText( const QString& i_strText );
    virtual QString getText() const;
    virtual void setColText( const QColor& i_col );
    virtual QColor getColText() const;
    virtual void setFont( const QFont& i_fnt );
    virtual QFont getFont() const;
    virtual void setTextMarginTop( int i_iMargin );
    virtual int getTextMarginTop() const;
    virtual void setTextMarginBottom( int i_iMargin );
    virtual int getTextMarginBottom() const;
    virtual void setTextMarginLeft( int i_iMargin );
    virtual int getTextMarginLeft() const;
    virtual void setTextMarginRight( int i_iMargin );
    virtual int getTextMarginRight() const;
    virtual void setTextAlignment( int i_iAlignmentFlags );
    virtual int getTextAlignment() const;
public: // must overridables
    virtual void draw( QPainter* i_pPainter ) const = 0;
    virtual CImageStyle* clone() const = 0;
protected: // instance members
    QPoint         m_ptPos;
    QSize          m_size;
    EImageHotSpot  m_hotSpot;
    SFrameStyle*   m_pFrameStyle;
    QColor         m_colBg;
    Qt::BrushStyle m_brushStyle;
    QString        m_strText;
    QColor         m_colText;
    QFont          m_fntText;
    int            m_iTextMarginTop;
    int            m_iTextMarginBottom;
    int            m_iTextMarginLeft;
    int            m_iTextMarginRight;
    int            m_iTextAlignmentFlags;

}; // class CImageStyle

//******************************************************************************
class ZSDIAGRAMDLL_API CImageStylePixmap : public CImageStyle
//******************************************************************************
{
public: // ctors and dtor
    CImageStylePixmap();
    CImageStylePixmap(
        const QImage&  i_img,
        const QImage&  i_imgMask,
        const QColor& i_colBg = Qt::white );
    CImageStylePixmap( const CImageStylePixmap& i_imageStyleOther );
    virtual ~CImageStylePixmap();
public: // instance methods
    void setPixmap( const QPixmap& i_pxm );
    void setMask( const QBitmap& i_bmpMask );
public: // must overridables of base class CImageStyle
    virtual void draw( QPainter* i_pPainter ) const;
    virtual CImageStyle* clone() const;
protected: // instance members
    QPixmap m_pxm;
    QBitmap m_bmpMask;

}; // class CImageStylePixmap

//******************************************************************************
class ZSDIAGRAMDLL_API CImageStyleTriangle : public CImageStyle
//******************************************************************************
{
public: // type definitions and constants
    typedef enum {                  // The hot spots most meaningful values are
        ETriangleTopDirUp    = 1,   // = TopCenter
        ETriangleTopDirDown  = 2,   // = BottomCenter
        ETriangleTopDirLeft  = 3,   // = LeftCenter
        ETriangleTopDirRight = 4    // = RightCenter
    }   ETriangleTopDir;            // and will be implicitly set on applying "setTopDir".
public: // ctors and dtor
    CImageStyleTriangle();
    CImageStyleTriangle(
        const QSize&    i_size,
        ETriangleTopDir i_topDir,
        const QColor&   i_colBg = Qt::white );
    CImageStyleTriangle( const CImageStyleTriangle& i_imageStyleOther );
    virtual ~CImageStyleTriangle();
public: // instance methods
    void setTopDir( ETriangleTopDir i_topDir ); // implicitly sets the hot spot position to the most meaningful value
    ETriangleTopDir getTopDir() const;
public: // must overridables of base class CImageStyle
    virtual void draw( QPainter* i_pPainter ) const;
    virtual CImageStyle* clone() const;
protected: // instance members
    ETriangleTopDir m_topDir;

}; // class CImageStyleTriangle

//******************************************************************************
class ZSDIAGRAMDLL_API CImageStyleHCursorFocused : public CImageStyle
//******************************************************************************
{
public: // ctors and dtor
    CImageStyleHCursorFocused();
    CImageStyleHCursorFocused( const CImageStyleHCursorFocused& i_imageStyleOther );
    virtual ~CImageStyleHCursorFocused();
public: // overridables of base class CImageStyle
    virtual void setColBg( const QColor& i_col );
public: // must overridables of base class CImageStyle
    virtual void draw( QPainter* i_pPainter ) const;
    virtual CImageStyle* clone() const;
protected: // instance members
    QPixmap m_pxm;
    QBitmap m_bmpMask;

}; // class CImageStyleHCursorFocused

//******************************************************************************
class ZSDIAGRAMDLL_API CImageStyleHCursorEditing : public CImageStyle
//******************************************************************************
{
public: // ctors and dtor
    CImageStyleHCursorEditing();
    CImageStyleHCursorEditing( const CImageStyleHCursorEditing& i_imageStyleOther );
    virtual ~CImageStyleHCursorEditing();
public: // overridables of base class CImageStyle
    virtual void setColBg( const QColor& i_col );
public: // must overridables of base class CImageStyle
    virtual void draw( QPainter* i_pPainter ) const;
    virtual CImageStyle* clone() const;
protected: // instance members
    QPixmap m_pxm;
    QBitmap m_bmpMask;

}; // class CImageStyleHCursorEditing

//******************************************************************************
class ZSDIAGRAMDLL_API CImageStyleVCursorFocused : public CImageStyle
//******************************************************************************
{
public: // ctors and dtor
    CImageStyleVCursorFocused();
    CImageStyleVCursorFocused( const CImageStyleVCursorFocused& i_imageStyleOther );
    virtual ~CImageStyleVCursorFocused();
public: // overridables of base class CImageStyle
    virtual void setColBg( const QColor& i_col );
public: // must overridables of base class CImageStyle
    virtual void draw( QPainter* i_pPainter ) const;
    virtual CImageStyle* clone() const;
protected: // instance members
    QPixmap m_pxm;
    QBitmap m_bmpMask;

}; // class CImageStyleVCursorFocused

//******************************************************************************
class ZSDIAGRAMDLL_API CImageStyleVCursorEditing : public CImageStyle
//******************************************************************************
{
public: // ctors and dtor
    CImageStyleVCursorEditing();
    CImageStyleVCursorEditing( const CImageStyleVCursorEditing& i_imageStyleOther );
    virtual ~CImageStyleVCursorEditing();
public: // overridables of base class CImageStyle
    virtual void setColBg( const QColor& i_col );
public: // must overridables of base class CImageStyle
    virtual void draw( QPainter* i_pPainter ) const;
    virtual CImageStyle* clone() const;
protected: // instance members
    QPixmap m_pxm;
    QBitmap m_bmpMask;

}; // class CImageStyleVCursorEditing

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagramImageStyles_h
