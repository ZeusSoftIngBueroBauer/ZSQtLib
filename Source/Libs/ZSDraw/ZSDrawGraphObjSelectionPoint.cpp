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
class CGraphObjSelectionPoint : public CGraphObj, public QGraphicsEllipseItem
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjSelectionPoint::s_iCtorsCount = 1;
qint64 CGraphObjSelectionPoint::s_iDtorsCount = 1;

CTrcAdminObj* CGraphObjSelectionPoint::s_pTrcAdminObjCtorsAndDtor = nullptr;
CTrcAdminObj* CGraphObjSelectionPoint::s_pTrcAdminObjBoundingRect = nullptr;
CTrcAdminObj* CGraphObjSelectionPoint::s_pTrcAdminObjPaint = nullptr;
CTrcAdminObj* CGraphObjSelectionPoint::s_pTrcAdminObjSceneEventFilter = nullptr;
CTrcAdminObj* CGraphObjSelectionPoint::s_pTrcAdminObjHoverEvents = nullptr;
CTrcAdminObj* CGraphObjSelectionPoint::s_pTrcAdminObjMouseEvents = nullptr;
CTrcAdminObj* CGraphObjSelectionPoint::s_pTrcAdminObjItemChange = nullptr;

double CGraphObjSelectionPoint::s_fRadius_px = 3.0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjSelectionPoint::CGraphObjSelectionPoint(
    CDrawingScene*  i_pDrawingScene,
    CGraphObj*      i_pGraphObjSelected,
    ESelectionPoint i_selectionPoint ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene */ i_pDrawingScene,
        /* strNameSpace  */ "ZS::Draw",
        /* strClassName  */ "CGraphObjSelectionPoint",
        /* type          */ EGraphObjTypeSelectionPoint,
        /* strType       */ ZS::Draw::graphObjType2Str(EGraphObjTypeSelectionPoint),
        /* strObjName    */ i_pGraphObjSelected->getObjName() + ".SelPt." + selectionPoint2Str(i_selectionPoint),
        /* strObjId      */ i_pGraphObjSelected->getObjName() + ".SelPt." + selectionPoint2Str(i_selectionPoint),
        /* drawSettings  */ CDrawSettings() ),
    QGraphicsEllipseItem( QRectF( -s_fRadius_px, -s_fRadius_px, 2.0*s_fRadius_px, 2.0*s_fRadius_px ) ),
    m_pGraphObjSelected(i_pGraphObjSelected),
    m_selPtType(ESelectionPointTypeBoundingRectangle),
    m_selPt(i_selectionPoint),
    m_idxPt(-1),
    m_fRadius_px(s_fRadius_px),
    m_bSelected(false)
{
    s_iCtorsCount++;

    QString strAddTrcInfo;

    if( s_pTrcAdminObjCtorsAndDtor == nullptr )
    {
        s_pTrcAdminObjCtorsAndDtor = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjSelectionPoint", "CtorsAndDtor");
        s_pTrcAdminObjBoundingRect = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjSelectionPoint", "BoundingRect");
        s_pTrcAdminObjPaint = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjSelectionPoint", "Paint");
        s_pTrcAdminObjSceneEventFilter = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjSelectionPoint", "SceneEventFilter");
        s_pTrcAdminObjHoverEvents = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjSelectionPoint", "HoverEvents");
        s_pTrcAdminObjMouseEvents = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjSelectionPoint", "MouseEvents");
        s_pTrcAdminObjItemChange = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjSelectionPoint", "ItemChange");

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

    m_selPtSelectedBoundingRect = i_selectionPoint;

    //setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges );

    setAcceptHoverEvents(true);

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // ctor

//------------------------------------------------------------------------------
CGraphObjSelectionPoint::CGraphObjSelectionPoint(
    CDrawingScene* i_pDrawingScene,
    CGraphObj*     i_pGraphObjSelected,
    int            i_idxPt ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene */ i_pDrawingScene,
        /* strNameSpace  */ "ZS::Draw",
        /* strClassName  */ "CGraphObjSelectionPoint",
        /* type          */ EGraphObjTypeSelectionPoint,
        /* strType       */ ZS::Draw::graphObjType2Str(EGraphObjTypeSelectionPoint),
        /* strObjName    */ i_pGraphObjSelected->getObjName() + ".SelPt." + QString::number(i_idxPt),
        /* strObjId      */ i_pGraphObjSelected->getObjName() + ".SelPt." + QString::number(i_idxPt),
        /* drawSettings  */ CDrawSettings() ),
    QGraphicsEllipseItem( QRectF( -s_fRadius_px, -s_fRadius_px, 2.0*s_fRadius_px, 2.0*s_fRadius_px ) ),
    m_pGraphObjSelected(i_pGraphObjSelected),
    m_selPtType(ESelectionPointTypePolygonShapePoint),
    m_selPt(ESelectionPointUndefined),
    m_idxPt(i_idxPt),
    m_fRadius_px(s_fRadius_px),
    m_bSelected(false)
{
    s_iCtorsCount++;

    QString strAddTrcInfo;

    if( s_pTrcAdminObjCtorsAndDtor == nullptr )
    {
        s_pTrcAdminObjCtorsAndDtor = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjSelectionPoint", "CtorsAndDtor");
        s_pTrcAdminObjBoundingRect = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjSelectionPoint", "BoundingRect");
        s_pTrcAdminObjPaint = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjSelectionPoint", "Paint");
        s_pTrcAdminObjSceneEventFilter = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjSelectionPoint", "SceneEventFilter");
        s_pTrcAdminObjHoverEvents = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjSelectionPoint", "HoverEvents");
        s_pTrcAdminObjMouseEvents = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjSelectionPoint", "MouseEvents");
        s_pTrcAdminObjItemChange = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjSelectionPoint", "ItemChange");
    }

    if( s_pTrcAdminObjCtorsAndDtor != nullptr && s_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strAddTrcInfo );

    m_idxSelPtSelectedPolygon = i_idxPt;

    //setFlags( /*QGraphicsItem::ItemIsMovable |*/ QGraphicsItem::ItemIsSelectable /*| QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges*/ );

    setAcceptHoverEvents(true);

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // ctor

//------------------------------------------------------------------------------
CGraphObjSelectionPoint::~CGraphObjSelectionPoint()
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

    m_pGraphObjSelected = nullptr;

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
CGraphObj* CGraphObjSelectionPoint::clone()
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

    CGraphObjSelectionPoint* pGraphObj = nullptr;

    return pGraphObj;

} // clone

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::setSelectionPoint( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjBoundingRect != nullptr && s_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "SelPt:" + selectionPoint2Str(i_selPt);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "setSelectionPoint",
        /* strAddInfo   */ strAddTrcInfo );

    m_selPt = i_selPt;

} // setSelectionPoint

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::setShapePointIndex( int i_idxPt )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjBoundingRect != nullptr && s_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Idx:" + QString::number(i_idxPt);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "setShapePointIndex",
        /* strAddInfo   */ strAddTrcInfo );

    m_idxPt = i_idxPt;

} // setShapePointIndex

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::setRadiusInPx( double i_fRadius_px )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjBoundingRect != nullptr && s_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Radius:" + QString::number(i_fRadius_px);
        strAddTrcInfo += ", Selected:" + bool2Str(m_bSelected);
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strAddTrcInfo += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "setRadiusInPx",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_fRadius_px != i_fRadius_px )
    {
        m_fRadius_px = i_fRadius_px;

        m_rctCurr = rect();

        m_ptRotOriginCurr = m_rctCurr.center();

        m_rctCurr.setLeft(m_ptRotOriginCurr.x()-m_fRadius_px);
        m_rctCurr.setTop(m_ptRotOriginCurr.y()-m_fRadius_px);
        m_rctCurr.setWidth(2.0*m_fRadius_px);
        m_rctCurr.setHeight(2.0*m_fRadius_px);

        setRect(m_rctCurr);
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strAddTrcInfo += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // setRadiusInPx

/*==============================================================================
public: // instance methods (replacing the methods of base class QGraphicsItem)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::setSelected( bool i_bSelected )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjBoundingRect != nullptr && s_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(i_bSelected);
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strAddTrcInfo += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "setSelected",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_bSelected != i_bSelected )
    {
        m_bSelected = i_bSelected;

        update();
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strAddTrcInfo += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // setSelected

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjSelectionPoint::getScenePolygonShapePointsString() const
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
void CGraphObjSelectionPoint::onDrawSettingsChanged()
//------------------------------------------------------------------------------
{
    if( m_drawSettings.isPenUsed() )
    {
        if( m_drawSettings.getLineStyle() != ELineStyleNoLine )
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
        if( m_drawSettings.getFillStyle() != EFillStyleNoFill )
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
void CGraphObjSelectionPoint::setWidth( double i_fWidth )
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
void CGraphObjSelectionPoint::setHeight( double i_fHeight )
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
void CGraphObjSelectionPoint::setSize( double i_fWidth, double i_fHeight )
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

    if( fWidth > fHeight )
    {
        fHeight = fWidth;
    }
    else if( fWidth < fHeight )
    {
        fWidth = fHeight;
    }

    if( fWidth != m_rctCurr.width() )
    {
        setRadiusInPx( fabs(fWidth) );
    }

} // setSize

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::setSize( const QSizeF& i_size )
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
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjSelectionPoint::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjBoundingRect != nullptr && s_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Point:" + point2Str(i_pt);
        strAddTrcInfo += ", Rect(x,y,w,h):" + rect2Str(rect());
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
        QRectF rct = rect();

        bIsHit = isRectHit( rct, EFillStyleSolidPattern, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo );

        if( bIsHit )
        {
            if( o_pHitInfo != nullptr )
            {
                if( m_selPtType == ESelectionPointTypeBoundingRectangle )
                {
                    o_pHitInfo->m_editMode = selectionPoint2EditMode(m_selPt);
                    o_pHitInfo->m_editResizeMode = selectionPoint2EditResizeMode(m_selPt);
                    o_pHitInfo->m_selPtBoundingRect = m_selPt;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = rct.center();
                }
                else if( m_selPtType == ESelectionPointTypePolygonShapePoint )
                {
                    o_pHitInfo->m_editMode = EEditModeMoveShapePoint;
                    o_pHitInfo->m_editResizeMode = EEditResizeModeUndefined;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPointUndefined;
                    o_pHitInfo->m_idxPolygonShapePoint = m_idxPt;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = rct.center();
                }
            }
        }

        if( bIsHit && o_pHitInfo != nullptr )
        {
            double fRotAngleCurr_deg = m_fRotAngleCurr_deg;

            if( m_pGraphObjSelected != nullptr )
            {
                fRotAngleCurr_deg = m_pGraphObjSelected->getRotationAngleInDegree();
            }
            o_pHitInfo->setCursor( Math::deg2Rad(fRotAngleCurr_deg) );
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
public: // overridables of base class QGraphicsPolygonItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjSelectionPoint::boundingRect() const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjBoundingRect != nullptr && s_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strAddTrcInfo += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ 1,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ strAddTrcInfo );

    QRectF rctBounding = QGraphicsEllipseItem::boundingRect();

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
void CGraphObjSelectionPoint::paint(
    QPainter*                       i_pPainter,
    const QStyleOptionGraphicsItem* /*i_pStyleOption*/,
    QWidget*                        /*i_pWdgt*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjPaint != nullptr && s_pTrcAdminObjPaint->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strAddTrcInfo += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjPaint,
        /* iDetailLevel */ 1,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "paint",
        /* strAddInfo   */ strAddTrcInfo );

    i_pPainter->save();

    QPen   pn   = pen();
    QBrush brsh = brush();

    if( m_pDrawingScene->getMode() == EMode::Edit && isSelected() )
    {
        if( m_idxSelPtSelectedPolygon >= 0 )
        {
            pn.setColor(Qt::magenta);
            brsh.setStyle(Qt::SolidPattern);
            brsh.setColor(Qt::magenta);
        }
        else if( m_selPtSelectedBoundingRect != ESelectionPointUndefined )
        {
            pn.setColor(Qt::blue);
            brsh.setStyle(Qt::SolidPattern);
            brsh.setColor(Qt::blue);
        }
    }
    else
    {
        pn.setColor(Qt::red);
        brsh.setStyle(Qt::SolidPattern);
        brsh.setColor(Qt::white);
    }

    //QGraphicsEllipseItem::paint( i_pPainter, i_pStyleOption, i_pWdgt );

    i_pPainter->setPen(pn);
    i_pPainter->setBrush(brsh);

    if( (spanAngle() != 0) && (qAbs(spanAngle()) % (360 * 16) == 0) )
    {
        i_pPainter->drawEllipse( rect() );
    }
    else
    {
        i_pPainter->drawPie( rect(), startAngle(), spanAngle() );
    }

    i_pPainter->restore();

} // paint

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjHoverEvents != nullptr && s_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strAddTrcInfo += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strAddTrcInfo );

    EEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( editToolDrawing == EEditToolSelect )
    {
        QCursor cursor = Qt::CustomCursor;

        double fRotAngleCurr_rad = 0.0;
        double fCursorAngle_rad  = 0.0;
        bool   bSetResizeCursor  = false;

        if( m_pGraphObjSelected != nullptr )
        {
            fRotAngleCurr_rad = Math::deg2Rad(m_pGraphObjSelected->getRotationAngleInDegree());
        }

        if( m_selPtSelectedBoundingRect != ESelectionPointUndefined )
        {
            switch( m_selPtSelectedBoundingRect )
            {
                case ESelectionPointTopLeft:
                case ESelectionPointBottomRight:
                {
                    fCursorAngle_rad = fRotAngleCurr_rad + Math::c_f135Degrees_rad; // 2nd quadrant: arrow from right/bottom -> top/left
                    cursor = Qt::SizeFDiagCursor; /*  \  */
                    bSetResizeCursor = true;
                    break;
                }
                case ESelectionPointTopRight:
                case ESelectionPointBottomLeft:
                {
                    fCursorAngle_rad = fRotAngleCurr_rad + Math::c_f45Degrees_rad; // 1st quadrant: arrow from bottom/left -> top/right
                    cursor = Qt::SizeBDiagCursor; /*  /  */
                    bSetResizeCursor = true;
                    break;
                }
                case ESelectionPointLeftCenter:
                case ESelectionPointRightCenter:
                {
                    fCursorAngle_rad = fRotAngleCurr_rad;
                    cursor = Qt::SizeHorCursor;
                    bSetResizeCursor = true;
                    break;
                }
                case ESelectionPointTopCenter:
                case ESelectionPointBottomCenter:
                {
                    fCursorAngle_rad = fRotAngleCurr_rad + Math::c_f90Degrees_rad;
                    cursor = Qt::SizeVerCursor;
                    bSetResizeCursor = true;
                    break;
                }
                case ESelectionPointCenter:
                {
                    cursor = Qt::SizeAllCursor;
                    break;
                }
                case ESelectionPointRotateTop:
                case ESelectionPointRotateBottom:
                {
                    QBitmap bmpCursor(":/ZS/Draw/CursorRotateFree16x16.bmp");
                    QBitmap bmpCursorMask = bmpCursor.createHeuristicMask();
                    cursor = QCursor(bmpCursor,bmpCursorMask);
                    break;
                }
                default:
                {
                    break;
                }
            }

            if( bSetResizeCursor )
            {
                // Force resulting cursor rotation angle to 1st or 2nd quadrant (0..180�)
                while( fCursorAngle_rad >= Math::c_f180Degrees_rad )
                {
                    fCursorAngle_rad -= Math::c_f180Degrees_rad;
                }
                while( fCursorAngle_rad < 0.0 )
                {
                    fCursorAngle_rad += Math::c_f180Degrees_rad;
                }
                if( fCursorAngle_rad >= 0.0 && fCursorAngle_rad < Math::c_f45Degrees_rad/2.0 ) // 0.0 .. 22.5�
                {
                    cursor = Qt::SizeHorCursor;
                }
                else if( fCursorAngle_rad >= Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 3.0*Math::c_f45Degrees_rad/2.0 ) // 22.5� .. 67.5�
                {
                    cursor = Qt::SizeBDiagCursor; // 1st quadrant: arrow from bottom/left -> top/right
                }
                else if( fCursorAngle_rad >= 3.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 5.0*Math::c_f45Degrees_rad/2.0 ) // 67.5� .. 112.5�
                {
                    cursor = Qt::SizeVerCursor;
                }
                else if( fCursorAngle_rad >= 5.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 7.0*Math::c_f45Degrees_rad/2.0 ) // 112.5� .. 157.5�
                {
                    cursor = Qt::SizeFDiagCursor; // 2nd quadrant: arrow from top/left -> bottom/right
                }
                else if( fCursorAngle_rad >= 7.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < Math::c_f180Degrees_rad ) // 157.5� .. 180.0�
                {
                    cursor = Qt::SizeHorCursor;
                }

            } // if( bSetResizeCursor )

        } // if( m_selPtSelectedBoundingRect != ESelectionPointUndefined )

        else if( m_idxSelPtSelectedPolygon >= 0 )
        {
            cursor = Qt::CrossCursor;
        }

        if( cursor.shape() != Qt::CustomCursor )
        {
            setCursor(cursor);
        }

    } // if( editToolDrawing == EEditToolSelect )

    else if( editToolDrawing == EEditToolCreateObjects )
    {
    }

} // hoverEnterEvent

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjHoverEvents != nullptr && s_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strAddTrcInfo += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ 1,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strAddTrcInfo );

    EEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( editToolDrawing == EEditToolSelect )
    {
        QCursor cursor = Qt::CustomCursor;

        double fRotAngleCurr_rad = 0.0;
        double fCursorAngle_rad  = 0.0;
        bool   bSetResizeCursor  = false;

        if( m_pGraphObjSelected != nullptr )
        {
            fRotAngleCurr_rad = Math::deg2Rad(m_pGraphObjSelected->getRotationAngleInDegree());
        }

        if( m_selPtSelectedBoundingRect != ESelectionPointUndefined )
        {
            switch( m_selPtSelectedBoundingRect )
            {
                case ESelectionPointTopLeft:
                case ESelectionPointBottomRight:
                {
                    fCursorAngle_rad = fRotAngleCurr_rad + Math::c_f135Degrees_rad; // 2nd quadrant: arrow from right/bottom -> top/left
                    cursor = Qt::SizeFDiagCursor; /*  \  */
                    bSetResizeCursor = true;
                    break;
                }
                case ESelectionPointTopRight:
                case ESelectionPointBottomLeft:
                {
                    fCursorAngle_rad = fRotAngleCurr_rad + Math::c_f45Degrees_rad; // 1st quadrant: arrow from bottom/left -> top/right
                    cursor = Qt::SizeBDiagCursor; /*  /  */
                    bSetResizeCursor = true;
                    break;
                }
                case ESelectionPointLeftCenter:
                case ESelectionPointRightCenter:
                {
                    fCursorAngle_rad = fRotAngleCurr_rad;
                    cursor = Qt::SizeHorCursor;
                    bSetResizeCursor = true;
                    break;
                }
                case ESelectionPointTopCenter:
                case ESelectionPointBottomCenter:
                {
                    fCursorAngle_rad = fRotAngleCurr_rad + Math::c_f90Degrees_rad;
                    cursor = Qt::SizeVerCursor;
                    bSetResizeCursor = true;
                    break;
                }
                case ESelectionPointCenter:
                {
                    cursor = Qt::SizeAllCursor;
                    break;
                }
                case ESelectionPointRotateTop:
                case ESelectionPointRotateBottom:
                {
                    QBitmap bmpCursor(":/ZS/Draw/CursorRotateFree16x16.bmp");
                    QBitmap bmpCursorMask = bmpCursor.createHeuristicMask();
                    cursor = QCursor(bmpCursor,bmpCursorMask);
                    break;
                }
                default:
                {
                    break;
                }
            }

            if( bSetResizeCursor )
            {
                // Force resulting cursor rotation angle to 1st or 2nd quadrant (0..180�)
                while( fCursorAngle_rad >= Math::c_f180Degrees_rad )
                {
                    fCursorAngle_rad -= Math::c_f180Degrees_rad;
                }
                while( fCursorAngle_rad < 0.0 )
                {
                    fCursorAngle_rad += Math::c_f180Degrees_rad;
                }
                if( fCursorAngle_rad >= 0.0 && fCursorAngle_rad < Math::c_f45Degrees_rad/2.0 ) // 0.0 .. 22.5�
                {
                    cursor = Qt::SizeHorCursor;
                }
                else if( fCursorAngle_rad >= Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 3.0*Math::c_f45Degrees_rad/2.0 ) // 22.5� .. 67.5�
                {
                    cursor = Qt::SizeBDiagCursor; // 1st quadrant: arrow from bottom/left -> top/right
                }
                else if( fCursorAngle_rad >= 3.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 5.0*Math::c_f45Degrees_rad/2.0 ) // 67.5� .. 112.5�
                {
                    cursor = Qt::SizeVerCursor;
                }
                else if( fCursorAngle_rad >= 5.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 7.0*Math::c_f45Degrees_rad/2.0 ) // 112.5� .. 157.5�
                {
                    cursor = Qt::SizeFDiagCursor; // 2nd quadrant: arrow from top/left -> bottom/right
                }
                else if( fCursorAngle_rad >= 7.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < Math::c_f180Degrees_rad ) // 157.5� .. 180.0�
                {
                    cursor = Qt::SizeHorCursor;
                }

            } // if( bSetResizeCursor )

        } // if( m_selPtSelectedBoundingRect != ESelectionPointUndefined )

        else if( m_idxSelPtSelectedPolygon >= 0 )
        {
            cursor = Qt::CrossCursor;
        }

        if( cursor.shape() != Qt::CustomCursor )
        {
            setCursor(cursor);
        }

    } // if( editToolDrawing == EEditToolSelect )

    else if( editToolDrawing == EEditToolCreateObjects )
    {
    }

} // hoverMoveEvent

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjHoverEvents != nullptr && s_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strAddTrcInfo += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
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
void CGraphObjSelectionPoint::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjMouseEvents != nullptr && s_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strAddTrcInfo += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strAddTrcInfo );

    //QGraphicsEllipseItem::mousePressEvent(i_pEv);

    //i_pEv->accept();

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strAddTrcInfo += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjMouseEvents != nullptr && s_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strAddTrcInfo += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ 2,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strAddTrcInfo );

    //QGraphicsEllipseItem::mouseMoveEvent(i_pEv);

    //i_pEv->accept();

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strAddTrcInfo += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjMouseEvents != nullptr && s_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strAddTrcInfo += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strAddTrcInfo );

    //QGraphicsEllipseItem::mouseReleaseEvent(i_pEv);

    //i_pEv->accept();

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strAddTrcInfo += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CGraphObjSelectionPoint::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjMouseEvents != nullptr && s_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strAddTrcInfo += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
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
    //QGraphicsEllipseItem::mouseDoubleClickEvent(i_pEv);

    i_pEv->accept();

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rect().center().x()) + "," + QString::number(rect().center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rect().x()) + "," + QString::number(rect().y());
        strAddTrcInfo += "," + QString::number(rect().width()) + "," + QString::number(rect().height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseDoubleClickEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphObjSelectionPoint::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
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
        strAddTrcInfo += ", Pos:(" + QString::number(pos().x()) + "," + QString::number(pos().y()) + ")";
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

    else // if( i_change == ItemMatrixChange
         //  || i_change == ItemPositionHasChanged
         //  || i_change == ItemTransformHasChanged
         //  || i_change == ItemParentHasChanged
         //  || i_change == ItemSceneHasChanged
         //  || i_change == ItemZValueHasChanged
         //  || i_change == ItemScenePositionHasChanged
         //  || i_change == ItemRotationHasChanged
         //  || i_change == ItemScaleHasChanged
         //  || i_change == ItemTransformOriginPointHasChanged )
    {
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
        strAddTrcInfo += ", Pos:(" + QString::number(pos().x()) + "," + QString::number(pos().y()) + ")";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return valChanged;

} // itemChange
