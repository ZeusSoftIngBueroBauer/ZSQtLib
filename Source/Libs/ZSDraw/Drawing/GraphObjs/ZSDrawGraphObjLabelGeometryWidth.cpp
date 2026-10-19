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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryWidth.h"
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
class CGraphObjLabelGeometryWidth : public CGraphObjLabel
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

QPainter::RenderHints CGraphObjLabelGeometryWidth::s_painterRenderHints = QPainter::Antialiasing;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QPainter::RenderHints CGraphObjLabelGeometryWidth::painterRenderHints()
//------------------------------------------------------------------------------
{
    return s_painterRenderHints;
}

//------------------------------------------------------------------------------
void CGraphObjLabelGeometryWidth::setPainterRenderHints(QPainter::RenderHints i_renderHints)
//------------------------------------------------------------------------------
{
    s_painterRenderHints = i_renderHints;
}

//------------------------------------------------------------------------------
void CGraphObjLabelGeometryWidth::resetPainterRenderHints()
//------------------------------------------------------------------------------
{
    s_painterRenderHints = QPainter::Antialiasing;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjLabelGeometryWidth::CGraphObjLabelGeometryWidth(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strKey,
    CGraphObj* i_pGraphObjLinked) :
//------------------------------------------------------------------------------
    CGraphObjLabel(
        /* pDrawingScene */ i_pDrawingScene,
        /* strKey        */ i_strKey,
        /* type          */ EGraphObjTypeLabelGeometryWidth,
        /* selPt         */ SGraphObjSelectionPoint(i_pGraphObjLinked, ESelectionPointType::BoundingRectangle, ESelectionPoint::Center)),
    m_drawSettingsArrowHeads(EGraphObjTypeLine),
    m_plgP1ArrowHead(),
    m_plgP2ArrowHead()
{
    createTraceAdminObjs("ZS::Draw::Drawing::GraphObjs::Labels", ClassName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Key: " + i_strKey +
            ", LinkedObject: " + QString(i_pGraphObjLinked == nullptr ? "null" : i_pGraphObjLinked->path());
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

    // Anchor0 and Anchor1 are the extension lines from the item to the dimension line ends.
    m_anchorLines.append(QLineF());
    m_anchorLines.append(QLineF());
    // Anchor2 and Anchor3 are the dimension lines from the extension lines to the text.
    m_anchorLines.append(QLineF());
    m_anchorLines.append(QLineF());
}

//------------------------------------------------------------------------------
CGraphObjLabelGeometryWidth::~CGraphObjLabelGeometryWidth()
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
int CGraphObjLabelGeometryWidth::type() const
//------------------------------------------------------------------------------
{
    return EGraphObjTypeLabelGeometryWidth;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjLabelGeometryWidth::clone()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    CGraphObjLabelGeometryWidth* pGraphObj = nullptr;
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
QRectF CGraphObjLabelGeometryWidth::boundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ "" );

    QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();

    // If the object is hit and the anchor line is visible also this area need to be updated.
    if (/*m_bIsHit ||*/ isSelected() || m_bShowAnchorLine) {
        for (const QLineF& anchorLine : m_anchorLines) {
            QRectF rctBoundingAnchorLine(anchorLine.p1(), anchorLine.p2());
            rctBounding |= rctBoundingAnchorLine;
        }
    }
    int iPenWidth = m_drawSettings.penWidth();
    if ((m_pDrawingScene->getMode() == EMode::Edit) && (m_bIsHighlighted || isSelected())) {
        iPenWidth += 3; // see paint method
    }
    rctBounding = QRectF(
        rctBounding.left() - static_cast<double>(iPenWidth)/2.0,
        rctBounding.top() - static_cast<double>(iPenWidth)/2.0,
        rctBounding.width() + static_cast<double>(iPenWidth),
        rctBounding.height() + static_cast<double>(iPenWidth));
    if (m_plgP1ArrowHead.size() > 0) {
        rctBounding |= m_plgP1ArrowHead.boundingRect();
    }
    if (m_plgP2ArrowHead.size() > 0) {
        rctBounding |= m_plgP2ArrowHead.boundingRect();
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
        QRectF rctBoundingSceneCoors = pGraphicsItemThis->mapToScene(rctBounding).boundingRect();
        mthTracer.setMethodReturn("Item {" + qRect2Str(rctBounding) + "}, Scene {" + qRect2Str(rctBoundingSceneCoors) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
void CGraphObjLabelGeometryWidth::paint(
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

    if (/*m_bIsHit ||*/ isSelected() || m_bShowAnchorLine) {
        i_pPainter->save();
        i_pPainter->setRenderHints(s_painterRenderHints);
        QPen pn = pen();
        pn.setWidth(1);
        pn.setStyle(Qt::SolidLine);
        #if 0
        QColor color = /*m_bIsHit ||*/ isSelected() ? Qt::blue : Qt::lightGray;
        color.setAlpha(192);
        #else
        QColor color = Qt::red;
        #endif
        pn.setColor(color);
        pn.setWidth(2);
        i_pPainter->setPen(pn);
        QBrush brsh(pn.color());
        i_pPainter->setBrush(brsh);
        QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();
        for (const QLineF& anchorLine : m_anchorLines) {
            i_pPainter->drawLine(anchorLine);
        }
        QLineF& anchorLine = m_anchorLines[2];
        if ((fabs(anchorLine.dx()) >= 5.0) || (fabs(anchorLine.dy()) >= 5.0)) {
            i_pPainter->drawPolygon(m_plgP1ArrowHead);
        }
        anchorLine = m_anchorLines[3];
        if ((fabs(anchorLine.dx()) >= 5.0) || (fabs(anchorLine.dy()) >= 5.0)) {
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
void CGraphObjLabelGeometryWidth::updatePosition()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updatePosition",
        /* strAddInfo   */ "" );

    QPointF ptSelPtLeftCenterScenePos =
        m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
            ESelectionPointType::BoundingRectangle, ESelectionPoint::LeftCenter);
    QPointF ptSelPtRightCenterScenePos =
        m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
            ESelectionPointType::BoundingRectangle, ESelectionPoint::RightCenter);
    QLineF lineSelPtsSceneCoors(ptSelPtLeftCenterScenePos, ptSelPtRightCenterScenePos);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValWidth(lineSelPtsSceneCoors.length(), Units.Length.px, drawingSize.imageCoorsResolutionInPx());
    physValWidth.convertValue(drawingSize.unit());
    QString strText = physValWidth.toString(EUnitFind::None, PhysValSubStr::Val);
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
    QLineF lineFPolarBase(lineSelPtsSceneCoors.center(), lineSelPtsSceneCoors.p2());
    QLineF anchorLine = ZS::Draw::getLineFromPolar(
        m_polarCoorsToLinkedSelPt.m_fLength_px,
        m_polarCoorsToLinkedSelPt.m_fAngle_degrees,
        lineFPolarBase);

    // The position of a QGraphicsTextItem is defined by its top left corner.
    // Move text item so that its center point is at the line end point of the anchor line.
    QRectF rctBoundingThis = getBoundingRect();
    QPointF posTextItem = anchorLine.p2() - rctBoundingThis.center();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strRuntimeInfo = "Pos {" + qPoint2Str(posTextItem) + "} px";
        mthTracer.trace(strRuntimeInfo, ELogDetailLevel::Debug, ELogDetailLevel::None);
    }
    setPos(posTextItem);

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

    For Width labels the linked selection point is always the center point
    of the two shape points the label is linked to.

    Example for a horizontal line:

                        Label
                           \
                            \ Angle
         P1 +----------------x----------------+ P2
                          Center
*/
void CGraphObjLabelGeometryWidth::updatePolarCoorsToLinkedSelPt()
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
    QPointF ptSelPtLeftCenterScenePos =
        m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
            ESelectionPointType::BoundingRectangle, ESelectionPoint::LeftCenter);
    QPointF ptSelPtRightCenterScenePos =
        m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
            ESelectionPointType::BoundingRectangle, ESelectionPoint::RightCenter);
    QLineF lineSelPtsSceneCoors(ptSelPtLeftCenterScenePos, ptSelPtRightCenterScenePos);

    QPointF ptSelPtSceneCoors = lineSelPtsSceneCoors.center();

    QRectF rctBoundingThis = getBoundingRect();
    QPointF ptCenterScenePosThis = pGraphicsItemThis->mapToScene(rctBoundingThis.center());

    // The start point of the anchor line should be the center point of the line
    // for which the length has to be indicated.
    QLineF lineFromSelPtSceneCoors(ptSelPtSceneCoors, ptCenterScenePosThis);

    m_polarCoorsToLinkedSelPt.m_fLength_px = lineFromSelPtSceneCoors.length();
    m_polarCoorsToLinkedSelPt.m_fAngle_degrees = lineSelPtsSceneCoors.angleTo(lineFromSelPtSceneCoors);
    m_polarCoorsToLinkedSelPt.m_fAngle_degrees =
        Math::toClockWiseAngleDegree(m_polarCoorsToLinkedSelPt.m_fAngle_degrees);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strRuntimeInfo = "PolarCoors {" + QString::number(m_polarCoorsToLinkedSelPt.m_fLength_px) + " px" +
            ", " + QString::number(m_polarCoorsToLinkedSelPt.m_fAngle_degrees) + " " + QString(Math::c_chSymbolDegree) + "}";
        mthTracer.trace(strRuntimeInfo, ELogDetailLevel::Debug, ELogDetailLevel::None);
    }

    // Update coordinates of the anchor line.
    updateAnchorLines();
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the coordinates (start and end point)
           of the anchor lines.

    The anchor lines will be drawn to one of the selection points at the bounding rectangle
    or to one of the polygon shape points of the linked graphical object.

    For the Width label three anchor lines are drawn. Two vertical lines through
    selection point 1 and selection point 2 and a horizontal between the end points
    of the vertical lines.

    Example for a rectangle:

               Anchor2              Anchor3
            |<-----------TextItem------------>|
         Anchor0                           Anchor1
            |                                 |
         TL +----------------x----------------+ TR
            |                                 |
            |             Center              |
            |                                 |
         BL +----------------x----------------+ BR

         AnchorLine0 and AnchorLine1 are the extension lines of the dimension line.
*/
void CGraphObjLabelGeometryWidth::updateAnchorLines()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateAnchorLines",
        /* strAddInfo   */ "" );

    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    QGraphicsItem* pGraphicsItemLinkedObject = dynamic_cast<QGraphicsItem*>(m_selPtLinkedObject.m_pGraphObj);

    QPointF anchorLine0Pt1InLinkedObjectCoors;
    QPointF anchorLine0Pt2InLinkedObjectCoors;
    QPointF anchorLine1Pt1InLinkedObjectCoors;
    QPointF anchorLine1Pt2InLinkedObjectCoors;

    QRectF rctBoundingThis = getBoundingRect();
    QPointF ptCenterThis = rctBoundingThis.center();
    QPointF ptCenterThisInLinkedObjectCoors =
        pGraphicsItemThis->mapToItem(pGraphicsItemLinkedObject, ptCenterThis);
    QRectF rctBoundingLinkedObject = m_selPtLinkedObject.m_pGraphObj->getBoundingRect();
    QLineF lineLeftRectBoundingLinkedObject(
        rctBoundingLinkedObject.topLeft(), rctBoundingLinkedObject.bottomLeft());
    QLineF linePerpendicularToLeftBorder = getPerpendicularLine(
        lineLeftRectBoundingLinkedObject, ptCenterThisInLinkedObjectCoors, 0.0, &anchorLine0Pt1InLinkedObjectCoors);
    QLineF lineRightRectBoundingLinkedObject(
        rctBoundingLinkedObject.topRight(), rctBoundingLinkedObject.bottomRight());
    QLineF linePerpendicularToRightBorder = getPerpendicularLine(
        lineRightRectBoundingLinkedObject, ptCenterThisInLinkedObjectCoors, 0.0, &anchorLine1Pt1InLinkedObjectCoors);

    // The center point of the label could be within the bounding rectangle, above or below
    // the bounding rectangle of the linked object.
    if (ptCenterThisInLinkedObjectCoors.y() < rctBoundingLinkedObject.top()) { // above
        anchorLine0Pt2InLinkedObjectCoors = rctBoundingLinkedObject.topLeft();
        anchorLine1Pt2InLinkedObjectCoors = rctBoundingLinkedObject.topRight();
    }
    else if (ptCenterThisInLinkedObjectCoors.y() > rctBoundingLinkedObject.bottom()) { // below
        anchorLine0Pt2InLinkedObjectCoors = rctBoundingLinkedObject.bottomLeft();
        anchorLine1Pt2InLinkedObjectCoors = rctBoundingLinkedObject.bottomRight();
    }
    else { // if (rctBoundingLinkedObject.contains(ptCenterThisInLinkedObjectCoors)) // within
        // If the center point of the label is within the bounding rectangle of the linked object,
        // the extension lines (anchor0 and anchor1) will not be drawn and are not visible.
        anchorLine0Pt2InLinkedObjectCoors = anchorLine0Pt1InLinkedObjectCoors;
        anchorLine1Pt2InLinkedObjectCoors = anchorLine1Pt1InLinkedObjectCoors;
    }

    QPointF anchorLine0Pt1InSceneCoors = pGraphicsItemLinkedObject->mapToScene(anchorLine0Pt1InLinkedObjectCoors);
    QPointF anchorLine0Pt2InSceneCoors = pGraphicsItemLinkedObject->mapToScene(anchorLine0Pt2InLinkedObjectCoors);
    QPointF anchorLine1Pt1InSceneCoors = pGraphicsItemLinkedObject->mapToScene(anchorLine1Pt1InLinkedObjectCoors);
    QPointF anchorLine1Pt2InSceneCoors = pGraphicsItemLinkedObject->mapToScene(anchorLine1Pt2InLinkedObjectCoors);

    QPointF ptLeftCenterThis(rctBoundingThis.left(), ptCenterThis.y());
    QPointF ptRightCenterThis(rctBoundingThis.right(), ptCenterThis.y());

    // As the anchor lines are painted by this object, they have to be translated into local coordinates.
    m_anchorLines[0] = QLineF(mapFromScene(anchorLine0Pt1InSceneCoors), mapFromScene(anchorLine0Pt2InSceneCoors));
    m_anchorLines[1] = QLineF(mapFromScene(anchorLine1Pt1InSceneCoors), mapFromScene(anchorLine1Pt2InSceneCoors));
    m_anchorLines[2] = QLineF(mapFromScene(anchorLine0Pt1InSceneCoors), ptCenterThis);
    m_anchorLines[3] = QLineF(mapFromScene(anchorLine1Pt1InSceneCoors), ptCenterThis);
    m_anchorLines[2].setLength(m_anchorLines[2].length() - rctBoundingThis.width()/2.0);
    m_anchorLines[3].setLength(m_anchorLines[3].length() - rctBoundingThis.width()/2.0);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        for (int idxLine = 0; idxLine < m_anchorLines.size(); idxLine++) {
            QString strRuntimeInfo = "AnchorLine" + QString::number(idxLine) +
                " {" + qLine2Str(m_anchorLines[idxLine]) + "} px";
            mthTracer.trace(strRuntimeInfo, ELogDetailLevel::Debug, ELogDetailLevel::None);
        }
    }

    getLineEndArrowPolygons(
        /* line          */ m_anchorLines[2],
        /* drawSetings   */ m_drawSettingsArrowHeads,
        /* pplgLineStart */ &m_plgP1ArrowHead,
        /* pplgLineEnd   */ nullptr);
    getLineEndArrowPolygons(
        /* line          */ m_anchorLines[3],
        /* drawSetings   */ m_drawSettingsArrowHeads,
        /* pplgLineStart */ &m_plgP2ArrowHead,
        /* pplgLineEnd   */ nullptr);
}
