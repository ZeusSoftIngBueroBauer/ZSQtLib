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

#include "ZSDraw/Widgets/Drawing/ZSDrawingView.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
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
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjMouseMoveEvent(nullptr),
    m_pTrcAdminObjPaintEvent(nullptr)
{
    setObjectName("theInst");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::Drawing", ClassName(), objectName());
    m_pTrcAdminObjMouseMoveEvent = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::Drawing", ClassName() + "::MouseMoveEvent", objectName());
    m_pTrcAdminObjPaintEvent = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::Drawing", ClassName() + "::PaintEvent", objectName());

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

    CDrawingSize drawingSize("DrawingScene");
    drawingSize.setImageSize(CPhysVal(1024, Units.Length.px), CPhysVal(768, Units.Length.px));
    m_pDrawingScene->setDrawingSize(drawingSize);

    //QWidget* pWdgtViewPort = new QWidget();
    //pWdgtViewPort->setBackgroundRole(QPalette::ToolTipBase);
    //QVBoxLayout* pLytViewPort = new QVBoxLayout();
    //pWdgtViewPort->setLayout(pLytViewPort);
    //QLabel* pLblHelloWorld = new QLabel("Hello World");
    //pLytViewPort->addWidget(pLblHelloWorld);

    //setViewport(pWdgtViewPort);

    //setViewportMargins(50.0, 50.0, 50.0, 50.0);

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
CDrawingSize CDrawingView::drawingSize() const
//------------------------------------------------------------------------------
{
    return m_pDrawingScene->drawingSize();
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
CDrawGridSettings CDrawingView::gridSettings() const
//------------------------------------------------------------------------------
{
    return m_pDrawingScene->gridSettings();
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

    if (areMethodCallsActive(m_pTrcAdminObjMouseMoveEvent, EMethodTraceDetailLevel::ArgsNormal))
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

    if (areMethodCallsActive(m_pTrcAdminObjPaintEvent, EMethodTraceDetailLevel::ArgsNormal))
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

    CDrawGridSettings gridSettings = m_pDrawingScene->gridSettings();

    if (gridSettings.areLabelsVisible()) {
        paintGridLabels(&painter);
    }
    painter.end();

    QGraphicsView::paintEvent(i_pEv);

} // paintEvent

/*==============================================================================
protected: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingView::paintGridLabels(QPainter* i_pPainter)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaintEvent,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "paintGridLabels",
        /* strAddInfo   */ "" );

    i_pPainter->save();

    QRectF rectScene = mapFromScene(sceneRect()).boundingRect();

    CDrawingSize drawingSize = m_pDrawingScene->drawingSize();
    CDrawGridSettings gridSettings = m_pDrawingScene->gridSettings();

    const GUI::Math::CScaleDivLinesMetrics& divLinesMetricsX = m_pDrawingScene->divLinesMetricsX();
    const GUI::Math::CScaleDivLinesMetrics& divLinesMetricsY = m_pDrawingScene->divLinesMetricsY();

    EDivLineLayer eLayer = EDivLineLayer::Main;

    // Lines from drawing scene to labels
    // ----------------------------------

    QPen pen(gridSettings.linesColor());
    pen.setStyle(lineStyle2QtPenStyle(gridSettings.linesStyle().enumerator()));
    pen.setWidth(gridSettings.linesWidth());
    i_pPainter->setPen(pen);

    // X Axis
    for (int idxDivLine = 0; idxDivLine < divLinesMetricsX.getDivLinesCount(EDivLineLayer::Main); ++idxDivLine ) {
        int x = rectScene.left() + divLinesMetricsX.getDivLineInPix(EDivLineLayer::Main, idxDivLine);
        if (drawingSize.dimensionUnit() == EDrawingDimensionUnit::Pixels) {
            i_pPainter->drawLine(x, rectScene.top(), x, rectScene.top() - 5);
        }
        else {
            i_pPainter->drawLine(x, rectScene.bottom(), x, rectScene.bottom() + 5);
        }
    }

    // Y Axis
    for (int idxDivLine = 0; idxDivLine < divLinesMetricsY.getDivLinesCount(EDivLineLayer::Main); ++idxDivLine ) {
        int y = rectScene.top() + divLinesMetricsY.getDivLineInPix(EDivLineLayer::Main, idxDivLine);
        i_pPainter->drawLine(rectScene.left(), y, rectScene.left() - 5, y);
    }

    // Labels and unit string
    // ----------------------

    i_pPainter->setPen(gridSettings.labelsTextColor());
    i_pPainter->setFont(gridSettings.labelsFont());

    QFontMetrics fntmtr(gridSettings.labelsFont());
    QSize sizeUnitString;
    QString strUnit;
    if (drawingSize.dimensionUnit() == EDrawingDimensionUnit::Pixels) {
        strUnit = "Px";
    }
    else {
        strUnit = drawingSize.metricUnit().symbol();
     }
    sizeUnitString = fntmtr.boundingRect(strUnit).size();
    sizeUnitString.setHeight(sizeUnitString.height() + 2);
    sizeUnitString.setWidth(sizeUnitString.width() + 2);

    QRect rectDivLineLabelsPhysUnit = QRect(
        0, 0, sizeUnitString.width(), sizeUnitString.height());

    // X Axis
    QRect rectXScaleMax = divLinesMetricsX.getScaleMaxValBoundingRect();
    rectXScaleMax.moveLeft(rectScene.left() + rectXScaleMax.left());
    if (drawingSize.dimensionUnit() == EDrawingDimensionUnit::Pixels) {
        rectXScaleMax.moveTop(rectScene.top() - rectXScaleMax.height() - 5);
    }
    else {
        rectXScaleMax.moveTop(rectScene.bottom() + 5);
    }
    QPoint ptCenterXScaleMax = rectXScaleMax.center();
    rectXScaleMax.setWidth(sizeUnitString.width());
    rectXScaleMax.setHeight(sizeUnitString.height());
    rectXScaleMax.moveCenter(ptCenterXScaleMax);
    //i_pPainter->drawRect(rectXScaleMax);
    i_pPainter->drawText(rectXScaleMax, Qt::AlignVCenter|Qt::AlignHCenter, strUnit);

    for (int idxDivLine = 0; idxDivLine < divLinesMetricsX.getDivLinesCount(eLayer); idxDivLine++)
    {
        if (divLinesMetricsX.isDivLineLabelVisible(eLayer, idxDivLine))
        {
            QString strDivLineLabel;
            if (drawingSize.dimensionUnit() == EDrawingDimensionUnit::Pixels) {
                int x = divLinesMetricsX.getDivLineInPix(EDivLineLayer::Main, idxDivLine);
                strDivLineLabel = QString::number(x);
            }
            else {
                strDivLineLabel = divLinesMetricsX.getDivLineLabelText(eLayer, idxDivLine);
            }
            QRect rectDivLineLabel = divLinesMetricsX.getDivLineLabelBoundingRect(eLayer, idxDivLine);
            QRect rect = rectDivLineLabel;
            rect.moveLeft(rectScene.left() + rectDivLineLabel.left());
            if (drawingSize.dimensionUnit() == EDrawingDimensionUnit::Pixels) {
                rect.moveTop(rectScene.top() - rectDivLineLabel.height() - 5);
            }
            else {
                rect.moveTop(rectScene.bottom() + 5);
            }
            if (!rect.intersects(rectXScaleMax)) {
                i_pPainter->drawText(rect, Qt::AlignVCenter|Qt::AlignHCenter, strDivLineLabel);
            }
        }
    }

    // Y Axis
    QRect rectYScaleMax = divLinesMetricsY.getScaleMaxValBoundingRect();
    rectYScaleMax.setWidth(sizeUnitString.width());
    rectYScaleMax.setHeight(sizeUnitString.height());
    rectYScaleMax.moveRight(rectScene.left() - 7);
    rectYScaleMax.moveTop(rectScene.top() + rectYScaleMax.top());
    //i_pPainter->drawRect(rectYScaleMax);
    i_pPainter->drawText(rectYScaleMax, Qt::AlignVCenter|Qt::AlignRight, strUnit);

    for (int idxDivLine = 0; idxDivLine < divLinesMetricsY.getDivLinesCount(eLayer); idxDivLine++)
    {
        if (divLinesMetricsY.isDivLineLabelVisible(eLayer, idxDivLine))
        {
            QString strDivLineLabel;
            if (drawingSize.dimensionUnit() == EDrawingDimensionUnit::Pixels) {
                int y = divLinesMetricsY.getDivLineInPix(EDivLineLayer::Main, idxDivLine);
                strDivLineLabel = QString::number(y);
            }
            else {
                strDivLineLabel = divLinesMetricsY.getDivLineLabelText(eLayer, idxDivLine);
            }
            QRect rectDivLineLabel = divLinesMetricsY.getDivLineLabelBoundingRect(eLayer, idxDivLine);
            QRect rect = rectDivLineLabel;
            rect.moveLeft(rectScene.left() - rectDivLineLabel.width() - 7);
            rect.moveTop(rectScene.top() + rectDivLineLabel.top());
            if (!rect.intersects(rectYScaleMax)) {
                i_pPainter->drawText(rect, Qt::AlignVCenter|Qt::AlignRight, strDivLineLabel);
            }
        }
    }

    i_pPainter->restore();

} // paintGridLabels

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
