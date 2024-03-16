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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryAngle.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysIdxTree.h"
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
class CGraphObjLabelGeometryAngle : public CGraphObjLabel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjLabelGeometryAngle::CGraphObjLabelGeometryAngle(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strKey,
    const SGraphObjSelectionPoint& i_selPt1,
    const SGraphObjSelectionPoint& i_selPt2) :
//------------------------------------------------------------------------------
    CGraphObjLabel(
        /* pDrawingScene */ i_pDrawingScene,
        /* strKey        */ i_strKey,
        /* strText       */ "Angle",
        /* type          */ EGraphObjTypeLabelGeometryAngle,
        /* selPt1        */ i_selPt1,
        /* selPt2        */ i_selPt2),
    m_fAnchorLine2Angle_degrees(0.0),
    m_rectAnchorLine2CircleSegment(),
    m_drawSettingsArrowHeads(EGraphObjTypeLine),
    m_plgP2ArrowHead()
{
    createTraceAdminObjs("Labels::" + ClassName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Key: " + i_strKey +
            ", SelPt1 {" + i_selPt1.toString(true) + "}" +
            ", SelPt2 {" + i_selPt2.toString(true) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettingsArrowHeads.setLineEndStyle(ELinePoint::Start, ELineEndStyle::ArrowHead);
    m_drawSettingsArrowHeads.setArrowHeadBaseLineType(ELinePoint::Start, EArrowHeadBaseLineType::Normal);
    m_drawSettingsArrowHeads.setArrowHeadFillStyle(ELinePoint::Start, EArrowHeadFillStyle::SolidPattern);
    m_drawSettingsArrowHeads.setArrowHeadWidth(ELinePoint::Start, EArrowHeadWidth::Thin);
    m_drawSettingsArrowHeads.setArrowHeadLength(ELinePoint::Start, EArrowHeadLength::Short);

    m_drawSettingsArrowHeads.setLineEndStyle(ELinePoint::End, ELineEndStyle::ArrowHead);
    m_drawSettingsArrowHeads.setArrowHeadBaseLineType(ELinePoint::End, EArrowHeadBaseLineType::Normal);
    m_drawSettingsArrowHeads.setArrowHeadFillStyle(ELinePoint::End, EArrowHeadFillStyle::SolidPattern);
    m_drawSettingsArrowHeads.setArrowHeadWidth(ELinePoint::End, EArrowHeadWidth::Thin);
    m_drawSettingsArrowHeads.setArrowHeadLength(ELinePoint::End, EArrowHeadLength::Short);

    m_anchorLines.append(QLineF());
    m_anchorLines.append(QLineF());

} // ctor

//------------------------------------------------------------------------------
CGraphObjLabelGeometryAngle::~CGraphObjLabelGeometryAngle()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    emit_aboutToBeDestroyed();

    //m_rectAnchorLine2CircleSegment;
    m_fAnchorLine2Angle_degrees = 0.0;
    //m_drawSettingsArrowHeads;
    //m_plgP2ArrowHead;

} // dtor

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the type method of QGraphicsItem.
*/
int CGraphObjLabelGeometryAngle::type() const
//------------------------------------------------------------------------------
{
    return EGraphObjTypeLabelGeometryAngle;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjLabelGeometryAngle::clone()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    CGraphObjLabelGeometryAngle* pGraphObj = nullptr;
    return pGraphObj;
}

/*==============================================================================
public: // must overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the bounding rectangle for the label.

    This method is called by the graphics scene to detect the area to be updated
    and for some other reasons (dispatching mouse events, ...).

    If the label is hit, selected or if the anchor line to the linked grapical
    object is visible the area to be updated on changing the items graphical
    representation also includes the anchor line.

    To get the rectangle around the labels text the base implementation of
    QGraphicsSimpleTextItem need to be called directly.

    @return Bounding rectangle.
*/
QRectF CGraphObjLabelGeometryAngle::boundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ "" );

    QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();

    // If the object is hit and the anchor line is visible also this area need to be updated.
    if (/*m_bIsHit ||*/ isSelected() || m_labelDscr.m_bShowAnchorLine) {
        for (const QLineF& anchorLine : m_anchorLines) {
            QRectF rctBoundingAnchorLine(anchorLine.p1(), anchorLine.p2());
            rctBounding |= rctBoundingAnchorLine;
        }
    }
    rctBounding |= m_rectAnchorLine2CircleSegment;
    if (m_plgP2ArrowHead.size() > 0) {
        rctBounding |= m_plgP2ArrowHead.boundingRect();
    }
    rctBounding = QRectF(
        rctBounding.left() - m_drawSettings.getPenWidth()/2,
        rctBounding.top() - m_drawSettings.getPenWidth()/2,
        rctBounding.width() + m_drawSettings.getPenWidth(),
        rctBounding.height() + m_drawSettings.getPenWidth() );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qRect2Str(rctBounding));
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
/*! @brief Called internally by QGraphicsItem::boundingRect.
*/
QPainterPath CGraphObjLabelGeometryAngle::shape() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "shape",
        /* strAddInfo   */ "" );

    QPainterPath painterPath = QGraphicsSimpleTextItem::shape();
    if (m_bIsHit || isSelected() || m_labelDscr.m_bShowAnchorLine) {
        for (const QLineF& anchorLine : m_anchorLines) {
            painterPath.addPolygon(ZS::Draw::line2Polygon(anchorLine));
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        const QGraphicsItem* pCThis = static_cast<const QGraphicsItem*>(this);
        QGraphicsItem* pVThis = const_cast<QGraphicsItem*>(pCThis);
        QString strMthRet = qPainterPath2Str(pVThis, painterPath);
        mthTracer.setMethodReturn(strMthRet);
    }
    return painterPath;
}

//------------------------------------------------------------------------------
void CGraphObjLabelGeometryAngle::paint(
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "paint",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjLabel::paint(i_pPainter, i_pStyleOption, i_pWdgt);

    // Draw anchor line 2 (circle segment) to indicate the angle if the label is hit,
    // hovered, selected or if the anchor line is set to be visible.
    // Use a different color if the line is hit, hovered or selected.
    if (/*m_bIsHit ||*/ isSelected() || m_labelDscr.m_bShowAnchorLine) {
        i_pPainter->save();
        i_pPainter->setRenderHint(QPainter::Antialiasing);
        QPen pn = pen();
        pn.setWidth(1);
        pn.setStyle(Qt::SolidLine);
        if (fabs(m_rectAnchorLine2CircleSegment.width()) >= 20.0) {
            QColor color = /*m_bIsHit ||*/ isSelected() ? Qt::blue : Qt::lightGray;
            color.setAlpha(192);
            pn.setColor(color);
            i_pPainter->setPen(pn);
            i_pPainter->drawPie(m_rectAnchorLine2CircleSegment, 0.0, 16.0 * m_fAnchorLine2Angle_degrees);
            QBrush brsh(pn.color());
            i_pPainter->setBrush(brsh);
            i_pPainter->drawPolygon(m_plgP2ArrowHead);
        }
        i_pPainter->restore();
    }
} // paint

/*==============================================================================
protected: // auxiliary overridable instance methods of base class CGraphObjLabel
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief 
*/
void CGraphObjLabelGeometryAngle::updatePosition()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updatePosition",
        /* strAddInfo   */ "" );

    QPointF pt1SelScenePosParent;
    if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::BoundingRectangle) {
        pt1SelScenePosParent = m_labelDscr.m_selPt1.m_pGraphObj->getSelectionPointCoorsInSceneCoors(m_labelDscr.m_selPt1.m_selPt);
    }
    else if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::PolygonShapePoint) {
        pt1SelScenePosParent = m_labelDscr.m_selPt1.m_pGraphObj->getSelectionPointCoorsInSceneCoors(m_labelDscr.m_selPt1.m_idxPt);
    }

    QPointF pt2SelScenePosParent;
    if (m_labelDscr.m_selPt2.m_selPtType == ESelectionPointType::BoundingRectangle) {
        pt2SelScenePosParent = m_labelDscr.m_selPt2.m_pGraphObj->getSelectionPointCoorsInSceneCoors(m_labelDscr.m_selPt2.m_selPt);
    }
    else if (m_labelDscr.m_selPt2.m_selPtType == ESelectionPointType::PolygonShapePoint) {
        pt2SelScenePosParent = m_labelDscr.m_selPt2.m_pGraphObj->getSelectionPointCoorsInSceneCoors(m_labelDscr.m_selPt2.m_idxPt);
    }

    QLineF lineSelPtSceneCoors(pt1SelScenePosParent, pt2SelScenePosParent);
    QString strText = CPhysVal(lineSelPtSceneCoors.angle(), Units.Angle.Degree, 0.1).toString();
    if (QGraphicsSimpleTextItem::text() != strText) {
        QGraphicsSimpleTextItem::setText(strText);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }

    // First set the text at the graphics item so that the
    // bounding rectangle includes the new text.
    m_bUpdatePositionInProgress = true;

    // Get anchor line in scene coordinates.
    // The start point of the anchor line should be the center point of the line
    // for which the angle has to be indicated.
    QLineF lineFPolarBase(lineSelPtSceneCoors.center(), lineSelPtSceneCoors.p2());
    QLineF anchorLine = ZS::Draw::getLineFromPolar(
        m_labelDscr.m_polarCoorsToLinkedSelPt.m_fLength_px,
        m_labelDscr.m_polarCoorsToLinkedSelPt.m_fAngle_degrees,
        lineFPolarBase);

    // The position of a QGraphicsTextItem is defined by its top left corner.
    // Move text item so that its center point is at the line end point of the anchor line.
    QRectF rctBoundingThis = getBoundingRect();
    QPointF anchorLineP2ScenePos = anchorLine.p2() - rctBoundingThis.center();
    setPos(anchorLineP2ScenePos);

    // Please note that on calling setPos the itemChange method of the
    // label is called invoking updateAnchorLines.

    m_bUpdatePositionInProgress = false;
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the relative position in polar
           coordinates (length in pixels, angle in degress) from the labels center
           point and the selection point the label is linked to.

    On moving the label the distance and the angle (polar coordinates) to the
    selection point have to be updated and saved.

    For Length labels the linked selection point is always the center point
    of the two shape points the label is linked to.

    Example for a vertical line:

                        P2
                        |
                        |  Label
                        |  /
                        | / Angle (to Label, not angle value to be indicated)
                        x--------
                        |
                        |
                        |
                        |
                        P1
*/
void CGraphObjLabelGeometryAngle::updatePolarCoorsToLinkedSelPt()
//------------------------------------------------------------------------------
{
    // If the position is updated because the parent's geometry is changed,
    // the relative distance in polar coordinates (length and angle) to the
    // linked selection point must not be changed.
    if (m_bPositionUpdateOnParentGeometryChanged) {
        return;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updatePolarCoorsToLinkedSelPt",
        /* strAddInfo   */ "" );

    // Get anchor line in scene coordinates.
    // The start point of the anchor line should be the center point of the line
    // for which the length has to be indicated.
    QPointF pt1SelScenePosParent;
    if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::BoundingRectangle) {
        pt1SelScenePosParent = m_labelDscr.m_selPt1.m_pGraphObj->getSelectionPointCoorsInSceneCoors(m_labelDscr.m_selPt1.m_selPt);
    }
    else if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::PolygonShapePoint) {
        pt1SelScenePosParent = m_labelDscr.m_selPt1.m_pGraphObj->getSelectionPointCoorsInSceneCoors(m_labelDscr.m_selPt1.m_idxPt);
    }

    QPointF pt2SelScenePosParent;
    if (m_labelDscr.m_selPt2.m_selPtType == ESelectionPointType::BoundingRectangle) {
        pt2SelScenePosParent = m_labelDscr.m_selPt2.m_pGraphObj->getSelectionPointCoorsInSceneCoors(m_labelDscr.m_selPt2.m_selPt);
    }
    else if (m_labelDscr.m_selPt2.m_selPtType == ESelectionPointType::PolygonShapePoint) {
        pt2SelScenePosParent = m_labelDscr.m_selPt2.m_pGraphObj->getSelectionPointCoorsInSceneCoors(m_labelDscr.m_selPt2.m_idxPt);
    }

    QLineF lineSelPtSceneCoors(pt1SelScenePosParent, pt2SelScenePosParent);
    QPointF ptSelPtSceneCoors = lineSelPtSceneCoors.center();

    QRectF rctBoundingThis = getBoundingRect();
    QPointF ptCenterScenePosThis = mapToScene(rctBoundingThis.center());

    // The start point of the anchor line should be the center point of the line
    // for which the length has to be indicated.
    QLineF lineFromSelPtSceneCoors(ptSelPtSceneCoors, ptCenterScenePosThis);

    m_labelDscr.m_polarCoorsToLinkedSelPt.m_fLength_px = lineFromSelPtSceneCoors.length();
    m_labelDscr.m_polarCoorsToLinkedSelPt.m_fAngle_degrees = lineSelPtSceneCoors.angleTo(lineFromSelPtSceneCoors);

    // Update coordinates of the anchor line.
    updateAnchorLines();
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the coordinates (start and end point)
           of the anchor lines.

    The anchor lines will be drawn to one of the selection points at the bounding rectangle
    or to one of the polygon shape points of the linked graphical object.

    For the Angle label three anchor lines are drawn:
    
    - An anchor line to indicate the horiuontal 0° degree line.
    - A line from the center point of the line between the two selection points the label
      is linked to and in the angle to be indicated.
    - A circle segment between the two end points of the anchor lines above.
      The radius for the circle segment is the distance of the label to the center point
      of the line between the two selection points the label is linked to and in the angle
      to be indicated

    Example for a vertical line:

                        P2 (SelPt2)
                        +
                        |
                Anchor  |<--    AnchorLine2 (circle segment)
                Line1   |  90\°
                (end of |     |
                circle  x----------- AnchorLine0 (0° line)
                segment)|
                        |
                        |
                        |
                        +
                        P1 (SelPt1)

    Example for a Rectangle (rotated by 90°):

                 TopRight          BottomRight
                    +---------x---------+
                    |      SelPt2       |
                    | Anchor  |<---     |
                    | Line1   |  90\° AnchorLine2 (circle segment)
                    | (end of |     |   |
                    x circle  x---------x--AnchorLine0 (horizontal Line for 0°)
                    | segment)          |
                    |                   |
                    |                   |
                    |      SelPt1       |
                    +---------x---------+
                 TopLeft           BottomLeft
*/
void CGraphObjLabelGeometryAngle::updateAnchorLines()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateAnchorLines",
        /* strAddInfo   */ "" );

    QPointF pt1SelScenePosParent;
    if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::BoundingRectangle) {
        pt1SelScenePosParent = m_labelDscr.m_selPt1.m_pGraphObj->getSelectionPointCoorsInSceneCoors(m_labelDscr.m_selPt1.m_selPt);
    }
    else if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::PolygonShapePoint) {
        pt1SelScenePosParent = m_labelDscr.m_selPt1.m_pGraphObj->getSelectionPointCoorsInSceneCoors(m_labelDscr.m_selPt1.m_idxPt);
    }

    QPointF pt2SelScenePosParent;
    if (m_labelDscr.m_selPt2.m_selPtType == ESelectionPointType::BoundingRectangle) {
        pt2SelScenePosParent = m_labelDscr.m_selPt2.m_pGraphObj->getSelectionPointCoorsInSceneCoors(m_labelDscr.m_selPt2.m_selPt);
    }
    else if (m_labelDscr.m_selPt2.m_selPtType == ESelectionPointType::PolygonShapePoint) {
        pt2SelScenePosParent = m_labelDscr.m_selPt2.m_pGraphObj->getSelectionPointCoorsInSceneCoors(m_labelDscr.m_selPt2.m_idxPt);
    }

    QLineF lineSelPtSceneCoors(pt1SelScenePosParent, pt2SelScenePosParent);
    QPointF ptCenterLineSelPtScenePos = lineSelPtSceneCoors.center();
    QLineF lineSelPt = QLineF(mapFromScene(lineSelPtSceneCoors.p1()), mapFromScene(lineSelPtSceneCoors.p2()));
    QPointF ptCenterLineSelPtPos = lineSelPt.center();

    QRectF rctBoundingThis = getBoundingRect();
    QPointF ptCenterThis = rctBoundingThis.center();

    // AnchorLine0 (horizontal Line for 0°) in local coordinates
    QPointF ptHorLine0DegreesP1(ptCenterLineSelPtPos.x(), ptCenterLineSelPtPos.y());
    QPointF ptHorLine0DegreesP2(ptCenterLineSelPtPos.x() + m_labelDscr.m_polarCoorsToLinkedSelPt.m_fLength_px, ptCenterLineSelPtPos.y());
    m_anchorLines[0] = QLineF(ptHorLine0DegreesP1, ptHorLine0DegreesP2);

    // AnchorLine1 (end of circle segment) in local coordinates
    QLineF lineEndOfSegment(ptCenterLineSelPtPos, mapFromScene(lineSelPtSceneCoors.p2()));
    lineEndOfSegment.setLength(m_labelDscr.m_polarCoorsToLinkedSelPt.m_fLength_px);
    m_anchorLines[1] = lineEndOfSegment;

    // AnchorLine1 (circle segment) in local coordinates
    m_rectAnchorLine2CircleSegment.setWidth(2.0 * m_labelDscr.m_polarCoorsToLinkedSelPt.m_fLength_px);
    m_rectAnchorLine2CircleSegment.setHeight(2.0 * m_labelDscr.m_polarCoorsToLinkedSelPt.m_fLength_px);
    m_rectAnchorLine2CircleSegment.moveCenter(ptCenterLineSelPtPos);
    m_fAnchorLine2Angle_degrees = lineSelPtSceneCoors.angle();

    QLineF perpendicularLineHorLine =
        QLineF(ptHorLine0DegreesP2, QPointF(ptHorLine0DegreesP2.x(), ptHorLine0DegreesP2.y() - 20.0));
    QLineF perpendicularLineEndOfSegment = ZS::Draw::getPerpendicularLine(
        QLineF(lineSelPt.p2(), lineSelPt.p1()), lineEndOfSegment.p2(), 10.0);

    getLineEndPolygons(
        /* line          */ perpendicularLineEndOfSegment,
        /* drawSetings   */ m_drawSettingsArrowHeads,
        /* pplgLineStart */ &m_plgP2ArrowHead,
        /* pplgLineEnd   */ nullptr);
}
