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
        /* pDrawingScene   */ i_pDrawingScene,
        /* strKey          */ i_strKey,
        /* strText         */ "Angle",
        /* type            */ EGraphObjTypeLabelGeometryAngle,
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
    return QGraphicsItem::UserType + EGraphObjTypeLabelGeometryAngle;
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
    QString strText = physValLine.angle().toString(EUnitFind::None, PhysValSubStr::Val);
    if (QGraphicsSimpleTextItem::text() != strText) {
        QGraphicsSimpleTextItem::setText(strText);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }

    // First set the text at the graphics item so that the
    // bounding rectangle includes the new text.
    CGraphObjLabel::updatePosition();
}

//------------------------------------------------------------------------------
/*! @brief 
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

    CGraphObjLabel::updateAnchorLines();
}
