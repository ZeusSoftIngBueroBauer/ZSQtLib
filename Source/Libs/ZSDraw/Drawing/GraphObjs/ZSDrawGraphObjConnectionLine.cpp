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
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
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
class CGraphObjConnectionLine : public CGraphObj, public QGraphicsPolygonItem
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjConnectionLine::s_iInstCount = 0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjConnectionLine::CGraphObjConnectionLine(
    CDrawingScene* i_pDrawingScene, const QString& i_strObjName ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameConnections,
        /* type                */ EGraphObjTypeConnectionLine,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeConnectionLine),
        /* strObjName          */ i_strObjName.isEmpty() ? "ConnectionLine" + QString::number(s_iInstCount) : i_strObjName),
    QGraphicsPolygonItem(),
    m_bCoorsDirty(true),
    m_plgCurr(),
    m_plgOrig(),
    m_plgLineStart(),
    m_plgLineEnd(),
    m_arpCnctPts(CEnumLinePoint::count())
{
    // Just incremented by the ctor but not decremented by the dtor.
    // Used to create a unique name for newly created objects of this type.
    s_iInstCount++;

    QString strMthInArgs;

    createTraceAdminObjs("Connections::" + ClassName());

    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemSendsGeometryChanges);

    //onDrawSettingsChanged();

    //updateToolTip();

} // ctor

//------------------------------------------------------------------------------
CGraphObjConnectionLine::~CGraphObjConnectionLine()
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

    for (int idxLinePt = 0; idxLinePt < m_arpCnctPts.size(); idxLinePt++) {
        CGraphObjConnectionPoint* pGraphObjCnctPt = m_arpCnctPts[idxLinePt];
        if (pGraphObjCnctPt != nullptr) {
            pGraphObjCnctPt->removeConnectionLine(this);
        }
        m_arpCnctPts[idxLinePt] = nullptr;
    }
} // dtor

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
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal))
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "clone",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjConnectionLine* pGraphObj = new CGraphObjConnectionLine(m_pDrawingScene, m_strName);
    pGraphObj->setDrawSettings(m_drawSettings);

    QPointF ptPos = pos();

    pGraphObj->setPolygon( polygon() );
    pGraphObj->setPos(ptPos);
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    pGraphObj->setRotationAngleInDegree(m_fRotAngleCurr_deg);
#endif
    //pGraphObj->setScaleFactors(m_fScaleFacXCurr,m_fScaleFacYCurr);
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    pGraphObj->acceptCurrentAsOriginalCoors();
#endif

    return pGraphObj;

} // clone

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjConnectionLine::setConnectionPoint( ELinePoint i_linePoint, CGraphObjConnectionPoint* i_pCnctPt )
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

    bool bConnected = false;

    if (static_cast<int>(i_linePoint) >= 0 && static_cast<int>(i_linePoint) < m_arpCnctPts.size() && i_pCnctPt != nullptr) {
        CGraphObjConnectionPoint* pGraphObjCnctPt = m_arpCnctPts[static_cast<int>(i_linePoint)];
        if (pGraphObjCnctPt == i_pCnctPt) {
            bConnected = true;
        }
        else {
            if (pGraphObjCnctPt != nullptr) {
                pGraphObjCnctPt->removeConnectionLine(this);
                pGraphObjCnctPt = nullptr;
                m_arpCnctPts[static_cast<int>(i_linePoint)] = nullptr;
            }
            bConnected = i_pCnctPt->appendConnectionLine(this);
            if (bConnected) {
                m_arpCnctPts[static_cast<int>(i_linePoint)] = i_pCnctPt;
                pGraphObjCnctPt = m_arpCnctPts[static_cast<int>(i_linePoint)];
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
            //updateToolTip();
            //updateEditInfo();
        }
    }
    return bConnected;

} // setConnectionPoint

//------------------------------------------------------------------------------
ELinePoint CGraphObjConnectionLine::getConnectionLinePoint( CGraphObjConnectionPoint* i_pCnctPt )
//------------------------------------------------------------------------------
{
    ELinePoint linePoint = ELinePoint::None;
    int        idxLinePtTmp;

    for( idxLinePtTmp = 0; idxLinePtTmp < m_arpCnctPts.size(); idxLinePtTmp++ )
    {
        CGraphObjConnectionPoint* pGraphObjCnctPt = m_arpCnctPts[idxLinePtTmp];

        if( pGraphObjCnctPt == i_pCnctPt )
        {
            linePoint = static_cast<ELinePoint>(idxLinePtTmp);
            break;
        }
    }
    return linePoint;

} // getConnectionLinePoint

//------------------------------------------------------------------------------
CGraphObjConnectionPoint* CGraphObjConnectionLine::getConnectionPoint( ELinePoint i_linePoint )
//------------------------------------------------------------------------------
{
    CGraphObjConnectionPoint* pGraphObjCnctPt = nullptr;

    if( static_cast<int>(i_linePoint) >= 0 && static_cast<int>(i_linePoint) < m_arpCnctPts.size() )
    {
        pGraphObjCnctPt = m_arpCnctPts[static_cast<int>(i_linePoint)];
    }
    return pGraphObjCnctPt;

} // getConnectionPoint

/*==============================================================================
public: // replacing methods of QGraphicsRectItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::setPolygon( const QPolygonF& i_plg )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPolygon2Str(i_plg) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPolygon",
        /* strAddInfo   */ strMthInArgs );

    if( i_plg.size() >= 2 )
    {
        QPolygonF plg = i_plg;

        // The first and last polygon shape points of the connection line
        // are connected with a connection point and cannot be changed.

        if( m_plgCurr.size() > 0 )
        {
            plg[0] = m_plgCurr[0];
            plg[plg.size()-1] = m_plgCurr[m_plgCurr.size()-1];
        }

        QGraphicsPolygonItem::setPolygon(plg);

        if( isSelected() )
        {
            if( plg.size() == m_plgCurr.size() )
            {
            }
            else
            {
                showSelectionPointsOfPolygon(plg);
            }
        }

        m_bCoorsDirty = true;

        updateLineEndPolygonCoors();

        //updateEditInfo();
        //updateToolTip();

    } // if( i_plg.size() >= 2 )

} // setPolygon

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjConnectionLine::getScenePolygonShapePointsString() const
//------------------------------------------------------------------------------
{
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QPolygonF plg = polygon();
    QPolygonF plgScene = pGraphicsItemThis->mapToScene(plg);
    return qPolygon2Str(plgScene);
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "OldSettings {" + i_drawSettingsOld.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "onDrawSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    m_bCoorsDirty = true;

    if( m_drawSettings.isPenUsed() )
    {
        if( m_drawSettings.getLineStyle() != ELineStyle::NoLine )
        {
            QPen pn;

            pn.setColor( m_drawSettings.getPenColor() );
            pn.setWidth( m_drawSettings.getPenWidth() );
            pn.setStyle( lineStyle2QtPenStyle(m_drawSettings.getLineStyle()) );

            setPen(pn);
        }
        else
        {
            setPen(Qt::NoPen);
        }
    }
    else
    {
        setPen(Qt::NoPen);
    }

} // onDrawSettingsChanged

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//------------------------------------------------------------------------------
void CGraphObjConnectionLine::acceptCurrentAsOriginalCoors()
//------------------------------------------------------------------------------
{
    m_plgOrig = m_plgCurr;

    CGraphObj::acceptCurrentAsOriginalCoors();

} // acceptCurrentAsOriginalCoors
#endif

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObjConnectionLine::setWidth( const CPhysVal& i_physValWidth )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_physValWidth.toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "setWidth",
//        /* strAddInfo   */ strMthInArgs );
//
//    // This method call does not really make sense for connection lines.
//    // But at least we update the current size member variable.
//    setSize(i_physValWidth, getHeight(i_physValWidth.unit()));
//}
//
////------------------------------------------------------------------------------
//void CGraphObjConnectionLine::setHeight( const CPhysVal& i_physValHeight )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_physValHeight.toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "setHeight",
//        /* strAddInfo   */ strMthInArgs );
//
//    // This method call does not really make sense for connection lines.
//    // But at least we update the current size member variable.
//    setSize(getWidth(i_physValHeight.unit()), i_physValHeight);
//}
//
////------------------------------------------------------------------------------
//void CGraphObjConnectionLine::setSize( const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_physValWidth.toString() + ", " + i_physValHeight.toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "setSize",
//        /* strAddInfo   */ strMthInArgs );
//
//    // This method call does not really make sense for connection lines.
//    // But at least we update the current size member variable.
//
//    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
//
//    CPhysVal physValWidth = i_physValWidth;
//    CPhysVal physValHeight = i_physValHeight;
//
//    physValWidth.convertValue(drawingSize.unit());
//    physValHeight.convertValue(drawingSize.unit());
//
//    if (hasFixedWidth() && physValWidth != getFixedWidth(drawingSize.unit())) {
//        physValWidth = getFixedWidth(drawingSize.unit());
//    }
//    else {
//        if (hasMinimumWidth() && physValWidth > getMinimumWidth(drawingSize.unit())) {
//            physValWidth = getMinimumWidth(drawingSize.unit());
//        }
//        if (hasMaximumWidth() && physValWidth > getMaximumWidth(drawingSize.unit())) {
//            physValWidth = getMaximumWidth(drawingSize.unit());
//        }
//    }
//
//    if (hasFixedHeight() && physValHeight != getFixedHeight(drawingSize.unit())) {
//        physValHeight = getFixedHeight(drawingSize.unit());
//    }
//    else {
//        if (hasMinimumHeight() && physValHeight > getMinimumHeight(drawingSize.unit())) {
//            physValHeight = getMinimumHeight(drawingSize.unit());
//        }
//        if (hasMaximumHeight() && physValHeight > getMaximumHeight(drawingSize.unit())) {
//            physValHeight = getMaximumHeight(drawingSize.unit());
//        }
//    }
//
//    //CPhysValLine physValLine = getLine();
//
//    //CPhysValPoint physValPoint1 = physValLine.p1();
//    //CPhysValPoint physValPoint2 = physValLine.p2();
//
//    //physValPoint2.setX(physValPoint1.x() + physValWidth);
//    //physValPoint2.setY(physValPoint1.y() + physValHeight);
//
//    //setLine(physValLine);
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//    if( fWidth != m_rctCurr.width() || fHeight != m_rctCurr.height() )
//    {
//        m_rctCurr.setWidth(fWidth);
//        m_rctCurr.setHeight(fHeight);
//
//        m_ptRotOriginCurr = m_rctCurr.center();
//
//        // As this method call doesn't make any sense for connection lines
//        // the polygon shape points have not changed and there is no need
//        // to update the selection points.
//        //updateSelectionPointsOfPolygon(polygon());
//    }
//#endif
//
//} // setSize
//
////------------------------------------------------------------------------------
//void CGraphObjConnectionLine::setSize( const CPhysValSize& i_physValSize )
////------------------------------------------------------------------------------
//{
//    setSize(i_physValSize.width(), i_physValSize.height());
//}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObjConnectionLine::setIsHit( bool i_bHit )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = bool2Str(i_bHit);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "setIsHit",
//        /* strAddInfo   */ strMthInArgs );
//
//    if (m_bIsHit != i_bHit) {
//        m_bIsHit = i_bHit;
//        if (m_bIsHit) {
//            showSelectionPointsOfPolygon(polygon());
//        }
//        else if (!isSelected()) {
//            hideSelectionPoints();
//        }
//        update();
//    }
//} // setIsHit

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
//bool CGraphObjConnectionLine::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjIsHit, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "Point:" + point2Str(i_pt) +
//            ", HitInfo {" + QString(o_pHitInfo == nullptr ? "null" : pointer2Str(o_pHitInfo)) + "}";
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjIsHit,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "isHit",
//        /* strAddInfo   */ strMthInArgs );
//
//    bool bIsHit = false;
//
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//    if (pGraphicsItem != nullptr) {
//        if (pGraphicsItem->isSelected()) {
//            bIsHit = isPolygonSelectionPointHit(i_pt, o_pHitInfo);
//        }
//        if (!bIsHit) {
//            QPolygonF plg = polygon();
//            bIsHit = isPolygonHit(plg, EFillStyle::NoFill, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo);
//        }
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//        if (bIsHit && o_pHitInfo != nullptr) {
//            o_pHitInfo->setCursor(Math::degree2Rad(m_fRotAngleCurr_deg));
//        }
//#endif
//    }
//
//    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
//        QString strMthOutArgs;
//        if (o_pHitInfo != nullptr) {
//            strMthOutArgs = "HitInfo {" + o_pHitInfo->toString() + "}";
//            mthTracer.setMethodOutArgs(strMthOutArgs);
//        }
//        mthTracer.setMethodReturn(bIsHit);
//    }
//    return bIsHit;
//}

/*==============================================================================
public: // reimplementing methods of base class QGraphicItem
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObjConnectionLine::setCursor( const QCursor& i_cursor )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = qCursorShape2Str(i_cursor.shape());
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "setCursor",
//        /* strAddInfo   */ strMthInArgs );
//
//    QGraphicsPolygonItem::setCursor(i_cursor);
//}

/*==============================================================================
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::showSelectionPoints(TSelectionPointTypes i_selPts)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + selectionPointTypes2Str(i_selPts) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "showSelectionPoints",
        /* strAddInfo   */ strMthInArgs );

    if (parentItem() == nullptr) {
        if (i_selPts & c_uSelectionPointsPolygonShapePoints) {
            showSelectionPointsOfPolygon( polygon() );
        }
    }
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Called by the parent graphic item to inform the child about geometry changes.

    Connection lines don't belong to groups. But their connection points do.
    If a group is moved also the connection points are moved by Qt's graphics scene.
    But not the connection lines which are linked to the connection points. The
    connection points can be considered as parents of connection lines and
    "onGraphObjParentGeometryOnSceneChanged" is called by the connection points if their
    position changes.
*/
void CGraphObjConnectionLine::onGraphObjParentGeometryOnSceneChanged( CGraphObj* i_pGraphObjParent )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObjParent->keyInTree();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "onGraphObjParentGeometryOnSceneChanged",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    CGraphObjConnectionPoint* pGraphObjCnctPt = dynamic_cast<CGraphObjConnectionPoint*>(i_pGraphObjParent);
    if (pGraphObjCnctPt != nullptr) {
        QGraphicsItem* pGraphicsItemCnctPt = dynamic_cast<QGraphicsItem*>(pGraphObjCnctPt);
        QPolygonF plg = polygon();
        ELinePoint linePoint = getConnectionLinePoint(pGraphObjCnctPt);
        QPointF ptCnctPtPos = pGraphObjCnctPt->rect().center();
        QPointF ptCnctPtScenePos = pGraphicsItemCnctPt->mapToScene(ptCnctPtPos);
        QPointF ptItemPosNew = mapFromScene(ptCnctPtScenePos);
        if (linePoint == ELinePoint::Start && plg.size() > 0) {
            // The origin of the poly line's coordinate system (the item's position)
            // is the first line point (in poly line's item's coordinate system the
            // first line point is at 0/0). If the first line point is moved it's
            // coordinates remain at 0/0 but all other points must be moved to fit into
            // the new item's coordinate system.
            QPointF ptItemPosOld = plg[0];
            setPos(ptCnctPtScenePos);
            double dx = ptItemPosNew.x() - ptItemPosOld.x();
            double dy = ptItemPosNew.y() - ptItemPosOld.y();
            QPointF ptPosOld, ptPosNew;
            for (int idxPt = 1; idxPt < plg.size(); idxPt++) {
                ptPosOld = plg[idxPt];
                ptPosNew.setX( ptPosOld.x() - dx );
                ptPosNew.setY( ptPosOld.y() - dy );
                plg[idxPt] = ptPosNew;
            }
            QGraphicsPolygonItem::setPolygon(plg);
            for (int idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++) {
                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
                if (pGraphObjSelPt != nullptr) {
                    QPointF ptSel = plg[idxSelPt];
                    ptSel = pGraphicsItemThis->mapToScene(ptSel);
                    pGraphObjSelPt->setPos(ptSel);
                }
            }
        }
        else if (linePoint == ELinePoint::End && plg.size() > 1) {
            int idxSelPt = plg.size()-1;
            plg[idxSelPt] = ptItemPosNew;
            QGraphicsPolygonItem::setPolygon(plg);
            if (idxSelPt >= 0 && idxSelPt < m_arpSelPtsPolygon.size()) {
                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
                if (pGraphObjSelPt != nullptr) {
                    QPointF ptSel = plg[idxSelPt];
                    ptSel = pGraphicsItemThis->mapToScene(ptSel);
                    pGraphObjSelPt->setPos(ptSel);
                }
            }
        }
        normalize();
        m_bCoorsDirty = true;
        updateLineEndPolygonCoors();
        //updateEditInfo();
        //updateToolTip();
    }

    // If the geometry of the parent on the scene of this item changes, also the geometry
    // on the scene of this item is changed.
    emit_geometryOnSceneChanged();

} // onGraphObjParentGeometryOnSceneChanged

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjConnectionLine::boundingRect() const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjBoundingRect, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ConnectionLine {" + qPolygon2Str(polygon()) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ strMthInArgs );

    QRectF rctBounding = QGraphicsPolygonItem::boundingRect();

    CGraphObjSelectionPoint* pGraphObjSelPt;
    QRectF                   rctSelPt;
    QPolygonF                plgSelPt;
    int                      idxSelPt;

    for (idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++) {
        pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
        if (pGraphObjSelPt != nullptr) {
            rctSelPt = pGraphObjSelPt->boundingRect();
            plgSelPt = mapFromItem( pGraphObjSelPt, rctSelPt );
            rctBounding |= plgSelPt.boundingRect();
        }
    }

    if (m_plgLineStart.size() > 0) {
        rctBounding |= m_plgLineStart.boundingRect();
    }
    if (m_plgLineEnd.size() > 0) {
        rctBounding |= m_plgLineEnd.boundingRect();
    }

    //if( m_bIsHit || isSelected() )
    //{
    //    QHashIterator<QString, CGraphObjLabel*> itLabels(m_arpLabels);
    //    CGraphObjLabel* pGraphObjLabel;
    //    QRectF          rctLabel;
    //    QPolygonF       plgLabel;

    //    while( itLabels.hasNext() )
    //    {
    //        itLabels.next();
    //        pGraphObjLabel = itLabels.value();

    //        if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
    //        {
    //            // Calling pGraphObjLabel->boundingRect() may lead to endless recursion as the
    //            // label itself may call the boundingRect method of its parent item (which is
    //            // this item) if the label is selected or has been hit. For this we call
    //            // boundingRect of the label with a different signature to indicate that we
    //            // are only interested in the bounding rect of the simple text item.
    //            rctLabel = pGraphObjLabel->m_pGraphObjLabel->boundingRect(true);
    //            plgLabel = mapFromItem( pGraphObjLabel->m_pGraphObjLabel, rctLabel );
    //            rctBounding |= plgLabel.boundingRect();
    //        }
    //    }
    //}

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;

} // boundingRect

//------------------------------------------------------------------------------
QPainterPath CGraphObjConnectionLine::shape() const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjBoundingRect, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "ConnectionLine {" + qPolygon2Str(polygon()) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "shape",
        /* strAddInfo   */ strMthInArgs );

    QPainterPath painterPath = QGraphicsPolygonItem::shape();

    if( m_plgLineStart.size() > 0 )
    {
        painterPath.addPolygon(m_plgLineStart);
    }
    if( m_plgLineEnd.size() > 0 )
    {
        painterPath.addPolygon(m_plgLineEnd);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        const QGraphicsItem* pCThis = static_cast<const QGraphicsItem*>(this);
        QGraphicsItem* pVThis = const_cast<QGraphicsItem*>(pCThis);
        strMthInArgs  = "Path:" + qPainterPath2Str(pVThis,painterPath);
        mthTracer.setMethodReturn(strMthInArgs);
    }

    return painterPath;

} // shape

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::paint(
    QPainter*                       i_pPainter,
    const QStyleOptionGraphicsItem* /*i_pStyleOption*/,
    QWidget*                        /*i_pWdgt*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "paint",
        /* strAddInfo   */ "" );

    i_pPainter->save();

    if( m_bCoorsDirty )
    {
        updateLineEndPolygonCoors();
    }

    QPen pn = pen();

    if( m_pDrawingScene->getMode() == EMode::Edit && isSelected() )
    {
        if( pn.width() < 2 )
        {
            pn.setWidth(2);
        }
        pn.setStyle(Qt::DotLine);
        pn.setColor(Qt::blue);
    }

    if( m_arpCnctPts[static_cast<int>(ELinePoint::Start)] == nullptr || m_arpCnctPts[static_cast<int>(ELinePoint::End)] == nullptr )
    {
        //if( m_editMode != EEditMode::Creating )
        {
            if( pn.width() < 2 )
            {
                pn.setWidth(2);
            }
            pn.setColor(Qt::red);
        }
    }

    i_pPainter->setPen(pn);

    if( m_plgCurr.size() > 0 )
    {
        i_pPainter->drawPolyline(m_plgCurr);
    }

    CEnumLineEndStyle lineEndStyleLineStart = m_drawSettings.getLineEndStyle(ELinePoint::Start);
    CEnumLineEndStyle lineEndStyleLineEnd   = m_drawSettings.getLineEndStyle(ELinePoint::End);

    if( lineEndStyleLineStart != ELineEndStyle::Normal || lineEndStyleLineEnd != ELineEndStyle::Normal )
    {
        CEnumArrowHeadBaseLineType baseLineTypeLineStart = m_drawSettings.getArrowHeadBaseLineType(ELinePoint::Start);
        CEnumArrowHeadBaseLineType baseLineTypeLineEnd   = m_drawSettings.getArrowHeadBaseLineType(ELinePoint::End);

        QBrush brsh;

        pn.setWidth(1);
        pn.setStyle(Qt::SolidLine);

        i_pPainter->setPen(pn);

        brsh.setColor(pn.color());

        if( lineEndStyleLineStart != ELineEndStyle::Normal )
        {
            brsh.setStyle( arrowHeadFillStyle2QtBrushStyle(m_drawSettings.getArrowHeadFillStyle(ELinePoint::Start)) );

            i_pPainter->setBrush(brsh);

            if( baseLineTypeLineStart == EArrowHeadBaseLineType::NoLine )
            {
                i_pPainter->drawPolyline(m_plgLineStart);
            }
            else
            {
                i_pPainter->drawPolygon(m_plgLineStart);
            }
        }

        if( lineEndStyleLineEnd != ELineEndStyle::Normal )
        {
            brsh.setStyle( arrowHeadFillStyle2QtBrushStyle(m_drawSettings.getArrowHeadFillStyle(ELinePoint::End)) );

            i_pPainter->setBrush(brsh);

            if( baseLineTypeLineEnd == EArrowHeadBaseLineType::NoLine )
            {
                i_pPainter->drawPolyline(m_plgLineEnd);
            }
            else
            {
                i_pPainter->drawPolygon(m_plgLineEnd);
            }
        }
    } // if( lineEndStyleLineStart != ELineEndStyle::Normal || lineEndStyleLineEnd != ELineEndStyle::Normal )

    if( m_pDrawingScene->getMode() == EMode::Edit && (/*m_bIsHit ||*/ isSelected()) )
    {
        pn.setStyle(Qt::DotLine);
        pn.setColor(Qt::blue);

        i_pPainter->setPen(pn);

        //QHashIterator<QString, CGraphObjLabel*> itLabels(m_arpLabels);
        //CGraphObjLabel* pGraphObjLabel;

        //QPointF ptSelPt;
        //QPointF ptLabelSelPt;

        //while( itLabels.hasNext() )
        //{
        //    itLabels.next();
        //    pGraphObjLabel = itLabels.value();

        //    if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
        //    {
        //        ptSelPt = getSelectionPointCoors(pGraphObjLabel->m_selPt.enumerator());

        //        ptLabelSelPt = pGraphObjLabel->m_pGraphObjLabel->getSelectionPointCoors(ESelectionPoint::Center);
        //        ptLabelSelPt = mapFromItem( pGraphObjLabel->m_pGraphObjLabel, ptLabelSelPt );

        //        i_pPainter->drawLine( ptSelPt, ptLabelSelPt );
        //    }
        //}
    } // if( m_pDrawingScene->getMode() == EMode::Edit && (m_bIsHit || isSelected()) )

    i_pPainter->restore();

} // paint

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

////------------------------------------------------------------------------------
//bool CGraphObjConnectionLine::sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv )
////------------------------------------------------------------------------------
//{
//    //--------------------------------------------------------------------
//    // Mouse events of selection points will be "watched" to forward those
//    // events to this item in order to resize and rotate this item.
//    //--------------------------------------------------------------------
//
//    CGraphObjSelectionPoint* pGraphObjSelPtWatched = dynamic_cast<CGraphObjSelectionPoint*>(i_pGraphicsItemWatched);
//    if (pGraphObjSelPtWatched == nullptr) {
//        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pGraphObjSelPtWatched == nullptr" );
//    }
//
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjSceneEventFilter, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "ItemWatched: " + pGraphObjSelPtWatched->name();
//        if (isGraphicsSceneHoverEvent(i_pEv)) {
//            QGraphicsSceneHoverEvent* pHoverEvent = dynamic_cast<QGraphicsSceneHoverEvent*>(i_pEv);
//            strMthInArgs += ", " + qEventType2Str(i_pEv->type()) + " {" + qGraphicsSceneHoverEvent2Str(pHoverEvent) + "}";
//        }
//        else if (isGraphicsSceneMouseEvent(i_pEv)) {
//            QGraphicsSceneMouseEvent* pMouseEvent = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
//            strMthInArgs += ", " + qEventType2Str(i_pEv->type()) + " {" + qGraphicsSceneMouseEvent2Str(pMouseEvent) + "}";
//        }
//        else {
//            strMthInArgs += ", Event: " + qEventType2Str(i_pEv->type());
//        }
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjSceneEventFilter,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "sceneEventFilter",
//        /* strAddInfo   */ strMthInArgs );
//
//    bool bEventHandled = false;
//
//    switch( i_pEv->type() )
//    {
//        case QEvent::GraphicsSceneMouseMove:
//        {
//            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
//            if( pEv != nullptr )
//            {
//                QPointF ptEvPos;
//                for( int i = 0x1; i <= 0x10; i <<= 1 )
//                {
//                    Qt::MouseButton button = Qt::MouseButton(i);
//                    ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
//                    pEv->setButtonDownPos( button, ptEvPos );
//                }
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
//                pEv->setPos(ptEvPos);
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
//                pEv->setLastPos(ptEvPos);
//                mouseMoveEvent(pEv);
//                bEventHandled = true;
//            }
//            break;
//        }
//        case QEvent::GraphicsSceneMousePress:
//        {
//            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
//            if( pEv != nullptr )
//            {
//                QPointF ptEvPos;
//                for( int i = 0x1; i <= 0x10; i <<= 1 )
//                {
//                    Qt::MouseButton button = Qt::MouseButton(i);
//                    ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
//                    pEv->setButtonDownPos( button, ptEvPos );
//                }
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
//                pEv->setPos(ptEvPos);
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
//                pEv->setLastPos(ptEvPos);
//                mousePressEvent(pEv);
//                bEventHandled = true;
//            }
//            break;
//        }
//        case QEvent::GraphicsSceneMouseRelease:
//        {
//            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
//            if( pEv != nullptr )
//            {
//                QPointF ptEvPos;
//                for( int i = 0x1; i <= 0x10; i <<= 1 )
//                {
//                    Qt::MouseButton button = Qt::MouseButton(i);
//                    ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
//                    pEv->setButtonDownPos( button, ptEvPos );
//                }
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
//                pEv->setPos(ptEvPos);
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
//                pEv->setLastPos(ptEvPos);
//                mouseReleaseEvent(pEv);
//                bEventHandled = true;
//            }
//            break;
//        }
//        case QEvent::GraphicsSceneMouseDoubleClick:
//        {
//            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
//            if( pEv != nullptr )
//            {
//                QPointF ptEvPos;
//                for( int i = 0x1; i <= 0x10; i <<= 1 )
//                {
//                    Qt::MouseButton button = Qt::MouseButton(i);
//                    ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
//                    pEv->setButtonDownPos( button, ptEvPos );
//                }
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
//                pEv->setPos(ptEvPos);
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
//                pEv->setLastPos(ptEvPos);
//                mouseDoubleClickEvent(pEv);
//                bEventHandled = true;
//            }
//            break;
//        }
//        default:
//        {
//            break;
//        }
//
//    } // switch( i_pEv->type() )
//
//    return bEventHandled;
//
//} // sceneEventFilter

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strMthInArgs );

    //QPointF ptItemPos = i_pEv->pos();

    //CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
    //CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    //if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )
    //{
    //    SGraphObjHitInfo hitInfo;

    //    bool bIsHit = isHit(ptItemPos,&hitInfo);

    //    if( bIsHit )
    //    {
    //        if( cursor().shape() != hitInfo.m_cursor.shape() )
    //        {
    //            setCursor(hitInfo.m_cursor);
    //        }
    //    }

    //} // if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )

} // hoverEnterEvent

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    //QPointF ptItemPos = i_pEv->pos();

    //CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
    //CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    //if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )
    //{
    //    SGraphObjHitInfo hitInfo;

    //    bool bIsHit = isHit(ptItemPos,&hitInfo);

    //    if( bIsHit )
    //    {
    //        if( cursor().shape() != hitInfo.m_cursor.shape() )
    //        {
    //            setCursor(hitInfo.m_cursor);
    //        }
    //    }

    //} // if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )

} // hoverMoveEvent

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverLeaveEvent",
        /* strAddInfo   */ strMthInArgs );

    unsetCursor();

} // hoverLeaveEvent

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

    //Qt::KeyboardModifiers iEvMouseKeyboardModifiers = i_pEv->modifiers() & Qt::KeyboardModifierMask;

    //QPointF ptMouseItemPos = i_pEv->pos();

    //CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
    //CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    //if( m_editMode == EEditMode::Creating )
    //{
    //    QGraphicsPolygonItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)

    //    QPolygonF plg = polygon();

    //    CGraphObjSelectionPoint* pGraphObjSelPt;
    //    QPointF ptSel;

    //    while( plg.size() < 2 )
    //    {
    //        plg.append( QPointF(0.0,0.0) );

    //        pGraphObjSelPt = new CGraphObjSelectionPoint(m_pDrawingScene, SGraphObjSelectionPoint(this, plg.size()-1));
    //        m_arpSelPtsPolygon.append(pGraphObjSelPt);
    //        QObject::connect(
    //            pGraphObjSelPt, &CGraphObj::aboutToBeDestroyed,
    //            this, &CGraphObjConnectionLine::onSelectionPointAboutToBeDestroyed);

    //        m_pDrawingScene->addGraphObj(pGraphObjSelPt);

    //        //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
    //        pGraphObjSelPt->installSceneEventFilter(this);

    //        ptSel = QPointF(0.0,0.0);
    //        ptSel = mapToScene(ptSel);
    //        pGraphObjSelPt->setPos(ptSel);
    //    }

    //    // If there was no polygon shape point selected ..
    //    if( m_idxSelPtSelectedPolygon < 0 || m_idxSelPtSelectedPolygon >= plg.size() )
    //    {
    //        // .. the shape has been initially hit right after creation.
    //        m_idxSelPtSelectedPolygon = plg.size()-1;
    //        plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;

    //        if( m_idxSelPtSelectedPolygon < m_arpSelPtsPolygon.size() )
    //        {
    //            pGraphObjSelPt = m_arpSelPtsPolygon[m_idxSelPtSelectedPolygon];

    //            if( pGraphObjSelPt != nullptr )
    //            {
    //                ptSel = plg[m_idxSelPtSelectedPolygon];
    //                ptSel = mapToScene(ptSel);
    //                pGraphObjSelPt->setPos(ptSel);
    //            }
    //        }
    //    }

    //    // If there was already a polygon shape point selected ..
    //    else
    //    {
    //        // .. the currently selected shape point will be fixed and
    //        // another point will be added.
    //        plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;
    //        plg.append(ptMouseItemPos);
    //        m_idxSelPtSelectedPolygon = plg.size()-1;

    //        pGraphObjSelPt = new CGraphObjSelectionPoint(
    //            m_pDrawingScene, SGraphObjSelectionPoint(this, m_idxSelPtSelectedPolygon));
    //        m_arpSelPtsPolygon.append(pGraphObjSelPt);
    //        QObject::connect(
    //            pGraphObjSelPt, &CGraphObj::aboutToBeDestroyed,
    //            this, &CGraphObjConnectionLine::onSelectionPointAboutToBeDestroyed);

    //        m_pDrawingScene->addGraphObj(pGraphObjSelPt);

    //        //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
    //        pGraphObjSelPt->installSceneEventFilter(this);

    //        ptSel = plg[m_idxSelPtSelectedPolygon];
    //        ptSel = mapToScene(ptSel);
    //        pGraphObjSelPt->setPos(ptSel);
    //    }

    //    QGraphicsPolygonItem::setPolygon(plg);

    //    m_bCoorsDirty = true;

    //    updateLineEndPolygonCoors();

    //    //updateEditInfo();
    //    //updateToolTip();

    //} // if( m_editMode == EEditMode::Creating )

    //else if( m_editMode == EEditMode::None )
    //{
    //    if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )
    //    {
    //        QGraphicsPolygonItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)

    //        QPointF   ptMouseItemPos = mapFromScene(i_pEv->scenePos());
    //        QPolygonF plg = polygon();

    //        m_editMode       = EEditMode::Move;
    //        m_editResizeMode = EEditResizeMode::None;
    //        m_idxSelPtSelectedPolygon   = -1;
    //        m_selPtSelectedBoundingRect = ESelectionPoint::None;

    //        CGraphObjSelectionPoint* pGraphObjSelPt;
    //        QRectF                   rctSelPt;
    //        QPolygonF                plgSelPt;
    //        int                      idxSelPt;

    //        // Polygon shape points may be very close or even overlap each other.
    //        // On creating the polygon the most likely point the user wants to grab
    //        // is the last shape point to move it around.

    //        if( m_arpSelPtsPolygon.size() > 0 )
    //        {
    //            for( idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= 0; idxSelPt-- )
    //            {
    //                pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

    //                if( pGraphObjSelPt != nullptr )
    //                {
    //                    rctSelPt = pGraphObjSelPt->boundingRect();
    //                    plgSelPt = mapFromItem( pGraphObjSelPt, rctSelPt );
    //                    rctSelPt = plgSelPt.boundingRect();

    //                    if( rctSelPt.contains(ptMouseItemPos) )
    //                    {
    //                        m_idxSelPtSelectedPolygon = idxSelPt;
    //                        m_editMode = EEditMode::MoveShapePoint;
    //                        break;
    //                    }
    //                }
    //            }
    //        }

    //        // If a polygon shape point has been hit ...
    //        if( m_editMode == EEditMode::MoveShapePoint && m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < plg.size() )
    //        {
    //            // .. move this shape point to the new position.
    //            plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;
    //        }

    //        // If none of the polygon shape points (but the poly line somewhere else) has been hit ...
    //        else if( m_editMode == EEditMode::Move )
    //        {
    //            // .. a new point will be inserted between the two connection points
    //            // enclosing the poly line segment which has been hit.
    //            if( plg.size() >= 2 ) // anyway ..
    //            {
    //                QLineF  line;
    //                QPointF ptSel;

    //                for( idxSelPt = 1; idxSelPt < plg.size(); idxSelPt++ )
    //                {
    //                    line = QLineF( plg[idxSelPt-1], plg[idxSelPt] );

    //                    if( isLineHit(line,ptMouseItemPos,m_pDrawingScene->getHitToleranceInPx()) )
    //                    {
    //                        break;
    //                    }
    //                }

    //                if( idxSelPt < plg.size() )
    //                {
    //                    plg.insert(idxSelPt,ptMouseItemPos);

    //                    m_idxSelPtSelectedPolygon = idxSelPt;
    //                    m_editMode = EEditMode::MoveShapePoint;

    //                    m_arpSelPtsPolygon.insert(idxSelPt,nullptr);
    //                    m_arpSelPtsPolygon[idxSelPt] = pGraphObjSelPt =
    //                        new CGraphObjSelectionPoint(m_pDrawingScene, SGraphObjSelectionPoint(this, idxSelPt));
    //                    m_pDrawingScene->addGraphObj(pGraphObjSelPt);

    //                    //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
    //                    pGraphObjSelPt->installSceneEventFilter(this);

    //                    ptSel = plg[idxSelPt];
    //                    ptSel = mapToScene(ptSel);
    //                    pGraphObjSelPt->setPos(ptSel);
    //                }
    //            }
    //        } // if( m_editMode == EEditMode::Move )

    //        QGraphicsPolygonItem::setPolygon(plg);

    //        for( idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++ )
    //        {
    //            pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

    //            if( pGraphObjSelPt != nullptr )
    //            {
    //                pGraphObjSelPt->setShapePoint(idxSelPt);

    //                if( idxSelPt == m_idxSelPtSelectedPolygon )
    //                {
    //                    pGraphObjSelPt->setSelected(true);
    //                }
    //                else
    //                {
    //                    pGraphObjSelPt->setSelected(false);
    //                }
    //            }
    //        }

    //        m_pDrawingScene->setMode( EMode::Undefined, EEditTool::Undefined, m_editMode, m_editResizeMode, false );

    //        m_bCoorsDirty = true;

    //        updateLineEndPolygonCoors();

    //        //updateEditInfo();
    //        //updateToolTip();

    //    } // if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )

    //} // if( m_editMode == EEditMode::None )

} // mousePressEvent

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

    //Qt::MouseButtons iEvMouseButtonState = i_pEv->buttons() & Qt::MouseButtonMask;

    //// If no mouse button is pressed ...
    //if( (iEvMouseButtonState == Qt::NoButton) || (iEvMouseButtonState & Qt::LeftButton) )
    //{
    //    if( m_editMode == EEditMode::Creating || m_editMode == EEditMode::MoveShapePoint )
    //    {
    //        QPolygonF plg = polygon();
    //        QPointF   ptMouseItemPos = i_pEv->pos();

    //        if( m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < plg.size() )
    //        {
    //            plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;
    //        }

    //        QGraphicsPolygonItem::setPolygon(plg);

    //        CGraphObjSelectionPoint* pGraphObjSelPt;
    //        QPointF                  ptSel;

    //        if( m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < m_arpSelPtsPolygon.size() )
    //        {
    //            pGraphObjSelPt = m_arpSelPtsPolygon[m_idxSelPtSelectedPolygon];

    //            if( pGraphObjSelPt != nullptr )
    //            {
    //                ptSel = plg[m_idxSelPtSelectedPolygon];
    //                ptSel = mapToScene(ptSel);
    //                pGraphObjSelPt->setPos(ptSel);
    //            }
    //        }

    //        m_bCoorsDirty = true;

    //        updateLineEndPolygonCoors();

    //        //updateEditInfo();
    //        //updateToolTip();

    //    } // if( m_editMode == EEditMode::MoveShapePoint )

    //} // if( (iEvMouseButtonState == Qt::NoButton) || (iEvMouseButtonState & Qt::LeftButton) )

} // mouseMoveEvent

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

//    QPointF ptMouseItemPos = i_pEv->pos();
//
//    if( m_editMode == EEditMode::Creating )
//    {
//        QGraphicsPolygonItem::mouseReleaseEvent(i_pEv);
//    }
//
//    else if( m_editMode == EEditMode::MoveShapePoint )
//    {
//        QGraphicsPolygonItem::mouseReleaseEvent(i_pEv);
//
//        QPolygonF plg = polygon();
//
//        if( plg.size() >= 2 ) // anyway ..
//        {
//            CGraphObjConnectionPoint* pGraphObjCnctPt;
//            CGraphObjSelectionPoint*  pGraphObjSelPt;
//            int                       idxSelPt;
//            QPointF                   ptSel;
//
//            // If the first or the last shape point is selected ..
//            if( m_idxSelPtSelectedPolygon == 0 || m_idxSelPtSelectedPolygon == plg.size()-1 )
//            {
//                // The currently selected shape point will be fixed.
//                plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;
//
//                if( m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < m_arpSelPtsPolygon.size() )
//                {
//                    pGraphObjSelPt = m_arpSelPtsPolygon[m_idxSelPtSelectedPolygon];
//
//                    if( pGraphObjSelPt != nullptr )
//                    {
//                        ptSel = plg[m_idxSelPtSelectedPolygon];
//                        ptSel = mapToScene(ptSel);
//                        pGraphObjSelPt->setPos(ptSel);
//                    }
//                }
//
//                // .. it depends whether the shape point has been moved onto a connection point
//                // or somewhere else whether the connection line will be connected with the
//                // connection point or whether a new shape points will be inserted or added.
//                pGraphObjCnctPt = m_pDrawingScene->getConnectionPoint(i_pEv->scenePos());
//
//                // If the mouse has not been released on a connection point ..
//                if( pGraphObjCnctPt == nullptr )
//                {
//                    // Another point will be added.
//                    if( m_idxSelPtSelectedPolygon == 0 )
//                    {
//                        plg.insert(0,ptMouseItemPos);
//
//                        pGraphObjSelPt = new CGraphObjSelectionPoint(
//                            m_pDrawingScene, SGraphObjSelectionPoint(this, m_idxSelPtSelectedPolygon));
//                        m_arpSelPtsPolygon.insert(0, pGraphObjSelPt);
//                        QObject::connect(
//                            pGraphObjSelPt, &CGraphObj::aboutToBeDestroyed,
//                            this, &CGraphObjConnectionLine::onSelectionPointAboutToBeDestroyed);
//                    }
//                    else // if( m_idxSelPtSelectedPolygon == plg.size()-1 )
//                    {
//                        plg.append(ptMouseItemPos);
//                        m_idxSelPtSelectedPolygon = plg.size()-1;
//
//                        pGraphObjSelPt = new CGraphObjSelectionPoint(
//                            m_pDrawingScene, SGraphObjSelectionPoint(this, m_idxSelPtSelectedPolygon));
//                        m_arpSelPtsPolygon.append(pGraphObjSelPt);
//                        QObject::connect(
//                            pGraphObjSelPt, &CGraphObj::aboutToBeDestroyed,
//                            this, &CGraphObjConnectionLine::onSelectionPointAboutToBeDestroyed);
//                    }
//
//                    m_pDrawingScene->addGraphObj(pGraphObjSelPt);
//
//                    //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
//                    pGraphObjSelPt->installSceneEventFilter(this);
//
//                    ptSel = plg[m_idxSelPtSelectedPolygon];
//                    ptSel = mapToScene(ptSel);
//                    pGraphObjSelPt->setPos(ptSel);
//
//                    QGraphicsPolygonItem::setPolygon(plg);
//
//                    for( idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++ )
//                    {
//                        pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
//
//                        if( pGraphObjSelPt != nullptr )
//                        {
//                            pGraphObjSelPt->setShapePoint(idxSelPt);
//
//                            if( idxSelPt == m_idxSelPtSelectedPolygon )
//                            {
//                                pGraphObjSelPt->setSelected(true);
//                            }
//                            else
//                            {
//                                pGraphObjSelPt->setSelected(false);
//                            }
//                        }
//                    }
//
//                    // From now on the drawing scene must dispatch mouse events "directly" to me:
//                    m_pDrawingScene->onGraphObjAddingShapePointsStarted(this);
//
//                } // if( pGraphObjCnctPt == nullptr )
//
//                // If the mouse has been released on a connection point ..
//                else // if( pGraphObjCnctPt != nullptr )
//                {
//                    QPointF ptItemPosOld = pos();
//                    QPointF ptItemPosNew = mapToScene(plg[0]);
//
//                    double  dx, dy;
//                    int     idxPt;
//
//                    setPos(ptItemPosNew);
//
//                    dx = ptItemPosNew.x() - ptItemPosOld.x();
//                    dy = ptItemPosNew.y() - ptItemPosOld.y();
//
//                    QPointF ptPosOld, ptPosNew;
//
//                    for( idxPt = 0; idxPt < plg.size(); idxPt++ )
//                    {
//                        ptPosOld = plg[idxPt];
//                        ptPosNew.setX( ptPosOld.x() - dx );
//                        ptPosNew.setY( ptPosOld.y() - dy );
//                        plg[idxPt] = ptPosNew;
//                    }
//
//                    QGraphicsPolygonItem::setPolygon(plg);
//
//                    if( m_idxSelPtSelectedPolygon == 0 )
//                    {
//                        if( pGraphObjCnctPt != m_arpCnctPts[static_cast<int>(ELinePoint::Start)] )
//                        {
//                            setConnectionPoint(ELinePoint::Start,pGraphObjCnctPt);
//                        }
//                    }
//                    else // if( m_idxSelPtSelectedPolygon == plg.size()-1 )
//                    {
//                        if( pGraphObjCnctPt != m_arpCnctPts[static_cast<int>(ELinePoint::End)] )
//                        {
//                            setConnectionPoint(ELinePoint::End,pGraphObjCnctPt);
//                        }
//                    }
//
//                    normalize();
//
//                    m_editMode = EEditMode::None;
//                    m_editResizeMode = EEditResizeMode::None;
//                    m_idxSelPtSelectedPolygon = -1;
//                    m_selPtSelectedBoundingRect = ESelectionPoint::None;
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//                    acceptCurrentAsOriginalCoors();
//#endif
//                    // Adding shape points has been finished:
//                    m_pDrawingScene->onGraphObjAddingShapePointsFinished(this);
//
//                } // if( pGraphObjCnctPt != nullptr )
//
//                m_bCoorsDirty = true;
//
//                updateLineEndPolygonCoors();
//
//                //updateEditInfo();
//                //updateToolTip();
//
//            } // if( m_idxSelPtSelectedPolygon == 0 || m_idxSelPtSelectedPolygon == plg.size()-1 )
//
//            // If neither the first nor the last shape point is selected (but any shape point in between) ..
//            else if( m_idxSelPtSelectedPolygon > 0 && m_idxSelPtSelectedPolygon < plg.size()-1 )
//            {
//                if( m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < plg.size() )
//                {
//                    plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;
//
//                    QGraphicsPolygonItem::setPolygon(plg);
//
//                    if( m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < m_arpSelPtsPolygon.size() )
//                    {
//                        pGraphObjSelPt = m_arpSelPtsPolygon[m_idxSelPtSelectedPolygon];
//
//                        if( pGraphObjSelPt != nullptr )
//                        {
//                            ptSel = plg[m_idxSelPtSelectedPolygon];
//                            ptSel = mapToScene(ptSel);
//                            pGraphObjSelPt->setPos(ptSel);
//                        }
//                    }
//                }
//
//                normalize();
//
//                m_editMode = EEditMode::None;
//                m_editResizeMode = EEditResizeMode::None;
//                m_idxSelPtSelectedPolygon = -1;
//                m_selPtSelectedBoundingRect = ESelectionPoint::None;
//
//                m_bCoorsDirty = true;
//
//                updateLineEndPolygonCoors();
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//                acceptCurrentAsOriginalCoors();
//#endif
//                //updateEditInfo();
//                //updateToolTip();
//
//            } // if( m_idxSelPtSelectedPolygon > 0 && m_idxSelPtSelectedPolygon < plg.size()-1 )
//
//        } // if( plg.size() >= 1 )
//
//    } // else if( m_editMode == EEditMode::MoveShapePoint )
//
//    else if( m_editMode != EEditMode::None )
//    {
//        // The mouse release event would select the object.
//        // This is not wanted if the selection tool is not active.
//        bool bIsSelectable = flags() & QGraphicsItem::ItemIsSelectable;
//        bool bIsSelectableReset = false;
//
//        if( bIsSelectable && m_pDrawingScene->getEditTool() != EEditTool::Select )
//        {
//            setFlag(QGraphicsItem::ItemIsSelectable,false);
//            bIsSelectableReset = true;
//        }
//
//        QGraphicsPolygonItem::mouseReleaseEvent(i_pEv);
//
//        if( bIsSelectableReset )
//        {
//            setFlag(QGraphicsItem::ItemIsSelectable,bIsSelectable);
//        }
//
//        normalize();
//
//        m_editMode = EEditMode::None;
//        m_editResizeMode = EEditResizeMode::None;
//        m_idxSelPtSelectedPolygon = -1;
//        m_selPtSelectedBoundingRect = ESelectionPoint::None;
//
//        m_bCoorsDirty = true;
//
//        updateLineEndPolygonCoors();
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//        acceptCurrentAsOriginalCoors();
//#endif
//        //updateEditInfo();
//        //updateToolTip();
//
//    } // if( m_editMode != EEditMode::Creating )

} // mouseReleaseEvent

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
//            onCreateAndExecDlgFormatGraphObjs();
//        }
//
//    } // if( modeDrawing == EMode::Edit )

} // mouseDoubleClickEvent

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

    QVariant valChanged = i_value;

    bool bZValueChanged = false;
    bool bTreeEntryChanged = false;

    if (i_change == ItemSceneHasChanged) {
        // The item may have been removed from the scene.
        if (scene() != nullptr) {
        }
    }
    else if (i_change == ItemSelectedHasChanged) {
        prepareGeometryChange();
        if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
            bringToFront();
            //if (m_editMode == EEditMode::Creating) {
            //    showSelectionPoints(
            //        ESelectionPointsBoundingRectCorner|ESelectionPointsBoundingRectLineCenter|ESelectionPointsPolygonShapePoints);
            //}
            //else {
            //    showSelectionPoints();
            //}
            // Not necessary as item has been brought to front and "showSelectionPoints"
            // sets zValue of selection points above item.
            //bringSelectionPointsToFront();
            setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
        }
        else {
            setAcceptedMouseButtons(Qt::NoButton);
            hideSelectionPoints();
            resetStackingOrderValueToOriginalValue(); // restore ZValue as before selecting the object
            //m_editMode = EEditMode::None;
            //m_editResizeMode = EEditResizeMode::None;
            //m_selPtSelectedBoundingRect = ESelectionPoint::None;
            //m_idxSelPtSelectedPolygon = -1;
        }
        //updateEditInfo();
        //updateToolTip();
        bTreeEntryChanged = true;
    }
    else if( i_change == ItemTransformChange
          || i_change == ItemPositionHasChanged
          || i_change == ItemParentHasChanged
          || i_change == ItemTransformHasChanged
          #if QT_VERSION >= 0x040700
          || i_change == ItemScenePositionHasChanged
          || i_change == ItemRotationHasChanged
          || i_change == ItemScaleHasChanged
          || i_change == ItemTransformOriginPointHasChanged )
          #else
          || i_change == ItemScenePositionHasChanged )
          #endif
    {
        //updateEditInfo();
        //updateToolTip();
        update();
    }

    else if (i_change == ItemZValueHasChanged) {
        bZValueChanged = true;
        bTreeEntryChanged = true;
    }

    if (bZValueChanged) {
        emit_zValueChanged(zValue());
    }
    if (bTreeEntryChanged && m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }

    valChanged = QGraphicsPolygonItem::itemChange(i_change, i_value);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthRet = qGraphicsItemChange2Str(i_change, valChanged, false);
        mthTracer.setMethodReturn(strMthRet);
    }
    return valChanged;

} // itemChange

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::normalize()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "normalize",
        /* strAddInfo   */ "" );

    QPolygonF plgOld = polygon();
    QPolygonF plgNew = normalizePolygon( plgOld, m_pDrawingScene->getHitToleranceInPx() );
    if (plgOld.size() != plgNew.size()) {
        QGraphicsPolygonItem::setPolygon(plgNew);
        hideSelectionPoints();
        if (isSelected()) {
            showSelectionPoints();
        }
    }
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::updateLineEndPolygonCoors()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateLineEndPolygonCoors",
        /* strAddInfo   */ "" );

    if (m_bCoorsDirty) {
        CEnumLineEndStyle lineEndStyleLineStart = m_drawSettings.getLineEndStyle(ELinePoint::Start);
        CEnumLineEndStyle lineEndStyleLineEnd   = m_drawSettings.getLineEndStyle(ELinePoint::End);

        m_plgCurr = polygon();

        if( m_plgCurr.size() > 1 )
        {
            if( lineEndStyleLineStart != ELineEndStyle::Normal || lineEndStyleLineEnd != ELineEndStyle::Normal )
            {
                CEnumArrowHeadBaseLineType baseLineTypeLineStart = m_drawSettings.getArrowHeadBaseLineType(ELinePoint::Start);
                CEnumArrowHeadBaseLineType baseLineTypeLineEnd   = m_drawSettings.getArrowHeadBaseLineType(ELinePoint::End);

                QLineF linFirst( m_plgCurr[0], m_plgCurr[1] );
                QLineF linLast( m_plgCurr[m_plgCurr.size()-2], m_plgCurr[m_plgCurr.size()-1] );

                if( lineEndStyleLineStart != ELineEndStyle::Normal )
                {
                    getLineEndPolygons(
                        /* line          */ linFirst,
                        /* drawSetings   */ m_drawSettings,
                        /* pplgLineStart */ &m_plgLineStart,
                        /* pplgLineEnd   */ nullptr );

                    if( baseLineTypeLineStart != EArrowHeadBaseLineType::NoLine )
                    {
                        if( m_plgLineStart.size() == 4 )
                        {
                            m_plgCurr[0] = m_plgLineStart[3];
                        }
                    }
                }

                if( lineEndStyleLineEnd != ELineEndStyle::Normal )
                {
                    getLineEndPolygons(
                        /* line          */ linLast,
                        /* drawSetings   */ m_drawSettings,
                        /* pplgLineStart */ nullptr,
                        /* pplgLineEnd   */ &m_plgLineEnd );

                    if( baseLineTypeLineEnd != EArrowHeadBaseLineType::NoLine )
                    {
                        if( m_plgLineEnd.size() == 4 )
                        {
                            m_plgCurr[m_plgCurr.size()-1] = m_plgLineEnd[3];
                        }
                    }
                }

            } // if( lineEndStyleLineStart != ELineEndStyle::Normal || lineEndStyleLineEnd != ELineEndStyle::Normal )

        } // if( m_plg.size() > 1 )

    } // if( m_bCoorsDirty )

    m_bCoorsDirty = false;

} // updateLineEndPolygonCoors
