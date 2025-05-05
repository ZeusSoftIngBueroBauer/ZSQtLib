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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionLine.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjConnectionLinePropertiesDlg.h"
#include "ZSDraw/Common/ZSDrawAux.h"
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
class CGraphObjConnectionLine : public CGraphObjPolygon
*******************************************************************************/

/*==============================================================================
public: // class members
==============================================================================*/

qint64 CGraphObjConnectionLine::s_iInstCount = 0;

/*==============================================================================
protected: // class members
==============================================================================*/

QPainter::RenderHints CGraphObjConnectionLine::s_painterRenderHints = QPainter::Antialiasing;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QPainter::RenderHints CGraphObjConnectionLine::painterRenderHints()
//------------------------------------------------------------------------------
{
    return s_painterRenderHints;
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::setPainterRenderHints(QPainter::RenderHints i_renderHints)
//------------------------------------------------------------------------------
{
    s_painterRenderHints = i_renderHints;
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::resetPainterRenderHints()
//------------------------------------------------------------------------------
{
    s_painterRenderHints = QPainter::Antialiasing;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjConnectionLine::CGraphObjConnectionLine(
    CDrawingScene* i_pDrawingScene, const QString& i_strObjName ) :
//------------------------------------------------------------------------------
    CGraphObjPolygon(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameConnections,
        /* type                */ EGraphObjTypeConnectionLine,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeConnectionLine),
        /* strObjName          */ i_strObjName.isEmpty() ? "ConnectionLine" + QString::number(s_iInstCount) : i_strObjName)
{
    // Just incremented by the ctor but not decremented by the dtor.
    // Used to create a unique name for newly created objects of this type.
    s_iInstCount++;

    createTraceAdminObjs("Connections::" + ClassName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ObjName: " + i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_strlstPredefinedLabelNames.append(c_strLabelName);
    addLabel(c_strLabelName, i_strObjName, ESelectionPointType::BoundingRectangle, ESelectionPoint::Center);

    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameCenter);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameWidth);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameHeight);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameAngle);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameTopLeft);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameTopRight);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameBottomRight);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameBottomLeft);

    for (const QString& strLabelName : m_strlstGeometryLabelNames) {
        if (strLabelName == c_strGeometryLabelNameCenter) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::Center);
        }
        else if (strLabelName == c_strGeometryLabelNameWidth) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryLength, ESelectionPoint::LeftCenter, ESelectionPoint::RightCenter);
        }
        else if (strLabelName == c_strGeometryLabelNameHeight) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryLength, ESelectionPoint::TopCenter, ESelectionPoint::BottomCenter);
        }
        else if (strLabelName == c_strGeometryLabelNameAngle) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryAngle, ESelectionPoint::LeftCenter, ESelectionPoint::RightCenter);
        }
        else if (strLabelName == c_strGeometryLabelNameTopLeft) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::TopLeft);
        }
        else if (strLabelName == c_strGeometryLabelNameTopRight) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::TopRight);
        }
        else if (strLabelName == c_strGeometryLabelNameBottomRight) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::BottomRight);
        }
        else if (strLabelName == c_strGeometryLabelNameBottomLeft) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::BottomLeft);
        }
    }

    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable
            |QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
    setAcceptHoverEvents(true);
}

//------------------------------------------------------------------------------
CGraphObjConnectionLine::~CGraphObjConnectionLine()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_bDtorInProgress = true;
    emit_aboutToBeDestroyed();

    for (ELinePoint linePoint : m_arpCnctPts.keys()) {
        CGraphObjConnectionPoint* pGraphObjCnctPt = m_arpCnctPts.value(linePoint);
        if (pGraphObjCnctPt != nullptr) {
            pGraphObjCnctPt->removeConnectionLine(this);
        }
        m_arpCnctPts.remove(linePoint);
    }
}

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the type method of QGraphicsItem.
*/
int CGraphObjConnectionLine::type() const
//------------------------------------------------------------------------------
{
    return EGraphObjTypeConnectionLine;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjConnectionLine::clone()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );
    return nullptr;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/* @brief

    Must be overridden to create a user defined dialog.
*/
void CGraphObjConnectionLine::openFormatGraphObjsDialog()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "openFormatGraphObjsDialog",
        /* strAddInfo   */ "" );

    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Format Connection Line";
    CDlgGraphObjConnectionLineProperties* pDlg = CDlgGraphObjConnectionLineProperties::GetInstance(this);
    if( pDlg == nullptr ) {
        pDlg = CDlgGraphObjConnectionLineProperties::CreateInstance(strDlgTitle, this);
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->setModal(false);
        pDlg->show();
    }
    else {
        if( pDlg->isHidden() ) {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Connects the specified line point with the specified connection point
           by appending the line to the connection point.

    @return true if the specified line point is connected with the specified connection point,
            false on error (e.g. if the line is already connected with the specified connection point).
*/
bool CGraphObjConnectionLine::setConnectionPoint(ELinePoint i_linePoint, CGraphObjConnectionPoint* i_pCnctPt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumLinePoint(i_linePoint).toString();
        strMthInArgs += ", CnctPoint: " + QString(i_pCnctPt == nullptr ? "nullptr" : i_pCnctPt->name());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setConnectionPoint",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceConnectionPoints(mthTracer, EMethodDir::Enter);
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter);
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
        traceThisPositionInfo(mthTracer, EMethodDir::Enter);
    }

    bool bConnected = false;

    if (i_pCnctPt != nullptr && (i_linePoint == ELinePoint::Start || i_linePoint == ELinePoint::End)) {
        CGraphObjConnectionPoint* pGraphObjCnctPt = m_arpCnctPts.value(i_linePoint, nullptr);
        if (pGraphObjCnctPt == i_pCnctPt) {
            bConnected = true;
        }
        else {
            if (pGraphObjCnctPt != nullptr) {
                pGraphObjCnctPt->removeConnectionLine(this);
                pGraphObjCnctPt = nullptr;
                m_arpCnctPts.remove(i_linePoint);
            }
            bConnected = i_pCnctPt->appendConnectionLine(this);
            if (bConnected) {
                m_arpCnctPts[i_linePoint] = i_pCnctPt;
                pGraphObjCnctPt = m_arpCnctPts[i_linePoint];
            }
            else {
                pGraphObjCnctPt = nullptr;
            }
        }
        if (pGraphObjCnctPt != nullptr) {
            QGraphicsItem* pGraphicsItemCnctPt = dynamic_cast<QGraphicsItem*>(pGraphObjCnctPt);
            QPolygonF plg = polygon();
            ELinePoint linePoint = getConnectionLinePoint(pGraphObjCnctPt);
            QPointF ptCnctPtPos = pGraphObjCnctPt->rect().center();
            QPointF ptCnctPtScenePos = pGraphicsItemCnctPt->mapToScene(ptCnctPtPos);
            if (linePoint == ELinePoint::Start) {
                QPointF pt(0.0,0.0);
                if (plg.size() == 0) {
                    plg.append(pt);
                }
                else {
                    plg[0] = pt;
                }
                QGraphicsPolygonItem::setPolygon(plg);
                setPos(ptCnctPtScenePos);
            }
            else if (linePoint == ELinePoint::End && plg.size() >= 1) {
                QPointF pt = mapFromScene(ptCnctPtScenePos);
                if (plg.size() == 1) {
                    plg.append(pt);
                }
                else {
                    int idxPt = plg.size()-1;
                    plg[idxPt] = pt;
                }
                QGraphicsPolygonItem::setPolygon(plg);
            }
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceConnectionPoints(mthTracer, EMethodDir::Leave);
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave);
        traceGraphObjStates(mthTracer, EMethodDir::Leave);
        traceThisPositionInfo(mthTracer, EMethodDir::Leave);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bConnected);
    }
    return bConnected;
}

//------------------------------------------------------------------------------
/*! @brief Returns the line point (start or end) which is connected to the specified connection point.
*/
ELinePoint CGraphObjConnectionLine::getConnectionLinePoint(CGraphObjConnectionPoint* i_pCnctPt) const
//------------------------------------------------------------------------------
{
    ELinePoint linePoint = ELinePoint::None;
    for (ELinePoint linePointTmp : m_arpCnctPts.keys()) {
        CGraphObjConnectionPoint* pGraphObjCnctPt = m_arpCnctPts[linePointTmp];
        if (pGraphObjCnctPt != nullptr && pGraphObjCnctPt == i_pCnctPt) {
            linePoint = linePointTmp;
            break;
        }
    }
    return linePoint;
}

//------------------------------------------------------------------------------
CGraphObjConnectionPoint* CGraphObjConnectionLine::getConnectionPoint(ELinePoint i_linePoint) const
//------------------------------------------------------------------------------
{
    return m_arpCnctPts.value(i_linePoint, nullptr);
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
///*! @brief Called by the parent graphic item to inform the child about geometry changes.
//
//    Connection lines don't belong to groups. But their connection points do.
//    If a group is moved also the connection points are moved by Qt's graphics scene.
//    But not the connection lines which are linked to the connection points. The
//    connection points can be considered as parents of connection lines and
//    "onGraphObjParentGeometryOnSceneChanged" is called by the connection points if their
//    position changes.
//*/
//void CGraphObjConnectionLine::onGraphObjParentGeometryOnSceneChanged(
//    CGraphObj* i_pGraphObjParent, bool i_bParentOfParentChanged)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_pGraphObjParent->keyInTree() + ", ParentOfParentChanged: " + bool2Str(i_bParentOfParentChanged);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "onGraphObjParentGeometryOnSceneChanged",
//        /* strAddInfo   */ strMthInArgs );
//
//    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
//    CGraphObjConnectionPoint* pGraphObjCnctPt = dynamic_cast<CGraphObjConnectionPoint*>(i_pGraphObjParent);
//    if (pGraphObjCnctPt != nullptr) {
//        QGraphicsItem* pGraphicsItemCnctPt = dynamic_cast<QGraphicsItem*>(pGraphObjCnctPt);
//        QPolygonF plg = polygon();
//        ELinePoint linePoint = getConnectionLinePoint(pGraphObjCnctPt);
//        QPointF ptCnctPtPos = pGraphObjCnctPt->rect().center();
//        QPointF ptCnctPtScenePos = pGraphicsItemCnctPt->mapToScene(ptCnctPtPos);
//        QPointF ptItemPosNew = mapFromScene(ptCnctPtScenePos);
//        if (linePoint == ELinePoint::Start && plg.size() > 0) {
//            // The origin of the poly line's coordinate system (the item's position)
//            // is the first line point (in poly line's item's coordinate system the
//            // first line point is at 0/0). If the first line point is moved it's
//            // coordinates remain at 0/0 but all other points must be moved to fit into
//            // the new item's coordinate system.
//            QPointF ptItemPosOld = plg[0];
//            setPos(ptCnctPtScenePos);
//            double dx = ptItemPosNew.x() - ptItemPosOld.x();
//            double dy = ptItemPosNew.y() - ptItemPosOld.y();
//            QPointF ptPosOld, ptPosNew;
//            for (int idxPt = 1; idxPt < plg.size(); idxPt++) {
//                ptPosOld = plg[idxPt];
//                ptPosNew.setX( ptPosOld.x() - dx );
//                ptPosNew.setY( ptPosOld.y() - dy );
//                plg[idxPt] = ptPosNew;
//            }
//            QGraphicsPolygonItem::setPolygon(plg);
//            for (int idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++) {
//                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
//                if (pGraphObjSelPt != nullptr) {
//                    QPointF ptSel = plg[idxSelPt];
//                    ptSel = pGraphicsItemThis->mapToScene(ptSel);
//                    pGraphObjSelPt->setPos(ptSel);
//                }
//            }
//        }
//        else if (linePoint == ELinePoint::End && plg.size() > 1) {
//            int idxSelPt = plg.size()-1;
//            plg[idxSelPt] = ptItemPosNew;
//            QGraphicsPolygonItem::setPolygon(plg);
//            if (idxSelPt >= 0 && idxSelPt < m_arpSelPtsPolygon.size()) {
//                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
//                if (pGraphObjSelPt != nullptr) {
//                    QPointF ptSel = plg[idxSelPt];
//                    ptSel = pGraphicsItemThis->mapToScene(ptSel);
//                    pGraphObjSelPt->setPos(ptSel);
//                }
//            }
//        }
//        normalize();
//        m_bCoorsDirty = true;
//        updateLineEndPolygonCoors();
//        //updateEditInfo();
//        //updateToolTip();
//    }
//
//    // If the geometry of the parent on the scene of this item changes, also the geometry
//    // on the scene of this item is changed.
//    emit_geometryOnSceneChanged();
//
//} // onGraphObjParentGeometryOnSceneChanged

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEnterLeaveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEnterLeaveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strMthInArgs );
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverMoveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strMthInArgs );
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEnterLeaveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEnterLeaveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverLeaveEvent",
        /* strAddInfo   */ strMthInArgs );

    unsetCursor();
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strMthInArgs );
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strMthInArgs );

    // When doubleclicking an item, the item will first receive a mouse
    // press event, followed by a release event (i.e., a click), then a
    // doubleclick event, and finally a release event.

    // The default implementation of "mouseDoubleClickEvent" calls mousePressEvent().
    // This is not necessary here.
    //QGraphicsPolygonItem::mouseDoubleClickEvent(i_pEv);

//    CEnumMode modeDrawing = m_pDrawingScene->getMode();
//
//    CEnumEditMode editModePrev = m_editMode;
//    int           idxSelPtSelectedPolygon = m_idxSelPtSelectedPolygon;
//
//    // Reset before calling "drawingScene->onGraphObjCreationFinished".
//    m_editMode = EEditMode::None;
//    m_editResizeMode = EEditResizeMode::None;
//    m_idxSelPtSelectedPolygon = -1;
//    m_selPtSelectedBoundingRect = ESelectionPoint::None;
//
//    if( modeDrawing == EMode::Edit )
//    {
//        if( editModePrev == EEditMode::Creating || editModePrev == EEditMode::MoveShapePoint )
//        {
//            QPolygonF plg = polygon();
//            QPointF   ptMouseItemPos = i_pEv->pos();
//
//            if( plg.size() >= 2 ) // anyway ..
//            {
//                CGraphObjConnectionPoint* pGraphObjCnctPt;
//                CGraphObjSelectionPoint*  pGraphObjSelPt;
//                int                       idxSelPt;
//                QPointF                   ptSel;
//
//                // If the first or the last shape point is selected ..
//                if( idxSelPtSelectedPolygon == 0 || idxSelPtSelectedPolygon == plg.size()-1 )
//                {
//                    // The currently selected shape point will be fixed.
//                    plg[idxSelPtSelectedPolygon] = ptMouseItemPos;
//
//                    // .. it depends whether the shape point has been moved onto a connection point
//                    // or somewhere else whether the connection line will be connected with the
//                    // connection point or whether a new shape points will be inserted or added.
//                    pGraphObjCnctPt = m_pDrawingScene->getConnectionPoint(i_pEv->scenePos());
//
//                    // If the mouse has not been released on a connection point ..
//                    if( pGraphObjCnctPt == nullptr )
//                    {
//                        // Another point will be added. In addition the connection line
//                        // should remain connected with the previous connection point.
//                        // For this a further point will be added which will be placed
//                        // upon the connection point.
//                        if( idxSelPtSelectedPolygon == 0 )
//                        {
//                            pGraphObjCnctPt = m_arpCnctPts[static_cast<int>(ELinePoint::Start)];
//
//                            plg.insert(0,ptMouseItemPos);
//
//                            pGraphObjSelPt = new CGraphObjSelectionPoint(
//                                m_pDrawingScene, SGraphObjSelectionPoint(this, idxSelPtSelectedPolygon));
//                            m_arpSelPtsPolygon.insert(0, pGraphObjSelPt);
//                            QObject::connect(
//                                pGraphObjSelPt, &CGraphObj::aboutToBeDestroyed,
//                                this, &CGraphObjConnectionLine::onSelectionPointAboutToBeDestroyed);
//
//                            m_pDrawingScene->addGraphObj(pGraphObjSelPt);
//
//                            //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
//                            pGraphObjSelPt->installSceneEventFilter(this);
//
//                            ptSel = plg[idxSelPtSelectedPolygon];
//                            ptSel = mapToScene(ptSel);
//                            pGraphObjSelPt->setPos(ptSel);
//
//                            plg.insert(0,ptMouseItemPos);
//
//                            pGraphObjSelPt = new CGraphObjSelectionPoint(
//                                m_pDrawingScene, SGraphObjSelectionPoint(this, idxSelPtSelectedPolygon));
//                            m_arpSelPtsPolygon.insert(0, pGraphObjSelPt);
//                            QObject::connect(
//                                pGraphObjSelPt, &CGraphObj::aboutToBeDestroyed,
//                                this, &CGraphObjConnectionLine::onSelectionPointAboutToBeDestroyed);
//
//                            m_pDrawingScene->addGraphObj(pGraphObjSelPt);
//
//                            //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
//                            pGraphObjSelPt->installSceneEventFilter(this);
//
//                            ptSel = plg[idxSelPtSelectedPolygon];
//                            ptSel = mapToScene(ptSel);
//                            pGraphObjSelPt->setPos(ptSel);
//
//                        } // if( idxSelPtSelectedPolygon == 0 )
//
//                        else // if( idxSelPtSelectedPolygon == plg.size()-1 )
//                        {
//                            pGraphObjCnctPt = m_arpCnctPts[static_cast<int>(ELinePoint::End)];
//
//                            plg.append(ptMouseItemPos);
//                            idxSelPtSelectedPolygon = plg.size()-1;
//
//                            pGraphObjSelPt = new CGraphObjSelectionPoint(
//                                m_pDrawingScene, SGraphObjSelectionPoint(this, idxSelPtSelectedPolygon));
//                            m_arpSelPtsPolygon.append(pGraphObjSelPt);
//                            QObject::connect(
//                                pGraphObjSelPt, &CGraphObj::aboutToBeDestroyed,
//                                this, &CGraphObjConnectionLine::onSelectionPointAboutToBeDestroyed);
//
//                            m_pDrawingScene->addGraphObj(pGraphObjSelPt);
//
//                            //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
//                            pGraphObjSelPt->installSceneEventFilter(this);
//
//                            ptSel = plg[idxSelPtSelectedPolygon];
//                            ptSel = mapToScene(ptSel);
//                            pGraphObjSelPt->setPos(ptSel);
//
//                            plg.append(ptMouseItemPos);
//                            idxSelPtSelectedPolygon = plg.size()-1;
//
//                            pGraphObjSelPt = new CGraphObjSelectionPoint(
//                                m_pDrawingScene, SGraphObjSelectionPoint(this, idxSelPtSelectedPolygon));
//                            m_arpSelPtsPolygon.append(pGraphObjSelPt);
//                            QObject::connect(
//                                pGraphObjSelPt, &CGraphObj::aboutToBeDestroyed,
//                                this, &CGraphObjConnectionLine::onSelectionPointAboutToBeDestroyed);
//
//                            m_pDrawingScene->addGraphObj(pGraphObjSelPt);
//
//                            //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
//                            pGraphObjSelPt->installSceneEventFilter(this);
//
//                            ptSel = plg[idxSelPtSelectedPolygon];
//                            ptSel = mapToScene(ptSel);
//                            pGraphObjSelPt->setPos(ptSel);
//                        }
//
//                        QPointF ptItemPosOld = pos();
//                        QPointF ptItemPosNew = mapToScene(plg[0]);
//
//                        double  dx, dy;
//                        int     idxPt;
//
//                        setPos(ptItemPosNew);
//
//                        dx = ptItemPosNew.x() - ptItemPosOld.x();
//                        dy = ptItemPosNew.y() - ptItemPosOld.y();
//
//                        QPointF ptPosOld, ptPosNew;
//
//                        for( idxPt = 0; idxPt < plg.size(); idxPt++ )
//                        {
//                            ptPosOld = plg[idxPt];
//                            ptPosNew.setX( ptPosOld.x() - dx );
//                            ptPosNew.setY( ptPosOld.y() - dy );
//                            plg[idxPt] = ptPosNew;
//                        }
//
//                        QGraphicsPolygonItem::setPolygon(plg);
//
//                        for( idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++ )
//                        {
//                            pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
//
//                            if( pGraphObjSelPt != nullptr )
//                            {
//                                pGraphObjSelPt->setShapePoint(idxSelPt);
//
//                                if( idxSelPt == idxSelPtSelectedPolygon )
//                                {
//                                    pGraphObjSelPt->setSelected(true);
//                                }
//                                else
//                                {
//                                    pGraphObjSelPt->setSelected(false);
//                                }
//                            }
//                        }
//
//                    } // if( pGraphObjCnctPt == nullptr )
//
//                    // If the mouse has been released on a connection point
//                    // or if the end point has been corrected to remain on
//                    // the previous connection point ..
//                    if( pGraphObjCnctPt != nullptr )
//                    {
//                        if( idxSelPtSelectedPolygon == 0 )
//                        {
//                            if( pGraphObjCnctPt != m_arpCnctPts[static_cast<int>(ELinePoint::Start)] )
//                            {
//                                setConnectionPoint(ELinePoint::Start,pGraphObjCnctPt);
//                            }
//                            else
//                            {
//                                onGraphObjParentGeometryOnSceneChanged(pGraphObjCnctPt);
//                            }
//                        }
//                        else // if( idxSelPtSelectedPolygon == plg.size()-1 )
//                        {
//                            if( pGraphObjCnctPt != m_arpCnctPts[static_cast<int>(ELinePoint::End)] )
//                            {
//                                setConnectionPoint(ELinePoint::End,pGraphObjCnctPt);
//                            }
//                            else
//                            {
//                                onGraphObjParentGeometryOnSceneChanged(pGraphObjCnctPt);
//                            }
//                        }
//
//                    } // if( pGraphObjCnctPt != nullptr )
//
//                } // if( idxSelPtSelectedPolygon == 0 || idxSelPtSelectedPolygon == plg.size()-1 )
//
//                // Before the double click event a mouse press event occurs creating an additional
//                // point which is not desired. This point got to be removed again. We are going
//                // to remove all succeeding points overlapping each other which will also remove the
//                // last undesired point.
//                normalize();
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//                acceptCurrentAsOriginalCoors();
//#endif
//                m_bCoorsDirty = true;
//
//                updateLineEndPolygonCoors();
//
//                // The object has been initially created.
//                if( editModePrev == EEditMode::Creating )
//                {
//                    //m_pDrawingScene->onGraphObjCreationFinished(this);
//                }
//                else // if( editModePrev == EEditMode::MoveShapePoint )
//                {
//                    // Editing shape points has been finished:
//                    m_pDrawingScene->onGraphObjAddingShapePointsFinished(this);
//                }
//
//                //updateEditInfo();
//                //updateToolTip();
//
//            } // if( plg.size() >= 2 ) // anyway ..
//
//        } // if( editModePrev == EEditMode::Creating || editModePrev == EEditMode::MoveShapePoint )
//
//        else if( isSelected() )
//        {
//            openFormatGraphObjsDialog();
//        }
//
//    } // if( modeDrawing == EMode::Edit )

} // mouseDoubleClickEvent

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseMoveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphObjConnectionLine::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
//------------------------------------------------------------------------------
{
    if (m_bDtorInProgress) {
        return i_value;
    }
    if (m_iItemChangeBlockedCounter > 0) {
        return i_value;
    }

    CTrcAdminObj* pTrcAdminObj = selectTraceAdminObj(i_change);
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsItemChange2Str(i_change, i_value);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "itemChange",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
    }

    QVariant valChanged = i_value;

    bool bGeometryChanged = false;
    bool bSelectedChanged = false;
    bool bZValueChanged = false;
    bool bTreeEntryChanged = false;

    if (i_change == ItemSceneHasChanged) {
        // The item may have been removed from the scene.
        if (scene() != nullptr) {
            if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                tracePositionInfo(mthTracer, EMethodDir::Enter);
            }
            updateLineEndArrowHeadPolygons();
            bGeometryChanged = true;
            bTreeEntryChanged = true;
        }
    }
    else if (i_change == ItemParentHasChanged) {
        if (m_iItemChangeUpdatePhysValCoorsBlockedCounter == 0) {
        }
        updateLineEndArrowHeadPolygons();
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemPositionHasChanged) {
        if (m_iItemChangeUpdatePhysValCoorsBlockedCounter == 0) {
            if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                tracePositionInfo(mthTracer, EMethodDir::Enter);
            }
            // Update the object shape point in parent coordinates kept in the unit of the drawing scene.
            // If the item is not a group and as long as the item is not added as a child to
            // a group, the current (transformed) and original coordinates are equal.
            // If the item is a child of a group, the current (transformed) coordinates are only
            // taken over as the original coordinates if initially creating the item or when
            // adding the item to or removing the item from a group.
            updateTransformedCoorsOnItemPositionChanged();
            bGeometryChanged = true;
        }
        updateLineEndArrowHeadPolygons();
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemRotationHasChanged) {
        tracePositionInfo(mthTracer, EMethodDir::Enter);
        bGeometryChanged = true;
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemSelectedHasChanged) {
        //QGraphicsItem_prepareGeometryChange();
        if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
            bringToFront();
            if (m_editMode == EEditMode::CreatingByMouseEvents || m_editMode == EEditMode::ModifyingPolygonPoints) {
                showSelectionPoints(c_uSelectionPointsPolygonPoints);
                hideSelectionPoints(c_uSelectionPointsBoundingRectAll);
            }
            else if (m_editMode == EEditMode::ModifyingBoundingRect) {
                hideSelectionPoints(c_uSelectionPointsPolygonPoints);
                showSelectionPoints(c_uSelectionPointsBoundingRectAll);
            }
            else /*if (m_editMode == EEditMode::None)*/ {
                hideSelectionPoints();
            }
        }
        else {
            setEditMode(EEditMode::None);
            hideSelectionPoints();
            resetStackingOrderValueToOriginalValue(); // restore ZValue as before selecting the object
        }
        bSelectedChanged = true;
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemZValueHasChanged) {
        bZValueChanged = true;
        bTreeEntryChanged = true;
    }

    if (bGeometryChanged) {
        if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
            tracePositionInfo(mthTracer, EMethodDir::Leave);
        }
        emit_geometryOnSceneChanged();
    }
    if (bSelectedChanged) {
        emit_selectedChanged(isSelected());
    }
    if (bZValueChanged) {
        emit_zValueChanged(zValue());
    }
    if (bTreeEntryChanged && m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }

    valChanged = QGraphicsPolygonItem::itemChange(i_change, i_value);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthRet = qGraphicsItemChange2Str(i_change, valChanged, false);
        mthTracer.setMethodReturn(strMthRet);
    }
    return valChanged;
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::traceConnectionPoints(CMethodTracer& i_mthTracer, EMethodDir i_mthDir) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTraceConnectionPointsBlockedCounter > 0) {
        return;
    }

    QString strRuntimeInfo;
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . ";
    strRuntimeInfo += "ConnectionPoints [" + QString::number(m_arpCnctPts.size()) + "]";
    if (!m_arpCnctPts.isEmpty()) {
        strRuntimeInfo += "(";
        for (ELinePoint linePoint : m_arpCnctPts.keys()) {
            const CGraphObjConnectionPoint* pGraphObjCnctPt = m_arpCnctPts[linePoint];
            if (!strRuntimeInfo.endsWith("(")) strRuntimeInfo += ", ";
            if (pGraphObjCnctPt == nullptr) {
                strRuntimeInfo += CEnumLinePoint(linePoint).toString() + ": null";
            }
            else {
                strRuntimeInfo += CEnumLinePoint(linePoint).toString() + ": " + pGraphObjCnctPt->path();
            }
        }
        strRuntimeInfo += ")";
    }
    i_mthTracer.trace(strRuntimeInfo);
}
