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

#include "ZSDiagram/ZSDiagObjGrid.h"
#include "ZSDiagram/ZSDiagramProcData.h"
#include "ZSDiagram/ZSDiagScale.h"
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
class CDiagObjGrid : public CDiagObj
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObjGrid::CDiagObjGrid(
    const QString& i_strObjName,
    CDiagScale*    i_pDiagScaleX,
    CDiagScale*    i_pDiagScaleY ) :
//------------------------------------------------------------------------------
    CDiagObj(
        /* strObjName  */ i_strObjName,
        /* pDiagScaleX */ i_pDiagScaleX,
        /* pDiagScaleY */ i_pDiagScaleY,
        /* layoutPos   */ ELayoutPosCenter ),
    //m_arcol[EDivLineLayerCount]
    //m_arpenStyle[EDivLineLayerCount]
    //m_arbShow[EDivLineLayerCount]
    m_bUpdWidget(true)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // Further initialization of instance members (arrays)
    //----------------------------------------------------

    int iLayer;

    for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
    {
        m_arbShow[iLayer] = true;
        m_arcol[iLayer] = Qt::darkGray;
        m_arpenStyle[iLayer] = Qt::SolidLine;
    }

    m_arbShow[EDivLineLayerSub] = false;
    m_arcol[EDivLineLayerSub] = Qt::lightGray;

} // ctor

//------------------------------------------------------------------------------
CDiagObjGrid::~CDiagObjGrid()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    memset(m_arcol, 0x00, EDivLineLayerCount*sizeof(m_arcol[0]));
    memset(m_arpenStyle, 0x00, EDivLineLayerCount*sizeof(m_arpenStyle[0]));
    memset(m_arbShow, 0x00, EDivLineLayerCount*sizeof(m_arbShow[0]));
    m_bUpdWidget = false;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjGrid::show( EDivLineLayer i_layer )
//------------------------------------------------------------------------------
{
    int iLayer;
    int iLayerMin = i_layer;
    int iLayerMax = i_layer;

    if( i_layer != EDivLineLayerCount && (i_layer < EDivLineLayerMin || i_layer > EDivLineLayerMax) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( i_layer == EDivLineLayerCount )
    {
        iLayerMin = 0;
        iLayerMax = EDivLineLayerCount-1;
    }
    for( iLayer = iLayerMin; iLayer <= iLayerMax; iLayer++ )
    {
        if( !m_arbShow[iLayer] )
        {
            m_arbShow[iLayer] = true;
            m_bUpdWidget = true;
            invalidate(EUpdatePixmapWidget,true);
        }
    }

} // show

//------------------------------------------------------------------------------
void CDiagObjGrid::hide( EDivLineLayer i_layer )
//------------------------------------------------------------------------------
{
    int iLayer;
    int iLayerMin = i_layer;
    int iLayerMax = i_layer;

    if( i_layer != EDivLineLayerCount && (i_layer < EDivLineLayerMin || i_layer > EDivLineLayerMax) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( i_layer == EDivLineLayerCount )
    {
        iLayerMin = 0;
        iLayerMax = EDivLineLayerCount-1;
    }
    for( iLayer = iLayerMin; iLayer <= iLayerMax; iLayer++ )
    {
        if( m_arbShow[iLayer] )
        {
            m_arbShow[iLayer] = false;
            m_bUpdWidget = true;
            invalidate(EUpdatePixmapWidget,true);
        }
    }

} // hide

//------------------------------------------------------------------------------
void CDiagObjGrid::setCol( EDivLineLayer i_layer, const QColor& i_col )
//------------------------------------------------------------------------------
{
    int iLayer;
    int iLayerMin = i_layer;
    int iLayerMax = i_layer;

    if( i_layer != EDivLineLayerCount && (i_layer < EDivLineLayerMin || i_layer > EDivLineLayerMax) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( i_layer == EDivLineLayerCount )
    {
        iLayerMin = 0;
        iLayerMax = EDivLineLayerCount-1;
    }
    for( iLayer = iLayerMin; iLayer <= iLayerMax; iLayer++ )
    {
        m_arcol[iLayer] = i_col;
        m_bUpdWidget = true;
        invalidate(EUpdatePixmapWidget,true);
    }

} // setCol

//------------------------------------------------------------------------------
QColor CDiagObjGrid::getCol( EDivLineLayer i_layer ) const
//------------------------------------------------------------------------------
{
    if( i_layer < EDivLineLayerMin || i_layer > EDivLineLayerMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    return m_arcol[i_layer];

} // getCol

//------------------------------------------------------------------------------
void CDiagObjGrid::setPenStyle( EDivLineLayer i_layer, const Qt::PenStyle& i_penStyle )
//------------------------------------------------------------------------------
{
    int iLayer;
    int iLayerMin = i_layer;
    int iLayerMax = i_layer;

    if( i_layer != EDivLineLayerCount && (i_layer < EDivLineLayerMin || i_layer > EDivLineLayerMax) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( i_layer == EDivLineLayerCount )
    {
        iLayerMin = 0;
        iLayerMax = EDivLineLayerCount-1;
    }
    for( iLayer = iLayerMin; iLayer <= iLayerMax; iLayer++ )
    {
        m_arpenStyle[iLayer] = i_penStyle;
        m_bUpdWidget = true;
        invalidate(EUpdatePixmapWidget);
    }

} // setPenStyle

//------------------------------------------------------------------------------
Qt::PenStyle CDiagObjGrid::getPenStyle( EDivLineLayer i_layer ) const
//------------------------------------------------------------------------------
{
    if( i_layer < EDivLineLayerMin || i_layer > EDivLineLayerMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    return m_arpenStyle[i_layer];

} // getPenStyle

/*==============================================================================
public: // overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjGrid::show( bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    CDiagObj::show(i_bInformDiagram);
    m_bUpdWidget = true;
}

//------------------------------------------------------------------------------
void CDiagObjGrid::hide( bool i_bInformDiagram )
//------------------------------------------------------------------------------
{
    CDiagObj::hide(i_bInformDiagram);
    m_bUpdWidget = true;
}

/*==============================================================================
public: // must overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObj* CDiagObjGrid::clone( CDataDiagram* i_pDiagramTrg ) const
//------------------------------------------------------------------------------
{
    if( i_pDiagramTrg == nullptr )
    {
        return nullptr;
    }

    if( m_arpDiagScale[EScaleDirX] == nullptr || m_arpDiagScale[EScaleDirY] == nullptr )
    {
        return nullptr;
    }

    CDiagScale* pDiagScaleX = i_pDiagramTrg->getDiagScale( m_arpDiagScale[EScaleDirX]->getObjName() );
    CDiagScale* pDiagScaleY = i_pDiagramTrg->getDiagScale( m_arpDiagScale[EScaleDirY]->getObjName() );

    if( pDiagScaleX == nullptr || pDiagScaleY == nullptr )
    {
        return nullptr;
    }

    CDiagObjGrid* pDiagObj = new CDiagObjGrid(
        /* strObjName  */ m_strObjName,
        /* pDiagScaleX */ pDiagScaleX,
        /* pDiagScaleY */ pDiagScaleY );

    int iLayer;

    // Members from base class CDiagObj:
    pDiagObj->m_layoutPos = m_layoutPos;
    pDiagObj->m_rectContent = m_rectContent;
    pDiagObj->m_bAdjustContentRect2DiagPartCenter = m_bAdjustContentRect2DiagPartCenter;
    pDiagObj->m_bVisible = m_bVisible;
    pDiagObj->m_state = m_state;
    pDiagObj->m_bIsFocusable = m_bIsFocusable;
    pDiagObj->m_bIsEditable = m_bIsEditable;

    // Members from this class:
    for( iLayer = 0; iLayer < EDivLineLayerCount; iLayer++ )
    {
        pDiagObj->m_arcol[iLayer] = m_arcol[iLayer];
        pDiagObj->m_arpenStyle[iLayer] = m_arpenStyle[iLayer];
        pDiagObj->m_arbShow[iLayer] = m_arbShow[iLayer];
    }

    i_pDiagramTrg->addDiagObj(pDiagObj);

    return pDiagObj;

} // clone

//------------------------------------------------------------------------------
void CDiagObjGrid::update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice )
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

    // A grid is always drawn in the center area of the diagram
    // and there is no need to update any internal data for the
    // sizeHint method.
    if( i_uUpdateFlags & EUpdateLayout && m_uUpdateFlags & EUpdateLayout )
    {
        mthTracer.trace("Processing Layout", ELogDetailLevel::Debug);

        // If layout or data processing was necessary for the grid the scale might have
        // been changed and the grids content need to be updated on the screen.
        m_bUpdWidget = true;

        // Mark current process depth as executed (reset bit):
        validate(EUpdateLayout);
    }

    // If the internal data structures need to be updated ..
    if( i_uUpdateFlags & EUpdateData && m_uUpdateFlags & EUpdateData )
    {
        mthTracer.trace("Processing Data", ELogDetailLevel::Debug);

        // The X and Y scale objects are calculating the division lines for the main
        // and sub grid lines.

        // If layout or data processing was necessary for the grid the scale might have
        // been changed and the grids content need to be updated on the screen.
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
            QPainter painter(i_pPaintDevice);
            int      xDivLine;
            int      yDivLine;
            int      xLeft, xRight, yTop, yBottom;
            int      iLayer;
            int      idxDivLine;

            // We are going to draw the grid lines starting with the lowest layer so
            // that main grid lines will not be covered by sub grid lines.

            for( iLayer = EDivLineLayerCount-1; iLayer >= 0; iLayer-- )
            {
                if( m_arbShow[iLayer] )
                {
                    QPen pen(m_arcol[iLayer],0,m_arpenStyle[iLayer]);
                    painter.setPen(pen);

                    // Vertical lines (from left to right)
                    yTop    = m_rectContent.top();
                    yBottom = m_rectContent.bottom();
                    for( idxDivLine = 0; idxDivLine < m_arpDiagScale[EScaleDirX]->getDivLineCount(iLayer); idxDivLine++ )
                    {
                        xDivLine = static_cast<int>(m_arpDiagScale[EScaleDirX]->getDivLinePix(iLayer,idxDivLine)+0.5);
                        if( xDivLine > m_rectContent.left()+1 && xDivLine < m_rectContent.right()-1 )
                        {
                            painter.drawLine(xDivLine,yTop,xDivLine,yBottom);
                        }
                    }

                    // Horizontal lines (from bottom to top)
                    xLeft  = m_rectContent.left();
                    xRight = m_rectContent.right();
                    for( idxDivLine = 0; idxDivLine < m_arpDiagScale[EScaleDirY]->getDivLineCount(iLayer); idxDivLine++ )
                    {
                        // Remember: drawing from bottom to top: the pixel values of the lower (min)
                        // values are greater than the pixel values of the higher (max) scale values.
                        yDivLine = static_cast<int>(m_arpDiagScale[EScaleDirY]->getDivLinePix(iLayer,idxDivLine)+0.5);
                        if( yDivLine < m_rectContent.bottom()-1 && yDivLine > m_rectContent.top()+1 )
                        {
                            painter.drawLine(xLeft,yDivLine,xRight,yDivLine);
                        }
                    }
                } // if( arbShow[iLayer] )
            } // for( iLayer >= 0 )
        } // if( isVisible() )

        // Mark current process depth as executed (reset bit):
        validate(EUpdatePixmap);

    } // if( EUpdatePixmap )

    // If the widget need to be updated ..
    if( i_uUpdateFlags & EUpdateWidget && m_uUpdateFlags & EUpdateWidget && m_pDiagram != nullptr )
    {
        mthTracer.trace("Processing Widget", ELogDetailLevel::Debug);

        // Invalidate output region of the diagram object to update (repaint) content of diagram.
        if( m_rectContent.isValid() && m_bUpdWidget )
        {
            m_pDiagram->update(this, m_rectContent);
        }

        // Mark current process depth as executed (reset bit):
        validate(EUpdateWidget);

        // Only on changing the scale (the division lines), the color or the pen style the grids
        // content need to be updated on the screen. Updating the content rectangle of the grid
        // is therefore only necessary if previously layout or data processing has been executed
        // or if the color or pen style has been changed.
        m_bUpdWidget = false;

    } // if( EUpdateWidget )

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // update
