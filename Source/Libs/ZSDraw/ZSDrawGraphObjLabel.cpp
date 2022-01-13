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

#include "ZSDraw/ZSDrawGraphObjLabel.h"
#include "ZSDraw/ZSDrawAux.h"
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
class CGraphObjLabel : public CGraphObj, public QGraphicsSimpleTextItem
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjLabel::s_iInstCount = 0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjLabel::CGraphObjLabel(
    CDrawingScene*  i_pDrawingScene,
    CGraphObj*      i_pGraphObj,
    const QString&  i_strKey,
    const QString&  i_strText,
    ESelectionPoint i_selPt ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ EGraphObjTypeLabel,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeLabel),
        /* strObjName          */ i_pGraphObj->name() + ".Label." + i_strKey,
        /* drawSettings        */ CDrawSettings(),
        /* idxTreeEntryType    */ EIdxTreeEntryType::Leave ),
    QGraphicsSimpleTextItem(i_strText),
    m_strKey(i_strKey),
    m_pGraphObjParent(i_pGraphObj),
    m_pGraphicsItemParent(nullptr),
    m_selPtGraphObjParent(i_selPt)
    //m_sizGraphObjDist(0.0,0.0)
{
    s_iInstCount++;

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

    m_pGraphicsItemParent = dynamic_cast<QGraphicsItem*>(m_pGraphObjParent);

    if( m_pGraphicsItemParent == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItem == nullptr" );
    }

    setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges );

    //setAcceptHoverEvents(true);

    m_rctCurr = QGraphicsSimpleTextItem::boundingRect();

} // ctor

//------------------------------------------------------------------------------
CGraphObjLabel::~CGraphObjLabel()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pGraphObjParent = nullptr;
    m_pGraphicsItemParent = nullptr;

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
CGraphObj* CGraphObjLabel::clone()
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

    CGraphObjLabel* pGraphObj = nullptr;

    return pGraphObj;

} // clone

/*==============================================================================
public: // replacing methods of QGraphicsSimpleTextItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabel::setText( const QString& i_strText )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = i_strText;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setText",
        /* strAddInfo   */ strAddTrcInfo );

    QGraphicsSimpleTextItem::setText(i_strText);

    m_rctCurr = QGraphicsSimpleTextItem::boundingRect();

    if( isSelected() )
    {
        updateSelectionPoints();
    }

    updateLabelPositions();

} // setText

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjLabel::getScenePolygonShapePointsString() const
//------------------------------------------------------------------------------
{
    QString   strScenePolygonShapePoints;
    QRectF    rct = boundingRect();
    QPolygonF plgScene = mapToScene(rct);

    strScenePolygonShapePoints = polygon2Str(plgScene);

    return strScenePolygonShapePoints;

} // getScenePolygonShapePointsString

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabel::onDrawSettingsChanged()
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

    if( m_drawSettings.isTextUsed() )
    {
        QFont fnt = m_drawSettings.getTextFont();

        ETextEffect textEffect = m_drawSettings.getTextEffect();

        ETextSize textSize = m_drawSettings.getTextSize();

        fnt.setPixelSize( textSize2SizeInPixels(textSize) );

        ETextStyle textStyle = m_drawSettings.getTextStyle();

        bool bItalic = (textStyle == ETextStyle::Italic || textStyle == ETextStyle::BoldItalic);
        bool bBold   = (textStyle == ETextStyle::Bold || textStyle == ETextStyle::BoldItalic);

        fnt.setItalic(bItalic);
        fnt.setBold(bBold);

        bool bStrikeout = (textEffect == ETextEffect::Strikeout || textEffect == ETextEffect::StrikeoutUnderline);
        bool bUnderline = (textEffect == ETextEffect::Underline || textEffect == ETextEffect::StrikeoutUnderline);

        fnt.setStrikeOut(bStrikeout);
        fnt.setUnderline(bUnderline);

        setFont(fnt);

        if( isSelected() )
        {
            updateSelectionPoints();
        }

        updateLabelPositions();
    }

} // onDrawSettingsChanged

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabel::setIsHit( bool i_bHit )
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

        update();

        if( m_pGraphObjParent != nullptr && m_pGraphicsItemParent != nullptr )
        {
            QRectF    rctUpd      = QGraphicsSimpleTextItem::boundingRect();
            QPolygonF plgLabel    = mapToScene(rctUpd);
            QRectF    rctGraphObj = QRectF( QPointF(0.0,0.0), m_pGraphObjParent->getSize() );
            QPolygonF plgGraphObj = m_pGraphicsItemParent->mapToScene(rctGraphObj);

            rctUpd      = plgLabel.boundingRect();
            rctGraphObj = plgGraphObj.boundingRect();
            rctUpd     |= rctGraphObj;

            m_pDrawingScene->update(rctUpd);
        }
    }

} // setIsHit

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjLabel::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjBoundingRect != nullptr && m_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        QRectF rctBounding = boundingRect();
        strAddTrcInfo  = "Point:" + point2Str(i_pt);
        strAddTrcInfo += ", Rect(x,y,w,h):(";
        strAddTrcInfo += QString::number(rctBounding.x(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.y(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.width(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.height(),'f',1) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "isHit",
        /* strAddInfo   */ strAddTrcInfo );

    bool bIsHit = false;

    QRectF rctBounding = boundingRect();

    bIsHit = isRectHit( rctBounding, EFillStyle::SolidPattern, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo );

    if( bIsHit && o_pHitInfo != nullptr )
    {
        o_pHitInfo->m_editMode = EEditMode::Move;
        o_pHitInfo->m_editResizeMode = EEditResizeMode::Undefined;
        o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::Undefined;
        o_pHitInfo->m_idxPolygonShapePoint = -1;
        o_pHitInfo->m_idxLineSegment = -1;
        o_pHitInfo->m_ptSelected = rctBounding.center();
    }

    if( bIsHit && o_pHitInfo != nullptr )
    {
        double fRotAngleCurr_deg = m_fRotAngleCurr_deg;

        if( m_pGraphObjParent != nullptr )
        {
            fRotAngleCurr_deg = m_pGraphObjParent->getRotationAngleInDegree();
        }
        o_pHitInfo->setCursor( Math::deg2Rad(fRotAngleCurr_deg) );
    }

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
void CGraphObjLabel::setCursor( const QCursor& i_cursor )
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

    QGraphicsSimpleTextItem::setCursor(i_cursor);

} // setCursor

/*==============================================================================
public: // overridables (to avoid endless recursion)
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjLabel::boundingRect( bool i_bSimpleTextItemOnly ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjBoundingRect != nullptr && m_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "SimpleTextItemOnly:" + bool2Str(i_bSimpleTextItemOnly);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ 1,
        /* strObjName   */ m_strName,
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ strAddTrcInfo );

    // If parent items are hit or selected they are calling this boundingRect method
    // in order to update the drawing scene region including the line from the parent
    // item to the label. The labels boundingRect method itself calls the boundingRect
    // method of its parent item if the label is selected or has been hit. This would
    // lead to an endless recursion. For this an additional boundingRect method of the
    // label is provided with a different signature to indicate that the caller is
    // only interested in the bounding rect of the simple text item.

    QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();

    if( !i_bSimpleTextItemOnly )
    {
        if( m_bIsHit || isSelected() )
        {
            if( m_pGraphObjParent != nullptr && m_pGraphicsItemParent != nullptr )
            {
                QRectF    rctGraphObj = m_pGraphicsItemParent->boundingRect();
                QPolygonF plgGraphObj = m_pGraphicsItemParent->mapToScene(rctGraphObj);
                QPolygonF plg = mapFromScene(plgGraphObj);

                rctBounding |= plg.boundingRect();
            }
        }
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "CenterPos:" + point2Str(rctBounding.center());
        strAddTrcInfo += ", Rect(x,y,w,h):(";
        strAddTrcInfo += QString::number(rctBounding.x(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.y(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.width(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.height(),'f',1) + ")";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return rctBounding;

} // boundingRect

/*==============================================================================
public: // overridables of base class QGraphicsPolygonItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjLabel::boundingRect() const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjBoundingRect != nullptr && m_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ 1,
        /* strObjName   */ m_strName,
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ strAddTrcInfo );

    QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();

    if( m_bIsHit || isSelected() )
    {
        if( m_pGraphObjParent != nullptr && m_pGraphicsItemParent != nullptr )
        {
            QRectF    rctGraphObj = m_pGraphicsItemParent->boundingRect();
            QPolygonF plgGraphObj = m_pGraphicsItemParent->mapToScene(rctGraphObj);
            QPolygonF plg = mapFromScene(plgGraphObj);

            rctBounding |= plg.boundingRect();
        }
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "CenterPos:" + point2Str(rctBounding.center());
        strAddTrcInfo += ", Rect(x,y,w,h):(";
        strAddTrcInfo += QString::number(rctBounding.x(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.y(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.width(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.height(),'f',1) + ")";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return rctBounding;

} // boundingRect

//------------------------------------------------------------------------------
void CGraphObjLabel::paint(
    QPainter*                       i_pPainter,
    const QStyleOptionGraphicsItem* i_pStyleOption,
    QWidget*                        i_pWdgt )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjPaint != nullptr && m_pTrcAdminObjPaint->isActive(ETraceDetailLevelMethodArgs) )
    {
        QRectF rctBounding = boundingRect();
        strAddTrcInfo  = "CenterPos:" + point2Str(rctBounding.center());
        strAddTrcInfo += ", Rect(x,y,w,h):(";
        strAddTrcInfo += QString::number(rctBounding.x(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.y(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.width(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.height(),'f',1) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ 1,
        /* strObjName   */ m_strName,
        /* strMethod    */ "paint",
        /* strAddInfo   */ strAddTrcInfo );

    i_pPainter->save();

    QPen pn;

    QRectF rct = QGraphicsSimpleTextItem::boundingRect();

    if( m_drawSettings.getFillStyle() != EFillStyle::None )
    {
        //QBrush brsh;

        //brsh.setColor( m_drawSettings.getFillColor() );
        //brsh.setStyle( fillStyle2QtBrushStyle(m_drawSettings.getFillStyle()) );

        //i_pPainter->setBrush(brsh);
    }

    if( m_pDrawingScene->getMode() == EMode::Edit )
    {
        if( m_bIsHit || isSelected() )
        {
            pn.setColor(Qt::blue);
            pn.setStyle(Qt::DotLine);

            i_pPainter->setPen(pn);
            i_pPainter->drawRect(rct);

            if( m_pGraphObjParent != nullptr && m_pGraphicsItemParent != nullptr )
            {
                QPointF ptGraphObjParentSelPt = m_pGraphObjParent->getSelectionPoint(m_selPtGraphObjParent);

                ptGraphObjParentSelPt = m_pGraphicsItemParent->mapToScene(ptGraphObjParentSelPt);
                ptGraphObjParentSelPt = mapFromScene(ptGraphObjParentSelPt);

                QPointF ptLabelSelPt = getSelectionPoint(ESelectionPoint::Center);

                i_pPainter->drawLine( ptLabelSelPt, ptGraphObjParentSelPt );
            }

        } // if( m_bIsHit || isSelected() )

        //else if( m_drawSettings.getLineStyle() != ELineStyle::NoLine )
        //{
        //    pn.setColor( m_drawSettings.getPenColor() );
        //    pn.setWidth( m_drawSettings.getPenWidth() );
        //    pn.setStyle( lineStyle2QtPenStyle(m_drawSettings.getLineStyle()) );

        //    i_pPainter->setPen(pn);
        //    i_pPainter->drawRect(rct);
        //}

        else if( text().isEmpty() )
        {
            pn.setColor(Qt::black);
            pn.setStyle(Qt::DotLine);

            i_pPainter->setPen(pn);
            i_pPainter->drawRect(rct);
        }

    } // if( m_pDrawingScene->getMode() == EMode::Edit )

    else // if( m_pDrawingScene->getMode() == EMode::Simulation )
    {
        if( m_drawSettings.getLineStyle() != ELineStyle::NoLine )
        {
            pn.setColor( m_drawSettings.getPenColor() );
            pn.setWidth( m_drawSettings.getPenWidth() );
            pn.setStyle( lineStyle2QtPenStyle(m_drawSettings.getLineStyle()) );

            i_pPainter->setPen(pn);
            i_pPainter->drawRect(rct);
        }
    }

    i_pPainter->restore();

    QStyleOptionGraphicsItem styleOption = *i_pStyleOption;

    // "QGraphicsSimpleTextItem::paint" calls "CGraphObjLabel::boundingRect" to draw the
    // highlighted focus rectangle around the item. "CGraphObjLabel::boundingRect"
    // returns the rectangle including the parent item. So we don't let
    // the base implementation paint the selection rectangle around the item.
    styleOption.state &= ~QStyle::State_Selected;
    styleOption.state &= ~QStyle::State_HasFocus;

    QGraphicsSimpleTextItem::paint(i_pPainter,&styleOption,i_pWdgt);

} // paint

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabel::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjHoverEvents != nullptr && m_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        QRectF rctBounding = boundingRect();
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rctBounding.center().x()) + "," + QString::number(rctBounding.center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rctBounding.x()) + "," + QString::number(rctBounding.y());
        strAddTrcInfo += "," + QString::number(rctBounding.width()) + "," + QString::number(rctBounding.height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strAddTrcInfo );

    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( editToolDrawing == EEditTool::Select )
    {
        if( cursor().shape() != Qt::SizeAllCursor )
        {
            setCursor(Qt::SizeAllCursor);
        }
    }
    else if( editToolDrawing == EEditTool::CreateObjects )
    {
    }

} // hoverEnterEvent

//------------------------------------------------------------------------------
void CGraphObjLabel::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjHoverEvents != nullptr && m_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodArgs) )
    {
        QRectF rctBounding = boundingRect();
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rctBounding.center().x()) + "," + QString::number(rctBounding.center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rctBounding.x()) + "," + QString::number(rctBounding.y());
        strAddTrcInfo += "," + QString::number(rctBounding.width()) + "," + QString::number(rctBounding.height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ 1,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strAddTrcInfo );

    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( editToolDrawing == EEditTool::Select )
    {
        if( cursor().shape() != Qt::SizeAllCursor )
        {
            setCursor(Qt::SizeAllCursor);
        }
    }
    else if( editToolDrawing == EEditTool::CreateObjects )
    {
    }

} // hoverMoveEvent

//------------------------------------------------------------------------------
void CGraphObjLabel::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjHoverEvents != nullptr && m_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        QRectF rctBounding = boundingRect();
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rctBounding.center().x()) + "," + QString::number(rctBounding.center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rctBounding.x()) + "," + QString::number(rctBounding.y());
        strAddTrcInfo += "," + QString::number(rctBounding.width()) + "," + QString::number(rctBounding.height()) + ")";
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
void CGraphObjLabel::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjMouseEvents != nullptr && m_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        QRectF rctBounding = boundingRect();
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rctBounding.center().x()) + "," + QString::number(rctBounding.center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rctBounding.x()) + "," + QString::number(rctBounding.y());
        strAddTrcInfo += "," + QString::number(rctBounding.width()) + "," + QString::number(rctBounding.height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QGraphicsSimpleTextItem::mousePressEvent(i_pEv);

    //i_pEv->accept();

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        QRectF rctBounding = boundingRect();
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rctBounding.center().x()) + "," + QString::number(rctBounding.center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rctBounding.x()) + "," + QString::number(rctBounding.y());
        strAddTrcInfo += "," + QString::number(rctBounding.width()) + "," + QString::number(rctBounding.height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CGraphObjLabel::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjMouseEvents != nullptr && m_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelInternalStates) )
    {
        QRectF rctBounding = boundingRect();
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rctBounding.center().x()) + "," + QString::number(rctBounding.center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rctBounding.x()) + "," + QString::number(rctBounding.y());
        strAddTrcInfo += "," + QString::number(rctBounding.width()) + "," + QString::number(rctBounding.height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ 2,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QGraphicsSimpleTextItem::mouseMoveEvent(i_pEv);

    //i_pEv->accept();

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        QRectF rctBounding = boundingRect();
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rctBounding.center().x()) + "," + QString::number(rctBounding.center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rctBounding.x()) + "," + QString::number(rctBounding.y());
        strAddTrcInfo += "," + QString::number(rctBounding.width()) + "," + QString::number(rctBounding.height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CGraphObjLabel::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjMouseEvents != nullptr && m_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        QRectF rctBounding = boundingRect();
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rctBounding.center().x()) + "," + QString::number(rctBounding.center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rctBounding.x()) + "," + QString::number(rctBounding.y());
        strAddTrcInfo += "," + QString::number(rctBounding.width()) + "," + QString::number(rctBounding.height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QGraphicsSimpleTextItem::mouseReleaseEvent(i_pEv);

    //i_pEv->accept();

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        QRectF rctBounding = boundingRect();
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rctBounding.center().x()) + "," + QString::number(rctBounding.center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rctBounding.x()) + "," + QString::number(rctBounding.y());
        strAddTrcInfo += "," + QString::number(rctBounding.width()) + "," + QString::number(rctBounding.height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CGraphObjLabel::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjMouseEvents != nullptr && m_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        QRectF rctBounding = boundingRect();
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rctBounding.center().x()) + "," + QString::number(rctBounding.center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rctBounding.x()) + "," + QString::number(rctBounding.y());
        strAddTrcInfo += "," + QString::number(rctBounding.width()) + "," + QString::number(rctBounding.height()) + ")";
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
    // This is not necessary here.
    //QGraphicsSimpleTextItem::mouseDoubleClickEvent(i_pEv);

    i_pEv->accept();

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        QRectF rctBounding = boundingRect();
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", CenterPos:(" + QString::number(rctBounding.center().x()) + "," + QString::number(rctBounding.center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rctBounding.x()) + "," + QString::number(rctBounding.y());
        strAddTrcInfo += "," + QString::number(rctBounding.width()) + "," + QString::number(rctBounding.height()) + ")";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // mouseDoubleClickEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphObjLabel::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
//------------------------------------------------------------------------------
{
    if( m_bDtorInProgress )
    {
        return i_value;
    }

    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        QRectF rctBounding = boundingRect();

        strAddTrcInfo = "Changed:" + qGraphicsItemChange2Str(i_change);

        if( i_value.type() == QVariant::PointF )
        {
            strAddTrcInfo += ", Value(" + qVariantType2Str(i_value.type()) + "):" + point2Str(i_value.toPointF());
        }
        else if( i_value.type() == QVariant::Cursor )
        {
            QCursor cursor = i_value.value<QCursor>();
            strAddTrcInfo += ", Value(" + qVariantType2Str(i_value.type()) + "):" + qCursorShape2Str(cursor.shape());
        }
        else
        {
            strAddTrcInfo += ", Value(" + qVariantType2Str(i_value.type()) + "):" + i_value.toString();
        }
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", Pos:(" + QString::number(pos().x()) + "," + QString::number(pos().y()) + ")";
        strAddTrcInfo += ", CenterPos:(" + QString::number(rctBounding.center().x()) + "," + QString::number(rctBounding.center().y()) + ")";
        strAddTrcInfo += ", Rect(x,y,w,h):(" + QString::number(rctBounding.x()) + "," + QString::number(rctBounding.y());
        strAddTrcInfo += "," + QString::number(rctBounding.width()) + "," + QString::number(rctBounding.height()) + ")";
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
        if( m_pGraphObjParent != nullptr && m_pGraphicsItemParent != nullptr )
        {
            QRectF    rctUpd      = QGraphicsSimpleTextItem::boundingRect();
            QPolygonF plgLabel    = mapToScene(rctUpd);
            QRectF    rctGraphObj = QRectF( QPointF(0.0,0.0), m_pGraphObjParent->getSize() );
            QPolygonF plgGraphObj = m_pGraphicsItemParent->mapToScene(rctGraphObj);

            rctUpd      = plgLabel.boundingRect();
            rctGraphObj = plgGraphObj.boundingRect();
            rctUpd     |= rctGraphObj;

            m_pDrawingScene->update(rctUpd);
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
        if( m_pGraphObjParent != nullptr && m_pGraphicsItemParent != nullptr )
        {
            m_pGraphObjParent->updateLabelDistance(m_strKey);
        }

        updateEditInfo();
        updateToolTip();
    }

    else // if( i_change == ItemSceneHasChanged
         //  || i_change == ItemZValueHasChanged )
    {
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
        strAddTrcInfo += ", Pos:(" + QString::number(pos().x()) + "," + QString::number(pos().y()) + ")";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return valChanged;

} // itemChange
