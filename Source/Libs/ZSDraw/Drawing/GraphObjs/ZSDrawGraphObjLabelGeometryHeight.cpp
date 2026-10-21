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
#include "ZSSys/ZSSysRefCountGuard.h"
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
protected: // class members
==============================================================================*/

QPainter::RenderHints CGraphObjLabelGeometryHeight::s_painterRenderHints = QPainter::Antialiasing;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QPainter::RenderHints CGraphObjLabelGeometryHeight::painterRenderHints()
//------------------------------------------------------------------------------
{
    return s_painterRenderHints;
}

//------------------------------------------------------------------------------
void CGraphObjLabelGeometryHeight::setPainterRenderHints(QPainter::RenderHints i_renderHints)
//------------------------------------------------------------------------------
{
    s_painterRenderHints = i_renderHints;
}

//------------------------------------------------------------------------------
void CGraphObjLabelGeometryHeight::resetPainterRenderHints()
//------------------------------------------------------------------------------
{
    s_painterRenderHints = QPainter::Antialiasing;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjLabelGeometryHeight::CGraphObjLabelGeometryHeight(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strKey,
    const SGraphObjSelectionPoint& i_selPt) :
//------------------------------------------------------------------------------
    CGraphObjLabel(
        /* pDrawingScene */ i_pDrawingScene,
        /* strKey        */ i_strKey,
        /* type          */ EGraphObjTypeLabelGeometryHeight,
        /* selPt         */ i_selPt),
    m_drawSettingsArrowHeads(EGraphObjTypeLine)
{
    createTraceAdminObjs("ZS::Draw::Drawing::GraphObjs::Labels", ClassName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Key: " + i_strKey + ", SelPt {" + i_selPt.toString(true) + "}";
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
CGraphObjLabelGeometryHeight::~CGraphObjLabelGeometryHeight()
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
}

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the type method of QGraphicsItem.
*/
int CGraphObjLabelGeometryHeight::type() const
//------------------------------------------------------------------------------
{
    return EGraphObjTypeLabelGeometryHeight;
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
        /* strObjName   */ path(),
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    CGraphObjLabelGeometryHeight* pGraphObj = nullptr;
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
QRectF CGraphObjLabelGeometryHeight::boundingRect() const
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
void CGraphObjLabelGeometryHeight::paint(
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceThisPositionInfo(mthTracer, EMethodDir::Enter);
    }
    CRefCountGuard refCountGuardTrace(&m_iTraceBlockedCounter);

    CGraphObjLabel::paint(i_pPainter, i_pStyleOption, i_pWdgt);

    if (/*m_bIsHit ||*/ isSelected() || m_bShowAnchorLine) {
        i_pPainter->save();
        i_pPainter->setRenderHint(QPainter::Antialiasing);
        QPen pn = pen();
        pn.setWidth(1);
        pn.setStyle(Qt::SolidLine);
        QColor color = /*m_bIsHit ||*/ isSelected() ? Qt::blue : Qt::lightGray;
        color.setAlpha(192);
        pn.setColor(color);
        i_pPainter->setPen(pn);
        QBrush brsh(pn.color());
        i_pPainter->setBrush(brsh);
        QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();
        for (const QLineF& anchorLine : m_anchorLines) {
            i_pPainter->drawLine(anchorLine);
        }
        QLineF anchorLine = m_anchorLines[2];
        if ((fabs(anchorLine.dx()) >= 5.0) || (fabs(anchorLine.dy()) >= 5.0)) {
            i_pPainter->drawPolygon(m_plgP1ArrowHead);
        }
        anchorLine = m_anchorLines[3];
        if ((fabs(anchorLine.dx()) >= 5.0) || (fabs(anchorLine.dy()) >= 5.0)) {
            i_pPainter->drawPolygon(m_plgP2ArrowHead);
        }
        i_pPainter->restore();
    }
}

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
        /* strObjName   */ path(),
        /* strMethod    */ "updatePosition",
        /* strAddInfo   */ "" );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceThisPositionInfo(mthTracer, EMethodDir::Enter);
    }
    CRefCountGuard refCountGuardTrace(&m_iTraceBlockedCounter);

    QPointF ptSelPtTopScenePos;
    QPointF ptSelPtBottomScenePos;

    // The center point of the label could be within, left or right
    // of the bounding rectangle of the linked object. The selection point, for which
    // the polar coordinates are calculated, was set correspondingly.
    if (m_selPtLinkedObject.m_selPt == ESelectionPoint::LeftCenter) { // left
        ptSelPtTopScenePos =
            m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                ESelectionPointType::BoundingRectangle, ESelectionPoint::TopLeft);
        ptSelPtBottomScenePos =
            m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomLeft);
    }
    else if (m_selPtLinkedObject.m_selPt == ESelectionPoint::RightCenter) { // right
        ptSelPtTopScenePos =
            m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                ESelectionPointType::BoundingRectangle, ESelectionPoint::TopRight);
        ptSelPtBottomScenePos =
            m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomRight);
    }
    else { // if (rctBoundingLinkedObject.contains(ptCenterThisInLinkedObjectCoors)) // within
        // If the center point of the label is within the bounding rectangle of the linked object,
        // the extension lines (anchor0 and anchor1) will not be drawn and are not visible.
        ptSelPtTopScenePos =
            m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                ESelectionPointType::BoundingRectangle, ESelectionPoint::TopCenter);
        ptSelPtBottomScenePos =
            m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomCenter);
    }

    QLineF lineSelPtsSceneCoors(ptSelPtTopScenePos, ptSelPtBottomScenePos);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValHeight(lineSelPtsSceneCoors.length(), Units.Length.px, drawingSize.imageCoorsResolutionInPx());
    physValHeight.convertValue(drawingSize.unit());
    QString strText = physValHeight.toString(EUnitFind::None, PhysValSubStr::Val);
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
    QGraphicsItem_setPos(posTextItem);

    // Please note that on calling setPos the itemChange method of the
    // label is called invoking updateAnchorLines.
    m_bUpdatePositionInProgress = false;

    refCountGuardTrace.decrementAndReleaseCounter();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceThisPositionInfo(mthTracer, EMethodDir::Leave);
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
        /* strObjName   */ path(),
        /* strMethod    */ "updatePolarCoorsToLinkedSelPt",
        /* strAddInfo   */ "" );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceThisPositionInfo(mthTracer, EMethodDir::Enter);
    }
    CRefCountGuard refCountGuardTrace(&m_iTraceBlockedCounter);

    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    QRectF rctBoundingThis = getBoundingRect();
    QPointF ptCenterThis = rctBoundingThis.center();
    QPointF ptCenterScenePosThis = pGraphicsItemThis->mapToScene(ptCenterThis);

    QGraphicsItem* pGraphicsItemLinkedObject = dynamic_cast<QGraphicsItem*>(m_selPtLinkedObject.m_pGraphObj);
    QRectF rctBoundingLinkedObject = m_selPtLinkedObject.m_pGraphObj->getBoundingRect();
    QPointF ptCenterThisInLinkedObjectCoors =
        pGraphicsItemThis->mapToItem(pGraphicsItemLinkedObject, ptCenterThis);

    QPointF ptSelPtTopScenePos;
    QPointF ptSelPtBottomScenePos;

    // The center point of the label could be within, left or right
    // of the bounding rectangle of the linked object. The selection point, for which
    // the polar coordinates are calculated, has to be changed correspondingly.
    if (ptCenterThisInLinkedObjectCoors.x() < rctBoundingLinkedObject.left()) {
        m_selPtLinkedObject.m_selPt = ESelectionPoint::LeftCenter;
        ptSelPtTopScenePos =
            m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                ESelectionPointType::BoundingRectangle, ESelectionPoint::TopLeft);
        ptSelPtBottomScenePos =
            m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomLeft);
    }
    else if (ptCenterThisInLinkedObjectCoors.x() > rctBoundingLinkedObject.right()) {
        m_selPtLinkedObject.m_selPt = ESelectionPoint::RightCenter;
        ptSelPtTopScenePos =
            m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                ESelectionPointType::BoundingRectangle, ESelectionPoint::TopRight);
        ptSelPtBottomScenePos =
            m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomRight);
    }
    else { // if (rctBoundingLinkedObject.contains(ptCenterThisInLinkedObjectCoors)) // within
        m_selPtLinkedObject.m_selPt = ESelectionPoint::Center;
        // If the center point of the label is within the bounding rectangle of the linked object,
        // the extension lines (anchor0 and anchor1) will not be drawn and are not visible.
        ptSelPtTopScenePos =
            m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                ESelectionPointType::BoundingRectangle, ESelectionPoint::TopCenter);
        ptSelPtBottomScenePos =
            m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomCenter);
    }

    QLineF lineSelPtsSceneCoors(ptSelPtTopScenePos, ptSelPtBottomScenePos);
    QPointF ptSelPtSceneCoors = lineSelPtsSceneCoors.center();

    // The start point of the anchor line should be the center point of the line
    // for which the length has to be indicated.
    QLineF lineFromSelPtSceneCoors(ptSelPtSceneCoors, ptCenterScenePosThis);

    m_polarCoorsToLinkedSelPt.m_fLength_px = lineFromSelPtSceneCoors.length();
    m_polarCoorsToLinkedSelPt.m_fAngle_degrees = lineSelPtsSceneCoors.angleTo(lineFromSelPtSceneCoors);
    m_polarCoorsToLinkedSelPt.m_fAngle_degrees =
        Math::toClockWiseAngleDegree(m_polarCoorsToLinkedSelPt.m_fAngle_degrees);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strRuntimeInfo = "AnchoredTo {" + m_selPtLinkedObject.toString() + "}" +
            ", PolarCoors {" + m_polarCoorsToLinkedSelPt.toString() + "}";
        mthTracer.trace(strRuntimeInfo, ELogDetailLevel::Debug, ELogDetailLevel::None);
    }

    // Update coordinates of the anchor line.
    updateAnchorLines();

    refCountGuardTrace.decrementAndReleaseCounter();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceThisPositionInfo(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the coordinates (start and end point)
           of the anchor lines.

    For the Height label four anchor lines are drawn. Two horizontal extension lines
    at the top and bottom border of the item's bounding rectangle and two vertical
    dimension lines between the text item and the end points of the horizontal extension lines.

    Example for a rectangle:

        TL                   TR
        +----------x----------+----- Anchor0
        |                     |    +
        |                     |    |
        |                     |    | Anchor2
        |                     |    |
        |                     |    |
        |       Center        | TextItem
        |                     |    |
        |                     |    |
        |                     |    | Anchor3
        |                     |    |
        |                     |    +
        +----------x----------+----- Anchor1
        BL                   BR

        Anchor0 and Anchor1 are the extension lines of the dimension line.
*/
void CGraphObjLabelGeometryHeight::updateAnchorLines()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateAnchorLines",
        /* strAddInfo   */ "" );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceThisPositionInfo(mthTracer, EMethodDir::Enter);
    }
    CRefCountGuard refCountGuardTrace(&m_iTraceBlockedCounter);

    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    QRectF rctBoundingThis = getBoundingRect();
    QPointF ptCenterThis = rctBoundingThis.center();

    QGraphicsItem* pGraphicsItemLinkedObject = dynamic_cast<QGraphicsItem*>(m_selPtLinkedObject.m_pGraphObj);
    QRectF rctBoundingLinkedObject = m_selPtLinkedObject.m_pGraphObj->getBoundingRect();
    QPointF ptCenterThisInLinkedObjectCoors =
        pGraphicsItemThis->mapToItem(pGraphicsItemLinkedObject, ptCenterThis);

    QPointF anchorLine0Pt1InLinkedObjectCoors;
    QPointF anchorLine0Pt2InLinkedObjectCoors;
    QPointF anchorLine1Pt1InLinkedObjectCoors;
    QPointF anchorLine1Pt2InLinkedObjectCoors;

    QLineF lineTopRectBoundingLinkedObject(
        rctBoundingLinkedObject.topLeft(), rctBoundingLinkedObject.topRight());
    QLineF linePerpendicularToLeftBorder = getPerpendicularLine(
        lineTopRectBoundingLinkedObject, ptCenterThisInLinkedObjectCoors, 0.0, &anchorLine0Pt1InLinkedObjectCoors);
    QLineF lineBottomRectBoundingLinkedObject(
        rctBoundingLinkedObject.bottomLeft(), rctBoundingLinkedObject.bottomRight());
    QLineF linePerpendicularToRightBorder = getPerpendicularLine(
        lineBottomRectBoundingLinkedObject, ptCenterThisInLinkedObjectCoors, 0.0, &anchorLine1Pt1InLinkedObjectCoors);

    // The center point of the label could be within, left or right
    // of the bounding rectangle of the linked object.
    if (ptCenterThisInLinkedObjectCoors.x() < rctBoundingLinkedObject.left()) {
        anchorLine0Pt2InLinkedObjectCoors = rctBoundingLinkedObject.topLeft();
        anchorLine1Pt2InLinkedObjectCoors = rctBoundingLinkedObject.bottomLeft();
    }
    else if (ptCenterThisInLinkedObjectCoors.x() > rctBoundingLinkedObject.right()) {
        anchorLine0Pt2InLinkedObjectCoors = rctBoundingLinkedObject.topRight();
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

    // As the anchor lines are painted by this object, they have to be translated into local coordinates.
    m_anchorLines[0] = QLineF(mapFromScene(anchorLine0Pt1InSceneCoors), mapFromScene(anchorLine0Pt2InSceneCoors));
    m_anchorLines[1] = QLineF(mapFromScene(anchorLine1Pt1InSceneCoors), mapFromScene(anchorLine1Pt2InSceneCoors));
    m_anchorLines[2] = QLineF(mapFromScene(anchorLine0Pt1InSceneCoors), ptCenterThis);
    m_anchorLines[3] = QLineF(mapFromScene(anchorLine1Pt1InSceneCoors), ptCenterThis);
    m_anchorLines[2].setLength(m_anchorLines[2].length() - rctBoundingThis.width()/2.0);
    m_anchorLines[3].setLength(m_anchorLines[3].length() - rctBoundingThis.width()/2.0);

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

    refCountGuardTrace.decrementAndReleaseCounter();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceThisPositionInfo(mthTracer, EMethodDir::Leave);
    }
}

/*==============================================================================
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabelGeometryHeight::traceThisPositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, const QString& i_strFilter) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTracePositionInfoBlockedCounter > 0 || m_iTraceThisPositionInfoInfoBlockedCounter > 0) {
        return;
    }

    CGraphObjLabel::traceThisPositionInfo(i_mthTracer, i_mthDir, i_strFilter);
}
