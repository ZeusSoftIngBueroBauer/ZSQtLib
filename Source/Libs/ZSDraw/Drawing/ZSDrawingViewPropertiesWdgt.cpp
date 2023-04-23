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
#include "ZSDraw/Drawing/ZSDrawDlgDrawingViewSetup.h"
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
#include <QtCore/qcoreapplication.h>
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
    CDrawingView* i_pDrawingView, EMode i_mode, QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtGraphObjPropertiesAbstract(i_pDrawingView->drawingScene(), i_pWdgtParent),
    m_pDrawingView(i_pDrawingView),
    m_mode(EMode::Edit),
    // Caching values
    m_drawingSize("DrawingViewPropertiesWdgt"),
    m_gridSettings("DrawingViewPropertiesWdgt"),
    // Blocking signals counter
    m_iValueChangedSignalsBlocked(0),
    // Edit Controls
    m_pLyt(nullptr),
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
    // Image Size in Pixels
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
    m_pCmbGridLineStyle(nullptr),
    m_pModelGridLineStyles(nullptr),
    m_pEdtGridPenWidth(nullptr),
    m_pPxmBtnGridPenColor(nullptr),
    m_rctBtnGridPenColor(0, 12, 16, 4),
    m_lineBtnGridPenColor1(QPoint(0,2), QPoint(8,2)),
    m_lineBtnGridPenColor2(QPoint(0,2), QPoint(0,10)),
    m_lineBtnGridPenColor3(QPoint(0,10), QPoint(5,10)),
    m_pBtnGridPenColor(nullptr),
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
    // Button Line
    m_pWdgtButtons(nullptr),
    m_pLytWdgtButtons(nullptr),
    m_pBtnEdit(nullptr),
    // Trace admin object for method tracing
    m_pTrcAdminObj(nullptr)
{
    setObjectName(CEnumMode(i_mode).toString() + i_pDrawingView->objectName());

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = CEnumMode(i_mode).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_pLyt = new QVBoxLayout;
    setLayout(m_pLyt);

    QObject::connect(
        m_pDrawingView, &CDrawingView::drawingSizeChanged,
        this, &CWdgtDrawingViewProperties::onDrawingViewDrawingSizeChanged );
    QObject::connect(
        m_pDrawingView, &CDrawingView::gridSettingsChanged,
        this, &CWdgtDrawingViewProperties::onDrawingViewGridSettingsChanged );

    m_drawingSize = m_pDrawingView->drawingSize();
    m_gridSettings = m_pDrawingView->gridSettings();

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
    m_pLblDimensionUnit->setFixedWidth(m_cxLblWidthClm1);
    m_pLytLineDimensionUnit->addWidget(m_pLblDimensionUnit);
    m_pCmbDimensionUnit = new QComboBox();
    m_pCmbDimensionUnit->setFixedWidth(m_cxEdtWidthClm1);
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
    m_pEdtResolution_pxpi->setEnabled(false);
    //m_pEdtResolution_pxpi->setReadOnly(true);
    m_pLytLineResolution->addWidget(m_pEdtResolution_pxpi);
    m_pLytLineResolution->addSpacing(m_cxClmSpacing);

    strResolution = strXPxpmm + " * " + strYPxpmm;
    m_pLblResolution_pxpmm = new QLabel("Pixels/mm:");
    m_pLblResolution_pxpmm->setFixedWidth(m_cxLblWidthClm2);
    m_pLytLineResolution->addWidget(m_pLblResolution_pxpmm);
    m_pEdtResolution_pxpmm = new QLineEdit(strResolution);
    m_pEdtResolution_pxpmm->setFixedWidth(m_cxEdtWidthClm2);
    m_pEdtResolution_pxpmm->setEnabled(false);
    //m_pEdtResolution_pxpmm->setReadOnly(true);
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
    if( m_drawingSize.normedPaperSize() == ENormedPaperSize::Undefined) {
        m_pCmbImageMetricNormedPaperSizes->setCurrentText(
            m_drawingSize.normedPaperSize().toString(EEnumEntryAliasStrText));
    }
    else {
        m_pCmbImageMetricNormedPaperSizes->setCurrentText(
            m_drawingSize.normedPaperSize().toString(EEnumEntryAliasStrSymbol));
    }
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
    m_pCmbImageMetricNormedPaperOrientation->addItem(m_drawingSize.normedPaperOrientation().toString());
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

    // <Visibility> Metric Widget
    //---------------------------

    m_pWdgtMetric->setVisible(false);

    // <Section> Image Size
    //---------------------

    m_pLytSepLineImageSize_px = new QHBoxLayout();
    m_pLytGrpGeometry->addLayout(m_pLytSepLineImageSize_px);
    m_pLblSepLineImageSize_px = new QLabel("Pixel Sizes");
    m_pLytSepLineImageSize_px->addWidget(m_pLblSepLineImageSize_px);
    m_pSepLineImageSize_px = new CSepLine(10);
    m_pLytSepLineImageSize_px->addWidget(m_pSepLineImageSize_px,1);

    // <Line> Image Size
    //------------------

    m_pLytLineImageSize_px = new QHBoxLayout();
    m_pLytGrpGeometry->addLayout(m_pLytLineImageSize_px);

    // <Edit> Image Width
    //--------------------

    m_pLblImageSizeWidth_px = new QLabel("Width:");
    m_pLblImageSizeWidth_px->setFixedWidth(m_cxLblWidthClm1);
    m_pLytLineImageSize_px->addWidget(m_pLblImageSizeWidth_px);
    m_pEdtImageSizeWidth_px = new QSpinBox();
    m_pEdtImageSizeWidth_px->setFixedWidth(m_cxEdtWidthClm1);
    m_pEdtImageSizeWidth_px->setMinimum(1);
    m_pEdtImageSizeWidth_px->setMaximum(100000);
    m_pEdtImageSizeWidth_px->setValue(m_drawingSize.imageSizeInPixels().width());
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
    m_pEdtImageSizeHeight_px->setValue(m_drawingSize.imageSizeInPixels().height());
    m_pEdtImageSizeHeight_px->setSuffix(" px");
    m_pEdtImageSizeHeight_px->setReadOnly(false);
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

    const int iClmLineHeadLines = 0;
    const int iClmChkVisible = 1;
    const int iClmCmbFont = 2;
    const int iClmSpanCmbFont = 3;
    const int iClmCmbFontSize = 5;
    const int iClmBtnTextColor = 6;
    const int iClmBtnFontStyleBold = 2;
    const int iClmBtnFontStyleItalic = 3;
    const int iClmBtnFontEffectUnderline = 4;
    const int iClmBtnFontEffectStrikeout = 5;

    m_pLytGridSettings->setColumnMinimumWidth(iClmChkVisible, 32);

    int iRow = 0;
    int iClm = iClmLineHeadLines;

    m_pLblGridLines = new QLabel("Lines:");
    m_pLblGridLines->setFixedWidth(m_cxLblWidthClm1);
    m_pLytGridSettings->addWidget(m_pLblGridLines, iRow, iClm);

    iClm = iClmChkVisible;
    m_pChkGridLinesVisible = new QCheckBox();
    m_pChkGridLinesVisible->setChecked(m_gridSettings.isVisible());
    m_pLytGridSettings->addWidget(m_pChkGridLinesVisible, iRow, iClm);
    QObject::connect(
        m_pChkGridLinesVisible, &QCheckBox::stateChanged,
        this, &CWdgtDrawingViewProperties::onChkGridLinesVisibleStateChanged);

    m_pModelGridLineStyles = new QStandardItemModel();
    QSize iconSize = fillGridLineStylesModel();
    iClm = iClmCmbFont;
    m_pCmbGridLineStyle = new QComboBox();
    m_pCmbGridLineStyle->setModel(m_pModelGridLineStyles);
    m_pCmbGridLineStyle->setIconSize(iconSize);
    m_pLytGridSettings->addWidget(m_pCmbGridLineStyle, iRow, iClm);
    QObject::connect(
        m_pCmbGridLineStyle, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtDrawingViewProperties::onCmbGridLineStyleCurrentIndexChanged);

    // The list view  "m_pListViewGridLineStyles" must be created for the first
    // time the button is pressed. If created before the widgets created afterwards
    // will be above the list view in the widget hierarchy and the list view will
    // be overlapped by those and is partly invisible.

    iClm = iClmCmbFontSize;
    m_pEdtGridPenWidth = new QSpinBox();
    m_pEdtGridPenWidth->setMinimum(1);
    m_pEdtGridPenWidth->setMaximum(9999);
    m_pEdtGridPenWidth->setSuffix(" px");
    m_pEdtGridPenWidth->setValue(m_gridSettings.penWidth());
    m_pLytGridSettings->addWidget(m_pEdtGridPenWidth, iRow, iClm);
    QObject::connect(
        m_pEdtGridPenWidth, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtDrawingViewProperties::onEdtGridPenWidthValueChanged);

    iClm = iClmBtnTextColor;
    m_pPxmBtnGridPenColor = new QPixmap(":/ZS/Draw/DrawToolPen16x16.bmp");
    m_pPxmBtnGridPenColor->setMask(m_pPxmBtnGridPenColor->createHeuristicMask());
    m_pBtnGridPenColor = new QPushButton();
    updateGridPenColorButtonIcon();
    m_pLytGridSettings->addWidget(m_pBtnGridPenColor, iRow, iClm);
    QObject::connect(
        m_pBtnGridPenColor, &QPushButton::clicked,
        this, &CWdgtDrawingViewProperties::onBtnGridPenColorClicked);

    // <Line> Grid Scale Labels Visible and Font Size
    //-----------------------------------------------

    ++iRow;
    iClm = iClmLineHeadLines;
    m_pLblGridScaleLabels = new QLabel("Labels:");
    m_pLblGridScaleLabels->setFixedWidth(m_cxLblWidthClm1);
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

    // <Stretch> to buttons line
    //==========================

    m_pLyt->addStretch();

    // <Buttons>
    //==========

    m_pWdgtButtons = new QWidget();
    m_pLytWdgtButtons = new QHBoxLayout();
    m_pWdgtButtons->setLayout(m_pLytWdgtButtons);
    m_pLytWdgtButtons->setContentsMargins(0, 0, 0, 0);
    m_pLyt->addWidget(m_pWdgtButtons);

    m_pBtnEdit = new QPushButton("Edit");
    m_pBtnEdit->setEnabled(false);
    m_pLytWdgtButtons->addWidget(m_pBtnEdit);
    QObject::connect(
        m_pBtnEdit, &QPushButton::clicked,
        this, &CWdgtDrawingViewProperties::onBtnEditClicked);

    m_pLytWdgtButtons->addStretch();

    m_pWdgtButtons->hide();

    // Update metrics
    //---------------

    updateDimensionUnit();

    // Set Mode
    //---------

    setMode(i_mode);

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
        delete m_pModelGridLineStyles;
    }
    catch(...) {
    }
    try {
        delete m_pPxmBtnGridPenColor;
    }
    catch(...) {
    }
    try {
        delete m_pPxmBtnGridScaleLabelsTextColor;
    }
    catch(...) {
    }

    mthTracer.onAdminObjAboutToBeReleased();
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDrawingView = nullptr;
    m_mode = static_cast<EMode>(0);
    // Caching values
    //m_drawingSize;
    //m_gridSettings;
    // Blocking signals counter
    m_iValueChangedSignalsBlocked = 0;
    // Edit Controls
    m_pLyt = nullptr;
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
    m_pLytSepLineImageSize_px = nullptr;
    m_pLblSepLineImageSize_px = nullptr;
    m_pSepLineImageSize_px = nullptr;
    m_pLytLineImageSize_px = nullptr;
    m_pLblImageSizeWidth_px = nullptr;
    m_pEdtImageSizeWidth_px = nullptr;
    m_pLblImageSizeHeight_px = nullptr;
    m_pEdtImageSizeHeight_px = nullptr;
    // Grid
    m_pGrpGrid = nullptr;
    m_pLytGrpGrid = nullptr;
    m_pLytGridSettings = nullptr;
    m_pLblGridLines = nullptr;
    m_pChkGridLinesVisible = nullptr;
    m_pCmbGridLineStyle = nullptr;
    m_pModelGridLineStyles = nullptr;
    m_pEdtGridPenWidth = nullptr;
    m_pPxmBtnGridPenColor = nullptr;
    //m_rctBtnGridPenColor;
    //m_lineBtnGridPenColor1;
    //m_lineBtnGridPenColor2;
    //m_lineBtnGridPenColor3;
    m_pBtnGridPenColor = nullptr;
    // Grid Scale Labels
    m_pLblGridScaleLabels = nullptr;
    m_pChkGridScaleLabelsVisible = nullptr;
    m_pCmbGridScaleLabelsFont = nullptr;
    m_pCmbGridScaleLabelsFontSize = nullptr;
    m_pPxmBtnGridScaleLabelsTextColor = nullptr;
    //m_rctBtnGridScaleLabelsTextColor;
    m_pBtnGridScaleLabelsTextColor = nullptr;
    m_pBtnGridScaleLabelsFontStyleBold = nullptr;
    m_pBtnGridScaleLabelsFontStyleItalic = nullptr;
    m_pBtnGridScaleLabelsTextEffectUnderline = nullptr;
    m_pBtnGridScaleLabelsTextEffectStrikeout = nullptr;
    // Button Line
    m_pWdgtButtons = nullptr;
    m_pLytWdgtButtons = nullptr;
    m_pBtnEdit = nullptr;
    // Trace admin object for method tracing
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtDrawingViewProperties::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = (m_drawingSize != m_pDrawingView->drawingSize());
    if (!bHasChanges) {
        bHasChanges = (m_gridSettings != m_pDrawingView->gridSettings());
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

    onDrawingViewDrawingSizeChanged(m_pDrawingView->drawingSize());
    onDrawingViewGridSettingsChanged(m_pDrawingView->gridSettings());
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setMode(EMode i_mode)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = CEnumMode(i_mode).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMode",
        /* strAddInfo   */ strMthInArgs );

    if (m_mode != i_mode)
    {
        m_mode = i_mode;

        if (m_mode == EMode::Edit)
        {
            CEnumDrawingDimensionUnit eDimensionUnit = m_drawingSize.dimensionUnit();

            m_pCmbDimensionUnit->setEnabled(true);
            //m_pEdtResolution_pxpi->setEnabled(true);
            //m_pEdtResolution_pxpmm->setEnabled(true);
            m_pCmbImageMetricUnit->setEnabled(eDimensionUnit == EDrawingDimensionUnit::Metric);
            m_pEdtImageMetricWidth->setEnabled(eDimensionUnit == EDrawingDimensionUnit::Metric);
            //m_pEdtImageMetricWidth->setReadOnly(eDimensionUnit != EDrawingDimensionUnit::Metric);
            m_pEdtImageMetricHeight->setEnabled(eDimensionUnit == EDrawingDimensionUnit::Metric);
            //m_pEdtImageMetricHeight->setReadOnly(eDimensionUnit != EDrawingDimensionUnit::Metric);
            m_pCmbImageMetricNormedPaperSizes->setEnabled(eDimensionUnit == EDrawingDimensionUnit::Metric);
            m_pCmbImageMetricNormedPaperOrientation->setEnabled(eDimensionUnit == EDrawingDimensionUnit::Metric);
            m_pCmbImageMetricScaleFactorDividend->setEnabled(eDimensionUnit == EDrawingDimensionUnit::Metric);
            m_pCmbImageMetricScaleFactorDivisor->setEnabled(eDimensionUnit == EDrawingDimensionUnit::Metric);
            m_pEdtImageSizeWidth_px->setEnabled(eDimensionUnit == EDrawingDimensionUnit::Pixels);
            //m_pEdtImageSizeWidth_px->setReadOnly(eDimensionUnit != EDrawingDimensionUnit::Pixels);
            m_pEdtImageSizeHeight_px->setEnabled(eDimensionUnit == EDrawingDimensionUnit::Pixels);
            //m_pEdtImageSizeHeight_px->setReadOnly(eDimensionUnit != EDrawingDimensionUnit::Pixels);

            m_pChkGridLinesVisible->setEnabled(true);
            m_pCmbGridLineStyle->setEnabled(true);
            m_pBtnGridPenColor->setEnabled(true);
            m_pEdtGridPenWidth->setEnabled(true);
            //m_pEdtGridPenWidth->setReadOnly(false);
            m_pChkGridScaleLabelsVisible->setEnabled(true);
            m_pCmbGridScaleLabelsFont->setEnabled(true);
            m_pCmbGridScaleLabelsFontSize->setEnabled(true);
            m_pBtnGridScaleLabelsFontStyleBold->setEnabled(true);
            m_pBtnGridScaleLabelsFontStyleItalic->setEnabled(true);
            m_pBtnGridScaleLabelsTextEffectUnderline->setEnabled(true);
            m_pBtnGridScaleLabelsTextEffectStrikeout->setEnabled(true);
            m_pBtnGridScaleLabelsTextColor->setEnabled(true);

            m_pWdgtButtons->show();
            m_pBtnEdit->setEnabled(false);
        }
        else // if (m_mode == EMode::View)
        {
            m_pCmbDimensionUnit->setEnabled(false);
            //m_pEdtResolution_pxpi->setEnabled(false);
            //m_pEdtResolution_pxpmm->setEnabled(false);
            m_pCmbImageMetricUnit->setEnabled(false);
            m_pEdtImageMetricWidth->setEnabled(false);
            //m_pEdtImageMetricWidth->setReadOnly(true);
            m_pEdtImageMetricHeight->setEnabled(false);
            //m_pEdtImageMetricHeight->setReadOnly(true);
            m_pCmbImageMetricNormedPaperSizes->setEnabled(false);
            m_pCmbImageMetricNormedPaperOrientation->setEnabled(false);
            m_pCmbImageMetricScaleFactorDividend->setEnabled(false);
            m_pCmbImageMetricScaleFactorDivisor->setEnabled(false);
            m_pEdtImageSizeWidth_px->setEnabled(false);
            m_pEdtImageSizeWidth_px->setReadOnly(true);
            m_pEdtImageSizeHeight_px->setEnabled(false);
            m_pEdtImageSizeHeight_px->setReadOnly(true);

            m_pChkGridLinesVisible->setEnabled(false);
            m_pCmbGridLineStyle->setEnabled(false);
            m_pBtnGridPenColor->setEnabled(false);
            m_pEdtGridPenWidth->setEnabled(false);
            //m_pEdtGridPenWidth->setReadOnly(true);
            m_pChkGridScaleLabelsVisible->setEnabled(false);
            m_pCmbGridScaleLabelsFont->setEnabled(false);
            m_pCmbGridScaleLabelsFontSize->setEnabled(false);
            m_pBtnGridScaleLabelsFontStyleBold->setEnabled(false);
            m_pBtnGridScaleLabelsFontStyleItalic->setEnabled(false);
            m_pBtnGridScaleLabelsTextEffectUnderline->setEnabled(false);
            m_pBtnGridScaleLabelsTextEffectStrikeout->setEnabled(false);
            m_pBtnGridScaleLabelsTextColor->setEnabled(false);

            m_pWdgtButtons->show();
            m_pBtnEdit->setEnabled(true);
        }
    }
}

//------------------------------------------------------------------------------
EMode CWdgtDrawingViewProperties::mode() const
//------------------------------------------------------------------------------
{
    return m_mode;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onDrawingViewDrawingSizeChanged(const CDrawingSize& i_size)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_size.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingViewDrawingSizeChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_drawingSize != i_size ) {
        m_drawingSize = i_size;
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        updateDimensionUnit();
        emit_drawingSizeChanged(m_drawingSize);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onDrawingViewGridSettingsChanged(const CDrawGridSettings& i_settings)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_settings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingViewGridSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings != i_settings ) {
        m_gridSettings = i_settings;
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        updateGridSettings();
        emit_gridSettingsChanged(m_gridSettings);
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

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

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onChkGridLinesVisibleStateChanged(int i_iState)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
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

    setGridVisible(i_iState == Qt::Checked);
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onCmbGridLineStyleCurrentIndexChanged(int i_idx)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbGridLineStyleCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

    if( i_idx >= 0 && i_idx < CEnumLineStyle::count() )
    {
        QVariant varData = m_pCmbGridLineStyle->itemData(i_idx, Qt::UserRole);
        ELineStyle lineStyle = static_cast<ELineStyle>(varData.toInt());
        setGridLineStyle(lineStyle);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onEdtGridPenWidthValueChanged(int i_iVal)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iVal);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnGridPenColorClicked",
        /* strAddInfo   */ strMthInArgs );

    setGridPenWidth(i_iVal);
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onBtnGridPenColorClicked(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnGridPenColorClicked",
        /* strAddInfo   */ strMthInArgs );

    QColor clr = QColorDialog::getColor(
        /* clrInitial  */ m_gridSettings.penColor(),
        /* pWdgtParent */ m_pBtnGridPenColor,
        /* strTitle    */ "Colors",
        /* options     */ QColorDialog::ShowAlphaChannel );

    if( clr.isValid() )
    {
        setGridPenColor(clr);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::onChkGridScaleLabelsVisibleStateChanged(int i_iState)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
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
    if( m_iValueChangedSignalsBlocked > 0 ) {
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
    if( m_iValueChangedSignalsBlocked > 0 ) {
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
    if( m_iValueChangedSignalsBlocked > 0 ) {
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
    if( m_iValueChangedSignalsBlocked > 0 ) {
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
    if( m_iValueChangedSignalsBlocked > 0 ) {
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
    if( m_iValueChangedSignalsBlocked > 0 ) {
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
void CWdgtDrawingViewProperties::onBtnEditClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnEditClicked",
        /* strAddInfo   */ "" );

    QString strDlgTitle = QCoreApplication::applicationName() + ": Page Setup";
    CDlgDrawingViewSetup* pDlg = CDlgDrawingViewSetup::GetInstance(m_pDrawingView);
    if( pDlg == nullptr ) {
        pDlg = CDlgDrawingViewSetup::CreateInstance(strDlgTitle, m_pDrawingView);
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->show();
    }
    else {
        if( pDlg->isHidden() ) {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setDimensionUnit( const CEnumDrawingDimensionUnit& i_eDimensionUnit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_eDimensionUnit.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDimensionUnit",
        /* strAddInfo   */ strMthInArgs );

    if( m_drawingSize.dimensionUnit() != i_eDimensionUnit ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_drawingSize.setDimensionUnit(i_eDimensionUnit);
        updateDimensionUnit();
        emit_drawingSizeChanged(m_drawingSize);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setMetricUnit( const CUnit& i_metricUnit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_metricUnit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMetricUnit",
        /* strAddInfo   */ strMthInArgs );

    if( m_drawingSize.metricUnit() != i_metricUnit ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        // If changing the unit the metrics width and height and also the
        // size in pixels remain the same. Only the unit in which the values
        // are indicated will be changed.
        m_drawingSize.setMetricUnit(i_metricUnit);
        updateImageSizeMetrics();
        emit_drawingSizeChanged(m_drawingSize);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setNormedPaperSize( const CEnumNormedPaperSize& i_ePaperSize )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_ePaperSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setNormedPaperSize",
        /* strAddInfo   */ strMthInArgs );

    if( m_drawingSize.normedPaperSize() != i_ePaperSize ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_drawingSize.setNormedPaperSize(i_ePaperSize);
        m_pLblImageMetricNormedPaperOrientation->setVisible(i_ePaperSize != ENormedPaperSize::Undefined);
        m_pCmbImageMetricNormedPaperOrientation->setVisible(i_ePaperSize != ENormedPaperSize::Undefined);
        m_pCmbImageMetricNormedPaperOrientation->clear();
        if( i_ePaperSize == ENormedPaperSize::Undefined ) {
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
            CEnumDirection eDirection = m_drawingSize.normedPaperOrientation();
            m_pCmbImageMetricNormedPaperOrientation->setCurrentText(eDirection.toString());
            m_pEdtImageMetricWidth->setValue(m_drawingSize.metricImageWidth().getVal());
            m_pEdtImageMetricHeight->setValue(m_drawingSize.metricImageHeight().getVal());
            updateImageSizeInPixels();
        }
        emit_drawingSizeChanged(m_drawingSize);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setNormedPaperOrientation( const CEnumDirection& i_eDirection )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_eDirection.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setNormedPaperOrientation",
        /* strAddInfo   */ strMthInArgs );

    if( m_drawingSize.normedPaperOrientation() != i_eDirection ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_drawingSize.setNormedPaperOrientation(i_eDirection);
        // Setting values according to orientation makes only sense for normed paper sizes.
        // For user defined paper sizes there is no orientation. The width might be
        // greater than the height or vice versa. The orientation will be stored if later
        // on a normed paper size is set but for now the values will not be adjusted.
        CEnumNormedPaperSize ePaperSize = m_drawingSize.normedPaperSize();
        if( ePaperSize != ENormedPaperSize::Undefined ) {
            m_pEdtImageMetricWidth->setValue(m_drawingSize.metricImageWidth().getVal());
            m_pEdtImageMetricHeight->setValue(m_drawingSize.metricImageHeight().getVal());
            updateImageSizeInPixels();
        }
        emit_drawingSizeChanged(m_drawingSize);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setScaleFactor( int i_iDividend, int i_iDivisor )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
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
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_drawingSize.setScaleFactor(i_iDividend, i_iDivisor);
        updateImageSizeInPixels();
        emit_drawingSizeChanged(m_drawingSize);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setImageSize(
    const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
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
    if( (i_physValWidth.unit() == Units.Length.pxX && i_physValHeight.unit() != Units.Length.pxY)
     || (i_physValWidth.unit() != Units.Length.pxX && i_physValHeight.unit() == Units.Length.pxY) ) {
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
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        if( i_physValWidth.unit() == Units.Length.pxX ) {
            updateImageSizeMetrics();
        }
        else {
            updateImageSizeInPixels();
        }
        updatePaperFormat();
        emit_drawingSizeChanged(m_drawingSize);
    }
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridVisible(bool i_bVisible)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = bool2Str(i_bVisible);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridVisible",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.isVisible() != i_bVisible) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_gridSettings.setVisible(i_bVisible);
        emit_gridSettingsChanged(m_gridSettings);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLineStyle(ELineStyle i_lineStyle)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = CEnumLineStyle(i_lineStyle).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLineStyle",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.lineStyle() != i_lineStyle) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_gridSettings.setLineStyle(i_lineStyle);
        emit_gridSettingsChanged(m_gridSettings);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridPenColor(const QColor& i_color)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_color.name();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridPenColor",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.penColor() != i_color) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_gridSettings.setPenColor(i_color);
        updateGridPenColorButtonIcon();
        emit_gridSettingsChanged(m_gridSettings);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridPenWidth(int i_iWidth_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString::number(i_iWidth_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridPenWidth",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.penWidth() != i_iWidth_px) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_gridSettings.setPenWidth(i_iWidth_px);
        updateGridPenColorButtonIcon();
        emit_gridSettingsChanged(m_gridSettings);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLabelsVisible(bool i_bVisible)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = bool2Str(i_bVisible);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsVisible",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.areLabelsVisible() != i_bVisible) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_gridSettings.setLabelsVisible(i_bVisible);
        emit_gridSettingsChanged(m_gridSettings);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLabelsFont(const QFont& i_fnt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_fnt.family();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsFont",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.labelsFont() != i_fnt) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_gridSettings.setLabelsFont(i_fnt);
        emit_gridSettingsChanged(m_gridSettings);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLabelsTextSize(ETextSize i_textSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = textSize2Str(i_textSize);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsTextSize",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.labelsTextSize() != i_textSize) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_gridSettings.setLabelsTextSize(i_textSize);
        emit_gridSettingsChanged(m_gridSettings);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLabelsTextColor(const QColor& i_color)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_color.name();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsTextColor",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.labelsTextColor() != i_color) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_gridSettings.setLabelsTextColor(i_color);
        updateGridLabelsTextColorButtonIcon();
        emit_gridSettingsChanged(m_gridSettings);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLabelsTextStyle(ETextStyle i_textStyle)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = CEnumTextStyle(i_textStyle).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsTextStyle",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.labelsTextStyle() != i_textStyle) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_gridSettings.setLabelsTextStyle(i_textStyle);
        updateGridLabelsTextStyleBoldButton();
        updateGridLabelsTextStyleItalicButton();
        emit_gridSettingsChanged(m_gridSettings);
    }
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::setGridLabelsTextEffect(const ETextEffect i_textEffect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = CEnumTextEffect(i_textEffect).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsTextEffect",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.labelsTextEffect() != i_textEffect) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_gridSettings.setLabelsTextEffect(i_textEffect);
        updateGridLabelsTextEffectUnderlineButton();
        updateGridLabelsTextEffectStrikeoutButton();
        emit_gridSettingsChanged(m_gridSettings);
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

    CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);

    CEnumDrawingDimensionUnit eDimensionUnit = m_drawingSize.dimensionUnit();

    m_pCmbDimensionUnit->setCurrentIndex(eDimensionUnit.enumeratorAsInt());
    m_pWdgtMetric->setVisible(eDimensionUnit == EDrawingDimensionUnit::Metric);
    m_pCmbImageMetricUnit->setEnabled(
        m_mode == EMode::Edit && eDimensionUnit == EDrawingDimensionUnit::Metric);
    m_pEdtImageMetricWidth->setReadOnly(
        m_mode != EMode::Edit || eDimensionUnit != EDrawingDimensionUnit::Metric);
    m_pEdtImageMetricHeight->setReadOnly(
        m_mode != EMode::Edit || eDimensionUnit != EDrawingDimensionUnit::Metric);
    m_pCmbImageMetricNormedPaperSizes->setEnabled(
        m_mode == EMode::Edit && eDimensionUnit == EDrawingDimensionUnit::Metric);
    m_pCmbImageMetricNormedPaperOrientation->setEnabled(
        m_mode == EMode::Edit && eDimensionUnit == EDrawingDimensionUnit::Metric);
    m_pCmbImageMetricScaleFactorDividend->setEnabled(
        m_mode == EMode::Edit && eDimensionUnit == EDrawingDimensionUnit::Metric);
    m_pCmbImageMetricScaleFactorDivisor->setEnabled(
        m_mode == EMode::Edit && eDimensionUnit == EDrawingDimensionUnit::Metric);
    m_pEdtImageSizeWidth_px->setReadOnly(
        m_mode != EMode::Edit || eDimensionUnit != EDrawingDimensionUnit::Pixels);
    m_pEdtImageSizeHeight_px->setReadOnly(
        m_mode != EMode::Edit || eDimensionUnit != EDrawingDimensionUnit::Pixels);

    updateImageSizeMetrics();
    updateImageSizeInPixels();
    updatePaperFormat();
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

    CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
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

    CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);

    m_pCmbImageMetricUnit->setCurrentText(m_drawingSize.metricUnit().symbol());

    m_pEdtImageMetricWidth->setUnit(m_drawingSize.metricUnit());
    #pragma message(__TODO__"setResolution depending on screen resolution")
    m_pEdtImageMetricWidth->setResolution(0.001);
    m_pEdtImageMetricWidth->setMaximum(100000);
    m_pEdtImageMetricWidth->setValue(m_drawingSize.metricImageWidth().getVal());

    m_pEdtImageMetricHeight->setUnit(m_drawingSize.metricUnit());
    #pragma message(__TODO__"setResolution depending on screen resolution")
    m_pEdtImageMetricHeight->setResolution(0.001);
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

    CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);

    CEnumNormedPaperSize ePaperSize = m_drawingSize.normedPaperSize();
    CEnumDirection eDirection = m_drawingSize.normedPaperOrientation();

    if( ePaperSize != ENormedPaperSize::Undefined ) {
        m_pCmbImageMetricNormedPaperSizes->setCurrentText(ePaperSize.toString(EEnumEntryAliasStrSymbol));
        m_pLblImageMetricNormedPaperOrientation->setVisible(true);
        m_pCmbImageMetricNormedPaperOrientation->setVisible(true);
        m_pCmbImageMetricNormedPaperOrientation->clear();
        for( CEnumDirection eDirection = 0; eDirection < CEnumDirection::count(); ++eDirection ) {
            if( eDirection != EDirection::Undefined) {
                m_pCmbImageMetricNormedPaperOrientation->addItem(
                    eDirection.toString(EEnumEntryAliasStrText));
            }
        }
        m_pCmbImageMetricNormedPaperOrientation->setCurrentText(eDirection.toString());
    }
    else {
        m_pCmbImageMetricNormedPaperSizes->setCurrentText(ePaperSize.toString(EEnumEntryAliasStrText));
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

    CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);

    m_pChkGridLinesVisible->setChecked(m_gridSettings.isVisible());
    m_pEdtGridPenWidth->setValue(m_gridSettings.penWidth());
    m_pChkGridScaleLabelsVisible->setChecked(m_gridSettings.areLabelsVisible());
    int idx = m_pCmbGridLineStyle->findData(m_gridSettings.lineStyle().enumeratorAsInt());
    if (idx >= 0) {
        m_pCmbGridLineStyle->setCurrentIndex(idx);
    }
    m_pCmbGridScaleLabelsFont->setCurrentFont(m_gridSettings.labelsFont());
    m_pCmbGridScaleLabelsFontSize->setCurrentIndex(m_gridSettings.labelsTextSize());

    updateGridPenColorButtonIcon();
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

    pen.setColor(m_gridSettings.penColor());
    pen.setWidth(m_gridSettings.penWidth());

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
            m_pModelGridLineStyles->appendRow(pLineStyleItem);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        mthTracer.setMethodReturn(qSize2Str(iconSize));
    }
    return iconSize;
}

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::updateGridPenColorButtonIcon()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateGridPenColorButtonIcon",
        /* strAddInfo   */ "" );

    QPainter painter(m_pPxmBtnGridPenColor);

    painter.setPen(m_gridSettings.penColor());
    painter.setBrush(Qt::NoBrush);
    painter.drawLine(m_lineBtnGridPenColor1);
    painter.drawLine(m_lineBtnGridPenColor2);
    painter.drawLine(m_lineBtnGridPenColor3);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_gridSettings.penColor());
    painter.drawRect(m_rctBtnGridPenColor);

    m_pBtnGridPenColor->setIcon(*m_pPxmBtnGridPenColor);
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

/*==============================================================================
protected: // instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDrawingViewProperties::emit_drawingSizeChanged(const ZS::Draw::CDrawingSize& i_size)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
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
void CWdgtDrawingViewProperties::emit_gridSettingsChanged(const ZS::Draw::CDrawGridSettings& i_settings)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
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
        + ", ButtonsEnabled (Edit: " + bool2Str(m_pBtnEdit->isEnabled()) + ")";
    mthTracer.trace(strMthLog);
}
