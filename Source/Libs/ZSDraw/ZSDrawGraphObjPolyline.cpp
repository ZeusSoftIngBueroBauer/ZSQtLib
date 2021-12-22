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

#include "ZSDraw/ZSDrawGraphObjPolyline.h"
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
class CGraphObjPolyline : public CGraphObj, public QGraphicsPolygonItem
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjPolyline::ResetCtorsDtorsCounters()
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

qint64 CGraphObjPolyline::s_iCtorsCount = 1;
qint64 CGraphObjPolyline::s_iDtorsCount = 1;

CTrcAdminObj* CGraphObjPolyline::s_pTrcAdminObjCtorsAndDtor = nullptr;
CTrcAdminObj* CGraphObjPolyline::s_pTrcAdminObjBoundingRect = nullptr;
CTrcAdminObj* CGraphObjPolyline::s_pTrcAdminObjPaint = nullptr;
CTrcAdminObj* CGraphObjPolyline::s_pTrcAdminObjSceneEventFilter = nullptr;
CTrcAdminObj* CGraphObjPolyline::s_pTrcAdminObjHoverEvents = nullptr;
CTrcAdminObj* CGraphObjPolyline::s_pTrcAdminObjMouseEvents = nullptr;
CTrcAdminObj* CGraphObjPolyline::s_pTrcAdminObjItemChange = nullptr;

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjPolyline::CGraphObjPolyline(
    CDrawingScene*       i_pDrawingScene,
    const CDrawSettings& i_drawSettings,
    const QString&       i_strObjName,
    const QString&       i_strObjId ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene */ i_pDrawingScene,
        /* strNameSpace  */ "ZS::Draw",
        /* strClassName  */ "CGraphObjPolyline",
        /* type          */ EGraphObjTypePolyline,
        /* strType       */ ZS::Draw::graphObjType2Str(EGraphObjTypePolyline),
        /* strObjName    */ i_strObjName.isEmpty() ? "Polyline" + QString::number(s_iCtorsCount) : i_strObjName,
        /* strObjId      */ i_strObjId.isEmpty() ? "Polyline" + QString::number(s_iCtorsCount) : i_strObjId,
        /* drawSettings  */ i_drawSettings ),
    QGraphicsPolygonItem(),
    m_bCoorsDirty(true),
    m_plgCurr(),
    m_plgOrig(),
    m_plgLineStart(),
    m_plgLineEnd(),
    m_plgOnMousePressEvent()
{
    s_iCtorsCount++;

    QString strAddTrcInfo;

    if( s_pTrcAdminObjCtorsAndDtor == nullptr )
    {
        s_pTrcAdminObjCtorsAndDtor = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjPolyline", "CtorsAndDtor");
        s_pTrcAdminObjBoundingRect = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjPolyline", "BoundingRect");
        s_pTrcAdminObjPaint = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjPolyline", "Paint");
        s_pTrcAdminObjSceneEventFilter = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjPolyline", "SceneEventFilter");
        s_pTrcAdminObjHoverEvents = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjPolyline", "HoverEvents");
        s_pTrcAdminObjMouseEvents = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjPolyline", "MouseEvents");
        s_pTrcAdminObjItemChange = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjPolyline", "ItemChange");

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

/*==============================================================================
protected: // ctor (used by derived classes, e.g. CGraphObjPolygon)
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjPolyline::CGraphObjPolyline(
    CDrawingScene*       i_pDrawingScene,
    const QString&       i_strNameSpace,
    const QString&       i_strClassName,
    EGraphObjType        i_type,
    const QString&       i_strType,
    const QString&       i_strObjName,
    const QString&       i_strObjId,
    const CDrawSettings& i_drawSettings ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene */ i_pDrawingScene,
        /* strNameSpace  */ i_strNameSpace,
        /* strClassName  */ i_strClassName,
        /* type          */ i_type,
        /* strType       */ i_strType,
        /* strObjName    */ i_strObjName,
        /* strObjId      */ i_strObjId,
        /* drawSettings  */ i_drawSettings ),
    QGraphicsPolygonItem(),
    m_bCoorsDirty(true),
    m_plgCurr(),
    m_plgOrig(),
    m_plgLineStart(),
    m_plgLineEnd(),
    m_plgOnMousePressEvent()
{
    s_iCtorsCount++;

    if( s_iCtorsCount == 1 )
    {
        s_pTrcAdminObjCtorsAndDtor = CTrcServer::GetTraceAdminObj("ZS::Draw", i_strClassName, "CtorsAndDtor");
        s_pTrcAdminObjBoundingRect = CTrcServer::GetTraceAdminObj("ZS::Draw", i_strClassName, "BoundingRect");
        s_pTrcAdminObjPaint = CTrcServer::GetTraceAdminObj("ZS::Draw", i_strClassName, "Paint");
        s_pTrcAdminObjSceneEventFilter = CTrcServer::GetTraceAdminObj("ZS::Draw", i_strClassName, "SceneEventFilter");
        s_pTrcAdminObjHoverEvents = CTrcServer::GetTraceAdminObj("ZS::Draw", i_strClassName, "HoverEvents");
        s_pTrcAdminObjMouseEvents = CTrcServer::GetTraceAdminObj("ZS::Draw", i_strClassName, "MouseEvents");
        s_pTrcAdminObjItemChange = CTrcServer::GetTraceAdminObj("ZS::Draw", i_strClassName, "ItemChange");
    }

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjPolyline::~CGraphObjPolyline()
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
CGraphObj* CGraphObjPolyline::clone()
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

    CGraphObjPolyline* pGraphObj = new CGraphObjPolyline(m_pDrawingScene,m_drawSettings);

    pGraphObj->setObjName(m_strObjName);
    pGraphObj->setPolygon( polygon() );
    pGraphObj->setPos( pos() );
    pGraphObj->setRotationAngleInDegree(m_fRotAngleCurr_deg);
    //pGraphObj->setScaleFactors(m_fScaleFacXCurr,m_fScaleFacYCurr);
    pGraphObj->acceptCurrentAsOriginalCoors();

    return pGraphObj;

} // clone

/*==============================================================================
public: // replacing methods of QGraphicsPolygonItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjPolyline::setPolygon( const QPolygonF& i_plg )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Polygon:" + polygon2Str(i_plg);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "setPolygon",
        /* strAddInfo   */ strAddTrcInfo );

    QGraphicsPolygonItem::setPolygon(i_plg);

    QRectF rct = i_plg.boundingRect();

    m_rctCurr.setWidth( rct.width() );
    m_rctCurr.setHeight( rct.height() );

    m_ptRotOriginCurr = m_rctCurr.center();

    if( isSelected() )
    {
        updateSelectionPointsOfBoundingRect(m_rctCurr);

        if( i_plg.size() == m_plgCurr.size() )
        {
            updateSelectionPointsOfPolygon(i_plg);
        }
        else
        {
            showSelectionPointsOfPolygon(i_plg);
        }
    }

    updateLabelPositions();

    m_bCoorsDirty = true;

    updateLineEndPolygonCoors();

    updateEditInfo();
    updateToolTip();

    // As "setPolygon" does not end up in an "itemChange" call (even if the
    // flag "ItemSendsGeometryChanges" has been set) we call the "itemChange"
    // method on our own to update the position of the selection points.
    //itemChange( ItemPositionHasChanged, pos() );

} // setPolygon

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjPolyline::getScenePolygonShapePointsString() const
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
void CGraphObjPolyline::onDrawSettingsChanged()
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
void CGraphObjPolyline::acceptCurrentAsOriginalCoors()
//------------------------------------------------------------------------------
{
    m_plgOrig = m_plgCurr;

    CGraphObj::acceptCurrentAsOriginalCoors();

} // acceptCurrentAsOriginalCoors

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjPolyline::setWidth( double i_fWidth )
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

    setSize( i_fWidth, m_rctCurr.height() );

} // setWidth

//------------------------------------------------------------------------------
void CGraphObjPolyline::setHeight( double i_fHeight )
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

    setSize( m_rctCurr.width(), i_fHeight );

} // setHeight

//------------------------------------------------------------------------------
void CGraphObjPolyline::setSize( double i_fWidth, double i_fHeight )
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

    if( !m_bHasValidOrigCoors )
    {
        m_plgOrig = polygon();
        m_sizOrig = m_plgOrig.boundingRect().size();
    }

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
        double fFacX = 1.0;
        double fFacY = 1.0;

        if( m_sizOrig.width() != 0.0 )
        {
            fFacX = i_fWidth / m_sizOrig.width();
        }
        if( m_sizOrig.height() != 0.0 )
        {
            fFacY = i_fHeight / m_sizOrig.height();
        }

        QPolygonF plg = m_plgOrig;
        QRectF    rct = m_rctCurr;

        rct.setWidth(fWidth);
        rct.setHeight(fHeight);

        QPointF pt;
        int     idxPt;

        for( idxPt = 0; idxPt < m_plgOrig.size(); idxPt++ )
        {
            pt = m_plgOrig[idxPt];

            pt.setX( fFacX * pt.x() );
            pt.setY( fFacY * pt.y() );

            plg[idxPt] = pt;
        }

        setPolygon(plg); // updates m_rctCurr and m_plgCurr
    }

} // setSize

//------------------------------------------------------------------------------
void CGraphObjPolyline::setSize( const QSizeF& i_size )
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

    setSize( i_size.width(), i_size.height() );

} // setSize

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjPolyline::setIsHit( bool i_bHit )
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

        if( i_bHit )
        {
            showSelectionPoints(ESelectionPointsBoundingRectCorner|ESelectionPointsBoundingRectLineCenter);
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
bool CGraphObjPolyline::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
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

            if( !bIsHit )
            {
                bIsHit = isBoundingRectSelectionPointHit(
                    /* pt               */ i_pt,
                    /* iSelPtsCount     */ -1,
                    /* pSelPts          */ nullptr,
                    /* pGraphObjHitInfo */ o_pHitInfo );
            }
        }

        if( !bIsHit )
        {
            QPolygonF plg = polygon();
            bIsHit = isPolygonHit( plg, EFillStyleNoFill, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo );
        }

        if( !bIsHit )
        {
            if( pGraphicsItem->isSelected() )
            {
                bIsHit = pGraphicsItem->contains(i_pt);

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditModeMove;
                    o_pHitInfo->m_editResizeMode = EEditResizeModeUndefined;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPointUndefined;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                }
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
void CGraphObjPolyline::showSelectionPoints( unsigned char i_selPts )
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

    QPolygonF plg = polygon();
    QRectF    rct = plg.boundingRect();

    // Create bounding rectangle's selection points before shape edge points so that
    // the shape edge points receive mouse events before the bounding rectangle's
    // selection points (as they are "above" them).

    showSelectionPointsOfBoundingRect( rct, i_selPts );

    if( i_selPts & ESelectionPointsPolygonShapePoints )
    {
        showSelectionPointsOfPolygon( plg );
    }

} // showSelectionPoints

//------------------------------------------------------------------------------
void CGraphObjPolyline::updateSelectionPoints( unsigned char i_selPts )
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

    QPolygonF plg = polygon();
    QRectF    rct = plg.boundingRect();

    // Create bounding rectangle's selection points before shape edge points so that
    // the shape edge points receive mouse events before the bounding rectangle's
    // selection points (as they are "above" them).

    updateSelectionPointsOfBoundingRect( rct, i_selPts );
    updateSelectionPointsOfPolygon( plg );

} // updateSelectionPoints

/*==============================================================================
public: // overridables of base class QGraphicsPolygonItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjPolyline::boundingRect() const
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

    if( m_bCoorsDirty )
    {
        CGraphObjPolyline* pVThis = const_cast<CGraphObjPolyline*>(this);
        pVThis->updateLineEndPolygonCoors();
    }

    QRectF rctBounding = m_plgCurr.boundingRect();

    CGraphObjSelectionPoint* pGraphObjSelPt;
    QRectF                   rctSelPt;
    QPolygonF                plgSelPt;
    ESelectionPoint          selPt;
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

    for( idxSelPt = 0; idxSelPt < ESelectionPointCount; idxSelPt++ )
    {
        selPt = static_cast<ESelectionPoint>(idxSelPt);

        pGraphObjSelPt = m_arpSelPtsBoundingRect[selPt];

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

    qreal halfpw = pen().widthF() / 2.0;

    if( halfpw > 0.0 )
    {
        rctBounding.adjust( -halfpw, -halfpw, halfpw, halfpw );
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
QPainterPath CGraphObjPolyline::shape() const
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
void CGraphObjPolyline::paint(
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

        pn.setStyle(Qt::SolidLine);
        pn.setWidth(1);

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
        if( m_plgCurr.size() > 0 )
        {
            QRectF rctBounding = m_plgCurr.boundingRect();

            pn.setStyle(Qt::DotLine);
            pn.setColor(Qt::blue);
            pn.setWidth(1);

            i_pPainter->setPen(pn);
            i_pPainter->setBrush(Qt::NoBrush);

            i_pPainter->drawRect(rctBounding);

            if( isSelected() )
            {
                if( m_arpSelPtsBoundingRect[ESelectionPointTopCenter] != nullptr && m_arpSelPtsBoundingRect[ESelectionPointRotateTop] != nullptr )
                {
                    CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[ESelectionPointTopCenter];
                    CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[ESelectionPointRotateTop];

                    QPointF ptRct = QPointF( pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y() );
                    QPointF ptRot = QPointF( pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y() );

                    QPointF ptRctM = mapFromScene(ptRct);
                    QPointF ptRotM = mapFromScene(ptRot);

                    i_pPainter->drawLine( ptRctM, ptRotM );
                }

                if( m_arpSelPtsBoundingRect[ESelectionPointBottomCenter] != nullptr && m_arpSelPtsBoundingRect[ESelectionPointRotateBottom] != nullptr )
                {
                    CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[ESelectionPointBottomCenter];
                    CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[ESelectionPointRotateBottom];

                    QPointF ptRct = QPointF( pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y() );
                    QPointF ptRot = QPointF( pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y() );

                    QPointF ptRctM = mapFromScene(ptRct);
                    QPointF ptRotM = mapFromScene(ptRot);

                    i_pPainter->drawLine( ptRctM, ptRotM );
                }
            } // if( isSelected() )

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

        } // if( m_plgCurr.size() > 0 )

    } // if( m_pDrawingScene->getMode() == EMode::Edit && (m_bIsHit || isSelected()) )

    i_pPainter->restore();

} // paint

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjPolyline::sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv )
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

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
        mthTracer.trace(strAddTrcInfo);
    }

    return bEventHandled;

} // sceneEventFilter

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjPolyline::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
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
void CGraphObjPolyline::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
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
void CGraphObjPolyline::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
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
void CGraphObjPolyline::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
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

    EMode     modeDrawing     = m_pDrawingScene->getMode();
    EEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit )
    {
        //Qt::MouseButtons iEvMouseButtonState = i_pEv->buttons() & Qt::MouseButtonMask;
        Qt::KeyboardModifiers iEvMouseKeyboardModifiers = i_pEv->modifiers() & Qt::KeyboardModifierMask;

        QPointF ptMouseItemPos = i_pEv->pos();

        // The "old" coordinates are used by resize (mouse move) events to calculate
        // the difference between the original coordinates on before starting the
        // resize event and the current coordinates during the resize event.
        m_rctOnMousePressEvent = m_rctCurr;
        m_ptRotOriginOnMousePressEvent = mapToScene(m_ptRotOriginCurr);

        if( editToolDrawing == EEditToolCreateObjects && m_editMode == EEditModeCreating )
        {
            QGraphicsPolygonItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)

            QPolygonF plg = polygon();
            QRectF    rctItem;

            if( plg.size() < 2 )
            {
                plg.append( QPointF(0.0,0.0) );
                plg.append( QPointF(1.0,1.0) );
            }

            // If there was no polygon shape point selected ..
            if( m_idxSelPtSelectedPolygon < 0 || m_idxSelPtSelectedPolygon >= plg.size() )
            {
                // .. the shape has been initially hit right after creation.
                m_idxSelPtSelectedPolygon = plg.size()-1;
                plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;
            }
            // If there was already a polygon shape point selected ..
            else
            {
                // .. the currently selected shape point will be fixed and
                // another point will be added.
                plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;
                plg.append(ptMouseItemPos);
                m_idxSelPtSelectedPolygon = plg.size()-1;
            }

            m_rctCurr = plg.boundingRect();

            m_ptRotOriginCurr = m_rctCurr.center();

            QGraphicsPolygonItem::setPolygon(plg);

            while( m_arpSelPtsPolygon.size() < plg.size() )
            {
                m_arpSelPtsPolygon.append(nullptr);
            }

            showSelectionPointsOfPolygon(plg);
            updateSelectionPointsOfBoundingRect(m_rctCurr);

            updateLabelPositions();

            updateTransform();

            updateEditInfo();
            updateToolTip();

        } // if( editToolDrawing == EEditToolCreateObjects && m_editMode == EEditModeCreating )

        else if( editToolDrawing == EEditToolSelect && m_editMode == EEditModeUndefined )
        {
            if( contains(ptMouseItemPos) )
            {
                QGraphicsPolygonItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)
            }

            if( isSelected() )
            {
                CGraphObjSelectionPoint* pGraphObjSelPt;
                ESelectionPoint          selPt;
                int                      idxSelPt;

                // Polygon shape points may be very close or even overlap each other.
                // On creating the polygon the most likely point the user wants to grab
                // is the last shape point to move it around. If the ALT keyboard key
                // is pressed together with the mouse the "alternate" selection point
                // is desired (if there is any) and we first check whether any selection
                // point from the bounding rectangle has been hit.

                SGraphObjHitInfo hitInfo;

                bool bIsHit = false;

                if( iEvMouseKeyboardModifiers & Qt::AltModifier )
                {
                    bIsHit = isBoundingRectSelectionPointHit(
                        /* pt               */ ptMouseItemPos,
                        /* iSelPtsCount     */ -1,
                        /* pSelPts          */ nullptr,
                        /* pGraphObjHitInfo */ &hitInfo );
                }

                if( !bIsHit )
                {
                    bIsHit = isHit( ptMouseItemPos, &hitInfo );
                }

                m_editMode                  = hitInfo.m_editMode;
                m_editResizeMode            = hitInfo.m_editResizeMode;
                m_idxSelPtSelectedPolygon   = hitInfo.m_idxPolygonShapePoint;
                m_selPtSelectedBoundingRect = hitInfo.m_selPtBoundingRect;

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

                for( idxSelPt = 0; idxSelPt < ESelectionPointCount; idxSelPt++ )
                {
                    selPt = static_cast<ESelectionPoint>(idxSelPt);

                    pGraphObjSelPt = m_arpSelPtsBoundingRect[selPt];

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

                if( m_editMode == EEditModeResize || m_editMode == EEditModeRotate || m_editMode == EEditModeMoveShapePoint )
                {
                    m_plgOnMousePressEvent = polygon();
                    m_rctOnMousePressEvent = m_plgOnMousePressEvent.boundingRect();
                    m_ptRotOriginOnMousePressEvent = mapToScene(m_ptRotOriginCurr);
                }

                m_pDrawingScene->setMode( EMode::Ignore, EEditToolIgnore, m_editMode, m_editResizeMode, false );

                updateTransform();

                updateEditInfo();
                updateToolTip();

            } // if( isSelected() )

        } // if( editToolDrawing == EEditToolSelect && m_editMode == EEditModeUndefined )

    } // if( modeDrawing == EMode::Edit )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
        mthTracer.trace(strAddTrcInfo);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CGraphObjPolyline::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
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

    EMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        Qt::MouseButtons iEvMouseButtonState = i_pEv->buttons() & Qt::MouseButtonMask;

        // If no mouse button is pressed ...
        if( iEvMouseButtonState == Qt::NoButton )
        {
            if( m_editMode == EEditModeCreating /*|| m_editMode == EEditModeMoveShapePoint*/ )
            {
                QPointF ptMouseItemPos = i_pEv->pos();

                QPolygonF plg = polygon();

                if( m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < plg.size() )
                {
                    plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;
                }

                m_rctCurr = plg.boundingRect();

                m_ptRotOriginCurr = m_rctCurr.center();

                QGraphicsPolygonItem::setPolygon(plg);

                // Don't change the position of the object as the position of further
                // mouse events should be received relative to the object's position
                // on starting the edit process on pressing the mouse.
                //setPos(ptPosNew); // does not lead to "itemChange" call even if flag ItemSendsGeometryChanges is set.

                updateSelectionPointsOfPolygon(plg);
                updateSelectionPointsOfBoundingRect(m_rctCurr);

                updateLabelPositions();

                m_bCoorsDirty = true;

                updateLineEndPolygonCoors();

                updateTransform();

                updateEditInfo();
                updateToolTip();

            } // if( m_editMode == EEditModeCreating || m_editMode == EEditModeMoveShapePoint )

        } // if( iEvMouseButtonState == Qt::NoButton )

        // If the left mouse button is pressed ...
        else if( iEvMouseButtonState & Qt::LeftButton )
        {
            if( m_editMode == EEditModeMove )
            {
                QGraphicsPolygonItem::mouseMoveEvent(i_pEv);
            }

            else if( m_editMode == EEditModeResize )
            {
                QPointF ptMouseItemPos = i_pEv->pos();

                QPolygonF plg;

                m_rctCurr = resizeRect( m_rctOnMousePressEvent, m_selPtSelectedBoundingRect, ptMouseItemPos );

                plg = resizePolygon( m_plgOnMousePressEvent, m_rctCurr, m_rctOnMousePressEvent );

                //m_ptRotOriginCurr = m_rctCurr.center();

                QGraphicsPolygonItem::setPolygon(plg);

                updateSelectionPointsOfPolygon(plg);
                updateSelectionPointsOfBoundingRect(m_rctCurr);

                updateLabelPositions();

                m_bCoorsDirty = true;

                updateLineEndPolygonCoors();

                //updateTransform();

                updateEditInfo();
                updateToolTip();

            } // if( m_editMode == EEditModeResize )

            else if( m_editMode == EEditModeRotate )
            {
                QPointF ptMouseScenePos = i_pEv->scenePos();

                double fRotAngle_rad = getAngleRad( m_ptRotOriginOnMousePressEvent, ptMouseScenePos );

                m_fRotAngleCurr_deg = Math::rad2Deg(fRotAngle_rad);

                switch( m_selPtSelectedBoundingRect )
                {
                    case ESelectionPointRotateTop:
                    {
                        m_fRotAngleCurr_deg -= 90.0;
                        break;
                    }
                    case ESelectionPointRotateBottom:
                    {
                        m_fRotAngleCurr_deg -= 270.0;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                m_fRotAngleCurr_deg = Math::round2Res( m_fRotAngleCurr_deg, m_pDrawingScene->getRotationAngleResolutionInDegree() );

                while( m_fRotAngleCurr_deg >= 360.0 )
                {
                    m_fRotAngleCurr_deg -= 360.0;
                }
                while( m_fRotAngleCurr_deg < 0.0 )
                {
                    m_fRotAngleCurr_deg += 360.0;
                }

                m_bCoorsDirty = true;

                updateLineEndPolygonCoors();

                updateTransform();

                updateEditInfo();
                updateToolTip();

            } // if( m_editMode == EEditModeRotate )

            else if( m_editMode == EEditModeMoveShapePoint )
            {
                QPointF ptMouseItemPos = i_pEv->pos();

                QPolygonF plg = polygon();

                CGraphObjSelectionPoint* pGraphObjSelPt;
                QPointF                  ptSel;

                if( plg.size() >= 2 ) // anyway ..
                {
                    if( m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < plg.size() )
                    {
                        plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;

                        setPolygon(plg);

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

                } // if( plg.size() >= 2 )

                m_bCoorsDirty = true;

                updateLineEndPolygonCoors();

                updateTransform();

                updateEditInfo();
                updateToolTip();

            } // if( m_editMode == EEditModeMoveShapePoint )

        } // if( iEvMouseButtonState & Qt::LeftButton )

    } // if( modeDrawing == EMode::Edit )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CGraphObjPolyline::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
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

    EMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        if( m_editMode == EEditModeCreating )
        {
            QGraphicsPolygonItem::mouseReleaseEvent(i_pEv);
        }

        else if( m_editMode == EEditModeMove )
        {
            QGraphicsPolygonItem::mouseReleaseEvent(i_pEv);

            m_editMode = EEditModeUndefined;
            m_editResizeMode = EEditResizeModeUndefined;
            m_idxSelPtSelectedPolygon = -1;
            m_selPtSelectedBoundingRect = ESelectionPointUndefined;

            acceptCurrentAsOriginalCoors();
        }

        else if( m_editMode == EEditModeResize )
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

            m_editMode = EEditModeUndefined;
            m_editResizeMode = EEditResizeModeUndefined;
            m_idxSelPtSelectedPolygon = -1;
            m_selPtSelectedBoundingRect = ESelectionPointUndefined;

            QPointF ptScenePosOld = scenePos();
            QPointF ptScenePosNew = ptScenePosOld;

            QPolygonF plgScene         = mapToScene(m_rctCurr);
            QRectF    rctBoundingScene = plgScene.boundingRect();
            QPointF   ptRotOriginScene = rctBoundingScene.center();
            double    fAngle_rad       = Math::deg2Rad(m_fRotAngleCurr_deg);

            ptScenePosNew = rotatePoint( ptRotOriginScene, ptScenePosOld, -fAngle_rad );

            m_ptRotOriginCurr = m_rctCurr.center();

            setPos(ptScenePosNew);

            m_bCoorsDirty = true;

            updateLineEndPolygonCoors();

            updateTransform();

            acceptCurrentAsOriginalCoors();

            updateEditInfo();
            updateToolTip();

        } // if( m_editMode == EEditModeResize )

        else if( m_editMode == EEditModeRotate )
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

            m_editMode = EEditModeUndefined;
            m_editResizeMode = EEditResizeModeUndefined;
            m_idxSelPtSelectedPolygon = -1;
            m_selPtSelectedBoundingRect = ESelectionPointUndefined;

            acceptCurrentAsOriginalCoors();

            updateEditInfo();
            updateToolTip();

        } // if( m_editMode == EEditModeRotate )

        else if( m_editMode == EEditModeMoveShapePoint )
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

            m_editMode = EEditModeUndefined;
            m_editResizeMode = EEditResizeModeUndefined;
            m_idxSelPtSelectedPolygon = -1;
            m_selPtSelectedBoundingRect = ESelectionPointUndefined;

            normalize();

            QPolygonF plg              = polygon();
            QRectF    rctItem          = plg.boundingRect();
            QPointF   ptScenePosOld    = mapToScene(rctItem.topLeft());
            QPointF   ptScenePosNew    = ptScenePosOld;
            QPolygonF polygonScene     = mapToScene(rctItem);
            QRectF    rctBoundingScene = polygonScene.boundingRect();
            QPointF   ptRotOriginScene = rctBoundingScene.center();
            double    fAngle_rad       = Math::deg2Rad(m_fRotAngleCurr_deg);

            QSizeF    sizMoveOffs( rctItem.left(), rctItem.top() );
            QPointF   pt;
            int       idxPt;

            // The position of the polyline should become the top left corner of it's bounding rectangle.
            // On moving one shape point all other shape points should keep their scene position.
            for( idxPt = 0; idxPt < plg.size(); idxPt++ )
            {
                pt = plg[idxPt];
                pt.setX( pt.x() - sizMoveOffs.width() );
                pt.setY( pt.y() - sizMoveOffs.height() );
                plg[idxPt] = pt;
            }

            QGraphicsPolygonItem::setPolygon(plg);

            m_rctCurr = plg.boundingRect();

            m_ptRotOriginCurr = m_rctCurr.center();

            ptScenePosNew = rotatePoint( ptRotOriginScene, ptScenePosOld, -fAngle_rad );

            setPos(ptScenePosNew);

            m_bCoorsDirty = true;

            updateLineEndPolygonCoors();

            updateTransform();

            acceptCurrentAsOriginalCoors();

            updateEditInfo();
            updateToolTip();

        } // if( m_editMode == EEditModeMoveShapePoint )

    } // if( modeDrawing == EMode::Edit )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CGraphObjPolyline::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
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
    // The default implementation of "mouseDoubleClickEvent" calls "mousePressEvent".
    // This is not necessary here.
    //QGraphicsPolygonItem::mouseDoubleClickEvent(i_pEv);

    EMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        if( m_editMode == EEditModeCreating )
        {
            // Before the double click event a mouse press event occurs creating an additional
            // point which is not desired. This point got to be removed again. We are going
            // to remove all succeeding points overlapping each other which will also remove the
            // last undesired point.

            QPolygonF plg = polygon();

            if( plg.size() > 2 )
            {
                int idxPt;

                for( idxPt = plg.size()-1; idxPt > 0; idxPt-- )
                {
                    if( plg[idxPt] == plg[idxPt-1] )
                    {
                        plg.remove(idxPt);
                    }
                }
            }

            QRectF  rctItem = plg.boundingRect();
            QPointF ptPos   = pos();   // current position (coordinates of first shape point)
            QPointF ptLT    = mapToScene(rctItem.topLeft());
            QSizeF  sizMoveOffs( ptLT.x() - ptPos.x(), ptLT.y() - ptPos.y() );
            QPointF pt;
            int     idxPt;

            // The position of the polyline should become the top left corner of it's bounding rectangle.
            for( idxPt = 0; idxPt < plg.size(); idxPt++ )
            {
                pt = plg[idxPt];
                pt.setX( pt.x() - sizMoveOffs.width() );
                pt.setY( pt.y() - sizMoveOffs.height() );
                plg[idxPt] = pt;
            }

            QGraphicsPolygonItem::setPolygon(plg);

            setPos(ptLT);

            normalize();

            m_bCoorsDirty = true;

            updateLineEndPolygonCoors();

            plg = polygon();
            m_rctCurr = plg.boundingRect();
            m_ptRotOriginCurr = m_rctCurr.center();

            m_editMode = EEditModeUndefined;
            m_editResizeMode = EEditResizeModeUndefined;
            m_idxSelPtSelectedPolygon = -1;
            m_selPtSelectedBoundingRect = ESelectionPointUndefined;

            updateTransform();

            acceptCurrentAsOriginalCoors();

            // The object has been initially created.
            m_pDrawingScene->onGraphObjCreated(this);

            updateEditInfo();
            updateToolTip();

        } // if( m_editMode == EEditModeCreating )

        else if( isSelected() )
        {
            onCreateAndExecDlgFormatGraphObjs();
        }

    } // if( modeDrawing == EMode::Edit )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", Polygon:" + polygon2Str(polygon());
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseDoubleClickEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphObjPolyline::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
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

    else if( i_change == ItemTransformHasChanged )
    {
        QPolygonF plg = polygon();
        QRectF    rctBounding = plg.boundingRect();

        updateSelectionPointsOfBoundingRect(rctBounding);
        updateSelectionPointsOfPolygon(plg);

        updateLabelPositions();

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

        QPolygonF plg = polygon();
        QRectF    rctBounding = plg.boundingRect();

        updateSelectionPointsOfBoundingRect(rctBounding);
        updateSelectionPointsOfPolygon(plg);

        updateLabelPositions();

        updateEditInfo();
        updateToolTip();
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
void CGraphObjPolyline::normalize()
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
        setPolygon(plgNew);

        hideSelectionPoints();

        if( isSelected() )
        {
            showSelectionPoints();
        }
    }

} // normalize

//------------------------------------------------------------------------------
void CGraphObjPolyline::updateLineEndPolygonCoors()
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
                //ELineEndBaseLineType baseLineTypeLineStart = m_drawSettings.getLineEndBaseLineType(ELinePointStart);
                //ELineEndBaseLineType baseLineTypeLineEnd   = m_drawSettings.getLineEndBaseLineType(ELinePointEnd);

                QLineF linFirst( m_plgCurr[0], m_plgCurr[1] );
                QLineF linLast( m_plgCurr[m_plgCurr.size()-2], m_plgCurr[m_plgCurr.size()-1] );

                if( lineEndStyleLineStart != ELineEndStyleNormal )
                {
                    getLineEndPolygons(
                        /* line          */ linFirst,
                        /* drawSetings   */ m_drawSettings,
                        /* pplgLineStart */ &m_plgLineStart,
                        /* pplgLineEnd   */ nullptr );

                    //if( baseLineTypeLineStart != ELineEndBaseLineTypeNoLine )
                    //{
                    //    if( m_plgLineStart.size() == 4 )
                    //    {
                    //        m_plgCurr[0] = m_plgLineStart[3];
                    //    }
                    //}
                }

                if( lineEndStyleLineEnd != ELineEndStyleNormal )
                {
                    getLineEndPolygons(
                        /* line          */ linLast,
                        /* drawSetings   */ m_drawSettings,
                        /* pplgLineStart */ nullptr,
                        /* pplgLineEnd   */ &m_plgLineEnd );

                    //if( baseLineTypeLineEnd != ELineEndBaseLineTypeNoLine )
                    //{
                    //    if( m_plgLineEnd.size() == 4 )
                    //    {
                    //        m_plgCurr[m_plgCurr.size()-1] = m_plgLineEnd[3];
                    //    }
                    //}
                }

            } // if( lineEndStyleLineStart != ELineEndStyleNormal || lineEndStyleLineEnd != ELineEndStyleNormal )

        } // if( m_plg.size() > 1 )

    } // if( m_bCoorsDirty )

    m_bCoorsDirty = false;

} // updateLineEndPolygonCoors
