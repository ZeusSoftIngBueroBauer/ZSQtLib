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

#include "ZSDiagram/ZSDiagObjCurve.h"
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
        /* strClassName */ CDiagObjCurve::ClassName(),
        /* strObjName   */ i_strObjName,
        /* pDiagTrace   */ i_pDiagTrace,
        /* layoutPos    */ ELayoutPos::Center ),
    m_col(Qt::yellow),
    m_pPtArr(nullptr),
    m_bUpdWidget(true)
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strObjName;
        strMthInArgs += ", Trace: " + QString(i_pDiagTrace == nullptr ? "nullptr" : i_pDiagTrace->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strMthInArgs */ strMthInArgs );

} // ctor

//------------------------------------------------------------------------------
CDiagObjCurve::~CDiagObjCurve()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    CDiagTrace* pDiagTrace = i_pDiagramTrg->findDiagTrace(m_pDiagTrace->getObjName());

    if( pDiagTrace == nullptr )
    {
        return nullptr;
    }

    CDiagObjCurve* pDiagObjCloned = new CDiagObjCurve(
        /* strObjName */ m_strObjName,
        /* pDiagTrace */ pDiagTrace );

    // Members from base class CDiagObj:
    pDiagObjCloned->m_layoutPos = m_layoutPos;
    pDiagObjCloned->m_rectContent = m_rectContent;
    pDiagObjCloned->m_bAdjustContentRect2DiagPartCenter = m_bAdjustContentRect2DiagPartCenter;
    pDiagObjCloned->m_bVisible = m_bVisible;
    pDiagObjCloned->m_state = m_state;
    pDiagObjCloned->m_bIsFocusable = m_bIsFocusable;
    pDiagObjCloned->m_bIsEditable = m_bIsEditable;

    // Members from this class:
    pDiagObjCloned->m_col = m_col;

    i_pDiagramTrg->addDiagObj(pDiagObjCloned);

    return pDiagObjCloned;

} // clone

//------------------------------------------------------------------------------
void CDiagObjCurve::update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice )
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

    // Curves are always drawn in the center area of the diagram
    // and there is no need to update any internal data for the
    // sizeHint method.
    if( i_uUpdateFlags & EUpdateLayout && m_uUpdateFlags & EUpdateLayout )
    {
        mthTracer.trace("Processing Layout", ELogDetailLevel::Debug);

        // If layout processing was necessary for the curve the scale might have
        // been changed and the curve need to be updated on the screen.
        m_bUpdWidget = true;

        // Mark current process depth as executed (reset bit):
        validate(EUpdateLayout);
    }

    // If the internal data structures need to be updated ..
    if( i_uUpdateFlags & EUpdateData && m_uUpdateFlags & EUpdateData )
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
        double        fXMin = m_pDiagTrace->getScale(EScaleDir::X).minVal().getVal();
        double        fXMax = m_pDiagTrace->getScale(EScaleDir::X).maxVal().getVal();
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
            xPix = m_pDiagTrace->getValPix(EScaleDir::X, fXMin);
            pPt->setX(xPix);

            m_pDiagTrace->getVal(
                /* scaleDirSrc */ EScaleDir::X,
                /* fValSrc     */ fXMin,
                /* pUnitSrc    */ nullptr,
                /* scaleDirDst */ EScaleDir::Y,
                /* pfValDst    */ &fy );
            yPix = m_pDiagTrace->getValPix(EScaleDir::Y, fy);
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
            xPix = m_pDiagTrace->getValPix(EScaleDir::X, fx);
            pPt->setX(xPix);

            fy = *pfY;
            yPix = m_pDiagTrace->getValPix(EScaleDir::Y, fy);
            pPt->setY(yPix);
        }

        // If the point at XScaleMax should be added ..
        if( bAddXMax )
        {
            xPix = m_pDiagTrace->getValPix(EScaleDir::X, fXMax);
            pPt->setX(xPix);

            m_pDiagTrace->getVal(
                /* scaleDirSrc */ EScaleDir::X,
                /* fValSrc     */ fXMax,
                /* pUnitSrc    */ nullptr,
                /* scaleDirDst */ EScaleDir::Y,
                /* pfValDst    */ &fy );
            yPix = m_pDiagTrace->getValPix(EScaleDir::Y,fy);
            pPt->setY(yPix);

            pPt++;
        }

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
            painter.setPen(m_col);

            if( m_pPtArr != nullptr )
            {
                painter.drawPolyline(*m_pPtArr);
            }
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
