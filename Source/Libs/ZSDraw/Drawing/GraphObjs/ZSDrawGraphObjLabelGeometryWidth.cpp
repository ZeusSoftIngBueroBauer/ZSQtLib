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
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjEllipse.h"
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
    const SGraphObjSelectionPoint& i_selPt) :
//------------------------------------------------------------------------------
    CGraphObjLabel(
        /* pDrawingScene */ i_pDrawingScene,
        /* strKey        */ i_strKey,
        /* type          */ EGraphObjTypeLabelGeometryWidth,
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

    // Anchor0 and Anchor1 are the extension lines from the linked object to the dimension line ends.
    m_anchorLines.append(QLineF());
    m_anchorLines.append(QLineF());
    m_arrowHeads.append(QPolygonF());
    // Anchor2 and Anchor3 are the dimension lines from the extension lines to the label.
    m_anchorLines.append(QLineF());
    m_anchorLines.append(QLineF());
    m_arrowHeads.append(QPolygonF());
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
}

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
protected: // auxiliary overridable instance methods of base class CGraphObjLabel
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief
*/
void CGraphObjLabelGeometryWidth::updateText()
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
        QPointF ptSelPtLeftScenePos;
        QPointF ptSelPtRightScenePos;

        // The center point of the label could be within, above or below
        // of the bounding rectangle of the linked object. The selection point, for which
        // the polar coordinates are calculated, was set correspondingly.
        if (m_selPtLinkedObject.m_selPt == ESelectionPoint::TopCenter) { // above
            ptSelPtLeftScenePos =
                m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                    ESelectionPointType::BoundingRectangle, ESelectionPoint::TopLeft);
            ptSelPtRightScenePos =
                m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                    ESelectionPointType::BoundingRectangle, ESelectionPoint::TopRight);
        }
        else if (m_selPtLinkedObject.m_selPt == ESelectionPoint::BottomCenter) { // below
            ptSelPtLeftScenePos =
                m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                    ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomLeft);
            ptSelPtRightScenePos =
                m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                    ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomRight);
        }
        else { // if (rctBoundingLinkedObject.contains(ptCenterThisInLinkedObjectCoors)) // within
            // If the center point of the label is within the bounding rectangle of the linked object,
            // the extension lines (anchor0 and anchor1) will not be drawn and are not visible.
            ptSelPtLeftScenePos =
                m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                    ESelectionPointType::BoundingRectangle, ESelectionPoint::LeftCenter);
            ptSelPtRightScenePos =
                m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                    ESelectionPointType::BoundingRectangle, ESelectionPoint::RightCenter);
        }
        QLineF lineSelPtsSceneCoors(ptSelPtLeftScenePos, ptSelPtRightScenePos);
        const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
        CPhysVal physValWidth(lineSelPtsSceneCoors.length(), Units.Length.px, drawingSize.imageCoorsResolutionInPx());
        physValWidth.convertValue(drawingSize.unit());
        strText = physValWidth.toString(EUnitFind::None, PhysValSubStr::Val);
    }
    setText(strText);
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the coordinates (start and end point)
           of the anchor lines.

    For the Width label four anchor lines are drawn. Two vertical extension lines
    at the left and right border of the item's bounding rectangle and two horizontal
    dimension lines between the text item and the end points of the vertical extension lines.

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

         Anchor0 and Anchor1 are the extension lines of the dimension line.
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceThisPositionInfo(mthTracer, EMethodDir::Enter);
    }
    CRefCountGuard refCountGuardTrace(&m_iTraceBlockedCounter);
    if (m_selPtLinkedObject.m_pGraphObj != nullptr) {
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
        QLineF lineLeftRectBoundingLinkedObject(
            rctBoundingLinkedObject.topLeft(), rctBoundingLinkedObject.bottomLeft());
        QLineF linePerpendicularToLeftBorder = getPerpendicularLine(
            lineLeftRectBoundingLinkedObject, ptCenterThisInLinkedObjectCoors, 0.0, &anchorLine0Pt1InLinkedObjectCoors);
        QLineF lineRightRectBoundingLinkedObject(
            rctBoundingLinkedObject.topRight(), rctBoundingLinkedObject.bottomRight());
        QLineF linePerpendicularToRightBorder = getPerpendicularLine(
            lineRightRectBoundingLinkedObject, ptCenterThisInLinkedObjectCoors, 0.0, &anchorLine1Pt1InLinkedObjectCoors);

        // The center point of the label could be within, above or below
        // of the bounding rectangle of the linked object.
        // The anchor lines will only be drawn if the center point is outside the bounding rectangle.
        // An exception are ellipsis for which the anchor lines are always drawn to the center point
        // of the bounding rectangle lines.
        if (m_selPtLinkedObject.m_pGraphObj->type() == EGraphObjTypeEllipse) {
            CGraphObjEllipse* pGraphObjEllipse = dynamic_cast<CGraphObjEllipse*>(m_selPtLinkedObject.m_pGraphObj);
            anchorLine0Pt2InLinkedObjectCoors = QPointF(
                rctBoundingLinkedObject.left(), rctBoundingLinkedObject.center().y());
            anchorLine1Pt2InLinkedObjectCoors = QPointF(
                rctBoundingLinkedObject.right(), rctBoundingLinkedObject.center().y());
        }
        else {
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
            /* pplgLineStart */ &m_arrowHeads[0],
            /* pplgLineEnd   */ nullptr);
        getLineEndArrowPolygons(
            /* line          */ m_anchorLines[3],
            /* drawSetings   */ m_drawSettingsArrowHeads,
            /* pplgLineStart */ &m_arrowHeads[1],
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
void CGraphObjLabelGeometryWidth::traceThisPositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, const QString& i_strFilter) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTracePositionInfoBlockedCounter > 0 || m_iTraceThisPositionInfoInfoBlockedCounter > 0) {
        return;
    }

    CGraphObjLabel::traceThisPositionInfo(i_mthTracer, i_mthDir, i_strFilter);
}
