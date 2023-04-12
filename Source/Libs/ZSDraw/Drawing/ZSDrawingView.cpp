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

#include <QtGui/qevent.h>

#include "ZSDraw/Drawing/ZSDrawingView.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/GraphObjFactories/ZSDrawObjFactory.h"
#include "ZSDraw/GraphObjs/ZSDrawGraphObj.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDrawingView : public QGraphicsView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDrawingView::CDrawingView( CDrawingScene* i_pDrawingScene, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QGraphicsView(i_pDrawingScene, i_pWdgtParent),
    m_pDrawingScene(i_pDrawingScene),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjMouseMoveEvent(nullptr),
    m_pTrcAdminObjPaintEvent(nullptr)
{
    setObjectName("Drawing");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pTrcAdminObjMouseMoveEvent = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName() + "-MouseMoveEvent");;
    m_pTrcAdminObjPaintEvent = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName() + "-PaintEvent");;

    Units.Length.setPxpis(logicalDpiX(), logicalDpiY());

    m_pDrawingScene->setDrawingSize(
        CDrawingSize("DrawingScene", QSize(1024, 768)));

    setViewportMargins(10.0, 10.0, 10.0, 10.0);

    setMouseTracking(true);
    setAcceptDrops(true);

    QObject::connect(
        m_pDrawingScene, &CDrawingScene::drawingSizeChanged,
        this, &CDrawingView::onSceneDrawingSizeChanged );

} // ctor

//------------------------------------------------------------------------------
CDrawingView::~CDrawingView()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    //saveSettings();

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjMouseMoveEvent);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjPaintEvent);

    //m_pageSetup;
    m_pDrawingScene = nullptr;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjMouseMoveEvent = nullptr;
    m_pTrcAdminObjPaintEvent = nullptr;

} // dtor

/*==============================================================================
public: // instance methods (drawing area)
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::setDrawingSize( const CDrawingSize& i_size )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_size.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDrawingSize",
        /* strAddInfo   */ strMthInArgs );

    // QGraphicsScene emits "drawingSizeChanged". The slot
    // "onSceneDrawingSizeChanged" is called emitting "drawingSizeChanged".
    m_pDrawingScene->setDrawingSize(i_size);
}

//------------------------------------------------------------------------------
CDrawingSize CDrawingView::drawingSize() const
//------------------------------------------------------------------------------
{
    return m_pDrawingScene->drawingSize();
}

/*==============================================================================
public: // instance methods (drawing area)
==============================================================================*/

////------------------------------------------------------------------------------
//void CDrawingView::setViewportMargins(int i_iLeft, int i_iTop, int i_iRight, int i_iBottom)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//
//    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
//    {
//        strMthInArgs  = "Left: "+ QString::number(i_iLeft);
//        strMthInArgs += ", Top: "+ QString::number(i_iTop);
//        strMthInArgs += ", Right: "+ QString::number(i_iRight);
//        strMthInArgs += ", Bottom: "+ QString::number(i_iBottom);
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "setViewportMargins",
//        /* strAddInfo   */ strMthInArgs );
//
//    QMargins margins(i_iLeft, i_iTop, i_iRight, i_iBottom);
//
//    if( margins != viewportMargins() )
//    {
//        QGraphicsView::setViewportMargins(margins);
//
//        // QGraphicsView has no signal "viewportMarginsChanged".
//        //emit_viewportMarginsChanged(viewportMargins());
//    }
//} // setViewportMargins
//
////------------------------------------------------------------------------------
//void CDrawingView::setViewportMargins(const QMargins& i_margins)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//
//    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
//    {
//        strMthInArgs = qMargins2Str(i_margins);
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "setViewportMargins",
//        /* strAddInfo   */ strMthInArgs );
//
//    if( i_margins != viewportMargins() )
//    {
//        QGraphicsView::setViewportMargins(i_margins);
//
//        // QGraphicsView has no signal "viewportMarginsChanged".
//        //emit_viewportMarginsChanged(viewportMargins());
//    }
//} // setViewportMargins

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        #if QT_VERSION < 0x050000
        strMthInArgs = "Pos:(" + QString::number(i_pEv->posF().x()) + "," + QString::number(i_pEv->posF().y()) + ")";
        #else
        strMthInArgs = "Pos:(" + QString::number(i_pEv->windowPos().x()) + "," + QString::number(i_pEv->windowPos().y()) + ")";
        #endif
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strMthInArgs );

    QPointF ptPos = i_pEv->pos();

    emit_mousePosChanged(ptPos);

    QGraphicsView::mousePressEvent(i_pEv);

    QList<QGraphicsItem*> arpGraphicsItemsUnderCursor = items(mapFromGlobal(QCursor::pos()));
    QGraphicsItem*        pGraphicsItem;
    int                   idxGraphObj;
    QCursor               cursor;
    bool                  bGraphicsItemHasCursor = false;

    for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsUnderCursor.size(); idxGraphObj++ )
    {
        pGraphicsItem = arpGraphicsItemsUnderCursor[idxGraphObj];

        if( pGraphicsItem->hasCursor() )
        {
            //QMetaObject::invokeMethod( this, "_q_setViewportCursor", Q_ARG(QCursor,itemUnderCursor->cursor()) );
            bGraphicsItemHasCursor = true;
            cursor = pGraphicsItem->cursor();
            break;
        }
    }

    if( bGraphicsItemHasCursor )
    {
        setCursor(cursor);
        viewport()->setCursor(cursor);
    }
    else // if( !bGraphicsItemHasCursor )
    {
        QPointF ptScenePos = mapToScene(ptPos.toPoint());
        QRectF  rctScene = m_pDrawingScene->sceneRect();

        if( rctScene.contains(ptScenePos) )
        {
            QCursor cursor = m_pDrawingScene->getProposedCursor(ptScenePos);
            setCursor(cursor);
            viewport()->setCursor(cursor);
        }
        else
        {
            unsetCursor();
            viewport()->unsetCursor();
        }
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CDrawingView::mouseMoveEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjMouseMoveEvent != nullptr && m_pTrcAdminObjMouseMoveEvent->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        #if QT_VERSION < 0x050000
        strMthInArgs = "Pos:(" + QString::number(i_pEv->posF().x()) + "," + QString::number(i_pEv->posF().y()) + ")";
        #else
        strMthInArgs = "Pos:(" + QString::number(i_pEv->windowPos().x()) + "," + QString::number(i_pEv->windowPos().y()) + ")";
        #endif
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvent,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    QPoint ptPos = i_pEv->pos();

    emit_mousePosChanged(ptPos);

    QGraphicsView::mouseMoveEvent(i_pEv);

    QList<QGraphicsItem*> arpGraphicsItemsUnderCursor = items(ptPos);
    QGraphicsItem*        pGraphicsItem;
    int                   idxGraphObj;
    QCursor               cursor;
    bool                  bGraphicsItemHasCursor = false;

    for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsUnderCursor.size(); idxGraphObj++ )
    {
        pGraphicsItem = arpGraphicsItemsUnderCursor[idxGraphObj];

        if( pGraphicsItem->hasCursor() )
        {
            bGraphicsItemHasCursor = true;
            cursor = pGraphicsItem->cursor();
            break;
        }
    }

    if( bGraphicsItemHasCursor )
    {
        setCursor(cursor);
        viewport()->setCursor(cursor);
    }
    else // if( !bGraphicsItemHasCursor )
    {
        QPointF ptScenePos = mapToScene(ptPos);
        QRectF  rctScene = m_pDrawingScene->sceneRect();

        if( rctScene.contains(ptScenePos) )
        {
            QCursor cursor = m_pDrawingScene->getProposedCursor(ptScenePos);
            setCursor(cursor);
            viewport()->setCursor(cursor);
        }
        else
        {
            unsetCursor();
            viewport()->unsetCursor();
        }
    }

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CDrawingView::mouseReleaseEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        #if QT_VERSION < 0x050000
        strMthInArgs = "Pos:(" + QString::number(i_pEv->posF().x()) + "," + QString::number(i_pEv->posF().y()) + ")";
        #else
        strMthInArgs = "Pos:(" + QString::number(i_pEv->windowPos().x()) + "," + QString::number(i_pEv->windowPos().y()) + ")";
        #endif
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

    QPointF ptPos = i_pEv->pos();

    emit_mousePosChanged(ptPos);

    QGraphicsView::mouseReleaseEvent(i_pEv);

    QList<QGraphicsItem*> arpGraphicsItemsUnderCursor = items(mapFromGlobal(QCursor::pos()));
    QGraphicsItem*        pGraphicsItem;
    int                   idxGraphObj;
    QCursor               cursor;
    bool                  bGraphicsItemHasCursor = false;

    for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsUnderCursor.size(); idxGraphObj++ )
    {
        pGraphicsItem = arpGraphicsItemsUnderCursor[idxGraphObj];

        if( pGraphicsItem->hasCursor() )
        {
            //QMetaObject::invokeMethod( this, "_q_setViewportCursor", Q_ARG(QCursor,itemUnderCursor->cursor()) );
            bGraphicsItemHasCursor = true;
            cursor = pGraphicsItem->cursor();
            break;
        }
    }

    if( bGraphicsItemHasCursor )
    {
        setCursor(cursor);
        viewport()->setCursor(cursor);
    }
    else // if( !bGraphicsItemHasCursor )
    {
        QPointF ptScenePos = mapToScene(ptPos.toPoint());
        QRectF  rctScene = m_pDrawingScene->sceneRect();

        if( rctScene.contains(ptScenePos) )
        {
            QCursor cursor = m_pDrawingScene->getProposedCursor(ptScenePos);
            setCursor(cursor);
            viewport()->setCursor(cursor);
        }
        else
        {
            unsetCursor();
            viewport()->unsetCursor();
        }
    }

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CDrawingView::mouseDoubleClickEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        #if QT_VERSION < 0x050000
        strMthInArgs = "Pos:(" + QString::number(i_pEv->posF().x()) + "," + QString::number(i_pEv->posF().y()) + ")";
        #else
        strMthInArgs = "Pos:(" + QString::number(i_pEv->windowPos().x()) + "," + QString::number(i_pEv->windowPos().y()) + ")";
        #endif
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strMthInArgs );

    QPointF ptPos = i_pEv->pos();

    emit_mousePosChanged(ptPos);

    QGraphicsView::mouseDoubleClickEvent(i_pEv);

    QList<QGraphicsItem*> arpGraphicsItemsUnderCursor = items(mapFromGlobal(QCursor::pos()));
    QGraphicsItem*        pGraphicsItem;
    int                   idxGraphObj;
    QCursor               cursor;
    bool                  bGraphicsItemHasCursor = false;

    for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsUnderCursor.size(); idxGraphObj++ )
    {
        pGraphicsItem = arpGraphicsItemsUnderCursor[idxGraphObj];

        if( pGraphicsItem->hasCursor() )
        {
            //QMetaObject::invokeMethod( this, "_q_setViewportCursor", Q_ARG(QCursor,itemUnderCursor->cursor()) );
            bGraphicsItemHasCursor = true;
            cursor = pGraphicsItem->cursor();
            break;
        }
    }

    if( bGraphicsItemHasCursor )
    {
        setCursor(cursor);
        viewport()->setCursor(cursor);
    }
    else // if( !bGraphicsItemHasCursor )
    {
        QPointF ptScenePos = mapToScene(ptPos.toPoint());
        QRectF  rctScene = m_pDrawingScene->sceneRect();

        if( rctScene.contains(ptScenePos) )
        {
            QCursor cursor = m_pDrawingScene->getProposedCursor(ptScenePos);
            setCursor(cursor);
            viewport()->setCursor(cursor);
        }
        else
        {
            unsetCursor();
            viewport()->unsetCursor();
        }
    }

} // mouseDoubleClickEvent

/*==============================================================================
public: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs  = "Key:" + qKeyCode2Str(i_pEv->key()) + " (" + i_pEv->text() + ")";
        strMthInArgs += ", Count:" + QString::number(i_pEv->count());
        strMthInArgs += ", IsAutoRepeat:" + QString::number(i_pEv->count());
        strMthInArgs += ", Modifiers:" + qKeyboardModifiers2Str(i_pEv->modifiers());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "keyPressEvent",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsView::keyPressEvent(i_pEv);

} // keyPressEvent

//------------------------------------------------------------------------------
void CDrawingView::keyReleaseEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs  = "Key:" + qKeyCode2Str(i_pEv->key()) + " (" + i_pEv->text() + ")";
        strMthInArgs += ", Count:" + QString::number(i_pEv->count());
        strMthInArgs += ", IsAutoRepeat:" + QString::number(i_pEv->count());
        strMthInArgs += ", Modifiers:" + qKeyboardModifiers2Str(i_pEv->modifiers());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "keyReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsView::keyReleaseEvent(i_pEv);

} // keyReleaseEvent

/*==============================================================================
protected: // overridables of base class QAbstractScrollArea
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::paintEvent( QPaintEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjPaintEvent != nullptr && m_pTrcAdminObjPaintEvent->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs  = "Rect: " + rect2Str(i_pEv->rect());
        //strMthInArgs += ", Region: " + region2Str(i_pEv->region());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaintEvent,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "paintEvent",
        /* strAddInfo   */ strMthInArgs );

    QRect rect = viewport()->rect();

    QColor colBackground = viewport()->palette().color(QPalette::Window);

    QPainter painter;
    painter.begin(viewport());
    painter.setBrush(colBackground);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect);
    painter.end();

    QGraphicsView::paintEvent(i_pEv);

} // paintEvent

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::onSceneDrawingSizeChanged( const CDrawingSize& i_size )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_size.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onSceneDrawingSizeChanged",
        /* strAddInfo   */ strMthInArgs );

    emit_drawingSizeChanged(i_size);
}

/*==============================================================================
protected: // instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::emit_drawingSizeChanged( const ZS::Draw::CDrawingSize& i_size )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_size.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_drawingSizeChanged",
        /* strAddInfo   */ strMthInArgs );
    emit drawingSizeChanged(i_size);
}

//------------------------------------------------------------------------------
void CDrawingView::emit_mousePosChanged( const QPointF& i_ptMousePos )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObjMouseMoveEvent, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = qPoint2Str(i_ptMousePos);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvent,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_mousePosChanged",
        /* strAddInfo   */ strMthInArgs );
    emit mousePosChanged(i_ptMousePos);
}
