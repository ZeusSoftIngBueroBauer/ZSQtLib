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

#include "ZSDraw/Widgets/Drawing/ZSDrawingViewPropertiesWdgt.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawingViewEditPropertyDlg.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Common/ZSDrawCommon.h"
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawingView.h"
#include "ZSPhysValGUI/ZSPhysValWdgtEditPhysVal.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtCore/qcoreapplication.h>
#include <QtGui/qevent.h>
#include <QtGui/qstandarditemmodel.h>

#if QT_VERSION < 0x050000
#include <QtGui/qcheckbox.h>
#include <QtGui/qcolordialog.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qfontcombobox.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qlistview.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qspinbox.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcolordialog.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qfontcombobox.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlistview.h>
#include <QtWidgets/qpushbutton.h>
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
    CDrawingView* i_pDrawingView,
    const QString& i_strObjName,
    bool i_bCreateButtonsWidget,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtGraphObjPropertiesAbstract(i_pDrawingView->drawingScene(), "Drawing::" + ClassName(), i_strObjName, i_pWdgtParent),
    m_pDrawingView(i_pDrawingView),
    // Caching values
    m_drawingSize(i_strObjName),
    m_gridSettings(i_strObjName),
    // Edit Controls
    // Geometry
    m_pGrpGeometry(nullptr),
    m_pLytGrpGeometry(nullptr),
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
    m_pLblImageMetricYScaleAxisOrientation(nullptr),
    m_pCmbImageMetricYScaleAxisOrientation(nullptr),
    // Image Size in Pixels
    m_pWdgtImageSize_px(nullptr),
    m_pLytWdgtImageSize_px(nullptr),
    m_pLytSepLineImageSize_px(nullptr),
    m_pLblSepLineImageSize_px(nullptr),
    m_pSepLineImageSize_px(nullptr),
    m_pLytLineImageSize_px(nullptr),
    m_pLblImageSizeWidth_px(nullptr),
    m_pEdtImageSizeWidth_px(nullptr),
    m_pLblImageSizeHeight_px(nullptr),
    m_pEdtImageSizeHeight_px(nullptr),
    // Grid
    m_pGrpGrid(nullptr),
    m_pLytGrpGrid(nullptr),
    m_pLytGridSettings(nullptr),
    // Grid Lines
    m_pLblGridLines(nullptr),
    m_pChkGridLinesVisible(nullptr),
    m_pLblGridLinesDistMin(nullptr),
    m_pEdtGridLinesDistMin(nullptr),
    m_pCmbGridLinesStyle(nullptr),
    m_pModelGridLinesStyles(nullptr),
    m_pEdtGridLinesWidth(nullptr),
    m_pPxmBtnGridLinesColor(nullptr),
    m_rctBtnGridLinesColor(0, 12, 16, 4),
    m_lineBtnGridLinesColor1(QPoint(0,2), QPoint(8,2)),
    m_lineBtnGridLinesColor2(QPoint(0,2), QPoint(0,10)),
    m_lineBtnGridLinesColor3(QPoint(0,10), QPoint(5,10)),
    m_pBtnGridLinesColor(nullptr),
    // Grid Scale Labels
    m_pLblGridScaleLabels(nullptr),
    m_pChkGridScaleLabelsVisible(nullptr),
    m_pCmbGridScaleLabelsFont(nullptr),
    m_pCmbGridScaleLabelsFontSize(nullptr),
    m_pPxmBtnGridScaleLabelsTextColor(nullptr),
    m_rctBtnGridScaleLabelsTextColor(0, 11, 16, 5),
    m_pBtnGridScaleLabelsTextColor(nullptr),
    m_pBtnGridScaleLabelsFontStyleBold(nullptr),
    m_pBtnGridScaleLabelsFontStyleItalic(nullptr),
    m_pBtnGridScaleLabelsTextEffectUnderline(nullptr),
    m_pBtnGridScaleLabelsTextEffectStrikeout(nullptr),
    // Edit dialog
    m_hshpRegisteredEditPropertyDialogs(),
    m_pDlgEditProperty(nullptr)
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "CreateButtonsWidget: " + bool2Str(i_bCreateButtonsWidget);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    QObject::connect(
        m_pDrawingView, &CDrawingView::gridSettingsChanged,
        this, &CWdgtDrawingViewProperties::onDrawingViewGridSettingsChanged );

    m_drawingSize = m_pDrawingView->drawingSize();
    m_gridSettings = m_pDrawingView->gridSettings();

    int cxLblWidthClm1 = 80;
    int cxEdtWidthClm1 = 145;
    int cxLblWidthClm2 = 60;
    int cxEdtWidthClm2 = 145;
    int cxClmSpacing = 30;

    setMinimumWidth(560);

    // <Group> Geometry
    //=========================

    m_pGrpGeometry = new QGroupBox("Geometry");
    m_pLyt->addWidget(m_pGrpGeometry);
    m_pLytGrpGeometry = new QVBoxLayout();
    m_pGrpGeometry->setLayout(m_pLytGrpGeometry);

    // <Section> Dimension Unit
    //-------------------------

    m_pLytLineDimensionUnit = new QHBoxLayout();
    m_pLytGrpGeometry->addLayout(m_pLytLineDimensionUnit);

    m_pLblDimensionUnit = new QLabel("Dimension Unit:");
    m_pLblDimensionUnit->setFixedWidth(cxLblWidthClm1);
    m_pLytLineDimensionUnit->addWidget(m_pLblDimensionUnit);
    m_pCmbDimensionUnit = new QComboBox();
    m_pCmbDimensionUnit->setFixedWidth(cxEdtWidthClm1);
    m_pLytLineDimensionUnit->addWidget(m_pCmbDimensionUnit);
    m_pLytLineDimensionUnit->addStretch();
    for( CEnumDrawingDimensionUnit eVal = 0; eVal < CEnumDrawingDimensionUnit::count(); ++eVal ) {
        m_pCmbDimensionUnit->addItem(eVal.toString(), eVal.enumeratorAsInt());
    }
    m_pCmbDimensionUnit->setCurrentIndex(m_drawingSize.dimensionUnit().enumeratorAsInt());
    QObject::connect(
        m_pCmbDimensionUnit, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtDrawingViewProperties::onCmbDimensionUnitCurrentIndexChanged );

    // <Section> Metric System
    //-------------------------

    m_pWdgtMetric = new QWidget();
    m_pLytWdgtMetric = new QVBoxLayout();
    m_pWdgtMetric->setLayout(m_pLytWdgtMetric);
    m_pLytWdgtMetric->setContentsMargins(0, 0, 0, 0);
    m_pLytGrpGeometry->addWidget(m_pWdgtMetric);

    // <Section> Resolution
    //-------------------------

    double fResolution_pxpin = 1.0 / m_drawingSize.resolution().getVal(Units.Length.in);
    double fResolution_pxpmm = 1.0 / m_drawingSize.resolution().getVal(Units.Length.mm);

    QString strPxpin = QString::number(fResolution_pxpin,'f',2);
    QString strPxpmm = QString::number(fResolution_pxpmm,'f',2);

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

    m_pLblResolution_pxpi = new QLabel("Pixels/Inch:");
    m_pLblResolution_pxpi->setFixedWidth(cxLblWidthClm1);
    m_pLytLineResolution->addWidget(m_pLblResolution_pxpi);
    m_pEdtResolution_pxpi = new QLineEdit(strPxpin);
    m_pEdtResolution_pxpi->setFixedWidth(cxEdtWidthClm1);
    m_pEdtResolution_pxpi->setReadOnly(true);
    m_pLytLineResolution->addWidget(m_pEdtResolution_pxpi);
    m_pLytLineResolution->addSpacing(cxClmSpacing);

    m_pLblResolution_pxpmm = new QLabel("Pixels/mm:");
    m_pLblResolution_pxpmm->setFixedWidth(cxLblWidthClm2);
    m_pLytLineResolution->addWidget(m_pLblResolution_pxpmm);
    m_pEdtResolution_pxpmm = new QLineEdit(strPxpmm);
    m_pEdtResolution_pxpmm->setObjectName(CDrawingScene::c_strXmlAttrScreenResolutionPxPerMilliMeter);
    m_pEdtResolution_pxpmm->setFixedWidth(cxEdtWidthClm2);
    m_pEdtResolution_pxpmm->setReadOnly(true);
    registerEditPropertyDialog(m_pEdtResolution_pxpmm);
    m_pLytLineResolution->addWidget(m_pEdtResolution_pxpmm);
    m_pLytLineResolution->addStretch();

    // <Section> Image Size in Metric System
    //--------------------------------------

    #pragma message(__TODO__"Add Edit Controls to set Resolution, Minimum and Maximum")

    m_pLytSepLineImageMetric = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytSepLineImageMetric);
    m_pLblSepLineImageMetric = new QLabel("Metric Sizes");
    m_pLytSepLineImageMetric->addWidget(m_pLblSepLineImageMetric);
    m_pSepLineImageMetric = new CSepLine(10);
    m_pLytSepLineImageMetric->addWidget(m_pSepLineImageMetric, 1);

    // <Line> Unit
    //------------

    m_pLytLineImageMetricUnit = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytLineImageMetricUnit);

    m_pLblImageMetricUnit = new QLabel("Unit:");
    m_pLblImageMetricUnit->setFixedWidth(cxLblWidthClm1);
    m_pLytLineImageMetricUnit->addWidget(m_pLblImageMetricUnit);
    m_pCmbImageMetricUnit = new QComboBox();
    m_pCmbImageMetricUnit->setFixedWidth(cxEdtWidthClm2);
    //m_pCmbImageMetricUnit->setEnabled(false);
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
    m_pCmbImageMetricUnit->setCurrentText(m_drawingSize.metricUnit().symbol());
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
    m_pLblImageMetricWidth->setFixedWidth(cxLblWidthClm1);
    m_pLytLineMetricSize->addWidget(m_pLblImageMetricWidth);
    m_pEdtImageMetricWidth = new CWdgtEditPhysVal();
    m_pEdtImageMetricWidth->setFixedWidth(cxEdtWidthClm1);
    m_pLytLineMetricSize->addWidget(m_pEdtImageMetricWidth);
    m_pLytLineMetricSize->addSpacing(cxClmSpacing);
    QObject::connect(
        m_pEdtImageMetricWidth, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtDrawingViewProperties::onEdtImageMetricWidthValueChanged);

    // <Edit> Image Height
    //--------------------

    m_pLblImageMetricHeight = new QLabel("Height:");
    m_pLblImageMetricHeight->setFixedWidth(cxLblWidthClm2);
    m_pLytLineMetricSize->addWidget(m_pLblImageMetricHeight);
    m_pEdtImageMetricHeight = new CWdgtEditPhysVal();
    m_pEdtImageMetricHeight->setFixedWidth(cxEdtWidthClm2);
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
    m_pLblImageMetricNormedPaperSizes->setFixedWidth(cxLblWidthClm1);
    m_pLytLineImageMetricNormedPaper->addWidget(m_pLblImageMetricNormedPaperSizes);
    m_pCmbImageMetricNormedPaperSizes = new QComboBox();
    m_pCmbImageMetricNormedPaperSizes->setFixedWidth(cxEdtWidthClm1);
    //m_pCmbImageMetricNormedPaperSizes->setEnabled(false);
    m_pLytLineImageMetricNormedPaper->addWidget(m_pCmbImageMetricNormedPaperSizes);
    m_pLytLineImageMetricNormedPaper->addSpacing(cxClmSpacing);
    for( CEnumNormedPaperSize ePaperSize = 0; ePaperSize < CEnumNormedPaperSize::count(); ++ePaperSize ) {
        m_pCmbImageMetricNormedPaperSizes->addItem(
            ePaperSize.toString(EEnumEntryAliasStrSymbol), ePaperSize.toValue());
    }
    m_pCmbImageMetricNormedPaperSizes->addItem("---");
    if (m_drawingSize.normedPaperSize().isValid()) {
        m_pCmbImageMetricNormedPaperSizes->setCurrentText(
            m_drawingSize.normedPaperSize().toString(EEnumEntryAliasStrSymbol));
    }
    else {
        m_pCmbImageMetricNormedPaperSizes->setCurrentText("---");
    }
    QObject::connect(
        m_pCmbImageMetricNormedPaperSizes, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtDrawingViewProperties::onCmbImageMetricNormedPaperSizesCurrentIndexChanged );

    // <ComboBox> Paper Orientation
    //-----------------------------

    m_pLblImageMetricNormedPaperOrientation = new QLabel("Orientation:");
    m_pLblImageMetricNormedPaperOrientation->setFixedWidth(cxLblWidthClm2);
    m_pLytLineImageMetricNormedPaper->addWidget(m_pLblImageMetricNormedPaperOrientation);
    m_pLblImageMetricNormedPaperOrientation->setVisible(false);
    m_pCmbImageMetricNormedPaperOrientation = new QComboBox();
    m_pCmbImageMetricNormedPaperOrientation->setFixedWidth(cxEdtWidthClm2);
    //m_pCmbImageMetricNormedPaperOrientation->setEnabled(false);
    m_pLytLineImageMetricNormedPaper->addWidget(m_pCmbImageMetricNormedPaperOrientation);
    m_pLytLineImageMetricNormedPaper->addStretch();
    for (CEnumOrientation eOrientation = 0; eOrientation < CEnumOrientation::count(); ++eOrientation ) {
        m_pCmbImageMetricNormedPaperOrientation->addItem(eOrientation.toString(EEnumEntryAliasStrText));
    }
    if (m_drawingSize.normedPaperOrientation().isValid()) {
        m_pCmbImageMetricNormedPaperOrientation->setCurrentText(
            m_drawingSize.normedPaperOrientation().toString(EEnumEntryAliasStrText));
    }
    m_pCmbImageMetricNormedPaperOrientation->setVisible(false);
    QObject::connect(
        m_pCmbImageMetricNormedPaperOrientation, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtDrawingViewProperties::onCmbImageMetricNormedPaperOrientationCurrentIndexChanged );

    // <Line> Scale Factor
    //--------------------

    m_pLytLineImageMetricScaleFactor = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytLineImageMetricScaleFactor);

    // <ComboBox> Scale Factor
    //------------------------

    m_pLblImageMetricScaleFactor = new QLabel("Scale Factor:");
    m_pLblImageMetricScaleFactor->setFixedWidth(cxLblWidthClm1);
    m_pLytLineImageMetricScaleFactor->addWidget(m_pLblImageMetricScaleFactor);

    QWidget* pWdgtImageMetricScaleFactor = new QWidget();
    pWdgtImageMetricScaleFactor->setFixedWidth(cxEdtWidthClm1);
    QHBoxLayout* pLytWdgtImageMetricScaleFactor = new QHBoxLayout();
    pLytWdgtImageMetricScaleFactor->setContentsMargins(0,0,0,0);
    pWdgtImageMetricScaleFactor->setLayout(pLytWdgtImageMetricScaleFactor);
    m_pLytLineImageMetricScaleFactor->addWidget(pWdgtImageMetricScaleFactor);

    m_pCmbImageMetricScaleFactorDividend = new QComboBox();
    //m_pCmbImageMetricScaleFactorDividend->setEnabled(false);
    m_pCmbImageMetricScaleFactorDividend->setEditable(true);
    m_pCmbImageMetricScaleFactorDividend->setInsertPolicy(QComboBox::NoInsert);
    m_pCmbImageMetricScaleFactorDividend->setValidator(
        new QIntValidator(m_pCmbImageMetricScaleFactorDividend));
    pLytWdgtImageMetricScaleFactor->addWidget(m_pCmbImageMetricScaleFactorDividend, 1);
    m_pLblImageMetricScaleFactorHyphen = new QLabel(":");
    pLytWdgtImageMetricScaleFactor->addWidget(m_pLblImageMetricScaleFactorHyphen);
    m_pCmbImageMetricScaleFactorDivisor = new QComboBox();
    //m_pCmbImageMetricScaleFactorDivisor->setEnabled(false);
    m_pCmbImageMetricScaleFactorDivisor->setEditable(true);
    m_pCmbImageMetricScaleFactorDivisor->setInsertPolicy(QComboBox::NoInsert);
    m_pCmbImageMetricScaleFactorDivisor->setValidator(
        new QIntValidator(m_pCmbImageMetricScaleFactorDivisor));
    pLytWdgtImageMetricScaleFactor->addWidget(m_pCmbImageMetricScaleFactorDivisor, 1);
    m_pLytLineImageMetricScaleFactor->addSpacing(cxClmSpacing);

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
        QString::number(m_drawingSize.scaleFactorDividend()));
    m_pCmbImageMetricScaleFactorDivisor->setCurrentText(
        QString::number(m_drawingSize.scaleFactorDivisor()));
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

    // <ComboBox> Y Scale Axis Orientation
    //------------------------------------

    m_pLblImageMetricYScaleAxisOrientation = new QLabel("Y Scale Axis:");
    m_pLblImageMetricYScaleAxisOrientation->setFixedWidth(cxLblWidthClm2);
    m_pLytLineImageMetricScaleFactor->addWidget(m_pLblImageMetricYScaleAxisOrientation);

    m_pCmbImageMetricYScaleAxisOrientation = new QComboBox();
    m_pCmbImageMetricYScaleAxisOrientation->setFixedWidth(cxEdtWidthClm2);
    m_pLytLineImageMetricScaleFactor->addWidget(m_pCmbImageMetricYScaleAxisOrientation);
    m_pLytLineImageMetricScaleFactor->addStretch();
    for (CEnumYScaleAxisOrientation eOrientation = 0; eOrientation < CEnumYScaleAxisOrientation::count(); ++eOrientation ) {
        m_pCmbImageMetricYScaleAxisOrientation->addItem(eOrientation.toString(EEnumEntryAliasStrText));
    }
    m_pCmbImageMetricYScaleAxisOrientation->setCurrentText(
        m_drawingSize.yScaleAxisOrientation().toString(EEnumEntryAliasStrText));
    QObject::connect(
        m_pCmbImageMetricYScaleAxisOrientation, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtDrawingViewProperties::onCmbImageMetricYScaleAxisOrientationCurrentIndexChanged );

    // <Visibility> Metric Widget
    //---------------------------

    m_pWdgtMetric->setVisible(false);

    // <Section> Image Size
    //---------------------

    m_pWdgtImageSize_px = new QWidget();
    m_pLytWdgtImageSize_px = new QVBoxLayout();
    m_pWdgtImageSize_px->setLayout(m_pLytWdgtImageSize_px);
    m_pLytWdgtImageSize_px->setContentsMargins(0, 0, 0, 0);
    m_pLytGrpGeometry->addWidget(m_pWdgtImageSize_px);

    m_pLytSepLineImageSize_px = new QHBoxLayout();
    m_pLytWdgtImageSize_px->addLayout(m_pLytSepLineImageSize_px);
    m_pLblSepLineImageSize_px = new QLabel("Pixel Sizes");
    m_pLytSepLineImageSize_px->addWidget(m_pLblSepLineImageSize_px);
    m_pSepLineImageSize_px = new CSepLine(10);
    m_pLytSepLineImageSize_px->addWidget(m_pSepLineImageSize_px,1);

    // <Line> Image Size
    //------------------

    m_pLytLineImageSize_px = new QHBoxLayout();
    m_pLytWdgtImageSize_px->addLayout(m_pLytLineImageSize_px);

    // <Edit> Image Width
    //--------------------

    m_pLblImageSizeWidth_px = new QLabel("Width:");
    m_pLblImageSizeWidth_px->setFixedWidth(cxLblWidthClm1);
    m_pLytLineImageSize_px->addWidget(m_pLblImageSizeWidth_px);
    m_pEdtImageSizeWidth_px = new QSpinBox();
    m_pEdtImageSizeWidth_px->setFixedWidth(cxEdtWidthClm1);
    m_pEdtImageSizeWidth_px->setMinimum(1);
    m_pEdtImageSizeWidth_px->setMaximum(100000);
    m_pEdtImageSizeWidth_px->setValue(m_drawingSize.imageSizeInPixels().width());
    m_pEdtImageSizeWidth_px->setSuffix(" px");
    m_pLytLineImageSize_px->addWidget(m_pEdtImageSizeWidth_px);
    m_pLytLineImageSize_px->addSpacing(cxClmSpacing);
    QObject::connect(
        m_pEdtImageSizeWidth_px, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtDrawingViewProperties::onEdtImageSizeWidthPxValueChanged);

    // <Edit> Image Height
    //--------------------

    m_pLblImageSizeHeight_px = new QLabel("Height:");
    m_pLblImageSizeHeight_px->setFixedWidth(cxLblWidthClm2);
    m_pLytLineImageSize_px->addWidget(m_pLblImageSizeHeight_px);
    m_pEdtImageSizeHeight_px = new QSpinBox();
    m_pEdtImageSizeHeight_px->setFixedWidth(cxEdtWidthClm2);
    m_pEdtImageSizeHeight_px->setMinimum(1);
    m_pEdtImageSizeHeight_px->setMaximum(100000);
    m_pEdtImageSizeHeight_px->setValue(m_drawingSize.imageSizeInPixels().height());
    m_pEdtImageSizeHeight_px->setSuffix(" px");
    m_pLytLineImageSize_px->addWidget(m_pEdtImageSizeHeight_px);
    m_pLytLineImageSize_px->addStretch();
    QObject::connect(
        m_pEdtImageSizeHeight_px, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtDrawingViewProperties::onEdtImageSizeHeightPxValueChanged);

    // <Group> Grid
    //=============

    m_pGrpGrid = new QGroupBox("Grid");
    m_pLyt->addWidget(m_pGrpGrid);

    // Need a stretch at the right side. So use HBoxLayout for the group box
    m_pLytGrpGrid = new QHBoxLayout();
    m_pGrpGrid->setLayout(m_pLytGrpGrid);

    m_pLytGridSettings = new QGridLayout();
    m_pLytGrpGrid->addLayout(m_pLytGridSettings);
    m_pLytGrpGrid->addStretch();

    // <Line> Grid Lines
    //------------------

    // Row 0:
    const int iClmLineHeadLines = 0;
    const int iClmChkVisible = 1;
    const int iClmLblLinesDistMin = 2;
    const int iClmEdtLinesDistMin = 3;
    const int iClmCmbLinesStyle = 4;
    const int iClmEdtLinesWidth = 5;
    const int iClmBtnLinesColor = 6;

    // Row 1:
    const int iClmCmbFont = 2;
    const int iClmSpanCmbFont = 3;
    const int iClmCmbFontSize = 5;
    const int iClmBtnTextColor = 6;

    // Row 2:
    const int iClmBtnFontStyleBold = 2;
    const int iClmBtnFontStyleItalic = 3;
    const int iClmBtnFontEffectUnderline = 4;
    const int iClmBtnFontEffectStrikeout = 5;

    m_pLytGridSettings->setColumnMinimumWidth(iClmChkVisible, 32);

    int iRow = 0;
    int iClm = iClmLineHeadLines;

    m_pLblGridLines = new QLabel("Lines:");
    m_pLblGridLines->setFixedWidth(cxLblWidthClm1);
    m_pLytGridSettings->addWidget(m_pLblGridLines, iRow, iClm);

    iClm = iClmChkVisible;
    m_pChkGridLinesVisible = new QCheckBox();
    m_pChkGridLinesVisible->setChecked(m_gridSettings.areLinesVisible());
    m_pLytGridSettings->addWidget(m_pChkGridLinesVisible, iRow, iClm);
    QObject::connect(
        m_pChkGridLinesVisible, &QCheckBox::stateChanged,
        this, &CWdgtDrawingViewProperties::onChkGridLinesVisibleStateChanged);

    iClm = iClmLblLinesDistMin;
    m_pLblGridLinesDistMin = new QLabel("Min Distance:");
    m_pLytGridSettings->addWidget(m_pLblGridLinesDistMin, iRow, iClm);

    iClm = iClmEdtLinesDistMin;
    m_pEdtGridLinesDistMin = new QSpinBox();
    m_pEdtGridLinesDistMin->setMinimum(1);
    m_pEdtGridLinesDistMin->setMaximum(9999);
    m_pEdtGridLinesDistMin->setSuffix(" px");
    m_pEdtGridLinesDistMin->setValue(m_gridSettings.linesDistMin());
    m_pLytGridSettings->addWidget(m_pEdtGridLinesDistMin, iRow, iClm);
    QObject::connect(
        m_pEdtGridLinesDistMin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtDrawingViewProperties::onEdtGridLinesDistMinValueChanged);

    iClm = iClmCmbLinesStyle;
    m_pModelGridLinesStyles = new QStandardItemModel();
    QSize iconSize = fillGridLineStylesModel();
    m_pCmbGridLinesStyle = new QComboBox();
    m_pCmbGridLinesStyle->setModel(m_pModelGridLinesStyles);
    m_pCmbGridLinesStyle->setIconSize(iconSize);
    m_pLytGridSettings->addWidget(m_pCmbGridLinesStyle, iRow, iClm);
    int idx = m_pCmbGridLinesStyle->findData(m_gridSettings.linesStyle().enumeratorAsInt());
    if (idx >= 0) {
        m_pCmbGridLinesStyle->setCurrentIndex(idx);
    }
    QObject::connect(
        m_pCmbGridLinesStyle, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtDrawingViewProperties::onCmbGridLinesStyleCurrentIndexChanged);

    // The list view  "m_pListViewGridLineStyles" must be created for the first
    // time the button is pressed. If created before the widgets created afterwards
    // will be above the list view in the widget hierarchy and the list view will
    // be overlapped by those and is partly invisible.

    iClm = iClmEdtLinesWidth;
    m_pEdtGridLinesWidth = new QSpinBox();
    m_pEdtGridLinesWidth->setMinimum(1);
    m_pEdtGridLinesWidth->setMaximum(9999);
    m_pEdtGridLinesWidth->setSuffix(" px");
    m_pEdtGridLinesWidth->setValue(m_gridSettings.linesWidth());
    m_pLytGridSettings->addWidget(m_pEdtGridLinesWidth, iRow, iClm);
    QObject::connect(
        m_pEdtGridLinesWidth, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtDrawingViewProperties::onEdtGridLinesWidthValueChanged);

    iClm = iClmBtnLinesColor;
    m_pPxmBtnGridLinesColor = new QPixmap(":/ZS/Draw/DrawToolPen16x16.bmp");
    m_pPxmBtnGridLinesColor->setMask(m_pPxmBtnGridLinesColor->createHeuristicMask());
    m_pBtnGridLinesColor = new QPushButton();
    updateGridLinesColorButtonIcon();
    m_pLytGridSettings->addWidget(m_pBtnGridLinesColor, iRow, iClm);
    QObject::connect(
        m_pBtnGridLinesColor, &QPushButton::clicked,
        this, &CWdgtDrawingViewProperties::onBtnGridLinesColorClicked);

    // <Line> Grid Scale Labels Visible and Font Size
    //-----------------------------------------------

    ++iRow;
    iClm = iClmLineHeadLines;
    m_pLblGridScaleLabels = new QLabel("Labels:");
    m_pLblGridScaleLabels->setFixedWidth(cxLblWidthClm1);
    m_pLytGridSettings->addWidget(m_pLblGridScaleLabels, iRow, iClm);

    iClm = iClmChkVisible;
    m_pChkGridScaleLabelsVisible = new QCheckBox();
    m_pChkGridScaleLabelsVisible->setChecked(m_gridSettings.areLabelsVisible());
    m_pLytGridSettings->addWidget(m_pChkGridScaleLabelsVisible, iRow, iClm);
    QObject::connect(
        m_pChkGridScaleLabelsVisible, &QCheckBox::stateChanged,
        this, &CWdgtDrawingViewProperties::onChkGridScaleLabelsVisibleStateChanged);

    iClm = iClmCmbFont;
    m_pCmbGridScaleLabelsFont = new QFontComboBox();
    m_pCmbGridScaleLabelsFont->setCurrentFont(m_gridSettings.labelsFont());
    m_pLytGridSettings->addWidget(m_pCmbGridScaleLabelsFont, iRow, iClm, 1, iClmSpanCmbFont);
    QObject::connect(
        m_pCmbGridScaleLabelsFont, &QFontComboBox::currentFontChanged,
        this, &CWdgtDrawingViewProperties::onCmbGridScaleLabelsCurrentFontChanged);

    iClm = iClmCmbFontSize;
    m_pCmbGridScaleLabelsFontSize = new QComboBox();
    m_pLytGridSettings->addWidget(m_pCmbGridScaleLabelsFontSize, iRow, iClm);
    for( int idx = 0; idx < ETextSizeCount; idx++ ) {
        m_pCmbGridScaleLabelsFontSize->addItem( textSize2Str(static_cast<ETextSize>(idx)), textSize2SizeInPixels(static_cast<ETextSize>(idx)) );
    }
    m_pCmbGridScaleLabelsFontSize->setCurrentIndex(m_gridSettings.labelsTextSize());
    QObject::connect(
        m_pCmbGridScaleLabelsFontSize, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtDrawingViewProperties::onCmbGridScaleLabelsFontSizeCurrentIndexChanged);

    iClm = iClmBtnTextColor;
    m_pPxmBtnGridScaleLabelsTextColor = new QPixmap(":/ZS/Draw/DrawToolText16x16.bmp");
    m_pPxmBtnGridScaleLabelsTextColor->setMask(m_pPxmBtnGridScaleLabelsTextColor->createHeuristicMask());
    m_pBtnGridScaleLabelsTextColor = new QPushButton();
    updateGridLabelsTextColorButtonIcon();
    m_pLytGridSettings->addWidget(m_pBtnGridScaleLabelsTextColor, iRow, iClm);
    QObject::connect(
        m_pBtnGridScaleLabelsTextColor, &QCheckBox::clicked,
        this, &CWdgtDrawingViewProperties::onBtnGridScaleLabelsTextColorClicked);

    // <Line> Grid Scale Labels Font Style and Font Effect
    //----------------------------------------------------

    ++iRow;
    iClm = iClmBtnFontStyleBold;
    m_pBtnGridScaleLabelsFontStyleBold = new QPushButton("F");
    QFont fntStyleBold = m_gridSettings.labelsFont();
    fntStyleBold.setBold(true);
    m_pBtnGridScaleLabelsFontStyleBold->setFont(fntStyleBold);
    m_pBtnGridScaleLabelsFontStyleBold->setCheckable(true);
    updateGridLabelsTextStyleBoldButton();
    m_pLytGridSettings->addWidget(m_pBtnGridScaleLabelsFontStyleBold, iRow, iClm);
    QObject::connect(
        m_pBtnGridScaleLabelsFontStyleBold, &QPushButton::toggled,
        this, &CWdgtDrawingViewProperties::onBtnGridScaleLabelsFontStyleBoldToggled);

    iClm = iClmBtnFontStyleItalic;
    m_pBtnGridScaleLabelsFontStyleItalic = new QPushButton("I");
    QFont fntStyleItalic = m_gridSettings.labelsFont();
    fntStyleItalic.setItalic(true);
    m_pBtnGridScaleLabelsFontStyleItalic->setFont(fntStyleItalic);
    m_pBtnGridScaleLabelsFontStyleItalic->setCheckable(true);
    updateGridLabelsTextStyleItalicButton();
    m_pLytGridSettings->addWidget(m_pBtnGridScaleLabelsFontStyleItalic, iRow, iClm);
    QObject::connect(
        m_pBtnGridScaleLabelsFontStyleItalic, &QPushButton::toggled,
        this, &CWdgtDrawingViewProperties::onBtnGridScaleLabelsFontStyleItalicToggled);

    iClm = iClmBtnFontEffectUnderline;
    m_pBtnGridScaleLabelsTextEffectUnderline = new QPushButton("U");
    QFont fntEffectUnderline = m_gridSettings.labelsFont();
    fntEffectUnderline.setUnderline(true);
    m_pBtnGridScaleLabelsTextEffectUnderline->setFont(fntEffectUnderline);
    m_pBtnGridScaleLabelsTextEffectUnderline->setCheckable(true);
    updateGridLabelsTextEffectUnderlineButton();
    m_pLytGridSettings->addWidget(m_pBtnGridScaleLabelsTextEffectUnderline, iRow, iClm);
    QObject::connect(
        m_pBtnGridScaleLabelsTextEffectUnderline, &QPushButton::toggled,
        this, &CWdgtDrawingViewProperties::onBtnGridScaleLabelsTextEffectUnderlineToggled);

    iClm = iClmBtnFontEffectStrikeout;
    m_pBtnGridScaleLabelsTextEffectStrikeout = new QPushButton("ab");
    QFont fntEffectStrikeout = m_gridSettings.labelsFont();
    fntEffectStrikeout.setStrikeOut(true);
    m_pBtnGridScaleLabelsTextEffectStrikeout->setFont(fntEffectStrikeout);
    m_pBtnGridScaleLabelsTextEffectStrikeout->setCheckable(true);
    updateGridLabelsTextEffectStrikeoutButton();
    m_pLytGridSettings->addWidget(m_pBtnGridScaleLabelsTextEffectStrikeout, iRow, iClm);
    QObject::connect(
        m_pBtnGridScaleLabelsTextEffectStrikeout, &QPushButton::toggled,
        this, &CWdgtDrawingViewProperties::onBtnGridScaleLabelsTextEffectStrikeoutToggled);

    // <Buttons>
    //==========

    if (i_bCreateButtonsWidget) {
        createButtonsLineWidget();
        m_pLyt->addWidget(m_pWdgtButtons);
    }
    m_pLyt->addStretch();

    // Update metrics
    //---------------

    updateDimensionUnit();

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

    try {
        delete m_pModelGridLinesStyles;
    }
    catch(...) {
    }
    try {
        delete m_pPxmBtnGridLinesColor;
    }
    catch(...) {
    }
    try {
        delete m_pPxmBtnGridScaleLabelsTextColor;
    }
    catch(...) {
    }

    if (m_pDlgEditProperty != nullptr) {
        m_pDlgEditProperty->close();
        try {
            delete m_pDlgEditProperty;
        }
        catch (...) {
        }
    }

    m_pDrawingView = nullptr;
    // Caching values
    //m_drawingSize;
    //m_gridSettings;
    // Edit Controls
    m_pGrpGeometry = nullptr;
    m_pLytGrpGeometry = nullptr;
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
    m_pWdgtImageSize_px = nullptr;
    m_pLytWdgtImageSize_px = nullptr;
    m_pLytSepLineImageSize_px = nullptr;
    m_pLblSepLineImageSize_px = nullptr;
    m_pSepLineImageSize_px = nullptr;
    m_pLytLineImageSize_px = nullptr;
    m_pLblImageSizeWidth_px = nullptr;
    m_pEdtImageSizeWidth_px = nullptr;
    m_pLblImageSizeHeight_px = nullptr;
    m_pEdtImageSizeHeight_px = nullptr;
    m_pLblImageMetricYScaleAxisOrientation = nullptr;
    m_pCmbImageMetricYScaleAxisOrientation = nullptr;
    // Grid
    m_pGrpGrid = nullptr;
    m_pLytGrpGrid = nullptr;
    m_pLytGridSettings = nullptr;
    m_pLblGridLines = nullptr;
    m_pChkGridLinesVisible = nullptr;
    m_pCmbGridLinesStyle = nullptr;
    m_pModelGridLinesStyles = nullptr;
    m_pEdtGridLinesWidth = nullptr;
    m_pPxmBtnGridLinesColor = nullptr;
    //m_rctBtnGridLinesColor;
    //m_lineBtnGridLinesColor1;
    //m_lineBtnGridLinesColor2;
    //m_lineBtnGridLinesColor3;
    m_pBtnGridLinesColor = nullptr;
    // Grid Scale Labels
    m_pLblGridScaleLabels = nullptr;
    m_pChkGridScaleLabelsVisible = nullptr;
    m_pLblGridLinesDistMin = nullptr;
    m_pEdtGridLinesDistMin = nullptr;
    m_pCmbGridScaleLabelsFont = nullptr;
    m_pCmbGridScaleLabelsFontSize = nullptr;
    m_pPxmBtnGridScaleLabelsTextColor = nullptr;
    //m_rctBtnGridScaleLabelsTextColor;
    m_pBtnGridScaleLabelsTextColor = nullptr;
    m_pBtnGridScaleLabelsFontStyleBold = nullptr;
    m_pBtnGridScaleLabelsFontStyleItalic = nullptr;
    m_pBtnGridScaleLabelsTextEffectUnderline = nullptr;
    m_pBtnGridScaleLabelsTextEffectStrikeout = nullptr;
    m_hshpRegisteredEditPropertyDialogs.clear();
    m_pDlgEditProperty = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtDrawingViewProperties::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ "" );

    bool bHasChanges = (m_drawingSize != m_pDrawingView->drawingSize());
    if (!bHasChanges) {
        bHasChanges = (m_gridSettings != m_pDrawingView->gridSettings());
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );

    m_pDrawingView->setDrawingSize(m_drawingSize);
    m_pDrawingView->setGridSettings(m_gridSettings);

    updateButtonsEnabled();
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::rejectChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rejectChanges",
        /* strAddInfo   */ "" );

    onDrawingSceneDrawingSizeChanged(m_pDrawingView->drawingSize());
    onDrawingViewGridSettingsChanged(m_pDrawingView->gridSettings());

    updateButtonsEnabled();
}

/*==============================================================================
protected slots: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onDrawingSceneDrawingSizeChanged(const CDrawingSize& i_drawingSize)
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_drawingSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneDrawingSizeChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_drawingSize != i_drawingSize ) {
        m_drawingSize = i_drawingSize;
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        updateDimensionUnit();
        updateResolution();
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onDrawingViewGridSettingsChanged(const CDrawGridSettings& i_gridSettings)
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_gridSettings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingViewGridSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings != i_gridSettings ) {
        m_gridSettings = i_gridSettings;
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        updateGridSettings();
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onCmbDimensionUnitCurrentIndexChanged( int i_idx )
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
void CWdgtDrawingViewProperties::onCmbImageMetricYScaleAxisOrientationCurrentIndexChanged(int i_idx)
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbImageMetricYScaleAxisOrientationCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

    setYScaleAxisOrientation(m_pCmbImageMetricYScaleAxisOrientation->currentText());
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onEdtImageSizeWidthPxValueChanged(int i_cxWidth_px)
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_cxWidth_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtImageSizeWidthPxValueChanged",
        /* strAddInfo   */ strMthInArgs );

    CPhysVal physValWidth(i_cxWidth_px, Units.Length.px);
    CPhysVal physValHeight(m_pEdtImageSizeHeight_px->value(), Units.Length.px);
    if( physValWidth.getVal() > 0.0 && physValHeight.getVal() > 0.0 ) {
        setImageSize(physValWidth, physValHeight);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onEdtImageSizeHeightPxValueChanged(int i_cyHeight_px)
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_cyHeight_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtImageSizeHeightPxValueChanged",
        /* strAddInfo   */ strMthInArgs );

    CPhysVal physValWidth(m_pEdtImageSizeWidth_px->value(), Units.Length.px);
    CPhysVal physValHeight(i_cyHeight_px, Units.Length.px);
    if( physValWidth.getVal() > 0.0 && physValHeight.getVal() > 0.0 ) {
        setImageSize(physValWidth, physValHeight);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onChkGridLinesVisibleStateChanged(int i_iState)
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qCheckState2Str(i_iState);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkGridLinesVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

    setGridLinesVisible(i_iState == Qt::Checked);
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onEdtGridLinesDistMinValueChanged(int i_iVal)
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iVal);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtGridLinesDistMinValueChanged",
        /* strAddInfo   */ strMthInArgs );

    setGridLinesDistMin(i_iVal);
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onCmbGridLinesStyleCurrentIndexChanged(int i_idx)
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbGridLinesStyleCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

    if( i_idx >= 0 && i_idx < CEnumLineStyle::count() )
    {
        QVariant varData = m_pCmbGridLinesStyle->itemData(i_idx, Qt::UserRole);
        ELineStyle lineStyle = static_cast<ELineStyle>(varData.toInt());
        setGridLinesStyle(lineStyle);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onEdtGridLinesWidthValueChanged(int i_iVal)
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iVal);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnGridLinesColorClicked",
        /* strAddInfo   */ strMthInArgs );

    setGridLinesWidth(i_iVal);
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onBtnGridLinesColorClicked(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnGridLinesColorClicked",
        /* strAddInfo   */ strMthInArgs );

    QColor clr = QColorDialog::getColor(
        /* clrInitial  */ m_gridSettings.linesColor(),
        /* pWdgtParent */ m_pBtnGridLinesColor,
        /* strTitle    */ "Colors",
        /* options     */ QColorDialog::ShowAlphaChannel );

    if( clr.isValid() )
    {
        setGridLinesColor(clr);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onChkGridScaleLabelsVisibleStateChanged(int i_iState)
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qCheckState2Str(i_iState);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkGridScaleLabelsVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

    setGridLabelsVisible(i_iState == Qt::Checked);
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onCmbGridScaleLabelsCurrentFontChanged(const QFont& i_fnt)
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_fnt.family();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbGridScaleLabelsCurrentFontChanged",
        /* strAddInfo   */ strMthInArgs );

    setGridLabelsFont(i_fnt);
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onCmbGridScaleLabelsFontSizeCurrentIndexChanged(int i_iCurrentIndex)
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iCurrentIndex);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbGridScaleLabelsFontSizeCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

    setGridLabelsTextSize(static_cast<ETextSize>(i_iCurrentIndex));
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onBtnGridScaleLabelsTextColorClicked(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnGridScaleLabelsTextColorClicked",
        /* strAddInfo   */ strMthInArgs );

    QColor clr = QColorDialog::getColor(
        /* clrInitial  */ m_gridSettings.labelsTextColor(),
        /* pWdgtParent */ m_pBtnGridScaleLabelsTextColor,
        /* strTitle    */ "Colors",
        /* options     */ QColorDialog::ShowAlphaChannel );

    if( clr.isValid() )
    {
        setGridLabelsTextColor(clr);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onBtnGridScaleLabelsFontStyleBoldToggled(int i_iState)
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qCheckState2Str(i_iState);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnGridScaleLabelsFontStyleBoldToggled",
        /* strAddInfo   */ strMthInArgs );

    CEnumTextStyle textStyle = ETextStyle::Normal;

    if( m_pBtnGridScaleLabelsFontStyleItalic->isChecked() && m_pBtnGridScaleLabelsFontStyleBold->isChecked() )
    {
        textStyle = ETextStyle::BoldItalic;
    }
    else if( m_pBtnGridScaleLabelsFontStyleItalic->isChecked() )
    {
        textStyle = ETextStyle::Italic;
    }
    else if( m_pBtnGridScaleLabelsFontStyleBold->isChecked() )
    {
        textStyle = ETextStyle::Bold;
    }
    setGridLabelsTextStyle(textStyle.enumerator());
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onBtnGridScaleLabelsFontStyleItalicToggled(int i_iState)
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qCheckState2Str(i_iState);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnGridScaleLabelsFontStyleItalicToggled",
        /* strAddInfo   */ strMthInArgs );

    CEnumTextStyle textStyle = ETextStyle::Normal;

    if( m_pBtnGridScaleLabelsFontStyleItalic->isChecked() && m_pBtnGridScaleLabelsFontStyleBold->isChecked() )
    {
        textStyle = ETextStyle::BoldItalic;
    }
    else if( m_pBtnGridScaleLabelsFontStyleItalic->isChecked() )
    {
        textStyle = ETextStyle::Italic;
    }
    else if( m_pBtnGridScaleLabelsFontStyleBold->isChecked() )
    {
        textStyle = ETextStyle::Bold;
    }
    setGridLabelsTextStyle(textStyle.enumerator());
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onBtnGridScaleLabelsTextEffectUnderlineToggled(int i_iState)
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qCheckState2Str(i_iState);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnGridScaleLabelsTextEffectUnderlineToggled",
        /* strAddInfo   */ strMthInArgs );

    CEnumTextEffect textEffect = ETextEffect::None;

    if( m_pBtnGridScaleLabelsTextEffectUnderline->isChecked() && m_pBtnGridScaleLabelsTextEffectStrikeout->isChecked() )
    {
        textEffect = ETextEffect::StrikeoutUnderline;
    }
    else if( m_pBtnGridScaleLabelsTextEffectUnderline->isChecked() )
    {
        textEffect = ETextEffect::Underline;
    }
    else if( m_pBtnGridScaleLabelsTextEffectStrikeout->isChecked() )
    {
        textEffect = ETextEffect::Strikeout;
    }
    setGridLabelsTextEffect(textEffect.enumerator());
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onBtnGridScaleLabelsTextEffectStrikeoutToggled(int i_iState)
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qCheckState2Str(i_iState);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnGridScaleLabelsTextEffectStrikeoutToggled",
        /* strAddInfo   */ strMthInArgs );

    CEnumTextEffect textEffect = ETextEffect::None;

    if( m_pBtnGridScaleLabelsTextEffectUnderline->isChecked() && m_pBtnGridScaleLabelsTextEffectStrikeout->isChecked() )
    {
        textEffect = ETextEffect::StrikeoutUnderline;
    }
    else if( m_pBtnGridScaleLabelsTextEffectUnderline->isChecked() )
    {
        textEffect = ETextEffect::Underline;
    }
    else if( m_pBtnGridScaleLabelsTextEffectStrikeout->isChecked() )
    {
        textEffect = ETextEffect::Strikeout;
    }
    setGridLabelsTextEffect(textEffect.enumerator());
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onDlgEditPropertyDestroyed(QObject* i_pObj)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDlgEditPropertyDestroyed",
        /* strAddInfo   */ "" );

    m_pDlgEditProperty = nullptr;
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setDimensionUnit( const CEnumDrawingDimensionUnit& i_eDimensionUnit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_eDimensionUnit.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDimensionUnit",
        /* strAddInfo   */ strMthInArgs );

    if( m_drawingSize.dimensionUnit() != i_eDimensionUnit ) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_drawingSize.setDimensionUnit(i_eDimensionUnit);
        updateDimensionUnit();
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setMetricUnit( const CUnit& i_metricUnit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_metricUnit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMetricUnit",
        /* strAddInfo   */ strMthInArgs );

    if( m_drawingSize.metricUnit() != i_metricUnit ) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        // If changing the unit the metrics width and height and also the
        // size in pixels remain the same. Only the unit in which the values
        // are indicated will be changed.
        m_drawingSize.setMetricUnit(i_metricUnit);
        updateImageSizeMetrics();
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setNormedPaperSize( const CEnumNormedPaperSize& i_ePaperSize )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_ePaperSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setNormedPaperSize",
        /* strAddInfo   */ strMthInArgs );

    if( m_drawingSize.normedPaperSize() != i_ePaperSize ) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_drawingSize.setNormedPaperSize(i_ePaperSize);
        m_pLblImageMetricNormedPaperOrientation->setVisible(i_ePaperSize.isValid());
        m_pCmbImageMetricNormedPaperOrientation->setVisible(i_ePaperSize.isValid());
        CEnumOrientation eOrientation = m_drawingSize.normedPaperOrientation();
        if (i_ePaperSize.isValid() && eOrientation.isValid()) {
            m_pCmbImageMetricNormedPaperOrientation->setCurrentText(
                eOrientation.toString(EEnumEntryAliasStrText));
            m_pEdtImageMetricWidth->setValue(m_drawingSize.metricImageWidth().getVal());
            m_pEdtImageMetricHeight->setValue(m_drawingSize.metricImageHeight().getVal());
            updateImageSizeInPixels();
        }
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setNormedPaperOrientation( const CEnumOrientation& i_eOrientation )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_eOrientation.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setNormedPaperOrientation",
        /* strAddInfo   */ strMthInArgs );

    if( m_drawingSize.normedPaperOrientation() != i_eOrientation ) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_drawingSize.setNormedPaperOrientation(i_eOrientation);
        // Setting values according to orientation makes only sense for normed paper sizes.
        // For user defined paper sizes there is no orientation. The width might be
        // greater than the height or vice versa. The orientation will be stored if later
        // on a normed paper size is set but for now the values will not be adjusted.
        CEnumNormedPaperSize ePaperSize = m_drawingSize.normedPaperSize();
        if (ePaperSize.isValid()) {
            m_pEdtImageMetricWidth->setValue(m_drawingSize.metricImageWidth().getVal());
            m_pEdtImageMetricHeight->setValue(m_drawingSize.metricImageHeight().getVal());
            updateImageSizeInPixels();
        }
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setScaleFactor( int i_iDividend, int i_iDivisor )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iDividend) + "/" + QString::number(i_iDivisor);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScaleFactor",
        /* strAddInfo   */ strMthInArgs );

    if( i_iDividend <= 0 ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            ClassName() + "::setScaleFactor("
                + QString::number(i_iDividend) + ", " + QString::number(i_iDivisor) + "): "
                + "!Dividend <= 0!");
    }
    if( i_iDivisor <= 0 ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            ClassName() + "::setScaleFactor("
                + QString::number(i_iDividend) + ", " + QString::number(i_iDivisor) + "): "
                + "!Divisor <= 0!");
    }
    if( m_drawingSize.scaleFactorDividend() != i_iDividend || m_drawingSize.scaleFactorDivisor() != i_iDivisor ) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_drawingSize.setScaleFactor(i_iDividend, i_iDivisor);
        updateImageSizeInPixels();
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setYScaleAxisOrientation( const CEnumYScaleAxisOrientation& i_eOrientation )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_eOrientation.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setYScaleAxisOrientation",
        /* strAddInfo   */ strMthInArgs );

    if( m_drawingSize.yScaleAxisOrientation() != i_eOrientation ) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_drawingSize.setYScaleAxisOrientation(i_eOrientation);
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setImageSize(
    const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValWidth.toString() + ", " + i_physValHeight.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setImageSize",
        /* strAddInfo   */ strMthInArgs );

    if( !i_physValWidth.isValid() || i_physValWidth.getVal() <= 0.0 ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            ClassName() + "::setImageSize("
                + i_physValWidth.toString() + ", " + i_physValHeight.toString() + "): "
                + "!i_physValWidth <= 0.0!");
    }
    if( !i_physValHeight.isValid() || i_physValHeight.getVal() <= 0.0 ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            ClassName() + "::setImageSize("
                + i_physValWidth.toString() + ", " + i_physValHeight.toString() + "): "
                + "!i_physValHeight <= 0.0!");
    }
    if( (i_physValWidth.unit() == Units.Length.px && i_physValHeight.unit() != Units.Length.px)
     || (i_physValWidth.unit() != Units.Length.px && i_physValHeight.unit() == Units.Length.px) ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            ClassName() + "::setImageSize("
                + i_physValWidth.toString() + ", " + i_physValHeight.toString() + "): "
                + "Either set size in pixels or in metrics");
    }

    QSize sizePrev_px = m_drawingSize.imageSizeInPixels();
    m_drawingSize.setImageSize(i_physValWidth, i_physValHeight);
    QSize sizeCurr_px = m_drawingSize.imageSizeInPixels();
    if( sizePrev_px != sizeCurr_px ) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        if( i_physValWidth.unit() == Units.Length.px ) {
            updateImageSizeMetrics();
        }
        else {
            updateImageSizeInPixels();
        }
        updatePaperFormat();
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLinesVisible(bool i_bVisible)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bVisible);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLinesVisible",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.areLinesVisible() != i_bVisible) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLinesVisible(i_bVisible);
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLinesDistMin(int i_iDistMin_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iDistMin_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLinesDistMin",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.linesDistMin() != i_iDistMin_px) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLinesDistMin(i_iDistMin_px);
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLinesStyle(ELineStyle i_lineStyle)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumLineStyle(i_lineStyle).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLinesStyle",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.linesStyle() != i_lineStyle) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLinesStyle(i_lineStyle);
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLinesColor(const QColor& i_color)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_color.name();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLinesColor",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.linesColor() != i_color) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLinesColor(i_color);
        updateGridLinesColorButtonIcon();
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLinesWidth(int i_iWidth_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iWidth_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLinesWidth",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.linesWidth() != i_iWidth_px) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLinesWidth(i_iWidth_px);
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLabelsVisible(bool i_bVisible)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bVisible);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsVisible",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.areLabelsVisible() != i_bVisible) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLabelsVisible(i_bVisible);
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLabelsFont(const QFont& i_fnt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_fnt.family();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsFont",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.labelsFont() != i_fnt) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLabelsFont(i_fnt);
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLabelsTextSize(ETextSize i_textSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = textSize2Str(i_textSize);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsTextSize",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.labelsTextSize() != i_textSize) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLabelsTextSize(i_textSize);
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLabelsTextColor(const QColor& i_color)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_color.name();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsTextColor",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.labelsTextColor() != i_color) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLabelsTextColor(i_color);
        updateGridLabelsTextColorButtonIcon();
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLabelsTextStyle(ETextStyle i_textStyle)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumTextStyle(i_textStyle).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsTextStyle",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.labelsTextStyle() != i_textStyle) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLabelsTextStyle(i_textStyle);
        updateGridLabelsTextStyleBoldButton();
        updateGridLabelsTextStyleItalicButton();
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLabelsTextEffect(const ETextEffect i_textEffect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumTextEffect(i_textEffect).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsTextEffect",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.labelsTextEffect() != i_textEffect) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLabelsTextEffect(i_textEffect);
        updateGridLabelsTextEffectUnderlineButton();
        updateGridLabelsTextEffectStrikeoutButton();
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::updateDimensionUnit()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateDimensionUnit",
        /* strAddInfo   */ "" );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

    CEnumDrawingDimensionUnit eDimensionUnit = m_drawingSize.dimensionUnit();

    m_pCmbDimensionUnit->setCurrentIndex(eDimensionUnit.enumeratorAsInt());
    m_pWdgtMetric->setVisible(eDimensionUnit == EDrawingDimensionUnit::Metric);
    //m_pCmbImageMetricUnit->setEnabled(eDimensionUnit == EDrawingDimensionUnit::Metric);
    //m_pEdtImageMetricWidth->setReadOnly(eDimensionUnit != EDrawingDimensionUnit::Metric);
    //m_pEdtImageMetricHeight->setReadOnly(eDimensionUnit != EDrawingDimensionUnit::Metric);
    //m_pCmbImageMetricNormedPaperSizes->setEnabled(eDimensionUnit == EDrawingDimensionUnit::Metric);
    //m_pCmbImageMetricNormedPaperOrientation->setEnabled(eDimensionUnit == EDrawingDimensionUnit::Metric);
    //m_pCmbImageMetricScaleFactorDividend->setEnabled(eDimensionUnit == EDrawingDimensionUnit::Metric);
    //m_pCmbImageMetricScaleFactorDivisor->setEnabled(eDimensionUnit == EDrawingDimensionUnit::Metric);
    m_pEdtImageSizeWidth_px->setReadOnly(eDimensionUnit != EDrawingDimensionUnit::Pixels);
    m_pEdtImageSizeHeight_px->setReadOnly(eDimensionUnit != EDrawingDimensionUnit::Pixels);

    updateImageSizeMetrics();
    updateImageSizeInPixels();
    updatePaperFormat();

} // updateDimensionUnit

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::updateResolution()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateResolution",
        /* strAddInfo   */ "" );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

    double fResolution_pxpin = 1.0 / Units.Length.physValResolution(Units.Length.in).getVal();
    double fResolution_pxpmm = 1.0 / Units.Length.physValResolution(Units.Length.mm).getVal();

    QString strPxpin = QString::number(fResolution_pxpin,'f',2);
    QString strPxpmm = QString::number(fResolution_pxpmm,'f',2);

    m_pEdtResolution_pxpi->setText(strPxpin);
    m_pEdtResolution_pxpmm->setText(strPxpmm);
}

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

    CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
    QSize size_px = m_drawingSize.imageSizeInPixels();
    m_pEdtImageSizeWidth_px->setValue(size_px.width());
    m_pEdtImageSizeHeight_px->setValue(size_px.height());

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

    CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

    m_pCmbImageMetricUnit->setCurrentText(m_drawingSize.metricUnit().symbol());

    m_pEdtImageMetricWidth->setUnit(m_drawingSize.metricUnit());
    m_pEdtImageMetricWidth->setResolution(m_drawingSize.metricImageWidth().getRes().getVal());
    m_pEdtImageMetricWidth->setMaximum(100000);
    m_pEdtImageMetricWidth->setValue(m_drawingSize.metricImageWidth().getVal());

    m_pEdtImageMetricHeight->setUnit(m_drawingSize.metricUnit());
    m_pEdtImageMetricHeight->setResolution(m_drawingSize.metricImageHeight().getRes().getVal());
    m_pEdtImageMetricHeight->setMaximum(100000);
    m_pEdtImageMetricHeight->setValue(m_drawingSize.metricImageHeight().getVal());

    m_pCmbImageMetricScaleFactorDividend->setCurrentText(
        QString::number(m_drawingSize.scaleFactorDividend()));
    m_pCmbImageMetricScaleFactorDivisor->setCurrentText(
        QString::number(m_drawingSize.scaleFactorDivisor()));

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

    CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

    CEnumNormedPaperSize ePaperSize = m_drawingSize.normedPaperSize();
    CEnumOrientation eOrientation = m_drawingSize.normedPaperOrientation();

    if (ePaperSize.isValid()) {
        m_pCmbImageMetricNormedPaperSizes->setCurrentText(ePaperSize.toString(EEnumEntryAliasStrSymbol));
        m_pLblImageMetricNormedPaperOrientation->setVisible(true);
        m_pCmbImageMetricNormedPaperOrientation->setVisible(true);
        if (eOrientation.isValid()) {
            m_pCmbImageMetricNormedPaperOrientation->setCurrentText(
                eOrientation.toString(EEnumEntryAliasStrText));
        }
    }
    else {
        m_pCmbImageMetricNormedPaperSizes->setCurrentText("---");
        m_pLblImageMetricNormedPaperOrientation->setVisible(false);
        m_pCmbImageMetricNormedPaperOrientation->setVisible(false);
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
} // updatePaperFormat

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::updateGridSettings()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateGridSettings",
        /* strAddInfo   */ "" );

    CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

    m_pChkGridLinesVisible->setChecked(m_gridSettings.areLinesVisible());
    m_pEdtGridLinesDistMin->setValue(m_gridSettings.linesDistMin());
    m_pEdtGridLinesWidth->setValue(m_gridSettings.linesWidth());
    m_pChkGridScaleLabelsVisible->setChecked(m_gridSettings.areLabelsVisible());
    int idx = m_pCmbGridLinesStyle->findData(m_gridSettings.linesStyle().enumeratorAsInt());
    if (idx >= 0) {
        m_pCmbGridLinesStyle->setCurrentIndex(idx);
    }
    m_pCmbGridScaleLabelsFont->setCurrentFont(m_gridSettings.labelsFont());
    m_pCmbGridScaleLabelsFontSize->setCurrentIndex(m_gridSettings.labelsTextSize());

    updateGridLinesColorButtonIcon();
    updateGridLabelsTextColorButtonIcon();
    updateGridLabelsTextStyleBoldButton();
    updateGridLabelsTextStyleItalicButton();
    updateGridLabelsTextEffectUnderlineButton();
    updateGridLabelsTextEffectStrikeoutButton();
}

//------------------------------------------------------------------------------
QSize CWdgtDrawingViewProperties::fillGridLineStylesModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "fillGridLineStylesModel",
        /* strAddInfo   */ "" );

    QSize iconSize(48, 16);
    QPixmap pxmLineStyle(iconSize);
    QPainter painter;
    QPen pen;

    pen.setColor(m_gridSettings.linesColor());
    pen.setWidth(m_gridSettings.linesWidth());

    for (CEnumLineStyle lineStyle = 0; lineStyle < CEnumLineStyle::count(); ++lineStyle)
    {
        if (lineStyle != ELineStyle::NoLine)
        {
            pxmLineStyle.fill(Qt::white);

            painter.begin(&pxmLineStyle);
            pen.setStyle(lineStyle2QtPenStyle(lineStyle.enumerator()));
            pen.setColor(Qt::black);
            pen.setWidth(1);
            painter.setPen(pen);
            painter.drawLine(4, pxmLineStyle.height()/2, pxmLineStyle.width()-4, pxmLineStyle.height()/2);
            painter.end();

            QStandardItem* pLineStyleItem = new QStandardItem();
            pLineStyleItem->setData(pxmLineStyle, Qt::DecorationRole);
            pLineStyleItem->setData(static_cast<int>(lineStyle.enumerator()), Qt::UserRole);
            m_pModelGridLinesStyles->appendRow(pLineStyleItem);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        mthTracer.setMethodReturn(qSize2Str(iconSize));
    }
    return iconSize;
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::updateGridLinesColorButtonIcon()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateGridLinesColorButtonIcon",
        /* strAddInfo   */ "" );

    QPainter painter(m_pPxmBtnGridLinesColor);

    painter.setPen(m_gridSettings.linesColor());
    painter.setBrush(Qt::NoBrush);
    painter.drawLine(m_lineBtnGridLinesColor1);
    painter.drawLine(m_lineBtnGridLinesColor2);
    painter.drawLine(m_lineBtnGridLinesColor3);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_gridSettings.linesColor());
    painter.drawRect(m_rctBtnGridLinesColor);

    m_pBtnGridLinesColor->setIcon(*m_pPxmBtnGridLinesColor);
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::updateGridLabelsTextColorButtonIcon()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateGridLabelsTextColorButtonIcon",
        /* strAddInfo   */ "" );

    QPainter painter(m_pPxmBtnGridScaleLabelsTextColor);

    painter.setPen(m_gridSettings.labelsTextColor());
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_gridSettings.labelsTextColor());
    painter.drawRect(m_rctBtnGridScaleLabelsTextColor);

    m_pBtnGridScaleLabelsTextColor->setIcon(*m_pPxmBtnGridScaleLabelsTextColor);
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::updateGridLabelsTextStyleBoldButton()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateGridLabelsTextStyleBoldButton",
        /* strAddInfo   */ "" );

    QPixmap pxmCheckmark(":/ZS/App/Checkmark.png");

    if (isTextStyleBold(m_gridSettings.labelsTextStyle().enumerator())) {
        m_pBtnGridScaleLabelsFontStyleBold->setChecked(true);
        m_pBtnGridScaleLabelsFontStyleBold->setIcon(pxmCheckmark);
    } else {
        m_pBtnGridScaleLabelsFontStyleBold->setChecked(false);
        m_pBtnGridScaleLabelsFontStyleBold->setIcon(QIcon());
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::updateGridLabelsTextStyleItalicButton()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateGridLabelsTextStyleItalicButton",
        /* strAddInfo   */ "" );

    QPixmap pxmCheckmark(":/ZS/App/Checkmark.png");

    if (isTextStyleItalic(m_gridSettings.labelsTextStyle().enumerator())) {
        m_pBtnGridScaleLabelsFontStyleItalic->setChecked(true);
        m_pBtnGridScaleLabelsFontStyleItalic->setIcon(pxmCheckmark);
    } else {
        m_pBtnGridScaleLabelsFontStyleItalic->setChecked(false);
        m_pBtnGridScaleLabelsFontStyleItalic->setIcon(QIcon());
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::updateGridLabelsTextEffectUnderlineButton()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateGridLabelsTextEffectUnderlineButton",
        /* strAddInfo   */ "" );

    QPixmap pxmCheckmark(":/ZS/App/Checkmark.png");

    if (isTextEffectUnderline(m_gridSettings.labelsTextEffect().enumerator())) {
        m_pBtnGridScaleLabelsTextEffectUnderline->setChecked(true);
        m_pBtnGridScaleLabelsTextEffectUnderline->setIcon(pxmCheckmark);
    } else {
        m_pBtnGridScaleLabelsTextEffectUnderline->setChecked(false);
        m_pBtnGridScaleLabelsTextEffectUnderline->setIcon(QIcon());
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::updateGridLabelsTextEffectStrikeoutButton()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateGridLabelsTextEffectStrikeoutButton",
        /* strAddInfo   */ "" );

    QPixmap pxmCheckmark(":/ZS/App/Checkmark.png");

    if (isTextEffectStrikeout(m_gridSettings.labelsTextEffect().enumerator())) {
        m_pBtnGridScaleLabelsTextEffectStrikeout->setChecked(true);
        m_pBtnGridScaleLabelsTextEffectStrikeout->setIcon(pxmCheckmark);
    } else {
        m_pBtnGridScaleLabelsTextEffectStrikeout->setChecked(false);
        m_pBtnGridScaleLabelsTextEffectStrikeout->setIcon(QIcon());
    }
}

//------------------------------------------------------------------------------
/*! @brief Registers the given edit widget for the edit property dialog by
           installing an event filter for the given edit widget for the
           mouse double click event.

    @param [in] i_pEdtWidget
        Edit widget for which the edit dialog should be opened by a mouse
        double click. As a precondition an object name must have been set
        at the edit widget as follows:
        <DimensionUnit>.<CoordinatePart1>[.<CoordinatePart2>]
        The sections of the object name will be used by the edit dialog
        to create the labels and to decide, which property has to be
        edited. E.g. for "Pixels.Point 1.X" the x coordinate of Point 1
        in dimension unit pixels will be edited.
*/
void CWdgtDrawingViewProperties::registerEditPropertyDialog(QLineEdit* i_pEdtWidget)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pEdtWidget->objectName();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "registerEditPropertyDialog",
        /* strAddInfo   */ strMthInArgs );

    QString strObjName = i_pEdtWidget->objectName();
    m_hshpRegisteredEditPropertyDialogs.insert(strObjName, i_pEdtWidget);
    // To filter mouse events the event filter must also be set at the spin box
    // and the line edit of the spin box. So we must call the installEventFilter
    // method of class CWdgtEditPhysVal which not just installs the event filter
    // on the EditPhysVal widget but also forwards the method to both the
    // spin box and the line edit.
    i_pEdtWidget->installEventFilter(this);
}

/*==============================================================================
protected: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Opens the edit property dialog on pressing Ctrl + MouseDblClick.
*/
bool CWdgtDrawingViewProperties::eventFilter(QObject* i_pObjWatched, QEvent* i_pEv)
//------------------------------------------------------------------------------
{
    bool bHandled = false;

    if (i_pEv->type() == QEvent::MouseButtonDblClick) {
        QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(i_pEv);
        if (pMouseEvent->modifiers() & Qt::ControlModifier) {
            QString strObjName = i_pObjWatched->objectName();
            QWidget* pEdtWidget = m_hshpRegisteredEditPropertyDialogs.value(strObjName, nullptr);
            if (pEdtWidget != nullptr) {
                if (m_pDlgEditProperty == nullptr) {
                    m_pDlgEditProperty = new CDlgDrawingViewEditProperty(m_pDrawingView, this);
                    m_pDlgEditProperty->setAttribute(Qt::WA_DeleteOnClose, true);
                    QObject::connect(
                        m_pDlgEditProperty, &QDialog::destroyed,
                        this, &CWdgtDrawingViewProperties::onDlgEditPropertyDestroyed);
                }
                m_pDlgEditProperty->setCoordinate(pEdtWidget->objectName());
                m_pDlgEditProperty->show();
                bHandled = true;
            }
        }
    }
    if (!bHandled) {
        bHandled = CWdgtGraphObjPropertiesAbstract::eventFilter(i_pObjWatched, i_pEv);
    }
    return bHandled;
}

/*==============================================================================
protected: // instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::traceValues(CMethodTracer& mthTracer, EMethodDir i_methodDir)
//------------------------------------------------------------------------------
{
    QString strMthLog = QString(i_methodDir == EMethodDir::Enter ? "-+ " : "+- ")
        + m_pCmbDimensionUnit->currentText()
        + ", Paper (" + m_pCmbImageMetricNormedPaperSizes->currentText() 
            + ", " + m_pCmbImageMetricNormedPaperOrientation->currentText() + ")"
        + ", Scale (" + m_pCmbImageMetricScaleFactorDividend->currentText()
            + "/" + m_pCmbImageMetricScaleFactorDivisor->currentText() + ")"
        + ", Size (" + m_pEdtImageMetricWidth->value().toString()
            + " * " + m_pEdtImageMetricHeight->value().toString() + ")"
        + ", Size (" + QString::number(m_pEdtImageSizeWidth_px->value())
            + " * " + QString::number(m_pEdtImageSizeHeight_px->value()) + " px)"
        + ", ButtonsEnabled (Apply: " + bool2Str(m_pBtnApply->isEnabled())
            + ", Reset: " + bool2Str(m_pBtnReset->isEnabled()) + ")";
    mthTracer.trace(strMthLog);
}
