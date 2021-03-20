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

#include <QtGui/qevent.h>

#include "ZSDraw/ZSDrawingView.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSDraw/ZSDrawGraphObj.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;
using namespace ZS::Trace;


/*******************************************************************************
class CDrawingView : public QGraphicsView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDrawingView::CDrawingView(
    CDrawingScene* i_pDrawingScene,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QGraphicsView(i_pDrawingScene,i_pWdgtParent),
    m_pDrawingScene(i_pDrawingScene),
    m_pageSetup(),
    m_pTrcAdminObj(nullptr)
{
    double fXResolution_dpmm = logicalDpiX() / 25.4; // dots per milli meter
    double fYResolution_dpmm = logicalDpiY() / 25.4; // dots per milli meter

    m_pDrawingScene->setXResolutionInDpmm(fXResolution_dpmm);
    m_pDrawingScene->setYResolutionInDpmm(fYResolution_dpmm);

    setObjectName("DrawingView");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Draw", "CDrawingView", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    setPageSetup(m_pageSetup);

    setMouseTracking(true);
    setAcceptDrops(true);

} // ctor

//------------------------------------------------------------------------------
CDrawingView::~CDrawingView()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    //saveSettings();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pDrawingScene = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
double CDrawingView::getXResolutionInDpmm()
//------------------------------------------------------------------------------
{
    return m_pDrawingScene->getXResolutionInDpmm();
}

//------------------------------------------------------------------------------
double CDrawingView::getYResolutionInDpmm()
//------------------------------------------------------------------------------
{
    return m_pDrawingScene->getYResolutionInDpmm();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::setPageSetup( const CPageSetup& i_pageSetup )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setPageSetup",
        /* strAddInfo   */ "" );

    m_pageSetup = i_pageSetup;

    int cxDrawingWidth_px  = m_pageSetup.getDrawingWidthInPixels();
    int cxDrawingHeight_px = m_pageSetup.getDrawingHeightInPixels();

    m_pDrawingScene->setSceneRect( QRectF(0,0,cxDrawingWidth_px,cxDrawingHeight_px) );

    emit pageSetupChanged(m_pageSetup);

} // setPageSetup

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        #if QT_VERSION < 0x050000
        strAddTrcInfo = "Pos:(" + QString::number(i_pEv->posF().x()) + "," + QString::number(i_pEv->posF().y()) + ")";
        #else
        strAddTrcInfo = "Pos:(" + QString::number(i_pEv->windowPos().x()) + "," + QString::number(i_pEv->windowPos().y()) + ")";
        #endif
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QPointF ptPos = i_pEv->pos();

    emit mousePosChanged(ptPos);

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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelInternalStates) )
    {
        #if QT_VERSION < 0x050000
        strAddTrcInfo = "Pos:(" + QString::number(i_pEv->posF().x()) + "," + QString::number(i_pEv->posF().y()) + ")";
        #else
        strAddTrcInfo = "Pos:(" + QString::number(i_pEv->windowPos().x()) + "," + QString::number(i_pEv->windowPos().y()) + ")";
        #endif
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ 2,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QPointF ptPos = i_pEv->pos();

    emit mousePosChanged(ptPos);

    QGraphicsView::mouseMoveEvent(i_pEv);

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

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CDrawingView::mouseReleaseEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        #if QT_VERSION < 0x050000
        strAddTrcInfo = "Pos:(" + QString::number(i_pEv->posF().x()) + "," + QString::number(i_pEv->posF().y()) + ")";
        #else
        strAddTrcInfo = "Pos:(" + QString::number(i_pEv->windowPos().x()) + "," + QString::number(i_pEv->windowPos().y()) + ")";
        #endif
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QPointF ptPos = i_pEv->pos();

    emit mousePosChanged(ptPos);

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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        #if QT_VERSION < 0x050000
        strAddTrcInfo = "Pos:(" + QString::number(i_pEv->posF().x()) + "," + QString::number(i_pEv->posF().y()) + ")";
        #else
        strAddTrcInfo = "Pos:(" + QString::number(i_pEv->windowPos().x()) + "," + QString::number(i_pEv->windowPos().y()) + ")";
        #endif
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QPointF ptPos = i_pEv->pos();

    emit mousePosChanged(ptPos);

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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Key:" + qKeyCode2Str(i_pEv->key()) + " (" + i_pEv->text() + ")";
        strAddTrcInfo += ", Count:" + QString::number(i_pEv->count());
        strAddTrcInfo += ", IsAutoRepeat:" + QString::number(i_pEv->count());
        strAddTrcInfo += ", Modifiers:" + qKeyboardModifiers2Str(i_pEv->modifiers());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "keyPressEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QGraphicsView::keyPressEvent(i_pEv);

} // keyPressEvent

//------------------------------------------------------------------------------
void CDrawingView::keyReleaseEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Key:" + qKeyCode2Str(i_pEv->key()) + " (" + i_pEv->text() + ")";
        strAddTrcInfo += ", Count:" + QString::number(i_pEv->count());
        strAddTrcInfo += ", IsAutoRepeat:" + QString::number(i_pEv->count());
        strAddTrcInfo += ", Modifiers:" + qKeyboardModifiers2Str(i_pEv->modifiers());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "keyReleaseEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QGraphicsView::keyReleaseEvent(i_pEv);

} // keyReleaseEvent

/*==============================================================================
protected: // overridables of base class QAbstractScrollArea
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::paintEvent( QPaintEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QGraphicsView::paintEvent(i_pEv);
}
