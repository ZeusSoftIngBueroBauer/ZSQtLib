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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryDX.h"
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
class CGraphObjLabelGeometryDX : public CGraphObjLabel
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

QPainter::RenderHints CGraphObjLabelGeometryDX::s_painterRenderHints = QPainter::Antialiasing;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QPainter::RenderHints CGraphObjLabelGeometryDX::painterRenderHints()
//------------------------------------------------------------------------------
{
    return s_painterRenderHints;
}

//------------------------------------------------------------------------------
void CGraphObjLabelGeometryDX::setPainterRenderHints(QPainter::RenderHints i_renderHints)
//------------------------------------------------------------------------------
{
    s_painterRenderHints = i_renderHints;
}

//------------------------------------------------------------------------------
void CGraphObjLabelGeometryDX::resetPainterRenderHints()
//------------------------------------------------------------------------------
{
    s_painterRenderHints = QPainter::Antialiasing;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjLabelGeometryDX::CGraphObjLabelGeometryDX(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strKey,
    const SGraphObjSelectionPoint& i_selPt) :
//------------------------------------------------------------------------------
    CGraphObjLabel(
        /* pDrawingScene */ i_pDrawingScene,
        /* strKey        */ i_strKey,
        /* type          */ EGraphObjTypeLabelGeometryDX,
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
CGraphObjLabelGeometryDX::~CGraphObjLabelGeometryDX()
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
int CGraphObjLabelGeometryDX::type() const
//------------------------------------------------------------------------------
{
    return EGraphObjTypeLabelGeometryDX;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjLabelGeometryDX::clone()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    CGraphObjLabelGeometryDX* pGraphObj = nullptr;
    return pGraphObj;
}

/*==============================================================================
protected: // auxiliary overridable instance methods of base class CGraphObjLabel
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief
*/
void CGraphObjLabelGeometryDX::updateText()
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
        QPointF ptSelPt1ScenePos;
        QPointF ptSelPt2ScenePos;
        if (m_selPtLinkedObject.m_selPtType == ESelectionPointType::BoundingRectangle) {
            ptSelPt1ScenePos =
                m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                    ESelectionPointType::BoundingRectangle, ESelectionPoint::LeftCenter);
            ptSelPt2ScenePos =
                m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                    ESelectionPointType::BoundingRectangle, ESelectionPoint::RightCenter);
        }
        else if (m_selPtLinkedObject.m_selPtType == ESelectionPointType::LineCenterPoint) {
            ptSelPt1ScenePos =
                m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                    ESelectionPointType::PolygonPoint, m_selPtLinkedObject.m_idxPtOrLine);
            ptSelPt2ScenePos =
                m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                    ESelectionPointType::PolygonPoint, m_selPtLinkedObject.m_idxPtOrLine + 1);
        }
        QLineF lineSelPtSceneCoors(ptSelPt1ScenePos, ptSelPt2ScenePos);
        const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
        CPhysValLine physValLine(*m_pDrawingScene, lineSelPtSceneCoors, Units.Length.px);
        physValLine = m_pDrawingScene->convert(physValLine);
        strText = physValLine.dx().toString(EUnitFind::None, PhysValSubStr::Val);
    }
    setText(strText);
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the coordinates (start and end point)
           of the anchor lines.

    The anchor lines will be drawn to one of the selection points at the bounding rectangle
    or to one of the polygon shape points of the linked graphical object.

    For the Width label three anchor lines are drawn. Two vertical lines through
    selection point 1 and selection point 2 and a horizontal between the end points
    of the vertical lines.

    Example for a diagonal line:

             P1
              +
       Anchor |\
       Line0  | \
              |  \ AnchorLine2
              |<--x-->|
                   \  |
                    \ | Anchor
                     \| Line1
                      +
                      P2
*/
void CGraphObjLabelGeometryDX::updateAnchorLines()
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
    QPointF ptCenterScenePosThis = pGraphicsItemThis->mapToScene(rctBoundingThis.center());

    QPointF ptSelPt1ScenePos;
    QPointF ptSelPt2ScenePos;

    if (m_selPtLinkedObject.m_selPtType == ESelectionPointType::BoundingRectangle) {
        ptSelPt1ScenePos =
            m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                ESelectionPointType::BoundingRectangle, ESelectionPoint::LeftCenter);
        ptSelPt2ScenePos =
            m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                ESelectionPointType::BoundingRectangle, ESelectionPoint::RightCenter);
    }
    else if (m_selPtLinkedObject.m_selPtType == ESelectionPointType::LineCenterPoint) {
        ptSelPt1ScenePos =
            m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                ESelectionPointType::PolygonPoint, m_selPtLinkedObject.m_idxPtOrLine);
        ptSelPt2ScenePos =
            m_selPtLinkedObject.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
                ESelectionPointType::PolygonPoint, m_selPtLinkedObject.m_idxPtOrLine+1);
    }

    QLineF lineSelPtSceneCoors(ptSelPt1ScenePos, ptSelPt2ScenePos);

    // Vertical lines.
    QPointF ptLineP1End(lineSelPtSceneCoors.p1().x(), ptCenterScenePosThis.y());
    QLineF lineP1(lineSelPtSceneCoors.p1(), ptLineP1End);
    QPointF ptLineP2End(lineSelPtSceneCoors.p2().x(), ptCenterScenePosThis.y());
    QLineF lineP2(lineSelPtSceneCoors.p2(), ptLineP2End);

    // As the anchor lines are painted by this object, they have to be translated into local coordinates.
    m_anchorLines[0] = QLineF(mapFromScene(lineP1.p1()), mapFromScene(lineP1.p2()));
    m_anchorLines[1] = QLineF(mapFromScene(lineP2.p1()), mapFromScene(lineP2.p2()));
    m_anchorLines[2] = QLineF(mapFromScene(lineP1.p2()), ptCenterThis);
    m_anchorLines[3] = QLineF(mapFromScene(lineP2.p2()), ptCenterThis);
    m_anchorLines[2].setLength(m_anchorLines[2].length() - rctBoundingThis.width()/2.0);
    m_anchorLines[3].setLength(m_anchorLines[3].length() - rctBoundingThis.width()/2.0);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strRuntimeInfo = "AnchoredTo {" + m_selPtLinkedObject.toString() + "}" +
            ", PolarCoors {" + m_polarCoorsToLinkedSelPt.toString() + "}";
        mthTracer.trace(strRuntimeInfo, ELogDetailLevel::Debug, ELogDetailLevel::None);
    }

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

    refCountGuardTrace.decrementAndReleaseCounter();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceThisPositionInfo(mthTracer, EMethodDir::Leave);
    }
}

/*==============================================================================
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabelGeometryDX::traceThisPositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, const QString& i_strFilter) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTracePositionInfoBlockedCounter > 0 || m_iTraceThisPositionInfoInfoBlockedCounter > 0) {
        return;
    }

    CGraphObjLabel::traceThisPositionInfo(i_mthTracer, i_mthDir, i_strFilter);
}
