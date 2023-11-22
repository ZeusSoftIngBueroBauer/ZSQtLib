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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelLength.h"
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
class CGraphObjLabelLength : public CGraphObjLabel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjLabelLength::CGraphObjLabelLength(
    CDrawingScene* i_pDrawingScene,
    CGraphObj* i_pGraphObjParent,
    const QString& i_strKey,
    const SGraphObjSelectionPoint& i_selPt) :
//------------------------------------------------------------------------------
    CGraphObjLabel(
        /* pDrawingScene   */ i_pDrawingScene,
        /* pGraphObjParent */ i_pGraphObjParent,
        /* strKey          */ i_strKey,
        /* strText         */ "Length",
        /* selPt           */ i_selPt,
        /* type            */ EGraphObjTypeLabelLength)
{
    createTraceAdminObjs("Labels::" + ClassName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs =
            "Parent: " + QString(i_pGraphObjParent == nullptr ? "null" : i_pGraphObjParent->keyInTree()) +
            ", Key: " + i_strKey +
            ", SelPt: " + i_selPt.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

} // ctor

//------------------------------------------------------------------------------
CGraphObjLabelLength::~CGraphObjLabelLength()
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
int CGraphObjLabelLength::type() const
//------------------------------------------------------------------------------
{
    return QGraphicsItem::UserType + EGraphObjTypeLabelLength;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjLabelLength::clone()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    CGraphObjLabelLength* pGraphObj = nullptr;
    return pGraphObj;
}

/*==============================================================================
protected: // auxiliary overridable instance methods of base class CGraphObjLabel
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the position of the label and the
           coordinates (start and end point) of the anchor line.

    On moving the label the current distance to the parent item is stored.
    If the geometry of the parent item changes (position moved or size changed or
    any other geometry change) the label must be moved so that the distance remains
    the same.
*/
void CGraphObjLabelLength::updatePosition()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updatePosition",
        /* strAddInfo   */ "" );

    CGraphObjLabel::updatePosition();

    m_bUpdatePositionInProgress = true;
    m_bUpdatePositionInProgress = false;
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the distance between the labels center
           point and the selection point of the parent item the label is linked to.

    On moving the label the current distance to the parent item is stored.
    If the geometry of the parent item changes (position moved or size changed or
    any other geometry change) the label must be moved so that the distance remains
    the same.
*/
void CGraphObjLabelLength::updateDistanceToLinkedSelPt()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateDistanceToLinkedSelPt",
        /* strAddInfo   */ "" );

    CGraphObjLabel::updateDistanceToLinkedSelPt();
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the coordinates (start and end point)
           of the anchor line.

    The anchor line will be drawn to one of the center points at the bounding rectangle.
    Which line of the labels bounding rectangle should be used depends on the relative
    position of the label to the selection point of the graph object the label is linked to.

    "QLineF::angle" is used to calculate the angle of the line between the two anchor points.

    From Qts documentation:
    -----------------------
    The return value will be in the range of values from 0.0 up
    to but not including 360.0. The angles are measured counter-clockwise from
    a point on the x-axis to the right of the origin (x > 0).
    The following diagram should also clarify whats been returned by "QLineF::angle":

                     90°
       135°    16     1     2    45°
            15        |        3
          14      +---+---+      4
    180° 13-------| Label |-------5   0°  (360°)
          12      +---+---+      6
            11        |        7
       225°    10     9     8   315°
                     270°

    Selection Point Position | clockwise | Selectoin Point
    of "Parent Item"         |           | of Label
    -------------------------+-----------+-----------------
    16, 1, 2                 | 135°-45°  | TopCenter
    3, 4, 5, 6, 7            |  45°-315° | RightCenter
    8, 9, 10                 | 315°-225° | BottomCenter
    11, 12, 13, 14, 15       | 225°-135° | LeftCenter

    If the angle is calculated the distance between the linked selection point
    of the parent item to the anchor line will also be taken into account.
    E.g. if the label is very close to the parent item it is better not to draw
    the anchor line.
*/
//------------------------------------------------------------------------------
void CGraphObjLabelLength::updateAnchorLine()
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateAnchorLine",
        /* strAddInfo   */ "" );

    CGraphObjLabel::updateAnchorLine();
}
