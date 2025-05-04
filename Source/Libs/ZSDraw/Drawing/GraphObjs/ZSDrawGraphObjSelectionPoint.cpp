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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
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
class CGraphObjSelectionPoint : public CGraphObj, public QGraphicsEllipseItem
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
double CGraphObjSelectionPoint::defaultWidthInPx()
//------------------------------------------------------------------------------
{
    return s_fDefaultWidth_px;
}

/*==============================================================================
protected: // class members
==============================================================================*/

double CGraphObjSelectionPoint::s_fDefaultWidth_px = 7.0;
int CGraphObjSelectionPoint::s_iPenWidth_px = 2;
QColor CGraphObjSelectionPoint::s_colPen = Qt::black;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Constructor for selection points at (bounding) rectangles.
*/
CGraphObjSelectionPoint::CGraphObjSelectionPoint(
    CDrawingScene* i_pDrawingScene, const SGraphObjSelectionPoint& i_selPt) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ EGraphObjTypeSelectionPoint,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeSelectionPoint),
        /* strObjName          */ "SelPt." + i_selPt.name(),
        /* idxTreeEntryType    */ EEntryType::Leave ),
    QGraphicsEllipseItem(QRectF(-s_fDefaultWidth_px/2.0, -s_fDefaultWidth_px/2.0, s_fDefaultWidth_px, s_fDefaultWidth_px)),
    m_selPt(i_selPt),
    m_fWidth_px(s_fDefaultWidth_px),
    m_bUpdatePositionInProgress(false)
{
    createTraceAdminObjs("SelectionPoints::" + ClassName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_selPt.toString(true);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    // When setting the ItemIsSelectable Flag the application crashes. Somehow the item
    // is added to an internal group which is not accessible when forwarding the mouse
    // press events. Couldn't figure out what is wrong but anyway the selection point
    // doesn't need this flag.
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
           //| QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton | Qt::MiddleButton | Qt::XButton1 | Qt::XButton2);
    setAcceptHoverEvents(true);

    if (m_selPt.m_pGraphObj == nullptr) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }

    double fZValueParent = m_selPt.m_pGraphObj->getStackingOrderValue();
    setStackingOrderValue(fZValueParent + c_fStackingOrderOffsetSelectionPoints, ERowVersion::Original);

    QObject::connect(
        m_selPt.m_pGraphObj, &CGraphObj::geometryOnSceneChanged,
        this, &CGraphObjSelectionPoint::onGraphObjParentGeometryOnSceneChanged);
    QObject::connect(
        m_selPt.m_pGraphObj, &CGraphObj::zValueChanged,
        this, &CGraphObjSelectionPoint::onGraphObjParentZValueChanged);

} // ctor

//------------------------------------------------------------------------------
CGraphObjSelectionPoint::~CGraphObjSelectionPoint()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_bDtorInProgress = true;
    emit_aboutToBeDestroyed();

    // Selection points have been "directly" added to the graphics scene and not by
    // invoking CDrawingScene::addGraphObj so that they don't appear in the index tree.
    // For this labels "directly" remove themselves from the graphics scene.
    if (m_pDrawingScene != nullptr) {
        m_pDrawingScene->removeItem(this);
    }

    m_fWidth_px = 0.0;
    m_bUpdatePositionInProgress = false;

} // dtor

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the type method of QGraphicsItem.
*/
int CGraphObjSelectionPoint::type() const
//------------------------------------------------------------------------------
{
    return EGraphObjTypeSelectionPoint;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjSelectionPoint::clone()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );
    return nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjSelectionPoint::pathNameOfLinkedObject() const
//------------------------------------------------------------------------------
{
    QString strPath;
    if (m_selPt.m_pGraphObj != nullptr) {
        strPath = m_selPt.m_pGraphObj->path();
    }
    return strPath;
}

//------------------------------------------------------------------------------
QString CGraphObjSelectionPoint::myPathName() const
//------------------------------------------------------------------------------
{
    QString strPath = pathNameOfLinkedObject();
    if (m_selPt.m_pGraphObj != nullptr) {
        strPath = m_selPt.m_pGraphObj->tree()->buildPathStr(strPath, m_strName);
    }
    return strPath;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the type of the selection point, the linked object and the
           position at the linked object the selection point is linked to.

    Selection points are differentiated into selection points on the bounding
    rectangle around the graphical object or into polygon shape points.
*/
SGraphObjSelectionPoint CGraphObjSelectionPoint::getSelectionPoint() const
//------------------------------------------------------------------------------
{
    return m_selPt;
}

#if 0
//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::setShapePoint( int i_idxPt )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idxPt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "setShapePoint",
        /* strAddInfo   */ strMthInArgs );

    if (m_selPt.m_selPtType != ESelectionPointType::PolygonPoint) {
        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall);
    }
    m_selPt.m_idxPt = i_idxPt;
}
#endif

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::setWidthInPx( double i_fWidth_px )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fWidth_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "setWidthInPx",
        /* strAddInfo   */ strMthInArgs );

    if (m_fWidth_px != i_fWidth_px) {
        m_fWidth_px = i_fWidth_px;
    }
}

//------------------------------------------------------------------------------
double CGraphObjSelectionPoint::getWidthInPx() const
//------------------------------------------------------------------------------
{
    return m_fWidth_px;
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "OldSettings {" + i_drawSettingsOld.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "onDrawSettingsChanged",
        /* strAddInfo   */ strMthInArgs );
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the current bounding rectangle of the object.
*/
QRectF CGraphObjSelectionPoint::getBoundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "getBoundingRect",
        /* strAddInfo   */ "" );

    // Please note that the boundingRect call of QGraphicsLineItem als takes the pen width
    // into account. So we cannot call this method to get the real bounding rectangle of
    // the line if only the real shape points should be considered.
    QRectF rctBounding = rect();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the cursor to be shown if the mouse is hovered over the
           selection point.

    The cursor shape depends on the type of the selection point. If the parent
    object is rotated, the cursor shape is adjusted correspondingly.

    @param i_pt [in] Point to be check in local coordinates.
*/
QCursor CGraphObjSelectionPoint::getProposedCursor(const QPointF& i_pt) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCursor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Point {" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCursor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "getProposedCursor",
        /* strAddInfo   */ "" );

    QCursor cursor = Qt::CrossCursor;
    if (m_selPt.m_selPtType == ESelectionPointType::BoundingRectangle) {
        double fRotationAngle_degree = 0.0;
        if (m_selPt.m_pGraphObj != nullptr) {
            fRotationAngle_degree = 0.0; //m_selPt.m_pGraphObj->getRotationAngleInDegree();
        }
        cursor = selectionPoint2Cursor(m_selPt.m_selPt, fRotationAngle_degree);
    }
    else if (m_selPt.m_selPtType == ESelectionPointType::PolygonPoint) {
        cursor = Qt::CrossCursor;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qCursorShape2Str(cursor.shape()));
    }
    return cursor;
}

/*==============================================================================
pbulic: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjSelectionPoint::contains(const QPointF& i_pt) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCursor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCursor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "contains",
        /* strAddInfo   */ strMthInArgs );

    bool bContains = QGraphicsEllipseItem::contains(i_pt);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bContains);
    }
    return bContains;
}

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjSelectionPoint::boundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ "" );

    QRectF rctBounding = QGraphicsEllipseItem::boundingRect();
    rctBounding = QRectF(
        rctBounding.left() - m_drawSettings.penWidth()/2,
        rctBounding.top() - m_drawSettings.penWidth()/2,
        rctBounding.width() + m_drawSettings.penWidth(),
        rctBounding.height() + m_drawSettings.penWidth() );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
/*! @brief Reimplements QGraphicsItem::shape.
*/
QPainterPath CGraphObjSelectionPoint::shape() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "shape",
        /* strAddInfo   */ "" );
    QPainterPath painterPath = QGraphicsEllipseItem::shape();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        const QGraphicsItem* pCThis = static_cast<const QGraphicsItem*>(this);
        QGraphicsItem* pVThis = const_cast<QGraphicsItem*>(pCThis);
        QString strMthRet = qPainterPath2Str(pVThis, painterPath);
        mthTracer.setMethodReturn(strMthRet);
    }
    return painterPath;
}

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::paint(
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceDrawSettings(mthTracer, EMethodDir::Enter);
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
    }

    i_pPainter->save();

    QPen pn = pen();
    QBrush brsh = brush();

    pn.setWidth(s_iPenWidth_px);
    pn.setColor(s_colPen);
    brsh.setStyle(Qt::NoBrush);

    i_pPainter->setPen(pn);
    i_pPainter->setBrush(brsh);

    QGraphicsEllipseItem::paint(i_pPainter, i_pStyleOption, i_pWdgt);

    i_pPainter->restore();
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEnterLeaveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneHoverEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEnterLeaveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
    }

    QGraphicsItem_setCursor(getProposedCursor(i_pEv->pos()));

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverMoveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneHoverEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
    }

    QGraphicsItem_setCursor(getProposedCursor(i_pEv->pos()));

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEnterLeaveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneHoverEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEnterLeaveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "hoverLeaveEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
    }

    QGraphicsItem_unsetCursor();

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief

    @note The selection point will forward the mouse event to its parent object.
          By calling the mouse event method of the parent object, the selection point might
          be destroyed. This means that after invoking calling the parents mouse event method
          the selection point MUST NOT access any instance members anymore.
*/
void CGraphObjSelectionPoint::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem_setCursor(getProposedCursor(i_pEv->pos()));

    // We inform the linked object of the mouse press event as the graphics
    // scene will not forward the event even if we would ignore the event.
    QGraphicsItem* pGraphicsItemParent = dynamic_cast<QGraphicsItem*>(m_selPt.m_pGraphObj);
    if (pGraphicsItemParent != nullptr) {
        m_pDrawingScene->sendEvent(pGraphicsItemParent, i_pEv);
        // Note: By calling the mouse event method of the parent object, the selection point might
        //       be destroyed. This means that after invoking the parents mouse event method
        //       the selection point MUST NOT access any instance members anymore.
    }
    i_pEv->accept();

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
/*! @brief

    @note The selection point will forward the mouse event to its parent object.
          By calling the mouse event method of the parent object, the selection point might
          be destroyed. This means that after invoking calling the parents mouse event method
          the selection point MUST NOT access any instance members anymore.

    @note After dispatching the mouse release event to the mouse grabber item, the
          graphics scene will reset the mouse grabber. So it is useless trying to
          keep the mouse grabber within the item's mouseReleaseEvent method.
*/
void CGraphObjSelectionPoint::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem_unsetCursor();

    // We inform the linked object of the mouse press event as the graphics
    // scene will not forward the event even if we would ignore the event.
    QGraphicsItem* pGraphicsItemParent = dynamic_cast<QGraphicsItem*>(m_selPt.m_pGraphObj);
    if (pGraphicsItemParent != nullptr) {
        m_pDrawingScene->sendEvent(pGraphicsItemParent, i_pEv);
        // Note: By calling the mouse event method of the parent object, the selection point might
        //       be destroyed. This means that after invoking calling the parents mouse event method
        //       the selection point MUST NOT access any instance members anymore.
    }
    i_pEv->accept();

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
/*! @brief

    @note The selection point will forward the mouse event to its parent object.
          By calling the mouse event method of the parent object, the selection point might
          be destroyed. This means that after invoking calling the parents mouse event method
          the selection point MUST NOT access any instance members anymore.
*/
void CGraphObjSelectionPoint::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strMthInArgs );

    // When doubleclicking an item, the item will first receive a mouse
    // press event, followed by a release event (i.e., a click), then a
    // doubleclick event, and finally a release event.
    // The default implementation of "mouseDoubleClickEvent" calls "mousePressEvent".
    // This is not necessary here.
    //QGraphicsEllipseItem::mouseDoubleClickEvent(i_pEv);

    // We inform the linked object of the mouse press event as the graphics
    // scene will not forward the event even if we would ignore the event.
    QGraphicsItem* pGraphicsItemParent = dynamic_cast<QGraphicsItem*>(m_selPt.m_pGraphObj);
    if (pGraphicsItemParent != nullptr) {
        m_pDrawingScene->sendEvent(pGraphicsItemParent, i_pEv);
        // Note: By calling the mouse event method of the parent object, the selection point might
        //       be destroyed. This means that after invoking calling the parents mouse event method
        //       the selection point MUST NOT access any instance members anymore.
    }
    i_pEv->accept();

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseMoveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
    }

    QGraphicsItem_setCursor(getProposedCursor(i_pEv->pos()));
    setPos(i_pEv->scenePos());

    // Eat the event. Don't pass it to other objects otherwise the object,
    // the selection point is connected to, will also be moved and gets a
    // itemChanged(PositionChanged) call which will move the complete object
    // but not just the selection point to resize the object.
    i_pEv->accept();

    // The mouse move event is not forwarded to the linked object.
    // On the one hand the move is complete "eaten" by the selection point and
    // on the other hand this would have unwanted side effects like moving the
    // object which again would move the selection point and so on.

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphObjSelectionPoint::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
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
    if (areMethodCallsActive(pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsItemChange2Str(i_change, i_value);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "itemChange",
        /* strAddInfo   */ strMthInArgs );

    QVariant valChanged = i_value;

    bool bGeometryChanged = false;
    bool bTreeEntryChanged = false;

    if (i_change == ItemSceneHasChanged) {
        // The item may have been removed from the scene.
        if (scene() != nullptr) {
            updatePosition();
            bGeometryChanged = true;
        }
    }
    else if (i_change == ItemParentHasChanged) {
        bGeometryChanged = true;
    }
    else if (i_change == ItemPositionHasChanged) {
        bGeometryChanged = true;
    }

    if (bGeometryChanged) {
        emit_geometryOnSceneChanged();
    }
    if (bTreeEntryChanged) {
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }

    valChanged = QGraphicsItem::itemChange(i_change, i_value);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthRet = qGraphicsItemChange2Str(i_change, valChanged, false);
        mthTracer.setMethodReturn(strMthRet);
    }
    return valChanged;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::onGraphObjParentGeometryOnSceneChanged(
    CGraphObj* i_pGraphObjParent, bool i_bParentOfParentChanged)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObjParent->keyInTree() + ", ParentOfParentChanged: " + bool2Str(i_bParentOfParentChanged);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "onGraphObjParentGeometryOnSceneChanged",
        /* strAddInfo   */ strMthInArgs );

    updatePosition();
}

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::onGraphObjParentZValueChanged(CGraphObj* i_pGraphObjParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObjParent->keyInTree();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "onGraphObjParentZValueChanged",
        /* strAddInfo   */ strMthInArgs );

    // The selectin point should be drawn after the parent object is drawn.
    double fZValueParent = m_selPt.m_pGraphObj->getStackingOrderValue();
    setStackingOrderValue(fZValueParent + c_fStackingOrderOffsetSelectionPoints);
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the position of the label and the
           coordinates (start and end point) of the anchor line.

    On moving the label the current distance to the parent item is stored.
    If the geometry of the parent item changes (position moved or size changed or
    any other geometry change) the label must be moved so that the distance remains
    the same.
*/
void CGraphObjSelectionPoint::updatePosition()
//------------------------------------------------------------------------------
{
    // "setPos" leads to a "itemChange" call with ItemPositionChange(d) whereupon
    // "updatePosition" would be called again. This reentry is not desired.
    if (m_bUpdatePositionInProgress) {
        return;
    }
    m_bUpdatePositionInProgress = true;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "updatePosition",
        /* strAddInfo   */ "" );

    QPointF ptSelScenePosParent;
    if (m_selPt.m_selPtType == ESelectionPointType::BoundingRectangle) {
        ptSelScenePosParent = m_selPt.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
            m_selPt.m_selPtType, m_selPt.m_selPt);
    }
    else {
        ptSelScenePosParent = m_selPt.m_pGraphObj->getPositionOfSelectionPointInSceneCoors(
            m_selPt.m_selPtType, m_selPt.m_idxPt);
    }
    setPos(ptSelScenePosParent);

    m_bUpdatePositionInProgress = false;
}
