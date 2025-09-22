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
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
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
protected: // class members
==============================================================================*/

QPainter::RenderHints CGraphObjLabelGeometryAngle::s_painterRenderHints = QPainter::Antialiasing;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QPainter::RenderHints CGraphObjLabelGeometryAngle::painterRenderHints()
//------------------------------------------------------------------------------
{
    return s_painterRenderHints;
}

//------------------------------------------------------------------------------
void CGraphObjLabelGeometryAngle::setPainterRenderHints(QPainter::RenderHints i_renderHints)
//------------------------------------------------------------------------------
{
    s_painterRenderHints = i_renderHints;
}

//------------------------------------------------------------------------------
void CGraphObjLabelGeometryAngle::resetPainterRenderHints()
//------------------------------------------------------------------------------
{
    s_painterRenderHints = QPainter::Antialiasing;
}

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
    m_lineSelPtsSceneCoors(),
    m_rectPie(),
    m_fPieStartAngle_degrees(0.0),
    m_fPieSpanAngle_degrees(0.0),
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
        /* strObjName   */ path(),
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

} // ctor

//------------------------------------------------------------------------------
CGraphObjLabelGeometryAngle::~CGraphObjLabelGeometryAngle()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    emit_aboutToBeDestroyed();

    //m_lineSelPtsSceneCoors;
    //m_rectPie;
    m_fPieStartAngle_degrees = 0.0;
    m_fPieSpanAngle_degrees = 0.0;
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ "" );

    QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();
    rctBounding |= m_rectPie;
    int iPenWidth = m_drawSettings.penWidth();
    if ((m_pDrawingScene->getMode() == EMode::Edit) && (m_bIsHighlighted || isSelected())) {
        iPenWidth += 3; // see paint method
    }
    rctBounding = QRectF(
        rctBounding.left() - static_cast<double>(iPenWidth)/2.0,
        rctBounding.top() - static_cast<double>(iPenWidth)/2.0,
        rctBounding.width() + static_cast<double>(iPenWidth),
        rctBounding.height() + static_cast<double>(iPenWidth));
    if (m_plgP2ArrowHead.size() > 0) {
        rctBounding |= m_plgP2ArrowHead.boundingRect();
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
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
        /* strObjName   */ path(),
        /* strMethod    */ "paint",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjLabel::paint(i_pPainter, i_pStyleOption, i_pWdgt);

    // Draw anchor line (circle segment) to indicate the angle if the label is hit,
    // hovered, selected or if the anchor line is set to be visible.
    // Use a different color if the line is hit, hovered or selected.
    if (/*m_bIsHit ||*/ isSelected() || m_labelDscr.m_bShowAnchorLine
     && (fabs(m_rectPie.width()) >= 20.0 || fabs(m_rectPie.height()) >= 20.0)) {
        i_pPainter->save();
        i_pPainter->setRenderHint(QPainter::Antialiasing);
        QPen pn = pen();
        pn.setWidth(1);
        pn.setStyle(Qt::SolidLine);
        QColor color = /*m_bIsHit ||*/ isSelected() ? Qt::blue : Qt::lightGray;
        color.setAlpha(192);
        pn.setColor(color);
        i_pPainter->setPen(pn);
        i_pPainter->setRenderHints(s_painterRenderHints);
        // The startAngle and spanAngle must be specified in 1/16th of a degree.
        // The painter counts angles counter clockwise whereas the graphics system
        // counts angles clockwise. Correct the angles before drawing the pies.
        double fPieStartAngle_degrees = Math::toCounterClockWiseAngleDegree(m_fPieStartAngle_degrees);
        double fPieSpanAngle_degrees = -m_fPieSpanAngle_degrees;
        i_pPainter->drawPie(m_rectPie, 16.0 * fPieStartAngle_degrees, 16.0 * fPieSpanAngle_degrees);
        QBrush brsh(pn.color());
        i_pPainter->setBrush(brsh);
        i_pPainter->drawPolygon(m_plgP2ArrowHead);
        //#pragma message(__TODO__"To be removed")
        //pn.setColor(Qt::red);
        //pn.setWidth(1);
        //pn.setStyle(Qt::SolidLine);
        //i_pPainter->setPen(pn);
        //QLineF lineSelPts = QLineF(mapFromScene(m_lineSelPtsSceneCoors.p1()), mapFromScene(m_lineSelPtsSceneCoors.p2()));
        //i_pPainter->drawLine(lineSelPts);
        //#pragma message(__TODO__"To be removed")
        i_pPainter->restore();
    }
}

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
        /* strObjName   */ path(),
        /* strMethod    */ "updatePosition",
        /* strAddInfo   */ "" );

    QPointF ptSelPt1ScenePos;
    if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::BoundingRectangle) {
        ptSelPt1ScenePos = m_labelDscr.m_selPt1.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
            m_labelDscr.m_selPt1.m_selPtType, m_labelDscr.m_selPt1.m_selPt);
    }
    else {
        ptSelPt1ScenePos = m_labelDscr.m_selPt1.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
            m_labelDscr.m_selPt1.m_selPtType, m_labelDscr.m_selPt1.m_idxPt);
    }
    QPointF ptSelPt2ScenePos;
    if (m_labelDscr.m_selPt2.m_selPtType == ESelectionPointType::BoundingRectangle) {
        ptSelPt2ScenePos = m_labelDscr.m_selPt2.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
            m_labelDscr.m_selPt2.m_selPtType, m_labelDscr.m_selPt2.m_selPt);
    }
    else {
        ptSelPt2ScenePos = m_labelDscr.m_selPt2.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
            m_labelDscr.m_selPt2.m_selPtType, m_labelDscr.m_selPt2.m_idxPt);
    }
    m_lineSelPtsSceneCoors = QLineF(ptSelPt1ScenePos, ptSelPt2ScenePos);

    QString strText;
    // If the label is linked to just one object ..
    if (m_labelDscr.m_selPt1.m_pGraphObj == m_labelDscr.m_selPt2.m_pGraphObj) {
        // .. get coordinate from graphical object.
        strText = m_labelDscr.m_selPt1.m_pGraphObj->rotationAngle().toString();
    }
    // If the label is linked to different objects ..
    else {
        // .. calculate coordinate from selection points.
        strText = QString::number(m_lineSelPtsSceneCoors.angle(), 'f', 1) + " " + QString(Math::c_chSymbolDegree);
    }
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
    QLineF lineFPolarBaseSceneCoors(m_lineSelPtsSceneCoors.center(), m_lineSelPtsSceneCoors.p2());
    QLineF anchorLineSceneCoors = ZS::Draw::getLineFromPolar(
        m_labelDscr.m_polarCoorsToLinkedSelPt.m_fLength_px,
        m_labelDscr.m_polarCoorsToLinkedSelPt.m_fAngle_degrees,
        lineFPolarBaseSceneCoors);

    // The position of a QGraphicsTextItem is defined by its top left corner.
    // Move text item so that its center point is at the line end point of the anchor line.
    // Note that labels are always positioned directly on the drawing scene and never belong to groups.
    QRectF rctBoundingThis = getBoundingRect();
    QPointF anchorLineP2ScenePos = anchorLineSceneCoors.p2() - rctBoundingThis.center();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strRuntimeInfo = "Pos {" + qPoint2Str(anchorLineP2ScenePos) + "} px";
        mthTracer.trace(strRuntimeInfo, ELogDetailLevel::Debug, ELogDetailLevel::None);
    }
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
                        | / Angle (to Label, !not angle value to be indicated!)
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
        /* strObjName   */ path(),
        /* strMethod    */ "updatePolarCoorsToLinkedSelPt",
        /* strAddInfo   */ "" );

    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);

    // Get anchor line in scene coordinates.
    // The start point of the anchor line should be the center point of the line
    // for which the length has to be indicated.
    QPointF ptSelPt1ScenePos;
    if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::BoundingRectangle) {
        ptSelPt1ScenePos = m_labelDscr.m_selPt1.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
            m_labelDscr.m_selPt1.m_selPtType, m_labelDscr.m_selPt1.m_selPt);
    }
    else {
        ptSelPt1ScenePos = m_labelDscr.m_selPt1.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
            m_labelDscr.m_selPt1.m_selPtType, m_labelDscr.m_selPt1.m_idxPt);
    }
    QPointF ptSelPt2ScenePos;
    if (m_labelDscr.m_selPt2.m_selPtType == ESelectionPointType::BoundingRectangle) {
        ptSelPt2ScenePos = m_labelDscr.m_selPt2.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
            m_labelDscr.m_selPt2.m_selPtType, m_labelDscr.m_selPt2.m_selPt);
    }
    else {
        ptSelPt2ScenePos = m_labelDscr.m_selPt2.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
            m_labelDscr.m_selPt2.m_selPtType, m_labelDscr.m_selPt2.m_idxPt);
    }
    m_lineSelPtsSceneCoors = QLineF(ptSelPt1ScenePos, ptSelPt2ScenePos);

    QPointF ptSelPtSceneCoors = m_lineSelPtsSceneCoors.center();

    QPointF ptThisScenePos = pGraphicsItemThis->pos();
    QRectF rctBoundingThis = getBoundingRect();
    QPointF ptCenterScenePosThis = pGraphicsItemThis->mapToScene(rctBoundingThis.center());

    // The start point of the anchor line should be the center point of the line
    // for which the length has to be indicated.
    QLineF lineFromSelPtSceneCoors(ptSelPtSceneCoors, ptCenterScenePosThis);

    m_labelDscr.m_polarCoorsToLinkedSelPt.m_fLength_px = lineFromSelPtSceneCoors.length();
    m_labelDscr.m_polarCoorsToLinkedSelPt.m_fAngle_degrees = m_lineSelPtsSceneCoors.angleTo(lineFromSelPtSceneCoors);
    m_labelDscr.m_polarCoorsToLinkedSelPt.m_fAngle_degrees =
        Math::toClockWiseAngleDegree(m_labelDscr.m_polarCoorsToLinkedSelPt.m_fAngle_degrees);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strRuntimeInfo = "PolarCoors {" + QString::number(m_labelDscr.m_polarCoorsToLinkedSelPt.m_fLength_px) + " px" +
            ", " + QString::number(m_labelDscr.m_polarCoorsToLinkedSelPt.m_fAngle_degrees) + " " + QString(Math::c_chSymbolDegree) + "}";
        mthTracer.trace(strRuntimeInfo, ELogDetailLevel::Debug, ELogDetailLevel::None);
    }

    // Update coordinates of the anchor line.
    updateAnchorLines();
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the coordinates (start and end point)
           of the anchor lines.

    The angle label does not use the common anchor lines but draws the anchor lines
    as a circle segment (pie).
*/
void CGraphObjLabelGeometryAngle::updateAnchorLines()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateAnchorLines",
        /* strAddInfo   */ "" );

    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);

    // Get pie rectangle in local local coordinates. The pie rectangle has its center
    // at the center point of line between the linked selection points.
    QPointF ptSelPt1 = mapFromScene(m_lineSelPtsSceneCoors.p1());
    QPointF ptSelPt2 = mapFromScene(m_lineSelPtsSceneCoors.p2());
    QLineF lineSelPts = QLineF(ptSelPt1, ptSelPt2);
    QPointF ptCenterLineSelPts = lineSelPts.center();
    m_rectPie.setWidth(2.0 * m_labelDscr.m_polarCoorsToLinkedSelPt.m_fLength_px);
    m_rectPie.setHeight(2.0 * m_labelDscr.m_polarCoorsToLinkedSelPt.m_fLength_px);
    m_rectPie.moveCenter(ptCenterLineSelPts);
    // The angle returned by QLineF is counter-clockwise with 0° at 3 o'clock.
    // The graphics scene uses clockwise angles with 0° at 3 o'clock.
    m_fPieSpanAngle_degrees = m_lineSelPtsSceneCoors.angle();
    m_fPieSpanAngle_degrees = Math::toClockWiseAngleDegree(m_fPieSpanAngle_degrees);
    // If both selection points are linked to the same object ...
    if (m_labelDscr.m_selPt1.m_pGraphObj == m_labelDscr.m_selPt2.m_pGraphObj) {
        // .. the scenes rotation angle of the parent group of the linked object must be subtracted
        // to get the start point of the pie and the span has to be corrected correspondingly.
        CGraphObjGroup* pGraphObjParentGroup = m_labelDscr.m_selPt1.m_pGraphObj->parentGroup();
        if (pGraphObjParentGroup != nullptr) {
            double fParentGroupSceneRotationAngle_degrees = pGraphObjParentGroup->getEffectiveRotationAngleOnScene(Units.Angle.Degree).getVal();
            m_fPieStartAngle_degrees = fParentGroupSceneRotationAngle_degrees;
            m_fPieStartAngle_degrees = Math::normalizeAngleInDegree(m_fPieStartAngle_degrees);
            m_fPieSpanAngle_degrees -= fParentGroupSceneRotationAngle_degrees;
            m_fPieSpanAngle_degrees = Math::normalizeAngleInDegree(m_fPieSpanAngle_degrees);
        }
    }
    else {
        m_fPieStartAngle_degrees = 0.0;
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strRuntimeInfo = "AnchorLine2 {Rect {" + qRect2Str(m_rectPie) + "} px" +
            ", PieAngles {Start: " + QString::number(m_fPieStartAngle_degrees) + " " + QString(Math::c_chSymbolDegree) +
            ", Span: " + QString::number(m_fPieSpanAngle_degrees) + " " + QString(Math::c_chSymbolDegree) + "}";
        mthTracer.trace(strRuntimeInfo, ELogDetailLevel::Debug, ELogDetailLevel::None);
    }

    QLineF lineEndOfSegment(ptCenterLineSelPts, ptSelPt2);
    lineEndOfSegment.setLength(m_labelDscr.m_polarCoorsToLinkedSelPt.m_fLength_px);
    QLineF perpendicularLineEndOfSegment = ZS::Draw::getPerpendicularLine(
        QLineF(lineSelPts.p1(), lineSelPts.p2()), lineEndOfSegment.p2(), 10.0);

    getLineEndArrowPolygons(
        /* line          */ perpendicularLineEndOfSegment,
        /* drawSetings   */ m_drawSettingsArrowHeads,
        /* pplgLineStart */ &m_plgP2ArrowHead,
        /* pplgLineEnd   */ nullptr);
}
