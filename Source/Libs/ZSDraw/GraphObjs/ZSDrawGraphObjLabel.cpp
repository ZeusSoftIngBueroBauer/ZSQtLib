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

#include "ZSDraw/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/GraphObjFactories/ZSDrawObjFactory.h"
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
    CGraphObj*      i_pGraphObjParent,
    const QString&  i_strKey,
    const QString&  i_strText,
    ESelectionPoint i_selPt ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ EGraphObjTypeLabel,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeLabel),
        /* strObjName          */ /*i_pGraphObjParent->path() + ".Label." +*/ i_strKey,
        /* drawSettings        */ CDrawSettings(),
        /* idxTreeEntryType    */ EEntryType::Leave ),
    QGraphicsSimpleTextItem(i_strText),
    m_strKey(i_strKey),
    m_pGraphObjParent(i_pGraphObjParent),
    m_pGraphicsItemParent(nullptr),
    m_selPtLinked(i_selPt),
    m_sizeLinkedSelPtDist(0.0, 0.0),
    m_bShowAnchorLine(false)
{
    // Just incremented by the ctor but not decremented by the dtor.
    // Used to create a unique name for newly created objects of this type.
    s_iInstCount++;

    createTraceAdminObjs("Labels::" + ClassName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Key: " + i_strKey + ", Text: " + i_strText +
            ", SelPt: " + CEnumSelectionPoint(i_selPt).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_pGraphicsItemParent = dynamic_cast<QGraphicsItem*>(m_pGraphObjParent);

    if( m_pGraphicsItemParent == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "m_pGraphicsItemParent == nullptr" );
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
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    if (m_pGraphObjParent != nullptr) {
        m_pGraphObjParent->onLabelAboutToBeDestroyed(this);
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
    if (pGraphicsItem != nullptr) {
        if (m_pDrawingScene != nullptr) {
            if (!m_strKeyInTree.isEmpty()) {
                try {
                    // Cannot be called from within dtor of "CGraphObj" as the dtor
                    // of class "QGraphicsItem" may have already been processed and
                    // models and Views may still try to access the graphical object.
                    m_pDrawingScene->onGraphObjAboutToBeDestroyed(m_strKeyInTree);
                }
                catch(...) {
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
        }
    }

    //m_strKey;
    m_pGraphObjParent = nullptr;
    m_pGraphicsItemParent = nullptr;
    m_selPtLinked = static_cast<ESelectionPoint>(0);
    //m_sizeLinkedSelPtDist;
    m_bShowAnchorLine = false;

} // dtor

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjLabel::clone()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    CGraphObjLabel* pGraphObj = nullptr;
    return pGraphObj;
}

/*==============================================================================
public: // replacing methods of QGraphicsSimpleTextItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabel::setText( const QString& i_strText )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strText;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setText",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsSimpleTextItem::setText(i_strText);
    m_rctCurr = QGraphicsSimpleTextItem::boundingRect();
}

//------------------------------------------------------------------------------
QString CGraphObjLabel::getText() const
//------------------------------------------------------------------------------
{
    return QGraphicsSimpleTextItem::text();
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabel::setLinkedSelectionPoint(ESelectionPoint i_selPt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumSelectionPoint(i_selPt).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setLinkedSelectionPoint",
        /* strAddInfo   */ strMthInArgs );

    if (m_selPtLinked != i_selPt)
    {
        QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

        m_selPtLinked = i_selPt;

        QPointF ptSelPt = m_pGraphObjParent->getSelectionPointCoors(i_selPt);
        ptSelPt = m_pGraphicsItemParent->mapToScene(ptSelPt);

        QPointF ptLabelTmp = ptSelPt;
        if (i_selPt != ESelectionPoint::BottomRight &&
            i_selPt != ESelectionPoint::BottomLeft &&
            i_selPt != ESelectionPoint::BottomCenter)
        {
            ptLabelTmp.setY(ptLabelTmp.y() - getHeight());
        }

        QSize sizeDist(ptLabelTmp.x() - ptSelPt.x(), ptLabelTmp.y() - ptSelPt.y());

        QPointF ptLabel(ptSelPt.x() + sizeDist.width(), ptSelPt.y() + sizeDist.height());

        setPos(ptLabel);
        setZValue(pGraphicsItem->zValue() + 0.02);

        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
void CGraphObjLabel::showAnchorLine()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "showAnchorLine",
        /* strAddInfo   */ "" );

    if (!m_bShowAnchorLine) {
        m_bShowAnchorLine = true;
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
void CGraphObjLabel::hideAnchorLine()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hideAnchorLine",
        /* strAddInfo   */ "" );

    if (m_bShowAnchorLine) {
        m_bShowAnchorLine = false;
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjLabel::getScenePolygonShapePointsString() const
//------------------------------------------------------------------------------
{
    QRectF rct = QGraphicsSimpleTextItem::boundingRect();
    QPolygonF plgScene = mapToScene(rct);
    return polygon2Str(plgScene);
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabel::onDrawSettingsChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "onDrawSettingsChanged",
        /* strAddInfo   */ "" );

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

    if (m_drawSettings.isTextUsed()) {
        QFont fnt = m_drawSettings.getTextFont();
        ETextEffect textEffect = m_drawSettings.getTextEffect();
        ETextSize textSize = m_drawSettings.getTextSize();
        ETextStyle textStyle = m_drawSettings.getTextStyle();
        bool bItalic = (textStyle == ETextStyle::Italic || textStyle == ETextStyle::BoldItalic);
        bool bBold = (textStyle == ETextStyle::Bold || textStyle == ETextStyle::BoldItalic);
        bool bStrikeout = (textEffect == ETextEffect::Strikeout || textEffect == ETextEffect::StrikeoutUnderline);
        bool bUnderline = (textEffect == ETextEffect::Underline || textEffect == ETextEffect::StrikeoutUnderline);

        fnt.setPixelSize( textSize2SizeInPixels(textSize) );
        fnt.setItalic(bItalic);
        fnt.setBold(bBold);
        fnt.setStrikeOut(bStrikeout);
        fnt.setUnderline(bUnderline);

        setFont(fnt);
    }
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabel::setIsHit( bool i_bHit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bHit);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setIsHit",
        /* strAddInfo   */ strMthInArgs );

    if (m_bIsHit != i_bHit)
    {
        m_bIsHit = i_bHit;

        update();

        QRectF rctUpd = boundingRect();
        QPolygonF plgLabel = mapToScene(rctUpd);
        QRectF rctGraphObj = QRectF( QPointF(0.0,0.0), m_pGraphObjParent->getSize() );
        QPolygonF plgGraphObj = m_pGraphicsItemParent->mapToScene(rctGraphObj);

        rctUpd = plgLabel.boundingRect();
        rctGraphObj = plgGraphObj.boundingRect();
        rctUpd |= rctGraphObj;

        m_pDrawingScene->update(rctUpd);
    }
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjLabel::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjIsHit, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs =
            "Point:" + point2Str(i_pt) +
            ", HitInfo, " + QString(o_pHitInfo == nullptr ? "null" : pointer2Str(o_pHitInfo));
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjIsHit,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "isHit",
        /* strAddInfo   */ strMthInArgs );

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();
        mthTracer.trace("BoundingRect {" + qRect2Str(rctBounding) + "}");
    }

    bool bIsHit = false;

    QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();

    bIsHit = isRectHit( rctBounding, EFillStyle::SolidPattern, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo );

    if (bIsHit && o_pHitInfo != nullptr) {
        o_pHitInfo->m_editMode = EEditMode::Move;
        o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
        o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
        o_pHitInfo->m_idxPolygonShapePoint = -1;
        o_pHitInfo->m_idxLineSegment = -1;
        o_pHitInfo->m_ptSelected = rctBounding.center();
    }

    if (bIsHit && o_pHitInfo != nullptr) {
        double fRotAngleCurr_deg = m_fRotAngleCurr_deg;
        if (m_pGraphObjParent != nullptr) {
            fRotAngleCurr_deg = m_pGraphObjParent->getRotationAngleInDegree();
        }
        o_pHitInfo->setCursor( Math::deg2Rad(fRotAngleCurr_deg) );
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
void CGraphObjLabel::setCursor( const QCursor& i_cursor )
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

    QGraphicsSimpleTextItem::setCursor(i_cursor);
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabel::showSelectionPoints( unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strAddTrcInfo = selectionPoints2Str(i_selPts);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "showSelectionPoints",
        /* strAddInfo   */ strAddTrcInfo );
}

//------------------------------------------------------------------------------
void CGraphObjLabel::updateSelectionPoints( unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strAddTrcInfo = selectionPoints2Str(i_selPts);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateSelectionPoints",
        /* strAddInfo   */ strAddTrcInfo );
}

/*==============================================================================
public: // must overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the bounding recangle for the label.

    This method is called by the graphics scene to detect the area to be updated
    and for some other reasons (dispatching mouse events, ...).

    If the label is hit, selected or if the anchor line to the linked grapical
    object is visible the area to be updated on changing the items graphical
    representation also includes the anchor line.

    When painting the selection frame around the label this method cannot be
    used as that would return a wrong area if the label is selected, hit or
    has a visible anchor line.

    To get the rectangle around the labels text the base implementation of
    QGraphicsSimpleTextItem need to be called directly.

    @return Bounding rectangle.
*/
QRectF CGraphObjLabel::boundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ "" );

    // If parent items are hit or selected they are calling this boundingRect method
    // in order to update the drawing scene region including the line from the parent
    // item to the label. The labels boundingRect method itself calls the boundingRect
    // method of its parent item if the label is selected or has been hit. This would
    // lead to an endless recursion. For this an additional boundingRect method of the
    // label is provided with a different signature to indicate that the caller is
    // only interested in the bounding rect of the simple text item.

    QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();

    // If the object is hit and the anchor line is visible also this area need to be updated.
    if( m_bIsHit || isSelected() || m_bShowAnchorLine )
    {
        QRectF    rctGraphObj = m_pGraphicsItemParent->boundingRect();
        QPolygonF plgGraphObj = m_pGraphicsItemParent->mapToScene(rctGraphObj);
        QPolygonF plg = mapFromScene(plgGraphObj);

        rctBounding |= plg.boundingRect();
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        QString strMthReturn =
            "CenterPos {" + point2Str(rctBounding.center()) + "}" +
            ", Rect {" + qRect2Str(rctBounding) + "}";
        mthTracer.setMethodReturn(strMthReturn);
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
void CGraphObjLabel::paint(
    QPainter*                       i_pPainter,
    const QStyleOptionGraphicsItem* i_pStyleOption,
    QWidget*                        i_pWdgt )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "paint",
        /* strAddInfo   */ "" );

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer);
    }

    i_pPainter->save();

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    QPen pn;

    QRectF rct = QGraphicsSimpleTextItem::boundingRect();

    // Draw bounding rectangle in dotted line style if the label is hit by
    // mouse move (hover) or if the label is selected or if no text is assigned.
    if( m_bIsHit || isSelected() )
    {
        pn.setColor(Qt::blue);
        pn.setStyle(Qt::DotLine);

        i_pPainter->setPen(pn);
        i_pPainter->drawRect(rct);
    }
    else if( text().isEmpty() )
    {
        pn.setColor(Qt::black);
        pn.setStyle(Qt::DotLine);

        i_pPainter->setPen(pn);
        i_pPainter->drawRect(rct);
    }

    // Draw anchor line to selection point of linked object if the label is hit by
    // mouse move (hover) or if the label is selected.
    // If the anchor line is set to be visible draw the anchor line in solid style.
    if( m_bIsHit || isSelected() || m_bShowAnchorLine )
    {
        /*  The anchor line will be drawn to one of the center points at
            the bounding rectangle. Which line of the labels bounding rectangle
            should be used depends on the relative position of the label to the
            selection point of the graph object the label is linked to.

            "QLineF::angle" is used to calculate the angle of the line between the two
            anchor points.

            From Qts documentation:
            -----------------------
            The return value will be in the range of values from 0.0 up
            to but not including 360.0. The angles are measured counter-clockwise from
            a point on the x-axis to the right of the origin (x > 0).
            The following diagram should also clarify whats been returned by "QLineF::angle":

                            90°
                135°    16     1     2    45°
                    15        |        3
                14      +---+---+      4
            180° 13-------| Label |-------5   0°  (360°)
                12      +---+---+      6
                    11        |        7
                225°    10     9     8   315°
                            270°

            Selection Point Position | clockwise | Selectoin Point
            of "Parent Item"         |           | of Label
            -------------------------+-----------+-----------------
            16, 1, 2                 | 135°-45°  | TopCenter
            3, 4, 5, 6, 7            |  45°-315° | RightCenter
            8, 9, 10                 | 315°-225° | BottomCenter
            11, 12, 13, 14, 15       | 225°-135° | LeftCenter

            If the angle is calculated the distance between the linked selection point
            of the parent item to the anchor line will also be taken into account.
            E.g. if the label is very close to the parent item it is better not to draw
            the anchor line.
        */

        QRectF rctBoundingThis = QGraphicsSimpleTextItem::boundingRect();
        QPointF ptThis = rctBoundingThis.center();

        QPointF ptSelPt = m_pGraphObjParent->getSelectionPointCoors(m_selPtLinked);
        ptSelPt = m_pGraphicsItemParent->mapToScene(ptSelPt);
        ptSelPt = pGraphicsItem->mapFromScene(ptSelPt);

        QLineF lineAnchor(ptThis, ptSelPt);
        double fAngle = lineAnchor.angle();

        bool bDrawAnchorLine = true;

        if( fAngle >= 45.0 && fAngle <= 135.0 )
        {
            ptThis = ZS::Draw::getSelectionPointCoors(rctBoundingThis, ESelectionPoint::TopCenter);
            lineAnchor.setP1(ptThis);
            if( fabs(lineAnchor.dy()) < 5.0 ) bDrawAnchorLine = false;
        }
        else if( (fAngle >= 225.0 && fAngle <= 315.0) )
        {
            ptThis = ZS::Draw::getSelectionPointCoors(rctBoundingThis, ESelectionPoint::BottomCenter);
            lineAnchor.setP1(ptThis);
            if( fabs(lineAnchor.dy()) < 5.0 ) bDrawAnchorLine = false;
        }
        else if( fAngle > 135.0 && fAngle < 225.0 )
        {
            ptThis = ZS::Draw::getSelectionPointCoors(rctBoundingThis, ESelectionPoint::LeftCenter);
            lineAnchor.setP1(ptThis);
            if( fabs(lineAnchor.dx()) < 5.0 ) bDrawAnchorLine = false;
        }
        else if( (fAngle > 315.0 && fAngle <= 360.0) || (fAngle >= 0.0 && fAngle < 45.0) )
        {
            ptThis = ZS::Draw::getSelectionPointCoors(rctBoundingThis, ESelectionPoint::RightCenter);
            lineAnchor.setP1(ptThis);
            if( fabs(lineAnchor.dx()) < 5.0 ) bDrawAnchorLine = false;
        }

        if( bDrawAnchorLine )
        {
            if( m_bIsHit || isSelected() )
            {
                pn.setColor(Qt::blue);
            }
            else
            {
                pn.setColor(Qt::lightGray);
            }
            pn.setStyle(Qt::DotLine);
            i_pPainter->setPen(pn);
            i_pPainter->drawLine(ptThis, ptSelPt);
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
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strMthInArgs );

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
}

//------------------------------------------------------------------------------
void CGraphObjLabel::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strMthInArgs );

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
}

//------------------------------------------------------------------------------
void CGraphObjLabel::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        qGraphicsSceneHoverEvent2Str(i_pEv);
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
void CGraphObjLabel::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsSimpleTextItem::mousePressEvent(i_pEv);

    //i_pEv->accept();
}

//------------------------------------------------------------------------------
void CGraphObjLabel::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsSimpleTextItem::mouseMoveEvent(i_pEv);

    //i_pEv->accept();
}

//------------------------------------------------------------------------------
void CGraphObjLabel::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsSimpleTextItem::mouseReleaseEvent(i_pEv);

    //i_pEv->accept();

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CGraphObjLabel::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        qGraphicsSceneMouseEvent2Str(i_pEv);
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
    // This is not necessary here.
    //QGraphicsSimpleTextItem::mouseDoubleClickEvent(i_pEv);

    i_pEv->accept();

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

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs =
            "Changed:" + qGraphicsItemChange2Str(i_change) +
            ", Value: " + qVariantType2Str(i_value.type());
        if (i_value.type() == QVariant::PointF) {
            strMthInArgs += " {" + point2Str(i_value.toPointF()) + "}";
        } else {
            strMthInArgs += " {" + i_value.toString() + "}";
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "itemChange",
        /* strAddInfo   */ strMthInArgs );

    QVariant valChanged = i_value;

    bool bTreeEntryChanged = false;

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( i_change == ItemSelectedHasChanged )
    {
        QRectF    rctUpd      = boundingRect();
        QPolygonF plgLabel    = mapToScene(rctUpd);
        QRectF    rctGraphObj = QRectF( QPointF(0.0,0.0), m_pGraphObjParent->getSize() );
        QPolygonF plgGraphObj = m_pGraphicsItemParent->mapToScene(rctGraphObj);

        rctUpd      = plgLabel.boundingRect();
        rctGraphObj = plgGraphObj.boundingRect();
        rctUpd     |= rctGraphObj;

        m_pDrawingScene->update(rctUpd);

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
        QPointF ptSelPt = m_pGraphObjParent->getSelectionPointCoors(m_selPtLinked);
        ptSelPt = m_pGraphicsItemParent->mapToScene(ptSelPt);

        QPointF ptThis = scenePos();

        m_sizeLinkedSelPtDist.setWidth( ptThis.x() - ptSelPt.x() );
        m_sizeLinkedSelPtDist.setHeight( ptThis.y() - ptSelPt.y() );

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

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthRet = "ValChanged: " + qVariantType2Str(valChanged.type());
        if( i_value.type() == QVariant::PointF ) {
            strMthRet += " {" + point2Str(valChanged.toPointF()) + "}";
        }
        else {
            strMthRet += " {" + valChanged.toString() + "}";
        }
        mthTracer.setMethodReturn(strMthRet);
    }

    return valChanged;

} // itemChange
