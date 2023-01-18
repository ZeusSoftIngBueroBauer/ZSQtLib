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

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qspinbox.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qspinbox.h>
#endif

#include "ZSDraw/DrawingPageSetup/ZSDrawWdgtDrawingViewPageSetup.h"
#include "ZSDraw/Drawing/ZSDrawingView.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


/*******************************************************************************
class CWdgtDrawingViewPageSetup : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtDrawingViewPageSetup::CWdgtDrawingViewPageSetup(
    CDrawingView* i_pDrawingView,
    QWidget*      i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pDrawingView(i_pDrawingView),
    m_pLyt(nullptr),
    m_pLblHeadLine(nullptr),
    m_pLytResolutionDpi(nullptr),
    m_pLblResolutionDpi(nullptr),
    m_pLblResolutionDpiVal(nullptr),
    m_pLblResolutionDpiUnit(nullptr),
    m_pLytResolutionDpmm(nullptr),
    m_pLblResolutionDpmm(nullptr),
    m_pLblResolutionDpmmVal(nullptr),
    m_pLblResolutionDpmmUnit(nullptr),
    m_pLytSepLineImageSize(nullptr),
    m_pLblSepLineImageSize(nullptr),
    m_pSepLineImageSize(nullptr),
    m_pLytLineImageSize(nullptr),
    m_pLblImageSizeWidth(nullptr),
    m_pEdtImageSizeWidth(nullptr),
    m_pLblImageSizeHeight(nullptr),
    m_pEdtImageSizeHeight(nullptr),
    m_pLytLineImageUseDrawingMetrics(nullptr),
    m_pLblImageUseDrawingMetrics(nullptr),
    m_pChkImageUseDrawingMetrics(nullptr),
    m_pLytSepLineDrawingMetricsSize(nullptr),
    m_pLblSepLineDrawingMetricsSize(nullptr),
    m_pSepLineDrawingMetricsSize(nullptr),
    m_pLytLineDrawingMetricsKeepAspectRatio(nullptr),
    m_pLblDrawingMetricsKeepAspectRatio(nullptr),
    m_pChkDrawingMetricsKeepAspectRatio(nullptr),
    m_pLytLineDrawingMetricsSize(nullptr),
    m_pLblDrawingMetricsSizeWidth(nullptr),
    m_pEdtDrawingMetricsSizeWidth(nullptr),
    m_pLblDrawingMetricsSizeHeight(nullptr),
    m_pEdtDrawingMetricsSizeHeight(nullptr),
    m_pLblDrawingMetricsSizeUnit(nullptr),
    m_pCmbDrawingMetricsSizeUnit(nullptr),
    m_pLytLineDrawingMetricsSizeScaleFactors(nullptr),
    m_pLblDrawingMetricsSizeScaleFactorWidth(nullptr),
    m_pEdtDrawingMetricsSizeScaleFactorWidth(nullptr),
    m_pLblDrawingMetricsSizeScaleFactorHeight(nullptr),
    m_pEdtDrawingMetricsSizeScaleFactorHeight(nullptr),
    m_pLytSepLineMargins(nullptr),
    m_pLblSepLineMargins(nullptr),
    m_pSepLineMargins(nullptr),
    m_pLytLineMarginsLeftRight(nullptr),
    m_pLblMarginsLeft(nullptr),
    m_pEdtMarginsLeft(nullptr),
    m_pLblMarginsRight(nullptr),
    m_pEdtMarginsRight(nullptr),
    m_pLytLineMarginsTopBottom(nullptr),
    m_pLblMarginsTop(nullptr),
    m_pEdtMarginsTop(nullptr),
    m_pLblMarginsBottom(nullptr),
    m_pEdtMarginsBottom(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtPageSetupDrawingScene");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    int cxLblWidth = 60;
    //int cxEdtWidth = 80;

    QFont fntHeadLine;

    m_pLblHeadLine = new QLabel("Drawing Scene");
    fntHeadLine = m_pLblHeadLine->font();
    fntHeadLine.setPointSize(fntHeadLine.pointSize()+4);
    m_pLblHeadLine->setFont(fntHeadLine);
    m_pLyt->addWidget(m_pLblHeadLine);
    m_pLyt->addSpacing(4);

    // <Section> Resolution
    //=====================

    double fXResolution_dpmm = Units.GraphDevice.dpmm(EDirection::Horizontal);
    double fYResolution_dpmm = Units.GraphDevice.dpmm(EDirection::Vertical);
    double fXResolution_dpi = fXResolution_dpmm*25.4;
    double fYResolution_dpi = fYResolution_dpmm*25.4;

    QString strXDpi  = QString::number(fXResolution_dpi,'f',2);
    QString strYDpi  = QString::number(fYResolution_dpi,'f',2);
    QString strXDpmm = QString::number(fXResolution_dpmm,'f',2);
    QString strYDpmm = QString::number(fYResolution_dpmm,'f',2);
    QString strResolution;

    // <Label> Resolution - dots per inch
    //-----------------------------------

    strResolution = strXDpi + " * " + strYDpi;
    m_pLytResolutionDpi = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytResolutionDpi);
    m_pLblResolutionDpi     = new QLabel("Resolution");
    m_pLblResolutionDpiVal  = new QLabel(strResolution);
    m_pLblResolutionDpiUnit = new QLabel("dots per inch");
    m_pLytResolutionDpi->addWidget(m_pLblResolutionDpi);
    m_pLytResolutionDpi->addWidget(m_pLblResolutionDpiVal);
    m_pLytResolutionDpi->addWidget(m_pLblResolutionDpiUnit);
    m_pLytResolutionDpi->addStretch();

    // <Label> Resolution - dots per millimeter
    //-----------------------------------------

    strResolution = strXDpmm + " * " + strYDpmm;
    m_pLytResolutionDpmm = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytResolutionDpmm);
    m_pLblResolutionDpmm     = new QLabel("Resolution");
    m_pLblResolutionDpmmVal  = new QLabel(strResolution);
    m_pLblResolutionDpmmUnit = new QLabel("dots per milli meter");
    m_pLytResolutionDpmm->addWidget(m_pLblResolutionDpmm);
    m_pLytResolutionDpmm->addWidget(m_pLblResolutionDpmmVal);
    m_pLytResolutionDpmm->addWidget(m_pLblResolutionDpmmUnit);
    m_pLytResolutionDpmm->addStretch();

    // <Section> Image Size
    //=====================

    // <Separator> Size
    //-----------------

    int cxEdtWidth = 100;

    QSize sizeDrawing = m_pDrawingView->getDrawingSizeInPixels();

    m_pLyt->addSpacing(4);

    m_pLytSepLineImageSize = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSepLineImageSize);

    m_pLblSepLineImageSize = new QLabel("Size:");
    m_pLytSepLineImageSize->addWidget(m_pLblSepLineImageSize);
    m_pSepLineImageSize = new QFrame();
    m_pSepLineImageSize->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    m_pLytSepLineImageSize->addWidget(m_pSepLineImageSize,1);

    m_pLyt->addSpacing(4);

    // <Line> Image Size
    //------------------

    m_pLytLineImageSize = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineImageSize);

    // <Edit> Image Width
    //--------------------

    m_pLblImageSizeWidth = new QLabel("Width:");
    m_pLblImageSizeWidth->setFixedWidth(cxLblWidth);
    m_pLytLineImageSize->addWidget(m_pLblImageSizeWidth);
    m_pEdtImageSizeWidth = new QSpinBox();
    m_pEdtImageSizeWidth->setFixedWidth(cxEdtWidth);
    m_pEdtImageSizeWidth->setMinimum(1);
    m_pEdtImageSizeWidth->setMaximum(100000);
    m_pEdtImageSizeWidth->setSuffix(" px");
    m_pEdtImageSizeWidth->setValue(sizeDrawing.width());
    m_pLytLineImageSize->addWidget(m_pEdtImageSizeWidth);

    if( !connect(
        /* pObjSender   */ m_pEdtImageSizeWidth,
        /* szSignal     */ SIGNAL(editingFinished()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtSizeWidthEditingFinished()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Edit> Image Height
    //--------------------

    m_pLblImageSizeHeight = new QLabel("Height:");
    m_pLblImageSizeHeight->setFixedWidth(cxLblWidth);
    m_pLytLineImageSize->addWidget(m_pLblImageSizeHeight);
    m_pEdtImageSizeHeight = new QSpinBox();
    m_pEdtImageSizeHeight->setFixedWidth(cxEdtWidth);
    m_pEdtImageSizeHeight->setMinimum(1);
    m_pEdtImageSizeHeight->setMaximum(100000);
    m_pEdtImageSizeHeight->setSuffix(" px");
    m_pEdtImageSizeHeight->setValue(sizeDrawing.height());
    m_pLytLineImageSize->addWidget(m_pEdtImageSizeHeight);
    m_pLytLineImageSize->addStretch();

    if( !connect(
        /* pObjSender   */ m_pEdtImageSizeHeight,
        /* szSignal     */ SIGNAL(editingFinished()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtSizeHeightEditingFinished()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Line> Use Drawing Metrics
    //---------------------------

    m_pLytLineImageUseDrawingMetrics = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineImageUseDrawingMetrics);

    // <CheckBox> Use Drawing Metrics
    //-------------------------------

    m_pLblImageUseDrawingMetrics = new QLabel("Use Metrics:");
    m_pLblImageUseDrawingMetrics->setFixedWidth(cxLblWidth);
    m_pLytLineImageUseDrawingMetrics->addWidget(m_pLblImageUseDrawingMetrics);
    m_pChkImageUseDrawingMetrics = new QCheckBox();
    m_pChkImageUseDrawingMetrics->setChecked(false);
    m_pLytLineImageUseDrawingMetrics->addWidget(m_pChkImageUseDrawingMetrics);
    m_pLytLineImageUseDrawingMetrics->addStretch();

    //if( !connect(
    //    /* pObjSender   */ m_pChkImageUseDrawingMetrics,
    //    /* szSignal     */ SIGNAL(stateChanged(int)),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT(onChkImageUseDrawingMetricsStateChanged(int)) ) )
    //{
    //    throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    //}

    // <Section> Drawing Metrics
    //==========================

    // <Separator> Metrics
    //--------------------

    //int cxEdtWidth = 100;

    m_pLyt->addSpacing(4);

    m_pLytSepLineDrawingMetricsSize = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSepLineDrawingMetricsSize);

    m_pLblSepLineDrawingMetricsSize = new QLabel("Metrics:");
    m_pLytSepLineDrawingMetricsSize->addWidget(m_pLblSepLineDrawingMetricsSize);
    m_pSepLineDrawingMetricsSize = new QFrame();
    m_pSepLineDrawingMetricsSize->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    m_pLytSepLineDrawingMetricsSize->addWidget(m_pSepLineDrawingMetricsSize,1);

    m_pLyt->addSpacing(4);

    // <Line> Keep Aspect Ratio
    //-------------------------

    m_pLytLineDrawingMetricsKeepAspectRatio = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineDrawingMetricsKeepAspectRatio);

    // <CheckBox> Keep Aspect Ratio
    //-----------------------------

    m_pLblDrawingMetricsKeepAspectRatio = new QLabel("Keep Aspect Ratio:");
    m_pLblDrawingMetricsKeepAspectRatio->setFixedWidth(cxLblWidth);
    m_pLytLineDrawingMetricsKeepAspectRatio->addWidget(m_pLblDrawingMetricsKeepAspectRatio);
    m_pChkDrawingMetricsKeepAspectRatio = new QCheckBox();
    m_pChkDrawingMetricsKeepAspectRatio->setChecked(true);
    m_pLytLineDrawingMetricsKeepAspectRatio->addWidget(m_pChkDrawingMetricsKeepAspectRatio);

    //if( !connect(
    //    /* pObjSender   */ m_pChkDrawingMetricsKeepAspectRatio,
    //    /* szSignal     */ SIGNAL(stateChanged(int)),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT(onChkDrawingMetricsKeepAspectRatioStateChanged(int)) ) )
    //{
    //    throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    //}

    // <Line> Metrics Size
    //--------------------

    m_pLytLineDrawingMetricsSize = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineDrawingMetricsSize);

    // <Edit> Metrics Width
    //---------------------

    m_pLblDrawingMetricsSizeWidth = new QLabel("Width:");
    m_pLblDrawingMetricsSizeWidth->setFixedWidth(cxLblWidth);
    m_pLytLineDrawingMetricsSize->addWidget(m_pLblDrawingMetricsSizeWidth);
    m_pEdtDrawingMetricsSizeWidth = new QSpinBox();
    m_pEdtDrawingMetricsSizeWidth->setFixedWidth(cxEdtWidth);
    m_pEdtDrawingMetricsSizeWidth->setMinimum(1);
    m_pEdtDrawingMetricsSizeWidth->setMaximum(100000);
    m_pEdtDrawingMetricsSizeWidth->setSuffix(" " + m_pCmbDrawingMetricsSizeUnit->currentText());
    //m_pEdtDrawingMetricsSizeWidth->setValue(sizeDrawing.width());
    m_pLytLineDrawingMetricsSize->addWidget(m_pEdtDrawingMetricsSizeWidth);

    //if( !connect(
    //    /* pObjSender   */ m_pEdtDrawingMetricsSizeWidth,
    //    /* szSignal     */ SIGNAL(editingFinished()),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT(onEdtDrawingMetricsSizeWidthEditingFinished()) ) )
    //{
    //    throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    //}

    // <Edit> Metrics Height
    //----------------------

    m_pLblDrawingMetricsSizeHeight = new QLabel("Height:");
    m_pLblDrawingMetricsSizeHeight->setFixedWidth(cxLblWidth);
    m_pLytLineDrawingMetricsSize->addWidget(m_pLblDrawingMetricsSizeHeight);
    m_pEdtDrawingMetricsSizeHeight = new QSpinBox();
    m_pEdtDrawingMetricsSizeHeight->setFixedWidth(cxEdtWidth);
    m_pEdtDrawingMetricsSizeHeight->setMinimum(1);
    m_pEdtDrawingMetricsSizeHeight->setMaximum(100000);
    m_pEdtDrawingMetricsSizeHeight->setSuffix(" " + m_pCmbDrawingMetricsSizeUnit->currentText());
    //m_pEdtDrawingMetricsSizeHeight->setValue(sizeDrawing.height());
    m_pLytLineDrawingMetricsSize->addWidget(m_pEdtDrawingMetricsSizeHeight);

    //if( !connect(
    //    /* pObjSender   */ m_pEdtDrawingMetricsSizeHeight,
    //    /* szSignal     */ SIGNAL(editingFinished()),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT(onEdtDrawingMetricsSizeHeightEditingFinished()) ) )
    //{
    //    throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    //}

    // <ComboBox> Metrics Unit
    //-------------------------

    m_pLblDrawingMetricsSizeUnit = new QLabel("Unit:");
    m_pLblDrawingMetricsSizeUnit->setFixedWidth(cxLblWidth);
    m_pLytLineDrawingMetricsSize->addWidget(m_pLblDrawingMetricsSizeUnit);
    m_pCmbDrawingMetricsSizeUnit = new QComboBox();
    m_pCmbDrawingMetricsSizeUnit->setFixedWidth(cxEdtWidth);

    CPageSetup* pageSetup = m_pDrawingView->getPageSetup();
    //CPhysSizeGeometry* physSizeGeometry = pageSetup->getPhysSizeWidth();

    //for( int idxUnit = 0; idxUnit < physSizeGeometry->getUnitCount(); ++idxUnit )
    //{
    //    m_pCmbDrawingMetricsSizeUnit->addItem(physSizeGeometry->getUnit(idxUnit)->getSymbol());
    //}
    //m_pCmbDrawingMetricsSizeUnit->setCurrentText(physSizeGeometry->CentiMeter()->getSymbol());
    m_pLytLineDrawingMetricsSize->addWidget(m_pCmbDrawingMetricsSizeUnit);
    m_pLytLineDrawingMetricsSize->addStretch();

    //if( !connect(
    //    /* pObjSender   */ m_pCmbDrawingMetricsSizeUnit,
    //    /* szSignal     */ SIGNAL(currentIndexChanged()),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT(onCmbDrawingMetricsSizeUnitCurrentIndexChanged(int)) ) )
    //{
    //    throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    //}

    // <Line> Scale Factors
    //---------------------

    m_pLytLineDrawingMetricsSizeScaleFactors = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineDrawingMetricsSizeScaleFactors);

    // <Edit> Factor Width
    //---------------------

    m_pLblDrawingMetricsSizeScaleFactorWidth = new QLabel("Scale Factor:");
    m_pLblDrawingMetricsSizeScaleFactorWidth->setFixedWidth(cxLblWidth);
    m_pLytLineDrawingMetricsSizeScaleFactors->addWidget(m_pLblDrawingMetricsSizeScaleFactorWidth);
    m_pEdtDrawingMetricsSizeScaleFactorWidth = new QSpinBox();
    m_pEdtDrawingMetricsSizeScaleFactorWidth->setFixedWidth(cxEdtWidth);
    m_pEdtDrawingMetricsSizeScaleFactorWidth->setEnabled(false);
    //m_pEdtDrawingMetricsSizeScaleFactorWidth->setValue(sizeDrawing.width());
    m_pLytLineDrawingMetricsSizeScaleFactors->addWidget(m_pEdtDrawingMetricsSizeScaleFactorWidth);

    // <Edit> Factor Height
    //----------------------

    m_pLblDrawingMetricsSizeScaleFactorHeight = new QLabel("Scale Factor:");
    m_pLblDrawingMetricsSizeScaleFactorHeight->setFixedWidth(cxLblWidth);
    m_pLytLineDrawingMetricsSizeScaleFactors->addWidget(m_pLblDrawingMetricsSizeScaleFactorHeight);
    m_pEdtDrawingMetricsSizeScaleFactorHeight = new QSpinBox();
    m_pEdtDrawingMetricsSizeScaleFactorHeight->setFixedWidth(cxEdtWidth);
    m_pEdtDrawingMetricsSizeScaleFactorHeight->setEnabled(false);
    //m_pEdtDrawingMetricsSizeScaleFactorHeight->setValue(sizeDrawing.width());
    m_pLytLineDrawingMetricsSizeScaleFactors->addWidget(m_pEdtDrawingMetricsSizeScaleFactorHeight);
    m_pLytLineDrawingMetricsSizeScaleFactors->addStretch();

    // <Section> Margins
    //==================

    // <Separator> Margins
    //--------------------

    QMargins margins = m_pDrawingView->getViewportMargins();

    m_pLyt->addSpacing(4);

    m_pLytSepLineMargins = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSepLineMargins);

    m_pLblSepLineMargins = new QLabel("Margins:");
    m_pLytSepLineMargins->addWidget(m_pLblSepLineMargins);
    m_pSepLineMargins = new QFrame();
    m_pSepLineMargins->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    m_pLytSepLineMargins->addWidget(m_pSepLineMargins,1);

    m_pLyt->addSpacing(4);

    // <Line> Margins - Left/Right
    //----------------------------

    m_pLytLineMarginsLeftRight = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineMarginsLeftRight);

    // <LineEdit> Margins - Left
    //--------------------------

    m_pLblMarginsLeft = new QLabel("Left:");
    m_pLblMarginsLeft->setFixedWidth(cxLblWidth);
    m_pLytLineMarginsLeftRight->addWidget(m_pLblMarginsLeft);
    m_pEdtMarginsLeft = new QSpinBox();
    m_pEdtMarginsLeft->setFixedWidth(cxEdtWidth);
    m_pEdtMarginsLeft->setMinimum(0);
    m_pEdtMarginsLeft->setMaximum(100);
    m_pEdtMarginsLeft->setSuffix(" px");
    m_pEdtMarginsLeft->setValue(margins.left());
    m_pLytLineMarginsLeftRight->addWidget(m_pEdtMarginsLeft);

    if( !connect(
        /* pObjSender   */ m_pEdtMarginsLeft,
        /* szSignal     */ SIGNAL(editingFinished()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtMarginsLeftEditingFinished()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <LineEdit> Margins - Right
    //--------------------------

    m_pLblMarginsRight = new QLabel("Right:");
    m_pLblMarginsRight->setFixedWidth(cxLblWidth);
    m_pLytLineMarginsLeftRight->addWidget(m_pLblMarginsRight);
    m_pEdtMarginsRight = new QSpinBox();
    m_pEdtMarginsRight->setFixedWidth(cxEdtWidth);
    m_pEdtMarginsRight->setMinimum(0);
    m_pEdtMarginsRight->setMaximum(100);
    m_pEdtMarginsRight->setSuffix(" px");
    m_pEdtMarginsRight->setValue(margins.left());
    m_pLytLineMarginsLeftRight->addWidget(m_pEdtMarginsRight);
    m_pLytLineMarginsLeftRight->addStretch();

    if( !connect(
        /* pObjSender   */ m_pEdtMarginsRight,
        /* szSignal     */ SIGNAL(editingFinished()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtMarginsRightEditingFinished()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Line> Margins - Top/Bottom
    //----------------------------

    m_pLytLineMarginsTopBottom = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineMarginsTopBottom);

    // <LineEdit> Margins - Top
    //--------------------------

    m_pLblMarginsTop = new QLabel("Top:");
    m_pLblMarginsTop->setFixedWidth(cxLblWidth);
    m_pLytLineMarginsTopBottom->addWidget(m_pLblMarginsTop);
    m_pEdtMarginsTop = new QSpinBox();
    m_pEdtMarginsTop->setFixedWidth(cxEdtWidth);
    m_pEdtMarginsTop->setMinimum(0);
    m_pEdtMarginsTop->setMaximum(100);
    m_pEdtMarginsTop->setSuffix(" px");
    m_pEdtMarginsTop->setValue(margins.left());
    m_pLytLineMarginsTopBottom->addWidget(m_pEdtMarginsTop);

    if( !connect(
        /* pObjSender   */ m_pEdtMarginsTop,
        /* szSignal     */ SIGNAL(editingFinished()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtMarginsTopEditingFinished()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <LineEdit> Margins - Bottom
    //--------------------------

    m_pLblMarginsBottom = new QLabel("Bottom:");
    m_pLblMarginsBottom->setFixedWidth(cxLblWidth);
    m_pLytLineMarginsTopBottom->addWidget(m_pLblMarginsBottom);
    m_pEdtMarginsBottom = new QSpinBox();
    m_pEdtMarginsBottom->setFixedWidth(cxEdtWidth);
    m_pEdtMarginsBottom->setMinimum(0);
    m_pEdtMarginsBottom->setMaximum(100);
    m_pEdtMarginsBottom->setSuffix(" px");
    m_pEdtMarginsBottom->setValue(margins.left());
    m_pLytLineMarginsTopBottom->addWidget(m_pEdtMarginsBottom);
    m_pLytLineMarginsTopBottom->addStretch();

    if( !connect(
        /* pObjSender   */ m_pEdtMarginsBottom,
        /* szSignal     */ SIGNAL(editingFinished()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtMarginsBottomEditingFinished()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Stretch> at bottom of widget
    //------------------------------

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtDrawingViewPageSetup::~CWdgtDrawingViewPageSetup()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pDrawingView = nullptr;
    m_pLyt = nullptr;
    m_pLblHeadLine = nullptr;
    m_pLytResolutionDpi = nullptr;
    m_pLblResolutionDpi = nullptr;
    m_pLblResolutionDpiVal = nullptr;
    m_pLblResolutionDpiUnit = nullptr;
    m_pLytResolutionDpmm = nullptr;
    m_pLblResolutionDpmm = nullptr;
    m_pLblResolutionDpmmVal = nullptr;
    m_pLblResolutionDpmmUnit = nullptr;
    m_pLytSepLineImageSize = nullptr;
    m_pLblSepLineImageSize = nullptr;
    m_pSepLineImageSize = nullptr;
    m_pLytLineImageSize = nullptr;
    m_pLblImageSizeWidth = nullptr;
    m_pEdtImageSizeWidth = nullptr;
    m_pLblImageSizeHeight = nullptr;
    m_pEdtImageSizeHeight = nullptr;
    m_pLytLineImageUseDrawingMetrics = nullptr;
    m_pLblImageUseDrawingMetrics = nullptr;
    m_pChkImageUseDrawingMetrics = nullptr;
    m_pLytSepLineDrawingMetricsSize = nullptr;
    m_pLblSepLineDrawingMetricsSize = nullptr;
    m_pSepLineDrawingMetricsSize = nullptr;
    m_pLytLineDrawingMetricsKeepAspectRatio = nullptr;
    m_pLblDrawingMetricsKeepAspectRatio = nullptr;
    m_pChkDrawingMetricsKeepAspectRatio = nullptr;
    m_pLytLineDrawingMetricsSize = nullptr;
    m_pLblDrawingMetricsSizeWidth = nullptr;
    m_pEdtDrawingMetricsSizeWidth = nullptr;
    m_pLblDrawingMetricsSizeHeight = nullptr;
    m_pEdtDrawingMetricsSizeHeight = nullptr;
    m_pLblDrawingMetricsSizeUnit = nullptr;
    m_pCmbDrawingMetricsSizeUnit = nullptr;
    m_pLytLineDrawingMetricsSizeScaleFactors = nullptr;
    m_pLblDrawingMetricsSizeScaleFactorWidth = nullptr;
    m_pEdtDrawingMetricsSizeScaleFactorWidth = nullptr;
    m_pLblDrawingMetricsSizeScaleFactorHeight = nullptr;
    m_pEdtDrawingMetricsSizeScaleFactorHeight = nullptr;
    m_pLytSepLineMargins = nullptr;
    m_pLblSepLineMargins = nullptr;
    m_pSepLineMargins = nullptr;
    m_pLytLineMarginsLeftRight = nullptr;
    m_pLblMarginsLeft = nullptr;
    m_pEdtMarginsLeft = nullptr;
    m_pLblMarginsRight = nullptr;
    m_pEdtMarginsRight = nullptr;
    m_pLytLineMarginsTopBottom = nullptr;
    m_pLblMarginsTop = nullptr;
    m_pEdtMarginsTop = nullptr;
    m_pLblMarginsBottom = nullptr;
    m_pEdtMarginsBottom = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtDrawingViewPageSetup::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;

    int cxWidth = m_pEdtImageSizeWidth->value();
    int cyHeight = m_pEdtImageSizeHeight->value();

    QRectF rect = m_pDrawingView->sceneRect();

    if( rect.width() != cxWidth || rect.height() != cyHeight )
    {
        bHasChanges = true;
    }

    if( !bHasChanges )
    {
        int iMarginLeft = m_pEdtMarginsLeft->value();
        int iMarginTop = m_pEdtMarginsTop->value();
        int iMarginRight = m_pEdtMarginsRight->value();
        int iMarginBottom = m_pEdtMarginsBottom->value();

        QMargins margins = m_pDrawingView->getViewportMargins();

        if( margins.left() != iMarginLeft || margins.top() != iMarginTop
            || margins.right() != iMarginRight || margins.bottom() != iMarginBottom )
        {
            bHasChanges = true;
        }
    }

    return bHasChanges;

} // hasChanges

//------------------------------------------------------------------------------
void CWdgtDrawingViewPageSetup::acceptChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );

    int cxWidth = m_pEdtImageSizeWidth->value();
    int cyHeight = m_pEdtImageSizeHeight->value();

    QRectF rect = m_pDrawingView->sceneRect();

    rect.setWidth(cxWidth);
    rect.setHeight(cyHeight);

    // The scene rectangle of QGraphicsView defines the area of the scene
    // visualized by the view. This is the area of the scene that can be navigated
    // by the scroll bars of the QGraphicsView.
    // Here we don't want to change the visible areay within the scroll bars but
    // the scenes rectangle size.
    m_pDrawingView->getDrawingScene()->setSceneRect(rect);

    int iMarginLeft = m_pEdtMarginsLeft->value();
    int iMarginTop = m_pEdtMarginsTop->value();
    int iMarginRight = m_pEdtMarginsRight->value();
    int iMarginBottom = m_pEdtMarginsBottom->value();

    QMargins margins = m_pDrawingView->getViewportMargins();

    margins.setLeft(iMarginLeft);
    margins.setTop(iMarginTop);
    margins.setRight(iMarginRight);
    margins.setBottom(iMarginBottom);

    m_pDrawingView->setViewportMargins(margins);

} // acceptChanges

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewPageSetup::onEdtSizeWidthEditingFinished()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Height: " + m_pEdtImageSizeWidth->text();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtSizeWidthEditingFinished",
        /* strAddInfo   */ strMthInArgs );

    emit settingsChanged();

} // onEdtSizeWidthEditingFinished

//------------------------------------------------------------------------------
void CWdgtDrawingViewPageSetup::onEdtSizeHeightEditingFinished()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Height: " + m_pEdtImageSizeHeight->text();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtSizeHeightEditingFinished",
        /* strAddInfo   */ strMthInArgs );

    emit settingsChanged();

} // onEdtSizeHeightEditingFinished

//------------------------------------------------------------------------------
void CWdgtDrawingViewPageSetup::onEdtMarginsLeftEditingFinished()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Margins.Left: " + m_pEdtMarginsLeft->text();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtMarginsLeftEditingFinished",
        /* strAddInfo   */ strMthInArgs );

    emit settingsChanged();

} // onEdtMarginsLeftEditingFinished

//------------------------------------------------------------------------------
void CWdgtDrawingViewPageSetup::onEdtMarginsTopEditingFinished()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Margins.Top: " + m_pEdtMarginsTop->text();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtMarginsTopEditingFinished",
        /* strAddInfo   */ strMthInArgs );

    emit settingsChanged();

} // onEdtMarginsTopEditingFinished

//------------------------------------------------------------------------------
void CWdgtDrawingViewPageSetup::onEdtMarginsRightEditingFinished()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Margins.Right: " + m_pEdtMarginsRight->text();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtMarginsRightEditingFinished",
        /* strAddInfo   */ strMthInArgs );

    emit settingsChanged();

} // onEdtMarginsRightEditingFinished

//------------------------------------------------------------------------------
void CWdgtDrawingViewPageSetup::onEdtMarginsBottomEditingFinished()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Margins.Bottom: " + m_pEdtMarginsBottom->text();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtMarginsBottomEditingFinished",
        /* strAddInfo   */ strMthInArgs );

    emit settingsChanged();

} // onEdtMarginsBottomEditingFinished
