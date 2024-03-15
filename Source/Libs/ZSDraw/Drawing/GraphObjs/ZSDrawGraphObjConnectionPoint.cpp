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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionLine.h"
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
class CGraphObjConnectionPoint : public CGraphObj, public QGraphicsEllipseItem
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjConnectionPoint::s_iInstCount = 0;

double CGraphObjConnectionPoint::s_fOuterCircleRadius_px = 5.0;
double CGraphObjConnectionPoint::s_fInnerCircleRadius_px = 2.0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjConnectionPoint::CGraphObjConnectionPoint(
    CDrawingScene* i_pDrawingScene, const QString& i_strObjName) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameConnections,
        /* type                */ EGraphObjTypeConnectionPoint,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeConnectionPoint),
        /* strObjName          */ i_strObjName.isEmpty() ? "ConnectionPoint" + QString::number(s_iInstCount) : i_strObjName),
    QGraphicsEllipseItem( QRectF( QPointF(0.0,0.0), QSize(2.0*s_fOuterCircleRadius_px,2.0*s_fOuterCircleRadius_px) ) ),
    m_lstConnectionLines(),
    m_fInnerCircleWidth_perCent(100.0*(s_fInnerCircleRadius_px/s_fOuterCircleRadius_px))
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    m_rctCurr = rect();
    m_ptRotOriginCurr = rect().center();
#endif

    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemSendsGeometryChanges);

    //onDrawSettingsChanged();

    //updateToolTip();

} // ctor

//------------------------------------------------------------------------------
CGraphObjConnectionPoint::~CGraphObjConnectionPoint()
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

    if (m_lstConnectionLines.size() > 0) {
        for (int idxLine = m_lstConnectionLines.count()-1; idxLine >= 0; idxLine--) {
            CGraphObjConnectionLine* pGraphObjCnctLine = m_lstConnectionLines[idxLine];
            m_lstConnectionLines[idxLine] = nullptr;
            try {
                delete pGraphObjCnctLine; // as a reentry dtor of connection line calls "removeConnectionLine"
            }
            catch(...) {
            }
            pGraphObjCnctLine = nullptr;
        }
    }
} // dtor

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the type method of QGraphicsItem.
*/
int CGraphObjConnectionPoint::type() const
//------------------------------------------------------------------------------
{
    return EGraphObjTypeConnectionPoint;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjConnectionPoint::clone()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal))
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "clone",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjConnectionPoint* pGraphObj = new CGraphObjConnectionPoint(m_pDrawingScene);
    pGraphObj->setDrawSettings(m_drawSettings);

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    pGraphObj->setName(m_strName);
    pGraphObj->setPos(pos());
    pGraphObj->setSize(m_rctCurr.size());
    pGraphObj->setInnerCircleWidthInPerCent(m_fInnerCircleWidth_perCent);
    pGraphObj->setRotationAngleInDegree(m_fRotAngleCurr_deg);
    //pGraphObj->setScaleFactors(m_fScaleFacXCurr,m_fScaleFacYCurr);
    pGraphObj->acceptCurrentAsOriginalCoors();
#endif

    return pGraphObj;

} // clone

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjConnectionPoint::appendConnectionLine( CGraphObjConnectionLine* i_pGraphObjCnctLine )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal))
    {
        if( i_pGraphObjCnctLine == nullptr )
        {
            strMthInArgs = "CnctLine:nullptr";
        }
        else
        {
            strMthInArgs = "CnctLine:" + i_pGraphObjCnctLine->name();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "appendConnectionLine",
        /* strAddInfo   */ strMthInArgs );

    bool bConnected = false;

    if( i_pGraphObjCnctLine != nullptr )
    {
        int idxLineTmp = findConnectionLineIdx(i_pGraphObjCnctLine);

        // If the connection line is not yet connected with me ...
        if( idxLineTmp < 0 )
        {
            m_lstConnectionLines.append(i_pGraphObjCnctLine);
            bConnected = true;
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        mthTracer.setMethodReturn( "Connected: " + bool2Str(bConnected) );
    }

    return bConnected;

} // appendConnectionLine

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::removeConnectionLine( CGraphObjConnectionLine* i_pGraphObjCnctLine )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal))
    {
        if( i_pGraphObjCnctLine == nullptr )
        {
            strMthInArgs = "CnctLine:nullptr";
        }
        else
        {
            strMthInArgs = "CnctLine:" + i_pGraphObjCnctLine->name();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "removeConnectionLine",
        /* strAddInfo   */ strMthInArgs );

    if( i_pGraphObjCnctLine != nullptr )
    {
        int iLineIdx = findConnectionLineIdx(i_pGraphObjCnctLine);

        if( iLineIdx < 0 || iLineIdx >= m_lstConnectionLines.count() )
        {
            // If a connection line is destroyed by the dtor of the connection point
            // "removeConnectionLine" is called as a reentry by the dtor of the connection line.
        }
        else // if( iLineIdx >= 0 && iLineIdx < m_lstConnectionLines.count() )
        {
            m_lstConnectionLines.removeAt(iLineIdx);
        }
    }

} // removeConnectionLine

//------------------------------------------------------------------------------
int CGraphObjConnectionPoint::findConnectionLineIdx( CGraphObjConnectionLine* i_pGraphObjCnctLine )
//------------------------------------------------------------------------------
{
    int idxLine = -1;

    if( i_pGraphObjCnctLine != nullptr )
    {
        int idxLineTmp;

        for( idxLineTmp = 0; idxLineTmp < m_lstConnectionLines.count(); idxLineTmp++ )
        {
            if( m_lstConnectionLines[idxLineTmp] == i_pGraphObjCnctLine )
            {
                idxLine = idxLineTmp;
                break;
            }
        }
    }
    return idxLine;

} // findConnectionLineIdx

//------------------------------------------------------------------------------
int CGraphObjConnectionPoint::getConnectionLinesCount() const
//------------------------------------------------------------------------------
{
    return m_lstConnectionLines.count();
}

//------------------------------------------------------------------------------
CGraphObjConnectionLine* CGraphObjConnectionPoint::getConnectionLine( int i_iLineIdx )
//------------------------------------------------------------------------------
{
    CGraphObjConnectionLine* pGraphObjCnctLine = nullptr;

    if( i_iLineIdx >= 0 && i_iLineIdx < m_lstConnectionLines.count() )
    {
        pGraphObjCnctLine = m_lstConnectionLines[i_iLineIdx];
    }
    return pGraphObjCnctLine;

} // getConnectionLine

/*==============================================================================
public: // replacing methods of QGraphicsEllipseItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setRect( const QRectF& i_rct )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "Rect:" + rect2Str(i_rct);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setRect",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsEllipseItem::setRect(i_rct);

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    m_rctCurr = i_rct;
    m_ptRotOriginCurr = m_rctCurr.center();

    if (isSelected()) {
        //updateSelectionPointsOfBoundingRect(m_rctCurr);
    }
#endif

    // As "setRect" does not end up in an "itemChange" call (even if the
    // flag "ItemSendsGeometryChanges" has been set) we call the "itemChange"
    // method on our own to update the position of the selection points.
    //itemChange( ItemPositionHasChanged, pos() );

} // setRect

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setRect( qreal i_x, qreal i_y, qreal i_width, qreal i_height )
//------------------------------------------------------------------------------
{
    setRect( QRectF( QPointF(i_x,i_y), QSizeF(i_width,i_height) ) );
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setInnerCircleWidthInPerCent( double i_fWidth_perCent )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Width:" + QString::number(i_fWidth_perCent) + "%";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setInnerCircleWidthInPerCent",
        /* strAddInfo   */ strMthInArgs );

    m_fInnerCircleWidth_perCent = i_fWidth_perCent;

    update();
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setInnerCircleWidthInPx( double i_fWidth_px )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Width:" + QString::number(i_fWidth_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setInnerCircleRadiusInPx",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        QRectF rctBounding = pGraphicsItem->boundingRect();
        if (rctBounding.width() != 0.0) {
            m_fInnerCircleWidth_perCent = 100.0*fabs(i_fWidth_px/rctBounding.width());
        }
        else {
            m_fInnerCircleWidth_perCent = 100.0;
        }
        update();
    }
}

//------------------------------------------------------------------------------
double CGraphObjConnectionPoint::getInnerCircleWidthInPerCent()
//------------------------------------------------------------------------------
{
    return m_fInnerCircleWidth_perCent;
}

//------------------------------------------------------------------------------
double CGraphObjConnectionPoint::getInnerCircleWidthInPx()
//------------------------------------------------------------------------------
{
    double fWidth = 0.0;
    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        QRectF rctBounding = pGraphicsItem->boundingRect();
        if (rctBounding.width() != 0.0) {
            fWidth = (m_fInnerCircleWidth_perCent/100.0) * rctBounding.width();
        }
    }
    return fWidth;
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjConnectionPoint::getScenePolygonShapePointsString() const
//------------------------------------------------------------------------------
{
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QPolygonF plgScene = pGraphicsItemThis->mapToScene(rect());
    return polygon2Str(plgScene);
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld)
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

    if( m_drawSettings.isPenUsed() )
    {
        if( m_drawSettings.getLineStyle() != ELineStyle::NoLine )
        {
            QPen pen;

            pen.setColor( m_drawSettings.getPenColor() );
            pen.setWidth( m_drawSettings.getPenWidth() );
            pen.setStyle( lineStyle2QtPenStyle(m_drawSettings.getLineStyle()) );

            setPen(pen);
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

    if( m_drawSettings.isFillUsed() )
    {
        if( m_drawSettings.getFillStyle() != EFillStyle::NoFill )
        {
            QBrush brsh;

            brsh.setColor( m_drawSettings.getFillColor() );
            brsh.setStyle( fillStyle2QtBrushStyle(m_drawSettings.getFillStyle()) );

            setBrush(brsh);
        }
        else
        {
            setBrush(Qt::NoBrush);
        }
    }
    else
    {
        setBrush(Qt::NoBrush);
    }

} // onDrawSettingsChanged

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObjConnectionPoint::setWidth( const CPhysVal& i_physValWidth )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_physValWidth.toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "setWidth",
//        /* strAddInfo   */ strMthInArgs );
//
//    // Width and height for circles should be equal:
//    setSize(i_physValWidth, i_physValWidth);
//}
//
////------------------------------------------------------------------------------
//void CGraphObjConnectionPoint::setHeight( const CPhysVal& i_physValHeight )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_physValHeight.toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "setHeight",
//        /* strAddInfo   */ strMthInArgs );
//
//    // Width and height for circles should be equal:
//    setSize(i_physValHeight, i_physValHeight);
//}
//
////------------------------------------------------------------------------------
//void CGraphObjConnectionPoint::setSize( const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_physValWidth.toString() + ", " + i_physValHeight.toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "setSize",
//        /* strAddInfo   */ strMthInArgs );
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
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//    if (pGraphicsItem != nullptr) {
//        QRectF rct = pGraphicsItem->boundingRect();
//        rct.setWidth(physValWidth.getVal(Units.Length.px));
//        rct.setHeight(physValHeight.getVal(Units.Length.px));
//        setRect(rct);
//    }
//} // setSize
//
////------------------------------------------------------------------------------
//void CGraphObjConnectionPoint::setSize(const CPhysValSize& i_physValSize )
////------------------------------------------------------------------------------
//{
//    setSize(i_physValSize.width(), i_physValSize.height());
//}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObjConnectionPoint::setIsHit( bool i_bHit )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "Hit:" + bool2Str(i_bHit);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "setIsHit",
//        /* strAddInfo   */ strMthInArgs );
//
//    if (m_bIsHit != i_bHit) {
//        m_bIsHit = i_bHit;
//        update();
//    }
//}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
//bool CGraphObjConnectionPoint::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjIsHit, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "Point:" + point2Str(i_pt) +
//            ", HitInfo {" + QString(o_pHitInfo == nullptr ? "null" : pointer2Str(o_pHitInfo)) + "}" +
//            ", Rect(x,y,w,h) {" + rect2Str(rect()) + "}";
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjIsHit,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "isHit",
//        /* strAddInfo   */ strMthInArgs );
//
//    bool bIsHit = false;
//
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//    if (pGraphicsItem != nullptr) {
//        QRectF rct = rect();
//        bIsHit = isEllipseHit(rct, EFillStyle::SolidPattern, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo);
//        if (bIsHit) {
//            if (o_pHitInfo != nullptr) {
//                //o_pHitInfo->m_editMode = EEditMode::Move;
//                //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
//                o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
//                o_pHitInfo->m_idxPolygonShapePoint = -1;
//                o_pHitInfo->m_idxLineSegment = -1;
//                o_pHitInfo->m_ptSelected = i_pt;
//                o_pHitInfo->m_cursor = Qt::SizeAllCursor;
//            }
//        }
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
//void CGraphObjConnectionPoint::setCursor( const QCursor& i_cursor )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal))
//    {
//        strMthInArgs = qCursorShape2Str(i_cursor.shape());
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "setCursor",
//        /* strAddInfo   */ strMthInArgs );
//
//    QGraphicsEllipseItem::setCursor(i_cursor);
//
//} // setCursor

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Called by the parent graphic item to inform the child about geometry changes.

    Connection lines don't belong to groups. But their connection points do.
    If a group is moved also the connection points are moved by Qt's graphics scene.
    But not the connection lines which are linked to the connection points.
    "onGraphObjParentGeometryChanged" is called to inform the connection points in order to
    forward the call to child groups and the connection points or their connection lines.
*/
void CGraphObjConnectionPoint::onGraphObjParentGeometryChanged( CGraphObj* i_pGraphObjParent )
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

    for (int idxLine = 0; idxLine < m_lstConnectionLines.count(); idxLine++) {
        CGraphObjConnectionLine* pGraphObjCnctLine = m_lstConnectionLines[idxLine];
        if (pGraphObjCnctLine != nullptr) {
            pGraphObjCnctLine->onGraphObjParentGeometryChanged(this);
        }
    }
    //updateEditInfo();
    //updateToolTip();
}

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjConnectionPoint::boundingRect() const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjBoundingRect, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs  = "Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strMthInArgs += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ strMthInArgs );

    QRectF rctBounding = QGraphicsEllipseItem::boundingRect();

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

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs  = "Rect(x,y,w,h):(";
        strMthInArgs += QString::number(rctBounding.x(),'f',1);
        strMthInArgs += "," + QString::number(rctBounding.y(),'f',1);
        strMthInArgs += "," + QString::number(rctBounding.width(),'f',1);
        strMthInArgs += "," + QString::number(rctBounding.height(),'f',1) + ")";
        mthTracer.setMethodReturn(strMthInArgs);
    }

    return rctBounding;

} // boundingRect

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::paint(
    QPainter*                       i_pPainter,
    const QStyleOptionGraphicsItem* /*i_pStyleOption*/,
    QWidget*                        /*i_pWdgt*/ )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjPaint, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ZValue: " + QString::number(zValue());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "paint",
        /* strAddInfo   */ strMthInArgs );

    i_pPainter->save();

    QPen   pn = pen();
    QBrush brsh = brush();

    QRectF rctOuterCircle = rect();

    if( m_fInnerCircleWidth_perCent < 100.0 )
    {
        if( m_pDrawingScene->getMode() == EMode::Edit && (/*m_bIsHit ||*/ isSelected()) )
        {
            pn.setStyle(Qt::SolidLine);
            pn.setColor(Qt::blue);

            brsh.setColor(Qt::cyan);
            brsh.setStyle(Qt::SolidPattern);

            i_pPainter->setPen(pn);
            i_pPainter->setBrush(brsh);
        }
        else
        {
            pn   = pen();
            brsh = Qt::NoBrush;

            i_pPainter->setPen(pn);
            i_pPainter->setBrush(brsh);
        }

        i_pPainter->drawEllipse(rctOuterCircle);
    }

    double fInnerCircleWidth_px = getInnerCircleWidthInPx();

    if( fInnerCircleWidth_px >= 1.0 )
    {
        if( m_pDrawingScene->getMode() == EMode::Edit && (/*m_bIsHit ||*/ isSelected()) )
        {
            pn.setStyle(Qt::SolidLine);
            pn.setColor(Qt::blue);

            brsh.setColor(Qt::blue);
            brsh.setStyle(Qt::SolidPattern);

            i_pPainter->setPen(pn);
            i_pPainter->setBrush(brsh);
        }
        else
        {
            pn   = pen();
            brsh = brush();

            i_pPainter->setPen(pn);
            i_pPainter->setBrush(brsh);
        }

        QRectF rctInnerCircle;

        rctInnerCircle.setLeft( rctOuterCircle.center().x()-fInnerCircleWidth_px/2.0 );
        rctInnerCircle.setTop( rctOuterCircle.center().y()-fInnerCircleWidth_px/2.0 );
        rctInnerCircle.setWidth(fInnerCircleWidth_px);
        rctInnerCircle.setHeight(fInnerCircleWidth_px);

        i_pPainter->drawEllipse(rctInnerCircle);
    }

    if( m_pDrawingScene->getMode() == EMode::Edit && (/*m_bIsHit ||*/ isSelected()) )
    {
        pn.setStyle(Qt::DotLine);
        pn.setColor(Qt::blue);

        i_pPainter->setPen(pn);
        i_pPainter->setBrush(Qt::NoBrush);

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
//bool CGraphObjConnectionPoint::sceneEvent( QEvent* i_pEv )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjSceneEvent, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "Event:" + qEvent2Str(i_pEv);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjSceneEvent,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "sceneEvent",
//        /* strAddInfo   */ strMthInArgs );
//
//    bool bEventHandled = false;
//
//    // switch type() added for debugging purposes (to allow setting a breakpoint depending on event type)
//    switch( i_pEv->type() )
//    {
//        case QEvent::GraphicsSceneMouseMove:
//        {
//            bEventHandled = QGraphicsEllipseItem::sceneEvent(i_pEv);
//            break;
//        }
//        case QEvent::GraphicsSceneMousePress:
//        {
//            bEventHandled = QGraphicsEllipseItem::sceneEvent(i_pEv);
//            break;
//        }
//        case QEvent::GraphicsSceneMouseRelease:
//        {
//            bEventHandled = QGraphicsEllipseItem::sceneEvent(i_pEv);
//            break;
//        }
//        case QEvent::GraphicsSceneMouseDoubleClick:
//        {
//            bEventHandled = QGraphicsEllipseItem::sceneEvent(i_pEv);
//            break;
//        }
//        case QEvent::GraphicsSceneContextMenu:
//        {
//            bEventHandled = QGraphicsEllipseItem::sceneEvent(i_pEv);
//            break;
//        }
//        case QEvent::GraphicsSceneHoverEnter:
//        case QEvent::GraphicsSceneHoverMove:
//        case QEvent::GraphicsSceneHoverLeave:
//        {
//            QGraphicsSceneHoverEvent* pEv = dynamic_cast<QGraphicsSceneHoverEvent*>(i_pEv);
//
//            // Hover events should be handled by this connection point but should not be
//            // dispatched to the ancestors (parent) objects.
//            if( pEv != nullptr )
//            {
//                if( i_pEv->type() == QEvent::GraphicsSceneHoverEnter )
//                {
//                    hoverEnterEvent(pEv);
//                }
//                else if( i_pEv->type() == QEvent::GraphicsSceneHoverMove )
//                {
//                    hoverMoveEvent(pEv);
//                }
//                else if( i_pEv->type() == QEvent::GraphicsSceneHoverLeave )
//                {
//                    hoverLeaveEvent(pEv);
//                }
//            }
//            else
//            {
//                bEventHandled = QGraphicsEllipseItem::sceneEvent(i_pEv);
//            }
//            break;
//        }
//        case QEvent::GraphicsSceneHelp:
//        case QEvent::GraphicsSceneDragEnter:
//        case QEvent::GraphicsSceneDragMove:
//        case QEvent::GraphicsSceneDragLeave:
//        case QEvent::GraphicsSceneDrop:
//        case QEvent::GraphicsSceneWheel:
//        default:
//        {
//            bEventHandled = QGraphicsEllipseItem::sceneEvent(i_pEv);
//            break;
//        }
//
//    } // switch( i_pEv->type() )
//
//    return bEventHandled;
//
//} // sceneEvent
//
////------------------------------------------------------------------------------
//bool CGraphObjConnectionPoint::sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv )
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
//        /* strObjName   */ m_strName,
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
//        case QEvent::GraphicsSceneContextMenu:
//        case QEvent::GraphicsSceneHoverEnter:
//        case QEvent::GraphicsSceneHoverMove:
//        case QEvent::GraphicsSceneHoverLeave:
//        case QEvent::GraphicsSceneHelp:
//        case QEvent::GraphicsSceneDragEnter:
//        case QEvent::GraphicsSceneDragMove:
//        case QEvent::GraphicsSceneDragLeave:
//        case QEvent::GraphicsSceneDrop:
//        case QEvent::GraphicsSceneWheel:
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
void CGraphObjConnectionPoint::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
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

    //QPointF ptItemPos = mapFromScene(i_pEv->scenePos());

    //CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
    //CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    //if( modeDrawing == EMode::Edit )
    //{
    //    if( editToolDrawing == EEditTool::Select )
    //    {
    //        SGraphObjHitInfo hitInfo;

    //        bool bIsHit = isHit(ptItemPos,&hitInfo);

    //        if( bIsHit )
    //        {
    //            if( cursor().shape() != hitInfo.m_cursor.shape() )
    //            {
    //                setCursor(hitInfo.m_cursor);
    //            }
    //        }
    //    }
    //    else if( editToolDrawing == EEditTool::CreateObjects )
    //    {
    //        if( m_pDrawingScene->getCurrentDrawingToolGraphObjType() == EGraphObjTypeConnectionLine )
    //        {
    //            //QPixmap pxmCursor(":/ZS/Draw/CursorPin16x16.png");
    //            //setCursor(QCursor(pxmCursor,0,pxmCursor.height()-1));
    //        }
    //    }

    //} // if( modeDrawing == EMode::Edit )

} // hoverEnterEvent

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
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

    //QPointF ptItemPos = mapFromScene(i_pEv->scenePos());

    //CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
    //CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    //if( modeDrawing == EMode::Edit )
    //{
    //    if( editToolDrawing == EEditTool::Select )
    //    {
    //        SGraphObjHitInfo hitInfo;

    //        bool bIsHit = isHit(ptItemPos,&hitInfo);

    //        if( bIsHit )
    //        {
    //            if( cursor().shape() != hitInfo.m_cursor.shape() )
    //            {
    //                setCursor(hitInfo.m_cursor);
    //            }
    //        }
    //    }
    //    else if( editToolDrawing == EEditTool::CreateObjects )
    //    {
    //        if( m_pDrawingScene->getCurrentDrawingToolGraphObjType() == EGraphObjTypeConnectionLine )
    //        {
    //            //QPixmap pxmCursor(":/ZS/Draw/CursorPin16x16.png");
    //            //setCursor(QCursor(pxmCursor,0,pxmCursor.height()-1));
    //        }
    //    }

    //} // if( modeDrawing == EMode::Edit )

} // hoverMoveEvent

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
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

} // hoverLeaveEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
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

//    CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
//    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();
//
//    if( m_editMode == EEditMode::Creating )
//    {
//        // The object has been initially created.
//        m_editMode = EEditMode::None;
//        m_editResizeMode = EEditResizeMode::None;
//        m_idxSelPtSelectedPolygon = -1;
//        m_selPtSelectedBoundingRect = ESelectionPoint::None;
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//        acceptCurrentAsOriginalCoors();
//#endif
//
//        //m_pDrawingScene->onGraphObjCreationFinished(this);
//
//        //updateEditInfo();
//        //updateToolTip();
//
//    } // if( m_editMode == EEditMode::Creating )
//
//    else if( m_editMode == EEditMode::None )
//    {
//        if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )
//        {
//            QGraphicsEllipseItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)
//
//            m_editMode       = EEditMode::Move;
//            m_editResizeMode = EEditResizeMode::None;
//            m_idxSelPtSelectedPolygon = 0;
//
//            m_pDrawingScene->setMode( EMode::Undefined, EEditTool::Undefined, m_editMode, m_editResizeMode, false );
//
//            //updateEditInfo();
//            //updateToolTip();
//
//        } // if( editToolDrawing == EEditTool::Select || m_editMode == EEditMode::Creating )
//
//    } // if( m_editMode == EEditMode::None )

} // mousePressEvent

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    //if( m_editMode == EEditMode::Move )
    //{
    //    QGraphicsEllipseItem::mouseMoveEvent(i_pEv);

    //    //updateEditInfo();
    //    //updateToolTip();

    //} // if( m_editMode == EEditMode::Move )

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
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

//    CEnumEditMode editModePrev = m_editMode;
//
//    m_editMode = EEditMode::None;
//    m_editResizeMode = EEditResizeMode::None;
//    m_idxSelPtSelectedPolygon = -1;
//    m_selPtSelectedBoundingRect = ESelectionPoint::None;
//
//    if( editModePrev == EEditMode::Move )
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
//        QGraphicsEllipseItem::mouseReleaseEvent(i_pEv);
//
//        if( bIsSelectableReset )
//        {
//            setFlag(QGraphicsItem::ItemIsSelectable,bIsSelectable);
//        }
//    }
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//    acceptCurrentAsOriginalCoors();
//#endif
//
//    //updateEditInfo();
//    //updateToolTip();
//

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
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
    //QGraphicsEllipseItem::mouseDoubleClickEvent(i_pEv);

    //CEnumMode modeDrawing = m_pDrawingScene->getMode();

    //if( modeDrawing == EMode::Edit )
    //{
    //    if( isSelected() )
    //    {
    //        onCreateAndExecDlgFormatGraphObjs();
    //    }

    //} // if( modeDrawing == EMode::Edit )

} // mouseDoubleClickEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphObjConnectionPoint::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
//------------------------------------------------------------------------------
{
    if( m_bDtorInProgress )
    {
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

    QVariant valChanged = i_value;

    bool bZValueChanged = false;
    bool bTreeEntryChanged = false;

    if (i_change == ItemSceneHasChanged) {
        // The item may have been removed from the scene.
        if (scene() != nullptr) {
        }
    }
    else if(i_change == ItemSelectedHasChanged) {
        prepareGeometryChange();
        // The connection point does not have selection points but is a selection point itself.
        if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
            bringToFront();
            setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
        }
        else {
            setAcceptedMouseButtons(Qt::NoButton);
            resetStackingOrderValueToOriginalValue(); // restore ZValue as before selecting the object
        }
        //updateEditInfo();
        //updateToolTip();
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemPositionHasChanged || i_change == ItemParentHasChanged) {
        for (int idxLine = 0; idxLine < m_lstConnectionLines.count(); idxLine++)
        {
            CGraphObjConnectionLine* pGraphObjCnctLine = m_lstConnectionLines[idxLine];
            if (pGraphObjCnctLine != nullptr) {
                pGraphObjCnctLine->onGraphObjParentGeometryChanged(this);
            }
        }
        //updateEditInfo();
        //updateToolTip();
    }
    else if( i_change == ItemTransformChange
          || i_change == ItemPositionHasChanged
          || i_change == ItemTransformHasChanged
          || i_change == ItemParentHasChanged
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

    valChanged = QGraphicsItem::itemChange(i_change, i_value);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthRet = qGraphicsItemChange2Str(i_change, valChanged, false);
        mthTracer.setMethodReturn(strMthRet);
    }
    return valChanged;

} // itemChange
