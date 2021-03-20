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
#include <QtGui/qpixmap.h>

#include "ZSDiagram/ZSDiagObjCurve.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::Diagram;
using namespace ZS::PhysVal;
using namespace ZS::Trace;


/*******************************************************************************
class CDiagObjCurve : public CDiagObj
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObjCurve::CDiagObjCurve(
    const QString& i_strObjName,
    CDiagTrace*    i_pDiagTrace ) :
//------------------------------------------------------------------------------
    CDiagObj(
        /* strObjName */ i_strObjName,
        /* pDiagTrace */ i_pDiagTrace,
        /* layoutPos  */ ELayoutPosCenter ),
    m_col(Qt::yellow),
    m_pPtArr(nullptr),
    m_bUpdWidget(true)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Diagram", "CDiagObjCurve", m_strObjName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
CDiagObjCurve::~CDiagObjCurve()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try
    {
        delete m_pPtArr;
    }
    catch(...)
    {
    }
    m_pPtArr = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjCurve::setCol( const QColor&  i_col )
//------------------------------------------------------------------------------
{
    m_col = i_col;
    m_bUpdWidget = true;
    invalidate(EUpdatePixmapWidget,true);
}

//------------------------------------------------------------------------------
QColor CDiagObjCurve::getCol() const
//------------------------------------------------------------------------------
{
    return m_col;
}

/*==============================================================================
public: // must overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObj* CDiagObjCurve::clone( CDataDiagram* i_pDiagramTrg ) const
//------------------------------------------------------------------------------
{
    if( i_pDiagramTrg == nullptr || m_pDiagTrace == nullptr )
    {
        return nullptr;
    }

    CDiagTrace* pDiagTrace = i_pDiagramTrg->getDiagTrace( m_pDiagTrace->getObjName() );

    if( pDiagTrace == nullptr )
    {
        return nullptr;
    }

    CDiagObjCurve* pDiagObj = new CDiagObjCurve(
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
    pDiagObj->m_col = m_col;

    i_pDiagramTrg->addDiagObj(pDiagObj);

    return pDiagObj;

} // clone

//------------------------------------------------------------------------------
void CDiagObjCurve::update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelUpdate) )
    {
        strTrcMsg = updateFlags2Str(i_uUpdateFlags);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelUpdate,
        /* strMethod    */ "update",
        /* strAddInfo   */ strTrcMsg );

    if( i_uUpdateFlags == EUpdateNone )
    {
        return;
    }
    if( mthTracer.isActive(c_iTrcDetailLevelUpdate) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelUpdate);
    }

    // Please note that only if by invalidating the flag (setting the bit) in the
    // internal update flags and on requesting the process depth by this update
    // method the corresponding update process will be executed.

    // Curves are always drawn in the center area of the diagram
    // and there is no need to update any internal data for the
    // sizeHint method.
    if( i_uUpdateFlags & EUpdateLayout && m_uUpdateFlags & EUpdateLayout )
    {
        mthTracer.trace("Processing Layout",c_iTrcDetailLevelUpdate);

        // If layout processing was necessary for the curve the scale might have
        // been changed and the curve need to be updated on the screen.
        m_bUpdWidget = true;

        // Mark current process depth as executed (reset bit):
        validate(EUpdateLayout);
    }

    // If the internal data structures need to be updated ..
    if( i_uUpdateFlags & EUpdateData && m_uUpdateFlags & EUpdateData )
    {
        QVector<double> arfXValues = m_pDiagTrace->getValues(EScaleDirX);
        QVector<double> arfYValues = m_pDiagTrace->getValues(EScaleDirY);

        mthTracer.trace("Processing Data",c_iTrcDetailLevelUpdate);

        const CPixmapDiagram* pPixmapDiagram = nullptr;

        // As a matter of fact there is no sense in adding a curve object to
        // a diagram just designed to analyse data.
        if( m_pDataDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
        {
            pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDataDiagram);
        }
        if( pPixmapDiagram != nullptr )
        {
            // Calculate the point array:
            //---------------------------

            int iValCount;

            iValCount = arfXValues.size();
            if( iValCount > arfYValues.size() )
            {
                iValCount = arfYValues.size();
            }
            if( !isVisible() || iValCount <= 1 )
            {
                delete m_pPtArr;
                m_pPtArr = nullptr;
                return;
            }

            int           idxValMinPrev;
            int           idxValMaxNext;
            int           idxVal;
            int           iPtCount;
            QPoint*       pPt;
            const double* pfX;
            const double* pfY;
            double        fXMin = m_pDiagTrace->getScale(EScaleDirX).m_fMin;
            double        fXMax = m_pDiagTrace->getScale(EScaleDirX).m_fMax;
            double        fx, fy;
            int           xPix, yPix;
            bool          bAddXMin = false;
            bool          bAddXMax = false;

            // TODO: Auch hier: der erste Startwert koennte optimiert berechnet werden.

            // Check how many points are visible ...
            for( idxVal = 0, idxValMinPrev = -1, idxValMaxNext = -1, iPtCount = 0, pfX = arfXValues.data();
                 idxVal < iValCount;
                 idxVal++, pfX++ )
            {
                if( *pfX < fXMin )
                {
                    idxValMinPrev = idxVal;
                }
                else if( *pfX > fXMax )
                {
                    idxValMaxNext = idxVal;
                    break;
                }
                else
                {
                    iPtCount++;
                }
            }

            // If all of the points are left of XScaleMin (incl. XScaleMin) ...
            if( iPtCount <= 1 && idxValMaxNext == -1 )
            {
                // ... none of the points is visible.
                delete m_pPtArr;
                m_pPtArr = nullptr;
                return;
            }
            // If all of the points are right of XScaleMax (incl. XScaleMax) ...
            else if( iPtCount <= 1 && idxValMinPrev == -1 )
            {
                // ... none of the points is visible.
                delete m_pPtArr;
                m_pPtArr = nullptr;
                return;
            }

            // If there are points left of XScaleMin ...
            if( idxValMinPrev != -1 )
            {
                // ... and if there is no point exactly on XScaleMin ...
                if( (idxValMinPrev+1) < iValCount && arfXValues[idxValMinPrev+1] != fXMin ) //lint !e777
                {
                    // ... the point at XScaleMin will be added.
                    iPtCount++;
                    bAddXMin = true;
                }
            }

            // If there are points right of XScaleMax ...
            if( idxValMaxNext != -1 )
            {
                // ... and if there is no point exactly on XScaleMax ...
                if( (idxValMaxNext-1) >= 0 && arfXValues[idxValMaxNext-1] != fXMax ) //lint !e777
                {
                    // ... the point at XScaleMax will be added.
                    iPtCount++;
                    bAddXMax = true;
                }
            }

            // The number of the points to be calculated are known now and the
            // point array will be allocated:
            if( m_pPtArr != nullptr )
            {
                if( m_pPtArr->size() != iPtCount )
                {
                    delete m_pPtArr;
                    #if QT_VERSION >= 0x040100
                    m_pPtArr = new QPolygon(iPtCount);
                    #else
                    m_pPtArr = new QPointArray(iPtCount);
                    #endif
                }
            }
            else
            {
                #if QT_VERSION >= 0x040100
                m_pPtArr = new QPolygon(iPtCount);
                #else
                m_pPtArr = new QPointArray(iPtCount);
                #endif
            }
            pPt = m_pPtArr->data();

            // If the point at XScaleMin should be added ..
            if( bAddXMin )
            {
                xPix = m_pDiagTrace->getValPix(EScaleDirX,fXMin);
                pPt->setX(xPix);

                m_pDiagTrace->getVal(
                    /* scaleDirSrc */ EScaleDirX,
                    /* fValSrc     */ fXMin,
                    /* pUnitSrc    */ nullptr,
                    /* scaleDirDst */ EScaleDirY,
                    /* pfValDst    */ &fy );
                yPix = m_pDiagTrace->getValPix(EScaleDirY,fy);
                pPt->setY(yPix);

                pPt++;
            }

            // Calculate the points between XScaleMin and XScaleMax ...
            pfX = &arfXValues.data()[idxValMinPrev+1];
            pfY = &arfYValues.data()[idxValMinPrev+1];
            for( idxVal = idxValMinPrev+1; idxVal < iValCount; idxVal++, pfX++, pfY++, pPt++ )
            {
                fx = *pfX;
                if( fx > fXMax )
                {
                    break;
                }
                xPix = m_pDiagTrace->getValPix(EScaleDirX,fx);
                pPt->setX(xPix);

                fy = *pfY;
                yPix = m_pDiagTrace->getValPix(EScaleDirY,fy);
                pPt->setY(yPix);
            }

            // If the point at XScaleMax should be added ..
            if( bAddXMax )
            {
                xPix = m_pDiagTrace->getValPix(EScaleDirX,fXMax);
                pPt->setX(xPix);

                m_pDiagTrace->getVal(
                    /* scaleDirSrc */ EScaleDirX,
                    /* fValSrc     */ fXMax,
                    /* pUnitSrc    */ nullptr,
                    /* scaleDirDst */ EScaleDirY,
                    /* pfValDst    */ &fy );
                yPix = m_pDiagTrace->getValPix(EScaleDirY,fy);
                pPt->setY(yPix);

                pPt++;
            }

        } // if( pPixmapDiagram != nullptr )

        // If data processing was necessary for the curve the trace values might have
        // been changed and the curve need to be updated on the screen.
        m_bUpdWidget = true;

        // Mark current process depth as executed (reset bit):
        validate(EUpdateData);

    } // if( EUpdateData )

    // If recalculation of bitmap is needed ..
    if( i_uUpdateFlags & EUpdatePixmap && m_uUpdateFlags & EUpdatePixmap && i_pPaintDevice != nullptr )
    {
        mthTracer.trace("Processing Pixmap",c_iTrcDetailLevelUpdate);

        if( isVisible() )
        {
            const CPixmapDiagram* pPixmapDiagram = nullptr;

            // As a matter of fact there is no sense in adding a curve object to
            // a diagram just designed to analyze data.
            if( m_pDataDiagram != nullptr && m_pDataDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
            {
                pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDataDiagram);
            }
            if( pPixmapDiagram != nullptr )
            {
                QPainter painter(i_pPaintDevice);

                painter.setClipRect(m_rectContent);
                painter.setClipping(true);
                painter.setPen(m_col);

                if( m_pPtArr != nullptr )
                {
                    painter.drawPolyline(*m_pPtArr);
                }

            }// if( pPixmapDiagram != nullptr )
        } // if( isVisible() )

        // Mark current process depth as executed (reset bit):
        validate(EUpdatePixmap);

    } // if( EUpdatePixmap )

    // If the widget need to be updated ..
    if( i_uUpdateFlags & EUpdateWidget && m_uUpdateFlags & EUpdateWidget && m_pDataDiagram != nullptr )
    {
        mthTracer.trace("Processing Widget",c_iTrcDetailLevelUpdate);

        CWdgtDiagram* pWdgtDiagram = dynamic_cast<CWdgtDiagram*>(m_pDataDiagram);

        if( pWdgtDiagram != nullptr )
        {
            // Invalidate output region of the diagram object to update (repaint) content of diagram.
            if( m_rectContent.isValid() && m_bUpdWidget )
            {
                pWdgtDiagram->update(this,m_rectContent);
            }

        } // if( pWdgtDiagram != nullptr )

        // Mark current process depth as executed (reset bit):
        validate(EUpdateWidget);

        // Only on changing the scale, the trace values, the colors or pen styles the curves content
        // need to be updated on the screen. Updating the content rectangle of the curve is therefore
        // only necessary if previously layout or data processing has been executed or if the color
        // or pen style has been changed.
        m_bUpdWidget = false;

    } // if( EUpdateWidget )

    if( mthTracer.isActive(c_iTrcDetailLevelUpdate) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelUpdate);
    }

} // update
