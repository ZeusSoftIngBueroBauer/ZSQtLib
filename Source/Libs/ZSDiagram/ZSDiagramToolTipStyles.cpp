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

#include <QtGui/qpainter.h>
#include "ZSDiagram/ZSDiagramToolTipStyles.h"
#include "ZSDiagram/ZSDiagramFrameStyles.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::Diagram;
using namespace ZS::PhysVal;


/*******************************************************************************
struct ZSDIAGRAMDLL_API CToolTipStyle
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CToolTipStyle::CToolTipStyle() :
//------------------------------------------------------------------------------
    m_strToolTip(),
    //m_bVisible(false),
    m_colFg(Qt::black),
    m_colBg(Qt::gray),
    m_fnt(),
    m_cxOffs(8),
    m_cyOffs(8),
    m_pFrameStyle(nullptr),
    m_iMarginTop(1),
    m_iMarginBottom(1),
    m_iMarginLeft(4),
    m_iMarginRight(4),
    m_cxArrowWidth(8),
    m_ptArrowHeadPos(),
    m_toolTipPos(EToolTipPosTopCenter),
    m_rectToolTip(),
    m_rectToolTipArrow(),
    m_iPtArrToolTipLength(0),
    m_ptarrToolTip(7),
    m_arpPtArrToolTip(nullptr)
{
    m_pFrameStyle = new SFrameStyle3DRaised(Qt::white,Qt::darkGray,2);

    if( m_pFrameStyle->getFrameWidth() > 0 )
    {
        int idxFrame;

        #if QT_VERSION >= 0x040100
        m_arpPtArrToolTip = new QPolygon*[m_pFrameStyle->getFrameWidth()];
        #else
        m_arpPtArrToolTip = new QPointArray*[m_pFrameStyle->getFrameWidth()];
        #endif

        for( idxFrame = 0; idxFrame < m_pFrameStyle->getFrameWidth(); idxFrame++ )
        {
            #if QT_VERSION >= 0x040100
            m_arpPtArrToolTip[idxFrame] = new QPolygon(m_ptarrToolTip.size()+1);
            #else
            m_arpPtArrToolTip[idxFrame] = new QPointArray(m_ptarrToolTip.size()+1);
            #endif
        }
    }

} // default ctor

//------------------------------------------------------------------------------
CToolTipStyle::CToolTipStyle(
    const QColor& i_colFg,
    const QColor& i_colBg,
    const QFont&  i_fnt,
    int           i_cxOffs,
    int           i_cyOffs,
    SFrameStyle*  i_pFrameStyle,
    int           i_iMarginTop,
    int           i_iMarginBottom,
    int           i_iMarginLeft,
    int           i_iMarginRight,
    int           i_cxArrowWidth ) :
//------------------------------------------------------------------------------
    m_strToolTip(),
    //m_bVisible(false),
    m_colFg(i_colFg),
    m_colBg(i_colBg),
    m_fnt(i_fnt),
    m_cxOffs(i_cxOffs),
    m_cyOffs(i_cyOffs),
    m_pFrameStyle(i_pFrameStyle),
    m_iMarginTop(i_iMarginTop),
    m_iMarginBottom(i_iMarginBottom),
    m_iMarginLeft(i_iMarginLeft),
    m_iMarginRight(i_iMarginRight),
    m_cxArrowWidth(i_cxArrowWidth),
    m_ptArrowHeadPos(),
    m_toolTipPos(EToolTipPosTopCenter),
    m_rectToolTip(),
    m_rectToolTipArrow(),
    m_iPtArrToolTipLength(0),
    m_ptarrToolTip(7),
    m_arpPtArrToolTip(nullptr)
{
    if( m_pFrameStyle != nullptr && m_pFrameStyle->getFrameWidth() > 0 )
    {
        int idxFrame;

        #if QT_VERSION >= 0x040100
        m_arpPtArrToolTip = new QPolygon*[m_pFrameStyle->getFrameWidth()];
        #else
        m_arpPtArrToolTip = new QPointArray*[m_pFrameStyle->getFrameWidth()];
        #endif

        for( idxFrame = 0; idxFrame < m_pFrameStyle->getFrameWidth(); idxFrame++ )
        {
            #if QT_VERSION >= 0x040100
            m_arpPtArrToolTip[idxFrame] = new QPolygon(m_ptarrToolTip.size()+1);
            #else
            m_arpPtArrToolTip[idxFrame] = new QPointArray(m_ptarrToolTip.size()+1);
            #endif
        }
    }

} // ctor

//------------------------------------------------------------------------------
CToolTipStyle::CToolTipStyle( const CToolTipStyle& i_toolTipStyleOther ) :
//------------------------------------------------------------------------------
    m_strToolTip(i_toolTipStyleOther.m_strToolTip),
    //m_bVisible(i_toolTipStyleOther.m_bVisible),
    m_colFg(i_toolTipStyleOther.m_colFg),
    m_colBg(i_toolTipStyleOther.m_colBg),
    m_fnt(i_toolTipStyleOther.m_fnt),
    m_cxOffs(i_toolTipStyleOther.m_cxOffs),
    m_cyOffs(i_toolTipStyleOther.m_cyOffs),
    m_pFrameStyle(nullptr),
    m_iMarginTop(i_toolTipStyleOther.m_iMarginTop),
    m_iMarginBottom(i_toolTipStyleOther.m_iMarginBottom),
    m_iMarginLeft(i_toolTipStyleOther.m_iMarginLeft),
    m_iMarginRight(i_toolTipStyleOther.m_iMarginRight),
    m_cxArrowWidth(i_toolTipStyleOther.m_cxArrowWidth),
    m_ptArrowHeadPos(i_toolTipStyleOther.m_ptArrowHeadPos),
    m_toolTipPos(i_toolTipStyleOther.m_toolTipPos),
    m_rectToolTip(),
    m_rectToolTipArrow(),
    m_iPtArrToolTipLength(0),
    m_ptarrToolTip(7),
    m_arpPtArrToolTip(nullptr)
{
    if( i_toolTipStyleOther.m_pFrameStyle != nullptr )
    {
        m_pFrameStyle = i_toolTipStyleOther.m_pFrameStyle->clone();

        if( m_pFrameStyle->getFrameWidth() > 0 )
        {
            int idxFrame;

            #if QT_VERSION >= 0x040100
            m_arpPtArrToolTip = new QPolygon*[m_pFrameStyle->getFrameWidth()];
            #else
            m_arpPtArrToolTip = new QPointArray*[m_pFrameStyle->getFrameWidth()];
            #endif

            for( idxFrame = 0; idxFrame < m_pFrameStyle->getFrameWidth(); idxFrame++ )
            {
                #if QT_VERSION >= 0x040100
                m_arpPtArrToolTip[idxFrame] = new QPolygon(m_ptarrToolTip.size()+1);
                #else
                m_arpPtArrToolTip[idxFrame] = new QPointArray(m_ptarrToolTip.size()+1);
                #endif
            }
        }
    }

} // copy ctor

//------------------------------------------------------------------------------
CToolTipStyle::~CToolTipStyle()
//------------------------------------------------------------------------------
{
    if( m_pFrameStyle != nullptr && m_pFrameStyle->getFrameWidth() > 0 )
    {
        int idxFrame;

        if( m_arpPtArrToolTip != nullptr )
        {
            for( idxFrame = 0; idxFrame < m_pFrameStyle->getFrameWidth(); idxFrame++ )
            {
                try
                {
                    delete m_arpPtArrToolTip[idxFrame];
                }
                catch(...)
                {
                }
                m_arpPtArrToolTip[idxFrame] = nullptr;
            }
        }
    }

    delete [] m_arpPtArrToolTip;
    m_arpPtArrToolTip = nullptr;

    delete m_pFrameStyle;
    m_pFrameStyle = nullptr;

} // dtor


/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CToolTipStyle& CToolTipStyle::operator = ( const CToolTipStyle& i_toolTipStyleOther )
//------------------------------------------------------------------------------
{
    if( &i_toolTipStyleOther != this )
    {
        int idxFrame;

        m_strToolTip     = i_toolTipStyleOther.m_strToolTip;
        //m_bVisible       = i_toolTipStyleOther.m_bVisible;
        m_colFg          = i_toolTipStyleOther.m_colFg;
        m_colBg          = i_toolTipStyleOther.m_colBg;
        m_fnt            = i_toolTipStyleOther.m_fnt;
        m_cxOffs         = i_toolTipStyleOther.m_cxOffs;
        m_cyOffs         = i_toolTipStyleOther.m_cyOffs;
        m_iMarginTop     = i_toolTipStyleOther.m_iMarginTop;
        m_iMarginBottom  = i_toolTipStyleOther.m_iMarginBottom;
        m_iMarginLeft    = i_toolTipStyleOther.m_iMarginLeft;
        m_iMarginRight   = i_toolTipStyleOther.m_iMarginRight;
        m_cxArrowWidth   = i_toolTipStyleOther.m_cxArrowWidth;
        m_ptArrowHeadPos = i_toolTipStyleOther.m_ptArrowHeadPos;
        m_toolTipPos     = i_toolTipStyleOther.m_toolTipPos;
        m_cxArrowWidth   = i_toolTipStyleOther.m_cxArrowWidth;

        m_rectToolTip         = QRect();
        m_rectToolTipArrow    = QRect();
        m_iPtArrToolTipLength = 0;
        m_ptarrToolTip.resize(7);

        if( m_arpPtArrToolTip != nullptr && m_pFrameStyle != nullptr )
        {
            for( idxFrame = 0; idxFrame < m_pFrameStyle->getFrameWidth(); idxFrame++ )
            {
                try
                {
                    delete m_arpPtArrToolTip[idxFrame];
                }
                catch(...)
                {
                }
                m_arpPtArrToolTip[idxFrame] = nullptr;
            }
        }

        delete [] m_arpPtArrToolTip;
        m_arpPtArrToolTip = nullptr;

        delete m_pFrameStyle;
        m_pFrameStyle = nullptr;

        if( i_toolTipStyleOther.m_pFrameStyle != nullptr )
        {
            m_pFrameStyle = i_toolTipStyleOther.m_pFrameStyle->clone();

            if( m_pFrameStyle->getFrameWidth() > 0 )
            {
                #if QT_VERSION >= 0x040100
                m_arpPtArrToolTip = new QPolygon*[m_pFrameStyle->getFrameWidth()];
                #else
                m_arpPtArrToolTip = new QPointArray*[m_pFrameStyle->getFrameWidth()];
                #endif

                for( idxFrame = 0; idxFrame < m_pFrameStyle->getFrameWidth(); idxFrame++ )
                {
                    #if QT_VERSION >= 0x040100
                    m_arpPtArrToolTip[idxFrame] = new QPolygon(m_ptarrToolTip.size()+1);
                    #else
                    m_arpPtArrToolTip[idxFrame] = new QPointArray(m_ptarrToolTip.size()+1);
                    #endif
                }
            }
        }
    }

    return *this;

} // operator =

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CToolTipStyle::setToolTip( const QString& i_strToolTip )
//------------------------------------------------------------------------------
{
    m_strToolTip = i_strToolTip;
}

//------------------------------------------------------------------------------
QString CToolTipStyle::getToolTip() const
//------------------------------------------------------------------------------
{
    return m_strToolTip;
}

//------------------------------------------------------------------------------
void CToolTipStyle::setArrowHeadPos( const QPoint& i_ptPos )
//------------------------------------------------------------------------------
{
    m_ptArrowHeadPos = i_ptPos;
}

//------------------------------------------------------------------------------
QPoint CToolTipStyle::getArrowHeadPos() const
//------------------------------------------------------------------------------
{
    return m_ptArrowHeadPos;
}

//------------------------------------------------------------------------------
void CToolTipStyle::setToolTipPos( EToolTipPos i_toolTipPos )
//------------------------------------------------------------------------------
{
    m_toolTipPos = i_toolTipPos;
}

//------------------------------------------------------------------------------
CToolTipStyle::EToolTipPos CToolTipStyle::getToolTipPos() const
//------------------------------------------------------------------------------
{
    return m_toolTipPos;
}

//------------------------------------------------------------------------------
void CToolTipStyle::setColFg( const QColor& i_col )
//------------------------------------------------------------------------------
{
    m_colFg = i_col;
}

//------------------------------------------------------------------------------
QColor CToolTipStyle::getColFg() const
//------------------------------------------------------------------------------
{
    return m_colFg;
}

//------------------------------------------------------------------------------
void CToolTipStyle::setColBg( const QColor& i_col )
//------------------------------------------------------------------------------
{
    m_colBg = i_col;
}

//------------------------------------------------------------------------------
QColor CToolTipStyle::getColBg() const
//------------------------------------------------------------------------------
{
    return m_colBg;
}

//------------------------------------------------------------------------------
void CToolTipStyle::setFont( const QFont& i_fnt )
//------------------------------------------------------------------------------
{
    m_fnt = i_fnt;
}

//------------------------------------------------------------------------------
QFont CToolTipStyle::getFont() const
//------------------------------------------------------------------------------
{
    return m_fnt;
}

//------------------------------------------------------------------------------
void CToolTipStyle::setOffsHor( int i_iOffs )
//------------------------------------------------------------------------------
{
    m_cxOffs = i_iOffs;
}

//------------------------------------------------------------------------------
int CToolTipStyle::getOffsHor() const
//------------------------------------------------------------------------------
{
    return m_cxOffs;
}

//------------------------------------------------------------------------------
void CToolTipStyle::setOffsVer( int i_iOffs )
//------------------------------------------------------------------------------
{
    m_cyOffs = i_iOffs;
}

//------------------------------------------------------------------------------
int CToolTipStyle::getOffsVer() const
//------------------------------------------------------------------------------
{
    return m_cyOffs;
}

//------------------------------------------------------------------------------
void CToolTipStyle::setFrameStyle( SFrameStyle* i_pFrameStyle )
//------------------------------------------------------------------------------
{
    delete m_pFrameStyle;
    m_pFrameStyle = i_pFrameStyle;
}

//------------------------------------------------------------------------------
SFrameStyle* CToolTipStyle::getFrameStyle()
//------------------------------------------------------------------------------
{
    return m_pFrameStyle;
}

//------------------------------------------------------------------------------
void CToolTipStyle::setMarginTop( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iMarginTop = i_iMargin;
}

//------------------------------------------------------------------------------
int CToolTipStyle::getMarginTop() const
//------------------------------------------------------------------------------
{
    return m_iMarginTop;
}

//------------------------------------------------------------------------------
void CToolTipStyle::setMarginBottom( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iMarginBottom = i_iMargin;
}

//------------------------------------------------------------------------------
int CToolTipStyle::getMarginBottom() const
//------------------------------------------------------------------------------
{
    return m_iMarginBottom;
}

//------------------------------------------------------------------------------
void CToolTipStyle::setMarginLeft( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iMarginLeft = i_iMargin;
}

//------------------------------------------------------------------------------
int CToolTipStyle::getMarginLeft() const
//------------------------------------------------------------------------------
{
    return m_iMarginLeft;
}

//------------------------------------------------------------------------------
void CToolTipStyle::setMarginRight( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iMarginRight = i_iMargin;
}

//------------------------------------------------------------------------------
int CToolTipStyle::getMarginRight() const
//------------------------------------------------------------------------------
{
    return m_iMarginRight;
}

//------------------------------------------------------------------------------
void CToolTipStyle::setArrowWidth( int i_cxWidth )
//------------------------------------------------------------------------------
{
    m_cxArrowWidth = i_cxWidth;
}

//------------------------------------------------------------------------------
int CToolTipStyle::getArrowWidth() const
//------------------------------------------------------------------------------
{
    return m_cxArrowWidth;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
QRect CToolTipStyle::boundingRect() const
//------------------------------------------------------------------------------
{
    // Unter Linux funktioniert die Berechnung der Text-Breite nicht korrekt.
    int cxInnerBorderCorrValForLinux = m_strToolTip.length();
    int cyInnerBorderCorrValForLinux = 4;

    QFontMetrics fntmtr(m_fnt);

    QRect rectToolTip = fntmtr.boundingRect(m_strToolTip);

    rectToolTip.setWidth(
        rectToolTip.width()
      + m_iMarginLeft
      + m_iMarginRight
      + cxInnerBorderCorrValForLinux
      + m_cxOffs );
    rectToolTip.setHeight(
        rectToolTip.height()
      + m_iMarginTop
      + m_iMarginBottom
      + cyInnerBorderCorrValForLinux
      + m_cyOffs );

    if( m_pFrameStyle != nullptr )
    {
        rectToolTip.setWidth( rectToolTip.width() + 2*m_pFrameStyle->getFrameWidth() );
        rectToolTip.setHeight( rectToolTip.height() + 2*m_pFrameStyle->getFrameWidth() );
    }
    return rectToolTip;

} // boundingRect

//------------------------------------------------------------------------------
void CToolTipStyle::setRectToolTip( const QRect& i_rect )
//------------------------------------------------------------------------------
{
    m_rectToolTip = i_rect;
}

//------------------------------------------------------------------------------
QRect CToolTipStyle::getRectToolTip() const
//------------------------------------------------------------------------------
{
    return m_rectToolTip;
}

//------------------------------------------------------------------------------
void CToolTipStyle::setRectArrow( const QRect& i_rect )
//------------------------------------------------------------------------------
{
    m_rectToolTipArrow = i_rect;
}

//------------------------------------------------------------------------------
QRect CToolTipStyle::getRectArrow() const
//------------------------------------------------------------------------------
{
    return m_rectToolTipArrow;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CToolTipStyle::updateFrameBorderPoints()
//------------------------------------------------------------------------------
{
    // Unter Linux funktioniert die Berechnung der Text-Breite nicht korrekt.
    int cxInnerBorderCorrValForLinux = m_strToolTip.length();
    int cyInnerBorderCorrValForLinux = 4;

    QFontMetrics fntmtr(m_fnt);

    m_rectToolTip = fntmtr.boundingRect(m_strToolTip);

    m_rectToolTip.setWidth(
        m_rectToolTip.width()
      + m_iMarginLeft
      + m_iMarginRight
      + cxInnerBorderCorrValForLinux );
    m_rectToolTip.setHeight(
        m_rectToolTip.height()
      + m_iMarginTop
      + m_iMarginBottom
      + cyInnerBorderCorrValForLinux );

    if( m_pFrameStyle != nullptr )
    {
        m_rectToolTip.setWidth( m_rectToolTip.width() + 2*m_pFrameStyle->getFrameWidth() );
        m_rectToolTip.setHeight( m_rectToolTip.height() + 2*m_pFrameStyle->getFrameWidth() );
    }

    m_rectToolTipArrow.setWidth( m_cxOffs + m_cxArrowWidth );
    m_rectToolTipArrow.setHeight( m_cyOffs );

    if( m_pFrameStyle != nullptr )
    {
        m_rectToolTipArrow.setWidth( m_rectToolTipArrow.width() + 2*m_pFrameStyle->getFrameWidth() );
        m_rectToolTipArrow.setHeight( m_rectToolTipArrow.height() + 2*m_pFrameStyle->getFrameWidth() );
    }

    // The point array defining the tool tip frame (including the array) starts
    // at the cursor position increasing clockwise.
    m_ptarrToolTip.setPoint(0,m_ptArrowHeadPos.x(),m_ptArrowHeadPos.y());

    // If there is enough space for the tool tip right of the cursor position ...
    switch( m_toolTipPos )
    {
        case EToolTipPosRightTop:
        {
            m_rectToolTip.moveLeft(m_ptArrowHeadPos.x()+m_cxOffs);
            m_rectToolTip.moveBottom(m_ptArrowHeadPos.y()-m_cyOffs);
            m_rectToolTipArrow.moveLeft(m_ptArrowHeadPos.x());
            m_rectToolTipArrow.moveBottom(m_ptArrowHeadPos.y());

            // Clockwise ..
            m_iPtArrToolTipLength = 6;
            m_ptarrToolTip.setPoint(1,m_rectToolTip.left(),m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(2,m_rectToolTip.left(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(3,m_rectToolTip.right(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(4,m_rectToolTip.right(),m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(5,m_rectToolTipArrow.right(),m_rectToolTip.bottom());
            break;

        } // EToolTipPosRightTop

        case EToolTipPosRightBottom:
        {
            m_rectToolTip.moveLeft(m_ptArrowHeadPos.x()+m_cxOffs);
            m_rectToolTip.moveTop(m_ptArrowHeadPos.y()+m_cyOffs);
            m_rectToolTipArrow.moveLeft(m_ptArrowHeadPos.x());
            m_rectToolTipArrow.moveTop(m_ptArrowHeadPos.y());

            // Clockwise ..
            m_iPtArrToolTipLength = 6;
            m_ptarrToolTip.setPoint(1,m_rectToolTipArrow.right(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(2,m_rectToolTip.right(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(3,m_rectToolTip.right(),m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(4,m_rectToolTip.left(),m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(5,m_rectToolTip.left(),m_rectToolTip.top());
            break;

        } // EToolTipPosRightBottom

        case EToolTipPosRightCenter:
        {
            m_rectToolTip.moveLeft(m_ptArrowHeadPos.x()+m_cxOffs);
            m_rectToolTip.moveTop(m_ptArrowHeadPos.y()-m_rectToolTip.height()/2);
            m_rectToolTipArrow.moveLeft(m_ptArrowHeadPos.x());
            m_rectToolTipArrow.moveTop(m_ptArrowHeadPos.y()+m_cyOffs/2);

            // Clockwise ..
            m_iPtArrToolTipLength = 7;
            m_ptarrToolTip.setPoint(1,m_rectToolTip.left(),m_ptArrowHeadPos.y()-m_cxArrowWidth/2);
            m_ptarrToolTip.setPoint(2,m_rectToolTip.left(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(3,m_rectToolTip.right(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(4,m_rectToolTip.right(),m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(5,m_rectToolTip.left(),m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(6,m_rectToolTip.left(),m_ptArrowHeadPos.y()+m_cxArrowWidth/2);
            break;

        } // EToolTipPosRightCenter

        case EToolTipPosLeftTop:
        {
            m_rectToolTip.moveRight(m_ptArrowHeadPos.x()-m_cxOffs);
            m_rectToolTip.moveBottom(m_ptArrowHeadPos.y()-m_cyOffs);
            m_rectToolTipArrow.moveRight(m_ptArrowHeadPos.x());
            m_rectToolTipArrow.moveBottom(m_ptArrowHeadPos.y());

            // Clockwise ..
            m_iPtArrToolTipLength = 6;
            m_ptarrToolTip.setPoint(1,m_rectToolTipArrow.left(),m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(2,m_rectToolTip.left(),m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(3,m_rectToolTip.left(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(4,m_rectToolTip.right(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(5,m_rectToolTip.right(),m_rectToolTip.bottom());
            break;

        } // EToolTipPosLeftTop

        case EToolTipPosLeftBottom:
        {
            m_rectToolTip.moveRight(m_ptArrowHeadPos.x()-m_cxOffs);
            m_rectToolTip.moveTop(m_ptArrowHeadPos.y()+m_cyOffs);
            m_rectToolTipArrow.moveRight(m_ptArrowHeadPos.x());
            m_rectToolTipArrow.moveTop(m_ptArrowHeadPos.y());

            // Clockwise ..
            m_iPtArrToolTipLength = 6;
            m_ptarrToolTip.setPoint(1,m_rectToolTip.right(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(2,m_rectToolTip.right(),m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(3,m_rectToolTip.left(),m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(4,m_rectToolTip.left(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(5,m_rectToolTipArrow.left(),m_rectToolTip.top());
            break;

        } // EToolTipPosLeftBottom

        case EToolTipPosLeftCenter:
        {
            m_rectToolTip.moveRight(m_ptArrowHeadPos.x()-m_cxOffs);
            m_rectToolTip.moveTop(m_ptArrowHeadPos.y()-m_rectToolTip.height()/2);
            m_rectToolTipArrow.moveRight(m_ptArrowHeadPos.x());
            m_rectToolTipArrow.moveTop(m_ptArrowHeadPos.y()+m_cyOffs/2);

            // Clockwise ..
            m_iPtArrToolTipLength = 7;
            m_ptarrToolTip.setPoint(1,m_rectToolTip.right(),m_ptArrowHeadPos.y()-m_cxArrowWidth/2);
            m_ptarrToolTip.setPoint(2,m_rectToolTip.right(),m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(3,m_rectToolTip.left(),m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(4,m_rectToolTip.left(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(5,m_rectToolTip.right(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(6,m_rectToolTipArrow.right(),m_ptArrowHeadPos.y()+m_cxArrowWidth/2);
            break;

        } // EToolTipPosLeftCenter

        case EToolTipPosTopCenter:
        {
            m_rectToolTip.moveLeft(m_ptArrowHeadPos.x()-m_rectToolTip.width()/2);
            m_rectToolTip.moveBottom(m_ptArrowHeadPos.y()-m_cyOffs);
            m_rectToolTipArrow.moveLeft(m_ptArrowHeadPos.x()-m_cxOffs/2);
            m_rectToolTipArrow.moveBottom(m_ptArrowHeadPos.y());

            // Clockwise ..
            m_iPtArrToolTipLength = 7;
            m_ptarrToolTip.setPoint(1,m_ptArrowHeadPos.x()-m_cxArrowWidth/2,m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(2,m_rectToolTip.left(),m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(3,m_rectToolTip.left(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(4,m_rectToolTip.right(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(5,m_rectToolTip.right(),m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(6,m_ptArrowHeadPos.x()+m_cxArrowWidth/2,m_rectToolTip.bottom());
            break;

        } // EToolTipPosTopCenter

        case EToolTipPosBottomCenter:
        {
            m_rectToolTip.moveLeft(m_ptArrowHeadPos.x()-m_rectToolTip.width()/2);
            m_rectToolTip.moveTop(m_ptArrowHeadPos.y()+m_cyOffs);
            m_rectToolTipArrow.moveLeft(m_ptArrowHeadPos.x()-m_cxOffs/2);
            m_rectToolTipArrow.moveTop(m_ptArrowHeadPos.y());

            // Clockwise ..
            m_iPtArrToolTipLength = 7;
            m_ptarrToolTip.setPoint(1,m_ptArrowHeadPos.x()+m_cxArrowWidth/2,m_rectToolTip.top());
            m_ptarrToolTip.setPoint(2,m_rectToolTip.right(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(3,m_rectToolTip.right(),m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(4,m_rectToolTip.left(),m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(5,m_rectToolTip.left(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(6,m_ptArrowHeadPos.x()-m_cxArrowWidth/2,m_rectToolTip.top());
            break;

        } // EToolTipPosBottomCenter

        case EToolTipPosCenterCenter:
        default:
        {
            m_rectToolTip.moveLeft(m_ptArrowHeadPos.x()-m_rectToolTip.width()/2);
            m_rectToolTip.moveTop(m_ptArrowHeadPos.y()-m_rectToolTip.height()/2);
            m_rectToolTipArrow.moveLeft(m_ptArrowHeadPos.x()-m_cxOffs/2);
            m_rectToolTipArrow.moveTop(m_ptArrowHeadPos.y()-m_cyOffs/2);
            m_rectToolTipArrow.setWidth(0);
            m_rectToolTipArrow.setHeight(0);

            // Clockwise ..
            m_iPtArrToolTipLength = 4;
            m_ptarrToolTip.setPoint(0,m_rectToolTip.left(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(1,m_rectToolTip.right(),m_rectToolTip.top());
            m_ptarrToolTip.setPoint(2,m_rectToolTip.right(),m_rectToolTip.bottom());
            m_ptarrToolTip.setPoint(3,m_rectToolTip.left(),m_rectToolTip.bottom());
            break;

        } // EToolTipPosCenterCenter

    } // switch( m_toolTipPos )

    if( m_pFrameStyle != nullptr && m_arpPtArrToolTip != nullptr )
    {
        int idxFrame;
        int idxPt;

        for( idxFrame = 0; idxFrame < m_pFrameStyle->getFrameWidth(); idxFrame++ )
        {
            if( idxFrame == 0 )
            {
                for( idxPt = 0; idxPt < m_iPtArrToolTipLength; idxPt++ )
                {
                    #if QT_VERSION >= 0x040100
                    m_arpPtArrToolTip[idxFrame]->setPoint( idxPt, m_ptarrToolTip.point(idxPt) );
                    #else
                    m_arpPtArrToolTip[idxFrame]->at(idxPt) = m_ptarrToolTip.at(idxPt);
                    #endif
                }
                #if QT_VERSION >= 0x040100
                m_arpPtArrToolTip[idxFrame]->setPoint( m_iPtArrToolTipLength, m_ptarrToolTip.point(0) );
                #else
                m_arpPtArrToolTip[idxFrame]->at(m_iPtArrToolTipLength) = m_ptarrToolTip.at(0);
                #endif
            }
            else
            {
                switch( m_toolTipPos )
                {
                    case EToolTipPosRightTop:
                    {
                        #if QT_VERSION >= 0x040100
                            if( idxFrame < 2 )
                            {
                                m_arpPtArrToolTip[idxFrame]->setPoint( 0, m_ptarrToolTip.point(0).x()+idxFrame, m_ptarrToolTip.point(0).y()-idxFrame );
                            }
                            else
                            {
                                m_arpPtArrToolTip[idxFrame]->setPoint( 0, m_arpPtArrToolTip[1]->point(0) );
                            }
                            m_arpPtArrToolTip[idxFrame]->setPoint( 1, m_ptarrToolTip.point(1).x()+idxFrame, m_ptarrToolTip.point(1).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 2, m_ptarrToolTip.point(2).x()+idxFrame, m_ptarrToolTip.point(2).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 3, m_ptarrToolTip.point(3).x()-idxFrame, m_ptarrToolTip.point(3).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 4, m_ptarrToolTip.point(4).x()-idxFrame, m_ptarrToolTip.point(4).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 5, m_ptarrToolTip.point(5).x()-idxFrame, m_ptarrToolTip.point(5).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 6, m_arpPtArrToolTip[idxFrame]->point(0) );
                        #else
                            if( idxFrame < 2 )
                            {
                                m_arpPtArrToolTip[idxFrame]->at(0).setX(m_ptarrToolTip.at(0).x()+idxFrame);
                                m_arpPtArrToolTip[idxFrame]->at(0).setY(m_ptarrToolTip.at(0).y()-idxFrame);
                            }
                            else
                            {
                                m_arpPtArrToolTip[idxFrame]->at(0) = m_arpPtArrToolTip[1]->at(0);
                            }
                            m_arpPtArrToolTip[idxFrame]->at(1).setX(m_ptarrToolTip.at(1).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(1).setY(m_ptarrToolTip.at(1).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setX(m_ptarrToolTip.at(2).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setY(m_ptarrToolTip.at(2).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setX(m_ptarrToolTip.at(3).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setY(m_ptarrToolTip.at(3).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(4).setX(m_ptarrToolTip.at(4).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(4).setY(m_ptarrToolTip.at(4).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(5).setX(m_ptarrToolTip.at(5).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(5).setY(m_ptarrToolTip.at(5).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(6) = m_arpPtArrToolTip[idxFrame]->at(0);
                        #endif
                        break;
                    }
                    case EToolTipPosRightBottom:
                    {
                        #if QT_VERSION >= 0x040100
                            if( idxFrame < 2 )
                            {
                                m_arpPtArrToolTip[idxFrame]->setPoint( 0, m_ptarrToolTip.point(0).x()+idxFrame, m_ptarrToolTip.point(0).y()+idxFrame );
                            }
                            else
                            {
                                m_arpPtArrToolTip[idxFrame]->setPoint( 0, m_arpPtArrToolTip[1]->point(0) );
                            }
                            m_arpPtArrToolTip[idxFrame]->setPoint( 1, m_ptarrToolTip.point(1).x()-idxFrame, m_ptarrToolTip.point(1).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 2, m_ptarrToolTip.point(2).x()-idxFrame, m_ptarrToolTip.point(2).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 3, m_ptarrToolTip.point(3).x()-idxFrame, m_ptarrToolTip.point(3).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 4, m_ptarrToolTip.point(4).x()+idxFrame, m_ptarrToolTip.point(4).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 5, m_ptarrToolTip.point(5).x()+idxFrame, m_ptarrToolTip.point(5).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 6, m_arpPtArrToolTip[idxFrame]->point(0) );
                        #else
                            if( idxFrame < 2 )
                            {
                                m_arpPtArrToolTip[idxFrame]->at(0).setX(m_ptarrToolTip.at(0).x()+idxFrame);
                                m_arpPtArrToolTip[idxFrame]->at(0).setY(m_ptarrToolTip.at(0).y()+idxFrame);
                            }
                            else
                            {
                                m_arpPtArrToolTip[idxFrame]->at(0) = m_arpPtArrToolTip[1]->at(0);
                            }
                            m_arpPtArrToolTip[idxFrame]->at(1).setX(m_ptarrToolTip.at(1).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(1).setY(m_ptarrToolTip.at(1).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setX(m_ptarrToolTip.at(2).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setY(m_ptarrToolTip.at(2).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setX(m_ptarrToolTip.at(3).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setY(m_ptarrToolTip.at(3).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(4).setX(m_ptarrToolTip.at(4).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(4).setY(m_ptarrToolTip.at(4).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(5).setX(m_ptarrToolTip.at(5).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(5).setY(m_ptarrToolTip.at(5).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(6) = m_arpPtArrToolTip[idxFrame]->at(0);
                        #endif
                        break;
                    }
                    case EToolTipPosRightCenter:
                    {
                        #if QT_VERSION >= 0x040100
                            if( idxFrame < 2 )
                            {
                                m_arpPtArrToolTip[idxFrame]->setPoint( 0, m_ptarrToolTip.point(0).x()+idxFrame, m_ptarrToolTip.point(0).y()+idxFrame );
                            }
                            else
                            {
                                m_arpPtArrToolTip[idxFrame]->setPoint( 0, m_arpPtArrToolTip[1]->point(0) );
                            }
                            m_arpPtArrToolTip[idxFrame]->setPoint( 1, m_ptarrToolTip.point(1).x()+idxFrame, m_ptarrToolTip.point(1).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 2, m_ptarrToolTip.point(2).x()+idxFrame, m_ptarrToolTip.point(2).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 3, m_ptarrToolTip.point(3).x()-idxFrame, m_ptarrToolTip.point(3).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 4, m_ptarrToolTip.point(4).x()-idxFrame, m_ptarrToolTip.point(4).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 5, m_ptarrToolTip.point(5).x()+idxFrame, m_ptarrToolTip.point(5).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 6, m_ptarrToolTip.point(6).x()+idxFrame, m_ptarrToolTip.point(6).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 7, m_arpPtArrToolTip[idxFrame]->point(0) );
                        #else
                            if( idxFrame < 2 )
                            {
                                m_arpPtArrToolTip[idxFrame]->at(0).setX(m_ptarrToolTip.at(0).x()+idxFrame);
                                m_arpPtArrToolTip[idxFrame]->at(0).setY(m_ptarrToolTip.at(0).y());
                            }
                            else
                            {
                                m_arpPtArrToolTip[idxFrame]->at(0) = m_arpPtArrToolTip[1]->at(0);
                            }
                            m_arpPtArrToolTip[idxFrame]->at(1).setX(m_ptarrToolTip.at(1).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(1).setY(m_ptarrToolTip.at(1).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setX(m_ptarrToolTip.at(2).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setY(m_ptarrToolTip.at(2).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setX(m_ptarrToolTip.at(3).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setY(m_ptarrToolTip.at(3).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(4).setX(m_ptarrToolTip.at(4).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(4).setY(m_ptarrToolTip.at(4).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(5).setX(m_ptarrToolTip.at(5).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(5).setY(m_ptarrToolTip.at(5).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(6).setX(m_ptarrToolTip.at(6).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(6).setY(m_ptarrToolTip.at(6).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(7) = m_arpPtArrToolTip[idxFrame]->at(0);
                        #endif
                        break;
                    }
                    case EToolTipPosLeftTop:
                    {
                        #if QT_VERSION >= 0x040100
                            if( idxFrame < 2 )
                            {
                                m_arpPtArrToolTip[idxFrame]->setPoint( 0, m_ptarrToolTip.point(0).x()-idxFrame, m_ptarrToolTip.point(0).y()-idxFrame );
                            }
                            else
                            {
                                m_arpPtArrToolTip[idxFrame]->setPoint( 0, m_arpPtArrToolTip[1]->point(0) );
                            }
                            m_arpPtArrToolTip[idxFrame]->setPoint( 1, m_ptarrToolTip.point(1).x()+idxFrame, m_ptarrToolTip.point(1).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 2, m_ptarrToolTip.point(2).x()+idxFrame, m_ptarrToolTip.point(2).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 3, m_ptarrToolTip.point(3).x()+idxFrame, m_ptarrToolTip.point(3).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 4, m_ptarrToolTip.point(4).x()-idxFrame, m_ptarrToolTip.point(4).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 5, m_ptarrToolTip.point(5).x()-idxFrame, m_ptarrToolTip.point(5).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 6, m_arpPtArrToolTip[idxFrame]->point(0) );
                        #else
                            if( idxFrame < 2 )
                            {
                                m_arpPtArrToolTip[idxFrame]->at(0).setX(m_ptarrToolTip.at(0).x()-idxFrame);
                                m_arpPtArrToolTip[idxFrame]->at(0).setY(m_ptarrToolTip.at(0).y()-idxFrame);
                            }
                            else
                            {
                                m_arpPtArrToolTip[idxFrame]->at(0) = m_arpPtArrToolTip[1]->at(0);
                            }
                            m_arpPtArrToolTip[idxFrame]->at(1).setX(m_ptarrToolTip.at(1).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(1).setY(m_ptarrToolTip.at(1).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setX(m_ptarrToolTip.at(2).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setY(m_ptarrToolTip.at(2).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setX(m_ptarrToolTip.at(3).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setY(m_ptarrToolTip.at(3).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(4).setX(m_ptarrToolTip.at(4).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(4).setY(m_ptarrToolTip.at(4).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(5).setX(m_ptarrToolTip.at(5).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(5).setY(m_ptarrToolTip.at(5).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(6) = m_arpPtArrToolTip[idxFrame]->at(0);
                        #endif
                        break;
                    }
                    case EToolTipPosLeftBottom:
                    {
                        #if QT_VERSION >= 0x040100
                            if( idxFrame < 2 )
                            {
                                m_arpPtArrToolTip[idxFrame]->setPoint( 0, m_ptarrToolTip.point(0).x()-idxFrame, m_ptarrToolTip.point(0).y()+idxFrame );
                            }
                            else
                            {
                                m_arpPtArrToolTip[idxFrame]->setPoint( 0, m_arpPtArrToolTip[1]->point(0) );
                            }
                            m_arpPtArrToolTip[idxFrame]->setPoint( 1, m_ptarrToolTip.point(1).x()-idxFrame, m_ptarrToolTip.point(1).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 2, m_ptarrToolTip.point(2).x()-idxFrame, m_ptarrToolTip.point(2).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 3, m_ptarrToolTip.point(3).x()+idxFrame, m_ptarrToolTip.point(3).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 4, m_ptarrToolTip.point(4).x()+idxFrame, m_ptarrToolTip.point(4).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 5, m_ptarrToolTip.point(5).x()+idxFrame, m_ptarrToolTip.point(5).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 6, m_arpPtArrToolTip[idxFrame]->point(0) );
                        #else
                            if( idxFrame < 2 )
                            {
                                m_arpPtArrToolTip[idxFrame]->at(0).setX(m_ptarrToolTip.at(0).x()-idxFrame);
                                m_arpPtArrToolTip[idxFrame]->at(0).setY(m_ptarrToolTip.at(0).y()+idxFrame);
                            }
                            else
                            {
                                m_arpPtArrToolTip[idxFrame]->at(0) = m_arpPtArrToolTip[1]->at(0);
                            }
                            m_arpPtArrToolTip[idxFrame]->at(1).setX(m_ptarrToolTip.at(1).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(1).setY(m_ptarrToolTip.at(1).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setX(m_ptarrToolTip.at(2).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setY(m_ptarrToolTip.at(2).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setX(m_ptarrToolTip.at(3).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setY(m_ptarrToolTip.at(3).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(4).setX(m_ptarrToolTip.at(4).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(4).setY(m_ptarrToolTip.at(4).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(5).setX(m_ptarrToolTip.at(5).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(5).setY(m_ptarrToolTip.at(5).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(6) = m_arpPtArrToolTip[idxFrame]->at(0);
                        #endif
                        break;
                    }
                    case EToolTipPosLeftCenter:
                    {
                        #if QT_VERSION >= 0x040100
                            if( idxFrame < 2 )
                            {
                                m_arpPtArrToolTip[idxFrame]->setPoint( 0, m_ptarrToolTip.point(0).x()-idxFrame, m_ptarrToolTip.point(0).y() );
                            }
                            else
                            {
                                m_arpPtArrToolTip[idxFrame]->setPoint( 0, m_arpPtArrToolTip[1]->point(0) );
                            }
                            m_arpPtArrToolTip[idxFrame]->setPoint( 1, m_ptarrToolTip.point(1).x()-idxFrame, m_ptarrToolTip.point(1).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 2, m_ptarrToolTip.point(2).x()-idxFrame, m_ptarrToolTip.point(2).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 3, m_ptarrToolTip.point(3).x()+idxFrame, m_ptarrToolTip.point(3).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 4, m_ptarrToolTip.point(4).x()+idxFrame, m_ptarrToolTip.point(4).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 5, m_ptarrToolTip.point(5).x()-idxFrame, m_ptarrToolTip.point(5).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 6, m_ptarrToolTip.point(6).x()-idxFrame, m_ptarrToolTip.point(6).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 7, m_arpPtArrToolTip[idxFrame]->point(0) );
                        #else
                            if( idxFrame < 2 )
                            {
                                m_arpPtArrToolTip[idxFrame]->at(0).setX(m_ptarrToolTip.at(0).x()-idxFrame);
                                m_arpPtArrToolTip[idxFrame]->at(0).setY(m_ptarrToolTip.at(0).y());
                            }
                            else
                            {
                                m_arpPtArrToolTip[idxFrame]->at(0) = m_arpPtArrToolTip[1]->at(0);
                            }
                            m_arpPtArrToolTip[idxFrame]->at(1).setX(m_ptarrToolTip.at(1).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(1).setY(m_ptarrToolTip.at(1).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setX(m_ptarrToolTip.at(2).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setY(m_ptarrToolTip.at(2).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setX(m_ptarrToolTip.at(3).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setY(m_ptarrToolTip.at(3).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(4).setX(m_ptarrToolTip.at(4).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(4).setY(m_ptarrToolTip.at(4).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(5).setX(m_ptarrToolTip.at(5).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(5).setY(m_ptarrToolTip.at(5).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(6).setX(m_ptarrToolTip.at(6).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(6).setY(m_ptarrToolTip.at(6).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(7) = m_arpPtArrToolTip[idxFrame]->at(0);
                        #endif
                        break;
                    }
                    case EToolTipPosTopCenter:
                    {
                        #if QT_VERSION >= 0x040100
                            if( idxFrame < 2 )
                            {
                                m_arpPtArrToolTip[idxFrame]->setPoint( 0, m_ptarrToolTip.point(0).x(), m_ptarrToolTip.point(0).y()-idxFrame );
                            }
                            else
                            {
                                m_arpPtArrToolTip[idxFrame]->setPoint( 0, m_arpPtArrToolTip[1]->point(0) );
                            }
                            m_arpPtArrToolTip[idxFrame]->setPoint( 1, m_ptarrToolTip.point(1).x()+idxFrame, m_ptarrToolTip.point(1).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 2, m_ptarrToolTip.point(2).x()+idxFrame, m_ptarrToolTip.point(2).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 3, m_ptarrToolTip.point(3).x()+idxFrame, m_ptarrToolTip.point(3).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 4, m_ptarrToolTip.point(4).x()-idxFrame, m_ptarrToolTip.point(4).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 5, m_ptarrToolTip.point(5).x()-idxFrame, m_ptarrToolTip.point(5).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 6, m_ptarrToolTip.point(6).x()-idxFrame, m_ptarrToolTip.point(6).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 7, m_arpPtArrToolTip[idxFrame]->point(0) );
                        #else
                            if( idxFrame < 2 )
                            {
                                m_arpPtArrToolTip[idxFrame]->at(0).setX(m_ptarrToolTip.at(0).x());
                                m_arpPtArrToolTip[idxFrame]->at(0).setY(m_ptarrToolTip.at(0).y()-idxFrame);
                            }
                            else
                            {
                                m_arpPtArrToolTip[idxFrame]->at(0) = m_arpPtArrToolTip[1]->at(0);
                            }
                            m_arpPtArrToolTip[idxFrame]->at(1).setX(m_ptarrToolTip.at(1).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(1).setY(m_ptarrToolTip.at(1).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setX(m_ptarrToolTip.at(2).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setY(m_ptarrToolTip.at(2).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setX(m_ptarrToolTip.at(3).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setY(m_ptarrToolTip.at(3).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(4).setX(m_ptarrToolTip.at(4).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(4).setY(m_ptarrToolTip.at(4).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(5).setX(m_ptarrToolTip.at(5).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(5).setY(m_ptarrToolTip.at(5).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(6).setX(m_ptarrToolTip.at(6).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(6).setY(m_ptarrToolTip.at(6).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(7) = m_arpPtArrToolTip[idxFrame]->at(0);
                        #endif
                        break;
                    }
                    case EToolTipPosBottomCenter:
                    {
                        #if QT_VERSION >= 0x040100
                            if( idxFrame < 2 )
                            {
                                m_arpPtArrToolTip[idxFrame]->setPoint( 0, m_ptarrToolTip.point(0).x(), m_ptarrToolTip.point(0).y()+idxFrame );
                            }
                            else
                            {
                                m_arpPtArrToolTip[idxFrame]->setPoint( 0, m_arpPtArrToolTip[1]->point(0) );
                            }
                            m_arpPtArrToolTip[idxFrame]->setPoint( 1, m_ptarrToolTip.point(1).x()-idxFrame, m_ptarrToolTip.point(1).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 2, m_ptarrToolTip.point(2).x()-idxFrame, m_ptarrToolTip.point(2).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 3, m_ptarrToolTip.point(3).x()-idxFrame, m_ptarrToolTip.point(3).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 4, m_ptarrToolTip.point(4).x()+idxFrame, m_ptarrToolTip.point(4).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 5, m_ptarrToolTip.point(5).x()+idxFrame, m_ptarrToolTip.point(5).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 6, m_ptarrToolTip.point(6).x()+idxFrame, m_ptarrToolTip.point(6).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 7, m_arpPtArrToolTip[idxFrame]->point(0) );
                        #else
                            if( idxFrame < 2 )
                            {
                                m_arpPtArrToolTip[idxFrame]->at(0).setX(m_ptarrToolTip.at(0).x());
                                m_arpPtArrToolTip[idxFrame]->at(0).setY(m_ptarrToolTip.at(0).y()+idxFrame);
                            }
                            else
                            {
                                m_arpPtArrToolTip[idxFrame]->at(0) = m_arpPtArrToolTip[1]->at(0);
                            }
                            m_arpPtArrToolTip[idxFrame]->at(1).setX(m_ptarrToolTip.at(1).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(1).setY(m_ptarrToolTip.at(1).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setX(m_ptarrToolTip.at(2).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setY(m_ptarrToolTip.at(2).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setX(m_ptarrToolTip.at(3).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setY(m_ptarrToolTip.at(3).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(4).setX(m_ptarrToolTip.at(4).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(4).setY(m_ptarrToolTip.at(4).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(5).setX(m_ptarrToolTip.at(5).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(5).setY(m_ptarrToolTip.at(5).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(6).setX(m_ptarrToolTip.at(6).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(6).setY(m_ptarrToolTip.at(6).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(7) = m_arpPtArrToolTip[idxFrame]->at(0);
                        #endif
                        break;
                    }
                    case EToolTipPosCenterCenter:
                    {
                        #if QT_VERSION >= 0x040100
                            m_arpPtArrToolTip[idxFrame]->setPoint( 0, m_ptarrToolTip.point(0).x()+idxFrame, m_ptarrToolTip.point(0).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 1, m_ptarrToolTip.point(1).x()-idxFrame, m_ptarrToolTip.point(1).y()+idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 2, m_ptarrToolTip.point(2).x()-idxFrame, m_ptarrToolTip.point(2).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 3, m_ptarrToolTip.point(3).x()+idxFrame, m_ptarrToolTip.point(3).y()-idxFrame );
                            m_arpPtArrToolTip[idxFrame]->setPoint( 4, m_arpPtArrToolTip[idxFrame]->point(0) );
                        #else
                            m_arpPtArrToolTip[idxFrame]->at(0).setX(m_ptarrToolTip.at(0).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(0).setY(m_ptarrToolTip.at(0).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(1).setX(m_ptarrToolTip.at(1).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(1).setY(m_ptarrToolTip.at(1).y()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setX(m_ptarrToolTip.at(2).x()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(2).setY(m_ptarrToolTip.at(2).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setX(m_ptarrToolTip.at(3).x()+idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(3).setY(m_ptarrToolTip.at(3).y()-idxFrame);
                            m_arpPtArrToolTip[idxFrame]->at(4) = m_arpPtArrToolTip[idxFrame]->at(0);
                        #endif
                        break;
                    }
                    default:
                    {
                        break;
                    }
                } // switch( toolTipPos )
            } // if( idxFrame > 0 )
        } // for( idxFrame < m_pFrameStyle->m_iFrameWidth )
    } // if( m_pFrameStyle != nullptr )

} // updateFrameBorderPoints

//------------------------------------------------------------------------------
void CToolTipStyle::draw( QPainter* i_pPainter )
//------------------------------------------------------------------------------
{
    i_pPainter->save();

    QBrush               brush;
    QPen                 pen;
    SFrameStyle3DSunken* pFrameStyle3DSunken = nullptr;
    SFrameStyle3DRaised* pFrameStyle3DRaised = nullptr;

    // Draw tool tip background and tool tip arrow background ...
    brush.setColor(m_colBg);
    brush.setStyle(Qt::SolidPattern);
    i_pPainter->setPen(Qt::NoPen);
    i_pPainter->setBrush(brush);
    #if QT_VERSION >= 0x040100
    i_pPainter->drawPolygon(m_ptarrToolTip.data(),m_iPtArrToolTipLength);
    #else
    i_pPainter->drawPolygon(m_ptarrToolTip,false,0,m_iPtArrToolTipLength);
    #endif

    pFrameStyle3DRaised = dynamic_cast<SFrameStyle3DRaised*>(m_pFrameStyle);
    if( pFrameStyle3DRaised == nullptr )
    {
        pFrameStyle3DSunken = dynamic_cast<SFrameStyle3DSunken*>(m_pFrameStyle);
    }

    // Draw tool tip frame (if any) ...
    if( m_pFrameStyle != nullptr && m_arpPtArrToolTip != nullptr )
    {
        int idxFrame;

        i_pPainter->setBrush(Qt::NoBrush);

        if( pFrameStyle3DSunken == nullptr && pFrameStyle3DRaised == nullptr )
        {
            pen.setColor(m_pFrameStyle->getCol());
            pen.setStyle(m_pFrameStyle->getPenStyle());
            i_pPainter->setPen(pen);

            for( idxFrame = 0; idxFrame < m_pFrameStyle->getFrameWidth(); idxFrame++ )
            {
                if( m_arpPtArrToolTip[idxFrame] != nullptr )
                {
                    #if QT_VERSION >= 0x040100
                    i_pPainter->drawPolyline(*m_arpPtArrToolTip[idxFrame]);
                    #else
                    i_pPainter->drawPolyline(*m_arpPtArrToolTip[idxFrame],0,m_iPtArrToolTipLength+1);
                    #endif
                }
            }
        }
        else // 3DFrame
        {
            QPen        penDark(Qt::darkGray);
            QPen        penLight(Qt::lightGray);
            #if QT_VERSION >= 0x040100
            QPolygon    ptarr(m_iPtArrToolTipLength+1);
            #else
            QPointArray ptarr(m_iPtArrToolTipLength+1);
            #endif

            if( pFrameStyle3DSunken != nullptr )
            {
                penDark.setColor(pFrameStyle3DSunken->getColDark());
                penLight.setColor(pFrameStyle3DSunken->getColLight());
            }
            else // if( pFrameStyle3DRaised != nullptr )
            {
                penDark.setColor(pFrameStyle3DRaised->getColLight());
                penLight.setColor(pFrameStyle3DRaised->getColDark());
            }
            for( idxFrame = 0; idxFrame < m_pFrameStyle->getFrameWidth(); idxFrame++ )
            {
                if( m_arpPtArrToolTip[idxFrame] != nullptr )
                {
                    switch( m_toolTipPos )
                    {
                        case EToolTipPosRightTop:
                        {
                            i_pPainter->setPen(penDark);
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(0),m_arpPtArrToolTip[idxFrame]->at(1));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(1),m_arpPtArrToolTip[idxFrame]->at(2));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(2),m_arpPtArrToolTip[idxFrame]->at(3));
                            i_pPainter->setPen(penLight);
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(3),m_arpPtArrToolTip[idxFrame]->at(4));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(4),m_arpPtArrToolTip[idxFrame]->at(5));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(5),m_arpPtArrToolTip[idxFrame]->at(6));
                            break;
                        }
                        case EToolTipPosRightBottom:
                        {
                            i_pPainter->setPen(penDark);
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(1),m_arpPtArrToolTip[idxFrame]->at(2));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(4),m_arpPtArrToolTip[idxFrame]->at(5));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(5),m_arpPtArrToolTip[idxFrame]->at(6));
                            i_pPainter->setPen(penLight);
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(0),m_arpPtArrToolTip[idxFrame]->at(1));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(2),m_arpPtArrToolTip[idxFrame]->at(3));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(3),m_arpPtArrToolTip[idxFrame]->at(4));
                            break;
                        }
                        case EToolTipPosRightCenter:
                        {
                            i_pPainter->setPen(penDark);
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(0),m_arpPtArrToolTip[idxFrame]->at(1));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(1),m_arpPtArrToolTip[idxFrame]->at(2));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(2),m_arpPtArrToolTip[idxFrame]->at(3));
                            i_pPainter->setPen(penLight);
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(3),m_arpPtArrToolTip[idxFrame]->at(4));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(4),m_arpPtArrToolTip[idxFrame]->at(5));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(5),m_arpPtArrToolTip[idxFrame]->at(6));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(6),m_arpPtArrToolTip[idxFrame]->at(7));
                            break;
                        }
                        case EToolTipPosLeftTop:
                        {
                            i_pPainter->setPen(penDark);
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(0),m_arpPtArrToolTip[idxFrame]->at(1));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(2),m_arpPtArrToolTip[idxFrame]->at(3));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(3),m_arpPtArrToolTip[idxFrame]->at(4));
                            i_pPainter->setPen(penLight);
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(1),m_arpPtArrToolTip[idxFrame]->at(2));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(4),m_arpPtArrToolTip[idxFrame]->at(5));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(5),m_arpPtArrToolTip[idxFrame]->at(6));
                            break;
                        }
                        case EToolTipPosLeftBottom:
                        {
                            i_pPainter->setPen(penDark);
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(3),m_arpPtArrToolTip[idxFrame]->at(4));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(4),m_arpPtArrToolTip[idxFrame]->at(5));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(5),m_arpPtArrToolTip[idxFrame]->at(6));
                            i_pPainter->setPen(penLight);
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(0),m_arpPtArrToolTip[idxFrame]->at(1));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(1),m_arpPtArrToolTip[idxFrame]->at(2));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(2),m_arpPtArrToolTip[idxFrame]->at(3));
                            break;
                        }
                        case EToolTipPosLeftCenter:
                        {
                            i_pPainter->setPen(penDark);
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(3),m_arpPtArrToolTip[idxFrame]->at(4));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(4),m_arpPtArrToolTip[idxFrame]->at(5));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(6),m_arpPtArrToolTip[idxFrame]->at(7));
                            i_pPainter->setPen(penLight);
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(0),m_arpPtArrToolTip[idxFrame]->at(1));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(1),m_arpPtArrToolTip[idxFrame]->at(2));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(2),m_arpPtArrToolTip[idxFrame]->at(3));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(5),m_arpPtArrToolTip[idxFrame]->at(6));
                            break;
                        }
                        case EToolTipPosTopCenter:
                        {
                            i_pPainter->setPen(penDark);
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(0),m_arpPtArrToolTip[idxFrame]->at(1));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(1),m_arpPtArrToolTip[idxFrame]->at(2));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(2),m_arpPtArrToolTip[idxFrame]->at(3));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(3),m_arpPtArrToolTip[idxFrame]->at(4));
                            i_pPainter->setPen(penLight);
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(4),m_arpPtArrToolTip[idxFrame]->at(5));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(5),m_arpPtArrToolTip[idxFrame]->at(6));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(6),m_arpPtArrToolTip[idxFrame]->at(7));
                            break;
                        }
                        case EToolTipPosBottomCenter:
                        {
                            i_pPainter->setPen(penDark);
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(1),m_arpPtArrToolTip[idxFrame]->at(2));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(4),m_arpPtArrToolTip[idxFrame]->at(5));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(5),m_arpPtArrToolTip[idxFrame]->at(6));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(6),m_arpPtArrToolTip[idxFrame]->at(7));
                            i_pPainter->setPen(penLight);
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(0),m_arpPtArrToolTip[idxFrame]->at(1));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(2),m_arpPtArrToolTip[idxFrame]->at(3));
                            i_pPainter->drawLine(m_arpPtArrToolTip[idxFrame]->at(3),m_arpPtArrToolTip[idxFrame]->at(4));
                            break;
                        }
                        case EToolTipPosCenterCenter:
                        {
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    } // switch( toolTipPos )
                } // if( m_arpPtArrToolTip[idxFrame] != nullptr )
            } // for( idxFrame < pFrameStyle->m_iFrameWidth )
        } // if( 3DFrame )
    } // if( m_pToolTipStyle->m_pFrameStyle != nullptr )

    // Show marker values ...
    pen.setColor(m_colFg);
    pen.setStyle(Qt::SolidLine);
    i_pPainter->setPen(pen);
    i_pPainter->setFont(m_fnt);
    i_pPainter->drawText(m_rectToolTip,Qt::AlignHCenter|Qt::AlignVCenter,m_strToolTip); //lint !e655

    i_pPainter->restore();

} // draw
