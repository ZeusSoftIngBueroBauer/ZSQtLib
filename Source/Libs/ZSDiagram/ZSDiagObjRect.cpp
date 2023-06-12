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

#include "ZSDiagram/ZSDiagObjRect.h"
#include "ZSDiagram/ZSDiagramProcData.h"
#include "ZSDiagram/ZSDiagTrace.h"
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
class CDiagObjRect : public CDiagObj
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObjRect::CDiagObjRect(
    const QString& i_strObjName,
    CDiagTrace*    i_pDiagTrace ) :
//------------------------------------------------------------------------------
    CDiagObj(
        /* strClassName */ CDiagObjRect::ClassName(),
        /* strObjName   */ i_strObjName,
        /* pDiagTrace   */ i_pDiagTrace,
        /* layoutPos    */ ELayoutPosCenter ),
    m_pen(Qt::white),
    m_brush(),
    m_rct(),
    m_bUpdWidget(true)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
CDiagObjRect::~CDiagObjRect()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    //m_pen;
    //m_brush;
    //m_rct;
    m_bUpdWidget = false;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjRect::setPen( const QPen&  i_pen )
//------------------------------------------------------------------------------
{
    m_pen = i_pen;
    m_bUpdWidget = true;
    invalidate(EUpdatePixmapWidget,true);
}

//------------------------------------------------------------------------------
void CDiagObjRect::setBrush( const QBrush& i_brush )
//------------------------------------------------------------------------------
{
    m_brush = i_brush;
    m_bUpdWidget = true;
    invalidate(EUpdatePixmapWidget,true);
}

/*==============================================================================
public: // must overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObj* CDiagObjRect::clone( CDataDiagram* i_pDiagramTrg ) const
//------------------------------------------------------------------------------
{
    if( i_pDiagramTrg == nullptr || m_pDiagTrace == nullptr )
    {
        return nullptr;
    }

    CDiagTrace* pDiagTrace = i_pDiagramTrg->findDiagTrace(m_pDiagTrace->getObjName());

    if( pDiagTrace == nullptr )
    {
        return nullptr;
    }

    CDiagObjRect* pDiagObj = new CDiagObjRect(
        /* strObjName */ m_strObjName,
        /* pDiagTrace */ pDiagTrace );

    // Members from base class CDiagObj:
    pDiagObj->m_layoutPos = m_layoutPos;
    pDiagObj->m_rectContent = m_rectContent;
    pDiagObj->m_bAdjustContentRect2DiagPartCenter = m_bAdjustContentRect2DiagPartCenter;
    pDiagObj->m_bVisible = m_bVisible;
    pDiagObj->m_state = m_state;
    pDiagObj->m_bIsFocusable = m_bIsFocusable;
    pDiagObj->m_bIsEditable = m_bIsEditable;

    // Members from this class:
    pDiagObj->m_pen = m_pen;
    pDiagObj->m_brush = m_brush;

    i_pDiagramTrg->addDiagObj(pDiagObj);

    return pDiagObj;

} // clone

//------------------------------------------------------------------------------
void CDiagObjRect::update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice )
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

    // Rectangles are always drawn in the center area of the diagram and there is
    // no need to update any internal data for the sizeHint method.
    if( i_uUpdateFlags & EUpdateLayout && m_uUpdateFlags & EUpdateLayout )
    {
        mthTracer.trace("Processing Layout", ELogDetailLevel::Debug);

        // If layout processing was necessary for the object the scale might have
        // been changed and the object need to be updated on the screen.
        m_bUpdWidget = true;

        // Mark current process depth as executed (reset bit):
        validate(EUpdateLayout);
    }

    // If the internal data structures need to be updated ..
    if( i_uUpdateFlags & EUpdateData && m_uUpdateFlags & EUpdateData )
    {
        if( m_pDiagTrace != nullptr )
        {
            QVector<double> arfXValues = m_pDiagTrace->getValues(EScaleDir::X);
            QVector<double> arfYValues = m_pDiagTrace->getValues(EScaleDir::Y);

            mthTracer.trace("Processing Data", ELogDetailLevel::Debug);

            // Calculate the point array:
            //---------------------------

            int iValCount;

            iValCount = arfXValues.size();
            if( iValCount > arfYValues.size() )
            {
                iValCount = arfYValues.size();
            }
            if( !isVisible() || iValCount < 2 )
            {
                m_rct = QRect();
                return;
            }

            double fXMin = m_pDiagTrace->getScale(EScaleDir::X).m_fMin;
            double fXMax = m_pDiagTrace->getScale(EScaleDir::X).m_fMax;
            double fYMin = m_pDiagTrace->getScale(EScaleDir::Y).m_fMin;
            double fYMax = m_pDiagTrace->getScale(EScaleDir::Y).m_fMax;
            int    x1, x2;
            int    y1, y2;

            double fX1 = arfXValues.first();
            double fX2 = arfXValues.last();
            double fY1 = arfYValues.first();
            double fY2 = arfYValues.last();

            // If the points out of the visible scale range move them to be witin the scale range.
            if( fX1 < fXMin ) fX1 = fXMin;
            if( fX1 > fXMax ) fX1 = fXMax;
            if( fY1 < fYMin ) fY1 = fYMin;
            if( fY1 > fYMax ) fY1 = fYMax;

            // Calculate the points between XScaleMin and XScaleMax ...
            x1 = m_pDiagTrace->getValPix(EScaleDir::X, fX1);
            x2 = m_pDiagTrace->getValPix(EScaleDir::X, fX2);
            y1 = m_pDiagTrace->getValPix(EScaleDir::Y, fY1);
            y2 = m_pDiagTrace->getValPix(EScaleDir::Y, fY2);

            if( x1 < x2 )
            {
                m_rct.setLeft(x1);
                m_rct.setRight(x2);
            }
            else
            {
                m_rct.setLeft(x2);
                m_rct.setRight(x1);
            }
            if( y1 < y2 )
            {
                m_rct.setTop(y1);
                m_rct.setBottom(y2);
            }
            else
            {
                m_rct.setTop(y2);
                m_rct.setBottom(y1);
            }
        } // if( m_pDiagTrace != nullptr )

        // If data processing was necessary for the curve the trace values might have
        // been changed and the curve need to be updated on the screen.
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

            painter.setClipRect(m_rectContent);
            painter.setClipping(true);
            painter.setPen(m_pen);
            painter.setBrush(m_brush);

            if( m_rct.isValid() )
            {
                painter.drawRect(m_rct);
            }
        } // if( isVisible() )

        // Mark current process depth as executed (reset bit):
        validate(EUpdatePixmap);

    } // if( EUpdatePixmap )

    // If the widget need to be updated ..
    if( i_uUpdateFlags & EUpdateWidget && m_uUpdateFlags & EUpdateWidget && m_pDiagram != nullptr )
    {
        mthTracer.trace("Processing Widget", ELogDetailLevel::Debug);

        m_pDiagram->update(this,m_rectContent);

        // Mark current process depth as executed (reset bit):
        validate(EUpdateWidget);

        // Only on changing the scale, the trace values, the colors or pen styles the curves content
        // need to be updated on the screen. Updating the content rectangle of the curve is therefore
        // only necessary if previously layout or data processing has been executed or if the color
        // or pen style has been changed.
        m_bUpdWidget = false;

    } // if( EUpdateWidget )

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // update
