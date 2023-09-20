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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLineGeometryPropertiesWdgt.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLine.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSPhysValGUI/ZSPhysValWdgtEditPhysVal.h"
#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#if QT_VERSION < 0x050000
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qspinbox.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
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
class CWdgtGraphObjLineGeometryProperties : public CWdgtFormatGraphObjs
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjLineGeometryProperties::CWdgtGraphObjLineGeometryProperties(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strParentClassName,
    const QString& i_strObjName,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtGraphObjPropertiesAbstract(
        i_pDrawingScene, i_strParentClassName + "::" + ClassName(), i_strObjName, i_pWdgtParent),
    m_pWdgtHeadline(nullptr),
    m_pLytWdgtHeadline(nullptr),
    m_pxmBtnDown(":/ZS/Button/ButtonArrowDown.png"),
    m_pxmBtnUp(":/ZS/Button/ButtonArrowUp.png"),
    m_pBtnCollapse(nullptr),
    m_pLblHeadlineIcon(nullptr),
    m_pLblHeadline(nullptr),
    m_pSepHeadline(nullptr),
    m_pWdgtGeometry(nullptr),
    m_pLytWdgtGeometry(nullptr),
    // Geometry in Metric System
    m_pWdgtMetric(nullptr),
    m_pLytWdgtMetric(nullptr),
    m_pWdgtSepLineMetricGeometry(nullptr),
    m_pLytSepLineMetricGeometry(nullptr),
    m_pLblSepLineMetricGeometry(nullptr),
    m_pSepLineMetricGeometry(nullptr),
    m_pLytLineMetricPt1(nullptr),
    m_pLblMetricPt1(nullptr),
    m_pLblMetricPt1X(nullptr),
    m_pEdtMetricPt1X(nullptr),
    m_pLblMetricPt1Y(nullptr),
    m_pEdtMetricPt1Y(nullptr),
    m_pLytLineMetricPt2(nullptr),
    m_pLblMetricPt2(nullptr),
    m_pLblMetricPt2X(nullptr),
    m_pEdtMetricPt2X(nullptr),
    m_pLblMetricPt2Y(nullptr),
    m_pEdtMetricPt2Y(nullptr),
    m_pLytLineMetricLengthAngle(nullptr),
    m_pLblMetricLength(nullptr),
    m_pEdtMetricLength(nullptr),
    m_pLblMetricAngle(nullptr),
    m_pEdtMetricAngle(nullptr),
    m_pLytLineMetricSize(nullptr),
    m_pLblMetricWidth(nullptr),
    m_pEdtMetricWidth(nullptr),
    m_pLblMetricHeight(nullptr),
    m_pEdtMetricHeight(nullptr),
    // Geometry in Pixels
    m_pWdgtPixels(nullptr),
    m_pLytWdgtPixels(nullptr),
    m_pWdgtSepLinePixelsGeometry(nullptr),
    m_pLytSepLinePixelsGeometry(nullptr),
    m_pLblSepLinePixelsGeometry(nullptr),
    m_pSepLinePixelsGeometry(nullptr),
    m_pLytLinePixelsPt1(nullptr),
    m_pLblPixelsPt1(nullptr),
    m_pLblPixelsPt1X(nullptr),
    m_pEdtPixelsPt1X(nullptr),
    m_pLblPixelsPt1Y(nullptr),
    m_pEdtPixelsPt1Y(nullptr),
    m_pLytLinePixelsPt2(nullptr),
    m_pLblPixelsPt2(nullptr),
    m_pLblPixelsPt2X(nullptr),
    m_pEdtPixelsPt2X(nullptr),
    m_pLblPixelsPt2Y(nullptr),
    m_pEdtPixelsPt2Y(nullptr),
    m_pLytLinePixelsLengthAngle(nullptr),
    m_pLblPixelsLength(nullptr),
    m_pEdtPixelsLength(nullptr),
    m_pLblPixelsAngle(nullptr),
    m_pEdtPixelsAngle(nullptr),
    m_pLytLinePixelsPos(nullptr),
    m_pLblPixelsPos(nullptr),
    m_pLblPixelsPosX(nullptr),
    m_pEdtPixelsPosX(nullptr),
    m_pLblPixelsPosY(nullptr),
    m_pEdtPixelsPosY(nullptr),
    m_pLytLinePixelsSize(nullptr),
    m_pLblPixelsWidth(nullptr),
    m_pEdtPixelsWidth(nullptr),
    m_pLblPixelsHeight(nullptr),
    m_pEdtPixelsHeight(nullptr)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // We need to fill the edit controls if the graphical object emits the geometryChanged signal:
    m_bContentUpdateOnGeometryChanged = true;

    const QVector<int> ariClmWidths = {
         40,  // Point1
         20,  // X:
         120, // <X-Value>
         30,  // Y:, Angle:
         120  // <Y-Value>
    };
    int cxClmSpacing = 30;
    int idxClm = 0;

    setMinimumWidth(560);

    // <Widget> Headline
    //==================

    m_pWdgtHeadline = new QWidget();
    m_pLytWdgtHeadline = new QHBoxLayout();
    m_pLytWdgtHeadline->setContentsMargins(0, 0, 0, 0);
    m_pWdgtHeadline->setLayout(m_pLytWdgtHeadline);
    m_pLyt->addWidget(m_pWdgtHeadline);

    m_pBtnCollapse = new QPushButton();
    m_pBtnCollapse->setIcon(m_pxmBtnDown);
    m_pLytWdgtHeadline->addWidget(m_pBtnCollapse);

    QObject::connect(
        m_pBtnCollapse, &QPushButton::clicked,
        this, &CWdgtGraphObjLineGeometryProperties::onBtnCollapseClicked);

    QPixmap pxmHeadline(":/ZS/Draw/DrawToolGeometry16x16.png");
    m_pLblHeadlineIcon = new QLabel();
    m_pLblHeadlineIcon->setPixmap(pxmHeadline);
    m_pLytWdgtHeadline->addWidget(m_pLblHeadlineIcon);

    m_pLblHeadline = new QLabel("Geometry");
    QFont fntHeadline = m_pLblHeadline->font();
    fntHeadline.setPointSize(fntHeadline.pointSize() + 2);
    m_pLblHeadline->setFont(fntHeadline);
    m_pLytWdgtHeadline->addWidget(m_pLblHeadline);

    m_pSepHeadline = new CSepLine();
    m_pLytWdgtHeadline->addWidget(m_pSepHeadline, 1);

    m_pLytWdgtHeadline->addStretch();

    // <Widget> Geometry
    //==================

    m_pWdgtGeometry = new QWidget();
    m_pLytWdgtGeometry = new QVBoxLayout();
    m_pLytWdgtGeometry->setContentsMargins(0, 0, 0, 0);
    m_pWdgtGeometry->setLayout(m_pLytWdgtGeometry);
    m_pLyt->addWidget(m_pWdgtGeometry);
    if (m_drawingSize.dimensionUnit() != EDrawingDimensionUnit::Metric) {
        m_pWdgtGeometry->hide();
    }

    // <Section> Geometry in Metric System
    //====================================

    m_pWdgtMetric = new QWidget();
    m_pLytWdgtMetric = new QVBoxLayout();
    m_pLytWdgtMetric->setContentsMargins(0, 0, 0, 0);
    m_pWdgtMetric->setLayout(m_pLytWdgtMetric);
    m_pLytWdgtGeometry->addWidget(m_pWdgtMetric);
    m_pWdgtMetric->hide();

    m_pWdgtSepLineMetricGeometry = new QWidget();
    m_pLytSepLineMetricGeometry = new QHBoxLayout();
    m_pLytSepLineMetricGeometry->setContentsMargins(0, 0, 0, 0);
    m_pWdgtSepLineMetricGeometry->setLayout(m_pLytSepLineMetricGeometry);
    m_pLytWdgtMetric->addWidget(m_pWdgtSepLineMetricGeometry);
    m_pLblSepLineMetricGeometry = new QLabel("Metric Values");
    m_pLytSepLineMetricGeometry->addWidget(m_pLblSepLineMetricGeometry);
    m_pSepLineMetricGeometry = new CSepLine(10);
    m_pLytSepLineMetricGeometry->addWidget(m_pSepLineMetricGeometry, 1);

    // <Line> Metric Point 1
    //----------------------

    idxClm = 0;

    m_pLytLineMetricPt1 = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytLineMetricPt1);

    m_pLblMetricPt1 = new QLabel("Point 1");
    m_pLblMetricPt1->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLineMetricPt1->addWidget(m_pLblMetricPt1);

    m_pLblMetricPt1X = new QLabel("X: ");
    m_pLblMetricPt1X->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLineMetricPt1->addWidget(m_pLblMetricPt1X);
    m_pEdtMetricPt1X = new CWdgtEditPhysVal();
    m_pEdtMetricPt1X->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtMetricPt1X->setReadOnly(true);
    m_pEdtMetricPt1X->setMinimum(0.0);
    m_pLytLineMetricPt1->addWidget(m_pEdtMetricPt1X);
    m_pLytLineMetricPt1->addSpacing(cxClmSpacing);
    QObject::connect(
        m_pEdtMetricPt1X, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricPt1XValueChanged);

    m_pLblMetricPt1Y = new QLabel("Y: ");
    m_pLblMetricPt1Y->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLineMetricPt1->addWidget(m_pLblMetricPt1Y);
    m_pEdtMetricPt1Y = new CWdgtEditPhysVal();
    m_pEdtMetricPt1Y->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtMetricPt1Y->setReadOnly(true);
    m_pEdtMetricPt1Y->setMinimum(0.0);
    m_pLytLineMetricPt1->addWidget(m_pEdtMetricPt1Y);
    m_pLytLineMetricPt1->addStretch();
    QObject::connect(
        m_pEdtMetricPt1Y, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricPt1YValueChanged);

    // <Line> Metric Point 2
    //----------------------

    idxClm = 0;

    m_pLytLineMetricPt2 = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytLineMetricPt2);

    m_pLblMetricPt2 = new QLabel("Point 2");
    m_pLblMetricPt2->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLineMetricPt2->addWidget(m_pLblMetricPt2);

    m_pLblMetricPt2X = new QLabel("X: ");
    m_pLblMetricPt2X->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLineMetricPt2->addWidget(m_pLblMetricPt2X);
    m_pEdtMetricPt2X = new CWdgtEditPhysVal();
    m_pEdtMetricPt2X->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtMetricPt2X->setReadOnly(true);
    m_pEdtMetricPt2X->setMinimum(0.0);
    m_pLytLineMetricPt2->addWidget(m_pEdtMetricPt2X);
    m_pLytLineMetricPt2->addSpacing(cxClmSpacing);
    QObject::connect(
        m_pEdtMetricPt2X, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricPt2XValueChanged);

    m_pLblMetricPt2Y = new QLabel("Y: ");
    m_pLblMetricPt2Y->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLineMetricPt2->addWidget(m_pLblMetricPt2Y);
    m_pEdtMetricPt2Y = new CWdgtEditPhysVal();
    m_pEdtMetricPt2Y->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtMetricPt2Y->setReadOnly(true);
    m_pEdtMetricPt2Y->setMinimum(0.0);
    m_pLytLineMetricPt2->addWidget(m_pEdtMetricPt2Y);
    m_pLytLineMetricPt2->addStretch();
    QObject::connect(
        m_pEdtMetricPt2Y, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricPt2YValueChanged);

    // <Line> Width and Angle
    //------------------------

    idxClm = 0;

    m_pLytLineMetricLengthAngle = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytLineMetricLengthAngle);

    m_pLblMetricLength = new QLabel("Length: ");
    m_pLblMetricLength->setFixedWidth(
        ariClmWidths[idxClm++] + ariClmWidths[idxClm++]
      + m_pLytLineMetricLengthAngle->contentsMargins().left()
      + m_pLytLineMetricLengthAngle->contentsMargins().right()
      + m_pLytLineMetricLengthAngle->spacing());
    m_pLytLineMetricLengthAngle->addWidget(m_pLblMetricLength);
    m_pEdtMetricLength = new CWdgtEditPhysVal();
    m_pEdtMetricLength->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtMetricLength->setReadOnly(true);
    m_pEdtMetricLength->setMinimum(0.0);
    m_pLytLineMetricLengthAngle->addWidget(m_pEdtMetricLength);
    m_pLytLineMetricLengthAngle->addSpacing(cxClmSpacing);
    QObject::connect(
        m_pEdtMetricLength, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricLengthValueChanged);

    m_pLblMetricAngle = new QLabel("Angle: ");
    m_pLblMetricAngle->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLineMetricLengthAngle->addWidget(m_pLblMetricAngle);
    m_pEdtMetricAngle = new CWdgtEditPhysVal();
    m_pEdtMetricAngle->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtMetricAngle->setReadOnly(true);
    m_pEdtMetricAngle->setUnit(Units.Angle.Degree);
    m_pEdtMetricAngle->setResolution(0.1);
    m_pEdtMetricAngle->setMinimum(-360.0);
    m_pEdtMetricAngle->setMaximum(360.0);
    m_pLytLineMetricLengthAngle->addWidget(m_pEdtMetricAngle);
    m_pLytLineMetricLengthAngle->addStretch();
    QObject::connect(
        m_pEdtMetricAngle, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricAngleValueChanged);

    // <Line> Size
    //------------

    idxClm = 0;

    m_pLytLineMetricSize = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytLineMetricSize);

    m_pLblMetricWidth = new QLabel("Width: ");
    m_pLblMetricWidth->setFixedWidth(
        ariClmWidths[idxClm++] + ariClmWidths[idxClm++]
      + m_pLytLineMetricSize->contentsMargins().left()
      + m_pLytLineMetricSize->contentsMargins().right()
      + m_pLytLineMetricSize->spacing());
    m_pLytLineMetricSize->addWidget(m_pLblMetricWidth);
    m_pEdtMetricWidth = new CWdgtEditPhysVal();
    m_pEdtMetricWidth->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtMetricWidth->setReadOnly(true);
    m_pEdtMetricWidth->setMinimum(0.0);
    m_pLytLineMetricSize->addWidget(m_pEdtMetricWidth);
    m_pLytLineMetricSize->addSpacing(cxClmSpacing);
    QObject::connect(
        m_pEdtMetricWidth, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricWidthValueChanged);

    m_pLblMetricHeight = new QLabel("Height: ");
    m_pLblMetricHeight->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLineMetricSize->addWidget(m_pLblMetricHeight);
    m_pEdtMetricHeight = new CWdgtEditPhysVal();
    m_pEdtMetricHeight->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtMetricHeight->setReadOnly(true);
    m_pEdtMetricHeight->setMinimum(0.0);
    m_pLytLineMetricSize->addWidget(m_pEdtMetricHeight);
    m_pLytLineMetricSize->addStretch();
    QObject::connect(
        m_pEdtMetricHeight, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricHeightValueChanged);

    // <Section> Geometry in Pixels
    //=============================

    m_pWdgtPixels = new QWidget();
    m_pLytWdgtPixels = new QVBoxLayout();
    m_pLytWdgtPixels->setContentsMargins(0, 0, 0, 0);
    m_pWdgtPixels->setLayout(m_pLytWdgtPixels);
    m_pLytWdgtGeometry->addWidget(m_pWdgtPixels);
    m_pWdgtMetric->hide();

    m_pWdgtSepLinePixelsGeometry = new QWidget();
    m_pLytSepLinePixelsGeometry = new QHBoxLayout();
    m_pLytSepLinePixelsGeometry->setContentsMargins(0, 0, 0, 0);
    m_pWdgtSepLinePixelsGeometry->setLayout(m_pLytSepLinePixelsGeometry);
    m_pLytWdgtPixels->addWidget(m_pWdgtSepLinePixelsGeometry);
    m_pLblSepLinePixelsGeometry = new QLabel("Pixel Values");
    m_pLytSepLinePixelsGeometry->addWidget(m_pLblSepLinePixelsGeometry);
    m_pSepLineMetricGeometry = new CSepLine(10);
    m_pLytSepLinePixelsGeometry->addWidget(m_pSepLineMetricGeometry, 1);
    if (m_drawingSize.dimensionUnit() == EDrawingDimensionUnit::Pixels) {
        m_pWdgtSepLinePixelsGeometry->hide();
    }

    // <Line> Pixels Point 1
    //----------------------

    idxClm = 0;

    m_pLytLinePixelsPt1 = new QHBoxLayout();
    m_pLytWdgtPixels->addLayout(m_pLytLinePixelsPt1);

    m_pLblPixelsPt1 = new QLabel("Point 1");
    m_pLblPixelsPt1->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLinePixelsPt1->addWidget(m_pLblPixelsPt1);

    m_pLblPixelsPt1X = new QLabel("X: ");
    m_pLblPixelsPt1X->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLinePixelsPt1->addWidget(m_pLblPixelsPt1X);
    m_pEdtPixelsPt1X = new QSpinBox();
    m_pEdtPixelsPt1X->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtPixelsPt1X->setSuffix(" " + Units.Length.px.symbol());
    m_pEdtPixelsPt1X->setSingleStep(1.0);
    m_pEdtPixelsPt1X->setMinimum(0.0);
    m_pLytLinePixelsPt1->addWidget(m_pEdtPixelsPt1X);
    m_pLytLinePixelsPt1->addSpacing(cxClmSpacing);
    QObject::connect(
        m_pEdtPixelsPt1X, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsPt1XValueChanged);

    m_pLblPixelsPt1Y = new QLabel("Y: ");
    m_pLblPixelsPt1Y->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLinePixelsPt1->addWidget(m_pLblPixelsPt1Y);
    m_pEdtPixelsPt1Y = new QSpinBox();
    m_pEdtPixelsPt1Y->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtPixelsPt1Y->setSuffix(" " + Units.Length.px.symbol());
    m_pEdtPixelsPt1Y->setSingleStep(1.0);
    m_pEdtPixelsPt1Y->setMinimum(0.0);
    m_pEdtPixelsPt1Y->setMaximum(m_drawingSize.imageHeightInPixels());
    m_pLytLinePixelsPt1->addWidget(m_pEdtPixelsPt1Y);
    m_pLytLinePixelsPt1->addStretch();
    QObject::connect(
        m_pEdtPixelsPt1Y, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsPt1YValueChanged);

    // <Line> Pixels Point 2
    //----------------------

    idxClm = 0;

    m_pLytLinePixelsPt2 = new QHBoxLayout();
    m_pLytWdgtPixels->addLayout(m_pLytLinePixelsPt2);

    m_pLblPixelsPt2 = new QLabel("Point 2");
    m_pLblPixelsPt2->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLinePixelsPt2->addWidget(m_pLblPixelsPt2);

    m_pLblPixelsPt2X = new QLabel("X: ");
    m_pLblPixelsPt2X->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLinePixelsPt2->addWidget(m_pLblPixelsPt2X);
    m_pEdtPixelsPt2X = new QSpinBox();
    m_pEdtPixelsPt2X->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtPixelsPt2X->setSuffix(" " + Units.Length.px.symbol());
    m_pEdtPixelsPt2X->setSingleStep(1.0);
    m_pEdtPixelsPt2X->setMinimum(0.0);
    m_pLytLinePixelsPt2->addWidget(m_pEdtPixelsPt2X);
    m_pLytLinePixelsPt2->addSpacing(cxClmSpacing);
    QObject::connect(
        m_pEdtPixelsPt2X, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsPt2XValueChanged);

    m_pLblPixelsPt2Y = new QLabel("Y: ");
    m_pLblPixelsPt2Y->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLinePixelsPt2->addWidget(m_pLblPixelsPt2Y);
    m_pEdtPixelsPt2Y = new QSpinBox();
    m_pEdtPixelsPt2Y->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtPixelsPt2Y->setSuffix(" " + Units.Length.px.symbol());
    m_pEdtPixelsPt2Y->setSingleStep(1.0);
    m_pEdtPixelsPt2Y->setMinimum(0.0);
    m_pEdtPixelsPt2Y->setMaximum(m_drawingSize.imageHeightInPixels());
    m_pLytLinePixelsPt2->addWidget(m_pEdtPixelsPt2Y);
    m_pLytLinePixelsPt2->addStretch();
    QObject::connect(
        m_pEdtPixelsPt2Y, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsPt2YValueChanged);

    // <Line> Length and Angle
    //------------------------

    idxClm = 0;

    m_pLytLinePixelsLengthAngle = new QHBoxLayout();
    m_pLytWdgtPixels->addLayout(m_pLytLinePixelsLengthAngle);

    m_pLblPixelsLength = new QLabel("Length: ");
    m_pLblPixelsLength->setFixedWidth(
        ariClmWidths[idxClm++] + ariClmWidths[idxClm++]
      + m_pLytLinePixelsLengthAngle->contentsMargins().left()
      + m_pLytLinePixelsLengthAngle->contentsMargins().right()
      + m_pLytLinePixelsLengthAngle->spacing());
    m_pLytLinePixelsLengthAngle->addWidget(m_pLblPixelsLength);
    m_pEdtPixelsLength = new QSpinBox();
    m_pEdtPixelsLength->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtPixelsLength->setReadOnly(true);
    m_pEdtPixelsLength->setSuffix(" " + Units.Length.px.symbol());
    m_pEdtPixelsLength->setSingleStep(0.1);
    m_pEdtPixelsLength->setMinimum(0.0);
    m_pLytLinePixelsLengthAngle->addWidget(m_pEdtPixelsLength);
    m_pLytLinePixelsLengthAngle->addSpacing(cxClmSpacing);
    QObject::connect(
        m_pEdtPixelsLength, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsLengthValueChanged);

    m_pLblPixelsAngle = new QLabel("Angle: ");
    m_pLblPixelsAngle->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLinePixelsLengthAngle->addWidget(m_pLblPixelsAngle);
    m_pEdtPixelsAngle = new CWdgtEditPhysVal();
    m_pEdtPixelsAngle->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtPixelsAngle->setReadOnly(true);
    m_pEdtPixelsAngle->setUnit(Units.Angle.Degree);
    m_pEdtPixelsAngle->setResolution(0.1);
    m_pEdtPixelsAngle->setMinimum(-360.0);
    m_pEdtPixelsAngle->setMaximum(360.0);
    m_pLytLinePixelsLengthAngle->addWidget(m_pEdtPixelsAngle);
    m_pLytLinePixelsLengthAngle->addStretch();
    QObject::connect(
        m_pEdtPixelsAngle, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsAngleValueChanged);

    // <Line> Center Position
    //-----------------------

    idxClm = 0;

    m_pLytLinePixelsPos = new QHBoxLayout();
    m_pLytWdgtPixels->addLayout(m_pLytLinePixelsPos);

    m_pLblPixelsPos = new QLabel("Center ");
    m_pLblPixelsPos->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLinePixelsPos->addWidget(m_pLblPixelsPos);

    m_pLblPixelsPosX = new QLabel("X: ");
    m_pLblPixelsPosX->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLinePixelsPos->addWidget(m_pLblPixelsPosX);
    m_pEdtPixelsPosX = new QSpinBox();
    m_pEdtPixelsPosX->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtPixelsPosX->setReadOnly(true);
    m_pEdtPixelsPosX->setSuffix(" " + Units.Length.px.symbol());
    m_pEdtPixelsPosX->setSingleStep(0.1);
    m_pEdtPixelsPosX->setMinimum(0.0);
    m_pLytLinePixelsPos->addWidget(m_pEdtPixelsPosX);
    m_pLytLinePixelsPos->addSpacing(cxClmSpacing);
    QObject::connect(
        m_pEdtPixelsPosX, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsPosXValueChanged);

    m_pLblPixelsPosY = new QLabel("Y: ");
    m_pLblPixelsPosY->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLinePixelsPos->addWidget(m_pLblPixelsPosY);
    m_pEdtPixelsPosY = new QSpinBox();
    m_pEdtPixelsPosY->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtPixelsPosY->setReadOnly(true);
    m_pEdtPixelsPosY->setSuffix(" " + Units.Length.px.symbol());
    m_pEdtPixelsPosY->setSingleStep(0.1);
    m_pEdtPixelsPosY->setMinimum(0.0);
    m_pLytLinePixelsPos->addWidget(m_pEdtPixelsPosY);
    m_pLytLinePixelsPos->addStretch();
    QObject::connect(
        m_pEdtPixelsPosY, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsPosYValueChanged);

    // <Line> Size
    //------------

    idxClm = 0;

    m_pLytLinePixelsSize = new QHBoxLayout();
    m_pLytWdgtPixels->addLayout(m_pLytLinePixelsSize);

    m_pLblPixelsWidth = new QLabel("Width: ");
    m_pLblPixelsWidth->setFixedWidth(
        ariClmWidths[idxClm++] + ariClmWidths[idxClm++]
      + m_pLytLinePixelsSize->contentsMargins().left()
      + m_pLytLinePixelsSize->contentsMargins().right()
      + m_pLytLinePixelsSize->spacing());
    m_pLytLinePixelsSize->addWidget(m_pLblPixelsWidth);
    m_pEdtPixelsWidth = new QSpinBox();
    m_pEdtPixelsWidth->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtPixelsWidth->setSuffix(" " + Units.Length.px.symbol());
    m_pEdtPixelsWidth->setSingleStep(1.0);
    m_pEdtPixelsWidth->setMinimum(0.0);
    m_pLytLinePixelsSize->addWidget(m_pEdtPixelsWidth);
    m_pLytLinePixelsSize->addSpacing(cxClmSpacing);
    QObject::connect(
        m_pEdtPixelsWidth, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsWidthValueChanged);

    m_pLblPixelsHeight = new QLabel("Height: ");
    m_pLblPixelsHeight->setFixedWidth(ariClmWidths[idxClm++]);
    m_pLytLinePixelsSize->addWidget(m_pLblPixelsHeight);
    m_pEdtPixelsHeight = new QSpinBox();
    m_pEdtPixelsHeight->setFixedWidth(ariClmWidths[idxClm++]);
    m_pEdtPixelsHeight->setSuffix(" " + Units.Length.px.symbol());
    m_pEdtPixelsHeight->setSingleStep(1.0);
    m_pEdtPixelsHeight->setMinimum(0.0);
    m_pLytLinePixelsSize->addWidget(m_pEdtPixelsHeight);
    m_pLytLinePixelsSize->addStretch();
    QObject::connect(
        m_pEdtPixelsHeight, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsHeightValueChanged);

    // Update controls depending on drawing size (dimension unit etc.)
    //================================================================

    updateDrawingSize();

} // ctor

//------------------------------------------------------------------------------
CWdgtGraphObjLineGeometryProperties::~CWdgtGraphObjLineGeometryProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pWdgtHeadline = nullptr;
    m_pLytWdgtHeadline = nullptr;
    //m_pxmBtnDown;
    //m_pxmBtnUp;
    m_pBtnCollapse = nullptr;
    m_pLblHeadlineIcon = nullptr;
    m_pLblHeadline = nullptr;
    m_pSepHeadline = nullptr;
    m_pWdgtGeometry = nullptr;
    m_pLytWdgtGeometry = nullptr;
    // Geometry in Metric System
    m_pWdgtMetric = nullptr;
    m_pLytWdgtMetric = nullptr;
    m_pWdgtSepLineMetricGeometry = nullptr;
    m_pLytSepLineMetricGeometry = nullptr;
    m_pLblSepLineMetricGeometry = nullptr;
    m_pSepLineMetricGeometry = nullptr;
    m_pLytLineMetricPt1 = nullptr;
    m_pLblMetricPt1 = nullptr;
    m_pLblMetricPt1X = nullptr;
    m_pEdtMetricPt1X = nullptr;
    m_pLblMetricPt1Y = nullptr;
    m_pEdtMetricPt1Y = nullptr;
    m_pLytLineMetricPt2 = nullptr;
    m_pLblMetricPt2 = nullptr;
    m_pLblMetricPt2X = nullptr;
    m_pEdtMetricPt2X = nullptr;
    m_pLblMetricPt2Y = nullptr;
    m_pEdtMetricPt2Y = nullptr;
    m_pLytLineMetricLengthAngle = nullptr;
    m_pLblMetricLength = nullptr;
    m_pEdtMetricLength = nullptr;
    m_pLblMetricAngle = nullptr;
    m_pEdtMetricAngle = nullptr;
    m_pLytLineMetricSize = nullptr;
    m_pLblMetricWidth = nullptr;
    m_pEdtMetricWidth = nullptr;
    m_pLblMetricHeight = nullptr;
    m_pEdtMetricHeight = nullptr;
    // Geometry in Pixels
    m_pWdgtPixels = nullptr;
    m_pLytWdgtPixels = nullptr;
    m_pWdgtSepLinePixelsGeometry = nullptr;
    m_pLytSepLinePixelsGeometry = nullptr;
    m_pLblSepLinePixelsGeometry = nullptr;
    m_pSepLinePixelsGeometry = nullptr;
    m_pLytLinePixelsPt1 = nullptr;
    m_pLblPixelsPt1 = nullptr;
    m_pLblPixelsPt1X = nullptr;
    m_pEdtPixelsPt1X = nullptr;
    m_pLblPixelsPt1Y = nullptr;
    m_pEdtPixelsPt1Y = nullptr;
    m_pLytLinePixelsPt2 = nullptr;
    m_pLblPixelsPt2 = nullptr;
    m_pLblPixelsPt2X = nullptr;
    m_pEdtPixelsPt2X = nullptr;
    m_pLblPixelsPt2Y = nullptr;
    m_pEdtPixelsPt2Y = nullptr;
    m_pLytLinePixelsLengthAngle = nullptr;
    m_pLblPixelsLength = nullptr;
    m_pEdtPixelsLength = nullptr;
    m_pLblPixelsAngle = nullptr;
    m_pEdtPixelsAngle = nullptr;
    m_pLytLinePixelsPos = nullptr;
    m_pLblPixelsPos = nullptr;
    m_pLblPixelsPosX = nullptr;
    m_pEdtPixelsPosX = nullptr;
    m_pLblPixelsPosY = nullptr;
    m_pEdtPixelsPosY = nullptr;
    m_pLytLinePixelsSize = nullptr;
    m_pLblPixelsWidth = nullptr;
    m_pEdtPixelsWidth = nullptr;
    m_pLblPixelsHeight = nullptr;
    m_pEdtPixelsHeight = nullptr;
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtGraphObjLineGeometryProperties::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ "" );

    bool bHasChanges = false;

    if (m_pGraphObj != nullptr) {
        CGraphObjLine* pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
        CPhysVal physValMetricPt1X = m_pEdtMetricPt1X->value();
        //if (pGraphObjLine->() != physValMetricPt1X) {
        //    bHasChanges = true;
        //}
        QLineF lin = pGraphObjLine->getLine().toQLineF(Units.Length.px);
        if (ZS::System::Math::round2Nearest(lin.p1().x(), 0) != m_pEdtPixelsPt1X->value()) {
            bHasChanges = true;
        }
        if (ZS::System::Math::round2Nearest(lin.p1().y(), 0) != m_pEdtPixelsPt1Y->value()) {
            bHasChanges = true;
        }
        if (ZS::System::Math::round2Nearest(lin.p2().x(), 0) != m_pEdtPixelsPt2X->value()) {
            bHasChanges = true;
        }
        if (ZS::System::Math::round2Nearest(lin.p2().y(), 0) != m_pEdtPixelsPt2Y->value()) {
            bHasChanges = true;
        }
        if (ZS::System::Math::round2Nearest(lin.length(), 1) != m_pEdtPixelsLength->value()) {
            bHasChanges = true;
        }
        if (ZS::System::Math::round2Nearest(lin.angle(), 1) != m_pEdtPixelsAngle->value().getVal()) {
            bHasChanges = true;
        }
        QPointF ptPos = pGraphObjLine->getPos().toQPointF(Units.Length.px);
        if (ZS::System::Math::round2Nearest(ptPos.x(), 1) != m_pEdtPixelsPosX->value()) {
            bHasChanges = true;
        }
        if (ZS::System::Math::round2Nearest(ptPos.y(), 1) != m_pEdtPixelsPosY->value()) {
            bHasChanges = true;
        }
        QSizeF size = pGraphObjLine->getSize().toQSizeF(Units.Length.px);
        if (ZS::System::Math::round2Nearest(size.width(), 0) != m_pEdtPixelsWidth->value()) {
            bHasChanges = true;
        }
        if (ZS::System::Math::round2Nearest(size.height(), 0) != m_pEdtPixelsHeight->value()) {
            bHasChanges = true;
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

/*==============================================================================
protected slots: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onDrawingSceneDrawingSizeChanged(const CDrawingSize& i_drawingSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_drawingSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneDrawingSizeChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawingSize != i_drawingSize) {
        m_drawingSize = i_drawingSize;
        updateDrawingSize();
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onBtnCollapseClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnCollapseClicked",
        /* strAddInfo   */ "" );

    if (m_pWdgtGeometry->isHidden()) {
        m_pBtnCollapse->setIcon(m_pxmBtnUp);
        m_pWdgtGeometry->show();
    }
    else {
        m_pBtnCollapse->setIcon(m_pxmBtnDown);
        m_pWdgtGeometry->hide();
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricPt1XValueChanged(const ZS::PhysVal::CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtMetricPt1XValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricPt1YValueChanged(const ZS::PhysVal::CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtMetricPt1YValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricPt2XValueChanged(const ZS::PhysVal::CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtMetricPt2XValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricPt2YValueChanged(const ZS::PhysVal::CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtMetricPt2YValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricLengthValueChanged(const ZS::PhysVal::CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtMetricLengthValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricAngleValueChanged(const ZS::PhysVal::CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtMetricAngleValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricWidthValueChanged(const ZS::PhysVal::CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtMetricWidthValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricHeightValueChanged(const ZS::PhysVal::CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtMetricHeightValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsPt1XValueChanged(int i_iVal_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iVal_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtPixelsPt1XValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsPt1YValueChanged(int i_iVal_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iVal_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtPixelsPt1YValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsPt2XValueChanged(int i_iVal_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iVal_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtPixelsPt2XValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsPt2YValueChanged(int i_iVal_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iVal_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtPixelsPt2YValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsLengthValueChanged(int i_iVal_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iVal_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtPixelsLengthValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsAngleValueChanged(const ZS::PhysVal::CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtPixelsAngleValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsPosXValueChanged(int i_iVal_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iVal_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtPixelsPosXValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsPosYValueChanged(int i_iVal_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iVal_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtPixelsPosYValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsWidthValueChanged(int i_iVal_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iVal_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtPixelsWidthValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsHeightValueChanged(int i_iVal_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iVal_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtPixelsHeightValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        emit_contentChanged();
    }
}

/*==============================================================================
protected: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::fillEditControls()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "fillEditControls",
        /* strAddInfo   */ "" );

    CGraphObjLine* pGraphObjLine = nullptr;
    QGraphicsLineItem* pGraphicsLineItem = nullptr;

    if (m_pGraphObj != nullptr) {
        pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
        pGraphicsLineItem = dynamic_cast<QGraphicsLineItem*>(m_pGraphObj);
    }

    if (pGraphObjLine == nullptr || pGraphicsLineItem == nullptr)
    {
        // Geometry in Metric System
        m_pEdtMetricPt1X->setEnabled(false);
        m_pEdtMetricPt1X->setReadOnly(true);
        m_pEdtMetricPt1X->setValue(0.0);
        m_pEdtMetricPt1Y->setEnabled(false);
        m_pEdtMetricPt1Y->setReadOnly(true);
        m_pEdtMetricPt1Y->setValue(0.0);
        m_pEdtMetricPt2X->setEnabled(false);
        m_pEdtMetricPt2X->setReadOnly(true);
        m_pEdtMetricPt2X->setValue(0.0);
        m_pEdtMetricPt2Y->setEnabled(false);
        m_pEdtMetricPt2Y->setReadOnly(true);
        m_pEdtMetricPt2Y->setValue(0.0);
        m_pEdtMetricLength->setEnabled(false);
        m_pEdtMetricLength->setReadOnly(true);
        m_pEdtMetricLength->setValue(0.0);
        m_pEdtMetricAngle->setEnabled(false);
        m_pEdtMetricAngle->setReadOnly(true);
        m_pEdtMetricAngle->setValue(0.0);
        m_pEdtMetricWidth->setEnabled(false);
        m_pEdtMetricWidth->setReadOnly(true);
        m_pEdtMetricWidth->setValue(0.0);
        m_pEdtMetricHeight->setEnabled(false);
        m_pEdtMetricHeight->setReadOnly(true);
        m_pEdtMetricHeight->setValue(0.0);
        // Geometry in Pixels
        m_pEdtPixelsPt1X->setEnabled(false);
        m_pEdtPixelsPt1X->setReadOnly(true);
        m_pEdtPixelsPt1X->setValue(0.0);
        m_pEdtPixelsPt1Y->setEnabled(false);
        m_pEdtPixelsPt1Y->setReadOnly(true);
        m_pEdtPixelsPt1Y->setValue(0.0);
        m_pEdtPixelsPt2X->setEnabled(false);
        m_pEdtPixelsPt2X->setReadOnly(true);
        m_pEdtPixelsPt2X->setValue(0.0);
        m_pEdtPixelsPt2Y->setEnabled(false);
        m_pEdtPixelsPt2Y->setReadOnly(true);
        m_pEdtPixelsPt2Y->setValue(0.0);
        m_pEdtPixelsLength->setEnabled(false);
        m_pEdtPixelsLength->setReadOnly(true);
        m_pEdtPixelsLength->setValue(0.0);
        m_pEdtPixelsAngle->setEnabled(false);
        m_pEdtPixelsAngle->setReadOnly(true);
        m_pEdtPixelsAngle->setValue(0.0);
        m_pEdtPixelsPosX->setEnabled(false);
        m_pEdtPixelsPosX->setReadOnly(true);
        m_pEdtPixelsPosX->setValue(0.0);
        m_pEdtPixelsPosY->setEnabled(false);
        m_pEdtPixelsPosY->setReadOnly(true);
        m_pEdtPixelsPosY->setValue(0.0);
        m_pEdtPixelsWidth->setEnabled(false);
        m_pEdtPixelsWidth->setReadOnly(true);
        m_pEdtPixelsWidth->setValue(0.0);
        m_pEdtPixelsHeight->setEnabled(false);
        m_pEdtPixelsHeight->setReadOnly(true);
        m_pEdtPixelsHeight->setValue(0.0);
    }
    else
    {
        QLineF line = pGraphObjLine->line();

        // Geometry in Metric System
        m_pEdtMetricPt1X->setEnabled(true);
        m_pEdtMetricPt1X->setReadOnly(true);
        m_pEdtMetricPt1Y->setEnabled(true);
        m_pEdtMetricPt1Y->setReadOnly(true);
        m_pEdtMetricPt1X->setEnabled(true);
        m_pEdtMetricPt2X->setReadOnly(true);
        m_pEdtMetricPt2Y->setEnabled(true);
        m_pEdtMetricPt2Y->setReadOnly(true);
        m_pEdtMetricLength->setEnabled(true);
        m_pEdtMetricLength->setReadOnly(true);
        m_pEdtMetricAngle->setEnabled(true);
        m_pEdtMetricAngle->setReadOnly(true);
        m_pEdtMetricWidth->setEnabled(false);
        m_pEdtMetricWidth->setReadOnly(true);
        m_pEdtMetricHeight->setEnabled(false);
        m_pEdtMetricHeight->setReadOnly(true);

        // Geometry in Pixels
        m_pEdtPixelsPt1X->setEnabled(true);
        m_pEdtPixelsPt1X->setReadOnly(false);
        m_pEdtPixelsPt1X->setValue(line.p1().x());
        m_pEdtPixelsPt1Y->setEnabled(true);
        m_pEdtPixelsPt1Y->setReadOnly(false);
        m_pEdtPixelsPt1Y->setValue(line.p1().y());
        m_pEdtPixelsPt2X->setEnabled(true);
        m_pEdtPixelsPt2X->setReadOnly(false);
        m_pEdtPixelsPt2X->setValue(line.p2().x());
        m_pEdtPixelsPt2Y->setEnabled(true);
        m_pEdtPixelsPt2Y->setReadOnly(false);
        m_pEdtPixelsPt2Y->setValue(line.p2().y());
        m_pEdtPixelsLength->setEnabled(true);
        m_pEdtPixelsLength->setReadOnly(true);
        m_pEdtPixelsLength->setValue(ZS::System::Math::round2Nearest(line.length(), 0));
        m_pEdtPixelsAngle->setEnabled(true);
        m_pEdtPixelsAngle->setReadOnly(true);
        m_pEdtPixelsAngle->setValue(line.angle());
        QPointF ptPos = pGraphObjLine->getPos().toQPointF(Units.Length.px);
        m_pEdtPixelsPosX->setEnabled(true);
        m_pEdtPixelsPosX->setReadOnly(true);
        m_pEdtPixelsPosX->setValue(ptPos.x());
        m_pEdtPixelsPosY->setEnabled(true);
        m_pEdtPixelsPosY->setReadOnly(true);
        m_pEdtPixelsPosY->setValue(ptPos.y());
        QSizeF size = pGraphObjLine->getSize().toQSizeF(Units.Length.px);
        m_pEdtPixelsWidth->setEnabled(true);
        m_pEdtPixelsWidth->setReadOnly(false);
        m_pEdtPixelsWidth->setValue(ZS::System::Math::round2Nearest(size.width(), 0));
        m_pEdtPixelsHeight->setEnabled(true);
        m_pEdtPixelsHeight->setReadOnly(false);
        m_pEdtPixelsHeight->setValue(ZS::System::Math::round2Nearest(size.height(), 0));
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::applySettings()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "applySettings",
        /* strAddInfo   */ "" );

    if (m_pGraphObj != nullptr && !hasErrors())
    {
    }
}

/*==============================================================================
private: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::updateDrawingSize()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateDrawingSize",
        /* strAddInfo   */ "" );

    if (m_drawingSize.dimensionUnit() == EDrawingDimensionUnit::Metric)
    {
        m_pEdtMetricPt1X->setUnit(m_drawingSize.metricUnit());
        m_pEdtMetricPt1X->setResolution(m_drawingSize.metricImageWidth().getRes().getVal());
        m_pEdtMetricPt1X->setMaximum(m_drawingSize.metricImageWidth().getVal());

        m_pEdtMetricPt1Y->setUnit(m_drawingSize.metricUnit());
        m_pEdtMetricPt1Y->setResolution(m_drawingSize.metricImageHeight().getRes().getVal());
        m_pEdtMetricPt1Y->setMaximum(m_drawingSize.metricImageHeight().getVal());

        m_pEdtMetricPt2X->setUnit(m_drawingSize.metricUnit());
        m_pEdtMetricPt2X->setResolution(m_drawingSize.metricImageWidth().getRes().getVal());
        m_pEdtMetricPt2X->setMaximum(m_drawingSize.metricImageWidth().getVal());

        m_pEdtMetricPt2Y->setUnit(m_drawingSize.metricUnit());
        m_pEdtMetricPt2Y->setResolution(m_drawingSize.metricImageHeight().getRes().getVal());
        m_pEdtMetricPt2Y->setMaximum(m_drawingSize.metricImageHeight().getVal());

        m_pEdtMetricLength->setUnit(m_drawingSize.metricUnit());
        m_pEdtMetricLength->setResolution(m_drawingSize.metricImageWidth().getRes().getVal());
        double fWidthMetricImageDiagonale = ZS::System::Math::sqrt(
            ZS::System::Math::sqr(m_drawingSize.metricImageWidth().getVal())
            + ZS::System::Math::sqr(m_drawingSize.metricImageHeight().getVal()));
        m_pEdtMetricLength->setMaximum(fWidthMetricImageDiagonale);

        m_pWdgtMetric->show();
        m_pWdgtSepLinePixelsGeometry->show();

        m_pEdtMetricWidth->setUnit(m_drawingSize.metricUnit());
        m_pEdtMetricWidth->setResolution(m_drawingSize.metricImageWidth().getRes().getVal());
        m_pEdtMetricLength->setMaximum(m_drawingSize.metricImageWidth().getVal());

        m_pEdtMetricHeight->setUnit(m_drawingSize.metricUnit());
        m_pEdtMetricHeight->setResolution(m_drawingSize.metricImageHeight().getRes().getVal());
        m_pEdtMetricHeight->setMaximum(m_drawingSize.metricImageHeight().getVal());
    }
    else // if (m_drawingSize.dimensionUnit() == EDrawingDimensionUnit::Pixels)
    {
        m_pWdgtMetric->hide();
        m_pWdgtSepLinePixelsGeometry->hide();
    }

    m_pEdtPixelsPt1X->setMaximum(m_drawingSize.imageWidthInPixels());
    m_pEdtPixelsPt1Y->setMaximum(m_drawingSize.imageHeightInPixels());
    m_pEdtPixelsPt2X->setMaximum(m_drawingSize.imageWidthInPixels());
    m_pEdtPixelsPt2Y->setMaximum(m_drawingSize.imageHeightInPixels());
    double fWidthPixelsImageDiagonale = ZS::System::Math::sqrt(
        ZS::System::Math::sqr(m_drawingSize.imageWidthInPixels())
        + ZS::System::Math::sqr(m_drawingSize.imageHeightInPixels()));
    m_pEdtPixelsLength->setMaximum(fWidthPixelsImageDiagonale);
    m_pEdtPixelsPosX->setMaximum(m_drawingSize.imageWidthInPixels());
    m_pEdtPixelsPosY->setMaximum(m_drawingSize.imageHeightInPixels());
    m_pEdtPixelsWidth->setMaximum(m_drawingSize.imageWidthInPixels());
    m_pEdtPixelsHeight->setMaximum(m_drawingSize.imageHeightInPixels());

} // updateDrawingSize
