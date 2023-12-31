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
    m_labelDscr(EGraphObjTypeLabel, i_strKey, i_selPt),
    m_anchorLines(),
    m_bUpdatePositionInProgress(false),
    m_bPositionUpdateOnParentGeometryChanged(false)
{
    createTraceAdminObjs("Labels::" + ClassName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Key: " + i_strKey + ", Text: " + i_strText + ", SelPt: " + i_selPt.toString(true);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    if (i_selPt.m_pGraphObj == nullptr) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges
           | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton | Qt::MiddleButton | Qt::XButton1 | Qt::XButton2);
    setAcceptHoverEvents(true);

    double fZValueParent = i_selPt.m_pGraphObj->getStackingOrderValue(ERowVersion::Original);
    setStackingOrderValue(fZValueParent - 0.1, ERowVersion::Original);

    QObject::connect(
        m_labelDscr.m_selPt1.m_pGraphObj, &CGraphObj::geometryChanged,
        this, &CGraphObjLabel::onGraphObjParentGeometryChanged);
    QObject::connect(
        m_labelDscr.m_selPt1.m_pGraphObj, &CGraphObj::zValueChanged,
        this, &CGraphObjLabel::onGraphObjParentZValueChanged);

} // ctor

/*==============================================================================
protected: // ctor (used by derived classes, e.g. CGraphObjLabelGeometryPosition)
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjLabel::CGraphObjLabel(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strKey,
    const QString& i_strText,
    EGraphObjType i_labelType,
    const SGraphObjSelectionPoint& i_selPt) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ i_labelType,
        /* strType             */ ZS::Draw::graphObjType2Str(i_labelType),
        /* strObjName          */ i_strKey,
        /* idxTreeEntryType    */ EEntryType::Leave ),
    QGraphicsSimpleTextItem(i_strText),
    m_labelDscr(i_labelType, i_strKey, i_selPt),
    m_anchorLines(),
    m_bUpdatePositionInProgress(false),
    m_bPositionUpdateOnParentGeometryChanged(false)
{
    if (i_selPt.m_pGraphObj == nullptr) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges
           | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton | Qt::MiddleButton | Qt::XButton1 | Qt::XButton2);
    setAcceptHoverEvents(true);

    double fZValueParent = i_selPt.m_pGraphObj->getStackingOrderValue(ERowVersion::Original);
    setStackingOrderValue(fZValueParent - 0.1, ERowVersion::Original);

    QObject::connect(
        m_labelDscr.m_selPt1.m_pGraphObj, &CGraphObj::geometryChanged,
        this, &CGraphObjLabel::onGraphObjParentGeometryChanged);
    QObject::connect(
        m_labelDscr.m_selPt1.m_pGraphObj, &CGraphObj::zValueChanged,
        this, &CGraphObjLabel::onGraphObjParentZValueChanged);
}

//------------------------------------------------------------------------------
CGraphObjLabel::CGraphObjLabel(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strKey,
    const QString& i_strText,
    EGraphObjType i_labelType,
    const SGraphObjSelectionPoint& i_selPt1,
    const SGraphObjSelectionPoint& i_selPt2) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ i_labelType,
        /* strType             */ ZS::Draw::graphObjType2Str(i_labelType),
        /* strObjName          */ i_strKey,
        /* idxTreeEntryType    */ EEntryType::Leave ),
    QGraphicsSimpleTextItem(i_strText),
    m_labelDscr(i_labelType, i_strKey, i_selPt1, i_selPt2),
    m_anchorLines(),
    m_bUpdatePositionInProgress(false),
    m_bPositionUpdateOnParentGeometryChanged(false)
{
    if (i_selPt1.m_pGraphObj == nullptr || i_selPt2.m_pGraphObj == nullptr) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges
           | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton | Qt::MiddleButton | Qt::XButton1 | Qt::XButton2);
    setAcceptHoverEvents(true);

    double fZValueParent = i_selPt1.m_pGraphObj->getStackingOrderValue(ERowVersion::Original);
    setStackingOrderValue(fZValueParent - 0.1, ERowVersion::Original);

    QObject::connect(
        m_labelDscr.m_selPt1.m_pGraphObj, &CGraphObj::geometryChanged,
        this, &CGraphObjLabel::onGraphObjParentGeometryChanged);
    QObject::connect(
        m_labelDscr.m_selPt1.m_pGraphObj, &CGraphObj::zValueChanged,
        this, &CGraphObjLabel::onGraphObjParentZValueChanged);
    QObject::connect(
        m_labelDscr.m_selPt2.m_pGraphObj, &CGraphObj::geometryChanged,
        this, &CGraphObjLabel::onGraphObjParentGeometryChanged);
    QObject::connect(
        m_labelDscr.m_selPt2.m_pGraphObj, &CGraphObj::zValueChanged,
        this, &CGraphObjLabel::onGraphObjParentZValueChanged);
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
    if (m_pDrawingScene != nullptr) {
        m_pDrawingScene->removeItem(this);
    }

    //m_labelDscr;
    //m_distanceToLinkedSelPt;
    //m_anchorLine;
    m_bUpdatePositionInProgress = false;
    m_bPositionUpdateOnParentGeometryChanged = false;

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
    return EGraphObjTypeLabel;
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

    if (m_labelDscr.m_strKey != i_strKey) {
        m_labelDscr.m_strKey = i_strKey;
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
QString CGraphObjLabel::key() const
//------------------------------------------------------------------------------
{
    return m_labelDscr.m_strKey;
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

    if (m_labelDscr.m_strText != i_strText) {
        m_labelDscr.m_strText = i_strText;
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
void CGraphObjLabel::setSelectionPoint1(const SGraphObjSelectionPoint& i_selPt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_selPt.toString(true);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setSelectionPoint1",
        /* strAddInfo   */ strMthInArgs );

    if (m_labelDscr.m_selPt1 != i_selPt)
    {
        m_labelDscr.m_selPt1 = i_selPt;
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
SGraphObjSelectionPoint CGraphObjLabel::selectionPoint1() const
//------------------------------------------------------------------------------
{
    return m_labelDscr.m_selPt1;
}

//------------------------------------------------------------------------------
void CGraphObjLabel::setSelectionPoint2(const SGraphObjSelectionPoint& i_selPt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_selPt.toString(true);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setSelectionPoint2",
        /* strAddInfo   */ strMthInArgs );

    if (m_labelDscr.m_selPt2 != i_selPt)
    {
        m_labelDscr.m_selPt2 = i_selPt;
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
SGraphObjSelectionPoint CGraphObjLabel::selectionPoint2() const
//------------------------------------------------------------------------------
{
    return m_labelDscr.m_selPt2;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLabel::setPolarCoorsToLinkedSelectionPoint(const SPolarCoors& i_polarCoors)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_polarCoors.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setPolarCoorsToLinkedSelectionPoint",
        /* strAddInfo   */ strMthInArgs );

    if (m_labelDscr.m_polarCoorsToLinkedSelPt != i_polarCoors) {
        m_labelDscr.m_polarCoorsToLinkedSelPt = i_polarCoors;
        updatePosition();
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
SPolarCoors CGraphObjLabel::polarCoorsToLinkedSelectionPoint() const
//------------------------------------------------------------------------------
{
    return m_labelDscr.m_polarCoorsToLinkedSelPt;
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

    if (!m_labelDscr.m_bShowAnchorLine) {
        m_labelDscr.m_bShowAnchorLine = true;
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

    if (m_labelDscr.m_bShowAnchorLine) {
        if (scene() != nullptr) {
            QRectF rctBounding = boundingRect();
            rctBounding = mapToScene(rctBounding).boundingRect();
            m_labelDscr.m_bShowAnchorLine = false;
            scene()->update(rctBounding);
        }
        else {
            m_labelDscr.m_bShowAnchorLine = false;
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
    return m_labelDscr.m_bShowAnchorLine;
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjLabel::getScenePolygonShapePointsString() const
//------------------------------------------------------------------------------
{
    QRectF rct = getCurrentBoundingRect();
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

////------------------------------------------------------------------------------
//void CGraphObjLabel::setIsHit( bool i_bHit )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = bool2Str(i_bHit);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "setIsHit",
//        /* strAddInfo   */ strMthInArgs );
//
//    if (m_bIsHit != i_bHit)
//    {
//        m_bIsHit = i_bHit;
//
//        update();
//
//        QRectF rctUpd = QGraphicsSimpleTextItem::boundingRect();
//        QPolygonF plgLabel = mapToScene(rctUpd);
//        QRectF rctGraphObj = QRectF(QPointF(0.0, 0.0), m_labelDscr.m_selPt1.m_pGraphObj->getSize(Units.Length.px).toQSizeF());
//        QPolygonF plgGraphObj = dynamic_cast<QGraphicsItem*>(m_labelDscr.m_selPt1.m_pGraphObj)->mapToScene(rctGraphObj);
//
//        rctUpd = plgLabel.boundingRect();
//        rctGraphObj = plgGraphObj.boundingRect();
//        rctUpd |= rctGraphObj;
//
//        m_pDrawingScene->update(rctUpd);
//    }
//}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
//bool CGraphObjLabel::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjIsHit, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs =
//            "Point:" + point2Str(i_pt) +
//            ", HitInfo, " + QString(o_pHitInfo == nullptr ? "null" : pointer2Str(o_pHitInfo));
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjIsHit,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "isHit",
//        /* strAddInfo   */ strMthInArgs );
//
//    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
//        QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();
//        mthTracer.trace("BoundingRect {" + qRect2Str(rctBounding) + "}");
//    }
//
//    bool bIsHit = false;
//
//    QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();
//
//    bIsHit = isRectHit( rctBounding, EFillStyle::SolidPattern, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo );
//
//    if (bIsHit && o_pHitInfo != nullptr) {
//        //o_pHitInfo->m_editMode = EEditMode::Move;
//        //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
//        o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
//        o_pHitInfo->m_idxPolygonShapePoint = -1;
//        o_pHitInfo->m_idxLineSegment = -1;
//        o_pHitInfo->m_ptSelected = rctBounding.center();
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
//void CGraphObjLabel::setCursor( const QCursor& i_cursor )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = qCursorShape2Str(i_cursor.shape());
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "setCursor",
//        /* strAddInfo   */ strMthInArgs );
//
//    QGraphicsSimpleTextItem::setCursor(i_cursor);
//}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the bounding rectangle of the object.
*/
QRectF CGraphObjLabel::getCurrentBoundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "getCurrentBoundingRect",
        /* strAddInfo   */ "" );

    QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
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
    if (m_bIsHit || isSelected() || m_labelDscr.m_bShowAnchorLine) {
        for (const QLineF& anchorLine : m_anchorLines) {
            QRectF rctBoundingAnchorLine(anchorLine.p1(), anchorLine.p2());
            rctBounding |= rctBoundingAnchorLine;
        }
    }
    rctBounding = QRectF(
        rctBounding.left() - m_drawSettings.getPenWidth()/2,
        rctBounding.top() - m_drawSettings.getPenWidth()/2,
        rctBounding.width() + m_drawSettings.getPenWidth(),
        rctBounding.height() + m_drawSettings.getPenWidth() );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
/*! @brief Called internally by QGraphicsItem::boundingRect.
*/
QPainterPath CGraphObjLabel::shape() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "shape",
        /* strAddInfo   */ "" );

    QPainterPath painterPath = QGraphicsSimpleTextItem::shape();
    if (m_bIsHit || isSelected() || m_labelDscr.m_bShowAnchorLine) {
        for (const QLineF& anchorLine : m_anchorLines) {
            painterPath.addPolygon(ZS::Draw::line2Polygon(anchorLine));
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        const QGraphicsItem* pCThis = static_cast<const QGraphicsItem*>(this);
        QGraphicsItem* pVThis = const_cast<QGraphicsItem*>(pCThis);
        QString strMthRet = qPainterPath2Str(pVThis, painterPath);
        mthTracer.setMethodReturn(strMthRet);
    }
    return painterPath;
}

//------------------------------------------------------------------------------
void CGraphObjLabel::paint(
    QPainter* i_pPainter,
    const QStyleOptionGraphicsItem* i_pStyleOption,
    QWidget* i_pWdgt )
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
    i_pPainter->setRenderHint(QPainter::Antialiasing);

    QRectF rct = getCurrentBoundingRect();

    // Draw bounding rectangle in dotted line style if the label is hit by
    // mouse move (hover) or if the label is selected or if no text is assigned.
    QPen pn(Qt::SolidLine);
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

    //#pragma message(__TODO__"To be removed")
    //pn.setColor(Qt::black);
    //pn.setStyle(Qt::DotLine);
    //i_pPainter->setPen(pn);
    //i_pPainter->drawRect(rct);
    //QLineF lineCenterDiag1(rct.topLeft(), rct.bottomRight());
    //QLineF lineCenterDiag2(rct.bottomLeft(), rct.topRight());
    //i_pPainter->drawLine(lineCenterDiag1);
    //i_pPainter->drawLine(lineCenterDiag2);
    //#pragma message(__TODO__"To be removed")

    // Draw anchor line to selection point of linked object if the label is hit,
    // hovered, selected or if the anchor line is set to be visible.
    // Use a different color if the line is hit, hovered or selected.
    if (/*m_bIsHit ||*/ isSelected() || m_labelDscr.m_bShowAnchorLine) {
        // P1 of the anchor line is on one of the center points of the
        // bounding rectangle of this label.
        // P2 is on the selection point of the linked parent item.
        // If P2 is within the bounding rectangle or very close to the label
        // (length of line < 5.0 pixels) the anchor line will not be drawn.
        QRectF rctBounding = QGraphicsSimpleTextItem::boundingRect();
        for (const QLineF& anchorLine : m_anchorLines) {
            if (!rctBounding.contains(anchorLine.p2())) {
                if ((fabs(anchorLine.dx()) >= 5.0) || (fabs(anchorLine.dy()) >= 5.0)) {
                    QColor color = /*m_bIsHit ||*/ isSelected() ? Qt::blue : Qt::lightGray;
                    color.setAlpha(192);
                    pn.setColor(color);
                    i_pPainter->setPen(pn);
                    i_pPainter->drawLine(anchorLine);
                }
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

    setIsHit(true);
    setCursor(getProposedCursor(i_pEv->pos()));
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

    setIsHit(true);
    setCursor(getProposedCursor(i_pEv->pos()));
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

    setIsHit(false);
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
}

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
}

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

    // If the position is updated because the parent's geometry is changed,
    // the relative distance in polar coordinates (length and angle) to the
    // linked selection point must not be changed.
    m_bPositionUpdateOnParentGeometryChanged = true;
    updatePosition();
    m_bPositionUpdateOnParentGeometryChanged = false;
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

    // The labels anchor line should be drawn before the parent object is drawn.
    // Otherwise the anchor lines may cover the painting of the parent object.
    double fZValueParent = m_labelDscr.m_selPt1.m_pGraphObj->getStackingOrderValue();
    setStackingOrderValue(fZValueParent - 0.1);
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

    QVariant valChanged = i_value;

    bool bTreeEntryChanged = false;

    if (i_change == ItemSceneHasChanged) {
        // The item may have been removed from the scene.
        if (scene() != nullptr) {
            updatePosition();
        }
    }
    else if (i_change == ItemPositionHasChanged) {
        // If the position is updated because the parent's geometry is changed,
        // the relative distance in polar coordinates (length and angle) to the
        // linked selection point must not be changed.
        if (!m_bPositionUpdateOnParentGeometryChanged) {
            updatePolarCoorsToLinkedSelPt();
        }
        updateAnchorLines();
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
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the position of the label and the
           coordinates (start and end point) of the anchor line.

    If the geometry of the parent item changes (position moved or size changed or
    any other geometry change) the label must be moved so that the relative position
    (distance and angle) of the label to the selection point remains the same.
*/
void CGraphObjLabel::updatePosition()
//------------------------------------------------------------------------------
{
    // "setPos" leads to a "itemChange" call with ItemPositionChange(d) whereupon
    // "updatePosition" would be called again. This reentry is not desired.
    if (m_bUpdatePositionInProgress) {
        return;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updatePosition",
        /* strAddInfo   */ "" );

    m_bUpdatePositionInProgress = true;

    // Get anchor line in scene coordinates.
    QLineF anchorLine;
    if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::BoundingRectangle) {
        anchorLine = m_labelDscr.m_selPt1.m_pGraphObj->getAnchorLineToSelectionPointFromPolarInSceneCoors(
            m_labelDscr.m_polarCoorsToLinkedSelPt, m_labelDscr.m_selPt1.m_selPt);
    }
    else if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::PolygonShapePoint) {
        anchorLine = m_labelDscr.m_selPt1.m_pGraphObj->getAnchorLineToSelectionPointFromPolarInSceneCoors(
            m_labelDscr.m_polarCoorsToLinkedSelPt, m_labelDscr.m_selPt1.m_idxPt);
    }

    // The position of a QGraphicsTextItem is defined by its top left corner.
    // Move text item so that its center point is at the line end point of the anchor line.
    QRectF rctBoundingThis = getCurrentBoundingRect();
    QPointF anchorLineP2ScenePos = anchorLine.p2() - rctBoundingThis.center();
    setPos(anchorLineP2ScenePos);

    // Please note that on calling setPos the itemChange method of the
    // label is called invoking updateAnchorLines.

    m_bUpdatePositionInProgress = false;
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the relative position in polar
           coordinates (length in pixels, angle in degress) from the labels center
           point and the selection point the label is linked to.

    On moving the label the distance and the angle (polar coordinates) to the
    selection point have to be updated and saved.

    When positioniong labels the distance to selection point the label
    is linked to should remain the same.
    This is managed by defining the length and the angle to the line
    the selection point is positioned.

    Defining the length to the selection point with a relative size
    as a scale factor will not lead to the desired result. If the
    object of the selection point is resized the distance between the
    label and the object may increase to an undesired value (far away
    from the object or to close).
*/
void CGraphObjLabel::updatePolarCoorsToLinkedSelPt()
//------------------------------------------------------------------------------
{
    // If the position is updated because the parent's geometry is changed,
    // the relative distance in polar coordinates (length and angle) to the
    // linked selection point must not be changed.
    if (m_bPositionUpdateOnParentGeometryChanged) {
        return;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updatePolarCoorsToLinkedSelPt",
        /* strAddInfo   */ "" );

    QRectF rctBoundingThis = getCurrentBoundingRect();
    QPointF ptCenterThis = rctBoundingThis.center();
    QPointF ptScenePosCenterThis = mapToScene(ptCenterThis);

    if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::BoundingRectangle) {
        m_labelDscr.m_polarCoorsToLinkedSelPt = m_labelDscr.m_selPt1.m_pGraphObj->getPolarCoorsToSelectionPointFromSceneCoors(
            ptScenePosCenterThis, m_labelDscr.m_selPt1.m_selPt);
    }
    else if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::PolygonShapePoint) {
        m_labelDscr.m_polarCoorsToLinkedSelPt = m_labelDscr.m_selPt1.m_pGraphObj->getPolarCoorsToSelectionPointFromSceneCoors(
            ptScenePosCenterThis, m_labelDscr.m_selPt1.m_idxPt);
    }

    // Update coordinates of the anchor line.
    updateAnchorLines();
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to update the coordinates (start and end point)
           of the anchor lines.

    The anchor lines will be drawn to one of the selection points at the bounding rectangle
    or to one of the polygon shape points of the linked graphical object.
*/
void CGraphObjLabel::updateAnchorLines()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateAnchorLines",
        /* strAddInfo   */ "" );

    QPointF ptSelScenePosParent;
    if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::BoundingRectangle) {
        ptSelScenePosParent = m_labelDscr.m_selPt1.m_pGraphObj->getSelectionPointCoorsInSceneCoors(m_labelDscr.m_selPt1.m_selPt);
    }
    else if (m_labelDscr.m_selPt1.m_selPtType == ESelectionPointType::PolygonShapePoint) {
        ptSelScenePosParent = m_labelDscr.m_selPt1.m_pGraphObj->getSelectionPointCoorsInSceneCoors(m_labelDscr.m_selPt1.m_idxPt);
    }

    QRectF rctBoundingThis = getCurrentBoundingRect();
    QPointF ptCenterThis = rctBoundingThis.center();
    QLineF anchorLine(ptCenterThis, mapFromScene(ptSelScenePosParent));

    if (m_anchorLines.isEmpty()) {
        m_anchorLines.append(anchorLine);
    }
    else {
        m_anchorLines[0] = anchorLine;
    }
}

/*==============================================================================
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObjLabel::traceInternalStates(
//    CMethodTracer& i_mthTracer,
//    EMethodDir i_mthDir,
//    ELogDetailLevel i_detailLevel) const
////------------------------------------------------------------------------------
//{
//    if (i_mthTracer.isRuntimeInfoActive(i_detailLevel)) {
//        QString strTrcInfo;
//        if (i_mthDir == EMethodDir::Enter) strTrcInfo = "-+ ";
//        else if (i_mthDir == EMethodDir::Leave) strTrcInfo = "+- ";
//        else strTrcInfo = "";
//        strTrcInfo +=
//            "Key: " + m_labelDscr.m_strKey +
//            ", LinkedTo {" + m_labelDscr.m_selPt1.toString(true) + ", " + m_labelDscr.m_selPt2.toString(true) + "}" +
//            ", Visible: " + QString(isVisible() ? "true" : "false") +
//            ", ShowAnchorLine: " + QString(m_labelDscr.m_bShowAnchorLine ? "true" : "false");
//        i_mthTracer.trace(strTrcInfo);
//        if (i_mthDir == EMethodDir::Enter) strTrcInfo = "-+ ";
//        else if (i_mthDir == EMethodDir::Leave) strTrcInfo = "+- ";
//        else strTrcInfo = "";
//        strTrcInfo +=
//            "PolarCoorsToLinkedSelPt {" + m_labelDscr.m_polarCoorsToLinkedSelPt.toString() + "}" +
//            ", AnchorLines [" + QString::number(m_anchorLines.size()) + "]";
//        if (m_anchorLines.size() > 0) {
//            strTrcInfo += "(";
//            for (const QLineF& anchorLine : m_anchorLines) {
//                if (!strTrcInfo.endsWith("(")) strTrcInfo += ", ";
//                strTrcInfo += "{" + qLine2Str(anchorLine) + "}";
//            }
//            strTrcInfo += ")";
//        }
//        i_mthTracer.trace(strTrcInfo);
//        i_mthTracer.trace(strTrcInfo);
//        CGraphObj::traceInternalStates(i_mthTracer, i_mthDir, i_detailLevel);
//    }
//}
