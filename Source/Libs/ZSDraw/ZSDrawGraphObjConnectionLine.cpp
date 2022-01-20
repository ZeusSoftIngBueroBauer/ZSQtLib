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

#include "ZSDraw/ZSDrawGraphObjConnectionLine.h"
#include "ZSDraw/ZSDrawAux.h"
#include "ZSDraw/ZSDrawGraphObjConnectionPoint.h"
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
    CDrawingScene*       i_pDrawingScene,
    const CDrawSettings& i_drawSettings,
    const QString&       i_strObjName ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameConnections,
        /* type                */ EGraphObjTypeConnectionLine,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeConnectionLine),
        /* strObjName          */ i_strObjName.isEmpty() ? "ConnectionLine" + QString::number(s_iInstCount) : i_strObjName,
        /* drawSettings        */ i_drawSettings ),
    QGraphicsPolygonItem(),
    m_bCoorsDirty(true),
    m_plgCurr(),
    m_plgOrig(),
    m_plgLineStart(),
    m_plgLineEnd(),
    m_arpCnctPts(CEnumLinePoint::count())
{
    s_iInstCount++;

    QString strAddTrcInfo;

    QString strNameSpace = NameSpace() + CObjFactory::c_strGroupSeparater + CObjFactory::c_strGroupNameConnections;
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
        mthTracer.trace(strAddTrcInfo);
    }

} // ctor

//------------------------------------------------------------------------------
CGraphObjConnectionLine::~CGraphObjConnectionLine()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    CGraphObjConnectionPoint* pGraphObjCnctPt;
    int                       idxLinePt;

    for( idxLinePt = 0; idxLinePt < m_arpCnctPts.size(); idxLinePt++ )
    {
        pGraphObjCnctPt = m_arpCnctPts[idxLinePt];

        if( pGraphObjCnctPt != nullptr )
        {
            pGraphObjCnctPt->removeConnectionLine(this);
        }
        m_arpCnctPts[idxLinePt] = nullptr;
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
CGraphObj* CGraphObjConnectionLine::clone()
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

    CGraphObjConnectionLine* pGraphObj = new CGraphObjConnectionLine(m_pDrawingScene,m_drawSettings);

    pGraphObj->setName(m_strName);

    QPointF ptPos = pos();

    pGraphObj->setPolygon( polygon() );
    pGraphObj->setPos(ptPos);
    pGraphObj->setRotationAngleInDegree(m_fRotAngleCurr_deg);
    //pGraphObj->setScaleFactors(m_fScaleFacXCurr,m_fScaleFacYCurr);
    pGraphObj->acceptCurrentAsOriginalCoors();

    return pGraphObj;

} // clone

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjConnectionLine::setConnectionPoint( ELinePoint i_linePoint, CGraphObjConnectionPoint* i_pCnctPt )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = CEnumLinePoint(i_linePoint).toString();
        strAddTrcInfo += ", CnctPoint: " + QString(i_pCnctPt == nullptr ? "nullptr" : i_pCnctPt->name());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setConnectionPoint",
        /* strAddInfo   */ strAddTrcInfo );

    bool bConnected = false;

    if( static_cast<int>(i_linePoint) >= 0 && static_cast<int>(i_linePoint) < m_arpCnctPts.size() && i_pCnctPt != nullptr )
    {
        CGraphObjConnectionPoint* pGraphObjCnctPt = m_arpCnctPts[static_cast<int>(i_linePoint)];

        if( pGraphObjCnctPt == i_pCnctPt )
        {
            bConnected = true;
        }
        else // if( pGraphObjCnctPt != i_pCnctPt )
        {
            if( pGraphObjCnctPt != nullptr )
            {
                pGraphObjCnctPt->removeConnectionLine(this);
                pGraphObjCnctPt = nullptr;
                m_arpCnctPts[static_cast<int>(i_linePoint)] = nullptr;
            }

            bConnected = i_pCnctPt->appendConnectionLine(this);

            if( bConnected )
            {
                m_arpCnctPts[static_cast<int>(i_linePoint)] = i_pCnctPt;
                pGraphObjCnctPt = m_arpCnctPts[static_cast<int>(i_linePoint)];
            }
            else
            {
                pGraphObjCnctPt = nullptr;
            }

        } // if( pGraphObjCnctPt != i_pCnctPt )

        if( pGraphObjCnctPt != nullptr )
        {
            QPolygonF  plg = polygon();
            ELinePoint linePoint = getConnectionLinePoint(pGraphObjCnctPt);
            QPointF    ptCnctPtPos = pGraphObjCnctPt->rect().center();
            QPointF    ptCnctPtScenePos = pGraphObjCnctPt->mapToScene(ptCnctPtPos);

            if( linePoint == ELinePoint::Start )
            {
                QPointF pt(0.0,0.0);

                if( plg.size() == 0 )
                {
                    plg.append(pt);
                }
                else
                {
                    plg[0] = pt;
                }

                QGraphicsPolygonItem::setPolygon(plg);

                setPos(ptCnctPtScenePos);

            } // if( linePoint == ELinePoint::Start )

            else if( linePoint == ELinePoint::End && plg.size() >= 1 )
            {
                QPointF pt = mapFromScene(ptCnctPtScenePos);

                if( plg.size() == 1 )
                {
                    plg.append(pt);
                }
                else
                {
                    int idxPt = plg.size()-1;
                    plg[idxPt] = pt;
                }

                QGraphicsPolygonItem::setPolygon(plg);

            } // if( linePoint == ELinePoint::End )

            updateSelectionPointsOfPolygon(plg);
            updateLabelPositions();
            updateToolTip();
            updateEditInfo();

        } // if( pGraphObjCnctPt != nullptr )

    } // if( static_cast<int>(i_linePoint) >= 0 && static_cast<int>(i_linePoint) < m_arpCnctPts.size() && i_pCnctPt != nullptr )

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
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = polygon2Str(i_plg);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setPolygon",
        /* strAddInfo   */ strAddTrcInfo );

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
                updateSelectionPointsOfPolygon(plg);
            }
            else
            {
                showSelectionPointsOfPolygon(plg);
            }
        }

        updateLabelPositions();

        m_bCoorsDirty = true;

        updateLineEndPolygonCoors();

        updateEditInfo();
        updateToolTip();

    } // if( i_plg.size() >= 2 )

} // setPolygon

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjConnectionLine::getScenePolygonShapePointsString() const
//------------------------------------------------------------------------------
{
    QString   strScenePolygonShapePoints;
    QPolygonF plg = polygon();
    QPolygonF plgScene = mapToScene(plg);

    strScenePolygonShapePoints = polygon2Str(plgScene);

    return strScenePolygonShapePoints;

} // getScenePolygonShapePointsString

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::onDrawSettingsChanged()
//------------------------------------------------------------------------------
{
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

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::acceptCurrentAsOriginalCoors()
//------------------------------------------------------------------------------
{
    m_plgOrig = m_plgCurr;

    CGraphObj::acceptCurrentAsOriginalCoors();

} // acceptCurrentAsOriginalCoors

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::setWidth( double i_fWidth )
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

    // This method call does not really make sense for connection lines.
    // But at least we update the current size member variable.
    setSize( i_fWidth, m_rctCurr.height() );

} // setWidth

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::setHeight( double i_fHeight )
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

    // This method call does not really make sense for connection lines.
    // But at least we update the current size member variable.
    setSize( m_rctCurr.width(), i_fHeight );

} // setHeight

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::setSize( double i_fWidth, double i_fHeight )
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

    // This method call does not really make sense for connection lines.
    // But at least we update the current size member variable.

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
        m_rctCurr.setWidth(fWidth);
        m_rctCurr.setHeight(fHeight);

        m_ptRotOriginCurr = m_rctCurr.center();

        // As this method call doesn't make any sense for connection lines
        // the polygon shape points have not changed and there is no need
        // to update the selection points.
        //updateSelectionPointsOfPolygon(polygon());
    }

} // setSize

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::setSize( const QSizeF& i_size )
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

    // This method call does not really make sense for connection lines.
    // But at least we update the current size member variable.
    setSize( i_size.width(), i_size.height() );

} // setSize

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::setIsHit( bool i_bHit )
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

        if( m_bIsHit )
        {
            showSelectionPointsOfPolygon( polygon() );
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
bool CGraphObjConnectionLine::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjBoundingRect != nullptr && m_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Point:" + point2Str(i_pt);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
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
            QPolygonF plg = polygon();
            bIsHit = isPolygonHit( plg, EFillStyle::NoFill, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo );
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
void CGraphObjConnectionLine::setCursor( const QCursor& i_cursor )
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

    QGraphicsPolygonItem::setCursor(i_cursor);

} // setCursor

/*==============================================================================
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::showSelectionPoints( unsigned char i_selPts )
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
        if( i_selPts & ESelectionPointsPolygonShapePoints )
        {
            showSelectionPointsOfPolygon( polygon() );
        }
    }
} // showSelectionPoints

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::updateSelectionPoints( unsigned char i_selPts )
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
        updateSelectionPointsOfPolygon( polygon() );
    }
} // updateSelectionPoints

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::onParentItemCoorsHasChanged( CGraphObj* i_pGraphObjParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Parent:" + i_pGraphObjParent->name();
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "onParentItemCoorsHasChanged",
        /* strAddInfo   */ strAddTrcInfo );

    // Connection lines don't belong to groups. But their connection points do.
    // If a group is moved also the connection points are moved by Qt's graphics scene.
    // But not the connection lines which are linked to the connection points. The
    // connection points can be considered as parents of connection lines and
    // "onParentItemCoorsHasChanged" is called by the connection points if their
    // position changes.

    CGraphObjConnectionPoint* pGraphObjCnctPt = dynamic_cast<CGraphObjConnectionPoint*>(i_pGraphObjParent);

    if( pGraphObjCnctPt != nullptr )
    {
        QPolygonF  plg = polygon();
        ELinePoint linePoint = getConnectionLinePoint(pGraphObjCnctPt);
        QPointF    ptCnctPtPos = pGraphObjCnctPt->rect().center();
        QPointF    ptCnctPtScenePos = pGraphObjCnctPt->mapToScene(ptCnctPtPos);
        QPointF    ptItemPosNew = mapFromScene(ptCnctPtScenePos);

        CGraphObjSelectionPoint* pGraphObjSelPt;
        QPointF                  ptSel;
        int                      idxSelPt;

        if( linePoint == ELinePoint::Start && plg.size() > 0 )
        {
            // The origin of the poly line's coordinate system (the item's position)
            // is the first line point (in poly line's item's coordinate system the
            // first line point is at 0/0). If the first line point is moved it's
            // coordinates remain at 0/0 but all other points must be moved to fit into
            // the new item's coordinate system.

            QPointF ptItemPosOld = plg[0];
            double  dx, dy;
            int     idxPt;

            setPos(ptCnctPtScenePos);

            dx = ptItemPosNew.x() - ptItemPosOld.x();
            dy = ptItemPosNew.y() - ptItemPosOld.y();

            QPointF ptPosOld, ptPosNew;

            for( idxPt = 1; idxPt < plg.size(); idxPt++ )
            {
                ptPosOld = plg[idxPt];
                ptPosNew.setX( ptPosOld.x() - dx );
                ptPosNew.setY( ptPosOld.y() - dy );
                plg[idxPt] = ptPosNew;
            }

            QGraphicsPolygonItem::setPolygon(plg);

            for( idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++ )
            {
                pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

                if( pGraphObjSelPt != nullptr )
                {
                    ptSel = plg[idxSelPt];
                    ptSel = mapToScene(ptSel);
                    pGraphObjSelPt->setPos(ptSel);
                }
            }

        } // if( linePoint == ELinePoint::Start )

        else if( linePoint == ELinePoint::End && plg.size() > 1 )
        {
            idxSelPt = plg.size()-1;

            plg[idxSelPt] = ptItemPosNew;

            QGraphicsPolygonItem::setPolygon(plg);

            if( idxSelPt >= 0 && idxSelPt < m_arpSelPtsPolygon.size() )
            {
                pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

                if( pGraphObjSelPt != nullptr )
                {
                    ptSel = plg[idxSelPt];
                    ptSel = mapToScene(ptSel);
                    pGraphObjSelPt->setPos(ptSel);
                }
            }

        } // if( linePoint == ELinePoint::End )

        normalize();

        m_bCoorsDirty = true;

        updateLineEndPolygonCoors();

        updateEditInfo();
        updateToolTip();

    } // if( pGraphObjCnctPt != nullptr )

} // onParentItemCoorsHasChanged

/*==============================================================================
public: // overridables of base class QGraphicsPolygonItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjConnectionLine::boundingRect() const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjBoundingRect != nullptr && m_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ strAddTrcInfo );

    QRectF rctBounding = QGraphicsPolygonItem::boundingRect();

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
    //    QHashIterator<QString,SGraphObjLabel*> itLabels(m_arpLabels);
    //    SGraphObjLabel* pGraphObjLabel;
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
QPainterPath CGraphObjConnectionLine::shape() const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjBoundingRect != nullptr && m_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "shape",
        /* strAddInfo   */ strAddTrcInfo );

    QPainterPath painterPath = QGraphicsPolygonItem::shape();

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
void CGraphObjConnectionLine::paint(
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
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
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
        if( m_editMode != EEditMode::Creating )
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

        //QHashIterator<QString,SGraphObjLabel*> itLabels(m_arpLabels);
        //SGraphObjLabel*                        pGraphObjLabel;

        //QPointF ptSelPt;
        //QPointF ptLabelSelPt;

        //while( itLabels.hasNext() )
        //{
        //    itLabels.next();
        //    pGraphObjLabel = itLabels.value();

        //    if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
        //    {
        //        ptSelPt = getSelectionPoint(pGraphObjLabel->m_selPt.enumerator());

        //        ptLabelSelPt = pGraphObjLabel->m_pGraphObjLabel->getSelectionPoint(ESelectionPoint::Center);
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
bool CGraphObjConnectionLine::sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv )
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
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
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
        default:
        {
            break;
        }

    } // switch( i_pEv->type() )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return bEventHandled;

} // sceneEventFilter

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
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
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
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
void CGraphObjConnectionLine::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
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
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
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
void CGraphObjConnectionLine::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
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
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
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
void CGraphObjConnectionLine::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
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
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strAddTrcInfo );

    //Qt::KeyboardModifiers iEvMouseKeyboardModifiers = i_pEv->modifiers() & Qt::KeyboardModifierMask;

    QPointF ptMouseItemPos = i_pEv->pos();

    CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( m_editMode == EEditMode::Creating )
    {
        QGraphicsPolygonItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)

        QPolygonF plg = polygon();

        CGraphObjSelectionPoint* pGraphObjSelPt;
        QPointF                  ptSel;

        while( plg.size() < 2 )
        {
            plg.append( QPointF(0.0,0.0) );

            pGraphObjSelPt = new CGraphObjSelectionPoint(
                /* pDrawingScene     */ m_pDrawingScene,
                /* pGraphObjSelected */ this,
                /* idxPt             */ plg.size()-1 );
            m_arpSelPtsPolygon.append(pGraphObjSelPt);

            m_pDrawingScene->addGraphObj(pGraphObjSelPt);

            //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
            pGraphObjSelPt->installSceneEventFilter(this);

            ptSel = QPointF(0.0,0.0);
            ptSel = mapToScene(ptSel);
            pGraphObjSelPt->setPos(ptSel);
            pGraphObjSelPt->setZValue( zValue()+0.05 );
        }

        // If there was no polygon shape point selected ..
        if( m_idxSelPtSelectedPolygon < 0 || m_idxSelPtSelectedPolygon >= plg.size() )
        {
            // .. the shape has been initially hit right after creation.
            m_idxSelPtSelectedPolygon = plg.size()-1;
            plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;

            if( m_idxSelPtSelectedPolygon < m_arpSelPtsPolygon.size() )
            {
                pGraphObjSelPt = m_arpSelPtsPolygon[m_idxSelPtSelectedPolygon];

                if( pGraphObjSelPt != nullptr )
                {
                    ptSel = plg[m_idxSelPtSelectedPolygon];
                    ptSel = mapToScene(ptSel);
                    pGraphObjSelPt->setPos(ptSel);
                }
            }
        }

        // If there was already a polygon shape point selected ..
        else
        {
            // .. the currently selected shape point will be fixed and
            // another point will be added.
            plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;
            plg.append(ptMouseItemPos);
            m_idxSelPtSelectedPolygon = plg.size()-1;

            pGraphObjSelPt = new CGraphObjSelectionPoint(
                /* pDrawingScene     */ m_pDrawingScene,
                /* pGraphObjSelected */ this,
                /* idxPt             */ m_idxSelPtSelectedPolygon );
            m_arpSelPtsPolygon.append(pGraphObjSelPt);

            m_pDrawingScene->addGraphObj(pGraphObjSelPt);

            //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
            pGraphObjSelPt->installSceneEventFilter(this);

            ptSel = plg[m_idxSelPtSelectedPolygon];
            ptSel = mapToScene(ptSel);
            pGraphObjSelPt->setPos(ptSel);
            pGraphObjSelPt->setZValue( zValue()+0.05 );
        }

        QGraphicsPolygonItem::setPolygon(plg);

        m_bCoorsDirty = true;

        updateLineEndPolygonCoors();

        updateEditInfo();
        updateToolTip();

    } // if( m_editMode == EEditMode::Creating )

    else if( m_editMode == EEditMode::None )
    {
        if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )
        {
            QGraphicsPolygonItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)

            QPointF   ptMouseItemPos = mapFromScene(i_pEv->scenePos());
            QPolygonF plg = polygon();

            m_editMode       = EEditMode::Move;
            m_editResizeMode = EEditResizeMode::None;
            m_idxSelPtSelectedPolygon   = -1;
            m_selPtSelectedBoundingRect = ESelectionPoint::None;

            CGraphObjSelectionPoint* pGraphObjSelPt;
            QRectF                   rctSelPt;
            QPolygonF                plgSelPt;
            int                      idxSelPt;

            // Polygon shape points may be very close or even overlap each other.
            // On creating the polygon the most likely point the user wants to grab
            // is the last shape point to move it around.

            if( m_arpSelPtsPolygon.size() > 0 )
            {
                for( idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= 0; idxSelPt-- )
                {
                    pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

                    if( pGraphObjSelPt != nullptr )
                    {
                        rctSelPt = pGraphObjSelPt->boundingRect();
                        plgSelPt = mapFromItem( pGraphObjSelPt, rctSelPt );
                        rctSelPt = plgSelPt.boundingRect();

                        if( rctSelPt.contains(ptMouseItemPos) )
                        {
                            m_idxSelPtSelectedPolygon = idxSelPt;
                            m_editMode = EEditMode::MoveShapePoint;
                            break;
                        }
                    }
                }
            }

            // If a polygon shape point has been hit ...
            if( m_editMode == EEditMode::MoveShapePoint && m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < plg.size() )
            {
                // .. move this shape point to the new position.
                plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;
            }

            // If none of the polygon shape points (but the poly line somewhere else) has been hit ...
            else if( m_editMode == EEditMode::Move )
            {
                // .. a new point will be inserted between the two connection points
                // enclosing the poly line segment which has been hit.
                if( plg.size() >= 2 ) // anyway ..
                {
                    QLineF  line;
                    QPointF ptSel;

                    for( idxSelPt = 1; idxSelPt < plg.size(); idxSelPt++ )
                    {
                        line = QLineF( plg[idxSelPt-1], plg[idxSelPt] );

                        if( isLineHit(line,ptMouseItemPos,m_pDrawingScene->getHitToleranceInPx()) )
                        {
                            break;
                        }
                    }

                    if( idxSelPt < plg.size() )
                    {
                        plg.insert(idxSelPt,ptMouseItemPos);

                        m_idxSelPtSelectedPolygon = idxSelPt;
                        m_editMode = EEditMode::MoveShapePoint;

                        m_arpSelPtsPolygon.insert(idxSelPt,nullptr);

                        m_arpSelPtsPolygon[idxSelPt] = pGraphObjSelPt = new CGraphObjSelectionPoint(
                            /* pDrawingScene     */ m_pDrawingScene,
                            /* pGraphObjSelected */ this,
                            /* idxPt             */ idxSelPt );

                        m_pDrawingScene->addGraphObj(pGraphObjSelPt);

                        //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
                        pGraphObjSelPt->installSceneEventFilter(this);

                        ptSel = plg[idxSelPt];
                        ptSel = mapToScene(ptSel);
                        pGraphObjSelPt->setPos(ptSel);
                        pGraphObjSelPt->setZValue( zValue()+0.05 );
                    }
                }
            } // if( m_editMode == EEditMode::Move )

            QGraphicsPolygonItem::setPolygon(plg);

            for( idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++ )
            {
                pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

                if( pGraphObjSelPt != nullptr )
                {
                    pGraphObjSelPt->setShapePointIndex(idxSelPt);

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

            m_bCoorsDirty = true;

            updateLineEndPolygonCoors();

            updateEditInfo();
            updateToolTip();

        } // if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )

    } // if( m_editMode == EEditMode::None )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
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
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strAddTrcInfo );

    Qt::MouseButtons iEvMouseButtonState = i_pEv->buttons() & Qt::MouseButtonMask;

    // If no mouse button is pressed ...
    if( (iEvMouseButtonState == Qt::NoButton) || (iEvMouseButtonState & Qt::LeftButton) )
    {
        if( m_editMode == EEditMode::Creating || m_editMode == EEditMode::MoveShapePoint )
        {
            QPolygonF plg = polygon();
            QPointF   ptMouseItemPos = i_pEv->pos();

            if( m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < plg.size() )
            {
                plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;
            }

            QGraphicsPolygonItem::setPolygon(plg);

            CGraphObjSelectionPoint* pGraphObjSelPt;
            QPointF                  ptSel;

            if( m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < m_arpSelPtsPolygon.size() )
            {
                pGraphObjSelPt = m_arpSelPtsPolygon[m_idxSelPtSelectedPolygon];

                if( pGraphObjSelPt != nullptr )
                {
                    ptSel = plg[m_idxSelPtSelectedPolygon];
                    ptSel = mapToScene(ptSel);
                    pGraphObjSelPt->setPos(ptSel);
                }
            }

            m_bCoorsDirty = true;

            updateLineEndPolygonCoors();

            updateEditInfo();
            updateToolTip();

        } // if( m_editMode == EEditMode::MoveShapePoint )

    } // if( (iEvMouseButtonState == Qt::NoButton) || (iEvMouseButtonState & Qt::LeftButton) )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
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
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QPointF ptMouseItemPos = i_pEv->pos();

    if( m_editMode == EEditMode::Creating )
    {
        QGraphicsPolygonItem::mouseReleaseEvent(i_pEv);
    }

    else if( m_editMode == EEditMode::MoveShapePoint )
    {
        QGraphicsPolygonItem::mouseReleaseEvent(i_pEv);

        QPolygonF plg = polygon();

        if( plg.size() >= 2 ) // anyway ..
        {
            CGraphObjConnectionPoint* pGraphObjCnctPt;
            CGraphObjSelectionPoint*  pGraphObjSelPt;
            int                       idxSelPt;
            QPointF                   ptSel;

            // If the first or the last shape point is selected ..
            if( m_idxSelPtSelectedPolygon == 0 || m_idxSelPtSelectedPolygon == plg.size()-1 )
            {
                // The currently selected shape point will be fixed.
                plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;

                if( m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < m_arpSelPtsPolygon.size() )
                {
                    pGraphObjSelPt = m_arpSelPtsPolygon[m_idxSelPtSelectedPolygon];

                    if( pGraphObjSelPt != nullptr )
                    {
                        ptSel = plg[m_idxSelPtSelectedPolygon];
                        ptSel = mapToScene(ptSel);
                        pGraphObjSelPt->setPos(ptSel);
                    }
                }

                // .. it depends whether the shape point has been moved onto a connection point
                // or somewhere else whether the connection line will be connected with the
                // connection point or whether a new shape points will be inserted or added.
                pGraphObjCnctPt = m_pDrawingScene->getConnectionPoint(i_pEv->scenePos());

                // If the mouse has not been released on a connection point ..
                if( pGraphObjCnctPt == nullptr )
                {
                    // Another point will be added.
                    if( m_idxSelPtSelectedPolygon == 0 )
                    {
                        plg.insert(0,ptMouseItemPos);

                        pGraphObjSelPt = new CGraphObjSelectionPoint(
                            /* pDrawingScene     */ m_pDrawingScene,
                            /* pGraphObjSelected */ this,
                            /* idxPt             */ m_idxSelPtSelectedPolygon );
                        m_arpSelPtsPolygon.insert(0,pGraphObjSelPt);
                    }
                    else // if( m_idxSelPtSelectedPolygon == plg.size()-1 )
                    {
                        plg.append(ptMouseItemPos);
                        m_idxSelPtSelectedPolygon = plg.size()-1;

                        pGraphObjSelPt = new CGraphObjSelectionPoint(
                            /* pDrawingScene     */ m_pDrawingScene,
                            /* pGraphObjSelected */ this,
                            /* idxPt             */ m_idxSelPtSelectedPolygon );
                        m_arpSelPtsPolygon.append(pGraphObjSelPt);
                    }

                    m_pDrawingScene->addGraphObj(pGraphObjSelPt);

                    //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
                    pGraphObjSelPt->installSceneEventFilter(this);

                    ptSel = plg[m_idxSelPtSelectedPolygon];
                    ptSel = mapToScene(ptSel);
                    pGraphObjSelPt->setPos(ptSel);
                    pGraphObjSelPt->setZValue( zValue()+0.05 );

                    QGraphicsPolygonItem::setPolygon(plg);

                    for( idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++ )
                    {
                        pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

                        if( pGraphObjSelPt != nullptr )
                        {
                            pGraphObjSelPt->setShapePointIndex(idxSelPt);

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

                    // From now on the drawing scene must dispatch mouse events "directly" to me:
                    m_pDrawingScene->onGraphObjAddingShapePointsStarted(this);

                } // if( pGraphObjCnctPt == nullptr )

                // If the mouse has been released on a connection point ..
                else // if( pGraphObjCnctPt != nullptr )
                {
                    QPointF ptItemPosOld = pos();
                    QPointF ptItemPosNew = mapToScene(plg[0]);

                    double  dx, dy;
                    int     idxPt;

                    setPos(ptItemPosNew);

                    dx = ptItemPosNew.x() - ptItemPosOld.x();
                    dy = ptItemPosNew.y() - ptItemPosOld.y();

                    QPointF ptPosOld, ptPosNew;

                    for( idxPt = 0; idxPt < plg.size(); idxPt++ )
                    {
                        ptPosOld = plg[idxPt];
                        ptPosNew.setX( ptPosOld.x() - dx );
                        ptPosNew.setY( ptPosOld.y() - dy );
                        plg[idxPt] = ptPosNew;
                    }

                    QGraphicsPolygonItem::setPolygon(plg);

                    if( m_idxSelPtSelectedPolygon == 0 )
                    {
                        if( pGraphObjCnctPt != m_arpCnctPts[static_cast<int>(ELinePoint::Start)] )
                        {
                            setConnectionPoint(ELinePoint::Start,pGraphObjCnctPt);
                        }
                    }
                    else // if( m_idxSelPtSelectedPolygon == plg.size()-1 )
                    {
                        if( pGraphObjCnctPt != m_arpCnctPts[static_cast<int>(ELinePoint::End)] )
                        {
                            setConnectionPoint(ELinePoint::End,pGraphObjCnctPt);
                        }
                    }

                    normalize();

                    m_editMode = EEditMode::None;
                    m_editResizeMode = EEditResizeMode::None;
                    m_idxSelPtSelectedPolygon = -1;
                    m_selPtSelectedBoundingRect = ESelectionPoint::None;

                    acceptCurrentAsOriginalCoors();

                    // Adding shape points has been finished:
                    m_pDrawingScene->onGraphObjAddingShapePointsFinished(this);

                } // if( pGraphObjCnctPt != nullptr )

                m_bCoorsDirty = true;

                updateLineEndPolygonCoors();

                updateEditInfo();
                updateToolTip();

            } // if( m_idxSelPtSelectedPolygon == 0 || m_idxSelPtSelectedPolygon == plg.size()-1 )

            // If neither the first nor the last shape point is selected (but any shape point in between) ..
            else if( m_idxSelPtSelectedPolygon > 0 && m_idxSelPtSelectedPolygon < plg.size()-1 )
            {
                if( m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < plg.size() )
                {
                    plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;

                    QGraphicsPolygonItem::setPolygon(plg);

                    if( m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < m_arpSelPtsPolygon.size() )
                    {
                        pGraphObjSelPt = m_arpSelPtsPolygon[m_idxSelPtSelectedPolygon];

                        if( pGraphObjSelPt != nullptr )
                        {
                            ptSel = plg[m_idxSelPtSelectedPolygon];
                            ptSel = mapToScene(ptSel);
                            pGraphObjSelPt->setPos(ptSel);
                        }
                    }
                }

                normalize();

                m_editMode = EEditMode::None;
                m_editResizeMode = EEditResizeMode::None;
                m_idxSelPtSelectedPolygon = -1;
                m_selPtSelectedBoundingRect = ESelectionPoint::None;

                m_bCoorsDirty = true;

                updateLineEndPolygonCoors();

                acceptCurrentAsOriginalCoors();

                updateEditInfo();
                updateToolTip();

            } // if( m_idxSelPtSelectedPolygon > 0 && m_idxSelPtSelectedPolygon < plg.size()-1 )

        } // if( plg.size() >= 1 )

    } // else if( m_editMode == EEditMode::MoveShapePoint )

    else if( m_editMode != EEditMode::None )
    {
        // The mouse release event would select the object.
        // This is not wanted if the selection tool is not active.
        bool bIsSelectable = flags() & QGraphicsItem::ItemIsSelectable;
        bool bIsSelectableReset = false;

        if( bIsSelectable && m_pDrawingScene->getEditTool() != EEditTool::Select )
        {
            setFlag(QGraphicsItem::ItemIsSelectable,false);
            bIsSelectableReset = true;
        }

        QGraphicsPolygonItem::mouseReleaseEvent(i_pEv);

        if( bIsSelectableReset )
        {
            setFlag(QGraphicsItem::ItemIsSelectable,bIsSelectable);
        }

        normalize();

        m_editMode = EEditMode::None;
        m_editResizeMode = EEditResizeMode::None;
        m_idxSelPtSelectedPolygon = -1;
        m_selPtSelectedBoundingRect = ESelectionPoint::None;

        m_bCoorsDirty = true;

        updateLineEndPolygonCoors();

        acceptCurrentAsOriginalCoors();

        updateEditInfo();
        updateToolTip();

    } // if( m_editMode != EEditMode::Creating )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
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
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
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

    // The default implementation of "mouseDoubleClickEvent" calls mousePressEvent().
    // This is not necessary here.
    //QGraphicsPolygonItem::mouseDoubleClickEvent(i_pEv);

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    CEnumEditMode editModePrev = m_editMode;
    int           idxSelPtSelectedPolygon = m_idxSelPtSelectedPolygon;

    // Reset before calling "drawingScene->onGraphObjCreationFinished".
    m_editMode = EEditMode::None;
    m_editResizeMode = EEditResizeMode::None;
    m_idxSelPtSelectedPolygon = -1;
    m_selPtSelectedBoundingRect = ESelectionPoint::None;

    if( modeDrawing == EMode::Edit )
    {
        if( editModePrev == EEditMode::Creating || editModePrev == EEditMode::MoveShapePoint )
        {
            QPolygonF plg = polygon();
            QPointF   ptMouseItemPos = i_pEv->pos();

            if( plg.size() >= 2 ) // anyway ..
            {
                CGraphObjConnectionPoint* pGraphObjCnctPt;
                CGraphObjSelectionPoint*  pGraphObjSelPt;
                int                       idxSelPt;
                QPointF                   ptSel;

                // If the first or the last shape point is selected ..
                if( idxSelPtSelectedPolygon == 0 || idxSelPtSelectedPolygon == plg.size()-1 )
                {
                    // The currently selected shape point will be fixed.
                    plg[idxSelPtSelectedPolygon] = ptMouseItemPos;

                    // .. it depends whether the shape point has been moved onto a connection point
                    // or somewhere else whether the connection line will be connected with the
                    // connection point or whether a new shape points will be inserted or added.
                    pGraphObjCnctPt = m_pDrawingScene->getConnectionPoint(i_pEv->scenePos());

                    // If the mouse has not been released on a connection point ..
                    if( pGraphObjCnctPt == nullptr )
                    {
                        // Another point will be added. In addition the connection line
                        // should remain connected with the previous connection point.
                        // For this a further point will be added which will be placed
                        // upon the connection point.
                        if( idxSelPtSelectedPolygon == 0 )
                        {
                            pGraphObjCnctPt = m_arpCnctPts[static_cast<int>(ELinePoint::Start)];

                            plg.insert(0,ptMouseItemPos);

                            pGraphObjSelPt = new CGraphObjSelectionPoint(
                                /* pDrawingScene     */ m_pDrawingScene,
                                /* pGraphObjSelected */ this,
                                /* idxPt             */ idxSelPtSelectedPolygon );
                            m_arpSelPtsPolygon.insert(0,pGraphObjSelPt);

                            m_pDrawingScene->addGraphObj(pGraphObjSelPt);

                            //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
                            pGraphObjSelPt->installSceneEventFilter(this);

                            ptSel = plg[idxSelPtSelectedPolygon];
                            ptSel = mapToScene(ptSel);
                            pGraphObjSelPt->setPos(ptSel);
                            pGraphObjSelPt->setZValue( zValue()+0.05 );

                            plg.insert(0,ptMouseItemPos);

                            pGraphObjSelPt = new CGraphObjSelectionPoint(
                                /* pDrawingScene     */ m_pDrawingScene,
                                /* pGraphObjSelected */ this,
                                /* idxPt             */ idxSelPtSelectedPolygon );
                            m_arpSelPtsPolygon.insert(0,pGraphObjSelPt);

                            m_pDrawingScene->addGraphObj(pGraphObjSelPt);

                            //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
                            pGraphObjSelPt->installSceneEventFilter(this);

                            ptSel = plg[idxSelPtSelectedPolygon];
                            ptSel = mapToScene(ptSel);
                            pGraphObjSelPt->setPos(ptSel);
                            pGraphObjSelPt->setZValue( zValue()+0.05 );

                        } // if( idxSelPtSelectedPolygon == 0 )

                        else // if( idxSelPtSelectedPolygon == plg.size()-1 )
                        {
                            pGraphObjCnctPt = m_arpCnctPts[static_cast<int>(ELinePoint::End)];

                            plg.append(ptMouseItemPos);
                            idxSelPtSelectedPolygon = plg.size()-1;

                            pGraphObjSelPt = new CGraphObjSelectionPoint(
                                /* pDrawingScene     */ m_pDrawingScene,
                                /* pGraphObjSelected */ this,
                                /* idxPt             */ idxSelPtSelectedPolygon );
                            m_arpSelPtsPolygon.append(pGraphObjSelPt);

                            m_pDrawingScene->addGraphObj(pGraphObjSelPt);

                            //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
                            pGraphObjSelPt->installSceneEventFilter(this);

                            ptSel = plg[idxSelPtSelectedPolygon];
                            ptSel = mapToScene(ptSel);
                            pGraphObjSelPt->setPos(ptSel);
                            pGraphObjSelPt->setZValue( zValue()+0.05 );

                            plg.append(ptMouseItemPos);
                            idxSelPtSelectedPolygon = plg.size()-1;

                            pGraphObjSelPt = new CGraphObjSelectionPoint(
                                /* pDrawingScene     */ m_pDrawingScene,
                                /* pGraphObjSelected */ this,
                                /* idxPt             */ idxSelPtSelectedPolygon );
                            m_arpSelPtsPolygon.append(pGraphObjSelPt);

                            m_pDrawingScene->addGraphObj(pGraphObjSelPt);

                            //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
                            pGraphObjSelPt->installSceneEventFilter(this);

                            ptSel = plg[idxSelPtSelectedPolygon];
                            ptSel = mapToScene(ptSel);
                            pGraphObjSelPt->setPos(ptSel);
                            pGraphObjSelPt->setZValue( zValue()+0.05 );
                        }

                        QPointF ptItemPosOld = pos();
                        QPointF ptItemPosNew = mapToScene(plg[0]);

                        double  dx, dy;
                        int     idxPt;

                        setPos(ptItemPosNew);

                        dx = ptItemPosNew.x() - ptItemPosOld.x();
                        dy = ptItemPosNew.y() - ptItemPosOld.y();

                        QPointF ptPosOld, ptPosNew;

                        for( idxPt = 0; idxPt < plg.size(); idxPt++ )
                        {
                            ptPosOld = plg[idxPt];
                            ptPosNew.setX( ptPosOld.x() - dx );
                            ptPosNew.setY( ptPosOld.y() - dy );
                            plg[idxPt] = ptPosNew;
                        }

                        QGraphicsPolygonItem::setPolygon(plg);

                        for( idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++ )
                        {
                            pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

                            if( pGraphObjSelPt != nullptr )
                            {
                                pGraphObjSelPt->setShapePointIndex(idxSelPt);

                                if( idxSelPt == idxSelPtSelectedPolygon )
                                {
                                    pGraphObjSelPt->setSelected(true);
                                }
                                else
                                {
                                    pGraphObjSelPt->setSelected(false);
                                }
                            }
                        }

                    } // if( pGraphObjCnctPt == nullptr )

                    // If the mouse has been released on a connection point
                    // or if the end point has been corrected to remain on
                    // the previous connection point ..
                    if( pGraphObjCnctPt != nullptr )
                    {
                        if( idxSelPtSelectedPolygon == 0 )
                        {
                            if( pGraphObjCnctPt != m_arpCnctPts[static_cast<int>(ELinePoint::Start)] )
                            {
                                setConnectionPoint(ELinePoint::Start,pGraphObjCnctPt);
                            }
                            else
                            {
                                onParentItemCoorsHasChanged(pGraphObjCnctPt);
                            }
                        }
                        else // if( idxSelPtSelectedPolygon == plg.size()-1 )
                        {
                            if( pGraphObjCnctPt != m_arpCnctPts[static_cast<int>(ELinePoint::End)] )
                            {
                                setConnectionPoint(ELinePoint::End,pGraphObjCnctPt);
                            }
                            else
                            {
                                onParentItemCoorsHasChanged(pGraphObjCnctPt);
                            }
                        }

                    } // if( pGraphObjCnctPt != nullptr )

                } // if( idxSelPtSelectedPolygon == 0 || idxSelPtSelectedPolygon == plg.size()-1 )

                // Before the double click event a mouse press event occurs creating an additional
                // point which is not desired. This point got to be removed again. We are going
                // to remove all succeeding points overlapping each other which will also remove the
                // last undesired point.
                normalize();

                acceptCurrentAsOriginalCoors();

                m_bCoorsDirty = true;

                updateLineEndPolygonCoors();

                // The object has been initially created.
                if( editModePrev == EEditMode::Creating )
                {
                    m_pDrawingScene->onGraphObjCreationFinished(this);
                }
                else // if( editModePrev == EEditMode::MoveShapePoint )
                {
                    // Editing shape points has been finished:
                    m_pDrawingScene->onGraphObjAddingShapePointsFinished(this);
                }

                updateEditInfo();
                updateToolTip();

            } // if( plg.size() >= 2 ) // anyway ..

        } // if( editModePrev == EEditMode::Creating || editModePrev == EEditMode::MoveShapePoint )

        else if( isSelected() )
        {
            onCreateAndExecDlgFormatGraphObjs();
        }

    } // if( modeDrawing == EMode::Edit )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // mouseDoubleClickEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphObjConnectionLine::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
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
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
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
        }

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
          || i_change == ItemParentHasChanged
          || i_change == ItemTransformHasChanged
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
        updateSelectionPointsOfPolygon( polygon() );
        updateLabelPositions();
        updateEditInfo();
        updateToolTip();
        update();
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

        //QHashIterator<QString,SGraphObjLabel*> itLabels(m_arpLabels);

        //SGraphObjLabel* pGraphObjLabel;

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

    valChanged = QGraphicsPolygonItem::itemChange(i_change,i_value);

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
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
        mthTracer.setMethodReturn(strAddTrcInfo);
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
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "normalize",
        /* strAddInfo   */ strAddTrcInfo );

    QPolygonF plgOld = polygon();

    QPolygonF plgNew = normalizePolygon( plgOld, m_pDrawingScene->getHitToleranceInPx() );

    if( plgOld.size() != plgNew.size() )
    {
        QGraphicsPolygonItem::setPolygon(plgNew);

        hideSelectionPoints();

        if( isSelected() )
        {
            showSelectionPoints();
        }
    }

} // normalize

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::updateLineEndPolygonCoors()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
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

        m_plgCurr = polygon();

        if( m_plgCurr.size() > 1 )
        {
            if( lineEndStyleLineStart != ELineEndStyle::Normal || lineEndStyleLineEnd != ELineEndStyle::Normal )
            {
                ELineEndBaseLineType baseLineTypeLineStart = m_drawSettings.getLineEndBaseLineType(ELinePoint::Start);
                ELineEndBaseLineType baseLineTypeLineEnd   = m_drawSettings.getLineEndBaseLineType(ELinePoint::End);

                QLineF linFirst( m_plgCurr[0], m_plgCurr[1] );
                QLineF linLast( m_plgCurr[m_plgCurr.size()-2], m_plgCurr[m_plgCurr.size()-1] );

                if( lineEndStyleLineStart != ELineEndStyle::Normal )
                {
                    getLineEndPolygons(
                        /* line          */ linFirst,
                        /* drawSetings   */ m_drawSettings,
                        /* pplgLineStart */ &m_plgLineStart,
                        /* pplgLineEnd   */ nullptr );

                    if( baseLineTypeLineStart != ELineEndBaseLineType::NoLine )
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

                    if( baseLineTypeLineEnd != ELineEndBaseLineType::NoLine )
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
