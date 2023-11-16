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
//#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLineGeometryEditPropertyDlg.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjGeometryModel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLine.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSPhysValGUI/ZSPhysValEditItemDelegate.h"
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
#include <QtGui/qheaderview.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
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

#pragma message(__TODO__ "To be removed")
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
    m_pModelPixelsGeometry(nullptr)
    // Edit dialog
    //m_hshpRegisteredEditPropertyDialogs(),
    //m_pDlgEditProperty(nullptr)
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
    m_pEdtPhysValDelegateMetricXVal =
        new CEditPhysValtemDelegate(m_pModelMetricGeometry, m_pTableViewMetricGeometry);
    m_pTableViewMetricGeometry->setItemDelegateForColumn(
        CModelGraphObjGeometry::EColumnXVal, m_pEdtPhysValDelegateMetricXVal);
    m_pEdtPhysValDelegateMetricYVal =
        new CEditPhysValtemDelegate(m_pModelMetricGeometry, m_pTableViewMetricGeometry);
    m_pTableViewMetricGeometry->setItemDelegateForColumn(
        CModelGraphObjGeometry::EColumnYVal, m_pEdtPhysValDelegateMetricYVal);
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
    m_pEdtPhysValDelegatePixelsXVal =
        new CEditPhysValtemDelegate(m_pModelPixelsGeometry, m_pTableViewPixelsGeometry);
    m_pTableViewPixelsGeometry->setItemDelegateForColumn(
        CModelGraphObjGeometry::EColumnXVal, m_pEdtPhysValDelegatePixelsXVal);
    m_pEdtPhysValDelegatePixelsYVal =
        new CEditPhysValtemDelegate(m_pModelPixelsGeometry, m_pTableViewPixelsGeometry);
    m_pTableViewPixelsGeometry->setItemDelegateForColumn(
        CModelGraphObjGeometry::EColumnYVal, m_pEdtPhysValDelegatePixelsYVal);
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

    QObject::connect(
        m_pModelPixelsGeometry, &CModelGraphObjGeometry::contentChanged,
        this, &CWdgtGraphObjLineGeometryProperties::onModelPixelsGeometryContentChanged);

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

    //if (m_pDlgEditProperty != nullptr) {
    //    m_pDlgEditProperty->close();
    //    try {
    //        delete m_pDlgEditProperty;
    //    }
    //    catch (...) {
    //    }
    //}

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

    {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

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
            //m_pEdtMetricPt1X->setUnit(i_drawingSize.metricUnit());
            //m_pEdtMetricPt1X->setResolution(i_drawingSize.metricImageWidth().getRes().getVal());
            //m_pEdtMetricPt1X->setMaximum(i_drawingSize.metricImageWidth().getVal());

            //m_pEdtMetricWidth->setUnit(i_drawingSize.metricUnit());
            //m_pEdtMetricWidth->setResolution(i_drawingSize.metricImageWidth().getRes().getVal());
            //m_pEdtMetricWidth->setMinimum(-i_drawingSize.metricImageWidth().getVal());
            //m_pEdtMetricWidth->setMaximum(i_drawingSize.metricImageWidth().getVal());

            //m_pEdtMetricLength->setUnit(i_drawingSize.metricUnit());
            //m_pEdtMetricLength->setResolution(i_drawingSize.metricImageWidth().getRes().getVal());
            //double fWidthMetricImageDiagonale = ZS::System::Math::sqrt(
            //    ZS::System::Math::sqr(i_drawingSize.metricImageWidth().getVal())
            //    + ZS::System::Math::sqr(i_drawingSize.metricImageHeight().getVal()));
            //m_pEdtMetricLength->setMaximum(fWidthMetricImageDiagonale);

            m_pWdgtMetric->show();
        }
        else // if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels)
        {
            m_pWdgtMetric->hide();
        }

        //m_pEdtPixelsPtCenterX->setMaximum(i_drawingSize.imageWidthInPixels());
        //m_pEdtPixelsPtCenterY->setMaximum(i_drawingSize.imageHeightInPixels());
        //m_pEdtPixelsWidth->setMinimum(-i_drawingSize.imageWidthInPixels());
        //m_pEdtPixelsWidth->setMaximum(i_drawingSize.imageWidthInPixels());
        //double fWidthPixelsImageDiagonale = ZS::System::Math::sqrt(
        //    ZS::System::Math::sqr(i_drawingSize.imageWidthInPixels())
        //    + ZS::System::Math::sqr(i_drawingSize.imageHeightInPixels()));
        //m_pEdtPixelsLength->setMaximum(fWidthPixelsImageDiagonale);
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

////------------------------------------------------------------------------------
//void CWdgtGraphObjLineGeometryProperties::onDlgEditPropertyDestroyed(QObject* i_pObj)
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "onDlgEditPropertyDestroyed",
//        /* strAddInfo   */ "" );
//
//    m_pDlgEditProperty = nullptr;
//}

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
//void CWdgtGraphObjLineGeometryProperties::registerEditPropertyDialog(CWdgtEditPhysVal* i_pEdtWidget)
//------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_pEdtWidget->objectName();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "registerEditPropertyDialog",
//        /* strAddInfo   */ strMthInArgs );
//
//    QString strObjName = i_pEdtWidget->objectName();
//    m_hshpRegisteredEditPropertyDialogs.insert(strObjName, i_pEdtWidget);
//    // To filter mouse events the event filter must also be set at the spin box
//    // and the line edit of the spin box. So we must call the installEventFilter
//    // method of class CWdgtEditPhysVal which not just installs the event filter
//    // on the EditPhysVal widget but also forwards the method to both the
//    // spin box and the line edit.
//    i_pEdtWidget->installEventFilter(this);
//}

/*==============================================================================
protected: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Opens the edit property dialog on pressing Ctrl + MouseDblClick.
*/
//bool CWdgtGraphObjLineGeometryProperties::eventFilter(QObject* i_pObjWatched, QEvent* i_pEv)
//------------------------------------------------------------------------------
//{
//    bool bHandled = false;
//    if (i_pEv->type() == QEvent::MouseButtonDblClick) {
//        QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(i_pEv);
//        if (pMouseEvent->modifiers() & Qt::ControlModifier) {
//            QString strObjName = i_pObjWatched->objectName();
//            QWidget* pEdtWidget = m_hshpRegisteredEditPropertyDialogs.value(strObjName, nullptr);
//            if (pEdtWidget != nullptr) {
//                if (m_pDlgEditProperty == nullptr) {
//                    m_pDlgEditProperty = new CDlgGraphObjLineGeometryEditProperty(m_pDrawingScene, this);
//                    m_pDlgEditProperty->setAttribute(Qt::WA_DeleteOnClose, true);
//                    QObject::connect(
//                        m_pDlgEditProperty, &QDialog::destroyed,
//                        this, &CWdgtGraphObjLineGeometryProperties::onDlgEditPropertyDestroyed);
//                }
//                CGraphObjLine* pGraphObjLine = nullptr;
//                if (m_pGraphObj != nullptr) {
//                    pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
//                }
//                m_pDlgEditProperty->setCoordinate(pGraphObjLine, pEdtWidget->objectName());
//                m_pDlgEditProperty->show();
//                bHandled = true;
//            }
//        }
//    }
//    if (!bHandled) {
//        bHandled = CWdgtGraphObjPropertiesAbstract::eventFilter(i_pObjWatched, i_pEv);
//    }
//    return bHandled;
//}
