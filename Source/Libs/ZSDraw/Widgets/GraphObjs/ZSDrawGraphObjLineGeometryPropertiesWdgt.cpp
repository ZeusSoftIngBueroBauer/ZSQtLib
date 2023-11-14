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
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLineGeometryEditPropertyDlg.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjGeometryModel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLine.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSPhysValGUI/ZSPhysValEditItemDelegate.h"
#include "ZSPhysValGUI/ZSPhysValEditWdgt.h"
#include "ZSSysGUI/ZSSysCheckBoxItemDelegate.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qevent.h>

#if QT_VERSION < 0x050000
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qtableview.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtGraphObjLineGeometryProperties : public CWdgtGraphObjPropertiesAbstract
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

const QString CWdgtGraphObjLineGeometryProperties::c_strCoorPoint1 = "Point 1";
const QString CWdgtGraphObjLineGeometryProperties::c_strCoorPoint2 = "Point 2";
const QString CWdgtGraphObjLineGeometryProperties::c_strCoorCenter = "Center";
const QString CWdgtGraphObjLineGeometryProperties::c_strCoorWidth = "Width";
const QString CWdgtGraphObjLineGeometryProperties::c_strCoorHeight = "Height";
const QString CWdgtGraphObjLineGeometryProperties::c_strCoorLength = "Length";
const QString CWdgtGraphObjLineGeometryProperties::c_strCoorAngle = "Angle";
const QString CWdgtGraphObjLineGeometryProperties::c_strCoorX = "X";
const QString CWdgtGraphObjLineGeometryProperties::c_strCoorY = "Y";

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjLineGeometryProperties::CWdgtGraphObjLineGeometryProperties(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strNameSpace,
    const QString& i_strObjName,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtGraphObjPropertiesAbstract(
        i_pDrawingScene,
        i_strNameSpace, "StandardShapes::Line", ClassName(),
        i_strObjName, i_pWdgtParent),
    // Caching values
    m_physValLine(),
    // Edit Controls
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
    // Metric Table View
    m_pLytLineMetricTableViewButtons(nullptr),
    m_pBtnMetricResizeRowsAndColumnsToContents(nullptr),
    m_pLytMetricGeometryListView(nullptr),
    m_pTableViewMetricGeometry(nullptr),
    m_pModelMetricGeometry(nullptr),
    // Metric Edit Controls
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
    m_pLytLineMetricPtCenter(nullptr),
    m_pLblMetricPtCenter(nullptr),
    m_pLblMetricPtCenterX(nullptr),
    m_pEdtMetricPtCenterX(nullptr),
    m_pLblMetricPtCenterY(nullptr),
    m_pEdtMetricPtCenterY(nullptr),
    m_pLytLineMetricSize(nullptr),
    m_pLblMetricWidth(nullptr),
    m_pEdtMetricWidth(nullptr),
    m_pLblMetricHeight(nullptr),
    m_pEdtMetricHeight(nullptr),
    m_pLytLineMetricAngle(nullptr),
    m_pLblMetricLength(nullptr),
    m_pEdtMetricLength(nullptr),
    m_pLblMetricAngle(nullptr),
    m_pEdtMetricAngle(nullptr),
    // Geometry in Pixels
    m_pWdgtPixels(nullptr),
    m_pLytWdgtPixels(nullptr),
    m_pWdgtSepLinePixelsGeometry(nullptr),
    m_pLytSepLinePixelsGeometry(nullptr),
    m_pLblSepLinePixelsGeometry(nullptr),
    m_pSepLinePixelsGeometry(nullptr),
    // Pixels Table View
    m_pLytLinePixelsTableViewButtons(nullptr),
    m_pBtnPixelsResizeRowsAndColumnsToContents(nullptr),
    m_pLytPixelsGeometryListView(nullptr),
    m_pTableViewPixelsGeometry(nullptr),
    m_pModelPixelsGeometry(nullptr),
    // Pixels Edit Controls
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
    m_pLytLinePixelsPtCenter(nullptr),
    m_pLblPixelsPtCenter(nullptr),
    m_pLblPixelsPtCenterX(nullptr),
    m_pEdtPixelsPtCenterX(nullptr),
    m_pLblPixelsPtCenterY(nullptr),
    m_pEdtPixelsPtCenterY(nullptr),
    m_pLytLinePixelsSize(nullptr),
    m_pLblPixelsWidth(nullptr),
    m_pEdtPixelsWidth(nullptr),
    m_pLblPixelsHeight(nullptr),
    m_pEdtPixelsHeight(nullptr),
    m_pLytLinePixelsAngle(nullptr),
    m_pLblPixelsLength(nullptr),
    m_pEdtPixelsLength(nullptr),
    m_pLblPixelsAngle(nullptr),
    m_pEdtPixelsAngle(nullptr),
    // Edit dialog
    m_hshpRegisteredEditPropertyDialogs(),
    m_pDlgEditProperty(nullptr)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // We need to fill the edit controls if the graphical object emits the geometryChanged signal.
    // The flag is checked if "setKeyInTree" is called.
    m_bContentUpdateOnGeometryChanged = true;

    // <Widget> Headline
    //==================

    m_pWdgtHeadline = new QWidget();
    m_pLytWdgtHeadline = new QHBoxLayout();
    m_pLytWdgtHeadline->setContentsMargins(0, 0, 0, 0);
    m_pWdgtHeadline->setLayout(m_pLytWdgtHeadline);
    m_pLyt->addWidget(m_pWdgtHeadline);

    m_pBtnCollapse = new QPushButton();
    if (s_bWdgtGeometryVisible) {
        m_pBtnCollapse->setIcon(m_pxmBtnUp);
    }
    else {
        m_pBtnCollapse->setIcon(m_pxmBtnDown);
    }
    m_pLytWdgtHeadline->addWidget(m_pBtnCollapse);

    QObject::connect(
        m_pBtnCollapse, &QPushButton::clicked,
        this, &CWdgtGraphObjLineGeometryProperties::onBtnCollapseClicked);

    QPixmap pxmHeadline(":/ZS/Draw/DrawSettingsGeometry16x16.png");
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

    const QString strMetric = CEnumScaleDimensionUnit(EScaleDimensionUnit::Metric).toString();
    const QString strPixels = CEnumScaleDimensionUnit(EScaleDimensionUnit::Pixels).toString();

    // <Widget> Geometry
    //==================

    m_pWdgtGeometry = new QWidget();
    m_pLytWdgtGeometry = new QVBoxLayout();
    m_pLytWdgtGeometry->setContentsMargins(0, 0, 0, 0);
    m_pWdgtGeometry->setLayout(m_pLytWdgtGeometry);
    m_pLyt->addWidget(m_pWdgtGeometry);

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
    m_pLytSepLineMetricGeometry->setContentsMargins(0, 5, 0, 0);
    m_pWdgtSepLineMetricGeometry->setLayout(m_pLytSepLineMetricGeometry);
    m_pLblSepLineMetricGeometry = new QLabel("Metric Values");
    QFont font = m_pLblSepLineMetricGeometry->font();
    font.setBold(true);
    font.setUnderline(true);
    m_pLblSepLineMetricGeometry->setFont(font);
    m_pLytSepLineMetricGeometry->addWidget(m_pLblSepLineMetricGeometry);
    m_pLytWdgtMetric->addWidget(m_pWdgtSepLineMetricGeometry);

    // <Line> Table View Buttons
    //--------------------------

    //m_pLytLineMetricTableViewButtons = new QHBoxLayout();
    //m_pLytWdgtMetric->addLayout(m_pLytLineMetricTableViewButtons);

    QPixmap pxmResizeToContents(":/ZS/TreeView/TreeViewResizeToContents.png");
    m_pBtnMetricResizeRowsAndColumnsToContents = new QPushButton();
    m_pBtnMetricResizeRowsAndColumnsToContents->setIcon(pxmResizeToContents);
    m_pBtnMetricResizeRowsAndColumnsToContents->setFixedSize(QSize(24, 24));
    m_pBtnMetricResizeRowsAndColumnsToContents->setToolTip("Press to resize the rows and columns to their contents");
    m_pLytSepLineMetricGeometry->addSpacing(10);
    m_pLytSepLineMetricGeometry->addWidget(m_pBtnMetricResizeRowsAndColumnsToContents);
    m_pLytSepLineMetricGeometry->addStretch();
    QObject::connect(
        m_pBtnMetricResizeRowsAndColumnsToContents, &QPushButton::clicked,
        this, &CWdgtGraphObjLineGeometryProperties::onBtnMetricResizeRowsAndColumnsToContentsClicked );

    // <Line> Table View
    //------------------

    m_pLytMetricGeometryListView = new QVBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytMetricGeometryListView, 1);

    m_pModelMetricGeometry = new CModelGraphObjGeometry(
        m_pDrawingScene, i_strNameSpace, "StandardShapes::Line",
        i_strObjName, EScaleDimensionUnit::Metric, this);
    m_pTableViewMetricGeometry = new QTableView();
    m_pTableViewMetricGeometry->setModel(m_pModelMetricGeometry);
    m_pTableViewMetricGeometry->setItemDelegateForColumn(
        CModelGraphObjGeometry::EColumnXVal,
        new CEditPhysValtemDelegate(m_pModelMetricGeometry, m_pTableViewMetricGeometry));
    m_pTableViewMetricGeometry->setItemDelegateForColumn(
        CModelGraphObjGeometry::EColumnYVal,
        new CEditPhysValtemDelegate(m_pModelMetricGeometry, m_pTableViewMetricGeometry));
    m_pTableViewMetricGeometry->setItemDelegateForColumn(
        CModelGraphObjGeometry::EColumnShowVals,
        new CCheckBoxItemDelegate(m_pModelMetricGeometry, m_pTableViewMetricGeometry));
    m_pTableViewMetricGeometry->setItemDelegateForColumn(
        CModelGraphObjGeometry::EColumnShowLine,
        new CCheckBoxItemDelegate(m_pModelMetricGeometry, m_pTableViewMetricGeometry));
    m_pTableViewMetricGeometry->setEditTriggers(QAbstractItemView::AllEditTriggers);
    m_pTableViewMetricGeometry->resizeColumnsToContents();
    m_pTableViewMetricGeometry->resizeRowsToContents();
    m_pLytMetricGeometryListView->addWidget(m_pTableViewMetricGeometry);

    QObject::connect(
        m_pModelMetricGeometry, &CModelGraphObjGeometry::contentChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onModelMetricGeometryContentChanged);

    /* Grid Layout in m_ariClmWidths
         |     0    |1| 2 |3|  4  |5| 6      |7| 8    | 
       --+----------+-+---+-+-----+-+--------+-+------+----
       0 |Metric                                      |<-->
       1 |Point 1   | |X: | |Value| |Y:      | |Value |<-->
       2 |Point 2   | |X: | |Value| |Y:      | |Value |<-->
       3 |Center    | |X: | |Value| |Y:      | |Value |<-->
       4 |Width:    | |   | |Value| |Height: | |Value |<-->
       5 |Length:   | |   | |Value| |Angle:  | |Value |<-->
    */

    m_ariClmWidths.resize(9);
    m_ariClmWidths[0] = 40;
    m_ariClmWidths[1] = 5;
    m_ariClmWidths[2] = 10;
    m_ariClmWidths[3] = 5;
    m_ariClmWidths[4] = 90;
    m_ariClmWidths[5] = 5;
    m_ariClmWidths[6] = 40;
    m_ariClmWidths[7] = 5;
    m_ariClmWidths[8] = 90;

    // <Line> Metric Point 1
    //----------------------

    m_pLytLineMetricPt1 = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytLineMetricPt1);

    m_pLblMetricPt1 = new QLabel(c_strCoorPoint1);
    m_pLblMetricPt1->setFixedWidth(m_ariClmWidths[0]);
    m_pLytLineMetricPt1->addWidget(m_pLblMetricPt1);

    m_pLytLineMetricPt1->addSpacing(m_ariClmWidths[1]);

    m_pLblMetricPt1X = new QLabel(c_strCoorX + ": ");
    m_pLblMetricPt1X->setFixedWidth(m_ariClmWidths[2]);
    m_pLytLineMetricPt1->addWidget(m_pLblMetricPt1X);

    m_pLytLineMetricPt1->addSpacing(m_ariClmWidths[3]);

    m_pEdtMetricPt1X = new CWdgtEditPhysVal(strMetric + "." + c_strCoorPoint1 + "." + c_strCoorX);
    m_pEdtMetricPt1X->setReadOnly(true);
    m_pEdtMetricPt1X->setFixedWidth(m_ariClmWidths[4]);
    m_pEdtMetricPt1X->setMinimum(0.0);
    registerEditPropertyDialog(m_pEdtMetricPt1X);
    m_pLytLineMetricPt1->addWidget(m_pEdtMetricPt1X);
    QObject::connect(
        m_pEdtMetricPt1X, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricPt1XValueChanged);

    m_pLytLineMetricPt1->addSpacing(m_ariClmWidths[5]);

    m_pLblMetricPt1Y = new QLabel(c_strCoorY + ": ");
    m_pLblMetricPt1Y->setFixedWidth(m_ariClmWidths[6]);
    m_pLytLineMetricPt1->addWidget(m_pLblMetricPt1Y);

    m_pLytLineMetricPt1->addSpacing(m_ariClmWidths[7]);

    m_pEdtMetricPt1Y = new CWdgtEditPhysVal(strMetric + "." + c_strCoorPoint1 + "." + c_strCoorY);
    m_pEdtMetricPt1Y->setReadOnly(true);
    m_pEdtMetricPt1Y->setFixedWidth(m_ariClmWidths[8]);
    m_pEdtMetricPt1Y->setMinimum(0.0);
    registerEditPropertyDialog(m_pEdtMetricPt1Y);
    m_pLytLineMetricPt1->addWidget(m_pEdtMetricPt1Y);
    QObject::connect(
        m_pEdtMetricPt1Y, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricPt1YValueChanged);

    m_pLytLineMetricPt1->addStretch();

    // <Line> Metric Point 2
    //----------------------

    m_pLytLineMetricPt2 = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytLineMetricPt2);

    m_pLblMetricPt2 = new QLabel(c_strCoorPoint2);
    m_pLblMetricPt2->setFixedWidth(m_ariClmWidths[0]);
    m_pLytLineMetricPt2->addWidget(m_pLblMetricPt2);

    m_pLytLineMetricPt2->addSpacing(m_ariClmWidths[1]);

    m_pLblMetricPt2X = new QLabel(c_strCoorX + ": ");
    m_pLblMetricPt2X->setFixedWidth(m_ariClmWidths[2]);
    m_pLytLineMetricPt2->addWidget(m_pLblMetricPt2X);

    m_pLytLineMetricPt2->addSpacing(m_ariClmWidths[3]);

    m_pEdtMetricPt2X = new CWdgtEditPhysVal(strMetric + "." + c_strCoorPoint2 + "." + c_strCoorX);
    m_pEdtMetricPt2X->setReadOnly(true);
    m_pEdtMetricPt2X->setFixedWidth(m_ariClmWidths[4]);
    m_pEdtMetricPt2X->setMinimum(0.0);
    registerEditPropertyDialog(m_pEdtMetricPt2X);
    m_pLytLineMetricPt2->addWidget(m_pEdtMetricPt2X);
    QObject::connect(
        m_pEdtMetricPt2X, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricPt2XValueChanged);

    m_pLytLineMetricPt2->addSpacing(m_ariClmWidths[5]);

    m_pLblMetricPt2Y = new QLabel(c_strCoorY + ": ");
    m_pLblMetricPt2Y->setFixedWidth(m_ariClmWidths[6]);
    m_pLytLineMetricPt2->addWidget(m_pLblMetricPt2Y);

    m_pLytLineMetricPt2->addSpacing(m_ariClmWidths[7]);

    m_pEdtMetricPt2Y = new CWdgtEditPhysVal(strMetric + "." + c_strCoorPoint2 + "." + c_strCoorY);
    m_pEdtMetricPt2Y->setReadOnly(true);
    m_pEdtMetricPt2Y->setFixedWidth(m_ariClmWidths[8]);
    m_pEdtMetricPt2Y->setMinimum(0.0);
    registerEditPropertyDialog(m_pEdtMetricPt2Y);
    m_pLytLineMetricPt2->addWidget(m_pEdtMetricPt2Y);
    QObject::connect(
        m_pEdtMetricPt2Y, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricPt2YValueChanged);

    m_pLytLineMetricPt2->addStretch();

    // <Line> Center Position
    //-----------------------

    m_pLytLineMetricPtCenter = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytLineMetricPtCenter);

    m_pLblMetricPtCenter = new QLabel(c_strCoorCenter);
    m_pLblMetricPtCenter->setFixedWidth(m_ariClmWidths[0]);
    m_pLytLineMetricPtCenter->addWidget(m_pLblMetricPtCenter);

    m_pLytLineMetricPtCenter->addSpacing(m_ariClmWidths[1]);

    m_pLblMetricPtCenterX = new QLabel(c_strCoorX + ": ");
    m_pLblMetricPtCenterX->setFixedWidth(m_ariClmWidths[2]);
    m_pLytLineMetricPtCenter->addWidget(m_pLblMetricPtCenterX);

    m_pLytLineMetricPtCenter->addSpacing(m_ariClmWidths[3]);

    m_pEdtMetricPtCenterX = new CWdgtEditPhysVal(strMetric + "." + c_strCoorCenter + "." + c_strCoorX);
    m_pEdtMetricPtCenterX->setReadOnly(true);
    m_pEdtMetricPtCenterX->setFixedWidth(m_ariClmWidths[4]);
    m_pEdtMetricPtCenterX->setResolution(0.1);
    m_pEdtMetricPtCenterX->setMinimum(0.0);
    registerEditPropertyDialog(m_pEdtMetricPtCenterX);
    m_pLytLineMetricPtCenter->addWidget(m_pEdtMetricPtCenterX);
    QObject::connect(
        m_pEdtMetricPtCenterX, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricPtCenterXValueChanged);

    m_pLytLineMetricPtCenter->addSpacing(m_ariClmWidths[5]);

    m_pLblMetricPtCenterY = new QLabel(c_strCoorY + ": ");
    m_pLblMetricPtCenterY->setFixedWidth(m_ariClmWidths[6]);
    m_pLytLineMetricPtCenter->addWidget(m_pLblMetricPtCenterY);

    m_pLytLineMetricPtCenter->addSpacing(m_ariClmWidths[7]);

    m_pEdtMetricPtCenterY = new CWdgtEditPhysVal(strMetric + "." + c_strCoorCenter + "." + c_strCoorY);
    m_pEdtMetricPtCenterY->setReadOnly(true);
    m_pEdtMetricPtCenterY->setFixedWidth(m_ariClmWidths[8]);
    m_pEdtMetricPtCenterY->setResolution(0.1);
    m_pEdtMetricPtCenterY->setMinimum(0.0);
    registerEditPropertyDialog(m_pEdtMetricPtCenterY);
    m_pLytLineMetricPtCenter->addWidget(m_pEdtMetricPtCenterY);
    QObject::connect(
        m_pEdtMetricPtCenterY, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricPtCenterYValueChanged);

    m_pLytLineMetricPtCenter->addStretch();

    // <Line> Size
    //------------

    m_pLytLineMetricSize = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytLineMetricSize);

    m_pLblMetricWidth = new QLabel(c_strCoorWidth + ": ");
    m_pLblMetricWidth->setFixedWidth(
        m_ariClmWidths[0]
        + m_pLytLineMetricSize->contentsMargins().left()
        + m_pLytLineMetricSize->contentsMargins().right()
        + m_pLytLineMetricSize->spacing());
    m_pLytLineMetricSize->addWidget(m_pLblMetricWidth);

    m_pLytLineMetricSize->addSpacing(m_ariClmWidths[1]);
    m_pLytLineMetricSize->addSpacing(m_ariClmWidths[2]);
    m_pLytLineMetricSize->addSpacing(m_ariClmWidths[3]);

    m_pEdtMetricWidth = new CWdgtEditPhysVal(strMetric + "." + c_strCoorWidth);
    m_pEdtMetricWidth->setReadOnly(true);
    m_pEdtMetricWidth->setFixedWidth(m_ariClmWidths[4]);
    registerEditPropertyDialog(m_pEdtMetricWidth);
    m_pLytLineMetricSize->addWidget(m_pEdtMetricWidth);
    QObject::connect(
        m_pEdtMetricWidth, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricWidthValueChanged);

    m_pLytLineMetricSize->addSpacing(m_ariClmWidths[5]);

    m_pLblMetricHeight = new QLabel(c_strCoorHeight + ": ");
    m_pLblMetricHeight->setFixedWidth(m_ariClmWidths[6]);
    m_pLytLineMetricSize->addWidget(m_pLblMetricHeight);

    m_pLytLineMetricSize->addSpacing(m_ariClmWidths[7]);

    m_pEdtMetricHeight = new CWdgtEditPhysVal(strMetric + "." + c_strCoorHeight);
    m_pEdtMetricHeight->setReadOnly(true);
    m_pEdtMetricHeight->setFixedWidth(m_ariClmWidths[8]);
    registerEditPropertyDialog(m_pEdtMetricHeight);
    m_pLytLineMetricSize->addWidget(m_pEdtMetricHeight);
    QObject::connect(
        m_pEdtMetricHeight, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricHeightValueChanged);

    m_pLytLineMetricSize->addStretch();

    // <Line> Width and Angle
    //------------------------

    m_pLytLineMetricAngle = new QHBoxLayout();
    m_pLytWdgtMetric->addLayout(m_pLytLineMetricAngle);

    m_pLblMetricLength = new QLabel(c_strCoorLength + ": ");
    m_pLblMetricLength->setFixedWidth(
        m_ariClmWidths[0]
        + m_pLytLineMetricAngle->contentsMargins().left()
        + m_pLytLineMetricAngle->contentsMargins().right()
        + m_pLytLineMetricAngle->spacing());
    m_pLytLineMetricAngle->addWidget(m_pLblMetricLength);

    m_pLytLineMetricAngle->addSpacing(m_ariClmWidths[1]);
    m_pLytLineMetricAngle->addSpacing(m_ariClmWidths[2]);
    m_pLytLineMetricAngle->addSpacing(m_ariClmWidths[3]);

    m_pEdtMetricLength = new CWdgtEditPhysVal(strMetric + "." + c_strCoorLength);
    m_pEdtMetricLength->setReadOnly(true);
    m_pEdtMetricLength->setFixedWidth(m_ariClmWidths[4]);
    m_pEdtMetricLength->setMinimum(0.0);
    registerEditPropertyDialog(m_pEdtMetricLength);
    m_pLytLineMetricAngle->addWidget(m_pEdtMetricLength);
    QObject::connect(
        m_pEdtMetricLength, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricLengthValueChanged);

    m_pLytLineMetricAngle->addSpacing(m_ariClmWidths[5]);

    m_pLblMetricAngle = new QLabel(c_strCoorAngle + ": ");
    m_pLblMetricAngle->setFixedWidth(m_ariClmWidths[6]);
    m_pLytLineMetricAngle->addWidget(m_pLblMetricAngle);

    m_pLytLineMetricAngle->addSpacing(m_ariClmWidths[7]);

    m_pEdtMetricAngle = new CWdgtEditPhysVal(strMetric + "." + c_strCoorAngle);
    m_pEdtMetricAngle->setReadOnly(true);
    m_pEdtMetricAngle->setFixedWidth(m_ariClmWidths[8]);
    m_pEdtMetricAngle->setUnit(Units.Angle.Degree);
    m_pEdtMetricAngle->setResolution(0.1);
    m_pEdtMetricAngle->setMinimum(-360.0);
    m_pEdtMetricAngle->setMaximum(360.0);
    registerEditPropertyDialog(m_pEdtMetricAngle);
    m_pLytLineMetricAngle->addWidget(m_pEdtMetricAngle);
    QObject::connect(
        m_pEdtMetricAngle, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtMetricAngleValueChanged);

    m_pLytLineMetricAngle->addStretch();

    // <Section> Geometry in Pixels
    //=============================

    m_pWdgtPixels = new QWidget();
    m_pLytWdgtPixels = new QVBoxLayout();
    m_pLytWdgtPixels->setContentsMargins(0, 0, 0, 0);
    m_pWdgtPixels->setLayout(m_pLytWdgtPixels);
    m_pLytWdgtGeometry->addWidget(m_pWdgtPixels);
    m_pWdgtPixels->show();

    m_pWdgtSepLinePixelsGeometry = new QWidget();
    m_pLytSepLinePixelsGeometry = new QHBoxLayout();
    m_pLytSepLinePixelsGeometry->setContentsMargins(0, 5, 0, 0);
    m_pWdgtSepLinePixelsGeometry->setLayout(m_pLytSepLinePixelsGeometry);
    m_pLblSepLinePixelsGeometry = new QLabel("Pixel Values");
    font = m_pLblSepLinePixelsGeometry->font();
    font.setBold(true);
    font.setUnderline(true);
    m_pLblSepLinePixelsGeometry->setFont(font);
    m_pLytSepLinePixelsGeometry->addWidget(m_pLblSepLinePixelsGeometry);
    m_pLytWdgtPixels->addWidget(m_pWdgtSepLinePixelsGeometry);

    // <Line> Table View Buttons
    //--------------------------

    //m_pLytLinePixelsTableViewButtons = new QHBoxLayout();
    //m_pLytWdgtPixels->addLayout(m_pLytLinePixelsTableViewButtons);

    m_pBtnPixelsResizeRowsAndColumnsToContents = new QPushButton();
    m_pBtnPixelsResizeRowsAndColumnsToContents->setIcon(pxmResizeToContents);
    m_pBtnPixelsResizeRowsAndColumnsToContents->setFixedSize(QSize(24, 24));
    m_pBtnPixelsResizeRowsAndColumnsToContents->setToolTip("Press to resize the rows and columns to their contents");
    m_pLytSepLinePixelsGeometry->addSpacing(10);
    m_pLytSepLinePixelsGeometry->addWidget(m_pBtnPixelsResizeRowsAndColumnsToContents);
    m_pLytSepLinePixelsGeometry->addStretch();
    QObject::connect(
        m_pBtnPixelsResizeRowsAndColumnsToContents, &QPushButton::clicked,
        this, &CWdgtGraphObjLineGeometryProperties::onBtnPixelsResizeRowsAndColumnsToContentsClicked );

    // <Line> Table View
    //------------------

    m_pLytPixelsGeometryListView = new QVBoxLayout();
    m_pLytWdgtPixels->addLayout(m_pLytPixelsGeometryListView, 1);

    m_pModelPixelsGeometry = new CModelGraphObjGeometry(
        m_pDrawingScene, i_strNameSpace, "StandardShapes::Line",
        i_strObjName, EScaleDimensionUnit::Pixels, this);
    m_pTableViewPixelsGeometry = new QTableView();
    m_pTableViewPixelsGeometry->setModel(m_pModelPixelsGeometry);
    m_pTableViewPixelsGeometry->setItemDelegateForColumn(
        CModelGraphObjGeometry::EColumnXVal,
        new CEditPhysValtemDelegate(m_pModelPixelsGeometry, m_pTableViewPixelsGeometry));
    m_pTableViewPixelsGeometry->setItemDelegateForColumn(
        CModelGraphObjGeometry::EColumnYVal,
        new CEditPhysValtemDelegate(m_pModelPixelsGeometry, m_pTableViewPixelsGeometry));
    m_pTableViewPixelsGeometry->setItemDelegateForColumn(
        CModelGraphObjGeometry::EColumnShowVals,
        new CCheckBoxItemDelegate(m_pModelPixelsGeometry, m_pTableViewPixelsGeometry));
    m_pTableViewPixelsGeometry->setItemDelegateForColumn(
        CModelGraphObjGeometry::EColumnShowLine,
        new CCheckBoxItemDelegate(m_pModelPixelsGeometry, m_pTableViewPixelsGeometry));
    m_pTableViewPixelsGeometry->setEditTriggers(QAbstractItemView::AllEditTriggers);
    m_pTableViewPixelsGeometry->resizeColumnsToContents();
    m_pTableViewPixelsGeometry->resizeRowsToContents();
    m_pLytPixelsGeometryListView->addWidget(m_pTableViewPixelsGeometry);

    //QObject::connect(
    //    m_pModelPixelsGeometry, &CModelGraphObjGeometry::contentChanged,
    //    this, &CWdgtGraphObjLineGeometryProperties::onModelPixelsGeometryContentChanged);

    /* Grid Layout in m_ariClmWidths
         |     0    |1| 2 |3|  4  |5| 6      |7| 8    | 
       --+----------+-+---+-+-----+-+--------+-+------+----
       0 |Pixels                                      |<-->
       1 |Point 1   | |X: | |Value| |Y:      | |Value |<-->
       2 |Point 2   | |X: | |Value| |Y:      | |Value |<-->
       3 |Center    | |X: | |Value| |Y:      | |Value |<-->
       4 |Width:    | |   | |Value| |Height: | |Value |<-->
       5 |Length:   | |   | |Value| |Angle:  | |Value |<-->
    */

    // <Line> Pixels Point 1
    //----------------------

    m_pLytLinePixelsPt1 = new QHBoxLayout();
    m_pLytWdgtPixels->addLayout(m_pLytLinePixelsPt1);

    m_pLblPixelsPt1 = new QLabel(c_strCoorPoint1);
    m_pLblPixelsPt1->setFixedWidth(m_ariClmWidths[0]);
    m_pLytLinePixelsPt1->addWidget(m_pLblPixelsPt1);

    m_pLytLinePixelsPt1->addSpacing(m_ariClmWidths[1]);

    m_pLblPixelsPt1X = new QLabel(c_strCoorX + ": ");
    m_pLblPixelsPt1X->setFixedWidth(m_ariClmWidths[2]);
    m_pLytLinePixelsPt1->addWidget(m_pLblPixelsPt1X);

    m_pLytLinePixelsPt1->addSpacing(m_ariClmWidths[3]);

    m_pEdtPixelsPt1X = new CWdgtEditPhysVal(strPixels + "." + c_strCoorPoint1 + "." + c_strCoorX);
    m_pEdtPixelsPt1X->setReadOnly(true);
    m_pEdtPixelsPt1X->setFixedWidth(m_ariClmWidths[4]);
    m_pEdtPixelsPt1X->setUnit(Units.Length.px);
    m_pEdtPixelsPt1X->setResolution(1.0);
    m_pEdtPixelsPt1X->setMinimum(0.0);
    registerEditPropertyDialog(m_pEdtPixelsPt1X);
    m_pLytLinePixelsPt1->addWidget(m_pEdtPixelsPt1X);
    QObject::connect(
        m_pEdtPixelsPt1X, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsPt1XValueChanged);

    m_pLytLinePixelsPt1->addSpacing(m_ariClmWidths[5]);

    m_pLblPixelsPt1Y = new QLabel(c_strCoorY + ": ");
    m_pLblPixelsPt1Y->setFixedWidth(m_ariClmWidths[6]);
    m_pLytLinePixelsPt1->addWidget(m_pLblPixelsPt1Y);

    m_pLytLinePixelsPt1->addSpacing(m_ariClmWidths[7]);

    m_pEdtPixelsPt1Y = new CWdgtEditPhysVal(strPixels + "." + c_strCoorPoint1 + "." + c_strCoorY);
    m_pEdtPixelsPt1Y->setReadOnly(true);
    m_pEdtPixelsPt1Y->setFixedWidth(m_ariClmWidths[8]);
    m_pEdtPixelsPt1Y->setUnit(Units.Length.px);
    m_pEdtPixelsPt1Y->setResolution(1.0);
    m_pEdtPixelsPt1Y->setMinimum(0.0);
    registerEditPropertyDialog(m_pEdtPixelsPt1Y);
    m_pLytLinePixelsPt1->addWidget(m_pEdtPixelsPt1Y);
    QObject::connect(
        m_pEdtPixelsPt1Y, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsPt1YValueChanged);

    m_pLytLinePixelsPt1->addStretch();

    // <Line> Pixels Point 2
    //----------------------

    m_pLytLinePixelsPt2 = new QHBoxLayout();
    m_pLytWdgtPixels->addLayout(m_pLytLinePixelsPt2);

    m_pLblPixelsPt2 = new QLabel(c_strCoorPoint2);
    m_pLblPixelsPt2->setFixedWidth(m_ariClmWidths[0]);
    m_pLytLinePixelsPt2->addWidget(m_pLblPixelsPt2);

    m_pLytLinePixelsPt2->addSpacing(m_ariClmWidths[1]);

    m_pLblPixelsPt2X = new QLabel(c_strCoorX + ": ");
    m_pLblPixelsPt2X->setFixedWidth(m_ariClmWidths[2]);
    m_pLytLinePixelsPt2->addWidget(m_pLblPixelsPt2X);

    m_pLytLinePixelsPt2->addSpacing(m_ariClmWidths[3]);

    m_pEdtPixelsPt2X = new CWdgtEditPhysVal(strPixels + "." + c_strCoorPoint2 + "." + c_strCoorX);
    m_pEdtPixelsPt2X->setReadOnly(true);
    m_pEdtPixelsPt2X->setFixedWidth(m_ariClmWidths[4]);
    m_pEdtPixelsPt2X->setUnit(Units.Length.px);
    m_pEdtPixelsPt2X->setResolution(1.0);
    m_pEdtPixelsPt2X->setMinimum(0.0);
    registerEditPropertyDialog(m_pEdtPixelsPt2X);
    m_pLytLinePixelsPt2->addWidget(m_pEdtPixelsPt2X);
    QObject::connect(
        m_pEdtPixelsPt2X, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsPt2XValueChanged);

    m_pLytLinePixelsPt2->addSpacing(m_ariClmWidths[5]);

    m_pLblPixelsPt2Y = new QLabel(c_strCoorY + ": ");
    m_pLblPixelsPt2Y->setFixedWidth(m_ariClmWidths[6]);
    m_pLytLinePixelsPt2->addWidget(m_pLblPixelsPt2Y);

    m_pLytLinePixelsPt2->addSpacing(m_ariClmWidths[7]);

    m_pEdtPixelsPt2Y = new CWdgtEditPhysVal(strPixels + "." + c_strCoorPoint2 + "." + c_strCoorY);
    m_pEdtPixelsPt2Y->setReadOnly(true);
    m_pEdtPixelsPt2Y->setFixedWidth(m_ariClmWidths[8]);
    m_pEdtPixelsPt2Y->setUnit(Units.Length.px);
    m_pEdtPixelsPt2Y->setResolution(1.0);
    m_pEdtPixelsPt2Y->setMinimum(0.0);
    registerEditPropertyDialog(m_pEdtPixelsPt2Y);
    m_pLytLinePixelsPt2->addWidget(m_pEdtPixelsPt2Y);
    QObject::connect(
        m_pEdtPixelsPt2Y, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsPt2YValueChanged);

    m_pLytLinePixelsPt2->addStretch();

    // <Line> Center Position
    //-----------------------

    m_pLytLinePixelsPtCenter = new QHBoxLayout();
    m_pLytWdgtPixels->addLayout(m_pLytLinePixelsPtCenter);

    m_pLblPixelsPtCenter = new QLabel(c_strCoorCenter);
    m_pLblPixelsPtCenter->setFixedWidth(m_ariClmWidths[0]);
    m_pLytLinePixelsPtCenter->addWidget(m_pLblPixelsPtCenter);

    m_pLytLinePixelsPtCenter->addSpacing(m_ariClmWidths[1]);

    m_pLblPixelsPtCenterX = new QLabel(c_strCoorX + ": ");
    m_pLblPixelsPtCenterX->setFixedWidth(m_ariClmWidths[2]);
    m_pLytLinePixelsPtCenter->addWidget(m_pLblPixelsPtCenterX);

    m_pLytLinePixelsPtCenter->addSpacing(m_ariClmWidths[3]);

    m_pEdtPixelsPtCenterX = new CWdgtEditPhysVal(strPixels + "." + c_strCoorCenter + "." + c_strCoorX);
    m_pEdtPixelsPtCenterX->setReadOnly(true);
    m_pEdtPixelsPtCenterX->setFixedWidth(m_ariClmWidths[4]);
    m_pEdtPixelsPtCenterX->setUnit(Units.Length.px);
    m_pEdtPixelsPtCenterX->setResolution(0.1);
    m_pEdtPixelsPtCenterX->setMinimum(0.0);
    m_pLytLinePixelsPtCenter->addWidget(m_pEdtPixelsPtCenterX);
    QObject::connect(
        m_pEdtPixelsPtCenterX, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsPtCenterXValueChanged);

    m_pLytLinePixelsPtCenter->addSpacing(m_ariClmWidths[5]);

    m_pLblPixelsPtCenterY = new QLabel(c_strCoorY + ": ");
    m_pLblPixelsPtCenterY->setFixedWidth(m_ariClmWidths[6]);
    m_pLytLinePixelsPtCenter->addWidget(m_pLblPixelsPtCenterY);

    m_pLytLinePixelsPtCenter->addSpacing(m_ariClmWidths[7]);

    m_pEdtPixelsPtCenterY = new CWdgtEditPhysVal(strPixels + "." + c_strCoorCenter + "." + c_strCoorY);
    m_pEdtPixelsPtCenterX->setReadOnly(true);
    m_pEdtPixelsPtCenterY->setFixedWidth(m_ariClmWidths[8]);
    m_pEdtPixelsPtCenterY->setUnit(Units.Length.px);
    m_pEdtPixelsPtCenterY->setResolution(0.1);
    m_pEdtPixelsPtCenterY->setMinimum(0.0);
    m_pLytLinePixelsPtCenter->addWidget(m_pEdtPixelsPtCenterY);
    QObject::connect(
        m_pEdtPixelsPtCenterY, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsPtCenterYValueChanged);

    m_pLytLinePixelsPtCenter->addStretch();

    // <Line> Size
    //------------

    m_pLytLinePixelsSize = new QHBoxLayout();
    m_pLytWdgtPixels->addLayout(m_pLytLinePixelsSize);

    m_pLblPixelsWidth = new QLabel(c_strCoorWidth + ": ");
    m_pLblPixelsWidth->setFixedWidth(
        m_ariClmWidths[0]
        + m_pLytLinePixelsSize->contentsMargins().left()
        + m_pLytLinePixelsSize->contentsMargins().right()
        + m_pLytLinePixelsSize->spacing());
    m_pLytLinePixelsSize->addWidget(m_pLblPixelsWidth);

    m_pLytLinePixelsSize->addSpacing(m_ariClmWidths[1]);
    m_pLytLinePixelsSize->addSpacing(m_ariClmWidths[2]);
    m_pLytLinePixelsSize->addSpacing(m_ariClmWidths[3]);

    m_pEdtPixelsWidth = new CWdgtEditPhysVal(strPixels + "." + c_strCoorWidth);
    m_pEdtPixelsWidth->setReadOnly(true);
    m_pEdtPixelsWidth->setFixedWidth(m_ariClmWidths[4]);
    m_pEdtPixelsWidth->setUnit(Units.Length.px);
    m_pEdtPixelsWidth->setResolution(1.0);
    registerEditPropertyDialog(m_pEdtPixelsWidth);
    m_pLytLinePixelsSize->addWidget(m_pEdtPixelsWidth);
    QObject::connect(
        m_pEdtPixelsWidth, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsWidthValueChanged);

    m_pLytLinePixelsSize->addSpacing(m_ariClmWidths[5]);

    m_pLblPixelsHeight = new QLabel(c_strCoorHeight + ": ");
    m_pLblPixelsHeight->setFixedWidth(m_ariClmWidths[6]);
    m_pLytLinePixelsSize->addWidget(m_pLblPixelsHeight);

    m_pLytLinePixelsSize->addSpacing(m_ariClmWidths[7]);

    m_pEdtPixelsHeight = new CWdgtEditPhysVal(strPixels + "." + c_strCoorHeight);
    m_pEdtPixelsHeight->setReadOnly(true);
    m_pEdtPixelsHeight->setFixedWidth(m_ariClmWidths[8]);
    m_pEdtPixelsHeight->setUnit(Units.Length.px);
    m_pEdtPixelsHeight->setResolution(1.0);
    registerEditPropertyDialog(m_pEdtPixelsHeight);
    m_pLytLinePixelsSize->addWidget(m_pEdtPixelsHeight);
    QObject::connect(
        m_pEdtPixelsHeight, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsHeightValueChanged);

    m_pLytLinePixelsSize->addStretch();

    // <Line> Length and Angle
    //------------------------

    m_pLytLinePixelsAngle = new QHBoxLayout();
    m_pLytWdgtPixels->addLayout(m_pLytLinePixelsAngle);

    m_pLblPixelsLength = new QLabel(c_strCoorLength + ": ");
    m_pLblPixelsLength->setFixedWidth(
        m_ariClmWidths[0]
        + m_pLytLinePixelsAngle->contentsMargins().left()
        + m_pLytLinePixelsAngle->contentsMargins().right()
        + m_pLytLinePixelsAngle->spacing());
    m_pLytLinePixelsAngle->addWidget(m_pLblPixelsLength);

    m_pLytLinePixelsAngle->addSpacing(m_ariClmWidths[1]);
    m_pLytLinePixelsAngle->addSpacing(m_ariClmWidths[2]);
    m_pLytLinePixelsAngle->addSpacing(m_ariClmWidths[3]);

    m_pEdtPixelsLength = new CWdgtEditPhysVal(strPixels + "." + c_strCoorLength);
    m_pEdtPixelsLength->setReadOnly(true);
    m_pEdtPixelsLength->setFixedWidth(m_ariClmWidths[4]);
    m_pEdtPixelsLength->setUnit(Units.Length.px);
    m_pEdtPixelsLength->setResolution(0.1);
    m_pEdtPixelsLength->setMinimum(0.0);
    m_pLytLinePixelsAngle->addWidget(m_pEdtPixelsLength);
    QObject::connect(
        m_pEdtPixelsLength, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsLengthValueChanged);

    m_pLytLinePixelsAngle->addSpacing(m_ariClmWidths[5]);

    m_pLblPixelsAngle = new QLabel(c_strCoorAngle + ": ");
    m_pLblPixelsAngle->setFixedWidth(m_ariClmWidths[6]);
    m_pLytLinePixelsAngle->addWidget(m_pLblPixelsAngle);

    m_pLytLinePixelsAngle->addSpacing(m_ariClmWidths[7]);

    m_pEdtPixelsAngle = new CWdgtEditPhysVal(strPixels + "." + c_strCoorAngle);
    m_pEdtPixelsAngle->setReadOnly(true);
    m_pEdtPixelsAngle->setFixedWidth(m_ariClmWidths[8]);
    m_pEdtPixelsAngle->setUnit(Units.Angle.Degree);
    m_pEdtPixelsAngle->setResolution(0.1);
    m_pEdtPixelsAngle->setMinimum(-360.0);
    m_pEdtPixelsAngle->setMaximum(360.0);
    m_pLytLinePixelsAngle->addWidget(m_pEdtPixelsAngle);
    QObject::connect(
        m_pEdtPixelsAngle, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onEdtPixelsAngleValueChanged);

    m_pLytLinePixelsAngle->addStretch();

    // Update controls depending on drawing size (dimension unit etc.)
    //----------------------------------------------------------------

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    onDrawingSceneDrawingSizeChanged(drawingSize);

    // Restore visibility
    //-------------------

    if (!s_bWdgtGeometryVisible) {
        m_pWdgtGeometry->hide();
    }

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

    if (m_pDlgEditProperty != nullptr) {
        m_pDlgEditProperty->close();
        try {
            delete m_pDlgEditProperty;
        }
        catch (...) {
        }
    }

    //m_physValLine;
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
    // Metric Table View
    m_pLytLineMetricTableViewButtons = nullptr;
    m_pBtnMetricResizeRowsAndColumnsToContents = nullptr;
    m_pLytMetricGeometryListView = nullptr;
    m_pTableViewMetricGeometry = nullptr;
    m_pModelMetricGeometry = nullptr;
    // Metric Edit Controls
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
    m_pLytLineMetricPtCenter = nullptr;
    m_pLblMetricPtCenter = nullptr;
    m_pLblMetricPtCenterX = nullptr;
    m_pEdtMetricPtCenterX = nullptr;
    m_pLblMetricPtCenterY = nullptr;
    m_pEdtMetricPtCenterY = nullptr;
    m_pLytLineMetricSize = nullptr;
    m_pLblMetricWidth = nullptr;
    m_pEdtMetricWidth = nullptr;
    m_pLblMetricHeight = nullptr;
    m_pEdtMetricHeight = nullptr;
    m_pLytLineMetricAngle = nullptr;
    m_pLblMetricLength = nullptr;
    m_pEdtMetricLength = nullptr;
    m_pLblMetricAngle = nullptr;
    m_pEdtMetricAngle = nullptr;
    // Geometry in Pixels
    m_pWdgtPixels = nullptr;
    m_pLytWdgtPixels = nullptr;
    m_pWdgtSepLinePixelsGeometry = nullptr;
    m_pLytSepLinePixelsGeometry = nullptr;
    m_pLblSepLinePixelsGeometry = nullptr;
    m_pSepLinePixelsGeometry = nullptr;
    // Pixels Table View
    m_pLytLinePixelsTableViewButtons = nullptr;
    m_pBtnPixelsResizeRowsAndColumnsToContents = nullptr;
    m_pLytPixelsGeometryListView = nullptr;
    m_pTableViewPixelsGeometry = nullptr;
    m_pModelPixelsGeometry =nullptr;
    // Pixels Edit Controls
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
    m_pLytLinePixelsPtCenter = nullptr;
    m_pLblPixelsPtCenter = nullptr;
    m_pLblPixelsPtCenterX = nullptr;
    m_pEdtPixelsPtCenterX = nullptr;
    m_pLblPixelsPtCenterY = nullptr;
    m_pEdtPixelsPtCenterY = nullptr;
    m_pLytLinePixelsSize = nullptr;
    m_pLblPixelsWidth = nullptr;
    m_pEdtPixelsWidth = nullptr;
    m_pLblPixelsHeight = nullptr;
    m_pEdtPixelsHeight = nullptr;
    m_pLytLinePixelsAngle = nullptr;
    m_pLblPixelsLength = nullptr;
    m_pEdtPixelsLength = nullptr;
    m_pLblPixelsAngle = nullptr;
    m_pEdtPixelsAngle = nullptr;
    m_hshpRegisteredEditPropertyDialogs.clear();
    m_pDlgEditProperty = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::expand(bool i_bExpand)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bExpand);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "expand",
        /* strAddInfo   */ strMthInArgs );

    if (i_bExpand && m_pWdgtGeometry->isHidden()) {
        m_pBtnCollapse->setIcon(m_pxmBtnUp);
        m_pWdgtGeometry->show();
        s_bWdgtGeometryVisible = true;
    }
    else if (!i_bExpand && !m_pWdgtGeometry->isHidden()) {
        m_pBtnCollapse->setIcon(m_pxmBtnDown);
        m_pWdgtGeometry->hide();
        s_bWdgtGeometryVisible = false;
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn("Expanded: " + bool2Str(s_bWdgtFillStyleVisible));
    }
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtGraphObjLineGeometryProperties::setKeyInTree(const QString& i_strKeyInTree)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setKeyInTree",
        /* strAddInfo   */ strMthInArgs );
    bool bObjectChanged = false;
    if (m_strKeyInTree != i_strKeyInTree) {
        bObjectChanged = true;
        CWdgtGraphObjPropertiesAbstract::setKeyInTree(i_strKeyInTree);
        if (m_pModelMetricGeometry != nullptr) {
            m_pModelMetricGeometry->setKeyInTree(i_strKeyInTree);
            m_pTableViewMetricGeometry->resizeColumnsToContents();
            m_pTableViewMetricGeometry->resizeRowsToContents();
        }
        if (m_pModelPixelsGeometry != nullptr) {
            m_pModelPixelsGeometry->setKeyInTree(i_strKeyInTree);
            m_pTableViewPixelsGeometry->resizeColumnsToContents();
            m_pTableViewPixelsGeometry->resizeRowsToContents();
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bObjectChanged);
    }
    return bObjectChanged;
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtGraphObjLineGeometryProperties::hasErrors() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasErrors",
        /* strAddInfo   */ "" );
    bool bHasErrors = false;
    if (!bHasErrors && m_pModelMetricGeometry != nullptr) {
        bHasErrors = m_pModelMetricGeometry->hasErrors();
    }
    if (!bHasErrors && m_pModelPixelsGeometry != nullptr) {
        bHasErrors = m_pModelPixelsGeometry->hasErrors();
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasErrors);
    }
    return bHasErrors;
}

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

#pragma message(__TODO__"line geometry completely handled in model")
    CGraphObjLine* pGraphObjLine = nullptr;
    if (m_pGraphObj != nullptr) {
        pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
    }
    if (pGraphObjLine != nullptr) {
        const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
        CPhysValLine physValLine = pGraphObjLine->getLine(drawingSize.unit());
        bHasChanges = (m_physValLine != physValLine);
    }
#pragma message(__TODO__"line geometry completely handled in model")
    if (!bHasChanges && m_pModelMetricGeometry != nullptr) {
        bHasChanges = m_pModelMetricGeometry->hasChanges();
    }
    if (!bHasChanges && m_pModelPixelsGeometry != nullptr) {
        bHasChanges = m_pModelPixelsGeometry->hasChanges();
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );

    if (!hasErrors())
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            // Apply the settings from the edit controls at the graphical object.
            applySettings();

            if (m_pModelMetricGeometry != nullptr) {
                m_pModelMetricGeometry->acceptChanges();
            }
            if (m_pModelPixelsGeometry != nullptr) {
                m_pModelPixelsGeometry->acceptChanges();
            }
        }

        // If the "contentChanged" signal is no longer blocked and the content of
        // properties widget has been changed ...
        if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
            // .. emit the contentChanged signal and update the enabled state
            // of the Apply and Reset buttons.
            emit_contentChanged();
        }
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::rejectChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rejectChanges",
        /* strAddInfo   */ "" );

    // "fillEditControls" is used to fill the edit controls with the current settings
    // of the graphical object. To avoid that the signal "contentChanged" is emitted
    // for each property of the graphical object set at the edit controls the
    // ContentChangedSignalBlockedCounter is incremented. After fillEditControls has
    // been executed the contentChanged flag is checked and the contentChanged signal
    // is emitted if necessary.

    {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

        // Fill the content of the edit controls.
        fillEditControls();

        if (m_pModelMetricGeometry != nullptr) {
            m_pModelMetricGeometry->rejectChanges();
        }
        if (m_pModelPixelsGeometry != nullptr) {
            m_pModelPixelsGeometry->rejectChanges();
        }
    }

    // If the "contentChanged" signal is no longer blocked and the content of
    // properties widget has been changed ...
    if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
        // .. emit the contentChanged signal and update the enabled state
        // of the Apply and Reset buttons.
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::applySettings(bool i_bImmediatelyApplySettings)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ImmediatelyApply: " + bool2Str(i_bImmediatelyApplySettings);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "applySettings",
        /* strAddInfo   */ strMthInArgs );

#pragma message(__TODO__"line geometry completely handled in model")
    //CGraphObjLine* pGraphObjLine = nullptr;
    //if (m_pGraphObj != nullptr) {
    //    pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
    //}
    //if (pGraphObjLine != nullptr && !hasErrors() && hasChanges()) {
    //    pGraphObjLine->setLine(m_physValLine);
    //}
#pragma message(__TODO__"line geometry completely handled in model")
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

#pragma message(__TODO__"line geometry completely handled in model")
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();

    CGraphObjLine* pGraphObjLine = nullptr;
    if (m_pGraphObj != nullptr) {
        pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
    }

    if (pGraphObjLine == nullptr) {
        m_physValLine = CPhysValLine(drawingSize.unit());
    }
    else {
        m_physValLine = pGraphObjLine->getLine(drawingSize.unit());
    }
    fillEditControls(m_physValLine);
#pragma message(__TODO__"line geometry completely handled in model")
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

    { CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

        if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric)
        {
            m_pEdtMetricPt1X->setUnit(i_drawingSize.metricUnit());
            m_pEdtMetricPt1X->setResolution(i_drawingSize.metricImageWidth().getRes().getVal());
            m_pEdtMetricPt1X->setMaximum(i_drawingSize.metricImageWidth().getVal());

            m_pEdtMetricPt1Y->setUnit(i_drawingSize.metricUnit());
            m_pEdtMetricPt1Y->setResolution(i_drawingSize.metricImageHeight().getRes().getVal());
            m_pEdtMetricPt1Y->setMaximum(i_drawingSize.metricImageHeight().getVal());

            m_pEdtMetricPt2X->setUnit(i_drawingSize.metricUnit());
            m_pEdtMetricPt2X->setResolution(i_drawingSize.metricImageWidth().getRes().getVal());
            m_pEdtMetricPt2X->setMaximum(i_drawingSize.metricImageWidth().getVal());

            m_pEdtMetricPt2Y->setUnit(i_drawingSize.metricUnit());
            m_pEdtMetricPt2Y->setResolution(i_drawingSize.metricImageHeight().getRes().getVal());
            m_pEdtMetricPt2Y->setMaximum(i_drawingSize.metricImageHeight().getVal());

            m_pEdtMetricPtCenterX->setUnit(i_drawingSize.metricUnit());
            m_pEdtMetricPtCenterX->setResolution(i_drawingSize.metricImageWidth().getRes().getVal());
            m_pEdtMetricPtCenterX->setMaximum(i_drawingSize.metricImageWidth().getVal());

            m_pEdtMetricPtCenterY->setUnit(i_drawingSize.metricUnit());
            m_pEdtMetricPtCenterY->setResolution(i_drawingSize.metricImageHeight().getRes().getVal());
            m_pEdtMetricPtCenterY->setMaximum(i_drawingSize.metricImageHeight().getVal());

            m_pEdtMetricWidth->setUnit(i_drawingSize.metricUnit());
            m_pEdtMetricWidth->setResolution(i_drawingSize.metricImageWidth().getRes().getVal());
            m_pEdtMetricWidth->setMinimum(-i_drawingSize.metricImageWidth().getVal());
            m_pEdtMetricWidth->setMaximum(i_drawingSize.metricImageWidth().getVal());

            m_pEdtMetricHeight->setUnit(i_drawingSize.metricUnit());
            m_pEdtMetricHeight->setResolution(i_drawingSize.metricImageHeight().getRes().getVal());
            m_pEdtMetricHeight->setMinimum(-i_drawingSize.metricImageHeight().getVal());
            m_pEdtMetricHeight->setMaximum(i_drawingSize.metricImageHeight().getVal());

            m_pEdtMetricLength->setUnit(i_drawingSize.metricUnit());
            m_pEdtMetricLength->setResolution(i_drawingSize.metricImageWidth().getRes().getVal());
            double fWidthMetricImageDiagonale = ZS::System::Math::sqrt(
                ZS::System::Math::sqr(i_drawingSize.metricImageWidth().getVal())
                + ZS::System::Math::sqr(i_drawingSize.metricImageHeight().getVal()));
            m_pEdtMetricLength->setMaximum(fWidthMetricImageDiagonale);

            m_pWdgtMetric->show();
            //m_pWdgtSepLinePixelsGeometry->show();

            //m_pEdtPixelsPt1X->setReadOnly(true);
            //m_pEdtPixelsPt1Y->setReadOnly(true);
            //m_pEdtPixelsPt2X->setReadOnly(true);
            //m_pEdtPixelsPt2Y->setReadOnly(true);
            //m_pEdtPixelsWidth->setReadOnly(true);
            //m_pEdtPixelsHeight->setReadOnly(true);
        }
        else // if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels)
        {
            //m_pEdtPixelsPt1X->setReadOnly(false);
            //m_pEdtPixelsPt1Y->setReadOnly(false);
            //m_pEdtPixelsPt2X->setReadOnly(false);
            //m_pEdtPixelsPt2Y->setReadOnly(false);
            //m_pEdtPixelsWidth->setReadOnly(false);
            //m_pEdtPixelsHeight->setReadOnly(false);

            m_pWdgtMetric->hide();
            //m_pWdgtSepLinePixelsGeometry->hide();
        }

        m_pEdtPixelsPt1X->setMaximum(i_drawingSize.imageWidthInPixels());
        m_pEdtPixelsPt1Y->setMaximum(i_drawingSize.imageHeightInPixels());
        m_pEdtPixelsPt2X->setMaximum(i_drawingSize.imageWidthInPixels());
        m_pEdtPixelsPt2Y->setMaximum(i_drawingSize.imageHeightInPixels());
        m_pEdtPixelsPtCenterX->setMaximum(i_drawingSize.imageWidthInPixels());
        m_pEdtPixelsPtCenterY->setMaximum(i_drawingSize.imageHeightInPixels());
        m_pEdtPixelsWidth->setMinimum(-i_drawingSize.imageWidthInPixels());
        m_pEdtPixelsWidth->setMaximum(i_drawingSize.imageWidthInPixels());
        m_pEdtPixelsHeight->setMinimum(-i_drawingSize.imageHeightInPixels());
        m_pEdtPixelsHeight->setMaximum(i_drawingSize.imageHeightInPixels());
        double fWidthPixelsImageDiagonale = ZS::System::Math::sqrt(
            ZS::System::Math::sqr(i_drawingSize.imageWidthInPixels())
            + ZS::System::Math::sqr(i_drawingSize.imageHeightInPixels()));
        m_pEdtPixelsLength->setMaximum(fWidthPixelsImageDiagonale);
    }

    fillEditControls();
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
        expand(true);
    }
    else {
        expand(false);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onBtnMetricResizeRowsAndColumnsToContentsClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnMetricResizeRowsAndColumnsToContentsClicked",
        /* strAddInfo   */ "" );

    m_pTableViewMetricGeometry->resizeColumnsToContents();
    m_pTableViewMetricGeometry->resizeRowsToContents();
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onBtnPixelsResizeRowsAndColumnsToContentsClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnPixelsResizeRowsAndColumnsToContentsClicked",
        /* strAddInfo   */ "" );

    m_pTableViewPixelsGeometry->resizeColumnsToContents();
    m_pTableViewPixelsGeometry->resizeRowsToContents();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onModelMetricGeometryContentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onModelMetricGeometryContentChanged",
        /* strAddInfo   */ "" );

    emit_contentChanged();
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onModelPixelsGeometryContentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onModelPixelsGeometryContentChanged",
        /* strAddInfo   */ "" );

    emit_contentChanged();
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricPt1XValueChanged(const CPhysVal& i_physVal)
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
        CPhysValPoint physValP1 = m_physValLine.p1();
        physValP1.setX(i_physVal);
        m_physValLine.setP1(physValP1);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricPt1YValueChanged(const CPhysVal& i_physVal)
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
        CPhysValPoint physValP1 = m_physValLine.p1();
        physValP1.setY(i_physVal);
        m_physValLine.setP1(physValP1);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricPt2XValueChanged(const CPhysVal& i_physVal)
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
        CPhysValPoint physValP2 = m_physValLine.p2();
        physValP2.setX(i_physVal);
        m_physValLine.setP2(physValP2);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricPt2YValueChanged(const CPhysVal& i_physVal)
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
        CPhysValPoint physValP2 = m_physValLine.p2();
        physValP2.setY(i_physVal);
        m_physValLine.setP2(physValP2);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricPtCenterXValueChanged(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtMetricPtCenterXValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        CPhysValPoint physValPtCenter = m_physValLine.center();
        physValPtCenter.setX(i_physVal);
        m_physValLine.setCenter(physValPtCenter);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricPtCenterYValueChanged(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtMetricPtCenterYValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        CPhysValPoint physValPtCenter = m_physValLine.center();
        physValPtCenter.setY(i_physVal);
        m_physValLine.setCenter(physValPtCenter);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricWidthValueChanged(const CPhysVal& i_physVal)
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
        m_physValLine.setWidth(i_physVal);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricHeightValueChanged(const CPhysVal& i_physVal)
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
        m_physValLine.setHeight(i_physVal);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricLengthValueChanged(const CPhysVal& i_physVal)
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
        m_physValLine.setLength(i_physVal);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtMetricAngleValueChanged(const CPhysVal& i_physVal)
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
        m_physValLine.setAngle(i_physVal);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsPt1XValueChanged(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
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
        CPhysValPoint physValP1 = m_physValLine.p1();
        physValP1.setX(i_physVal);
        m_physValLine.setP1(physValP1);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsPt1YValueChanged(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
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
        CPhysValPoint physValP1 = m_physValLine.p1();
        physValP1.setY(i_physVal);
        m_physValLine.setP1(physValP1);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsPt2XValueChanged(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
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
        CPhysValPoint physValP2 = m_physValLine.p2();
        physValP2.setX(i_physVal);
        m_physValLine.setP2(physValP2);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsPt2YValueChanged(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
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
        CPhysValPoint physValP2 = m_physValLine.p2();
        physValP2.setY(i_physVal);
        m_physValLine.setP2(physValP2);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsPtCenterXValueChanged(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtPixelsPtCenterXValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        CPhysValPoint physValPtCenter = m_physValLine.center();
        physValPtCenter.setX(i_physVal);
        m_physValLine.setCenter(physValPtCenter);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsPtCenterYValueChanged(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtPixelsPtCenterYValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        CPhysValPoint physValPtCenter = m_physValLine.center();
        physValPtCenter.setY(i_physVal);
        m_physValLine.setCenter(physValPtCenter);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsWidthValueChanged(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
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
        m_physValLine.setWidth(i_physVal);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsHeightValueChanged(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
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
        m_physValLine.setHeight(i_physVal);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsLengthValueChanged(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
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
        m_physValLine.setLength(i_physVal);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onEdtPixelsAngleValueChanged(const CPhysVal& i_physVal)
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
        m_physValLine.setAngle(i_physVal);
        fillEditControls(m_physValLine);
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::onDlgEditPropertyDestroyed(QObject* i_pObj)
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
private: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLineGeometryProperties::fillEditControls(const CPhysValLine& i_physValLine)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValLine.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "fillEditControls",
        /* strAddInfo   */ strMthInArgs );

    CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();

    if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric)
    {
        // Geometry in Metric System
        m_pEdtMetricPt1X->setValue(i_physValLine.p1().x().getVal());
        m_pEdtMetricPt1Y->setValue(i_physValLine.p1().y().getVal());
        m_pEdtMetricPt2X->setValue(i_physValLine.p2().x().getVal());
        m_pEdtMetricPt2Y->setValue(i_physValLine.p2().y().getVal());
        m_pEdtMetricPtCenterX->setValue(i_physValLine.center().x().getVal());
        m_pEdtMetricPtCenterY->setValue(i_physValLine.center().y().getVal());
        m_pEdtMetricWidth->setValue(i_physValLine.width().getVal());
        m_pEdtMetricHeight->setValue(i_physValLine.height().getVal());
        m_pEdtMetricLength->setValue(i_physValLine.length().getVal());
        m_pEdtMetricAngle->setValue(i_physValLine.angle().getVal());

        // No simple unit conversion is possible here. The Y Scale Axis may
        // be oriented from top to bottom or bottom to top.
        // To get the correct scene coordinates we must let the drawing scene
        // convert the coordinates into pixel values.
        CPhysValLine physValLinePx = m_pDrawingScene->convert(i_physValLine, Units.Length.px);
        m_pEdtPixelsPt1X->setValue(physValLinePx.p1().x().getVal());
        m_pEdtPixelsPt1Y->setValue(physValLinePx.p1().y().getVal());
        m_pEdtPixelsPt2X->setValue(physValLinePx.p2().x().getVal());
        m_pEdtPixelsPt2Y->setValue(physValLinePx.p2().y().getVal());
        m_pEdtPixelsPtCenterX->setValue(physValLinePx.center().x().getVal());
        m_pEdtPixelsPtCenterY->setValue(physValLinePx.center().y().getVal());
        m_pEdtPixelsWidth->setValue(physValLinePx.width().getVal());
        m_pEdtPixelsHeight->setValue(physValLinePx.height().getVal());
        m_pEdtPixelsLength->setValue(physValLinePx.length().getVal());
        m_pEdtPixelsAngle->setValue(physValLinePx.angle().getVal());
    }
    else /*if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels)*/ {
        // Geometry in Pixels
        m_pEdtPixelsPt1X->setValue(i_physValLine.p1().x().getVal());
        m_pEdtPixelsPt1Y->setValue(i_physValLine.p1().y().getVal());
        m_pEdtPixelsPt2X->setValue(i_physValLine.p2().x().getVal());
        m_pEdtPixelsPt2Y->setValue(i_physValLine.p2().y().getVal());
        m_pEdtPixelsPtCenterX->setValue(i_physValLine.center().x().getVal());
        m_pEdtPixelsPtCenterY->setValue(i_physValLine.center().y().getVal());
        m_pEdtPixelsWidth->setValue(i_physValLine.width().getVal());
        m_pEdtPixelsHeight->setValue(i_physValLine.height().getVal());
        m_pEdtPixelsLength->setValue(i_physValLine.length().getVal());
        m_pEdtPixelsAngle->setValue(i_physValLine.angle().getVal());
    }
} // fillEditControls

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
void CWdgtGraphObjLineGeometryProperties::registerEditPropertyDialog(CWdgtEditPhysVal* i_pEdtWidget)
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
bool CWdgtGraphObjLineGeometryProperties::eventFilter(QObject* i_pObjWatched, QEvent* i_pEv)
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
                    m_pDlgEditProperty = new CDlgGraphObjLineGeometryEditProperty(m_pDrawingScene, this);
                    m_pDlgEditProperty->setAttribute(Qt::WA_DeleteOnClose, true);
                    QObject::connect(
                        m_pDlgEditProperty, &QDialog::destroyed,
                        this, &CWdgtGraphObjLineGeometryProperties::onDlgEditPropertyDestroyed);
                }
                CGraphObjLine* pGraphObjLine = nullptr;
                if (m_pGraphObj != nullptr) {
                    pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
                }
                m_pDlgEditProperty->setCoordinate(pGraphObjLine, pEdtWidget->objectName());
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
