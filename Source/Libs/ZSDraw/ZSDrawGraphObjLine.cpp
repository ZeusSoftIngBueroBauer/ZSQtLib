/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include <QtGui/qevent.h>
#include <QtGui/QPainter>

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsSceneEvent>
#include <QtGui/QStyleOption>
#else
#include <QtWidgets/QGraphicsSceneEvent>
#include <QtWidgets/QStyleOption>
#endif

#include "ZSDraw/ZSDrawGraphObjLine.h"
#include "ZSDraw/ZSDrawAux.h"
#include "ZSDraw/ZSDrawGraphObjGroup.h"
#include "ZSDraw/ZSDrawGraphObjLabel.h"
#include "ZSDraw/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;
using namespace ZS::Trace;


/*******************************************************************************
class CGraphObjLine : public CGraphObj, public QGraphicsLineItem
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjLine::s_iInstCount = 0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjLine::CGraphObjLine(
    CDrawingScene*       i_pDrawingScene,
    const CDrawSettings& i_drawSettings,
    const QString&       i_strObjName ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ EGraphObjTypeLine,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeLine),
        /* strObjName          */ i_strObjName.isEmpty() ? "Line" + QString::number(s_iInstCount) : i_strObjName,
        /* drawSettings        */ i_drawSettings ),
    QGraphicsLineItem(),
    m_bCoorsDirty(true),
    m_plgLineStart(),
    m_plgLineEnd()
{
    s_iInstCount++;

    m_arpSelPtsPolygon.append(nullptr); // P1, Start
    m_arpSelPtsPolygon.append(nullptr); // P2, End

    QString strAddTrcInfo;

    QString strNameSpace = NameSpace() + CObjFactory::c_strGroupSeparater + CObjFactory::c_strGroupNameStandardShapes;
    m_pTrcAdminObjCtorsAndDtor = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "CtorsAndDtor");
    m_pTrcAdminObjItemChange = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "ItemChange");
    m_pTrcAdminObjBoundingRect = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "BoundingRect");
    m_pTrcAdminObjPaint = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "Paint");
    m_pTrcAdminObjSceneEvent = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "SceneEvent");
    m_pTrcAdminObjSceneEventFilter = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "SceneEventFilter");
    m_pTrcAdminObjHoverEvents = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "HoverEvents");
    m_pTrcAdminObjMouseEvents = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "MouseEvents");
    m_pTrcAdminObjKeyEvents = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "KeyEvents");

    if( m_pTrcAdminObjCtorsAndDtor != nullptr && m_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strAddTrcInfo );

    setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges );

    onDrawSettingsChanged();

    updateToolTip();

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // ctor

//------------------------------------------------------------------------------
CGraphObjLine::~CGraphObjLine()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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
                    m_pDrawingScene->onGraphObjDestroying(m_strKeyInTree);
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
CGraphObj* CGraphObjLine::clone()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjCtorsAndDtor != nullptr && m_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "clone",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjLine* pGraphObj = new CGraphObjLine(m_pDrawingScene,m_drawSettings);

    pGraphObj->setName(m_strName);
    pGraphObj->setLine( line() );
    pGraphObj->setPos( pos() );
    pGraphObj->acceptCurrentAsOriginalCoors();

    return pGraphObj;

} // clone

/*==============================================================================
public: // replacing methods of QGraphicsLineItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::setLine( const QLineF& i_lin )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Line:" + line2Str(i_lin);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setLine",
        /* strAddInfo   */ strAddTrcInfo );

    QGraphicsLineItem::setLine(i_lin);

    m_rctCurr.setTopLeft( i_lin.p1() );
    m_rctCurr.setWidth( i_lin.dx() );
    m_rctCurr.setHeight( i_lin.dy() );

    m_ptRotOriginCurr = m_rctCurr.center();

    QPolygonF plg;

    plg.append(i_lin.p1());
    plg.append(i_lin.p2());

    if( isSelected() )
    {
        updateSelectionPointsOfPolygon(plg);
    }

    updateLabelPositionsAndContents();

    m_bCoorsDirty = true;

    updateLineEndPolygonCoors();

    updateEditInfo();
    updateToolTip();

    // As "setLine" does not end up in an "itemChange" call (even if the
    // flag "ItemSendsGeometryChanges" has been set) we call the "itemChange"
    // method on our own to update the position of the selection points.
    //itemChange( ItemPositionHasChanged, pos() );

} // setLine

//------------------------------------------------------------------------------
void CGraphObjLine::setLine( qreal i_x1, qreal i_y1, qreal i_x2, qreal i_y2 )
//------------------------------------------------------------------------------
{
    setLine( QLineF( QPointF(i_x1,i_y1), QPointF(i_x2,i_y2) ) );
}

//------------------------------------------------------------------------------
QLineF CGraphObjLine::getLine() const
//------------------------------------------------------------------------------
{
    return QGraphicsLineItem::line();
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjLine::getScenePolygonShapePointsString() const
//------------------------------------------------------------------------------
{
    QString   strScenePolygonShapePoints;
    QLineF    lineF = line();
    QPolygonF plgScene;

    plgScene.append( mapToScene(lineF.p1()) );
    plgScene.append( mapToScene(lineF.p2()) );

    strScenePolygonShapePoints = polygon2Str(plgScene);

    return strScenePolygonShapePoints;

} // getScenePolygonShapePointsString

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::onDrawSettingsChanged()
//------------------------------------------------------------------------------
{
    m_bCoorsDirty = true;

    update();

} // onDrawSettingsChanged

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::setWidth( double i_fWidth )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Width:" + QString::number(i_fWidth);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setWidth",
        /* strAddInfo   */ strAddTrcInfo );

    setSize( i_fWidth, m_rctCurr.height() );

} // setWidth

//------------------------------------------------------------------------------
void CGraphObjLine::setHeight( double i_fHeight )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Height:" + QString::number(i_fHeight);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setHeight",
        /* strAddInfo   */ strAddTrcInfo );

    setSize( m_rctCurr.width(), i_fHeight );

} // setHeight

//------------------------------------------------------------------------------
void CGraphObjLine::setSize( double i_fWidth, double i_fHeight )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Width:" + QString::number(i_fWidth);
        strAddTrcInfo += ", Height:" + QString::number(i_fHeight);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setSize",
        /* strAddInfo   */ strAddTrcInfo );

    double fWidth  = i_fWidth;
    double fHeight = i_fHeight;

    if( hasFixedWidth() && fWidth != getFixedWidth() )
    {
        fWidth = getFixedWidth();
    }
    else
    {
        if( hasMinimumWidth() && fWidth > getMinimumWidth() )
        {
            fWidth = getMinimumWidth();
        }
        if( hasMaximumWidth() && fWidth > getMaximumWidth() )
        {
            fWidth = getMaximumWidth();
        }
    }

    if( hasFixedHeight() && fHeight != getFixedHeight() )
    {
        fHeight = getFixedHeight();
    }
    else
    {
        if( hasMinimumHeight() && fHeight > getMinimumHeight() )
        {
            fHeight = getMinimumHeight();
        }
        if( hasMaximumHeight() && fHeight > getMaximumHeight() )
        {
            fHeight = getMaximumHeight();
        }
    }

    if( fWidth != m_rctCurr.width() || fHeight != m_rctCurr.height() )
    {
        QRectF rct = m_rctCurr;

        rct.setWidth(fWidth);
        rct.setHeight(fHeight);

        QLineF  lin = line();
        QPointF pt1 = lin.p1();
        QPointF pt2 = lin.p2();

        pt2.setX( pt1.x() + fWidth );
        pt2.setY( pt1.y() + fHeight );

        lin.setP2(pt2);

        setLine(lin); // updates m_rctCurr
    }

} // setSize

//------------------------------------------------------------------------------
void CGraphObjLine::setSize( const QSizeF& i_size )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Width:" + QString::number(i_size.width());
        strAddTrcInfo += ", Height:" + QString::number(i_size.height());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setSize",
        /* strAddInfo   */ strAddTrcInfo );

    setSize( i_size.width(), i_size.height() );

} // setSize

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::setIsHit( bool i_bHit )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Hit:" + bool2Str(i_bHit);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setIsHit",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_bIsHit != i_bHit )
    {
        m_bIsHit = i_bHit;

        if( i_bHit )
        {
            QPolygonF plg;
            QLineF    lineF = line();

            plg.append(lineF.p1());
            plg.append(lineF.p2());

            showSelectionPointsOfPolygon(plg);
        }
        else if( !isSelected() )
        {
            hideSelectionPoints();
        }

        update();
    }

} // setIsHit

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjLine::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjBoundingRect != nullptr && m_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Point:" + point2Str(i_pt);
        strAddTrcInfo += ", Line:" + line2Str(line());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "isHit",
        /* strAddInfo   */ strAddTrcInfo );

    bool bIsHit = false;

    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        if( pGraphicsItem->isSelected() )
        {
            bIsHit = isPolygonSelectionPointHit(i_pt,o_pHitInfo);
        }

        if( !bIsHit )
        {
            QLineF lin = line();

            bIsHit = isLineHit( lin, i_pt, m_pDrawingScene->getHitToleranceInPx() );

            if( o_pHitInfo != nullptr )
            {
                o_pHitInfo->m_editMode = EEditMode::Move;
                o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                o_pHitInfo->m_idxPolygonShapePoint = -1;
                o_pHitInfo->m_idxLineSegment = 0;
                o_pHitInfo->m_ptSelected = i_pt;
            }
        }

        if( bIsHit && o_pHitInfo != nullptr )
        {
            o_pHitInfo->setCursor( Math::deg2Rad(m_fRotAngleCurr_deg) );
        }

    } // if( pGraphicsItem != nullptr )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "IsHit:" + bool2Str(bIsHit);

        if( o_pHitInfo != nullptr )
        {
            strAddTrcInfo += ", EditMode:" + o_pHitInfo->m_editMode.toString();
            strAddTrcInfo += ", ResizeMode:" + o_pHitInfo->m_editResizeMode.toString();
            strAddTrcInfo += ", SelPtBoundingRect:" + o_pHitInfo->m_selPtBoundingRect.toString();
            strAddTrcInfo += ", PolygonShapePoint:" + QString::number(o_pHitInfo->m_idxPolygonShapePoint);
            strAddTrcInfo += ", LineSegment:" + QString::number(o_pHitInfo->m_idxLineSegment);
            strAddTrcInfo += ", PointSelected:" + point2Str(o_pHitInfo->m_ptSelected);
            strAddTrcInfo += ", Cursor:" + qCursorShape2Str(o_pHitInfo->m_cursor.shape());
        }
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return bIsHit;

} // isHit

/*==============================================================================
public: // reimplementing methods of base class QGraphicItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::setCursor( const QCursor& i_cursor )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = qCursorShape2Str(i_cursor.shape());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setCursor",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsLineItem::setCursor(i_cursor);

} // setCursor

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
QPointF CGraphObjLine::getSelectionPointCoors( ESelectionPoint i_selPt ) const
//------------------------------------------------------------------------------
{
    QLineF lin = line();

    QPointF ptSel = ZS::Draw::getSelectionPointCoors(lin,i_selPt);

    return ptSel;

} // getSelectionPointCoors

/*==============================================================================
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::showSelectionPoints( unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "SelectionPoints:" + selectionPoints2Str(i_selPts);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "showSelectionPoints",
        /* strAddInfo   */ strAddTrcInfo );

    if( parentItem() == nullptr )
    {
        QPolygonF plg;
        QLineF    lineF = line();

        plg.append(lineF.p1());
        plg.append(lineF.p2());

        if( i_selPts & ESelectionPointsPolygonShapePoints )
        {
            showSelectionPointsOfPolygon(plg);
        }
    }
} // showSelectionPoints

//------------------------------------------------------------------------------
void CGraphObjLine::updateSelectionPoints( unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "SelectionPoints:" + selectionPoints2Str(i_selPts);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateSelectionPoints",
        /* strAddInfo   */ strAddTrcInfo );

    if( parentItem() == nullptr )
    {
        QPolygonF plg;
        QLineF    lineF = line();

        plg.append(lineF.p1());
        plg.append(lineF.p2());

        updateSelectionPointsOfPolygon(plg);
    }
} // updateSelectionPoints

/*==============================================================================
public: // overridables of base class QGraphicsPolygonItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjLine::boundingRect() const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjBoundingRect != nullptr && m_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ strAddTrcInfo );

    QRectF rctBounding = QGraphicsLineItem::boundingRect();

    CGraphObjSelectionPoint* pGraphObjSelPt;
    QRectF                   rctSelPt;
    QPolygonF                plgSelPt;
    int                      idxSelPt;

    for( idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++ )
    {
        pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

        if( pGraphObjSelPt != nullptr )
        {
            rctSelPt = pGraphObjSelPt->boundingRect();

            plgSelPt = mapFromItem( pGraphObjSelPt, rctSelPt );

            rctBounding |= plgSelPt.boundingRect();
        }
    }

    if( m_plgLineStart.size() > 0 )
    {
        rctBounding |= m_plgLineStart.boundingRect();
    }

    if( m_plgLineEnd.size() > 0 )
    {
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

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Rect(x,y,w,h):(";
        strAddTrcInfo += QString::number(rctBounding.x(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.y(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.width(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.height(),'f',1) + ")";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return rctBounding;

} // boundingRect

//------------------------------------------------------------------------------
QPainterPath CGraphObjLine::shape() const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjBoundingRect != nullptr && m_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Line:" + line2Str(line());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "shape",
        /* strAddInfo   */ strAddTrcInfo );

    QPainterPath painterPath = QGraphicsLineItem::shape();

    if( m_plgLineStart.size() > 0 )
    {
        painterPath.addPolygon(m_plgLineStart);
    }
    if( m_plgLineEnd.size() > 0 )
    {
        painterPath.addPolygon(m_plgLineEnd);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        const QGraphicsItem* pCThis = static_cast<const QGraphicsItem*>(this);
        QGraphicsItem* pVThis = const_cast<QGraphicsItem*>(pCThis);
        strAddTrcInfo  = "Path:" + qPainterPath2Str(pVThis,painterPath);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return painterPath;

} // shape

//------------------------------------------------------------------------------
void CGraphObjLine::paint(
    QPainter*                       i_pPainter,
    const QStyleOptionGraphicsItem* /*i_pStyleOption*/,
    QWidget*                        /*i_pWdgt*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjPaint != nullptr && m_pTrcAdminObjPaint->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "paint",
        /* strAddInfo   */ strAddTrcInfo );

    i_pPainter->save();

    if( m_bCoorsDirty )
    {
        updateLineEndPolygonCoors();
    }

    QPen pn = pen();

    pn.setColor( m_drawSettings.getPenColor() );
    pn.setWidth( m_drawSettings.getPenWidth() );
    pn.setStyle( lineStyle2QtPenStyle(m_drawSettings.getLineStyle()) );

    if( m_pDrawingScene->getMode() == EMode::Edit && isSelected() )
    {
        //if( m_editMode == EEditMode::Move )
        {
            pn.setStyle(Qt::DotLine);
            pn.setColor(Qt::blue);
        }
    }

    i_pPainter->setPen(pn);

    QLineF lin = line();

    i_pPainter->drawLine(lin);

    ELineEndStyle lineEndStyleLineStart = m_drawSettings.getLineEndStyle(ELinePoint::Start);
    ELineEndStyle lineEndStyleLineEnd   = m_drawSettings.getLineEndStyle(ELinePoint::End);

    if( lineEndStyleLineStart != ELineEndStyle::Normal || lineEndStyleLineEnd != ELineEndStyle::Normal )
    {
        ELineEndBaseLineType baseLineTypeLineStart = m_drawSettings.getLineEndBaseLineType(ELinePoint::Start);
        ELineEndBaseLineType baseLineTypeLineEnd   = m_drawSettings.getLineEndBaseLineType(ELinePoint::End);
        QBrush               brsh;

        pn.setWidth(1);
        pn.setStyle(Qt::SolidLine);
        i_pPainter->setPen(pn);

        brsh.setColor(pn.color());

        if( lineEndStyleLineStart != ELineEndStyle::Normal )
        {
            brsh.setStyle( lineEndFillStyle2QtBrushStyle(m_drawSettings.getLineEndFillStyle(ELinePoint::Start)) );

            i_pPainter->setBrush(brsh);

            if( baseLineTypeLineStart == ELineEndBaseLineType::NoLine )
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
            brsh.setStyle( lineEndFillStyle2QtBrushStyle(m_drawSettings.getLineEndFillStyle(ELinePoint::End)) );

            i_pPainter->setBrush(brsh);

            if( baseLineTypeLineEnd == ELineEndBaseLineType::NoLine )
            {
                i_pPainter->drawPolyline(m_plgLineEnd);
            }
            else
            {
                i_pPainter->drawPolygon(m_plgLineEnd);
            }
        }
    } // if( lineEndStyleLineStart != ELineEndStyle::Normal || lineEndStyleLineEnd != ELineEndStyle::Normal )

    if( m_pDrawingScene->getMode() == EMode::Edit && (m_bIsHit || isSelected()) )
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

//------------------------------------------------------------------------------
bool CGraphObjLine::sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv )
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

    QString strAddTrcInfo;

    if( m_pTrcAdminObjSceneEventFilter != nullptr && m_pTrcAdminObjSceneEventFilter->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "ItemWatched:" + pGraphObjSelPtWatched->name();
        strAddTrcInfo += ", Event:" + qEventType2Str(i_pEv->type());
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjSceneEventFilter,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "sceneEventFilter",
        /* strAddInfo   */ strAddTrcInfo );

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

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

    return bEventHandled;

} // sceneEventFilter

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjHoverEvents != nullptr && m_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QPointF ptItemPos = i_pEv->pos();

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
void CGraphObjLine::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjHoverEvents != nullptr && m_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QPointF ptItemPos = i_pEv->pos();

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
void CGraphObjLine::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjHoverEvents != nullptr && m_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverLeaveEvent",
        /* strAddInfo   */ strAddTrcInfo );

    unsetCursor();

} // hoverLeaveEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjMouseEvents != nullptr && m_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strAddTrcInfo );

    CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit )
    {
        if( editToolDrawing == EEditTool::CreateObjects && m_editMode == EEditMode::Creating )
        {
            QGraphicsLineItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)

            m_idxSelPtSelectedPolygon = 1;

            if( m_arpSelPtsPolygon[m_idxSelPtSelectedPolygon] != nullptr )
            {
                m_arpSelPtsPolygon[m_idxSelPtSelectedPolygon]->setSelected(true);
            }

            updateEditInfo();
            updateToolTip();

        } // if( editToolDrawing == EEditTool::CreateObjects && m_editMode == EEditMode::Creating )

        else if( editToolDrawing == EEditTool::Select && m_editMode == EEditMode::None )
        {
            QGraphicsLineItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)

            CGraphObjSelectionPoint* pGraphObjSelPt;
            int                      idxSelPt;

            SGraphObjHitInfo hitInfo;

            //bool bIsHit = isHit( ptMouseItemPos, &hitInfo );

            m_editMode                = hitInfo.m_editMode;
            m_editResizeMode          = hitInfo.m_editResizeMode;
            m_idxSelPtSelectedPolygon = hitInfo.m_idxPolygonShapePoint;

            for( idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++ )
            {
                pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

                if( pGraphObjSelPt != nullptr )
                {
                    if( idxSelPt == m_idxSelPtSelectedPolygon )
                    {
                        pGraphObjSelPt->setSelected(true);
                    }
                    else
                    {
                        pGraphObjSelPt->setSelected(false);
                    }
                }
            }

            m_pDrawingScene->setMode( EMode::Ignore, EEditTool::Ignore, m_editMode, m_editResizeMode, false );

            updateEditInfo();
            updateToolTip();

        } // if( editToolDrawing == EEditTool::Select && m_editMode == EEditMode::None )

    } // if( modeDrawing == EMode::Edit )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CGraphObjLine::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjMouseEvents != nullptr && m_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strAddTrcInfo );

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        if( m_editMode == EEditMode::Creating || m_editMode == EEditMode::MoveShapePoint )
        {
            QRectF  sceneRect = m_pDrawingScene->sceneRect();
            QPointF posEv = i_pEv->pos();

            if( sceneRect.contains(mapToScene(posEv)) )
            {
                QLineF lin = line();

                if( m_idxSelPtSelectedPolygon == 1 )
                {
                    lin.setP2(posEv);
                }
                else // if( m_idxSelPtSelectedPolygon == 0 )
                {
                    lin.setP1(posEv);
                }

                m_rctCurr.setWidth( lin.dx() );
                m_rctCurr.setHeight( lin.dy() );

                QGraphicsLineItem::setLine(lin);

                QPolygonF plg;

                plg.append(lin.p1());
                plg.append(lin.p2());

                updateSelectionPointsOfPolygon(plg);

                updateLabelPositionsAndContents();

                m_bCoorsDirty = true;

                updateLineEndPolygonCoors();

                updateEditInfo();
                updateToolTip();
            }
        } // if( m_editMode == EEditMode::Creating || m_editMode == EEditMode::MoveShapePoint )

        else if( m_editMode == EEditMode::Move )
        {
            QGraphicsLineItem::mouseMoveEvent(i_pEv);

            updateEditInfo();
            updateToolTip();

        } // if( m_editMode == EEditMode::Move )

        else if( m_editMode == EEditMode::Resize )
        {
        } // if( m_editMode == EEditMode::Resize )

        else if( m_editMode == EEditMode::Rotate )
        {
        } // if( m_editMode == EEditMode::Rotate )

        else if( m_editMode == EEditMode::EditText )
        {
        } // if( m_editMode == EEditMode::EditText )

    } // if( modeDrawing == EMode::Edit )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CGraphObjLine::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjMouseEvents != nullptr && m_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strAddTrcInfo );

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        if( m_editMode == EEditMode::Creating )
        {
            // The object has been initially created.
            m_pDrawingScene->onGraphObjCreationFinished(this);

        } // if( m_editMode == EEditMode::Creating )

        else if( m_editMode == EEditMode::Move )
        {
            updateEditInfo();
            updateToolTip();

        } // if( m_editMode == EEditMode::Move )

        else if( m_editMode == EEditMode::Resize )
        {
        } // if( m_editMode == EEditMode::Resize )

        else if( m_editMode == EEditMode::Rotate )
        {
        } // if( m_editMode == EEditMode::Rotate )

        else if( m_editMode == EEditMode::MoveShapePoint )
        {
        } // if( m_editMode == EEditMode::MoveShapePoint )

        else if( m_editMode == EEditMode::EditText )
        {
        } // if( m_editMode == EEditMode::EditText )

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

    QGraphicsLineItem::mouseReleaseEvent(i_pEv);

    if( bIsSelectableReset )
    {
        setFlag(QGraphicsItem::ItemIsSelectable,bIsSelectable);
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CGraphObjLine::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjMouseEvents != nullptr && m_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strAddTrcInfo );

    // When doubleclicking an item, the item will first receive a mouse
    // press event, followed by a release event (i.e., a click), then a
    // doubleclick event, and finally a release event.
    // The default implementation of "mouseDoubleClickEvent" calls "mousePressEvent".
    //QGraphicsLineItem::mouseDoubleClickEvent(i_pEv);

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        if( isSelected() )
        {
            onCreateAndExecDlgFormatGraphObjs();
        }

    } // if( modeDrawing == EMode::Edit )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseDoubleClickEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphObjLine::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
//------------------------------------------------------------------------------
{
    if( m_bDtorInProgress )
    {
        return i_value;
    }

    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Changed:" + qGraphicsItemChange2Str(i_change);

        if( i_value.type() == QVariant::PointF )
        {
            strAddTrcInfo += ", Value(" + qVariantType2Str(i_value.type()) + "):" + point2Str(i_value.toPointF());
        }
        else
        {
            strAddTrcInfo += ", Value(" + qVariantType2Str(i_value.type()) + "):" + i_value.toString();
        }
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "itemChange",
        /* strAddInfo   */ strAddTrcInfo );

    QVariant valChanged = i_value;

    bool bTreeEntryChanged = false;

    if( i_change == ItemSelectedHasChanged )
    {
        prepareGeometryChange();

        if( m_pDrawingScene->getMode() == EMode::Edit && isSelected() )
        {
            bringToFront(); // does not set "m_fZValue" as it is used to restore the stacking order on deselecting the object

            showSelectionPoints();

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

    else if( i_change == ItemTransformChange
          || i_change == ItemPositionHasChanged
          || i_change == ItemTransformHasChanged
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
        QPolygonF plg;
        QLineF    lineF = line();

        plg.append(lineF.p1());
        plg.append(lineF.p2());

        updateSelectionPointsOfPolygon(plg);

        updateLabelPositionsAndContents();

        m_bCoorsDirty = true;

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

    valChanged = QGraphicsItem::itemChange(i_change,i_value);

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_value.type() == QVariant::PointF )
        {
            strAddTrcInfo = "ValChanged(" + qVariantType2Str(valChanged.type()) + "):" + point2Str(valChanged.toPointF());
        }
        else
        {
            strAddTrcInfo = "ValChanged(" + qVariantType2Str(valChanged.type()) + "):" + valChanged.toString();
        }
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return valChanged;

} // itemChange

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::updateLineEndPolygonCoors()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", LineStartPos:(" + QString::number(line().p1().x()) + "," + QString::number(line().p1().y()) + ")";
        strAddTrcInfo += ", LineEndPos:(" + QString::number(line().p2().x()) + "," + QString::number(line().p2().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateLineEndPolygonCoors",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_bCoorsDirty )
    {
        ELineEndStyle lineEndStyleLineStart = m_drawSettings.getLineEndStyle(ELinePoint::Start);
        ELineEndStyle lineEndStyleLineEnd   = m_drawSettings.getLineEndStyle(ELinePoint::End);

        QLineF lin = line();

        if( lineEndStyleLineStart != ELineEndStyle::Normal || lineEndStyleLineEnd != ELineEndStyle::Normal )
        {
            //ELineEndBaseLineType baseLineTypeLineStart = m_drawSettings.getLineEndBaseLineType(ELinePoint::Start);
            //ELineEndBaseLineType baseLineTypeLineEnd   = m_drawSettings.getLineEndBaseLineType(ELinePoint::End);

            getLineEndPolygons(
                /* line          */ lin,
                /* drawSetings   */ m_drawSettings,
                /* pplgLineStart */ lineEndStyleLineStart != ELineEndStyle::Normal ? &m_plgLineStart : nullptr,
                /* pplgLineEnd   */ lineEndStyleLineEnd   != ELineEndStyle::Normal ? &m_plgLineEnd : nullptr );

        } // if( lineEndStyleLineStart != ELineEndStyle::Normal || lineEndStyleLineEnd != ELineEndStyle::Normal )

    } // if( m_bCoorsDirty )

    m_bCoorsDirty = false;

} // updateLineEndPolygonCoors

/*==============================================================================
protected: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::updateToolTip()
//------------------------------------------------------------------------------
{
    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        QString strNodeSeparator = m_pDrawingScene->getGraphObjNameNodeSeparator();
        QLineF  lin = line();
        QPointF ptPos;

        m_strToolTip  = "ObjPath:\t" + path();

        m_strToolTip += "\nP1:\t\t" + point2Str(lin.p1());
        m_strToolTip += "\nP2:\t\t" + point2Str(lin.p2());

        // "scenePos" returns mapToScene(0,0). This is NOT equivalent to the
        // position of the item's top left corner before applying the rotation
        // transformation matrix but includes the transformation. What we want
        // (or what I want) is the position of the item before rotating the item
        // around the rotation origin point. In contrary it looks like "pos"
        // always returns the top left corner before rotating the object.

        if( pGraphicsItem->parentItem() != nullptr )
        {
            ptPos = pGraphicsItem->pos();
            m_strToolTip += "\nPos:\t\t" + point2Str(ptPos);
        }
        else
        {
            ptPos = pGraphicsItem->pos(); // don't use "scenePos" here (see comment above)
            m_strToolTip += "\nPos:\t\t" + point2Str(ptPos);
        }

        m_strToolTip += "\nSize:\t\t" + size2Str(getSize());
        m_strToolTip += "\nZValue:\t\t" + QString::number(pGraphicsItem->zValue());

        pGraphicsItem->setToolTip(m_strToolTip);

    } // if( pGraphicsItem != nullptr )

} // updateToolTip
