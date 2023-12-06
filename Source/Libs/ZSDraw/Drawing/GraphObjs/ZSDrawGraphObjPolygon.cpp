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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjPolygon.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/QBitmap>
#include <QtGui/qevent.h>
#include <QtGui/QPainter>

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsSceneEvent>
#include <QtGui/QStyleOption>
#else
#include <QtWidgets/QGraphicsSceneEvent>
#include <QtWidgets/QStyleOption>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


/*******************************************************************************
class CGraphObjPolygon : public CGraphObj, public QGraphicsPolygonItem
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjPolygon::s_iInstCount = 0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjPolygon::CGraphObjPolygon(
    CDrawingScene* i_pDrawingScene, const QString& i_strObjName) :
//------------------------------------------------------------------------------
    CGraphObjPolyline(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ EGraphObjTypePolygon,
        /* strObjName          */ i_strObjName.isEmpty() ? "Polygon" + QString::number(s_iInstCount) : i_strObjName)
{
    // Just incremented by the ctor but not decremented by the dtor.
    // Used to create a unique name for newly created objects of this type.
    s_iInstCount++;

    QString strAddTrcInfo;

    createTraceAdminObjs("StandardShapes::" + ClassName());

    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strAddTrcInfo );

    QPolygonF plg = polygon();
    QRectF    rctBounding = plg.boundingRect();

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    m_ptRotOriginCurr = rctBounding.center();
#endif

    //onDrawSettingsChanged();

    updateToolTip();

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

} // ctor

//------------------------------------------------------------------------------
CGraphObjPolygon::~CGraphObjPolygon()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    emit_aboutToBeDestroyed();

} // dtor

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the type method of QGraphicsItem.
*/
int CGraphObjPolygon::type() const
//------------------------------------------------------------------------------
{
    return EGraphObjTypePolygon;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjPolygon::clone()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal))
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "clone",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjPolygon* pGraphObj = new CGraphObjPolygon(m_pDrawingScene, m_strName);
    pGraphObj->setDrawSettings(m_drawSettings);

    pGraphObj->setPolygon( polygon() );
    pGraphObj->setPos( pos() );
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    pGraphObj->setRotationAngleInDegree(m_fRotAngleCurr_deg);
    //pGraphObj->setScaleFactors(m_fScaleFacXCurr,m_fScaleFacYCurr);
    pGraphObj->acceptCurrentAsOriginalCoors();
#endif

    return pGraphObj;

} // clone

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjPolygon::onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "OldSettings {" + i_drawSettingsOld.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "onDrawSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_drawSettings.isPenUsed() )
    {
        if( m_drawSettings.getLineStyle() != ELineStyle::NoLine )
        {
            QPen pen;

            pen.setColor( m_drawSettings.getPenColor() );
            pen.setWidth( m_drawSettings.getPenWidth() );
            pen.setStyle( lineStyle2QtPenStyle(m_drawSettings.getLineStyle()) );

            setPen(pen);
        }
        else
        {
            setPen(Qt::NoPen);
        }
    }
    else
    {
        setPen(Qt::NoPen);
    }

    if( m_drawSettings.isFillUsed() )
    {
        if( m_drawSettings.getFillStyle() != EFillStyle::NoFill )
        {
            QBrush brsh;

            brsh.setColor( m_drawSettings.getFillColor() );
            brsh.setStyle( fillStyle2QtBrushStyle(m_drawSettings.getFillStyle()) );

            setBrush(brsh);
        }
        else
        {
            setBrush(Qt::NoBrush);
        }
    }
    else
    {
        setBrush(Qt::NoBrush);
    }

} // onDrawSettingsChanged

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjPolygon::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;
    if (areMethodCallsActive(m_pTrcAdminObjIsHit, EMethodTraceDetailLevel::ArgsNormal)) {
        strAddTrcInfo = "Point:" + point2Str(i_pt) +
            ", HitInfo, " + QString(o_pHitInfo == nullptr ? "null" : pointer2Str(o_pHitInfo)) +
            ", Polygon:" + polygon2Str(polygon());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjIsHit,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "isHit",
        /* strAddInfo   */ strAddTrcInfo );

    bool bIsHit = false;

    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        if( pGraphicsItem->isSelected() )
        {
            bIsHit = isPolygonSelectionPointHit(i_pt,o_pHitInfo);

            if( !bIsHit )
            {
                bIsHit = isBoundingRectSelectionPointHit(
                    /* pt               */ i_pt,
                    /* iSelPtsCount     */ -1,
                    /* pSelPts          */ nullptr,
                    /* pGraphObjHitInfo */ o_pHitInfo );
            }
        }

        if( !bIsHit )
        {
            QPolygonF plg = polygon();
            bIsHit = isPolygonHit( plg, EFillStyle::NoFill, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo );
        }

        if( !bIsHit )
        {
            // In contrary to polylines for polygons we also have to check "contains" for solid fill styles.
            if( pGraphicsItem->isSelected() || m_drawSettings.getFillStyle() == EFillStyle::SolidPattern )
            {
                bIsHit = pGraphicsItem->contains(i_pt);

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditMode::Move;
                    o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                }
            }
        }

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
        if( bIsHit && o_pHitInfo != nullptr )
        {
            o_pHitInfo->setCursor( Math::deg2Rad(m_fRotAngleCurr_deg) );
        }
#endif

    } // if( pGraphicsItem != nullptr )

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs;
        if (o_pHitInfo != nullptr) {
            strMthOutArgs = "HitInfo {" + o_pHitInfo->toString() + "}";
            mthTracer.setMethodOutArgs(strMthOutArgs);
        }
        mthTracer.setMethodReturn(bIsHit);
    }

    return bIsHit;

} // isHit

/*==============================================================================
public: // reimplementing methods of base class QGraphicItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjPolygon::setCursor( const QCursor& i_cursor )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = qCursorShape2Str(i_cursor.shape());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setCursor",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjPolyline::setCursor(i_cursor);

} // setCursor

/*==============================================================================
public: // overridables of base class QGraphicsPolygonItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjPolygon::boundingRect() const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObjBoundingRect, EMethodTraceDetailLevel::ArgsNormal))
    {
        strAddTrcInfo = "Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ strAddTrcInfo );

    QRectF rctBounding = QGraphicsPolygonItem::boundingRect();

    CGraphObjSelectionPoint* pGraphObjSelPt;
    QRectF                   rctSelPt;
    QPolygonF                plgSelPt;
    ESelectionPoint          selPt;
    int                      idxSelPt;

    for( idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++ )
    {
        pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

        if( pGraphObjSelPt != nullptr )
        {
            rctSelPt = pGraphObjSelPt->boundingRect();
            plgSelPt = mapFromItem( pGraphObjSelPt, rctSelPt );
            rctBounding |= plgSelPt.boundingRect();
        }
    }

    for( idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++ )
    {
        selPt = static_cast<ESelectionPoint>(idxSelPt);

        pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

        if( pGraphObjSelPt != nullptr )
        {
            rctSelPt = pGraphObjSelPt->boundingRect();
            plgSelPt = mapFromItem( pGraphObjSelPt, rctSelPt );
            rctBounding |= plgSelPt.boundingRect();
        }
    }

    if( m_plgLineStart.size() > 0 )
    {
        rctBounding |= m_plgLineStart.boundingRect();
    }

    if( m_plgLineEnd.size() > 0 )
    {
        rctBounding |= m_plgLineEnd.boundingRect();
    }

    //if( m_bIsHit || isSelected() )
    //{
    //    QHashIterator<QString, CGraphObjLabel*> itLabels(m_arpLabels);
    //    CGraphObjLabel* pGraphObjLabel;
    //    QRectF          rctLabel;
    //    QPolygonF       plgLabel;

    //    while( itLabels.hasNext() )
    //    {
    //        itLabels.next();
    //        pGraphObjLabel = itLabels.value();

    //        if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
    //        {
    //            // Calling pGraphObjLabel->boundingRect() may lead to endless recursion as the
    //            // label itself may call the boundingRect method of its parent item (which is
    //            // this item) if the label is selected or has been hit. For this we call
    //            // boundingRect of the label with a different signature to indicate that we
    //            // are only interested in the bounding rect of the simple text item.
    //            rctLabel = pGraphObjLabel->m_pGraphObjLabel->boundingRect(true);
    //            plgLabel = mapFromItem( pGraphObjLabel->m_pGraphObjLabel, rctLabel );
    //            rctBounding |= plgLabel.boundingRect();
    //        }
    //    }
    //}

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo  = "Rect(x,y,w,h):(";
        strAddTrcInfo += QString::number(rctBounding.x(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.y(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.width(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.height(),'f',1) + ")";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return rctBounding;

} // boundingRect

//------------------------------------------------------------------------------
void CGraphObjPolygon::paint(
    QPainter*                       i_pPainter,
    const QStyleOptionGraphicsItem* /*i_pStyleOption*/,
    QWidget*                        /*i_pWdgt*/ )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjPaint, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ZValue: " + QString::number(zValue());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "paint",
        /* strAddInfo   */ strMthInArgs );

    i_pPainter->save();

    QPen   pn   = pen();
    QBrush brsh = brush();

    i_pPainter->setPen( pen() );
    i_pPainter->setBrush( brush() );

    QPolygonF plg = polygon();

    i_pPainter->drawPolygon(plg);

    if( m_pDrawingScene->getMode() == EMode::Edit && (m_bIsHit || isSelected()) )
    {
        if( plg.size() > 0 )
        {
            QRectF rctBounding = plg.boundingRect();

            pn.setStyle(Qt::DotLine);
            pn.setColor(Qt::blue);
            pn.setWidth(1);

            i_pPainter->setPen(pn);
            i_pPainter->setBrush(Qt::NoBrush);

            i_pPainter->drawRect(rctBounding);

            if( isSelected() )
            {
                if( m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::TopCenter)] != nullptr && m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateTop)] != nullptr )
                {
                    CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::TopCenter)];
                    CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateTop)];

                    QPointF ptRct = QPointF( pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y() );
                    QPointF ptRot = QPointF( pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y() );

                    QPointF ptRctM = mapFromScene(ptRct);
                    QPointF ptRotM = mapFromScene(ptRot);

                    i_pPainter->drawLine( ptRctM, ptRotM );
                }

                if( m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::BottomCenter)] != nullptr && m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateBottom)] != nullptr )
                {
                    CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::BottomCenter)];
                    CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateBottom)];

                    QPointF ptRct = QPointF( pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y() );
                    QPointF ptRot = QPointF( pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y() );

                    QPointF ptRctM = mapFromScene(ptRct);
                    QPointF ptRotM = mapFromScene(ptRot);

                    i_pPainter->drawLine( ptRctM, ptRotM );
                }
            } // if( isSelected() )

            //QHashIterator<QString, CGraphObjLabel*> itLabels(m_arpLabels);
            //CGraphObjLabel* pGraphObjLabel;

            //QPointF ptSelPt;
            //QPointF ptLabelSelPt;

            //while( itLabels.hasNext() )
            //{
            //    itLabels.next();
            //    pGraphObjLabel = itLabels.value();

            //    if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
            //    {
            //        ptSelPt = getSelectionPointCoors(pGraphObjLabel->m_selPt.enumerator());

            //        ptLabelSelPt = pGraphObjLabel->m_pGraphObjLabel->getSelectionPointCoors(ESelectionPoint::Center);
            //        ptLabelSelPt = mapFromItem( pGraphObjLabel->m_pGraphObjLabel, ptLabelSelPt );

            //        i_pPainter->drawLine( ptSelPt, ptLabelSelPt );
            //    }
            //}
        } // if( plg.size() > 0 )
    } // if( m_pDrawingScene->getMode() == EMode::Edit && (m_bIsHit || isSelected()) )

    i_pPainter->restore();

} // paint
