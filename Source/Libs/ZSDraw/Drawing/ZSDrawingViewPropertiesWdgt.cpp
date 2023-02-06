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

#include "ZSDraw/Drawing/ZSDrawingViewPropertiesWdgt.h"
#include "ZSDraw/Common/ZSDrawCommon.h"
#include "ZSDraw/Drawing/ZSDrawUnits.h"
#include "ZSDraw/Drawing/ZSDrawingView.h"
#include "ZSPhysValGUI/ZSPhysValWdgtEditPhysVal.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtCore/qmetaobject.h>
#if QT_VERSION < 0x050000
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qspinbox.h>
#else
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qspinbox.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtDrawingViewProperties : public CWdgtGraphObjPropertiesAbstract
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtDrawingViewProperties::CWdgtDrawingViewProperties(
    CDrawingView* i_pDrawingView, QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtGraphObjPropertiesAbstract(i_pDrawingView->drawingScene(), i_pWdgtParent),
    m_pDrawingView(i_pDrawingView),
    m_pLyt(nullptr),
    m_pLytLineDimensionUnit(nullptr),
    m_pLblDimensionUnit(nullptr),
    m_pCmbDimensionUnit(nullptr),
    // Metric system
    m_pWdgtMetric(nullptr),
    m_pLytWdgtMetric(nullptr),
    // Resolution of a pixel on screen
    m_pLytSepLineResolution(nullptr),
    m_pLblSepLineResolution(nullptr),
    m_pSepLineResolution(nullptr),
    m_pLytLineResolution(nullptr),
    m_pLblResolution_pxpi(nullptr),
    m_pEdtResolution_pxpi(nullptr),
    m_pLblResolution_pxpmm(nullptr),
    m_pEdtResolution_pxpmm(nullptr),
    // Image Size in Metric System
    m_pLytSepLineImageMetric(nullptr),
    m_pLblSepLineImageMetric(nullptr),
    m_pSepLineImageMetric(nullptr),
    m_pLytLineImageMetricUnit(nullptr),
    m_pLblImageMetricUnit(nullptr),
    m_pCmbImageMetricUnit(nullptr),
    m_pLytLineMetricSize(nullptr),
    m_pLblImageMetricWidth(nullptr),
    m_pEdtImageMetricWidth(nullptr),
    m_pLblImageMetricHeight(nullptr),
    m_pEdtImageMetricHeight(nullptr),
    m_pLytLineImageMetricNormedPaper(nullptr),
    m_pLblImageMetricNormedPaperSizes(nullptr),
    m_pCmbImageMetricNormedPaperSizes(nullptr),
    m_pLblImageMetricNormedPaperOrientation(nullptr),
    m_pCmbImageMetricNormedPaperOrientation(nullptr),
    m_pLytLineImageMetricScaleFactor(nullptr),
    m_pLblImageMetricScaleFactor(nullptr),
    m_pCmbImageMetricScaleFactorDividend(nullptr),
    m_pLblImageMetricScaleFactorHyphen(nullptr),
    m_pCmbImageMetricScaleFactorDivisor(nullptr),
    // Image Size in Pixels
    m_pLytSepLineImageSize_px(nullptr),
    m_pLblSepLineImageSize_px(nullptr),
    m_pSepLineImageSize_px(nullptr),
    m_pLytLineImageSize_px(nullptr),
    m_pLblImageSizeWidth_px(nullptr),
    m_pEdtImageSizeWidth_px(nullptr),
    m_pLblImageSizeHeight_px(nullptr),
    m_pEdtImageSizeHeight_px(nullptr),
    // Caching values
    m_dimensionUnit(EDrawingDimensionUnit::Pixels),
    m_strMetricUnitSymbol(Units.Length.mm.symbol()),
    m_fImageMetricWidth(0.0),
    m_fImageMetricHeight(0.0),
    m_strImageMetricNormedPaperSize(
        CEnumNormedPaperSize(ENormedPaperSize::Undefined).toString(EEnumEntryAliasStrText)),
    m_strImageMetricNormedPaperOrientation(
        CEnumDirection(EDirection::Undefined).toString(EEnumEntryAliasStrText)),
    m_iMetricScaleFactorDividend(1),
    m_iMetricScaleFactorDivisor(1),
    m_fImageSizeWidth_px(0.0),
    m_fImageSizeHeight_px(0.0),
    // Blocking signals counter
    m_iValueChangedSignalsBlocked(0),
    // Trace admin object for method tracing
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_pDrawingView->objectName());

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pLyt = new QVBoxLayout;
    setLayout(m_pLyt);

    QObject::connect(
        m_pDrawingView, &CDrawingView::drawingSizeChanged,
        this, &CWdgtDrawingViewProperties::onDrawingViewDrawingSizeChanged );

    QSize sizeDrawing = m_pDrawingView->drawingSizeInPixels();

    m_fImageSizeWidth_px = sizeDrawing.width();
    m_fImageSizeHeight_px = sizeDrawing.height();

    //CPhysVal physValWidth(m_fImageSizeWidth_px, Units.Length.pxX);
    //CPhysVal physValHeight(m_fImageSizeHeight_px, Units.Length.pxY);

    //physValWidth.convertValue(Units.Length.mm);
    //physValHeight.convertValue(Units.Length.mm);

    //m_fImageMetricWidth = physValWidth.getVal();
    //m_fImageMetricHeight = physValHeight.getVal();

    // <Section> Dimension Unit
    //=========================

    m_pLytLineDimensionUnit = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineDimensionUnit);

    m_pLblDimensionUnit = new QLabel("Dimension Unit:");
    m_pLblDimensionUnit->setFixedWidth(m_cxLblWidthClm1);
    m_pLytLineDimensionUnit->addWidget(m_pLblDimensionUnit);
    m_pCmbDimensionUnit = new QComboBox();
    m_pCmbDimensionUnit->setFixedWidth(m_cxEdtWidthClm1);
    m_pLytLineDimensionUnit->addWidget(m_pCmbDimensionUnit);
    m_pLytLineDimensionUnit->addStretch();
    for( CEnumDrawingDimensionUnit eVal = 0; eVal < CEnumDrawingDimensionUnit::count(); ++eVal ) {
        m_pCmbDimensionUnit->addItem(eVal.toString(), eVal.enumeratorAsInt());
    }
    m_pCmbDimensionUnit->setCurrentIndex(static_cast<int>(EDrawingDimensionUnit::Pixels));
    QObject::connect(
        m_pCmbDimensionUnit, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtDrawingViewProperties::onCmbDimensionUnitCurrentIndexChanged );

    // <Section> Metric System
    //========================

    m_pWdgtMetric = new QWidget();
    m_pLytWdgtMetric = new QVBoxLayout();
    m_pWdgtMetric->setLayout(m_pLytWdgtMetric);
    m_pLytWdgtMetric->setContentsMargins(0, 0, 0, 0);
    m_pLyt->addWidget(m_pWdgtMetric);

    // <Section> Resolution
    //=====================

    double fXResolution_pxpi = Units.Length.pxpi(EDirection::Horizontal);
    double fYResolution_pxpi = Units.Length.pxpi(EDirection::Vertical);
    // (1 Inch = 2.54cm = 25.4 mm)
    double fXResolution_pxpmm = fXResolution_pxpi/25.4;
    double fYResolution_pxpmm = fYResolution_pxpi/25.4;

    QString strXPxpi  = QString::number(fXResolution_pxpi,'f',2);
    QString strYPxpi  = QString::number(fYResolution_pxpi,'f',2);
    QString strXPxpmm = QString::number(fXResolution_pxpmm,'f',2);
    QString strYPxpmm = QString::number(fYResolution_pxpmm,'f',2);
    QString strResolution;

    m_pLytSepLineResolution = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytSepLineResolution);
    m_pLblSepLineResolution = new QLabel("Screen Resolution");
    m_pLytSepLineResolution->addWidget(m_pLblSepLineResolution);
    m_pSepLineImageSize_px = new CSepLine(10);
    m_pLytSepLineResolution->addWidget(m_pSepLineImageSize_px,1);

    // <Line> Resolution
    //------------------

    m_pLytLineResolution = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytLineResolution);

    strResolution = strXPxpi + " * " + strYPxpi;
    m_pLblResolution_pxpi = new QLabel("Pixels/Inch:");
    m_pLblResolution_pxpi->setFixedWidth(m_cxLblWidthClm1);
    m_pLytLineResolution->addWidget(m_pLblResolution_pxpi);
    m_pEdtResolution_pxpi = new QLineEdit(strResolution);
    m_pEdtResolution_pxpi->setFixedWidth(m_cxEdtWidthClm1);
    m_pEdtResolution_pxpi->setReadOnly(true);
    m_pLytLineResolution->addWidget(m_pEdtResolution_pxpi);
    m_pLytLineResolution->addSpacing(m_cxClmSpacing);

    strResolution = strXPxpmm + " * " + strYPxpmm;
    m_pLblResolution_pxpmm = new QLabel("Pixels/mm:");
    m_pLblResolution_pxpmm->setFixedWidth(m_cxLblWidthClm2);
    m_pLytLineResolution->addWidget(m_pLblResolution_pxpmm);
    m_pEdtResolution_pxpmm = new QLineEdit(strResolution);
    m_pEdtResolution_pxpmm->setFixedWidth(m_cxEdtWidthClm2);
    m_pEdtResolution_pxpmm->setReadOnly(true);
    m_pLytLineResolution->addWidget(m_pEdtResolution_pxpmm);
    m_pLytLineResolution->addStretch();

    // <Section> Image Size in Metric System
    //======================================

    #pragma message(__TODO__"Add Edit Controls to set Resolution, Minimum and Maximum")

    m_pLytSepLineImageMetric = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytSepLineImageMetric);
    m_pLblSepLineImageMetric = new QLabel("Metric Sizes");
    m_pLytSepLineImageMetric->addWidget(m_pLblSepLineImageMetric);
    m_pSepLineImageMetric = new CSepLine(10);
    m_pLytSepLineImageMetric->addWidget(m_pSepLineImageMetric,1);

    // <Line> Unit
    //------------

    m_pLytLineImageMetricUnit = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytLineImageMetricUnit);

    m_pLblImageMetricUnit = new QLabel("Unit:");
    m_pLblImageMetricUnit->setFixedWidth(m_cxLblWidthClm2);
    m_pLytLineImageMetricUnit->addWidget(m_pLblImageMetricUnit);
    m_pCmbImageMetricUnit = new QComboBox();
    m_pCmbImageMetricUnit->setFixedWidth(m_cxEdtWidthClm2);
    m_pCmbImageMetricUnit->setEnabled(false);
    m_pLytLineImageMetricUnit->addWidget(m_pCmbImageMetricUnit);
    m_pLytLineImageMetricUnit->addStretch();
    for( int idxUnit = 0; idxUnit < Units.Length.count(); ++idxUnit ) {
        CUnitsTreeEntryPhysUnit* pUnitEntry =
            dynamic_cast<CUnitsTreeEntryPhysUnit*>(Units.Length.at(idxUnit));
        if( pUnitEntry != nullptr ) {
            if( Units.Length.isMetricUnit(pUnitEntry->symbol()) ) {
                m_pCmbImageMetricUnit->addItem(pUnitEntry->symbol());
            }
        }
    }
    m_pCmbImageMetricUnit->setCurrentText(m_strMetricUnitSymbol);
    QObject::connect(
        m_pCmbImageMetricUnit, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtDrawingViewProperties::onCmbImageMetricUnitCurrentIndexChanged );

    // <Line> Metric Image Size
    //-------------------------

    m_pLytLineMetricSize = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytLineMetricSize);

    // <Edit> Image Width
    //--------------------

    m_pLblImageMetricWidth = new QLabel("Width:");
    m_pLblImageMetricWidth->setFixedWidth(m_cxLblWidthClm1);
    m_pLytLineMetricSize->addWidget(m_pLblImageMetricWidth);
    m_pEdtImageMetricWidth = new CWdgtEditPhysVal();
    m_pEdtImageMetricWidth->setFixedWidth(m_cxEdtWidthClm1);
    m_pEdtImageMetricWidth->setReadOnly(true);
    m_pLytLineMetricSize->addWidget(m_pEdtImageMetricWidth);
    m_pLytLineMetricSize->addSpacing(m_cxClmSpacing);
    QObject::connect(
        m_pEdtImageMetricWidth, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtDrawingViewProperties::onEdtImageMetricWidthValueChanged);

    // <Edit> Image Height
    //--------------------

    m_pLblImageMetricHeight = new QLabel("Height:");
    m_pLblImageMetricHeight->setFixedWidth(m_cxLblWidthClm2);
    m_pLytLineMetricSize->addWidget(m_pLblImageMetricHeight);
    m_pEdtImageMetricHeight = new CWdgtEditPhysVal();
    m_pEdtImageMetricHeight->setFixedWidth(m_cxEdtWidthClm2);
    m_pEdtImageMetricHeight->setReadOnly(true);
    m_pLytLineMetricSize->addWidget(m_pEdtImageMetricHeight);
    m_pLytLineMetricSize->addStretch();
    QObject::connect(
        m_pEdtImageMetricHeight, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtDrawingViewProperties::onEdtImageMetricHeightValueChanged);

    // <Line> Normed Paper Sizes
    //--------------------------

    m_pLytLineImageMetricNormedPaper = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytLineImageMetricNormedPaper);

    // <ComboBox> Normed Paper Sizes
    //------------------------------

    m_pLblImageMetricNormedPaperSizes = new QLabel("Normed Sizes:");
    m_pLblImageMetricNormedPaperSizes->setFixedWidth(m_cxLblWidthClm1);
    m_pLytLineImageMetricNormedPaper->addWidget(m_pLblImageMetricNormedPaperSizes);
    m_pCmbImageMetricNormedPaperSizes = new QComboBox();
    m_pCmbImageMetricNormedPaperSizes->setFixedWidth(m_cxEdtWidthClm1);
    m_pCmbImageMetricNormedPaperSizes->setEnabled(false);
    m_pLytLineImageMetricNormedPaper->addWidget(m_pCmbImageMetricNormedPaperSizes);
    m_pLytLineImageMetricNormedPaper->addSpacing(m_cxClmSpacing);
    for( CEnumNormedPaperSize ePaperSize = 0; ePaperSize < CEnumNormedPaperSize::count(); ++ePaperSize ) {
        if( ePaperSize == ENormedPaperSize::Undefined) {
            m_pCmbImageMetricNormedPaperSizes->addItem(
                ePaperSize.toString(EEnumEntryAliasStrText), ePaperSize.toValue());
        }
        else {
            m_pCmbImageMetricNormedPaperSizes->addItem(
                ePaperSize.toString(EEnumEntryAliasStrSymbol), ePaperSize.toValue());
        }
    }
    m_pCmbImageMetricNormedPaperSizes->setCurrentText(m_strImageMetricNormedPaperSize);
    QObject::connect(
        m_pCmbImageMetricNormedPaperSizes, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtDrawingViewProperties::onCmbImageMetricNormedPaperSizesCurrentIndexChanged );

    // <ComboBox> Paper Layout
    //------------------------

    m_pLblImageMetricNormedPaperOrientation = new QLabel("Orientation:");
    m_pLblImageMetricNormedPaperOrientation->setFixedWidth(m_cxLblWidthClm1);
    m_pLytLineImageMetricNormedPaper->addWidget(m_pLblImageMetricNormedPaperOrientation);
    m_pLblImageMetricNormedPaperOrientation->setVisible(false);
    m_pCmbImageMetricNormedPaperOrientation = new QComboBox();
    m_pCmbImageMetricNormedPaperOrientation->setFixedWidth(m_cxEdtWidthClm1);
    m_pCmbImageMetricNormedPaperOrientation->setEnabled(false);
    m_pLytLineImageMetricNormedPaper->addWidget(m_pCmbImageMetricNormedPaperOrientation);
    m_pLytLineImageMetricNormedPaper->addStretch();
    m_pCmbImageMetricNormedPaperOrientation->addItem(m_strImageMetricNormedPaperOrientation);
    m_pCmbImageMetricNormedPaperOrientation->setVisible(false);
    QObject::connect(
        m_pCmbImageMetricNormedPaperOrientation, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtDrawingViewProperties::onCmbImageMetricNormedPaperOrientationCurrentIndexChanged );

    // <Line> Scale Factor
    //--------------------

    m_pLytLineImageMetricScaleFactor = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytLineImageMetricScaleFactor);

    m_pLblImageMetricScaleFactor = new QLabel("Scale Factor:");
    m_pLblImageMetricScaleFactor->setFixedWidth(m_cxLblWidthClm1);
    m_pLytLineImageMetricScaleFactor->addWidget(m_pLblImageMetricScaleFactor);

    QWidget* pWdgtImageMetricScaleFactor = new QWidget();
    pWdgtImageMetricScaleFactor->setFixedWidth(m_cxEdtWidthClm1);
    QHBoxLayout* pLytWdgtImageMetricScaleFactor = new QHBoxLayout();
    pLytWdgtImageMetricScaleFactor->setContentsMargins(0,0,0,0);
    pWdgtImageMetricScaleFactor->setLayout(pLytWdgtImageMetricScaleFactor);
    m_pLytLineImageMetricScaleFactor->addWidget(pWdgtImageMetricScaleFactor);

    m_pCmbImageMetricScaleFactorDividend = new QComboBox();
    m_pCmbImageMetricScaleFactorDividend->setEnabled(false);
    m_pCmbImageMetricScaleFactorDividend->setEditable(true);
    m_pCmbImageMetricScaleFactorDividend->setInsertPolicy(QComboBox::NoInsert);
    m_pCmbImageMetricScaleFactorDividend->setValidator(
        new QIntValidator(m_pCmbImageMetricScaleFactorDividend));
    pLytWdgtImageMetricScaleFactor->addWidget(m_pCmbImageMetricScaleFactorDividend, 1);
    m_pLblImageMetricScaleFactorHyphen = new QLabel(":");
    pLytWdgtImageMetricScaleFactor->addWidget(m_pLblImageMetricScaleFactorHyphen);
    m_pCmbImageMetricScaleFactorDivisor = new QComboBox();
    m_pCmbImageMetricScaleFactorDivisor->setEnabled(false);
    m_pCmbImageMetricScaleFactorDivisor->setEditable(true);
    m_pCmbImageMetricScaleFactorDivisor->setInsertPolicy(QComboBox::NoInsert);
    m_pCmbImageMetricScaleFactorDivisor->setValidator(
        new QIntValidator(m_pCmbImageMetricScaleFactorDivisor));
    pLytWdgtImageMetricScaleFactor->addWidget(m_pCmbImageMetricScaleFactorDivisor, 1);
    m_pLytLineImageMetricScaleFactor->addStretch();

    const QVector<int> arMetricScaleFactorsPredefined = {
        1, 2, 5, 10, 20, 50, 100, 200, 500,
        1000, 2000, 5000, 10000, 20000, 50000,
        100000, 200000, 500000, 1000000
    };
    for( int iScaleFactor : arMetricScaleFactorsPredefined ) {
        m_pCmbImageMetricScaleFactorDividend->addItem(QString::number(iScaleFactor));
        m_pCmbImageMetricScaleFactorDivisor->addItem(QString::number(iScaleFactor));
    }
    m_pCmbImageMetricScaleFactorDividend->setCurrentText(
        QString::number(m_iMetricScaleFactorDividend));
    m_pCmbImageMetricScaleFactorDivisor->setCurrentText(
        QString::number(m_iMetricScaleFactorDivisor));
    QObject::connect(
        m_pCmbImageMetricScaleFactorDividend, &QComboBox::currentTextChanged,
        this, &CWdgtDrawingViewProperties::onCmbImageMetricScaleFactorDividendCurrentTextChanged );
    QObject::connect(
        m_pCmbImageMetricScaleFactorDividend, &QComboBox::editTextChanged,
        this, &CWdgtDrawingViewProperties::onCmbImageMetricScaleFactorDividendEditTextChanged );
    QObject::connect(
        m_pCmbImageMetricScaleFactorDivisor, &QComboBox::currentTextChanged,
        this, &CWdgtDrawingViewProperties::onCmbImageMetricScaleFactorDivisorCurrentTextChanged );
    QObject::connect(
        m_pCmbImageMetricScaleFactorDivisor, &QComboBox::editTextChanged,
        this, &CWdgtDrawingViewProperties::onCmbImageMetricScaleFactorDivisorEditTextChanged );

    // <Visibility> Metric Widget
    //---------------------------

    m_pWdgtMetric->setVisible(false);

    // <Section> Image Size
    //=====================

    m_pLytSepLineImageSize_px = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSepLineImageSize_px);
    m_pLblSepLineImageSize_px = new QLabel("Pixel Sizes");
    m_pLytSepLineImageSize_px->addWidget(m_pLblSepLineImageSize_px);
    m_pSepLineImageSize_px = new CSepLine(10);
    m_pLytSepLineImageSize_px->addWidget(m_pSepLineImageSize_px,1);

    // <Line> Image Size
    //------------------

    m_pLytLineImageSize_px = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineImageSize_px);

    // <Edit> Image Width
    //--------------------

    m_pLblImageSizeWidth_px = new QLabel("Width:");
    m_pLblImageSizeWidth_px->setFixedWidth(m_cxLblWidthClm1);
    m_pLytLineImageSize_px->addWidget(m_pLblImageSizeWidth_px);
    m_pEdtImageSizeWidth_px = new QSpinBox();
    m_pEdtImageSizeWidth_px->setFixedWidth(m_cxEdtWidthClm1);
    m_pEdtImageSizeWidth_px->setMinimum(1);
    m_pEdtImageSizeWidth_px->setMaximum(100000);
    m_pEdtImageSizeWidth_px->setValue(m_fImageSizeWidth_px);
    m_pEdtImageSizeWidth_px->setSuffix(" px");
    m_pEdtImageSizeWidth_px->setReadOnly(false);
    m_pLytLineImageSize_px->addWidget(m_pEdtImageSizeWidth_px);
    m_pLytLineImageSize_px->addSpacing(m_cxClmSpacing);
    QObject::connect(
        m_pEdtImageSizeWidth_px, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtDrawingViewProperties::onEdtImageSizeWidthPxValueChanged);

    // <Edit> Image Height
    //--------------------

    m_pLblImageSizeHeight_px = new QLabel("Height:");
    m_pLblImageSizeHeight_px->setFixedWidth(m_cxLblWidthClm2);
    m_pLytLineImageSize_px->addWidget(m_pLblImageSizeHeight_px);
    m_pEdtImageSizeHeight_px = new QSpinBox();
    m_pEdtImageSizeHeight_px->setFixedWidth(m_cxEdtWidthClm2);
    m_pEdtImageSizeHeight_px->setMinimum(1);
    m_pEdtImageSizeHeight_px->setMaximum(100000);
    m_pEdtImageSizeHeight_px->setValue(m_fImageSizeHeight_px);
    m_pEdtImageSizeHeight_px->setSuffix(" px");
    m_pEdtImageSizeHeight_px->setReadOnly(false);
    m_pLytLineImageSize_px->addWidget(m_pEdtImageSizeHeight_px);
    m_pLytLineImageSize_px->addStretch();
    QObject::connect(
        m_pEdtImageSizeHeight_px, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtDrawingViewProperties::onEdtImageSizeHeightPxValueChanged);

    // <Stretch> At bottom of main layout
    //-----------------------------------

    m_pLyt->addStretch();

    // Update metrics
    //---------------

    updateImageSizeMetrics();

} // ctor

//------------------------------------------------------------------------------
CWdgtDrawingViewProperties::~CWdgtDrawingViewProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDrawingView = nullptr;
    m_pLyt = nullptr;
    m_pLytLineDimensionUnit = nullptr;
    m_pLblDimensionUnit = nullptr;
    m_pCmbDimensionUnit = nullptr;
    // Metric system
    m_pWdgtMetric = nullptr;
    m_pLytWdgtMetric = nullptr;
    // Resolution of a pixel on screen
    m_pLytSepLineResolution = nullptr;
    m_pLblSepLineResolution = nullptr;
    m_pSepLineResolution = nullptr;
    m_pLytLineResolution = nullptr;
    m_pLblResolution_pxpi = nullptr;
    m_pEdtResolution_pxpi = nullptr;
    m_pLblResolution_pxpmm = nullptr;
    m_pEdtResolution_pxpmm = nullptr;
    // Image Size in Metric System
    m_pLytSepLineImageMetric = nullptr;
    m_pLblSepLineImageMetric = nullptr;
    m_pSepLineImageMetric = nullptr;
    m_pLytLineImageMetricUnit = nullptr;
    m_pLblImageMetricUnit = nullptr;
    m_pCmbImageMetricUnit = nullptr;
    m_pLytLineMetricSize = nullptr;
    m_pLblImageMetricWidth = nullptr;
    m_pEdtImageMetricWidth = nullptr;
    m_pLblImageMetricHeight = nullptr;
    m_pEdtImageMetricHeight = nullptr;
    m_pLytLineImageMetricNormedPaper = nullptr;
    m_pLblImageMetricNormedPaperSizes = nullptr;
    m_pCmbImageMetricNormedPaperSizes = nullptr;
    m_pLblImageMetricNormedPaperOrientation = nullptr;
    m_pCmbImageMetricNormedPaperOrientation = nullptr;
    m_pLytLineImageMetricScaleFactor = nullptr;
    m_pLblImageMetricScaleFactor = nullptr;
    m_pCmbImageMetricScaleFactorDividend = nullptr;
    m_pLblImageMetricScaleFactorHyphen = nullptr;
    m_pCmbImageMetricScaleFactorDivisor = nullptr;
    // Image Size in Pixels
    m_pLytSepLineImageSize_px = nullptr;
    m_pLblSepLineImageSize_px = nullptr;
    m_pSepLineImageSize_px = nullptr;
    m_pLytLineImageSize_px = nullptr;
    m_pLblImageSizeWidth_px = nullptr;
    m_pEdtImageSizeWidth_px = nullptr;
    m_pLblImageSizeHeight_px = nullptr;
    m_pEdtImageSizeHeight_px = nullptr;
    // Caching values
    m_dimensionUnit = static_cast<EDrawingDimensionUnit>(0);
    //m_strMetricUnitSymbol;
    m_fImageMetricWidth = 0.0;
    m_fImageMetricHeight = 0.0;
    //m_strImageMetricNormedPaperSize;
    //m_strImageMetricNormedPaperOrientation;
    m_iMetricScaleFactorDividend = 0;
    m_iMetricScaleFactorDivisor = 0;
    m_fImageSizeWidth_px = 0.0;
    m_fImageSizeHeight_px = 0.0;
    m_iValueChangedSignalsBlocked = 0;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onDrawingViewDrawingSizeChanged(const QSize& i_size)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = qSize2Str(i_size);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingViewDrawingSizeChanged",
        /* strAddInfo   */ strMthInArgs );

    CPhysVal physValWidth(i_size.width(), Units.Length.pxX);
    CPhysVal physValHeight(i_size.height(), Units.Length.pxY);
    if( physValWidth.getVal() > 0.0 && physValHeight.getVal() > 0.0 ) {
        setImageSize(physValWidth, physValHeight);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onCmbDimensionUnitCurrentIndexChanged( int i_idx )
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbDimensionUnitCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

    setDimensionUnit(static_cast<EDrawingDimensionUnit>(i_idx));
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onCmbImageMetricUnitCurrentIndexChanged( int i_idx )
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbImageMetricUnitCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

    setMetricUnit(m_pCmbImageMetricUnit->currentText());
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onEdtImageMetricWidthValueChanged(const CPhysVal& i_physValWidth)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_physValWidth.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtImageMetricWidthValueChanged",
        /* strAddInfo   */ strMthInArgs );

    CPhysVal physValHeight = m_pEdtImageMetricHeight->value();
    if( i_physValWidth.getVal() > 0.0 && physValHeight.getVal() > 0.0 ) {
        setImageSize(i_physValWidth, physValHeight);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onEdtImageMetricHeightValueChanged(const CPhysVal& i_physValHeight)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_physValHeight.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtImageMetricHeightValueChanged",
        /* strAddInfo   */ strMthInArgs );

    CPhysVal physValWidth = m_pEdtImageMetricWidth->value();
    if( physValWidth.getVal() > 0.0 && i_physValHeight.getVal() > 0.0 ) {
        setImageSize(physValWidth, i_physValHeight);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onCmbImageMetricNormedPaperSizesCurrentIndexChanged(int i_idx)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbImageMetricNormedPaperSizesCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

    setNormedPaperSize(m_pCmbImageMetricNormedPaperSizes->currentText());
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onCmbImageMetricNormedPaperOrientationCurrentIndexChanged(int i_idx)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbImageMetricNormedPaperOrientationCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

    setNormedPaperOrientation(m_pCmbImageMetricNormedPaperOrientation->currentText());
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onCmbImageMetricScaleFactorDividendCurrentTextChanged(const QString& i_strDividend)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_strDividend;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbImageMetricScaleFactorDividendCurrentTextChanged",
        /* strAddInfo   */ strMthInArgs );

    QString strDivisor = m_pCmbImageMetricScaleFactorDivisor->currentText();
    if( !strDivisor.isEmpty() && !i_strDividend.isEmpty() ) {
        bool bDividendIsInt = false;
        bool bDivisorIsInt = false;
        int iDividend = i_strDividend.toInt(&bDividendIsInt);
        int iDivisor = strDivisor.toInt(&bDivisorIsInt);
        if( bDividendIsInt && bDivisorIsInt && iDividend > 0 && iDivisor > 0 ) {
            setScaleFactor(iDividend, iDivisor);
        }
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onCmbImageMetricScaleFactorDividendEditTextChanged(const QString& i_strDividend)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_strDividend;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbImageMetricScaleFactorDividendEditTextChanged",
        /* strAddInfo   */ strMthInArgs );

    QString strDivisor = m_pCmbImageMetricScaleFactorDivisor->currentText();
    if( !strDivisor.isEmpty() && !i_strDividend.isEmpty() ) {
        bool bDividendIsInt = false;
        bool bDivisorIsInt = false;
        int iDividend = i_strDividend.toInt(&bDividendIsInt);
        int iDivisor = strDivisor.toInt(&bDivisorIsInt);
        if( bDividendIsInt && bDivisorIsInt && iDividend > 0 && iDivisor > 0 ) {
            setScaleFactor(iDividend, iDivisor);
        }
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onCmbImageMetricScaleFactorDivisorCurrentTextChanged(const QString& i_strDivisor)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_strDivisor;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbImageMetricScaleFactorDivisorCurrentTextChanged",
        /* strAddInfo   */ strMthInArgs );

    QString strDividend = m_pCmbImageMetricScaleFactorDividend->currentText();
    if( !i_strDivisor.isEmpty() && !strDividend.isEmpty() ) {
        bool bDividendIsInt = false;
        bool bDivisorIsInt = false;
        int iDividend = strDividend.toInt(&bDividendIsInt);
        int iDivisor = i_strDivisor.toInt(&bDivisorIsInt);
        if( bDividendIsInt && bDivisorIsInt && iDividend > 0 && iDivisor > 0 ) {
            setScaleFactor(iDividend, iDivisor);
        }
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onCmbImageMetricScaleFactorDivisorEditTextChanged(const QString& i_strDivisor)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_strDivisor;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbImageMetricScaleFactorDivisorEditTextChanged",
        /* strAddInfo   */ strMthInArgs );

    QString strDividend = m_pCmbImageMetricScaleFactorDividend->currentText();
    if( !i_strDivisor.isEmpty() && !strDividend.isEmpty() ) {
        bool bDividendIsInt = false;
        bool bDivisorIsInt = false;
        int iDividend = strDividend.toInt(&bDividendIsInt);
        int iDivisor = i_strDivisor.toInt(&bDivisorIsInt);
        if( bDividendIsInt && bDivisorIsInt && iDividend > 0 && iDivisor > 0 ) {
            setScaleFactor(iDividend, iDivisor);
        }
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onEdtImageSizeWidthPxValueChanged(int i_cxWidth_px)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString::number(i_cxWidth_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtImageSizeWidthPxValueChanged",
        /* strAddInfo   */ strMthInArgs );

    CPhysVal physValWidth(i_cxWidth_px, Units.Length.pxX);
    CPhysVal physValHeight(m_pEdtImageSizeHeight_px->value(), Units.Length.pxY);
    if( physValWidth.getVal() > 0.0 && physValHeight.getVal() > 0.0 ) {
        setImageSize(physValWidth, physValHeight);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onEdtImageSizeHeightPxValueChanged(int i_cyHeight_px)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString::number(i_cyHeight_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtImageSizeHeightPxValueChanged",
        /* strAddInfo   */ strMthInArgs );

    CPhysVal physValWidth(m_pEdtImageSizeWidth_px->value(), Units.Length.pxX);
    CPhysVal physValHeight(i_cyHeight_px, Units.Length.pxY);
    if( physValWidth.getVal() > 0.0 && physValHeight.getVal() > 0.0 ) {
        setImageSize(physValWidth, physValHeight);
    }
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setDimensionUnit( EDrawingDimensionUnit i_dimensionUnit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = CEnumDrawingDimensionUnit(i_dimensionUnit).toString()
                     + " (Prev: " + CEnumDrawingDimensionUnit(m_dimensionUnit).toString() + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDimensionUnit",
        /* strAddInfo   */ strMthInArgs );

    if( m_dimensionUnit != i_dimensionUnit ) {
        m_dimensionUnit = i_dimensionUnit;
        m_pCmbDimensionUnit->setCurrentIndex(static_cast<int>(m_dimensionUnit));
        m_pWdgtMetric->setVisible(
            m_dimensionUnit == EDrawingDimensionUnit::Metric);
        m_pCmbImageMetricUnit->setEnabled(
            m_dimensionUnit == EDrawingDimensionUnit::Metric);
        m_pEdtImageMetricWidth->setReadOnly(
            m_dimensionUnit != EDrawingDimensionUnit::Metric);
        m_pEdtImageMetricHeight->setReadOnly(
            m_dimensionUnit != EDrawingDimensionUnit::Metric);
        m_pCmbImageMetricNormedPaperSizes->setEnabled(
            m_dimensionUnit == EDrawingDimensionUnit::Metric);
        m_pCmbImageMetricNormedPaperOrientation->setEnabled(
            m_dimensionUnit == EDrawingDimensionUnit::Metric);
        m_pCmbImageMetricScaleFactorDividend->setEnabled(
            m_dimensionUnit == EDrawingDimensionUnit::Metric);
        m_pCmbImageMetricScaleFactorDivisor->setEnabled(
            m_dimensionUnit == EDrawingDimensionUnit::Metric);
        m_pEdtImageSizeWidth_px->setReadOnly(
            m_dimensionUnit != EDrawingDimensionUnit::Pixels);
        m_pEdtImageSizeHeight_px->setReadOnly(
            m_dimensionUnit != EDrawingDimensionUnit::Pixels);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setMetricUnit( const QString& i_strMetricUnitSymbol )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_strMetricUnitSymbol + " (Prev: " + m_strMetricUnitSymbol + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMetricUnit",
        /* strAddInfo   */ strMthInArgs );

    if( m_strMetricUnitSymbol != i_strMetricUnitSymbol ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        // If changing the unit the metrics width and height and also the
        // size in pixels remain the same. Only the unit in which the values
        // are indicated will be changed.
        CPhysVal physValWidth(m_fImageMetricWidth, m_strMetricUnitSymbol);   // Value in previous unit
        CPhysVal physValHeight(m_fImageMetricHeight, m_strMetricUnitSymbol); // Value in previous unit
        m_strMetricUnitSymbol = i_strMetricUnitSymbol;
        physValWidth.convertValue(m_strMetricUnitSymbol);
        physValHeight.convertValue(m_strMetricUnitSymbol);
        m_fImageMetricWidth = physValWidth.getVal();
        m_fImageMetricHeight = physValHeight.getVal();
        m_pCmbImageMetricUnit->setCurrentText(m_strMetricUnitSymbol);
        m_pEdtImageMetricWidth->setUnit(physValWidth.unit());
        m_pEdtImageMetricHeight->setUnit(physValHeight.unit());
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setNormedPaperSize( const QString& i_strPaperSize )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_strPaperSize + " (Prev: " + m_strImageMetricNormedPaperSize + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setNormedPaperSize",
        /* strAddInfo   */ strMthInArgs );

    if( m_strImageMetricNormedPaperSize != i_strPaperSize ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_strImageMetricNormedPaperSize = i_strPaperSize;
        CEnumNormedPaperSize ePaperSize = m_strImageMetricNormedPaperSize;
        m_pLblImageMetricNormedPaperOrientation->setVisible(ePaperSize != ENormedPaperSize::Undefined);
        m_pCmbImageMetricNormedPaperOrientation->setVisible(ePaperSize != ENormedPaperSize::Undefined);
        m_pCmbImageMetricNormedPaperOrientation->clear();
        if( ePaperSize == ENormedPaperSize::Undefined ) {
            // The combo box is hidden. Not really needed to update the items.
            // Only updated for the sake of clarification. But we keep the current
            // setting for the orientation so that it can be restored when switching back
            // to a normed paper size.
            CEnumDirection eDirection(EDirection::Undefined);
            m_pCmbImageMetricNormedPaperOrientation->addItem(eDirection.toString(EEnumEntryAliasStrText));
        }
        else {
            for( CEnumDirection eDirection = 0; eDirection < CEnumDirection::count(); ++eDirection ) {
                if( eDirection != EDirection::Undefined) {
                    m_pCmbImageMetricNormedPaperOrientation->addItem(
                        eDirection.toString(EEnumEntryAliasStrText));
                }
            }
            CEnumDirection eDirection = m_strImageMetricNormedPaperOrientation;
            if( eDirection == EDirection::Undefined ) {
                eDirection = EDirection::Horizontal;
                m_strImageMetricNormedPaperOrientation = eDirection.toString();
            }
            m_pCmbImageMetricNormedPaperOrientation->setCurrentText(m_strImageMetricNormedPaperOrientation);

            QVariant varPaperSize = ePaperSize.toValue();
            if( varPaperSize.type() == QVariant::SizeF ) {
                QSizeF sizeF = varPaperSize.toSizeF();
                // Default: Horizontal Orientation
                CPhysVal physValWidth(sizeF.height(), Units.Length.mm);
                CPhysVal physValHeight(sizeF.width(), Units.Length.mm);
                if( eDirection == EDirection::Vertical ) {
                    physValWidth = sizeF.width();
                    physValHeight = sizeF.height();
                }
                physValWidth.convertValue(m_strMetricUnitSymbol);
                physValHeight.convertValue(m_strMetricUnitSymbol);
                m_fImageMetricWidth = physValWidth.getVal();
                m_fImageMetricHeight = physValHeight.getVal();
                m_pEdtImageMetricWidth->setValue(physValWidth.getVal());
                m_pEdtImageMetricHeight->setValue(physValHeight.getVal());
                updateImageSizeInPixels();
            }
        }
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setNormedPaperOrientation( const QString& i_strOrientation )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_strOrientation + " (Prev: " + m_strImageMetricNormedPaperOrientation + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setNormedPaperOrientation",
        /* strAddInfo   */ strMthInArgs );

    if( m_strImageMetricNormedPaperOrientation != i_strOrientation ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_strImageMetricNormedPaperOrientation = i_strOrientation;
        // Setting values according to orientation makes only sense for normed paper sizes.
        // For user defined paper sizes there is no orientation. The width might be
        // greater than the height or vice versa. The orientation will be stored if later
        // on a normed paper size is set but for now the values will not be adjusted.
        CEnumNormedPaperSize ePaperSize = m_strImageMetricNormedPaperSize;
        if( ePaperSize != ENormedPaperSize::Undefined ) {
            QVariant varPaperSize = ePaperSize.toValue();
            if( varPaperSize.type() == QVariant::SizeF ) {
                QSizeF sizeF = varPaperSize.toSizeF();
                CEnumDirection eDirection = m_strImageMetricNormedPaperOrientation;
                // Default: Horizontal Orientation
                CPhysVal physValWidth(sizeF.height(), Units.Length.mm);
                CPhysVal physValHeight(sizeF.width(), Units.Length.mm);
                if( eDirection == EDirection::Vertical ) {
                    physValWidth = sizeF.width();
                    physValHeight = sizeF.height();
                }
                physValWidth.convertValue(m_strMetricUnitSymbol);
                physValHeight.convertValue(m_strMetricUnitSymbol);
                m_fImageMetricWidth = physValWidth.getVal();
                m_fImageMetricHeight = physValHeight.getVal();
                m_pEdtImageMetricWidth->setValue(physValWidth.getVal());
                m_pEdtImageMetricHeight->setValue(physValHeight.getVal());
                updateImageSizeInPixels();
            }
        }
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setScaleFactor( int i_iDividend, int i_iDivisor )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString::number(i_iDividend) + "/" + QString::number(i_iDivisor)
                     + " (Prev: " + QString::number(m_iMetricScaleFactorDividend)
                     + "/" + QString::number(m_iMetricScaleFactorDivisor) + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScaleFactor",
        /* strAddInfo   */ strMthInArgs );

    if( i_iDividend <= 0 ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            "CWdgtDrawingViewProperties::setScaleFactor("
                + QString::number(i_iDividend) + ", " + QString::number(i_iDivisor) + "): "
                + "!Dividend <= 0!");
    }
    if( i_iDivisor <= 0 ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            "CWdgtDrawingViewProperties::setScaleFactor("
                + QString::number(i_iDividend) + ", " + QString::number(i_iDivisor) + "): "
                + "!Divisor <= 0!");
    }
    if( m_iMetricScaleFactorDividend != i_iDividend || m_iMetricScaleFactorDivisor != i_iDivisor ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_iMetricScaleFactorDividend = i_iDividend;
        m_iMetricScaleFactorDivisor = i_iDivisor;
        updateImageSizeInPixels();
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setImageSize(
    const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_physValWidth.toString() + ", " + i_physValHeight.toString()
                    + " (Prev: " + CPhysVal(m_fImageSizeWidth_px, Units.Length.pxX).toString()
                    + ", " + CPhysVal(m_fImageSizeHeight_px, Units.Length.pxY).toString() + ")"
                    + ", (Prev: " + CPhysVal(m_fImageMetricWidth, m_strMetricUnitSymbol).toString()
                    + ", " + CPhysVal(m_fImageMetricHeight, m_strMetricUnitSymbol).toString() + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setImageSize",
        /* strAddInfo   */ strMthInArgs );

    if( !i_physValWidth.isValid() || i_physValWidth.getVal() <= 0.0 ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            "CWdgtDrawingViewProperties::setImageSize("
                + i_physValWidth.toString() + ", " + i_physValHeight.toString() + "): "
                + "!i_physValWidth <= 0.0!");
    }
    if( !i_physValHeight.isValid() || i_physValHeight.getVal() <= 0.0 ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            "CWdgtDrawingViewProperties::setImageSize("
                + i_physValWidth.toString() + ", " + i_physValHeight.toString() + "): "
                + "!i_physValHeight <= 0.0!");
    }
    if( (i_physValWidth.unit() == Units.Length.pxX && i_physValHeight.unit() != Units.Length.pxY)
     || (i_physValWidth.unit() != Units.Length.pxX && i_physValHeight.unit() == Units.Length.pxY) ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            "CWdgtDrawingViewProperties::setImageSize("
                + i_physValWidth.toString() + ", " + i_physValHeight.toString() + "): "
                + "Either set size in pixels or in metrics");
    }

    CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
    if( i_physValWidth.unit() == Units.Length.pxX ) {
        double fImageSizeWidth_px = i_physValWidth.getVal();
        double fImageSizeHeight_px = i_physValHeight.getVal();
        if( m_fImageSizeWidth_px != fImageSizeWidth_px || m_fImageSizeHeight_px != fImageSizeHeight_px ) {
            m_fImageSizeWidth_px = fImageSizeWidth_px;
            m_fImageSizeHeight_px = fImageSizeHeight_px;
            updateImageSizeMetrics();
            updatePaperFormat();
        }
    }
    else {
        double fImageMetricWidth = i_physValWidth.getVal(m_strMetricUnitSymbol);
        double fImageMetricHeight = i_physValHeight.getVal(m_strMetricUnitSymbol);
        if( m_fImageMetricWidth != fImageMetricWidth || m_fImageMetricHeight != fImageMetricHeight ) {
            m_fImageMetricWidth = fImageMetricWidth;
            m_fImageMetricHeight = fImageMetricHeight;
            updateImageSizeInPixels();
            updatePaperFormat();
        }
    }
}

/*==============================================================================
protected: // instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::updateImageSizeInPixels()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateImageSizeInPixels",
        /* strAddInfo   */ "" );
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
    double fFactor =  static_cast<double>(m_iMetricScaleFactorDividend)
                    / static_cast<double>(m_iMetricScaleFactorDivisor);
    double fScaledWidth = fFactor * m_fImageMetricWidth;
    double fScaledHeight = fFactor * m_fImageMetricHeight;
    CPhysVal physValWidth(fScaledWidth, m_strMetricUnitSymbol);
    CPhysVal physValHeight(fScaledHeight, m_strMetricUnitSymbol);
    physValWidth.convertValue(Units.Length.pxX);
    physValHeight.convertValue(Units.Length.pxY);
    m_fImageSizeWidth_px = physValWidth.getVal();
    m_fImageSizeHeight_px = physValHeight.getVal();
    m_pEdtImageSizeWidth_px->setValue(physValWidth.getVal());
    m_pEdtImageSizeHeight_px->setValue(physValHeight.getVal());

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
} // updateImageSizeInPixels

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::updateImageSizeMetrics()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateImageSizeMetrics",
        /* strAddInfo   */ "" );
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
    CPhysVal physValWidth(m_fImageSizeWidth_px, Units.Length.pxX);
    CPhysVal physValHeight(m_fImageSizeHeight_px, Units.Length.pxY);
    physValWidth.convertValue(m_strMetricUnitSymbol);
    physValHeight.convertValue(m_strMetricUnitSymbol);
    m_fImageMetricWidth = physValWidth.getVal();
    m_fImageMetricHeight = physValHeight.getVal();
    m_pEdtImageMetricWidth->setUnit(physValWidth.unit());
    #pragma message(__TODO__"setResolution depending on screen resolution")
    m_pEdtImageMetricWidth->setResolution(0.001);
    m_pEdtImageMetricWidth->setMaximum(100000);
    m_pEdtImageMetricWidth->setValue(physValWidth.getVal());
    m_pEdtImageMetricHeight->setUnit(physValHeight.unit());
    #pragma message(__TODO__"setResolution depending on screen resolution")
    m_pEdtImageMetricHeight->setResolution(0.001);
    m_pEdtImageMetricHeight->setMaximum(1.0e6);
    m_pEdtImageMetricHeight->setValue(physValHeight.getVal());

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
} // updateImageSizeMetrics

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::updatePaperFormat()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updatePaperFormat",
        /* strAddInfo   */ "" );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);

    CPhysVal physValWidth(m_fImageMetricWidth, m_strMetricUnitSymbol);
    CPhysVal physValHeight(m_fImageMetricHeight, m_strMetricUnitSymbol);
    CEnumDirection eDirection = m_strImageMetricNormedPaperOrientation;
    if( eDirection == EDirection::Horizontal ) {
        physValWidth.setVal(m_fImageMetricHeight);
        physValHeight.setVal(m_fImageMetricWidth);
    }
    physValWidth.convertValue(Units.Length.mm);
    physValHeight.convertValue(Units.Length.mm);
    QSizeF sizePaper(physValWidth.getVal(), physValHeight.getVal());
    int idx = m_pCmbImageMetricNormedPaperSizes->findData(sizePaper);
    if( idx >= 0 ) {
        m_pCmbImageMetricNormedPaperSizes->setCurrentIndex(idx);
        if( m_strImageMetricNormedPaperSize != m_pCmbImageMetricNormedPaperSizes->currentText() ) {
            m_strImageMetricNormedPaperSize = m_pCmbImageMetricNormedPaperSizes->currentText();
            CEnumNormedPaperSize ePaperSize = m_strImageMetricNormedPaperSize;
            m_pLblImageMetricNormedPaperOrientation->setVisible(ePaperSize != ENormedPaperSize::Undefined);
            m_pCmbImageMetricNormedPaperOrientation->setVisible(ePaperSize != ENormedPaperSize::Undefined);
            m_pCmbImageMetricNormedPaperOrientation->clear();
            if( ePaperSize == ENormedPaperSize::Undefined ) {
                // The combo box is hidden. Not really needed to update the items.
                // Only updated for the sake of clarification. But we keep the current
                // setting for the orientation so that it can be restored when switching back
                // to a normed paper size.
                CEnumDirection eDirection(EDirection::Undefined);
                m_pCmbImageMetricNormedPaperOrientation->addItem(eDirection.toString(EEnumEntryAliasStrText));
            }
            else {
                for( CEnumDirection eDirection = 0; eDirection < CEnumDirection::count(); ++eDirection ) {
                    if( eDirection != EDirection::Undefined) {
                        m_pCmbImageMetricNormedPaperOrientation->addItem(
                            eDirection.toString(EEnumEntryAliasStrText));
                    }
                }
                if( eDirection == EDirection::Undefined ) {
                    eDirection = EDirection::Horizontal;
                    m_strImageMetricNormedPaperOrientation = eDirection.toString();
                }
                m_pCmbImageMetricNormedPaperOrientation->setCurrentText(m_strImageMetricNormedPaperOrientation);
            }
        }
    }
    else {
        CEnumNormedPaperSize ePaperSize = ENormedPaperSize::Undefined;
        m_strImageMetricNormedPaperSize = ePaperSize.toString(EEnumEntryAliasStrText);
        m_pCmbImageMetricNormedPaperSizes->setCurrentText(m_strImageMetricNormedPaperSize);
        m_pCmbImageMetricNormedPaperOrientation->setVisible(false);
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
} // updatePaperFormat

/*==============================================================================
protected: // instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::traceValues(CMethodTracer& mthTracer, EMethodDir i_methodDir)
//------------------------------------------------------------------------------
{
    QString strMthLog = QString(i_methodDir == EMethodDir::Enter ? "-+ " : "+- ")
        + "Cached Values {" + CEnumDrawingDimensionUnit(m_dimensionUnit).toString()
        + ", Paper (" + m_strImageMetricNormedPaperSize
            + ", " + m_strImageMetricNormedPaperOrientation + ")"
        + ", Scale (" + QString::number(m_iMetricScaleFactorDividend)
            + "/" + QString::number(m_iMetricScaleFactorDivisor) + ")"
        + ", Size (" + QString::number(m_fImageMetricWidth) + " " + m_strMetricUnitSymbol
            + " * " + QString::number(m_fImageMetricHeight) + " " + m_strMetricUnitSymbol + ")"
        + ", Size (" + QString::number(m_fImageSizeWidth_px)
            + " * " + QString::number(m_fImageSizeHeight_px) + " px)}";
    mthTracer.trace(strMthLog);
    strMthLog = QString(i_methodDir == EMethodDir::Enter ? "-+ " : "+- ")
        + "Edit Controls {" + m_pCmbDimensionUnit->currentText()
        + ", Paper (" + m_pCmbImageMetricNormedPaperSizes->currentText() 
            + ", " + m_pCmbImageMetricNormedPaperOrientation->currentText() + ")"
        + ", Scale (" + m_pCmbImageMetricScaleFactorDividend->currentText()
            + "/" + m_pCmbImageMetricScaleFactorDivisor->currentText() + ")"
        + ", Size (" + m_pEdtImageMetricWidth->value().toString()
            + " * " + m_pEdtImageMetricHeight->value().toString() + ")"
        + ", Size (" + QString::number(m_pEdtImageSizeWidth_px->value())
            + " * " + QString::number(m_pEdtImageSizeHeight_px->value()) + " px)}";
    mthTracer.trace(strMthLog);
}
