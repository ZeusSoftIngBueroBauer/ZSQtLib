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

#include "ZSDiagram/ZSDiagObjProgressBar.h"
#include "ZSDiagram/ZSDiagramProcWdgt.h"
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
class CDiagObjProgressBar : public CDiagObj
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObjProgressBar::CDiagObjProgressBar(
    const QString& i_strObjName,
    ELayoutPos     i_layoutPos ) :
//------------------------------------------------------------------------------
    CDiagObj(
        /* strObjName */ i_strObjName,
        /* pDiagTrace */ nullptr,
        /* layoutPos  */ i_layoutPos ),
    m_fValMin(0.0),
    m_fValMax(100.0),
    m_fVal(0.0),
    m_colFg(Qt::blue),
    m_colBg(Qt::white),
    m_iIndentTop(0),
    m_iIndentBottom(0),
    m_iIndentLeft(0),
    m_iIndentRight(0),
    m_pFrameStyle(nullptr),
    m_iWidth(10),
    m_rectBarContent(),
    m_rectBarFrame(),
    m_rectBarInFrameContent(),
    m_rectBarInFrameValue(),
    m_bUpdWidget(true)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Diagram", "CDiagObjProgressBar", m_strObjName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
CDiagObjProgressBar::~CDiagObjProgressBar()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try
    {
        delete m_pFrameStyle;
    }
    catch(...)
    {
    }
    m_pFrameStyle = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjProgressBar::setValMin( double i_fVal )
//------------------------------------------------------------------------------
{
    m_fValMin = i_fVal;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
double CDiagObjProgressBar::getValMin() const
//------------------------------------------------------------------------------
{
    return m_fValMin;
}

//------------------------------------------------------------------------------
void CDiagObjProgressBar::setValMax( double i_fVal )
//------------------------------------------------------------------------------
{
    m_fValMax = i_fVal;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
double CDiagObjProgressBar::getValMax() const
//------------------------------------------------------------------------------
{
    return m_fValMax;
}

//------------------------------------------------------------------------------
void CDiagObjProgressBar::setVal( double i_fVal )
//------------------------------------------------------------------------------
{
    m_fVal = i_fVal;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
double CDiagObjProgressBar::getVal() const
//------------------------------------------------------------------------------
{
    return m_fVal;
}

//------------------------------------------------------------------------------
void CDiagObjProgressBar::setColFg( const QColor& i_col )
//------------------------------------------------------------------------------
{
    m_colFg = i_col;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
QColor CDiagObjProgressBar::getColFg() const
//------------------------------------------------------------------------------
{
    return m_colFg;
}

//------------------------------------------------------------------------------
void CDiagObjProgressBar::setColBg( const QColor& i_col )
//------------------------------------------------------------------------------
{
    m_colBg = i_col;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
QColor CDiagObjProgressBar::getColBg() const
//------------------------------------------------------------------------------
{
    return m_colBg;
}

//------------------------------------------------------------------------------
void CDiagObjProgressBar::setIndentTop( int i_iIndent )
//------------------------------------------------------------------------------
{
    m_iIndentTop = i_iIndent;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjProgressBar::getIndentTop() const
//------------------------------------------------------------------------------
{
    return m_iIndentTop;
}

//------------------------------------------------------------------------------
void CDiagObjProgressBar::setIndentBottom( int i_iIndent )
//------------------------------------------------------------------------------
{
    m_iIndentBottom = i_iIndent;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjProgressBar::getIndentBottom() const
//------------------------------------------------------------------------------
{
    return m_iIndentBottom;
}

//------------------------------------------------------------------------------
void CDiagObjProgressBar::setIndentLeft( int i_iIndent )
//------------------------------------------------------------------------------
{
    m_iIndentLeft = i_iIndent;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjProgressBar::getIndentLeft() const
//------------------------------------------------------------------------------
{
    return m_iIndentLeft;
}

//------------------------------------------------------------------------------
void CDiagObjProgressBar::setIndentRight( int i_iIndent )
//------------------------------------------------------------------------------
{
    m_iIndentRight = i_iIndent;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjProgressBar::getIndentRight() const
//------------------------------------------------------------------------------
{
    return m_iIndentRight;
}

//------------------------------------------------------------------------------
void CDiagObjProgressBar::setFrameStyle( SFrameStyle* i_pFrameStyle )
//------------------------------------------------------------------------------
{
    delete m_pFrameStyle;
    m_pFrameStyle = i_pFrameStyle;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
SFrameStyle* CDiagObjProgressBar::getFrameStyle()
//------------------------------------------------------------------------------
{
    return m_pFrameStyle;
}

//------------------------------------------------------------------------------
void CDiagObjProgressBar::setWidth( int i_iWidth )
//------------------------------------------------------------------------------
{
    m_iWidth = i_iWidth;
    invalidate(EUpdateLayoutDataPixmapWidget,true);
}

//------------------------------------------------------------------------------
int CDiagObjProgressBar::getWidth() const
//------------------------------------------------------------------------------
{
    return m_iWidth;
}

/*==============================================================================
public: // overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
QSize CDiagObjProgressBar::sizeHint()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "sizeHint",
        /* strAddInfo   */ "" );

    int cxWidth  = 0;
    int cyHeight = 0;

    // As a matter of fact there is no sense in adding a progress bar object to
    // a diagram just designed to analyze data.
    switch( m_layoutPos )
    {
        case ELayoutPosTop:
        case ELayoutPosBottom:
        {
            cyHeight = m_iIndentTop + m_iIndentBottom + m_iWidth;
            if( m_pFrameStyle != nullptr )
            {
                cyHeight += m_pFrameStyle->getFrameWidth();
            }
            break;
        }
        case ELayoutPosLeft:
        case ELayoutPosRight:
        {
            cxWidth = m_iIndentLeft + m_iIndentRight + m_iWidth;
            if( m_pFrameStyle != nullptr )
            {
                cxWidth += m_pFrameStyle->getFrameWidth();
            }
            break;
        }
        default:
        {
            break;
        }
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg  = "Size[w,h]=";
        strTrcMsg += QString::number(cxWidth) + ",";
        strTrcMsg += QString::number(cyHeight);
        mthTracer.setMethodReturn(strTrcMsg);
    }
    return QSize(cxWidth,cyHeight);

} // sizeHint

/*==============================================================================
public: // must overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObj* CDiagObjProgressBar::clone( CDataDiagram* i_pDiagramTrg ) const
//------------------------------------------------------------------------------
{
    if( i_pDiagramTrg == nullptr )
    {
        return nullptr;
    }

    CDiagObjProgressBar* pDiagObj = new CDiagObjProgressBar(
        /* strObjName */ m_strObjName,
        /* layoutPos  */ m_layoutPos );

    // Members from base class CDiagObj:
    pDiagObj->m_rectContent = m_rectContent;
    pDiagObj->m_bAdjustContentRect2DiagPartCenter = m_bAdjustContentRect2DiagPartCenter;
    pDiagObj->m_bVisible = m_bVisible;
    pDiagObj->m_state = m_state;
    pDiagObj->m_bIsFocusable = m_bIsFocusable;
    pDiagObj->m_bIsEditable = m_bIsEditable;

    // Members from this class:
    pDiagObj->m_fValMin = m_fValMin;
    pDiagObj->m_fValMax = m_fValMax;
    //pDiagObj->m_fVal = m_fVal;
    pDiagObj->m_colFg = m_colFg;
    pDiagObj->m_colBg = m_colBg;
    pDiagObj->m_iIndentTop = m_iIndentTop;
    pDiagObj->m_iIndentBottom = m_iIndentBottom;
    pDiagObj->m_iIndentLeft = m_iIndentLeft;
    pDiagObj->m_iIndentRight = m_iIndentRight;
    if( m_pFrameStyle != nullptr )
    {
        pDiagObj->m_pFrameStyle = new SFrameStyle(*m_pFrameStyle);
    }
    pDiagObj->m_iWidth = m_iWidth;
    pDiagObj->m_rectBarContent = m_rectBarContent;
    pDiagObj->m_rectBarFrame = m_rectBarFrame;
    //pDiagObj->m_rectBarInFrameContent = m_rectBarInFrameContent;
    //pDiagObj->m_rectBarInFrameValue = m_rectBarInFrameValue;

    i_pDiagramTrg->addDiagObj(pDiagObj);

    return pDiagObj;

} // clone

//------------------------------------------------------------------------------
void CDiagObjProgressBar::update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObjUpdate != nullptr && m_pTrcAdminObjUpdate->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
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

    // For the progress bar there is no need for layout or data processing.
    // The sizeHint method just needs to consider the frame width, indents
    // and margins to return the desired width or height of the progress bar.
    // And it is sufficient to calculate the geometry of the progress during
    // pixmap processing.
    if( i_uUpdateFlags & EUpdateLayout && m_uUpdateFlags & EUpdateLayout )
    {
        mthTracer.trace("Processing Layout", ELogDetailLevel::Debug);

        // If layout processing was necessary for the progress bar the size
        // of the diagrams widget might have been changed and the bar need
        // to be updated on the screen.
        m_bUpdWidget = true;

        // Mark current process depth as executed (reset bit):
        validate(EUpdateLayout);
    }

    // If the internal data structures need to be updated ..
    if( i_uUpdateFlags & EUpdateData && m_uUpdateFlags & EUpdateData )
    {
        mthTracer.trace("Processing Data", ELogDetailLevel::Debug);

        // If data processing was necessary for the progress bar the content
        // of the progress bar might have been changed and the bar need
        // to be updated on the screen.
        m_bUpdWidget = true;

        // Mark current process depth as executed (reset bit):
        validate(EUpdateData);

    } // if( EUpdateData )

    // If recalculation of bitmap is needed ..
    if( i_uUpdateFlags & EUpdatePixmap && m_uUpdateFlags & EUpdatePixmap && i_pPaintDevice != nullptr )
    {
        mthTracer.trace("Processing Pixmap", ELogDetailLevel::Debug);

        if( isVisible() )
        {
            const CPixmapDiagram* pPixmapDiagram = nullptr;

            // As a matter of fact there is no sense in adding a progress bar object to
            // a diagram just designed to analyze data.
            if( m_pDataDiagram != nullptr && m_pDataDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
            {
                pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDataDiagram);
            }
            if( pPixmapDiagram != nullptr )
            {
                QPainter painter(i_pPaintDevice);
                QBrush   brush;

                // During update data the progress bar rect has been calculated. But this
                // was for the sizeHint method which the diagram called to calculate
                // the content rectangle of the diagram object. The progress bars rectangle
                // may differ from the content rectangle if the extent should be
                // limited to the diagrams center part.
                m_rectBarContent = m_rectContent;

                if( m_bAdjustContentRect2DiagPartCenter )
                {
                    switch( m_layoutPos )
                    {
                        case ELayoutPosTop:
                        case ELayoutPosBottom:
                        {
                            m_rectBarContent.setLeft(pPixmapDiagram->getRectPartCenter().left());
                            m_rectBarContent.setWidth(pPixmapDiagram->getRectPartCenter().width());
                            break;
                        }
                        case ELayoutPosLeft:
                        case ELayoutPosRight:
                        {
                            m_rectBarContent.setTop(pPixmapDiagram->getRectPartCenter().top());
                            m_rectBarContent.setHeight(pPixmapDiagram->getRectPartCenter().height());
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    } // switch( m_layoutPos )
                }

                m_rectBarFrame = m_rectBarContent;

                m_rectBarFrame.moveLeft( m_rectBarContent.left() + m_iIndentLeft );
                m_rectBarFrame.moveTop( m_rectBarContent.top() + m_iIndentTop );
                m_rectBarFrame.setWidth( m_rectBarContent.width() - m_iIndentLeft - m_iIndentRight ); //lint !e834
                m_rectBarFrame.setHeight( m_rectBarContent.height() - m_iIndentTop - m_iIndentTop );  //lint !e834

                m_rectBarInFrameContent = m_rectBarFrame;

                if( m_pFrameStyle != nullptr )
                {
                    int iWidthFrame = m_pFrameStyle->getFrameWidth();

                    m_pFrameStyle->draw( &painter, m_rectBarFrame );

                    m_rectBarInFrameContent.moveLeft( m_rectBarFrame.left() + iWidthFrame );
                    m_rectBarInFrameContent.moveTop( m_rectBarFrame.top() + iWidthFrame );
                    m_rectBarInFrameContent.setWidth( m_rectBarFrame.width() - 2*iWidthFrame );
                    m_rectBarInFrameContent.setHeight( m_rectBarFrame.height() - 2*iWidthFrame );
                }

                m_rectBarInFrameValue = m_rectBarInFrameContent;

                double fValWidth  = m_rectBarInFrameContent.width() * (m_fVal-m_fValMin)/(m_fValMax-m_fValMin);
                int    cxValWidth = static_cast<int>(fValWidth);

                m_rectBarInFrameValue.setTop(m_rectBarInFrameContent.top());
                m_rectBarInFrameValue.setLeft(m_rectBarInFrameContent.left());
                m_rectBarInFrameValue.setWidth(cxValWidth);
                m_rectBarInFrameValue.setHeight(m_rectBarInFrameContent.height());

                // Draw background of progress bar
                brush.setColor(m_colBg);
                brush.setStyle(Qt::SolidPattern);
                painter.fillRect(m_rectBarInFrameContent,brush);

                // Draw value (foreground) of progress bar
                brush.setColor(m_colFg);
                brush.setStyle(Qt::SolidPattern);
                painter.fillRect(m_rectBarInFrameValue,brush);

            }// if( pPixmapDiagram != nullptr )
        } // if( isVisible() )

        // Mark current process depth as executed (reset bit):
        validate(EUpdatePixmap);

    } // if( EUpdatePixmap )

    // If the widget need to be updated ..
    if( i_uUpdateFlags & EUpdateWidget && m_uUpdateFlags & EUpdateWidget && m_pDataDiagram != nullptr )
    {
        mthTracer.trace("Processing Widget", ELogDetailLevel::Debug);

        CWdgtDiagram* pWdgtDiagram = nullptr;

        if( m_pDataDiagram->getUpdateType() >= EDiagramUpdateTypeWidget )
        {
            pWdgtDiagram = dynamic_cast<CWdgtDiagram*>(m_pDataDiagram);
        }
        if( pWdgtDiagram != nullptr )
        {
            // Invalidate output region of the diagram object to update (repaint) content of diagram.
            if( m_rectContent.isValid() && m_bUpdWidget )
            {
                pWdgtDiagram->update(this,m_rectContent);
            }

        } // if( pWdgtDiagram != nullptr )

        // Only on changing the size of the diagram or the content of the progress bar the bar
        // need to be updated on the screen. Updating the content rectangle of the bar is therefore
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
protected: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CDiagObjProgressBar::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    #if 0

    if( i_pEv->type() == CBarGraphEvent::EEventTypeBarGraphVal )
    {
        const CBarGraphEvent* pBarGraphEvent = dynamic_cast<const CBarGraphEvent*>(i_pEv);
        CPixmapDiagram*       pPixmapDiagram = nullptr;
        CWdgtDiagram*         pWdgtDiagram   = nullptr;

        if( pBarGraphEvent != nullptr )
        {
            double fVal = pBarGraphEvent->getVal();

            if( fVal < m_fValMin )
            {
                fVal = m_fValMin;
            }
            else if( fVal > m_fValMax )
            {
                fVal = m_fValMax;
            }
            if( fVal >= m_fVal )
            {
                m_fVal = fVal;

                // As a matter of fact there is no sense in adding a progress bar object to
                // a diagram just designed to analyze data.
                if( m_pDataDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
                {
                    pPixmapDiagram = dynamic_cast<CPixmapDiagram*>(m_pDataDiagram);
                }
                if( pPixmapDiagram != nullptr )
                {
                    invalidate(EUpdateDataPixmapWidget,false);
                    update( EUpdateDataPixmapWidget, pPixmapDiagram->getPixmap() );
                }
                if( m_pDataDiagram->getUpdateType() >= EDiagramUpdateTypeWidget )
                {
                    pWdgtDiagram = dynamic_cast<CWdgtDiagram*>(m_pDataDiagram);
                }
                if( pWdgtDiagram != nullptr )
                {
                    QPaintEvent* pPaintEv = new QPaintEvent(m_rectContent);
                    QApplication::postEvent(pWdgtDiagram,pPaintEv);
                    pPaintEv = nullptr; //lint !e423
                }
            }
            else
            {
                m_fVal = fVal;

                // Update content of progress bar
                invalidate(EUpdateDataPixmapWidget,true);
                m_pDataDiagram->update(EUpdateDataPixmapWidget);
            }
            bEventHandled = true;
        }
    }
    #endif

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pEv);
    }
    return bEventHandled;

} // event
