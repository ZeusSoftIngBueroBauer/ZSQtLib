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

    createTraceAdminObjs("StandardShapes::" + ClassName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ObjName: " + i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    initInstance();
}

//------------------------------------------------------------------------------
CGraphObjPolygon::~CGraphObjPolygon()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_bDtorInProgress = true;
    emit_aboutToBeDestroyed();
}

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
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    CGraphObjPolygon* pGraphObj = new CGraphObjPolygon(m_pDrawingScene, m_strName);
    pGraphObj->setDrawSettings(m_drawSettings);
    pGraphObj->setPolygon( getPolygon() );
    pGraphObj->setPos( pos() );
    return pGraphObj;
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
//bool CGraphObjPolygon::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjIsHit, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "Point:" + point2Str(i_pt) +
//            ", HitInfo, " + QString(o_pHitInfo == nullptr ? "null" : pointer2Str(o_pHitInfo)) +
//            ", Polygon:" + polygon2Str(polygon());
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjIsHit,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "isHit",
//        /* strAddInfo   */ strMthInArgs );
//
//    bool bIsHit = false;
//
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//
//    if( pGraphicsItem != nullptr )
//    {
//        if( pGraphicsItem->isSelected() )
//        {
//            bIsHit = isPolygonSelectionPointHit(i_pt,o_pHitInfo);
//
//            if( !bIsHit )
//            {
//                bIsHit = isBoundingRectSelectionPointHit(
//                    /* pt               */ i_pt,
//                    /* iSelPtsCount     */ -1,
//                    /* pSelPts          */ nullptr,
//                    /* pGraphObjHitInfo */ o_pHitInfo );
//            }
//        }
//
//        if( !bIsHit )
//        {
//            QPolygonF plg = polygon();
//            bIsHit = isPolygonHit( plg, EFillStyle::NoFill, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo );
//        }
//
//        if( !bIsHit )
//        {
//            // In contrary to polylines for polygons we also have to check "contains" for solid fill styles.
//            if( pGraphicsItem->isSelected() || m_drawSettings.getFillStyle() == EFillStyle::SolidPattern )
//            {
//                bIsHit = pGraphicsItem->contains(i_pt);
//
//                if( o_pHitInfo != nullptr )
//                {
//                    //o_pHitInfo->m_editMode = EEditMode::Move;
//                    //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
//                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
//                    o_pHitInfo->m_idxPolygonShapePoint = -1;
//                    o_pHitInfo->m_idxLineSegment = -1;
//                    o_pHitInfo->m_ptSelected = i_pt;
//                }
//            }
//        }
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//        if( bIsHit && o_pHitInfo != nullptr )
//        {
//            o_pHitInfo->setCursor( Math::degree2Rad(m_fRotAngleCurr_deg) );
//        }
//#endif
//
//    } // if( pGraphicsItem != nullptr )
//
//    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
//        QString strMthOutArgs;
//        if (o_pHitInfo != nullptr) {
//            strMthOutArgs = "HitInfo {" + o_pHitInfo->toString() + "}";
//            mthTracer.setMethodOutArgs(strMthOutArgs);
//        }
//        mthTracer.setMethodReturn(bIsHit);
//    }
//
//    return bIsHit;
//
//} // isHit

/*==============================================================================
public: // reimplementing methods of base class QGraphicItem
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObjPolygon::setCursor( const QCursor& i_cursor )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal))
//    {
//        strMthInArgs = qCursorShape2Str(i_cursor.shape());
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "setCursor",
//        /* strAddInfo   */ strMthInArgs );
//
//    CGraphObjPolyline::setCursor(i_cursor);
//
//} // setCursor

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjPolygon::boundingRect() const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjBoundingRect, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Polygon {" + qPolygon2Str(polygon()) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ strMthInArgs );

    QRectF rctBounding = QGraphicsPolygonItem::boundingRect();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::paint(
    QPainter* i_pPainter,
    const QStyleOptionGraphicsItem* i_pStyleOption,
    QWidget* i_pWdgt )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjPaint, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ZValue: " + QString::number(zValue());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "paint",
        /* strAddInfo   */ strMthInArgs );

    //QGraphicsPolygonItem::paint(i_pPainter, i_pStyleOption, i_pWdgt);

    QPolygonF polygon = this->polygon();
    if (polygon.size() < 2) {
        return;
    }

    i_pPainter->save();

    QPen pn = pen();
    QBrush brsh;
    if ((m_pDrawingScene->getMode() == EMode::Edit) && (m_bIsHit || m_bIsHighlighted || isSelected())) {
        if (isSelected()) {
            pn.setColor(s_selectionColor);
            pn.setWidth(3 + m_drawSettings.getPenWidth());
        }
        else {
            pn.setColor(s_highlightColor);
            pn.setWidth(3 + m_drawSettings.getPenWidth());
        }
        pn.setStyle(Qt::SolidLine);
        QPainterPath outline;
        outline.moveTo(polygon[0]);
        for (int idxPt = 1; idxPt < polygon.size(); ++idxPt) {
            outline.lineTo(polygon[idxPt]);
        }
        outline.lineTo(polygon[0]);
        i_pPainter->strokePath(outline, pn);
        pn.setWidth(1 + m_drawSettings.getPenWidth());
    }
    pn.setColor(m_drawSettings.getPenColor());
    pn.setWidth(m_drawSettings.getPenWidth());
    pn.setStyle(lineStyle2QtPenStyle(m_drawSettings.getLineStyle().enumerator()));
    i_pPainter->setPen(pn);
    brsh.setColor(m_drawSettings.getFillColor());
    brsh.setStyle(fillStyle2QtBrushStyle(m_drawSettings.getFillStyle()));
    i_pPainter->setBrush(brsh);
    i_pPainter->setRenderHints(s_painterRenderHints);
    i_pPainter->drawPolygon(polygon);

    CEnumLineEndStyle lineEndStyleLineStart = m_drawSettings.getLineEndStyle(ELinePoint::Start);
    CEnumLineEndStyle lineEndStyleLineEnd = m_drawSettings.getLineEndStyle(ELinePoint::End);
    if (lineEndStyleLineStart != ELineEndStyle::Normal || lineEndStyleLineEnd != ELineEndStyle::Normal) {
        CEnumArrowHeadBaseLineType baseLineTypeLineStart = m_drawSettings.getArrowHeadBaseLineType(ELinePoint::Start);
        CEnumArrowHeadBaseLineType baseLineTypeLineEnd   = m_drawSettings.getArrowHeadBaseLineType(ELinePoint::End);
        pn.setWidth(1);
        pn.setStyle(Qt::SolidLine);
        i_pPainter->setPen(pn);
        if (lineEndStyleLineStart != ELineEndStyle::Normal) {
            brsh.setStyle(arrowHeadFillStyle2QtBrushStyle(m_drawSettings.getArrowHeadFillStyle(ELinePoint::Start)));
            i_pPainter->setBrush(brsh);
            if (baseLineTypeLineStart == EArrowHeadBaseLineType::NoLine) {
                i_pPainter->drawPolyline(m_plgLineStartArrowHead);
            }
            else {
                if (m_drawSettings.getArrowHeadFillStyle(ELinePoint::Start) == EArrowHeadFillStyle::NoFill) {
                    i_pPainter->setBrush(Qt::white);
                }
                else {
                    i_pPainter->setBrush(Qt::black);
                }
                i_pPainter->drawPolygon(m_plgLineStartArrowHead);
            }
        }
        if (lineEndStyleLineEnd != ELineEndStyle::Normal) {
            brsh.setStyle(arrowHeadFillStyle2QtBrushStyle(m_drawSettings.getArrowHeadFillStyle(ELinePoint::End)));
            i_pPainter->setBrush(brsh);
            if (baseLineTypeLineEnd == EArrowHeadBaseLineType::NoLine) {
                i_pPainter->drawPolyline(m_plgLineEndArrowHead);
            }
            else {
                if (m_drawSettings.getArrowHeadFillStyle(ELinePoint::End) == EArrowHeadFillStyle::NoFill) {
                    i_pPainter->setBrush(Qt::white);
                }
                else {
                    i_pPainter->setBrush(Qt::black);
                }
                i_pPainter->drawPolygon(m_plgLineEndArrowHead);
            }
        }
    }

    if ((m_pDrawingScene->getMode() == EMode::Edit) && isSelected()) {
        if (!polygon.isEmpty()) {
            QRectF rctBounding = getBoundingRect();
            pn.setColor(Qt::blue);
            pn.setStyle(Qt::DotLine);
            pn.setWidth(1);
            i_pPainter->setPen(pn);
            i_pPainter->setBrush(Qt::NoBrush);
            i_pPainter->drawRect(rctBounding);
            if (m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::TopCenter)] != nullptr
             && m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateTop)] != nullptr) {
                CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::TopCenter)];
                CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateTop)];
                QPointF ptRct = QPointF(pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y());
                QPointF ptRot = QPointF(pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y());
                QPointF ptRctM = mapFromScene(ptRct);
                QPointF ptRotM = mapFromScene(ptRot);
                i_pPainter->drawLine(ptRctM, ptRotM);
            }
            if (m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::BottomCenter)] != nullptr
             && m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateBottom)] != nullptr) {
                CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::BottomCenter)];
                CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateBottom)];
                QPointF ptRct = QPointF(pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y());
                QPointF ptRot = QPointF(pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y());
                QPointF ptRctM = mapFromScene(ptRct);
                QPointF ptRotM = mapFromScene(ptRot);
                i_pPainter->drawLine( ptRctM, ptRotM );
            }
        }
    }
    i_pPainter->restore();

} // paint
