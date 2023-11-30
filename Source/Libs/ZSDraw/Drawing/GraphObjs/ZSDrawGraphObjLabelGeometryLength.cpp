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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryLength.h"
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
class CGraphObjLabelGeometryLength : public CGraphObjLabel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjLabelGeometryLength::CGraphObjLabelGeometryLength(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strKey,
    const SGraphObjSelectionPoint& i_selPt1,
    const SGraphObjSelectionPoint& i_selPt2) :
//------------------------------------------------------------------------------
    CGraphObjLabel(
        /* pDrawingScene   */ i_pDrawingScene,
        /* strKey          */ i_strKey,
        /* strText         */ "Length",
        /* type            */ EGraphObjTypeLabelGeometryLength,
        /* selPt           */ i_selPt1,
        /* selPt           */ i_selPt2)
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

    m_anchorLines.append(QLineF());
    m_anchorLines.append(QLineF());
    m_anchorLines.append(QLineF());

} // ctor

//------------------------------------------------------------------------------
CGraphObjLabelGeometryLength::~CGraphObjLabelGeometryLength()
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

} // dtor

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the type method of QGraphicsItem.
*/
int CGraphObjLabelGeometryLength::type() const
//------------------------------------------------------------------------------
{
    return QGraphicsItem::UserType + EGraphObjTypeLabelGeometryLength;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjLabelGeometryLength::clone()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    CGraphObjLabelGeometryLength* pGraphObj = nullptr;
    return pGraphObj;
}

/*==============================================================================
protected: // auxiliary overridable instance methods of base class CGraphObjLabel
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief 
*/
void CGraphObjLabelGeometryLength::updatePosition()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updatePosition",
        /* strAddInfo   */ "" );

    m_bUpdatePositionInProgress = true;

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

    QPointF ptScenePosThis = m_pDrawingScene->convert(physValLine.center(), Units.Length.px).toQPointF();
    QRectF rctBoundingThis = QGraphicsSimpleTextItem::boundingRect();

    // As default the labels bottom left point is on the parent items selection point.
    // The position of the text item on the scene is the top left corner. So we
    // got to move the item correspondingly.
    ptScenePosThis.setX(ptScenePosThis.x() - rctBoundingThis.width()/2.0);
    ptScenePosThis.setY(ptScenePosThis.y() - rctBoundingThis.height()/2.0);

    // Move the label keeping the distance.
    ptScenePosThis.setX(ptScenePosThis.x() + m_labelDscr.m_distanceToLinkedSelPt.width());
    ptScenePosThis.setY(ptScenePosThis.y() + m_labelDscr.m_distanceToLinkedSelPt.height());

    setPos(ptScenePosThis);

    #pragma message(__TODO_"Ich bin muede")
    QLineF anchorLineP1_0 = m_labelDscr.m_selPt1.m_pGraphObj->getAnchorLineToSelectionPointFromPolar(20, 0, 0);
    QLineF anchorLineP2_0 = m_labelDscr.m_selPt2.m_pGraphObj->getAnchorLineToSelectionPointFromPolar(20, 0, 1);
    QLineF anchorLineP1_90 = m_labelDscr.m_selPt1.m_pGraphObj->getAnchorLineToSelectionPointFromPolar(20, 90, 0);
    QLineF anchorLineP2_90 = m_labelDscr.m_selPt2.m_pGraphObj->getAnchorLineToSelectionPointFromPolar(20, 90, 1);
    QLineF anchorLineP1_180 = m_labelDscr.m_selPt1.m_pGraphObj->getAnchorLineToSelectionPointFromPolar(20, 180, 0);
    QLineF anchorLineP2_180 = m_labelDscr.m_selPt2.m_pGraphObj->getAnchorLineToSelectionPointFromPolar(20, 180, 1);
    QLineF anchorLineP1_270 = m_labelDscr.m_selPt1.m_pGraphObj->getAnchorLineToSelectionPointFromPolar(20, 270, 0);
    QLineF anchorLineP2_270 = m_labelDscr.m_selPt2.m_pGraphObj->getAnchorLineToSelectionPointFromPolar(20, 270, 1);
    QLineF anchorLineP1_360 = m_labelDscr.m_selPt1.m_pGraphObj->getAnchorLineToSelectionPointFromPolar(20, 360, 0);
    QLineF anchorLineP2_360 = m_labelDscr.m_selPt2.m_pGraphObj->getAnchorLineToSelectionPointFromPolar(20, 360, 1);
    #pragma message(__TODO_"Ich bin muede")

    QString strText = physValLine.length().toString(EUnitFind::None, PhysValSubStr::Val);

    if (QGraphicsSimpleTextItem::text() != strText) {
        QGraphicsSimpleTextItem::setText(strText);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }

    // Update coordinates of the anchor line.
    updateAnchorLines();

    m_bUpdatePositionInProgress = false;
}

//------------------------------------------------------------------------------
/*! @brief 
*/
void CGraphObjLabelGeometryLength::updateDistanceToLinkedSelPt()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateDistanceToLinkedSelPt",
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

    QPointF ptSelScenePosParent = m_pDrawingScene->convert(physValLine.center(), Units.Length.px).toQPointF();

    QRectF rctBoundingThis = QGraphicsSimpleTextItem::boundingRect();
    QPointF ptCenterThis = rctBoundingThis.center();
    QPointF ptScenePosCenterThis = mapToScene(ptCenterThis);

    m_labelDscr.m_distanceToLinkedSelPt.setWidth(ptScenePosCenterThis.x() - ptSelScenePosParent.x());
    m_labelDscr.m_distanceToLinkedSelPt.setHeight(ptScenePosCenterThis.y() - ptSelScenePosParent.y());
}

//------------------------------------------------------------------------------
/*! @brief 
*/
void CGraphObjLabelGeometryLength::updateAnchorLines()
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
    QLineF line = m_pDrawingScene->convert(physValLine, Units.Length.px).toQLineF();

    // The distances to the parent's selection points must be calculated in scene coordinates.
    QRectF rctBoundingThis = QGraphicsSimpleTextItem::boundingRect();
    QPointF ptCenterScenePosThis = mapToScene(rctBoundingThis.center());
    // The shortest distance to the line from the labels center point is the perpendicular line
    // which goes through the center of the labels bounding rectangle.
    QLineF perpendicularLine = getPerpendicularLine(line, ptCenterScenePosThis);

    // We need two perpendicular lines at the start and end point of the line.
    // Create those two liney by moving the perpendicular line correspondingly.
    QPointF ptOffset1 = line.p1() - perpendicularLine.p2();
    QPointF ptOffset2 = line.p2() - perpendicularLine.p2();
    QLineF perpendicularLine1 = perpendicularLine.translated(ptOffset1);
    QLineF perpendicularLine2 = perpendicularLine.translated(ptOffset2);

    // But the anchor lines are painted by this object. For this they have to be
    // translated into local coordinates.
    m_anchorLines[0] = QLineF(mapFromScene(perpendicularLine1.p1()), mapFromScene(perpendicularLine1.p2()));
    m_anchorLines[1] = QLineF(mapFromScene(perpendicularLine2.p1()), mapFromScene(perpendicularLine2.p2()));
    m_anchorLines[2] = QLineF(mapFromScene(perpendicularLine1.p1()), mapFromScene(perpendicularLine2.p1()));
}
