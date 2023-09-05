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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjEllipse.h"
#include "ZSDraw/Common/ZSDrawAux.h"
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
class CGraphObjEllipse : public CGraphObj, public QGraphicsEllipseItem
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjEllipse::s_iInstCount = 0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjEllipse::CGraphObjEllipse(
    CDrawingScene*       i_pDrawingScene,
    const CDrawSettings& i_drawSettings,
    const QString&       i_strObjName ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ EGraphObjTypeEllipse,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeEllipse),
        /* strObjName          */ i_strObjName.isEmpty() ? "Ellipse" + QString::number(s_iInstCount) : i_strObjName,
        /* drawSettings        */ i_drawSettings ),
    QGraphicsEllipseItem()
{
    // Just incremented by the ctor but not decremented by the dtor.
    // Used to create a unique name for newly created objects of this type.
    s_iInstCount++;

    QString strMthInArgs;

    createTraceAdminObjs("StandardShapes::" + ClassName());

    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    m_ptRotOriginCurr = rect().center();
#endif

    setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges );

    onDrawSettingsChanged();

    updateToolTip();

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strMthInArgs  = "Selected:" + bool2Str(isSelected());
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strMthInArgs += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strMthInArgs += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
        mthTracer.trace(strMthInArgs);
    }

} // ctor

//------------------------------------------------------------------------------
CGraphObjEllipse::~CGraphObjEllipse()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    // Please note that the dynamic cast to QGraphicsItem returns nullptr if the
    // dtor of QGraphicsItem has already been executed. The order the dtors
    // of inherited classes are called depends on the order the classes
    // appear in the list of the inherited classes on defining the
    // class implementation. So we can't call "removeItem" from within the
    // dtor of the base class CGraphObj but must remove the graphics item from
    // the drawing scene's item list before the dtor of class QGraphicsItem is
    // called. And this is only always the case in the dtor of the class
    // derived from QGraphicsItem.

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        if( m_pDrawingScene != nullptr )
        {
            if( !m_strKeyInTree.isEmpty() )
            {
                try
                {
                    // Cannot be called from within dtor of "CGraphObj" as the dtor
                    // of class "QGraphicsItem" may have already been processed and
                    // models and Views may still try to access the graphical object.
                    m_pDrawingScene->onGraphObjAboutToBeDestroyed(m_strKeyInTree);
                }
                catch(...)
                {
                }
            }

            // Please note that the dynamic cast to QGraphicsItem returns nullptr if the
            // dtor of QGraphicsItem has already been executed. The order the dtors
            // of inherited classes are called depends on the order the classes
            // appear in the list of the inherited classes on defining the
            // class implementation. So we can't call "removeItem" from within the
            // dtor of the base class CGraphObj but must remove the graphics item from
            // the drawing scene's item list before the dtor of class QGraphicsItem is
            // called. And this is only always the case in the dtor of the class
            // derived from QGraphicsItem.
            // Moreover on removing (deleting) a group the group's children have already
            // been removed from the drawing scene by the dtor of class QGraphicsItemGroup
            // (which is inherited by CGraphObjGroup) and "scene()" may return nullptr.
            m_pDrawingScene->removeGraphObj(this);

        } // if( m_pDrawingScene != nullptr )
    } // if( pGraphicsItem != nullptr )

} // dtor

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjEllipse::clone()
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

    CGraphObjEllipse* pGraphObj = new CGraphObjEllipse(m_pDrawingScene,m_drawSettings);

    pGraphObj->setName(m_strName);
    pGraphObj->setRect( rect() );
    pGraphObj->setPos( pos() );
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    pGraphObj->setRotationAngleInDegree(m_fRotAngleCurr_deg);
    //pGraphObj->setScaleFactors(m_fScaleFacXCurr,m_fScaleFacYCurr);
    pGraphObj->acceptCurrentAsOriginalCoors();
#endif

    return pGraphObj;

} // clone

/*==============================================================================
public: // replacing methods of QGraphicsEllipseItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjEllipse::setRect( const QRectF& i_rct )
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
        updateSelectionPointsOfBoundingRect(m_rctCurr);
    }
#endif

    updateLabelPositionsAndContents();

    // As "setRect" does not end up in an "itemChange" call (even if the
    // flag "ItemSendsGeometryChanges" has been set) we call the "itemChange"
    // method on our own to update the position of the selection points.
    //itemChange( ItemPositionHasChanged, pos() );

} // setRect

//------------------------------------------------------------------------------
void CGraphObjEllipse::setRect( qreal i_x, qreal i_y, qreal i_width, qreal i_height )
//------------------------------------------------------------------------------
{
    setRect( QRectF( QPointF(i_x,i_y), QSizeF(i_width,i_height) ) );
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjEllipse::getScenePolygonShapePointsString() const
//------------------------------------------------------------------------------
{
    QString   strScenePolygonShapePoints;
    QRectF    rct = rect();
    QPolygonF plgScene = mapToScene(rct);

    strScenePolygonShapePoints = polygon2Str(plgScene);

    return strScenePolygonShapePoints;

} // getScenePolygonShapePointsString

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjEllipse::onDrawSettingsChanged()
//------------------------------------------------------------------------------
{
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

//------------------------------------------------------------------------------
void CGraphObjEllipse::setWidth( const CPhysVal& i_physValWidth )
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

    setSize(i_physValWidth, getHeight());
}

//------------------------------------------------------------------------------
void CGraphObjEllipse::setHeight( const CPhysVal& i_physValHeight )
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

    setSize(getWidth(), i_physValHeight);
}

//------------------------------------------------------------------------------
void CGraphObjEllipse::setSize( const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight )
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

    if (hasFixedWidth() && physValWidth != getFixedWidth()) {
        physValWidth = getFixedWidth();
    }
    else {
        if (hasMinimumWidth() && physValWidth > getMinimumWidth()) {
            physValWidth = getMinimumWidth();
        }
        if (hasMaximumWidth() && physValWidth > getMaximumWidth()) {
            physValWidth = getMaximumWidth();
        }
    }

    if (hasFixedHeight() && physValHeight != getFixedHeight()) {
        physValHeight = getFixedHeight();
    }
    else {
        if (hasMinimumHeight() && physValHeight > getMinimumHeight()) {
            physValHeight = getMinimumHeight();
        }
        if (hasMaximumHeight() && physValHeight > getMaximumHeight()) {
            physValHeight = getMaximumHeight();
        }
    }

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    if( fWidth != m_rctCurr.width() || fHeight != m_rctCurr.height() )
    {
        QRectF rct = m_rctCurr;

        rct.setWidth(fWidth);
        rct.setHeight(fHeight);

        setRect(rct); // updates m_rctCurr
    }
#endif

} // setSize

//------------------------------------------------------------------------------
void CGraphObjEllipse::setSize( const CPhysValSize& i_physValSize )
//------------------------------------------------------------------------------
{
    setSize(i_physValSize.width(), i_physValSize.height());
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjEllipse::setIsHit( bool i_bHit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Hit:" + bool2Str(i_bHit);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setIsHit",
        /* strAddInfo   */ strMthInArgs );

    if (m_bIsHit != i_bHit) {
        m_bIsHit = i_bHit;
        if (m_bIsHit) {
            showSelectionPoints(ESelectionPointsBoundingRectCorner|ESelectionPointsBoundingRectLineCenter);
        }
        else if (!isSelected()) {
            hideSelectionPoints();
        }
        update();
    }
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjEllipse::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjIsHit, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Point: " + point2Str(i_pt) +
            ", HitInfo {" + QString(o_pHitInfo == nullptr ? "null" : pointer2Str(o_pHitInfo)) + "}" +
            ", Rect(x,y,w,h) {" + rect2Str(rect()) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjIsHit,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "isHit",
        /* strAddInfo   */ strMthInArgs );

    bool bIsHit = false;

    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        if( pGraphicsItem->isSelected() )
        {
            bIsHit = isBoundingRectSelectionPointHit(
                /* pt               */ i_pt,
                /* iSelPtsCount     */ -1,
                /* pSelPts          */ nullptr,
                /* pGraphObjHitInfo */ o_pHitInfo );
        }

        if( !bIsHit )
        {
            QRectF rct = rect();
            bIsHit = isEllipseHit( rct, m_drawSettings.getFillStyle(), i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo );
        }

        if( !bIsHit )
        {
            if( pGraphicsItem->isSelected() || m_drawSettings.getFillStyle() == EFillStyle::SolidPattern )
            {
                bIsHit = pGraphicsItem->contains(i_pt);

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditMode::Move;
                    o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                }
            }
        }

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
        if( bIsHit && o_pHitInfo != nullptr )
        {
            o_pHitInfo->setCursor( Math::deg2Rad(m_fRotAngleCurr_deg) );
        }
#endif
    } // if( pGraphicsItem != nullptr )

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs;
        if (o_pHitInfo != nullptr) {
            strMthOutArgs = "HitInfo {" + o_pHitInfo->toString() + "}";
            mthTracer.setMethodOutArgs(strMthOutArgs);
        }
        mthTracer.setMethodReturn(bIsHit);
    }

    return bIsHit;

} // isHit

/*==============================================================================
public: // reimplementing methods of base class QGraphicItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjEllipse::setCursor( const QCursor& i_cursor )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = qCursorShape2Str(i_cursor.shape());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setCursor",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsEllipseItem::setCursor(i_cursor);

} // setCursor

/*==============================================================================
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjEllipse::showSelectionPoints( unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "SelectionPoints:" + selectionPoints2Str(i_selPts);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "showSelectionPoints",
        /* strAddInfo   */ strMthInArgs );

    if( parentItem() == nullptr )
    {
        showSelectionPointsOfBoundingRect( rect(), i_selPts );
    }
} // showSelectionPoints

//------------------------------------------------------------------------------
void CGraphObjEllipse::updateSelectionPoints( unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "SelectionPoints:" + selectionPoints2Str(i_selPts);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateSelectionPoints",
        /* strAddInfo   */ strMthInArgs );

    if( parentItem() == nullptr )
    {
        updateSelectionPointsOfBoundingRect( rect(), i_selPts );
    }
} // updateSelectionPoints

/*==============================================================================
public: // overridables of base class QGraphicsPolygonItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjEllipse::boundingRect() const
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

    CGraphObjSelectionPoint* pGraphObjSelPt;
    QRectF                   rctSelPt;
    QPolygonF                plgSelPt;
    ESelectionPoint          selPt;
    int                      idxSelPt;

    for( idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++ )
    {
        selPt = static_cast<ESelectionPoint>(idxSelPt);

        pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

        if( pGraphObjSelPt != nullptr )
        {
            rctSelPt = pGraphObjSelPt->boundingRect();
            plgSelPt = mapFromItem( pGraphObjSelPt, rctSelPt );
            rctBounding |= plgSelPt.boundingRect();
        }
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

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
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
void CGraphObjEllipse::paint(
    QPainter*                       i_pPainter,
    const QStyleOptionGraphicsItem* /*i_pStyleOption*/,
    QWidget*                        /*i_pWdgt*/ )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjPaint, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs  = "Selected:" + bool2Str(isSelected());
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strMthInArgs += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strMthInArgs += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "paint",
        /* strAddInfo   */ strMthInArgs );

    i_pPainter->save();

    QPen pn = pen();

    if( m_pDrawingScene->getMode() == EMode::Edit && isSelected() )
    {
        pn.setColor(Qt::blue);
    }

    i_pPainter->setPen(pn);
    i_pPainter->setBrush( brush() );

    i_pPainter->drawEllipse( rect() );

    if( m_pDrawingScene->getMode() == EMode::Edit && (m_bIsHit || isSelected()) )
    {
        QRectF rctBounding = rect();

        pn.setStyle(Qt::DotLine);
        pn.setColor(Qt::blue);
        pn.setWidth(1);

        i_pPainter->setPen(pn);
        i_pPainter->setBrush(Qt::NoBrush);

        i_pPainter->drawRect(rctBounding);

        if( isSelected() )
        {
            if( m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::TopCenter)] != nullptr && m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateTop)] != nullptr )
            {
                CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::TopCenter)];
                CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateTop)];

                QPointF ptRct = QPointF( pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y() );
                QPointF ptRot = QPointF( pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y() );

                QPointF ptRctM = mapFromScene(ptRct);
                QPointF ptRotM = mapFromScene(ptRot);

                i_pPainter->drawLine( ptRctM, ptRotM );
            }

            if( m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::BottomCenter)] != nullptr && m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateBottom)] != nullptr )
            {
                CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::BottomCenter)];
                CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateBottom)];

                QPointF ptRct = QPointF( pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y() );
                QPointF ptRot = QPointF( pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y() );

                QPointF ptRctM = mapFromScene(ptRct);
                QPointF ptRotM = mapFromScene(ptRot);

                i_pPainter->drawLine( ptRctM, ptRotM );
            }
        } // if( isSelected() )

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

//------------------------------------------------------------------------------
bool CGraphObjEllipse::sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    //--------------------------------------------------------------------
    // Mouse events of selection points will be "watched" to forward those
    // events to this item in order to resize and rotate this item.
    //--------------------------------------------------------------------

    CGraphObjSelectionPoint* pGraphObjSelPtWatched = dynamic_cast<CGraphObjSelectionPoint*>(i_pGraphicsItemWatched);

    if( pGraphObjSelPtWatched == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pGraphObjSelPtWatched == nullptr" );
    }

    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjSceneEventFilter, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs  = "ItemWatched:" + pGraphObjSelPtWatched->name();
        strMthInArgs += ", Event:" + qEventType2Str(i_pEv->type());
        strMthInArgs += ", Selected:" + bool2Str(isSelected());
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strMthInArgs += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strMthInArgs += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjSceneEventFilter,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "sceneEventFilter",
        /* strAddInfo   */ strMthInArgs );

    bool bEventHandled = false;

    switch( i_pEv->type() )
    {
        case QEvent::GraphicsSceneMouseMove:
        {
            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
            if( pEv != nullptr )
            {
                QPointF ptEvPos;
                for( int i = 0x1; i <= 0x10; i <<= 1 )
                {
                    Qt::MouseButton button = Qt::MouseButton(i);
                    ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
                    pEv->setButtonDownPos( button, ptEvPos );
                }
                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
                pEv->setPos(ptEvPos);
                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
                pEv->setLastPos(ptEvPos);
                mouseMoveEvent(pEv);
                bEventHandled = true;
            }
            break;
        }
        case QEvent::GraphicsSceneMousePress:
        {
            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
            if( pEv != nullptr )
            {
                QPointF ptEvPos;
                for( int i = 0x1; i <= 0x10; i <<= 1 )
                {
                    Qt::MouseButton button = Qt::MouseButton(i);
                    ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
                    pEv->setButtonDownPos( button, ptEvPos );
                }
                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
                pEv->setPos(ptEvPos);
                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
                pEv->setLastPos(ptEvPos);
                mousePressEvent(pEv);
                bEventHandled = true;
            }
            break;
        }
        case QEvent::GraphicsSceneMouseRelease:
        {
            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
            if( pEv != nullptr )
            {
                QPointF ptEvPos;
                for( int i = 0x1; i <= 0x10; i <<= 1 )
                {
                    Qt::MouseButton button = Qt::MouseButton(i);
                    ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
                    pEv->setButtonDownPos( button, ptEvPos );
                }
                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
                pEv->setPos(ptEvPos);
                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
                pEv->setLastPos(ptEvPos);
                mouseReleaseEvent(pEv);
                bEventHandled = true;
            }
            break;
        }
        case QEvent::GraphicsSceneMouseDoubleClick:
        {
            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
            if( pEv != nullptr )
            {
                QPointF ptEvPos;
                for( int i = 0x1; i <= 0x10; i <<= 1 )
                {
                    Qt::MouseButton button = Qt::MouseButton(i);
                    ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
                    pEv->setButtonDownPos( button, ptEvPos );
                }
                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
                pEv->setPos(ptEvPos);
                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
                pEv->setLastPos(ptEvPos);
                mouseDoubleClickEvent(pEv);
                bEventHandled = true;
            }
            break;
        }
        case QEvent::GraphicsSceneContextMenu:
        case QEvent::GraphicsSceneHoverEnter:
        case QEvent::GraphicsSceneHoverMove:
        case QEvent::GraphicsSceneHoverLeave:
        case QEvent::GraphicsSceneHelp:
        case QEvent::GraphicsSceneDragEnter:
        case QEvent::GraphicsSceneDragMove:
        case QEvent::GraphicsSceneDragLeave:
        case QEvent::GraphicsSceneDrop:
        case QEvent::GraphicsSceneWheel:
        default:
        {
            break;
        }

    } // switch( i_pEv->type() )

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strMthInArgs  = "Selected:" + bool2Str(isSelected());
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strMthInArgs += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strMthInArgs += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
        mthTracer.setMethodReturn(strMthInArgs);
    }

    return bEventHandled;

} // sceneEventFilter

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjEllipse::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strMthInArgs += ", Selected:" + bool2Str(isSelected());
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strMthInArgs += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strMthInArgs += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strMthInArgs );

    QPointF ptItemPos = mapFromScene(i_pEv->scenePos());

    CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )
    {
        SGraphObjHitInfo hitInfo;

        bool bIsHit = isHit(ptItemPos,&hitInfo);

        if( bIsHit )
        {
            if( cursor().shape() != hitInfo.m_cursor.shape() )
            {
                setCursor(hitInfo.m_cursor);
            }
        }

    } // if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )

} // hoverEnterEvent

//------------------------------------------------------------------------------
void CGraphObjEllipse::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strMthInArgs += ", Selected:" + bool2Str(isSelected());
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strMthInArgs += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strMthInArgs += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    QPointF ptItemPos = mapFromScene(i_pEv->scenePos());

    CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )
    {
        SGraphObjHitInfo hitInfo;

        bool bIsHit = isHit(ptItemPos,&hitInfo);

        if( bIsHit )
        {
            if( cursor().shape() != hitInfo.m_cursor.shape() )
            {
                setCursor(hitInfo.m_cursor);
            }
        }

    } // if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )

} // hoverMoveEvent

//------------------------------------------------------------------------------
void CGraphObjEllipse::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strMthInArgs += ", Selected:" + bool2Str(isSelected());
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strMthInArgs += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strMthInArgs += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
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
void CGraphObjEllipse::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjMouseEvents, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strMthInArgs += ", Selected:" + bool2Str(isSelected());
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strMthInArgs += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strMthInArgs += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strMthInArgs );

    CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit )
    {
        if( editToolDrawing == EEditTool::CreateObjects && m_editMode == EEditMode::Creating )
        {
            QGraphicsEllipseItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)

            m_selPtSelectedBoundingRect = ESelectionPoint::BottomRight;

            m_editResizeMode = selectionPoint2EditResizeMode(m_selPtSelectedBoundingRect.enumerator());

            if( m_arpSelPtsBoundingRect[static_cast<int>(m_selPtSelectedBoundingRect.enumerator())] != nullptr )
            {
                m_arpSelPtsBoundingRect[static_cast<int>(m_selPtSelectedBoundingRect.enumerator())]->setSelected(true);
            }

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
            m_rctOnMousePressEvent = m_rctCurr;
            m_ptRotOriginOnMousePressEvent = mapToScene(m_ptRotOriginCurr);
#endif

            updateEditInfo();
            updateToolTip();

        } // if( editToolDrawing == EEditTool::CreateObjects && m_editMode == EEditMode::Creating )

        else if( editToolDrawing == EEditTool::Select && m_editMode == EEditMode::None )
        {
            QGraphicsEllipseItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)

            CGraphObjSelectionPoint* pGraphObjSelPt;
            ESelectionPoint          selPt;
            int                      idxSelPt;

            SGraphObjHitInfo hitInfo;

            //bool bIsHit = isHit( ptMouseItemPos, &hitInfo );

            m_editMode                  = hitInfo.m_editMode;
            m_editResizeMode            = hitInfo.m_editResizeMode;
            m_selPtSelectedBoundingRect = hitInfo.m_selPtBoundingRect;

            for( idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++ )
            {
                selPt = static_cast<ESelectionPoint>(idxSelPt);

                pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

                if( pGraphObjSelPt != nullptr )
                {
                    if( m_selPtSelectedBoundingRect == selPt )
                    {
                        pGraphObjSelPt->setSelected(true);
                    }
                    else
                    {
                        pGraphObjSelPt->setSelected(false);
                    }
                }
            }

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
            m_rctOnMousePressEvent = m_rctCurr;
            m_ptRotOriginOnMousePressEvent = mapToScene(m_ptRotOriginCurr);
#endif

            m_pDrawingScene->setMode( EMode::Ignore, EEditTool::Ignore, m_editMode, m_editResizeMode, false );

            updateEditInfo();
            updateToolTip();

        } // if( editToolDrawing == EEditTool::Select && m_editMode == EEditMode::None )

    } // if( modeDrawing == EMode::Edit )

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strMthInArgs  = "Selected:" + bool2Str(isSelected());
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strMthInArgs += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strMthInArgs += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
        mthTracer.trace(strMthInArgs);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CGraphObjEllipse::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjMouseEvents, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strMthInArgs += ", Selected:" + bool2Str(isSelected());
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strMthInArgs += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strMthInArgs += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        if( m_editMode == EEditMode::Creating || m_editMode == EEditMode::Resize )
        {
            QPointF ptMouseItemPos = i_pEv->pos();

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
            m_rctCurr = resizeRect( m_rctOnMousePressEvent, m_selPtSelectedBoundingRect.enumerator(), ptMouseItemPos, nullptr );

            // Don't change the position of the object as the position of further
            // mouse events should be received relative to the object's position
            // on starting the edit process on pressing the mouse.
            //setPos(ptPosNew); // does not lead to "itemChange" call even if flag ItemSendsGeometryChanges is set.

            QGraphicsEllipseItem::setRect(m_rctCurr); // does not lead to "itemChange" call even if flag ItemSendsGeometryChanges is set.

            updateSelectionPointsOfBoundingRect(m_rctCurr);
#endif

            updateLabelPositionsAndContents();

            updateEditInfo();
            updateToolTip();

        } // if( m_editMode == EEditMode::Creating || m_editMode == EEditMode::Resize )

        else if( m_editMode == EEditMode::Move )
        {
            QGraphicsEllipseItem::mouseMoveEvent(i_pEv);

        } // if( m_editMode == EEditMode::Move )

        else if( m_editMode == EEditMode::Resize )
        {
            // Handled together with EEditMode::Creating above.

        } // if( m_editMode == EEditMode::Resize )

        else if( m_editMode == EEditMode::Rotate )
        {
            QPointF ptMouseScenePos = i_pEv->scenePos(); // see comment above

            double fRotAngle_rad = getAngleRad( m_ptRotOriginOnMousePressEvent, ptMouseScenePos );

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
            m_fRotAngleCurr_deg = Math::rad2Deg(fRotAngle_rad);

            switch( m_selPtSelectedBoundingRect.enumerator() )
            {
                case ESelectionPoint::RotateTop:
                {
                    m_fRotAngleCurr_deg -= 90.0;
                    break;
                }
                case ESelectionPoint::RotateBottom:
                {
                    m_fRotAngleCurr_deg -= 270.0;
                    break;
                }
                default:
                {
                    break;
                }
            }

            m_fRotAngleCurr_deg = Math::round2Resolution( m_fRotAngleCurr_deg, m_pDrawingScene->getRotationAngleResolutionInDegree() );

            while( m_fRotAngleCurr_deg >= 360.0 )
            {
                m_fRotAngleCurr_deg -= 360.0;
            }
            while( m_fRotAngleCurr_deg < 0.0 )
            {
                m_fRotAngleCurr_deg += 360.0;
            }
#endif
            updateTransform();
            updateEditInfo();
            updateToolTip();

        } // if( m_editMode == EEditMode::Rotate )

    } // if( modeDrawing == EMode::Edit )

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strMthInArgs  = "Selected:" + bool2Str(isSelected());
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strMthInArgs += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strMthInArgs += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
        mthTracer.setMethodReturn(strMthInArgs);
    }

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CGraphObjEllipse::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjMouseEvents, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strMthInArgs += ", Selected:" + bool2Str(isSelected());
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strMthInArgs += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strMthInArgs += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        if( m_editMode == EEditMode::Creating )
        {
            QPointF ptMouseItemPos = i_pEv->pos();

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
            m_rctCurr = resizeRect( m_rctOnMousePressEvent, m_selPtSelectedBoundingRect.enumerator(), ptMouseItemPos, nullptr );

            m_ptRotOriginCurr = m_rctCurr.center();

            QGraphicsEllipseItem::setRect(m_rctCurr); // does not lead to "itemChange" call even if flag ItemSendsGeometryChanges is set.

            updateSelectionPointsOfBoundingRect(m_rctCurr);
#endif
            updateLabelPositionsAndContents();

            // The object has been initially created.
            m_pDrawingScene->onGraphObjCreationFinished(this);

            updateEditInfo();
            updateToolTip();

        } // if( m_editMode == EEditMode::Creating )

        else if( m_editMode == EEditMode::Move )
        {
        } // if( m_editMode == EEditMode::Move )

        else if( m_editMode == EEditMode::Resize )
        {
            // The item will not be resized to the position of the mouse release event.
            // A selection point might have been clicked and released immediately (without
            // moving the mouse). In this case changing the size of the item according to
            // position of the mouse is not expected.

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
            QPolygonF plgSceneNew         = mapToScene(m_rctCurr);
            QRectF    rctBoundingSceneNew = plgSceneNew.boundingRect();
            QPointF   ptRotOriginSceneNew = rctBoundingSceneNew.center();
            double    fAngle_rad          = Math::deg2Rad(m_fRotAngleCurr_deg);
            //QPointF   ptLTScenePosOld   = mapToScene( QPointF(0.0,0.0) );
            QPointF   ptLTScenePosNew     = mapToScene( m_rctCurr.topLeft() );
            QPointF   ptItemScenePosNew   = rotatePoint( ptRotOriginSceneNew, ptLTScenePosNew, -fAngle_rad );

            if( m_rctCurr.left() != 0.0 )
            {
                m_rctCurr.moveLeft(0.0);
            }
            if( m_rctCurr.top() != 0.0 )
            {
                m_rctCurr.moveTop(0.0);
            }

            m_ptRotOriginCurr = m_rctCurr.center();

            QGraphicsEllipseItem::setRect(m_rctCurr); // does not lead to "itemChange" call even if flag ItemSendsGeometryChanges is set.

            setPos(ptItemScenePosNew);    // does not lead to "itemChange" call even if flag ItemSendsGeometryChanges is set.

            updateTransform();

            updateSelectionPointsOfBoundingRect(m_rctCurr);

            updateLabelPositionsAndContents();

            acceptCurrentAsOriginalCoors();
#endif

            updateEditInfo();
            updateToolTip();

        } // if( m_editMode == EEditMode::Resize )

        else if( m_editMode == EEditMode::Rotate )
        {
        } // if( m_editMode == EEditMode::Rotate )

        else if( m_editMode == EEditMode::MoveShapePoint )
        {
        } // if( m_editMode == EEditMode::MoveShapePoint )

        m_editMode = EEditMode::None;
        m_editResizeMode = EEditResizeMode::None;
        m_idxSelPtSelectedPolygon = -1;
        m_selPtSelectedBoundingRect = ESelectionPoint::None;

    } // if( modeDrawing == EMode::Edit )

    // The mouse release event would select the object.
    // This is not wanted if the selection tool is not active.
    bool bIsSelectable = flags() & QGraphicsItem::ItemIsSelectable;
    bool bIsSelectableReset = false;

    if( bIsSelectable && m_pDrawingScene->getEditTool() != EEditTool::Select )
    {
        setFlag(QGraphicsItem::ItemIsSelectable,false);
        bIsSelectableReset = true;
    }

    QGraphicsEllipseItem::mouseReleaseEvent(i_pEv);

    if( bIsSelectableReset )
    {
        setFlag(QGraphicsItem::ItemIsSelectable,bIsSelectable);
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strMthInArgs  = "Selected:" + bool2Str(isSelected());
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strMthInArgs += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strMthInArgs += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
        mthTracer.trace(strMthInArgs);
    }

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CGraphObjEllipse::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjMouseEvents, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strMthInArgs += ", Selected:" + bool2Str(isSelected());
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strMthInArgs += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strMthInArgs += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strMthInArgs );

    // When doubleclicking an item, the item will first receive a mouse
    // press event, followed by a release event (i.e., a click), then a
    // doubleclick event, and finally a release event.
    // The default implementation of "mouseDoubleClickEvent" calls "mousePressEvent".
    //QGraphicsEllipseItem::mouseDoubleClickEvent(i_pEv);

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        if( isSelected() )
        {
            onCreateAndExecDlgFormatGraphObjs();
        }
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strMthInArgs  = "Selected:" + bool2Str(isSelected());
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strMthInArgs += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strMthInArgs += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
        mthTracer.trace(strMthInArgs);
    }

} // mouseDoubleClickEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphObjEllipse::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
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

    bool bTreeEntryChanged = false;

    if( i_change == ItemSelectedHasChanged )
    {
        prepareGeometryChange();

        if( m_pDrawingScene->getMode() == EMode::Edit && isSelected() )
        {
            bringToFront(); // does not set "m_fZValue" as it is used to restore the stacking order on deselecting the object

            if( m_editMode == EEditMode::Creating )
            {
                showSelectionPoints(ESelectionPointsBoundingRectCorner|ESelectionPointsBoundingRectLineCenter|ESelectionPointsPolygonShapePoints);
            }
            else
            {
                showSelectionPoints();
            }

            // Not necessary as item has been brought to front and "showSelectionPoints"
            // sets zValue of selection points above item.
            //bringSelectionPointsToFront();

            setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
        }
        else
        {
            setAcceptedMouseButtons(Qt::NoButton);

            hideSelectionPoints();

            setZValue(m_fZValue); // restore ZValue as before selecting the object

            m_editMode = EEditMode::None;
            m_editResizeMode = EEditResizeMode::None;
            m_selPtSelectedBoundingRect = ESelectionPoint::None;
            m_idxSelPtSelectedPolygon = -1;

        } // if( !isSelected() )

        updateEditInfo();
        updateToolTip();

        bTreeEntryChanged = true;

    } // if( i_change == ItemSelectedHasChanged )

    else if( i_change == ItemToolTipChange || i_change == ItemToolTipHasChanged
          || i_change == ItemFlagsChange || i_change == ItemFlagsHaveChanged
          || i_change == ItemPositionChange
          || i_change == ItemVisibleChange
          || i_change == ItemEnabledChange
          || i_change == ItemSelectedChange
          || i_change == ItemParentChange
          || i_change == ItemTransformChange
          || i_change == ItemSceneChange
          || i_change == ItemCursorChange
          || i_change == ItemZValueChange
          || i_change == ItemOpacityChange
          #if QT_VERSION >= 0x040700
          || i_change == ItemOpacityChange
          || i_change == ItemRotationChange
          || i_change == ItemScaleChange
          || i_change == ItemTransformOriginPointChange )
          #else
          || i_change == ItemOpacityChange )
          #endif
    {
    }

    else if( i_change == ItemChildAddedChange
          || i_change == ItemChildRemovedChange
          || i_change == ItemVisibleHasChanged
          || i_change == ItemEnabledHasChanged
          || i_change == ItemCursorHasChanged
          || i_change == ItemOpacityHasChanged )
    {
    }

    else if( i_change == ItemTransformHasChanged )
    {
        updateSelectionPointsOfBoundingRect( rect() );
        updateLabelPositionsAndContents();
        updateEditInfo();
        updateToolTip();
    }

    else if( i_change == ItemTransformChange
          || i_change == ItemPositionHasChanged
          || i_change == ItemParentHasChanged
          || i_change == ItemSceneHasChanged
          #if QT_VERSION >= 0x040700
          || i_change == ItemScenePositionHasChanged
          || i_change == ItemRotationHasChanged
          || i_change == ItemScaleHasChanged
          || i_change == ItemTransformOriginPointHasChanged )
          #else
          || i_change == ItemScenePositionHasChanged )
          #endif
    {
        updateTransform();
        updateSelectionPointsOfBoundingRect( rect() );
        updateLabelPositionsAndContents();
        updateEditInfo();
        updateToolTip();
    }

    else if( i_change == ItemZValueHasChanged )
    {
        CGraphObjSelectionPoint* pGraphObjSelPt;
        ESelectionPoint          selPt;
        int                      idxSelPt;

        for( idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++ )
        {
            selPt = static_cast<ESelectionPoint>(idxSelPt);

            pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

            if( pGraphObjSelPt != nullptr )
            {
                pGraphObjSelPt->setZValue( zValue() + 0.05 );
            }
        }

        for( idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++ )
        {
            pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

            if( pGraphObjSelPt != nullptr )
            {
                pGraphObjSelPt->setZValue( zValue()+0.05 );
            }
        }

        //QHashIterator<QString, CGraphObjLabel*> itLabels(m_arpLabels);

        //CGraphObjLabel* pGraphObjLabel;

        //while( itLabels.hasNext() )
        //{
        //    itLabels.next();

        //    pGraphObjLabel = itLabels.value();

        //    if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
        //    {
        //        pGraphObjLabel->m_pGraphObjLabel->setZValue( zValue() + 0.02 );
        //    }
        //}
    }

    if( bTreeEntryChanged && m_pTree != nullptr )
    {
        m_pTree->onTreeEntryChanged(this);
    }

    valChanged = QGraphicsItem::itemChange(i_change, i_value);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthRet = qGraphicsItemChange2Str(i_change, valChanged, false);
        mthTracer.setMethodReturn(strMthRet);
    }

    return valChanged;

} // itemChange