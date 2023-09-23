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
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

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
    EScaleDir i_scaleDir,
    ELayoutPos i_layoutPos,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pDrawingView(i_pDrawingView),
    m_divLinesMetrics("GridLabels" + i_strName, i_scaleDir),
    m_scaleDir(i_scaleDir),
    m_layoutPos(i_layoutPos),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::Drawing", ClassName(), objectName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Name: " + i_strName +
            ", ScaleDir: " + CEnumScaleDir(i_scaleDir).toString() +
            ", LayoutPos: " + CEnumLayoutPos(i_layoutPos).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    if (m_scaleDir == EScaleDir::X) {
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

    //m_divLinesMetrics;
    m_pDrawingView = nullptr;
    m_scaleDir = static_cast<EScaleDir>(0);
    m_layoutPos = static_cast<ELayoutPos>(0);
    m_pTrcAdminObj = nullptr;

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
    if (m_scaleDir == EScaleDir::X) {
        sizeRecommended.setHeight(20);
    }
    else {
        sizeRecommended.setWidth(40);
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
    QWidget::paintEvent(i_pEv);
    QPainter painter(this);
    paintGridLabels(&painter);
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

    if (m_scaleDir == EScaleDir::X) {
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

    if (m_scaleDir == EScaleDir::X) {
        m_divLinesMetrics = m_pDrawingView->drawingScene()->divLinesMetricsX();
    }
    else {
        m_divLinesMetrics = m_pDrawingView->drawingScene()->divLinesMetricsY();
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
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "paintGridLabels",
        /* strAddInfo   */ "" );

    const CDrawGridSettings& gridSettings = m_pDrawingView->gridSettings();

    if (gridSettings.areLabelsVisible())
    {
        i_pPainter->save();

        // Rectangles in parent widget.
        QRect rectGridLabelsWidget = geometry();
        QRect rectDrawingView = m_pDrawingView->geometry();
        int iDrawingViewFrameWidth = m_pDrawingView->frameWidth();

        // Size and rectangle of this widget.
        QSize sizeWidget = rectGridLabelsWidget.size();

        const CDrawingSize& drawingSize = m_pDrawingView->drawingSize();

        const EDivLineLayer eLayer = EDivLineLayer::Main;

        // Lines from drawing scene to labels
        // ----------------------------------

        QPen pen(gridSettings.linesColor());
        pen.setStyle(lineStyle2QtPenStyle(gridSettings.linesStyle().enumerator()));
        pen.setWidth(gridSettings.linesWidth());
        i_pPainter->setPen(pen);
        int iLineLength = 5;

        if (m_scaleDir == EScaleDir::X) {
            // Offset between geometries in parent widget of this labels widget to the drawing view.
            int xOffset = rectGridLabelsWidget.left() - iDrawingViewFrameWidth;
            for (int idxDivLine = 0; idxDivLine < m_divLinesMetrics.getDivLinesCount(EDivLineLayer::Main); ++idxDivLine ) {
                int xDivLine = m_divLinesMetrics.getDivLineInPix(EDivLineLayer::Main, idxDivLine);
                int x = rectDrawingView.left() - xOffset + xDivLine;
                if (m_layoutPos == ELayoutPos::Top) {
                    i_pPainter->drawLine(x, (sizeWidget.height()-1), x, (sizeWidget.height()-1) - iLineLength);
                }
                else if (m_layoutPos == ELayoutPos::Bottom) {
                    i_pPainter->drawLine(x, 0, x, iLineLength);
                }
            }
        }
        else {
            // Offset between geometries in parent widget of this labels widget to the drawing view.
            int yOffset = rectGridLabelsWidget.top() - iDrawingViewFrameWidth;
            for (int idxDivLine = 0; idxDivLine < m_divLinesMetrics.getDivLinesCount(EDivLineLayer::Main); ++idxDivLine ) {
                int yDivLine = m_divLinesMetrics.getDivLineInPix(EDivLineLayer::Main, idxDivLine);
                int y = rectDrawingView.top() - yOffset + yDivLine;
                if (m_layoutPos == ELayoutPos::Left) {
                    i_pPainter->drawLine((sizeWidget.width()-1) - iLineLength, y, (sizeWidget.width()-1), y);
                }
                else if (m_layoutPos == ELayoutPos::Right) {
                    i_pPainter->drawLine(0, y, iLineLength, y);
                }
            }
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

        if (m_scaleDir == EScaleDir::X) {
            // Offset between geometries in parent widget of this labels widget to the drawing view.
            int xOffset = rectGridLabelsWidget.left() - iDrawingViewFrameWidth;
            QRect rectXScaleMax = m_divLinesMetrics.getScaleMaxValBoundingRect();
            rectXScaleMax.moveLeft(rectDrawingView.left() - xOffset + rectXScaleMax.left());
            if (m_layoutPos == ELayoutPos::Top) {
                rectXScaleMax.moveTop((sizeWidget.height()-1) - rectXScaleMax.height() - iLineLength);
            }
            else if (m_layoutPos == ELayoutPos::Bottom) {
                rectXScaleMax.moveTop(iLineLength);
            }
            QPoint ptCenterXScaleMax = rectXScaleMax.center();
            rectXScaleMax.setWidth(sizeUnitString.width());
            rectXScaleMax.setHeight(sizeUnitString.height());
            rectXScaleMax.moveCenter(ptCenterXScaleMax);
            i_pPainter->drawText(rectXScaleMax, Qt::AlignVCenter|Qt::AlignHCenter, strUnit);

            for (int idxDivLine = 0; idxDivLine < m_divLinesMetrics.getDivLinesCount(eLayer); idxDivLine++) {
                if (m_divLinesMetrics.isDivLineLabelVisible(eLayer, idxDivLine)) {
                    QString strDivLineLabel;
                    if (drawingSize.dimensionUnit() == EDrawingDimensionUnit::Pixels) {
                        int x = m_divLinesMetrics.getDivLineInPix(EDivLineLayer::Main, idxDivLine);
                        strDivLineLabel = QString::number(x);
                    }
                    else {
                        strDivLineLabel = m_divLinesMetrics.getDivLineLabelText(eLayer, idxDivLine);
                    }
                    QRect rectDivLineLabel = m_divLinesMetrics.getDivLineLabelBoundingRect(eLayer, idxDivLine);
                    QRect rect = rectDivLineLabel;
                    rect.moveLeft(rectDrawingView.left() - xOffset + rectDivLineLabel.left());
                    if (m_layoutPos == ELayoutPos::Top) {
                        rect.moveTop((sizeWidget.height()-1) - rectDivLineLabel.height() - iLineLength);
                    }
                    else if (m_layoutPos == ELayoutPos::Bottom) {
                        rect.moveTop(iLineLength);
                    }
                    if (!rect.intersects(rectXScaleMax)) {
                        i_pPainter->drawText(rect, Qt::AlignVCenter|Qt::AlignHCenter, strDivLineLabel);
                    }
                }
            }
        } // if (m_scaleDir == EScaleDir::X)

        else /* if (m_scaleDir == EScaleDir::Y) */ {
            int yOffset = rectGridLabelsWidget.top() - iDrawingViewFrameWidth;
            QRect rectYScaleMax = m_divLinesMetrics.getScaleMaxValBoundingRect();
            rectYScaleMax.setWidth(sizeUnitString.width());
            rectYScaleMax.setHeight(sizeUnitString.height());
            if (m_layoutPos == ELayoutPos::Left) {
                rectYScaleMax.moveRight((sizeWidget.width()-1) - iLineLength - 2);
            }
            else if (m_layoutPos == ELayoutPos::Right) {
                rectYScaleMax.moveLeft(iLineLength + 2);
            }
            rectYScaleMax.moveTop(rectDrawingView.top() - yOffset + rectYScaleMax.top());
            i_pPainter->drawText(rectYScaleMax, Qt::AlignVCenter|Qt::AlignRight, strUnit);

            for (int idxDivLine = 0; idxDivLine < m_divLinesMetrics.getDivLinesCount(eLayer); idxDivLine++) {
                if (m_divLinesMetrics.isDivLineLabelVisible(eLayer, idxDivLine)) {
                    QString strDivLineLabel;
                    if (drawingSize.dimensionUnit() == EDrawingDimensionUnit::Pixels) {
                        int y = m_divLinesMetrics.getDivLineInPix(EDivLineLayer::Main, idxDivLine);
                        strDivLineLabel = QString::number(y);
                    }
                    else {
                        strDivLineLabel = m_divLinesMetrics.getDivLineLabelText(eLayer, idxDivLine);
                    }
                    QRect rectDivLineLabel = m_divLinesMetrics.getDivLineLabelBoundingRect(eLayer, idxDivLine);
                    QRect rect = rectDivLineLabel;
                    if (m_layoutPos == ELayoutPos::Left) {
                        rect.moveLeft((sizeWidget.width()-1) - rectDivLineLabel.width() - iLineLength - 2);
                    }
                    else if (m_layoutPos == ELayoutPos::Right) {
                        rect.moveLeft(iLineLength + 2);
                    }
                    rect.moveTop(rectDrawingView.top() - yOffset + rectDivLineLabel.top());
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
        } // if (m_scaleDir == EScaleDir::Y)

        i_pPainter->restore();

    } // if (gridSettings.areLabelsVisible())

} // paintGridLabels
