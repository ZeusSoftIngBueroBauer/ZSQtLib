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
    m_pLytLineImageMetricScaleFactor(nullptr),
    m_pLblImageMetricScaleFactor(nullptr),
    m_pCmbImageMetricScaleFactorDividend(nullptr),
    m_pLblImageMetricScaleFactorHyphen(nullptr),
    m_pCmbImageMetricScaleFactorDivisor(nullptr),
    m_pLblImageMetricUnit(nullptr),
    m_pCmbImageMetricUnit(nullptr),
    m_pLytLineMetricSize(nullptr),
    m_pLblImageMetricWidth(nullptr),
    m_pEdtImageMetricWidth(nullptr),
    m_pLblImageMetricHeight(nullptr),
    m_pEdtImageMetricHeight(nullptr),
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
    m_iMetricScaleFactorDividend(1),
    m_iMetricScaleFactorDivisor(1),
    m_strMetricUnitSymbol(Units.Length.mm.symbol()),
    m_fImageMetricWidth(0.0),
    m_fImageMetricHeight(0.0),
    m_cxImageSizeWidth_px(0),
    m_cyImageSizeHeight_px(0)
{
    m_pLyt = new QVBoxLayout;
    setLayout(m_pLyt);

    QObject::connect(
        m_pDrawingView, &CDrawingView::drawingSizeChanged,
        this, &CWdgtDrawingViewProperties::onDrawingViewDrawingSizeChanged );

    QSize sizeDrawing = m_pDrawingView->drawingSizeInPixels();

    m_cxImageSizeWidth_px = sizeDrawing.width();
    m_cyImageSizeHeight_px = sizeDrawing.height();

    CPhysVal physValWidth(m_cxImageSizeWidth_px, Units.Length.pxX);
    CPhysVal physValHeight(m_cyImageSizeHeight_px, Units.Length.pxY);

    physValWidth.convertValue(Units.Length.mm);
    physValHeight.convertValue(Units.Length.mm);

    m_fImageMetricWidth = physValWidth.getVal();
    m_fImageMetricHeight = physValHeight.getVal();

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

    // <Line> Scale Factor and Unit
    //-----------------------------

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
    pLytWdgtImageMetricScaleFactor->addWidget(m_pCmbImageMetricScaleFactorDividend, 1);
    m_pLblImageMetricScaleFactorHyphen = new QLabel(":");
    pLytWdgtImageMetricScaleFactor->addWidget(m_pLblImageMetricScaleFactorHyphen);
    m_pCmbImageMetricScaleFactorDivisor = new QComboBox();
    m_pCmbImageMetricScaleFactorDivisor->setEnabled(false);
    pLytWdgtImageMetricScaleFactor->addWidget(m_pCmbImageMetricScaleFactorDivisor, 1);
    m_pLytLineImageMetricScaleFactor->addSpacing(m_cxClmSpacing);

    const QVector<int> arMetricScaleFactorsPredefined = {
        1, 10, 100,
        static_cast<int>(1e3), static_cast<int>(1e4),
        static_cast<int>(1e5), static_cast<int>(1e6)
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
        m_pCmbImageMetricScaleFactorDivisor, &QComboBox::currentTextChanged,
        this, &CWdgtDrawingViewProperties::onCmbImageMetricScaleFactorDivisorCurrentTextChanged );

    m_pLblImageMetricUnit = new QLabel("Unit:");
    m_pLblImageMetricUnit->setFixedWidth(m_cxLblWidthClm2);
    m_pLytLineImageMetricScaleFactor->addWidget(m_pLblImageMetricUnit);
    m_pCmbImageMetricUnit = new QComboBox();
    m_pCmbImageMetricUnit->setFixedWidth(m_cxEdtWidthClm2);
    m_pCmbImageMetricUnit->setEnabled(false);
    m_pLytLineImageMetricScaleFactor->addWidget(m_pCmbImageMetricUnit);
    m_pLytLineImageMetricScaleFactor->addStretch();
    for( int idxUnit = 0; idxUnit < Units.Length.count(); ++idxUnit )
    {
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
    m_pEdtImageMetricWidth->setUnit(physValWidth.unit());
    m_pEdtImageMetricWidth->setValue(physValWidth.getVal());
    m_pEdtImageMetricWidth->setMinimum(1.0e-9);
    m_pEdtImageMetricWidth->setMaximum(100000);
    #pragma message(__TODO__"setResolution depending on screen resolution")
    m_pEdtImageMetricWidth->setResolution(0.001);
    m_pLytLineMetricSize->addWidget(m_pEdtImageMetricWidth);
    m_pLytLineMetricSize->addSpacing(m_cxClmSpacing);
    QObject::connect(
        m_pEdtImageMetricWidth, &CWdgtEditPhysVal::editingFinished,
        this, &CWdgtDrawingViewProperties::onEdtImageMetricWidthEditingFinished);

    // <Edit> Image Height
    //--------------------

    m_pLblImageMetricHeight = new QLabel("Height:");
    m_pLblImageMetricHeight->setFixedWidth(m_cxLblWidthClm2);
    m_pLytLineMetricSize->addWidget(m_pLblImageMetricHeight);
    m_pEdtImageMetricHeight = new CWdgtEditPhysVal();
    m_pEdtImageMetricHeight->setUnit(physValHeight.unit());
    m_pEdtImageMetricHeight->setValue(physValHeight.getVal());
    m_pEdtImageMetricHeight->setMinimum(1.0e-9);
    m_pEdtImageMetricHeight->setMaximum(1.0e6);
    #pragma message(__TODO__"setResolution depending on screen resolution")
    m_pEdtImageMetricHeight->setResolution(0.001);
    m_pEdtImageMetricHeight->setFixedWidth(m_cxEdtWidthClm2);
    m_pEdtImageMetricHeight->setReadOnly(true);
    m_pLytLineMetricSize->addWidget(m_pEdtImageMetricHeight);
    m_pLytLineMetricSize->addStretch();
    QObject::connect(
        m_pEdtImageMetricHeight, &CWdgtEditPhysVal::editingFinished,
        this, &CWdgtDrawingViewProperties::onEdtImageMetricHeightEditingFinished);

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
    m_pEdtImageSizeWidth_px->setValue(m_cxImageSizeWidth_px);
    m_pEdtImageSizeWidth_px->setSuffix(" px");
    m_pEdtImageSizeWidth_px->setReadOnly(false);
    m_pLytLineImageSize_px->addWidget(m_pEdtImageSizeWidth_px);
    m_pLytLineImageSize_px->addSpacing(m_cxClmSpacing);
    QObject::connect(
        m_pEdtImageSizeWidth_px, &QSpinBox::editingFinished,
        this, &CWdgtDrawingViewProperties::onEdtImageSizeWidthPxEditingFinished);

    // <Edit> Image Height
    //--------------------

    m_pLblImageSizeHeight_px = new QLabel("Height:");
    m_pLblImageSizeHeight_px->setFixedWidth(m_cxLblWidthClm2);
    m_pLytLineImageSize_px->addWidget(m_pLblImageSizeHeight_px);
    m_pEdtImageSizeHeight_px = new QSpinBox();
    m_pEdtImageSizeHeight_px->setFixedWidth(m_cxEdtWidthClm2);
    m_pEdtImageSizeHeight_px->setMinimum(1);
    m_pEdtImageSizeHeight_px->setMaximum(100000);
    m_pEdtImageSizeHeight_px->setValue(m_cyImageSizeHeight_px);
    m_pEdtImageSizeHeight_px->setSuffix(" px");
    m_pEdtImageSizeHeight_px->setReadOnly(false);
    m_pLytLineImageSize_px->addWidget(m_pEdtImageSizeHeight_px);
    m_pLytLineImageSize_px->addStretch();
    QObject::connect(
        m_pEdtImageSizeHeight_px, &QSpinBox::editingFinished,
        this, &CWdgtDrawingViewProperties::onEdtImageSizeHeightPxEditingFinished);

    // <Stretch> At bottom of main layout
    //-----------------------------------

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtDrawingViewProperties::~CWdgtDrawingViewProperties()
//------------------------------------------------------------------------------
{
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
    m_pLytLineImageMetricScaleFactor = nullptr;
    m_pLblImageMetricScaleFactor = nullptr;
    m_pCmbImageMetricScaleFactorDividend = nullptr;
    m_pLblImageMetricScaleFactorHyphen = nullptr;
    m_pCmbImageMetricScaleFactorDivisor = nullptr;
    m_pLblImageMetricUnit = nullptr;
    m_pCmbImageMetricUnit = nullptr;
    m_pLytLineMetricSize = nullptr;
    m_pLblImageMetricWidth = nullptr;
    m_pEdtImageMetricWidth = nullptr;
    m_pLblImageMetricHeight = nullptr;
    m_pEdtImageMetricHeight = nullptr;
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
    m_iMetricScaleFactorDividend = 0;
    m_iMetricScaleFactorDivisor = 0;
    //m_strMetricUnitSymbol;
    m_fImageMetricWidth = 0.0;
    m_fImageMetricHeight = 0.0;
    m_cxImageSizeWidth_px = 0;
    m_cyImageSizeHeight_px = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setDimensionUnit( EDrawingDimensionUnit i_dimensionUnit )
//------------------------------------------------------------------------------
{
    if( m_dimensionUnit != i_dimensionUnit ) {
        m_dimensionUnit = i_dimensionUnit;
        m_pCmbDimensionUnit->setCurrentIndex(static_cast<int>(m_dimensionUnit));
        m_pWdgtMetric->setVisible(
            m_dimensionUnit == EDrawingDimensionUnit::Metric);
        m_pCmbImageMetricScaleFactorDividend->setEnabled(
            m_dimensionUnit == EDrawingDimensionUnit::Metric);
        m_pCmbImageMetricScaleFactorDivisor->setEnabled(
            m_dimensionUnit == EDrawingDimensionUnit::Metric);
        m_pCmbImageMetricUnit->setEnabled(
            m_dimensionUnit == EDrawingDimensionUnit::Metric);
        m_pEdtImageMetricWidth->setReadOnly(
            m_dimensionUnit != EDrawingDimensionUnit::Metric);
        m_pEdtImageMetricHeight->setReadOnly(
            m_dimensionUnit != EDrawingDimensionUnit::Metric);
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
    if( m_strMetricUnitSymbol != i_strMetricUnitSymbol ) {
        // If changing the unit the metrics width and height and also the
        // size in pixels remain the same. Only the unit in which the values
        // are indicated will be changed.
        CPhysVal physValWidth(m_fImageMetricWidth, m_strMetricUnitSymbol);
        CPhysVal physValHeight(m_fImageMetricHeight, m_strMetricUnitSymbol);
        m_strMetricUnitSymbol = i_strMetricUnitSymbol;
        physValWidth.convertValue(m_strMetricUnitSymbol);
        physValHeight.convertValue(m_strMetricUnitSymbol);
        m_fImageMetricWidth = physValWidth.getVal();
        m_fImageMetricHeight = physValHeight.getVal();
        m_pCmbImageMetricUnit->setCurrentText(m_strMetricUnitSymbol);
        m_pEdtImageMetricWidth->setUnit(physValWidth.unit());
        m_pEdtImageMetricWidth->setValue(physValWidth.getVal());
        m_pEdtImageMetricWidth->setMinimum(1.0e-9);
        m_pEdtImageMetricWidth->setMaximum(1.0e6);
        #pragma message(__TODO__"setResolution depending on screen resolution")
        m_pEdtImageMetricWidth->setResolution(0.001);
        m_pEdtImageMetricHeight->setUnit(physValHeight.unit());
        m_pEdtImageMetricHeight->setValue(physValHeight.getVal());
        m_pEdtImageMetricHeight->setMinimum(1.0e-9);
        m_pEdtImageMetricHeight->setMaximum(1.0e6);
        #pragma message(__TODO__"setResolution depending on screen resolution")
        m_pEdtImageMetricHeight->setResolution(0.001);
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onDrawingViewDrawingSizeChanged(const QSize& i_size)
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onCmbDimensionUnitCurrentIndexChanged( int i_idx )
//------------------------------------------------------------------------------
{
    setDimensionUnit(static_cast<EDrawingDimensionUnit>(i_idx));
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onCmbImageMetricScaleFactorDividendCurrentTextChanged(const QString& i_strText)
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onCmbImageMetricScaleFactorDivisorCurrentTextChanged(const QString& i_strText)
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onCmbImageMetricUnitCurrentIndexChanged(int i_idx)
//------------------------------------------------------------------------------
{
    setMetricUnit(m_pCmbImageMetricUnit->currentText());
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onEdtImageMetricWidthEditingFinished()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onEdtImageMetricHeightEditingFinished()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onEdtImageSizeWidthPxEditingFinished()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onEdtImageSizeHeightPxEditingFinished()
//------------------------------------------------------------------------------
{
}
