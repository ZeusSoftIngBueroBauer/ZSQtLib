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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryHeight.h"
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
class CGraphObjLabelGeometryHeight : public CGraphObjLabel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjLabelGeometryHeight::CGraphObjLabelGeometryHeight(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strKey,
    const SGraphObjSelectionPoint& i_selPt1,
    const SGraphObjSelectionPoint& i_selPt2) :
//------------------------------------------------------------------------------
    CGraphObjLabel(
        /* pDrawingScene   */ i_pDrawingScene,
        /* strKey          */ i_strKey,
        /* strText         */ "Height",
        /* type            */ EGraphObjTypeLabelGeometryHeight,
        /* selPt           */ i_selPt1,
        /* selPt           */ i_selPt2),
    m_drawSettingsArrowHeads(EGraphObjTypeLine),
    m_plgP1ArrowHead(),
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
    m_anchorLines.append(QLineF());

} // ctor

//------------------------------------------------------------------------------
CGraphObjLabelGeometryHeight::~CGraphObjLabelGeometryHeight()
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

    //m_drawSettingsArrowHeads;
    //m_plgP1ArrowHead;
    //m_plgP2ArrowHead;

} // dtor

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the type method of QGraphicsItem.
*/
int CGraphObjLabelGeometryHeight::type() const
//------------------------------------------------------------------------------
{
    return QGraphicsItem::UserType + EGraphObjTypeLabelGeometryHeight;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjLabelGeometryHeight::clone()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    CGraphObjLabelGeometryHeight* pGraphObj = nullptr;
    return pGraphObj;
}

/*==============================================================================
public: // must overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabelGeometryHeight::paint(
    QPainter* i_pPainter,
    const QStyleOptionGraphicsItem* i_pStyleOption,
    QWidget* i_pWdgt )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "paint",
        /* strAddInfo   */ "" );
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer);
    }

    CGraphObjLabel::paint(i_pPainter, i_pStyleOption, i_pWdgt);

    if (m_bIsHit || isSelected() || m_labelDscr.m_bShowAnchorLine) {
        i_pPainter->save();
        i_pPainter->setRenderHint(QPainter::Antialiasing);
        QPen pn = pen();
        pn.setWidth(1);
        pn.setStyle(Qt::SolidLine);
        // P1 of the anchor line is on one of the center points of the
        // bounding rectangle of this label.
        // P2 is on the selection point of the linked parent item.
        // If P2 is within the bounding rectangle or very close to the label
        // (length of line < 5.0 pixels) the anchor line will not be drawn.
        QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();
        const QLineF& anchorLine = m_anchorLines[2];
         if (!rctBounding.contains(anchorLine.p2())) {
             if ((fabs(anchorLine.dx()) >= 5.0) || (fabs(anchorLine.dy()) >= 5.0)) {
                if (m_bIsHit || isSelected()) {
                    pn.setColor(Qt::blue);
                }
                else {
                    pn.setColor(Qt::gray);
                }
                //pn.setStyle(Qt::DotLine);
                i_pPainter->setPen(pn);
                QBrush brsh(pn.color());
                i_pPainter->setBrush(brsh);
                i_pPainter->drawPolygon(m_plgP1ArrowHead);
                i_pPainter->drawPolygon(m_plgP2ArrowHead);
            }
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
void CGraphObjLabelGeometryHeight::updatePosition()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updatePosition",
        /* strAddInfo   */ "" );

    CPhysValPoint physValSelPoint1Parent;
    if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::BoundingRectangle) {
        physValSelPoint1Parent = m_labelDscr.m_selPt1.m_pGraphObj->getSelectionPointCoors(m_labelDscr.m_selPt1.m_selPt);
    }
    else if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::PolygonShapePoint) {
        physValSelPoint1Parent = m_labelDscr.m_selPt1.m_pGraphObj->getSelectionPointCoors(m_labelDscr.m_selPt1.m_idxPt);
    }

    CPhysValPoint physValSelPoint2Parent;
    if (m_labelDscr.m_selPt2.m_selPtType == ESelectionPointType::BoundingRectangle) {
        physValSelPoint2Parent = m_labelDscr.m_selPt2.m_pGraphObj->getSelectionPointCoors(m_labelDscr.m_selPt2.m_selPt);
    }
    else if (m_labelDscr.m_selPt2.m_selPtType == ESelectionPointType::PolygonShapePoint) {
        physValSelPoint2Parent = m_labelDscr.m_selPt2.m_pGraphObj->getSelectionPointCoors(m_labelDscr.m_selPt2.m_idxPt);
    }

    CPhysValLine physValLine(physValSelPoint1Parent, physValSelPoint2Parent);
    QString strText = physValLine.height().toString(EUnitFind::None, PhysValSubStr::Val);
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
    // for which the length has to be indicated.
    CPhysValLine physValLinePolarBase(physValLine.center(), physValLine.p2());
    QLineF lineSelPtSceneCoors = m_pDrawingScene->convert(physValLinePolarBase, Units.Length.px).toQLineF();
    QLineF anchorLine = ZS::Draw::getLineFromPolar(
        m_labelDscr.m_polarCoorsToLinkedSelPt.m_fLength_px,
        m_labelDscr.m_polarCoorsToLinkedSelPt.m_fAngle_degrees,
        lineSelPtSceneCoors);

    // The position of a QGraphicsTextItem is defined by its top left corner.
    QRectF rctBoundingThis = QGraphicsSimpleTextItem::boundingRect();
    QPointF anchorLineP2ScenePos = anchorLine.p2() - rctBoundingThis.center();
    setPos(anchorLineP2ScenePos);

    // Update coordinates of the anchor line.
    updateAnchorLines();

    m_bUpdatePositionInProgress = false;

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave,
            m_pTrcAdminObjItemChange->getRuntimeInfoTraceDetailLevel());
    }
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the relative position in polar
           coordinates (length in pixels, angle in degress) from the labels center
           point and the selection point the label is linked to.

    On moving the label the distance and the angle (polar coordinates) to the
    selection point have to be updated and saved.

    For Height labels the linked selection point is always the center point
    of the two shape points the label is linked to.

    Example for a horizontal line:

                        Label
                           \
                            \ Angle
         P1 +----------------x----------------+ P2
                          Center
*/
void CGraphObjLabelGeometryHeight::updatePolarCoorsToLinkedSelPt()
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
    CPhysValPoint physValSelPoint1Parent;
    if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::BoundingRectangle) {
        physValSelPoint1Parent = m_labelDscr.m_selPt1.m_pGraphObj->getSelectionPointCoors(m_labelDscr.m_selPt1.m_selPt);
    }
    else if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::PolygonShapePoint) {
        physValSelPoint1Parent = m_labelDscr.m_selPt1.m_pGraphObj->getSelectionPointCoors(m_labelDscr.m_selPt1.m_idxPt);
    }

    CPhysValPoint physValSelPoint2Parent;
    if (m_labelDscr.m_selPt2.m_selPtType == ESelectionPointType::BoundingRectangle) {
        physValSelPoint2Parent = m_labelDscr.m_selPt2.m_pGraphObj->getSelectionPointCoors(m_labelDscr.m_selPt2.m_selPt);
    }
    else if (m_labelDscr.m_selPt2.m_selPtType == ESelectionPointType::PolygonShapePoint) {
        physValSelPoint2Parent = m_labelDscr.m_selPt2.m_pGraphObj->getSelectionPointCoors(m_labelDscr.m_selPt2.m_idxPt);
    }

    CPhysValLine physValLine(physValSelPoint1Parent, physValSelPoint2Parent);
    QLineF lineSelPtSceneCoors = m_pDrawingScene->convert(physValLine, Units.Length.px).toQLineF();
    QPointF ptSelPtSceneCoors = lineSelPtSceneCoors.center();

    QRectF rctBoundingThis = QGraphicsSimpleTextItem::boundingRect();
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

    For the Height label three anchor lines are drawn. Two hjorizontal lines through
    selection point 1 and selection point 2 and a vertical between the end points
    of the vertical lines.

    Example for a diagonal line:

             P1
              +-----------------+ AnchorLine0
               \                |
                \               |
                 \              |
                  x        AnchorLine2
                   \            |
                    \           |
                     \          |
                      +---------+ AnchorLine1
                      P2
*/
void CGraphObjLabelGeometryHeight::updateAnchorLines()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateAnchorLines",
        /* strAddInfo   */ "" );

    CPhysValPoint physValSelPoint1Parent;
    if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::BoundingRectangle) {
        physValSelPoint1Parent = m_labelDscr.m_selPt1.m_pGraphObj->getSelectionPointCoors(m_labelDscr.m_selPt1.m_selPt);
    }
    else if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::PolygonShapePoint) {
        physValSelPoint1Parent = m_labelDscr.m_selPt1.m_pGraphObj->getSelectionPointCoors(m_labelDscr.m_selPt1.m_idxPt);
    }
    CPhysValPoint physValSelPoint2Parent;
    if (m_labelDscr.m_selPt2.m_selPtType == ESelectionPointType::BoundingRectangle) {
        physValSelPoint2Parent = m_labelDscr.m_selPt2.m_pGraphObj->getSelectionPointCoors(m_labelDscr.m_selPt2.m_selPt);
    }
    else if (m_labelDscr.m_selPt2.m_selPtType == ESelectionPointType::PolygonShapePoint) {
        physValSelPoint2Parent = m_labelDscr.m_selPt2.m_pGraphObj->getSelectionPointCoors(m_labelDscr.m_selPt2.m_idxPt);
    }

    CPhysValLine physValLine = CPhysValLine(physValSelPoint1Parent, physValSelPoint2Parent);
    QLineF lineSelPtSceneCoors = m_pDrawingScene->convert(physValLine, Units.Length.px).toQLineF();

    // The distances to the parent's selection points must be calculated in scene coordinates.
    QRectF rctBoundingThis = QGraphicsSimpleTextItem::boundingRect();
    QPointF ptCenterScenePosThis = mapToScene(rctBoundingThis.center());

    // Horizontal lines.
    QPointF ptLineP1End(ptCenterScenePosThis.x(), lineSelPtSceneCoors.p1().y());
    QLineF lineP1(lineSelPtSceneCoors.p1(), ptLineP1End);
    QPointF ptLineP2End(ptCenterScenePosThis.x(), lineSelPtSceneCoors.p2().y());
    QLineF lineP2(lineSelPtSceneCoors.p2(), ptLineP2End);

    // As the anchor lines are painted by this object, they have to be translated into local coordinates.
    m_anchorLines[0] = QLineF(mapFromScene(lineP1.p1()), mapFromScene(lineP1.p2()));
    m_anchorLines[1] = QLineF(mapFromScene(lineP2.p1()), mapFromScene(lineP2.p2()));
    m_anchorLines[2] = QLineF(mapFromScene(lineP1.p2()), mapFromScene(lineP2.p2()));

    getLineEndPolygons(
        /* line          */ m_anchorLines[2],
        /* drawSetings   */ m_drawSettingsArrowHeads,
        /* pplgLineStart */ &m_plgP1ArrowHead,
        /* pplgLineEnd   */ &m_plgP2ArrowHead);
}
