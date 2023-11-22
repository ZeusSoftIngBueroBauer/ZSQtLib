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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Common/ZSDrawAux.h"
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
class CGraphObjLabel : public CGraphObj, public QGraphicsSimpleTextItem
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjLabel::CGraphObjLabel(
    CDrawingScene* i_pDrawingScene,
    CGraphObj* i_pGraphObjParent,
    const QString& i_strKey,
    const QString& i_strText,
    const SGraphObjSelectionPoint& i_selPt) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ EGraphObjTypeLabel,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeLabel),
        /* strObjName          */ i_strKey,
        /* idxTreeEntryType    */ EEntryType::Leave ),
    QGraphicsSimpleTextItem(i_strText),
    m_strKey(i_strKey),
    m_selPt(i_selPt),
    m_distanceToLinkedSelPt(0.0, 0.0),
    m_bShowAnchorLine(false),
    m_anchorLine(),
    m_bUpdatePositionInProgress(false)
{
    createTraceAdminObjs("Labels::" + ClassName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Key: " + i_strKey + ", Text: " + i_strText +
            ", Parent: " + QString(i_pGraphObjParent == nullptr ? "null" : i_pGraphObjParent->keyInTree()) +
            ", Text: " + i_strText +
            ", SelPt: " + i_selPt.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemSendsGeometryChanges);

    setParentGraphObj(i_pGraphObjParent);

} // ctor

/*==============================================================================
protected: // ctor (used by derived classes, e.g. CGraphObjLabelPosition)
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjLabel::CGraphObjLabel(
    CDrawingScene* i_pDrawingScene,
    CGraphObj* i_pGraphObjParent,
    const QString& i_strKey,
    const QString& i_strText,
    const SGraphObjSelectionPoint& i_selPt,
    EGraphObjType i_type) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ i_type,
        /* strType             */ ZS::Draw::graphObjType2Str(i_type),
        /* strObjName          */ i_strKey,
        /* idxTreeEntryType    */ EEntryType::Leave ),
    QGraphicsSimpleTextItem(i_strText),
    m_strKey(i_strKey),
    m_selPt(i_selPt),
    m_distanceToLinkedSelPt(0.0, 0.0),
    m_bShowAnchorLine(false),
    m_anchorLine(),
    m_bUpdatePositionInProgress(false)
{
    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemSendsGeometryChanges);

    setParentGraphObj(i_pGraphObjParent);
}

/*==============================================================================
public: // dtor
==============================================================================*/

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

    emit_aboutToBeDestroyed();

    // Labels have been "directly" added to the graphics scene and have not by
    // invoking CDrawingScene::addGraphObj so that they don't appear in the index tree.
    // For this selection points "directly" remove themselves from the graphics scene.
    if (scene() != nullptr) {
        scene()->removeItem(this);
    }

    //m_strKey;
    //m_selPt;
    //m_distanceToLinkedSelPt;
    m_bShowAnchorLine = false;
    //m_anchorLine;
    m_bUpdatePositionInProgress = false;

} // dtor

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the type method of QGraphicsItem.
*/
int CGraphObjLabel::type() const
//------------------------------------------------------------------------------
{
    return QGraphicsItem::UserType + EGraphObjTypeLabel;
}

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
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabel::setKey(const QString& i_strKey)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strKey;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setKey",
        /* strAddInfo   */ strMthInArgs );

    if (m_strKey != i_strKey) {
        m_strKey = i_strKey;
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
QString CGraphObjLabel::key() const
//------------------------------------------------------------------------------
{
    return m_strKey;
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

    if (QGraphicsSimpleTextItem::text() != i_strText) {
        QGraphicsSimpleTextItem::setText(i_strText);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
QString CGraphObjLabel::text() const
//------------------------------------------------------------------------------
{
    return QGraphicsSimpleTextItem::text();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabel::setSelectionPoint(const SGraphObjSelectionPoint& i_selPt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_selPt.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setSelectionPoint",
        /* strAddInfo   */ strMthInArgs );

    if (m_selPt != i_selPt)
    {
        m_selPt = i_selPt;
        updatePosition();
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the type of the selection point.

    Selection points are differentiated into selection points on the bounding
    rectangle around the graphical object or into polygon shape points.
*/
SGraphObjSelectionPoint CGraphObjLabel::selectionPoint() const
//------------------------------------------------------------------------------
{
    return m_selPt;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabel::setDistanceToLinkedSelPt(const QSizeF& i_size)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qSize2Str(i_size);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setDistanceToLinkedSelPt",
        /* strAddInfo   */ strMthInArgs );

    if (m_distanceToLinkedSelPt != i_size) {
        m_distanceToLinkedSelPt = i_size;
        updatePosition();
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
QSizeF CGraphObjLabel::distanceToLinkedSelPt() const
//------------------------------------------------------------------------------
{
    return m_distanceToLinkedSelPt;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

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
        if (scene() != nullptr) {
            QRectF rctBounding = boundingRect();
            rctBounding = mapToScene(rctBounding).boundingRect();
            scene()->update(rctBounding);
        }
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
        if (scene() != nullptr) {
            QRectF rctBounding = boundingRect();
            rctBounding = mapToScene(rctBounding).boundingRect();
            m_bShowAnchorLine = false;
            scene()->update(rctBounding);
        }
        else {
            m_bShowAnchorLine = false;
        }
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
bool CGraphObjLabel::isAnchorLineVisible() const
//------------------------------------------------------------------------------
{
    return m_bShowAnchorLine;
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
void CGraphObjLabel::onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld)
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
        CEnumTextEffect textEffect = m_drawSettings.getTextEffect();
        ETextSize textSize = m_drawSettings.getTextSize();
        CEnumTextStyle textStyle = m_drawSettings.getTextStyle();
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

        QRectF rctUpd = QGraphicsSimpleTextItem::boundingRect();
        QPolygonF plgLabel = mapToScene(rctUpd);
        QRectF rctGraphObj = QRectF(QPointF(0.0, 0.0), m_pGraphObjParent->getSize(Units.Length.px).toQSizeF());
        QPolygonF plgGraphObj = dynamic_cast<QGraphicsItem*>(m_pGraphObjParent)->mapToScene(rctGraphObj);

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

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    if (bIsHit && o_pHitInfo != nullptr) {
        double fRotAngleCurr_deg = m_fRotAngleCurr_deg;
        if (m_pGraphObjParent != nullptr) {
            fRotAngleCurr_deg = m_pGraphObjParent->getRotationAngleInDegree();
        }
        o_pHitInfo->setCursor( Math::deg2Rad(fRotAngleCurr_deg) );
    }
#endif

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
public: // must overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the bounding rectangle for the label.

    This method is called by the graphics scene to detect the area to be updated
    and for some other reasons (dispatching mouse events, ...).

    If the label is hit, selected or if the anchor line to the linked grapical
    object is visible the area to be updated on changing the items graphical
    representation also includes the anchor line.

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

    QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();

    // If the object is hit and the anchor line is visible also this area need to be updated.
    if (m_bIsHit || isSelected() || m_bShowAnchorLine) {
        QRectF rctBoundingAnchorLine(m_anchorLine.p1(), m_anchorLine.p2());
        rctBounding |= rctBoundingAnchorLine;
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthReturn =
            "CenterPos {" + point2Str(rctBounding.center()) + "}" +
            ", Rect {" + qRect2Str(rctBounding) + "}";
        mthTracer.setMethodReturn(strMthReturn);
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
void CGraphObjLabel::paint(
    QPainter* i_pPainter,
    const QStyleOptionGraphicsItem* i_pStyleOption,
    QWidget* i_pWdgt )
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
    if (m_bIsHit || isSelected()) {
        pn.setColor(Qt::blue);
        pn.setStyle(Qt::DotLine);
        i_pPainter->setPen(pn);
        i_pPainter->drawRect(rct);
    }
    else if (text().isEmpty()) {
        pn.setColor(Qt::black);
        pn.setStyle(Qt::DotLine);
        i_pPainter->setPen(pn);
        i_pPainter->drawRect(rct);
    }

    // Draw anchor line to selection point of linked object if the label is hit by
    // mouse move (hover) or if the label is selected.
    // If the anchor line is set to be visible draw the anchor line in solid style.
    if (m_bIsHit || isSelected() || m_bShowAnchorLine) {
        // P1 of the anchor line is on one of the center points of the
        // bounding rectangle of this label.
        // P2 is on the selection point of the linked parent item.
        // If P2 is within the bounding rectangle or very close to the label
        // (length of line < 5.0 pixels) the anchor line will not be drawn.
        QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();
        if (!rctBounding.contains(m_anchorLine.p2())) {
            if ((fabs(m_anchorLine.dx()) >= 5.0) || (fabs(m_anchorLine.dy() >= 5.0))) {
                if (m_bIsHit || isSelected()) {
                    pn.setColor(Qt::blue);
                }
                else {
                    pn.setColor(Qt::gray);
                }
                pn.setStyle(Qt::DotLine);
                i_pPainter->setPen(pn);
                i_pPainter->drawLine(m_anchorLine);
            }
        }
    }

    i_pPainter->restore();

    QStyleOptionGraphicsItem styleOption = *i_pStyleOption;

    styleOption.state &= ~QStyle::State_Selected;
    styleOption.state &= ~QStyle::State_HasFocus;

    QGraphicsSimpleTextItem::paint(i_pPainter, &styleOption, i_pWdgt);

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
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
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
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
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
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
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
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
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
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvents,
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
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
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
    // This is not necessary here.
    //QGraphicsSimpleTextItem::mouseDoubleClickEvent(i_pEv);

    i_pEv->accept();

} // mouseDoubleClickEvent

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabel::onGraphObjParentGeometryChanged(CGraphObj* i_pGraphObjParent)
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

    updatePosition();
}

//------------------------------------------------------------------------------
void CGraphObjLabel::onGraphObjParentZValueChanged(CGraphObj* i_pGraphObjParent)
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
        /* strMethod    */ "onGraphObjParentZValueChanged",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItemParent = dynamic_cast<QGraphicsItem*>(m_pGraphObjParent);
    setZValue(pGraphicsItemParent->zValue() + 0.1);
}

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
        strMthInArgs = qGraphicsItemChange2Str(i_change, i_value);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "itemChange",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter,
            m_pTrcAdminObjItemChange->getRuntimeInfoTraceDetailLevel());
    }

    QVariant valChanged = i_value;

    bool bTreeEntryChanged = false;

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if (i_change == ItemSceneHasChanged) {
        // The item may have been removed from the scene.
        if (scene() != nullptr) {
            updatePosition();
        }
    }
    else if (i_change == ItemPositionHasChanged) {
        updateDistanceToLinkedSelPt();
        updateAnchorLine();
    }

    if (bTreeEntryChanged && m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }

    valChanged = QGraphicsItem::itemChange(i_change, i_value);

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave,
            m_pTrcAdminObjItemChange->getRuntimeInfoTraceDetailLevel());
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthRet = qGraphicsItemChange2Str(i_change, valChanged, false);
        mthTracer.setMethodReturn(strMthRet);
    }
    return valChanged;
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the position of the label and the
           coordinates (start and end point) of the anchor line.

    On moving the label the current distance to the parent item is stored.
    If the geometry of the parent item changes (position moved or size changed or
    any other geometry change) the label must be moved so that the distance remains
    the same.
*/
void CGraphObjLabel::updatePosition()
//------------------------------------------------------------------------------
{
    // "setPos" leads to a "itemChange" call with ItemPositionChange(d) whereupon
    // "updatePosition" would be called again. This reentry is not desired.
    if (m_bUpdatePositionInProgress) {
        return;
    }
    m_bUpdatePositionInProgress = true;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updatePosition",
        /* strAddInfo   */ "" );
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter,
            m_pTrcAdminObjItemChange->getRuntimeInfoTraceDetailLevel());
    }

    CPhysValPoint physValSelPointParent = m_pGraphObjParent->getSelectionPointCoors(m_selPt);
    QPointF ptScenePosThis = m_pDrawingScene->convert(physValSelPointParent, Units.Length.px).toQPointF();
    QRectF rctBoundingThis = QGraphicsSimpleTextItem::boundingRect();
    QPointF ptCenterThis = rctBoundingThis.center();

    // As default the labels bottom left point is on the parent items selection point.
    // The position of the text item on the scene is the top left corner. So we
    // got to move the item correspondingly.
    ptScenePosThis.setX(ptScenePosThis.x() - rctBoundingThis.width()/2.0);
    ptScenePosThis.setY(ptScenePosThis.y() - rctBoundingThis.height()/2.0);

    // Move the label keeping the distance.
    ptScenePosThis.setX(ptScenePosThis.x() + m_distanceToLinkedSelPt.width());
    ptScenePosThis.setY(ptScenePosThis.y() + m_distanceToLinkedSelPt.height());

    setPos(ptScenePosThis);

    // Update coordinates of the anchor line.
    updateAnchorLine();

    m_bUpdatePositionInProgress = false;

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave,
            m_pTrcAdminObjItemChange->getRuntimeInfoTraceDetailLevel());
    }
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the distance between the labels center
           point and the selection point of the parent item the label is linked to.

    On moving the label the current distance to the parent item is stored.
    If the geometry of the parent item changes (position moved or size changed or
    any other geometry change) the label must be moved so that the distance remains
    the same.
*/
void CGraphObjLabel::updateDistanceToLinkedSelPt()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateDistanceToLinkedSelPt",
        /* strAddInfo   */ "" );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    CPhysValPoint physValSelPointParent = m_pGraphObjParent->getSelectionPointCoors(m_selPt);
    QPointF ptSelScenePosParent = m_pDrawingScene->convert(physValSelPointParent, Units.Length.px).toQPointF();

    QRectF rctBoundingThis = QGraphicsSimpleTextItem::boundingRect();
    QPointF ptCenterThis = rctBoundingThis.center();
    QPointF ptScenePosCenterThis = mapToScene(ptCenterThis);

    m_distanceToLinkedSelPt.setWidth(ptScenePosCenterThis.x() - ptSelScenePosParent.x());
    m_distanceToLinkedSelPt.setHeight(ptScenePosCenterThis.y() - ptSelScenePosParent.y());
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the coordinates (start and end point)
           of the anchor line.

    The anchor line will be drawn to one of the center points at the bounding rectangle.
    Which line of the labels bounding rectangle should be used depends on the relative
    position of the label to the selection point of the graph object the label is linked to.

    "QLineF::angle" is used to calculate the angle of the line between the two anchor points.

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
//------------------------------------------------------------------------------
void CGraphObjLabel::updateAnchorLine()
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateAnchorLine",
        /* strAddInfo   */ "" );

    CPhysValPoint physValSelPointParent = m_pGraphObjParent->getSelectionPointCoors(m_selPt);
    QPointF ptSelScenePosParent = m_pDrawingScene->convert(physValSelPointParent, Units.Length.px).toQPointF();
    QRectF rctBoundingThis = QGraphicsSimpleTextItem::boundingRect();
    QPointF ptCenterThis = rctBoundingThis.center();

    m_anchorLine.setP1(mapToScene(ptCenterThis));
    m_anchorLine.setP2(ptSelScenePosParent);
    double fAngle = m_anchorLine.angle();

    // Map anchor line to local coordinates.
    if (fAngle >= 45.0 && fAngle <= 135.0) {
        m_anchorLine.setP1(
            ZS::Draw::getSelectionPointCoors(rctBoundingThis, ESelectionPoint::TopCenter));
    }
    else if (fAngle >= 225.0 && fAngle <= 315.0) {
        m_anchorLine.setP1(
            ZS::Draw::getSelectionPointCoors(rctBoundingThis, ESelectionPoint::BottomCenter));
    }
    else if (fAngle > 135.0 && fAngle < 225.0) {
        m_anchorLine.setP1(
            ZS::Draw::getSelectionPointCoors(rctBoundingThis, ESelectionPoint::LeftCenter));
    }
    else if ((fAngle > 315.0 && fAngle <= 360.0) || (fAngle >= 0.0 && fAngle < 45.0)) {
        m_anchorLine.setP1(
            ZS::Draw::getSelectionPointCoors(rctBoundingThis, ESelectionPoint::RightCenter));
    }
    m_anchorLine.setP2(mapFromScene(ptSelScenePosParent));
}

/*==============================================================================
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabel::traceInternalStates(
    CMethodTracer& i_mthTracer,
    EMethodDir i_mthDir,
    ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (i_mthTracer.isRuntimeInfoActive(i_detailLevel)) {
        QString strTrcInfo;
        if (i_mthDir == EMethodDir::Enter) strTrcInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strTrcInfo = "+- ";
        else strTrcInfo = "";
        strTrcInfo +=
            "Key: " + m_strKey + ", Parent: " + m_pGraphObjParent->path() +
            ", LinkedTo: " + m_selPt.toString() +
            ", Visible: " + QString(isVisible() ? "true" : "false") +
            ", ShowAnchorLine: " + QString(m_bShowAnchorLine ? "true" : "false");
        i_mthTracer.trace(strTrcInfo);
        if (i_mthDir == EMethodDir::Enter) strTrcInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strTrcInfo = "+- ";
        else strTrcInfo = "";
        strTrcInfo +=
            "DistanceToParent {" + qSize2Str(m_distanceToLinkedSelPt) + "}" +
            ", AnchorLine {" + qLine2Str(m_anchorLine) + "}";
        i_mthTracer.trace(strTrcInfo);
        i_mthTracer.trace(strTrcInfo);
        CGraphObj::traceInternalStates(i_mthTracer, i_mthDir, i_detailLevel);
    }
}
