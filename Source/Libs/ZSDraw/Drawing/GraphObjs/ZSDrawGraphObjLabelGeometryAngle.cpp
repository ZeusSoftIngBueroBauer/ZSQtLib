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
    const SGraphObjSelectionPoint& i_selPt) :
//------------------------------------------------------------------------------
    CGraphObjLabel(
        /* pDrawingScene */ i_pDrawingScene,
        /* strKey        */ i_strKey,
        /* type          */ EGraphObjTypeLabelGeometryAngle,
        /* selPt         */ i_selPt)
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

    // Anchor0 and Anchor1 are the extension lines from the center of the linked object
    // to the end of the rectangle pie.
    m_anchorLines.append(QLineF());
    m_anchorLines.append(QLineF());
    m_arrowHeads.append(QPolygonF());
}

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
}

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

    QRectF rctBounding = CGraphObjLabel::boundingRect();
    int iPenWidth = m_drawSettings.penWidth();
    if ((m_pDrawingScene->getMode() == EMode::Edit) && (m_bIsHighlighted || isSelected())) {
        iPenWidth += 3; // see paint method
    }
    QRectF rctPie = m_rectPie;
    rctPie = QRectF(
        rctPie.left() - static_cast<double>(iPenWidth)/2.0,
        rctPie.top() - static_cast<double>(iPenWidth)/2.0,
        rctPie.width() + static_cast<double>(iPenWidth),
        rctPie.height() + static_cast<double>(iPenWidth));
    rctBounding |= rctPie;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
        QRectF rctBoundingSceneCoors = pGraphicsItemThis->mapToScene(rctBounding).boundingRect();
        mthTracer.setMethodReturn("Item {" + qRect2Str(rctBounding) + "}, Scene {" + qRect2Str(rctBoundingSceneCoors) + "}");
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceThisPositionInfo(mthTracer, EMethodDir::Enter);
    }
    CRefCountGuard refCountGuardTrace(&m_iTraceBlockedCounter);

    CGraphObjLabel::paint(i_pPainter, i_pStyleOption, i_pWdgt);

    // Draw anchor line (circle segment) to indicate the angle if the label is hit,
    // hovered, selected or if the anchor line is set to be visible.
    // Use a different color if the line is hit, hovered or selected.
    if (/*m_bIsHit ||*/ isSelected() || m_bShowAnchorLine) {
        i_pPainter->save();
        // The startAngle and spanAngle must be specified in 1/16th of a degree.
        // The painter counts angles counter clockwise whereas the graphics system
        // counts angles clockwise. Correct the angles before drawing the pies.
        if ((fabs(m_rectPie.width()) >= 20.0) || (fabs(m_rectPie.height()) >= 20.0)) {
            double fPieStartAngle_degrees = Math::toCounterClockWiseAngleDegree(m_fPieStartAngle_degrees);
            double fPieSpanAngle_degrees = -m_fPieSpanAngle_degrees;
            i_pPainter->drawPie(m_rectPie, 16.0 * fPieStartAngle_degrees, 16.0 * fPieSpanAngle_degrees);
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
void CGraphObjLabelGeometryAngle::updateText()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateText",
        /* strAddInfo   */ "");

    QString strText;
    if (m_selPtLinkedObject.m_pGraphObj == nullptr) {
        strText = "?";
    }
    else {
        strText = m_selPtLinkedObject.m_pGraphObj->rotationAngle().toString();
    }
    setText(strText);
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the coordinates (start and end point)
           of the anchor lines.

    The angle label does not use the common anchor lines but draws the anchor lines
    as a circle segment (pie).

    Example for a Rectangle (rotated by 270°):

                TopRight               BottomRight
                    +-----------------------+
                    |          270°         |
                    |    +----->|           |
                    |   /     Anchor1       |
                    |  /        |           |
                    x |         x-Anchor0-+ x (Start Angle of Pie)
                    |  \                  / |
                    |   \                /  |
                    |    +--------------+   |
                    |                       |
                    +-----------------------+
                TopLeft                BottomLeft

        Anchor0: Start boundary line of the angular dimensioning
        Anchor1: End boundary line of the angular dimensioning
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceThisPositionInfo(mthTracer, EMethodDir::Enter);
    }
    CRefCountGuard refCountGuardTrace(&m_iTraceBlockedCounter);
    if (m_selPtLinkedObject.m_pGraphObj != nullptr) {
        QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
        QRectF rctBoundingThis = getBoundingRect();
        QPointF ptThisCenterScenePos = pGraphicsItemThis->mapToScene(rctBoundingThis.center());
        SGraphObjSelectionPoint selPtLinkedObjectBoundingRectCenter(
            m_selPtLinkedObject.m_pGraphObj, ESelectionPointType::BoundingRectangle, ESelectionPoint::Center);
        QPointF ptCenterLinkedObjectSceneCoors =
            getPositionOfLinkedSelectionPointInSceneCoors(selPtLinkedObjectBoundingRectCenter);
        // The length of the start and end boundary lines is the distance betweeen this
        // label's center and the center point of the lined object's bounding rectangle.
        // Note! To avoid that the line goes through the text of the angle label, the radius
        // may be changed depending on the angle (for angles < 90° the top left corner could be
        // the start point, for angles > 90° the top right corner, etc.).
        QLineF lineLinkedObjectCenterToThisCenterInSceneCoors(ptCenterLinkedObjectSceneCoors, ptThisCenterScenePos);
        double fDistanceToLinkedObjectCenter = lineLinkedObjectCenterToThisCenterInSceneCoors.length();

        CPhysVal physValAngle = m_selPtLinkedObject.m_pGraphObj->rotationAngle();
        double fAngle_degrees = physValAngle.getVal(Units.Angle.Degree);
        CPhysVal physValAngleSceneCoors_degrees = getEffectiveRotationAngleOfLinkedObjectInSceneCoors();
        double fAngleSceneCoors_degrees = physValAngleSceneCoors_degrees.getVal(Units.Angle.Degree);

        // Anchor0 is the start boundary of the angular dimensioning at 0°.
        // The boundary line starts in the center of the linked object with the angle
        // of 0° in the coordinate system of the linked object's parent.
        QLineF anchorLine0InSceneCoors = getLineFromPolar(
            ptCenterLinkedObjectSceneCoors,
            SPolarCoors(fDistanceToLinkedObjectCenter, fAngleSceneCoors_degrees - fAngle_degrees));

        // Anchor1 is the end boundary of the angular dimensionang at the angle to be indicated.
        // The boundary line starts in the center of the linked object with the rotation angle
        // of the linked object in the coordinate system of the linked object's parent.
        QLineF anchorLine1InSceneCoors = getLineFromPolar(
            ptCenterLinkedObjectSceneCoors, 
            SPolarCoors(fDistanceToLinkedObjectCenter, fAngleSceneCoors_degrees));

        m_anchorLines[0] = QLineF(mapFromScene(anchorLine0InSceneCoors.p1()), mapFromScene(anchorLine0InSceneCoors.p2()));
        m_anchorLines[1] = QLineF(mapFromScene(anchorLine1InSceneCoors.p1()), mapFromScene(anchorLine1InSceneCoors.p2()));

        //QLineF lineFromLinkedObjectCenter(m_lineSelPtsSceneCoors.p1(), ptThisCenterScenePos);
        //lineFromLinkedObjectCenter.setLength(lineFromLinkedObjectCenter.length() - rctBoundingThis.width()/2.0);
        m_rectPie.setWidth(2.0 * fDistanceToLinkedObjectCenter);
        m_rectPie.setHeight(2.0 * fDistanceToLinkedObjectCenter);
        m_rectPie.moveCenter(mapFromScene(ptCenterLinkedObjectSceneCoors));

        // The angle returned by QLineF is counter-clockwise with 0° at 3 o'clock.
        // The graphics scene uses clockwise angles with 0° at 3 o'clock.
        m_fPieStartAngle_degrees = fAngleSceneCoors_degrees - fAngle_degrees;
        m_fPieSpanAngle_degrees = fAngleSceneCoors_degrees;
        // The scenes rotation angle of the parent group of the linked object must be subtracted
        // to get the start point of the pie and the span has to be corrected correspondingly.
        //CGraphObjGroup* pGraphObjParentGroup = m_selPtLinkedObject.m_pGraphObj->parentGroup();
        //if (pGraphObjParentGroup != nullptr) {
        //    double fParentGroupSceneRotationAngle_degrees =
        //        pGraphObjParentGroup->getEffectiveRotationAngleOnScene(Units.Angle.Degree).getVal();
        //    m_fPieStartAngle_degrees = fParentGroupSceneRotationAngle_degrees;
        //    m_fPieStartAngle_degrees = Math::normalizeAngleInDegree(m_fPieStartAngle_degrees);
        //    m_fPieSpanAngle_degrees -= fParentGroupSceneRotationAngle_degrees;
        //    m_fPieSpanAngle_degrees = Math::normalizeAngleInDegree(m_fPieSpanAngle_degrees);
        //}

        QLineF perpendicularLineEndOfSegment = ZS::Draw::getPerpendicularLine(
            m_anchorLines[1], m_anchorLines[1].p2(), 10.0);

        getLineEndArrowPolygons(
            /* line          */ perpendicularLineEndOfSegment,
            /* drawSetings   */ m_drawSettingsArrowHeads,
            /* pplgLineStart */ &m_arrowHeads[0],
            /* pplgLineEnd   */ nullptr);
    }
    refCountGuardTrace.decrementAndReleaseCounter();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceThisPositionInfo(mthTracer, EMethodDir::Leave);
    }
}

/*==============================================================================
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabelGeometryAngle::traceThisPositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, const QString& i_strFilter) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTracePositionInfoBlockedCounter > 0 || m_iTraceThisPositionInfoInfoBlockedCounter > 0) {
        return;
    }

    CGraphObjLabel::traceThisPositionInfo(i_mthTracer, i_mthDir, i_strFilter);

    QString strRuntimeInfo;
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . ";
    strRuntimeInfo += "Pie {" + qRect2Str(m_rectPie) + "}" +
        ", Start: " + QString::number(m_fPieStartAngle_degrees) + Math::c_chSymbolDegree + 
        ", Span: " + QString::number(m_fPieSpanAngle_degrees) + Math::c_chSymbolDegree;
    i_mthTracer.trace(strRuntimeInfo);
}
