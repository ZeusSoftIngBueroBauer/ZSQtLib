/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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
#include "ZSDraw/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/ZSDrawGraphObjGroup.h"
#include "ZSDraw/ZSDrawGraphObjLabel.h"
#include "ZSDraw/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysException.h"
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
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::ResetCtorsDtorsCounters()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjCtorsAndDtor != nullptr && s_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "CtorsCount: " + QString::number(s_iCtorsCount);
        strAddTrcInfo += ", DtorsCount: " + QString::number(s_iDtorsCount);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ "",
        /* strMethod    */ "ResetCtorsDtorsCounters",
        /* strAddInfo   */ strAddTrcInfo );

    if( s_iCtorsCount != s_iDtorsCount )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "CtorsCount(=" + QString::number(s_iCtorsCount) + ") != DtorsCount(=" + QString::number(s_iDtorsCount) + ")" );
    }

    s_iCtorsCount = 1;
    s_iDtorsCount = 1;

} // ResetCtorsDtorsCounters

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjConnectionLine::s_iCtorsCount = 1;
qint64 CGraphObjConnectionLine::s_iDtorsCount = 1;

CTrcAdminObj* CGraphObjConnectionLine::s_pTrcAdminObjCtorsAndDtor = nullptr;
CTrcAdminObj* CGraphObjConnectionLine::s_pTrcAdminObjBoundingRect = nullptr;
CTrcAdminObj* CGraphObjConnectionLine::s_pTrcAdminObjPaint = nullptr;
CTrcAdminObj* CGraphObjConnectionLine::s_pTrcAdminObjSceneEventFilter = nullptr;
CTrcAdminObj* CGraphObjConnectionLine::s_pTrcAdminObjHoverEvents = nullptr;
CTrcAdminObj* CGraphObjConnectionLine::s_pTrcAdminObjMouseEvents = nullptr;
CTrcAdminObj* CGraphObjConnectionLine::s_pTrcAdminObjItemChange = nullptr;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjConnectionLine::CGraphObjConnectionLine(
    CDrawingScene*       i_pDrawingScene,
    const CDrawSettings& i_drawSettings,
    const QString&       i_strObjName,
    const QString&       i_strObjId ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene */ i_pDrawingScene,
        /* strNameSpace  */ "ZS::Draw",
        /* strClassName  */ "CGraphObjConnectionLine",
        /* type          */ EGraphObjTypeConnectionLine,
        /* strType       */ ZS::Draw::graphObjType2Str(EGraphObjTypeConnectionLine),
        /* strObjName    */ i_strObjName.isEmpty() ? "ConnectionLine" + QString::number(s_iCtorsCount) : i_strObjName,
        /* strObjId      */ i_strObjId.isEmpty() ? "ConnectionLine" + QString::number(s_iCtorsCount) : i_strObjId,
        /* drawSettings  */ i_drawSettings ),
    QGraphicsPolygonItem(),
    m_bCoorsDirty(true),
    m_plgCurr(),
    m_plgOrig(),
    m_plgLineStart(),
    m_plgLineEnd()
    //m_arpCnctPts[ELinePointCount]
{
    s_iCtorsCount++;

    memset( m_arpCnctPts, 0x00, _ZSArrLen(m_arpCnctPts)*sizeof(m_arpCnctPts[0]) );

    QString strAddTrcInfo;

    if( s_pTrcAdminObjCtorsAndDtor == nullptr )
    {
        s_pTrcAdminObjCtorsAndDtor = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjConnectionLine", "CtorsAndDtor");
        s_pTrcAdminObjBoundingRect = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjConnectionLine", "BoundingRect");
        s_pTrcAdminObjPaint = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjConnectionLine", "Paint");
        s_pTrcAdminObjSceneEventFilter = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjConnectionLine", "SceneEventFilter");
        s_pTrcAdminObjHoverEvents = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjConnectionLine", "HoverEvents");
        s_pTrcAdminObjMouseEvents = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjConnectionLine", "MouseEvents");
        s_pTrcAdminObjItemChange = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjConnectionLine", "ItemChange");

    } // if( s_pTrcAdminObjCtorsAndDtor == nullptr )

    if( s_pTrcAdminObjCtorsAndDtor != nullptr && s_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strAddTrcInfo );

    setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges );

    onDrawSettingsChanged();

    updateToolTip();

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        mthTracer.trace(strAddTrcInfo);
    }

} // ctor

//------------------------------------------------------------------------------
CGraphObjConnectionLine::~CGraphObjConnectionLine()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    s_iDtorsCount++;

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    CGraphObjConnectionPoint* pGraphObjCnctPt;
    int                       idxLinePt;

    for( idxLinePt = 0; idxLinePt < _ZSArrLen(m_arpCnctPts); idxLinePt++ )
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
            if( !m_strObjId.isEmpty() )
            {
                try
                {
                    // Cannot be called from within dtor of "CGraphObj" as the dtor
                    // of class "QGraphicsItem" may have already been processed and
                    // models and Views may still try to access the graphical object
                    // if the drawing scene emits the signal "graphObjDestroying".
                    m_pDrawingScene->onGraphObjDestroying(m_strObjId);
                }
                catch(...)
                {
                }
            }

            // On removing (deleting) a group the group's children have already been
            // removed from the graphics scene. The dtor of class CGraphObjGroup
            // removes the item from the drawing scene (see following line) whereupon
            // also the children of the group are removed by the QGraphicsScene class.
            // In this case "scene()" will return nullptr.
            QGraphicsScene* pGraphicsScene = pGraphicsItem->scene();
            if( pGraphicsScene != nullptr ) // if not already removed from the scene ...
            {
                m_pDrawingScene->removeItem(pGraphicsItem);
            }

        } // if( m_pDrawingScene != nullptr )

    } // if( pGraphicsItem != nullptr )

    if( s_iCtorsCount == s_iDtorsCount )
    {
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjCtorsAndDtor);
        s_pTrcAdminObjCtorsAndDtor = nullptr;
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjBoundingRect);
        s_pTrcAdminObjBoundingRect = nullptr;
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjPaint);
        s_pTrcAdminObjPaint = nullptr;
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjSceneEventFilter);
        s_pTrcAdminObjSceneEventFilter = nullptr;
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjHoverEvents);
        s_pTrcAdminObjHoverEvents = nullptr;
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjMouseEvents);
        s_pTrcAdminObjMouseEvents = nullptr;
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjItemChange);
        s_pTrcAdminObjItemChange = nullptr;
    }

} // dtor

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjConnectionLine::clone()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjCtorsAndDtor != nullptr && s_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "clone",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjConnectionLine* pGraphObj = new CGraphObjConnectionLine(m_pDrawingScene,m_drawSettings);

    pGraphObj->setObjName(m_strObjName);

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

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = linePoint2Str(i_linePoint);

        if( i_pCnctPt == nullptr )
        {
            strAddTrcInfo += ", CnctPoint:nullptr";
        }
        else
        {
            strAddTrcInfo += ", CnctPoint:" + i_pCnctPt->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "setConnectionPoint",
        /* strAddInfo   */ strAddTrcInfo );

    bool bConnected = false;

    if( i_linePoint >= 0 && i_linePoint < _ZSArrLen(m_arpCnctPts) && i_pCnctPt != nullptr )
    {
        CGraphObjConnectionPoint* pGraphObjCnctPt = m_arpCnctPts[i_linePoint];

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
                m_arpCnctPts[i_linePoint] = nullptr;
            }

            bConnected = i_pCnctPt->appendConnectionLine(this);

            if( bConnected )
            {
                m_arpCnctPts[i_linePoint] = i_pCnctPt;
                pGraphObjCnctPt = m_arpCnctPts[i_linePoint];
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

            if( linePoint == ELinePointStart )
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

            } // if( linePoint == ELinePointStart )

            else if( linePoint == ELinePointEnd && plg.size() >= 1 )
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

            } // if( linePoint == ELinePointEnd )

            updateSelectionPointsOfPolygon(plg);
            updateLabelPositions();
            updateToolTip();
            updateEditInfo();

        } // if( pGraphObjCnctPt != nullptr )

    } // if( i_linePoint >= 0 && i_linePoint < _ZSArrLen(m_arpCnctPts) && i_pCnctPt != nullptr )

    return bConnected;

} // setConnectionPoint

//------------------------------------------------------------------------------
ELinePoint CGraphObjConnectionLine::getConnectionLinePoint( CGraphObjConnectionPoint* i_pCnctPt )
//------------------------------------------------------------------------------
{
    ELinePoint linePoint = ELinePointUndefined;
    int        idxLinePtTmp;

    for( idxLinePtTmp = 0; idxLinePtTmp < _ZSArrLen(m_arpCnctPts); idxLinePtTmp++ )
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

    if( i_linePoint >= 0 && i_linePoint < _ZSArrLen(m_arpCnctPts) )
    {
        pGraphObjCnctPt = m_arpCnctPts[i_linePoint];
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

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = polygon2Str(i_plg);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
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
        if( m_drawSettings.getLineStyle() != ELineStyleNoLine )
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

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Width:" + QString::number(i_fWidth);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
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

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Height:" + QString::number(i_fHeight);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
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

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Width:" + QString::number(i_fWidth);
        strAddTrcInfo += ", Height:" + QString::number(i_fHeight);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
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

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Width:" + QString::number(i_size.width());
        strAddTrcInfo += ", Height:" + QString::number(i_size.height());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
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

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Hit:" + bool2Str(i_bHit);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
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

    if( s_pTrcAdminObjBoundingRect != nullptr && s_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Point:" + point2Str(i_pt);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
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
            bIsHit = isPolygonHit( plg, EFillStyleNoFill, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo );
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
            strAddTrcInfo += ", EditMode:" + editMode2Str(o_pHitInfo->m_editMode);
            strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(o_pHitInfo->m_editResizeMode);
            strAddTrcInfo += ", SelPtBoundingRect:" + selectionPoint2Str(o_pHitInfo->m_selPtBoundingRect);
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
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::showSelectionPoints( unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "SelectionPoints:" + selectionPoints2Str(i_selPts);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "showSelectionPoints",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_selPts & ESelectionPointsPolygonShapePoints )
    {
        showSelectionPointsOfPolygon( polygon() );
    }

} // showSelectionPoints

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::updateSelectionPoints( unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "SelectionPoints:" + selectionPoints2Str(i_selPts);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "updateSelectionPoints",
        /* strAddInfo   */ strAddTrcInfo );

    updateSelectionPointsOfPolygon( polygon() );

} // updateSelectionPoints

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::onParentItemCoorsHasChanged( CGraphObj* i_pGraphObjParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Parent:" + i_pGraphObjParent->getObjName();
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
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

        if( linePoint == ELinePointStart && plg.size() > 0 )
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

        } // if( linePoint == ELinePointStart )

        else if( linePoint == ELinePointEnd && plg.size() > 1 )
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

        } // if( linePoint == ELinePointEnd )

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

    if( s_pTrcAdminObjBoundingRect != nullptr && s_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
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

    if( m_bIsHit || isSelected() )
    {
        QHashIterator<QString,SGraphObjLabel*> itLabels(m_arpLabels);
        SGraphObjLabel* pGraphObjLabel;
        QRectF          rctLabel;
        QPolygonF       plgLabel;

        while( itLabels.hasNext() )
        {
            itLabels.next();
            pGraphObjLabel = itLabels.value();

            if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
            {
                // Calling pGraphObjLabel->boundingRect() may lead to endless recursion as the
                // label itself may call the boundingRect method of its parent item (which is
                // this item) if the label is selected or has been hit. For this we call
                // boundingRect of the label with a different signature to indicate that we
                // are only interested in the bounding rect of the simple text item.
                rctLabel = pGraphObjLabel->m_pGraphObjLabel->boundingRect(true);
                plgLabel = mapFromItem( pGraphObjLabel->m_pGraphObjLabel, rctLabel );
                rctBounding |= plgLabel.boundingRect();
            }
        }
    }

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

    if( s_pTrcAdminObjBoundingRect != nullptr && s_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
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
        strAddTrcInfo  = "Path:" + painterPath2Str(pVThis,painterPath);
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

    if( s_pTrcAdminObjPaint != nullptr && s_pTrcAdminObjPaint->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjPaint,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
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

    if( m_arpCnctPts[ELinePointStart] == nullptr || m_arpCnctPts[ELinePointEnd] == nullptr )
    {
        if( m_editMode != EEditModeCreating )
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

    ELineEndStyle lineEndStyleLineStart = m_drawSettings.getLineEndStyle(ELinePointStart);
    ELineEndStyle lineEndStyleLineEnd   = m_drawSettings.getLineEndStyle(ELinePointEnd);

    if( lineEndStyleLineStart != ELineEndStyleNormal || lineEndStyleLineEnd != ELineEndStyleNormal )
    {
        ELineEndBaseLineType baseLineTypeLineStart = m_drawSettings.getLineEndBaseLineType(ELinePointStart);
        ELineEndBaseLineType baseLineTypeLineEnd   = m_drawSettings.getLineEndBaseLineType(ELinePointEnd);
        QBrush               brsh;

        pn.setWidth(1);
        pn.setStyle(Qt::SolidLine);

        i_pPainter->setPen(pn);

        brsh.setColor(pn.color());

        if( lineEndStyleLineStart != ELineEndStyleNormal )
        {
            brsh.setStyle( lineEndFillStyle2QtBrushStyle(m_drawSettings.getLineEndFillStyle(ELinePointStart)) );

            i_pPainter->setBrush(brsh);

            if( baseLineTypeLineStart == ELineEndBaseLineTypeNoLine )
            {
                i_pPainter->drawPolyline(m_plgLineStart);
            }
            else
            {
                i_pPainter->drawPolygon(m_plgLineStart);
            }
        }

        if( lineEndStyleLineEnd != ELineEndStyleNormal )
        {
            brsh.setStyle( lineEndFillStyle2QtBrushStyle(m_drawSettings.getLineEndFillStyle(ELinePointEnd)) );

            i_pPainter->setBrush(brsh);

            if( baseLineTypeLineEnd == ELineEndBaseLineTypeNoLine )
            {
                i_pPainter->drawPolyline(m_plgLineEnd);
            }
            else
            {
                i_pPainter->drawPolygon(m_plgLineEnd);
            }
        }

    } // if( lineEndStyleLineStart != ELineEndStyleNormal || lineEndStyleLineEnd != ELineEndStyleNormal )

    if( m_pDrawingScene->getMode() == EMode::Edit && (m_bIsHit || isSelected()) )
    {
        pn.setStyle(Qt::DotLine);
        pn.setColor(Qt::blue);

        i_pPainter->setPen(pn);

        QHashIterator<QString,SGraphObjLabel*> itLabels(m_arpLabels);
        SGraphObjLabel*                        pGraphObjLabel;

        QPointF ptSelPt;
        QPointF ptLabelSelPt;

        while( itLabels.hasNext() )
        {
            itLabels.next();
            pGraphObjLabel = itLabels.value();

            if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
            {
                ptSelPt = getSelectionPoint(pGraphObjLabel->m_selPt);

                ptLabelSelPt = pGraphObjLabel->m_pGraphObjLabel->getSelectionPoint(ESelectionPointCenter);
                ptLabelSelPt = mapFromItem( pGraphObjLabel->m_pGraphObjLabel, ptLabelSelPt );

                i_pPainter->drawLine( ptSelPt, ptLabelSelPt );
            }
        }

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

    if( s_pTrcAdminObjSceneEventFilter != nullptr && s_pTrcAdminObjSceneEventFilter->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "ItemWatched:" + pGraphObjSelPtWatched->getObjName();
        strAddTrcInfo += ", Event:" + qEventType2Str(i_pEv->type());
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjSceneEventFilter,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
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
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
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

    if( s_pTrcAdminObjHoverEvents != nullptr && s_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QPointF ptItemPos = i_pEv->pos();

    EMode     modeDrawing     = m_pDrawingScene->getMode();
    EEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit && editToolDrawing == EEditToolSelect )
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

    } // if( modeDrawing == EMode::Edit && editToolDrawing == EEditToolSelect )

} // hoverEnterEvent

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjHoverEvents != nullptr && s_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ 1,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QPointF ptItemPos = i_pEv->pos();

    EMode     modeDrawing     = m_pDrawingScene->getMode();
    EEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit && editToolDrawing == EEditToolSelect )
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

    } // if( modeDrawing == EMode::Edit && editToolDrawing == EEditToolSelect )

} // hoverMoveEvent

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjHoverEvents != nullptr && s_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
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

    if( s_pTrcAdminObjMouseEvents != nullptr && s_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strAddTrcInfo );

    //Qt::KeyboardModifiers iEvMouseKeyboardModifiers = i_pEv->modifiers() & Qt::KeyboardModifierMask;

    QPointF ptMouseItemPos = i_pEv->pos();

    EMode     modeDrawing     = m_pDrawingScene->getMode();
    EEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( m_editMode == EEditModeCreating )
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

            m_pDrawingScene->addItem(pGraphObjSelPt);

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

            m_pDrawingScene->addItem(pGraphObjSelPt);

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

    } // if( m_editMode == EEditModeCreating )

    else if( m_editMode == EEditModeUndefined )
    {
        if( modeDrawing == EMode::Edit && editToolDrawing == EEditToolSelect )
        {
            QGraphicsPolygonItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)

            QPointF   ptMouseItemPos = mapFromScene(i_pEv->scenePos());
            QPolygonF plg = polygon();

            m_editMode       = EEditModeMove;
            m_editResizeMode = EEditResizeModeUndefined;
            m_idxSelPtSelectedPolygon   = -1;
            m_selPtSelectedBoundingRect = ESelectionPointUndefined;

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
                            m_editMode = EEditModeMoveShapePoint;
                            break;
                        }
                    }
                }
            }

            // If a polygon shape point has been hit ...
            if( m_editMode == EEditModeMoveShapePoint && m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < plg.size() )
            {
                // .. move this shape point to the new position.
                plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;
            }

            // If none of the polygon shape points (but the poly line somewhere else) has been hit ...
            else if( m_editMode == EEditModeMove )
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
                        m_editMode = EEditModeMoveShapePoint;

                        m_arpSelPtsPolygon.insert(idxSelPt,nullptr);

                        m_arpSelPtsPolygon[idxSelPt] = pGraphObjSelPt = new CGraphObjSelectionPoint(
                            /* pDrawingScene     */ m_pDrawingScene,
                            /* pGraphObjSelected */ this,
                            /* idxPt             */ idxSelPt );

                        m_pDrawingScene->addItem(pGraphObjSelPt);

                        //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
                        pGraphObjSelPt->installSceneEventFilter(this);

                        ptSel = plg[idxSelPt];
                        ptSel = mapToScene(ptSel);
                        pGraphObjSelPt->setPos(ptSel);
                        pGraphObjSelPt->setZValue( zValue()+0.05 );
                    }
                }
            } // if( m_editMode == EEditModeMove )

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

            m_pDrawingScene->setMode( EMode::Ignore, EEditToolIgnore, m_editMode, m_editResizeMode, false );

            m_bCoorsDirty = true;

            updateLineEndPolygonCoors();

            updateEditInfo();
            updateToolTip();

        } // if( modeDrawing == EMode::Edit && editToolDrawing == EEditToolSelect )

    } // if( m_editMode == EEditModeUndefined )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjMouseEvents != nullptr && s_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ 2,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strAddTrcInfo );

    Qt::MouseButtons iEvMouseButtonState = i_pEv->buttons() & Qt::MouseButtonMask;

    // If no mouse button is pressed ...
    if( (iEvMouseButtonState == Qt::NoButton) || (iEvMouseButtonState & Qt::LeftButton) )
    {
        if( m_editMode == EEditModeCreating || m_editMode == EEditModeMoveShapePoint )
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

        } // if( m_editMode == EEditModeMoveShapePoint )

    } // if( (iEvMouseButtonState == Qt::NoButton) || (iEvMouseButtonState & Qt::LeftButton) )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjMouseEvents != nullptr && s_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QPointF ptMouseItemPos = i_pEv->pos();

    if( m_editMode == EEditModeCreating )
    {
        QGraphicsPolygonItem::mouseReleaseEvent(i_pEv);
    }

    else if( m_editMode == EEditModeMoveShapePoint )
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

                    m_pDrawingScene->addItem(pGraphObjSelPt);

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
                        if( pGraphObjCnctPt != m_arpCnctPts[ELinePointStart] )
                        {
                            setConnectionPoint(ELinePointStart,pGraphObjCnctPt);
                        }
                    }
                    else // if( m_idxSelPtSelectedPolygon == plg.size()-1 )
                    {
                        if( pGraphObjCnctPt != m_arpCnctPts[ELinePointEnd] )
                        {
                            setConnectionPoint(ELinePointEnd,pGraphObjCnctPt);
                        }
                    }

                    normalize();

                    m_editMode = EEditModeUndefined;
                    m_editResizeMode = EEditResizeModeUndefined;
                    m_idxSelPtSelectedPolygon = -1;
                    m_selPtSelectedBoundingRect = ESelectionPointUndefined;

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

                m_editMode = EEditModeUndefined;
                m_editResizeMode = EEditResizeModeUndefined;
                m_idxSelPtSelectedPolygon = -1;
                m_selPtSelectedBoundingRect = ESelectionPointUndefined;

                m_bCoorsDirty = true;

                updateLineEndPolygonCoors();

                acceptCurrentAsOriginalCoors();

                updateEditInfo();
                updateToolTip();

            } // if( m_idxSelPtSelectedPolygon > 0 && m_idxSelPtSelectedPolygon < plg.size()-1 )

        } // if( plg.size() >= 1 )

    } // else if( m_editMode == EEditModeMoveShapePoint )

    else if( m_editMode != EEditModeUndefined )
    {
        // The mouse release event would select the object.
        // This is not wanted if the selection tool is not active.
        bool bIsSelectable = flags() & QGraphicsItem::ItemIsSelectable;
        bool bIsSelectableReset = false;

        if( bIsSelectable && m_pDrawingScene->getEditTool() != EEditToolSelect )
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

        m_editMode = EEditModeUndefined;
        m_editResizeMode = EEditResizeModeUndefined;
        m_idxSelPtSelectedPolygon = -1;
        m_selPtSelectedBoundingRect = ESelectionPointUndefined;

        m_bCoorsDirty = true;

        updateLineEndPolygonCoors();

        acceptCurrentAsOriginalCoors();

        updateEditInfo();
        updateToolTip();

    } // if( m_editMode != EEditModeCreating )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjMouseEvents != nullptr && s_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strAddTrcInfo );

    // When doubleclicking an item, the item will first receive a mouse
    // press event, followed by a release event (i.e., a click), then a
    // doubleclick event, and finally a release event.

    // The default implementation of "mouseDoubleClickEvent" calls mousePressEvent().
    // This is not necessary here.
    //QGraphicsPolygonItem::mouseDoubleClickEvent(i_pEv);

    EMode modeDrawing = m_pDrawingScene->getMode();

    EEditMode editModePrev = m_editMode;
    int       idxSelPtSelectedPolygon = m_idxSelPtSelectedPolygon;

    // Reset before calling "drawingScene->onGraphObjCreated".
    m_editMode = EEditModeUndefined;
    m_editResizeMode = EEditResizeModeUndefined;
    m_idxSelPtSelectedPolygon = -1;
    m_selPtSelectedBoundingRect = ESelectionPointUndefined;

    if( modeDrawing == EMode::Edit )
    {
        if( editModePrev == EEditModeCreating || editModePrev == EEditModeMoveShapePoint )
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
                            pGraphObjCnctPt = m_arpCnctPts[ELinePointStart];

                            plg.insert(0,ptMouseItemPos);

                            pGraphObjSelPt = new CGraphObjSelectionPoint(
                                /* pDrawingScene     */ m_pDrawingScene,
                                /* pGraphObjSelected */ this,
                                /* idxPt             */ idxSelPtSelectedPolygon );
                            m_arpSelPtsPolygon.insert(0,pGraphObjSelPt);

                            m_pDrawingScene->addItem(pGraphObjSelPt);

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

                            m_pDrawingScene->addItem(pGraphObjSelPt);

                            //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
                            pGraphObjSelPt->installSceneEventFilter(this);

                            ptSel = plg[idxSelPtSelectedPolygon];
                            ptSel = mapToScene(ptSel);
                            pGraphObjSelPt->setPos(ptSel);
                            pGraphObjSelPt->setZValue( zValue()+0.05 );

                        } // if( idxSelPtSelectedPolygon == 0 )

                        else // if( idxSelPtSelectedPolygon == plg.size()-1 )
                        {
                            pGraphObjCnctPt = m_arpCnctPts[ELinePointEnd];

                            plg.append(ptMouseItemPos);
                            idxSelPtSelectedPolygon = plg.size()-1;

                            pGraphObjSelPt = new CGraphObjSelectionPoint(
                                /* pDrawingScene     */ m_pDrawingScene,
                                /* pGraphObjSelected */ this,
                                /* idxPt             */ idxSelPtSelectedPolygon );
                            m_arpSelPtsPolygon.append(pGraphObjSelPt);

                            m_pDrawingScene->addItem(pGraphObjSelPt);

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

                            m_pDrawingScene->addItem(pGraphObjSelPt);

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
                            if( pGraphObjCnctPt != m_arpCnctPts[ELinePointStart] )
                            {
                                setConnectionPoint(ELinePointStart,pGraphObjCnctPt);
                            }
                            else
                            {
                                onParentItemCoorsHasChanged(pGraphObjCnctPt);
                            }
                        }
                        else // if( idxSelPtSelectedPolygon == plg.size()-1 )
                        {
                            if( pGraphObjCnctPt != m_arpCnctPts[ELinePointEnd] )
                            {
                                setConnectionPoint(ELinePointEnd,pGraphObjCnctPt);
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
                if( editModePrev == EEditModeCreating )
                {
                    m_pDrawingScene->onGraphObjCreated(this);
                }
                else // if( editModePrev == EEditModeMoveShapePoint )
                {
                    // Editing shape points has been finished:
                    m_pDrawingScene->onGraphObjAddingShapePointsFinished(this);
                }

                updateEditInfo();
                updateToolTip();

            } // if( plg.size() >= 2 ) // anyway ..

        } // if( editModePrev == EEditModeCreating || editModePrev == EEditModeMoveShapePoint )

        else if( isSelected() )
        {
            onCreateAndExecDlgFormatGraphObjs();
        }

    } // if( modeDrawing == EMode::Edit )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
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

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Changed:" + graphicsItemChange2Str(i_change);

        if( i_value.type() == QVariant::PointF )
        {
            strAddTrcInfo += ", Value(" + qVariantType2Str(i_value.type()) + "):" + point2Str(i_value.toPointF());
        }
        else
        {
            strAddTrcInfo += ", Value(" + qVariantType2Str(i_value.type()) + "):" + i_value.toString();
        }
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "itemChange",
        /* strAddInfo   */ strAddTrcInfo );

    QVariant valChanged = i_value;

    if( i_change == ItemSelectedHasChanged )
    {
        prepareGeometryChange();

        if( m_pDrawingScene->getMode() == EMode::Edit && isSelected() )
        {
            bringToFront(); // does not set "m_fZValue" as it is used to restore the stacking order on deselecting the object

            if( m_editMode == EEditModeCreating )
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

            m_editMode = EEditModeUndefined;
            m_editResizeMode = EEditResizeModeUndefined;
            m_selPtSelectedBoundingRect = ESelectionPointUndefined;
            m_idxSelPtSelectedPolygon = -1;
        }

        updateEditInfo();
        updateToolTip();

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

        for( idxSelPt = 0; idxSelPt < ESelectionPointCount; idxSelPt++ )
        {
            selPt = static_cast<ESelectionPoint>(idxSelPt);

            pGraphObjSelPt = m_arpSelPtsBoundingRect[selPt];

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

        QHashIterator<QString,SGraphObjLabel*> itLabels(m_arpLabels);

        SGraphObjLabel* pGraphObjLabel;

        while( itLabels.hasNext() )
        {
            itLabels.next();

            pGraphObjLabel = itLabels.value();

            if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
            {
                pGraphObjLabel->m_pGraphObjLabel->setZValue( zValue() + 0.02 );
            }
        }
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
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
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

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
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

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + QString::number(m_idxSelPtSelectedPolygon);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "updateLineEndPolygonCoors",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_bCoorsDirty )
    {
        ELineEndStyle lineEndStyleLineStart = m_drawSettings.getLineEndStyle(ELinePointStart);
        ELineEndStyle lineEndStyleLineEnd   = m_drawSettings.getLineEndStyle(ELinePointEnd);

        m_plgCurr = polygon();

        if( m_plgCurr.size() > 1 )
        {
            if( lineEndStyleLineStart != ELineEndStyleNormal || lineEndStyleLineEnd != ELineEndStyleNormal )
            {
                ELineEndBaseLineType baseLineTypeLineStart = m_drawSettings.getLineEndBaseLineType(ELinePointStart);
                ELineEndBaseLineType baseLineTypeLineEnd   = m_drawSettings.getLineEndBaseLineType(ELinePointEnd);

                QLineF linFirst( m_plgCurr[0], m_plgCurr[1] );
                QLineF linLast( m_plgCurr[m_plgCurr.size()-2], m_plgCurr[m_plgCurr.size()-1] );

                if( lineEndStyleLineStart != ELineEndStyleNormal )
                {
                    getLineEndPolygons(
                        /* line          */ linFirst,
                        /* drawSetings   */ m_drawSettings,
                        /* pplgLineStart */ &m_plgLineStart,
                        /* pplgLineEnd   */ nullptr );

                    if( baseLineTypeLineStart != ELineEndBaseLineTypeNoLine )
                    {
                        if( m_plgLineStart.size() == 4 )
                        {
                            m_plgCurr[0] = m_plgLineStart[3];
                        }
                    }
                }

                if( lineEndStyleLineEnd != ELineEndStyleNormal )
                {
                    getLineEndPolygons(
                        /* line          */ linLast,
                        /* drawSetings   */ m_drawSettings,
                        /* pplgLineStart */ nullptr,
                        /* pplgLineEnd   */ &m_plgLineEnd );

                    if( baseLineTypeLineEnd != ELineEndBaseLineTypeNoLine )
                    {
                        if( m_plgLineEnd.size() == 4 )
                        {
                            m_plgCurr[m_plgCurr.size()-1] = m_plgLineEnd[3];
                        }
                    }
                }

            } // if( lineEndStyleLineStart != ELineEndStyleNormal || lineEndStyleLineEnd != ELineEndStyleNormal )

        } // if( m_plg.size() > 1 )

    } // if( m_bCoorsDirty )

    m_bCoorsDirty = false;

} // updateLineEndPolygonCoors
