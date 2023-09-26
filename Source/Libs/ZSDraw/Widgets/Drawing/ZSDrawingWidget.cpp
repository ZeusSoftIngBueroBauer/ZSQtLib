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

#include "ZSDraw/Widgets/Drawing/ZSDrawingWidget.h"
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawGridLabels.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawingView.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

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
//using namespace ZS::System::GUI;
using namespace ZS::Draw;
//using namespace ZS::PhysVal;


/*******************************************************************************
class CWdgtDrawing : public QWidget
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
CWdgtDrawing::CWdgtDrawing(QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pLyt(nullptr),
    m_pWdgtGridLabelsXTop(nullptr),
    m_pWdgtGridLabelsXBottom(nullptr),
    m_pWdgtGridLabelsYLeft(nullptr),
    m_pWdgtGridLabelsYRight(nullptr),
    m_pDrawingView(nullptr),
    m_pDrawingScene(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName("theInst");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::Drawing", ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    /*
         0               1                      2
       +---+----------------------------------+---+
     0 | G         GridLabelXTop                G |
       + r +----------------------------------+ r +
       | i |                                  | i |
       | d |                                  | d |
       | L |                                  | L |
       | a |                                  | a |
     1 | b |        DrawingView               | b |
       | e |                                  | e |
       | l |                                  | l |
       | Y |                                  | Y |
       | L |                                  | R |
       | e |                                  | i |
       + f +----------------------------------+ g +
     2 | t         GridLabelXBottom             h |
       +---+----------------------------------+---+
    */

    m_pLyt = new QGridLayout();
    setLayout(m_pLyt);

    // The drawing size instance created by the drawing scene accesses the
    // unit to get the screen resolution. To get the screen resolution the
    // screen resolution must be set before. Setting it in the drawing views
    // constructor is too late. It must be done by the creator of the drawing view.

    // Factor to convert Px/inch to Px/m: 1.0/0.0254
    double fDpi = logicalDpiX();  // e.g. 100 px/inch
    double fDpmm = fDpi / Units.Length.in.convertValue(1.0, Units.Length.mm);
    Units.Length.setResolutionInPxPerMM(fDpmm);

    m_pDrawingScene = new CDrawingScene("theInst");

    m_pDrawingView = new CDrawingView(m_pDrawingScene);
    m_pDrawingView->setMouseTracking(true);
    m_pLyt->addWidget(m_pDrawingView, 1, 1, 1, 1);

    m_pWdgtGridLabelsXTop = new CWdgtGridLabels(
        "XTop", m_pDrawingView, EScaleDir::X, ELayoutPos::Top);
    m_pLyt->addWidget(m_pWdgtGridLabelsXTop, 0, 0, 1, 3);

    m_pWdgtGridLabelsXBottom = new CWdgtGridLabels(
        "XBottom", m_pDrawingView, EScaleDir::X, ELayoutPos::Bottom);
    m_pLyt->addWidget(m_pWdgtGridLabelsXBottom, 2, 0, 1, 3);

    m_pWdgtGridLabelsYLeft = new CWdgtGridLabels(
        "YLeft", m_pDrawingView, EScaleDir::Y, ELayoutPos::Left);
    m_pLyt->addWidget(m_pWdgtGridLabelsYLeft, 0, 0, 3, 1);

    m_pWdgtGridLabelsYRight = new CWdgtGridLabels(
        "YRight", m_pDrawingView, EScaleDir::Y, ELayoutPos::Right);
    m_pLyt->addWidget(m_pWdgtGridLabelsYRight, 0, 2, 3, 1);

    m_pLyt->setRowStretch(1, 1);
    m_pLyt->setColumnStretch(1, 1);
    m_pLyt->setHorizontalSpacing(0);
    m_pLyt->setVerticalSpacing(0);

    onDrawingViewDrawingSizeChanged(m_pDrawingView->drawingSize());
    onDrawingViewGridSettingsChanged(m_pDrawingView->gridSettings());

    QObject::connect(
        m_pDrawingView, &CDrawingView::drawingSizeChanged,
        this, &CWdgtDrawing::onDrawingViewDrawingSizeChanged);
    QObject::connect(
        m_pDrawingView, &CDrawingView::gridSettingsChanged,
        this, &CWdgtDrawing::onDrawingViewGridSettingsChanged);
    QObject::connect(
        m_pDrawingView, &CDrawingView::contentAreaChanged,
        this, &CWdgtDrawing::onDrawingViewContentAreaChanged);

} // ctor

//------------------------------------------------------------------------------
CWdgtDrawing::~CWdgtDrawing()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try
    {
        delete m_pDrawingScene;
    }
    catch (...)
    {
    }

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pLyt = nullptr;
    m_pWdgtGridLabelsXTop = nullptr;
    m_pWdgtGridLabelsXBottom = nullptr;
    m_pWdgtGridLabelsYLeft = nullptr;
    m_pWdgtGridLabelsYRight = nullptr;
    m_pDrawingView = nullptr;
    m_pDrawingScene = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawing::onDrawingViewDrawingSizeChanged(const CDrawingSize& i_size)
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

    adjustDrawingViewMaximumSize();
}

//------------------------------------------------------------------------------
void CWdgtDrawing::onDrawingViewGridSettingsChanged(const CDrawGridSettings& i_settings)
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

    adjustDrawingViewMaximumSize();
}

//------------------------------------------------------------------------------
void CWdgtDrawing::onDrawingViewContentAreaChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingViewContentAreaChanged",
        /* strAddInfo   */ "" );

    adjustDrawingViewMaximumSize();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawing::adjustDrawingViewMaximumSize()
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingView->drawingSize();
    QMargins marginsDrawingView = m_pDrawingView->contentsMargins();

    int cxMarginHor = marginsDrawingView.left() + marginsDrawingView.right();
    int cyMarginVer = marginsDrawingView.top() + marginsDrawingView.bottom();
    int iZoomFactor_perCent = m_pDrawingView->zoomFactorInPerCent();
    int iImageWidth_px = (iZoomFactor_perCent * drawingSize.imageWidthInPixels()) / 100;
    int iImageHeight_px = (iZoomFactor_perCent * drawingSize.imageHeightInPixels()) / 100;
    int iDrawingViewMaxWidth = iImageWidth_px + cxMarginHor;
    int iDrawingViewMaxHeight = iImageHeight_px + cyMarginVer;

    //if (m_pDrawingView->horizontalScrollBar()->isVisible()) {
    //    QSize sizeHorScrollBar = m_pDrawingView->horizontalScrollBar()->size();
    //    iDrawingViewMaxWidth += sizeHorScrollBar.width();
    //}
    //if (m_pDrawingView->verticalScrollBar()->isVisible()) {
    //    QSize sizeVerScrollBar = m_pDrawingView->horizontalScrollBar()->size();
    //    iDrawingViewMaxHeight += sizeVerScrollBar.height();
    //}

    m_pDrawingView->setMaximumSize(iDrawingViewMaxWidth, iDrawingViewMaxHeight);
    // But in Qt? If set separately the viewport geometry is not updated correctly and
    // the second setMaximum call seem to be ignored.
    //m_pDrawingView->setMaximumWidth(iImageWidth_px + cxMarginHor);
    //m_pDrawingView->setMaximumHeight(iImageHeight_px + cyMarginVer);
    //m_pDrawingScene->invalidate();

    //m_pWdgtGridLabelsXTop->setFixedHeight(m_pWdgtGridLabelsXTop->sizeHint().height());
    //m_pWdgtGridLabelsXBottom->setFixedHeight(m_pWdgtGridLabelsXBottom->sizeHint().height());
    //m_pWdgtGridLabelsYLeft->setFixedWidth(m_pWdgtGridLabelsYLeft->sizeHint().width());
    //m_pWdgtGridLabelsYRight->setFixedWidth(m_pWdgtGridLabelsYRight->sizeHint().width());
}
