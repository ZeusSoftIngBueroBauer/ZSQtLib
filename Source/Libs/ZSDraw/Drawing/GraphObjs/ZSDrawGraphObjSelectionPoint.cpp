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
double CGraphObjSelectionPoint::GetDefaultRadiusInPx()
//------------------------------------------------------------------------------
{
    return s_fRadius_px;
}

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjSelectionPoint::s_iInstCount = 0;
double CGraphObjSelectionPoint::s_fRadius_px = 3.0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Constructor for selection points at (bounding) rectangles.
*/
CGraphObjSelectionPoint::CGraphObjSelectionPoint(
    CDrawingScene* i_pDrawingScene,
    CGraphObj* i_pGraphObjParent,
    ESelectionPoint i_selectionPoint) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ EGraphObjTypeSelectionPoint,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeSelectionPoint),
        /* strObjName          */ "SelPt." + CEnumSelectionPoint(i_selectionPoint).toString(),
        /* drawSettings        */ CDrawSettings(),
        /* idxTreeEntryType    */ EEntryType::Leave ),
    QGraphicsEllipseItem(QRectF(-s_fRadius_px, -s_fRadius_px, 2.0*s_fRadius_px, 2.0*s_fRadius_px)),
    m_selPtType(ESelectionPointType::BoundingRectangle),
    m_selPt(i_selectionPoint),
    m_idxPt(-1),
    m_fRadius_px(s_fRadius_px),
    m_bSelected(false),
    m_bUpdatePositionInProgress(false)
{
    s_iInstCount++; // not really used

    createTraceAdminObjs("SelectionPoints::" + ClassName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs =
            "ObjName: " + m_strName +
            ", Parent: " + QString(i_pGraphObjParent == nullptr ? "null" : i_pGraphObjParent->keyInTree()) +
            ", SelPt: " + CEnumSelectionPoint(i_selectionPoint).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_selPtSelectedBoundingRect = i_selectionPoint;

    //setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemSendsGeometryChanges);

    setAcceptHoverEvents(true);

    setParentGraphObj(i_pGraphObjParent);

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Undefined,
            m_pTrcAdminObjCtorsAndDtor->getRuntimeInfoTraceDetailLevel());
    }
} // ctor

//------------------------------------------------------------------------------
/*! @brief Constructor for selection points at polygon shape points.
*/
CGraphObjSelectionPoint::CGraphObjSelectionPoint(
    CDrawingScene* i_pDrawingScene,
    CGraphObj* i_pGraphObjParent,
    int i_idxPt) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ EGraphObjTypeSelectionPoint,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeSelectionPoint),
        /* strObjName          */ "SelPt." + QString::number(i_idxPt),
        /* drawSettings        */ CDrawSettings(),
        /* idxTreeEntryType    */ EEntryType::Leave ),
    QGraphicsEllipseItem(QRectF(-s_fRadius_px, -s_fRadius_px, 2.0*s_fRadius_px, 2.0*s_fRadius_px)),
    m_selPtType(ESelectionPointType::PolygonShapePoint),
    m_selPt(ESelectionPoint::None),
    m_idxPt(i_idxPt),
    m_fRadius_px(s_fRadius_px),
    m_bSelected(false),
    m_bUpdatePositionInProgress(false)
{
    s_iInstCount++; // not really used

    createTraceAdminObjs("SelectionPoints::" + ClassName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs =
            "ObjName: " + m_strName +
            ", Parent: " + QString(i_pGraphObjParent == nullptr ? "null" : i_pGraphObjParent->keyInTree()) +
            ", IdxPt: " + QString::number(i_idxPt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_idxSelPtSelectedPolygon = i_idxPt;

    //setFlags(/*QGraphicsItem::ItemIsMovable|*/QGraphicsItem::ItemIsSelectable/*|QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemSendsGeometryChanges*/);

    setAcceptHoverEvents(true);

    setParentGraphObj(i_pGraphObjParent);

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Undefined,
            m_pTrcAdminObjCtorsAndDtor->getRuntimeInfoTraceDetailLevel());
    }
} // ctor

//------------------------------------------------------------------------------
CGraphObjSelectionPoint::~CGraphObjSelectionPoint()
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

    // Selection points have been "directly" added to the graphics scene and not by
    // invoking CDrawingScene::addGraphObj so that they don't appear in the index tree.
    // For this labels "directly" remove themselves from the graphics scene.
    if (scene() != nullptr) {
        scene()->removeItem(this);
    }

    m_selPtType = static_cast<ESelectionPointType>(0);
    m_selPt =  static_cast<ESelectionPoint>(0);
    m_idxPt = 0;
    m_fRadius_px = 0.0;
    m_bSelected = false;
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
    return QGraphicsItem::UserType + EGraphObjTypeSelectionPoint;
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    CGraphObjSelectionPoint* pGraphObj = nullptr;
    return pGraphObj;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the type of the selection point.

    Selection points are differentiated into selection points on the bounding
    rectangle around the graphical object or into polygon shape points.
*/
ESelectionPointType CGraphObjSelectionPoint::getSelectionPointType() const
//------------------------------------------------------------------------------
{
    return m_selPtType.enumerator();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::setSelectionPoint( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjBoundingRect, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "SelPt:" + CEnumSelectionPoint(i_selPt).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setSelectionPoint",
        /* strAddInfo   */ strMthInArgs );

    m_selPt = i_selPt;
}

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::setShapePointIndex( int i_idxPt )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjBoundingRect, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Idx:" + QString::number(i_idxPt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setShapePointIndex",
        /* strAddInfo   */ strMthInArgs );

    m_idxPt = i_idxPt;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::setRadiusInPx( double i_fRadius_px )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjBoundingRect, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Radius:" + QString::number(i_fRadius_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setRadiusInPx",
        /* strAddInfo   */ strMthInArgs );

    if (m_fRadius_px != i_fRadius_px) {
        m_fRadius_px = i_fRadius_px;

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
        m_rctCurr = rect();
        m_ptRotOriginCurr = m_rctCurr.center();
        m_rctCurr.setLeft(m_ptRotOriginCurr.x()-m_fRadius_px);
        m_rctCurr.setTop(m_ptRotOriginCurr.y()-m_fRadius_px);
        m_rctCurr.setWidth(2.0*m_fRadius_px);
        m_rctCurr.setHeight(2.0*m_fRadius_px);

        setRect(m_rctCurr);
#endif
    }
}

/*==============================================================================
public: // instance methods (replacing the methods of base class QGraphicsItem)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::setSelected( bool i_bSelected )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjBoundingRect, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Selected:" + bool2Str(i_bSelected);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setSelected",
        /* strAddInfo   */ strMthInArgs );

    if (m_bSelected != i_bSelected) {
        m_bSelected = i_bSelected;
        update();
    }
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjSelectionPoint::getScenePolygonShapePointsString() const
//------------------------------------------------------------------------------
{
    QPolygonF plgScene = mapToScene(rect());
    return polygon2Str(plgScene);
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "onDrawSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.isPenUsed()) {
        if (m_drawSettings.getLineStyle() != ELineStyle::NoLine) {
            QPen pen;
            pen.setColor( m_drawSettings.getPenColor() );
            pen.setWidth( m_drawSettings.getPenWidth() );
            pen.setStyle( lineStyle2QtPenStyle(m_drawSettings.getLineStyle()) );
            setPen(pen);
        }
        else {
            setPen(Qt::NoPen);
        }
    }
    else {
        setPen(Qt::NoPen);
    }

    if (m_drawSettings.isFillUsed()) {
        if (m_drawSettings.getFillStyle() != EFillStyle::NoFill) {
            QBrush brsh;
            brsh.setColor( m_drawSettings.getFillColor() );
            brsh.setStyle( fillStyle2QtBrushStyle(m_drawSettings.getFillStyle()) );
            setBrush(brsh);
        }
        else {
            setBrush(Qt::NoBrush);
        }
    }
    else {
        setBrush(Qt::NoBrush);
    }
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::setWidth( const CPhysVal& i_physValWidth )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValWidth.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setWidth",
        /* strAddInfo   */ strMthInArgs );

    setSize(i_physValWidth, getHeight(i_physValWidth.unit()));
}

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::setHeight( const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValHeight.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setHeight",
        /* strAddInfo   */ strMthInArgs );

    setSize(getWidth(i_physValHeight.unit()), i_physValHeight);
}

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::setSize( const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValWidth.toString() + ", " + i_physValHeight.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setSize",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();

    CPhysVal physValWidth = i_physValWidth;
    CPhysVal physValHeight = i_physValHeight;

    physValWidth.convertValue(drawingSize.unit());
    physValHeight.convertValue(drawingSize.unit());

    if (hasFixedWidth() && physValWidth != getFixedWidth(drawingSize.unit())) {
        physValWidth = getFixedWidth(drawingSize.unit());
    }
    else {
        if (hasMinimumWidth() && physValWidth > getMinimumWidth(drawingSize.unit())) {
            physValWidth = getMinimumWidth(drawingSize.unit());
        }
        if (hasMaximumWidth() && physValWidth > getMaximumWidth(drawingSize.unit())) {
            physValWidth = getMaximumWidth(drawingSize.unit());
        }
    }

    if (hasFixedHeight() && physValHeight != getFixedHeight(drawingSize.unit())) {
        physValHeight = getFixedHeight(drawingSize.unit());
    }
    else {
        if (hasMinimumHeight() && physValHeight > getMinimumHeight(drawingSize.unit())) {
            physValHeight = getMinimumHeight(drawingSize.unit());
        }
        if (hasMaximumHeight() && physValHeight > getMaximumHeight(drawingSize.unit())) {
            physValHeight = getMaximumHeight(drawingSize.unit());
        }
    }

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    if( fWidth != m_rctCurr.width() )
    {
        setRadiusInPx( fabs(fWidth) );
    }
#endif

} // setSize

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::setSize( const CPhysValSize& i_physValSize )
//------------------------------------------------------------------------------
{
    setSize(i_physValSize.width(), i_physValSize.height());
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjSelectionPoint::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthOutArgs;
    if (areMethodCallsActive(m_pTrcAdminObjIsHit, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Point:" + point2Str(i_pt) +
            ", HitInfo, " + QString(o_pHitInfo == nullptr ? "null" : pointer2Str(o_pHitInfo));
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjIsHit,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "isHit",
        /* strAddInfo   */ strMthInArgs );

    bool bIsHit = false;

    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        QRectF rct = rect();
        bIsHit = isRectHit(
            rct, EFillStyle::SolidPattern, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo);
        if (bIsHit) {
            if (o_pHitInfo != nullptr) {
                if (m_selPtType == ESelectionPointType::BoundingRectangle) {
                    o_pHitInfo->m_editMode = selectionPoint2EditMode(m_selPt.enumerator());
                    o_pHitInfo->m_editResizeMode = selectionPoint2EditResizeMode(m_selPt.enumerator());
                    o_pHitInfo->m_selPtBoundingRect = m_selPt;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = rct.center();
                }
                else if (m_selPtType == ESelectionPointType::PolygonShapePoint) {
                    o_pHitInfo->m_editMode = EEditMode::MoveShapePoint;
                    o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                    o_pHitInfo->m_idxPolygonShapePoint = m_idxPt;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = rct.center();
                }
            }
        }

        if (bIsHit && o_pHitInfo != nullptr) {
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
            double fRotAngleCurr_deg = m_fRotAngleCurr_deg;
            if (m_pGraphObjParent != nullptr) {
                fRotAngleCurr_deg = m_pGraphObjParent->getRotationAngleInDegree();
            }
            o_pHitInfo->setCursor( Math::deg2Rad(fRotAngleCurr_deg) );
#endif
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs;
        if (o_pHitInfo != nullptr) {
            strMthOutArgs = "HitInfo {" + o_pHitInfo->toString() + "}";
            mthTracer.setMethodOutArgs(strMthOutArgs);
        }
        mthTracer.setMethodReturn(bIsHit);
    }
    return bIsHit;
}

/*==============================================================================
public: // reimplementing methods of base class QGraphicItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::setCursor( const QCursor& i_cursor )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qCursorShape2Str(i_cursor.shape());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setCursor",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsEllipseItem::setCursor(i_cursor);
}

/*==============================================================================
protected: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::updateToolTip()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateToolTip",
        /* strAddInfo   */ "" );

    if (m_pGraphObjParent != nullptr) {
        m_strToolTip = m_pGraphObjParent->getToolTip();
    }
    else {
        m_strToolTip = CGraphObj::getToolTip();
    }
}

/*==============================================================================
public: // overridables of base class QGraphicsPolygonItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjSelectionPoint::boundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ "" );

    QRectF rctBounding = QGraphicsEllipseItem::boundingRect();

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthRet = qRect2Str(rctBounding);
        mthTracer.setMethodReturn(strMthRet);
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::paint(
    QPainter* i_pPainter,
    const QStyleOptionGraphicsItem* /*i_pStyleOption*/,
    QWidget* /*i_pWdgt*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "paint",
        /* strAddInfo   */ "" );

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter,
            m_pTrcAdminObjPaint->getRuntimeInfoTraceDetailLevel());
    }

    i_pPainter->save();

    QPen pn = pen();
    QBrush brsh = brush();

    if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
        if (m_idxSelPtSelectedPolygon >= 0) {
            pn.setColor(Qt::magenta);
            //brsh.setStyle(Qt::SolidPattern);
            //brsh.setColor(Qt::magenta);
            brsh.setStyle(Qt::NoBrush);
        }
        else if (m_selPtSelectedBoundingRect != ESelectionPoint::None) {
            pn.setColor(Qt::blue);
            //brsh.setStyle(Qt::SolidPattern);
            //brsh.setColor(Qt::blue);
            brsh.setStyle(Qt::NoBrush);
        }
    }
    else {
        pn.setColor(Qt::red);
        //brsh.setStyle(Qt::SolidPattern);
        //brsh.setColor(Qt::white);
        brsh.setStyle(Qt::NoBrush);
    }

    //QGraphicsEllipseItem::paint(i_pPainter, i_pStyleOption, i_pWdgt);

    i_pPainter->setPen(pn);
    i_pPainter->setBrush(brsh);

    if ((spanAngle() != 0) && (qAbs(spanAngle()) % (360 * 16) == 0)) {
        i_pPainter->drawEllipse(rect());
    }
    else {
        i_pPainter->drawPie(rect(), startAngle(), spanAngle());
    }
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
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strMthInArgs );

    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if (editToolDrawing == EEditTool::Select)
    {
        QCursor cursor = Qt::CustomCursor;

        double fRotAngleCurr_rad = 0.0;
        double fCursorAngle_rad  = 0.0;
        bool   bSetResizeCursor  = false;

        if (m_pGraphObjParent != nullptr) {
            fRotAngleCurr_rad = Math::deg2Rad(m_pGraphObjParent->getRotationAngleInDegree());
        }

        if (m_selPtSelectedBoundingRect != ESelectionPoint::None) {
            switch (m_selPtSelectedBoundingRect.enumerator()) {
                case ESelectionPoint::TopLeft:
                case ESelectionPoint::BottomRight:
                {
                    fCursorAngle_rad = fRotAngleCurr_rad + Math::c_f135Degrees_rad; // 2nd quadrant: arrow from right/bottom -> top/left
                    cursor = Qt::SizeFDiagCursor; /*  \  */
                    bSetResizeCursor = true;
                    break;
                }
                case ESelectionPoint::TopRight:
                case ESelectionPoint::BottomLeft:
                {
                    fCursorAngle_rad = fRotAngleCurr_rad + Math::c_f45Degrees_rad; // 1st quadrant: arrow from bottom/left -> top/right
                    cursor = Qt::SizeBDiagCursor; /*  /  */
                    bSetResizeCursor = true;
                    break;
                }
                case ESelectionPoint::LeftCenter:
                case ESelectionPoint::RightCenter:
                {
                    fCursorAngle_rad = fRotAngleCurr_rad;
                    cursor = Qt::SizeHorCursor;
                    bSetResizeCursor = true;
                    break;
                }
                case ESelectionPoint::TopCenter:
                case ESelectionPoint::BottomCenter:
                {
                    fCursorAngle_rad = fRotAngleCurr_rad + Math::c_f90Degrees_rad;
                    cursor = Qt::SizeVerCursor;
                    bSetResizeCursor = true;
                    break;
                }
                case ESelectionPoint::Center:
                {
                    cursor = Qt::SizeAllCursor;
                    break;
                }
                case ESelectionPoint::RotateTop:
                case ESelectionPoint::RotateBottom:
                {
                    QBitmap bmpCursor(":/ZS/Draw/CursorRotateFree16x16.png");
                    cursor = QCursor(bmpCursor);
                    break;
                }
                default:
                {
                    break;
                }
            }

            if (bSetResizeCursor) {
                // Force resulting cursor rotation angle to 1st or 2nd quadrant (0..180°)
                while (fCursorAngle_rad >= Math::c_f180Degrees_rad) {
                    fCursorAngle_rad -= Math::c_f180Degrees_rad;
                }
                while (fCursorAngle_rad < 0.0) {
                    fCursorAngle_rad += Math::c_f180Degrees_rad;
                }
                if (fCursorAngle_rad >= 0.0 && fCursorAngle_rad < Math::c_f45Degrees_rad/2.0) { // 0.0 .. 22.5°
                    cursor = Qt::SizeHorCursor;
                }
                else if (fCursorAngle_rad >= Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 3.0*Math::c_f45Degrees_rad/2.0) { // 22.5° .. 67.5°
                    cursor = Qt::SizeBDiagCursor; // 1st quadrant: arrow from bottom/left -> top/right
                }
                else if (fCursorAngle_rad >= 3.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 5.0*Math::c_f45Degrees_rad/2.0) { // 67.5° .. 112.5°
                    cursor = Qt::SizeVerCursor;
                }
                else if (fCursorAngle_rad >= 5.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 7.0*Math::c_f45Degrees_rad/2.0) { // 112.5° .. 157.5°
                    cursor = Qt::SizeFDiagCursor; // 2nd quadrant: arrow from top/left -> bottom/right
                }
                else if (fCursorAngle_rad >= 7.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < Math::c_f180Degrees_rad) { // 157.5° .. 180.0°
                    cursor = Qt::SizeHorCursor;
                }
            } // if( bSetResizeCursor )
        } // if( m_selPtSelectedBoundingRect != ESelectionPoint::None )

        else if (m_idxSelPtSelectedPolygon >= 0) {
            cursor = Qt::CrossCursor;
        }

        if (cursor.shape() != Qt::CustomCursor) {
            setCursor(cursor);
        }
    } // if( editToolDrawing == EEditTool::Select )
} // hoverEnterEvent

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if (editToolDrawing == EEditTool::Select)
    {
        QCursor cursor = Qt::CustomCursor;

        double fRotAngleCurr_rad = 0.0;
        double fCursorAngle_rad  = 0.0;
        bool   bSetResizeCursor  = false;

        if (m_pGraphObjParent != nullptr) {
            fRotAngleCurr_rad = Math::deg2Rad(m_pGraphObjParent->getRotationAngleInDegree());
        }

        if (m_selPtSelectedBoundingRect != ESelectionPoint::None) {
            switch (m_selPtSelectedBoundingRect.enumerator())
            {
                case ESelectionPoint::TopLeft:
                case ESelectionPoint::BottomRight:
                {
                    fCursorAngle_rad = fRotAngleCurr_rad + Math::c_f135Degrees_rad; // 2nd quadrant: arrow from right/bottom -> top/left
                    cursor = Qt::SizeFDiagCursor; /*  \  */
                    bSetResizeCursor = true;
                    break;
                }
                case ESelectionPoint::TopRight:
                case ESelectionPoint::BottomLeft:
                {
                    fCursorAngle_rad = fRotAngleCurr_rad + Math::c_f45Degrees_rad; // 1st quadrant: arrow from bottom/left -> top/right
                    cursor = Qt::SizeBDiagCursor; /*  /  */
                    bSetResizeCursor = true;
                    break;
                }
                case ESelectionPoint::LeftCenter:
                case ESelectionPoint::RightCenter:
                {
                    fCursorAngle_rad = fRotAngleCurr_rad;
                    cursor = Qt::SizeHorCursor;
                    bSetResizeCursor = true;
                    break;
                }
                case ESelectionPoint::TopCenter:
                case ESelectionPoint::BottomCenter:
                {
                    fCursorAngle_rad = fRotAngleCurr_rad + Math::c_f90Degrees_rad;
                    cursor = Qt::SizeVerCursor;
                    bSetResizeCursor = true;
                    break;
                }
                case ESelectionPoint::Center:
                {
                    cursor = Qt::SizeAllCursor;
                    break;
                }
                case ESelectionPoint::RotateTop:
                case ESelectionPoint::RotateBottom:
                {
                    QBitmap bmpCursor(":/ZS/Draw/CursorRotateFree16x16.png");
                    cursor = QCursor(bmpCursor);
                    break;
                }
                default:
                {
                    break;
                }
            }

            if (bSetResizeCursor) {
                // Force resulting cursor rotation angle to 1st or 2nd quadrant (0..180°)
                while (fCursorAngle_rad >= Math::c_f180Degrees_rad) {
                    fCursorAngle_rad -= Math::c_f180Degrees_rad;
                }
                while (fCursorAngle_rad < 0.0) {
                    fCursorAngle_rad += Math::c_f180Degrees_rad;
                }
                if (fCursorAngle_rad >= 0.0 && fCursorAngle_rad < Math::c_f45Degrees_rad/2.0) { // 0.0 .. 22.5°
                    cursor = Qt::SizeHorCursor;
                }
                else if (fCursorAngle_rad >= Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 3.0*Math::c_f45Degrees_rad/2.0) { // 22.5° .. 67.5°
                    cursor = Qt::SizeBDiagCursor; // 1st quadrant: arrow from bottom/left -> top/right
                }
                else if (fCursorAngle_rad >= 3.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 5.0*Math::c_f45Degrees_rad/2.0) { // 67.5° .. 112.5°
                    cursor = Qt::SizeVerCursor;
                }
                else if (fCursorAngle_rad >= 5.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 7.0*Math::c_f45Degrees_rad/2.0) { // 112.5° .. 157.5°
                    cursor = Qt::SizeFDiagCursor; // 2nd quadrant: arrow from top/left -> bottom/right
                }
                else if (fCursorAngle_rad >= 7.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < Math::c_f180Degrees_rad) { // 157.5° .. 180.0°
                    cursor = Qt::SizeHorCursor;
                }
            } // if( bSetResizeCursor )
        } // if( m_selPtSelectedBoundingRect != ESelectionPoint::None )

        else if (m_idxSelPtSelectedPolygon >= 0) {
            cursor = Qt::CrossCursor;
        }

        if (cursor.shape() != Qt::CustomCursor) {
            setCursor(cursor);
        }
    } // if( editToolDrawing == EEditTool::Select )
} // hoverMoveEvent

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverLeaveEvent",
        /* strAddInfo   */ strMthInArgs );

    unsetCursor();
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strMthInArgs );

    //QGraphicsEllipseItem::mousePressEvent(i_pEv);

    //i_pEv->accept();

} // mousePressEvent

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseMoveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    //QGraphicsEllipseItem::mouseMoveEvent(i_pEv);

    //i_pEv->accept();

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

    //QGraphicsEllipseItem::mouseReleaseEvent(i_pEv);

    //i_pEv->accept();

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strMthInArgs );

    // When doubleclicking an item, the item will first receive a mouse
    // press event, followed by a release event (i.e., a click), then a
    // doubleclick event, and finally a release event.
    // The default implementation of "mouseDoubleClickEvent" calls "mousePressEvent".
    // This is not necessary here.
    //QGraphicsEllipseItem::mouseDoubleClickEvent(i_pEv);

    i_pEv->accept();

} // mouseDoubleClickEvent

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::onGraphObjParentGeometryChanged(CGraphObj* i_pGraphObjParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObjParent->keyInTree();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "onGraphObjParentGeometryChanged",
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "onGraphObjParentZValueChanged",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItemParent = dynamic_cast<QGraphicsItem*>(m_pGraphObjParent);
    setZValue(pGraphicsItemParent->zValue() + 0.05);
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

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsItemChange2Str(i_change, i_value);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "itemChange",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter,
            m_pTrcAdminObjItemChange->getRuntimeInfoTraceDetailLevel());
    }

    QVariant valChanged = i_value;

    bool bTreeEntryChanged = false;

    if (i_change == ItemSceneHasChanged) {
        // The item may have been removed from the scene.
        if (scene() != nullptr) {
            updatePosition();
        }
    }

    if (bTreeEntryChanged && m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }

    valChanged = QGraphicsItem::itemChange(i_change, i_value);

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave,
            m_pTrcAdminObjItemChange->getRuntimeInfoTraceDetailLevel());
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthRet = qGraphicsItemChange2Str(i_change, valChanged, false);
        mthTracer.setMethodReturn(strMthRet);
    }
    return valChanged;
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "updatePosition",
        /* strAddInfo   */ "" );
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter,
            m_pTrcAdminObjItemChange->getRuntimeInfoTraceDetailLevel());
    }

    QPointF ptSelScenePosParent;
    if (m_selPtType == ESelectionPointType::BoundingRectangle) {
        QPointF ptSelPosParent = m_pGraphObjParent->getBoundingRectSelectionPointCoors(m_selPt.enumerator());
        ptSelScenePosParent = dynamic_cast<QGraphicsItem*>(m_pGraphObjParent)->mapToScene(ptSelPosParent);
        if (m_selPt == ESelectionPoint::RotateTop) {
            ptSelScenePosParent.setY(ptSelScenePosParent.y() - getSelectionPointRotateDistance());
        }
        else if (m_selPt == ESelectionPoint::RotateBottom) {
            ptSelScenePosParent.setY(ptSelScenePosParent.y() + getSelectionPointRotateDistance());
        }
    }
    else /*if (m_selPtType == ESelectionPointType::PolygonShapePoint)*/ {
        QPointF ptSelPosParent = m_pGraphObjParent->getPolygonSelectionPointCoors(m_idxPt);
        ptSelScenePosParent = dynamic_cast<QGraphicsItem*>(m_pGraphObjParent)->mapToScene(ptSelPosParent);
    }
    setPos(ptSelScenePosParent);

    m_bUpdatePositionInProgress = false;

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave,
            m_pTrcAdminObjItemChange->getRuntimeInfoTraceDetailLevel());
    }
}

/*==============================================================================
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::traceInternalStates(
    CMethodTracer& i_mthTracer,
    EMethodDir i_mthDir,
    ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (i_mthTracer.isRuntimeInfoActive(i_detailLevel)) {
        QString strTrcInfo;
        if (i_mthDir == EMethodDir::Enter) strTrcInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strTrcInfo = "+- ";
        else strTrcInfo = "";
        strTrcInfo +=
            "GraphObjParent: " + QString(m_pGraphObjParent == nullptr ? "null" : m_pGraphObjParent->keyInTree());
        i_mthTracer.trace(strTrcInfo);

        if (i_mthDir == EMethodDir::Enter) strTrcInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strTrcInfo = "+- ";
        else strTrcInfo = "";
        strTrcInfo +=
            "SelectionPoint {Type: " + QString(m_selPtType.isValid() ? m_selPtType.toString() : "?") +
            ", " + QString(m_selPt.isValid() ? m_selPt.toString() : "?") +
            ", Idx: " + QString::number(m_idxPt) + "}";
        i_mthTracer.trace(strTrcInfo);

        if (i_mthDir == EMethodDir::Enter) strTrcInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strTrcInfo = "+- ";
        else strTrcInfo = "";
        strTrcInfo +=
            "Geometry: GraphObj {Radius: " + QString::number(m_fRadius_px) + " px}"
            ", Item { Pos {" + point2Str(pos()) + "}, Ellipse {" + rect2Str(rect()) + "}";
        i_mthTracer.trace(strTrcInfo);
        if (i_mthDir == EMethodDir::Enter) strTrcInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strTrcInfo = "+- ";
        else strTrcInfo = "";
        strTrcInfo += "Selected: " + bool2Str(isSelected());
        i_mthTracer.trace(strTrcInfo);
        CGraphObj::traceInternalStates(i_mthTracer, i_mthDir, i_detailLevel);
    }
}
