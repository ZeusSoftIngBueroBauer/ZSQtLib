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

#include "ZSDraw/Widgets/Drawing/ZSDrawGridLabels.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawingView.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#if QT_VERSION < 0x050000
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qscrollbar.h>
#else
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qscrollbar.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


/*******************************************************************************
class CWdgtGridLabels : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief 
*/
CWdgtGridLabels::CWdgtGridLabels(
    const QString& i_strName,
    CDrawingView* i_pDrawingView,
    EScaleAxis i_scaleAxis,
    ELayoutPos i_layoutPos,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pDrawingView(i_pDrawingView),
    m_divLinesMetrics("DrawingViewGridLabels" + i_strName, i_scaleAxis),
    m_iDivLineLabelsLineLength(5),
    m_scaleAxis(i_scaleAxis),
    m_layoutPos(i_layoutPos),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::Drawing", ClassName(), objectName());
    m_pTrcAdminObjPaintEvent = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::Drawing", ClassName() + "::PaintEvent", objectName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Name: " + i_strName +
            ", ScaleAxis: " + CEnumScaleAxis(i_scaleAxis).toString() +
            ", LayoutPos: " + CEnumLayoutPos(i_layoutPos).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    if (m_scaleAxis == EScaleAxis::X) {
        m_divLinesMetrics = i_pDrawingView->drawingScene()->divLinesMetricsX();
    }
    else {
        m_divLinesMetrics = i_pDrawingView->drawingScene()->divLinesMetricsY();
    }

    QObject::connect(
        m_pDrawingView, &CDrawingView::drawingSizeChanged,
        this, &CWdgtGridLabels::onDrawingViewDrawingSizeChanged);
    QObject::connect(
        m_pDrawingView, &CDrawingView::gridSettingsChanged,
        this, &CWdgtGridLabels::onDrawingViewGridSettingsChanged);
    QObject::connect(
        m_pDrawingView, &CDrawingView::contentAreaChanged,
        this, &CWdgtGridLabels::onDrawingViewContentAreaChanged);

} // ctor

//------------------------------------------------------------------------------
CWdgtGridLabels::~CWdgtGridLabels()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjPaintEvent);

    m_pDrawingView = nullptr;
    //m_divLinesMetrics;
    m_iDivLineLabelsLineLength = 0;
    m_scaleAxis = static_cast<EScaleAxis>(0);
    m_layoutPos = static_cast<ELayoutPos>(0);
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjPaintEvent = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
QSize CWdgtGridLabels::sizeHint() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "sizeHint",
        /* strAddInfo   */ "" );

    QSize sizeRecommended;
    if (m_scaleAxis == EScaleAxis::X) {
        sizeRecommended.setHeight(
            m_divLinesMetrics.getDivLineLabelsMaxTextExtent().height() + m_iDivLineLabelsLineLength + 5);
    }
    else {
        sizeRecommended.setWidth(
            m_divLinesMetrics.getDivLineLabelsMaxTextExtent().width() + m_iDivLineLabelsLineLength + 5);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qSize2Str(sizeRecommended));
    }
    return sizeRecommended;
}

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGridLabels::paintEvent(QPaintEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjPaintEvent, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qPaintEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaintEvent,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "paintEvent",
        /* strAddInfo   */ strMthInArgs );

    QWidget::paintEvent(i_pEv);

    const CDrawGridSettings& gridSettings = m_pDrawingView->gridSettings();
    if (gridSettings.areLabelsVisible()) {
        QPainter painter(this);
        paintGridLabels(&painter);
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGridLabels::onDrawingViewDrawingSizeChanged(const CDrawingSize& i_size)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_size.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingViewDrawingSizeChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_scaleAxis == EScaleAxis::X) {
        m_divLinesMetrics = m_pDrawingView->drawingScene()->divLinesMetricsX();
    }
    else {
        m_divLinesMetrics = m_pDrawingView->drawingScene()->divLinesMetricsY();
    }

    update();
}

//------------------------------------------------------------------------------
void CWdgtGridLabels::onDrawingViewGridSettingsChanged(const CDrawGridSettings& i_settings)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_settings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingViewGridSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_scaleAxis == EScaleAxis::X) {
        m_divLinesMetrics = m_pDrawingView->drawingScene()->divLinesMetricsX();
    }
    else {
        m_divLinesMetrics = m_pDrawingView->drawingScene()->divLinesMetricsY();
    }

    update();
}

//------------------------------------------------------------------------------
void CWdgtGridLabels::onDrawingViewContentAreaChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingViewContentAreaChanged",
        /* strAddInfo   */ "" );

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        QRect rect = m_pDrawingView->geometry();
        QString strAddTrcInfo = "DrawingView.Geometry {" + qRect2Str(rect) + "}";
        mthTracer.trace(strAddTrcInfo);
        rect = m_pDrawingView->viewport()->geometry();
        strAddTrcInfo = "DrawingView.Viewport.Geometry {" + qRect2Str(rect) + "}";
        mthTracer.trace(strAddTrcInfo);
        int iScrollBarValue = m_pDrawingView->verticalScrollBar()->value();
        strAddTrcInfo = "DrawingView.VerticalScrollBar.Value: " + QString::number(iScrollBarValue);
        mthTracer.trace(strAddTrcInfo);
        iScrollBarValue = m_pDrawingView->horizontalScrollBar()->value();
        strAddTrcInfo = "DrawingView.HorizontalScrollBar.Value: " + QString::number(iScrollBarValue);
        mthTracer.trace(strAddTrcInfo);
    }

    update();
}

/*==============================================================================
protected: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGridLabels::paintGridLabels(QPainter* i_pPainter)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaintEvent,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "paintGridLabels",
        /* strAddInfo   */ "" );

    i_pPainter->save();

    // Size of this widget.
    QSize sizeThisWidget = size();

    // Size of the drawing scene.
    QSizeF sizeDrawingScene = m_pDrawingView->sceneRect().size();

    // Rectangles in parent widget.
    QRect rectThisWidget = geometry();
    QRect rectDrawingView = m_pDrawingView->geometry();
    QRect rectDrawingViewport = m_pDrawingView->viewport()->geometry();
    int iDrawingViewFrameWidth = m_pDrawingView->frameWidth();

    // Map viewport geometry rectangle to this widget.
    QPoint ptTL = rectDrawingViewport.topLeft();
    QPoint ptBR = rectDrawingViewport.bottomRight();
    ptTL = m_pDrawingView->viewport()->mapTo(parentWidget(), ptTL);
    ptBR = m_pDrawingView->viewport()->mapTo(parentWidget(), ptBR);
    ptTL = mapFromParent(ptTL);
    ptBR = mapFromParent(ptBR);
    QRect rectDrawingViewportMapped(ptTL, ptBR);

    int iHorScrollBarValue = m_pDrawingView->horizontalScrollBar()->value();
    int iVerScrollBarValue = m_pDrawingView->verticalScrollBar()->value();

    int iZoomFactor_perCent = m_pDrawingView->zoomFactorInPerCent();

    const CDrawingSize& drawingSize = m_pDrawingView->drawingSize();
    const CDrawGridSettings& gridSettings = m_pDrawingView->gridSettings();

    const EDivLineLayer eLayer = EDivLineLayer::Main;

    // Lines from drawing scene to labels
    // ----------------------------------

    QPen pen(gridSettings.linesColor());
    //pen.setStyle(lineStyle2QtPenStyle(gridSettings.linesStyle().enumerator()));
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(gridSettings.linesWidth());
    i_pPainter->setPen(pen);

    if (m_scaleAxis == EScaleAxis::X) {
        for (int idxDivLine = 0; idxDivLine < m_divLinesMetrics.getDivLinesCount(EDivLineLayer::Main); ++idxDivLine ) {
            int xDivLine = m_divLinesMetrics.getDivLineInPix(EDivLineLayer::Main, idxDivLine);
            int xOffset = (iZoomFactor_perCent * xDivLine) / 100 - iHorScrollBarValue;
            int x = rectDrawingViewportMapped.left() + xOffset;
            if (x >= rectDrawingViewportMapped.left() && x <= rectDrawingViewportMapped.right()) {
                x -= 1; // iDrawingViewFrameWidth;
                if (m_layoutPos == ELayoutPos::Top) {
                    i_pPainter->drawLine(x, (sizeThisWidget.height()-1), x, (sizeThisWidget.height()-1) - m_iDivLineLabelsLineLength);
                }
                else if (m_layoutPos == ELayoutPos::Bottom) {
                    i_pPainter->drawLine(x, 0, x, m_iDivLineLabelsLineLength);
                }
            }
        }
    }
    else {
        for (int idxDivLine = 0; idxDivLine < m_divLinesMetrics.getDivLinesCount(EDivLineLayer::Main); ++idxDivLine ) {
            int yDivLine = m_divLinesMetrics.getDivLineInPix(EDivLineLayer::Main, idxDivLine);
            int yOffset = (iZoomFactor_perCent * yDivLine) / 100 - iVerScrollBarValue;
            int y = rectDrawingViewportMapped.top() + yOffset;
            if (y >= rectDrawingViewportMapped.top() && y <= rectDrawingViewportMapped.bottom()) {
                y -= 1; // iDrawingViewFrameWidth;
                if (m_layoutPos == ELayoutPos::Left) {
                    i_pPainter->drawLine((sizeThisWidget.width()-1) - m_iDivLineLabelsLineLength, y, (sizeThisWidget.width()-1), y);
                }
                else if (m_layoutPos == ELayoutPos::Right) {
                    i_pPainter->drawLine(0, y, m_iDivLineLabelsLineLength, y);
                }
            }
        }
    }

    // Labels and unit string
    // ----------------------

    QFont font = gridSettings.labelsFont();
    CEnumTextStyle textStyle = gridSettings.labelsTextStyle();
    CEnumTextEffect textEffect = gridSettings.labelsTextEffect();
    ETextSize textSize = gridSettings.labelsTextSize();

    if (isTextStyleBold(textStyle)) {
        font.setBold(true);
    }
    if (isTextStyleItalic(textStyle)) {
        font.setItalic(true);
    }
    if (isTextEffectUnderline(textEffect)) {
        font.setUnderline(true);
    }
    if (isTextEffectStrikeout(textEffect)) {
        font.setStrikeOut(true);
    }
    font.setPixelSize(textSize2SizeInPixels(textSize));

    i_pPainter->setPen(gridSettings.labelsTextColor());
    i_pPainter->setFont(font);

    QFontMetrics fntmtr(gridSettings.labelsFont());
    QSize sizeUnitString;
    QString strUnit;
    if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
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

    if (m_scaleAxis == EScaleAxis::X) {
        QRect rectXScaleMax = m_divLinesMetrics.getScaleMaxValBoundingRect();
        rectXScaleMax.moveLeft(rectDrawingViewportMapped.right() - rectXScaleMax.width()/2);
        if (m_layoutPos == ELayoutPos::Top) {
            rectXScaleMax.moveTop((sizeThisWidget.height()-1) - rectXScaleMax.height() - m_iDivLineLabelsLineLength);
        }
        else if (m_layoutPos == ELayoutPos::Bottom) {
            rectXScaleMax.moveTop(m_iDivLineLabelsLineLength);
        }
        QPoint ptCenterXScaleMax = rectXScaleMax.center();
        rectXScaleMax.setWidth(sizeUnitString.width());
        rectXScaleMax.setHeight(sizeUnitString.height());
        rectXScaleMax.moveCenter(ptCenterXScaleMax);
        i_pPainter->drawText(rectXScaleMax, Qt::AlignVCenter|Qt::AlignHCenter, strUnit);

        for (int idxDivLine = 0; idxDivLine < m_divLinesMetrics.getDivLinesCount(eLayer); idxDivLine++) {
            if (m_divLinesMetrics.isDivLineLabelVisible(eLayer, idxDivLine)) {
                QString strDivLineLabel;
                strDivLineLabel = m_divLinesMetrics.getDivLineLabelText(eLayer, idxDivLine);
                if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
                    int x = static_cast<int>(strDivLineLabel.toDouble());
                    strDivLineLabel = QString::number(x);
                }
                QRect rectDivLineLabel = m_divLinesMetrics.getDivLineLabelBoundingRect(eLayer, idxDivLine);
                QRect rect = rectDivLineLabel;
                int xOffset = (iZoomFactor_perCent * rectDivLineLabel.center().x()) / 100 - iHorScrollBarValue;
                rect.moveCenter(QPoint(rectDrawingViewportMapped.left() + xOffset, rect.center().y()));
                if (rect.right() >= rectDrawingViewportMapped.left() && rect.left() <= rectDrawingViewportMapped.right()) {
                    if (m_layoutPos == ELayoutPos::Top) {
                        rect.moveTop((sizeThisWidget.height()-1) - rectDivLineLabel.height() - m_iDivLineLabelsLineLength);
                    }
                    else if (m_layoutPos == ELayoutPos::Bottom) {
                        rect.moveTop(m_iDivLineLabelsLineLength);
                    }
                    if (!rect.intersects(rectXScaleMax)) {
                        i_pPainter->drawText(rect, Qt::AlignVCenter|Qt::AlignHCenter, strDivLineLabel);
                    }
                }
            }
        }
    } // if (m_scaleAxis == EScaleAxis::X)

    else /* if (m_scaleAxis == EScaleAxis::Y) */ {
        QRect rectYScaleMax = m_divLinesMetrics.getScaleMaxValBoundingRect();
        rectYScaleMax.setWidth(sizeUnitString.width());
        rectYScaleMax.setHeight(sizeUnitString.height());
        if (m_layoutPos == ELayoutPos::Left) {
            rectYScaleMax.moveRight((sizeThisWidget.width()-1) - m_iDivLineLabelsLineLength - 2);
        }
        else if (m_layoutPos == ELayoutPos::Right) {
            rectYScaleMax.moveLeft(m_iDivLineLabelsLineLength + 2);
        }
        rectYScaleMax.moveTop(rectDrawingViewportMapped.bottom() - rectYScaleMax.height()/2);
        i_pPainter->drawText(rectYScaleMax, Qt::AlignVCenter|Qt::AlignRight, strUnit);

        for (int idxDivLine = 0; idxDivLine < m_divLinesMetrics.getDivLinesCount(eLayer); idxDivLine++) {
            if (m_divLinesMetrics.isDivLineLabelVisible(eLayer, idxDivLine)) {
                QString strDivLineLabel;
                strDivLineLabel = m_divLinesMetrics.getDivLineLabelText(eLayer, idxDivLine);
                if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
                    int y = static_cast<int>(strDivLineLabel.toDouble());
                    strDivLineLabel = QString::number(y);
                }
                QRect rectDivLineLabel = m_divLinesMetrics.getDivLineLabelBoundingRect(eLayer, idxDivLine);
                QRect rect = rectDivLineLabel;
                if (m_layoutPos == ELayoutPos::Left) {
                    rect.moveLeft((sizeThisWidget.width()-1) - rectDivLineLabel.width() - m_iDivLineLabelsLineLength - 2);
                }
                else if (m_layoutPos == ELayoutPos::Right) {
                    rect.moveLeft(m_iDivLineLabelsLineLength + 2);
                }
                int yOffset = (iZoomFactor_perCent * rectDivLineLabel.center().y()) / 100 - iVerScrollBarValue;
                rect.moveCenter(QPoint(rect.center().x(), rectDrawingViewportMapped.top() + yOffset));
                if (rect.bottom() >= rectDrawingViewportMapped.top() && rect.top() <= rectDrawingViewportMapped.bottom()) {
                    if (!rect.intersects(rectYScaleMax)) {
                        if (m_layoutPos == ELayoutPos::Left) {
                            i_pPainter->drawText(rect, Qt::AlignVCenter|Qt::AlignRight, strDivLineLabel);
                        }
                        else if (m_layoutPos == ELayoutPos::Right) {
                            i_pPainter->drawText(rect, Qt::AlignVCenter|Qt::AlignLeft, strDivLineLabel);
                        }
                    }
                }
            }
        }
    } // if (m_scaleAxis == EScaleAxis::Y)

    i_pPainter->restore();

} // paintGridLabels
