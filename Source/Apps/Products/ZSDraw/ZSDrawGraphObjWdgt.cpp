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
#include <QtGui/QPainter>

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsSceneEvent>
#include <QtGui/QStyleOption>
#else
#include <QtWidgets/QGraphicsSceneEvent>
#include <QtWidgets/QStyleOption>
#endif

#include "ZSDrawGraphObjWdgt.h"

#include "ZSDraw/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/ZSDrawDlgFormatGraphObjs.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Draw::QtWidgets;


/*******************************************************************************
class CGraphObjWdgt : public CGraphObj, public QGraphicsProxyWidget
*******************************************************************************/

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjWdgt::CGraphObjWdgt(
    CDrawingScene*       i_pDrawingScene,
    const QString&       i_strNameSpace,
    const QString&       i_strClassName,
    const QString&       i_strType,
    const QString&       i_strObjName,
    const QString&       i_strObjId,
    const CDrawSettings& i_drawSettings ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene */ i_pDrawingScene,
        /* strNameSpace  */ i_strNameSpace,
        /* strClassName  */ i_strClassName,
        /* type          */ EGraphObjTypeUserDefined,
        /* strType       */ i_strType,
        /* strObjName    */ i_strObjName,
        /* strObjId      */ i_strObjId,
        /* drawSettings  */ i_drawSettings ),
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

            QGraphicsScene* pGraphicsScene = pGraphicsItem->scene();
            if( pGraphicsScene != nullptr )
            {
                pGraphicsScene->removeItem(pGraphicsItem);
            }

        } // if( m_pDrawingScene != nullptr )

    } // if( pGraphicsItem != nullptr )

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
void CGraphObjWdgt::setWidth( double i_fWidth )
//------------------------------------------------------------------------------
{
    setSize( i_fWidth, m_rctCurr.height() );
}

//------------------------------------------------------------------------------
void CGraphObjWdgt::setHeight( double i_fHeight )
//------------------------------------------------------------------------------
{
    setSize( m_rctCurr.width(), i_fHeight );
}

//------------------------------------------------------------------------------
void CGraphObjWdgt::setSize( double i_fWidth, double i_fHeight )
//------------------------------------------------------------------------------
{
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

} // setSize

//------------------------------------------------------------------------------
void CGraphObjWdgt::setSize( const QSizeF& i_size )
//------------------------------------------------------------------------------
{
    setSize( i_size.width(), i_size.height() );
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
    ESelectionPoint          selPt;
    int                      idxSelPt;

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

    i_pPainter->restore();

} // paint

/*==============================================================================
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjWdgt::showSelectionPoints( unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QRectF rct = QGraphicsProxyWidget::boundingRect();

    showSelectionPointsOfBoundingRect(rct,i_selPts);

} // showSelectionPoints

//------------------------------------------------------------------------------
void CGraphObjWdgt::updateSelectionPoints( unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QRectF rct = QGraphicsProxyWidget::boundingRect();

    updateSelectionPointsOfBoundingRect(rct,i_selPts);

} // updateSelectionPoints

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
    EMode     modeDrawing     = m_pDrawingScene->getMode();
    EEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit )
    {
        if( editToolDrawing == EEditToolSelect )
        {
            QPointF ptItemPos = mapFromScene(i_pEv->scenePos());

            SGraphObjHitInfo hitInfo;

            bool bIsHit = isHit(ptItemPos,&hitInfo);

            if( bIsHit )
            {
                setCursor(hitInfo.m_cursor);
            }
        } // if( editToolDrawing == EEditToolSelect )

    } // if( modeDrawing == EMode::Edit )

    else // if( modeDrawing == EMode::Simulation )
    {
        QGraphicsProxyWidget::hoverEnterEvent(i_pEv);
    }

} // hoverEnterEvent

//------------------------------------------------------------------------------
void CGraphObjWdgt::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    EMode     modeDrawing     = m_pDrawingScene->getMode();
    EEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit )
    {
        if( editToolDrawing == EEditToolSelect )
        {
            QPointF ptItemPos = mapFromScene(i_pEv->scenePos());

            SGraphObjHitInfo hitInfo;

            bool bIsHit = isHit(ptItemPos,&hitInfo);

            if( bIsHit )
            {
                setCursor(hitInfo.m_cursor);
            }
        } // if( editToolDrawing == EEditToolSelect )

    } // if( modeDrawing == EMode::Edit )

    else // if( modeDrawing == EMode::Simulation )
    {
        QGraphicsProxyWidget::hoverMoveEvent(i_pEv);
    }

} // hoverMoveEvent

//------------------------------------------------------------------------------
void CGraphObjWdgt::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    EMode modeDrawing = m_pDrawingScene->getMode();

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
    EMode     modeDrawing     = m_pDrawingScene->getMode();
    EEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit )
    {
        if( m_editMode == EEditModeCreating )
        {
            // Widgets are created by drop events.

        } // if( m_editMode == EEditModeCreating )

        else if( m_editMode == EEditModeUndefined )
        {
            if( editToolDrawing == EEditToolSelect )
            {
                QGraphicsItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)

                //QPointF ptMousePos = i_pEv->pos();

                CGraphObjSelectionPoint* pGraphObjSelPt;
                ESelectionPoint          selPt;
                int                      idxSelPt;

                SGraphObjHitInfo hitInfo;

                //bool bIsHit = isHit( ptMousePos, &hitInfo );

                m_editMode                  = hitInfo.m_editMode;
                m_editResizeMode            = hitInfo.m_editResizeMode;
                m_selPtSelectedBoundingRect = hitInfo.m_selPtBoundingRect;

                m_ptRotOriginCurr = rect().center();

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

                m_pDrawingScene->setMode( EMode::Ignore, EEditToolIgnore, m_editMode, m_editResizeMode, false );

                updateEditInfo();
                updateToolTip();

            } // if( editToolDrawing == EEditToolSelect )

        } // if( m_editMode == EEditModeUndefined )

    } // if( modeDrawing == EMode::Edit )

    else // if( modeDrawing == EMode::Simulation )
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
        if( m_editMode == EEditModeCreating )
        {
            // Widgets are created by drop events.

        } // if( m_editMode == EEditModeCreating )

        else if( m_editMode == EEditModeMove )
        {
            QGraphicsItem::mouseMoveEvent(i_pEv);

        } // if( m_editMode == EEditModeMove )

        else if( m_editMode == EEditModeResize )
        {
            QRectF  rctItemOld = geometry();
            QRectF  rctItemNew = rctItemOld;
            QPointF ptPosOld = pos(); // relative to the parent (which might be the drawing scene)
            QPointF ptPosNew = ptPosOld;
            QPointF ptRotOriginOld = m_ptRotOriginCurr;
            QPointF ptRotOriginNew = ptRotOriginOld;
            QPointF ptMouseItemPos = i_pEv->pos();
            double  fdx = 0.0;
            double  fdy = 0.0;

            rctItemNew = resizeRect( rctItemOld, m_selPtSelectedBoundingRect, ptMouseItemPos, &m_selPtSelectedBoundingRect );

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
            ESelectionPoint          selPt;
            int                      idxSelPt;

            for( idxSelPt = 0; idxSelPt < ESelectionPointCount; idxSelPt++ )
            {
                selPt = static_cast<ESelectionPoint>(idxSelPt);

                pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

                if( pGraphObjSelPt != nullptr )
                {
                    ptSel = ZS::Draw::getSelectionPoint(rctItemNew,selPt);
                    ptSel = mapToScene(ptSel);
                    pGraphObjSelPt->setPos(ptSel);
                }
            }

            updateEditInfo();
            updateToolTip();

        } // if( m_editMode == EEditModeResize )

        else if( m_editMode == EEditModeRotate )
        {
            // Rotating a widget is not supported.

        } // if( m_editMode == EEditModeRotate )

    } // if( m_pDrawingScene->getMode() == EMode::Edit )

    else // if( m_pDrawingScene->getMode() == EMode::Simulation )
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

        m_editMode = EEditModeUndefined;
        m_editResizeMode = EEditResizeModeUndefined;
        m_idxSelPtSelectedPolygon = -1;
        m_selPtSelectedBoundingRect = ESelectionPointUndefined;

        // The mouse release event would select the object.
        // This is not wanted if the selection tool is not active.
        bool bIsSelectable = flags() & QGraphicsItem::ItemIsSelectable;
        bool bIsSelectableReset = false;

        if( bIsSelectable && m_pDrawingScene->getEditTool() != EEditToolSelect )
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

    } // if( m_pDrawingScene->getMode() == EMode::Edit )

    else // if( m_pDrawingScene->getMode() == EMode::Simulation )
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

    EMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        if( isSelected() )
        {
            onCreateAndExecDlgFormatGraphObjs();
        }

    } // if( modeDrawing == EMode::Edit )

    else // if( modeDrawing == EMode::Simulation )
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

    if( i_change == ItemSelectedHasChanged )
    {
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
        CGraphObjSelectionPoint* pGraphObjSelPt;
        QPointF                  ptSel;
        ESelectionPoint          selPt;
        int                      idxSelPt;

        for( idxSelPt = 0; idxSelPt < ESelectionPointCount; idxSelPt++ )
        {
            selPt = static_cast<ESelectionPoint>(idxSelPt);

            pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

            if( pGraphObjSelPt != nullptr )
            {
                ptSel = getSelectionPoint(selPt);
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
         //  || i_change == ItemZValueHasChanged
         //  || i_change == ItemScenePositionHasChanged
         //  || i_change == ItemRotationHasChanged
         //  || i_change == ItemScaleHasChanged
         //  || i_change == ItemTransformOriginPointHasChanged )
    {
        CGraphObjSelectionPoint* pGraphObjSelPt;
        QPointF                  ptSel;
        ESelectionPoint          selPt;
        int                      idxSelPt;

        for( idxSelPt = 0; idxSelPt < ESelectionPointCount; idxSelPt++ )
        {
            selPt = static_cast<ESelectionPoint>(idxSelPt);

            pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

            if( pGraphObjSelPt != nullptr )
            {
                ptSel = getSelectionPoint(selPt);
                ptSel = mapToScene(ptSel);
                pGraphObjSelPt->setPos(ptSel);

                if( i_change == ItemZValueHasChanged )
                {
                    pGraphObjSelPt->setZValue( zValue() + 0.05 );
                }
            }
        }

        updateEditInfo();
        updateToolTip();
    }

    valChanged = QGraphicsItem::itemChange(i_change,i_value);

    return valChanged;

} // itemChange
