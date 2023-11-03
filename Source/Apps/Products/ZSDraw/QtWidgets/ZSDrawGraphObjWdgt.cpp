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

#include "QtWidgets/ZSDrawGraphObjWdgt.h"

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Widgets/GraphObjFormat/ZSDrawDlgFormatGraphObjs.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysMath.h"

#include <QtGui/QBitmap>
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
using namespace ZS::PhysVal;
using namespace ZS::Draw;
using namespace ZS::Draw::QtWidgets;


/*******************************************************************************
class CGraphObjWdgt : public CGraphObj, public QGraphicsProxyWidget
*******************************************************************************/

/*==============================================================================
public: // class members
==============================================================================*/

const QString CGraphObjWdgt::c_strFactoryGroupName = "Widgets";

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjWdgt::CGraphObjWdgt(
    CDrawingScene*       i_pDrawingScene,
    const QString&       i_strType,
    const QString&       i_strObjName,
    const CDrawSettings& i_drawSettings ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene */ i_pDrawingScene,
        /* strFactoryGroupName */ c_strFactoryGroupName,
        /* type                */ EGraphObjTypeUserDefined,
        /* strType             */ i_strType,
        /* strObjName          */ i_strObjName,
        /* drawSettings        */ i_drawSettings ),
    QGraphicsProxyWidget()
{
} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjWdgt::~CGraphObjWdgt()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    emit_aboutToBeDestroyed();

} // dtor

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjWdgt::setIsHit( bool i_bHit )
//------------------------------------------------------------------------------
{
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
QString CGraphObjWdgt::getScenePolygonShapePointsString() const
//------------------------------------------------------------------------------
{
    QString   strScenePolygonShapePoints;
    QRectF    rctBounding = QGraphicsProxyWidget::boundingRect();
    QPolygonF plgScene = mapToScene(rctBounding);

    strScenePolygonShapePoints = polygon2Str(plgScene);

    return strScenePolygonShapePoints;

} // getScenePolygonShapePointsString

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjWdgt::setWidth( const CPhysVal& i_physValWidth )
//------------------------------------------------------------------------------
{
    setSize( i_physValWidth, getHeight(i_physValWidth.unit()) );
}

//------------------------------------------------------------------------------
void CGraphObjWdgt::setHeight( const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    setSize( getWidth(i_physValHeight.unit()), i_physValHeight );
}

//------------------------------------------------------------------------------
void CGraphObjWdgt::setSize( const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
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

        setGeometry(m_rctCurr);

        updateSelectionPointsOfBoundingRect(m_rctCurr);
    }
#endif

} // setSize

//------------------------------------------------------------------------------
void CGraphObjWdgt::setSize( const CPhysValSize& i_physValSize )
//------------------------------------------------------------------------------
{
    setSize( i_physValSize.width(), i_physValSize.height() );
}

/*==============================================================================
public: // overridables of base class QGraphicsProxyWidget
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjWdgt::boundingRect() const
//------------------------------------------------------------------------------
{
    QRectF rctBounding = QGraphicsProxyWidget::boundingRect();

    CGraphObjSelectionPoint* pGraphObjSelPt;
    QRectF                   rctSelPt;
    QPolygonF                plgSelPt;
    CEnumSelectionPoint      selPt;

    for( selPt = 0; selPt < CEnumSelectionPoint::count(); selPt++ )
    {
        pGraphObjSelPt = m_arpSelPtsBoundingRect[selPt.enumeratorAsInt()];

        if( pGraphObjSelPt != nullptr )
        {
            rctSelPt = pGraphObjSelPt->boundingRect();
            plgSelPt = mapFromItem( pGraphObjSelPt, rctSelPt );
            rctBounding |= plgSelPt.boundingRect();
        }
    }

    return rctBounding;

} // boundingRect

//------------------------------------------------------------------------------
void CGraphObjWdgt::paint(
    QPainter*                       i_pPainter,
    const QStyleOptionGraphicsItem* i_pStyleOption,
    QWidget*                        i_pWdgt )
//------------------------------------------------------------------------------
{
    QGraphicsProxyWidget::paint(i_pPainter,i_pStyleOption,i_pWdgt);

    i_pPainter->save();

    if( m_pDrawingScene->getMode() == EMode::Edit && isSelected() )
    {
        QPen pn;

        pn.setStyle(Qt::DotLine);
        pn.setColor(Qt::blue);

        i_pPainter->setPen(pn);
        i_pPainter->drawRect( QGraphicsProxyWidget::boundingRect() );

        if( m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::TopCenter)] != nullptr && m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateTop)] != nullptr )
        {
            CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::TopCenter)];
            CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateTop)];

            QPointF ptRct = QPointF( pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y() );
            QPointF ptRot = QPointF( pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y() );

            QPointF ptRctM = mapFromScene(ptRct);
            QPointF ptRotM = mapFromScene(ptRot);

            i_pPainter->drawLine( ptRctM, ptRotM );
        }

        if( m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::BottomCenter)] != nullptr && m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateBottom)] != nullptr )
        {
            CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::BottomCenter)];
            CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateBottom)];

            QPointF ptRct = QPointF( pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y() );
            QPointF ptRot = QPointF( pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y() );

            QPointF ptRctM = mapFromScene(ptRct);
            QPointF ptRotM = mapFromScene(ptRot);

            i_pPainter->drawLine( ptRctM, ptRotM );
        }

    } // if( isSelected() )

    i_pPainter->restore();

} // paint

/*==============================================================================
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjWdgt::showSelectionPoints( unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    if (parentItem() == nullptr) {
        QRectF rct = QGraphicsProxyWidget::boundingRect();
        showSelectionPointsOfBoundingRect(rct,i_selPts);
    }
}

//------------------------------------------------------------------------------
void CGraphObjWdgt::updateSelectionPoints( unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    if (parentItem() == nullptr) {
        QRectF rct = QGraphicsProxyWidget::boundingRect();
        updateSelectionPointsOfBoundingRect(rct,i_selPts);
    }
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjWdgt::sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv )
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
        {
            break;
        }
        //case QEvent::GraphicsSceneHoverEnter:
        //{
        //    QGraphicsSceneHoverEvent* pEv = dynamic_cast<QGraphicsSceneHoverEvent*>(i_pEv);
        //    if( pEv != nullptr )
        //    {
        //        hoverEnterEvent(pEv);
        //        bEventHandled = true;
        //    }
        //    break;
        //}
        //case QEvent::GraphicsSceneHoverMove:
        //{
        //    QGraphicsSceneHoverEvent* pEv = dynamic_cast<QGraphicsSceneHoverEvent*>(i_pEv);
        //    if( pEv != nullptr )
        //    {
        //        hoverMoveEvent(pEv);
        //        bEventHandled = true;
        //    }
        //    break;
        //}
        //case QEvent::GraphicsSceneHoverLeave:
        //{
        //    QGraphicsSceneHoverEvent* pEv = dynamic_cast<QGraphicsSceneHoverEvent*>(i_pEv);
        //    if( pEv != nullptr )
        //    {
        //        hoverLeaveEvent(pEv);
        //        bEventHandled = true;
        //    }
        //    break;
        //}
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

    return bEventHandled;

} // sceneEventFilter

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjWdgt::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit )
    {
        if( editToolDrawing == EEditTool::Select )
        {
            QPointF ptItemPos = mapFromScene(i_pEv->scenePos());

            SGraphObjHitInfo hitInfo;

            bool bIsHit = isHit(ptItemPos,&hitInfo);

            if( bIsHit )
            {
                setCursor(hitInfo.m_cursor);
            }
        }
    }
    else
    {
        QGraphicsProxyWidget::hoverEnterEvent(i_pEv);
    }

} // hoverEnterEvent

//------------------------------------------------------------------------------
void CGraphObjWdgt::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit )
    {
        if( editToolDrawing == EEditTool::Select )
        {
            QPointF ptItemPos = mapFromScene(i_pEv->scenePos());

            SGraphObjHitInfo hitInfo;

            bool bIsHit = isHit(ptItemPos,&hitInfo);

            if( bIsHit )
            {
                setCursor(hitInfo.m_cursor);
            }
        }
    }
    else
    {
        QGraphicsProxyWidget::hoverMoveEvent(i_pEv);
    }

} // hoverMoveEvent

//------------------------------------------------------------------------------
void CGraphObjWdgt::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        unsetCursor();
    }
    else
    {
        QGraphicsProxyWidget::hoverLeaveEvent(i_pEv);
    }

} // hoverLeaveEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjWdgt::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit )
    {
        if( m_editMode == EEditMode::Creating )
        {
            // Widgets are created by drop events.

        } // if( m_editMode == EEditMode::Creating )

        else if( m_editMode == EEditMode::None )
        {
            if( editToolDrawing == EEditTool::Select )
            {
                QGraphicsItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)

                //QPointF ptMousePos = i_pEv->pos();

                CGraphObjSelectionPoint* pGraphObjSelPt;
                CEnumSelectionPoint      selPt;

                SGraphObjHitInfo hitInfo;

                //bool bIsHit = isHit( ptMousePos, &hitInfo );

                m_editMode                  = hitInfo.m_editMode;
                m_editResizeMode            = hitInfo.m_editResizeMode;
                m_selPtSelectedBoundingRect = hitInfo.m_selPtBoundingRect;

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
                m_ptRotOriginCurr = rect().center();
#endif
                for( selPt = 0; selPt < CEnumSelectionPoint::count(); selPt++ )
                {
                    pGraphObjSelPt = m_arpSelPtsBoundingRect[selPt.enumeratorAsInt()];

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

                m_pDrawingScene->setMode( EMode::Ignore, EEditTool::Ignore, m_editMode, m_editResizeMode, false );

                updateEditInfo();
                updateToolTip();
            }
        }
    }

    else // if( modeDrawing == EMode::View )
    {
        QGraphicsProxyWidget::mousePressEvent(i_pEv);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CGraphObjWdgt::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    if( m_pDrawingScene->getMode() == EMode::Edit )
    {
        if( m_editMode == EEditMode::Creating )
        {
            // Widgets are created by drop events.
        }
        else if( m_editMode == EEditMode::Move )
        {
            QGraphicsItem::mouseMoveEvent(i_pEv);
        }
        else if( m_editMode == EEditMode::Resize )
        {
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
            QRectF  rctItemOld = geometry();
            QRectF  rctItemNew = rctItemOld;
            QPointF ptPosOld = pos(); // relative to the parent (which might be the drawing scene)
            QPointF ptPosNew = ptPosOld;
            QPointF ptRotOriginOld = m_ptRotOriginCurr;
            QPointF ptRotOriginNew = ptRotOriginOld;
            QPointF ptMouseItemPos = i_pEv->pos();
            double  fdx = 0.0;
            double  fdy = 0.0;

            rctItemNew = resizeRect( rctItemOld, m_selPtSelectedBoundingRect.enumerator(), ptMouseItemPos, &m_selPtSelectedBoundingRect );

            if( rctItemNew.left() != 0.0 )
            {
                fdx = rctItemNew.left();
                rctItemNew.translate(-fdx,0.0);
                ptPosNew.setX( ptPosOld.x() + fdx );
                ptRotOriginNew.setX( ptRotOriginOld.x() - fdx );
            }

            if( rctItemNew.top() != 0.0 )
            {
                fdy = rctItemNew.top();
                rctItemNew.translate(0.0,-fdy);
                ptPosNew.setY( ptPosOld.y() + fdy );
                ptRotOriginNew.setY( ptRotOriginOld.y() - fdy );
            }

            setGeometry(rctItemNew); // does not lead to "itemChange" call even if flag ItemSendsGeometryChanges is set.

            setPos(ptPosNew);    // does not lead to "itemChange" call even if flag ItemSendsGeometryChanges is set.

            m_ptRotOriginCurr = ptRotOriginNew;

            CGraphObjSelectionPoint* pGraphObjSelPt;
            QPointF                  ptSel;
            CEnumSelectionPoint      selPt;

            for( selPt = 0; selPt < CEnumSelectionPoint::count(); selPt++ )
            {
                pGraphObjSelPt = m_arpSelPtsBoundingRect[selPt.enumeratorAsInt()];

                if( pGraphObjSelPt != nullptr )
                {
                    ptSel = ZS::Draw::getSelectionPointCoors(rctItemNew,selPt.enumerator());
                    ptSel = mapToScene(ptSel);
                    pGraphObjSelPt->setPos(ptSel);
                }
            }

            updateEditInfo();
            updateToolTip();
#endif
        } // if( m_editMode == EEditMode::Resize )

        else if( m_editMode == EEditMode::Rotate )
        {
            // Rotating a widget is not supported.
        }
    }

    else // if( m_pDrawingScene->getMode() == EMode::View )
    {
        QGraphicsProxyWidget::mouseMoveEvent(i_pEv);
    }

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CGraphObjWdgt::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    if( m_pDrawingScene->getMode() == EMode::Edit )
    {
        //EEditMode editModePrev = m_editMode;

        m_editMode = EEditMode::None;
        m_editResizeMode = EEditResizeMode::None;
        m_idxSelPtSelectedPolygon = -1;
        m_selPtSelectedBoundingRect = ESelectionPoint::None;

        // The mouse release event would select the object.
        // This is not wanted if the selection tool is not active.
        bool bIsSelectable = flags() & QGraphicsItem::ItemIsSelectable;
        bool bIsSelectableReset = false;

        if( bIsSelectable && m_pDrawingScene->getEditTool() != EEditTool::Select )
        {
            setFlag(QGraphicsItem::ItemIsSelectable,false);
            bIsSelectableReset = true;
        }

        QGraphicsItem::mouseReleaseEvent(i_pEv);

        if( bIsSelectableReset )
        {
            setFlag(QGraphicsItem::ItemIsSelectable,bIsSelectable);
        }

        updateEditInfo();
        updateToolTip();
    }
    else // if( m_pDrawingScene->getMode() == EMode::View )
    {
        QGraphicsProxyWidget::mouseReleaseEvent(i_pEv);
    }

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CGraphObjWdgt::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    // When doubleclicking an item, the item will first receive a mouse
    // press event, followed by a release event (i.e., a click), then a
    // doubleclick event, and finally a release event.
    // The default implementation of "mouseDoubleClickEvent" calls "mousePressEvent".

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        if( isSelected() )
        {
            onCreateAndExecDlgFormatGraphObjs();
        }
    }
    else // if( modeDrawing == EMode::View )
    {
        QGraphicsProxyWidget::mouseDoubleClickEvent(i_pEv);
    }

} // mouseDoubleClickEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphObjWdgt::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
//------------------------------------------------------------------------------
{
    QVariant valChanged = i_value;

    bool bZValueChanged = false;
    bool bTreeEntryChanged = false;

    if (i_change == ItemSceneHasChanged) {
        // The item may have been removed from the scene.
        if (scene() != nullptr) {
        }
    }
    else if (i_change == ItemSelectedHasChanged) {
        if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
            bringToFront();
            if (m_editMode == EEditMode::Creating) {
                showSelectionPoints(
                    ESelectionPointsBoundingRectCorner|ESelectionPointsBoundingRectLineCenter|ESelectionPointsPolygonShapePoints);
            }
            else {
                showSelectionPoints();
            }
            // Not necessary as item has been brought to front and "showSelectionPoints"
            // sets zValue of selection points above item.
            //bringSelectionPointsToFront();
            setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
        }
        else {
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
    }
    else if (i_change == ItemTransformHasChanged) {
        for (CEnumSelectionPoint selPt = 0; selPt < CEnumSelectionPoint::count(); selPt++) {
            CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[selPt.enumeratorAsInt()];
            if (pGraphObjSelPt != nullptr) {
                QPointF ptSel = getSelectionPointCoors(selPt.enumerator());
                ptSel = mapToScene(ptSel);
                pGraphObjSelPt->setPos(ptSel);
            }
        }
        updateEditInfo();
        updateToolTip();
    }

    else // if( i_change == ItemMatrixChange
         //  || i_change == ItemPositionHasChanged
         //  || i_change == ItemParentHasChanged
         //  || i_change == ItemSceneHasChanged
         //  || i_change == ItemScenePositionHasChanged
         //  || i_change == ItemRotationHasChanged
         //  || i_change == ItemScaleHasChanged
         //  || i_change == ItemTransformOriginPointHasChanged )
    {
        if (i_change == ItemZValueHasChanged) {
            bZValueChanged = true;
        }
        updateEditInfo();
        updateToolTip();
    }

    if (bZValueChanged) {
        emit_zValueChanged();
    }
    if (bTreeEntryChanged && m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }

    valChanged = QGraphicsItem::itemChange(i_change,i_value);

    return valChanged;

} // itemChange
