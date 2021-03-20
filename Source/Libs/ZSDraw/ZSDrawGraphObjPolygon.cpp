/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSDraw/ZSDrawGraphObjPolygon.h"
#include "ZSDraw/ZSDrawGraphObjLabel.h"
#include "ZSDraw/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
//#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;
using namespace ZS::Trace;


/*******************************************************************************
class CGraphObjPolygon : public CGraphObj, public QGraphicsPolygonItem
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjPolygon::ResetCtorsDtorsCounters()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjCtorsAndDtor != nullptr && s_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "CtorsCount: " + QString::number(s_iCtorsCount);
        strAddTrcInfo += ", DtorsCount: " + QString::number(s_iDtorsCount);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ "",
        /* strMethod    */ "ResetCtorsDtorsCounters",
        /* strAddInfo   */ strAddTrcInfo );

    if( s_iCtorsCount != s_iDtorsCount )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "CtorsCount(=" + QString::number(s_iCtorsCount) + ") != DtorsCount(=" + QString::number(s_iDtorsCount) + ")" );
    }

    s_iCtorsCount = 1;
    s_iDtorsCount = 1;

} // ResetCtorsDtorsCounters

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjPolygon::s_iCtorsCount = 1;
qint64 CGraphObjPolygon::s_iDtorsCount = 1;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjPolygon::CGraphObjPolygon(
    CDrawingScene*       i_pDrawingScene,
    const CDrawSettings& i_drawSettings,
    const QString&       i_strObjName,
    const QString&       i_strObjId ) :
//------------------------------------------------------------------------------
    CGraphObjPolyline(
        /* pDrawingScene */ i_pDrawingScene,
        /* strNameSpace  */ "ZS::Draw",
        /* strClassName  */ "CGraphObjPolygon",
        /* type          */ EGraphObjTypePolygon,
        /* strType       */ ZS::Draw::graphObjType2Str(EGraphObjTypePolygon),
        /* strObjName    */ i_strObjName.isEmpty() ? "Polygon" + QString::number(s_iCtorsCount) : i_strObjName,
        /* strObjId      */ i_strObjId.isEmpty() ? "Polygon" + QString::number(s_iCtorsCount) : i_strObjId,
        /* drawSettings  */ i_drawSettings )
{
    s_iCtorsCount++;

    QString strAddTrcInfo;

    if( s_pTrcAdminObjCtorsAndDtor != nullptr && s_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strAddTrcInfo );

    QPolygonF plg = polygon();
    QRectF    rctBounding = plg.boundingRect();

    m_ptRotOriginCurr = rctBounding.center();

    setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges );

    onDrawSettingsChanged();

    updateToolTip();

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        mthTracer.trace(strAddTrcInfo);
    }

} // ctor

//------------------------------------------------------------------------------
CGraphObjPolygon::~CGraphObjPolygon()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    s_iDtorsCount++;

} // dtor

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjPolygon::clone()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjCtorsAndDtor != nullptr && s_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "clone",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjPolygon* pGraphObj = new CGraphObjPolygon(m_pDrawingScene,m_drawSettings);

    pGraphObj->setObjName(m_strObjName);
    pGraphObj->setPolygon( polygon() );
    pGraphObj->setPos( pos() );
    pGraphObj->setRotationAngleInDegree(m_fRotAngleCurr_deg);
    //pGraphObj->setScaleFactors(m_fScaleFacXCurr,m_fScaleFacYCurr);
    pGraphObj->acceptCurrentAsOriginalCoors();

    return pGraphObj;

} // clone

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjPolygon::onDrawSettingsChanged()
//------------------------------------------------------------------------------
{
    if( m_drawSettings.isPenUsed() )
    {
        if( m_drawSettings.getLineStyle() != ELineStyleNoLine )
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
        if( m_drawSettings.getFillStyle() != EFillStyleNoFill )
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

    if( s_pTrcAdminObjBoundingRect != nullptr && s_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Point:" + point2Str(i_pt);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
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
            bIsHit = isPolygonHit( plg, EFillStyleNoFill, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo );
        }

        if( !bIsHit )
        {
            // In contrary to polylines for polygons we also have to check "contains" for solid fill styles.
            if( pGraphicsItem->isSelected() || m_drawSettings.getFillStyle() == EFillStyleSolidPattern )
            {
                bIsHit = pGraphicsItem->contains(i_pt);

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditModeMove;
                    o_pHitInfo->m_editResizeMode = EEditResizeModeUndefined;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPointUndefined;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                }
            }
        }

        if( bIsHit && o_pHitInfo != nullptr )
        {
            o_pHitInfo->setCursor( Math::deg2Rad(m_fRotAngleCurr_deg) );
        }

    } // if( pGraphicsItem != nullptr )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "IsHit:" + bool2Str(bIsHit);

        if( o_pHitInfo != nullptr )
        {
            strAddTrcInfo += ", EditMode:" + editMode2Str(o_pHitInfo->m_editMode);
            strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(o_pHitInfo->m_editResizeMode);
            strAddTrcInfo += ", SelPtBoundingRect:" + selectionPoint2Str(o_pHitInfo->m_selPtBoundingRect);
            strAddTrcInfo += ", PolygonShapePoint:" + QString::number(o_pHitInfo->m_idxPolygonShapePoint);
            strAddTrcInfo += ", LineSegment:" + QString::number(o_pHitInfo->m_idxLineSegment);
            strAddTrcInfo += ", PointSelected:" + point2Str(o_pHitInfo->m_ptSelected);
            strAddTrcInfo += ", Cursor:" + qCursorShape2Str(o_pHitInfo->m_cursor.shape());
        }
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return bIsHit;

} // isHit

/*==============================================================================
public: // overridables of base class QGraphicsPolygonItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjPolygon::boundingRect() const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjBoundingRect != nullptr && s_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
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

    for( idxSelPt = 0; idxSelPt < ESelectionPointCount; idxSelPt++ )
    {
        selPt = static_cast<ESelectionPoint>(idxSelPt);

        pGraphObjSelPt = m_arpSelPtsBoundingRect[selPt];

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

    if( m_bIsHit || isSelected() )
    {
        QHashIterator<QString,SGraphObjLabel*> itLabels(m_arpLabels);
        SGraphObjLabel* pGraphObjLabel;
        QRectF          rctLabel;
        QPolygonF       plgLabel;

        while( itLabels.hasNext() )
        {
            itLabels.next();
            pGraphObjLabel = itLabels.value();

            if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
            {
                // Calling pGraphObjLabel->boundingRect() may lead to endless recursion as the
                // label itself may call the boundingRect method of its parent item (which is
                // this item) if the label is selected or has been hit. For this we call
                // boundingRect of the label with a different signature to indicate that we
                // are only interested in the bounding rect of the simple text item.
                rctLabel = pGraphObjLabel->m_pGraphObjLabel->boundingRect(true);
                plgLabel = mapFromItem( pGraphObjLabel->m_pGraphObjLabel, rctLabel );
                rctBounding |= plgLabel.boundingRect();
            }
        }
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
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
    QString strAddTrcInfo;

    if( s_pTrcAdminObjPaint != nullptr && s_pTrcAdminObjPaint->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjPaint,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "paint",
        /* strAddInfo   */ strAddTrcInfo );

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
                if( m_arpSelPtsBoundingRect[ESelectionPointTopCenter] != nullptr && m_arpSelPtsBoundingRect[ESelectionPointRotateTop] != nullptr )
                {
                    CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[ESelectionPointTopCenter];
                    CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[ESelectionPointRotateTop];

                    QPointF ptRct = QPointF( pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y() );
                    QPointF ptRot = QPointF( pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y() );

                    QPointF ptRctM = mapFromScene(ptRct);
                    QPointF ptRotM = mapFromScene(ptRot);

                    i_pPainter->drawLine( ptRctM, ptRotM );
                }

                if( m_arpSelPtsBoundingRect[ESelectionPointBottomCenter] != nullptr && m_arpSelPtsBoundingRect[ESelectionPointRotateBottom] != nullptr )
                {
                    CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[ESelectionPointBottomCenter];
                    CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[ESelectionPointRotateBottom];

                    QPointF ptRct = QPointF( pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y() );
                    QPointF ptRot = QPointF( pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y() );

                    QPointF ptRctM = mapFromScene(ptRct);
                    QPointF ptRotM = mapFromScene(ptRot);

                    i_pPainter->drawLine( ptRctM, ptRotM );
                }
            } // if( isSelected() )

            QHashIterator<QString,SGraphObjLabel*> itLabels(m_arpLabels);
            SGraphObjLabel*                        pGraphObjLabel;

            QPointF ptSelPt;
            QPointF ptLabelSelPt;

            while( itLabels.hasNext() )
            {
                itLabels.next();
                pGraphObjLabel = itLabels.value();

                if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
                {
                    ptSelPt = getSelectionPoint(pGraphObjLabel->m_selPt);

                    ptLabelSelPt = pGraphObjLabel->m_pGraphObjLabel->getSelectionPoint(ESelectionPointCenter);
                    ptLabelSelPt = mapFromItem( pGraphObjLabel->m_pGraphObjLabel, ptLabelSelPt );

                    i_pPainter->drawLine( ptSelPt, ptLabelSelPt );
                }
            }

        } // if( plg.size() > 0 )

    } // if( m_pDrawingScene->getMode() == EMode::Edit && (m_bIsHit || isSelected()) )

    i_pPainter->restore();

} // paint
