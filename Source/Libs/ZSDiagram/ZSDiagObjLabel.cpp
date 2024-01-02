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

#include "ZSDiagram/ZSDiagObjLabel.h"
#include "ZSDiagram/ZSDiagramProcPixmap.h"
#include "ZSDiagram/ZSDiagramFrameStyles.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qpainter.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDiagObjLabel : public CDiagObj
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObjLabel::CDiagObjLabel(
    const QString& i_strObjName,
    const QString& i_strText,
    ELayoutPos     i_layoutPos,
    int            i_iStateCount ) :
//------------------------------------------------------------------------------
    CDiagObj(
        /* strClassName */ CDiagObjLabel::ClassName(),
        /* strObjName   */ i_strObjName,
        /* pDiagTrace   */ nullptr,
        /* layoutPos    */ i_layoutPos ),
    m_iState(0),
    m_iStateCount(i_iStateCount),
    m_cxMinimumWidth(0),
    m_cxMaximumWidth(0),
    m_cyMinimumHeight(0),
    m_cyMaximumHeight(0),
    m_iLayoutPosAlignmentFlags(Qt::AlignVCenter|Qt::AlignHCenter), //lint !e655
    m_cxLayoutPosOffs(0),
    m_cyLayoutPosOffs(0),
    m_pFrameStyle(nullptr),
    m_arcolBg(nullptr),
    m_arbrushStyle(nullptr),
    m_iMarginTop(0),
    m_iMarginBottom(0),
    m_iMarginLeft(0),
    m_iMarginRight(0),
    m_arstrText(nullptr),
    m_arcolText(nullptr),
    m_fntText(),
    m_iTextAlignmentFlags(Qt::AlignVCenter|Qt::AlignHCenter), //lint !e655
    m_textOrientation(ETextOrientationLeft2Right),
    m_arpxm(nullptr),
    m_iPixmapAlignmentFlags(Qt::AlignVCenter|Qt::AlignHCenter), //lint !e655
    m_rectOuterFrame(),
    m_rectInnerFrame(),
    m_rectText(),
    m_rectPixmap(),
    m_bUpdWidget(true)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    if( m_iStateCount == 0 )
    {
        m_iStateCount = 1;
    }

    m_arcolBg = new QColor[m_iStateCount];
    m_arbrushStyle = new Qt::BrushStyle[m_iStateCount];
    m_arstrText = new QString[m_iStateCount];
    m_arcolText = new QColor[m_iStateCount];
    m_arpxm = new QPixmap[m_iStateCount];

    m_arcolBg[0] = Qt::lightGray;
    m_arbrushStyle[0] = Qt::NoBrush;
    m_arstrText[0] = i_strText;
    m_arcolText[0] = Qt::black;
    //m_arpxm[0] = ; default ctor is ok here

    for( int idxState = 1; idxState < m_iStateCount; idxState++ )
    {
        m_arcolBg[idxState] = Qt::lightGray;
        m_arbrushStyle[idxState] = Qt::NoBrush;
        m_arstrText[idxState] = "";
        m_arcolText[idxState] = Qt::black;
        //m_arpxm[idxState] = ; default ctor is ok here
    }

} // ctor

//------------------------------------------------------------------------------
CDiagObjLabel::CDiagObjLabel(
    const QString& i_strObjName,
    const QPixmap& i_pxm,
    ELayoutPos     i_layoutPos,
    int            i_iStateCount ) :
//------------------------------------------------------------------------------
    CDiagObj(
        /* strClassName */ CDiagObjLabel::ClassName(),
        /* strObjName   */ i_strObjName,
        /* pDiagTrace   */ nullptr,
        /* layoutPos    */ i_layoutPos ),
    m_iState(0),
    m_iStateCount(i_iStateCount),
    m_cxMinimumWidth(0),
    m_cxMaximumWidth(0),
    m_cyMinimumHeight(0),
    m_cyMaximumHeight(0),
    m_iLayoutPosAlignmentFlags(Qt::AlignVCenter|Qt::AlignHCenter), //lint !e655
    m_cxLayoutPosOffs(0),
    m_cyLayoutPosOffs(0),
    m_pFrameStyle(nullptr),
    m_arcolBg(nullptr),
    m_arbrushStyle(nullptr),
    m_iMarginTop(0),
    m_iMarginBottom(0),
    m_iMarginLeft(0),
    m_iMarginRight(0),
    m_arstrText(nullptr),
    m_arcolText(nullptr),
    m_fntText(),
    m_iTextAlignmentFlags(Qt::AlignVCenter|Qt::AlignHCenter), //lint !e655
    m_textOrientation(ETextOrientationLeft2Right),
    m_arpxm(nullptr),
    m_iPixmapAlignmentFlags(Qt::AlignVCenter|Qt::AlignHCenter), //lint !e655
    m_rectOuterFrame(),
    m_rectInnerFrame(),
    m_rectText(),
    m_rectPixmap(),
    m_bUpdWidget(true)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    if( m_iStateCount == 0 )
    {
        m_iStateCount = 1;
    }

    m_arcolBg = new QColor[m_iStateCount];
    m_arbrushStyle = new Qt::BrushStyle[m_iStateCount];
    m_arstrText = new QString[m_iStateCount];
    m_arcolText = new QColor[m_iStateCount];
    m_arpxm = new QPixmap[m_iStateCount];

    m_arcolBg[0] = Qt::lightGray;
    m_arbrushStyle[0] = Qt::NoBrush;
    m_arstrText[0] = "";
    m_arcolText[0] = Qt::black;
    m_arpxm[0] = i_pxm;

    for( int idxState = 1; idxState < m_iStateCount; idxState++ )
    {
        m_arcolBg[idxState] = Qt::lightGray;
        m_arbrushStyle[idxState] = Qt::NoBrush;
        m_arstrText[idxState] = "";
        m_arcolText[idxState] = Qt::black;
        //m_arpxm[idxState] = ; default ctor is ok here
    }

} // ctor

//------------------------------------------------------------------------------
CDiagObjLabel::~CDiagObjLabel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try {
        delete m_pFrameStyle;
    } catch(...) {
    }
    m_pFrameStyle = nullptr;

    try {
        delete [] m_arcolBg;
    } catch(...) {
    }
    m_arcolBg = nullptr;

    try
    {
        delete [] m_arbrushStyle;
    } catch(...) {
    }
    m_arbrushStyle = nullptr;

    try
    {
        delete [] m_arcolText;
    } catch(...) {
    }
    m_arcolText = nullptr;

    try {
        delete [] m_arstrText;
    } catch(...) {
    }
    m_arstrText = nullptr;

    try {
        delete [] m_arpxm;
    } catch(...) {
    }
    m_arpxm = nullptr;

    m_iState = 0;
    m_iStateCount = 0;
    m_cxMinimumWidth = 0;
    m_cxMaximumWidth = 0;
    m_cyMinimumHeight = 0;
    m_cyMaximumHeight = 0;
    m_iLayoutPosAlignmentFlags = 0;
    m_cxLayoutPosOffs = 0;
    m_cyLayoutPosOffs = 0;
    //m_pFrameStyle;
    //m_arcolBg;
    //m_arbrushStyle;
    m_iMarginTop = 0;
    m_iMarginBottom = 0;
    m_iMarginLeft = 0;
    m_iMarginRight = 0;
    //m_arstrText;
    //m_arcolText;
    //m_fntText;
    m_iTextAlignmentFlags = 0;
    m_textOrientation = static_cast<ETextOrientation>(0);
    //m_arpxm;
    m_iPixmapAlignmentFlags = 0;
    //m_rectOuterFrame;
    //m_rectInnerFrame;
    //m_rectText;
    //m_rectPixmap;
    m_bUpdWidget = false;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjLabel::setState( int i_iState )
//------------------------------------------------------------------------------
{
    if( i_iState < m_iStateCount )
    {
        m_iState = i_iState;
        invalidate(EUpdateLayoutDataPixmapWidget,true);
    }
}

//------------------------------------------------------------------------------
int CDiagObjLabel::getState() const
//------------------------------------------------------------------------------
{
    return m_iState;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setMinimumWidth( int i_cxWidth )
//------------------------------------------------------------------------------
{
    m_cxMinimumWidth = i_cxWidth;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjLabel::getMinimumWidth() const
//------------------------------------------------------------------------------
{
    return m_cxMinimumWidth;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setMaximumWidth( int i_cxWidth )
//------------------------------------------------------------------------------
{
    m_cxMaximumWidth = i_cxWidth;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjLabel::getMaximumWidth() const
//------------------------------------------------------------------------------
{
    return m_cxMaximumWidth;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setMinimumHeight( int i_cyHeight )
//------------------------------------------------------------------------------
{
    m_cyMinimumHeight = i_cyHeight;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjLabel::getMinimumHeight() const
//------------------------------------------------------------------------------
{
    return m_cyMinimumHeight;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setMaximumHeight( int i_cyHeight )
//------------------------------------------------------------------------------
{
    m_cyMaximumHeight = i_cyHeight;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjLabel::getMaximumHeight() const
//------------------------------------------------------------------------------
{
    return m_cyMaximumHeight;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setLayoutPosAlignmentFlags( int i_iAlignmentFlags )
//------------------------------------------------------------------------------
{
    m_iLayoutPosAlignmentFlags = i_iAlignmentFlags;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjLabel::getLayoutPosAlignmentFlags() const
//------------------------------------------------------------------------------
{
    return m_iLayoutPosAlignmentFlags;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setLayoutPosOffsX( int i_cxOffs )
//------------------------------------------------------------------------------
{
    m_cxLayoutPosOffs = i_cxOffs;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjLabel::getLayoutPosOffsX() const
//------------------------------------------------------------------------------
{
    return m_cxLayoutPosOffs;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setLayoutPosOffsY( int i_cyOffs )
//------------------------------------------------------------------------------
{
    m_cyLayoutPosOffs = i_cyOffs;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjLabel::getLayoutPosOffsY() const
//------------------------------------------------------------------------------
{
    return m_cyLayoutPosOffs;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setFrameStyle( SFrameStyle* i_pFrameStyle )
//------------------------------------------------------------------------------
{
    if( i_pFrameStyle != m_pFrameStyle )
    {
        delete m_pFrameStyle;
    }
    m_pFrameStyle = i_pFrameStyle;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
SFrameStyle* CDiagObjLabel::getFrameStyle()
//------------------------------------------------------------------------------
{
    return m_pFrameStyle;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setColBg( const QColor& i_col, int i_iState )
//------------------------------------------------------------------------------
{
    if( i_iState < m_iStateCount )
    {
        m_arcolBg[i_iState] = i_col; //lint !e661 ... manchmal ist lint wirklich blind

        if( m_iState == i_iState )
        {
            m_bUpdWidget = true;
            invalidate(EUpdatePixmapWidget,true);
        }
    }
}

//------------------------------------------------------------------------------
QColor CDiagObjLabel::getColBg( int i_iState ) const
//------------------------------------------------------------------------------
{
    QColor colBg = m_arcolBg[0];

    if( i_iState > 0 && i_iState < m_iStateCount )
    {
        colBg = m_arcolBg[i_iState]; //lint !e661 ... manchmal ist lint wirklich blind
    }
    return colBg;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setBrushStyle( Qt::BrushStyle i_brushStyle, int i_iState )
//------------------------------------------------------------------------------
{
    if( i_iState < m_iStateCount )
    {
        m_arbrushStyle[i_iState] = i_brushStyle; //lint !e661 ... manchmal ist lint wirklich blind

        if( m_iState == i_iState )
        {
            m_bUpdWidget = true;
            invalidate(EUpdatePixmapWidget,true);
        }
    }
}

//------------------------------------------------------------------------------
Qt::BrushStyle CDiagObjLabel::getBrushStyle( int i_iState ) const
//------------------------------------------------------------------------------
{
    Qt::BrushStyle brushStyle = m_arbrushStyle[0];

    if( i_iState > 0 && i_iState < m_iStateCount )
    {
        brushStyle = m_arbrushStyle[i_iState]; //lint !e661 ... manchmal ist lint wirklich blind
    }
    return brushStyle;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setMarginTop( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iMarginTop = i_iMargin;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjLabel::getMarginTop() const
//------------------------------------------------------------------------------
{
    return m_iMarginTop;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setMarginBottom( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iMarginBottom = i_iMargin;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjLabel::getMarginBottom() const
//------------------------------------------------------------------------------
{
    return m_iMarginBottom;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setMarginLeft( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iMarginLeft = i_iMargin;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjLabel::getMarginLeft() const
//------------------------------------------------------------------------------
{
    return m_iMarginLeft;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setMarginRight( int i_iMargin )
//------------------------------------------------------------------------------
{
    m_iMarginRight = i_iMargin;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjLabel::getMarginRight() const
//------------------------------------------------------------------------------
{
    return m_iMarginRight;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setText( const QString& i_strText, int i_iState )
//------------------------------------------------------------------------------
{
    if( i_iState < m_iStateCount )
    {
        m_arstrText[i_iState] = i_strText; //lint !e661 ... manchmal ist lint wirklich blind

        if( i_iState == m_state )
        {
            invalidate(EUpdateDataPixmapWidget,true);
        }
    }
}

//------------------------------------------------------------------------------
QString CDiagObjLabel::getText( int i_iState ) const
//------------------------------------------------------------------------------
{
    QString strText = m_arstrText[0];

    if( i_iState > 0 && i_iState < m_iStateCount )
    {
        strText = m_arstrText[i_iState]; //lint !e661 ... manchmal ist lint wirklich blind
    }
    return strText;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setColText( const QColor& i_col, int i_iState )
//------------------------------------------------------------------------------
{
    if( i_iState < m_iStateCount )
    {
        m_arcolText[i_iState] = i_col; //lint !e661 ... manchmal ist lint wirklich blind

        if( i_iState == m_state )
        {
            m_bUpdWidget = true;
            invalidate(EUpdateDataPixmapWidget,true);
        }
    }
}

//------------------------------------------------------------------------------
QColor CDiagObjLabel::getColText( int i_iState ) const
//------------------------------------------------------------------------------
{
    QColor colText = m_arcolText[0];

    if( i_iState > 0 && i_iState < m_iStateCount )
    {
        colText = m_arcolText[i_iState]; //lint !e661 ... manchmal ist lint wirklich blind
    }
    return colText;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setFont( const QFont& i_fnt )
//------------------------------------------------------------------------------
{
    m_fntText = i_fnt;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
QFont CDiagObjLabel::getFont() const
//------------------------------------------------------------------------------
{
    return m_fntText;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setTextAlignmentFlags( int i_iAlignmentFlags )
//------------------------------------------------------------------------------
{
    m_iTextAlignmentFlags = i_iAlignmentFlags;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjLabel::getTextAlignmentFlags() const
//------------------------------------------------------------------------------
{
    return m_iTextAlignmentFlags;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setTextOrientation( ETextOrientation i_textOrientation )
//------------------------------------------------------------------------------
{
    m_textOrientation = i_textOrientation;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
ETextOrientation CDiagObjLabel::getTextOrientation() const
//------------------------------------------------------------------------------
{
    return m_textOrientation;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setPixmap( const QPixmap& i_pxm, int i_iState )
//------------------------------------------------------------------------------
{
    if( i_iState < m_iStateCount )
    {
        m_arpxm[i_iState] = i_pxm; //lint !e661 ... manchmal ist lint wirklich blind

        if( i_iState == m_state )
        {
            invalidate(EUpdateDataPixmapWidget,true);
        }
    }
}

//------------------------------------------------------------------------------
QPixmap CDiagObjLabel::getPixmap( int i_iState ) const
//------------------------------------------------------------------------------
{
    QPixmap pxm = m_arpxm[0];

    if( i_iState > 0 && i_iState < m_iStateCount )
    {
        pxm = m_arpxm[i_iState]; //lint !e661 ... manchmal ist lint wirklich blind
    }
    return pxm;
}

//------------------------------------------------------------------------------
void CDiagObjLabel::setPixmapAlignmentFlags( int i_iAlignmentFlags )
//------------------------------------------------------------------------------
{
    m_iPixmapAlignmentFlags = i_iAlignmentFlags;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjLabel::getPixmapAlignmentFlags() const
//------------------------------------------------------------------------------
{
    return m_iTextAlignmentFlags;
}

/*==============================================================================
public: // overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
QSize CDiagObjLabel::sizeHint()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "sizeHint",
        /* strAddInfo   */ "" );

    int cxOuterFrameWidth  = 0;
    int cyOuterFrameHeight = 0;

    if( isVisible() )
    {
        QSize sizeInnerFrame;
        QSize sizeOuterFrame;
        QSize sizeContent;
        QSize sizeText;
        bool  bShowPixmap = false;

        // Calculate extent of text (if any):
        if( !m_arstrText[m_iState].isEmpty() )
        {
            QFontMetrics fntmtr(m_fntText);
            QString      strDummyLabel = "Oy,2";
            QRect        rectText;

            rectText = fntmtr.boundingRect(strDummyLabel);
            sizeText.setWidth( rectText.width() );
            sizeText.setHeight( rectText.height() );

            switch( m_textOrientation )
            {
                case ETextOrientationBottom2Top:
                case ETextOrientationTop2Bottom:
                {
                    sizeText.setWidth( rectText.height() );
                    sizeText.setHeight( rectText.width() );
                    break;
                }
                case ETextOrientationLeft2Right:
                case ETextOrientationRight2Left:
                default:
                {
                    break;
                }
            }
        }

        // Extent of pixmap:
        if( m_arpxm[m_iState].width() > 0 && m_arpxm[m_iState].height() > 0 )
        {
            bShowPixmap = true;
        }

        // If there is just a text to be indicated ...
        if( !m_arstrText[m_iState].isEmpty() && !bShowPixmap )
        {
            sizeContent = sizeText;
        }
        // If there is just a pixmap to be indicated ...
        else if( m_arstrText[m_iState].isEmpty() && bShowPixmap )
        {
            sizeContent = m_arpxm[m_iState].size();
        }
        // If both a text and a pixmap has to be shown ...
        else if( !m_arstrText[m_iState].isEmpty() && bShowPixmap )
        {
            switch( m_textOrientation )
            {
                case ETextOrientationBottom2Top:
                case ETextOrientationTop2Bottom:
                {
                    if( ((m_iTextAlignmentFlags & Qt::AlignTop) && (m_iPixmapAlignmentFlags & Qt::AlignBottom))
                     || ((m_iTextAlignmentFlags & Qt::AlignBottom) && (m_iPixmapAlignmentFlags & Qt::AlignTop)) )
                    {
                        sizeContent.setHeight( sizeText.height() + m_arpxm[m_iState].height() );
                        if( sizeText.width() > m_arpxm[m_iState].width() )
                        {
                            sizeContent.setWidth( sizeText.width() );
                        }
                        else
                        {
                            sizeContent.setWidth( m_arpxm[m_iState].width() );
                        }
                    }
                    else
                    {
                        if( sizeText.width() > m_arpxm[m_iState].width() )
                        {
                            sizeContent.setWidth( sizeText.width() );
                        }
                        else
                        {
                            sizeContent.setWidth( m_arpxm[m_iState].width() );
                        }
                        if( sizeText.height() > m_arpxm[m_iState].height() )
                        {
                            sizeContent.setHeight( sizeText.height() );
                        }
                        else
                        {
                            sizeContent.setHeight( m_arpxm[m_iState].height() );
                        }
                    }
                    break;
                }
                case ETextOrientationLeft2Right:
                case ETextOrientationRight2Left:
                default:
                {
                    if( ((m_iTextAlignmentFlags & Qt::AlignLeft) && (m_iPixmapAlignmentFlags & Qt::AlignRight))
                     || ((m_iTextAlignmentFlags & Qt::AlignRight) && (m_iPixmapAlignmentFlags & Qt::AlignLeft)) )
                    {
                        sizeContent.setWidth( sizeText.width() + m_arpxm[m_iState].width() );
                        if( sizeText.height() > m_arpxm[m_iState].height() )
                        {
                            sizeContent.setHeight( sizeText.height() );
                        }
                        else
                        {
                            sizeContent.setHeight( m_arpxm[m_iState].height() );
                        }
                    }
                    else
                    {
                        if( sizeText.width() > m_arpxm[m_iState].width() )
                        {
                            sizeContent.setWidth( sizeText.width() );
                        }
                        else
                        {
                            sizeContent.setWidth( m_arpxm[m_iState].width() );
                        }
                        if( sizeText.height() > m_arpxm[m_iState].height() )
                        {
                            sizeContent.setHeight( sizeText.height() );
                        }
                        else
                        {
                            sizeContent.setHeight( m_arpxm[m_iState].height() );
                        }
                    }
                    break;
                }
            }
        }

        sizeInnerFrame.setWidth( sizeContent.width() + m_iMarginLeft + m_iMarginRight );
        sizeInnerFrame.setHeight( sizeContent.height() + m_iMarginTop + m_iMarginBottom );

        if( m_pFrameStyle == nullptr )
        {
            sizeOuterFrame = sizeInnerFrame;
        }
        else
        {
            sizeOuterFrame.setWidth( sizeInnerFrame.width() + 2*m_pFrameStyle->getFrameWidth() );
            sizeOuterFrame.setHeight( sizeInnerFrame.width() + 2*m_pFrameStyle->getFrameWidth() );
        }
        cxOuterFrameWidth  = sizeOuterFrame.width();
        cyOuterFrameHeight = sizeOuterFrame.height();

    } // if( isVisible() )

    if( m_cxMinimumWidth > 0 && cxOuterFrameWidth < m_cxMinimumWidth )
    {
        cxOuterFrameWidth = m_cxMinimumWidth;
    }
    if( m_cxMaximumWidth > 0 && cxOuterFrameWidth > m_cxMaximumWidth )
    {
        cxOuterFrameWidth = m_cxMaximumWidth;
    }
    if( m_cyMinimumHeight > 0 && cyOuterFrameHeight < m_cyMinimumHeight )
    {
        cyOuterFrameHeight = m_cyMinimumHeight;
    }
    if( m_cyMaximumHeight > 0 && cyOuterFrameHeight > m_cyMaximumHeight )
    {
        cyOuterFrameHeight = m_cyMaximumHeight;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg  = "Size[w,h]=";
        strTrcMsg += QString::number(cxOuterFrameWidth) + ",";
        strTrcMsg += QString::number(cyOuterFrameHeight);
        mthTracer.setMethodReturn(strTrcMsg);
    }
    return QSize(cxOuterFrameWidth,cyOuterFrameHeight);

} // sizeHint

/*==============================================================================
public: // must overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObj* CDiagObjLabel::clone( CDataDiagram* i_pDiagramTrg ) const
//------------------------------------------------------------------------------
{
    if( i_pDiagramTrg == nullptr )
    {
        return nullptr;
    }

    CDiagObjLabel* pDiagObjCloned = nullptr;

    if( !m_arstrText[0].isEmpty() )
    {
        pDiagObjCloned = new CDiagObjLabel(
            /* strObjName */ m_strObjName,
            /* strLabel   */ m_arstrText[0],
            /* layoutPos  */ m_layoutPos,
            /* stateCount */ m_iStateCount );
    }
    else
    {
        pDiagObjCloned = new CDiagObjLabel(
            /* strObjName */ m_strObjName,
            /* pxm        */ m_arpxm[0],
            /* layoutPos  */ m_layoutPos,
            /* stateCount */ m_iStateCount );
    }

    // Members from base class CDiagObj:
    pDiagObjCloned->m_rectContent = m_rectContent;
    pDiagObjCloned->m_bAdjustContentRect2DiagPartCenter = m_bAdjustContentRect2DiagPartCenter;
    pDiagObjCloned->m_bVisible = m_bVisible;
    pDiagObjCloned->m_state = m_state;
    pDiagObjCloned->m_bIsFocusable = m_bIsFocusable;
    pDiagObjCloned->m_bIsEditable = m_bIsEditable;

    // Members from this class:
    pDiagObjCloned->m_iState = m_iState;
    pDiagObjCloned->m_cxMinimumWidth = m_cxMinimumWidth;
    pDiagObjCloned->m_cxMaximumWidth = m_cxMaximumWidth;
    pDiagObjCloned->m_cyMinimumHeight = m_cyMinimumHeight;
    pDiagObjCloned->m_cyMaximumHeight = m_cyMaximumHeight;
    pDiagObjCloned->m_iLayoutPosAlignmentFlags = m_iLayoutPosAlignmentFlags;
    pDiagObjCloned->m_cxLayoutPosOffs = m_cxLayoutPosOffs;
    pDiagObjCloned->m_cyLayoutPosOffs = m_cyLayoutPosOffs;

    if( m_pFrameStyle != nullptr )
    {
        pDiagObjCloned->m_pFrameStyle = new SFrameStyle(*m_pFrameStyle);
    }
    for( int idxState = 0; idxState < m_iStateCount; idxState++ )
    {
        pDiagObjCloned->m_arcolBg[idxState] = m_arcolBg[idxState];
        pDiagObjCloned->m_arbrushStyle[idxState] = m_arbrushStyle[idxState];
    }
    pDiagObjCloned->m_iMarginTop = m_iMarginTop;
    pDiagObjCloned->m_iMarginBottom = m_iMarginBottom;
    pDiagObjCloned->m_iMarginLeft = m_iMarginLeft;
    pDiagObjCloned->m_iMarginRight = m_iMarginRight;
    for( int idxState = 0; idxState < m_iStateCount; idxState++ )
    {
        pDiagObjCloned->m_arstrText[idxState] = m_arstrText[idxState];
        pDiagObjCloned->m_arcolText[idxState] = m_arcolText[idxState];
    }
    pDiagObjCloned->m_fntText = m_fntText;
    pDiagObjCloned->m_iTextAlignmentFlags = m_iTextAlignmentFlags;
    pDiagObjCloned->m_textOrientation = m_textOrientation;
    for( int idxState = 0; idxState < m_iStateCount; idxState++ )
    {
        pDiagObjCloned->m_arpxm[idxState] = m_arpxm[idxState];
    }
    pDiagObjCloned->m_iPixmapAlignmentFlags = m_iPixmapAlignmentFlags;
    pDiagObjCloned->m_rectOuterFrame = m_rectOuterFrame;
    pDiagObjCloned->m_rectInnerFrame = m_rectInnerFrame;
    pDiagObjCloned->m_rectText = m_rectText;
    pDiagObjCloned->m_rectPixmap = m_rectPixmap;

    i_pDiagramTrg->addDiagObj(pDiagObjCloned);

    return pDiagObjCloned;

} // clone

//------------------------------------------------------------------------------
void CDiagObjLabel::update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObjUpdate, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = updateFlags2Str(i_uUpdateFlags);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ strTrcMsg );

    if( i_uUpdateFlags == EUpdateNone )
    {
        return;
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    // Please note that only if by invalidating the flag (setting the bit) in the
    // internal update flags and on requesting the process depth by this update
    // method the corresponding update process will be executed.

    // Layout processing is the task of the diagram:
    if( i_uUpdateFlags & EUpdateLayout && m_uUpdateFlags & EUpdateLayout )
    {
        mthTracer.trace("Processing Layout", ELogDetailLevel::Debug);

        // If layout processing was necessary the size of the diagrams widget might
        // have been changed and the label need to be updated on the screen.
        m_bUpdWidget = true;

        // Mark current process depth as executed (reset bit):
        validate(EUpdateLayout);
    }

    const CPixmapDiagram* pPixmapDiagram = nullptr;

    // As a matter of fact there is no sense in adding a label object to
    // a diagram just designed to analyze data.
    if( m_pDiagram != nullptr && m_pDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
    {
        pPixmapDiagram = dynamic_cast<CPixmapDiagram*>(m_pDiagram);
    }

    // If the internal data structures need to be updated ..
    if( i_uUpdateFlags & EUpdateData && m_uUpdateFlags & EUpdateData )
    {
        mthTracer.trace("Processing Data", ELogDetailLevel::Debug);

        // As a matter of fact there is no sense in adding a label object to
        // a diagram just designed to analyze data.
        if( isVisible() && pPixmapDiagram != nullptr )
        {
            m_rectPixmap.setWidth(0);
            m_rectPixmap.setHeight(0);
            m_rectText.setWidth(0);
            m_rectText.setHeight(0);
            m_rectInnerFrame.setWidth(0);
            m_rectInnerFrame.setHeight(0);
            m_rectOuterFrame.setWidth(0);
            m_rectOuterFrame.setHeight(0);

            bool  bShowPixmap = false;
            QRect rectLayoutPos = pPixmapDiagram->getRectPartCenter();

            // Calculate extent of text (if any):
            if( !m_arstrText[m_iState].isEmpty() )
            {
                // Calculate extent of label:
                QFontMetrics fntmtr(m_fntText);
                QSize sizeText;

                m_rectText = fntmtr.boundingRect(m_arstrText[m_iState]); //lint !e661 ... manchmal ist lint wirklich blind
                sizeText.setWidth( m_rectText.width() );
                sizeText.setHeight( m_rectText.height() );

                switch( m_textOrientation )
                {
                    case ETextOrientationBottom2Top:
                    case ETextOrientationTop2Bottom:
                    {
                        sizeText.setWidth( m_rectText.height() );
                        sizeText.setHeight( m_rectText.width() );
                        break;
                    }
                    case ETextOrientationLeft2Right:
                    case ETextOrientationRight2Left:
                    default:
                    {
                        break;
                    }
                } // switch( m_textOrientation )

                m_rectText.setSize(sizeText);

            } // if( !m_arstrText[m_iState].isEmpty() )

            // Extent of pixmap:
            if( m_arpxm[m_iState].width() > 0 && m_arpxm[m_iState].height() > 0 )
            {
                bShowPixmap = true;
            }

            // If there is just a text to be indicated ...
            if( !m_arstrText[m_iState].isEmpty() && !bShowPixmap )
            {
                m_rectInnerFrame.setSize( m_rectText.size() );
            }
            // If there is just a pixmap to be indicated ...
            else if( m_arstrText[m_iState].isEmpty() && bShowPixmap )
            {
                m_rectInnerFrame.setSize( m_arpxm[m_iState].size() );
            }
            // If both a text and a pixmap has to be shown ...
            else if( !m_arstrText[m_iState].isEmpty() && bShowPixmap )
            {
                switch( m_textOrientation )
                {
                    case ETextOrientationBottom2Top:
                    case ETextOrientationTop2Bottom:
                    {
                        if( ((m_iTextAlignmentFlags & Qt::AlignTop) && (m_iPixmapAlignmentFlags & Qt::AlignBottom))
                         || ((m_iTextAlignmentFlags & Qt::AlignBottom) && (m_iPixmapAlignmentFlags & Qt::AlignTop)) )
                        {
                            m_rectInnerFrame.setHeight( m_rectText.height() + m_arpxm[m_iState].height() );
                            if( m_rectText.width() > m_arpxm[m_iState].width() )
                            {
                                m_rectInnerFrame.setWidth( m_rectText.width() );
                            }
                            else
                            {
                                m_rectInnerFrame.setWidth( m_arpxm[m_iState].width() );
                            }
                        }
                        else
                        {
                            if( m_rectText.width() > m_arpxm[m_iState].width() )
                            {
                                m_rectInnerFrame.setWidth( m_rectText.width() );
                            }
                            else
                            {
                                m_rectInnerFrame.setWidth( m_arpxm[m_iState].width() );
                            }
                            if( m_rectText.height() > m_arpxm[m_iState].height() )
                            {
                                m_rectInnerFrame.setHeight( m_rectText.height() );
                            }
                            else
                            {
                                m_rectInnerFrame.setHeight( m_arpxm[m_iState].height() );
                            }
                        }
                        break;
                    }
                    case ETextOrientationLeft2Right:
                    case ETextOrientationRight2Left:
                    default:
                    {
                        if( ((m_iTextAlignmentFlags & Qt::AlignLeft) && (m_iPixmapAlignmentFlags & Qt::AlignRight))
                         || ((m_iTextAlignmentFlags & Qt::AlignRight) && (m_iPixmapAlignmentFlags & Qt::AlignLeft)) )
                        {
                            m_rectInnerFrame.setWidth( m_rectText.width() + m_arpxm[m_iState].width() );
                            if( m_rectText.height() > m_arpxm[m_iState].height() )
                            {
                                m_rectInnerFrame.setHeight( m_rectText.height() );
                            }
                            else
                            {
                                m_rectInnerFrame.setHeight( m_arpxm[m_iState].height() );
                            }
                        }
                        else
                        {
                            if( m_rectText.width() > m_arpxm[m_iState].width() )
                            {
                                m_rectInnerFrame.setWidth( m_rectText.width() );
                            }
                            else
                            {
                                m_rectInnerFrame.setWidth( m_arpxm[m_iState].width() );
                            }
                            if( m_rectText.height() > m_arpxm[m_iState].height() )
                            {
                                m_rectInnerFrame.setHeight( m_rectText.height() );
                            }
                            else
                            {
                                m_rectInnerFrame.setHeight( m_arpxm[m_iState].height() );
                            }
                        }
                        break;
                    }
                }
            }

            m_rectInnerFrame.setWidth( m_rectInnerFrame.width() + m_iMarginLeft + m_iMarginRight );
            m_rectInnerFrame.setHeight( m_rectInnerFrame.height() + m_iMarginTop + m_iMarginBottom );

            if( m_pFrameStyle == nullptr )
            {
                m_rectOuterFrame = m_rectInnerFrame;
            }
            else
            {
                m_rectOuterFrame.setWidth( m_rectInnerFrame.width() + 2*m_pFrameStyle->getFrameWidth() );
                m_rectOuterFrame.setHeight( m_rectInnerFrame.width() + 2*m_pFrameStyle->getFrameWidth() );
            }
            if( m_cxMinimumWidth > 0 && m_rectOuterFrame.width() < m_cxMinimumWidth )
            {
                m_rectOuterFrame.setWidth(m_cxMinimumWidth);
            }
            if( m_cxMaximumWidth > 0 && m_rectOuterFrame.width() > m_cxMaximumWidth )
            {
                m_rectOuterFrame.setWidth(m_cxMaximumWidth);
            }
            if( m_cyMinimumHeight > 0 && m_rectOuterFrame.height() < m_cyMinimumHeight )
            {
                m_rectOuterFrame.setHeight(m_cyMinimumHeight);
            }
            if( m_cyMaximumHeight > 0 && m_rectOuterFrame.height() > m_cyMaximumHeight )
            {
                m_rectOuterFrame.setHeight(m_cyMaximumHeight);
            }
            switch( m_layoutPos )
            {
                case ELayoutPos::Top:
                {
                    rectLayoutPos = pPixmapDiagram->getRectPartTop();
                    break;
                }
                case ELayoutPos::Bottom:
                {
                    rectLayoutPos = pPixmapDiagram->getRectPartBottom();
                    break;
                }
                case ELayoutPos::Left:
                {
                    rectLayoutPos = pPixmapDiagram->getRectPartLeft();
                    break;
                }
                case ELayoutPos::Right:
                {
                    rectLayoutPos = pPixmapDiagram->getRectPartRight();
                    break;
                }
                case ELayoutPos::Center:
                default:
                {
                    break;
                }
            } // switch( m_layoutPos )

            if( m_iLayoutPosAlignmentFlags & Qt::AlignTop )
            {
                m_rectOuterFrame.moveTop( rectLayoutPos.top() + m_cyLayoutPosOffs );
            }
            else if( m_iLayoutPosAlignmentFlags & Qt::AlignBottom )
            {
                m_rectOuterFrame.moveBottom( rectLayoutPos.bottom() - m_cyLayoutPosOffs );
            }
            else
            {
                m_rectOuterFrame.moveTop( rectLayoutPos.top() + rectLayoutPos.height()/2 - m_rectOuterFrame.height()/2 );
            }
            if( m_iLayoutPosAlignmentFlags & Qt::AlignLeft )
            {
                m_rectOuterFrame.moveLeft( rectLayoutPos.left() + m_cxLayoutPosOffs  );
            }
            else if( m_iLayoutPosAlignmentFlags & Qt::AlignRight )
            {
                m_rectOuterFrame.moveRight( rectLayoutPos.right() - m_cxLayoutPosOffs );
            }
            else
            {
                m_rectOuterFrame.moveLeft( rectLayoutPos.left() + rectLayoutPos.width()/2 - m_rectOuterFrame.width()/2 );
            }

            m_rectInnerFrame = m_rectOuterFrame;
            if( m_pFrameStyle != nullptr )
            {
                m_rectInnerFrame.moveTop( m_rectOuterFrame.top() + m_pFrameStyle->getFrameWidth() );
                m_rectInnerFrame.moveLeft( m_rectOuterFrame.left() + m_pFrameStyle->getFrameWidth() );
                m_rectInnerFrame.setWidth( m_rectOuterFrame.width() - 2*m_pFrameStyle->getFrameWidth() );
                m_rectInnerFrame.setHeight( m_rectOuterFrame.height() - 2*m_pFrameStyle->getFrameWidth() );
            }

            if( m_arpxm[m_iState].width() > 0 && m_arpxm[m_iState].height() > 0 )
            {
                // Please note that the "drawPixmap" method of the QPainter does not support
                // aligning the pixmap within a rectangle. For this we need to align the
                // pixmap rectangle on our own:
                m_rectPixmap = m_rectInnerFrame;
                if( m_arpxm[m_iState].width() < m_rectInnerFrame.width() )
                {
                    m_rectPixmap.setWidth( m_arpxm[m_iState].width() );
                }
                if( m_arpxm[m_iState].height() < m_rectInnerFrame.height() )
                {
                    m_rectPixmap.setHeight( m_arpxm[m_iState].height() );
                }
                if( m_iPixmapAlignmentFlags & Qt::AlignLeft )
                {
                    m_rectPixmap.moveLeft( m_rectInnerFrame.left() );
                }
                else if( m_iPixmapAlignmentFlags & Qt::AlignRight )
                {
                    m_rectPixmap.moveRight( m_rectInnerFrame.right() );
                }
                else
                {
                    m_rectPixmap.moveLeft( m_rectInnerFrame.center().x() - m_rectPixmap.width()/2 );
                }
                if( m_iPixmapAlignmentFlags & Qt::AlignTop )
                {
                    m_rectPixmap.moveTop( m_rectInnerFrame.top() );
                }
                else if( m_iPixmapAlignmentFlags & Qt::AlignBottom )
                {
                    m_rectPixmap.moveBottom( m_rectInnerFrame.bottom() );
                }
                else
                {
                    m_rectPixmap.moveTop( m_rectInnerFrame.center().y() - m_rectPixmap.height()/2 );
                }
            }
            if( !m_arstrText[m_iState].isEmpty() )
            {
                // Please note that the text will be aligned within the inner rect frame
                // by the "drawText" method of the QPainter.
                m_rectText = m_rectInnerFrame;
                m_rectText.moveTop( m_rectInnerFrame.top() + m_iMarginTop );
                m_rectText.moveLeft( m_rectInnerFrame.left() + m_iMarginLeft );
                m_rectText.setWidth( m_rectInnerFrame.width() - m_iMarginLeft - m_iMarginRight );    //lint !e834 ... rechne ich im Kopf aus
                m_rectText.setHeight( m_rectInnerFrame.height() - m_iMarginTop - m_iMarginBottom );  //lint !e834 ... rechne ich im Kopf aus
            }

        } // if( isVisible() && m_pDataDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )

        // If data processing was necessary the size of the diagrams widget or the
        // content of the label might have been changed and the label need to be
        // updated on the screen.
        m_bUpdWidget = true;

        // Mark current process depth as executed (reset bit):
        validate(EUpdateData);

    } // if( EUpdateData )

    // If recalculation of bitmap is needed ..
    if( i_uUpdateFlags & EUpdatePixmap && m_uUpdateFlags & EUpdatePixmap && i_pPaintDevice != nullptr )
    {
        mthTracer.trace("Processing Pixmap", ELogDetailLevel::Debug);

        // As a matter of fact there is no sense in adding a label object to
        // a diagram just designed to analyze data.
        if( isVisible() && pPixmapDiagram != nullptr )
        {
            QPainter painter(i_pPaintDevice);

            if( m_pFrameStyle != nullptr )
            {
                m_pFrameStyle->draw(&painter,m_rectOuterFrame);
            }
            if( m_arbrushStyle[m_iState] != Qt::NoBrush ) //lint !e661 ... manchmal ist lint wirklich blind
            {
                painter.fillRect( m_rectInnerFrame, QBrush(m_arcolBg[m_iState],m_arbrushStyle[m_iState]) ); //lint !e661 ... manchmal ist lint wirklich blind
            }
            if( m_arpxm[m_iState].width() > 0 && m_arpxm[m_iState].height() > 0 )
            {
                painter.drawPixmap(m_rectPixmap,m_arpxm[m_iState]); //lint !e661 ... manchmal ist lint wirklich blind
            }
            if( !m_arstrText[m_iState].isEmpty() )
            {
                painter.setFont(m_fntText);
                painter.setPen(m_arcolText[m_iState]); //lint !e661 ... manchmal ist lint wirklich blind

                switch( m_textOrientation )
                {
                    case ETextOrientationLeft2Right:
                    case ETextOrientationRight2Left:
                    {
                        painter.drawText( m_rectText, m_iTextAlignmentFlags, m_arstrText[m_iState] ); //lint !e655 !e661 ... manchmal ist lint wirklich blind
                        break;
                    }
                    case ETextOrientationBottom2Top:
                    {
                        #if QT_VERSION < 0x050000
                        QMatrix matrix;
                        #else
                        QTransform matrix;
                        #endif
                        painter.save();
                        matrix.translate( static_cast<double>(m_rectText.center().x()), static_cast<double>(m_rectText.center().y()) );
                        matrix.rotate(-90.0);
                        #if QT_VERSION < 0x050000
                        painter.setWorldMatrix(matrix);
                        #else
                        painter.setWorldTransform(matrix);
                        #endif
                        painter.drawText(
                            /* x   */ -m_rectText.height()/2,
                            /* y   */ m_rectText.width()/2,
                            /* str */ m_arstrText[m_iState] ); //lint !e661 ... manchmal ist lint wirklich blind
                        painter.restore();
                        break;
                    }
                    case ETextOrientationTop2Bottom:
                    {
                        #if QT_VERSION < 0x050000
                        QMatrix matrix;
                        #else
                        QTransform matrix;
                        #endif
                        painter.save();
                        matrix.translate( static_cast<double>(m_rectText.center().x()), static_cast<double>(m_rectText.center().y()) );
                        matrix.rotate(90.0);
                        #if QT_VERSION < 0x050000
                        painter.setWorldMatrix(matrix);
                        #else
                        painter.setWorldTransform(matrix);
                        #endif
                        painter.drawText(
                            /* x   */ -m_rectText.height()/2,
                            /* y   */ m_rectText.width()/2,
                            /* str */ m_arstrText[m_iState] ); //lint !e661 ... manchmal ist lint wirklich blind
                        painter.restore();
                        break;
                    }
                    default:
                    {
                        break;
                    }

                } // switch( m_textOrientation )
            } // if( !m_arstrText[m_iState].isEmpty() )

        } // if( isVisible() )

        // Mark current process depth as executed (reset bit):
        validate(EUpdatePixmap);

    } // if( EUpdatePixmap )

    // If the widget need to be updated ..
    if( i_uUpdateFlags & EUpdateWidget && m_uUpdateFlags & EUpdateWidget && m_pDiagram != nullptr )
    {
        mthTracer.trace("Processing Widget", ELogDetailLevel::Debug);

        // Invalidate output region of the diagram object to update (repaint) content of diagram.
        if( m_rectText.isValid() && m_bUpdWidget )
        {
            m_pDiagram->update(this, m_rectText);
        }
        if( !isVisible() )
        {
            m_rectText.setWidth(0);
            m_rectText.setHeight(0);
        }

        // Only on changing the size of the diagram or the content of the label the label
        // need to be updated on the screen. Updating the content rectangle of the label is therefore
        // only necessary if previously layout or data processing has been executed or if the color
        // or pen style has been changed.
        m_bUpdWidget = false;

        // Mark current process depth as executed (reset bit):
        validate(EUpdateWidget);

    } // if( EUpdateWidget )

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // update

/*==============================================================================
protected: // overridables of base class CDiagObj
==============================================================================*/

////------------------------------------------------------------------------------
//ZS::System::SErrResultInfo CDiagObjLabel::onReceivedReqSetValue( CMsgReqSetValue* i_pMsgReq )
////------------------------------------------------------------------------------
//{
//    if( i_pMsgReq != nullptr && i_pMsgReq->isValid(EMsgValTypeString) )
//    {
//        setText(i_pMsgReq->getValStr() );
//    }
//    return ErrResultSuccess;
//
//} // onReceivedReqSetValue
