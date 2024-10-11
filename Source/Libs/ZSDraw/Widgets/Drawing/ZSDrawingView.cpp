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

#include "ZSDraw/Widgets/Drawing/ZSDrawingView.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#if QT_VERSION < 0x050000
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#else
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDrawingView : public QGraphicsView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates the graphics view.

    @note The drawing size instance created by the drawing scene accesses the
          the unit to get the screen resolution. To get the screen resolution
          the screen resolution must be set before. The screen resolution must
          therefore be set at the draw units BEFORE creating the drawing scene.
*/
CDrawingView::CDrawingView( CDrawingScene* i_pDrawingScene, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QGraphicsView(i_pDrawingScene, i_pWdgtParent),
    m_pDrawingScene(i_pDrawingScene),
    m_iZoomFactor_perCent(100),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjCursor(nullptr),
    m_pTrcAdminObjPaint(nullptr),
    m_pTrcAdminObjMouseClickEvents(nullptr),
    m_pTrcAdminObjMouseMoveEvents(nullptr),
    m_pTrcAdminObjKeyEvents(nullptr)
{
    setObjectName("theInst");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::Drawing", ClassName(), objectName());
    m_pTrcAdminObjCursor = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::Drawing", ClassName() + "::Cursor", objectName());
    m_pTrcAdminObjPaint = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::Drawing", ClassName() + "::Paint", objectName());
    m_pTrcAdminObjMouseClickEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::Drawing", ClassName() + "::MouseClickEvents", objectName());
    m_pTrcAdminObjMouseMoveEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::Drawing", ClassName() + "::MouseMoveEvents", objectName());
    m_pTrcAdminObjKeyEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::Drawing", ClassName() + "::KeyEvents", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // The drawing size instance created by the drawing scene accesses the
    // the unit to get the screen resolution. To get the screen resolution
    // the screen resolution must be set before. Here it is too late as
    // the drawing scene is already created. It must be done by the creator
    // of the drawing view.
    //Units.Length.setPxpis(logicalDpiX(), logicalDpiY());
    //Units.Length.setDpis(physicalDpiX(), physicalDpiY());

    // When using a background brush, DrawingScene::drawBackground will not be called anymore.
    //setBackgroundBrush(Qt::yellow);
    //setAutoFillBackground(true);
    setStyleSheet("border-style: None");

    CDrawingSize drawingSize;
    drawingSize.setImageSize(CPhysVal(800, Units.Length.px), CPhysVal(600, Units.Length.px));
    m_pDrawingScene->setDrawingSize(drawingSize);

    setMouseTracking(true);
    setAcceptDrops(true);

    QObject::connect(
        m_pDrawingScene, &CDrawingScene::drawingSizeChanged,
        this, &CDrawingView::onSceneDrawingSizeChanged );
    QObject::connect(
        m_pDrawingScene, &CDrawingScene::gridSettingsChanged,
        this, &CDrawingView::onSceneGridSettingsChanged );

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

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjCursor);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjPaint);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjMouseClickEvents);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjMouseMoveEvents);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjKeyEvents);

    m_pDrawingScene = nullptr;
    m_iZoomFactor_perCent = 0;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjCursor = nullptr;
    m_pTrcAdminObjPaint = nullptr;
    m_pTrcAdminObjMouseClickEvents = nullptr;
    m_pTrcAdminObjMouseMoveEvents = nullptr;
    m_pTrcAdminObjKeyEvents = nullptr;

} // dtor

/*==============================================================================
public: // instance methods (drawing area)
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::setDrawingSize( const CDrawingSize& i_size )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
const CDrawingSize& CDrawingView::drawingSize() const
//------------------------------------------------------------------------------
{
    return m_pDrawingScene->drawingSize();
}

/*==============================================================================
public: // instance methods (drawing area)
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::setZoomFactorInPerCent( int i_iFactor_perCent )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iFactor_perCent);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setZoomFactorInPerCent",
        /* strAddInfo   */ strMthInArgs );

    if (m_iZoomFactor_perCent != i_iFactor_perCent) {
        double fScale = static_cast<double>(i_iFactor_perCent) / static_cast<double>(m_iZoomFactor_perCent);
        m_iZoomFactor_perCent = i_iFactor_perCent;
        scale(fScale, fScale);
        emit_contentAreaChanged();
    }
}

//------------------------------------------------------------------------------
int CDrawingView::zoomFactorInPerCent() const
//------------------------------------------------------------------------------
{
    return m_iZoomFactor_perCent;
}

/*==============================================================================
public: // instance methods (grid lines with labels)
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::setGridSettings( const CDrawGridSettings& i_settings )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_settings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridSettings",
        /* strAddInfo   */ strMthInArgs );

    m_pDrawingScene->setGridSettings(i_settings);
}

//------------------------------------------------------------------------------
const CDrawGridSettings& CDrawingView::gridSettings() const
//------------------------------------------------------------------------------
{
    return m_pDrawingScene->gridSettings();
}

/*==============================================================================
public: // instance methods (grid lines with labels)
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::setDrawSettings( const CDrawSettings& i_settings )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_settings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDrawSettings",
        /* strAddInfo   */ strMthInArgs );

    m_pDrawingScene->setDrawSettings(i_settings);
}

//------------------------------------------------------------------------------
const CDrawSettings& CDrawingView::drawSettings() const
//------------------------------------------------------------------------------
{
    return m_pDrawingScene->drawSettings();
}

/*==============================================================================
public: // reimplemented method of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::setCursor(const QCursor& i_cursor)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCursor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qCursor2Str(i_cursor);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCursor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCursor",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        QString strMthInArgs = "CurrentCursor: " + qCursor2Str(cursor());
        mthTracer.trace(strMthInArgs);
    }

    QGraphicsView::setCursor(i_cursor);
    viewport()->setCursor(i_cursor);
}

//------------------------------------------------------------------------------
void CDrawingView::unsetCursor()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCursor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "unsetCursor",
        /* strAddInfo   */ "" );
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        QString strMthInArgs = "CurrentCursor: " + qCursor2Str(cursor());
        mthTracer.trace(strMthInArgs);
    }

    QGraphicsView::unsetCursor();
    viewport()->unsetCursor();
}

/*==============================================================================
public: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strMthInArgs );
    emit_mousePosChanged(i_pEv->pos());

    // Before dispatching the mouse event to the drawing scene the drawing scene
    // will be asked for its proposed cursor which depends on whether which (if any)
    // drawing tool has been selected. Setting the cursor by the drawing view after
    // dispatching the mouse event would overwrite the cursor set by the graphics
    // items in hoverEvents.
    QPointF ptScenePos = mapToScene(i_pEv->pos());
    QRectF rctScene = m_pDrawingScene->sceneRect();
    if (rctScene.contains(ptScenePos)) {
        setCursor(m_pDrawingScene->getProposedCursor(ptScenePos));
    }
    else {
        unsetCursor();
    }
    QGraphicsView::mousePressEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CDrawingView::mouseMoveEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseMoveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );
    emit_mousePosChanged(i_pEv->pos());

    // Before dispatching the mouse event to the drawing scene the drawing scene
    // will be asked for its proposed cursor which depends on whether which (if any)
    // drawing tool has been selected. Setting the cursor by the drawing view after
    // dispatching the mouse event would overwrite the cursor set by the graphics
    // items in hoverEvents.
    QPointF ptScenePos = mapToScene(i_pEv->pos());
    QRectF rctScene = m_pDrawingScene->sceneRect();
    if (rctScene.contains(ptScenePos)) {
        setCursor(m_pDrawingScene->getProposedCursor(ptScenePos));
    }
    else {
        unsetCursor();
    }
    QGraphicsView::mouseMoveEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CDrawingView::mouseReleaseEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );
    emit_mousePosChanged(i_pEv->pos());

    // Before dispatching the mouse event to the drawing scene the drawing scene
    // will be asked for its proposed cursor which depends on whether which (if any)
    // drawing tool has been selected. Setting the cursor by the drawing view after
    // dispatching the mouse event would overwrite the cursor set by the graphics
    // items in hoverEvents.
    QPointF ptScenePos = mapToScene(i_pEv->pos());
    QRectF rctScene = m_pDrawingScene->sceneRect();
    if (rctScene.contains(ptScenePos)) {
        setCursor(m_pDrawingScene->getProposedCursor(ptScenePos));
    }
    else {
        unsetCursor();
    }
    QGraphicsView::mouseReleaseEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CDrawingView::mouseDoubleClickEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strMthInArgs );
    emit_mousePosChanged(i_pEv->pos());

    // Before dispatching the mouse event to the drawing scene the drawing scene
    // will be asked for its proposed cursor which depends on whether which (if any)
    // drawing tool has been selected. Setting the cursor by the drawing view after
    // dispatching the mouse event would overwrite the cursor set by the graphics
    // items in hoverEvents.
    QPointF ptScenePos = mapToScene(i_pEv->pos());
    QRectF rctScene = m_pDrawingScene->sceneRect();
    if (rctScene.contains(ptScenePos)) {
        setCursor(m_pDrawingScene->getProposedCursor(ptScenePos));
    }
    else {
        unsetCursor();
    }
    QGraphicsView::mouseDoubleClickEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

/*==============================================================================
public: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjKeyEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qKeyEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "keyPressEvent",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsView::keyPressEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CDrawingView::keyReleaseEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjKeyEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qKeyEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "keyReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsView::keyReleaseEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

/*==============================================================================
protected: // overridables of base class QAbstractScrollArea
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::scrollContentsBy( int i_dx, int i_dy )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Dx: " + QString::number(i_dx) + ", Dy: " + QString::number(i_dy);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "scrollContentsBy",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsView::scrollContentsBy(i_dx, i_dy);

    emit_contentAreaChanged();

    // Workaround for bug in Qt? Without invalidate here some regions
    // are not updated and the grid for example has gaps.
    m_pDrawingScene->invalidate();
}

//------------------------------------------------------------------------------
void CDrawingView::resizeEvent( QResizeEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qResizeEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "resizeEvent",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsView::resizeEvent(i_pEv);

    emit_contentAreaChanged();

    // Workaround for bug in Qt? Without invalidate here some regions
    // are not updated and the grid for example has gaps.
    m_pDrawingScene->invalidate();

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::onSceneDrawingSizeChanged( const CDrawingSize& i_size )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_size.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onSceneDrawingSizeChanged",
        /* strAddInfo   */ strMthInArgs );

    viewport()->update();

    emit_drawingSizeChanged(i_size);
}

//------------------------------------------------------------------------------
void CDrawingView::onSceneGridSettingsChanged( const CDrawGridSettings& i_settings )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_settings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onSceneGridSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    viewport()->update();

    emit_gridSettingsChanged(i_settings);
}

/*==============================================================================
protected: // instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::emit_drawingSizeChanged( const ZS::Draw::CDrawingSize& i_size )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
void CDrawingView::emit_gridSettingsChanged( const ZS::Draw::CDrawGridSettings& i_settings )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_settings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_gridSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    emit gridSettingsChanged(i_settings);
}

//------------------------------------------------------------------------------
void CDrawingView::emit_mousePosChanged( const QPointF& i_ptMousePos )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObjMouseMoveEvents, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = qPoint2Str(i_ptMousePos);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_mousePosChanged",
        /* strAddInfo   */ strMthInArgs );

    emit mousePosChanged(i_ptMousePos);
}

//------------------------------------------------------------------------------
void CDrawingView::emit_contentAreaChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_contentAreaChanged",
        /* strAddInfo   */ "" );

    emit contentAreaChanged();
}
