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

#ifndef ZSDiagramFrameStyles_h
#define ZSDiagramFrameStyles_h

#include <QtGui/qcolor.h>
#include "ZSDiagram/ZSDiagramDllMain.h"

namespace ZS
{
namespace Diagram
{
//******************************************************************************
struct ZSDIAGRAMDLL_API SFrameStyle
//******************************************************************************
{
public: // ctors and dtor
    SFrameStyle(
        int                 i_iFrameWidth = 1,
        const QColor&       i_col = Qt::black,
        const Qt::PenStyle& i_penStyle = Qt::SolidLine );
    SFrameStyle( const SFrameStyle& i_frameStyleOther );
    virtual ~SFrameStyle();
public: // overridables
    virtual SFrameStyle* clone() const;
    virtual void setFrameWidth( int i_iFrameWidth );
    virtual int getFrameWidth() const;
    virtual void setCol( const QColor& i_col );
    virtual QColor getCol() const;
    virtual void setPenStyle( const Qt::PenStyle& i_penStyle );
    virtual Qt::PenStyle getPenStyle() const;
    virtual void draw( QPainter* i_pPainter, const QRect& i_rect );
    #if QT_VERSION >= 0x040100
    virtual void draw( QPainter* i_pPainter, const QPolygon& i_ptArr );
    #else
    virtual void draw( QPainter* i_pPainter, const QPointArray& i_ptArr );
    #endif
protected: // instance members
    int          m_iFrameWidth;
    QColor       m_col;
    Qt::PenStyle m_penStyle;
};

//******************************************************************************
struct ZSDIAGRAMDLL_API SFrameStyle3DSunken : public SFrameStyle
//******************************************************************************
{
public: // ctors and dtor
    SFrameStyle3DSunken(
        const QColor& i_colLight = Qt::white,
        const QColor& i_colDark  = Qt::darkGray,
        int           i_iFrameWidth = 2 );
    SFrameStyle3DSunken( const SFrameStyle3DSunken& i_frameStyleOther );
    virtual ~SFrameStyle3DSunken();
public: // instance methods
    void setColLight( const QColor& i_col );
    QColor getColLight() const;
    void setColDark( const QColor& i_col );
    QColor getColDark() const;
public: // overridables of base class SFrameStyle
    virtual SFrameStyle* clone() const;
    virtual void draw( QPainter* i_pPainter, const QRect& i_rect );
protected: // instance members
    QColor m_colLight;
    QColor m_colDark;
};

//******************************************************************************
struct ZSDIAGRAMDLL_API SFrameStyle3DRaised : public SFrameStyle
//******************************************************************************
{
public: // ctors and dtor
    SFrameStyle3DRaised(
        const QColor& i_colLight = Qt::white,
        const QColor& i_colDark  = Qt::darkGray,
        int           i_iFrameWidth = 2 );
    SFrameStyle3DRaised( const SFrameStyle3DRaised& i_frameStyleOther );
    virtual ~SFrameStyle3DRaised();
public: // instance methods
    void setColLight( const QColor& i_col );
    QColor getColLight() const;
    void setColDark( const QColor& i_col );
    QColor getColDark() const;
public: // overridables of base class SFrameStyle
    virtual SFrameStyle* clone() const;
    virtual void draw( QPainter* i_pPainter, const QRect& i_rect );
protected: // instance members
    QColor m_colLight;
    QColor m_colDark;
};

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagramFrameStyles_h
