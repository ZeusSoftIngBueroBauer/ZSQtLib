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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryDY.h"
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
class CGraphObjLabelGeometryDY : public CGraphObjLabel
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

QPainter::RenderHints CGraphObjLabelGeometryDY::s_painterRenderHints = QPainter::Antialiasing;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QPainter::RenderHints CGraphObjLabelGeometryDY::painterRenderHints()
//------------------------------------------------------------------------------
{
    return s_painterRenderHints;
}

//------------------------------------------------------------------------------
void CGraphObjLabelGeometryDY::setPainterRenderHints(QPainter::RenderHints i_renderHints)
//------------------------------------------------------------------------------
{
    s_painterRenderHints = i_renderHints;
}

//------------------------------------------------------------------------------
void CGraphObjLabelGeometryDY::resetPainterRenderHints()
//------------------------------------------------------------------------------
{
    s_painterRenderHints = QPainter::Antialiasing;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjLabelGeometryDY::CGraphObjLabelGeometryDY(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strKey,
    const SGraphObjSelectionPoint& i_selPt1,
    const SGraphObjSelectionPoint& i_selPt2) :
//------------------------------------------------------------------------------
    CGraphObjLabel(
        /* pDrawingScene */ i_pDrawingScene,
        /* strKey        */ i_strKey,
        /* strText       */ "dY",
        /* type          */ EGraphObjTypeLabelGeometryDY,
        /* selPt         */ i_selPt1,
        /* selPt         */ i_selPt2),
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
        /* strObjName   */ myPathName(),
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
CGraphObjLabelGeometryDY::~CGraphObjLabelGeometryDY()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
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
int CGraphObjLabelGeometryDY::type() const
//------------------------------------------------------------------------------
{
    return EGraphObjTypeLabelGeometryDY;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjLabelGeometryDY::clone()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    CGraphObjLabelGeometryDY* pGraphObj = nullptr;
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
QRectF CGraphObjLabelGeometryDY::boundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
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
    if (m_plgP1ArrowHead.size() > 0) {
        rctBounding |= m_plgP1ArrowHead.boundingRect();
    }
    if (m_plgP2ArrowHead.size() > 0) {
        rctBounding |= m_plgP2ArrowHead.boundingRect();
    }
    rctBounding = QRectF(
        rctBounding.left() - m_drawSettings.getPenWidth()/2,
        rctBounding.top() - m_drawSettings.getPenWidth()/2,
        rctBounding.width() + m_drawSettings.getPenWidth(),
        rctBounding.height() + m_drawSettings.getPenWidth() );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
void CGraphObjLabelGeometryDY::paint(
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
        /* strObjName   */ myPathName(),
        /* strMethod    */ "paint",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjLabel::paint(i_pPainter, i_pStyleOption, i_pWdgt);

    if (/*m_bIsHit ||*/ isSelected() || m_labelDscr.m_bShowAnchorLine) {
        i_pPainter->save();
        i_pPainter->setRenderHint(QPainter::Antialiasing);
        QPen pn = pen();
        pn.setWidth(1);
        pn.setStyle(Qt::SolidLine);
        QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();
        const QLineF& anchorLine = m_anchorLines[2];
         if (!rctBounding.contains(anchorLine.p2())) {
             if ((fabs(anchorLine.dx()) >= 5.0) || (fabs(anchorLine.dy()) >= 5.0)) {
                QColor color = /*m_bIsHit ||*/ isSelected() ? Qt::blue : Qt::lightGray;
                color.setAlpha(192);
                pn.setColor(color);
                i_pPainter->setPen(pn);
                QBrush brsh(pn.color());
                i_pPainter->setBrush(brsh);
                i_pPainter->setRenderHints(s_painterRenderHints);
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
void CGraphObjLabelGeometryDY::updatePosition()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
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

    QLineF lineSelPtSceneCoors(ptSelPt1ScenePos, ptSelPt2ScenePos);
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValdY(lineSelPtSceneCoors.dy(), Units.Length.px, drawingSize.imageCoorsResolutionInPx());
    physValdY.convertValue(drawingSize.unit());
    QString strText = physValdY.toString(EUnitFind::None, PhysValSubStr::Val);
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

    For Height labels the linked selection point is always the center point
    of the two shape points the label is linked to.

    Example for a horizontal line:

                        Label
                           \
                            \ Angle
         P1 +----------------x----------------+ P2
                          Center
*/
void CGraphObjLabelGeometryDY::updatePolarCoorsToLinkedSelPt()
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
        /* strObjName   */ myPathName(),
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

    QLineF lineSelPtSceneCoors(ptSelPt1ScenePos, ptSelPt2ScenePos);
    QPointF ptSelPtSceneCoors = lineSelPtSceneCoors.center();

    QRectF rctBoundingThis = getBoundingRect();
    QPointF ptCenterScenePosThis = pGraphicsItemThis->mapToScene(rctBoundingThis.center());

    // The start point of the anchor line should be the center point of the line
    // for which the length has to be indicated.
    QLineF lineFromSelPtSceneCoors(ptSelPtSceneCoors, ptCenterScenePosThis);

    m_labelDscr.m_polarCoorsToLinkedSelPt.m_fLength_px = lineFromSelPtSceneCoors.length();
    m_labelDscr.m_polarCoorsToLinkedSelPt.m_fAngle_degrees = lineSelPtSceneCoors.angleTo(lineFromSelPtSceneCoors);
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
void CGraphObjLabelGeometryDY::updateAnchorLines()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "updateAnchorLines",
        /* strAddInfo   */ "" );

    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);

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

    QLineF lineSelPtSceneCoors(ptSelPt1ScenePos, ptSelPt2ScenePos);

    // The distances to the parent's selection points must be calculated in scene coordinates.
    QRectF rctBoundingThis = getBoundingRect();
    QPointF ptCenterScenePosThis = pGraphicsItemThis->mapToScene(rctBoundingThis.center());

    // Horizontal lines.
    QPointF ptLineP1End(ptCenterScenePosThis.x(), lineSelPtSceneCoors.p1().y());
    QLineF lineP1(lineSelPtSceneCoors.p1(), ptLineP1End);
    QPointF ptLineP2End(ptCenterScenePosThis.x(), lineSelPtSceneCoors.p2().y());
    QLineF lineP2(lineSelPtSceneCoors.p2(), ptLineP2End);

    // As the anchor lines are painted by this object, they have to be translated into local coordinates.
    m_anchorLines[0] = QLineF(mapFromScene(lineP1.p1()), mapFromScene(lineP1.p2()));
    m_anchorLines[1] = QLineF(mapFromScene(lineP2.p1()), mapFromScene(lineP2.p2()));
    m_anchorLines[2] = QLineF(mapFromScene(lineP1.p2()), mapFromScene(lineP2.p2()));

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strRuntimeInfo = "AnchorLine0 {" + qLine2Str(m_anchorLines[0]) + "} px";
        mthTracer.trace(strRuntimeInfo, ELogDetailLevel::Debug, ELogDetailLevel::None);
        strRuntimeInfo = "AnchorLine1 {" + qLine2Str(m_anchorLines[1]) + "} px";
        mthTracer.trace(strRuntimeInfo, ELogDetailLevel::Debug, ELogDetailLevel::None);
        strRuntimeInfo = "AnchorLine2 {" + qLine2Str(m_anchorLines[2]) + "} px";
        mthTracer.trace(strRuntimeInfo, ELogDetailLevel::Debug, ELogDetailLevel::None);
    }

    getLineEndArrowPolygons(
        /* line          */ m_anchorLines[2],
        /* drawSetings   */ m_drawSettingsArrowHeads,
        /* pplgLineStart */ &m_plgP1ArrowHead,
        /* pplgLineEnd   */ &m_plgP2ArrowHead);
}
