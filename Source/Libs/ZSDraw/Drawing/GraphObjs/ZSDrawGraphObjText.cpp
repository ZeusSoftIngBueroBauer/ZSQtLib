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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjText.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
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
using namespace ZS::System::GUI;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


/*******************************************************************************
class CGraphObjText : public CGraphObj, public QGraphicsTextitem
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjText::s_iInstCount = 0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjText::CGraphObjText(
    CDrawingScene* i_pDrawingScene, const QString& i_strObjName) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ EGraphObjTypeText,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeText),
        /* strObjName          */ i_strObjName.isEmpty() ? "Text" + QString::number(s_iInstCount) : i_strObjName),
    QGraphicsTextItem()
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
        /* strObjName   */ path(),
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    QRectF rctBounding = QGraphicsTextItem::boundingRect();

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    m_ptRotOriginCurr = rctBounding.center();
#endif

    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemSendsGeometryChanges);

    setTextInteractionFlags(Qt::TextEditorInteraction);

    //onDrawSettingsChanged();

    //updateToolTip();

} // ctor

//------------------------------------------------------------------------------
CGraphObjText::~CGraphObjText()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    emit_aboutToBeDestroyed();

} // dtor

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the type method of QGraphicsItem.
*/
int CGraphObjText::type() const
//------------------------------------------------------------------------------
{
    return EGraphObjTypeText;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjText::clone()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal))
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "clone",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjText* pGraphObj = new CGraphObjText(m_pDrawingScene, m_strName);
    pGraphObj->setDrawSettings(m_drawSettings);

    pGraphObj->setPlainText( toPlainText() );
    pGraphObj->setPos( pos() );
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    pGraphObj->setRotationAngleInDegree(m_fRotAngleCurr_deg);
    //pGraphObj->setScaleFactors(m_fScaleFacXCurr,m_fScaleFacYCurr);
    pGraphObj->acceptCurrentAsOriginalCoors();
#endif

    return pGraphObj;

} // clone

/*==============================================================================
public: // replacing methods of QGraphicsTextItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjText::setHtml( const QString& i_strText )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = i_strText;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setHtml",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsTextItem::setHtml(i_strText);

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    m_rctCurr = QGraphicsTextItem::boundingRect();
#endif

} // setHtml

//------------------------------------------------------------------------------
void CGraphObjText::setPlainText( const QString& i_strText )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = i_strText;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPlainText",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsTextItem::setPlainText(i_strText);

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    m_rctCurr = QGraphicsTextItem::boundingRect();
#endif

} // setPlainText

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjText::rect() const
//------------------------------------------------------------------------------
{
    QRectF rctBounding = QGraphicsTextItem::boundingRect();

    return rctBounding;

} // rect

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjText::getScenePolygonShapePointsString() const
//------------------------------------------------------------------------------
{
    QString strScenePolygonShapePoints;
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    QPolygonF plgScene = mapToScene(m_rctCurr);
    strScenePolygonShapePoints = polygon2Str(plgScene);
#endif
    return strScenePolygonShapePoints;
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjText::onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "OldSettings {" + i_drawSettingsOld.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "onDrawSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_drawSettings.isTextUsed() )
    {
        QFont fnt = m_drawSettings.getTextFont();

        ETextSize textSize = m_drawSettings.getTextSize();

        fnt.setPixelSize( textSize2SizeInPixels(textSize) );

        CEnumTextStyle textStyle = m_drawSettings.getTextStyle();

        bool bItalic = (textStyle == ETextStyle::Italic || textStyle == ETextStyle::BoldItalic);
        bool bBold   = (textStyle == ETextStyle::Bold || textStyle == ETextStyle::BoldItalic);

        fnt.setItalic(bItalic);
        fnt.setBold(bBold);

        CEnumTextEffect textEffect = m_drawSettings.getTextEffect();

        bool bStrikeout = (textEffect == ETextEffect::Strikeout || textEffect == ETextEffect::StrikeoutUnderline);
        bool bUnderline = (textEffect == ETextEffect::Underline || textEffect == ETextEffect::StrikeoutUnderline);

        fnt.setStrikeOut(bStrikeout);
        fnt.setUnderline(bUnderline);

        setFont(fnt);

        setDefaultTextColor(m_drawSettings.getTextColor());

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
        m_rctCurr = QGraphicsTextItem::boundingRect();
#endif

    }

} // onDrawSettingsChanged

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObjText::setIsHit( bool i_bHit )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal))
//    {
//        strMthInArgs = "Hit:" + bool2Str(i_bHit);
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "setIsHit",
//        /* strAddInfo   */ strMthInArgs );
//
//    if( m_bIsHit != i_bHit )
//    {
//        m_bIsHit = i_bHit;
//
//        update();
//    }
//
//} // setIsHit

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
//bool CGraphObjText::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjIsHit, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "Point:" + point2Str(i_pt) +
//            ", HitInfo, " + QString(o_pHitInfo == nullptr ? "null" : pointer2Str(o_pHitInfo)) +
//            ", Rect(x,y,w,h):" + rect2Str(rect());
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjIsHit,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "isHit",
//        /* strAddInfo   */ strMthInArgs );
//
//    bool bIsHit = false;
//
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//
//    if (pGraphicsItem != nullptr) {
//        if (pGraphicsItem->isSelected()) {
//            bIsHit = isBoundingRectSelectionPointHit(
//                /* pt               */ i_pt,
//                /* iSelPtsCount     */ -1,
//                /* pSelPts          */ nullptr,
//                /* pGraphObjHitInfo */ o_pHitInfo );
//        }
//        if (!bIsHit) {
//            QRectF rct = rect();
//            bIsHit = isRectHit(rct, EFillStyle::None, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo);
//            if (bIsHit) {
//                // Text items cannot be resized by mouse cursor:
//                if (o_pHitInfo != nullptr/* && o_pHitInfo->m_editMode == EEditMode::Resize*/) {
//                    //o_pHitInfo->m_editMode = EEditMode::Move;
//                    //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
//                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
//                }
//            }
//            else {
//                bIsHit = isRectHit(rct, EFillStyle::SolidPattern, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo);
//                // If "inside" the text item ...
//                if (bIsHit) {
//                    if (o_pHitInfo != nullptr/*&& o_pHitInfo->m_editMode == EEditMode::Move*/) {
//                        //o_pHitInfo->m_editMode = EEditMode::EditText;
//                        //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
//                        o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
//                    }
//                }
//            }
//        }
//        if (!bIsHit) {
//            if (pGraphicsItem->isSelected() || m_drawSettings.getFillStyle() == EFillStyle::SolidPattern) {
//                bIsHit = pGraphicsItem->contains(i_pt);
//                if (o_pHitInfo != nullptr) {
//                    //o_pHitInfo->m_editMode = EEditMode::EditText;
//                    //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
//                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
//                    o_pHitInfo->m_idxPolygonShapePoint = -1;
//                    o_pHitInfo->m_idxLineSegment = -1;
//                    o_pHitInfo->m_ptSelected = i_pt;
//                }
//            }
//        }
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//        if( bIsHit && o_pHitInfo != nullptr )
//        {
//            o_pHitInfo->setCursor( Math::degree2Rad(m_fRotAngleCurr_deg) );
//        }
//#endif
//    } // if( pGraphicsItem != nullptr )
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
//void CGraphObjText::setCursor( const QCursor& i_cursor )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal))
//    {
//        strMthInArgs = qCursorShape2Str(i_cursor.shape());
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "setCursor",
//        /* strAddInfo   */ strMthInArgs );
//
//    QGraphicsTextItem::setCursor(i_cursor);
//
//} // setCursor

/*==============================================================================
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjText::showSelectionPoints(TSelectionPointTypes i_selPts)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + selectionPointTypes2Str(i_selPts) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "showSelectionPoints",
        /* strAddInfo   */ strMthInArgs );

    if (QGraphicsObject::parent() == nullptr) {
        unsigned char selPts = i_selPts & c_uSelectionPointsBoundingRectRotate;
        showSelectionPointsOfBoundingRect( rect(), selPts );
    }
}

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjText::boundingRect() const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjBoundingRect, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "TextWidth:" + QString::number(textWidth());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ strMthInArgs );

    QRectF rctBounding = QGraphicsTextItem::boundingRect();

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

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }

    return rctBounding;

} // boundingRect

//------------------------------------------------------------------------------
void CGraphObjText::paint(
    QPainter*                       i_pPainter,
    const QStyleOptionGraphicsItem* i_pStyleOption,
    QWidget*                        i_pWdgt )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjPaint, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ZValue: " + QString::number(zValue());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "paint",
        /* strAddInfo   */ strMthInArgs );

    i_pPainter->save();

    QPen pn;

    QRectF rct = QGraphicsTextItem::boundingRect();

    if( m_drawSettings.getFillStyle() != EFillStyle::None )
    {
        QBrush brsh;

        brsh.setColor( m_drawSettings.getFillColor() );
        brsh.setStyle( fillStyle2QtBrushStyle(m_drawSettings.getFillStyle()) );

        i_pPainter->setBrush(brsh);
    }

    if( m_pDrawingScene->getMode() == EMode::Edit )
    {
        if( /*m_bIsHit ||*/ isSelected() )
        {
            pn.setColor(Qt::blue);
            pn.setStyle(Qt::DotLine);

            i_pPainter->setPen(pn);
            i_pPainter->setBrush(Qt::NoBrush);

            i_pPainter->drawRect(rct);

            if( isSelected() )
            {
                if( m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateTop)] != nullptr )
                {
                    CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateTop)];

                    QPointF ptRot = QPointF( pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y() );

                    QPointF ptRctM = ZS::Draw::getSelectionPointCoors(rct,ESelectionPoint::TopCenter);
                    QPointF ptRotM = mapFromScene(ptRot);

                    i_pPainter->drawLine( ptRctM, ptRotM );
                }

                if( m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateBottom)] != nullptr )
                {
                    CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateBottom)];

                    QPointF ptRot = QPointF( pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y() );

                    QPointF ptRctM = ZS::Draw::getSelectionPointCoors(rct,ESelectionPoint::BottomCenter);
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
        } // if( m_bIsHit || isSelected() )

        else if( m_drawSettings.getLineStyle() != ELineStyle::NoLine )
        {
            pn.setColor( m_drawSettings.getPenColor() );
            pn.setWidth( m_drawSettings.getPenWidth() );
            pn.setStyle( lineStyle2QtPenStyle(m_drawSettings.getLineStyle()) );

            i_pPainter->setPen(pn);
            i_pPainter->drawRect(rct);
        }

        else if( toPlainText().isEmpty() )
        {
            pn.setColor(Qt::black);
            pn.setStyle(Qt::DotLine);

            i_pPainter->setPen(pn);
            i_pPainter->drawRect(rct);
        }
    } // if( m_pDrawingScene->getMode() == EMode::Edit )

    else // if( m_pDrawingScene->getMode() == EMode::View )
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

    // "QGraphicsTextItem::paint" calls "CGraphObjText::boundingRect" to draw the
    // highlighted focus rectangle around the text item. "CGraphObjText::boundingRect"
    // returns the rectangle including the rotation selection points. So we don't let
    // the base implementation paint the selection rectangle around the item.
    styleOption.state &= ~QStyle::State_Selected;
    styleOption.state &= ~QStyle::State_HasFocus;

    QGraphicsTextItem::paint(i_pPainter,&styleOption,i_pWdgt);

} // paint

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

////------------------------------------------------------------------------------
//bool CGraphObjText::sceneEvent( QEvent* i_pEv )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjSceneEvent, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = qEvent2Str(i_pEv);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjSceneEvent,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "sceneEvent",
//        /* strAddInfo   */ strMthInArgs );
//
//    bool bEventHandled = false;
//    // Method has been overwritten to allow setting a breakpoint for debugging purposes.
//    if (i_pEv->type() == QEvent::FocusOut) {
//        bEventHandled = QGraphicsTextItem::sceneEvent(i_pEv);
//    }
//    else {
//        bEventHandled = QGraphicsTextItem::sceneEvent(i_pEv);
//    }
//    return bEventHandled;
//}
//
////------------------------------------------------------------------------------
//bool CGraphObjText::sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv )
////------------------------------------------------------------------------------
//{
//    //--------------------------------------------------------------------
//    // Mouse events of selection points will be "watched" to forward those
//    // events to this item in order to resize and rotate this item.
//    //--------------------------------------------------------------------
//
//    CGraphObjSelectionPoint* pGraphObjSelPtWatched = dynamic_cast<CGraphObjSelectionPoint*>(i_pGraphicsItemWatched);
//    if (pGraphObjSelPtWatched == nullptr) {
//        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "pGraphObjSelPtWatched == nullptr");
//    }
//
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjSceneEventFilter, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "ItemWatched:" + pGraphObjSelPtWatched->name() + ", Event:" + qEventType2Str(i_pEv->type());
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjSceneEventFilter,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "sceneEventFilter",
//        /* strAddInfo   */ strMthInArgs );
//
//    bool bEventHandled = false;
//
//    if (i_pEv->type() == QEvent::GraphicsSceneMouseMove) {
//        QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
//        if (pEv != nullptr) {
//            QPointF ptEvPos;
//            for (int i = 0x1; i <= 0x10; i <<= 1) {
//                Qt::MouseButton button = Qt::MouseButton(i);
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
//                pEv->setButtonDownPos( button, ptEvPos );
//            }
//            ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
//            pEv->setPos(ptEvPos);
//            ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
//            pEv->setLastPos(ptEvPos);
//            mouseMoveEvent(pEv);
//            bEventHandled = true;
//        }
//    }
//    else if (i_pEv->type() == QEvent::GraphicsSceneMousePress) {
//        QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
//        if (pEv != nullptr) {
//            QPointF ptEvPos;
//            for (int i = 0x1; i <= 0x10; i <<= 1) {
//                Qt::MouseButton button = Qt::MouseButton(i);
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
//                pEv->setButtonDownPos( button, ptEvPos );
//            }
//            ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
//            pEv->setPos(ptEvPos);
//            ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
//            pEv->setLastPos(ptEvPos);
//            mousePressEvent(pEv);
//            bEventHandled = true;
//        }
//    }
//    else if (i_pEv->type() == QEvent::GraphicsSceneMouseRelease) {
//        QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
//        if (pEv != nullptr) {
//            QPointF ptEvPos;
//            for (int i = 0x1; i <= 0x10; i <<= 1) {
//                Qt::MouseButton button = Qt::MouseButton(i);
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
//                pEv->setButtonDownPos( button, ptEvPos );
//            }
//            ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
//            pEv->setPos(ptEvPos);
//            ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
//            pEv->setLastPos(ptEvPos);
//            mouseReleaseEvent(pEv);
//            bEventHandled = true;
//        }
//    }
//    else if (i_pEv->type() == QEvent::GraphicsSceneMouseDoubleClick) {
//        QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
//        if (pEv != nullptr) {
//            QPointF ptEvPos;
//            for (int i = 0x1; i <= 0x10; i <<= 1) {
//                Qt::MouseButton button = Qt::MouseButton(i);
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
//                pEv->setButtonDownPos( button, ptEvPos );
//            }
//            ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
//            pEv->setPos(ptEvPos);
//            ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
//            pEv->setLastPos(ptEvPos);
//            mouseDoubleClickEvent(pEv);
//            bEventHandled = true;
//        }
//    }
//    return bEventHandled;
//}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjText::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strMthInArgs );

    //QPointF ptItemPos = mapFromScene(i_pEv->scenePos());

    //CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
    //CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    //if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )
    //{
    //    SGraphObjHitInfo hitInfo;

    //    bool bIsHit = isHit(ptItemPos,&hitInfo);

    //    if( bIsHit )
    //    {
    //        if( cursor().shape() != hitInfo.m_cursor.shape() )
    //        {
    //            setCursor(hitInfo.m_cursor);
    //        }
    //    }
    //} // if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )
}

//------------------------------------------------------------------------------
void CGraphObjText::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    //QPointF ptItemPos = mapFromScene(i_pEv->scenePos());

    //CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
    //CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    //if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )
    //{
    //    SGraphObjHitInfo hitInfo;

    //    bool bIsHit = isHit(ptItemPos,&hitInfo);

    //    if( bIsHit )
    //    {
    //        if( cursor().shape() != hitInfo.m_cursor.shape() )
    //        {
    //            setCursor(hitInfo.m_cursor);
    //        }
    //    }
    //} // if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )

} // hoverMoveEvent

//------------------------------------------------------------------------------
void CGraphObjText::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverLeaveEvent",
        /* strAddInfo   */ strMthInArgs );

    unsetCursor();
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjText::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strMthInArgs );

//    CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
//    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();
//
//    if( modeDrawing == EMode::Edit )
//    {
//        if( editToolDrawing == EEditTool::CreateObjects && m_editMode == EEditMode::Creating )
//        {
//            QPointF ptEvItemPos = i_pEv->pos();
//            QPointF ptEvScenePos = i_pEv->scenePos();
//            QPoint  ptEvScreenPos = i_pEv->screenPos();
//            QRectF  rctBounding = QGraphicsTextItem::boundingRect();
//
//            double fXOffs = rctBounding.center().x() - ptEvItemPos.x();
//            double fYOffs = rctBounding.center().y() - ptEvItemPos.y();
//
//            ptEvItemPos = rctBounding.center();
//            ptEvScenePos = mapToScene(ptEvItemPos);
//
//            ptEvScreenPos.setX( ptEvScreenPos.x() + fXOffs );
//            ptEvScreenPos.setY( ptEvScreenPos.y() + fYOffs );
//
//            i_pEv->setPos(ptEvItemPos);
//            i_pEv->setScenePos(ptEvScenePos);
//            i_pEv->setScreenPos(ptEvScreenPos);
//
//            QGraphicsTextItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)
//
//            setFocus(Qt::MouseFocusReason);
//
//            //m_pDrawingScene->onGraphObjCreationFinished(this);
//
//            //// The object has been initially created.
//            //m_editMode = EEditMode::None;
//            //m_editResizeMode = EEditResizeMode::None;
//            //m_idxSelPtSelectedPolygon = -1;
//            //m_selPtSelectedBoundingRect = ESelectionPoint::None;
//
//            //m_pDrawingScene->setMode( EMode::Undefined, EEditTool::Select, m_editMode, m_editResizeMode, true );
//
//            ////prepareGeometryChange();
//
//            ////setSelected(true);
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//            m_rctCurr = QGraphicsTextItem::boundingRect();
//
//            m_ptRotOriginCurr = m_rctCurr.center();
//
//            //acceptCurrentAsOriginalCoors();
//
//            m_rctOnMousePressEvent = m_rctCurr;
//
//            m_ptRotOriginOnMousePressEvent = mapToScene(m_ptRotOriginCurr);
//#endif
//
//            //updateEditInfo();
//            //updateToolTip();
//
//        } // if( editToolDrawing == EEditTool::CreateObjects && m_editMode == EEditMode::Creating )
//
//        else if( editToolDrawing == EEditTool::Select && m_editMode == EEditMode::None )
//        {
//            if( !isSelected() )
//            {
//                setSelected(true);
//            }
//
//            CGraphObjSelectionPoint* pGraphObjSelPt;
//            ESelectionPoint          selPt;
//            int                      idxSelPt;
//
//            SGraphObjHitInfo hitInfo;
//
//            //bool bIsHit = isHit( ptMouseItemPos, &hitInfo );
//
//            m_editMode                  = hitInfo.m_editMode;
//            m_editResizeMode            = hitInfo.m_editResizeMode;
//            m_selPtSelectedBoundingRect = hitInfo.m_selPtBoundingRect;
//
//            for( idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++ )
//            {
//                selPt = static_cast<ESelectionPoint>(idxSelPt);
//
//                pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
//
//                if( pGraphObjSelPt != nullptr )
//                {
//                    if( m_selPtSelectedBoundingRect == selPt )
//                    {
//                        pGraphObjSelPt->setSelected(true);
//                    }
//                    else
//                    {
//                        pGraphObjSelPt->setSelected(false);
//                    }
//                }
//            }
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//            m_rctCurr = QGraphicsTextItem::boundingRect();
//            m_ptRotOriginCurr = m_rctCurr.center();
//            acceptCurrentAsOriginalCoors();
//#endif
//
//            m_ptScenePosOnMousePressEvent = pos();
//            m_ptMouseEvScenePosOnMousePressEvent = i_pEv->scenePos();
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//            m_rctOnMousePressEvent = m_rctCurr;
//            m_ptRotOriginOnMousePressEvent = mapToScene(m_ptRotOriginCurr);
//#endif
//
//            m_pDrawingScene->setMode( EMode::Undefined, EEditTool::Undefined, m_editMode, m_editResizeMode, false );
//
//            if( m_editMode == EEditMode::EditText )
//            {
//                QGraphicsTextItem::mousePressEvent(i_pEv);
//            }
//            //else
//            //{
//            //    QFocusEvent focusEv(QEvent::FocusOut,Qt::MouseFocusReason);
//            //    QGraphicsTextItem::focusOutEvent(&focusEv);
//            //}
//
//            //updateEditInfo();
//            //updateToolTip();
//
//        } // if( editToolDrawing == EEditTool::Select && m_editMode == EEditMode::None )
//
//    } // if( modeDrawing == EMode::Edit )

} // mousePressEvent

//------------------------------------------------------------------------------
void CGraphObjText::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseMoveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );

//    CEnumMode modeDrawing = m_pDrawingScene->getMode();
//
//    if( modeDrawing == EMode::Edit )
//    {
//        QPointF ptMouseScenePos = i_pEv->scenePos();
//
//        if( m_editMode == EEditMode::Creating )
//        {
//        } // if( m_editMode == EEditMode::Creating )
//
//        else if( m_editMode == EEditMode::Move )
//        {
//            double fxDiff = ptMouseScenePos.x() - m_ptMouseEvScenePosOnMousePressEvent.x();
//            double fyDiff = ptMouseScenePos.y() - m_ptMouseEvScenePosOnMousePressEvent.y();
//
//            QPointF ptPos( m_ptScenePosOnMousePressEvent.x() + fxDiff, m_ptScenePosOnMousePressEvent.y() + fyDiff );
//
//            setPos(ptPos);
//
//            //updateEditInfo();
//            //updateToolTip();
//
//        } // if( m_editMode == EEditMode::Move )
//
//        else if( m_editMode == EEditMode::Resize )
//        {
//        } // if( m_editMode == EEditMode::Resize )
//
//        else if( m_editMode == EEditMode::Rotate )
//        {
//            double fRotAngle_rad = getAngleRad( m_ptRotOriginOnMousePressEvent, ptMouseScenePos );
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//            m_fRotAngleCurr_deg = Math::rad2Degree(fRotAngle_rad);
//
//            switch( m_selPtSelectedBoundingRect.enumerator() )
//            {
//                case ESelectionPoint::RotateTop:
//                {
//                    m_fRotAngleCurr_deg -= 90.0;
//                    break;
//                }
//                case ESelectionPoint::RotateBottom:
//                {
//                    m_fRotAngleCurr_deg -= 270.0;
//                    break;
//                }
//                default:
//                {
//                    break;
//                }
//            }
//
//            m_fRotAngleCurr_deg = Math::round2Resolution( m_fRotAngleCurr_deg, m_pDrawingScene->getRotationAngleResolutionInDegree() );
//
//            while( m_fRotAngleCurr_deg >= 360.0 )
//            {
//                m_fRotAngleCurr_deg -= 360.0;
//            }
//            while( m_fRotAngleCurr_deg < 0.0 )
//            {
//                m_fRotAngleCurr_deg += 360.0;
//            }
//#endif
//
//            updateTransform();
//            //updateEditInfo();
//            //updateToolTip();
//
//        } // if( m_editMode == EEditMode::Rotate )
//
//    } // if( modeDrawing == EMode::Edit )

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CGraphObjText::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

//    CEnumMode modeDrawing = m_pDrawingScene->getMode();
//
//    if( modeDrawing == EMode::Edit )
//    {
//        if( m_editMode == EEditMode::Creating )
//        {
//            QPointF ptEvItemPos = i_pEv->pos();
//            QPointF ptEvScenePos = i_pEv->scenePos();
//            QPoint  ptEvScreenPos = i_pEv->screenPos();
//            QRectF  rctBounding = QGraphicsTextItem::boundingRect();
//
//            double fXOffs = rctBounding.center().x() - ptEvItemPos.x();
//            double fYOffs = rctBounding.center().y() - ptEvItemPos.y();
//
//            ptEvItemPos = rctBounding.center();
//            ptEvScenePos = mapToScene(ptEvItemPos);
//
//            ptEvScreenPos.setX( ptEvScreenPos.x() + fXOffs );
//            ptEvScreenPos.setY( ptEvScreenPos.y() + fYOffs );
//
//            i_pEv->setPos(ptEvItemPos);
//            i_pEv->setScenePos(ptEvScenePos);
//            i_pEv->setScreenPos(ptEvScreenPos);
//
//            QGraphicsTextItem::mouseReleaseEvent(i_pEv);
//
//            // The object has been initially created.
//            //m_pDrawingScene->onGraphObjCreationFinished(this);
//
//            // After creating the text item the edit cursor is active, the text should be entered
//            // but there should not be a new text object created by pressing the mouse again.
//            m_pDrawingScene->setMode( EMode::Undefined, EEditTool::Select, m_editMode, m_editResizeMode, true );
//
//            ////prepareGeometryChange();
//
//            ////setSelected(true);
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//            m_rctCurr = QGraphicsTextItem::boundingRect();
//            m_ptRotOriginCurr = m_rctCurr.center();
//
//            //acceptCurrentAsOriginalCoors();
//#endif
//
//        } // if( m_editMode == EEditMode::Creating )
//
//        else // if( m_editMode != EEditMode::Creating )
//        {
//            if( m_editMode == EEditMode::Move )
//            {
//            } // if( m_editMode == EEditMode::Move )
//
//            else if( m_editMode == EEditMode::Resize )
//            {
//            } // if( m_editMode == EEditMode::Resize )
//
//            else if( m_editMode == EEditMode::Rotate )
//            {
//            } // if( m_editMode == EEditMode::Rotate )
//
//            else if( m_editMode == EEditMode::MoveShapePoint )
//            {
//            } // if( m_editMode == EEditMode::MoveShapePoint )
//
//            m_editMode = EEditMode::None;
//            m_editResizeMode = EEditResizeMode::None;
//            m_idxSelPtSelectedPolygon = -1;
//            m_selPtSelectedBoundingRect = ESelectionPoint::None;
//
//            // The mouse release event would select the object.
//            // This is not wanted if the selection tool is not active.
//            bool bIsSelectable = flags() & QGraphicsItem::ItemIsSelectable;
//            bool bIsSelectableReset = false;
//
//            if( bIsSelectable && m_pDrawingScene->getEditTool() != EEditTool::Select )
//            {
//                setFlag(QGraphicsItem::ItemIsSelectable,false);
//                bIsSelectableReset = true;
//            }
//
//            QGraphicsTextItem::mouseReleaseEvent(i_pEv);
//
//            if( bIsSelectableReset )
//            {
//                setFlag(QGraphicsItem::ItemIsSelectable,bIsSelectable);
//            }
//
//        } // if( m_editMode != EEditMode::Creating )
//
//    } // if( modeDrawing == EMode::Edit )

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CGraphObjText::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strMthInArgs );

    // When doubleclicking an item, the item will first receive a mouse
    // press event, followed by a release event (i.e., a click), then a
    // doubleclick event, and finally a release event.
    // The default implementation of "mouseDoubleClickEvent" calls "mousePressEvent".
    //QGraphicsTextItem::mouseDoubleClickEvent(i_pEv);

    //CEnumMode modeDrawing = m_pDrawingScene->getMode();

    //if( modeDrawing == EMode::Edit )
    //{
    //    if( isSelected() )
    //    {
    //        onCreateAndExecDlgFormatGraphObjs();
    //    }

    //} // if( modeDrawing == EMode::Edit )

} // mouseDoubleClickEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjText::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjKeyEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        qKeyEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "keyPressEvent",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsTextItem::keyPressEvent(i_pEv);

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    m_rctCurr = QGraphicsTextItem::boundingRect();
#endif

} // keyPressEvent

//------------------------------------------------------------------------------
void CGraphObjText::keyReleaseEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjKeyEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        qKeyEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "keyReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsTextItem::keyReleaseEvent(i_pEv);

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    m_rctCurr = QGraphicsTextItem::boundingRect();
#endif

} // keyReleaseEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphObjText::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
//------------------------------------------------------------------------------
{
    if (m_bDtorInProgress) {
        return i_value;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsItemChange2Str(i_change, i_value);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "itemChange",
        /* strAddInfo   */ strMthInArgs );

    QVariant valChanged = i_value;

    bool bZValueChanged = false;
    bool bTreeEntryChanged = false;

    if (i_change == ItemSceneHasChanged) {
        // The item may have been removed from the scene.
        if (scene() != nullptr) {
        }
    }
    else if (i_change == ItemSelectedHasChanged) {
        prepareGeometryChange();
        //if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
        //    bringToFront();
        //    if (m_editMode == EEditMode::Creating) {
        //        //showSelectionPoints(
        //        //    ESelectionPointsBoundingRectCorner|ESelectionPointsBoundingRectLineCenter|ESelectionPointsPolygonShapePoints);
        //    }
        //    else {
        //        showSelectionPoints(ESelectionPointsBoundingRectRotate);
        //    }
        //    // Not necessary as item has been brought to front and "showSelectionPoints"
        //    // sets zValue of selection points above item.
        //    //bringSelectionPointsToFront();
        //    setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
        //}
        //else {
        //    setAcceptedMouseButtons(Qt::NoButton);
        //    hideSelectionPoints();
        //    resetStackingOrderValueToOriginalValue(); // restore ZValue as before selecting the object
        //    m_editMode = EEditMode::None;
        //    m_editResizeMode = EEditResizeMode::None;
        //    m_selPtSelectedBoundingRect = ESelectionPoint::None;
        //    m_idxSelPtSelectedPolygon = -1;
        //}
        //updateEditInfo();
        //updateToolTip();
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemTransformHasChanged) {
        //updateEditInfo();
        //updateToolTip();
    }
    else if( i_change == ItemTransformChange
          || i_change == ItemPositionHasChanged
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
        //updateTransform();
        //updateEditInfo();
        //updateToolTip();
    }

    else if (i_change == ItemZValueHasChanged) {
        bZValueChanged = true;
        bTreeEntryChanged = true;
    }

    if (bZValueChanged) {
        emit_zValueChanged(zValue());
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

} // itemChange
